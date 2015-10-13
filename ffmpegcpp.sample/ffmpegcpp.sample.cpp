/*
* ffmpegcpp sample and test project
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/



#include "stdafx.h"
#include <stdarg.h>
#include <ffmpeg.h>

#include "Tests/all.h"

//only windows...
//#include "audio\wavoutcpp.h"

void logger(std::string module, int level, std::string message)
{
	std::cout << module << " [" << level << "] " << message << std::endl;
}

static const int MAX_AUDIO_LENGTH = 1024 * 1024;

int main(int argc, char* argv[])
{
	auto result = ExecuteTests(argc, argv);

	ffmpegcpp::ffmpeg::setLogger(std::function<void(std::string, int, std::string)>(logger));
	ffmpegcpp::ffmpeg ffm;

	//_CrtDumpMemoryLeaks();
}
