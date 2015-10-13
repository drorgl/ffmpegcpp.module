/*
* ffmpeg include aggregate
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _FFMPEGCPP_FFMPEG_INCLUDES_H_
#define _FFMPEGCPP_FFMPEG_INCLUDES_H_

#define CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
//#include <crtdbg.h>


extern "C" {
//#include <libavutil/avassert.h>
//#include <libavutil/channel_layout.h>
//#include <libavutil/opt.h>
//#include <libavutil/mathematics.h>
//#include <libavutil/timestamp.h>


#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/opt.h>
#include <libavdevice/avdevice.h>
#include <libavfilter/avfilter.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/pixdesc.h>
//#include <libavutil/intreadwrite.h>
}

//#include "ffmpeg.h"
//#include "codec_pimpl.h"
//#include "formatcontext_pimpl.h"
//#include "dictionary_pimpl.h"
//#include "stream_pimpl.h"



#endif