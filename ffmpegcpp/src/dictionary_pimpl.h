/*
* dictionary shared implementation
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _FFMPEGCPP_DICTIONARY_PIMPL_H_
#define _FFMPEGCPP_DICTIONARY_PIMPL_H_

#include "stdafx.h"

#include "dictionary.h"

extern "C" {
	#include <libavutil/opt.h>
}

#include <unordered_map>

namespace ffmpegcpp
{
	struct dictionary::pimpl_dictionary
	{
		std::unordered_map<std::string, std::string> _dictionaryMap;

		void fromAVDictionary(AVDictionary ** dict, bool clearFirst, bool freeAVDictionary){
			if (clearFirst){
				_dictionaryMap.clear();
			}

			AVDictionaryEntry *t = NULL;

			while ((t = av_dict_get(*dict, "", t, AV_DICT_IGNORE_SUFFIX))){
				_dictionaryMap[t->key] = t->value;
			}

			if (freeAVDictionary && (*dict != NULL)){
				av_dict_free(dict);
			}
		}

		AVDictionary * toAVDictionary(){
			if (_dictionaryMap.size() == 0){
				return NULL;
			}

			AVDictionary *dict = NULL;

			for (auto kv : _dictionaryMap) {
				av_dict_set(&dict, kv.first.c_str(), kv.second.c_str(), 0);
			}

			return dict;
		}

		static option_type convertFrom(AVOptionType opttype){
			switch (opttype){
			case AVOptionType::AV_OPT_TYPE_BINARY:
				return option_type::BINARY;
			case AVOptionType::AV_OPT_TYPE_COLOR:
				return option_type::COLOR;
			case AVOptionType::AV_OPT_TYPE_CONST:
				return option_type::REAL_CONST;
			case AVOptionType::AV_OPT_TYPE_DOUBLE:
				return option_type::DOUBLE;
			case AVOptionType::AV_OPT_TYPE_DURATION:
				return option_type::DURATION; 
			case AVOptionType::AV_OPT_TYPE_FLAGS:
				return option_type::FLAGS;
			case AVOptionType::AV_OPT_TYPE_FLOAT:
				return option_type::FLOAT;
			case AVOptionType::AV_OPT_TYPE_IMAGE_SIZE:
				return option_type::IMAGE_SIZE;
			case AVOptionType::AV_OPT_TYPE_INT:
				return option_type::INT;
			case AVOptionType::AV_OPT_TYPE_INT64:
				return option_type::INT64;
			case AVOptionType::AV_OPT_TYPE_PIXEL_FMT:
				return option_type::PIXEL_FMT;
			case AVOptionType::AV_OPT_TYPE_RATIONAL:
				return option_type::RATIONAL;
			case AVOptionType::AV_OPT_TYPE_SAMPLE_FMT:
				return option_type::SAMPLE_FMT;
			case AVOptionType::AV_OPT_TYPE_STRING:
				return option_type::STRING;
			case AVOptionType::AV_OPT_TYPE_VIDEO_RATE:
				return option_type::VIDEO_RATE;
			case AVOptionType::AV_OPT_TYPE_DICT:
				return option_type::DICT;
			case AVOptionType::AV_OPT_TYPE_CHANNEL_LAYOUT:
				return option_type::CHANNEL_LAYOUT;
			}
			assert(false && "AVOptionType enum value not implemented, returning string as default");
			return option_type::STRING;
		}

	};
}

#endif