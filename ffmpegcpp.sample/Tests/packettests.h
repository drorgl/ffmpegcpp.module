/*
* packet tests
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _TEST_PACKET_
#define _TEST_PACKET_

#include <ffmpeg.h>
#include <memory>

namespace packet{

	SCENARIO("ffmpeg packet", "[packet]"){
		GIVEN("ffmpeg packet creation/deletion/actions"){
			WHEN("packet ctor"){
				std::shared_ptr<ffmpegcpp::packet> pkt = std::make_shared<ffmpegcpp::packet>();
				CHECK((pkt != nullptr) == true);

				WHEN("unref"){
					pkt->unref();
				}

				WHEN("stream_index"){
					CHECK(pkt->stream_index() == 0);
				}


				WHEN("frame dtor"){
					pkt = nullptr;
					CHECK((pkt == nullptr) == true);
				}
			}
		}
	}

}
#endif