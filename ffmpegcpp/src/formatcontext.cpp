/*
* format context implementation
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"

#include "formatcontext.h"

#include "formatcontext_pimpl.h"
#include "memorycontext_pimpl.h"
#include "bitstreamfilter_pimpl.h"
#include "packet_pimpl.h"

#include "stringutils.h"

namespace ffmpegcpp
{
	formatcontext::formatcontext()
		: _pimpl(std::make_shared<formatcontext::formatcontext_pimpl>()) {}

	const std::shared_ptr<std::vector<std::shared_ptr<stream>>> &formatcontext::getStreams()
	{
		return _pimpl->getStreams(shared_from_this());
	}

	std::shared_ptr<stream> formatcontext::addStream(std::string codecName, rational stream_time_base)
	{
		return _pimpl->addStream(shared_from_this(), codecName,stream_time_base);
	}


	readPacketState formatcontext::readPacket(std::shared_ptr<packet> packet)
	{
		if (packet == nullptr){
			throw ffmpeg_exception("readPacket must be passed an allocated packet");
		}

		int result = av_read_frame(_pimpl->_pFormatCtx, &packet->_pimpl->_packet);
		if (result == AVERROR_EOF){ // || (_pimpl->_pFormatCtx->pb && _pimpl->_pFormatCtx->pb->eof_reached) 
			av_log(_pimpl->_pFormatCtx, AV_LOG_INFO, "av_read_frame EOF");
			return readPacketState::eof;
		}
		else if (result == AVERROR(EAGAIN)){
			av_log(_pimpl->_pFormatCtx, AV_LOG_DEBUG, "av_read_frame EAGAIN");
			return readPacketState::again;
		} else if (result < 0){
			throw ffmpeg_exception(result, "unable to read frame");
		}

		_bitstreamfilter_packet(*packet);

		return readPacketState::success;
	}

	void formatcontext::_bitstreamfilter_packet(packet &packet_)
	{
		auto stream = _pimpl->getStream(packet_.stream_index());

		if (stream == nullptr)
		{
			throw ffmpegcpp::ffmpeg_exception("unable to write packet, attempted to write packet index " + std::to_string(packet_.stream_index()) + " and it does not have a stream associated");
		}

		for (auto bsfilter : stream->_bsfilters){
			bsfilter->_pimpl->filter(packet_._pimpl->_packet);
		}
	}

	void formatcontext::write_packet(packet &packet_)
	{
		assert(_pimpl->_outputWroteHeader == true && "header needs to be written first");

		_bitstreamfilter_packet(packet_);

		int retval = av_write_frame(_pimpl->_pFormatCtx, &packet_._pimpl->_packet);
		if (retval < 0){
			throw ffmpeg_exception(retval, "failed to write frame");
		}
		
	}

	void formatcontext::write_packet_interleaved(packet &packet_)
	{
		assert(_pimpl->_outputWroteHeader == true && "header needs to be written first");

		_bitstreamfilter_packet(packet_);

		int retval = av_interleaved_write_frame(_pimpl->_pFormatCtx, &packet_._pimpl->_packet);
		if (retval < 0){
			throw ffmpeg_exception(retval, "failed to write interleaved frame");
		}
	}

	//write muxer header
	void formatcontext::write_header(const std::shared_ptr<dictionary> options_, const std::shared_ptr<dictionary> metadata_)
	{
		assert(_pimpl->_outputWroteHeader == false && "header already written");

		_pimpl->dump_format();


		if (_pimpl->_memoryctx != nullptr){
			_pimpl->_pFormatCtx->pb = _pimpl->_memoryctx->_pimpl->_iobuffer->_io_context;
			_pimpl->_pFormatCtx->flags |= AVFMT_FLAG_CUSTOM_IO;

			//_pimpl->_pFormatCtx->max_delay = (int)(0.7*AV_TIME_BASE);

		}else if (!(_pimpl->_pFormatCtx->flags & AVFMT_NOFILE) && !(_pimpl->_pFormatCtx->flags & AVFMT_FLAG_CUSTOM_IO)) {
			int result = avio_open(&_pimpl->_pFormatCtx->pb, _pimpl->_filename.c_str(), AVIO_FLAG_WRITE);
			if (result < 0) {
				throw ffmpeg_exception(result, "Could not open " + _pimpl->_filename);
			}
		}

		auto dict = (options_ != nullptr) ? options_->_pimpl->toAVDictionary() : NULL;

		//append metadata to formatcontext if provided
		if (metadata_ != nullptr){
			metadata_->_pimpl->fromAVDictionary(&_pimpl->_pFormatCtx->metadata, false, true);
			_pimpl->_pFormatCtx->metadata = metadata_->_pimpl->toAVDictionary();
		}

		auto result = avformat_write_header(_pimpl->_pFormatCtx, &dict);

		if (options_ != nullptr){
			options_->_pimpl->fromAVDictionary(&dict, true, true);
		}

		if (result < 0){
			throw ffmpeg_exception(result, "unable to write header");
		}

		

		_pimpl->_outputWroteHeader = true;
	}

	void formatcontext::close()
	{
		//handle output
		if ((_pimpl->_outputFormat != NULL) && (_pimpl->_pFormatCtx->nb_streams > 0) && (_pimpl->_outputWroteHeader == true)){

			//flush codecs/streams
			packet output_packet;

			for (auto stream : *this->getStreams())
			{
				output_packet.unref();
				while (stream->getEncoder()->flush(&output_packet)){
					this->write_packet_interleaved(output_packet);
					output_packet.unref();
				}
			}

			

			av_write_trailer(_pimpl->_pFormatCtx);
			if (!(_pimpl->_pFormatCtx->flags & AVFMT_FLAG_CUSTOM_IO)){
				avio_close(_pimpl->_pFormatCtx->pb);
			}
		}
		
		//close input is also in the destructor of context_pimpl.
		if (_pimpl->_inputFormat != NULL){
			avformat_close_input(&_pimpl->_pFormatCtx);
			_pimpl->_inputFormat = NULL;
		}

		//throw ffmpeg_exception("formatcontext::close : not implemented ");
	}

	void formatcontext::listInputFormats(std::vector<component> &list)
	{
		assert(ffmpeg::instance_count() > 0 && "ffmpeg not initialized, at least one live instance must exist");

		AVInputFormat * nextFormat = NULL;

		while ((nextFormat = av_iformat_next(nextFormat)) != NULL){
			component c;
			if (nextFormat->name != NULL){
				c.name = nextFormat->name;
			}

			if (nextFormat->long_name != NULL){
				c.long_name = nextFormat->long_name;
			}
			c.flags = (component_flags)nextFormat->flags;
			
			if (nextFormat->priv_class != NULL){
				c.options = dictionary::get_options(const_cast<AVClass**>(&nextFormat->priv_class));
			}
			

			list.push_back(c);
		}
	}
	void formatcontext::listOutputFormats(std::vector<component> &list)
	{
		assert(ffmpeg::instance_count() > 0 && "ffmpeg not initialized, at least one live instance must exist");

		AVOutputFormat * nextFormat = NULL;

		while ((nextFormat = av_oformat_next(nextFormat)) != NULL){
			component c;
			if (nextFormat->name != NULL){
				c.name = nextFormat->name;
			}

			if (nextFormat->long_name != NULL){
				c.long_name = nextFormat->long_name;
			}

			c.flags = (component_flags)nextFormat->flags;

			if (nextFormat->mime_type != NULL){
				c.mime_type = nextFormat->mime_type;
			}

			if (nextFormat->extensions != NULL){
				c.extensions = nextFormat->extensions;
			}
			//nextFormat->audio_codec; //consider implementing
			//nextFormat->video_codec; //consider implementing
			//nextFormat->subtitle_codec; //consider implementing

			if (nextFormat->priv_class != NULL){
				c.options = dictionary::get_options(const_cast<AVClass**>(&nextFormat->priv_class));
			}

			list.push_back(c);
		}
	}

	std::string formatcontext::getSDP()
	{
		std::string retval(4096, ' ');

		auto result = av_sdp_create(&_pimpl->_pFormatCtx, 1, &retval[0], (int)retval.size());
		if (result < 0){
			throw ffmpeg_exception(result, "unable to get SDP");
		}

		inplace_fulltrim(retval);
		retval.shrink_to_fit();

		return retval;
	}

}