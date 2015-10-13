/*
* swscale implementation
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"

#include "ffmpeg.h"
#include "ffmpeg_includes.h"
#include "datastructures_conversion.h"


#include "swscale.h"
#include "codec_pimpl.h"
#include "dictionary_pimpl.h"

#include "frame_pimpl.h"

#include <opencv2/core.hpp>

namespace ffmpegcpp
{
	struct swscale::swscale_pimpl
	{
		enum swsdirection _direction;

		SwsContext *pSwsContext = NULL;
		int _width = 0;
		int _height = 0;

		SwrContext *pSwrContext = NULL;
		enum channel_layout _layout;
		int _nb_channels = 0;
		enum sample_format _format;
		int _sample_rate = 0;


		AVCodecContext *_codecCtx = NULL;

		/*string type2str(int type) {
			string r;

			uchar depth = type & CV_MAT_DEPTH_MASK;
			uchar chans = 1 + (type >> CV_CN_SHIFT);

			switch (depth) {
			case CV_8U:  r = "8U"; break;
			case CV_8S:  r = "8S"; break;
			case CV_16U: r = "16U"; break;
			case CV_16S: r = "16S"; break;
			case CV_32S: r = "32S"; break;
			case CV_32F: r = "32F"; break;
			case CV_64F: r = "64F"; break;
			default:     r = "User"; break;
			}

			r += "C";
			r += (chans + '0');

			return r;
		}*/

		/*enum AVSampleFormat getFormatFromMat(cv::Mat &mat)
		{
			uchar depth = mat.type & CV_MAT_DEPTH_MASK; 

		}*/
	};

	swscale::swscale(std::weak_ptr<codec> codec_) :
		_pimpl(std::make_shared<swscale_pimpl>())
	{
		_codec = codec_;

		auto shared_codec = codec_.lock();
		_pimpl->_codecCtx = shared_codec->_pimpl->pCodecCtx;
	}

	swscale::~swscale()
	{
		freeVideoContext();
		freeAudioContext();
	}

	std::shared_ptr<swscale> swscale::fromVideoCodec(std::weak_ptr<codec> codec_, int dstWidth, int dstHeight)
	{
		auto sws = std::make_shared<swscale>(codec_);
		sws->_pimpl->_direction = swsdirection::fromCodec;
		sws->_pimpl->_width = dstWidth;
		sws->_pimpl->_height = dstHeight;
		return sws;
	}
	std::shared_ptr<swscale> swscale::fromAudioCodec(std::weak_ptr<codec> codec_, enum channel_layout layout, int nb_channels, enum sample_format format, int sample_rate)
	{
		auto sws = std::make_shared<swscale>(codec_);
		sws->_pimpl->_direction = swsdirection::fromCodec;
		sws->_pimpl->_layout = layout;
		sws->_pimpl->_nb_channels = nb_channels;
		sws->_pimpl->_format = format;
		sws->_pimpl->_sample_rate = sample_rate;
		return sws;
	}
	std::shared_ptr<swscale> swscale::toVideoCodec(std::weak_ptr<codec> codec_, int srcWidth, int srcHeight)
	{
		auto sws = std::make_shared<swscale>(codec_);
		sws->_pimpl->_direction = swsdirection::toCodec;
		sws->_pimpl->_width = srcWidth;
		sws->_pimpl->_height = srcHeight;
		return sws;
	}
	std::shared_ptr<swscale> swscale::toAudioCodec(std::weak_ptr<codec> codec_, enum channel_layout layout, int nb_channels, enum sample_format format, int sample_rate)
	{
		auto sws = std::make_shared<swscale>(codec_);
		sws->_pimpl->_direction = swsdirection::toCodec;
		sws->_pimpl->_layout = layout;
		sws->_pimpl->_nb_channels = nb_channels;
		sws->_pimpl->_format = format;
		sws->_pimpl->_sample_rate = sample_rate;
		return sws;
	}

	void swscale::freeAudioContext()
	{
		if (_pimpl->pSwrContext != NULL){
			swr_free(&_pimpl->pSwrContext);
			_pimpl->pSwrContext = NULL;
		}
	}

	void swscale::freeVideoContext()
	{
		if (_pimpl->pSwsContext != NULL){
			sws_freeContext(_pimpl->pSwsContext);
			_pimpl->pSwsContext = NULL;
		}
	}

	void swscale::prepareVideoContext()
	{

		if (_pimpl->pSwsContext != NULL){
			return;
		}

		switch (_pimpl->_direction)
		{
		case swsdirection::fromCodec:
			_pimpl->pSwsContext = sws_getContext(_pimpl->_codecCtx->width, _pimpl->_codecCtx->height,_pimpl->_codecCtx->pix_fmt, 
				_pimpl->_width, _pimpl->_height, AVPixelFormat::PIX_FMT_BGR24, SWS_POINT, NULL, NULL, NULL);
			break;
		case swsdirection::toCodec:
			_pimpl->pSwsContext = sws_getContext(_pimpl->_width, _pimpl->_height, AVPixelFormat::PIX_FMT_BGR24,
				_pimpl->_codecCtx->width, _pimpl->_codecCtx->height,_pimpl->_codecCtx->pix_fmt, SWS_POINT, NULL, NULL, NULL);
			break;
		default:
			throw ffmpeg_exception("Cannot initialize the video conversion context! unimplemented direction!");
			break;
		}
		
		if (_pimpl->pSwsContext == NULL)
		{
			throw ffmpeg_exception("Cannot initialize the video conversion context!");
		}

	}
	void swscale::prepareAudioContext()
	{
		if (_pimpl->pSwrContext != NULL){
			return;
		}

		/*int dec_channel_layout =
			(_codec->_pimpl->pCodecCtx->channel_layout && _codec->_pimpl->pCodecCtx->channels == av_get_channel_layout_nb_channels(_codec->_pimpl->pCodecCtx->channel_layout)) ?
			_codec->_pimpl->pCodecCtx->channel_layout : av_get_default_channel_layout(_codec->_pimpl->pCodecCtx->channels);
*/
		switch (_pimpl->_direction)
		{
		case swsdirection::fromCodec:
			_pimpl->pSwrContext = swr_alloc_set_opts(NULL,
				_pimpl->_layout,
				conversion::convertFrom(_pimpl->_format),
				_pimpl->_sample_rate,
				//dec_channel_layout,
				_pimpl->_codecCtx->channel_layout,
				_pimpl->_codecCtx->sample_fmt,
				_pimpl->_codecCtx->sample_rate,
				0,
				NULL);
			break;
		case swsdirection::toCodec:
			_pimpl->pSwrContext = swr_alloc_set_opts(NULL,
				_pimpl->_codecCtx->channel_layout,
				_pimpl->_codecCtx->sample_fmt,
				_pimpl->_codecCtx->sample_rate,

				_pimpl->_layout,
				conversion::convertFrom(_pimpl->_format),
				_pimpl->_sample_rate,
				0,
				NULL);
			break;
		default:
			throw ffmpeg_exception("Cannot initialize the audio conversion context! unimplemented direction!");
			break;
		}
		

		
		if (!_pimpl->pSwrContext || swr_init(_pimpl->pSwrContext) < 0) {
			throw ffmpeg_exception("Cannot initialize the audio conversion context!");
		}

	}

	void swscale::scaleVideo(frame *frame_, cv::Mat &mat)
	{
		prepareVideoContext();

		uint8_t *matdata[AV_NUM_DATA_POINTERS];
		matdata[0] = mat.data;
		int stepsize[AV_NUM_DATA_POINTERS];
		stepsize[0] = (int)mat.step1();

		int retval;

		switch (_pimpl->_direction)
		{
		case swsdirection::fromCodec:
			retval = sws_scale(_pimpl->pSwsContext, frame_->_pimpl->pFrame->data, frame_->_pimpl->pFrame->linesize, 0, _pimpl->_codecCtx->height, matdata, stepsize);
			break;
		case swsdirection::toCodec:
			retval = sws_scale(_pimpl->pSwsContext, matdata, stepsize,  0, _pimpl->_height, frame_->_pimpl->pFrame->data, frame_->_pimpl->pFrame->linesize);
			break;
		default:
			throw ffmpeg_exception("Cannot convert video! unimplemented direction!");
			break;
		}


		if (retval < 1){
			throw ffmpeg_exception(retval, "video scale failed");
		}
	}

	void swscale::scaleAudio(frame * frame_, cv::Mat &mat, int &samplesRead)
	{
		prepareAudioContext();
		
		//uint8_t *input[] = { frame_->_pimpl->pFrame->data[0] };
		//uint8_t *output[] = { mat.data};
		

		//throw exception if attempting to scale an audio with too many channels
		if (mat.rows > AV_NUM_DATA_POINTERS){
			throw ffmpeg_exception("attempting to scale audio beyond AV_NUM_DATA_POINTERS, too many channels or incorrect mat rows, try to use non-planar audio format");
		}

		

		uint8_t *output[AV_NUM_DATA_POINTERS];
		memset(output, 0, AV_NUM_DATA_POINTERS * sizeof(uint8_t*));
		for (auto i = 0; i < mat.rows; i++){
			output[i] = mat.data + (mat.step * i);
		}

		
		if (isSampleFormatPlanar(_pimpl->_format)){
			if (mat.rows < getChannelLayoutNumberOfChannels(_pimpl->_layout)){
				throw ffmpeg_exception("error converting planar audio, mat rows are smaller than number of channels specified");
			}
		}



		int result; 

		switch (_pimpl->_direction)
		{
		case swsdirection::fromCodec:
			result = swr_convert(_pimpl->pSwrContext, (uint8_t **)&output, mat.cols, (const uint8_t**)&frame_->_pimpl->pFrame->data, frame_->_pimpl->pFrame->nb_samples);
			break;
		case swsdirection::toCodec:
			result = swr_convert(_pimpl->pSwrContext, frame_->_pimpl->pFrame->data, frame_->_pimpl->pFrame->nb_samples, (const uint8_t **)&output, samplesRead);
			break;
		default:
			throw ffmpeg_exception("Cannot convert audio! unimplemented direction!");
			break;
		}

		if (result < 1){
			throw ffmpeg_exception(result, "audio conversion failed");
		}
		samplesRead = result;
	}
}