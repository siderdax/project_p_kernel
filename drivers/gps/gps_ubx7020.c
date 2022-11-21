#include <linux/init.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/hrtimer.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/sysfs.h>

#include <linux/sec_sysfs.h>

static struct device *gps_dev;
static unsigned int gps_pwr_on = 0;
static unsigned int gps_reset_en = 0;

int check_gps_op(void)
{
	/* This pin is high when gps is working */
	int gps_is_running = gpio_get_value(gps_pwr_on);

	pr_debug("LPA : check_gps_op(%d)\n", gps_is_running);

	return gps_is_running;
}
EXPORT_SYMBOL(check_gps_op);
#if 0
static ssize_t gps_show(struct device *dev, struct device_attribute *attr,
		char *buf) 
{                          
	if (!strcmp(attr->attr.name, "gps_reset_en"))
		return snprintf(buf, 4, "%d\n", gpio_get_value(gps_reset_en));
	else    
		return snprintf(buf, 4, "%d\n", gpio_get_value(gps_pwr_on));
}   

static ssize_t gps_store(struct device *dev, struct device_attribute *attr,
		const char *buff, size_t size)
{                           
	int gpio, gpio_value;

	if (!strcmp(attr->attr.name, "gps_reset_en"))
		gpio = gps_reset_en;
	else    
		gpio = gps_pwr_on;

	if (!sscanf(buff, "%d", &gpio_value))
		return -EINVAL; 

	gpio_direction_output(gpio, 1);
	gpio_set_value(gpio, gpio_value);

	return size;
}

static DEVICE_ATTR(gps_reset_en, S_IRUGO | S_IWUSR | S_IWGRP, gps_show, gps_store);
static DEVICE_ATTR(gps_pwr_on, S_IRUGO | S_IWUSR | S_IWGRP, gps_show, gps_store);
#endif


static int __init gps_ubx7020_init(void)
{
	int ret = 0;
	const char *gps_node = "samsung,exynos7420-ubx7020";

	struct device_node *root_node = NULL;

	gps_dev = sec_device_create(NULL, "gps");
	BUG_ON(!gps_dev);

	root_node = of_find_compatible_node(NULL, NULL, gps_node);
	if (!root_node) {
		WARN(1, "failed to get device node of ubx7020\n");
		ret = -ENODEV;
		goto err_sec_device_create;
	}
	//----------------------------------
	gps_pwr_on = of_get_gpio(root_node, 0);
	if (!gpio_is_valid(gps_pwr_on)) {
		WARN(1, "Invalied gpio pin : %d\n", gps_pwr_on);
		ret = -ENODEV;
		goto err_sec_device_create;
	}

	if (gpio_request(gps_pwr_on, "GPS_PWR_EN")) {
		WARN(1, "fail to request gpio(GPS_PWR_EN)\n");
		ret = -ENODEV;
		goto err_sec_device_create;
	}

	//----------------------------------
	gps_reset_en = of_get_gpio(root_node, 1);
	if (!gpio_is_valid(gps_reset_en)) {
		WARN(1, "Invalied gpio pin : %d\n", gps_reset_en);
		ret = -ENODEV;
		goto err_sec_device_create;
	}

	if (gpio_request(gps_reset_en, "GPS_RESET_EN")) {
		WARN(1, "fail to request gpio(GPS_RESET_EN)\n");
		ret = -ENODEV;
		goto err_sec_device_create;
	}

#if 0
	device_create_file(gps_dev, &dev_attr_gps_reset_en);
	device_create_file(gps_dev, &dev_attr_gps_pwr_on);
#endif
	gpio_direction_output(gps_pwr_on, 1);
	gpio_export(gps_pwr_on, 1);
	gpio_export_link(gps_dev, "GPS_PWR_EN", gps_pwr_on);

	gpio_direction_output(gps_reset_en, 1);
	mdelay(100);
	gpio_direction_output(gps_reset_en, 0);
	mdelay(100);
	gpio_direction_output(gps_reset_en, 1);
	mdelay(100);
	gpio_export(gps_reset_en, 1);
	gpio_export_link(gps_dev, "GPS_RESET_EN", gps_reset_en);

	//----------------------------------

	return 0;

err_sec_device_create:
	sec_device_destroy(gps_dev->devt);
	return ret;
}

device_initcall(gps_ubx7020_init);
