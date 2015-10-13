/*
* stream
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _FFMPEGCPP_STREAM_H_
#define _FFMPEGCPP_STREAM_H_

#include <string>
#include <vector>
#include <memory>

namespace ffmpegcpp
{
	/*struct frac {
		int64_t val, num, den;
		frac(int64_t val_, int64_t num_, int64_t den_){
			val = val_;
			num = num_;
			den = den_;
		}
	};*/

	

	
	class codec;
	class decoder;
	class encoder;
	class formatcontext;
	class bitstreamfilter;

	class stream : public std::enable_shared_from_this<stream>
	{
	private:
		struct stream_pimpl;
		std::shared_ptr<stream_pimpl> _pimpl;

		friend class formatcontext;
		friend class codec;
		friend class encoder;

		std::shared_ptr<codec> _codec = nullptr;
		std::shared_ptr<decoder> _decoder = nullptr;
		std::shared_ptr<encoder> _encoder = nullptr;
		std::shared_ptr<formatcontext> _context = nullptr;

		std::vector<std::shared_ptr<bitstreamfilter>> _bsfilters;
		
	public:
		stream(const stream& that) = delete;
		stream& operator=(const stream&) = delete;
		stream(std::shared_ptr<formatcontext> context_);
		static std::shared_ptr<std::vector<std::shared_ptr<stream>>> getStreams(std::shared_ptr<formatcontext> context_);
		static std::shared_ptr<stream> addStream(std::shared_ptr<formatcontext> context_, std::string codecName, rational stream_time_base);

		//TODO: add function to round image dimension to match codec size

		FFMPEGCPP_API const int index();
		FFMPEGCPP_API const rational r_frame_rate();
		FFMPEGCPP_API const rational time_base();
		FFMPEGCPP_API const int64_t duration();

		FFMPEGCPP_API const std::shared_ptr<formatcontext> getContext();
		FFMPEGCPP_API const std::shared_ptr<codec> getCodec();
		FFMPEGCPP_API const std::shared_ptr<decoder> getDecoder();
		FFMPEGCPP_API const std::shared_ptr<encoder> getEncoder();

		FFMPEGCPP_API const void addBitstreamFilter(std::shared_ptr<bitstreamfilter> bsfilter);

		
	};
}

#endif