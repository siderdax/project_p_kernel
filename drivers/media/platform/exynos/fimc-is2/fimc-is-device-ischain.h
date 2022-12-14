/*
 * Samsung Exynos5 SoC series FIMC-IS driver
 *
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef FIMC_IS_DEVICE_ISCHAIN_H
#define FIMC_IS_DEVICE_ISCHAIN_H

#include <linux/pm_qos.h>

#include "fimc-is-mem.h"
#include "fimc-is-subdev-ctrl.h"
#include "fimc-is-groupmgr.h"
#include "fimc-is-resourcemgr.h"
#include "fimc-is-binary.h"

#define SENSOR_MAX_CTL			0x10
#define SENSOR_MAX_CTL_MASK		(SENSOR_MAX_CTL-1)

#define REPROCESSING_FLAG		0x80000000
#define REPROCESSING_MASK		0xF0000000
#define REPROCESSING_SHIFT		28
#define OTF_3AA_MASK			0x0F000000
#define OTF_3AA_SHIFT			24
#define SSX_VINDEX_MASK			0x00FF0000
#define SSX_VINDEX_SHIFT		16
#define TAX_VINDEX_MASK			0x0000FF00
#define TAX_VINDEX_SHIFT		8
#define MODULE_MASK			0x000000FF

#define FIMC_IS_SETFILE_MASK		0x0000FFFF
#define FIMC_IS_ISP_CRANGE_MASK		0x0F000000
#define FIMC_IS_ISP_CRANGE_SHIFT	24
#define FIMC_IS_SCC_CRANGE_MASK		0x00F00000
#define FIMC_IS_SCC_CRANGE_SHIFT	20
#define FIMC_IS_SCP_CRANGE_MASK		0x000F0000
#define FIMC_IS_SCP_CRANGE_SHIFT	16
#define FIMC_IS_CRANGE_FULL		0
#define FIMC_IS_CRANGE_LIMITED		1

/*global state*/
enum fimc_is_ischain_state {
	FIMC_IS_ISCHAIN_OPENING,
	FIMC_IS_ISCHAIN_CLOSING,
	FIMC_IS_ISCHAIN_OPEN,
	FIMC_IS_ISCHAIN_INITING,
	FIMC_IS_ISCHAIN_INIT,
	FIMC_IS_ISCHAIN_START,
	FIMC_IS_ISCHAIN_LOADED,
	FIMC_IS_ISCHAIN_POWER_ON,
	FIMC_IS_ISCHAIN_OPEN_STREAM,
	FIMC_IS_ISCHAIN_REPROCESSING,
};

enum fimc_is_camera_device {
	CAMERA_SINGLE_REAR,
	CAMERA_SINGLE_FRONT,
};

struct fimc_is_ishcain_mem {
	/* buffer base */
	dma_addr_t		base;
	/* total length */
	size_t			size;
	/* buffer base */
	dma_addr_t		vaddr_base;
	/* current addr */
	dma_addr_t		vaddr_curr;
	void			*fw_cookie;

	/* fw memory base */
	u32			dvaddr;
	ulong			kvaddr;
	/* debug part of fw memory */
	u32			dvaddr_debug;
	ulong			kvaddr_debug;
	/* is region part of fw memory */
	u32			offset_region;
	u32			dvaddr_region;
	ulong			kvaddr_region;
	/* shared part of is region */
	ulong		offset_shared;
	u32			dvaddr_shared;
	ulong			kvaddr_shared;
	/* internal memory for ODC */
	u32			dvaddr_odc;
	ulong			kvaddr_odc;
	/* internal memory for DIS */
	u32			dvaddr_dis;
	u32			kvaddr_dis;
	/* internal memory for 3DNR */
	u32			dvaddr_3dnr;
	ulong			kvaddr_3dnr;

	struct is_region	*is_region;
};

struct fimc_is_device_ischain {
	struct platform_device			*pdev;
	struct exynos_platform_fimc_is		*pdata;

	struct fimc_is_resourcemgr		*resourcemgr;
	struct fimc_is_groupmgr			*groupmgr;
	struct fimc_is_interface		*interface;
	struct fimc_is_mem			*mem;

	u32					instance;
	u32					instance_sensor;
	u32					module;
	struct fimc_is_ishcain_mem		imemory;
	struct fimc_is_path_info		path;

	struct is_region			*is_region;

	unsigned long				state;
	atomic_t				group_open_cnt;
	atomic_t				open_cnt;
	atomic_t				init_cnt;

	u32					setfile;

	struct camera2_sm			capability;
	struct camera2_uctl			cur_peri_ctl;
	struct camera2_uctl			peri_ctls[SENSOR_MAX_CTL];

	/* isp margin */
	u32					margin_left;
	u32					margin_right;
	u32					margin_width;
	u32					margin_top;
	u32					margin_bottom;
	u32					margin_height;

	struct fimc_is_group			group_3aa;
	struct fimc_is_subdev			txc;
	struct fimc_is_subdev			txp;

	struct fimc_is_group			group_isp;
	struct fimc_is_subdev			ixc;
	struct fimc_is_subdev			ixp;
	struct fimc_is_subdev			drc;
	struct fimc_is_subdev			scc;

	struct fimc_is_group			group_dis;
	struct fimc_is_subdev			odc;
	struct fimc_is_subdev			dnr;
	struct fimc_is_subdev			scp;
	struct fimc_is_subdev			vra;

	u32					private_data;
	struct fimc_is_device_sensor		*sensor;
	struct pm_qos_request			user_qos;
};

/*global function*/
int fimc_is_ischain_probe(struct fimc_is_device_ischain *device,
	struct fimc_is_interface *interface,
	struct fimc_is_resourcemgr *resourcemgr,
	struct fimc_is_groupmgr *groupmgr,
	struct fimc_is_mem *mem,
	struct platform_device *pdev,
	u32 instance);
int fimc_is_ischain_g_capability(struct fimc_is_device_ischain *this,
	ulong user_ptr);
void fimc_is_ischain_meta_invalid(struct fimc_is_frame *frame);

int fimc_is_ischain_open_wrap(struct fimc_is_device_ischain *device, bool EOS);
int fimc_is_ischain_close_wrap(struct fimc_is_device_ischain *device);

void fimc_is_ischain_version(enum fimc_is_bin_type type, const char *load_bin, u32 size);
/* 3AA subdev */
int fimc_is_ischain_3aa_open(struct fimc_is_device_ischain *device,
	struct fimc_is_video_ctx *vctx);
int fimc_is_ischain_3aa_close(struct fimc_is_device_ischain *device,
	struct fimc_is_video_ctx *vctx);
int fimc_is_ischain_3aa_s_input(struct fimc_is_device_ischain *device,
	u32 stream_type,
	u32 module_id,
	u32 video_id,
	u32 otf_input,
	u32 stream_leader);
int fimc_is_ischain_3aa_reqbufs(struct fimc_is_device_ischain *device,
	u32 count);
int fimc_is_ischain_3aa_buffer_queue(struct fimc_is_device_ischain *device,
	struct fimc_is_queue *queue,
	u32 index);
int fimc_is_ischain_3aa_buffer_finish(struct fimc_is_device_ischain *device,
	u32 index);

/* isp subdev */
int fimc_is_ischain_isp_open(struct fimc_is_device_ischain *device,
	struct fimc_is_video_ctx *vctx);
int fimc_is_ischain_isp_close(struct fimc_is_device_ischain *device,
	struct fimc_is_video_ctx *vctx);
int fimc_is_ischain_isp_s_input(struct fimc_is_device_ischain *device,
	u32 stream_type,
	u32 module_id,
	u32 video_id,
	u32 otf_input,
	u32 stream_leader);
int fimc_is_ischain_isp_reqbufs(struct fimc_is_device_ischain *device,
	u32 count);
int fimc_is_ischain_isp_buffer_queue(struct fimc_is_device_ischain *device,
	struct fimc_is_queue *queue,
	u32 index);
int fimc_is_ischain_isp_buffer_finish(struct fimc_is_device_ischain *this,
	u32 index);

/*scc subdev*/
/*scp subdev*/
int fimc_is_ischain_scp_s_format(struct fimc_is_device_ischain *device,
	u32 pixelformat, u32 width, u32 height);

/* dis subdev */
int fimc_is_ischain_dis_open(struct fimc_is_device_ischain *device,
	struct fimc_is_video_ctx *vctx);
int fimc_is_ischain_dis_close(struct fimc_is_device_ischain *device,
	struct fimc_is_video_ctx *vctx);
int fimc_is_ischain_dis_s_input(struct fimc_is_device_ischain *device,
	u32 stream_type,
	u32 module_id,
	u32 video_id,
	u32 otf_input,
	u32 stream_leader);
int fimc_is_ischain_dis_buffer_queue(struct fimc_is_device_ischain *device,
	struct fimc_is_queue *queue,
	u32 index);
int fimc_is_ischain_dis_buffer_finish(struct fimc_is_device_ischain *this,
	u32 index);

/*special api for sensor*/
int fimc_is_ischain_camctl(struct fimc_is_device_ischain *this,
	struct fimc_is_frame *frame,
	u32 fcount);
int fimc_is_ischain_tag(struct fimc_is_device_ischain *ischain,
	struct fimc_is_frame *frame);

int fimc_is_itf_stream_on(struct fimc_is_device_ischain *this);
int fimc_is_itf_stream_off(struct fimc_is_device_ischain *this);
int fimc_is_itf_process_start(struct fimc_is_device_ischain *device,
	u32 group);
int fimc_is_itf_process_stop(struct fimc_is_device_ischain *device,
	u32 group);
int fimc_is_itf_force_stop(struct fimc_is_device_ischain *device,
	u32 group);
int fimc_is_itf_map(struct fimc_is_device_ischain *device,
	u32 group, u32 shot_addr, u32 shot_size);
int fimc_is_itf_grp_shot(struct fimc_is_device_ischain *device,
	struct fimc_is_group *group,
	struct fimc_is_frame *frame);
int fimc_is_itf_i2c_lock(struct fimc_is_device_ischain *this,
	int i2c_clk, bool lock);
void * fimc_is_itf_g_param(struct fimc_is_device_ischain *device,
	struct fimc_is_frame *frame,
	u32 index);
void fimc_is_itf_storefirm(struct fimc_is_device_ischain *device);
void fimc_is_itf_restorefirm(struct fimc_is_device_ischain *device);
int fimc_is_itf_set_fwboot(struct fimc_is_device_ischain *device, u32 val);

extern const struct fimc_is_queue_ops fimc_is_ischain_3aa_ops;
extern const struct fimc_is_queue_ops fimc_is_ischain_isp_ops;
extern const struct fimc_is_queue_ops fimc_is_ischain_dis_ops;
extern const struct fimc_is_queue_ops fimc_is_ischain_subdev_ops;

int fimc_is_itf_power_down(struct fimc_is_interface *interface);
int fimc_is_ischain_power(struct fimc_is_device_ischain *this, int on);

#define IS_EQUAL_COORD(i, o)				\
	(((i)[0] != (o)[0]) || ((i)[1] != (o)[1]) ||	\
	 ((i)[2] != (o)[2]) || ((i)[3] != (o)[3]))
#define IS_NULL_COORD(c)				\
	(!(c)[0] && !(c)[1] && !(c)[2] && !(c)[3])
#endif
