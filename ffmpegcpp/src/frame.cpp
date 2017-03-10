/*
* frame implementation
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"

#include "frame.h"

#include "dictionary_pimpl.h"
#include "frame_pimpl.h"
#include "formatcontext_pimpl.h"
#include "datastructures_conversion.h"

namespace ffmpegcpp
{
	

	frame::frame()
		: _pimpl(std::make_shared<frame::frame_pimpl>())
	{
		
		_pimpl->pFrame = av_frame_alloc();
		//check for null
	}

	frame::~frame()
	{
		unref();
		av_frame_free(&_pimpl->pFrame);
	}

	void frame::unref()
	{
		av_frame_unref(_pimpl->pFrame);
		//avcodec_get_frame_defaults(_pimpl->pFrame);
	}

	void frame::get_buffer(int align)
	{
		assert(_pimpl->pFrame->data != NULL && "frame is not empty, it will leak");
		assert((_pimpl->pFrame->width > 0 && _pimpl->pFrame->height > 0) ||
			(_pimpl->pFrame->nb_samples > 0 && _pimpl->pFrame->channel_layout)
			&& "frame should either be set with video or audio parameters before attempting to allocate buffers");

		int retval = av_frame_get_buffer(_pimpl->pFrame, align);
		if (retval < 0){
			throw ffmpeg_exception(retval, "unable to allocate buffers for frame");
		}
	}
	
	void frame::set_video_parameters(int width, int height, pixel_format format)
	{
		_pimpl->pFrame->width = width;
		_pimpl->pFrame->height = height;
		_pimpl->pFrame->format = conversion::convertFrom(format);
	}
	void frame::set_audio_parameters(int channels, sample_format format, channel_layout channel_layout, int sample_rate, int nb_samples)
	{
		_pimpl->pFrame->channels = channels;
		_pimpl->pFrame->format = conversion::convertFrom(format);
		_pimpl->pFrame->channel_layout = channel_layout;
		_pimpl->pFrame->sample_rate = sample_rate;
		_pimpl->pFrame->nb_samples = nb_samples;
	}

}