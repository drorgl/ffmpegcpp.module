/*
* bitstream filter shared implementation
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _FFMPEGCPP_BITSTREAMFILTER_PIMPL_H_
#define _FFMPEGCPP_BITSTREAMFILTER_PIMPL_H_

extern "C" {
	#include <libavcodec/avcodec.h>
	#include <libavutil/pixdesc.h>
}
#include "../includes/bitstreamfilter.h"


namespace ffmpegcpp
{
	struct bitstreamfilter::bitstreamfilter_pimpl
	{
		AVBitStreamFilterContext* _pbsFilterCtx = NULL;
		std::string _parameters;
		AVCodecContext * _codecctx = NULL;

		~bitstreamfilter_pimpl()
		{
			av_bitstream_filter_close(_pbsFilterCtx);
			_codecctx = NULL;
		}

		void filter(AVPacket &_packet)
		{
			uint8_t *poutbuf;
			int poutbuf_size;

			av_packet_split_side_data(&_packet);

			auto ret = av_bitstream_filter_filter(_pbsFilterCtx, _codecctx, _parameters.c_str(),
				&poutbuf,
				&poutbuf_size,
				_packet.data,
				_packet.size,
				_packet.flags & AV_PKT_FLAG_KEY);

			if (ret < 0){ //nop
				throw ffmpegcpp::ffmpeg_exception(ret, "unable to filter packet " + std::string( _pbsFilterCtx->filter->name));
			}
			else if (ret > 0){
				if (poutbuf_size > _packet.size){
					av_grow_packet(&_packet, poutbuf_size - _packet.size);
				}
				else if (poutbuf_size < _packet.size){
					av_shrink_packet(&_packet, poutbuf_size);
				}
				memcpy(_packet.data, poutbuf, poutbuf_size);

				//free only in case the av_bitstream_filter_filter was successful
				av_free(poutbuf);
			}
			
		}
	};

}

#endif