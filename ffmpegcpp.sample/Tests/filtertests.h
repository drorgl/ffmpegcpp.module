/*
* bitstream filter tests
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _TEST_BITSTREAMFILTER_
#define _TEST_BITSTREAMFILTER_

#include <ffmpeg.h>
#include <memory>

namespace bitstreamfilter{

	SCENARIO("ffmpeg bitstream filter", "[frame]"){
		GIVEN("ffmpeg bitstream list"){
			WHEN("listing"){	
				auto bsfilters = ffmpegcpp::bitstreamfilter::listBitstreamFilters();
				REQUIRE(bsfilters.size() > 0);
				for (auto bs : bsfilters){
					REQUIRE(bs != "");
				}
			}
		}
	}

}
#endif