/*
 * Register definition file for Samsung MFC V9 Driver
 *
 * Copyright (c) 2010 Samsung Electronics
 * http://www.samsung.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef _REGS_FIMV_V9_H
#define _REGS_FIMV_V9_H

#define S5P_FIMV_REG_SIZE	(S5P_FIMV_END_ADDR - S5P_FIMV_START_ADDR)
#define S5P_FIMV_REG_COUNT	((S5P_FIMV_END_ADDR - S5p_FIMV_START_ADDR) / 4)

/* Number of bits that the buffer address should be shifted for particular
 * MFC  buffers.  */
#define S5P_FIMV_MEM_OFFSET		0

#define S5P_FIMV_START_ADDR		0x0000
#define S5P_FIMV_END_ADDR		0xfdd0

#define S5P_FIMV_REG_CLEAR_BEGIN	0xf000
#define S5P_FIMV_REG_CLEAR_COUNT	1024

/* Codec Common Registers */
#define S5P_FIMV_RISC_ON			0x0000
#define S5P_FIMV_RISC2HOST_INT			0x003C
#define S5P_FIMV_HOST2RISC_INT			0x0044
#define S5P_FIMV_RISC_BASE_ADDRESS		0x0054

#define S5P_FIMV_MFC_RESET			0x1070

/* Common SFRs for MFC v7.x or later */
#define S5P_FIMV_DIS_SHARED_MEM_ADDR		0xF044

#define S5P_FIMV_HOST2RISC_CMD			0x1100
#define S5P_FIMV_H2R_CMD_EMPTY			0
#define S5P_FIMV_H2R_CMD_SYS_INIT		1
#define S5P_FIMV_H2R_CMD_OPEN_INSTANCE		2
#define S5P_FIMV_CH_SEQ_HEADER			3
#define S5P_FIMV_CH_INIT_BUFS			4
#define S5P_FIMV_CH_NAL_START			5
#define S5P_FIMV_CH_FRAME_START			S5P_FIMV_CH_NAL_START
#define S5P_FIMV_H2R_CMD_CLOSE_INSTANCE		6
#define S5P_FIMV_H2R_CMD_SLEEP			7
#define S5P_FIMV_H2R_CMD_WAKEUP			8
#define S5P_FIMV_CH_LAST_FRAME			9
#define S5P_FIMV_H2R_CMD_FLUSH			10
#define S5P_FIMV_CH_NAL_ABORT			11
#define S5P_FIMV_CH_CACHE_FLUSH			12
/* RMVME: REALLOC used? */
#define S5P_FIMV_CH_FRAME_START_REALLOC		5

#define S5P_FIMV_RISC2HOST_CMD			0x1104
#define S5P_FIMV_R2H_CMD_EMPTY			0
#define S5P_FIMV_R2H_CMD_SYS_INIT_RET		1
#define S5P_FIMV_R2H_CMD_OPEN_INSTANCE_RET	2
#define S5P_FIMV_R2H_CMD_SEQ_DONE_RET		3
#define S5P_FIMV_R2H_CMD_INIT_BUFFERS_RET	4

#define S5P_FIMV_R2H_CMD_CLOSE_INSTANCE_RET	6
#define S5P_FIMV_R2H_CMD_SLEEP_RET		7
#define S5P_FIMV_R2H_CMD_WAKEUP_RET		8
#define S5P_FIMV_R2H_CMD_COMPLETE_SEQ_RET	9
#define S5P_FIMV_R2H_CMD_DPB_FLUSH_RET		10
#define S5P_FIMV_R2H_CMD_NAL_ABORT_RET		11
#define S5P_FIMV_R2H_CMD_FW_STATUS_RET		12
#define S5P_FIMV_R2H_CMD_FRAME_DONE_RET		13
#define S5P_FIMV_R2H_CMD_FIELD_DONE_RET		14
#define S5P_FIMV_R2H_CMD_SLICE_DONE_RET		15
#define S5P_FIMV_R2H_CMD_CACHE_FLUSH_RET	20
#define S5P_FIMV_R2H_CMD_ERR_RET		32

#define S5P_FIMV_H2R_CMD_FW_STATUS		12	/* dummy H/W command */
#define R2H_BIT(x)	(((x) > 0) ? (1 << ((x) - 1)) : 0)
static inline unsigned int h2r_to_r2h_bits(int cmd)
{
	unsigned int mask = 0;

	switch (cmd) {
	case S5P_FIMV_H2R_CMD_FW_STATUS:
	case S5P_FIMV_H2R_CMD_SYS_INIT:
	case S5P_FIMV_H2R_CMD_OPEN_INSTANCE:
	case S5P_FIMV_CH_SEQ_HEADER:
	case S5P_FIMV_CH_INIT_BUFS:
	case S5P_FIMV_H2R_CMD_CLOSE_INSTANCE:
	case S5P_FIMV_H2R_CMD_SLEEP:
	case S5P_FIMV_H2R_CMD_WAKEUP:
	case S5P_FIMV_H2R_CMD_FLUSH:
	case S5P_FIMV_CH_NAL_ABORT:
		mask |= R2H_BIT(cmd);
		break;
	case S5P_FIMV_CH_FRAME_START:
		mask |= (R2H_BIT(S5P_FIMV_R2H_CMD_FRAME_DONE_RET) |
			 R2H_BIT(S5P_FIMV_R2H_CMD_FIELD_DONE_RET) |
			 R2H_BIT(S5P_FIMV_R2H_CMD_SLICE_DONE_RET));
		break;
	case S5P_FIMV_CH_LAST_FRAME:
		mask |= R2H_BIT(S5P_FIMV_R2H_CMD_FRAME_DONE_RET);
		break;
	}

	return (mask |= R2H_BIT(S5P_FIMV_R2H_CMD_ERR_RET));
}

static inline unsigned int r2h_bits(int cmd)
{
	unsigned int mask = R2H_BIT(cmd);

	if (cmd == S5P_FIMV_R2H_CMD_FRAME_DONE_RET)
		mask |= (R2H_BIT(S5P_FIMV_R2H_CMD_FIELD_DONE_RET) |
			 R2H_BIT(S5P_FIMV_R2H_CMD_COMPLETE_SEQ_RET) |
			 R2H_BIT(S5P_FIMV_R2H_CMD_SLICE_DONE_RET));
	/* FIXME: Temporal mask for S3D SEI processing */
	else if (cmd == S5P_FIMV_R2H_CMD_INIT_BUFFERS_RET)
		mask |= (R2H_BIT(S5P_FIMV_R2H_CMD_FIELD_DONE_RET) |
			 R2H_BIT(S5P_FIMV_R2H_CMD_SLICE_DONE_RET) |
			 R2H_BIT(S5P_FIMV_R2H_CMD_FRAME_DONE_RET));

	return (mask |= R2H_BIT(S5P_FIMV_R2H_CMD_ERR_RET));
}

#define S5P_FIMV_MFC_BUS_RESET_CTRL		0x7110
#define S5P_FIMV_MFC_BUS_STATUS			0x7018
#define S5P_FIMV_FW_VERSION			0xF000

#define S5P_FIMV_INSTANCE_ID			0xF008
#define S5P_FIMV_CODEC_TYPE			0xF00C
#define S5P_FIMV_CONTEXT_MEM_ADDR		0xF014
#define S5P_FIMV_CONTEXT_MEM_SIZE		0xF018
#define S5P_FIMV_PIXEL_FORMAT			0xF020

#define S5P_FIMV_METADATA_ENABLE		0xF024
#define S5P_FIMV_MFC_VERSION			0xF028
#define S5P_FIMV_DBG_BUFFER_ADDR		0xF030
#define S5P_FIMV_DBG_BUFFER_SIZE		0xF034

#define S5P_FIMV_HED_CONTROL		0xF038
#define S5P_FIMV_DEC_TIMEOUT_VALUE		0xF03C
#define S5P_FIMV_SHARED_MEM_ADDR		0xF040

#define S5P_FIMV_RET_INSTANCE_ID		0xF070
#define S5P_FIMV_ERROR_CODE			0xF074

#define S5P_FIMV_ERR_WARNINGS_START		160
#define S5P_FIMV_ERR_WARNINGS_END		222
#define S5P_FIMV_ERR_DEC_MASK			0xFFFF
#define S5P_FIMV_ERR_DEC_SHIFT			0
#define S5P_FIMV_ERR_DSPL_MASK			0xFFFF0000
#define S5P_FIMV_ERR_DSPL_SHIFT			16

#define S5P_FIMV_DBG_BUFFER_OUTPUT_SIZE		0xF078
#define S5P_FIMV_METADATA_STATUS		0xF07C
#define S5P_FIMV_METADATA_ADDR_MB_INFO		0xF080
#define S5P_FIMV_METADATA_SIZE_MB_INFO		0xF084
#define S5P_FIMV_DBG_INFO_STAGE_COUNTER		0xF088

/* Decoder Registers */
#define S5P_FIMV_D_CRC_CTRL			0xF0B0
#define S5P_FIMV_D_DEC_OPTIONS			0xF0B4
#define S5P_FIMV_D_OPT_DISPLAY_LINEAR_EN	11
#define S5P_FIMV_D_OPT_DISCARD_RCV_HEADER	7
#define S5P_FIMV_D_OPT_IDR_DECODING_SHFT	6
#define S5P_FIMV_D_OPT_FMO_ASO_CTRL_MASK	4
#define S5P_FIMV_D_OPT_DDELAY_EN_SHIFT		3
#define S5P_FIMV_D_OPT_LF_CTRL_SHIFT		1
#define S5P_FIMV_D_OPT_LF_CTRL_MASK		0x3
#define S5P_FIMV_D_OPT_TILE_MODE_SHIFT		0
#define S5P_FIMV_D_OPT_DYNAMIC_DPB_SET_SHIFT	3
#define S5P_FIMV_D_OPT_NOT_CODED_SET_SHIFT	4

#define S5P_FIMV_D_DISPLAY_DELAY		0xF0B8

#define S5P_FIMV_D_SET_FRAME_WIDTH		0xF0BC
#define S5P_FIMV_D_SET_FRAME_HEIGHT		0xF0C0

#define S5P_FIMV_D_SEI_ENABLE			0xF0C4
#define S5P_FIMV_D_SEI_NEED_INIT_BUFFER_SHIFT	1

/* Buffer setting registers */
/* Session return */
#define S5P_FIMV_D_MIN_NUM_DPB                                  0xF0F0
#define S5P_FIMV_D_MIN_FIRST_PLANE_DPB_SIZE                     0xF0F4
#define S5P_FIMV_D_MIN_SECOND_PLANE_DPB_SIZE                    0xF0F8
#define S5P_FIMV_D_MIN_THIRD_PLANE_DPB_SIZE                     0xF0FC
#define S5P_FIMV_D_MIN_NUM_MV                                   0xF100
#define S5P_FIMV_D_MVC_NUM_VIEWS                                0xF104
#define S5P_FIMV_D_MIN_SCRATCH_BUFFER_SIZE			0xF108

/* Buffers */
#define S5P_FIMV_D_NUM_DPB                                      0xF130
#define S5P_FIMV_D_NUM_MV                                       0xF134
#define S5P_FIMV_D_FIRST_PLANE_DPB_STRIDE_SIZE                  0xF138
#define S5P_FIMV_D_SECOND_PLANE_DPB_STRIDE_SIZE                 0xF13C
#define S5P_FIMV_D_THIRD_PLANE_DPB_STRIDE_SIZE                  0xF140
#define S5P_FIMV_D_FIRST_PLANE_DPB_SIZE                         0xF144
#define S5P_FIMV_D_SECOND_PLANE_DPB_SIZE                        0xF148
#define S5P_FIMV_D_THIRD_PLANE_DPB_SIZE                         0xF14C
#define S5P_FIMV_D_MV_BUFFER_SIZE                               0xF150
#define S5P_FIMV_D_INIT_BUFFER_OPTIONS                          0xF154
#define S5P_FIMV_D_FIRST_PLANE_DPB0                             0xF160
#define S5P_FIMV_D_SECOND_PLANE_DPB0                            0xF260
#define S5P_FIMV_D_THIRD_PLANE_DPB0                             0xF360
#define S5P_FIMV_D_MV_BUFFER0                                   0xF460
#define S5P_FIMV_D_SCRATCH_BUFFER_ADDR                          0xF560
#define S5P_FIMV_D_SCRATCH_BUFFER_SIZE                          0xF564
#define S5P_FIMV_D_METADATA_BUFFER_ADDR                         0xF568
#define S5P_FIMV_D_METADATA_BUFFER_SIZE                         0xF56C

#define S5P_FIMV_D_STATIC_BUFFER_ADDR				0xF570
#define S5P_FIMV_D_STATIC_BUFFER_SIZE				0xF574

/* Nal cmd */
#define S5P_FIMV_D_CPB_BUFFER_ADDR                              0xF5B0
#define S5P_FIMV_D_CPB_BUFFER_SIZE                              0xF5B4
#define S5P_FIMV_D_AVAILABLE_DPB_FLAG_UPPER                     0xF5B8
#define S5P_FIMV_D_AVAILABLE_DPB_FLAG_LOWER                     0xF5BC
#define S5P_FIMV_D_CPB_BUFFER_OFFSET                            0xF5C0
#define S5P_FIMV_D_SLICE_IF_ENABLE                              0xF5C4
#define S5P_FIMV_D_PICTURE_TAG                                  0xF5C8
#define S5P_FIMV_D_STREAM_DATA_SIZE                             0xF5D0
#define S5P_FIMV_D_DYNAMIC_DPB_FLAG_UPPER			0xF5D4
#define S5P_FIMV_D_DYNAMIC_DPB_FLAG_LOWER			0xF5D8

#define S5P_FIMV_D_MVC_VIEW_ID_DISP_MASK                        0xFFFF

/* Nal return */
#define S5P_FIMV_D_DISPLAY_FRAME_WIDTH                          0xF600
#define S5P_FIMV_D_DISPLAY_FRAME_HEIGHT                         0xF604
#define S5P_FIMV_D_DISPLAY_STATUS                               0xF608
#define S5P_FIMV_D_DISPLAY_FIRST_PLANE_ADDR                     0xF60C
#define S5P_FIMV_D_DISPLAY_SECOND_PLANE_ADDR                    0xF610
#define S5P_FIMV_D_DISPLAY_THIRD_PLANE_ADDR                     0xF614
#define S5P_FIMV_D_DISPLAY_FRAME_TYPE                           0xF618
#define S5P_FIMV_D_DISPLAY_CROP_INFO1                           0xF61C
#define S5P_FIMV_D_DISPLAY_CROP_INFO2                           0xF620
#define S5P_FIMV_D_DISPLAY_PICTURE_PROFILE                      0xF624
#define S5P_FIMV_D_DISPLAY_FIRST_PLANE_CRC                      0xF628
#define S5P_FIMV_D_DISPLAY_SECOND_PLANE_CRC                     0xF62C
#define S5P_FIMV_D_DISPLAY_THIRD_PLANE_CRC                      0xF630
#define S5P_FIMV_D_DISPLAY_ASPECT_RATIO                         0xF634
#define S5P_FIMV_D_DISPLAY_EXTENDED_AR                          0xF638
#define S5P_FIMV_D_DECODED_FRAME_WIDTH                          0xF63C
#define S5P_FIMV_D_DECODED_FRAME_HEIGHT                         0xF640
#define S5P_FIMV_D_DECODED_STATUS                               0xF644
#define S5P_FIMV_D_DECODED_FIRST_PLANE_ADDR                     0xF648
#define S5P_FIMV_D_DECODED_SECOND_PLANE_ADDR                    0xF64C
#define S5P_FIMV_D_DECODED_THIRD_PLANE_ADDR                     0xF650
#define S5P_FIMV_D_DECODED_FRAME_TYPE                           0xF654
#define S5P_FIMV_D_DECODED_CROP_INFO1                           0xF658
#define S5P_FIMV_D_DECODED_CROP_INFO2                           0xF65C
#define S5P_FIMV_D_DECODED_PICTURE_PROFILE                      0xF660
#define S5P_FIMV_D_DECODED_NAL_SIZE                             0xF664
#define S5P_FIMV_D_DECODED_FIRST_PLANE_CRC                      0xF668
#define S5P_FIMV_D_DECODED_SECOND_PLANE_CRC                     0xF66C
#define S5P_FIMV_D_DECODED_THIRD_PLANE_CRC                      0xF670
#define S5P_FIMV_D_RET_PICTURE_TAG_TOP                          0xF674
#define S5P_FIMV_D_RET_PICTURE_TAG_BOT                          0xF678
#define S5P_FIMV_D_RET_PICTURE_TIME_TOP                         0xF67C
#define S5P_FIMV_D_RET_PICTURE_TIME_BOT                         0xF680
#define S5P_FIMV_D_CHROMA_FORMAT                                0xF684

#define S5P_FIMV_D_VC1_INFO					0xF688
#define S5P_FIMV_D_MPEG4_INFO					0xF68C
#define S5P_FIMV_D_H264_INFO					0xF690
#define S5P_FIMV_D_HEVC_INFO					0xF6A0

#define S5P_FIMV_D_METADATA_ADDR_CONCEALED_MB			0xF6B0
#define S5P_FIMV_D_METADATA_SIZE_CONCEALED_MB			0xF6B4
#define S5P_FIMV_D_METADATA_ADDR_VC1_PARAM			0xF6B8
#define S5P_FIMV_D_METADATA_SIZE_VC1_PARAM			0xF6BC
#define S5P_FIMV_D_METADATA_ADDR_SEI_NAL                        0xF6C0
#define S5P_FIMV_D_METADATA_SIZE_SEI_NAL                        0xF6C4
#define S5P_FIMV_D_METADATA_ADDR_VUI                            0xF6C8
#define S5P_FIMV_D_METADATA_SIZE_VUI                            0xF6CC
#define S5P_FIMV_D_METADATA_ADDR_MVCVUI				0xF6D0
#define S5P_FIMV_D_METADATA_SIZE_MVCVUI				0xF6D4

#define S5P_FIMV_D_MVC_VIEW_ID			                0xF6D8
#define S5P_FIMV_D_MVC_VIEW_ID_DISP_MASK	                0xFFFF

#define S5P_FIMV_D_FRAME_PACK_SEI_AVAIL                         0xF6DC
#define S5P_FIMV_D_FRAME_PACK_ARRGMENT_ID                       0xF6E0
#define S5P_FIMV_D_FRAME_PACK_SEI_INFO                          0xF6E4
#define S5P_FIMV_D_FRAME_PACK_GRID_POS                          0xF6E8

#define S5P_FIMV_D_DISPLAY_RECOVERY_SEI_INFO			0xF6EC
#define S5P_FIMV_D_DECODED_RECOVERY_SEI_INFO			0xF6F0

#define S5P_FIMV_D_USED_DPB_FLAG_UPPER		                0xF720
#define S5P_FIMV_D_USED_DPB_FLAG_LOWER		                0xF724

/* Display status */
#define S5P_FIMV_DEC_STATUS_DECODING_ONLY		0
#define S5P_FIMV_DEC_STATUS_DECODING_DISPLAY		1
#define S5P_FIMV_DEC_STATUS_DISPLAY_ONLY		2
#define S5P_FIMV_DEC_STATUS_DECODING_EMPTY		3
#define S5P_FIMV_DEC_STATUS_LAST_DISP			4	/* Hooking value */
#define S5P_FIMV_DEC_STATUS_DECODING_STATUS_MASK	7
#define S5P_FIMV_DEC_STATUS_PROGRESSIVE			(0<<3)
#define S5P_FIMV_DEC_STATUS_INTERLACE			(1<<3)
#define S5P_FIMV_DEC_STATUS_INTERLACE_MASK		(1<<3)
#define S5P_FIMV_DEC_STATUS_INTERLACE_SHIFT		3
#define S5P_FIMV_DEC_STATUS_RESOLUTION_MASK		(3<<4)
#define S5P_FIMV_DEC_STATUS_RESOLUTION_INC		(1<<4)
#define S5P_FIMV_DEC_STATUS_RESOLUTION_DEC		(2<<4)
#define S5P_FIMV_DEC_STATUS_RESOLUTION_SHIFT		4
#define S5P_FIMV_DEC_STATUS_CRC_GENERATED		(1<<5)
#define S5P_FIMV_DEC_STATUS_CRC_NOT_GENERATED		(0<<5)
#define S5P_FIMV_DEC_STATUS_CRC_MASK			(1<<5)

#define S5P_FIMV_D_DISPLAY_LUMA_ADDR		0xF60C
#define S5P_FIMV_D_DISPLAY_CHROMA_ADDR		0xF610

#define S5P_FIMV_DISPLAY_FRAME_MASK		7
#define S5P_FIMV_DISPLAY_TEMP_INFO_MASK		0x1
#define S5P_FIMV_DISPLAY_TEMP_INFO_SHIFT	7
#define S5P_FIMV_DISPLAY_FRAME_NOT_CODED	0
#define S5P_FIMV_DISPLAY_FRAME_I		1
#define S5P_FIMV_DISPLAY_FRAME_P		2
#define S5P_FIMV_DISPLAY_FRAME_B		3
#define S5P_FIMV_DISPLAY_LAST_INFO_MASK		0x1
#define S5P_FIMV_DISPLAY_LAST_INFO_SHIFT	11
#define S5P_FIMV_SHARED_CROP_INFO_H		0x0020
#define S5P_FIMV_SHARED_CROP_LEFT_MASK		0xFFFF
#define S5P_FIMV_SHARED_CROP_LEFT_SHIFT		0
#define S5P_FIMV_SHARED_CROP_RIGHT_MASK		0xFFFF0000
#define S5P_FIMV_SHARED_CROP_RIGHT_SHIFT	16
#define S5P_FIMV_SHARED_CROP_INFO_V		0x0024
#define S5P_FIMV_SHARED_CROP_TOP_MASK		0xFFFF
#define S5P_FIMV_SHARED_CROP_TOP_SHIFT		0
#define S5P_FIMV_SHARED_CROP_BOTTOM_MASK	0xFFFF0000
#define S5P_FIMV_SHARED_CROP_BOTTOM_SHIFT	16


#define S5P_FIMV_DEC_CRC_GEN_MASK		0x1
#define S5P_FIMV_DEC_CRC_GEN_SHIFT		6

#define S5P_FIMV_DECODED_FRAME_MASK		7
#define S5P_FIMV_DECODED_FRAME_NOT_CODED	0
#define S5P_FIMV_DECODED_FRAME_I		1
#define S5P_FIMV_DECODED_FRAME_P		2
#define S5P_FIMV_DECODED_FRAME_B		3

#define S5P_FIMV_D_DECODED_LUMA_ADDR		0xF648
#define S5P_FIMV_D_DECODED_CHROMA_ADDR		0xF64C

/* Encoder Registers */
#define S5P_FIMV_E_FRAME_WIDTH			0xF770
#define S5P_FIMV_E_FRAME_HEIGHT			0xF774
#define S5P_FIMV_E_CROPPED_FRAME_WIDTH		0xF778
#define S5P_FIMV_E_CROPPED_FRAME_HEIGHT		0xF77C
#define S5P_FIMV_E_FRAME_CROP_OFFSET		0xF780
#define S5P_FIMV_E_ENC_OPTIONS			0xF784
#define S5P_FIMV_E_PICTURE_PROFILE		0xF788
#define S5P_FIMV_E_VBV_BUFFER_SIZE		0xF78C
#define S5P_FIMV_E_VBV_INIT_DELAY		0xF790
#define S5P_FIMV_ENC_PROFILE_H264_BASELINE             0
#define S5P_FIMV_ENC_PROFILE_H264_MAIN                 1
#define S5P_FIMV_ENC_PROFILE_H264_HIGH                 2
#define S5P_FIMV_ENC_PROFILE_H264_CONSTRAINED_BASELINE 3
#define S5P_FIMV_ENC_PROFILE_MPEG4_SIMPLE              0
#define S5P_FIMV_ENC_PROFILE_MPEG4_ADVANCED_SIMPLE     1
#define S5P_FIMV_E_FIXED_PICTURE_QP		0xF794
#define S5P_FIMV_E_RC_CONFIG			0xF798
#define S5P_FIMV_E_RC_QP_BOUND			0xF79C
#define S5P_FIMV_E_RC_QP_BOUND_PB		0xF7A0
#define S5P_FIMV_E_RC_MODE			0xF7A4

#define S5P_FIMV_ENC_TIGHT_CBR			1
#define S5P_FIMV_ENC_LOOSE_CBR			2
#define S5P_FIMV_ENC_ADV_TIGHT_CBR		0
#define S5P_FIMV_ENC_ADV_LOOSE_CBR		1
#define S5P_FIMV_ENC_ADV_CAM_CBR		2
#define S5P_FIMV_ENC_ADV_I_LIMIT_CBR		3

#define S5P_FIMV_E_MB_RC_CONFIG			0xF7A8
#define S5P_FIMV_E_PADDING_CTRL			0xF7AC
#define S5P_FIMV_E_AIR_THRESHOLD		0xF7B0

#define S5P_FIMV_E_MV_HOR_RANGE			0xF7B4
#define S5P_FIMV_E_MV_VER_RANGE			0xF7B8
#define S5P_FIMV_E_NUM_DPB			0xF890
#define S5P_FIMV_E_MIN_SCRATCH_BUFFER_SIZE	0xF894

#define S5P_FIMV_E_LUMA_DPB			0xF8C0
#define S5P_FIMV_E_CHROMA_DPB			0xF904
#define S5P_FIMV_E_ME_BUFFER			0xF948

#define S5P_FIMV_E_SCRATCH_BUFFER_ADDR		0xF98C
#define S5P_FIMV_E_SCRATCH_BUFFER_SIZE		0xF990
#define S5P_FIMV_E_TMV_BUFFER0			0xF994
#define S5P_FIMV_E_TMV_BUFFER1			0xF998
#define S5P_FIMV_E_IR_BUFFER_ADDR		0xF99C
#define S5P_FIMV_E_SOURCE_FIRST_ADDR		0xF9E0
#define S5P_FIMV_E_SOURCE_SECOND_ADDR		0xF9E4
#define S5P_FIMV_E_SOURCE_THIRD_ADDR		0xF9E8
#define S5P_FIMV_E_SOURCE_FIRST_STRIDE		0xF9EC
#define S5P_FIMV_E_SOURCE_SECOND_STRIDE		0xF9F0
#define S5P_FIMV_E_SOURCE_THIRD_STRIDE		0xF9F4
#define S5P_FIMV_E_STREAM_BUFFER_ADDR		0xF9F8
#define S5P_FIMV_E_STREAM_BUFFER_SIZE		0xF9FC
#define S5P_FIMV_E_ROI_BUFFER_ADDR		0xFA00

#define S5P_FIMV_E_PARAM_CHANGE			0xFA04
#define S5P_FIMV_E_IR_SIZE			0xFA08
#define S5P_FIMV_E_GOP_CONFIG			0xFA0C
#define S5P_FIMV_E_MSLICE_MODE			0xFA10
#define S5P_FIMV_E_MSLICE_SIZE_MB		0xFA14
#define S5P_FIMV_E_MSLICE_SIZE_BITS		0xFA18
#define S5P_FIMV_E_FRAME_INSERTION		0xFA1C

#define S5P_FIMV_E_RC_FRAME_RATE		0xFA20
#define S5P_FIMV_E_RC_BIT_RATE			0xFA24
#define S5P_FIMV_E_RC_ROI_CTRL			0xFA2C
#define S5P_FIMV_E_PICTURE_TAG			0xFA30
#define S5P_FIMV_E_BIT_COUNT_ENABLE		0xFA34
#define S5P_FIMV_E_MAX_BIT_COUNT		0xFA38
#define S5P_FIMV_E_MIN_BIT_COUNT		0xFA3C

#define S5P_FIMV_E_METADATA_BUFFER_ADDR		0xFA40
#define S5P_FIMV_E_METADATA_BUFFER_SIZE		0xFA44

#define S5P_FIMV_E_ENCODING_ORDER_TIME_INFO	0xFA50
#define S5P_FIMV_E_ENCODING_ORDER_INFO		0xFA54
#define S5P_FIMV_E_STREAM_BUFFER_OFFSET		0xFA58
#define S5P_FIMV_E_GOP_CONFIG2			0xFA5C

#define S5P_FIMV_E_ENCODED_SOURCE_FIRST_ADDR	0xFA70
#define S5P_FIMV_E_ENCODED_SOURCE_SECOND_ADDR	0xFA74
#define S5P_FIMV_E_ENCODED_SOURCE_THIRD_ADDR	0xFA78

#define S5P_FIMV_E_STREAM_SIZE			0xFA80
#define S5P_FIMV_E_SLICE_TYPE			0xFA84
#define S5P_FIMV_ENCODED_TYPE_NOT_CODED		0
#define S5P_FIMV_ENCODED_TYPE_I			1
#define S5P_FIMV_ENCODED_TYPE_P			2
#define S5P_FIMV_ENCODED_TYPE_B			3
#define S5P_FIMV_ENCODED_TYPE_SKIPPED		4
#define S5P_FIMV_E_PICTURE_COUNT		0xFA88
#define S5P_FIMV_E_RET_PICTURE_TAG		0xFA8C

#define S5P_FIMV_E_RECON_LUMA_DPB_ADDR		0xFA9C
#define S5P_FIMV_E_RECON_CHROMA_DPB_ADDR	0xFAA0
#define S5P_FIMV_E_METADATA_ADDR_ENC_SLICE	0xFAA4
#define S5P_FIMV_E_METADATA_SIZE_ENC_SLICE	0xFAA8

#define S5P_FIMV_E_NAL_DONE_INFO		0xFAEC

#define S5P_FIMV_E_MPEG4_OPTIONS		0xFB10
#define S5P_FIMV_E_MPEG4_HEC_PERIOD		0xFB14
#define S5P_FIMV_E_ASPECT_RATIO			0xFB4C
#define S5P_FIMV_E_EXTENDED_SAR			0xFB50

#define S5P_FIMV_E_IDR_H264_IDR			1
#define S5P_FIMV_E_H264_OPTIONS			0xFB54
#define S5P_FIMV_E_H264_OPTIONS_2		0xFB58
#define S5P_FIMV_E_H264_LF_ALPHA_OFFSET		0xFB5C
#define S5P_FIMV_E_H264_LF_BETA_OFFSET		0xFB60
#define S5P_FIMV_E_H264_I_PERIOD		S5P_FIMV_E_H264_REFRESH_PERIOD
#define S5P_FIMV_E_H264_REFRESH_PERIOD		0xFB64

#define S5P_FIMV_E_H264_FMO_SLICE_GRP_MAP_TYPE			0xFB68
#define S5P_FIMV_E_H264_FMO_NUM_SLICE_GRP_MINUS1		0xFB6C
#define S5P_FIMV_E_H264_FMO_SLICE_GRP_CHANGE_DIR		0xFB70
#define S5P_FIMV_E_H264_FMO_SLICE_GRP_CHANGE_RATE_MINUS1	0xFB74
#define S5P_FIMV_E_H264_FMO_RUN_LENGTH_MINUS1_0	0xFB78
#define S5P_FIMV_E_H264_FMO_RUN_LENGTH_MINUS1_1	0xFB7C
#define S5P_FIMV_E_H264_FMO_RUN_LENGTH_MINUS1_2	0xFB80
#define S5P_FIMV_E_H264_FMO_RUN_LENGTH_MINUS1_3	0xFB84

#define S5P_FIMV_E_H264_ASO_SLICE_ORDER_0	0xFB88
#define S5P_FIMV_E_H264_ASO_SLICE_ORDER_1	0xFB8C
#define S5P_FIMV_E_H264_ASO_SLICE_ORDER_2	0xFB90
#define S5P_FIMV_E_H264_ASO_SLICE_ORDER_3	0xFB94
#define S5P_FIMV_E_H264_ASO_SLICE_ORDER_4	0xFB98
#define S5P_FIMV_E_H264_ASO_SLICE_ORDER_5	0xFB9C
#define S5P_FIMV_E_H264_ASO_SLICE_ORDER_6	0xFBA0
#define S5P_FIMV_E_H264_ASO_SLICE_ORDER_7	0xFBA4

#define S5P_FIMV_E_NUM_T_LAYER			0xFBAC

/* For backward compatibility */
#define S5P_FIMV_E_H264_CHROMA_QP_OFFSET	0xFBA8
#define S5P_FIMV_E_H264_NUM_T_LAYER		S5P_FIMV_E_NUM_T_LAYER

#define S5P_FIMV_E_HIERARCHICAL_QP_LAYER0	0xFBB0
#define S5P_FIMV_E_HIERARCHICAL_QP_LAYER1	0xFBB4
#define S5P_FIMV_E_HIERARCHICAL_QP_LAYER2	0xFBB8
#define S5P_FIMV_E_HIERARCHICAL_QP_LAYER3	0xFBBC
#define S5P_FIMV_E_HIERARCHICAL_QP_LAYER4	0xFBC0
#define S5P_FIMV_E_HIERARCHICAL_QP_LAYER5	0xFBC4
#define S5P_FIMV_E_HIERARCHICAL_QP_LAYER6	0xFBC8

/* For backward compatibility */
#define S5P_FIMV_E_H264_HIERARCHICAL_QP_LAYER0	S5P_FIMV_E_HIERARCHICAL_QP_LAYER0
#define S5P_FIMV_E_H264_HIERARCHICAL_QP_LAYER1	S5P_FIMV_E_HIERARCHICAL_QP_LAYER1
#define S5P_FIMV_E_H264_HIERARCHICAL_QP_LAYER2	S5P_FIMV_E_HIERARCHICAL_QP_LAYER2
#define S5P_FIMV_E_H264_HIERARCHICAL_QP_LAYER3	S5P_FIMV_E_HIERARCHICAL_QP_LAYER3
#define S5P_FIMV_E_H264_HIERARCHICAL_QP_LAYER4	S5P_FIMV_E_HIERARCHICAL_QP_LAYER4
#define S5P_FIMV_E_H264_HIERARCHICAL_QP_LAYER5	S5P_FIMV_E_HIERARCHICAL_QP_LAYER5
#define S5P_FIMV_E_H264_HIERARCHICAL_QP_LAYER6	S5P_FIMV_E_HIERARCHICAL_QP_LAYER6

#define S5P_FIMV_E_H264_FRAME_PACKING_SEI_INFO	0xFC4C

#define S5P_FIMV_E_H264_NAL_CONTROL		0xFD14
#define S5P_FIMV_E_H264_HIERARCHICAL_BIT_RATE_LAYER0	S5P_FIMV_E_HIERARCHICAL_BIT_RATE_LAYER0
#define S5P_FIMV_E_H264_HIERARCHICAL_BIT_RATE_LAYER1	S5P_FIMV_E_HIERARCHICAL_BIT_RATE_LAYER1
#define S5P_FIMV_E_H264_HIERARCHICAL_BIT_RATE_LAYER2	S5P_FIMV_E_HIERARCHICAL_BIT_RATE_LAYER2
#define S5P_FIMV_E_H264_HIERARCHICAL_BIT_RATE_LAYER3	S5P_FIMV_E_HIERARCHICAL_BIT_RATE_LAYER3
#define S5P_FIMV_E_H264_HIERARCHICAL_BIT_RATE_LAYER4	S5P_FIMV_E_HIERARCHICAL_BIT_RATE_LAYER4
#define S5P_FIMV_E_H264_HIERARCHICAL_BIT_RATE_LAYER5	S5P_FIMV_E_HIERARCHICAL_BIT_RATE_LAYER5
#define S5P_FIMV_E_H264_HIERARCHICAL_BIT_RATE_LAYER6	S5P_FIMV_E_HIERARCHICAL_BIT_RATE_LAYER6
#define S5P_FIMV_E_HIERARCHICAL_BIT_RATE_LAYER0	0xFD18
#define S5P_FIMV_E_HIERARCHICAL_BIT_RATE_LAYER1	0xFD1C
#define S5P_FIMV_E_HIERARCHICAL_BIT_RATE_LAYER2	0xFD20
#define S5P_FIMV_E_HIERARCHICAL_BIT_RATE_LAYER3	0xFD24
#define S5P_FIMV_E_HIERARCHICAL_BIT_RATE_LAYER4	0xFD28
#define S5P_FIMV_E_HIERARCHICAL_BIT_RATE_LAYER5	0xFD2C
#define S5P_FIMV_E_HIERARCHICAL_BIT_RATE_LAYER6	0xFD30

#define S5P_FIMV_E_MVC_FRAME_QP_VIEW1		0xFD40
#define S5P_FIMV_E_MVC_RC_FRAME_RATE_VIEW1	0xFD44
#define S5P_FIMV_E_MVC_RC_BIT_RATE_VIEW1	0xFD48
#define S5P_FIMV_E_MVC_RC_QBOUND_VIEW1		0xFD4C
#define S5P_FIMV_E_MVC_RC_MODE_VIEW1		0xFD50
#define S5P_FIMV_E_MVC_INTER_VIEW_PREDICTION_ON	0xFD80

#define S5P_FIMV_E_VP8_OPTION			0xFDB0
#define S5P_FIMV_E_VP8_FILTER_OPTIONS		0xFDB4
#define S5P_FIMV_E_VP8_GOLDEN_FRAME_OPTION	0xFDB8

/* For backward compatibility */
#define S5P_FIMV_E_VP8_NUM_T_LAYER		S5P_FIMV_E_NUM_T_LAYER
#define S5P_FIMV_E_VP8_HIERARCHICAL_QP_LAYER0	S5P_FIMV_E_HIERARCHICAL_QP_LAYER0
#define S5P_FIMV_E_VP8_HIERARCHICAL_QP_LAYER1	S5P_FIMV_E_HIERARCHICAL_QP_LAYER1
#define S5P_FIMV_E_VP8_HIERARCHICAL_QP_LAYER2	S5P_FIMV_E_HIERARCHICAL_QP_LAYER2

#define S5P_FIMV_E_HEVC_OPTIONS			0xFDD4
#define S5P_FIMV_E_HEVC_REFRESH_PERIOD		0xFDD8
#define S5P_FIMV_E_HEVC_CHROMA_QP_OFFSET	0xFDDC
#define S5P_FIMV_E_HEVC_LF_BETA_OFFSET_DIV2	0xFDE0
#define S5P_FIMV_E_HEVC_LF_TC_OFFSET_DIV2	0xFDE4
#define S5P_FIMV_E_HEVC_NAL_CONTROL		0xFDE8

#define S5P_FIMV_E_VP8_NAL_CONTROL		0xFDF0

/* Codec numbers  */
#define MFC_FORMATS_NO_CODEC		-1

#define S5P_FIMV_CODEC_H264_DEC		0
#define S5P_FIMV_CODEC_H264_MVC_DEC	1

#define S5P_FIMV_CODEC_MPEG4_DEC	3
#define S5P_FIMV_CODEC_FIMV1_DEC	4
#define S5P_FIMV_CODEC_FIMV2_DEC	5
#define S5P_FIMV_CODEC_FIMV3_DEC	6
#define S5P_FIMV_CODEC_FIMV4_DEC	7
#define S5P_FIMV_CODEC_H263_DEC		8
#define S5P_FIMV_CODEC_VC1RCV_DEC	9
#define S5P_FIMV_CODEC_VC1_DEC		10

#define S5P_FIMV_CODEC_MPEG2_DEC	13
#define S5P_FIMV_CODEC_VP8_DEC		14
#define S5P_FIMV_CODEC_HEVC_DEC		17
#define S5P_FIMV_CODEC_VP9_DEC		18

#define S5P_FIMV_CODEC_H264_ENC		20
#define S5P_FIMV_CODEC_H264_MVC_ENC	21

#define S5P_FIMV_CODEC_MPEG4_ENC	23
#define S5P_FIMV_CODEC_H263_ENC		24
#define S5P_FIMV_CODEC_VP8_ENC		25
#define S5P_FIMV_CODEC_HEVC_ENC		26

#define S5P_FIMV_SI_DISPLAY_Y_ADR		S5P_FIMV_D_DISPLAY_LUMA_ADDR
#define S5P_FIMV_SI_DISPLAY_C_ADR		S5P_FIMV_D_DISPLAY_CHROMA_ADDR
#define S5P_FIMV_SI_DECODED_Y_ADR		S5P_FIMV_D_DECODED_LUMA_ADDR
#define S5P_FIMV_SI_DECODED_C_ADR		S5P_FIMV_D_DECODED_CHROMA_ADDR

#define S5P_FIMV_CRC_LUMA0			S5P_FIMV_D_DECODED_FIRST_PLANE_CRC
#define S5P_FIMV_CRC_CHROMA0			S5P_FIMV_D_DECODED_SECOND_PLANE_CRC
#define S5P_FIMV_CRC_LUMA1			S5P_FIMV_D_DECODED_FIRST_PLANE_CRC
#define S5P_FIMV_CRC_CHROMA1			S5P_FIMV_D_DECODED_SECOND_PLANE_CRC
#define S5P_FIMV_CRC_DISP_LUMA0			S5P_FIMV_D_DISPLAY_FIRST_PLANE_CRC
#define S5P_FIMV_CRC_DISP_CHROMA0		S5P_FIMV_D_DISPLAY_SECOND_PLANE_CRC

#define S5P_FIMV_SI_DECODED_STATUS		S5P_FIMV_D_DECODED_STATUS
#define S5P_FIMV_SI_DISPLAY_STATUS		S5P_FIMV_D_DISPLAY_STATUS
#define S5P_FIMV_SHARED_SET_FRAME_TAG		S5P_FIMV_D_PICTURE_TAG
#define S5P_FIMV_SHARED_GET_FRAME_TAG_TOP	S5P_FIMV_D_RET_PICTURE_TAG_TOP
#define S5P_FIMV_CRC_DISP_STATUS		S5P_FIMV_D_DISPLAY_STATUS

/* SEI related information */
#define S5P_FIMV_FRAME_PACK_SEI_AVAIL		S5P_FIMV_D_FRAME_PACK_SEI_AVAIL
#define S5P_FIMV_FRAME_PACK_ARRGMENT_ID		S5P_FIMV_D_FRAME_PACK_ARRGMENT_ID
#define S5P_FIMV_FRAME_PACK_SEI_INFO		S5P_FIMV_D_FRAME_PACK_SEI_INFO
#define S5P_FIMV_FRAME_PACK_GRID_POS		S5P_FIMV_D_FRAME_PACK_GRID_POS

#define S5P_FIMV_SHARED_SET_E_FRAME_TAG		S5P_FIMV_E_PICTURE_TAG
#define S5P_FIMV_SHARED_GET_E_FRAME_TAG		S5P_FIMV_E_RET_PICTURE_TAG
#define S5P_FIMV_ENCODED_LUMA_ADDR		S5P_FIMV_E_ENCODED_SOURCE_LUMA_ADDR
#define S5P_FIMV_ENCODED_CHROMA_ADDR		S5P_FIMV_E_ENCODED_SOURCE_CHROMA_ADDR
#define	S5P_FIMV_FRAME_INSERTION		S5P_FIMV_E_FRAME_INSERTION

#define S5P_FIMV_PARAM_CHANGE_FLAG		S5P_FIMV_E_PARAM_CHANGE /* flag */
#define S5P_FIMV_NEW_I_PERIOD			S5P_FIMV_E_GOP_CONFIG
#define S5P_FIMV_NEW_RC_FRAME_RATE		S5P_FIMV_E_RC_FRAME_RATE
#define S5P_FIMV_NEW_RC_BIT_RATE		S5P_FIMV_E_RC_BIT_RATE
#define S5P_FIMV_NEW_RC_QP_BOUND		S5P_FIMV_E_RC_QP_BOUND

#define S5P_FIMV_ERR_FRAME_CONCEAL		150

#define S5P_FIMV_VPS_ONLY_ERROR		42
#define S5P_FIMV_SW_RESET		0x0000

/* Memory controller register */
#define S5P_FIMV_MC_DRAMBASE_ADR_A	0x0508
#define S5P_FIMV_MC_DRAMBASE_ADR_B	0x050c
#define S5P_FIMV_MC_STATUS		0x0510

/* Command from RISC to HOST */
#define S5P_FIMV_RISC2HOST_CMD_MASK	0x1FFFF

/* Encoder SFRs for MFC v6.x only */
#define S5P_FIMV_E_SOURCE_LUMA_ADDR		0xF9E0
#define S5P_FIMV_E_SOURCE_CHROMA_ADDR		0xF9E4
#define S5P_FIMV_E_ENCODED_SOURCE_LUMA_ADDR	0xFA70
#define S5P_FIMV_E_ENCODED_SOURCE_CHROMA_ADDR	0xFA74

/* Channel & stream interface register */
#define S5P_FIMV_SI_RTN_CHID		0x2000 /* Return CH instance ID register */
#define S5P_FIMV_SI_CH0_INST_ID		0x2040 /* codec instance ID */
#define S5P_FIMV_SI_CH1_INST_ID		0x2080 /* codec instance ID */

#define S5P_FIMV_NV12M_HALIGN			512
#define S5P_FIMV_NV12MT_HALIGN			16
#define S5P_FIMV_NV12MT_VALIGN			16

#define S5P_FIMV_D_LUMA_DPB_SIZE		S5P_FIMV_D_FIRST_PLANE_DPB_SIZE
#define S5P_FIMV_D_CHROMA_DPB_SIZE		S5P_FIMV_D_SECOND_PLANE_DPB_SIZE

#define S5P_FIMV_D_LUMA_DPB			S5P_FIMV_D_FIRST_PLANE_DPB0
#define S5P_FIMV_D_CHROMA_DPB			S5P_FIMV_D_SECOND_PLANE_DPB0

#define S5P_FIMV_D_MV_BUFFER			S5P_FIMV_D_MV_BUFFER0

#define S5P_FIMV_E_ENCODED_SOURCE_FIRST_ADDR	0xFA70
#define S5P_FIMV_E_ENCODED_SOURCE_SECOND_ADDR	0xFA74
#define S5P_FIMV_E_ENCODED_SOURCE_THIRD_ADDR	0xFA78

#define S5P_FIMV_SI_CH0_DPB_CONF_CTRL   0x2068 /* DPB Config Control Register */

#endif /* _REGS_S5P_FIMV_V9_H */
