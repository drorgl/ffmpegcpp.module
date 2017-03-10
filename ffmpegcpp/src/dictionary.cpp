/*
* dictionary implementation
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"

#include "dictionary.h"

#include "dictionary_pimpl.h"

namespace ffmpegcpp
{
	dictionary::dictionary()
		: _pimpl(std::make_shared<pimpl_dictionary>())
	{}

	dictionary::dictionary(const std::shared_ptr<std::map<std::string, std::string>> &fromMap)
		: _pimpl(std::make_shared<pimpl_dictionary>())
	{
		if (fromMap != nullptr){
			parseMap(*fromMap);
		}
	}

	dictionary::dictionary(const std::map<std::string, std::string> &fromMap)
		: _pimpl(std::make_shared<pimpl_dictionary>())
	{
		parseMap(fromMap);
	}

	void dictionary::clear()
	{
		_pimpl->_dictionaryMap.clear();
	}

	int dictionary::count()
	{
		return _pimpl->_dictionaryMap.size();
	}
	void dictionary::set(std::string key, std::string value)
	{
		_pimpl->_dictionaryMap[key] = value;
	}

	std::string dictionary::get(std::string key)
	{
		auto value = _pimpl->_dictionaryMap.find(key);
		if (value == _pimpl->_dictionaryMap.end())
			return "";
		
		return value->second;
	}

	std::vector<option> dictionary::get_options(void * obj)
	{
		if (obj == NULL)
		{
			return std::vector<option>();
		}
		std::vector<option> retval;
		const AVOption * avopt = NULL;

		

		while ((avopt = av_opt_next(obj, avopt)) != NULL)
		{
			option o;

			auto dst = (uint8_t*)obj + avopt->offset;

			if (avopt->name != NULL){
				o.name = avopt->name;
			}

			if (avopt->help != NULL){
				o.help = avopt->help;
			}

			if (avopt->unit != NULL){
				o.unit = avopt->unit;
			}

			o.flags = avopt->flags;
			o.max = avopt->max;
			o.min = avopt->min;
			o.type = dictionary::pimpl_dictionary::convertFrom(avopt->type);

			//http://ffmpeg.org/doxygen/trunk/opt_8c_source.html
			switch (o.type){
			case option_type::FLAGS:
			case option_type::INT:
			case option_type::INT64:
			case option_type::DURATION:
			case option_type::PIXEL_FMT://maybe add pixel format to variant?
			case option_type::SAMPLE_FMT://maybe add sample format to variant?
				o.default_val = avopt->default_val.i64;
				break;
			case option_type::DOUBLE:
			case option_type::FLOAT:
			case option_type::REAL_CONST:
				o.default_val = avopt->default_val.dbl;
				break;
			case option_type::COLOR:
			case option_type::IMAGE_SIZE:
			case option_type::VIDEO_RATE:
			case option_type::STRING:
				if (avopt->default_val.str != NULL){
					o.default_val = avopt->default_val.str;
				}
				else{
					o.default_val = "";
				}
				break;
			case option_type::RATIONAL:
				o.default_val = rational(avopt->default_val.q.num, avopt->default_val.q.den);
				break;
			//TODO: implement dict
			//TODO: implement channel_layout
			default:
				assert(false && "Not Implemented!!");
			}

			retval.push_back(o);
		}
		return retval;
	}

	
}