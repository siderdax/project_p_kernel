# drivers/staging/samsung/Makefile
#
# Copyright (c) 2010-2011 Samsung Electronics Co., Ltd.
#		http://www.samsung.com/
#
# Licensed under GPLv2

obj-y				:=
obj-m				:=
obj-n				:=
obj-				:=

obj-$(CONFIG_BATTERY_SAMSUNG)	+= sec_batt.o

# Samsung Logging Feature
obj-$(CONFIG_SEC_EXT)		+= sec_log.o

# Samsung sec sysfs Feature
obj-$(CONFIG_SEC_SYSFS)         += sec_sysfs.o

# Samsung Reset Feature
obj-$(CONFIG_SEC_REBOOT)        += sec_reboot.o
# Samsung Debugging Feature
#
# Samsung Argos
obj-$(CONFIG_ARGOS)		+= sec_argos.o
