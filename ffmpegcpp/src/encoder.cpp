/*
* encoder implementation
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"

#include "encoder.h"

#include "ffmpeg_includes.h"
#include "codec_pimpl.h"
#include "dictionary_pimpl.h"
#include "formatcontext_pimpl.h"
#include "frame_pimpl.h"
#include "packet_pimpl.h"
#include "stream_pimpl.h"

namespace ffmpegcpp
{
	struct encoder::encoder_pimpl
	{
		int _localPts = 0;
	};

	encoder::encoder(std::shared_ptr<stream> stream_, std::shared_ptr<codec> codec_)
		: _pimpl(std::make_shared<encoder_pimpl>()){
		_codec = codec_;
		_stream = stream_;
	}

	void encoder::open(const std::shared_ptr<dictionary> &options)
	{
		_codec->openAsEncoder(options);
	}

	bool encoder::CodecNeedsFlushing()
	{
		return ((_codec->_pimpl->pCodecCtx->codec->capabilities & CODEC_CAP_DELAY) != 0);
	}

	bool encoder::encode(frame *frame_, packet *packet_)
	{
		assert(_codec->_wasOpened && "encoder is not opened");

		AVPacket *p = (packet_ != NULL) ? &packet_->_pimpl->_packet : NULL;
		AVFrame *f = (frame_ != NULL) ? frame_->_pimpl->pFrame : NULL;

		assert(p != NULL && "encode got null packet");

		p->flags |= AV_PKT_FLAG_KEY;

		int retval = -1;
		int got_packet_ptr = 0;

		switch (_codec->mediaType())
		{
		case ffmpegcpp::mediaType::video:
			//f->pts = _codec->_pimpl->pCodecCtx->frame_number * _codec->_pimpl->pCodecCtx->ticks_per_frame;
			if (f != NULL){ //needed for flushing
				f->pts = _pimpl->_localPts;
				_pimpl->_localPts++;
			}
			retval = avcodec_encode_video2(_codec->_pimpl->pCodecCtx, p, f, &got_packet_ptr);
			break;
		case ffmpegcpp::mediaType::audio:
		{
											if (f != NULL){ //needed for flushing
												auto codecTimeBase = _codec->_pimpl->pCodecCtx->time_base;
												f->pts = av_rescale_q(_pimpl->_localPts, AVRational{ 1, _codec->_pimpl->pCodecCtx->sample_rate }, AVRational{ codecTimeBase.num, codecTimeBase.den });
												_pimpl->_localPts += f->nb_samples;
											}
		}
			retval = avcodec_encode_audio2(_codec->_pimpl->pCodecCtx, p, f, &got_packet_ptr);
			break;
		default:
			throw ffmpeg_exception("unable to encode media type, not implemented");
		}

		if (retval < -1){
			throw ffmpeg_exception(retval, "failed to encode frame");
		}

		if (got_packet_ptr > 0)
		{
			packet_->_pimpl->_packet.pos = -1;

			auto codecTimeBase = _codec->_pimpl->pCodecCtx->time_base;
			auto streamTimeBase = _stream->_pimpl->pStream->time_base;
			packet_->rescale_rnd(ffmpegcpp::rational(codecTimeBase.num, codecTimeBase.den), ffmpegcpp::rational(streamTimeBase.num, streamTimeBase.den));

			//some encoding functions cause the packet stream index not to match the stream index!
			//assert(pt->stream_index() == _stream->index());
			packet_->_pimpl->_packet.stream_index = _stream->index();

			return true;
		}

		return false;
	}

	bool encoder::flush(packet *packet_)
	{
		//check if codec supports flushing, otherwise, no need to flush.
		if (!this->CodecNeedsFlushing()){
			return false;
		}

		return encode(NULL, packet_);

		//_codec->flush();
	}


}