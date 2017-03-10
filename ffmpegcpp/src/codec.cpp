/*
* codec implementation
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"

#include "codec.h"

#include "codec_pimpl.h"
#include "stream_pimpl.h"
#include "dictionary_pimpl.h"

#include "datastructures_conversion.h"

namespace ffmpegcpp
{
	codec::codec(std::shared_ptr<stream> stream_)
		: _pimpl(std::make_shared<codec::codec_pimpl>())
	{
		_stream = stream_;
		//initialize codec
		_pimpl->pCodecCtx = stream_->_pimpl->pStream->codec;
		
		assert(_pimpl->pCodecCtx != NULL);
	}

	codec::~codec()
	{
		close();
	}

	void codec::open(const std::shared_ptr<dictionary> &options)
	{
		auto dict = (options != nullptr) ? options->_pimpl->toAVDictionary() : NULL; 

		//check if it is actually found
		if (_pimpl->pCodecCtx->codec == NULL){
			throw ffmpeg_exception("unable to find a compatible codec");
		}

		int retval = avcodec_open2(_pimpl->pCodecCtx, _pimpl->pCodecCtx->codec, &dict);
		
		if (options != nullptr){
			options->_pimpl->fromAVDictionary(&dict, true, true);
		}


		if (retval < 0){
			throw ffmpeg_exception(retval, "unable to open codec!");
		}

	}

	void codec::flush(bool immediate)
	{
		if (!immediate){
			if ((_flushCounter % _flushInterval) != 0){
				if (_flushCounter > 100000) //magic number!
				{
					_flushCounter = 0;
				}
				return;
			}
			
		}
		avcodec_flush_buffers(_pimpl->pCodecCtx);
	}

	void codec::openAsDecoder(const std::shared_ptr<dictionary> &options)
	{
		if (_wasOpened == true){
			throw ffmpeg_exception("codec was already opened");
		}

		//find decoder
		if (_pimpl->pCodecCtx->codec == NULL){
			_pimpl->pCodecCtx->codec = avcodec_find_decoder(_pimpl->pCodecCtx->codec_id);

			if (_pimpl->pCodecCtx->codec == NULL){
				throw ffmpeg_exception("unable to find a compatible decoder: " + codec_pimpl::get_codec_name(_pimpl->pCodecCtx->codec_id));
			}
		}

		open(options);

		_wasOpened = true;
	}

	void codec::openAsEncoder(const std::shared_ptr<dictionary> &options)
	{
		if (_wasOpened == true){
			throw ffmpeg_exception("codec was already opened");
		}

		//find encoder
		if (_pimpl->pCodecCtx->codec == NULL){
			_pimpl->pCodecCtx->codec = avcodec_find_encoder(_pimpl->pCodecCtx->codec_id);

			if (_pimpl->pCodecCtx->codec == NULL){
				throw ffmpeg_exception("unable to find a compatible encoder: " + codec_pimpl::get_codec_name(_pimpl->pCodecCtx->codec_id) );
			}
		}

		open(options);

		_wasOpened = true;
	}

	void codec::close()
	{
		if (_wasOpened == true){
			avcodec_close(_pimpl->pCodecCtx);
			avcodec_free_context(&_pimpl->pCodecCtx);
			_wasOpened = false;
		}
	}


	const int codec::width()
	{
		return _pimpl->pCodecCtx->width;
	}

	void codec::width(int width_){
		if (_wasOpened == true){
			throw ffmpeg_exception("unable to set value on opened codec");
		}
		_pimpl->pCodecCtx->width = width_;
	}

	const int codec::height()
	{
		return _pimpl->pCodecCtx->height;
	}

	void codec::height(int height_){
		if (_wasOpened == true){
			throw ffmpeg_exception("unable to set value on opened codec");
		}
		_pimpl->pCodecCtx->height = height_;
	}

	const pixel_format codec::pix_fmt(){
		return conversion::convertFrom(_pimpl->pCodecCtx->pix_fmt);
	}
	void codec::pix_fmt(pixel_format pix_fmt_){
		if (_wasOpened == true){
			throw ffmpeg_exception("unable to set value on opened codec");
		}
		_pimpl->pCodecCtx->pix_fmt = conversion::convertFrom(pix_fmt_);
	}

	
	const std::string codec::name()
	{
		if (_wasOpened == false){
			throw ffmpeg_exception("must open codec before calling name");
		}
		return _pimpl->pCodecCtx->codec->name;
	}

	const int codec::sample_rate()
	{
		return _pimpl->pCodecCtx->sample_rate;
	}

	void codec::sample_rate(int sample_rate_){
		if (_wasOpened == true){
			throw ffmpeg_exception("unable to set value on opened codec");
		}
		_pimpl->pCodecCtx->sample_rate = sample_rate_;
	}

	const int codec::channels()
	{
		return _pimpl->pCodecCtx->channels;
	}

	void codec::channels(int channels_){
		if (_wasOpened == true){
			throw ffmpeg_exception("unable to set value on opened codec");
		}
		_pimpl->pCodecCtx->channels = channels_;
	}

	const int codec::bit_rate()
	{
		return _pimpl->pCodecCtx->bit_rate;
	}
	void codec::bit_rate(int bit_rate_){
		if (_wasOpened == true){
			throw ffmpeg_exception("unable to set value on opened codec");
		}
		_pimpl->pCodecCtx->bit_rate = bit_rate_;
	}

	const enum ffmpegcpp::channel_layout codec::channels_layout(){
		return (enum ffmpegcpp::channel_layout)_pimpl->pCodecCtx->channel_layout;
	}
	void codec::channels_layout(enum ffmpegcpp::channel_layout channels_layout_){
		if (_wasOpened == true){
			throw ffmpeg_exception("unable to set value on opened codec");
		}
		_pimpl->pCodecCtx->channel_layout = channels_layout_;
	}

	const enum ffmpegcpp::sample_format codec::sample_fmt()
	{
		return conversion::convertFrom(_pimpl->pCodecCtx->sample_fmt);
	}

	void codec::sample_fmt(enum ffmpegcpp::sample_format sample_fmt_){
		if (_wasOpened == true){
			throw ffmpeg_exception("unable to set value on opened codec");
		}
		_pimpl->pCodecCtx->sample_fmt = conversion::convertFrom(sample_fmt_);
	}

	const ffmpegcpp::rational codec::time_base(){
		return rational(_pimpl->pCodecCtx->time_base.num, _pimpl->pCodecCtx->time_base.den);
	}
	void codec::time_base(ffmpegcpp::rational time_base_){
		if (_wasOpened == true){
			throw ffmpeg_exception("unable to set value on opened codec");
		}
		_pimpl->pCodecCtx->time_base = AVRational{ time_base_.num, time_base_.den };
	}

	const int codec::gop_size(){
		return _pimpl->pCodecCtx->gop_size;
	}

	void codec::gop_size(int gop_size_){
		if (_wasOpened == true){
			throw ffmpeg_exception("unable to set value on opened codec");
		}
		_pimpl->pCodecCtx->gop_size = gop_size_;
	}

	const enum ffmpegcpp::mediaType codec::mediaType()
	{
		return conversion::convertFrom(this->_pimpl->pCodecCtx->codec_type);
	}

	
	std::vector<codecinfo> codec::listCodecs()
	{
		//assert(ffmpeg::InstanceCount() > 0 && "ffmpeg not initialized, at least one live instance must exist");

		std::vector<codecinfo> outputCodecList;
		AVCodec * ccodec = NULL;

		while ((ccodec = av_codec_next(ccodec)) != NULL){
			codecinfo c;

			c.isEncoder = (av_codec_is_encoder(ccodec) != 0) ? true : false;
			c.isDecoder = (av_codec_is_decoder(ccodec) != 0) ? true : false;

			c.name = ccodec->name;
			if (ccodec->long_name != NULL){
				c.long_name = ccodec->long_name;
			}
			c.mediaType = conversion::convertFrom(ccodec->type);
			c.capabilities = (codec_capabilities)ccodec->capabilities;

			const AVRational *sfr = ccodec->supported_framerates;
			while (sfr != NULL && sfr->num != 0 && sfr->den != 0)
			{
				c.frameRates.push_back(rational(sfr->num, sfr->den));
				sfr++;
			}


			const AVPixelFormat *pf = ccodec->pix_fmts;
			while (pf != NULL && *pf != -1)
			{
				c.pixelFormats.push_back(conversion::convertFrom(*pf));
				pf++;
			}

			const int *sr = ccodec->supported_samplerates;
			while (sr != NULL && *sr != 0)
			{
				c.sampleRates.push_back(*sr);
				sr++;
			}

			const AVSampleFormat *sf = ccodec->sample_fmts;
			while (sf != NULL && *sf != -1)
			{
				c.sampleFormats.push_back(conversion::convertFrom(*sf));
				sf++;
			}

			const uint64_t *cl = ccodec->channel_layouts;
			while (cl != NULL && *cl != 0)
			{
				c.channelLayouts.push_back((channel_layout)*cl);
				cl++;
			}

			c.max_lowres = ccodec->max_lowres;
			//av_codec_get_lowres() - receives codec ctx!!
			

			const AVProfile * pr = ccodec->profiles;
			while (pr != NULL && pr->profile != FF_PROFILE_UNKNOWN)
			{
				c.profiles.push_back(codec_profile(pr->profile, pr->name));
				pr++;
			}

			outputCodecList.push_back(c);
		}

		return outputCodecList;
	}
}