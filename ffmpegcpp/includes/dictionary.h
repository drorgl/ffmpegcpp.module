/*
* dictionary
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _FFMPEGCPP_DICTIONARY_H_
#define _FFMPEGCPP_DICTIONARY_H_


#include "exports.h"
#include "ffmpeg_exception.h"
#include "datastructures.h"

#include <vector>
#include <memory>
#include <string>
#include <map>

namespace ffmpegcpp
{

	class dictionary
	{
	private:
		struct pimpl_dictionary;
		std::shared_ptr<pimpl_dictionary> _pimpl;

		friend class formatcontext;
		friend class decoder;
		friend class codec;
		
		void parseMap(std::map<std::string, std::string> fromMap)
		{
			for (auto kv : fromMap)
			{
				set(kv.first, kv.second);
			}
		}

		static std::vector<option> get_options(void * obj);

	public:
		dictionary(const dictionary& that) = delete;
		dictionary& operator=(const dictionary&) = delete;
		FFMPEGCPP_API dictionary();
		FFMPEGCPP_API dictionary(const std::shared_ptr<std::map<std::string, std::string>> &fromMap);
		FFMPEGCPP_API dictionary(const std::map<std::string, std::string> &fromMap);

		FFMPEGCPP_API void clear();

		FFMPEGCPP_API int count();
		FFMPEGCPP_API void set(std::string key, std::string value);
		FFMPEGCPP_API std::string get(std::string key);

	};
}

#endif