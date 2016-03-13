/*
* ffmpeg wrapper implementation
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
#include "formatcontext_pimpl.h"
#include "memorycontext_pimpl.h"

#include "stringutils.h"

#ifdef __linux__
#include "linux/alsa/alsawrapper.h"
#include "linux/v4l2/v4l2_wrapper.h"
#endif

#pragma warning(disable: 4996)

namespace ffmpegcpp
{
	std::atomic<int> ffmpeg::_instanceCount;
	std::mutex ffmpeg::_instanceLock;
	
	std::string ffmpeg::_ffmpeg_log;

	std::function<void(std::string, int, std::string)> ffmpeg::_logger;

	int ffmpeg::instance_count()
	{
		return _instanceCount;
	}

	void ffmpeg::_logcallback(void * avcl, int level, const char * fmt, va_list vl_arg)
	{
		std::string moduleName;

		if (avcl)
		{
			AVClass *avc = *(AVClass**)avcl;
			auto module = avc->item_name(avcl);
			//_ffmpeg_log.append(module);
			moduleName.append(module);
		}

		//workaround for ffmpeg ZU bug (solvable by changing the HAVE_LIBC_MSVCRT in the config.h)
		std::string fmtstr(fmt);
#ifndef __linux__
		inplace_replace(fmtstr, "%zu", "%Iu");
#endif


		char buffer[1024];
		//std::string buffer;
		//buffer.resize(1024*4);

		vsnprintf(buffer, sizeof(buffer) / sizeof(buffer[0]), fmtstr.c_str(), vl_arg);

		/*va_list vl;
		va_copy(vl, vl_arg);
		int len = vsnprintf((char*)buffer.data(), buffer.size(), fmtstr.c_str(), vl);
		va_end(vl);

		if (len > -1 && len < buffer.size()){
			buffer.resize(len);
			buffer.shrink_to_fit();
		}*/

		//vsprintf()
		//int len = vsnprintf(buffer, sizeof(buffer) / sizeof(buffer[0]), fmt, vl);
		//vsprintf_s(buffer, sizeof(buffer) / sizeof(buffer[0]), fmt, vl);
		//_ffmpeg_log.append(buffer);
		

		

		if (_logger){
			(_logger)(moduleName, level, buffer);
			//(_logger)(moduleName, level, "bla");
		}
	}

	//https://stackoverflow.com/questions/13888915/thread-safety-of-libav-ffmpeg
	static int lockmgr_cb(void **mutex, enum AVLockOp op)
	{
		if (NULL == mutex)
			return -1;

		switch (op)
		{
		case AV_LOCK_CREATE:
		{
							   *mutex = NULL;
							   std::mutex * m = new std::mutex();
							   assert(m != NULL);
							   *mutex = static_cast<void*>(m);
							   break;
		}
		case AV_LOCK_OBTAIN:
		{
							   std::mutex * m = static_cast<std::mutex*>(*mutex);
							   assert(m != NULL);
							   m->lock();
							   break;
		}
		case AV_LOCK_RELEASE:
		{
								std::mutex * m = static_cast<std::mutex*>(*mutex);
								assert(m != NULL);
								m->unlock();
								break;
		}
		case AV_LOCK_DESTROY:
		{
								std::mutex * m = static_cast<std::mutex*>(*mutex);
								assert(m != NULL);
								delete m;
								break;
		}
		default:
			break;
		}
		return 0;
	}

	struct ffmpeg::ffmpeg_pimpl
	{
		//std::shared_ptr<formatcontext> _context;
	};

	ffmpeg::ffmpeg()
		: _pimpl(std::make_shared<ffmpeg_pimpl>())
	{
		std::lock_guard<std::mutex> _lock(_instanceLock);

		if (_instanceCount == 0)
		{
			av_register_all();
			avdevice_register_all();
			avcodec_register_all();
			int networkRetval = avformat_network_init();
			if (networkRetval < 0)
			{
				throw ffmpeg_exception(networkRetval, "unable to initialize network components");
			}
			avfilter_register_all();

			av_log_set_callback(&_logcallback);
			av_log_set_level(AV_LOG_INFO);
			av_log_set_flags(0xFF);

			int lockManagerRetval = av_lockmgr_register(&lockmgr_cb);
			if (lockManagerRetval < 0){
				throw ffmpeg_exception(lockManagerRetval, "unable to register lock manager");
			}
		}

		_instanceCount++;
	}

	ffmpeg::~ffmpeg()
	{
		std::lock_guard<std::mutex> _lock(_instanceLock);

		_instanceCount--;

		if (_instanceCount == 0){
			avformat_network_deinit();
			av_log_set_callback(NULL);
		}
	}

	std::shared_ptr<formatcontext> ffmpeg::open_input(std::string filename, std::string format, std::shared_ptr<dictionary> options,std::shared_ptr<dictionary> metadata)
	{
		/*if ((format != "") && (format != "file"))
		{
			_pimpl->_inputFormat = av_find_input_format(format.c_str());
			if (_pimpl->_inputFormat == NULL){
				throw ffmpeg_exception("invalid input format " + format);
			}
		}*/
		
		auto inputContext = std::make_shared<formatcontext>();

		inputContext->_pimpl->open_input(filename, format, options, metadata);

		inputContext->_pimpl->find_stream_info();

		inputContext->_pimpl->dump_format();

		return inputContext;
	}

	std::shared_ptr<formatcontext> ffmpeg::open_output(std::string filename, std::string format)
	{
		auto outputCtx = std::make_shared<formatcontext>();
		outputCtx->_pimpl->open_output(filename, format);

		return outputCtx;
	}

	std::shared_ptr<formatcontext> ffmpeg::open_output(std::shared_ptr<memorycontext> memoryctx, std::string filename, std::string format)
	{
		auto outputCtx = std::make_shared<formatcontext>();
		outputCtx->_pimpl->open_output(memoryctx,filename, format);

		return outputCtx;

		//auto outputCtx = std::make_shared<formatcontext>();
		//outputCtx->_pimpl->open_output("", format);

		//if (outputCtx->_pimpl->_outputFormat->flags & AVFMT_NOFILE){
		//	throw ffmpeg_exception("output format with AVFMT_NOFILE should not be used with memorycontext");
		//}

		////test, do not commit!!
		///*outputCtx->_pimpl->_pFormatCtx->flags |= AVFMT_GLOBALHEADER;
		//outputCtx->_pimpl->_outputFormat->flags |= AVFMT_GLOBALHEADER;*/

		//outputCtx->_pimpl->_pFormatCtx->flags |= AVFMT_FLAG_CUSTOM_IO;

		////only for reference counting, so the memory context won't be freed as long as its being held by pb
		//outputCtx->_memoryContext = memoryctx;
		//outputCtx->_pimpl->_pFormatCtx->pb = memoryctx->_pimpl->_iobuffer->_io_context;
		////_pimpl->_pFormatCtx->pb

		//return outputCtx;
	}


	void ffmpeg::setLogger(std::function<void(std::string, int, std::string)> logger_)
	{
		_logger = logger_;
	}


	std::vector<std::string> ffmpeg::ExecuteCommand(std::string filename, std::string format, std::shared_ptr<dictionary> dict)
	{
		//save logger
		auto logger = _logger;
		std::vector<std::string> rrawlist;

		std::string line;

		try{
			//set temporary logger
			_logger = [&logger, &rrawlist, &format, &line](std::string moduleName, int level, std::string buffer){
				if (logger){
					(logger)(moduleName, level, buffer);
				}

				if (moduleName == format){

					if ((buffer.find("\r") != std::string::npos) || (buffer.find("\n") != std::string::npos)){
						rrawlist.push_back(line + buffer);
						line = "";
					}
					else{
						line.append(buffer);
					}
				}

		};

			ffmpeg ffm;

			auto ctx = ffm.open_input(filename, format, dict);
			ctx->close();
	}
		catch (std::runtime_error ex){
			std::cout << ex.what() << std::endl;
		}

		_logger = logger;

		return rrawlist;

	}

	void ffmpeg::ParseInputDevice(std::shared_ptr<device> d)
	{
#ifdef __linux__
		std::string deviceFormat = "v4l2";

#elif _WIN32
		std::string deviceFormat = "dshow";
#else
		throw ffmpegcpp::ffmpeg_exception("not implemented");
#endif

		std::string devicename;
		{
			switch (d->type)
			{
			case devicetype::audio:
				devicename = "audio";
				break;
			case devicetype::video:
				devicename = "video";
				break;
			}

			devicename.append("=");
			devicename.append(d->name);
		}

		std::shared_ptr<ffmpegcpp::dictionary> dict = std::make_shared<ffmpegcpp::dictionary>();
		dict->set("list_options", "true");
		auto rawlist = ExecuteCommand(devicename, deviceFormat, dict);
		
		std::string pin;

		for (std::string item : rawlist){
			item.erase(std::remove_if(item.begin(), item.end(), [](char c){return (c == '\r' || c == '\n'); }), item.end());
			item.erase(item.begin(), std::find_if(item.begin(), item.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
			item.erase(std::find_if(item.rbegin(), item.rend(), std::bind1st(std::not_equal_to<char>(), ' ')).base(), item.end());

			//attempt to populate pin
			char lpin[100];
			if (sscanf(item.c_str(), "Pin \"%s\"", lpin))
			{
				
				pin = lpin;
				
				pin.erase(std::remove_if(pin.begin(), pin.end(), [](char c){return (c == '"'); }), pin.end());
			}

			int minchannels;
			int minbits;
			int minrate;
			int maxchannels;
			int maxbits;
			int maxrate;

			if (sscanf(item.c_str(), "min ch=%d bits=%d rate= %d max ch=%d bits=%d rate= %d", &minchannels, &minbits, &minrate, &maxchannels, &maxbits, &maxrate))
			{
				std::shared_ptr<deviceinfoaudio> dia = std::make_shared<deviceinfoaudio>();
				dia->pin = pin;
				dia->minChannels = minchannels;
				dia->minBits = minbits;
				dia->minRate = minrate;
				dia->maxChannels = maxchannels;
				dia->maxBits = maxbits;
				dia->maxRate = maxrate;

				d->deviceInfos.push_back(dia);

				continue;
			}

			char pixelformat[20];
			int minwidth = 0;
			int minheight = 0;
			double minfps = 0;
			int maxwidth = 0;
			int maxheight = 0;
			double maxfps = 0;
			if (sscanf(item.c_str(), "pixel_format=%s  min s=%dx%d fps=%lf max s=%dx%d fps=%lf", pixelformat, &minwidth,&minheight, &minfps, &maxwidth,&maxheight, &maxfps))
			{
				std::shared_ptr<deviceinfovideo> div = std::make_shared<deviceinfovideo>();
				div->pin = pin;
				div->pixelFormat = (std::string(pixelformat) != "") ? getPixelFormatByName(pixelformat) : pixel_format::PIX_FMT_NONE;
				div->minWidth = minwidth;
				div->minHeight = minheight;
				div->minFPS = minfps;
				div->maxWidth = maxwidth;
				div->maxHeight = maxheight;
				div->maxFPS = maxfps;
				d->deviceInfos.push_back(div);
			}

			//attempt to read compressed video line
			char scodec[20];
			if (sscanf(item.c_str(), "vcodec=%s  min s=%dx%d fps=%lf max s=%dx%d fps=%lf", scodec, &minwidth, &minheight, &minfps, &maxwidth, &maxheight, &maxfps))
			{
				std::shared_ptr<deviceinfovideo> div = std::make_shared<deviceinfovideo>();
				div->pin = pin;
				div->codec = scodec;
				div->pixelFormat = pixel_format::PIX_FMT_NONE;
				div->minWidth = minwidth;
				div->minHeight = minheight;
				div->minFPS = minfps;
				div->maxWidth = maxwidth;
				div->maxHeight = maxheight;
				div->maxFPS = maxfps;
				d->deviceInfos.push_back(div);
			}

		}

	}


	std::vector<std::shared_ptr<device>> ffmpeg::ListInputDevices()
	{
		std::vector<std::shared_ptr<device>> retlist;

#ifdef __linux__
		//std::string deviceFormat = "v4l2";

		//std::string deviceFormat = "v4l2";

		auto v4l2devices = v4l2_wrapper::listDevices();
		auto alsadevices = alsa_wrapper::listDevices();

		retlist.insert(retlist.end(),v4l2devices.begin(),v4l2devices.end());
		retlist.insert(retlist.end(),alsadevices.begin(),alsadevices.end());



#elif _WIN32
		std::string deviceFormat = "dshow";

		std::shared_ptr<ffmpegcpp::dictionary> dict = std::make_shared<ffmpegcpp::dictionary>();
		dict->set("list_devices", "true");
		auto rawlist = ExecuteCommand("dummy", deviceFormat, dict);

		auto type = devicetype::unknown;


		//parse rawlist
		for (std::string item : rawlist){
			item.erase(std::remove_if(item.begin(), item.end(), [](char c){return (c == '\r' || c == '\n'); }), item.end());
			item.erase(item.begin(), std::find_if(item.begin(), item.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
			item.erase(std::find_if(item.rbegin(), item.rend(), std::bind1st(std::not_equal_to<char>(), ' ')).base(), item.end());

			auto videostr = "DirectShow video devices";
			auto audiostr = "DirectShow audio devices";

			if (item.find(videostr) != std::string::npos)
				type = devicetype::video;
			else if (item.find(audiostr) != std::string::npos)
				type = devicetype::audio;

			//remove strings without quotes
			//remove quotes and return as vector of devices
			if ((item.size() > 0) && (item.at(0) == '\"'))
			{
				item.erase(std::remove_if(item.begin(), item.end(), [](char c){return (c == '\"'); }), item.end());
				std::shared_ptr<device> d = std::make_shared<device>();
				d->name = item;
				d->full_name = item;
				d->type = type;
				d->format = deviceFormat;
				retlist.push_back(d);
			}
		}

		for (auto dev : retlist)
		{
			ParseInputDevice(dev);
		}

#else
		throw ffmpegcpp::ffmpeg_exception("not implemented");
#endif

		

		return retlist;
		
	}



	
}