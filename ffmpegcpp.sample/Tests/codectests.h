/*
* codec tests
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _TEST_CODEC_
#define _TEST_CODEC_

#include <ffmpeg.h>
#include <memory>

namespace codec{
	/*void listCodecs(std::vector<ffmpegcpp::codecinfo> codecs)
	{
		for (auto codec : codecs)
		{
			INFO(codec.name + " " + codec.long_name);
			CHECK(codec.name != "");

			if (codec.isEncoder)
				INFO("is encoder");
			if (codec.isDecoder)
				INFO("is decoder");
			
			auto mediaTypeName = ffmpegcpp::getMediaTypeName(codec.mediaType);
			auto codecCap = ffmpegcpp::getCodecCapabilities(codec.capabilities);

			INFO("Media " + mediaTypeName + " Capabilities " + codecCap + " maxlowres " + std::to_string( codec.max_lowres));

			INFO("FrameRates:");

			
			for (auto fr : codec.frameRates)
			{
				INFO(std::to_string(fr.toDouble()));
			}

			INFO("Pixel Formats: ");
			for (auto fr : codec.pixelFormats)
			{
				auto pixfmt = ffmpegcpp::getPixelFormatName(fr);
				INFO(pixfmt);
			}
				
			INFO("Sample Rates: ");
			for (auto fr : codec.sampleRates)
			{
				INFO(std::to_string(fr));
			}

			INFO("Sample Formats: ");
			for (auto fr : codec.sampleFormats)
			{
				auto sfn = ffmpegcpp::getSampleFormatName(fr);
				INFO(sfn);
			}

			INFO("Channel Layouts: ");
			for (auto fr : codec.channelLayouts)
			{
				auto cln = ffmpegcpp::getChannelLayoutName(fr);
				INFO(cln);
			}

			INFO("Profiles: ");
			for (auto fr : codec.profiles)
			{
				INFO(fr.name + "(" + std::to_string(fr.profile) + ")");
			}

		}
	}*/


	SCENARIO("ffmpeg codec", "[codec]"){
		GIVEN("ffmpeg codec list"){
			WHEN("codec list has at least one codec"){
				auto clist = ffmpegcpp::codec::listCodecs();
				REQUIRE(clist.size() > 0);
			}
			WHEN("codec names are not empty")
			{
				auto clist = ffmpegcpp::codec::listCodecs();
				THEN("got codec names"){
					for (auto codec : clist)
					{
						REQUIRE(codec.name != "");
					}
				}
			}

			WHEN("codecs are at least encoders or decoders")
			{
				auto clist = ffmpegcpp::codec::listCodecs();
				THEN("codecs have either decoder or encoder"){
					for (auto codec : clist)
					{
						INFO(codec.name);
						REQUIRE((codec.isEncoder || codec.isDecoder) == true);
					}
				}
			}

			WHEN("codec media types are translatable to string")
			{
				auto clist = ffmpegcpp::codec::listCodecs();
				THEN("codecs have a translatable media type"){
					for (auto codec : clist)
					{
						INFO(codec.name);
						auto mediaTypeName = ffmpegcpp::getMediaTypeName(codec.mediaType);
						CHECK(mediaTypeName != "");
					}
				}
			}

			WHEN("codec capabilities are translatable to string")
			{
				auto clist = ffmpegcpp::codec::listCodecs();
				for (auto codec : clist)
				{
					INFO(codec.name);
					auto codecCap = ffmpegcpp::getCodecCapabilities(codec.capabilities);

					if ((uint32_t)codec.capabilities != 0){
						CHECK(codecCap != "");
					}
				}
			}

			WHEN("codec pixel formats are translatable to string")
			{
				auto clist = ffmpegcpp::codec::listCodecs();
				for (auto codec : clist)
				{
					INFO(codec.name);
					for (auto fr : codec.pixelFormats)
					{
						auto pixfmt = ffmpegcpp::getPixelFormatName(fr);
						CHECK(pixfmt != "");
					}
				}
			}

			WHEN("codec sample formats are translatable to string")
			{
				auto clist = ffmpegcpp::codec::listCodecs();
				for (auto codec : clist)
				{
					INFO(codec.name);
					for (auto fr : codec.sampleFormats)
					{
						auto sfn = ffmpegcpp::getSampleFormatName(fr);
						CHECK(sfn != "");
					}
				}
			}

			WHEN("codec channel layouts are translatable to string")
			{
				auto clist = ffmpegcpp::codec::listCodecs();
				for (auto codec : clist)
				{
					INFO(codec.name);
					for (auto fr : codec.channelLayouts)
					{
						auto cln = ffmpegcpp::getChannelLayoutName(fr);
						CHECK(cln != "");
					}
				}
			}
		}
	}

}

#endif