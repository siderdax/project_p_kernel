/*
 * exynos7420_thermal.c - Samsung EXYNOS TMU (Thermal Management Unit)
 *
 *  Copyright (C) 2014 Samsung Electronics
 *  Donggeun Kim <dg77.kim@samsung.com>
 *  Amit Daniel Kachhap <amit.kachhap@linaro.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <linux/module.h>
#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/ipa.h>
#include <linux/workqueue.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/io.h>
#include <linux/mutex.h>
#include <linux/platform_data/exynos_thermal.h>
#include <linux/thermal.h>
#include <linux/cpufreq.h>
#include <linux/cpu_cooling.h>
#include <linux/of.h>
#include <linux/delay.h>
#include <linux/suspend.h>
#include <linux/exynos-ss.h>
#include <linux/pm_qos.h>
#include <plat/cpu.h>
#include <mach/tmu.h>
#include <mach/cpufreq.h>
#include <mach/asv-exynos.h>
#include <mach/exynos-pm.h>
#include "cal_tmu7420.h"

#ifdef CONFIG_ARM_EXYNOS_MP_CPUFREQ
static struct cpumask mp_cluster_cpus[CL_END];
#endif

#define CLUST0_POLICY_CORE		((exynos_boot_cluster == CL_ZERO) ? 0 : 4)
#define CLUST1_POLICY_CORE 	((exynos_boot_cluster == CL_ONE) ? 0 : 4)
#define CS_POLICY_CORE		0

#ifdef CONFIG_EXYNOS_SWTRIP
#define SWTRIP_TEMP				110
#define SWTRIP_NOISE_COUNT		1

static unsigned int swtrip_counter = 0;
#endif

static bool is_tmu_probed;

static enum tmu_noti_state_t tmu_old_state = TMU_NORMAL;
static enum gpu_noti_state_t gpu_old_state = GPU_NORMAL;
static enum mif_noti_state_t mif_old_state = MIF_TH_LV1;
static enum isp_noti_state_t isp_old_state = ISP_NORMAL;
static bool is_suspending;
static bool is_cpu_hotplugged_out;

static BLOCKING_NOTIFIER_HEAD(exynos_tmu_notifier);
static BLOCKING_NOTIFIER_HEAD(exynos_gpu_notifier);
static BLOCKING_NOTIFIER_HEAD(exynos_isp_notifier);

struct exynos_tmu_data {
	struct exynos_tmu_platform_data *pdata;
	struct resource *mem[EXYNOS_TMU_COUNT];
	void __iomem *base[EXYNOS_TMU_COUNT];
	int irq[EXYNOS_TMU_COUNT];
	enum soc_type soc;
	struct work_struct irq_work;
	struct mutex lock;
	struct cal_tmu_data *cal_data;
	bool lpa;
};

struct	thermal_trip_point_conf {
	int trip_val[MAX_TRIP_COUNT];
	int trip_count;
	u8 trigger_falling;
};

struct	thermal_cooling_conf {
	struct freq_clip_table freq_data[MAX_TRIP_COUNT];
	int size[THERMAL_TRIP_CRITICAL + 1];
	int freq_clip_count;
};

struct thermal_sensor_conf {
	char name[SENSOR_NAME_LEN];
	int (*read_temperature)(void *data);
	int (*write_emul_temp)(void *drv_data, unsigned long temp);
	struct thermal_trip_point_conf trip_data;
	struct thermal_cooling_conf cooling_data;
	void *private_data;
};

struct exynos_thermal_zone {
	enum thermal_device_mode mode;
	struct thermal_zone_device *therm_dev;
	struct thermal_cooling_device *cool_dev[MAX_COOLING_DEVICE];
	unsigned int cool_dev_size;
	struct platform_device *exynos4_dev;
	struct thermal_sensor_conf *sensor_conf;
	bool bind;
};

static struct exynos_thermal_zone *th_zone;
static struct platform_device *exynos_tmu_pdev;
static struct exynos_tmu_data *tmudata;
static void exynos_unregister_thermal(void);
static int exynos_register_thermal(struct thermal_sensor_conf *sensor_conf);
static int exynos_tmu_cpufreq_notifier(struct notifier_block *notifier, unsigned long event, void *v);

static struct notifier_block exynos_cpufreq_nb = {
	.notifier_call = exynos_tmu_cpufreq_notifier,
};

#ifdef CONFIG_ARM_EXYNOS_MP_CPUFREQ
static void __init init_mp_cpumask_set(void)
{
	 unsigned int i;

	 for_each_cpu(i, cpu_possible_mask) {
		 if (exynos_boot_cluster == CL_ZERO) {
			 if (i >= NR_CLUST0_CPUS)
				 cpumask_set_cpu(i, &mp_cluster_cpus[CL_ONE]);
			 else
				 cpumask_set_cpu(i, &mp_cluster_cpus[CL_ZERO]);
		 } else {
			 if (i >= NR_CLUST1_CPUS)
				 cpumask_set_cpu(i, &mp_cluster_cpus[CL_ZERO]);
			 else
				 cpumask_set_cpu(i, &mp_cluster_cpus[CL_ONE]);
		 }
	 }
}
#endif

/* Get mode callback functions for thermal zone */
static int exynos_get_mode(struct thermal_zone_device *thermal,
			enum thermal_device_mode *mode)
{
	if (th_zone)
		*mode = th_zone->mode;
	return 0;
}

/* Set mode callback functions for thermal zone */
static int exynos_set_mode(struct thermal_zone_device *thermal,
			enum thermal_device_mode mode)
{
	if (!th_zone->therm_dev) {
		pr_notice("thermal zone not registered\n");
		return 0;
	}

	th_zone->mode = mode;
	thermal_zone_device_update(th_zone->therm_dev);
	return 0;
}


/* Get trip type callback functions for thermal zone */
static int exynos_get_trip_type(struct thermal_zone_device *thermal, int trip,
				 enum thermal_trip_type *type)
{
	int active_size, passive_size;

	active_size = th_zone->sensor_conf->cooling_data.size[THERMAL_TRIP_ACTIVE];
	passive_size = th_zone->sensor_conf->cooling_data.size[THERMAL_TRIP_PASSIVE];

	if (trip < active_size)
		*type = THERMAL_TRIP_ACTIVE;
	else if (trip >= active_size && trip < active_size + passive_size)
		*type = THERMAL_TRIP_PASSIVE;
	else if (trip >= active_size + passive_size)
		*type = THERMAL_TRIP_CRITICAL;
	else
		return -EINVAL;

	return 0;
}

/* Get trip temperature callback functions for thermal zone */
static int exynos_get_trip_temp(struct thermal_zone_device *thermal, int trip,
				unsigned long *temp)
{
	int active_size, passive_size;

	active_size = th_zone->sensor_conf->cooling_data.size[THERMAL_TRIP_ACTIVE];
	passive_size = th_zone->sensor_conf->cooling_data.size[THERMAL_TRIP_PASSIVE];

	if (trip < 0 || trip > active_size + passive_size)
		return -EINVAL;

	*temp = th_zone->sensor_conf->trip_data.trip_val[trip];
	/* convert the temperature into millicelsius */
	*temp = *temp * MCELSIUS;

	return 0;
}

/* Get critical temperature callback functions for thermal zone */
static int exynos_get_crit_temp(struct thermal_zone_device *thermal,
				unsigned long *temp)
{
	int ret;
	int active_size, passive_size;

	active_size = th_zone->sensor_conf->cooling_data.size[THERMAL_TRIP_ACTIVE];
	passive_size = th_zone->sensor_conf->cooling_data.size[THERMAL_TRIP_PASSIVE];

	/* Panic zone */
	ret = exynos_get_trip_temp(thermal, active_size + passive_size, temp);
	return ret;
}

/* Bind callback functions for thermal zone */
static int exynos_bind(struct thermal_zone_device *thermal,
			struct thermal_cooling_device *cdev)
{
	int ret = 0, i, tab_size;
	unsigned long level = THERMAL_CSTATE_INVALID;
#ifdef CONFIG_ARM_EXYNOS_MP_CPUFREQ
	int cluster_idx = 0;
	struct cpufreq_policy policy;
#endif
	struct freq_clip_table *tab_ptr, *clip_data;
	struct thermal_sensor_conf *data = th_zone->sensor_conf;
	enum thermal_trip_type type = 0;

	tab_ptr = (struct freq_clip_table *)data->cooling_data.freq_data;
	tab_size = data->cooling_data.freq_clip_count;

	if (tab_ptr == NULL || tab_size == 0)
		return -EINVAL;

	/* find the cooling device registered*/
	for (i = 0; i < th_zone->cool_dev_size; i++)
		if (cdev == th_zone->cool_dev[i]) {
#ifdef CONFIG_ARM_EXYNOS_MP_CPUFREQ
			cluster_idx = i;
#endif
			break;
		}

	/* No matching cooling device */
	if (i == th_zone->cool_dev_size)
		return 0;

	/* Bind the thermal zone to the cpufreq cooling device */
	for (i = 0; i < tab_size; i++) {
		clip_data = (struct freq_clip_table *)&(tab_ptr[i]);
#ifdef CONFIG_ARM_EXYNOS_MP_CPUFREQ
		if (cluster_idx == CL_ZERO) {
			cpufreq_get_policy(&policy, CLUST0_POLICY_CORE);

			if (clip_data->freq_clip_max_cluster0 > policy.max) {
				pr_warn("%s: throttling freq(%d) is greater than policy max(%d)\n", __func__, clip_data->freq_clip_max_cluster0, policy.max);
				clip_data->freq_clip_max_cluster0 = policy.max;
			} else if (clip_data->freq_clip_max_cluster0 < policy.min) {
				pr_warn("%s: throttling freq(%d) is less than policy min(%d)\n", __func__, clip_data->freq_clip_max_cluster0, policy.min);
				clip_data->freq_clip_max_cluster0 = policy.min;
			}

			level = cpufreq_cooling_get_level(CLUST0_POLICY_CORE, clip_data->freq_clip_max_cluster0);
		} else if (cluster_idx == CL_ONE) {
			cpufreq_get_policy(&policy, CLUST1_POLICY_CORE);

			if (clip_data->freq_clip_max > policy.max) {
				pr_warn("%s: throttling freq(%d) is greater than policy max(%d)\n", __func__, clip_data->freq_clip_max, policy.max);
				clip_data->freq_clip_max = policy.max;
			} else if (clip_data->freq_clip_max < policy.min) {
				pr_warn("%s: throttling freq(%d) is less than policy min(%d)\n", __func__, clip_data->freq_clip_max, policy.min);
				clip_data->freq_clip_max = policy.min;
			}

			level = cpufreq_cooling_get_level(CLUST1_POLICY_CORE, clip_data->freq_clip_max);
		}
#else
		level = cpufreq_cooling_get_level(CS_POLICY_CORE, clip_data->freq_clip_max);
#endif
		if (level == THERMAL_CSTATE_INVALID) {
			thermal->cooling_dev_en = false;
			return 0;
		}
		exynos_get_trip_type(th_zone->therm_dev, i, &type);
		switch (type) {
		case THERMAL_TRIP_ACTIVE:
		case THERMAL_TRIP_PASSIVE:
			if (thermal_zone_bind_cooling_device(thermal, i, cdev,
								level, 0)) {
				pr_err("error binding cdev inst %d\n", i);
				thermal->cooling_dev_en = false;
				ret = -EINVAL;
			}
			th_zone->bind = true;
			break;
		default:
			ret = -EINVAL;
		}
	}

	return ret;
}

/* Unbind callback functions for thermal zone */
static int exynos_unbind(struct thermal_zone_device *thermal,
			struct thermal_cooling_device *cdev)
{
	int ret = 0, i, tab_size;
	struct thermal_sensor_conf *data = th_zone->sensor_conf;
	enum thermal_trip_type type = 0;

	if (th_zone->bind == false)
		return 0;

	tab_size = data->cooling_data.freq_clip_count;

	if (tab_size == 0)
		return -EINVAL;

	/* find the cooling device registered*/
	for (i = 0; i < th_zone->cool_dev_size; i++)
		if (cdev == th_zone->cool_dev[i])
			break;

	/* No matching cooling device */
	if (i == th_zone->cool_dev_size)
		return 0;

	/* Bind the thermal zone to the cpufreq cooling device */
	for (i = 0; i < tab_size; i++) {
		exynos_get_trip_type(th_zone->therm_dev, i, &type);
		switch (type) {
		case THERMAL_TRIP_ACTIVE:
		case THERMAL_TRIP_PASSIVE:
			if (thermal_zone_unbind_cooling_device(thermal, i,
								cdev)) {
				pr_err("error unbinding cdev inst=%d\n", i);
				ret = -EINVAL;
			}
			th_zone->bind = false;
			break;
		default:
			ret = -EINVAL;
		}
	}
	return ret;
}


int exynos_tmu_add_notifier(struct notifier_block *n)
{
	int ret;
	ret = blocking_notifier_chain_register(&exynos_tmu_notifier, n);
	blocking_notifier_call_chain(&exynos_tmu_notifier, mif_old_state, NULL);
	return ret;
}

void exynos_tmu_call_notifier(enum tmu_noti_state_t cur_state, int temp)
{
	if (is_suspending)
		cur_state = TMU_COLD;

	if (cur_state != tmu_old_state) {
		if ((cur_state == TMU_COLD) ||
			((cur_state == TMU_NORMAL) && (tmu_old_state == TMU_COLD)))
			blocking_notifier_call_chain(&exynos_tmu_notifier, TMU_COLD, &cur_state);
		else
			blocking_notifier_call_chain(&exynos_tmu_notifier, cur_state, &tmu_old_state);
		if (cur_state == TMU_COLD)
			pr_info("tmu temperature state %d to %d\n", tmu_old_state, cur_state);
		else
			pr_info("tmu temperature state %d to %d, cur_temp : %d\n", tmu_old_state, cur_state, temp);
		tmu_old_state = cur_state;
	}
}

int exynos_gpu_add_notifier(struct notifier_block *n)
{
	return blocking_notifier_chain_register(&exynos_gpu_notifier, n);
}

void exynos_gpu_call_notifier(enum gpu_noti_state_t cur_state)
{
	if (is_suspending)
		cur_state = GPU_COLD;

	if (cur_state != gpu_old_state) {
		pr_info("gpu temperature state %d to %d\n", gpu_old_state, cur_state);
		blocking_notifier_call_chain(&exynos_gpu_notifier, cur_state, &cur_state);
		gpu_old_state = cur_state;
	}
}

static void exynos_check_tmu_noti_state(int temp)
{
	enum tmu_noti_state_t cur_state;

	/* check current temperature state */
	if (temp > HOT_CRITICAL_TEMP)
		cur_state = TMU_CRITICAL;
	else if (temp > HOT_NORMAL_TEMP && temp <= HOT_CRITICAL_TEMP)
		cur_state = TMU_HOT;
	else if (temp > COLD_TEMP && temp <= HOT_NORMAL_TEMP)
		cur_state = TMU_NORMAL;
	else
		cur_state = TMU_COLD;

	exynos_tmu_call_notifier(cur_state, temp);
}

static void exynos_check_mif_noti_state(int temp)
{
	enum mif_noti_state_t cur_state;

	/* check current temperature state */
	if (temp < MIF_TH_TEMP1)
		cur_state = MIF_TH_LV1;
	else if (temp >= MIF_TH_TEMP1)
		cur_state = MIF_TH_LV2;

	if (cur_state != mif_old_state) {
		blocking_notifier_call_chain(&exynos_tmu_notifier, cur_state, &mif_old_state);
		mif_old_state = cur_state;
	}
}

static void exynos_check_gpu_noti_state(int temp)
{
	enum gpu_noti_state_t cur_state;

	/* check current temperature state */
	if (temp >= GPU_TH_TEMP5)
		cur_state = GPU_TRIPPING;
	else if (temp >= GPU_TH_TEMP4 && temp < GPU_TH_TEMP5)
		cur_state = GPU_THROTTLING4;
	else if (temp >= GPU_TH_TEMP3 && temp < GPU_TH_TEMP4)
		cur_state = GPU_THROTTLING3;
	else if (temp >= GPU_TH_TEMP2 && temp < GPU_TH_TEMP3)
		cur_state = GPU_THROTTLING2;
	else if (temp >= GPU_TH_TEMP1 && temp < GPU_TH_TEMP2)
		cur_state = GPU_THROTTLING1;
	else if (temp > COLD_TEMP && temp < GPU_TH_TEMP1)
		cur_state = GPU_NORMAL;
	else
		cur_state = GPU_COLD;

	exynos_gpu_call_notifier(cur_state);
}

int exynos_tmu_isp_add_notifier(struct notifier_block *n)
{
	return blocking_notifier_chain_register(&exynos_isp_notifier, n);
}

void exynos_isp_call_notifier(enum isp_noti_state_t cur_state)
{
	if (cur_state != isp_old_state) {
		pr_info("isp temperature state %d to %d\n", isp_old_state, cur_state);
		blocking_notifier_call_chain(&exynos_isp_notifier, cur_state, &cur_state);
		isp_old_state = cur_state;
	}
}

static void exynos_check_isp_noti_state(int temp)
{
	enum isp_noti_state_t cur_state;

	/* check current temperature state */
	if (temp >= ISP_TH_TEMP5)
		cur_state = ISP_TRIPPING;
	else if (temp >= ISP_TH_TEMP4 && temp < ISP_TH_TEMP5)
		cur_state = ISP_THROTTLING4;
	else if (temp >= ISP_TH_TEMP3 && temp < ISP_TH_TEMP4)
		cur_state = ISP_THROTTLING3;
	else if (temp >= ISP_TH_TEMP2 && temp < ISP_TH_TEMP3)
		cur_state = ISP_THROTTLING2;
	else if (temp >= ISP_TH_TEMP1 && temp < ISP_TH_TEMP2)
		cur_state = ISP_THROTTLING1;
	else
		cur_state = ISP_NORMAL;

	exynos_isp_call_notifier(cur_state);
}

/* Get temperature callback functions for thermal zone */
static int exynos_get_temp(struct thermal_zone_device *thermal,
			unsigned long *temp)
{
	void *data;

	if (!th_zone->sensor_conf) {
		pr_info("Temperature sensor not initialised\n");
		return -EINVAL;
	}
	data = th_zone->sensor_conf->private_data;
	*temp = th_zone->sensor_conf->read_temperature(data);
	/* convert the temperature into millicelsius */
	*temp = *temp * MCELSIUS;
	return 0;
}

/* Get temperature callback functions for thermal zone */
static int exynos_set_emul_temp(struct thermal_zone_device *thermal,
						unsigned long temp)
{
	void *data;
	int ret = -EINVAL;

	if (!th_zone->sensor_conf) {
		pr_info("Temperature sensor not initialised\n");
		return -EINVAL;
	}
	data = th_zone->sensor_conf->private_data;
	if (th_zone->sensor_conf->write_emul_temp)
		ret = th_zone->sensor_conf->write_emul_temp(data, temp);
	return ret;
}

/* Get the temperature trend */
static int exynos_get_trend(struct thermal_zone_device *thermal,
			int trip, enum thermal_trend *trend)
{
	int ret;
	unsigned long trip_temp;

	ret = exynos_get_trip_temp(thermal, trip, &trip_temp);
	if (ret < 0)
		return ret;

	if (thermal->temperature >= trip_temp)
		*trend = THERMAL_TREND_RAISE_FULL;
	else
		*trend = THERMAL_TREND_DROP_FULL;

	return 0;
}

#if defined(CONFIG_EXYNOS_MARCH_DYNAMIC_CPU_HOTPLUG) && defined(CONFIG_EXYNOS_GPU_PM_QOS)
static struct pm_qos_request cluster1_cpu_num_qos;
#endif
static int __ref exynos_throttle_cpu_hotplug(struct thermal_zone_device *thermal)
{
	int ret = 0;
	int cur_temp = 0;
	struct exynos_tmu_data *data = th_zone->sensor_conf->private_data;
	struct exynos_tmu_platform_data *pdata = data->pdata;
	struct cpufreq_cooling_device *cpufreq_device = (struct cpufreq_cooling_device *)th_zone->cool_dev[CL_ONE]->devdata;

	if (!thermal->temperature)
		return -EINVAL;

	cur_temp = thermal->temperature / MCELSIUS;

	if (is_cpu_hotplugged_out) {
		if (cur_temp < pdata->hotplug_in_threshold) {
			/*
			 * If current temperature is lower than low threshold,
			 * call cluster1_cores_hotplug(false) for hotplugged out cpus.
			 */
#if defined(CONFIG_EXYNOS_MARCH_DYNAMIC_CPU_HOTPLUG) && defined(CONFIG_EXYNOS_GPU_PM_QOS)
			pm_qos_update_request(&cluster1_cpu_num_qos, NR_CLUST1_CPUS);
			is_cpu_hotplugged_out = false;
			cpufreq_device->cpufreq_state = 0;
#else
			ret = cluster1_cores_hotplug(false);
			if (ret) {
				pr_err("%s: failed cluster1 cores hotplug in\n",
							__func__);
			} else {
				is_cpu_hotplugged_out = false;
				cpufreq_device->cpufreq_state = 0;
			}
#endif
		}
	} else {
		if (cur_temp >= pdata->hotplug_out_threshold) {
			/*
			 * If current temperature is higher than high threshold,
			 * call cluster1_cores_hotplug(true) to hold temperature down.
			 */
#ifdef CONFIG_EXYNOS_MARCH_DYNAMIC_CPU_HOTPLUG
			pm_qos_update_request(&cluster1_cpu_num_qos, 0);
			is_cpu_hotplugged_out = true;			 
#else
			ret = cluster1_cores_hotplug(true);
			if (ret)
				pr_err("%s: failed cluster1 cores hotplug out\n",
							__func__);
			else
				is_cpu_hotplugged_out = true;
#endif			
		}

	}

	return ret;
}

/* Operation callback functions for thermal zone */
static struct thermal_zone_device_ops exynos_dev_ops = {
	.bind = exynos_bind,
	.unbind = exynos_unbind,
	.get_temp = exynos_get_temp,
	.set_emul_temp = exynos_set_emul_temp,
	.get_trend = exynos_get_trend,
	.get_mode = exynos_get_mode,
	.set_mode = exynos_set_mode,
	.get_trip_type = exynos_get_trip_type,
	.get_trip_temp = exynos_get_trip_temp,
	.get_crit_temp = exynos_get_crit_temp,
	.throttle_cpu_hotplug = exynos_throttle_cpu_hotplug,
};

/*
 * This function may be called from interrupt based temperature sensor
 * when threshold is changed.
 */
static void exynos_report_trigger(void)
{
	unsigned int i;
	char data[10];
	char *envp[] = { data, NULL };
	enum thermal_trip_type type = 0;

	if (!th_zone || !th_zone->therm_dev)
		return;
	if (th_zone->bind == false) {
		for (i = 0; i < th_zone->cool_dev_size; i++) {
			if (!th_zone->cool_dev[i])
				continue;
			exynos_bind(th_zone->therm_dev,
					th_zone->cool_dev[i]);
		}
	}

	thermal_zone_device_update(th_zone->therm_dev);

	mutex_lock(&th_zone->therm_dev->lock);
	/* Find the level for which trip happened */
	for (i = 0; i < th_zone->sensor_conf->trip_data.trip_count; i++) {
		if (th_zone->therm_dev->last_temperature <
			th_zone->sensor_conf->trip_data.trip_val[i] * MCELSIUS)
			break;
	}

	if (th_zone->mode == THERMAL_DEVICE_ENABLED) {
		exynos_get_trip_type(th_zone->therm_dev, i, &type);
		if (type == THERMAL_TRIP_ACTIVE)
			th_zone->therm_dev->passive_delay = ACTIVE_INTERVAL;
		else
			th_zone->therm_dev->passive_delay = PASSIVE_INTERVAL;
	}

	mutex_unlock(&th_zone->therm_dev->lock);

	snprintf(data, sizeof(data), "%u", i);
	kobject_uevent_env(&th_zone->therm_dev->device.kobj, KOBJ_CHANGE, envp);
}

/* Register with the in-kernel thermal management */
static int exynos_register_thermal(struct thermal_sensor_conf *sensor_conf)
{
	int ret, count = 0;
#ifdef CONFIG_ARM_EXYNOS_MP_CPUFREQ
	int i, j;
#endif
	struct cpumask mask_val;

	if (!sensor_conf || !sensor_conf->read_temperature) {
		pr_err("Temperature sensor not initialised\n");
		return -EINVAL;
	}

	th_zone = kzalloc(sizeof(struct exynos_thermal_zone), GFP_KERNEL);
	if (!th_zone)
		return -ENOMEM;

	th_zone->sensor_conf = sensor_conf;
	cpumask_clear(&mask_val);
	cpumask_set_cpu(0, &mask_val);

#ifdef CONFIG_ARM_EXYNOS_MP_CPUFREQ
	for (i = 0; i < EXYNOS_ZONE_COUNT; i++) {
		for (j = 0; j < CL_END; j++) {
			th_zone->cool_dev[count] = cpufreq_cooling_register(&mp_cluster_cpus[count]);
			if (IS_ERR(th_zone->cool_dev[count])) {
				pr_err("Failed to register cpufreq cooling device\n");
				ret = -EINVAL;
				th_zone->cool_dev_size = count;
				goto err_unregister;
			}
			count++;
		}
	}
#else
	for (count = 0; count < EXYNOS_ZONE_COUNT; count++) {
		th_zone->cool_dev[count] = cpufreq_cooling_register(&mask_val);
		if (IS_ERR(th_zone->cool_dev[count])) {
			 pr_err("Failed to register cpufreq cooling device\n");
			 ret = -EINVAL;
			 th_zone->cool_dev_size = count;
			 goto err_unregister;
		 }
	}
#endif
	th_zone->cool_dev_size = count;

	th_zone->therm_dev = thermal_zone_device_register(sensor_conf->name,
			th_zone->sensor_conf->trip_data.trip_count, 0, NULL, &exynos_dev_ops, NULL, PASSIVE_INTERVAL,
			IDLE_INTERVAL);

	if (IS_ERR(th_zone->therm_dev)) {
		pr_err("Failed to register thermal zone device\n");
		ret = PTR_ERR(th_zone->therm_dev);
		goto err_unregister;
	}
	th_zone->mode = THERMAL_DEVICE_ENABLED;

	pr_info("Exynos: Kernel Thermal management registered\n");

	return 0;

err_unregister:
	exynos_unregister_thermal();
	return ret;
}

/* Un-Register with the in-kernel thermal management */
static void exynos_unregister_thermal(void)
{
	int i;

	if (!th_zone)
		return;

	if (th_zone->therm_dev)
		thermal_zone_device_unregister(th_zone->therm_dev);

	for (i = 0; i < th_zone->cool_dev_size; i++) {
		if (th_zone->cool_dev[i])
			cpufreq_cooling_unregister(th_zone->cool_dev[i]);
	}

	kfree(th_zone);
	pr_info("Exynos: Kernel Thermal management unregistered\n");
}

static int exynos_tmu_initialize(struct platform_device *pdev, int id)
{
	struct exynos_tmu_data *data = platform_get_drvdata(pdev);
	struct exynos_tmu_platform_data *pdata = data->pdata;
	unsigned int status;
	unsigned int rising_threshold[4] = {0,}, falling_threshold[4] = {0,};
	int ret = 0, threshold_code, i, trigger_levs = 0, index;
	int timeout = 20000;

	mutex_lock(&data->lock);

	while(1) {
		status = readb(data->base[id] + EXYNOS_TMU_REG_STATUS);
		if (status)
			break;

		timeout--;
		if (!timeout) {
			pr_err("%s: timeout TMU busy\n", __func__);
			ret = -EBUSY;
			goto out;
		}

		cpu_relax();
		usleep_range(1, 2);
	};

	/* Count trigger levels to be enabled */
	for (i = 0; i < MAX_THRESHOLD_LEVS; i++)
		if (pdata->trigger_levels[i])
			trigger_levs++;

	for (i = 0; i < trigger_levs; i++) {
		index = i / 2;
		threshold_code = cal_tmu_temp_to_code(data->cal_data,
				pdata->trigger_levels[i], id);
		if (threshold_code < 0) {
			ret = threshold_code;
			goto out;
		}
		if (i % 2)
			rising_threshold[index] |= threshold_code << 16;
		else
			rising_threshold[index] |= threshold_code;
		if (pdata->threshold_falling) {
			threshold_code = cal_tmu_temp_to_code(data->cal_data,
					pdata->trigger_levels[i] -
					pdata->threshold_falling, id);
			if (threshold_code > 0) {
				if (i % 2)
					falling_threshold[index] |= threshold_code << 16;
				else
					falling_threshold[index] |= threshold_code;
			}
		}
	}

	for (i = 0; i < (trigger_levs / 2); i++) {
		writel(rising_threshold[i],
				data->base[id] + EXYNOS_THD_TEMP_RISE1_0 - (EXYNOS_THD_OFFSET * i));
		writel(falling_threshold[i],
				data->base[id] + EXYNOS_THD_TEMP_FALL1_0 - (EXYNOS_THD_OFFSET * i));
	}

	writel(EXYNOS_TMU_CLEAR_RISE_INT | EXYNOS_TMU_CLEAR_FALL_INT,
			data->base[id] + EXYNOS_TMU_REG_INTCLEAR);

	/* Adjuest sampling interval default -> 1ms */
	/* W/A for WTSR */
	writel(0xE10, data->base[id] + EXYNOS_TMU_REG_SAMPLING_INTERVAL);

out:
	mutex_unlock(&data->lock);

	return ret;
}

static int get_cur_probe_sensor(struct exynos_tmu_data *data, int id)
{
	int status, ret = 0;

	status = readl(data->base[id] + EXYNOS_TMU_REG_STATUS);
	ret = (status >> EXYNOS_CUR_SENSING_PROBE_SHIFT) & EXYNOS_CUR_SENSING_PROBE_MASK;

	return ret;
}

static void exynos_tmu_get_efuse(struct platform_device *pdev, int id)
{
	struct exynos_tmu_data *data = platform_get_drvdata(pdev);
	struct exynos_tmu_platform_data *pdata = data->pdata;
	unsigned int trim_info, temp;

	mutex_lock(&data->lock);

	while (get_cur_probe_sensor(data, id) != SENSOR_P0)
		usleep_range(1, 2);

	/* Save trimming info in order to perform calibration */
	trim_info = readl(data->base[id] + EXYNOS_TMU_REG_TRIMINFO);

	if (trim_info & CALIB_SEL_MASK)
		pdata->cal_type = TYPE_TWO_POINT_TRIMMING;
	else
		pdata->cal_type = TYPE_ONE_POINT_TRIMMING;

	data->cal_data->cal_type = pdata->cal_type;
	data->cal_data->temp_error1[id] = trim_info & EXYNOS_TMU_TRIM_TEMP_MASK;
	data->cal_data->temp_error2[id] = ((trim_info >> 9) & EXYNOS_TMU_TRIM_TEMP_MASK);

	if (data->cal_data->temp_error1[id] == 0)
		data->cal_data->temp_error1[id] = pdata->efuse_value;

	if (id != EXYNOS_TMU_CORE_LITTLE) {
		temp = (trim_info >> EXYNOS_TMU_TRIM_BUF_VREF_SHIFT) & EXYNOS_TMU_TRIM_BUF_VREF_MASK;

		if (temp != 0)
			data->cal_data->reference_voltage[id] = temp;
	}

	mutex_unlock(&data->lock);
}

static void exynos_tmu_control(struct platform_device *pdev, int id, bool on)
{
	struct exynos_tmu_data *data = platform_get_drvdata(pdev);

	mutex_lock(&data->lock);

	cal_tmu_control(data->cal_data, id, on);

	mutex_unlock(&data->lock);
}

static int exynos_tmu_read(struct exynos_tmu_data *data)
{
	int temp, i, max = INT_MIN, min = INT_MAX, gpu_temp = -1;
	int alltemp[EXYNOS_TMU_COUNT] = {0, };
	unsigned int threshold[EXYNOS_TMU_COUNT] = {0, };
	unsigned int cur_probe[EXYNOS_TMU_COUNT], num_probe[EXYNOS_TMU_COUNT];
#ifdef CONFIG_EXYNOS_SWTRIP
	char tmustate_string[20];
	char *envp[2];
#endif

	mutex_lock(&data->lock);

	for (i = 0; i < EXYNOS_TMU_COUNT; i++) {
		temp = cal_tmu_read(data->cal_data, i);
		alltemp[i] = temp;

		if (temp != -1) {
			if (i == EXYNOS_GPU_NUMBER) {
				gpu_temp = temp;
			} else {
				if (temp > max)
					max = temp;
				if (temp < min)
					min = temp;
			}
		}

		threshold[i] = readl(data->base[i] + EXYNOS_THD_TEMP_RISE7_6) >> 16;
		num_probe[i] = (readl(data->base[i] + EXYNOS_TMU_REG_CONTROL1) >> 16) & 0x7;
		cur_probe[i] = (readl(data->base[i] + EXYNOS_TMU_REG_STATUS) >> 8) & 0x7;
	}

	exynos_ss_printk("[TMU]%d, %d, %d, %d; %d, %d, %d, %d\n",
			alltemp[0], alltemp[1], alltemp[2], alltemp[3], threshold[0], threshold[1], threshold[2], threshold[3]);
	exynos_ss_printk("[TMU_PROBE](%d/%d)(%d/%d)(%d/%d)(%d/%d)\n", cur_probe[0], num_probe[0], cur_probe[1], num_probe[1],
					cur_probe[2], num_probe[2], cur_probe[3], num_probe[3]);
#ifdef CONFIG_EXYNOS_SWTRIP
	if (max >= SWTRIP_TEMP)
		swtrip_counter++;
	else
		swtrip_counter = 0;

	if (swtrip_counter >= SWTRIP_NOISE_COUNT) {
		snprintf(tmustate_string, sizeof(tmustate_string), "TMUSTATE=%d", 3);
		envp[0] = tmustate_string;
		envp[1] = NULL;
		pr_err("[TMU] SW trip by reaching trip temp(%d)!\n", SWTRIP_TEMP);
		if (th_zone && th_zone->therm_dev)
			kobject_uevent_env(&th_zone->therm_dev->device.kobj, KOBJ_CHANGE, envp);
		else
			pr_err("[TMU] do not call kobject_uevent_env() because th_zone not initialised\n");
	}
#endif

	if (likely(!tmudata->lpa)) {
		exynos_check_tmu_noti_state(max);
		exynos_check_mif_noti_state(max);

		if (gpu_temp != -1)
			exynos_check_gpu_noti_state(gpu_temp);

		if (alltemp[3] != -1)
			exynos_check_isp_noti_state(alltemp[3]);
	}

	mutex_unlock(&data->lock);
#if defined(CONFIG_CPU_THERMAL_IPA)
	check_switch_ipa_on(max);
#endif
	pr_debug("[TMU] TMU0 = %d, TMU1 = %d, TMU2 = %d, TMU3 = %d    MAX = %d, GPU = %d\n",
			alltemp[0], alltemp[1], alltemp[2], alltemp[3], max, gpu_temp);

	return max;
}

#if defined(CONFIG_CPU_THERMAL_IPA)
int ipa_hotplug(bool removecores)
{
#if defined(CONFIG_EXYNOS_MARCH_DYNAMIC_CPU_HOTPLUG) && defined(CONFIG_EXYNOS_GPU_PM_QOS)
	if (removecores == 0)
		pm_qos_update_request(&cluster1_cpu_num_qos, NR_CLUST1_CPUS);
	else if (removecores == 1)
		pm_qos_update_request(&cluster1_cpu_num_qos, 0);
	else
		return 1;
	return 0;
#else
	return cluster1_cores_hotplug(removecores);
#endif
}
#endif


#ifdef CONFIG_THERMAL_EMULATION
static int exynos_tmu_set_emulation(void *drv_data, unsigned long temp)
{
	struct exynos_tmu_data *data = drv_data;
	unsigned int reg;
	int ret = -EINVAL;
	int i;

	if (temp && temp < MCELSIUS)
		goto out;

	mutex_lock(&data->lock);

	if (temp)
		temp /= MCELSIUS;

	for (i = 0; i < EXYNOS_TMU_COUNT; i++) {
		reg = readl(data->base[i] + EXYNOS_EMUL_CON);

		if (temp) {
			reg = (EXYNOS_EMUL_TIME << EXYNOS_EMUL_TIME_SHIFT) |
				(cal_tmu_temp_to_code(data->cal_data, temp, i)
				 << EXYNOS_EMUL_DATA_SHIFT) | EXYNOS_EMUL_ENABLE;
		} else {
			reg &= ~EXYNOS_EMUL_ENABLE;
		}

		writel(reg, data->base[i] + EXYNOS_EMUL_CON);
	}

	mutex_unlock(&data->lock);
	return 0;
out:
	return ret;
}
#else
static int exynos_tmu_set_emulation(void *drv_data,	unsigned long temp)
	{ return -EINVAL; }
#endif/*CONFIG_THERMAL_EMULATION*/

static void exynos_tmu_work(struct work_struct *work)
{
	struct exynos_tmu_data *data = container_of(work,
			struct exynos_tmu_data, irq_work);
	int i, j;
	unsigned int pending[EXYNOS_TMU_COUNT][5], code[EXYNOS_TMU_COUNT], reg_data;

	mutex_lock(&data->lock);
	for (i = 0; i < EXYNOS_TMU_COUNT; i++) {
		for (j = 0; j < 5; j++)
			pending[i][j] = readl(data->base[i] + EXYNOS_TMU_REG_INTCLEAR + (j * 0x10));

		code[i] = readl(data->base[i] + EXYNOS_TMU_REG_CURRENT_TEMP0);
		writel(EXYNOS_TMU_CLEAR_RISE_INT | EXYNOS_TMU_CLEAR_FALL_INT,
				data->base[i] + EXYNOS_TMU_REG_INTCLEAR);
	}
	mutex_unlock(&data->lock);
	exynos_report_trigger();

	for (i = 0; i < EXYNOS_TMU_COUNT; i++) {
		for (j = 1; j < 5; j++) {
			if(pending[i][j] != 0x0)
				exynos_ss_printkl(i << 8 | j, pending[i][j]);
		}
		if (pending[i][0] & (1 << 7)) {
			exynos_ss_printkl(i, code[i]);

			mutex_lock(&data->lock);

			reg_data = readl(data->base[i] + EXYNOS_TMU_REG_TRIMINFO);
			pr_info("TRIMINFO[%d] = 0x%x\n", i, reg_data);
			reg_data = readl(data->base[i] + EXYNOS_TMU_REG_CONTROL);
			pr_info("TMU_CONTROL[%d] = 0x%x\n", i, reg_data);
			reg_data = readl(data->base[i] + EXYNOS_TMU_REG_CONTROL1);
			pr_info("TMU_CONTROL1[%d] = 0x%x\n", i, reg_data);
			reg_data = readl(data->base[i] + EXYNOS_TMU_REG_CURRENT_TEMP0);
			pr_info("CURRENT_TEMP0[%d] = 0x%x\n", i, reg_data);
			reg_data = readl(data->base[i] + EXYNOS_TMU_REG_CURRENT_TEMP1);
			pr_info("CURRENT_TEMP1[%d] = 0x%x\n", i, reg_data);
			reg_data = readl(data->base[i] + EXYNOS_THD_TEMP_RISE1_0);
			pr_info("THRESHOLD_TEMP_RISE1_0[%d] = 0x%x\n", i, reg_data);
			reg_data = readl(data->base[i] + EXYNOS_THD_TEMP_RISE3_2);
			pr_info("THRESHOLD_TEMP_RISE3_2[%d] = 0x%x\n", i, reg_data);
			reg_data = readl(data->base[i] + EXYNOS_THD_TEMP_RISE5_4);
			pr_info("THRESHOLD_TEMP_RISE5_4[%d] = 0x%x\n", i, reg_data);
			reg_data = readl(data->base[i] + EXYNOS_THD_TEMP_RISE7_6);
			pr_info("THRESHOLD_TEMP_RISE7_6[%d] = 0x%x\n", i, reg_data);
			reg_data = readl(data->base[i] + EXYNOS_THD_TEMP_FALL1_0);
			pr_info("THRESHOLD_TEMP_FALL1_0[%d] = 0x%x\n", i, reg_data);
			reg_data = readl(data->base[i] + EXYNOS_THD_TEMP_FALL3_2);
			pr_info("THRESHOLD_TEMP_FALL3_2[%d] = 0x%x\n", i, reg_data);
			reg_data = readl(data->base[i] + EXYNOS_THD_TEMP_FALL5_4);
			pr_info("THRESHOLD_TEMP_FALL5_4[%d] = 0x%x\n", i, reg_data);
			reg_data = readl(data->base[i] + EXYNOS_THD_TEMP_FALL7_6);
			pr_info("THRESHOLD_TEMP_FALL7_6[%d] = 0x%x\n", i, reg_data);
			reg_data = readl(data->base[i] + EXYNOS_TMU_REG_INTEN);
			pr_info("INTEN[%d] = 0x%x\n", i, reg_data);
			pr_info("INTCLEAR[%d] = 0x%x\n", i, pending[i][0]);
			reg_data = readl(data->base[i] + EXYNOS_TMU_REG_STATUS);
			pr_info("TMUSTATUS[%d] = 0x%x\n", i, reg_data);


			for (j = 0; j < 8; j++) {
				reg_data = readl(data->base[i] + 0x70 + (0x4 * i));
				pr_info("PAST_TEMP(%d) = 0x%x\n", j, reg_data);
			}

			mutex_unlock(&data->lock);

			panic("TMU debugging panic: interrupt_7(%d) is detected\n", i);
		}
	}

	for (i = 0; i < EXYNOS_TMU_COUNT; i++)
		enable_irq(data->irq[i]);
}

static irqreturn_t exynos_tmu_irq(int irq, void *id)
{
	struct exynos_tmu_data *data = id;
	int i;

	pr_debug("[TMUIRQ] irq = %d\n", irq);

	for (i = 0; i < EXYNOS_TMU_COUNT; i++)
		disable_irq_nosync(data->irq[i]);
	schedule_work(&data->irq_work);

	return IRQ_HANDLED;
}
static struct thermal_sensor_conf exynos_sensor_conf = {
	.name			= "exynos-therm",
	.read_temperature	= (int (*)(void *))exynos_tmu_read,
	.write_emul_temp	= exynos_tmu_set_emulation,
};
#if defined(CONFIG_CPU_THERMAL_IPA)
static struct ipa_sensor_conf ipa_sensor_conf = {
	.read_soc_temperature	= (int (*)(void *))exynos_tmu_read,
};
#endif
static int exynos_pm_notifier(struct notifier_block *notifier,
		unsigned long pm_event, void *v)
{
	switch (pm_event) {
	case PM_SUSPEND_PREPARE:
		is_suspending = true;
		exynos_tmu_call_notifier(TMU_COLD, 0);
		exynos_gpu_call_notifier(TMU_COLD);
		break;
	case PM_POST_SUSPEND:
		is_suspending = false;
		break;
	}

	return NOTIFY_OK;
}

static struct notifier_block exynos_pm_nb = {
	.notifier_call = exynos_pm_notifier,
};

void exynos_tmu_core_control(bool on, int id)
{
	int i;
	unsigned int con;
	struct exynos_tmu_data *data;

	if (exynos_tmu_pdev == NULL)
		return;

	data = platform_get_drvdata(exynos_tmu_pdev);

	if (!is_tmu_probed || data == NULL)
		return;

	con = readl(data->base[id] + EXYNOS_TMU_REG_CONTROL);
	con &= TMU_CONTROL_ONOFF_MASK;
	con |= (on ? EXYNOS_TMU_CORE_ON : EXYNOS_TMU_CORE_OFF);
	writel(con, data->base[id] + EXYNOS_TMU_REG_CONTROL);

	if (!on) {
		for (i = 0; i < IDLE_MAX_TIME; i++) {
			if (readl(data->base[id] + EXYNOS_TMU_REG_STATUS) & 0x1)
				break;
			usleep_range(1, 2);
		}
		if (i == (IDLE_MAX_TIME - 1))
			pr_err("@@@@@ TMU CHECK BUSY @@@@@@\n");
	}
}

#if defined(CONFIG_CPU_IDLE)
static void exynos_tmu_all_cores_control(bool on)
{
	int i, j;
	unsigned int con;
	unsigned int status;
	struct exynos_tmu_data *data;

	if (exynos_tmu_pdev == NULL)
		return;

	data = platform_get_drvdata(exynos_tmu_pdev);

	if (data == NULL)
		return;

	for (i = 0; i < EXYNOS_TMU_COUNT; i++) {
		con = readl(data->base[i] + EXYNOS_TMU_REG_CONTROL);
		con &= TMU_CONTROL_ONOFF_MASK;
		con |= (on ? EXYNOS_TMU_CORE_ON : EXYNOS_TMU_CORE_OFF);
		writel(con, data->base[i] + EXYNOS_TMU_REG_CONTROL);
	}

	if (!on) {
		for (j=0; j < IDLE_MAX_TIME; j++) {
			status = 0;

			for (i = 0; i < EXYNOS_TMU_COUNT; i++)
				status |= (((readl(data->base[i] + EXYNOS_TMU_REG_STATUS) & 0x1)) << i);

			if (status == 0xF)
				break;
		}
		if (j == (IDLE_MAX_TIME - 1))
			pr_err("@@@@@ TMU CHECK BUSY @@@@@@\n");
	}

}


void lpa_enter(struct work_struct *work)
{
	exynos_tmu_call_notifier(TMU_COLD, 0);
	exynos_gpu_call_notifier(TMU_COLD);
}

void lpa_exit(struct work_struct *work)
{
	tmudata->lpa = false;
}

static DECLARE_WORK(lpa_enter_work, lpa_enter);
static DECLARE_DELAYED_WORK(lpa_exit_work, lpa_exit);

static void schedule_lpa_enter(unsigned long data)
{
	queue_work(system_freezable_wq, &lpa_enter_work);
}

static void schedule_lpa_exit(unsigned long data)
{
	mod_delayed_work(system_freezable_wq, &lpa_exit_work, msecs_to_jiffies(1000));
}

static DECLARE_TASKLET(lpa_enter_tasklet, schedule_lpa_enter, 0);
static DECLARE_TASKLET(lpa_exit_tasklet, schedule_lpa_exit, 0);

static int exynos_pm_dstop_notifier(struct notifier_block *notifier,
		unsigned long pm_event, void *v)
{
	switch (pm_event) {
	case LPA_ENTER:
		exynos_tmu_all_cores_control(false);
		if (unlikely(!tmudata->lpa)) {
			tmudata->lpa = true;
			tasklet_schedule(&lpa_enter_tasklet);
		}
		break;
	case LPA_ENTER_FAIL:
	case LPA_EXIT:
		exynos_tmu_all_cores_control(true);
		tasklet_schedule(&lpa_exit_tasklet);
		break;
	}

	return NOTIFY_OK;
}

static struct notifier_block exynos_pm_dstop_nb = {
	.notifier_call = exynos_pm_dstop_notifier,
};
#endif

#if defined(CONFIG_SOC_EXYNOS7420)
static struct exynos_tmu_platform_data exynos7420_tmu_data = {
	.type = SOC_ARCH_EXYNOS7420,
};
#define EXYNOS7420_TMU_DRV_DATA (&exynos7420_tmu_data)
#else
#define EXYNOS7420_TMU_DRV_DATA (NULL)
#endif

#ifdef CONFIG_OF
static const struct of_device_id exynos_tmu_match[] = {
	{
		.compatible = "samsung,exynos7420-tmu",
		.data = (void *)EXYNOS7420_TMU_DRV_DATA,
	},
	{},
};
MODULE_DEVICE_TABLE(of, exynos_tmu_match);
#endif

static struct platform_device_id exynos_tmu_driver_ids[] = {
	{
		.name		= "exynos7420-tmu",
		.driver_data	= (kernel_ulong_t)EXYNOS7420_TMU_DRV_DATA,
	},
	{ },
};
MODULE_DEVICE_TABLE(platform, exynos_tmu_driver_ids);

static inline struct  exynos_tmu_platform_data *exynos_get_driver_data(
			struct platform_device *pdev)
{
#ifdef CONFIG_OF
	if (pdev->dev.of_node) {
		const struct of_device_id *match;
#ifdef CONFIG_ARM_EXYNOS_MP_CPUFREQ
		init_mp_cpumask_set();
#endif
		match = of_match_node(exynos_tmu_match, pdev->dev.of_node);
		if (!match)
			return NULL;
		return (struct exynos_tmu_platform_data *) match->data;
	}
#endif
	return (struct exynos_tmu_platform_data *)
			platform_get_device_id(pdev)->driver_data;
}

/* sysfs interface : /sys/devices/1006000.tmu/temp */
static ssize_t
exynos_thermal_sensor_temp(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct exynos_tmu_data *data = th_zone->sensor_conf->private_data;
	unsigned long temp[EXYNOS_TMU_COUNT] = {0,};
	int i, len = 0;

	mutex_lock(&data->lock);

	for (i = 0; i < EXYNOS_TMU_COUNT; i++) {
		temp[i] = cal_tmu_read(data->cal_data, i);
		if (temp[i] != -1)	temp[i] = temp[i] * MCELSIUS;
	}

	mutex_unlock(&data->lock);

	for (i = 0; i < EXYNOS_TMU_COUNT; i++)
		len += snprintf(&buf[len], PAGE_SIZE, "sensor%d : %ld\n", i, temp[i]);

	return len;
}

static DEVICE_ATTR(temp, S_IRUSR | S_IRGRP, exynos_thermal_sensor_temp, NULL);

/* sysfs interface : /sys/devices/1006000.tmu/curr_temp */
static ssize_t
exynos_thermal_curr_temp(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct exynos_tmu_data *data = th_zone->sensor_conf->private_data;
	unsigned long temp[EXYNOS_TMU_COUNT];
	int i, len = 0;

	mutex_lock(&data->lock);

	for (i = 0; i < EXYNOS_TMU_COUNT; i++) {
		temp[i] = cal_tmu_read(data->cal_data, i);
	}

	mutex_unlock(&data->lock);

	len += snprintf(&buf[len], PAGE_SIZE,"%ld,", temp[0]);
	len += snprintf(&buf[len], PAGE_SIZE,"%ld,", temp[1]);
	len += snprintf(&buf[len], PAGE_SIZE,"%ld,", temp[2]);
	len += snprintf(&buf[len], PAGE_SIZE,"%ld\n", temp[3]);

	return len;
}

static DEVICE_ATTR(curr_temp, S_IRUGO, exynos_thermal_curr_temp, NULL);

static struct attribute *exynos_thermal_sensor_attributes[] = {
	&dev_attr_temp.attr,
	&dev_attr_curr_temp.attr,
	NULL
};

static const struct attribute_group exynos_thermal_sensor_attr_group = {
	.attrs = exynos_thermal_sensor_attributes,
};

static void exynos_set_cal_data(struct platform_device *pdev, struct exynos_tmu_data *data)
{
	int i;

	for (i = 0; i < EXYNOS_TMU_COUNT; i++)
		data->cal_data->base[i] = data->base[i];

	of_property_read_u32_array(pdev->dev.of_node, "gain", data->cal_data->gain, EXYNOS_TMU_COUNT);
	of_property_read_u32_array(pdev->dev.of_node, "reference_voltage", data->cal_data->reference_voltage, EXYNOS_TMU_COUNT);

	data->cal_data->noise_cancel_mode = data->pdata->noise_cancel_mode;
	data->cal_data->cal_type = data->pdata->cal_type;

	data->cal_data->trigger_level_en[0] = data->pdata->trigger_level0_en;
	data->cal_data->trigger_level_en[1] = data->pdata->trigger_level1_en;
	data->cal_data->trigger_level_en[2] = data->pdata->trigger_level2_en;
	data->cal_data->trigger_level_en[3] = data->pdata->trigger_level3_en;
	data->cal_data->trigger_level_en[4] = data->pdata->trigger_level4_en;
	data->cal_data->trigger_level_en[5] = data->pdata->trigger_level5_en;
	data->cal_data->trigger_level_en[6] = data->pdata->trigger_level6_en;
	data->cal_data->trigger_level_en[7] = data->pdata->trigger_level7_en;
}

static void exynos_tmu_regdump(struct platform_device *pdev, int id)
{
	struct exynos_tmu_data *data = platform_get_drvdata(pdev);
	unsigned int reg_data;

	mutex_lock(&data->lock);

	reg_data = readl(data->base[id] + EXYNOS_TMU_REG_TRIMINFO);
	pr_info("TRIMINFO[%d] = 0x%x\n", id, reg_data);
	reg_data = readl(data->base[id] + EXYNOS_TMU_REG_CONTROL);
	pr_info("TMU_CONTROL[%d] = 0x%x\n", id, reg_data);
	reg_data = readl(data->base[id] + EXYNOS_TMU_REG_CONTROL1);
	pr_info("TMU_CONTROL1[%d] = 0x%x\n", id, reg_data);
	reg_data = readl(data->base[id] + EXYNOS_TMU_REG_CURRENT_TEMP0);
	pr_info("CURRENT_TEMP0[%d] = 0x%x\n", id, reg_data);
	reg_data = readl(data->base[id] + EXYNOS_TMU_REG_CURRENT_TEMP1);
	pr_info("CURRENT_TEMP1[%d] = 0x%x\n", id, reg_data);
	reg_data = readl(data->base[id] + EXYNOS_THD_TEMP_RISE1_0);
	pr_info("THRESHOLD_TEMP_RISE1_0[%d] = 0x%x\n", id, reg_data);
	reg_data = readl(data->base[id] + EXYNOS_THD_TEMP_RISE3_2);
	pr_info("THRESHOLD_TEMP_RISE3_2[%d] = 0x%x\n", id, reg_data);
	reg_data = readl(data->base[id] + EXYNOS_THD_TEMP_RISE5_4);
	pr_info("THRESHOLD_TEMP_RISE5_4[%d] = 0x%x\n", id, reg_data);
	reg_data = readl(data->base[id] + EXYNOS_THD_TEMP_RISE7_6);
	pr_info("THRESHOLD_TEMP_RISE7_6[%d] = 0x%x\n", id, reg_data);
	reg_data = readl(data->base[id] + EXYNOS_THD_TEMP_FALL1_0);
	pr_info("THRESHOLD_TEMP_FALL1_0[%d] = 0x%x\n", id, reg_data);
	reg_data = readl(data->base[id] + EXYNOS_THD_TEMP_FALL3_2);
	pr_info("THRESHOLD_TEMP_FALL3_2[%d] = 0x%x\n", id, reg_data);
	reg_data = readl(data->base[id] + EXYNOS_THD_TEMP_FALL5_4);
	pr_info("THRESHOLD_TEMP_FALL5_4[%d] = 0x%x\n", id, reg_data);
	reg_data = readl(data->base[id] + EXYNOS_THD_TEMP_FALL7_6);
	pr_info("THRESHOLD_TEMP_FALL7_6[%d] = 0x%x\n", id, reg_data);
	reg_data = readl(data->base[id] + EXYNOS_TMU_REG_INTEN);
	pr_info("INTEN[%d] = 0x%x\n", id, reg_data);
	reg_data = readl(data->base[id] + EXYNOS_TMU_REG_INTCLEAR);
	pr_info("INTCLEAR[%d] = 0x%x\n", id, reg_data);
	reg_data = readl(data->base[id] + EXYNOS_TMU_REG_STATUS);
	pr_info("TMUSTATUS[%d] = 0x%x\n", id, reg_data);

	mutex_unlock(&data->lock);
}

static int parse_trigger_data(struct device_node *np, struct exynos_tmu_platform_data *pdata, int i)
{
	int ret = 0;
	u32 enable, temp;
	struct device_node *np_trigger;
	char node_name[16];

	snprintf(node_name, sizeof(node_name), "trigger_level_%d", i);

	np_trigger = of_find_node_by_name(np, node_name);
	if (!np_trigger)
		return -EINVAL;

	of_property_read_u32(np_trigger, "temp", &temp);
	of_property_read_u32(np_trigger, "enable", &enable);

	pdata->trigger_levels[i] = temp;
	switch (i) {
	case 0:
		pdata->trigger_level0_en = (enable == 0) ? 0 : 1;
		break;
	case 1:
		pdata->trigger_level1_en = (enable == 0) ? 0 : 1;
		break;
	case 2:
		pdata->trigger_level2_en = (enable == 0) ? 0 : 1;
		break;
	case 3:
		pdata->trigger_level3_en = (enable == 0) ? 0 : 1;
		break;
	case 4:
		pdata->trigger_level4_en = (enable == 0) ? 0 : 1;
		break;
	case 5:
		pdata->trigger_level5_en = (enable == 0) ? 0 : 1;
		break;
	case 6:
		pdata->trigger_level6_en = (enable == 0) ? 0 : 1;
		break;
	case 7:
		pdata->trigger_level7_en = (enable == 0) ? 0 : 1;
		break;
	}

	return ret;
}

static int parse_throttle_data(struct device_node *np, struct exynos_tmu_platform_data *pdata, int i)
{
	int ret = 0;
	struct device_node *np_throttle;
	char node_name[15];

	snprintf(node_name, sizeof(node_name), "throttle_tab_%d", i);

	np_throttle = of_find_node_by_name(np, node_name);
	if (!np_throttle)
		return -EINVAL;

	of_property_read_u32(np_throttle, "temp", &pdata->freq_tab[i].temp_level);
	of_property_read_u32(np_throttle, "freq_max_cluster1", &pdata->freq_tab[i].freq_clip_max);

#ifdef CONFIG_ARM_EXYNOS_MP_CPUFREQ
	of_property_read_u32(np_throttle, "freq_max_cluster0", &pdata->freq_tab[i].freq_clip_max_cluster0);
	pdata->freq_tab[i].mask_val = &mp_cluster_cpus[CL_ONE];
	pdata->freq_tab[i].mask_val_cluster0 = &mp_cluster_cpus[CL_ZERO];
#endif

	return ret;
}


static int exynos_tmu_parse_dt(struct device_node *np, struct exynos_tmu_platform_data *pdata)
{
	u32 value, cal_type, trigger_level_count;
	int ret = 0, i;

	if (!np)
		return -EINVAL;

	of_property_read_u32(np, "threshold_falling", &value);
	pdata->threshold_falling = value;
	of_property_read_u32(np, "noise_cancel_mode", &value);
	pdata->noise_cancel_mode = value;
	of_property_read_u32(np, "cal_type", &cal_type);
	of_property_read_u32(np, "efuse_value", &pdata->efuse_value);
	of_property_read_u32(np, "trigger_level_count", &trigger_level_count);
	of_property_read_u32(np, "throttle_count", &pdata->freq_tab_count);
	of_property_read_u32(np, "throttle_active_count", &pdata->size[THERMAL_TRIP_ACTIVE]);
	of_property_read_u32(np, "throttle_passive_count", &pdata->size[THERMAL_TRIP_PASSIVE]);
	of_property_read_u32(np, "hotplug_out_threshold", &pdata->hotplug_out_threshold);
	of_property_read_u32(np, "hotplug_in_threshold", &pdata->hotplug_in_threshold);

	for (i = 0; i < trigger_level_count; i++) {
		ret = parse_trigger_data(np, pdata, i);
		if (ret) {
			pr_err("Failed to load trigger data(%d)\n", i);
			return -EINVAL;
		}
	}

	for (i = 0; i < pdata->freq_tab_count; i++) {
		ret = parse_throttle_data(np, pdata, i);
		if (ret) {
			pr_err("Failed to load throttle data(%d)\n", i);
			return -EINVAL;
		}
	}

	if (cal_type == 1)
		pdata->cal_type = TYPE_ONE_POINT_TRIMMING;
	else if (cal_type == 2)
		pdata->cal_type = TYPE_TWO_POINT_TRIMMING;
	else
		pdata->cal_type = TYPE_NONE;

	return ret;
}

#ifdef CONFIG_ARM_EXYNOS_MP_CPUFREQ
static int exynos_tmu_cpufreq_notifier(struct notifier_block *notifier, unsigned long event, void *v)
{
	int ret = 0, i;

	switch (event) {
	case CPUFREQ_INIT_COMPLETE:
		ret = exynos_register_thermal(&exynos_sensor_conf);
		is_tmu_probed = true;

		if (ret) {
			dev_err(&exynos_tmu_pdev->dev, "Failed to register thermal interface\n");
			sysfs_remove_group(&exynos_tmu_pdev->dev.kobj, &exynos_thermal_sensor_attr_group);
			unregister_pm_notifier(&exynos_pm_nb);
#if defined(CONFIG_CPU_IDLE)
			exynos_pm_unregister_notifier(&exynos_pm_dstop_nb);
#endif
#ifdef CONFIG_ARM_EXYNOS_MP_CPUFREQ
			exynos_cpufreq_init_unregister_notifier(&exynos_cpufreq_nb);
#endif
			platform_set_drvdata(exynos_tmu_pdev, NULL);
			for (i = 0; i < EXYNOS_TMU_COUNT; i++) {
				if (tmudata->irq[i])
					free_irq(tmudata->irq[i], tmudata);
			}
			kfree(tmudata);

			return ret;
		}
#if defined(CONFIG_CPU_THERMAL_IPA)
		ipa_sensor_conf.private_data = exynos_sensor_conf.private_data;
		ipa_register_thermal_sensor(&ipa_sensor_conf);
#endif
		break;
	}
	return 0;
}
#endif

static int exynos_tmu_probe(struct platform_device *pdev)
{
	struct exynos_tmu_data *data;
	struct exynos_tmu_platform_data *pdata = pdev->dev.platform_data;
	int ret, i;

	exynos_tmu_pdev = pdev;
	is_suspending = false;

	if (!pdata)
		pdata = exynos_get_driver_data(pdev);

	if (!pdata) {
		dev_err(&pdev->dev, "No platform init data supplied.\n");
		return -ENODEV;
	}

	ret = exynos_tmu_parse_dt(pdev->dev.of_node, pdata);
	if (ret) {
		dev_err(&pdev->dev, "Failed to load platform data from device tree.\n");
		return -ENODEV;
	}

	data = devm_kzalloc(&pdev->dev, sizeof(struct exynos_tmu_data),
					GFP_KERNEL);
	if (!data) {
		dev_err(&pdev->dev, "Failed to allocate driver structure\n");
		return -ENOMEM;
	}

	data->cal_data = devm_kzalloc(&pdev->dev, sizeof(struct cal_tmu_data),
					GFP_KERNEL);
	if (!data->cal_data) {
		dev_err(&pdev->dev, "Failed to allocate cal data structure\n");
		return -ENOMEM;
	}

#ifdef CONFIG_ARM_EXYNOS_MP_CPUFREQ
	exynos_cpufreq_init_register_notifier(&exynos_cpufreq_nb);
#endif

	INIT_WORK(&data->irq_work, exynos_tmu_work);

	for (i = 0; i < EXYNOS_TMU_COUNT; i++) {
		data->irq[i] = platform_get_irq(pdev, i);
		if (data->irq[i] < 0) {
			ret = data->irq[i];
			dev_err(&pdev->dev, "Failed to get platform irq\n");
			goto err_get_irq;
		}

		ret = request_irq(data->irq[i], exynos_tmu_irq,
				IRQF_TRIGGER_RISING, "exynos_tmu", data);
		if (ret) {
			dev_err(&pdev->dev, "Failed to request irq: %d\n", data->irq[i]);
			goto err_request_irq;
		}

		data->mem[i] = platform_get_resource(pdev, IORESOURCE_MEM, i);
		if (!data->mem[i]) {
			ret = -ENOENT;
			dev_err(&pdev->dev, "Failed to get platform resource\n");
			goto err_get_resource;
		}

		data->base[i] = devm_request_and_ioremap(&pdev->dev, data->mem[i]);
		if (IS_ERR(data->base[i])) {
			ret = PTR_ERR(data->base[i]);
			dev_err(&pdev->dev, "Failed to ioremap memory\n");
			goto err_io_remap;
		}
	}

	data->soc = pdata->type;
	data->pdata = pdata;
	tmudata = data;
	platform_set_drvdata(pdev, data);
	mutex_init(&data->lock);

	exynos_set_cal_data(pdev, data);

	/* Save the eFuse value before initializing TMU */
	for (i = 0; i < EXYNOS_TMU_COUNT; i++)
		exynos_tmu_get_efuse(pdev, i);

	for (i = 0; i < EXYNOS_TMU_COUNT; i++) {
		ret = exynos_tmu_initialize(pdev, i);
		if (ret) {
			dev_err(&pdev->dev, "Failed to initialize TMU\n");
			goto err_tmu;
		}

		exynos_tmu_control(pdev, i, true);
		exynos_tmu_regdump(pdev, i);
	}

	mutex_lock(&data->lock);
	for (i = 0; i < EXYNOS_TMU_COUNT; i++) {
		int temp = cal_tmu_read(data->cal_data, i);
		pr_debug("[TMU]temp[%d] : %d\n", i, temp);
	}
	mutex_unlock(&data->lock);


	/* Register the sensor with thermal management interface */
	(&exynos_sensor_conf)->private_data = data;
	exynos_sensor_conf.trip_data.trip_count = pdata->freq_tab_count;

	for (i = 0; i < pdata->freq_tab_count; i++) {
		exynos_sensor_conf.trip_data.trip_val[i] =
			pdata->threshold + pdata->freq_tab[i].temp_level;
	}

	exynos_sensor_conf.trip_data.trigger_falling = pdata->threshold_falling;

	exynos_sensor_conf.cooling_data.freq_clip_count =
						pdata->freq_tab_count;
	for (i = 0; i < pdata->freq_tab_count; i++) {
		exynos_sensor_conf.cooling_data.freq_data[i].freq_clip_max =
					pdata->freq_tab[i].freq_clip_max;
#ifdef CONFIG_ARM_EXYNOS_MP_CPUFREQ
		exynos_sensor_conf.cooling_data.freq_data[i].freq_clip_max_cluster0 =
					pdata->freq_tab[i].freq_clip_max_cluster0;
#endif
		exynos_sensor_conf.cooling_data.freq_data[i].temp_level =
					pdata->freq_tab[i].temp_level;
		if (pdata->freq_tab[i].mask_val) {
			exynos_sensor_conf.cooling_data.freq_data[i].mask_val =
				pdata->freq_tab[i].mask_val;
#ifdef CONFIG_ARM_EXYNOS_MP_CPUFREQ
			exynos_sensor_conf.cooling_data.freq_data[i].mask_val_cluster0 =
				pdata->freq_tab[i].mask_val_cluster0;
#endif
		} else
			exynos_sensor_conf.cooling_data.freq_data[i].mask_val =
				cpu_all_mask;
	}

	exynos_sensor_conf.cooling_data.size[THERMAL_TRIP_ACTIVE] = pdata->size[THERMAL_TRIP_ACTIVE];
	exynos_sensor_conf.cooling_data.size[THERMAL_TRIP_PASSIVE] = pdata->size[THERMAL_TRIP_PASSIVE];

	register_pm_notifier(&exynos_pm_nb);
#if defined(CONFIG_CPU_IDLE)
	exynos_pm_register_notifier(&exynos_pm_dstop_nb);
#endif
	ret = sysfs_create_group(&pdev->dev.kobj, &exynos_thermal_sensor_attr_group);
	if (ret)
		dev_err(&pdev->dev, "cannot create thermal sensor attributes\n");

	is_cpu_hotplugged_out = false;

	return 0;

err_tmu:
	platform_set_drvdata(pdev, NULL);
err_io_remap:
err_get_resource:
	for (i = 0; i < EXYNOS_TMU_COUNT; i++) {
		if (data->irq[i])
			free_irq(data->irq[i], data);
	}
err_request_irq:
err_get_irq:
	kfree(data);

	return ret;
}

static int exynos_tmu_remove(struct platform_device *pdev)
{
	int i;

	for (i = 0; i < EXYNOS_TMU_COUNT; i++)
		exynos_tmu_control(pdev, i, false);

	unregister_pm_notifier(&exynos_pm_nb);
#if defined(CONFIG_CPU_IDLE)
	exynos_pm_unregister_notifier(&exynos_pm_dstop_nb);
#endif
	exynos_unregister_thermal();

	platform_set_drvdata(pdev, NULL);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int exynos_tmu_suspend(struct device *dev)
{
	int i;

	for (i = 0; i < EXYNOS_TMU_COUNT; i++)
		exynos_tmu_control(to_platform_device(dev), i, false);

	return 0;
}

static int exynos_tmu_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	int i;

	for (i = 0; i < EXYNOS_TMU_COUNT; i++) {
		exynos_tmu_initialize(pdev, i);
		exynos_tmu_control(pdev, i, true);
	}

	return 0;
}

static SIMPLE_DEV_PM_OPS(exynos_tmu_pm,
			 exynos_tmu_suspend, exynos_tmu_resume);
#define EXYNOS_TMU_PM	(&exynos_tmu_pm)
#else
#define EXYNOS_TMU_PM	NULL
#endif

static struct platform_driver exynos_tmu_driver = {
	.driver = {
		.name   = "exynos-tmu",
		.owner  = THIS_MODULE,
		.pm     = EXYNOS_TMU_PM,
		.of_match_table = of_match_ptr(exynos_tmu_match),
	},
	.probe = exynos_tmu_probe,
	.remove	= exynos_tmu_remove,
	.id_table = exynos_tmu_driver_ids,
};

module_platform_driver(exynos_tmu_driver);

MODULE_DESCRIPTION("EXYNOS TMU Driver");
MODULE_AUTHOR("Donggeun Kim <dg77.kim@samsung.com>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:exynos-tmu");
