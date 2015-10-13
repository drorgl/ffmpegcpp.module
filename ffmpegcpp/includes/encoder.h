/*
* encoder
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _FFMPEGCPP_ENCODER_H_
#define _FFMPEGCPP_ENCODER_H_


#include "exports.h"
#include "ffmpeg_exception.h"

#include <memory>
#include <string>

//#include <opencv2/core.hpp>

namespace ffmpegcpp
{
	class codec;
	class stream;
	class dictionary;
	class frame;
	class packet;

	class encoder
	{
	private:
		struct encoder_pimpl;
		std::shared_ptr<encoder_pimpl> _pimpl;

		std::shared_ptr<codec> _codec = nullptr;
		std::shared_ptr<stream> _stream = nullptr;

		//returns true if codec needs flushing at the end of the encoding session
		bool CodecNeedsFlushing();

		
	public:
		encoder(const encoder& that) = delete;
		encoder& operator=(const encoder&) = delete;
		encoder(std::shared_ptr<stream> stream_, std::shared_ptr<codec> codec_);

		FFMPEGCPP_API void open(const std::shared_ptr<dictionary> &options = nullptr);

		FFMPEGCPP_API bool encode(frame *frame_, packet *packet_);
		bool flush(packet *packet_);
	};
}

#endif