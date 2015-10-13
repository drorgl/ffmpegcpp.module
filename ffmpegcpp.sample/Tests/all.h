/*
* main catch test 
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _TEST_ALL_
#define _TEST_ALL_

#define CATCH_CONFIG_RUNNER


#include "catch.hpp"
#include <chrono>
#include <cstdio>


#define REQUIRE_VECTOR_HAS(vec, val)    \
{										 \
	bool has = false;					 \
	for (auto item : (vec))				 \
	{									 \
		if ((item) == (val)){				 \
			has = true;					 \
			break;						 \
		}								 \
	}									 \
	REQUIRE(has);						 \
}


#ifdef _WIN32
#include <direct.h>
#else
#endif

#define MAXPATHLEN 2048

std::string get_working_path()
{
	char temp[MAXPATHLEN];
#ifdef _WIN32
	_getcwd(temp,MAXPATHLEN);
#else
	getcwd(temp, MAXPATHLEN);
#endif
	return std::string(temp);
}


inline bool file_exists(const std::string& name) {
	std::ifstream f(name.c_str());
	if (f.good()) {
		f.close();
		return true;
	}
	else {
		f.close();
		return false;
	}
}

template <typename T>
static std::string tostring(T val)
{
	std::stringstream ss;
	ss.unsetf(std::ios::fixed | std::ios::scientific);
	ss << std::setprecision(2) << val;
	return ss.str();
}

#define SAMPLE_MAXIMUM_PACKETS 500
#define SAMPLE_INPUT_FILENAME "../testdata/big buck bunny.mp4"
#define SAMPLE_OUTPUT_MKV "../testdata/big buck bunny.mkv"
#define SAMPLE_OUTPUT_WEBM "../testdata/big buck bunny.webm"
#define SAMPLE_OUTPUT_MPEGTS "../testdata/big buck bunny.ts"


#include "ffmpegtests.h"
#include "dictionarytest.h"
#include "memorycontexttest.h"
#include "frametests.h"
#include "packettests.h"
#include "codectests.h"
#include "datastructurestest.h"
#include "encodingdecodingtests.h"
#include "filtertests.h"

#include <iostream>

inline int ExecuteTests(int argc, char* argv[]){
	std::cout << "Current working directory " << get_working_path() << std::endl;
	std::cout << "Executing tests..." << std::endl;

	auto result = Catch::Session().run(argc, argv);
	std::cout << "Done executing tests, success " << (result == 0) << std::endl;
	return result;
}

#endif