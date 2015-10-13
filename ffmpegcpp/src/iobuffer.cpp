/*
* iobuffer implementation
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#include "iobuffer.h"

#include <cassert>

#include "ffmpeg_includes.h"

namespace ffmpegcpp{
	const AVClass iobuffer::_iobuffercls = {
		"iobuffer", av_default_item_name
	};


	int iobuffer::_read_packet(void *opaque, uint8_t *buf, int buf_size){
		iobuffer * iobuf = (iobuffer*)opaque;
		auto blocksize = iobuf->_packetSizesQueue.front();
		if (buf_size < blocksize){
			throw iobuffer_exception("error reading packet, supplied buffer is too small");
		}
		auto bytesToRead = blocksize;
		av_fifo_generic_read(iobuf->_fifo_buffer, buf, bytesToRead, NULL);
		
		iobuf->_packetSizesQueue.pop();
		return blocksize;
	}
	int iobuffer::_write_packet(void *opaque, uint8_t *buf, int buf_size){
		iobuffer * iobuf = (iobuffer*)opaque;

		assert(buf_size <= iobuf->_blockSize && "programming error, write_packet is being called with buf_size larger than block size");

		auto spaceLeft = av_fifo_space(iobuf->_fifo_buffer);

		if (buf_size > spaceLeft){
			throw iobuffer_exception("error writing packet, not enough space in buffer");
		}

		int written = av_fifo_generic_write(iobuf->_fifo_buffer, buf, buf_size, NULL);
		iobuf->_packetSizesQueue.push(buf_size);
		return written;
	}
	
	//int64_t * seek(void *opaque, int64_t offset, int whence){
	//}

	iobuffer::iobuffer(int blockSize, int bufferSize){
		_bufferSize = bufferSize;
		_blockSize = blockSize;

		_buffer = (uint8_t*)av_mallocz(_blockSize);
		_fifo_buffer = av_fifo_alloc(_bufferSize);
		_io_context = avio_alloc_context(
			_buffer, 
			_blockSize, 
			AVIO_FLAG_WRITE, 
			this, 
			_read_packet, 
			_write_packet, 
			NULL);
		
		//_io_context->direct = 0; //AVIO_FLAG_DIRECT for inputs only
		//_io_context->seekable = 0;
		_io_context->max_packet_size = _blockSize;
		_io_context->av_class = &_iobuffercls;
	}
	iobuffer::~iobuffer(){
		flush();

		

		av_fifo_free(_fifo_buffer);
		
		//avio_close(_io_context);
		av_free(_io_context);
		av_free(_buffer);
		
	}

	int iobuffer::getBufferSize()
	{
		return _bufferSize;
	}

	int iobuffer::getBlockSize()
	{
		return _blockSize;
	}

	int iobuffer::write_next_block(uint8_t *buf, int buf_size){
		return iobuffer::_write_packet(this, buf, buf_size);
	}

	int iobuffer::read_next_block(uint8_t *buf, int buf_size){
		return iobuffer::_read_packet(this, buf, buf_size);
	}

	void iobuffer::flush(){
		avio_flush(_io_context);
	}

	void iobuffer::clear(){
		flush();
		av_fifo_drain(_fifo_buffer, av_fifo_size(_fifo_buffer));
		//we don't want to reallocate/free memory, just clear the queue, for performance reasons
		while (!_packetSizesQueue.empty()) _packetSizesQueue.pop();
	}

	int iobuffer::blocks(){
		return (int)_packetSizesQueue.size();
	}

	int iobuffer::size(){
		return av_fifo_size(_fifo_buffer);
	}

}