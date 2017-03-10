/*
* packet implementation
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"

#include "packet.h"

#include "dictionary_pimpl.h"
#include "packet_pimpl.h"

namespace ffmpegcpp
{


	packet::packet()
		: _pimpl(std::make_shared<packet::packet_pimpl>())
	{
		av_init_packet(&_pimpl->_packet);
		unref();
	}

	packet::~packet()
	{
		unref();
		av_free_packet(&_pimpl->_packet);
	}

	void packet::unref()
	{
		/*if (_pimpl->_packet.data != NULL)
		{
			av_free(_pimpl->_packet.data);
			_pimpl->_packet.data = NULL;
		}*/
		av_packet_unref(&_pimpl->_packet);
	}

	void packet::rescale_ts(const rational &src, const rational &dst)
	{
		AVRational src_tb = { src.num, src.den };
		AVRational dst_tb = { dst.num, dst.den };
		if (_pimpl->_packet.pts != AV_NOPTS_VALUE)
			_pimpl->_packet.pts = av_rescale_q(_pimpl->_packet.pts, src_tb, dst_tb);
		if (_pimpl->_packet.dts != AV_NOPTS_VALUE)
			_pimpl->_packet.dts = av_rescale_q(_pimpl->_packet.dts, src_tb, dst_tb);
		if (_pimpl->_packet.duration > 0)
			_pimpl->_packet.duration = (int)av_rescale_q(_pimpl->_packet.duration, src_tb, dst_tb);
		if (_pimpl->_packet.convergence_duration > 0)
			_pimpl->_packet.convergence_duration = av_rescale_q(_pimpl->_packet.convergence_duration, src_tb, dst_tb);
	}

	void packet::rescale_rnd(const rational &src, const rational &dst)
	{
		AVRational src_tb = { src.num, src.den };
		AVRational dst_tb = { dst.num, dst.den };
		enum AVRounding roundingType = (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
		if (_pimpl->_packet.pts != AV_NOPTS_VALUE)
			_pimpl->_packet.pts = av_rescale_q_rnd(_pimpl->_packet.pts, src_tb, dst_tb, roundingType);
		if (_pimpl->_packet.dts != AV_NOPTS_VALUE)
			_pimpl->_packet.dts = av_rescale_q_rnd(_pimpl->_packet.dts, src_tb, dst_tb, roundingType);
		if (_pimpl->_packet.duration > 0)
			_pimpl->_packet.duration = (int)av_rescale_q(_pimpl->_packet.duration, src_tb, dst_tb);
		if (_pimpl->_packet.convergence_duration > 0)
			_pimpl->_packet.convergence_duration = av_rescale_q(_pimpl->_packet.convergence_duration, src_tb, dst_tb);
	}

	int packet::stream_index()
	{
		return _pimpl->_packet.stream_index;
	}

	void packet::stream_index(int newindex)
	{
		_pimpl->_packet.stream_index = newindex;
	}
	

}