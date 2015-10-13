/*
* frame
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _FFMPEGCPP_FRAME_H_
#define _FFMPEGCPP_FRAME_H_

#include "datastructures.h"

#include <memory>
//#include <opencv2/core.hpp>

namespace ffmpegcpp
{
	class frame
	{
	private:
		struct frame_pimpl;
		std::shared_ptr<frame_pimpl> _pimpl;

		friend class swscale;
		friend class decoder;
		friend class encoder;

	public:
		FFMPEGCPP_API frame();
		FFMPEGCPP_API ~frame();

		frame(const frame &that) = delete;
		frame& operator=(frame const&) = delete;

		//reset frame state, used for frame reuse
		FFMPEGCPP_API void unref();

		FFMPEGCPP_API void get_buffer(int align);

		FFMPEGCPP_API void set_video_parameters(int width, int height, pixel_format format);
		FFMPEGCPP_API void set_audio_parameters(int channels, sample_format format, channel_layout channel_layout, int sample_rate, int nb_samples);
	};		
}

#endif