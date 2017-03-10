/*
* format context shared implementation
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _FFMPEGCPP_CONTEXT_PIMPL_H_
#define _FFMPEGCPP_CONTEXT_PIMPL_H_

#include "ffmpeg.h"
#include <algorithm>
#include "dictionary_pimpl.h"
#include "stream_pimpl.h"

namespace ffmpegcpp
{
	struct formatcontext::formatcontext_pimpl
	{
		AVInputFormat *_inputFormat = NULL;
		AVOutputFormat *_outputFormat = NULL;
		bool _outputWroteHeader = false; //not threadsafe! 
		std::string _filename;
		std::shared_ptr< memorycontext> _memoryctx = nullptr;

		AVFormatContext   * _pFormatCtx = NULL;
		std::shared_ptr<std::vector<std::shared_ptr<stream>>> _vector = nullptr;

		formatcontext_pimpl()
		{
			_pFormatCtx = avformat_alloc_context();
		}

		~formatcontext_pimpl()
		{
			if (_inputFormat != NULL){
				avformat_close_input(&_pFormatCtx);
				_inputFormat = NULL;
			}

			avformat_free_context(_pFormatCtx);
		}

		void open_input(std::string filename, std::string _format, std::shared_ptr<dictionary> options, std::shared_ptr<dictionary> metadata)
		{
			_filename = filename;
			if ((_format != "") && (_format != "file"))
			{
				_inputFormat = av_find_input_format(_format.c_str());
				if (_inputFormat == NULL){
					throw ffmpeg_exception("invalid input format " + _format);
				}
			}

			AVDictionary * dict = (options != nullptr) ? options->_pimpl->toAVDictionary() : NULL;

			int result = avformat_open_input(&_pFormatCtx, _filename.c_str(), _inputFormat, &dict);

			if (options != nullptr){
				options->_pimpl->fromAVDictionary(&dict, true, true);
			}

			//handle metadata if a dictionary was provided and the file has it
			if (metadata != nullptr){
				metadata->_pimpl->fromAVDictionary(&_pFormatCtx->metadata, false, false);
			}

			if (result != 0)
			{
				//printdebug("camera::avformat_open_input error %d", result);
				throw ffmpeg_exception(result, "avformat_open_input error");
			}
		}

		void open_output(std::shared_ptr< memorycontext> memctx,std::string _filename, std::string _format)
		{
			open_output(_filename, _format);
			_memoryctx = memctx;
		}

		void open_output(std::string filename, std::string _format)
		{
			_filename = filename;

			auto fmt = (_format != "") ? _format.c_str() : NULL;
			auto fname = (filename != "") ? filename.c_str() : NULL;

			_outputFormat = av_guess_format(fmt, fname, NULL);

			if (_outputFormat == NULL){
				throw ffmpeg_exception("unable to guess output format file '" + filename + "' or format " + _format);
			}

			_pFormatCtx->oformat = _outputFormat;
		}
		
		//attempt to retrieve mpeg4 header from first packet
		void extract_mpeg4_header(AVFormatContext *infile){
			int mpeg4_count, i, size;
			AVPacket pkt;
			AVStream *st;
			const uint8_t *p;

			infile->flags |= AVFMT_FLAG_NOFILLIN | AVFMT_FLAG_NOPARSE;

			mpeg4_count = 0;
			for (i = 0; i < (int)infile->nb_streams; i++) {
				st = infile->streams[i];
				if (st->codec->codec_id == AV_CODEC_ID_MPEG4 &&
					st->codec->extradata_size == 0) {
					mpeg4_count++;
				}
			}
			if (!mpeg4_count)
				return;

			av_log(infile, AV_LOG_INFO, "MPEG4 without extra data: trying to find header in %s\n", infile->filename);

			while (mpeg4_count > 0) {
				if (av_read_frame(infile, &pkt) < 0)
					break;
				st = infile->streams[pkt.stream_index];
				if (st->codec->codec_id == AV_CODEC_ID_MPEG4 &&
					st->codec->extradata_size == 0) {
					av_freep(&st->codec->extradata);
					/* fill extradata with the header */
					/* XXX: we make hard suppositions here ! */
					p = pkt.data;
					while (p < pkt.data + pkt.size - 4) {
						/* stop when vop header is found */
						if (p[0] == 0x00 && p[1] == 0x00 &&
							p[2] == 0x01 && p[3] == 0xb6) {
							size = (int)( p - pkt.data);
							av_hex_dump_log(infile, AV_LOG_DEBUG, pkt.data, size);
							st->codec->extradata = (uint8_t*)av_malloc(size);
							st->codec->extradata_size = size;
							memcpy(st->codec->extradata, pkt.data, size);
							break;
						}
						p++;
					}
					mpeg4_count--;
				}
				av_free_packet(&pkt);
			}
		}

		void find_stream_info()
		{
			int result = avformat_find_stream_info(_pFormatCtx, NULL);
			if (result != 0)
			{
				throw ffmpeg_exception(result, "avformat_find_stream_info error");
			}

			
			extract_mpeg4_header(_pFormatCtx);
		}

		void dump_format()
		{
			if (_pFormatCtx->iformat != NULL){
				av_dump_format(_pFormatCtx, 0, _filename.c_str(), false);
			}
			if (_pFormatCtx->oformat != NULL){
				av_dump_format(_pFormatCtx, 0, _filename.c_str(),true);
			}
		}

		const std::shared_ptr<std::vector<std::shared_ptr<stream>>> &getStreams(std::shared_ptr<formatcontext> context_)
		{
			//add detection if input context
			if (_vector == nullptr){
				_vector = stream::getStreams(context_);
			}

			return _vector;
		}

		std::shared_ptr<stream> addStream(std::shared_ptr<formatcontext> context_, std::string codecName, rational stream_time_base)
		{
			//add detection if output context
			if (_vector == nullptr){
				_vector = std::make_shared<std::vector<std::shared_ptr<stream>>>();
			}

			auto stream = stream::addStream(context_, codecName,stream_time_base);
			_vector->push_back(stream);
			return stream;
		}

		std::shared_ptr<stream> getStream(int streamid)
		{
			if (_vector == nullptr)
			{
				return nullptr;
			}

			//TODO: potential performance problem!
			auto found  = std::find_if(_vector->begin(), _vector->end(), [streamid](std::shared_ptr<stream> const strm){
				return (strm->index() == streamid);
			});

			if (found == _vector->end()){
				return nullptr;
			}

			return *found;
		}

	};
}


#endif