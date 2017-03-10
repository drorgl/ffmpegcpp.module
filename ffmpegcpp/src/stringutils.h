/*
* string utility functions
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _FFMPEGCPP_STRING_UTILS_
#define _FFMPEGCPP_STRING_UTILS_

#include <string>
#include <algorithm>
#include <cstdarg>
#include <cstring>
#include <numeric>
#include <cctype>

#include <iostream>

inline void inplace_replace(std::string &str, const std::string &find, const std::string &repl)
{
	//std::replace(str.begin(), str.end(), find, repl);
	size_t pos = 0, fpos;
	while ((fpos = str.find(find, pos)) != std::string::npos)
	{
		str.replace(fpos, find.size(), repl);
		pos = fpos + repl.size();
	}
}

inline std::string replace(std::string str, const std::string &find, const std::string &repl)
{
	std::string retval = str;
	inplace_replace(retval, find, repl);
	//std::replace(retval.begin(), retval.end(), find, repl);
	return retval;
}



inline void inplace_ltrim(std::string &str)
{
	str.erase(std::find_if(str.rbegin(), str.rend(), std::bind1st(std::not_equal_to<char>(), ' ')).base(), str.end());
}


inline std::string ltrim(std::string str)
{
	std::string retval = str;
	inplace_ltrim(retval);
	return retval;
}

inline void inplace_fulltrim(std::string &str)
{
	str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](char const &ch){return std::isspace(ch); }));
	str.erase(std::find_if(str.rbegin(), str.rend(), [](char const &ch){return std::isspace(ch); }).base(), str.end());
}

inline void inplace_rtrim(std::string &str)
{
	str.erase(str.begin(), std::find_if(str.begin(), str.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
} 

inline std::string rtrim(std::string str)
{
	std::string retval = str;
	inplace_rtrim(retval);
	return retval;
}



inline void inplace_trim(std::string &str)
{
	inplace_rtrim(str);
	inplace_ltrim(str);
}

inline std::string trim(std::string str)
{
	std::string retval = str;
	inplace_trim(retval);
	return retval;
}

inline std::string join(std::vector <std::string> &vec, std::string delim)
{
	return std::accumulate(vec.begin(), vec.end(), std::string(),
		[delim](const std::string& a, const std::string& b) -> std::string {
		return a + (a.length() > 0 ? delim : "") + b;
	});
}

inline std::string format(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	
	char buf[1024];
	memset(&buf,'\0', sizeof(char) * 1024);

	auto len = vsprintf(buf,fmt, args);

	//check for error and throw
	if (len < 0){
		std::string error = "unable to format string ";
		error.append(fmt);
		throw std::runtime_error(error);
	}
	
	if (len > (sizeof(buf) / sizeof(*buf)))
	{
		assert(true && "formatted string is longer than buffer");
	}

	va_end(args);
	
	return std::string(buf);
}


#endif