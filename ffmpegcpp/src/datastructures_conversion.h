/*
* data structures shared conversion functions
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _DATA_STRUCTURES_CONVERSION_
#define _DATA_STRUCTURES_CONVERSION_

//ignore warning: type qualifiers ignored on function return type
#pragma GCC diagnostic ignored "-Wignored-qualifiers"


extern "C" {
	#include <libavutil/samplefmt.h>
	#include <libavutil/pixdesc.h>
}

#include "../includes/datastructures.h"

namespace ffmpegcpp
{
	class conversion
	{
	public:
		//convert from sample_format to AVSampleFormat
		static inline AVSampleFormat convertFrom(enum sample_format samplefmt)
		{
			switch (samplefmt)
			{
			case sample_format::none:
				return AVSampleFormat::AV_SAMPLE_FMT_NONE;
				break;
			case sample_format::u8:
				return AVSampleFormat::AV_SAMPLE_FMT_U8;
				break;
			case sample_format::s16:
				return AVSampleFormat::AV_SAMPLE_FMT_S16;
				break;
			case sample_format::s32:
				return AVSampleFormat::AV_SAMPLE_FMT_S32;
				break;
			case sample_format::flt:
				return AVSampleFormat::AV_SAMPLE_FMT_FLT;
				break;
			case sample_format::dbl:
				return AVSampleFormat::AV_SAMPLE_FMT_DBL;
				break;
			case sample_format::u8p:
				return AVSampleFormat::AV_SAMPLE_FMT_U8P;
				break;
			case sample_format::s16p:
				return AVSampleFormat::AV_SAMPLE_FMT_S16P;
				break;
			case sample_format::s32p:
				return AVSampleFormat::AV_SAMPLE_FMT_S32P;
				break;
			case sample_format::fltp:
				return AVSampleFormat::AV_SAMPLE_FMT_FLTP;
				break;
			case sample_format::dblp:
				return AVSampleFormat::AV_SAMPLE_FMT_DBLP;
				break;
			default:
				throw ffmpeg_exception("invalid sample format");
				break;
			}
		}

		//convert from AVSampleFormat to sample_format
		static inline sample_format convertFrom(enum AVSampleFormat samplefmt)
		{
			switch (samplefmt)
			{
			case AVSampleFormat::AV_SAMPLE_FMT_NONE:
				return sample_format::none;
				break;
			case AVSampleFormat::AV_SAMPLE_FMT_U8:
				return sample_format::u8;
				break;
			case AVSampleFormat::AV_SAMPLE_FMT_S16:
				return sample_format::s16;
				break;
			case AVSampleFormat::AV_SAMPLE_FMT_S32:
				return sample_format::s32;
				break;
			case AVSampleFormat::AV_SAMPLE_FMT_FLT:
				return sample_format::flt;
				break;
			case AVSampleFormat::AV_SAMPLE_FMT_DBL:
				return sample_format::dbl;
				break;
			case AVSampleFormat::AV_SAMPLE_FMT_U8P:
				return sample_format::u8p;
				break;
			case AVSampleFormat::AV_SAMPLE_FMT_S16P:
				return sample_format::s16p;
				break;
			case AVSampleFormat::AV_SAMPLE_FMT_S32P:
				return sample_format::s32p;
				break;
			case AVSampleFormat::AV_SAMPLE_FMT_FLTP:
				return sample_format::fltp;
				break;
			case AVSampleFormat::AV_SAMPLE_FMT_DBLP:
				return sample_format::dblp;
				break;
			default:
				throw ffmpeg_exception("invalid AVSampleFormat");
				break;
			}
		}

		static inline pixel_format convertFrom(AVPixelFormat pix_fmt)
		{
			switch (pix_fmt)
			{
			case AVPixelFormat::PIX_FMT_NONE: return pixel_format::PIX_FMT_NONE;
			case AVPixelFormat::PIX_FMT_YUV420P: return pixel_format::PIX_FMT_YUV420P;
			case AVPixelFormat::PIX_FMT_YUYV422: return pixel_format::PIX_FMT_YUYV422;
			case AVPixelFormat::PIX_FMT_RGB24: return pixel_format::PIX_FMT_RGB24;
			case AVPixelFormat::PIX_FMT_BGR24: return pixel_format::PIX_FMT_BGR24;
			case AVPixelFormat::PIX_FMT_YUV422P: return pixel_format::PIX_FMT_YUV422P;
			case AVPixelFormat::PIX_FMT_YUV444P: return pixel_format::PIX_FMT_YUV444P;
			case AVPixelFormat::PIX_FMT_YUV410P: return pixel_format::PIX_FMT_YUV410P;
			case AVPixelFormat::PIX_FMT_YUV411P: return pixel_format::PIX_FMT_YUV411P;
			case AVPixelFormat::PIX_FMT_GRAY8: return pixel_format::PIX_FMT_GRAY8;
			case AVPixelFormat::PIX_FMT_MONOWHITE: return pixel_format::PIX_FMT_MONOWHITE;
			case AVPixelFormat::PIX_FMT_MONOBLACK: return pixel_format::PIX_FMT_MONOBLACK;
			case AVPixelFormat::PIX_FMT_PAL8: return pixel_format::PIX_FMT_PAL8;
			case AVPixelFormat::PIX_FMT_YUVJ420P: return pixel_format::PIX_FMT_YUVJ420P;
			case AVPixelFormat::PIX_FMT_YUVJ422P: return pixel_format::PIX_FMT_YUVJ422P;
			case AVPixelFormat::PIX_FMT_YUVJ444P: return pixel_format::PIX_FMT_YUVJ444P;
			case AVPixelFormat::PIX_FMT_XVMC_MPEG2_MC: return pixel_format::PIX_FMT_XVMC_MPEG2_MC;
			case AVPixelFormat::PIX_FMT_XVMC_MPEG2_IDCT: return pixel_format::PIX_FMT_XVMC_MPEG2_IDCT;
			case AVPixelFormat::PIX_FMT_UYVY422: return pixel_format::PIX_FMT_UYVY422;
			case AVPixelFormat::PIX_FMT_UYYVYY411: return pixel_format::PIX_FMT_UYYVYY411;
			case AVPixelFormat::PIX_FMT_BGR8: return pixel_format::PIX_FMT_BGR8;
			case AVPixelFormat::PIX_FMT_BGR4: return pixel_format::PIX_FMT_BGR4;
			case AVPixelFormat::PIX_FMT_BGR4_BYTE: return pixel_format::PIX_FMT_BGR4_BYTE;
			case AVPixelFormat::PIX_FMT_RGB8: return pixel_format::PIX_FMT_RGB8;
			case AVPixelFormat::PIX_FMT_RGB4: return pixel_format::PIX_FMT_RGB4;
			case AVPixelFormat::PIX_FMT_RGB4_BYTE: return pixel_format::PIX_FMT_RGB4_BYTE;
			case AVPixelFormat::PIX_FMT_NV12: return pixel_format::PIX_FMT_NV12;
			case AVPixelFormat::AV_PIX_FMT_NV16: return pixel_format::PIX_FMT_NV16;
			case AVPixelFormat::PIX_FMT_NV21: return pixel_format::PIX_FMT_NV21;
			case AVPixelFormat::PIX_FMT_ARGB: return pixel_format::PIX_FMT_ARGB;
			case AVPixelFormat::PIX_FMT_RGBA: return pixel_format::PIX_FMT_RGBA;
			case AVPixelFormat::PIX_FMT_ABGR: return pixel_format::PIX_FMT_ABGR;
			case AVPixelFormat::PIX_FMT_BGRA: return pixel_format::PIX_FMT_BGRA;
			case AVPixelFormat::PIX_FMT_GRAY16BE: return pixel_format::PIX_FMT_GRAY16BE;
			case AVPixelFormat::PIX_FMT_GRAY16LE: return pixel_format::PIX_FMT_GRAY16LE;
			case AVPixelFormat::PIX_FMT_YUV440P: return pixel_format::PIX_FMT_YUV440P;
			case AVPixelFormat::PIX_FMT_YUVJ440P: return pixel_format::PIX_FMT_YUVJ440P;
			case AVPixelFormat::PIX_FMT_YUVA420P: return pixel_format::PIX_FMT_YUVA420P;
			case AVPixelFormat::PIX_FMT_VDPAU_H264: return pixel_format::PIX_FMT_VDPAU_H264;
			case AVPixelFormat::PIX_FMT_VDPAU_MPEG1: return pixel_format::PIX_FMT_VDPAU_MPEG1;
			case AVPixelFormat::PIX_FMT_VDPAU_MPEG2: return pixel_format::PIX_FMT_VDPAU_MPEG2;
			case AVPixelFormat::PIX_FMT_VDPAU_WMV3: return pixel_format::PIX_FMT_VDPAU_WMV3;
			case AVPixelFormat::PIX_FMT_VDPAU_VC1: return pixel_format::PIX_FMT_VDPAU_VC1;
			case AVPixelFormat::PIX_FMT_RGB48BE: return pixel_format::PIX_FMT_RGB48BE;
			case AVPixelFormat::PIX_FMT_RGB48LE: return pixel_format::PIX_FMT_RGB48LE;
			case AVPixelFormat::PIX_FMT_RGB565BE: return pixel_format::PIX_FMT_RGB565BE;
			case AVPixelFormat::PIX_FMT_RGB565LE: return pixel_format::PIX_FMT_RGB565LE;
			case AVPixelFormat::PIX_FMT_RGB555BE: return pixel_format::PIX_FMT_RGB555BE;
			case AVPixelFormat::PIX_FMT_RGB555LE: return pixel_format::PIX_FMT_RGB555LE;
			case AVPixelFormat::PIX_FMT_BGR565BE: return pixel_format::PIX_FMT_BGR565BE;
			case AVPixelFormat::PIX_FMT_BGR565LE: return pixel_format::PIX_FMT_BGR565LE;
			case AVPixelFormat::PIX_FMT_BGR555BE: return pixel_format::PIX_FMT_BGR555BE;
			case AVPixelFormat::PIX_FMT_BGR555LE: return pixel_format::PIX_FMT_BGR555LE;
			case AVPixelFormat::PIX_FMT_VAAPI_MOCO: return pixel_format::PIX_FMT_VAAPI_MOCO;
			case AVPixelFormat::PIX_FMT_VAAPI_IDCT: return pixel_format::PIX_FMT_VAAPI_IDCT;
			case AVPixelFormat::PIX_FMT_VAAPI_VLD: return pixel_format::PIX_FMT_VAAPI_VLD;
			case AVPixelFormat::PIX_FMT_YUV420P16LE: return pixel_format::PIX_FMT_YUV420P16LE;
			case AVPixelFormat::PIX_FMT_YUV420P16BE: return pixel_format::PIX_FMT_YUV420P16BE;
			case AVPixelFormat::PIX_FMT_YUV422P16LE: return pixel_format::PIX_FMT_YUV422P16LE;
			case AVPixelFormat::PIX_FMT_YUV422P16BE: return pixel_format::PIX_FMT_YUV422P16BE;
			case AVPixelFormat::PIX_FMT_YUV444P16LE: return pixel_format::PIX_FMT_YUV444P16LE;
			case AVPixelFormat::PIX_FMT_YUV444P16BE: return pixel_format::PIX_FMT_YUV444P16BE;
			case AVPixelFormat::PIX_FMT_VDPAU_MPEG4: return pixel_format::PIX_FMT_VDPAU_MPEG4;
			case AVPixelFormat::PIX_FMT_DXVA2_VLD: return pixel_format::PIX_FMT_DXVA2_VLD;
			case AVPixelFormat::PIX_FMT_RGB444LE: return pixel_format::PIX_FMT_RGB444LE;
			case AVPixelFormat::PIX_FMT_RGB444BE: return pixel_format::PIX_FMT_RGB444BE;
			case AVPixelFormat::PIX_FMT_BGR444LE: return pixel_format::PIX_FMT_BGR444LE;
			case AVPixelFormat::PIX_FMT_BGR444BE: return pixel_format::PIX_FMT_BGR444BE;
			case AVPixelFormat::PIX_FMT_GRAY8A: return pixel_format::PIX_FMT_GRAY8A;
			case AVPixelFormat::PIX_FMT_BGR48BE: return pixel_format::PIX_FMT_BGR48BE;
			case AVPixelFormat::PIX_FMT_BGR48LE: return pixel_format::PIX_FMT_BGR48LE;
			case AVPixelFormat::PIX_FMT_YUV420P9BE: return pixel_format::PIX_FMT_YUV420P9BE;
			case AVPixelFormat::PIX_FMT_YUV420P9LE: return pixel_format::PIX_FMT_YUV420P9LE;
			case AVPixelFormat::PIX_FMT_YUV420P10BE: return pixel_format::PIX_FMT_YUV420P10BE;
			case AVPixelFormat::PIX_FMT_YUV420P10LE: return pixel_format::PIX_FMT_YUV420P10LE;
			case AVPixelFormat::PIX_FMT_YUV422P10BE: return pixel_format::PIX_FMT_YUV422P10BE;
			case AVPixelFormat::PIX_FMT_YUV422P10LE: return pixel_format::PIX_FMT_YUV422P10LE;
			case AVPixelFormat::PIX_FMT_YUV444P9BE: return pixel_format::PIX_FMT_YUV444P9BE;
			case AVPixelFormat::PIX_FMT_YUV444P9LE: return pixel_format::PIX_FMT_YUV444P9LE;
			case AVPixelFormat::PIX_FMT_YUV444P10BE: return pixel_format::PIX_FMT_YUV444P10BE;
			case AVPixelFormat::PIX_FMT_YUV444P10LE: return pixel_format::PIX_FMT_YUV444P10LE;
			case AVPixelFormat::PIX_FMT_YUV422P9BE: return pixel_format::PIX_FMT_YUV422P9BE;
			case AVPixelFormat::PIX_FMT_YUV422P9LE: return pixel_format::PIX_FMT_YUV422P9LE;
			case AVPixelFormat::PIX_FMT_VDA_VLD: return pixel_format::PIX_FMT_VDA_VLD;
			case AVPixelFormat::PIX_FMT_RGBA64BE: return pixel_format::PIX_FMT_RGBA64BE;
			case AVPixelFormat::PIX_FMT_RGBA64LE: return pixel_format::PIX_FMT_RGBA64LE;
			case AVPixelFormat::PIX_FMT_BGRA64BE: return pixel_format::PIX_FMT_BGRA64BE;
			case AVPixelFormat::PIX_FMT_BGRA64LE: return pixel_format::PIX_FMT_BGRA64LE;
			case AVPixelFormat::PIX_FMT_GBRP: return pixel_format::PIX_FMT_GBRP;
			case AVPixelFormat::PIX_FMT_GBRP9BE: return pixel_format::PIX_FMT_GBRP9BE;
			case AVPixelFormat::PIX_FMT_GBRP9LE: return pixel_format::PIX_FMT_GBRP9LE;
			case AVPixelFormat::PIX_FMT_GBRP10BE: return pixel_format::PIX_FMT_GBRP10BE;
			case AVPixelFormat::PIX_FMT_GBRP10LE: return pixel_format::PIX_FMT_GBRP10LE;
			case AVPixelFormat::PIX_FMT_GBRP16BE: return pixel_format::PIX_FMT_GBRP16BE;
			case AVPixelFormat::PIX_FMT_GBRP16LE: return pixel_format::PIX_FMT_GBRP16LE;
			//case AVPixelFormat::AV_PIX_FMT_YUVA422P_LIBAV: return pixel_format::PIX_FMT_YUVA422P_LIBAV;
			//case AVPixelFormat::AV_PIX_FMT_YUVA444P_LIBAV: return pixel_format::PIX_FMT_YUVA444P_LIBAV;
			case AVPixelFormat::AV_PIX_FMT_YUVA420P9BE: return pixel_format::PIX_FMT_YUVA420P9BE;
			case AVPixelFormat::AV_PIX_FMT_YUVA420P9LE: return pixel_format::PIX_FMT_YUVA420P9LE;
			case AVPixelFormat::AV_PIX_FMT_YUVA422P9BE: return pixel_format::PIX_FMT_YUVA422P9BE;
			case AVPixelFormat::AV_PIX_FMT_YUVA422P9LE: return pixel_format::PIX_FMT_YUVA422P9LE;
			case AVPixelFormat::AV_PIX_FMT_YUVA444P9BE: return pixel_format::PIX_FMT_YUVA444P9BE;
			case AVPixelFormat::AV_PIX_FMT_YUVA444P9LE: return pixel_format::PIX_FMT_YUVA444P9LE;
			case AVPixelFormat::AV_PIX_FMT_YUVA420P10BE: return pixel_format::PIX_FMT_YUVA420P10BE;
			case AVPixelFormat::AV_PIX_FMT_YUVA420P10LE: return pixel_format::PIX_FMT_YUVA420P10LE;
			case AVPixelFormat::AV_PIX_FMT_YUVA422P10BE: return pixel_format::PIX_FMT_YUVA422P10BE;
			case AVPixelFormat::AV_PIX_FMT_YUVA422P10LE: return pixel_format::PIX_FMT_YUVA422P10LE;
			case AVPixelFormat::AV_PIX_FMT_YUVA444P10BE: return pixel_format::PIX_FMT_YUVA444P10BE;
			case AVPixelFormat::AV_PIX_FMT_YUVA444P10LE: return pixel_format::PIX_FMT_YUVA444P10LE;
			case AVPixelFormat::AV_PIX_FMT_YUVA420P16BE: return pixel_format::PIX_FMT_YUVA420P16BE;
			case AVPixelFormat::AV_PIX_FMT_YUVA420P16LE: return pixel_format::PIX_FMT_YUVA420P16LE;
			case AVPixelFormat::AV_PIX_FMT_YUVA422P16BE: return pixel_format::PIX_FMT_YUVA422P16BE;
			case AVPixelFormat::AV_PIX_FMT_YUVA422P16LE: return pixel_format::PIX_FMT_YUVA422P16LE;
			case AVPixelFormat::AV_PIX_FMT_YUVA444P16BE: return pixel_format::PIX_FMT_YUVA444P16BE;
			case AVPixelFormat::AV_PIX_FMT_YUVA444P16LE: return pixel_format::PIX_FMT_YUVA444P16LE;
			case AVPixelFormat::AV_PIX_FMT_VDPAU: return pixel_format::PIX_FMT_VDPAU;
			case AVPixelFormat::AV_PIX_FMT_XYZ12LE: return pixel_format::PIX_FMT_XYZ12LE;
			case AVPixelFormat::AV_PIX_FMT_XYZ12BE: return pixel_format::PIX_FMT_XYZ12BE;
			/*case AVPixelFormat::PIX_FMT_RGBA64BE: return pixel_format::PIX_FMT_RGBA64BE;
			case AVPixelFormat::PIX_FMT_RGBA64LE: return pixel_format::PIX_FMT_RGBA64LE;
			case AVPixelFormat::PIX_FMT_BGRA64BE: return pixel_format::PIX_FMT_BGRA64BE;
			case AVPixelFormat::PIX_FMT_BGRA64LE: return pixel_format::PIX_FMT_BGRA64LE;*/
			case AVPixelFormat::PIX_FMT_0RGB: return pixel_format::PIX_FMT_0RGB;
			case AVPixelFormat::PIX_FMT_RGB0: return pixel_format::PIX_FMT_RGB0;
			case AVPixelFormat::PIX_FMT_0BGR: return pixel_format::PIX_FMT_0BGR;
			case AVPixelFormat::PIX_FMT_BGR0: return pixel_format::PIX_FMT_BGR0;
			case AVPixelFormat::PIX_FMT_YUVA444P: return pixel_format::PIX_FMT_YUVA444P;
			case AVPixelFormat::PIX_FMT_YUVA422P: return pixel_format::PIX_FMT_YUVA422P;
			case AVPixelFormat::PIX_FMT_YUV420P12BE: return pixel_format::PIX_FMT_YUV420P12BE;
			case AVPixelFormat::PIX_FMT_YUV420P12LE: return pixel_format::PIX_FMT_YUV420P12LE;
			case AVPixelFormat::PIX_FMT_YUV420P14BE: return pixel_format::PIX_FMT_YUV420P14BE;
			case AVPixelFormat::PIX_FMT_YUV420P14LE: return pixel_format::PIX_FMT_YUV420P14LE;
			case AVPixelFormat::PIX_FMT_YUV422P12BE: return pixel_format::PIX_FMT_YUV422P12BE;
			case AVPixelFormat::PIX_FMT_YUV422P12LE: return pixel_format::PIX_FMT_YUV422P12LE;
			case AVPixelFormat::PIX_FMT_YUV422P14BE: return pixel_format::PIX_FMT_YUV422P14BE;
			case AVPixelFormat::PIX_FMT_YUV422P14LE: return pixel_format::PIX_FMT_YUV422P14LE;
			case AVPixelFormat::PIX_FMT_YUV444P12BE: return pixel_format::PIX_FMT_YUV444P12BE;
			case AVPixelFormat::PIX_FMT_YUV444P12LE: return pixel_format::PIX_FMT_YUV444P12LE;
			case AVPixelFormat::PIX_FMT_YUV444P14BE: return pixel_format::PIX_FMT_YUV444P14BE;
			case AVPixelFormat::PIX_FMT_YUV444P14LE: return pixel_format::PIX_FMT_YUV444P14LE;
			case AVPixelFormat::PIX_FMT_GBRP12BE: return pixel_format::PIX_FMT_GBRP12BE;
			case AVPixelFormat::PIX_FMT_GBRP12LE: return pixel_format::PIX_FMT_GBRP12LE;
			case AVPixelFormat::PIX_FMT_GBRP14BE: return pixel_format::PIX_FMT_GBRP14BE;
			case AVPixelFormat::PIX_FMT_GBRP14LE: return pixel_format::PIX_FMT_GBRP14LE;
		/*	case AVPixelFormat::PIX_FMT_GBRAP: return pixel_format::PIX_FMT_GBRAP;
			case AVPixelFormat::PIX_FMT_GBRAP16BE: return pixel_format::PIX_FMT_GBRAP16BE;
			case AVPixelFormat::PIX_FMT_GBRAP16LE: return pixel_format::PIX_FMT_GBRAP16LE;
			case AVPixelFormat::PIX_FMT_YUVJ411P: return pixel_format::PIX_FMT_YUVJ411P;*/
			case AVPixelFormat::PIX_FMT_NB: return pixel_format::PIX_FMT_NB;
			case AVPixelFormat::AV_PIX_FMT_YA16BE: return pixel_format::PIX_FMT_YA16BE;
			case AVPixelFormat::AV_PIX_FMT_YA16LE: return pixel_format::PIX_FMT_YA16LE;




				/*case AVPixelFormat::AV_PIX_FMT_BGR24:
					return pixel_format::PIX_FMT_BGR24;
					case AVPixelFormat::AV_PIX_FMT_YUV420P:
					return pixel_format::PIX_FMT_YUV420P;
					case AVPixelFormat::AV_PIX_FMT_GRAY8:
					return pixel_format::PIX_FMT_GRAY8;
					case AVPixelFormat::PIX_FMT_YUVJ420P:
					return pixel_format::PIX_FMT_YUVJ420P;
					case AVPixelFormat::PIX_FMT_YUVJ422P:
					return pixel_format::PIX_FMT_YUVJ422P;
					case AVPixelFormat::PIX_FMT_RGB48LE:
					return pixel_format::PIX_FMT_RGB48LE;
					case AVPixelFormat::PIX_FMT_UYVY422:
					return pixel_format::PIX_FMT_UYVY422;
					case AVPixelFormat::PIX_FMT_YUVA444P:
					return pixel_format::PIX_FMT_YUVA444P;*/
			default:
				{
				std::string message = "AVPixelFormat not implemented ";
				message.append(av_get_pix_fmt_name(pix_fmt));
					throw ffmpeg_exception(message);
				}
			}
		}

		static inline AVPixelFormat convertFrom(pixel_format pix_fmt){
			switch (pix_fmt)
			{
			case pixel_format::PIX_FMT_NONE: return AVPixelFormat::PIX_FMT_NONE;
			case pixel_format::PIX_FMT_YUV420P: return AVPixelFormat::PIX_FMT_YUV420P;
			case pixel_format::PIX_FMT_YUYV422: return AVPixelFormat::PIX_FMT_YUYV422;
			case pixel_format::PIX_FMT_RGB24: return AVPixelFormat::PIX_FMT_RGB24;
			case pixel_format::PIX_FMT_BGR24: return AVPixelFormat::PIX_FMT_BGR24;
			case pixel_format::PIX_FMT_YUV422P: return AVPixelFormat::PIX_FMT_YUV422P;
			case pixel_format::PIX_FMT_YUV444P: return AVPixelFormat::PIX_FMT_YUV444P;
			case pixel_format::PIX_FMT_YUV410P: return AVPixelFormat::PIX_FMT_YUV410P;
			case pixel_format::PIX_FMT_YUV411P: return AVPixelFormat::PIX_FMT_YUV411P;
			case pixel_format::PIX_FMT_GRAY8: return AVPixelFormat::PIX_FMT_GRAY8;
			case pixel_format::PIX_FMT_MONOWHITE: return AVPixelFormat::PIX_FMT_MONOWHITE;
			case pixel_format::PIX_FMT_MONOBLACK: return AVPixelFormat::PIX_FMT_MONOBLACK;
			case pixel_format::PIX_FMT_PAL8: return AVPixelFormat::PIX_FMT_PAL8;
			case pixel_format::PIX_FMT_YUVJ420P: return AVPixelFormat::PIX_FMT_YUVJ420P;
			case pixel_format::PIX_FMT_YUVJ422P: return AVPixelFormat::PIX_FMT_YUVJ422P;
			case pixel_format::PIX_FMT_YUVJ444P: return AVPixelFormat::PIX_FMT_YUVJ444P;
			case pixel_format::PIX_FMT_XVMC_MPEG2_MC: return AVPixelFormat::PIX_FMT_XVMC_MPEG2_MC;
			case pixel_format::PIX_FMT_XVMC_MPEG2_IDCT: return AVPixelFormat::PIX_FMT_XVMC_MPEG2_IDCT;
			case pixel_format::PIX_FMT_UYVY422: return AVPixelFormat::PIX_FMT_UYVY422;
			case pixel_format::PIX_FMT_UYYVYY411: return AVPixelFormat::PIX_FMT_UYYVYY411;
			case pixel_format::PIX_FMT_BGR8: return AVPixelFormat::PIX_FMT_BGR8;
			case pixel_format::PIX_FMT_BGR4: return AVPixelFormat::PIX_FMT_BGR4;
			case pixel_format::PIX_FMT_BGR4_BYTE: return AVPixelFormat::PIX_FMT_BGR4_BYTE;
			case pixel_format::PIX_FMT_RGB8: return AVPixelFormat::PIX_FMT_RGB8;
			case pixel_format::PIX_FMT_RGB4: return AVPixelFormat::PIX_FMT_RGB4;
			case pixel_format::PIX_FMT_RGB4_BYTE: return AVPixelFormat::PIX_FMT_RGB4_BYTE;
			case pixel_format::PIX_FMT_NV12: return AVPixelFormat::PIX_FMT_NV12;
			case pixel_format::PIX_FMT_NV16: return AVPixelFormat::AV_PIX_FMT_NV16;
			case pixel_format::PIX_FMT_NV21: return AVPixelFormat::PIX_FMT_NV21;
			case pixel_format::PIX_FMT_ARGB: return AVPixelFormat::PIX_FMT_ARGB;
			case pixel_format::PIX_FMT_RGBA: return AVPixelFormat::PIX_FMT_RGBA;
			case pixel_format::PIX_FMT_ABGR: return AVPixelFormat::PIX_FMT_ABGR;
			case pixel_format::PIX_FMT_BGRA: return AVPixelFormat::PIX_FMT_BGRA;
			case pixel_format::PIX_FMT_GRAY16BE: return AVPixelFormat::PIX_FMT_GRAY16BE;
			case pixel_format::PIX_FMT_GRAY16LE: return AVPixelFormat::PIX_FMT_GRAY16LE;
			case pixel_format::PIX_FMT_YUV440P: return AVPixelFormat::PIX_FMT_YUV440P;
			case pixel_format::PIX_FMT_YUVJ440P: return AVPixelFormat::PIX_FMT_YUVJ440P;
			case pixel_format::PIX_FMT_YUVA420P: return AVPixelFormat::PIX_FMT_YUVA420P;
			case pixel_format::PIX_FMT_VDPAU_H264: return AVPixelFormat::PIX_FMT_VDPAU_H264;
			case pixel_format::PIX_FMT_VDPAU_MPEG1: return AVPixelFormat::PIX_FMT_VDPAU_MPEG1;
			case pixel_format::PIX_FMT_VDPAU_MPEG2: return AVPixelFormat::PIX_FMT_VDPAU_MPEG2;
			case pixel_format::PIX_FMT_VDPAU_WMV3: return AVPixelFormat::PIX_FMT_VDPAU_WMV3;
			case pixel_format::PIX_FMT_VDPAU_VC1: return AVPixelFormat::PIX_FMT_VDPAU_VC1;
			case pixel_format::PIX_FMT_RGB48BE: return AVPixelFormat::PIX_FMT_RGB48BE;
			case pixel_format::PIX_FMT_RGB48LE: return AVPixelFormat::PIX_FMT_RGB48LE;
			case pixel_format::PIX_FMT_RGB565BE: return AVPixelFormat::PIX_FMT_RGB565BE;
			case pixel_format::PIX_FMT_RGB565LE: return AVPixelFormat::PIX_FMT_RGB565LE;
			case pixel_format::PIX_FMT_RGB555BE: return AVPixelFormat::PIX_FMT_RGB555BE;
			case pixel_format::PIX_FMT_RGB555LE: return AVPixelFormat::PIX_FMT_RGB555LE;
			case pixel_format::PIX_FMT_BGR565BE: return AVPixelFormat::PIX_FMT_BGR565BE;
			case pixel_format::PIX_FMT_BGR565LE: return AVPixelFormat::PIX_FMT_BGR565LE;
			case pixel_format::PIX_FMT_BGR555BE: return AVPixelFormat::PIX_FMT_BGR555BE;
			case pixel_format::PIX_FMT_BGR555LE: return AVPixelFormat::PIX_FMT_BGR555LE;
			case pixel_format::PIX_FMT_VAAPI_MOCO: return AVPixelFormat::PIX_FMT_VAAPI_MOCO;
			case pixel_format::PIX_FMT_VAAPI_IDCT: return AVPixelFormat::PIX_FMT_VAAPI_IDCT;
			case pixel_format::PIX_FMT_VAAPI_VLD: return AVPixelFormat::PIX_FMT_VAAPI_VLD;
			case pixel_format::PIX_FMT_YUV420P16LE: return AVPixelFormat::PIX_FMT_YUV420P16LE;
			case pixel_format::PIX_FMT_YUV420P16BE: return AVPixelFormat::PIX_FMT_YUV420P16BE;
			case pixel_format::PIX_FMT_YUV422P16LE: return AVPixelFormat::PIX_FMT_YUV422P16LE;
			case pixel_format::PIX_FMT_YUV422P16BE: return AVPixelFormat::PIX_FMT_YUV422P16BE;
			case pixel_format::PIX_FMT_YUV444P16LE: return AVPixelFormat::PIX_FMT_YUV444P16LE;
			case pixel_format::PIX_FMT_YUV444P16BE: return AVPixelFormat::PIX_FMT_YUV444P16BE;
			case pixel_format::PIX_FMT_VDPAU_MPEG4: return AVPixelFormat::PIX_FMT_VDPAU_MPEG4;
			case pixel_format::PIX_FMT_DXVA2_VLD: return AVPixelFormat::PIX_FMT_DXVA2_VLD;
			case pixel_format::PIX_FMT_RGB444LE: return AVPixelFormat::PIX_FMT_RGB444LE;
			case pixel_format::PIX_FMT_RGB444BE: return AVPixelFormat::PIX_FMT_RGB444BE;
			case pixel_format::PIX_FMT_BGR444LE: return AVPixelFormat::PIX_FMT_BGR444LE;
			case pixel_format::PIX_FMT_BGR444BE: return AVPixelFormat::PIX_FMT_BGR444BE;
			case pixel_format::PIX_FMT_GRAY8A: return AVPixelFormat::PIX_FMT_GRAY8A;
			case pixel_format::PIX_FMT_BGR48BE: return AVPixelFormat::PIX_FMT_BGR48BE;
			case pixel_format::PIX_FMT_BGR48LE: return AVPixelFormat::PIX_FMT_BGR48LE;
			case pixel_format::PIX_FMT_YUV420P9BE: return AVPixelFormat::PIX_FMT_YUV420P9BE;
			case pixel_format::PIX_FMT_YUV420P9LE: return AVPixelFormat::PIX_FMT_YUV420P9LE;
			case pixel_format::PIX_FMT_YUV420P10BE: return AVPixelFormat::PIX_FMT_YUV420P10BE;
			case pixel_format::PIX_FMT_YUV420P10LE: return AVPixelFormat::PIX_FMT_YUV420P10LE;
			case pixel_format::PIX_FMT_YUV422P10BE: return AVPixelFormat::PIX_FMT_YUV422P10BE;
			case pixel_format::PIX_FMT_YUV422P10LE: return AVPixelFormat::PIX_FMT_YUV422P10LE;
			case pixel_format::PIX_FMT_YUV444P9BE: return AVPixelFormat::PIX_FMT_YUV444P9BE;
			case pixel_format::PIX_FMT_YUV444P9LE: return AVPixelFormat::PIX_FMT_YUV444P9LE;
			case pixel_format::PIX_FMT_YUV444P10BE: return AVPixelFormat::PIX_FMT_YUV444P10BE;
			case pixel_format::PIX_FMT_YUV444P10LE: return AVPixelFormat::PIX_FMT_YUV444P10LE;
			case pixel_format::PIX_FMT_YUV422P9BE: return AVPixelFormat::PIX_FMT_YUV422P9BE;
			case pixel_format::PIX_FMT_YUV422P9LE: return AVPixelFormat::PIX_FMT_YUV422P9LE;
			case pixel_format::PIX_FMT_VDA_VLD: return AVPixelFormat::PIX_FMT_VDA_VLD;
			case pixel_format::PIX_FMT_RGBA64BE: return AVPixelFormat::PIX_FMT_RGBA64BE;
			case pixel_format::PIX_FMT_RGBA64LE: return AVPixelFormat::PIX_FMT_RGBA64LE;
			case pixel_format::PIX_FMT_BGRA64BE: return AVPixelFormat::PIX_FMT_BGRA64BE;
			case pixel_format::PIX_FMT_BGRA64LE: return AVPixelFormat::PIX_FMT_BGRA64LE;
			case pixel_format::PIX_FMT_GBRP: return AVPixelFormat::PIX_FMT_GBRP;
			case pixel_format::PIX_FMT_GBRP9BE: return AVPixelFormat::PIX_FMT_GBRP9BE;
			case pixel_format::PIX_FMT_GBRP9LE: return AVPixelFormat::PIX_FMT_GBRP9LE;
			case pixel_format::PIX_FMT_GBRP10BE: return AVPixelFormat::PIX_FMT_GBRP10BE;
			case pixel_format::PIX_FMT_GBRP10LE: return AVPixelFormat::PIX_FMT_GBRP10LE;
			case pixel_format::PIX_FMT_GBRP16BE: return AVPixelFormat::PIX_FMT_GBRP16BE;
			case pixel_format::PIX_FMT_GBRP16LE: return AVPixelFormat::PIX_FMT_GBRP16LE;
			//case pixel_format::PIX_FMT_YUVA422P_LIBAV: return AVPixelFormat::AV_PIX_FMT_YUVA422P_LIBAV;
			//case pixel_format::PIX_FMT_YUVA444P_LIBAV: return AVPixelFormat::AV_PIX_FMT_YUVA444P_LIBAV;
			case pixel_format::PIX_FMT_YUVA420P9BE: return AVPixelFormat::AV_PIX_FMT_YUVA420P9BE;
			case pixel_format::PIX_FMT_YUVA420P9LE: return AVPixelFormat::AV_PIX_FMT_YUVA420P9LE;
			case pixel_format::PIX_FMT_YUVA422P9BE: return AVPixelFormat::AV_PIX_FMT_YUVA422P9BE;
			case pixel_format::PIX_FMT_YUVA422P9LE: return AVPixelFormat::AV_PIX_FMT_YUVA422P9LE;
			case pixel_format::PIX_FMT_YUVA444P9BE: return AVPixelFormat::AV_PIX_FMT_YUVA444P9BE;
			case pixel_format::PIX_FMT_YUVA444P9LE: return AVPixelFormat::AV_PIX_FMT_YUVA444P9LE;
			case pixel_format::PIX_FMT_YUVA420P10BE: return AVPixelFormat::AV_PIX_FMT_YUVA420P10BE;
			case pixel_format::PIX_FMT_YUVA420P10LE: return AVPixelFormat::AV_PIX_FMT_YUVA420P10LE;
			case pixel_format::PIX_FMT_YUVA422P10BE: return AVPixelFormat::AV_PIX_FMT_YUVA422P10BE;
			case pixel_format::PIX_FMT_YUVA422P10LE: return AVPixelFormat::AV_PIX_FMT_YUVA422P10LE;
			case pixel_format::PIX_FMT_YUVA444P10BE: return AVPixelFormat::AV_PIX_FMT_YUVA444P10BE;
			case pixel_format::PIX_FMT_YUVA444P10LE: return AVPixelFormat::AV_PIX_FMT_YUVA444P10LE;
			case pixel_format::PIX_FMT_YUVA420P16BE: return AVPixelFormat::AV_PIX_FMT_YUVA420P16BE;
			case pixel_format::PIX_FMT_YUVA420P16LE: return AVPixelFormat::AV_PIX_FMT_YUVA420P16LE;
			case pixel_format::PIX_FMT_YUVA422P16BE: return AVPixelFormat::AV_PIX_FMT_YUVA422P16BE;
			case pixel_format::PIX_FMT_YUVA422P16LE: return AVPixelFormat::AV_PIX_FMT_YUVA422P16LE;
			case pixel_format::PIX_FMT_YUVA444P16BE: return AVPixelFormat::AV_PIX_FMT_YUVA444P16BE;
			case pixel_format::PIX_FMT_YUVA444P16LE: return AVPixelFormat::AV_PIX_FMT_YUVA444P16LE;
			case pixel_format::PIX_FMT_VDPAU: return AVPixelFormat::AV_PIX_FMT_VDPAU;
			case pixel_format::PIX_FMT_XYZ12LE: return AVPixelFormat::AV_PIX_FMT_XYZ12LE;
			case pixel_format::PIX_FMT_XYZ12BE: return AVPixelFormat::AV_PIX_FMT_XYZ12BE;
			/*case pixel_format::PIX_FMT_RGBA64BE: return AVPixelFormat::PIX_FMT_RGBA64BE;
			case pixel_format::PIX_FMT_RGBA64LE: return AVPixelFormat::PIX_FMT_RGBA64LE;
			case pixel_format::PIX_FMT_BGRA64BE: return AVPixelFormat::PIX_FMT_BGRA64BE;
			case pixel_format::PIX_FMT_BGRA64LE: return AVPixelFormat::PIX_FMT_BGRA64LE;*/
			case pixel_format::PIX_FMT_0RGB: return AVPixelFormat::PIX_FMT_0RGB;
			case pixel_format::PIX_FMT_RGB0: return AVPixelFormat::PIX_FMT_RGB0;
			case pixel_format::PIX_FMT_0BGR: return AVPixelFormat::PIX_FMT_0BGR;
			case pixel_format::PIX_FMT_BGR0: return AVPixelFormat::PIX_FMT_BGR0;
			case pixel_format::PIX_FMT_YUVA444P: return AVPixelFormat::PIX_FMT_YUVA444P;
			case pixel_format::PIX_FMT_YUVA422P: return AVPixelFormat::PIX_FMT_YUVA422P;
			case pixel_format::PIX_FMT_YUV420P12BE: return AVPixelFormat::PIX_FMT_YUV420P12BE;
			case pixel_format::PIX_FMT_YUV420P12LE: return AVPixelFormat::PIX_FMT_YUV420P12LE;
			case pixel_format::PIX_FMT_YUV420P14BE: return AVPixelFormat::PIX_FMT_YUV420P14BE;
			case pixel_format::PIX_FMT_YUV420P14LE: return AVPixelFormat::PIX_FMT_YUV420P14LE;
			case pixel_format::PIX_FMT_YUV422P12BE: return AVPixelFormat::PIX_FMT_YUV422P12BE;
			case pixel_format::PIX_FMT_YUV422P12LE: return AVPixelFormat::PIX_FMT_YUV422P12LE;
			case pixel_format::PIX_FMT_YUV422P14BE: return AVPixelFormat::PIX_FMT_YUV422P14BE;
			case pixel_format::PIX_FMT_YUV422P14LE: return AVPixelFormat::PIX_FMT_YUV422P14LE;
			case pixel_format::PIX_FMT_YUV444P12BE: return AVPixelFormat::PIX_FMT_YUV444P12BE;
			case pixel_format::PIX_FMT_YUV444P12LE: return AVPixelFormat::PIX_FMT_YUV444P12LE;
			case pixel_format::PIX_FMT_YUV444P14BE: return AVPixelFormat::PIX_FMT_YUV444P14BE;
			case pixel_format::PIX_FMT_YUV444P14LE: return AVPixelFormat::PIX_FMT_YUV444P14LE;
			case pixel_format::PIX_FMT_GBRP12BE: return AVPixelFormat::PIX_FMT_GBRP12BE;
			case pixel_format::PIX_FMT_GBRP12LE: return AVPixelFormat::PIX_FMT_GBRP12LE;
			case pixel_format::PIX_FMT_GBRP14BE: return AVPixelFormat::PIX_FMT_GBRP14BE;
			case pixel_format::PIX_FMT_GBRP14LE: return AVPixelFormat::PIX_FMT_GBRP14LE;
			/*case pixel_format::PIX_FMT_GBRAP: return AVPixelFormat::PIX_FMT_GBRAP;
			case pixel_format::PIX_FMT_GBRAP16BE: return AVPixelFormat::PIX_FMT_GBRAP16BE;
			case pixel_format::PIX_FMT_GBRAP16LE: return AVPixelFormat::PIX_FMT_GBRAP16LE;
			case pixel_format::PIX_FMT_YUVJ411P: return AVPixelFormat::PIX_FMT_YUVJ411P;*/
			case pixel_format::PIX_FMT_NB: return AVPixelFormat::PIX_FMT_NB;
			case pixel_format::PIX_FMT_YA16BE: return AVPixelFormat::AV_PIX_FMT_YA16BE;
			case pixel_format::PIX_FMT_YA16LE: return AVPixelFormat::AV_PIX_FMT_YA16LE;
				/*

							case pixel_format::PIX_FMT_BGR24:
							return AVPixelFormat::AV_PIX_FMT_BGR24;
							case pixel_format::PIX_FMT_YUV420P:
							return AVPixelFormat::AV_PIX_FMT_YUV420P;
							case pixel_format::PIX_FMT_GRAY8:
							return AVPixelFormat::AV_PIX_FMT_GRAY8;
							case pixel_format::PIX_FMT_YUVJ420P:
							return AVPixelFormat::PIX_FMT_YUVJ420P;
							case pixel_format::PIX_FMT_YUVJ422P:
							return AVPixelFormat::PIX_FMT_YUVJ422P;
							case pixel_format::PIX_FMT_RGB48LE:
							return AVPixelFormat::PIX_FMT_RGB48LE;
							case pixel_format::PIX_FMT_UYVY422:
							return AVPixelFormat::PIX_FMT_UYVY422;
							case pixel_format::PIX_FMT_YUVA444P:
							return AVPixelFormat::PIX_FMT_YUVA444P;*/
			default:
				throw ffmpeg_exception("pixel_format not implemented");
			}
		}

		static inline const enum ffmpegcpp::mediaType convertFrom(AVMediaType media_type)
		{
			switch (media_type)
			{
			case AVMediaType::AVMEDIA_TYPE_ATTACHMENT:
				return mediaType::attachment;
			case AVMediaType::AVMEDIA_TYPE_AUDIO:
				return mediaType::audio;
			case AVMediaType::AVMEDIA_TYPE_DATA:
				return mediaType::data;
			case AVMediaType::AVMEDIA_TYPE_NB:
				return mediaType::nb;
			case AVMediaType::AVMEDIA_TYPE_SUBTITLE:
				return mediaType::subtitle;
			case AVMediaType::AVMEDIA_TYPE_UNKNOWN:
				return mediaType::unknown;
			case AVMediaType::AVMEDIA_TYPE_VIDEO:
				return mediaType::video;
			}
			throw ffmpeg_exception("media type not implemented");
		}

		
	};
};

#endif