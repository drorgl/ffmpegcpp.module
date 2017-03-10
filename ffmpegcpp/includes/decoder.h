/*
* decoder
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _FFMPEGCPP_DECODER_H_
#define _FFMPEGCPP_DECODER_H_


#include "exports.h"
#include "ffmpeg_exception.h"

#include <memory>

namespace ffmpegcpp
{
	class frame;
	class packet;
	class swscale;
	class dictionary;
	class formatcontext;
	class stream;
	class codec;

	class decoder
	{
	private:
		struct decoder_pimpl;
		std::shared_ptr<decoder_pimpl> _pimpl;

		std::shared_ptr<codec> _codec = nullptr;
		
	public:
		decoder(const decoder& that) = delete;
		decoder& operator=(const decoder&) = delete;
		decoder(std::shared_ptr<codec> codec_);

		FFMPEGCPP_API void open(const std::shared_ptr<dictionary> &options = nullptr);

		//decode a packet into a frame
		FFMPEGCPP_API bool decode(packet &packet_, frame &frame_);

	};
}

#endif