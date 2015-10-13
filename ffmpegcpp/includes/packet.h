/*
* packet
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _FFMPEGCPP_PACKET_H_
#define _FFMPEGCPP_PACKET_H_

#include "exports.h"
#include "datastructures.h"
#include <memory>

namespace ffmpegcpp
{
	class packet
	{
	private:
		struct packet_pimpl;
		std::shared_ptr<packet_pimpl> _pimpl;

		friend class encoder;
		friend class decoder;
		friend class formatcontext;
		friend class bitstreamfilter;

	public:
		FFMPEGCPP_API packet();
		FFMPEGCPP_API ~packet();

		packet(const packet &) = delete;
		packet& operator=(packet const&) = delete;

		FFMPEGCPP_API void unref();
		FFMPEGCPP_API void rescale_ts(const rational &src, const rational &dst);
		FFMPEGCPP_API void rescale_rnd(const rational &src, const rational &dst);

		FFMPEGCPP_API int stream_index();
		FFMPEGCPP_API void stream_index(int newindex);
	};
}

#endif