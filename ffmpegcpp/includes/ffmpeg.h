/*
* main ffmpeg wrapper
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _FFMPEGCPP_FFMPEG_H_
#define _FFMPEGCPP_FFMPEG_H_

#include "exports.h"

#include "codec.h"
#include "formatcontext.h"
#include "stream.h"
#include "dictionary.h"
#include "decoder.h"
#include "frame.h"
#include "swscale.h"
#include "ffmpeg_exception.h"
#include "datastructures.h"
#include "encoder.h"
#include "packet.h"

#include "bitstreamfilter.h"

#include "memorycontext.h"

#include <memory>
#include <atomic>
#include <mutex>
#include <map>
#include <vector>
#include <string>

#include <algorithm>
#include <iostream>
#include <cctype>

namespace ffmpegcpp
{
	class memorycontext;

	class ffmpeg
	{
	private:
		static std::atomic<int> _instanceCount;
		static std::mutex _instanceLock;

		static std::string _ffmpeg_log;
		static void _logcallback(void * avcl, int level, const char * fmt, va_list vl);

		friend class codec;
		friend class formatcontext;

		struct ffmpeg_pimpl;
		std::shared_ptr<ffmpeg_pimpl> _pimpl;

		static std::function<void(std::string, int, std::string)> _logger;

		static std::vector<std::string> ExecuteCommand(std::string filename, std::string format, std::shared_ptr<dictionary> dict);

		static void ParseInputDevice(std::shared_ptr<device> d);

	public:
		ffmpeg(const ffmpeg& that) = delete;
		ffmpeg& operator=(const ffmpeg&) = delete;
		FFMPEGCPP_API ffmpeg();
		FFMPEGCPP_API ~ffmpeg();

		FFMPEGCPP_API static int instance_count();

		FFMPEGCPP_API std::shared_ptr<formatcontext> open_input(std::string filename, std::string format, std::shared_ptr<dictionary> options = nullptr, std::shared_ptr<dictionary> metadata = nullptr);
		FFMPEGCPP_API std::shared_ptr<formatcontext> open_output(std::string filename, std::string format);

		FFMPEGCPP_API std::shared_ptr<formatcontext> open_output(std::shared_ptr<memorycontext> memoryctx, std::string filename, std::string format);

		FFMPEGCPP_API static void setLogger(std::function<void(std::string, int, std::string)> logger_);

		
		FFMPEGCPP_API static std::vector<std::shared_ptr<device>> ListInputDevices();
	};

}

#endif