/*
 * Samsung Exynos5 SoC series Sensor driver
 *
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <linux/gpio.h>
#include <linux/clk.h>
#include <linux/regulator/consumer.h>
#include <linux/videodev2.h>
#include <linux/videodev2_exynos_camera.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/platform_device.h>
#ifdef CONFIG_OF
#include <linux/of_gpio.h>
#endif
#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-subdev.h>
#include <mach/exynos-fimc-is-sensor.h>

#include "fimc-is-hw.h"
#include "fimc-is-core.h"
#include "fimc-is-device-sensor.h"
#include "fimc-is-resourcemgr.h"
#include "fimc-is-dt.h"
#include "fimc-is-device-4h5.h"

#define SENSOR_NAME "S5K4H5"
#if 1
static struct fimc_is_sensor_cfg config_4h5[] = {
	/*(weight, height, frame rate,settle value, index) Setting info for fw*/
        FIMC_IS_SENSOR_CFG(3264, 2448, 15, 15,4),
	 FIMC_IS_SENSOR_CFG(2884, 2162, 15, 15,4),
	 FIMC_IS_SENSOR_CFG(3236, 2426, 15, 15,4),
	 FIMC_IS_SENSOR_CFG(3236,1820, 15, 15,5),
	 FIMC_IS_SENSOR_CFG(1604, 902, 30, 15,6),
	 FIMC_IS_SENSOR_CFG(1620, 1080, 30, 15,7),
	 FIMC_IS_SENSOR_CFG(1412, 1154, 30, 15,8)

};
#endif
#if 1
static struct fimc_is_vci vci_4h5[] = {
	{
		.pixelformat = V4L2_PIX_FMT_SBGGR10,
		.config = {{0, HW_FORMAT_RAW10}, {1, HW_FORMAT_UNKNOWN}, {2, HW_FORMAT_UNKNOWN}, {3, 0}}
	}, {
		.pixelformat = V4L2_PIX_FMT_SBGGR12,
		.config = {{0, HW_FORMAT_RAW10}, {1, HW_FORMAT_UNKNOWN}, {2, HW_FORMAT_UNKNOWN}, {3, 0}}
	}, {
		.pixelformat = V4L2_PIX_FMT_SBGGR16,
		.config = {{0, HW_FORMAT_RAW10}, {1, HW_FORMAT_UNKNOWN}, {2, HW_FORMAT_UNKNOWN}, {3, 0}}
	}
};
#endif

static int sensor_4h5_init(struct v4l2_subdev *subdev, u32 val)
{
	int ret = 0;
	struct fimc_is_module_enum *module;

	BUG_ON(!subdev);

	module = (struct fimc_is_module_enum *)v4l2_get_subdevdata(subdev);
	if (unlikely(!module)) {
		printk("IN 4h5 driver module is NULL\n");
		ret = -EINVAL;
		goto p_err;
	}

	pr_info("[MOD:D:%d] %s(%d)\n", module->sensor_id, __func__, val);

p_err:
	return ret;
}

static const struct v4l2_subdev_core_ops core_ops = {
	.init = sensor_4h5_init
};

static const struct v4l2_subdev_ops subdev_ops = {
	.core = &core_ops
};

#ifdef CONFIG_OF
static int sensor_4h5_power_setpin(struct platform_device *pdev,
		struct exynos_platform_fimc_is_module *pdata)
{
	struct device *dev;
	struct device_node *dnode;
	int  gpio_func = 0;
	int  gpio_reset = 0, gpio_standby = 0;
	int  gpio_af_2v8en = 0;
	int  gpio_cam_2v8en = 0; 
	int  gpio_cam_1v8en = 0;
	int  gpio_cam_1v2en = 0;
	int gpio_none = 0;


	BUG_ON(!pdev);

	dev = &pdev->dev;
	dnode = dev->of_node;
	printk("Sensor 4H5 sensor_4h5_power_setpin  IN !!!\n");
	gpio_reset = of_get_named_gpio(dnode, "gpio_reset", 0);
	if (!gpio_is_valid(gpio_reset)) {
		dev_err(dev, "failed to get PIN_RESET\n");
		return -EINVAL;
	} else {
		gpio_request_one(gpio_reset, GPIOF_OUT_INIT_LOW, "CAM_GPIO_OUTPUT_LOW");
		gpio_free(gpio_reset);
	}

	gpio_standby = of_get_named_gpio(dnode, "gpio_standby", 0);
	if (!gpio_is_valid(gpio_standby)) {
		dev_err(dev, "failed to get gpio_standby\n");
	} else {
		gpio_request_one(gpio_standby, GPIOF_OUT_INIT_LOW, "CAM_GPIO_OUTPUT_LOW");
		gpio_free(gpio_standby);
	}
	
	/*Get the power control pin*/
	gpio_af_2v8en = of_get_named_gpio(dnode, "gpio_af_2v8en", 0);
	gpio_cam_2v8en = of_get_named_gpio(dnode, "gpio_cam_2v8en", 0);
	gpio_cam_1v8en = of_get_named_gpio(dnode, "gpio_cam_1v8en", 0);
	gpio_cam_1v2en = of_get_named_gpio(dnode, "gpio_cam_1v2en", 0);
	gpio_func = of_get_named_gpio(dnode, "pinctl", 0);//I2C&CLK

	SET_PIN_INIT(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON);
	SET_PIN_INIT(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF);

	SET_PIN_INIT(pdata, SENSOR_SCENARIO_VISION, GPIO_SCENARIO_ON);
	SET_PIN_INIT(pdata, SENSOR_SCENARIO_VISION, GPIO_SCENARIO_OFF);
	
	/*Set the func gpio(I2C&clock)  use the default setting*/
	//SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_func, "pinctl", PIN_FUNCTION, 1, 0);
	
	/* BACK CAMERA - POWER ON (SET THE POWER PIN HIGH TO ENABLE)*/
       SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_af_2v8en, "gpio_af_2v8en", PIN_OUTPUT, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_cam_2v8en, "gpio_cam_2v8en", PIN_OUTPUT, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_cam_1v8en, "gpio_cam_1v8en", PIN_OUTPUT, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_cam_1v2en, "gpio_cam_1v2en", PIN_OUTPUT, 1, 3000);//here we need add  delay for reset pih.lzy

	
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_reset, "sen_rst high", PIN_OUTPUT, 1, 0);

        SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_reset, "sen_rst low", PIN_OUTPUT, 0, 0);
        SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_none, "VDDA28_CAMSEN", PIN_REGULATOR, 1, 0);
        SET_PIN_VOLTAGE(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_none, "VDD28_CAMAF", PIN_REGULATOR, 1, 0, 2800000);
        SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_none, "VDD18_CAMIO", PIN_REGULATOR, 1, 0);
        SET_PIN_VOLTAGE(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_none, "VDD12_CAMCORE", PIN_REGULATOR, 1, 0, 1200000);
        SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_none, "pin", PIN_FUNCTION, 1, 0);
        SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_reset, "sen_rst high", PIN_OUTPUT, 1, 0);


        SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_reset, "sen_rst", PIN_RESET, 0, 0);
        SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_reset, "sen_rst input", PIN_INPUT, 0 ,0);
        SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_none, "VDDA28_CAMSEN", PIN_REGULATOR, 0, 0);
        SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_none, "VDD28_CAMAF", PIN_REGULATOR, 0, 0);
        SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_none, "VDD18_CAMIO", PIN_REGULATOR, 0, 0);
        SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_none, "VDD12_CAMCORE", PIN_REGULATOR, 0, 0);
        SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_none, "pin", PIN_FUNCTION, 0, 0);


	dev_info(dev, "%s exit here\n", __func__);

	return 0;
}
#endif


int sensor_4h5_probe(struct platform_device *pdev)
{
	int ret = 0;

	struct fimc_is_core *core;
	struct v4l2_subdev *subdev_module;
	static bool probe_retried = false;

	struct fimc_is_module_enum *module;
	struct fimc_is_device_sensor *device;
	struct sensor_open_extended *ext;
	struct exynos_platform_fimc_is_module *pdata;
	struct device *dev;


	if (!fimc_is_dev)
		goto probe_defer;

	core = (struct fimc_is_core *)dev_get_drvdata(fimc_is_dev);
	if (!core) {
		printk("core device is not yet probed");
		return -EPROBE_DEFER;
	}

	dev = &pdev->dev;


#ifdef CONFIG_OF
	fimc_is_sensor_module_parse_dt(pdev, sensor_4h5_power_setpin);
	printk("4H5 ==>fimc_is_sensor_module_parse_dt finished \n");
#endif


	pdata = dev_get_platdata(dev);

	device = &core->sensor[pdata->id];

	subdev_module = kzalloc(sizeof(struct v4l2_subdev), GFP_KERNEL);
	if (!subdev_module) {
		printk("4h5 subdev_module is NULL");
		ret = -ENOMEM;
		goto p_err;
	}
	
	
	module = &device->module_enum[atomic_read(&core->resourcemgr.rsccount_module)];
	atomic_inc(&core->resourcemgr.rsccount_module);
	clear_bit(FIMC_IS_MODULE_GPIO_ON, &module->state);
	module->pdata = pdata;
	module->pdev = pdev;
	module->sensor_id = SENSOR_NAME_S5K4H5;
	module->subdev = subdev_module;
	module->device = pdata->id;
	module->client = NULL;
	module->active_width = 3264;
	module->active_height = 2448;
	module->pixel_width = module->active_width + 16;
	module->pixel_height = module->active_height + 12;
	module->max_framerate = 30;
	module->position = pdata->position;
	module->mode = CSI_MODE_CH0_ONLY;
	module->lanes = CSI_DATA_LANES_4;
	module->vcis = ARRAY_SIZE(vci_4h5);
	module->vci = vci_4h5;
	module->sensor_maker = "SLSI";
	module->sensor_name = "S5K4H5";
	module->setfile_name = "setfile_4h5.bin";
	module->cfgs = ARRAY_SIZE(config_4h5);
	module->cfg = config_4h5;
	module->ops = NULL;
	module->private_data = NULL;

	ext = &module->ext;
	ext->mipi_lane_num = module->lanes;
	ext->I2CSclk = I2C_L0;

	ext->sensor_con.product_name = SENSOR_NAME_S5K4H5;
	ext->sensor_con.peri_type = SE_I2C;
	ext->sensor_con.peri_setting.i2c.channel = pdata->sensor_i2c_ch;
	ext->sensor_con.peri_setting.i2c.slave_address = pdata->sensor_i2c_addr;
	ext->sensor_con.peri_setting.i2c.speed = 400000;
	printk("4h5 set ext->sensor_con.peri_setting.i2c.channel= %d,pdata->sensor_i2c_addr=%d  !!!\n",ext->sensor_con.peri_setting.i2c.channel,pdata->sensor_i2c_addr);
#if 1
	ext->actuator_con.product_name = ACTUATOR_NAME_NOTHING;//ACTUATOR_NAME_DW9718;
//	ext->actuator_con.peri_type = SE_I2C;
//	ext->actuator_con.peri_setting.i2c.channel
//		= SENSOR_CONTROL_I2C0;
#endif

	ext->flash_con.product_name = FLADRV_NAME_NOTHING;

	ext->from_con.product_name = FROMDRV_NAME_NOTHING;

	ext->companion_con.product_name = COMPANION_NAME_NOTHING;
	/*
	if (client) {
		v4l2_i2c_subdev_init(subdev_module, client, &subdev_ops);
	} else {
		v4l2_subdev_init(subdev_module, &subdev_ops);
	}
	*/
	v4l2_subdev_init(subdev_module, &subdev_ops);
	v4l2_set_subdevdata(subdev_module, module);
	v4l2_set_subdev_hostdata(subdev_module, device);
	snprintf(subdev_module->name, V4L2_SUBDEV_NAME_SIZE, "sensor-subdev.%d", module->sensor_id);
	

p_err:

	printk("Here we are probe finished :%s(%d)\n", __func__, ret);
	return ret;


probe_defer:
	if (probe_retried) {
		printk("probe has already been retried!!!");
		BUG();
	}

	probe_retried = true;
	printk("Fimc-is core device is not yet probed");
	return -EPROBE_DEFER;

}

static int sensor_4h5_remove(struct platform_device *pdev)
{
	int ret = 0;
	return ret;
}

#ifdef CONFIG_OF
static const struct of_device_id exynos_fimc_is_sensor_4h5_match[] = {
	{
		.compatible = "samsung,exynos5-fimc-is-sensor-4h5",
	},
	{},
};
#endif
MODULE_DEVICE_TABLE(of, exynos_fimc_is_sensor_4h5_match);

static struct platform_driver sensor_4h5_driver = {
	.driver = {
		.name	= SENSOR_NAME,
		.owner	= THIS_MODULE,
#ifdef CONFIG_OF
		.of_match_table = exynos_fimc_is_sensor_4h5_match
#endif
	},
	.probe	= sensor_4h5_probe,
	.remove	= sensor_4h5_remove,
};

module_platform_driver(sensor_4h5_driver);

static int __init fimc_is_sensor_module_init(void)
{
	int ret = 0;
	ret = platform_driver_register(&sensor_4h5_driver);
	
	if (ret)
		err("platform_driver_register failed: %d\n", ret);

	return ret;
}

static void __exit fimc_is_sensor_module_exit(void)
{
	platform_driver_unregister(&sensor_4h5_driver);
}
module_init(fimc_is_sensor_module_init);
module_exit(fimc_is_sensor_module_exit);

MODULE_AUTHOR("L");
MODULE_DESCRIPTION("Sensor 4h5 driver");
MODULE_LICENSE("GPL v2");
