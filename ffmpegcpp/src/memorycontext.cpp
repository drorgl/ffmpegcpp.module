/*
* memory context implementation
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#include "memorycontext.h"
#include "memorycontext_pimpl.h"

namespace ffmpegcpp{

	memorycontext::memorycontext(int blockSize, int bufferSize)
		: _pimpl(std::make_shared<memorycontext::memorycontext_pimpl>())
	{
		_pimpl->_iobuffer = std::make_shared<iobuffer>(blockSize, bufferSize);
	}
	memorycontext::~memorycontext()
	{

	}

	int memorycontext::getBlockSize()
	{
		return _pimpl->_iobuffer->getBlockSize();
	}

	int memorycontext::getBufferSize()
	{
		return _pimpl->_iobuffer->getBufferSize();
	}

	int memorycontext::write_next_block(uint8_t *buf, int buf_size)
	{
		return _pimpl->_iobuffer->write_next_block(buf, buf_size);
	}

	int memorycontext::read_next_block(uint8_t *buf, int buf_size)
	{
		return _pimpl->_iobuffer->read_next_block(buf, buf_size);
	}

	void memorycontext::flush()
	{
		_pimpl->_iobuffer->flush();
	}

	void memorycontext::clear()
	{
		_pimpl->_iobuffer->clear();
	}

	int memorycontext::blocks()
	{
		return _pimpl->_iobuffer->blocks();
	}

	int memorycontext::size()
	{
		return _pimpl->_iobuffer->size();
	}


}