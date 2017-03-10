/*
* bitstream filter wrapper
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/


#ifndef _FFMPEGCPP_BITSTREAMFILTER_H_
#define _FFMPEGCPP_BITSTREAMFILTER_H_

#include "exports.h"
#include "ffmpeg_exception.h"

#include <string>
#include <memory>
#include <vector>

namespace ffmpegcpp
{
	class packet;
	class codec;

	class bitstreamfilter
	{
	private:
		struct bitstreamfilter_pimpl;
		std::shared_ptr<bitstreamfilter_pimpl> _pimpl;

		friend class formatcontext;

	public:
		bitstreamfilter(const bitstreamfilter& that) = delete;
		bitstreamfilter & operator=(const bitstreamfilter&) = delete;

		FFMPEGCPP_API bitstreamfilter(std::string name, std::string parameters, std::shared_ptr<codec> codec);
		FFMPEGCPP_API ~bitstreamfilter();

		FFMPEGCPP_API static std::vector<std::string> listBitstreamFilters();

		FFMPEGCPP_API void apply(std::shared_ptr<packet> packet);
	};

}

#endif