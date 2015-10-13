/*
* decoder implementation
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"

#include "decoder.h"

#include "ffmpeg_includes.h"
#include "decoder_pimpl.h"
#include "codec_pimpl.h"
#include "dictionary_pimpl.h"
#include "formatcontext_pimpl.h"
#include "frame_pimpl.h"
#include "packet_pimpl.h"

namespace ffmpegcpp
{
	decoder::decoder(std::shared_ptr<codec> codec_)
		: _pimpl(std::make_shared<decoder::decoder_pimpl>())
	{
		_codec = codec_;
	}

	void decoder::open(const std::shared_ptr<dictionary> &options)
	{
		_codec->openAsDecoder(options);
	}

	bool decoder::decode(packet &packet_, frame &frame_)
	{
		assert(_codec->_wasOpened && "decoder is not opened");

		int frameDecoded = 0;

		//make sure this decoder is decoding a packet that belongs to its stream
		int result = -1;

		switch (_codec->mediaType())
		{
		case mediaType::audio:
			result = avcodec_decode_audio4(_codec->_pimpl->pCodecCtx, frame_._pimpl->pFrame, &frameDecoded, &packet_._pimpl->_packet);
			break;
		case mediaType::video:
			result = avcodec_decode_video2(_codec->_pimpl->pCodecCtx, frame_._pimpl->pFrame, &frameDecoded, &packet_._pimpl->_packet);
			break;
			default:
				throw ffmpeg_exception("unable to decode packet, media type not implemented");
				break;
		}

		if (result < 0){
			throw ffmpeg_exception(result, "unable to decode packet");
		}

		//if packet was processed, it needs to be free, otherwise the next read packet will just override its buffers and we have an orphan allocation (leak).
		packet_.unref();

		if (frameDecoded){
			frame_._pimpl->pFrame->pts = av_frame_get_best_effort_timestamp(frame_._pimpl->pFrame);
			return true;
		}

		return false;
	}

	
}