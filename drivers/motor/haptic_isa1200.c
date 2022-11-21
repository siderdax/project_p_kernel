/*
 *  isa1200.c - Haptic Motor
 *
 *  Copyright (C) 2009 Samsung Electronics
 *  Kyungmin Park <kyungmin.park@samsung.com>
 *  Copyright (c) 2010-2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/pwm.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include "../staging/android/timed_output.h"
#include <linux/i2c/haptic_isa1200.h>
#include <linux/regulator/consumer.h>
#include <linux/regulator/machine.h>
#include <linux/clk.h>

#include <linux/of_gpio.h>

#define DRIVER_NAME "isa1200"

#define ISA1200_HCTRL0		0x30
#define ISA1200_HCTRL0_DEFAULT	1 /* power down mode, default */
#define ISA1200_HCTRL0_HAPDREN			BIT(7)
#define ISA1200_HCTRL0_HAPDIGMOD_PWM_IN		BIT(3)
#define ISA1200_HCTRL0_PWMMOD_DIVIDER_128	0
#define ISA1200_HCTRL0_PWMMOD_DIVIDER_256	1
#define ISA1200_HCTRL0_PWMMOD_DIVIDER_512	2
#define ISA1200_HCTRL0_PWMMOD_DIVIDER_1024	3
#define ISA1200_HCTRL0_OVEREN			BIT(6)
#define ISA1200_HCTRL0_OVERHL			BIT(5)

#define ISA1200_HCTRL1		0x31
#define ISA1200_HCTRL1_RESERVED_BIT6_ON		BIT(6)

#define ISA1200_HCTRL1_MOTTYP_ERM	BIT(5)
#define ISA1200_HCTRL1_MOTTYP_LRA	0

#define	ISA1200_PWR_ON		0x1
#define ISA1200_PWR_OFF		0x0

#define PWM_HAPTIC_PERIOD	38676 /* 202 Hz with 128 divider */
/* duty cycle for max vibration effect */
#define PWM_MAX_VIBRATE_DUTY	(PWM_HAPTIC_PERIOD * 99/100)
/* duty cycle for no vibration effect */
#define PWM_NO_VIBRATE_DUTY	(PWM_HAPTIC_PERIOD * 50/100)

struct isa1200_data {
	struct i2c_client *client;
	struct isa1200_platform_data *pdata;
	struct hrtimer timer;
	struct timed_output_dev dev;
	struct pwm_device *pwm;
	bool vibrate;
};
#if 0
static void isa1200_power(int onoff)
{
	struct regulator *ldo24_regulator = NULL;
	int rc=0;
//motor vdd 3.0v
	ldo24_regulator = regulator_get(NULL, "vdd_ldo24");
	if(ldo24_regulator == NULL) {
 		pr_err("motor power(ldo24) regulator error\n");
 		return;
 	} else {
		pr_debug("motor power(ldo24) turn on\n");
	}
	if(onoff == ISA1200_PWR_ON)
		rc = regulator_enable(ldo24_regulator);
	else
		rc = regulator_disable(ldo24_regulator);
		
	if (rc < 0) {
		pr_err("%s: regulator %sable fail %d\n", __func__,
				onoff ? "en" : "dis", rc);

		goto regs_fail;
	}

regs_fail:
	regulator_put(ldo24_regulator);
	return;
}
#endif
static int isa1200_vibrate_on(struct isa1200_data *haptic)
{
	int ret;
	u8 value;

	pr_debug("vibrate is %d\n", haptic->vibrate);

	if (!haptic->vibrate) {
		ret = pwm_config(haptic->pwm, PWM_NO_VIBRATE_DUTY,
				PWM_HAPTIC_PERIOD);
		if (ret) {
			pr_err("pwm_config failed %d\n", ret);
		}
		pwm_enable(haptic->pwm);
		gpio_set_value(haptic->pdata->hap_en_gpio, 1);
		usleep_range(100, 200);

		value = ISA1200_HCTRL0_HAPDREN |
			ISA1200_HCTRL0_HAPDIGMOD_PWM_IN |
			ISA1200_HCTRL0_PWMMOD_DIVIDER_128; 
		ret = i2c_smbus_write_byte_data(haptic->client, ISA1200_HCTRL0,
						value);
		if (ret < 0) {
			pr_err("write HCTRL0 failed %d\n", ret);
			goto err_return;
		}
		value = ISA1200_HCTRL1_RESERVED_BIT6_ON |
			ISA1200_HCTRL1_MOTTYP_LRA;
		ret = i2c_smbus_write_byte_data(haptic->client, ISA1200_HCTRL1,
						value);
		if (ret < 0) {
			pr_err("write HCTRL1 failed %d\n", ret);
			goto err_return;
		}
		ret = pwm_config(haptic->pwm, PWM_MAX_VIBRATE_DUTY,
				PWM_HAPTIC_PERIOD);
		if (ret) {
			pr_err("pwm_config for max duty failed %d\n", ret);
			goto err_return;
		}
		haptic->vibrate = true;
	}
	return 0;

err_return:
	gpio_set_value(haptic->pdata->hap_en_gpio, 0);
	pwm_disable(haptic->pwm);
	return ret;
}
#if 1
static void isa1200_vibrate_off(struct isa1200_data *haptic)
{
	pr_debug("vibrate is %d\n", haptic->vibrate);
	if (haptic->vibrate) {
		gpio_set_value(haptic->pdata->hap_en_gpio, 0);
		pwm_disable(haptic->pwm);
		haptic->vibrate = false;
	}
}
#endif

static void isa1200_enable(struct timed_output_dev *dev, int timeout)
{
	struct isa1200_data *haptic = container_of(dev, struct isa1200_data,
					dev);

	hrtimer_cancel(&haptic->timer);
	pr_debug("timeout is %d msec\n", timeout);
	if (timeout > 0) {
		isa1200_vibrate_on(haptic);
		if (timeout > haptic->pdata->max_timeout)
			timeout = haptic->pdata->max_timeout;
		hrtimer_start(&haptic->timer,
			ns_to_ktime((u64)timeout * NSEC_PER_MSEC * 3),	// vibration time
			HRTIMER_MODE_REL);
	} else {
		isa1200_vibrate_off(haptic);
	}

}

static int isa1200_get_time(struct timed_output_dev *dev)
{
	struct isa1200_data *haptic = container_of(dev, struct isa1200_data,
					dev);
	if (hrtimer_active(&haptic->timer)) {
		ktime_t r = hrtimer_get_remaining(&haptic->timer);
		return ktime_to_ms(r);
	}
	return 0;
}

static enum hrtimer_restart isa1200_timer_func(struct hrtimer *timer)
{
	struct isa1200_data *haptic = container_of(timer, struct isa1200_data,
					timer);
	pr_debug("vibrate is %d\n", haptic->vibrate);

	isa1200_vibrate_off(haptic);
	return HRTIMER_NORESTART;
}

static int vibetonz_clk_on(struct device *dev, bool en)
{       
	struct clk *vibetonz_clk = NULL;

#if defined(CONFIG_OF) 
	struct device_node *np;

	np = of_find_node_by_name(NULL,"pwm");
	if (np == NULL) {
		pr_err("[VIB] %s : pwm error to get dt node\n", __func__);
		return -EINVAL;
	}

	vibetonz_clk = of_clk_get_by_name(np, "gate_timers");
	if (!vibetonz_clk) {
		pr_debug("[VIB] %s fail to get the vibetonz_clk\n", __func__);
		return -EINVAL;
	}
#endif          
	pr_debug("[VIB] DEV NAME %s %lu\n",
			dev_name(dev), clk_get_rate(vibetonz_clk));

	if (IS_ERR(vibetonz_clk)) {
		pr_err("[VIB] failed to get clock for the motor\n");
		goto err_clk_get;
	}

	if (en)
		clk_enable(vibetonz_clk);
	else
		clk_disable(vibetonz_clk);

	clk_put(vibetonz_clk);
	return 0;

err_clk_get:
	clk_put(vibetonz_clk);
	return -EINVAL;
} 
#ifdef CONFIG_OF
static int isa1200_parse_dt(struct device *dev,
			struct isa1200_platform_data *pdata)
{
	struct device_node *np = dev->of_node;
	enum of_gpio_flags hap_en_flags = OF_GPIO_ACTIVE_LOW;

	int rc = 0;
	printk(KERN_WARNING "%s\n", __func__);

	pdata->hap_en_gpio = of_get_named_gpio_flags(np,
				"imagis,hap-en-gpio", 0, &hap_en_flags);
	printk(KERN_WARNING "%s: %d\n", "imagis,hap-en-gpio", pdata->hap_en_gpio);

	rc = of_property_read_u32(np, "imagis,max-timeout",
				&pdata->max_timeout);
	if (rc) {
		dev_err(dev, "Unable to read max timeout\n");
		return rc;
	}

	rc = of_property_read_u32(np, "imagis,pwm-ch", &pdata->pwm_ch);
	if (rc && (rc != -EINVAL)) {
		dev_err(dev, "Unable to read pwm channel\n");
		return rc;
	}

	return 0;
}
#endif

static int isa1200_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	struct isa1200_data *haptic;
	struct isa1200_platform_data *pdata;
	int ret;
	pr_debug("\n");

	printk(KERN_WARNING "%s: %s registering\n", __func__, id->name);
	
	if (!i2c_check_functionality(client->adapter,
			I2C_FUNC_SMBUS_BYTE_DATA)) {
		dev_err(&client->dev, "%s: no support for i2c read/write"
				"byte data\n", __func__);
		return -EIO;
	}

	if (client->dev.of_node) {
		pdata = devm_kzalloc(&client->dev,
			sizeof(struct isa1200_platform_data), GFP_KERNEL);
		if (!pdata) {
			dev_err(&client->dev, "Failed to allocate memory\n");
			return -ENOMEM;
		}

		ret = isa1200_parse_dt(&client->dev, pdata);
		if (ret) {
			dev_err(&client->dev, "Parsing DT failed(%d)", ret);
			return ret;
		}
	} else
		pdata = client->dev.platform_data;

	if (!pdata) {
		dev_err(&client->dev, "%s: no platform data\n", __func__);
		return -EINVAL;
	}

	vibetonz_clk_on(&client->dev, true);


	haptic = kzalloc(sizeof(*haptic), GFP_KERNEL);
	if (!haptic)
		return -ENOMEM;
	haptic->client = client;
	haptic->pdata = pdata;
	haptic->vibrate = false;

	ret = gpio_request_one(pdata->hap_en_gpio, GPIOF_OUT_INIT_LOW,
				"haptic_gpio");
	if (ret) {
		printk(KERN_ERR "%s: gpio %d request failed with error %d\n",
			__func__, pdata->hap_en_gpio, ret);
		goto enable_gpio_fail;
	}

	haptic->pwm = pwm_request(pdata->pwm_ch, id->name);
	if (IS_ERR(haptic->pwm)) {
		pr_err("pwm request failed\n");
		ret = PTR_ERR(haptic->pwm);
		goto pwm_req_fail;
	}


	hrtimer_init(&haptic->timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	haptic->timer.function = isa1200_timer_func;

	i2c_set_clientdata(client, haptic);

	/* register with timed output class */
	haptic->dev.name = "vibrator";
	haptic->dev.get_time = isa1200_get_time;
	haptic->dev.enable = isa1200_enable;
	ret = timed_output_dev_register(&haptic->dev);
	if (ret < 0) {
		pr_err("timed output register failed %d\n", ret);
		goto setup_fail;
	}
	//isa1200_power(ISA1200_PWR_ON);		// PMIC ldo24 ON VCC_3.0V_MOTOR

	pr_debug("%s registered\n", id->name);
	return 0;

setup_fail:
	pwm_free(haptic->pwm);
pwm_req_fail:
	gpio_free(pdata->hap_en_gpio);
enable_gpio_fail:
	kfree(haptic);
	return ret;
}

static int isa1200_remove(struct i2c_client *client)
{
	struct isa1200_data *haptic = i2c_get_clientdata(client);
	gpio_set_value(haptic->pdata->hap_en_gpio, 0);
	timed_output_dev_unregister(&haptic->dev);
	hrtimer_cancel(&haptic->timer);
	pwm_free(haptic->pwm);
	gpio_free(haptic->pdata->hap_en_gpio);
	kfree(haptic);
	return 0;
}

static const struct i2c_device_id isa1200_id[] = {
	{ DRIVER_NAME, 0 },
	{ },
};
MODULE_DEVICE_TABLE(i2c, isa1200_id);
#ifdef CONFIG_OF
static struct of_device_id isa1200_match_table[] = {
	{ .compatible = "imagis,isa1200",},
	{ },
};
#else
#define isa1200_match_table NULL
#endif

static struct i2c_driver isa1200_driver = {
	.driver	= {
		.name	= DRIVER_NAME,
		.of_match_table = isa1200_match_table,
	},
	.probe		= isa1200_probe,
	.remove		= isa1200_remove,
	.id_table	= isa1200_id,
};

static int __init isa1200_init(void)
{
	printk(KERN_WARNING "%s: %s\n", __func__, isa1200_driver.driver.name);
	return i2c_add_driver(&isa1200_driver);
}

static void __exit isa1200_exit(void)
{
	i2c_del_driver(&isa1200_driver);
}

module_init(isa1200_init);
module_exit(isa1200_exit);

MODULE_AUTHOR("Kyungmin Park <kyungmin.park@samsung.com>");
MODULE_DESCRIPTION("ISA1200 Haptic Motor driver");
MODULE_LICENSE("GPL");
