/*
* format context
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _FFMPEGCPP_CONTEXT_H_
#define _FFMPEGCPP_CONTEXT_H_

#include "exports.h"
#include "ffmpeg_exception.h"

#include "datastructures.h"

#include <vector>
#include <memory>

namespace ffmpegcpp
{
	class frame;
	class packet;
	class memorycontext;
	class stream;
	class dictionary;

	enum readPacketState {
		success,
		eof,
		again
	};

	class formatcontext : public std::enable_shared_from_this<formatcontext>
	{
	private:
		struct formatcontext_pimpl;
		std::shared_ptr<formatcontext_pimpl> _pimpl;

		//std::shared_ptr<frame> _frame = nullptr;
		std::shared_ptr<packet> _packet = nullptr;

		friend class ffmpeg;
		friend class stream;
		friend class decoder;
		friend class encoder;
		friend class frame;
		friend class packet;
		friend class iocontext;

		//std::shared_ptr<memorycontext> _memoryContext = nullptr;

		//pass a packet through bitstream
		void _bitstreamfilter_packet(packet &packet_);
	public:
		formatcontext(const formatcontext& that) = delete;
		formatcontext& operator=(const formatcontext&) = delete;
		formatcontext();

		//get streams for context
		FFMPEGCPP_API const std::shared_ptr<std::vector<std::shared_ptr<stream>>> &getStreams();

		//not implemented yet
		//timestamp = dts, flags, forward/backward/begining etc', make enum
		//void seek_frame(int stream_index, int64_t timestamp, int flags);
		//void seek_file(int stream_index, int64_t mints, int64_t targettimestamp, int64_t maxts, int flags);

		FFMPEGCPP_API readPacketState readPacket(std::shared_ptr<packet> packet);

		//writes a header if output context
		FFMPEGCPP_API void write_header(const std::shared_ptr<dictionary> options_ = nullptr, const std::shared_ptr<dictionary> metadata_ = nullptr);

		//writes a packet if output context
		FFMPEGCPP_API void write_packet(packet &packet_);

		//writes an interleaved packet if output context
		FFMPEGCPP_API void write_packet_interleaved(packet &packet_);

		//adds an output stream
		FFMPEGCPP_API std::shared_ptr<stream> addStream(std::string codecName, rational stream_time_base);

		FFMPEGCPP_API std::string getSDP();

		//close input and flush output and close.
		FFMPEGCPP_API void close();

		FFMPEGCPP_API static void listInputFormats(std::vector<component> &list);
		FFMPEGCPP_API static void listOutputFormats(std::vector<component> &list);
	};
}

#endif