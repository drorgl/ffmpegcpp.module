/*
* bitstream filter implementation
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"

#include "bitstreamfilter.h"

#include "bitstreamfilter_pimpl.h"

#include "codec_pimpl.h"

#include "ffmpeg_includes.h"
#include "packet_pimpl.h"


#include "datastructures_conversion.h"

namespace ffmpegcpp
{
	std::vector<std::string> bitstreamfilter::listBitstreamFilters()
	{
		std::vector<std::string> outputFiltersList;
		AVBitStreamFilter * bsfilter = NULL;

		while ((bsfilter = av_bitstream_filter_next(bsfilter)) != NULL){
			outputFiltersList.push_back(bsfilter->name);
		}

		return outputFiltersList;
	}

	bitstreamfilter::bitstreamfilter(std::string name, std::string parameters, std::shared_ptr<codec> codec)
		: _pimpl(std::make_shared<bitstreamfilter::bitstreamfilter_pimpl>())
	{
		_pimpl->_parameters = parameters;
		_pimpl->_pbsFilterCtx = av_bitstream_filter_init(name.c_str());

		if (_pimpl->_pbsFilterCtx == NULL){
			throw ffmpeg_exception("unable to initialize bitstream filter " + name);
		}

		if (codec != nullptr){
			_pimpl->_codecctx = codec->_pimpl->pCodecCtx;
		}
	}

	bitstreamfilter::~bitstreamfilter(){}

	void bitstreamfilter::apply(std::shared_ptr<packet> packet)
	{
		_pimpl->filter(packet->_pimpl->_packet);
	}
}