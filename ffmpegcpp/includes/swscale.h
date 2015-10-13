/*
* swscale
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _FFMPEGCPP_SWSCALE_H_
#define _FFMPEGCPP_SWSCALE_H_

#define CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
//#include <crtdbg.h>

#include <memory>

#include "exports.h"

namespace cv
{
	class Mat;
}

namespace ffmpegcpp
{
	enum swsdirection
	{
		fromCodec,
		toCodec
	};

	class frame;
	class codec;

	

	class swscale
	{
	private:
		struct swscale_pimpl;
		std::shared_ptr<swscale_pimpl> _pimpl;

		std::weak_ptr<codec> _codec;

		void prepareVideoContext();
		void freeVideoContext();

		void prepareAudioContext();
		void freeAudioContext();

		
	public:
		swscale(const swscale& that) = delete;
		swscale& operator=(const swscale&) = delete;
		//DO NOT CALL DIRECTLY
		swscale(std::weak_ptr<codec> codec_);
		~swscale();

		//convert from video codec to selected width and height
		FFMPEGCPP_API static std::shared_ptr<swscale> fromVideoCodec(std::weak_ptr<codec> codec_, int dstWidth, int dstHeight);

		//convert from audio codec to selected layout/channels/format/sample rate
		FFMPEGCPP_API static std::shared_ptr<swscale> fromAudioCodec(std::weak_ptr<codec> codec_, enum channel_layout layout, int nb_channels, enum sample_format format, int sample_rate);

		//convert from selected width/height to codec
		FFMPEGCPP_API static std::shared_ptr<swscale> toVideoCodec(std::weak_ptr<codec> codec_, int srcWidth, int srcHeight);

		//convert from selected layout/channels/format/sample rate to codec
		FFMPEGCPP_API static std::shared_ptr<swscale> toAudioCodec(std::weak_ptr<codec> codec_, enum channel_layout layout, int nb_channels, enum sample_format format, int sample_rate);

		//convert between codec and mat
		FFMPEGCPP_API void scaleVideo(frame *frame_, cv::Mat &mat);

		//convert between codec and mat, 
		//if from codec then samplesRead are the number of samples read from codec to mat
		//if to codec then samplesRead are the number of samples to read from mat to codec
		FFMPEGCPP_API void scaleAudio(frame *frame_, cv::Mat &mat, int &samplesRead);
	};
}

#endif