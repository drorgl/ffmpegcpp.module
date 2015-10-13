/*
* ffmpegcpp exception implementation
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"

#include "ffmpeg.h"

#include "ffmpeg_includes.h"

namespace ffmpegcpp
{
	std::string ffmpeg_exception::getAVError(int errorno)
	{
		char buf[256];
		av_strerror(errorno, buf, sizeof(buf));
		return std::string(buf);
	}
}