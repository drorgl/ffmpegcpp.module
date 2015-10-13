/*
* codec shared implementation
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _FFMPEGCPP_CODEC_PIMPL_H_
#define _FFMPEGCPP_CODEC_PIMPL_H_

#include "ffmpeg_includes.h"
#include "../includes/codec.h"

namespace ffmpegcpp
{
	struct codec::codec_pimpl
	{
		AVCodecContext *pCodecCtx = NULL;

		static AVCodec * find_encoder_by_name(std::string name)
		{
			auto pcodec = avcodec_find_encoder_by_name(name.c_str());
			if (pcodec == NULL){
				throw ffmpeg_exception("codec not found: " + name);
			}

			return pcodec;
		}

		static std::string get_codec_name(enum AVCodecID id){
			return avcodec_get_name(id);
		}

	};

}

#endif