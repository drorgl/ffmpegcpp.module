/*
* codec
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _FFMPEGCPP_CODEC_H_
#define _FFMPEGCPP_CODEC_H_

//ignore warning: type qualifiers ignored on function return type
#pragma GCC diagnostic ignored "-Wignored-qualifiers"

#include "exports.h"
#include "ffmpeg_exception.h"

#include "datastructures.h"

#include <string>
#include <memory>
#include <map>

namespace ffmpegcpp
{
	class stream;
	class dictionary;

	class codec
	{
	private:
		struct codec_pimpl;
		std::shared_ptr<codec_pimpl> _pimpl;

		friend class decoder;
		friend class encoder;
		friend class swscale;
		friend class stream;
		friend class ffmpeg;
		friend class bitstreamfilter;

		std::shared_ptr<stream> _stream;

		bool _wasOpened = false;

		int _flushInterval = 10; //every 10 calls to flush an actual flush will occur
		int _flushCounter = 0;

		void open(const std::shared_ptr<dictionary> &options);
	public:
		codec(const codec& that) = delete;
		codec& operator=(const codec&) = delete;
		codec(std::shared_ptr<stream> stream_);
		~codec();

		void openAsDecoder(const std::shared_ptr< dictionary> &options = nullptr);
		void openAsEncoder(const std::shared_ptr< dictionary> &options = nullptr);
		void close();
		void flush(bool immediate = false);


		FFMPEGCPP_API const std::string name();

		FFMPEGCPP_API const int width();
		FFMPEGCPP_API void width(int width_);

		FFMPEGCPP_API const int height();
		FFMPEGCPP_API void height(int height_);

		FFMPEGCPP_API const pixel_format pix_fmt();
		FFMPEGCPP_API void pix_fmt(pixel_format pix_fmt_);

		FFMPEGCPP_API const int sample_rate();
		FFMPEGCPP_API void sample_rate(int sample_rate_);

		FFMPEGCPP_API const int channels();
		FFMPEGCPP_API void channels(int channels_);

		FFMPEGCPP_API const int bit_rate();
		FFMPEGCPP_API void bit_rate(int bit_rate_);

		FFMPEGCPP_API const enum ffmpegcpp::channel_layout channels_layout();
		FFMPEGCPP_API void channels_layout(enum ffmpegcpp::channel_layout channels_layout_);

		FFMPEGCPP_API const enum ffmpegcpp::sample_format sample_fmt();
		FFMPEGCPP_API void sample_fmt(enum ffmpegcpp::sample_format sample_fmt_);

		FFMPEGCPP_API const ffmpegcpp::rational time_base();
		FFMPEGCPP_API void time_base(ffmpegcpp::rational time_base_);

		FFMPEGCPP_API const int gop_size();
		FFMPEGCPP_API void gop_size(int gop_size_);

		FFMPEGCPP_API const enum ffmpegcpp::mediaType mediaType();
		
		FFMPEGCPP_API static std::vector<codecinfo> listCodecs();
	};

}

#endif