/*
* ffmpegcpp exception
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _FFMPEGCPP_FFMPEG_EXCEPTION_H_
#define _FFMPEGCPP_FFMPEG_EXCEPTION_H_

#include <stdexcept>

namespace ffmpegcpp
{
	class ffmpeg_exception : public std::runtime_error
	{
		std::string getAVError(int errorno);
		int _averrornumber;
	public:
		ffmpeg_exception(std::string message)
			: std::runtime_error(message)
		{}

		ffmpeg_exception(int avreturnvalue)
			: std::runtime_error(getAVError(avreturnvalue))
		{
				_averrornumber = avreturnvalue;
		}

		ffmpeg_exception(int avreturnvalue, std::string message)
			: std::runtime_error(getAVError(avreturnvalue) + " " + message)
		{
				_averrornumber = avreturnvalue;
		}
	};
}

#endif