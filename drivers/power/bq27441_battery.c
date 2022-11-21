/*
 * BQ27441 battery driver
 *
 * Copyright (C) 2008 Rodolfo Giometti <giometti@linux.it>
 * Copyright (C) 2008 Eurotech S.p.A. <info@eurotech.it>
 * Copyright (C) 2010-2011 Lars-Peter Clausen <lars@metafoo.de>
 * Copyright (C) 2011 Pali Roh谩r <pali.rohar@gmail.com>
 *
 * Based on a previous work by Copyright (C) 2008 Texas Instruments, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */


#include <linux/module.h>
#include <linux/param.h>
#include <linux/jiffies.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/power_supply.h>
#include <linux/idr.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <asm/unaligned.h>
#include <linux/of.h>
#include <linux/of_gpio.h>


#define BQ27441_DBG

#ifdef bq27441_DBG
//#define bq27441_print(fmt, args...) printk(fmt, ##args)
#define bq27441_print(fmt, ...) do { if (bq27441_dbg) printk("bq27441: "fmt, ##__VA_ARGS__); } while(0)
#else
//#define bq27441_print(fmt, args...)
#define bq27441_print(fmt, ...)
#endif

#define DRIVER_VERSION			"1.0.0"

#define bq27421CMD_CNTL_LSB           0x00
#define bq27421CMD_CNTL_MSB           0x01
#define bq27421CMD_TEMP_LSB           0x02
#define bq27421CMD_TEMP_MSB           0x03
#define bq27421CMD_VOLT_LSB           0x04
#define bq27421CMD_VOLT_MSB           0x05
#define bq27421CMD_FLAG_LSB           0x06
#define bq27421CMD_FLAG_MSB           0x07
#define bq27421CMD_NAC_LSB            0x08
#define bq27421CMD_NAC_MSB            0x09
#define bq27421CMD_FAC_LSB            0x0a
#define bq27421CMD_FAC_MSB            0x0b
#define bq27421CMD_RM_LSB             0x0c
#define bq27421CMD_RM_MSB             0x0d
#define bq27421CMD_FCC_LSB            0x0e
#define bq27421CMD_FCC_MSB            0x0f
#define bq27421CMD_AI_LSB             0x10
#define bq27421CMD_AI_MSB             0x11
#define bq27421CMD_SI_LSB             0x12
#define bq27421CMD_SI_MSB             0x13
#define bq27421CMD_MLI_LSB            0x14
#define bq27421CMD_MLI_MSB            0x15
#define bq27421CMD_AP_LSB             0x18
#define bq27421CMD_AP_MSB             0x19
#define bq27421CMD_SOC_LSB            0x1c
#define bq27421CMD_SOC_MSB            0x1d
#define bq27421CMD_ITEMP_LSB          0x1e
#define bq27421CMD_ITEMP_MSB          0x1f
#define bq27421CMD_SOH_LSB            0x20
#define bq27421CMD_SOH_MSB            0x21

#define I2CSLAVEADDR         0x55           // 7-bit slave address
#define BUFFERSIZE             32           // # of bytes for Tx & Rx buffers


//subclass 82
unsigned short const Qmax={0x0040};                   //Qmax=16384,after learning this parameter can update. ref the FAE's para
unsigned short const terminatevoltage={0x7A0D};       //terminate voltage=3450mV,system shutdown voltage. ref the FAE's para
unsigned short const loadselect={0x81};               //load select/load mode; 0x81 power-mode 0x01 current mode, normally use 0x81 ,if design capacity > 8500mAh change to 0x01
unsigned short const Designcapacity={0x800C};         //Design capacity=3200mAh
unsigned short const DesignEnergy={0x402E};           //Design Energy=11840mWh; Design Energy=Design capacity*3.7 for 4.2V battery,Design Energy=Design capacity*3.8 for 4.35V battery,
unsigned short const Taperrate={0xC800};              //Taper rate=200,taper current=Design Capacity*10/taper rate=220mA; a little higher than charger taper current(~>20mA)
unsigned short const checksum82={0xBC};
//subclass 81
unsigned short const Dsgcurrentthreshold={0x4001};    //Dsg current threshold(num)=500, Dsg current threshold(mAh)=Design capacity*10/Dsg current threshold(num)=80mA
unsigned short const Chgcurrentthreshold={0x4001};    //Chg current threshold(num)=500, Chg current threshold(mAh)=Design capacity*10/Chg current threshold(num)=80mA,must smaller than charger taper current
unsigned short const Quitcurrent={0x8002};            //Quit current threshold(num)=1000,Quit current threshold(mAh)=Design capacity*10/Quit current threshold(num)=40mA
unsigned short const checksum81={0x8B};
//subclass 89
unsigned short const Ra[15] ={0x6600,0x6600,0x6300,0x6B00,0x4800,0x3B00,0x3E00,0x3F00,0x3500,0x2F00,0x3C00,0x4600,0x8C00,0x7101,0x4C02}; //Ra table,after learning these parameters can update
unsigned short const checksum89={0x33};

volatile int s_suspend = 0;

struct bq27441_device_info;
struct bq27441_access_methods {
	int (*read)(struct bq27441_device_info *di, u8 reg, bool single);
};

struct bq27441_reg_cache {
	int temperature;
	int charge_full;
	int full_charge_capacity;
	int remaining_charge_capacity;
	int full_available_capacity;
	int cycle_count;
	int capacity;
	int flags;
};

struct bq27441_device_info {
	struct device 		*dev;
	int			id;

	struct bq27441_reg_cache cache;
	int charge_design_full;

	unsigned long last_update;
	struct delayed_work work;

	struct power_supply	bat;

	struct bq27441_access_methods bus;

	struct mutex lock;
};

static enum power_supply_property bq27441_battery_props[] = {
	POWER_SUPPLY_PROP_STATUS,
	POWER_SUPPLY_PROP_PRESENT,
	POWER_SUPPLY_PROP_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_CURRENT_NOW,
	POWER_SUPPLY_PROP_CAPACITY,
	POWER_SUPPLY_PROP_TEMP,
	POWER_SUPPLY_PROP_TECHNOLOGY,
	POWER_SUPPLY_PROP_CHARGE_NOW,
	POWER_SUPPLY_PROP_CHARGE_FULL_DESIGN,
};

#ifdef CONFIG_CHARGER_BQ24195
extern int bq24195_get_charging_status(void);
#endif
static unsigned int poll_interval = 20;
module_param(poll_interval, uint, 0644);
MODULE_PARM_DESC(poll_interval, "battery poll interval in seconds - " \
				"0 disables polling");

/*
 * Common code for BQ27441 devices
 */

static inline int bq27441_read(struct bq27441_device_info *di, u8 reg,
		bool single)
{
	return di->bus.read(di, reg, single);
}

#if 1
static int bq27441_write_i2c_Abyte(struct bq27441_device_info *di,u8 reg, u8 value)
{
	struct i2c_client *client = to_i2c_client(di->dev);
	int data;

	data =  i2c_smbus_write_byte_data(client, reg, value);
	
	if (data < 0)
		dev_err(&client->dev, "I2C Write Abyte error REG: %x DATA: 0x%.2x return: %d\n", reg, value, data);
	return data;
	
}

static int bq27441_write_i2c_word(struct bq27441_device_info *di,u8 reg, u16 value)
{
	struct i2c_client *client = to_i2c_client(di->dev);
	int data;

	data =  i2c_smbus_write_word_data(client, reg, value);
	
	if (data < 0)
		dev_err(&client->dev, "I2C Write word error REG: %x DATA: 0x%.2x return: %d\n", reg, value, data);
	return data;
	
}
#if	0
static int bq27441_write_i2c_bytes(struct bq27441_device_info *di,char *buf, int len)
{
	struct i2c_msg msg;	
	struct i2c_client *client = to_i2c_client(di->dev);
	int ret;

	msg.flags = !I2C_M_RD;
	msg.addr  = client->addr;
	msg.len   = len;
	msg.buf   = buf;	

	ret = i2c_transfer(client->adapter, &msg, 1);
	if(ret < 0)
		printk("bq27441_write_i2c_bytes err.ret=%d",ret);

	return ret;
	
}
#endif
static void bq27421_MemoryWrite(struct bq27441_device_info *di,unsigned char offset, unsigned short data)
{
	
	bq27441_write_i2c_word(di,offset,data);

}

#endif


/*
 * Return the battery Relative State-of-Charge
 * Or < 0 if something fails.
 */
static int bq27441_battery_read_rsoc(struct bq27441_device_info *di)
{
	int rsoc;

	rsoc = bq27441_read(di, bq27421CMD_SOC_LSB, false);

	if (rsoc < 0)
		dev_dbg(di->dev, "error reading relative State-of-Charge\n");

	return rsoc;
}

/*
 * Return the battery Nominal available capaciy in 碌Ah
 * Or < 0 if something fails.
 */
static inline int bq27441_battery_read_nac(struct bq27441_device_info *di)
{
	return bq27441_read(di, bq27421CMD_NAC_LSB, false);
}


/*
 * Return the battery temperature in tenths of degree Celsius
 * Or < 0 if something fails.
 */
static int bq27441_battery_read_temperature(struct bq27441_device_info *di)
{
	int temp;

	temp = bq27441_read(di, bq27421CMD_TEMP_LSB, false);
	if (temp < 0) {
		dev_err(di->dev, "error reading temperature\n");
		return temp;
	}

	temp -= 2731;

	return temp;
}

static unsigned char bq27421_check_ITPOR(struct bq27441_device_info *di)
{
	return (bq27441_read(di, bq27421CMD_FLAG_LSB, 1)&0x20);
}

void bq27421_CONFIG_subclass82(struct bq27441_device_info *di)//基本信息设置
{
	bq27441_write_i2c_Abyte(di,0x3e,0x52);//选择0x52区域
	bq27441_write_i2c_Abyte(di,0x3f,0x00);
	
	bq27441_write_i2c_Abyte(di,0x40,0x40);
	bq27441_write_i2c_Abyte(di,0x41,0x00);
	bq27441_write_i2c_Abyte(di,0x42,0x00);
	bq27441_write_i2c_Abyte(di,0x43,0x00);	
	
	bq27421_MemoryWrite(di,0x40,Qmax);
	bq27441_write_i2c_Abyte(di,0x42,0x00);
	bq27421_MemoryWrite(di,0x50,terminatevoltage);
	bq27441_write_i2c_Abyte(di,0x45,loadselect);
	bq27421_MemoryWrite(di,0x4A,Designcapacity);
	bq27421_MemoryWrite(di,0x4C,DesignEnergy);
	bq27421_MemoryWrite(di,0x4E,0x800C);
	bq27421_MemoryWrite(di,0x5B,Taperrate);

	bq27441_write_i2c_Abyte(di,0x60,checksum82);
}

void bq27421_CONFIG_subclass81(struct bq27441_device_info *di)//充放电阈值设置
{
	bq27441_write_i2c_Abyte(di,0x3e,0x51);//选择0x51区域
	bq27441_write_i2c_Abyte(di,0x3f,0x00);

	bq27441_write_i2c_Abyte(di,0x40,0x01);
	bq27441_write_i2c_Abyte(di,0x41,0x40);
	bq27441_write_i2c_Abyte(di,0x42,0x01);
	bq27441_write_i2c_Abyte(di,0x43,0x40);	
	
	bq27421_MemoryWrite(di,0x40,Dsgcurrentthreshold);
	bq27421_MemoryWrite(di,0x42,Chgcurrentthreshold);
	bq27421_MemoryWrite(di,0x44,Quitcurrent);

	bq27441_write_i2c_Abyte(di,0x60,checksum81);
}

void bq27421_CONFIG_subclass89(struct bq27441_device_info *di)//内阻表设置
{
	unsigned char i;

	bq27441_write_i2c_Abyte(di,0x3e,0x59);//选择0x51区域
	bq27441_write_i2c_Abyte(di,0x3f,0x00);
	bq27441_write_i2c_Abyte(di,0x40,0x00);
	bq27441_write_i2c_Abyte(di,0x41,0x66);
	bq27441_write_i2c_Abyte(di,0x42,0x00);
	bq27441_write_i2c_Abyte(di,0x43,0x66);		
	for(i=0;i<15;i++)
	{
	    bq27421_MemoryWrite(di,0x40+i*2,Ra[i]);
	}

	bq27441_write_i2c_Abyte(di,0x60,checksum89);//写checksum
}

static void bq27421_CONFIG(struct bq27441_device_info *di)
{
	if(bq27421_check_ITPOR(di))
	{
		bq27441_write_i2c_Abyte(di,0x00,0x13);//进入配置模式
		bq27441_write_i2c_Abyte(di,0x01,0x00);
		bq27421_CONFIG_subclass82(di);
		bq27421_CONFIG_subclass81(di);
		bq27421_CONFIG_subclass89(di);
		bq27441_write_i2c_Abyte(di,0x00,0x42);//软复位 退出配置模式
		bq27441_write_i2c_Abyte(di,0x01,0x00);
		bq27441_write_i2c_Abyte(di,0x00,0x00);
		bq27441_write_i2c_Abyte(di,0x01,0x00);
	 	bq27441_write_i2c_Abyte(di,0x00,0x20);//seal
	 	bq27441_write_i2c_Abyte(di,0x01,0x00);
		bq27441_write_i2c_Abyte(di,0x00,0x00);
		bq27441_write_i2c_Abyte(di,0x01,0x00);
	}
	else{
		;

	}
}

static void bq27441_update(struct bq27441_device_info *di)
{
	struct bq27441_reg_cache cache = {0, };

	if(s_suspend) {
		printk("%s skip, suspend\n", __FUNCTION__);
		return;
	}

	cache.flags = bq27421_check_ITPOR(di);

	if (cache.flags >= 0) {
		if (cache.flags) {
			dev_info(di->dev, "battery is not calibrated! ignoring capacity values\n");
			bq27421_CONFIG(di);
			cache.capacity = -ENODATA;
		} else {
			cache.capacity = bq27441_battery_read_rsoc(di);
		}
		cache.temperature = bq27441_battery_read_temperature(di);

		/* We only have to read charge design full once */
		if (di->charge_design_full <= 0)
			di->charge_design_full = bq27441_read(di, bq27421CMD_FAC_LSB, false);	//FullAvailableCapacity
	}

	if (memcmp(&di->cache, &cache, sizeof(cache)) != 0) {
		di->cache = cache;
		power_supply_changed(&di->bat);
	}

	di->last_update = jiffies;
}

static void bq27441_battery_poll(struct work_struct *work)
{
	struct bq27441_device_info *di =
		container_of(work, struct bq27441_device_info, work.work);

	bq27441_update(di);

	if (poll_interval > 0) {
		/* The timer does not have to be accurate. */
		set_timer_slack(&di->work.timer, poll_interval * HZ / 4);
		schedule_delayed_work(&di->work, poll_interval * HZ);
	}
}

/*
 * Return the battery average current in 碌A
 * Note that current can be negative signed as well
 * Or 0 if something fails.
 */
static int bq27441_battery_current(struct bq27441_device_info *di,
	union power_supply_propval *val)
{
	int curr;

	curr = bq27441_read(di, bq27421CMD_AI_LSB, false);
	if (curr < 0) {
		dev_err(di->dev, "error reading current\n");
		return curr;
	}
	
	val->intval = (int)((s16)curr) * 1000;
	
	return 0;
}

static int bq27441_battery_status(struct bq27441_device_info *di,
	union power_supply_propval *val)
{
	int status;

#ifdef CONFIG_CHARGER_BQ24195
	status = bq24195_get_charging_status();
#else
	status = POWER_SUPPLY_STATUS_CHARGING;
#endif
	val->intval = status;

	return 0;
}



/*
 * Return the battery Voltage in milivolts
 * Or < 0 if something fails.
 */
static int bq27441_battery_voltage(struct bq27441_device_info *di,
	union power_supply_propval *val)
{
	int volt;
#if 1 //raymanfeng
	volt = bq27441_read(di, bq27421CMD_VOLT_LSB, false);
	if (volt < 0) {
		dev_err(di->dev, "error reading voltage\n");
		return volt;
	}

	val->intval = volt * 1000;
#else
	volt = 4000;
	val->intval = volt * 1000;
#endif
	return 0;
}

static int bq27441_simple_value(int value,
	union power_supply_propval *val)
{
	if (value < 0)
		return value;

	val->intval = value;

	return 0;
}

#define to_bq27441_device_info(x) container_of((x), \
				struct bq27441_device_info, bat);

static int bq27441_battery_get_property(struct power_supply *psy,
					enum power_supply_property psp,
					union power_supply_propval *val)
{
	int ret = 0;
	struct bq27441_device_info *di = to_bq27441_device_info(psy);

	mutex_lock(&di->lock);
	if (time_is_before_jiffies(di->last_update + 5 * HZ)) {
		cancel_delayed_work_sync(&di->work);
		bq27441_battery_poll(&di->work.work);
	}
	mutex_unlock(&di->lock);

	if (psp != POWER_SUPPLY_PROP_PRESENT && di->cache.flags < 0)
		return -ENODEV;

	switch (psp) {
	case POWER_SUPPLY_PROP_STATUS:
		ret = bq27441_battery_status(di, val);
		break;
	case POWER_SUPPLY_PROP_VOLTAGE_NOW:
		ret = bq27441_battery_voltage(di, val);
		break;
	case POWER_SUPPLY_PROP_PRESENT:
		val->intval = di->cache.flags < 0 ? 0 : 1;
		break;
	case POWER_SUPPLY_PROP_CURRENT_NOW:
		ret = bq27441_battery_current(di, val);
		break;
	case POWER_SUPPLY_PROP_CAPACITY:
		#if 1 //raymanfeng
		ret = bq27441_simple_value(di->cache.capacity, val);
		#else
		val->intval = 50;
		ret = 0;
		#endif
		break;
	case POWER_SUPPLY_PROP_TEMP:
		ret = bq27441_simple_value(di->cache.temperature, val);
		break;
	case POWER_SUPPLY_PROP_TECHNOLOGY:
		val->intval = POWER_SUPPLY_TECHNOLOGY_LION;
		break;
	case POWER_SUPPLY_PROP_CHARGE_NOW:
		ret = bq27441_simple_value(bq27441_battery_read_nac(di), val);
		break;
	case POWER_SUPPLY_PROP_CHARGE_FULL_DESIGN:
		ret = bq27441_simple_value(di->charge_design_full, val);
		break;
	default:
		return -EINVAL;
	}

	return ret;
}

static void bq27441_external_power_changed(struct power_supply *psy)
{
	struct bq27441_device_info *di = to_bq27441_device_info(psy);
	
	cancel_delayed_work_sync(&di->work);
	schedule_delayed_work(&di->work, 0);
}

static int bq27441_powersupply_init(struct bq27441_device_info *di)
{
	int ret;

	di->bat.type = POWER_SUPPLY_TYPE_BATTERY;
	di->bat.properties = bq27441_battery_props;
	di->bat.num_properties = ARRAY_SIZE(bq27441_battery_props);
	di->bat.get_property = bq27441_battery_get_property;
	di->bat.external_power_changed = bq27441_external_power_changed;

	INIT_DELAYED_WORK(&di->work, bq27441_battery_poll);
	mutex_init(&di->lock);

	ret = power_supply_register(di->dev, &di->bat);
	if (ret) {
		dev_err(di->dev, "failed to register battery: %d\n", ret);
		return ret;
	}

	dev_info(di->dev, "support ver. %s enabled\n", DRIVER_VERSION);

	bq27441_update(di);

	return 0;
}

static void bq27441_powersupply_unregister(struct bq27441_device_info *di)
{
	/*
	 * power_supply_unregister call bq27441_battery_get_property which
	 * call bq27441_battery_poll.
	 * Make sure that bq27441_battery_poll will not call
	 * schedule_delayed_work again after unregister (which cause OOPS).
	 */
	poll_interval = 0;

	cancel_delayed_work_sync(&di->work);

	power_supply_unregister(&di->bat);

	mutex_destroy(&di->lock);
}



static DEFINE_MUTEX(battery_mutex);

static int bq27441_read_i2c(struct bq27441_device_info *di, u8 reg, bool single)
{
	struct i2c_client *client = to_i2c_client(di->dev);
	struct i2c_msg msg[2];
	unsigned char data[2];
	int ret;

	if (!client->adapter)
		return -ENODEV;

	msg[0].addr = client->addr;
	msg[0].flags = 0;
	msg[0].buf = &reg;
	msg[0].len = sizeof(reg);
	msg[1].addr = client->addr;
	msg[1].flags = I2C_M_RD;
	msg[1].buf = data;
	if (single)
		msg[1].len = 1;
	else
		msg[1].len = 2;

	ret = i2c_transfer(client->adapter, msg, ARRAY_SIZE(msg));
	if (ret < 0)
		return ret;

	if (!single)
		ret = get_unaligned_le16(data);
	else
		ret = data[0];

	return ret;
}

int bq27441_battery_suspend(struct i2c_client * client, pm_message_t mesg)
{
	printk("%s\n", __FUNCTION__);
	s_suspend = 1;
	return 0;
}

int bq27441_battery_resume(struct i2c_client * client)
{
	printk("%s\n", __FUNCTION__);
	s_suspend = 0;
	return 0;
}


static int bq27441_battery_probe(struct i2c_client *client,
				 const struct i2c_device_id *id)
{
	struct bq27441_device_info *di;
	
	int retval = 0;
	
	printk("bq27441_battery_probe in\n");

	di = kzalloc(sizeof(*di), GFP_KERNEL);
	if (!di) {
		dev_err(&client->dev, "failed to allocate device info data\n");
		retval = -ENOMEM;
		goto batt_failed_2;
	}

	//di->id = num;
	di->dev = &client->dev;
	di->bat.name = "battery";
	di->bus.read = &bq27441_read_i2c;

	if (bq27441_powersupply_init(di))
		goto batt_failed_3;

	i2c_set_clientdata(client, di);
	printk("bq27441_battery_probe out\n");
	return 0;

batt_failed_3:
	kfree(di);
batt_failed_2:

	return retval;
}

static int bq27441_battery_remove(struct i2c_client *client)
{
	struct bq27441_device_info *di = i2c_get_clientdata(client);

	bq27441_powersupply_unregister(di);

	kfree(di->bat.name);

	mutex_lock(&battery_mutex);
	mutex_unlock(&battery_mutex);

	kfree(di);

	return 0;
}

#ifdef CONFIG_OF
static struct of_device_id bq27441_of_match_table[] = {
	{
		.compatible = "ti,bq27441",
	},
	{},
};
MODULE_DEVICE_TABLE(of, bq27441_of_match_table);
#endif

#define bq27441_I2C_NAME "bq27441_i2c"// TODO

static const struct i2c_device_id bq27441_id[] = {
	{ bq27441_I2C_NAME, 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, bq27441_id);

static struct i2c_driver bq27441_battery_driver = {
	.driver = {
		.name	= bq27441_I2C_NAME,
		.owner	= THIS_MODULE,
#ifdef CONFIG_OF
		.of_match_table = bq27441_of_match_table,
#endif
	},
	.probe = bq27441_battery_probe,
	.remove = bq27441_battery_remove,
	.suspend = bq27441_battery_suspend,
	.resume = bq27441_battery_resume,

	.id_table = bq27441_id,
};

static inline int bq27441_battery_i2c_init(void)
{
	int ret = i2c_add_driver(&bq27441_battery_driver);
	if (ret)
		printk(KERN_ERR "Unable to register BQ27441 i2c driver\n");

	return ret;
}

static inline void bq27441_battery_i2c_exit(void)
{
	i2c_del_driver(&bq27441_battery_driver);
}



/*
 * Module stuff
 */

static int __init bq27441_battery_init(void)
{
	int ret;

	ret = bq27441_battery_i2c_init();
	if (ret)
		return ret;

	return ret;
}
module_init(bq27441_battery_init);

static void __exit bq27441_battery_exit(void)
{
	bq27441_battery_i2c_exit();
}
module_exit(bq27441_battery_exit);

MODULE_AUTHOR("Rodolfo Giometti <giometti@linux.it>");
MODULE_DESCRIPTION("BQ27441 battery monitor driver");
MODULE_LICENSE("GPL");
