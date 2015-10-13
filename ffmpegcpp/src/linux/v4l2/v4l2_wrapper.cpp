#include "v4l2_wrapper.h"
#include <map>
#include <vector>
#include <dirent.h>
//#include <workaround/videodev2.h>
#include <unistd.h>
#include <errno.h>
#include <algorithm>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <cstring>

#include "linux/videodev2.h"

#if 0

#include <linux/ioctl.h>
#include <linux/types.h>



/*
*	I O C T L   C O D E S   F O R   V I D E O   D E V I C E S
*
*/
#define VIDIOC_QUERYCAP		 _IOR('V',  0, struct v4l2_capability)
#define VIDIOC_RESERVED		  _IO('V',  1)
#define VIDIOC_ENUM_FMT         _IOWR('V',  2, struct v4l2_fmtdesc)
#define VIDIOC_G_FMT		_IOWR('V',  4, struct v4l2_format)
#define VIDIOC_S_FMT		_IOWR('V',  5, struct v4l2_format)
#define VIDIOC_REQBUFS		_IOWR('V',  8, struct v4l2_requestbuffers)
#define VIDIOC_QUERYBUF		_IOWR('V',  9, struct v4l2_buffer)
#define VIDIOC_G_FBUF		 _IOR('V', 10, struct v4l2_framebuffer)
#define VIDIOC_S_FBUF		 _IOW('V', 11, struct v4l2_framebuffer)
#define VIDIOC_OVERLAY		 _IOW('V', 14, int)
#define VIDIOC_QBUF		_IOWR('V', 15, struct v4l2_buffer)
#define VIDIOC_EXPBUF		_IOWR('V', 16, struct v4l2_exportbuffer)
#define VIDIOC_DQBUF		_IOWR('V', 17, struct v4l2_buffer)
#define VIDIOC_STREAMON		 _IOW('V', 18, int)
#define VIDIOC_STREAMOFF	 _IOW('V', 19, int)
#define VIDIOC_G_PARM		_IOWR('V', 21, struct v4l2_streamparm)
#define VIDIOC_S_PARM		_IOWR('V', 22, struct v4l2_streamparm)
#define VIDIOC_G_STD		 _IOR('V', 23, v4l2_std_id)
#define VIDIOC_S_STD		 _IOW('V', 24, v4l2_std_id)
#define VIDIOC_ENUMSTD		_IOWR('V', 25, struct v4l2_standard)
#define VIDIOC_ENUMINPUT	_IOWR('V', 26, struct v4l2_input)
#define VIDIOC_G_CTRL		_IOWR('V', 27, struct v4l2_control)
#define VIDIOC_S_CTRL		_IOWR('V', 28, struct v4l2_control)
#define VIDIOC_G_TUNER		_IOWR('V', 29, struct v4l2_tuner)
#define VIDIOC_S_TUNER		 _IOW('V', 30, struct v4l2_tuner)
#define VIDIOC_G_AUDIO		 _IOR('V', 33, struct v4l2_audio)
#define VIDIOC_S_AUDIO		 _IOW('V', 34, struct v4l2_audio)
#define VIDIOC_QUERYCTRL	_IOWR('V', 36, struct v4l2_queryctrl)
#define VIDIOC_QUERYMENU	_IOWR('V', 37, struct v4l2_querymenu)
#define VIDIOC_G_INPUT		 _IOR('V', 38, int)
#define VIDIOC_S_INPUT		_IOWR('V', 39, int)
#define VIDIOC_G_EDID		_IOWR('V', 40, struct v4l2_edid)
#define VIDIOC_S_EDID		_IOWR('V', 41, struct v4l2_edid)
#define VIDIOC_G_OUTPUT		 _IOR('V', 46, int)
#define VIDIOC_S_OUTPUT		_IOWR('V', 47, int)
#define VIDIOC_ENUMOUTPUT	_IOWR('V', 48, struct v4l2_output)
#define VIDIOC_G_AUDOUT		 _IOR('V', 49, struct v4l2_audioout)
#define VIDIOC_S_AUDOUT		 _IOW('V', 50, struct v4l2_audioout)
#define VIDIOC_G_MODULATOR	_IOWR('V', 54, struct v4l2_modulator)
#define VIDIOC_S_MODULATOR	 _IOW('V', 55, struct v4l2_modulator)
#define VIDIOC_G_FREQUENCY	_IOWR('V', 56, struct v4l2_frequency)
#define VIDIOC_S_FREQUENCY	 _IOW('V', 57, struct v4l2_frequency)
#define VIDIOC_CROPCAP		_IOWR('V', 58, struct v4l2_cropcap)
#define VIDIOC_G_CROP		_IOWR('V', 59, struct v4l2_crop)
#define VIDIOC_S_CROP		 _IOW('V', 60, struct v4l2_crop)
#define VIDIOC_G_JPEGCOMP	 _IOR('V', 61, struct v4l2_jpegcompression)
#define VIDIOC_S_JPEGCOMP	 _IOW('V', 62, struct v4l2_jpegcompression)
#define VIDIOC_QUERYSTD      	 _IOR('V', 63, v4l2_std_id)
#define VIDIOC_TRY_FMT      	_IOWR('V', 64, struct v4l2_format)
#define VIDIOC_ENUMAUDIO	_IOWR('V', 65, struct v4l2_audio)
#define VIDIOC_ENUMAUDOUT	_IOWR('V', 66, struct v4l2_audioout)
#define VIDIOC_G_PRIORITY	 _IOR('V', 67, __u32) /* enum v4l2_priority */
#define VIDIOC_S_PRIORITY	 _IOW('V', 68, __u32) /* enum v4l2_priority */
#define VIDIOC_G_SLICED_VBI_CAP _IOWR('V', 69, struct v4l2_sliced_vbi_cap)
#define VIDIOC_LOG_STATUS         _IO('V', 70)
#define VIDIOC_G_EXT_CTRLS	_IOWR('V', 71, struct v4l2_ext_controls)
#define VIDIOC_S_EXT_CTRLS	_IOWR('V', 72, struct v4l2_ext_controls)
#define VIDIOC_TRY_EXT_CTRLS	_IOWR('V', 73, struct v4l2_ext_controls)
#define VIDIOC_ENUM_FRAMESIZES	_IOWR('V', 74, struct v4l2_frmsizeenum)
#define VIDIOC_ENUM_FRAMEINTERVALS _IOWR('V', 75, struct v4l2_frmivalenum)
#define VIDIOC_G_ENC_INDEX       _IOR('V', 76, struct v4l2_enc_idx)
#define VIDIOC_ENCODER_CMD      _IOWR('V', 77, struct v4l2_encoder_cmd)
#define VIDIOC_TRY_ENCODER_CMD  _IOWR('V', 78, struct v4l2_encoder_cmd)

/* Experimental, meant for debugging, testing and internal use.
Only implemented if CONFIG_VIDEO_ADV_DEBUG is defined.
You must be root to use these ioctls. Never use these in applications! */
#define	VIDIOC_DBG_S_REGISTER 	 _IOW('V', 79, struct v4l2_dbg_register)
#define	VIDIOC_DBG_G_REGISTER 	_IOWR('V', 80, struct v4l2_dbg_register)

#define VIDIOC_S_HW_FREQ_SEEK	 _IOW('V', 82, struct v4l2_hw_freq_seek)

#define	VIDIOC_S_DV_TIMINGS	_IOWR('V', 87, struct v4l2_dv_timings)
#define	VIDIOC_G_DV_TIMINGS	_IOWR('V', 88, struct v4l2_dv_timings)
#define	VIDIOC_DQEVENT		 _IOR('V', 89, struct v4l2_event)
#define	VIDIOC_SUBSCRIBE_EVENT	 _IOW('V', 90, struct v4l2_event_subscription)
#define	VIDIOC_UNSUBSCRIBE_EVENT _IOW('V', 91, struct v4l2_event_subscription)

/* Experimental, the below two ioctls may change over the next couple of kernel
versions */
#define VIDIOC_CREATE_BUFS	_IOWR('V', 92, struct v4l2_create_buffers)
#define VIDIOC_PREPARE_BUF	_IOWR('V', 93, struct v4l2_buffer)

/* Experimental selection API */
#define VIDIOC_G_SELECTION	_IOWR('V', 94, struct v4l2_selection)
#define VIDIOC_S_SELECTION	_IOWR('V', 95, struct v4l2_selection)

/* Experimental, these two ioctls may change over the next couple of kernel
versions. */
#define VIDIOC_DECODER_CMD	_IOWR('V', 96, struct v4l2_decoder_cmd)
#define VIDIOC_TRY_DECODER_CMD	_IOWR('V', 97, struct v4l2_decoder_cmd)

/* Experimental, these three ioctls may change over the next couple of kernel
versions. */
#define VIDIOC_ENUM_DV_TIMINGS  _IOWR('V', 98, struct v4l2_enum_dv_timings)
#define VIDIOC_QUERY_DV_TIMINGS  _IOR('V', 99, struct v4l2_dv_timings)
#define VIDIOC_DV_TIMINGS_CAP   _IOWR('V', 100, struct v4l2_dv_timings_cap)

/* Experimental, this ioctl may change over the next couple of kernel
versions. */
#define VIDIOC_ENUM_FREQ_BANDS	_IOWR('V', 101, struct v4l2_frequency_band)

/* Experimental, meant for debugging, testing and internal use.
Never use these in applications! */
#define VIDIOC_DBG_G_CHIP_INFO  _IOWR('V', 102, struct v4l2_dbg_chip_info)


/*
* Common stuff for both V4L1 and V4L2
* Moved from videodev.h
*/
#define VIDEO_MAX_FRAME               32
#define VIDEO_MAX_PLANES               8

/*
*	M I S C E L L A N E O U S
*/

/*  Four-character-code (FOURCC) */
#define v4l2_fourcc(a, b, c, d)\
	((__u32)(a) | ((__u32)(b) << 8) | ((__u32)(c) << 16) | ((__u32)(d) << 24))
#define v4l2_fourcc_be(a, b, c, d)	(v4l2_fourcc(a, b, c, d) | (1 << 31))

/*
*	E N U M S
*/
enum v4l2_field {
	V4L2_FIELD_ANY = 0, /* driver can choose from none,
						top, bottom, interlaced
						depending on whatever it thinks
						is approximate ... */
						V4L2_FIELD_NONE = 1, /* this device has no fields ... */
						V4L2_FIELD_TOP = 2, /* top field only */
						V4L2_FIELD_BOTTOM = 3, /* bottom field only */
						V4L2_FIELD_INTERLACED = 4, /* both fields interlaced */
						V4L2_FIELD_SEQ_TB = 5, /* both fields sequential into one
											   buffer, top-bottom order */
											   V4L2_FIELD_SEQ_BT = 6, /* same as above + bottom-top order */
											   V4L2_FIELD_ALTERNATE = 7, /* both fields alternating into
																		 separate buffers */
																		 V4L2_FIELD_INTERLACED_TB = 8, /* both fields interlaced, top field
																									   first and the top field is
																									   transmitted first */
																									   V4L2_FIELD_INTERLACED_BT = 9, /* both fields interlaced, top field
																																	 first and the bottom field is
																																	 transmitted first */
};
#define V4L2_FIELD_HAS_TOP(field)	\
	((field) == V4L2_FIELD_TOP 	||\
	 (field) == V4L2_FIELD_INTERLACED ||\
	 (field) == V4L2_FIELD_INTERLACED_TB ||\
	 (field) == V4L2_FIELD_INTERLACED_BT ||\
	 (field) == V4L2_FIELD_SEQ_TB	||\
	 (field) == V4L2_FIELD_SEQ_BT)
#define V4L2_FIELD_HAS_BOTTOM(field)	\
	((field) == V4L2_FIELD_BOTTOM 	||\
	 (field) == V4L2_FIELD_INTERLACED ||\
	 (field) == V4L2_FIELD_INTERLACED_TB ||\
	 (field) == V4L2_FIELD_INTERLACED_BT ||\
	 (field) == V4L2_FIELD_SEQ_TB	||\
	 (field) == V4L2_FIELD_SEQ_BT)
#define V4L2_FIELD_HAS_BOTH(field)	\
	((field) == V4L2_FIELD_INTERLACED ||\
	 (field) == V4L2_FIELD_INTERLACED_TB ||\
	 (field) == V4L2_FIELD_INTERLACED_BT ||\
	 (field) == V4L2_FIELD_SEQ_TB ||\
	 (field) == V4L2_FIELD_SEQ_BT)
#define V4L2_FIELD_HAS_T_OR_B(field)	\
	((field) == V4L2_FIELD_BOTTOM ||\
	 (field) == V4L2_FIELD_TOP ||\
	 (field) == V4L2_FIELD_ALTERNATE)

enum v4l2_buf_type {
	V4L2_BUF_TYPE_VIDEO_CAPTURE = 1,
	V4L2_BUF_TYPE_VIDEO_OUTPUT = 2,
	V4L2_BUF_TYPE_VIDEO_OVERLAY = 3,
	V4L2_BUF_TYPE_VBI_CAPTURE = 4,
	V4L2_BUF_TYPE_VBI_OUTPUT = 5,
	V4L2_BUF_TYPE_SLICED_VBI_CAPTURE = 6,
	V4L2_BUF_TYPE_SLICED_VBI_OUTPUT = 7,
#if 1
	/* Experimental */
	V4L2_BUF_TYPE_VIDEO_OUTPUT_OVERLAY = 8,
#endif
	V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE = 9,
	V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE = 10,
	V4L2_BUF_TYPE_SDR_CAPTURE = 11,
	/* Deprecated, do not use */
	V4L2_BUF_TYPE_PRIVATE = 0x80,
};

#define V4L2_TYPE_IS_MULTIPLANAR(type)			\
	((type) == V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE	\
	 || (type) == V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE)

#define V4L2_TYPE_IS_OUTPUT(type)				\
	((type) == V4L2_BUF_TYPE_VIDEO_OUTPUT			\
	 || (type) == V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE		\
	 || (type) == V4L2_BUF_TYPE_VIDEO_OVERLAY		\
	 || (type) == V4L2_BUF_TYPE_VIDEO_OUTPUT_OVERLAY	\
	 || (type) == V4L2_BUF_TYPE_VBI_OUTPUT			\
	 || (type) == V4L2_BUF_TYPE_SLICED_VBI_OUTPUT)

enum v4l2_tuner_type {
	V4L2_TUNER_RADIO = 1,
	V4L2_TUNER_ANALOG_TV = 2,
	V4L2_TUNER_DIGITAL_TV = 3,
	V4L2_TUNER_ADC = 4,
	V4L2_TUNER_RF = 5,
};

enum v4l2_memory {
	V4L2_MEMORY_MMAP = 1,
	V4L2_MEMORY_USERPTR = 2,
	V4L2_MEMORY_OVERLAY = 3,
	V4L2_MEMORY_DMABUF = 4,
};

/* see also http://vektor.theorem.ca/graphics/ycbcr/ */
enum v4l2_colorspace {
	/* SMPTE 170M: used for broadcast NTSC/PAL SDTV */
	V4L2_COLORSPACE_SMPTE170M = 1,

	/* Obsolete pre-1998 SMPTE 240M HDTV standard, superseded by Rec 709 */
	V4L2_COLORSPACE_SMPTE240M = 2,

	/* Rec.709: used for HDTV */
	V4L2_COLORSPACE_REC709 = 3,

	/*
	* Deprecated, do not use. No driver will ever return this. This was
	* based on a misunderstanding of the bt878 datasheet.
	*/
	V4L2_COLORSPACE_BT878 = 4,

	/*
	* NTSC 1953 colorspace. This only makes sense when dealing with
	* really, really old NTSC recordings. Superseded by SMPTE 170M.
	*/
	V4L2_COLORSPACE_470_SYSTEM_M = 5,

	/*
	* EBU Tech 3213 PAL/SECAM colorspace. This only makes sense when
	* dealing with really old PAL/SECAM recordings. Superseded by
	* SMPTE 170M.
	*/
	V4L2_COLORSPACE_470_SYSTEM_BG = 6,

	/*
	* Effectively shorthand for V4L2_COLORSPACE_SRGB, V4L2_YCBCR_ENC_601
	* and V4L2_QUANTIZATION_FULL_RANGE. To be used for (Motion-)JPEG.
	*/
	V4L2_COLORSPACE_JPEG = 7,

	/* For RGB colorspaces such as produces by most webcams. */
	V4L2_COLORSPACE_SRGB = 8,

	/* AdobeRGB colorspace */
	V4L2_COLORSPACE_ADOBERGB = 9,

	/* BT.2020 colorspace, used for UHDTV. */
	V4L2_COLORSPACE_BT2020 = 10,
};

enum v4l2_ycbcr_encoding {
	/*
	* Mapping of V4L2_YCBCR_ENC_DEFAULT to actual encodings for the
	* various colorspaces:
	*
	* V4L2_COLORSPACE_SMPTE170M, V4L2_COLORSPACE_470_SYSTEM_M,
	* V4L2_COLORSPACE_470_SYSTEM_BG, V4L2_COLORSPACE_ADOBERGB and
	* V4L2_COLORSPACE_JPEG: V4L2_YCBCR_ENC_601
	*
	* V4L2_COLORSPACE_REC709: V4L2_YCBCR_ENC_709
	*
	* V4L2_COLORSPACE_SRGB: V4L2_YCBCR_ENC_SYCC
	*
	* V4L2_COLORSPACE_BT2020: V4L2_YCBCR_ENC_BT2020
	*
	* V4L2_COLORSPACE_SMPTE240M: V4L2_YCBCR_ENC_SMPTE240M
	*/
	V4L2_YCBCR_ENC_DEFAULT = 0,

	/* ITU-R 601 -- SDTV */
	V4L2_YCBCR_ENC_601 = 1,

	/* Rec. 709 -- HDTV */
	V4L2_YCBCR_ENC_709 = 2,

	/* ITU-R 601/EN 61966-2-4 Extended Gamut -- SDTV */
	V4L2_YCBCR_ENC_XV601 = 3,

	/* Rec. 709/EN 61966-2-4 Extended Gamut -- HDTV */
	V4L2_YCBCR_ENC_XV709 = 4,

	/* sYCC (Y'CbCr encoding of sRGB) */
	V4L2_YCBCR_ENC_SYCC = 5,

	/* BT.2020 Non-constant Luminance Y'CbCr */
	V4L2_YCBCR_ENC_BT2020 = 6,

	/* BT.2020 Constant Luminance Y'CbcCrc */
	V4L2_YCBCR_ENC_BT2020_CONST_LUM = 7,

	/* SMPTE 240M -- Obsolete HDTV */
	V4L2_YCBCR_ENC_SMPTE240M = 8,
};

enum v4l2_quantization {
	/*
	* The default for R'G'B' quantization is always full range, except
	* for the BT2020 colorspace. For Y'CbCr the quantization is always
	* limited range, except for COLORSPACE_JPEG, SYCC, XV601 or XV709:
	* those are full range.
	*/
	V4L2_QUANTIZATION_DEFAULT = 0,
	V4L2_QUANTIZATION_FULL_RANGE = 1,
	V4L2_QUANTIZATION_LIM_RANGE = 2,
};

enum v4l2_priority {
	V4L2_PRIORITY_UNSET = 0,  /* not initialized */
	V4L2_PRIORITY_BACKGROUND = 1,
	V4L2_PRIORITY_INTERACTIVE = 2,
	V4L2_PRIORITY_RECORD = 3,
	V4L2_PRIORITY_DEFAULT = V4L2_PRIORITY_INTERACTIVE,
};

struct v4l2_rect {
	__s32   left;
	__s32   top;
	__u32   width;
	__u32   height;
};

struct v4l2_fract {
	__u32   numerator;
	__u32   denominator;
};

/**
* struct v4l2_capability - Describes V4L2 device caps returned by VIDIOC_QUERYCAP
*
* @driver:	   name of the driver module (e.g. "bttv")
* @card:	   name of the card (e.g. "Hauppauge WinTV")
* @bus_info:	   name of the bus (e.g. "PCI:" + pci_name(pci_dev) )
* @version:	   KERNEL_VERSION
* @capabilities: capabilities of the physical device as a whole
* @device_caps:  capabilities accessed via this particular device (node)
* @reserved:	   reserved fields for future extensions
*/
struct v4l2_capability {
	__u8	driver[16];
	__u8	card[32];
	__u8	bus_info[32];
	__u32   version;
	__u32	capabilities;
	__u32	device_caps;
	__u32	reserved[3];
};

/* Values for 'capabilities' field */
#define V4L2_CAP_VIDEO_CAPTURE		0x00000001  /* Is a video capture device */
#define V4L2_CAP_VIDEO_OUTPUT		0x00000002  /* Is a video output device */
#define V4L2_CAP_VIDEO_OVERLAY		0x00000004  /* Can do video overlay */
#define V4L2_CAP_VBI_CAPTURE		0x00000010  /* Is a raw VBI capture device */
#define V4L2_CAP_VBI_OUTPUT		0x00000020  /* Is a raw VBI output device */
#define V4L2_CAP_SLICED_VBI_CAPTURE	0x00000040  /* Is a sliced VBI capture device */
#define V4L2_CAP_SLICED_VBI_OUTPUT	0x00000080  /* Is a sliced VBI output device */
#define V4L2_CAP_RDS_CAPTURE		0x00000100  /* RDS data capture */
#define V4L2_CAP_VIDEO_OUTPUT_OVERLAY	0x00000200  /* Can do video output overlay */
#define V4L2_CAP_HW_FREQ_SEEK		0x00000400  /* Can do hardware frequency seek  */
#define V4L2_CAP_RDS_OUTPUT		0x00000800  /* Is an RDS encoder */

/* Is a video capture device that supports multiplanar formats */
#define V4L2_CAP_VIDEO_CAPTURE_MPLANE	0x00001000
/* Is a video output device that supports multiplanar formats */
#define V4L2_CAP_VIDEO_OUTPUT_MPLANE	0x00002000
/* Is a video mem-to-mem device that supports multiplanar formats */
#define V4L2_CAP_VIDEO_M2M_MPLANE	0x00004000
/* Is a video mem-to-mem device */
#define V4L2_CAP_VIDEO_M2M		0x00008000

#define V4L2_CAP_TUNER			0x00010000  /* has a tuner */
#define V4L2_CAP_AUDIO			0x00020000  /* has audio support */
#define V4L2_CAP_RADIO			0x00040000  /* is a radio device */
#define V4L2_CAP_MODULATOR		0x00080000  /* has a modulator */

#define V4L2_CAP_SDR_CAPTURE		0x00100000  /* Is a SDR capture device */
#define V4L2_CAP_EXT_PIX_FORMAT		0x00200000  /* Supports the extended pixel format */

#define V4L2_CAP_READWRITE              0x01000000  /* read/write systemcalls */
#define V4L2_CAP_ASYNCIO                0x02000000  /* async I/O */
#define V4L2_CAP_STREAMING              0x04000000  /* streaming I/O ioctls */

#define V4L2_CAP_DEVICE_CAPS            0x80000000  /* sets device capabilities field */

/*
*	V I D E O   I M A G E   F O R M A T
*/
struct v4l2_pix_format {
	__u32         		width;
	__u32			height;
	__u32			pixelformat;
	__u32			field;		/* enum v4l2_field */
	__u32            	bytesperline;	/* for padding, zero if unused */
	__u32          		sizeimage;
	__u32			colorspace;	/* enum v4l2_colorspace */
	__u32			priv;		/* private data, depends on pixelformat */
	__u32			flags;		/* format flags (V4L2_PIX_FMT_FLAG_*) */
	__u32			ycbcr_enc;	/* enum v4l2_ycbcr_encoding */
	__u32			quantization;	/* enum v4l2_quantization */
};

/*      Pixel format         FOURCC                          depth  Description  */

/* RGB formats */
#define V4L2_PIX_FMT_RGB332  v4l2_fourcc('R', 'G', 'B', '1') /*  8  RGB-3-3-2     */
#define V4L2_PIX_FMT_RGB444  v4l2_fourcc('R', '4', '4', '4') /* 16  xxxxrrrr ggggbbbb */
#define V4L2_PIX_FMT_ARGB444 v4l2_fourcc('A', 'R', '1', '2') /* 16  aaaarrrr ggggbbbb */
#define V4L2_PIX_FMT_XRGB444 v4l2_fourcc('X', 'R', '1', '2') /* 16  xxxxrrrr ggggbbbb */
#define V4L2_PIX_FMT_RGB555  v4l2_fourcc('R', 'G', 'B', 'O') /* 16  RGB-5-5-5     */
#define V4L2_PIX_FMT_ARGB555 v4l2_fourcc('A', 'R', '1', '5') /* 16  ARGB-1-5-5-5  */
#define V4L2_PIX_FMT_XRGB555 v4l2_fourcc('X', 'R', '1', '5') /* 16  XRGB-1-5-5-5  */
#define V4L2_PIX_FMT_RGB565  v4l2_fourcc('R', 'G', 'B', 'P') /* 16  RGB-5-6-5     */
#define V4L2_PIX_FMT_RGB555X v4l2_fourcc('R', 'G', 'B', 'Q') /* 16  RGB-5-5-5 BE  */
#define V4L2_PIX_FMT_ARGB555X v4l2_fourcc_be('A', 'R', '1', '5') /* 16  ARGB-5-5-5 BE */
#define V4L2_PIX_FMT_XRGB555X v4l2_fourcc_be('X', 'R', '1', '5') /* 16  XRGB-5-5-5 BE */
#define V4L2_PIX_FMT_RGB565X v4l2_fourcc('R', 'G', 'B', 'R') /* 16  RGB-5-6-5 BE  */
#define V4L2_PIX_FMT_BGR666  v4l2_fourcc('B', 'G', 'R', 'H') /* 18  BGR-6-6-6	  */
#define V4L2_PIX_FMT_BGR24   v4l2_fourcc('B', 'G', 'R', '3') /* 24  BGR-8-8-8     */
#define V4L2_PIX_FMT_RGB24   v4l2_fourcc('R', 'G', 'B', '3') /* 24  RGB-8-8-8     */
#define V4L2_PIX_FMT_BGR32   v4l2_fourcc('B', 'G', 'R', '4') /* 32  BGR-8-8-8-8   */
#define V4L2_PIX_FMT_ABGR32  v4l2_fourcc('A', 'R', '2', '4') /* 32  BGRA-8-8-8-8  */
#define V4L2_PIX_FMT_XBGR32  v4l2_fourcc('X', 'R', '2', '4') /* 32  BGRX-8-8-8-8  */
#define V4L2_PIX_FMT_RGB32   v4l2_fourcc('R', 'G', 'B', '4') /* 32  RGB-8-8-8-8   */
#define V4L2_PIX_FMT_ARGB32  v4l2_fourcc('B', 'A', '2', '4') /* 32  ARGB-8-8-8-8  */
#define V4L2_PIX_FMT_XRGB32  v4l2_fourcc('B', 'X', '2', '4') /* 32  XRGB-8-8-8-8  */

/* Grey formats */
#define V4L2_PIX_FMT_GREY    v4l2_fourcc('G', 'R', 'E', 'Y') /*  8  Greyscale     */
#define V4L2_PIX_FMT_Y4      v4l2_fourcc('Y', '0', '4', ' ') /*  4  Greyscale     */
#define V4L2_PIX_FMT_Y6      v4l2_fourcc('Y', '0', '6', ' ') /*  6  Greyscale     */
#define V4L2_PIX_FMT_Y10     v4l2_fourcc('Y', '1', '0', ' ') /* 10  Greyscale     */
#define V4L2_PIX_FMT_Y12     v4l2_fourcc('Y', '1', '2', ' ') /* 12  Greyscale     */
#define V4L2_PIX_FMT_Y16     v4l2_fourcc('Y', '1', '6', ' ') /* 16  Greyscale     */

/* Grey bit-packed formats */
#define V4L2_PIX_FMT_Y10BPACK    v4l2_fourcc('Y', '1', '0', 'B') /* 10  Greyscale bit-packed */

/* Palette formats */
#define V4L2_PIX_FMT_PAL8    v4l2_fourcc('P', 'A', 'L', '8') /*  8  8-bit palette */

/* Chrominance formats */
#define V4L2_PIX_FMT_UV8     v4l2_fourcc('U', 'V', '8', ' ') /*  8  UV 4:4 */

/* Luminance+Chrominance formats */
#define V4L2_PIX_FMT_YVU410  v4l2_fourcc('Y', 'V', 'U', '9') /*  9  YVU 4:1:0     */
#define V4L2_PIX_FMT_YVU420  v4l2_fourcc('Y', 'V', '1', '2') /* 12  YVU 4:2:0     */
#define V4L2_PIX_FMT_YUYV    v4l2_fourcc('Y', 'U', 'Y', 'V') /* 16  YUV 4:2:2     */
#define V4L2_PIX_FMT_YYUV    v4l2_fourcc('Y', 'Y', 'U', 'V') /* 16  YUV 4:2:2     */
#define V4L2_PIX_FMT_YVYU    v4l2_fourcc('Y', 'V', 'Y', 'U') /* 16 YVU 4:2:2 */
#define V4L2_PIX_FMT_UYVY    v4l2_fourcc('U', 'Y', 'V', 'Y') /* 16  YUV 4:2:2     */
#define V4L2_PIX_FMT_VYUY    v4l2_fourcc('V', 'Y', 'U', 'Y') /* 16  YUV 4:2:2     */
#define V4L2_PIX_FMT_YUV422P v4l2_fourcc('4', '2', '2', 'P') /* 16  YVU422 planar */
#define V4L2_PIX_FMT_YUV411P v4l2_fourcc('4', '1', '1', 'P') /* 16  YVU411 planar */
#define V4L2_PIX_FMT_Y41P    v4l2_fourcc('Y', '4', '1', 'P') /* 12  YUV 4:1:1     */
#define V4L2_PIX_FMT_YUV444  v4l2_fourcc('Y', '4', '4', '4') /* 16  xxxxyyyy uuuuvvvv */
#define V4L2_PIX_FMT_YUV555  v4l2_fourcc('Y', 'U', 'V', 'O') /* 16  YUV-5-5-5     */
#define V4L2_PIX_FMT_YUV565  v4l2_fourcc('Y', 'U', 'V', 'P') /* 16  YUV-5-6-5     */
#define V4L2_PIX_FMT_YUV32   v4l2_fourcc('Y', 'U', 'V', '4') /* 32  YUV-8-8-8-8   */
#define V4L2_PIX_FMT_YUV410  v4l2_fourcc('Y', 'U', 'V', '9') /*  9  YUV 4:1:0     */
#define V4L2_PIX_FMT_YUV420  v4l2_fourcc('Y', 'U', '1', '2') /* 12  YUV 4:2:0     */
#define V4L2_PIX_FMT_HI240   v4l2_fourcc('H', 'I', '2', '4') /*  8  8-bit color   */
#define V4L2_PIX_FMT_HM12    v4l2_fourcc('H', 'M', '1', '2') /*  8  YUV 4:2:0 16x16 macroblocks */
#define V4L2_PIX_FMT_M420    v4l2_fourcc('M', '4', '2', '0') /* 12  YUV 4:2:0 2 lines y, 1 line uv interleaved */

/* two planes -- one Y, one Cr + Cb interleaved  */
#define V4L2_PIX_FMT_NV12    v4l2_fourcc('N', 'V', '1', '2') /* 12  Y/CbCr 4:2:0  */
#define V4L2_PIX_FMT_NV21    v4l2_fourcc('N', 'V', '2', '1') /* 12  Y/CrCb 4:2:0  */
#define V4L2_PIX_FMT_NV16    v4l2_fourcc('N', 'V', '1', '6') /* 16  Y/CbCr 4:2:2  */
#define V4L2_PIX_FMT_NV61    v4l2_fourcc('N', 'V', '6', '1') /* 16  Y/CrCb 4:2:2  */
#define V4L2_PIX_FMT_NV24    v4l2_fourcc('N', 'V', '2', '4') /* 24  Y/CbCr 4:4:4  */
#define V4L2_PIX_FMT_NV42    v4l2_fourcc('N', 'V', '4', '2') /* 24  Y/CrCb 4:4:4  */

/* two non contiguous planes - one Y, one Cr + Cb interleaved  */
#define V4L2_PIX_FMT_NV12M   v4l2_fourcc('N', 'M', '1', '2') /* 12  Y/CbCr 4:2:0  */
#define V4L2_PIX_FMT_NV21M   v4l2_fourcc('N', 'M', '2', '1') /* 21  Y/CrCb 4:2:0  */
#define V4L2_PIX_FMT_NV16M   v4l2_fourcc('N', 'M', '1', '6') /* 16  Y/CbCr 4:2:2  */
#define V4L2_PIX_FMT_NV61M   v4l2_fourcc('N', 'M', '6', '1') /* 16  Y/CrCb 4:2:2  */
#define V4L2_PIX_FMT_NV12MT  v4l2_fourcc('T', 'M', '1', '2') /* 12  Y/CbCr 4:2:0 64x32 macroblocks */
#define V4L2_PIX_FMT_NV12MT_16X16 v4l2_fourcc('V', 'M', '1', '2') /* 12  Y/CbCr 4:2:0 16x16 macroblocks */

/* three non contiguous planes - Y, Cb, Cr */
#define V4L2_PIX_FMT_YUV420M v4l2_fourcc('Y', 'M', '1', '2') /* 12  YUV420 planar */
#define V4L2_PIX_FMT_YVU420M v4l2_fourcc('Y', 'M', '2', '1') /* 12  YVU420 planar */

/* Bayer formats - see http://www.siliconimaging.com/RGB%20Bayer.htm */
#define V4L2_PIX_FMT_SBGGR8  v4l2_fourcc('B', 'A', '8', '1') /*  8  BGBG.. GRGR.. */
#define V4L2_PIX_FMT_SGBRG8  v4l2_fourcc('G', 'B', 'R', 'G') /*  8  GBGB.. RGRG.. */
#define V4L2_PIX_FMT_SGRBG8  v4l2_fourcc('G', 'R', 'B', 'G') /*  8  GRGR.. BGBG.. */
#define V4L2_PIX_FMT_SRGGB8  v4l2_fourcc('R', 'G', 'G', 'B') /*  8  RGRG.. GBGB.. */
#define V4L2_PIX_FMT_SBGGR10 v4l2_fourcc('B', 'G', '1', '0') /* 10  BGBG.. GRGR.. */
#define V4L2_PIX_FMT_SGBRG10 v4l2_fourcc('G', 'B', '1', '0') /* 10  GBGB.. RGRG.. */
#define V4L2_PIX_FMT_SGRBG10 v4l2_fourcc('B', 'A', '1', '0') /* 10  GRGR.. BGBG.. */
#define V4L2_PIX_FMT_SRGGB10 v4l2_fourcc('R', 'G', '1', '0') /* 10  RGRG.. GBGB.. */
/* 10bit raw bayer packed, 5 bytes for every 4 pixels */
#define V4L2_PIX_FMT_SBGGR10P v4l2_fourcc('p', 'B', 'A', 'A')
#define V4L2_PIX_FMT_SGBRG10P v4l2_fourcc('p', 'G', 'A', 'A')
#define V4L2_PIX_FMT_SGRBG10P v4l2_fourcc('p', 'g', 'A', 'A')
#define V4L2_PIX_FMT_SRGGB10P v4l2_fourcc('p', 'R', 'A', 'A')
/* 10bit raw bayer a-law compressed to 8 bits */
#define V4L2_PIX_FMT_SBGGR10ALAW8 v4l2_fourcc('a', 'B', 'A', '8')
#define V4L2_PIX_FMT_SGBRG10ALAW8 v4l2_fourcc('a', 'G', 'A', '8')
#define V4L2_PIX_FMT_SGRBG10ALAW8 v4l2_fourcc('a', 'g', 'A', '8')
#define V4L2_PIX_FMT_SRGGB10ALAW8 v4l2_fourcc('a', 'R', 'A', '8')
/* 10bit raw bayer DPCM compressed to 8 bits */
#define V4L2_PIX_FMT_SBGGR10DPCM8 v4l2_fourcc('b', 'B', 'A', '8')
#define V4L2_PIX_FMT_SGBRG10DPCM8 v4l2_fourcc('b', 'G', 'A', '8')
#define V4L2_PIX_FMT_SGRBG10DPCM8 v4l2_fourcc('B', 'D', '1', '0')
#define V4L2_PIX_FMT_SRGGB10DPCM8 v4l2_fourcc('b', 'R', 'A', '8')
#define V4L2_PIX_FMT_SBGGR12 v4l2_fourcc('B', 'G', '1', '2') /* 12  BGBG.. GRGR.. */
#define V4L2_PIX_FMT_SGBRG12 v4l2_fourcc('G', 'B', '1', '2') /* 12  GBGB.. RGRG.. */
#define V4L2_PIX_FMT_SGRBG12 v4l2_fourcc('B', 'A', '1', '2') /* 12  GRGR.. BGBG.. */
#define V4L2_PIX_FMT_SRGGB12 v4l2_fourcc('R', 'G', '1', '2') /* 12  RGRG.. GBGB.. */
#define V4L2_PIX_FMT_SBGGR16 v4l2_fourcc('B', 'Y', 'R', '2') /* 16  BGBG.. GRGR.. */

/* compressed formats */
#define V4L2_PIX_FMT_MJPEG    v4l2_fourcc('M', 'J', 'P', 'G') /* Motion-JPEG   */
#define V4L2_PIX_FMT_JPEG     v4l2_fourcc('J', 'P', 'E', 'G') /* JFIF JPEG     */
#define V4L2_PIX_FMT_DV       v4l2_fourcc('d', 'v', 's', 'd') /* 1394          */
#define V4L2_PIX_FMT_MPEG     v4l2_fourcc('M', 'P', 'E', 'G') /* MPEG-1/2/4 Multiplexed */
#define V4L2_PIX_FMT_H264     v4l2_fourcc('H', '2', '6', '4') /* H264 with start codes */
#define V4L2_PIX_FMT_H264_NO_SC v4l2_fourcc('A', 'V', 'C', '1') /* H264 without start codes */
#define V4L2_PIX_FMT_H264_MVC v4l2_fourcc('M', '2', '6', '4') /* H264 MVC */
#define V4L2_PIX_FMT_H263     v4l2_fourcc('H', '2', '6', '3') /* H263          */
#define V4L2_PIX_FMT_MPEG1    v4l2_fourcc('M', 'P', 'G', '1') /* MPEG-1 ES     */
#define V4L2_PIX_FMT_MPEG2    v4l2_fourcc('M', 'P', 'G', '2') /* MPEG-2 ES     */
#define V4L2_PIX_FMT_MPEG4    v4l2_fourcc('M', 'P', 'G', '4') /* MPEG-4 part 2 ES */
#define V4L2_PIX_FMT_XVID     v4l2_fourcc('X', 'V', 'I', 'D') /* Xvid           */
#define V4L2_PIX_FMT_VC1_ANNEX_G v4l2_fourcc('V', 'C', '1', 'G') /* SMPTE 421M Annex G compliant stream */
#define V4L2_PIX_FMT_VC1_ANNEX_L v4l2_fourcc('V', 'C', '1', 'L') /* SMPTE 421M Annex L compliant stream */
#define V4L2_PIX_FMT_VP8      v4l2_fourcc('V', 'P', '8', '0') /* VP8 */

/*  Vendor-specific formats   */
#define V4L2_PIX_FMT_CPIA1    v4l2_fourcc('C', 'P', 'I', 'A') /* cpia1 YUV */
#define V4L2_PIX_FMT_WNVA     v4l2_fourcc('W', 'N', 'V', 'A') /* Winnov hw compress */
#define V4L2_PIX_FMT_SN9C10X  v4l2_fourcc('S', '9', '1', '0') /* SN9C10x compression */
#define V4L2_PIX_FMT_SN9C20X_I420 v4l2_fourcc('S', '9', '2', '0') /* SN9C20x YUV 4:2:0 */
#define V4L2_PIX_FMT_PWC1     v4l2_fourcc('P', 'W', 'C', '1') /* pwc older webcam */
#define V4L2_PIX_FMT_PWC2     v4l2_fourcc('P', 'W', 'C', '2') /* pwc newer webcam */
#define V4L2_PIX_FMT_ET61X251 v4l2_fourcc('E', '6', '2', '5') /* ET61X251 compression */
#define V4L2_PIX_FMT_SPCA501  v4l2_fourcc('S', '5', '0', '1') /* YUYV per line */
#define V4L2_PIX_FMT_SPCA505  v4l2_fourcc('S', '5', '0', '5') /* YYUV per line */
#define V4L2_PIX_FMT_SPCA508  v4l2_fourcc('S', '5', '0', '8') /* YUVY per line */
#define V4L2_PIX_FMT_SPCA561  v4l2_fourcc('S', '5', '6', '1') /* compressed GBRG bayer */
#define V4L2_PIX_FMT_PAC207   v4l2_fourcc('P', '2', '0', '7') /* compressed BGGR bayer */
#define V4L2_PIX_FMT_MR97310A v4l2_fourcc('M', '3', '1', '0') /* compressed BGGR bayer */
#define V4L2_PIX_FMT_JL2005BCD v4l2_fourcc('J', 'L', '2', '0') /* compressed RGGB bayer */
#define V4L2_PIX_FMT_SN9C2028 v4l2_fourcc('S', 'O', 'N', 'X') /* compressed GBRG bayer */
#define V4L2_PIX_FMT_SQ905C   v4l2_fourcc('9', '0', '5', 'C') /* compressed RGGB bayer */
#define V4L2_PIX_FMT_PJPG     v4l2_fourcc('P', 'J', 'P', 'G') /* Pixart 73xx JPEG */
#define V4L2_PIX_FMT_OV511    v4l2_fourcc('O', '5', '1', '1') /* ov511 JPEG */
#define V4L2_PIX_FMT_OV518    v4l2_fourcc('O', '5', '1', '8') /* ov518 JPEG */
#define V4L2_PIX_FMT_STV0680  v4l2_fourcc('S', '6', '8', '0') /* stv0680 bayer */
#define V4L2_PIX_FMT_TM6000   v4l2_fourcc('T', 'M', '6', '0') /* tm5600/tm60x0 */
#define V4L2_PIX_FMT_CIT_YYVYUY v4l2_fourcc('C', 'I', 'T', 'V') /* one line of Y then 1 line of VYUY */
#define V4L2_PIX_FMT_KONICA420  v4l2_fourcc('K', 'O', 'N', 'I') /* YUV420 planar in blocks of 256 pixels */
#define V4L2_PIX_FMT_JPGL	v4l2_fourcc('J', 'P', 'G', 'L') /* JPEG-Lite */
#define V4L2_PIX_FMT_SE401      v4l2_fourcc('S', '4', '0', '1') /* se401 janggu compressed rgb */
#define V4L2_PIX_FMT_S5C_UYVY_JPG v4l2_fourcc('S', '5', 'C', 'I') /* S5C73M3 interleaved UYVY/JPEG */

/* SDR formats - used only for Software Defined Radio devices */
#define V4L2_SDR_FMT_CU8          v4l2_fourcc('C', 'U', '0', '8') /* IQ u8 */
#define V4L2_SDR_FMT_CU16LE       v4l2_fourcc('C', 'U', '1', '6') /* IQ u16le */
#define V4L2_SDR_FMT_CS8          v4l2_fourcc('C', 'S', '0', '8') /* complex s8 */
#define V4L2_SDR_FMT_CS14LE       v4l2_fourcc('C', 'S', '1', '4') /* complex s14le */
#define V4L2_SDR_FMT_RU12LE       v4l2_fourcc('R', 'U', '1', '2') /* real u12le */

/* priv field value to indicates that subsequent fields are valid. */
#define V4L2_PIX_FMT_PRIV_MAGIC		0xfeedcafe

/* Flags */
#define V4L2_PIX_FMT_FLAG_PREMUL_ALPHA	0x00000001

/*
*	F O R M A T   E N U M E R A T I O N
*/
struct v4l2_fmtdesc {
	__u32		    index;             /* Format number      */
	__u32		    type;              /* enum v4l2_buf_type */
	__u32               flags;
	__u8		    description[32];   /* Description string */
	__u32		    pixelformat;       /* Format fourcc      */
	__u32		    reserved[4];
};

#define V4L2_FMT_FLAG_COMPRESSED 0x0001
#define V4L2_FMT_FLAG_EMULATED   0x0002

#if 1
/* Experimental Frame Size and frame rate enumeration */
/*
*	F R A M E   S I Z E   E N U M E R A T I O N
*/
enum v4l2_frmsizetypes {
	V4L2_FRMSIZE_TYPE_DISCRETE = 1,
	V4L2_FRMSIZE_TYPE_CONTINUOUS = 2,
	V4L2_FRMSIZE_TYPE_STEPWISE = 3,
};

struct v4l2_frmsize_discrete {
	__u32			width;		/* Frame width [pixel] */
	__u32			height;		/* Frame height [pixel] */
};

struct v4l2_frmsize_stepwise {
	__u32			min_width;	/* Minimum frame width [pixel] */
	__u32			max_width;	/* Maximum frame width [pixel] */
	__u32			step_width;	/* Frame width step size [pixel] */
	__u32			min_height;	/* Minimum frame height [pixel] */
	__u32			max_height;	/* Maximum frame height [pixel] */
	__u32			step_height;	/* Frame height step size [pixel] */
};

struct v4l2_frmsizeenum {
	__u32			index;		/* Frame size number */
	__u32			pixel_format;	/* Pixel format */
	__u32			type;		/* Frame size type the device supports. */

	union {					/* Frame size */
		struct v4l2_frmsize_discrete	discrete;
		struct v4l2_frmsize_stepwise	stepwise;
	};

	__u32   reserved[2];			/* Reserved space for future use */
};

/*
*	F R A M E   R A T E   E N U M E R A T I O N
*/
enum v4l2_frmivaltypes {
	V4L2_FRMIVAL_TYPE_DISCRETE = 1,
	V4L2_FRMIVAL_TYPE_CONTINUOUS = 2,
	V4L2_FRMIVAL_TYPE_STEPWISE = 3,
};

struct v4l2_frmival_stepwise {
	struct v4l2_fract	min;		/* Minimum frame interval [s] */
	struct v4l2_fract	max;		/* Maximum frame interval [s] */
	struct v4l2_fract	step;		/* Frame interval step size [s] */
};

struct v4l2_frmivalenum {
	__u32			index;		/* Frame format index */
	__u32			pixel_format;	/* Pixel format */
	__u32			width;		/* Frame width */
	__u32			height;		/* Frame height */
	__u32			type;		/* Frame interval type the device supports. */

	union {					/* Frame interval */
		struct v4l2_fract		discrete;
		struct v4l2_frmival_stepwise	stepwise;
	};

	__u32	reserved[2];			/* Reserved space for future use */
};
#endif

/*
*	T I M E C O D E
*/
struct v4l2_timecode {
	__u32	type;
	__u32	flags;
	__u8	frames;
	__u8	seconds;
	__u8	minutes;
	__u8	hours;
	__u8	userbits[4];
};

/*  Type  */
#define V4L2_TC_TYPE_24FPS		1
#define V4L2_TC_TYPE_25FPS		2
#define V4L2_TC_TYPE_30FPS		3
#define V4L2_TC_TYPE_50FPS		4
#define V4L2_TC_TYPE_60FPS		5


#endif























typedef std::map<std::string, std::string> dev_map;
typedef std::vector<std::string> dev_vec;

std::string v4l2_exception::getstrerror(int errnum)
{
	return strerror(errnum);
}

static bool is_v4l_dev(const char *name)
{
	
	return 	(memcmp(name, "video_", 6) < 0) &&
			!memcmp(name, "video", 5) ||
			!memcmp(name, "radio", 5) ||
			!memcmp(name, "vbi", 3) ||
			!memcmp(name, "v4l-subdev", 10);
}


int doioctl_name(int fd, unsigned long int request, void *parm, const char *name)
{
	int retval = ioctl(fd, request, parm);

	if (retval < 0)
		printf("%s: failed: %s\n", name, strerror(errno));
	//else if (verbose)
	//	printf("%s: ok\n", name);

	return retval;
}

#define doioctl(n, r, p) doioctl_name(n, r, p, #r)

static int calc_node_val(const char *s)
{
	int n = 0;

	s = strrchr(s, '/') + 1;
	if (!memcmp(s, "video", 5)) n = 0;
	else if (!memcmp(s, "radio", 5)) n = 0x100;
	else if (!memcmp(s, "vbi", 3)) n = 0x200;
	else if (!memcmp(s, "v4l-subdev", 10)) n = 0x300;
	n += atol(s + (n >= 0x200 ? 3 : 5));
	return n;
}

static bool sort_on_device_name(const std::string &s1, const std::string &s2)
{
	int n1 = calc_node_val(s1.c_str());
	int n2 = calc_node_val(s2.c_str());

	return n1 < n2;
}

//void v4l2_wrapper::listDevices()
//{
//	DIR *dp;
//	struct dirent *ep;
//	dev_vec files;
//	dev_map links;
//	dev_map cards;
//	struct v4l2_capability vcap;
//
//	dp = opendir("/dev");
//	if (dp == NULL) {
//		perror("Couldn't open the directory");
//		return;
//	}
//	while ((ep = readdir(dp)))
//		if (is_v4l_dev(ep->d_name))
//			files.push_back(std::string("/dev/") + ep->d_name);
//	closedir(dp);
//
//	/* Find device nodes which are links to other device nodes */
//	for (dev_vec::iterator iter = files.begin();
//		iter != files.end();) {
//		char link[64 + 1];
//		int link_len;
//		std::string target;
//
//		link_len = readlink(iter->c_str(), link, 64);
//		if (link_len < 0) {	/* Not a link or error */
//			iter++;
//			continue;
//		}
//		link[link_len] = '\0';
//
//		/* Only remove from files list if target itself is in list */
//		if (link[0] != '/')	/* Relative link */
//			target = std::string("/dev/");
//		target += link;
//		if (find(files.begin(), files.end(), target) == files.end()) {
//			iter++;
//			continue;
//		}
//
//		/* Move the device node from files to links */
//		if (links[target].empty())
//			links[target] = *iter;
//		else
//			links[target] += ", " + *iter;
//		files.erase(iter);
//	}
//
//	std::sort(files.begin(), files.end(), sort_on_device_name);
//
//	for (dev_vec::iterator iter = files.begin();
//		iter != files.end(); ++iter) {
//		int fd = open(iter->c_str(), O_RDWR);
//		std::string bus_info;
//
//		if (fd < 0)
//			continue;
//		doioctl(fd, VIDIOC_QUERYCAP, &vcap);
//		close(fd);
//		bus_info = (const char *)vcap.bus_info;
//		if (cards[bus_info].empty())
//			cards[bus_info] += std::string((char *)vcap.card) + " (" + bus_info + "):\n";
//		cards[bus_info] += "\t" + (*iter);
//		if (!(links[*iter].empty()))
//			cards[bus_info] += " <- " + links[*iter];
//		cards[bus_info] += "\n";
//	}
//	for (dev_map::iterator iter = cards.begin();
//		iter != cards.end(); ++iter) {
//		printf("%s\n", iter->second.c_str());
//	}
//}

//static std::string num2s(unsigned num)
//{
//	char buf[10];
//
//	sprintf(buf, "%08x", num);
//	return buf;
//}

//std::string buftype2s(int type)
//{
//	switch (type) {
//	case 0:
//		return "Invalid";
//	case V4L2_BUF_TYPE_VIDEO_CAPTURE:
//		return "Video Capture";
//	case V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE:
//		return "Video Capture Multiplanar";
//	case V4L2_BUF_TYPE_VIDEO_OUTPUT:
//		return "Video Output";
//	case V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE:
//		return "Video Output Multiplanar";
//	case V4L2_BUF_TYPE_VIDEO_OVERLAY:
//		return "Video Overlay";
//	case V4L2_BUF_TYPE_VBI_CAPTURE:
//		return "VBI Capture";
//	case V4L2_BUF_TYPE_VBI_OUTPUT:
//		return "VBI Output";
//	case V4L2_BUF_TYPE_SLICED_VBI_CAPTURE:
//		return "Sliced VBI Capture";
//	case V4L2_BUF_TYPE_SLICED_VBI_OUTPUT:
//		return "Sliced VBI Output";
//	case V4L2_BUF_TYPE_VIDEO_OUTPUT_OVERLAY:
//		return "Video Output Overlay";
//	case V4L2_BUF_TYPE_SDR_CAPTURE:
//		return "SDR Capture";
//	default:
//		return "Unknown (" + num2s(type) + ")";
//	}
//}
//
//std::string fcc2s(unsigned int val)
//{
//	std::string s;
//
//	s += val & 0xff;
//	s += (val >> 8) & 0xff;
//	s += (val >> 16) & 0xff;
//	s += (val >> 24) & 0xff;
//	return s;
//}

typedef struct {
	unsigned flag;
	const char *str;
} flag_def;

////workaround for android headers not having this flag
//#ifndef V4L2_FMT_FLAG_EMULATED
//#define V4L2_FMT_FLAG_EMULATED   0x0002
//#endif

static const flag_def fmtdesc_def[] = {
		{ V4L2_FMT_FLAG_COMPRESSED, "compressed" },
		{ V4L2_FMT_FLAG_EMULATED, "emulated" },
		{ 0, NULL }
};

//std::string flags2s(unsigned val, const flag_def *def)
//{
//	std::string s;
//
//	while (def->flag) {
//		if (val & def->flag) {
//			if (s.length()) s += ", ";
//			s += def->str;
//			val &= ~def->flag;
//		}
//		def++;
//	}
//	if (val) {
//		if (s.length()) s += ", ";
//		s += num2s(val);
//	}
//	return s;
//}
//
//
//
//std::string fmtdesc2s(unsigned flags)
//{
//	return flags2s(flags, fmtdesc_def);
//}

//static std::string frmtype2s(unsigned type)
//{
//	static const char *types[] = {
//		"Unknown",
//		"Discrete",
//		"Continuous",
//		"Stepwise"
//	};
//
//	if (type > 3)
//		type = 0;
//	return types[type];
//}

//static void print_frmsize(const struct v4l2_frmsizeenum &frmsize, const char *prefix)
//{
//	printf("%s\tSize: %s ", prefix, frmtype2s(frmsize.type).c_str());
//	if (frmsize.type == V4L2_FRMSIZE_TYPE_DISCRETE) {
//		printf("%dx%d", frmsize.discrete.width, frmsize.discrete.height);
//	}
//	else if (frmsize.type == V4L2_FRMSIZE_TYPE_STEPWISE) {
//		printf("%dx%d - %dx%d with step %d/%d",
//			frmsize.stepwise.min_width,
//			frmsize.stepwise.min_height,
//			frmsize.stepwise.max_width,
//			frmsize.stepwise.max_height,
//			frmsize.stepwise.step_width,
//			frmsize.stepwise.step_height);
//	}
//	printf("\n");
//}
//
//static std::string fract2sec(const struct v4l2_fract &f)
//{
//	char buf[100];
//
//	sprintf(buf, "%.3f", (1.0 * f.numerator) / f.denominator);
//	return buf;
//}
//
//static std::string fract2fps(const struct v4l2_fract &f)
//{
//	char buf[100];
//
//	sprintf(buf, "%.3f", (1.0 * f.denominator) / f.numerator);
//	return buf;
//}
//
//static void print_frmival(const struct v4l2_frmivalenum &frmival, const char *prefix)
//{
//	printf("%s\tInterval: %s ", prefix, frmtype2s(frmival.type).c_str());
//	if (frmival.type == V4L2_FRMIVAL_TYPE_DISCRETE) {
//		printf("%ss (%s fps)\n", fract2sec(frmival.discrete).c_str(),
//			fract2fps(frmival.discrete).c_str());
//	}
//	else if (frmival.type == V4L2_FRMIVAL_TYPE_STEPWISE) {
//		printf("%ss - %ss with step %ss (%s-%s fps)\n",
//			fract2sec(frmival.stepwise.min).c_str(),
//			fract2sec(frmival.stepwise.max).c_str(),
//			fract2sec(frmival.stepwise.step).c_str(),
//			fract2fps(frmival.stepwise.max).c_str(),
//			fract2fps(frmival.stepwise.min).c_str());
//	}
//}

struct fmt_map {
	enum ffmpegcpp::pixel_format ff_fmt;
	std::string codec_id;
	uint32_t v4l2_fmt;
};

const struct fmt_map avpriv_fmt_conversion_table[] = {
	//ff_fmt										codec_id              v4l2_fmt

		{ ffmpegcpp::pixel_format::PIX_FMT_YUV420P, "", V4L2_PIX_FMT_YUV420 },
		{ ffmpegcpp::pixel_format::PIX_FMT_YUV420P, "", V4L2_PIX_FMT_YVU420 },
		{ ffmpegcpp::pixel_format::PIX_FMT_YUV422P, "", V4L2_PIX_FMT_YUV422P },
		{ ffmpegcpp::pixel_format::PIX_FMT_YUYV422, "", V4L2_PIX_FMT_YUYV },
		{ ffmpegcpp::pixel_format::PIX_FMT_UYVY422, "", V4L2_PIX_FMT_UYVY },
		{ ffmpegcpp::pixel_format::PIX_FMT_YUV411P, "", V4L2_PIX_FMT_YUV411P },
		{ ffmpegcpp::pixel_format::PIX_FMT_YUV410P, "", V4L2_PIX_FMT_YUV410 },
		{ ffmpegcpp::pixel_format::PIX_FMT_YUV410P, "", V4L2_PIX_FMT_YVU410 },
		{ ffmpegcpp::pixel_format::PIX_FMT_RGB555LE, "", V4L2_PIX_FMT_RGB555 },
		{ ffmpegcpp::pixel_format::PIX_FMT_RGB555BE, "", V4L2_PIX_FMT_RGB555X },
		{ ffmpegcpp::pixel_format::PIX_FMT_RGB565LE, "", V4L2_PIX_FMT_RGB565 },
		{ ffmpegcpp::pixel_format::PIX_FMT_RGB565BE, "", V4L2_PIX_FMT_RGB565X },
		{ ffmpegcpp::pixel_format::PIX_FMT_BGR24, "", V4L2_PIX_FMT_BGR24 },
		{ ffmpegcpp::pixel_format::PIX_FMT_RGB24, "", V4L2_PIX_FMT_RGB24 },
		{ ffmpegcpp::pixel_format::PIX_FMT_BGR0, "", V4L2_PIX_FMT_BGR32 },
		{ ffmpegcpp::pixel_format::PIX_FMT_0RGB, "", V4L2_PIX_FMT_RGB32 },
		{ ffmpegcpp::pixel_format::PIX_FMT_GRAY8, "", V4L2_PIX_FMT_GREY },
#ifdef V4L2_PIX_FMT_Y16
		{ ffmpegcpp::pixel_format::PIX_FMT_GRAY16LE, "", V4L2_PIX_FMT_Y16 },
#endif
		{ ffmpegcpp::pixel_format::PIX_FMT_NV12, "", V4L2_PIX_FMT_NV12 },
		{ ffmpegcpp::pixel_format::PIX_FMT_NONE, "MJPEG", V4L2_PIX_FMT_MJPEG },
		{ ffmpegcpp::pixel_format::PIX_FMT_NONE, "MJPEG", V4L2_PIX_FMT_JPEG },
#ifdef V4L2_PIX_FMT_H264
		{ ffmpegcpp::pixel_format::PIX_FMT_NONE, "H264", V4L2_PIX_FMT_H264 },
#endif
#ifdef V4L2_PIX_FMT_CPIA1
		{ ffmpegcpp::pixel_format::PIX_FMT_NONE, "CPIA", V4L2_PIX_FMT_CPIA1 },
#endif
};

static fmt_map getfmt_mapByV4L2(uint32_t v4l2_fmt)
{
	for (auto fmt : avpriv_fmt_conversion_table)
	{
		if (fmt.v4l2_fmt == v4l2_fmt)
			return fmt;
	}

	return fmt_map();
}

////this should be loaded from v4l-utils/include/linux
//
//#ifndef V4L2_CAP_DEVICE_CAPS
//#define V4L2_CAP_DEVICE_CAPS            0x80000000  /* sets device capabilities field */
//#endif
//
//#ifndef V4L2_CAP_VIDEO_CAPTURE_MPLANE
///* Is a video capture device that supports multiplanar formats */
//#define V4L2_CAP_VIDEO_CAPTURE_MPLANE	0x00001000
//#endif
//
//#ifndef V4L2_CAP_VIDEO_OUTPUT_MPLANE
///* Is a video output device that supports multiplanar formats */
//#define V4L2_CAP_VIDEO_OUTPUT_MPLANE	0x00002000
//#endif
//
//#ifndef V4L2_CAP_VIDEO_M2M_MPLANE
///* Is a video mem-to-mem device that supports multiplanar formats */
//#define V4L2_CAP_VIDEO_M2M_MPLANE	0x00004000
//#endif
//
//#ifndef V4L2_CAP_VIDEO_M2M
///* Is a video mem-to-mem device */
//#define V4L2_CAP_VIDEO_M2M		0x00008000
//#endif
//
//#ifndef VIDIOC_ENUM_FRAMESIZES
//#define VIDIOC_ENUM_FRAMESIZES	_IOWR('V', 74, struct v4l2_frmsizeenum)
//#endif
//
//#ifndef V4L2_FRMSIZE_TYPE_DISCRETE
//#define	V4L2_FRMSIZE_TYPE_DISCRETE  1
//#endif
//
//#ifndef VIDIOC_ENUM_FRAMEINTERVALS
//#define VIDIOC_ENUM_FRAMEINTERVALS _IOWR('V', 75, struct v4l2_frmivalenum)
//#endif

static std::vector<std::shared_ptr<ffmpegcpp::deviceinfo>> getDevInfos(std::shared_ptr<ffmpegcpp::device> dev){
	std::vector<std::shared_ptr<ffmpegcpp::deviceinfo>> devinfos;

	int fd = -1;

	//const char *device = "/dev/video0";	/* -d device */
	const char * device = dev->name.c_str();

	if ((fd = open(device, O_RDWR)) < 0) {
		throw v4l2_exception(errno, format("Failed to open %s", device));
	}

	__u32 vidcap_buftype;
	__u32 vidout_buftype;
	struct v4l2_capability vcap;

	if (ioctl(fd, VIDIOC_QUERYCAP, &vcap)) {
		throw v4l2_exception(format("%s: not a v4l2 node\n", device));
	}

	unsigned capabilities;

	capabilities = vcap.capabilities;
	if (capabilities & V4L2_CAP_DEVICE_CAPS)
		capabilities = vcap.device_caps;

	bool is_multiplanar;

	is_multiplanar = capabilities & (V4L2_CAP_VIDEO_CAPTURE_MPLANE |
		V4L2_CAP_VIDEO_M2M_MPLANE |
		V4L2_CAP_VIDEO_OUTPUT_MPLANE);

	vidcap_buftype = is_multiplanar ? V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE :
		V4L2_BUF_TYPE_VIDEO_CAPTURE;
	vidout_buftype = is_multiplanar ? V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE :
		V4L2_BUF_TYPE_VIDEO_OUTPUT;

	struct v4l2_fmtdesc fmt;
	struct v4l2_frmsizeenum frmsize;
	struct v4l2_frmivalenum frmival;

	fmt.index = 0;
	fmt.type = vidcap_buftype;
	while (ioctl(fd, VIDIOC_ENUM_FMT, &fmt) >= 0) {
		fmt_map curfmt = getfmt_mapByV4L2(fmt.pixelformat);
		//std::shared_ptr<ffmpegcpp::deviceinfovideo> div = std::make_shared<ffmpegcpp::deviceinfovideo>();
		//div->pixelFormat = getfmt_mapByV4L2(fmt.pixelformat).ff_fmt;


		//printf("\tIndex       : %d\n", fmt.index);
		//printf("\tType        : %s\n", buftype2s(fmt.type).c_str());
		//printf("\tPixel Format: '%s'", fcc2s(fmt.pixelformat).c_str());
		//if (fmt.flags)
		//	printf(" (%s)", fmtdesc2s(fmt.flags).c_str());
		//printf("\n");
		//printf("\tName        : %s\n", fmt.description);
		frmsize.pixel_format = fmt.pixelformat;
		frmsize.index = 0;
		while (ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frmsize) >= 0) {
			//print_frmsize(frmsize, "\t");
			if (frmsize.type == V4L2_FRMSIZE_TYPE_DISCRETE) {
				frmival.index = 0;
				frmival.pixel_format = fmt.pixelformat;
				frmival.width = frmsize.discrete.width;
				frmival.height = frmsize.discrete.height;

				float minfps = 1000;
				float maxfps = -1;

				while (ioctl(fd, VIDIOC_ENUM_FRAMEINTERVALS, &frmival) >= 0) {

					float fps = (1.0 * frmival.discrete.denominator) / frmival.discrete.numerator;

					if (fps > maxfps){
						maxfps = fps;
					}

					if (fps < minfps){
						minfps = fps;
					}

					//print_frmival(frmival, "\t\t");
					frmival.index++;
				}

				if (minfps == 1000 || maxfps == -1){
					minfps = 0;
					maxfps = 0;
				}

				std::shared_ptr<ffmpegcpp::deviceinfovideo> div = std::make_shared<ffmpegcpp::deviceinfovideo>();
				div->pixelFormat = curfmt.ff_fmt;
				div->codec = curfmt.codec_id;
				div->maxFPS = maxfps;
				div->minFPS = minfps;
				div->minWidth = frmsize.discrete.width;
				div->minHeight = frmsize.discrete.height;
				div->maxHeight = div->minHeight;
				div->maxWidth = div->minWidth;
				devinfos.push_back(div);
			}
			//}else if (frmsize.type == V4L2_FRMSIZE_TYPE_STEPWISE) {
			//	/*printf("%dx%d - %dx%d with step %d/%d",
			//		frmsize.stepwise.min_width,
			//		frmsize.stepwise.min_height,
			//		frmsize.stepwise.max_width,
			//		frmsize.stepwise.max_height,
			//		frmsize.stepwise.step_width,
			//		frmsize.stepwise.step_height);*/
			//	std::shared_ptr<ffmpegcpp::deviceinfovideo> div = std::make_shared<ffmpegcpp::deviceinfovideo>();
			//	div->pixelFormat = curfmt.ff_fmt;
			//	div->codec = curfmt.codec_id;

			//	div->maxFPS = maxfps;
			//	div->minFPS = minfps;
			//	div->minWidth = frmsize.discrete.width;
			//	div->minHeight = frmsize.discrete.height;
			//	div->maxHeight = div->minHeight;
			//	div->maxWidth = div->minWidth;
			//}

			frmsize.index++;
		}
		//printf("\n");
		fmt.index++;
	}

	close(fd);
	return devinfos;
}





static std::vector<std::shared_ptr<ffmpegcpp::device>> getDevices()
{
	std::vector<std::shared_ptr<ffmpegcpp::device>> devlist;

	DIR *dp;
	struct dirent *ep;
	dev_vec files;
	dev_map links;
	dev_map cards;
	struct v4l2_capability vcap;

	dp = opendir("/dev");
	if (dp == NULL) {
		throw v4l2_exception("Couldn't open the directory");
	}
	while ((ep = readdir(dp)))
		if (is_v4l_dev(ep->d_name))
			files.push_back(std::string("/dev/") + ep->d_name);
	closedir(dp);

	/* Find device nodes which are links to other device nodes */
	for (dev_vec::iterator iter = files.begin();
		iter != files.end();) {
		char link[64 + 1];
		int link_len;
		std::string target;

		link_len = readlink(iter->c_str(), link, 64);
		if (link_len < 0) {	/* Not a link or error */
			iter++;
			continue;
		}
		link[link_len] = '\0';

		/* Only remove from files list if target itself is in list */
		if (link[0] != '/')	/* Relative link */
			target = std::string("/dev/");
		target += link;
		if (find(files.begin(), files.end(), target) == files.end()) {
			iter++;
			continue;
		}

		/* Move the device node from files to links */
		if (links[target].empty())
			links[target] = *iter;
		else
			links[target] += ", " + *iter;
		files.erase(iter);
	}

	std::sort(files.begin(), files.end(), sort_on_device_name);

	for (dev_vec::iterator iter = files.begin();
		iter != files.end(); ++iter) {
		int fd = open(iter->c_str(), O_RDWR);
		std::string bus_info;

		if (fd < 0)
			continue;
		doioctl(fd, VIDIOC_QUERYCAP, &vcap);
		close(fd);
		bus_info = (const char *)vcap.bus_info;

		std::shared_ptr<ffmpegcpp::device> dev = std::make_shared<ffmpegcpp::device>();
		dev->format = "v4l2";
		dev->name = *iter;
		dev->full_name = (char*)vcap.card;
		dev->type = ffmpegcpp::devicetype::video;
		devlist.push_back(dev);
		/*if (cards[bus_info].empty())
			cards[bus_info] += std::string((char *)vcap.card) + " (" + bus_info + "):\n";
		cards[bus_info] += "\t" + (*iter);
		if (!(links[*iter].empty()))
			cards[bus_info] += " <- " + links[*iter];
		cards[bus_info] += "\n";*/
	}
	
	/*for (dev_map::iterator iter = cards.begin();
		iter != cards.end(); ++iter) {
		printf("%s\n", iter->second.c_str());
	}*/

	return devlist;
}

std::vector<std::shared_ptr<ffmpegcpp::device>> v4l2_wrapper::listDevices()
{
	std::vector<std::shared_ptr<ffmpegcpp::device>> devlist = getDevices();
	for (auto dev : devlist){
		dev->deviceInfos = getDevInfos(dev);
	}

	return devlist;
}