/*
* memory context
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _FFMPEGCPP_MEMORYCONTEXT_H_
#define _FFMPEGCPP_MEMORYCONTEXT_H_

#include "exports.h"

#include <memory>

namespace ffmpegcpp
{
	class memorycontext{
	private:
		struct memorycontext_pimpl;
		std::shared_ptr<memorycontext_pimpl> _pimpl;

		friend class ffmpeg;
		friend class formatcontext;

	public:
		memorycontext(const memorycontext& that) = delete;
		memorycontext& operator=(const memorycontext&) = delete;
		FFMPEGCPP_API memorycontext(int blockSize, int bufferSize);
		FFMPEGCPP_API ~memorycontext();

		FFMPEGCPP_API int getBlockSize();
		FFMPEGCPP_API int getBufferSize();

		//writes the next block into fifo/buffer/queue
		FFMPEGCPP_API int write_next_block(uint8_t *buf, int buf_size);

		//reads the next block from fifo/buffer/queue
		FFMPEGCPP_API int read_next_block(uint8_t *buf, int buf_size);

		//flushes aviocontext, so latest bytes will be in buffer
		FFMPEGCPP_API void flush();

		//clears everything but keeps the collections allocated, for reusing memory allocations
		FFMPEGCPP_API void clear();

		//number of blocks available
		FFMPEGCPP_API int blocks();

		//total number of bytes in use
		FFMPEGCPP_API int size();

	};
}

#endif