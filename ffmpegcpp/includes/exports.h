/*
* dll exports and os specific utilities
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _FFMPEGCPP_EXPORTS_H_
#define _FFMPEGCPP_EXPORTS_H_



#ifdef FFMPEGCPP_DLL_EXPORT
#define FFMPEGCPP_API __declspec( dllexport )
#elif FFMPEGCPP_DLL_IMPORT
#define FFMPEGCPP_API __declspec( dllimport )
#else
#define FFMPEGCPP_API
#endif

#ifndef _WIN32

#include <string>
#include <sstream>

#define __int64 int64_t

namespace std
{
	template<typename T>
	string to_string(const T& t) {
		ostringstream os;
		os << t;
		return os.str();
	}
}


#endif


#endif