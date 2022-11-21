/* drivers/devfreq/exynos7420_ppmu.c
 *
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
 *              http://www.samsung.com/
 *
 * EXYNOS7420 - PPMU control.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/workqueue.h>
#include <linux/platform_device.h>

#include "exynos7420_ppmu.h"
#include "exynos_ppmu_fw.h"

#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
//#include <mach/exynos5_bus.h>
#include <mach/map.h>

#include <asm/uaccess.h>
#include <linux/debugfs.h>

/*#define PPMU_DEBUG*/
#define FIXED_POINT_OFFSET 8
#define FIXED_POINT_MASK ((1 << FIXED_POINT_OFFSET) - 1)

#define MIF_DEFAULT_PPMU_WEIGHT	100

static struct workqueue_struct *exynos_ppmu_wq;
static struct delayed_work exynos_ppmu_work;
#ifdef CONFIG_HYBRID_INVOKING
static struct delayed_work exynos_ppmu_work_df;
#endif
static unsigned long exynos_ppmu_polling_period;
static unsigned long long mif_weight = MIF_DEFAULT_PPMU_WEIGHT;

static DEFINE_MUTEX(exynos_ppmu_lock);
static LIST_HEAD(exynos_ppmu_list);

static struct srcu_notifier_head exynos_ppmu_notifier_list[DEVFREQ_TYPE_COUNT];

static struct ppmu_info ppmu[PPMU_COUNT] = {
	[PPMU0_0_GEN_RT] = {
		.base = (void __iomem *)PPMU0_0_GEN_RT_ADDR,
	},
	[PPMU0_1_CPU] = {
		.base = (void __iomem *)PPMU0_1_CPU_ADDR,
	},
	[PPMU1_0_GEN_RT] = {
		.base = (void __iomem *)PPMU1_0_GEN_RT_ADDR,
	},
	[PPMU1_1_CPU] = {
		.base = (void __iomem *)PPMU1_1_CPU_ADDR,
	},
	[PPMU2_0_GEN_RT] = {
		.base = (void __iomem *)PPMU2_0_GEN_RT_ADDR,
	},
	[PPMU2_1_CPU] = {
		.base = (void __iomem *)PPMU2_1_CPU_ADDR,
	},
	[PPMU3_0_GEN_RT] = {
		.base = (void __iomem *)PPMU3_0_GEN_RT_ADDR,
	},
	[PPMU3_1_CPU] = {
		.base = (void __iomem *)PPMU3_1_CPU_ADDR,
	},
};

static int exynos7420_ppmu_notifier_list_init(void)
{
	int i;

	for (i = 0; i < DEVFREQ_TYPE_COUNT; ++i)
		srcu_init_notifier_head(&exynos_ppmu_notifier_list[i]);

	return 0;
}

int exynos7420_devfreq_init(struct devfreq_exynos *de)
{
	INIT_LIST_HEAD(&de->node);

	return 0;
}

int exynos7420_devfreq_register(struct devfreq_exynos *de)
{
	int i;

	for (i = 0; i < de->ppmu_count; ++i) {
		if (ppmu_init(&de->ppmu_list[i]))
			return -EINVAL;
	}

	mutex_lock(&exynos_ppmu_lock);
	list_add_tail(&de->node, &exynos_ppmu_list);
	mutex_unlock(&exynos_ppmu_lock);

	return 0;
}

int exynos7420_ppmu_register_notifier(enum DEVFREQ_TYPE type, struct notifier_block *nb)
{
	return srcu_notifier_chain_register(&exynos_ppmu_notifier_list[type], nb);
}

int exynos7420_ppmu_unregister_notifier(enum DEVFREQ_TYPE type, struct notifier_block *nb)
{
	return srcu_notifier_chain_unregister(&exynos_ppmu_notifier_list[type], nb);
}

#ifdef CONFIG_HYBRID_INVOKING
static int exynos7420_ppmu_notify(enum DEVFREQ_TYPE type, int * data)
{
	BUG_ON(irqs_disabled());

	return srcu_notifier_call_chain(&exynos_ppmu_notifier_list[type], 0, (void *) data);
}
#else // CONFIG_HYBRID_INVOKING
static int exynos7420_ppmu_notify(enum DEVFREQ_TYPE type)
{
	BUG_ON(irqs_disabled());

	return srcu_notifier_call_chain(&exynos_ppmu_notifier_list[type], 0, NULL);
}
#endif // CONFIG_HYBRID_INVOKING

static void exynos7420_update_polling(unsigned int period)
{
	exynos_ppmu_polling_period = period;

	cancel_delayed_work_sync(&exynos_ppmu_work);
#ifdef CONFIG_HYBRID_INVOKING
	cancel_delayed_work_sync(&exynos_ppmu_work_df);
#endif

	if (period == 0)
		return;

	queue_delayed_work(exynos_ppmu_wq, &exynos_ppmu_work,
			msecs_to_jiffies(period));
}

static int exynos7420_ppmu_mif_calculate(struct ppmu_info *ppmu,
						unsigned int size,
						unsigned long long *ccnt,
						unsigned long long *pmcnt)
{
	unsigned int i;
	unsigned long long val_ccnt = 0;
	unsigned long long val_pmcnt0 = 0;
	unsigned long long val_pmcnt1 = 0;
	unsigned long long val_pmcnt3 = 0;
	unsigned long long ppmu_n = 0;

	*pmcnt = 0;
	for (i = 0; i < size; ++i) {
		if (ppmu_count(ppmu + i, &val_ccnt, &val_pmcnt0, &val_pmcnt1, &val_pmcnt3))
			return -EINVAL;

		if (*ccnt < val_ccnt)
			*ccnt = val_ccnt;

		val_pmcnt3 = div_u64(val_pmcnt3 * mif_weight, 100);

		if (i % 2 == 0) {
			ppmu_n = val_pmcnt3;

			continue;
		} else {
			ppmu_n += val_pmcnt3;

			*pmcnt = max(*pmcnt, ppmu_n);
		}
	}

	return 0;
}

static int exynos7420_ppmu_int_calculatate(struct ppmu_info *ppmu,
						unsigned int size,
						unsigned long long *ccnt,
						unsigned long long *pmcnt)
{
	unsigned int i;
	unsigned long long val_ccnt = 0;
	unsigned long long val_pmcnt0 = 0;
	unsigned long long val_pmcnt1 = 0;
	unsigned long long val_pmcnt3 = 0;

	for (i = 0; i < size; ++i) {
		if (ppmu_count(ppmu + i, &val_ccnt, &val_pmcnt0, &val_pmcnt1, &val_pmcnt3))
			return -EINVAL;

		if (*ccnt < val_ccnt)
			*ccnt = val_ccnt;

		*pmcnt = max3(*pmcnt, val_pmcnt0, val_pmcnt1);
	}

	return 0;
}

static void exynos7420_ppmu_update(void)
{
	struct devfreq_exynos *devfreq;
	pfn_ppmu_count pfn_count;

	/* before getting ppmu count, it first should stop ppmu */
	if (ppmu_count_stop(ppmu,
				ARRAY_SIZE(ppmu))) {
		pr_err("DEVFREQ(PPMU) : ppmu can't stop\n");
		return;
	}

	list_for_each_entry(devfreq, &exynos_ppmu_list, node) {
		switch (devfreq->type) {
		case MIF:
			pfn_count = exynos7420_ppmu_mif_calculate;
			break;
		case INT:
			pfn_count = exynos7420_ppmu_int_calculatate;
			break;
		default:
			pfn_count = NULL;
			break;
		}

		if (ppmu_count_total(devfreq->ppmu_list,
					devfreq->ppmu_count,
					pfn_count,
					&devfreq->val_ccnt,
					&devfreq->val_pmcnt)) {
			pr_err("DEVFREQ(PPMU) : ppmu can't update data\n");
			continue;
		}
	}
}

int exynos7420_ppmu_activate(void)
{
	int i;

	mutex_lock(&exynos_ppmu_lock);
	for (i = 0; i < PPMU_COUNT; ++i) {
		if (ppmu_init(&ppmu[i])) {
			mutex_unlock(&exynos_ppmu_lock);
			goto err;
		}

		if (ppmu_init_fw(&ppmu[i])) {
			mutex_unlock(&exynos_ppmu_lock);
			goto err;
		}

		if (ppmu_reset(&ppmu[i])) {
			mutex_unlock(&exynos_ppmu_lock);
			goto err;
		}
	}
	mutex_unlock(&exynos_ppmu_lock);

	exynos7420_update_polling(100);
	return 0;

err:
	for (; i >= 0; --i)
		ppmu_term(&ppmu[i]);

	return -EINVAL;
}

int exynos7420_ppmu_deactivate(void)
{
	int i;

	exynos7420_update_polling(0);

	mutex_lock(&exynos_ppmu_lock);
	for (i = 0; i < PPMU_COUNT; ++i) {
		if (ppmu_disable(&ppmu[i])) {
			mutex_unlock(&exynos_ppmu_lock);
			goto err;
		}
	}

	mutex_unlock(&exynos_ppmu_lock);

	return 0;

err:
	pr_err("DEVFREQ(PPMU) : can't deactivate counter\n");
	return -EINVAL;
}

static int exynos7420_ppmu_reset(void)
{
	if (ppmu_reset_total(ppmu,
			ARRAY_SIZE(ppmu))) {
		pr_err("DEVFREQ(PPMU) : ppmu can't reset data\n");
		return -EAGAIN;
	}

	return 0;
}

static void exynos7420_monitor(struct work_struct *work)
{
	int i;
#ifdef CONFIG_HYBRID_INVOKING
	int deferred= 1;
	int noti_ret = 1;
#ifdef CONFIG_NONUNIFORM_DELAY
	unsigned int NNF_polling_period = COARSE_SAMPLING;
#endif
#endif

	mutex_lock(&exynos_ppmu_lock);

	exynos7420_ppmu_update();

	for (i = 0; i < DEVFREQ_TYPE_COUNT; ++i) {
#ifdef CONFIG_HYBRID_INVOKING
		if(exynos7420_ppmu_notify(i, &noti_ret) != NOTIFY_OK)
			noti_ret = 1;
		
#ifdef CONFIG_NONUNIFORM_DELAY
			if(NNF_polling_period >= NORMAL_SAMPLING &&
				(noti_ret & MASK_SAMPLE_CODE) == FINE_SAMPLE_CODE) {
				NNF_polling_period = FINE_SAMPLING;
			}
			else if(NNF_polling_period >= COARSE_SAMPLING &&
				(noti_ret & MASK_SAMPLE_CODE) == NORMAL_SAMPLE_CODE) {
				NNF_polling_period = NORMAL_SAMPLING;
			}
#endif // CONFIG_NONUNIFORM_DELAY
			deferred = deferred & noti_ret;
			noti_ret = 1;
#else	// CONFIG_HYBRID_INVOKING
			exynos7420_ppmu_notify(i);
#endif	// CONFIG_HYBRID_INVOKING
	}

	exynos7420_ppmu_reset();

	mutex_unlock(&exynos_ppmu_lock);

#ifdef CONFIG_HYBRID_INVOKING
#ifdef CONFIG_NONUNIFORM_DELAY
	exynos_ppmu_polling_period = NNF_polling_period;
#endif
	if(deferred) {
		queue_delayed_work(exynos_ppmu_wq, &exynos_ppmu_work_df,
				msecs_to_jiffies(exynos_ppmu_polling_period));
	}
	else {
		queue_delayed_work(exynos_ppmu_wq, &exynos_ppmu_work,
				msecs_to_jiffies(exynos_ppmu_polling_period));
	}
#else
	queue_delayed_work(exynos_ppmu_wq, &exynos_ppmu_work,
			msecs_to_jiffies(exynos_ppmu_polling_period));
#endif

}

static int exynos7420_ppmu_probe(struct platform_device *pdev)
{
	int ret = 0;
	exynos_ppmu_wq = create_freezable_workqueue("exynos7420_ppmu_wq");

#if defined(CONFIG_DEFERRABLE_INVOKING)
		INIT_DEFERRABLE_WORK(&exynos_ppmu_work, exynos7420_monitor);
#elif defined(CONFIG_HYBRID_INVOKING)
		INIT_DEFERRABLE_WORK(&exynos_ppmu_work_df, exynos7420_monitor);
		INIT_DELAYED_WORK(&exynos_ppmu_work, exynos7420_monitor);
#else
		INIT_DELAYED_WORK(&exynos_ppmu_work, exynos7420_monitor);
#endif

	ret = exynos7420_ppmu_activate();

	return ret;
}

static int exynos7420_ppmu_remove(struct platform_device *pdev)
{
	exynos7420_ppmu_deactivate();
	flush_workqueue(exynos_ppmu_wq);
	destroy_workqueue(exynos_ppmu_wq);

	return 0;
}

static int exynos7420_ppmu_suspend(struct device *dev)
{
	exynos7420_ppmu_deactivate();

	return 0;
}

static int exynos7420_ppmu_resume(struct device *dev)
{
	int i;

	mutex_lock(&exynos_ppmu_lock);
	for (i = 0; i < PPMU_COUNT; ++i) {
		if (ppmu_init_fw(&ppmu[i])) {
			mutex_unlock(&exynos_ppmu_lock);
			goto err;
		}
	}
	exynos7420_ppmu_reset();
	mutex_unlock(&exynos_ppmu_lock);

	exynos7420_update_polling(100);

	return 0;
err:
	pr_err("DEVFREQ(PPMU) : ppmu can't resume.\n");

	return -EINVAL;
}

static struct dev_pm_ops exynos7420_ppmu_pm = {
	.suspend	= exynos7420_ppmu_suspend,
	.resume		= exynos7420_ppmu_resume,
};

static struct platform_driver exynos7420_ppmu_driver = {
	.probe	= exynos7420_ppmu_probe,
	.remove	= exynos7420_ppmu_remove,
	.driver	= {
		.name	= "exynos7420-ppmu",
		.owner	= THIS_MODULE,
		.pm	= &exynos7420_ppmu_pm,
	},
};

static struct platform_device exynos7420_ppmu_device = {
	.name	= "exynos7420-ppmu",
	.id	= -1,
};

static int __init exynos7420_ppmu_early_init(void)
{
	return exynos7420_ppmu_notifier_list_init();
}
arch_initcall_sync(exynos7420_ppmu_early_init);

static int __init exynos7420_ppmu_init(void)
{
	int ret;

	ret = platform_device_register(&exynos7420_ppmu_device);
	if (ret)
		return ret;

	return platform_driver_register(&exynos7420_ppmu_driver);
}
late_initcall_sync(exynos7420_ppmu_init);

static void __exit exynos7420_ppmu_exit(void)
{
	platform_driver_unregister(&exynos7420_ppmu_driver);
	platform_device_unregister(&exynos7420_ppmu_device);
}
module_exit(exynos7420_ppmu_exit);

#ifdef CONFIG_PPMU_DEBUGFS_INTERFACE
static int exynos7420_ppmu_debug_show(struct seq_file *s, void *d)
{
	
	struct devfreq_exynos *devfreq;

	unsigned long long busy;
	unsigned long long total;
	unsigned long long ratio;


	list_for_each_entry(devfreq, &exynos_ppmu_list, node) {
		
		total = devfreq->val_ccnt;
		busy = devfreq->val_pmcnt;
		ratio = (u64)busy * 100 * (1 << FIXED_POINT_OFFSET);
		ratio += total / 2;
		do_div(ratio, total);


		seq_printf(s, "%-10s %-10s %4llu %04llu MBps\n %2llu.%02llu%% \n", "total", "read+write",
			devfreq->val_ccnt,
			devfreq->val_pmcnt,
			//ratio, ratio);
			ratio >> FIXED_POINT_OFFSET,
			((ratio & FIXED_POINT_MASK) * 100 / (1 << FIXED_POINT_OFFSET)));
	}
	seq_printf(s, "\n");

	return 0;
}


static int exynos7420_ppmu_debug_open(struct inode *inode, struct file *file)
{
	return single_open(file, exynos7420_ppmu_debug_show, inode->i_private);
}



#ifdef PPMU_DEBUG
static int exynos7420_ppmu_debug_write(struct file *file, 
	const char __user *buf, size_t size, loff_t *ppos )
{
	char info[255];

	memset(info, 0, 255);
	if(copy_from_user(info, buf, size))
		return -EINVAL;

	printk("%s:",__func__);
	if (info[0] == 'r'){
		/* fix me */
		printk("read");
		
	} else if(info[0] == 's'){
		/* fix me */
		printk("start");
	} else
		printk("Unkown command %c", info[0]);

	printk(" %s\n", &info[2]);
	
	return size;
}
#endif
const static struct file_operations exynos7420_ppmu_debug_fops = {
	.open		= exynos7420_ppmu_debug_open,
	.read		= seq_read,
#ifdef PPMU_DEBUG
	.write		= exynos7420_ppmu_debug_write,
#endif
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int __init exynos7420_ppmu_debug_init(void)
{
	debugfs_create_file("exynos7420_bus", S_IRUGO, NULL, NULL,
		&exynos7420_ppmu_debug_fops);
	return 0;
}
late_initcall(exynos7420_ppmu_debug_init);

#endif /*CONFIG_PPMU_DEBUGFS_INTERFACE*/

