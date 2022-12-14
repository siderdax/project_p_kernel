#ifndef FIMC_IS_VIDEO_H
#define FIMC_IS_VIDEO_H

#include <linux/version.h>
#include <media/v4l2-ioctl.h>
#include "fimc-is-type.h"
#include "fimc-is-mem.h"
#include "fimc-is-framemgr.h"
#ifdef CONFIG_ENABLE_HAL3_2_META_INTERFACE
#include "fimc-is-metadata.h"
#else
#include "fimc-is-metadata-legacy.h"
#endif
#include "fimc-is-config.h"

/* configuration by linux kernel version */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0))
#define VFL_DIR_RX				0
#define VFL_DIR_TX				1
#define VFL_DIR_M2M				2
#endif

#define INPUT_STREAM_MASK			0xFF000000 /* stream type : 1 (reprocessing) */
#define INPUT_STREAM_SHIFT			24
#define INPUT_MODULE_MASK			0x00FF0000 /* module id : unique sensor id */
#define INPUT_MODULE_SHIFT			16
#define INPUT_VINDEX_MASK			0x0000FF00 /* vindex : connected capture node */
#define INPUT_VINDEX_SHIFT			8
#define INPUT_MEMORY_MASK			0x000000F0 /* memory interface : 0 (on-the-fly) */
#define INPUT_MEMORY_SHIFT			4
#define INPUT_LEADER_MASK			0x0000000F /* leader : 1 (leader video node) */
#define INPUT_LEADER_SHIFT			0

#define FIMC_IS_MAX_NODES			FIMC_IS_STREAM_COUNT
#define FIMC_IS_INVALID_BUF_INDEX		0xFF

#define VIDEO_SSX_READY_BUFFERS			0
#define VIDEO_3XS_READY_BUFFERS			0
#define VIDEO_3XC_READY_BUFFERS			0
#define VIDEO_3XP_READY_BUFFERS			0
#define VIDEO_IXS_READY_BUFFERS			0
#define VIDEO_IXC_READY_BUFFERS			0
#define VIDEO_IXP_READY_BUFFERS			0
#define VIDEO_SCC_READY_BUFFERS			0
#define VIDEO_SCP_READY_BUFFERS			0
#define VIDEO_DIS_READY_BUFFERS			0

#define FIMC_IS_VIDEO_NAME(name)		("exynos-fimc-is-"name)
#define FIMC_IS_VIDEO_SSX_NAME			FIMC_IS_VIDEO_NAME("ss")
#define FIMC_IS_VIDEO_3XS_NAME(id)		FIMC_IS_VIDEO_NAME("3"#id"s")
#define FIMC_IS_VIDEO_3XC_NAME(id)		FIMC_IS_VIDEO_NAME("3"#id"c")
#define FIMC_IS_VIDEO_3XP_NAME(id)		FIMC_IS_VIDEO_NAME("3"#id"p")
#define FIMC_IS_VIDEO_IXS_NAME(id)		FIMC_IS_VIDEO_NAME("i"#id"s")
#define FIMC_IS_VIDEO_IXC_NAME(id)		FIMC_IS_VIDEO_NAME("i"#id"c")
#define FIMC_IS_VIDEO_IXP_NAME(id)		FIMC_IS_VIDEO_NAME("i"#id"p")
#define FIMC_IS_VIDEO_DIS_NAME			FIMC_IS_VIDEO_NAME("dis")
#define FIMC_IS_VIDEO_SCC_NAME			FIMC_IS_VIDEO_NAME("scc")
#define FIMC_IS_VIDEO_SCP_NAME			FIMC_IS_VIDEO_NAME("scp")

struct fimc_is_device_ischain;
struct fimc_is_subdev;
struct fimc_is_queue;
struct fimc_is_resourcemgr;

enum fimc_is_video_dev_num {
	FIMC_IS_VIDEO_SS0_NUM = 1,
	FIMC_IS_VIDEO_SS1_NUM,
	FIMC_IS_VIDEO_SS2_NUM,
	FIMC_IS_VIDEO_SS3_NUM,
	FIMC_IS_VIDEO_30S_NUM = 10,
	FIMC_IS_VIDEO_30C_NUM,
	FIMC_IS_VIDEO_30P_NUM,
	FIMC_IS_VIDEO_31S_NUM = 20,
	FIMC_IS_VIDEO_31C_NUM,
	FIMC_IS_VIDEO_31P_NUM,
	FIMC_IS_VIDEO_I0S_NUM = 30,
	FIMC_IS_VIDEO_I0C_NUM,
	FIMC_IS_VIDEO_I0P_NUM,
	FIMC_IS_VIDEO_I1S_NUM = 40,
	FIMC_IS_VIDEO_I1C_NUM,
	FIMC_IS_VIDEO_I1P_NUM,
	FIMC_IS_VIDEO_DIS_NUM = 50,
	FIMC_IS_VIDEO_SCC_NUM,
	FIMC_IS_VIDEO_SCP_NUM,
	FIMC_IS_VIDEO_VRA_NUM = 60,
	FIMC_IS_VIDEO_MAX_NUM
};

enum fimc_is_video_type {
	FIMC_IS_VIDEO_TYPE_LEADER,
	FIMC_IS_VIDEO_TYPE_CAPTURE,
};

enum fimc_is_video_state {
	FIMC_IS_VIDEO_CLOSE,
	FIMC_IS_VIDEO_OPEN,
	FIMC_IS_VIDEO_S_INPUT,
	FIMC_IS_VIDEO_S_FORMAT,
	FIMC_IS_VIDEO_S_BUFS,
	FIMC_IS_VIDEO_STOP,
	FIMC_IS_VIDEO_START,
};

enum fimc_is_queue_state {
	FIMC_IS_QUEUE_BUFFER_PREPARED,
	FIMC_IS_QUEUE_BUFFER_READY,
	FIMC_IS_QUEUE_STREAM_ON
};

struct fimc_is_frame_cfg {
	struct fimc_is_fmt		format;
	enum v4l2_colorspace		colorspace;
	u32				width;
	u32				height;
	u32				width_stride[FIMC_IS_MAX_PLANES];
	u32				size[FIMC_IS_MAX_PLANES];
	u32				bytesperline[FIMC_IS_MAX_PLANES];
};

struct fimc_is_queue_ops {
	int (*start_streaming)(void *qdevice,
		struct fimc_is_queue *queue);
	int (*stop_streaming)(void *qdevice,
		struct fimc_is_queue *queue);
	int (*s_format)(void *qdevice,
		struct fimc_is_queue *queue);
};

struct fimc_is_queue {
	struct vb2_queue		*vbq;
	const struct fimc_is_queue_ops	*qops;
	struct fimc_is_framemgr 	framemgr;
	struct fimc_is_frame_cfg	framecfg;

	u32				buf_maxcount;
	u32				buf_rdycount;
	u32				buf_refcount;
	u32				buf_dva[FIMC_IS_MAX_BUFS][FIMC_IS_MAX_PLANES];
	ulong				buf_kva[FIMC_IS_MAX_BUFS][FIMC_IS_MAX_PLANES];
	ulong				buf_box[FIMC_IS_MAX_BUFS][FIMC_IS_MAX_PLANES];

	/* for debugging */
	u32				buf_req;
	u32				buf_pre;
	u32				buf_que;
	u32				buf_com;
	u32				buf_dqe;

	u32				id;
	unsigned long			state;
};

struct fimc_is_video_ctx {
	struct fimc_is_queue		queue;
	struct mutex			lock;
	u32				instance;
	u32				refcount;
	unsigned long			state;

	void				*device;
	void				*subdev;
	struct fimc_is_video		*video;

	const struct vb2_ops		*vb2_ops;
	const struct vb2_mem_ops	*mem_ops;
};

struct fimc_is_video {
	u32				id;
	enum fimc_is_video_type 	type;
	atomic_t			refcount;
	struct mutex			lock;

	struct video_device		vd;
	struct media_pad		pads;
	struct fimc_is_resourcemgr	*resourcemgr;
	const struct fimc_is_vb2	*vb2;
	void				*alloc_ctx;
};

/* video context operation */
int open_vctx(struct file *file,
	struct fimc_is_video *video,
	struct fimc_is_video_ctx **vctx,
	u32 instance,
	u32 id);
int close_vctx(struct file *file,
	struct fimc_is_video *video,
	struct fimc_is_video_ctx *vctx);

/* queue operation */
int fimc_is_queue_setup(struct fimc_is_queue *queue,
	void *alloc_ctx,
	unsigned int *num_planes,
	unsigned int sizes[],
	void *allocators[]);
int fimc_is_queue_buffer_queue(struct fimc_is_queue *queue,
	const struct fimc_is_vb2 *vb2,
	struct vb2_buffer *vb);
int fimc_is_queue_prepare(struct vb2_buffer *vb);
void fimc_is_queue_wait_prepare(struct vb2_queue *vbq);
void fimc_is_queue_wait_finish(struct vb2_queue *vbq);
int fimc_is_queue_start_streaming(struct fimc_is_queue *queue,
	void *qdevice);
int fimc_is_queue_stop_streaming(struct fimc_is_queue *queue,
	void *qdevice);

/* video operation */
int fimc_is_video_probe(struct fimc_is_video *video,
	char *video_name,
	u32 video_number,
	u32 vfl_dir,
	struct fimc_is_mem *mem,
	struct v4l2_device *v4l2_dev,
	const struct v4l2_file_operations *fops,
	const struct v4l2_ioctl_ops *ioctl_ops);
int fimc_is_video_open(struct fimc_is_video_ctx *vctx,
	void *device,
	u32 buf_rdycount,
	struct fimc_is_video *video,
	const struct vb2_ops *vb2_ops,
	const struct fimc_is_queue_ops *qops);
int fimc_is_video_close(struct fimc_is_video_ctx *vctx);
int fimc_is_video_s_input(struct file *file,
	struct fimc_is_video_ctx *vctx);
int fimc_is_video_poll(struct file *file,
	struct fimc_is_video_ctx *vctx,
	struct poll_table_struct *wait);
int fimc_is_video_mmap(struct file *file,
	struct fimc_is_video_ctx *vctx,
	struct vm_area_struct *vma);
int fimc_is_video_reqbufs(struct file *file,
	struct fimc_is_video_ctx *vctx,
	struct v4l2_requestbuffers *request);
int fimc_is_video_querybuf(struct file *file,
	struct fimc_is_video_ctx *vctx,
	struct v4l2_buffer *buf);
int fimc_is_video_set_format_mplane(struct file *file,
	struct fimc_is_video_ctx *vctx,
	struct v4l2_format *format);
int fimc_is_video_qbuf(struct file *file,
	struct fimc_is_video_ctx *vctx,
	struct v4l2_buffer *buf);
int fimc_is_video_dqbuf(struct file *file,
	struct fimc_is_video_ctx *vctx,
	struct v4l2_buffer *buf);
int fimc_is_video_streamon(struct file *file,
	struct fimc_is_video_ctx *vctx,
	enum v4l2_buf_type type);
int fimc_is_video_streamoff(struct file *file,
	struct fimc_is_video_ctx *vctx,
	enum v4l2_buf_type type);
int fimc_is_video_s_ctrl(struct file *file,
	struct fimc_is_video_ctx *vctx,
	struct v4l2_control *ctrl);

int buffer_done(struct fimc_is_video_ctx *vctx,
	u32 index, u32 state);

#define GET_VIDEO(vctx) 		(vctx ? (vctx)->video : NULL)
#define GET_QUEUE(vctx) 		(vctx ? &(vctx)->queue : NULL)
#define GET_FRAMEMGR(vctx)		(vctx ? &(vctx)->queue.framemgr : NULL)
#define GET_DEVICE(vctx)		(vctx ? (vctx)->device : NULL)
#define CALL_QOPS(q, op, args...)	(((q)->qops->op) ? ((q)->qops->op(args)) : 0)

#endif
