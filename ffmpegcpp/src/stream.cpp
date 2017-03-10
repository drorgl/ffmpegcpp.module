/*
* stream implementation
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

#include "stream.h"

#include "formatcontext_pimpl.h"
#include "stream_pimpl.h"
#include "codec_pimpl.h"
#include "decoder_pimpl.h"
#include "dictionary_pimpl.h"
#include "datastructures_conversion.h"

namespace ffmpegcpp
{

	stream::stream(std::shared_ptr<ffmpegcpp::formatcontext> context_)
		: _pimpl(std::make_shared<stream_pimpl>())
	{
		_context = context_;
		//_frame = std::make_shared<ffmpegcpp::frame>();
	}

	std::shared_ptr<std::vector<std::shared_ptr<stream>>> stream::getStreams(std::shared_ptr<ffmpegcpp::formatcontext> context_)
	{
		auto vector = std::make_shared<std::vector<std::shared_ptr<stream>>>();
		for (unsigned int i = 0; i < context_->_pimpl->_pFormatCtx->nb_streams; i++){

			auto s = std::make_shared<stream>(context_);
			s->_pimpl->pStream = context_->_pimpl->_pFormatCtx->streams[i];
			//s->context = context_;
			vector->push_back(s);
		}

		return vector;
	}




	std::shared_ptr<stream> stream::addStream(std::shared_ptr<formatcontext> context_, std::string codecName, rational stream_time_base)
	{
		auto s = std::make_shared<stream>(context_);

		auto codec = codec::codec_pimpl::find_encoder_by_name(codecName);
		s->_pimpl->pStream = avformat_new_stream(context_->_pimpl->_pFormatCtx, codec);

		s->_pimpl->pStream->id = context_->_pimpl->_pFormatCtx->nb_streams - 1;
		s->_pimpl->pStream->time_base = AVRational{ stream_time_base.num, stream_time_base.den };

		//configure codec
		auto codecCtx = s->_pimpl->pStream->codec;
		avcodec_get_context_defaults3(codecCtx, codec);

		codecCtx->codec_id = codec->id;
		codecCtx->codec = codec;
		

		//codecCtx->sample_fmt = conversion::convertFrom(config.sample_format);
		//codecCtx->bit_rate = config.bit_rate;
		//codecCtx->sample_rate = config.sample_rate;
		//codecCtx->channels = config.channels;
		//codecCtx->channel_layout = config.channel_layout;
		////for codecs that support variable bitrate
		//codecCtx->flags |= CODEC_FLAG_QSCALE;
		//codecCtx->global_quality = 1 * FF_QP2LAMBDA;



		//codecCtx->width = config.width;
		//codecCtx->height = config.height;
		//codecCtx->time_base = AVRational{config.time_base.num, config.time_base.den};

		//codecCtx->gop_size = config.gop_size;

		////TODO, this should be the codec's choice!!
		//codecCtx->pix_fmt = AV_PIX_FMT_YUV420P;

		if (codecCtx->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
			/* just for testing, we also add B frames */
			codecCtx->max_b_frames = 2;
		}
		if (codecCtx->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
			/* Needed to avoid using macroblocks in which some coeffs overflow.
			* This does not happen with normal video, it just happens here as
			* the motion of the chroma plane does not match the luma plane. */
			codecCtx->mb_decision = 2;
		}

		if (context_->_pimpl->_pFormatCtx->oformat->flags & AVFMT_GLOBALHEADER){
			codecCtx->flags |= CODEC_FLAG_GLOBAL_HEADER;
		}

		return s;
	}

	const std::shared_ptr<formatcontext> stream::getContext()
	{
		return _context;
	}

	const std::shared_ptr<codec> stream::getCodec()
	{
		if (_codec == nullptr){
			_codec = std::make_shared<ffmpegcpp::codec>(shared_from_this());
		}
		return _codec;
	}

	const std::shared_ptr<decoder> stream::getDecoder()
	{
		if (_decoder == nullptr){
			_decoder = std::make_shared<ffmpegcpp::decoder>(this->getCodec());
		}
		return _decoder;
	}

	const std::shared_ptr<encoder> stream::getEncoder()
	{
		if (_encoder == nullptr){
			_encoder = std::make_shared<ffmpegcpp::encoder>(shared_from_this(), this->getCodec());
		}
		return _encoder;
	}

	const void stream::addBitstreamFilter(std::shared_ptr<bitstreamfilter> bsfilter)
	{
		_bsfilters.push_back(bsfilter);
	}

/*
	const enum mediaType stream::media()
	{
		return this->codec()->mediaType();
	}*/

	/*FFMPEGCPP_API const cvMatDefinition stream::sample_fmt_mat()
	{
		return this->codec()->sample_fmt_mat();
	}*/

	const rational stream::r_frame_rate()
	{
		return rational(_pimpl->pStream->r_frame_rate.num, _pimpl->pStream->r_frame_rate.den);
	}

	const rational stream::time_base()
	{
		return rational(_pimpl->pStream->time_base.num, _pimpl->pStream->time_base.den);
	}

	const int stream::index()
	{
		return _pimpl->pStream->index;
	}

	const int64_t stream::duration()
	{
		return _pimpl->pStream->duration;
	}
/*
	const int stream::width()
	{
		return this->codec()->width();
	}
	const int stream::height()
	{
		return this->codec()->height();

	}*/
/*
	const int stream::sample_rate()
	{
		return this->codec()->sample_rate();
	}
	const int stream::channels()
	{
		return this->codec()->channels();
	}
	const enum ffmpegcpp::sample_format stream::sample_fmt()
	{
		return this->codec()->sample_fmt();
	}
*/
}