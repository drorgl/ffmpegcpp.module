/*
* iobuffer
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _FFMPEGCPP_IOBUFFER_H_
#define _FFMPEGCPP_IOBUFFER_H_

#include <stdint.h>
extern "C" {
#include <libavutil/fifo.h>
#include <libavformat/avio.h>
}
#include <queue>
#include <stdexcept>

namespace ffmpegcpp
{
	class iobuffer_exception : public std::runtime_error
	{
	public:
		iobuffer_exception(std::string message) : std::runtime_error(message) {}
	};

	//Implementation of buffered AVIOContext 
	class iobuffer
	{
	private:
		friend class ffmpeg;
		friend class formatcontext;

		static const AVClass _iobuffercls;

		//internal aviocontext read_packet
		static int _read_packet(void *opaque, uint8_t *buf, int buf_size);
		//internal aviocontext write_packet
		static int _write_packet(void *opaque, uint8_t *buf, int buf_size);

		//not implemented
		//static int64_t _seek(void *opaque, int64_t offset, int whence);

		//the fifo/circular buffer container for the actual bytes
		AVFifoBuffer * _fifo_buffer;

		//avio context
		AVIOContext * _io_context;

		//the packet sizes queue, in sync with bytes in fifo
		std::queue<int> _packetSizesQueue;

		//the total buffer size
		int _bufferSize;

		//the splitted blocks size
		int _blockSize;

		//the buffer for AVIOContext
		uint8_t * _buffer;
	public:
		iobuffer(const iobuffer& that) = delete;
		iobuffer& operator=(const iobuffer&) = delete;
		iobuffer(int blockSize, int bufferSize);
		~iobuffer();

		int getBufferSize();
		int getBlockSize();

		//writes the next block into fifo/buffer/queue
		int write_next_block(uint8_t *buf, int buf_size);

		//reads the next block from fifo/buffer/queue
		int read_next_block(uint8_t *buf, int buf_size);

		//flushes aviocontext, so latest bytes will be in buffer
		void flush();

		//clears everything but keeps the collections allocated, for reusing memory allocations
		void clear();

		//number of blocks available
		int blocks();

		//total number of bytes in use
		int size();
	};
}

#endif