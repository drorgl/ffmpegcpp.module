/*
* frame tests
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _TEST_FRAME_
#define _TEST_FRAME_

#include <ffmpeg.h>
#include <memory>

namespace frame{

	SCENARIO("ffmpeg frame", "[frame]"){
		GIVEN("ffmpeg frame creation/deletion/actions"){

			WHEN("frame ctor/dtor"){
				std::shared_ptr<ffmpegcpp::frame> frm = std::make_shared<ffmpegcpp::frame>();
				frm.reset();
				REQUIRE(frm.use_count() == 0);
			}

			WHEN("frame ctor/dtor"){
				std::shared_ptr<ffmpegcpp::frame> frm = std::make_shared<ffmpegcpp::frame>();
				
				REQUIRE_NOTHROW(frm->set_video_parameters(10, 20, ffmpegcpp::pixel_format::PIX_FMT_YUV420P));

				REQUIRE_NOTHROW(frm.reset());

				REQUIRE(frm.use_count() == 0);
			}

			WHEN("set_video_parameters"){
				std::shared_ptr<ffmpegcpp::frame> frm = std::make_shared<ffmpegcpp::frame>();

				REQUIRE_NOTHROW(frm->set_video_parameters(10, 20, ffmpegcpp::pixel_format::PIX_FMT_YUV420P));

				REQUIRE_NOTHROW(frm->get_buffer(64));
				REQUIRE_NOTHROW(frm->unref());


			}

			WHEN("set_audio_parameters"){
				std::shared_ptr<ffmpegcpp::frame> frm = std::make_shared<ffmpegcpp::frame>();

				REQUIRE_NOTHROW(frm->set_audio_parameters(2, ffmpegcpp::sample_format::s16, ffmpegcpp::channel_layout::STEREO, 44100, 1024));
				REQUIRE_NOTHROW(frm->get_buffer(64));
				REQUIRE_NOTHROW(frm->unref());
			}
		}
	}


}
#endif