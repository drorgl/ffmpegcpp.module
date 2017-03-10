/*
* stream shared implementation
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _FFMPEGCPP_STREAM_PIMPL_H_
#define _FFMPEGCPP_STREAM_PIMPL_H_

#include "stdafx.h"

#include "stream.h"

extern "C" {
	#include <libavformat/avformat.h>
}

namespace ffmpegcpp
{
	struct stream::stream_pimpl
	{
		AVStream *pStream;
		AVCodecContext *pCodecCtx;

		AVFrame *pFrame;
	};

}

#endif