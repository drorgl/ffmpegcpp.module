/*
* data structures implementation
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"

#include <string>
#include <sstream>

#include "ffmpeg.h"

#include "../includes/datastructures.h"
#include "datastructures_conversion.h"

#include "ffmpeg_includes.h"

#include <numeric>
#include "stringutils.h"

namespace ffmpegcpp
{
	double rational::toDouble() const
	{
		AVRational rational;
		rational.num = this->num;
		rational.den = this->den;
		return av_q2d(rational);
	}

	//

	rational rational::fromDouble(double avq,int max)
	{
		auto qr = av_d2q(avq, max);
		return rational(qr.num, qr.den);
	}

	static std::vector<std::string> cleanAndSplitFlags(std::string flags)
	{
		std::string flags_sterilized = flags;
		inplace_replace(flags_sterilized, " ", ",");
		inplace_replace(flags_sterilized, ";", ",");
		inplace_replace(flags_sterilized, "|", ",");
		inplace_replace(flags_sterilized, "\r", ",");
		inplace_replace(flags_sterilized, "\n", ",");

		std::vector<std::string> retval;

		//split
		std::stringstream ss(flags_sterilized);
		std::string item;
		while (std::getline(ss, item, ',')) {
			inplace_trim(item);

			if (item != ""){
				retval.push_back(item);
			}
		}

		return retval;
	}

	static std::vector<std::tuple<option_type, std::string>> createOptionTypeMap()
	{
		std::vector<std::tuple<option_type, std::string>> retval;
		retval.push_back(std::tuple<option_type, std::string>(option_type::BINARY,"binary"));
		retval.push_back(std::tuple<option_type, std::string>(option_type::COLOR, "color"));
		retval.push_back(std::tuple<option_type, std::string>(option_type::DOUBLE, "double"));
		retval.push_back(std::tuple<option_type, std::string>(option_type::DURATION, "duration"));
		retval.push_back(std::tuple<option_type, std::string>(option_type::FLAGS, "flags"));
		retval.push_back(std::tuple<option_type, std::string>(option_type::FLOAT, "float"));
		retval.push_back(std::tuple<option_type, std::string>(option_type::IMAGE_SIZE, "image_size"));
		retval.push_back(std::tuple<option_type, std::string>(option_type::INT, "int"));
		retval.push_back(std::tuple<option_type, std::string>(option_type::INT64, "int64"));
		retval.push_back(std::tuple<option_type, std::string>(option_type::PIXEL_FMT, "pixel_fmt"));
		retval.push_back(std::tuple<option_type, std::string>(option_type::RATIONAL, "rational"));
		retval.push_back(std::tuple<option_type, std::string>(option_type::REAL_CONST, "real_const"));
		retval.push_back(std::tuple<option_type, std::string>(option_type::SAMPLE_FMT, "sample_fmt"));
		retval.push_back(std::tuple<option_type, std::string>(option_type::STRING, "string"));
		retval.push_back(std::tuple<option_type, std::string>(option_type::VIDEO_RATE, "video_rate"));
		retval.push_back(std::tuple<option_type, std::string>(option_type::VOPTIONS, "voptions"));

		return retval;
	}

	static std::vector<std::tuple<option_type, std::string>> optionTypeMap = createOptionTypeMap();

	option_type operator & (option_type x, option_type y)
	{
		return static_cast<option_type>(static_cast<unsigned>(x)& static_cast<unsigned>(y));
	}

	std::string getOptionTypeName(option_type opttype)
	{
		for (auto t : optionTypeMap)
		{
			if ((std::get<0>(t) == opttype)){
				return std::get<1>(t);
			}
		}
		return "";
	}

	option_type getOptionTypeByName(std::string opttype)
	{
		for (auto t : optionTypeMap){
			if (std::get<1>(t) == opttype){
				return std::get<0>(t);
			}
		}

		return (option_type)0;
	}

	component_flags operator | (component_flags x, component_flags y){
		return static_cast<component_flags>(static_cast<unsigned>(x)| static_cast<unsigned>(y));
	}

	component_flags operator & (component_flags x, component_flags y){
		return static_cast<component_flags>(static_cast<unsigned>(x)& static_cast<unsigned>(y));
	}

	static std::vector<std::tuple<component_flags, std::string>> createComponentFlagsMap()
	{
		std::vector<std::tuple< component_flags, std::string>> map;
		map.push_back(std::tuple<component_flags, std::string>(component_flags::ALLOW_FLUSH, "allow_flush"));
		map.push_back(std::tuple<component_flags, std::string>(component_flags::GENERIC_INDEX, "generic_index"));
		map.push_back(std::tuple<component_flags, std::string>(component_flags::GLOBALHEADER, "globalheader"));
		map.push_back(std::tuple<component_flags, std::string>(component_flags::NEEDNUMBER, "neednumber"));
		map.push_back(std::tuple<component_flags, std::string>(component_flags::NOBINSEARCH, "nobinsearch"));
		map.push_back(std::tuple<component_flags, std::string>(component_flags::NODIMENSIONS, "nodimensions"));
		map.push_back(std::tuple<component_flags, std::string>(component_flags::NOFILE, "nofile"));
		map.push_back(std::tuple<component_flags, std::string>(component_flags::NOGENSEARCH, "nogensearch"));
		map.push_back(std::tuple<component_flags, std::string>(component_flags::NOSTREAMS, "nostreams"));
		map.push_back(std::tuple<component_flags, std::string>(component_flags::NOTIMESTAMPS, "notimestamps"));
		map.push_back(std::tuple<component_flags, std::string>(component_flags::NO_BYTE_SEEK, "no_byte_seek"));
		map.push_back(std::tuple<component_flags, std::string>(component_flags::RAWPICTURE, "rawpicture"));
		map.push_back(std::tuple<component_flags, std::string>(component_flags::SEEK_TO_PTS, "seek_to_pts"));
		map.push_back(std::tuple<component_flags, std::string>(component_flags::SHOW_IDS, "show_ids"));
		map.push_back(std::tuple<component_flags, std::string>(component_flags::TS_DISCONT, "ts_discont"));
		map.push_back(std::tuple<component_flags, std::string>(component_flags::TS_NEGATIVE, "ts_negative"));
		map.push_back(std::tuple<component_flags, std::string>(component_flags::TS_NONSTRICT, "ts_nonstrict"));
		map.push_back(std::tuple<component_flags, std::string>(component_flags::VARIABLE_FPS, "variable_fps"));
		return map;
	}

	static std::vector<std::tuple<component_flags, std::string>> componentFlagsMap = createComponentFlagsMap();

	std::string getComponentFlags(component_flags comp_flags)
	{
		std::vector<std::string> cflags;
		for (auto t : componentFlagsMap)
		{
			if ((std::get<0>(t) & comp_flags) == std::get<0>(t)){
				cflags.push_back(std::get<1>(t));
			}
		}
		return join(cflags, ", ");
	}

	component_flags getComponentFlagsByName(std::string comp_flags)
	{
		auto cflags = cleanAndSplitFlags(comp_flags);

		component_flags ccflags = (component_flags)0;

		for (auto c : cflags){
			for (auto t : componentFlagsMap){
				if (std::get<1>(t) == c){
					ccflags = ccflags | std::get<0>(t);
				}
			}
		}

		return ccflags;
	}



	std::string getSampleFormatName(sample_format samplefmt)
	{
		if (samplefmt == sample_format::none)
		{
			return "";
		}

		return av_get_sample_fmt_name(conversion::convertFrom(samplefmt));
	}

	sample_format getSampleFormatByName(std::string samplefmt_name)
	{
		if (samplefmt_name == "")
		{
			return sample_format::none;
		}

		return conversion::convertFrom(av_get_sample_fmt(samplefmt_name.c_str()));
	}

	bool isSampleFormatPlanar(sample_format samplefmt)
	{
		return av_sample_fmt_is_planar(conversion::convertFrom(samplefmt));
	}

	std::string getChannelLayoutName(channel_layout channellayout)
	{
		char buf[256];
		memset(buf, 0, sizeof(buf));
		av_get_channel_layout_string(buf, sizeof(buf), -1, channellayout);
		return buf;
	}
	channel_layout getChannelLayoutByName(std::string channellayout_name)
	{
		return static_cast<channel_layout>(av_get_channel_layout(channellayout_name.c_str()));
	}

	int getChannelLayoutNumberOfChannels(channel_layout channellayout){
		return av_get_channel_layout_nb_channels(channellayout);
	}

	std::string getPixelFormatName(pixel_format pixfmt)
	{
		auto fmtname = av_get_pix_fmt_name(conversion::convertFrom(pixfmt));
		if (fmtname == NULL){
			return "";
		}
		return fmtname;
	}

	pixel_format getPixelFormatByName(std::string pixfmt_name)
	{
		return conversion::convertFrom(av_get_pix_fmt(pixfmt_name.c_str()));
	}

	static std::map<mediaType, std::string> createMediaTypesToStringMap()
	{
		std::map<mediaType, std::string> map;
		map[mediaType::attachment] = "attachment";
		map[mediaType::audio] =  "audio";
		map[mediaType::data] = "data";
		map[mediaType::nb] =  "nb";
		map[mediaType::subtitle] =  "subtitle";
		map[mediaType::unknown] = "unknown";
		map[mediaType::video] =  "video";
		return map;
	}

	static std::map<mediaType, std::string> mediaTypesToStringMap = createMediaTypesToStringMap();

	std::string getMediaTypeName(mediaType media)
	{
		if (!mediaTypesToStringMap.count(media)) {
			throw ffmpeg_exception("media type not implemented");
		}

		return mediaTypesToStringMap[media];
	}

	static std::map<std::string, mediaType> createMediaTypesFromStringMap()
	{
		std::map<std::string, mediaType> map;
		map["attachment"] = mediaType::attachment;
		map["data"] = mediaType::data;
		map["nb"] = mediaType::nb;
		map["subtitle"] = mediaType::subtitle;
		map["unknown"] = mediaType::unknown;
		map["audio"] = mediaType::audio;
		map["video"] = mediaType::video;
		return map;
	}

	static std::map<std::string, mediaType> mediaTypesFromStringMap = createMediaTypesFromStringMap();

	mediaType getMediaTypeByName(std::string media)
	{
		if (!mediaTypesFromStringMap.count(media)) {
			throw ffmpeg_exception("media type not found");
		}

		return mediaTypesFromStringMap[media];
	}

	codec_capabilities operator | (codec_capabilities x, codec_capabilities y)
	{
		return static_cast<codec_capabilities>(static_cast<unsigned>(x)| static_cast<unsigned>(y));
	}

	codec_capabilities operator & (codec_capabilities x, codec_capabilities y)
	{
		return static_cast<codec_capabilities>(static_cast<unsigned>(x)& static_cast<unsigned>(y));
	}


	static std::vector<std::tuple<codec_capabilities, std::string>> createCodecCapMap()
	{
		std::vector<std::tuple<codec_capabilities, std::string>> retval;
		retval.push_back(std::tuple<codec_capabilities, std::string>(codec_capabilities::AUTO_THREADS, "auto_threads"));
		retval.push_back(std::tuple<codec_capabilities, std::string>(codec_capabilities::CHANNEL_CONF, "channel_conf"));
		retval.push_back(std::tuple<codec_capabilities, std::string>(codec_capabilities::DELAY, "delay"));
		retval.push_back(std::tuple<codec_capabilities, std::string>(codec_capabilities::DR1, "dr1"));
		retval.push_back(std::tuple<codec_capabilities, std::string>(codec_capabilities::DRAW_HORIZ_BAND, "draw_horiz_band"));
		retval.push_back(std::tuple<codec_capabilities, std::string>(codec_capabilities::EXPERIMENTAL, "experimental"));
		retval.push_back(std::tuple<codec_capabilities, std::string>(codec_capabilities::FRAME_THREADS, "frame_threads"));
		retval.push_back(std::tuple<codec_capabilities, std::string>(codec_capabilities::HWACCEL, "hwaccel"));
		retval.push_back(std::tuple<codec_capabilities, std::string>(codec_capabilities::HWACCEL_VDPAU, "hwaccel_vdpau"));
		retval.push_back(std::tuple<codec_capabilities, std::string>(codec_capabilities::INTRA_ONLY, "intra_only"));
		retval.push_back(std::tuple<codec_capabilities, std::string>(codec_capabilities::LOSSLESS, "lossless"));
		retval.push_back(std::tuple<codec_capabilities, std::string>(codec_capabilities::NEG_LINESIZES, "neg_linesizes"));
		retval.push_back(std::tuple<codec_capabilities, std::string>(codec_capabilities::PARAM_CHANGE, "param_change"));
		retval.push_back(std::tuple<codec_capabilities, std::string>(codec_capabilities::SLICE_THREADS, "slice_threads"));
		retval.push_back(std::tuple<codec_capabilities, std::string>(codec_capabilities::SMALL_LAST_FRAME, "small_last_frame"));
		retval.push_back(std::tuple<codec_capabilities, std::string>(codec_capabilities::SUBFRAMES, "subframes"));
		retval.push_back(std::tuple<codec_capabilities, std::string>(codec_capabilities::TRUNCATED, "truncated"));
		retval.push_back(std::tuple<codec_capabilities, std::string>(codec_capabilities::VARIABLE_FRAME_SIZE, "variable_frame_size"));

		return retval;
	}

	static std::vector<std::tuple<codec_capabilities, std::string>> codecCapMap = createCodecCapMap();

	std::string getCodecCapabilities(codec_capabilities codec_cap)
	{
		std::vector<std::string> caps;

		for (auto t : codecCapMap)
		{
			if ((std::get<0>(t) & codec_cap) == std::get<0>(t)){
				caps.push_back(std::get<1>(t));
			}
		}

		return join(caps, ", ");
	}

	std::vector<std::string> getCodecCapabilitiesVector(codec_capabilities codec_cap)
	{
		std::vector<std::string> caps;

		for (auto t : codecCapMap)
		{
			if ((std::get<0>(t) & codec_cap) == std::get<0>(t)){
				caps.push_back(std::get<1>(t));
			}
		}

		return caps;
	}

	codec_capabilities getCodecCapabilitiesByName(std::string codec_cap)
	{
		auto caps = cleanAndSplitFlags(codec_cap);

		codec_capabilities ccap = (codec_capabilities)0;

		for (auto c : caps){
			for (auto t : codecCapMap){
				if (std::get<1>(t) == c){
					ccap = ccap | std::get<0>(t);
				}
			}
		}

		return ccap;
	}
}