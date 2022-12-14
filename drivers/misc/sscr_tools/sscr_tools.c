#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/rtc.h>

struct delayed_work timestamp_delayed_work;
void timestamp_delayed_work_func(struct work_struct *work)
{
	struct rtc_time tm;
	struct timespec tv;
	struct rtc_device *rtc0 = rtc_class_open("rtc0");

	if (rtc0 == NULL)
	{
		printk("No RTC available! Do not use timestamp!\n");
		return ;
	}
	rtc_read_time(rtc0, &tm);
	rtc_tm_to_time(&tm, &tv.tv_sec);
	printk("Timestamp: mv7420_pre_rtm_1.0.0 %d-%02d-%02d %02d:%02d:%02d UTC (%u)\n",
		tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
		tm.tm_hour, tm.tm_min, tm.tm_sec,
		(unsigned int) tv.tv_sec);
	schedule_delayed_work(&timestamp_delayed_work, 60*HZ);
}

static int exynos_debug_tool_init(void)
{
	INIT_DELAYED_WORK(&timestamp_delayed_work, timestamp_delayed_work_func);
	schedule_delayed_work(&timestamp_delayed_work, 60*HZ);
	return 0;
}

late_initcall(exynos_debug_tool_init);
