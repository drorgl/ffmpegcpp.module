/*
* packet shared implementation
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _FFMPEGCPP_PACKET_PIMPL_H_
#define _FFMPEGCPP_PACKET_PIMPL_H_

#include "stdafx.h"

#include "packet.h"

extern "C" {
	#include <libavcodec/avcodec.h>
}

namespace ffmpegcpp
{
	struct packet::packet_pimpl
	{
		AVPacket _packet;
	};
}

#endif