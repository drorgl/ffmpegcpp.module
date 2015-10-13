/*
* data structures tests
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _TEST_DATASTRUCTURES_
#define _TEST_DATASTRUCTURES_

#include <ffmpeg.h>
#include <memory>

namespace datastructures{

	SCENARIO("ffmpeg datastructures", "[datastructure]"){
		GIVEN("ffmpeg datastructure tests"){

			WHEN("media type name"){
				std::vector<ffmpegcpp::mediaType> mtvec;
				mtvec.push_back(ffmpegcpp::mediaType::attachment);
				mtvec.push_back(ffmpegcpp::mediaType::audio);
				mtvec.push_back(ffmpegcpp::mediaType::data);
				mtvec.push_back(ffmpegcpp::mediaType::nb);
				mtvec.push_back(ffmpegcpp::mediaType::subtitle);
				mtvec.push_back(ffmpegcpp::mediaType::unknown);
				mtvec.push_back(ffmpegcpp::mediaType::video);

				for (auto mt : mtvec){
					auto mtname = ffmpegcpp::getMediaTypeName(mt);
					REQUIRE(mtname != "");
					INFO(mtname);
					auto mtret = ffmpegcpp::getMediaTypeByName(mtname);
					REQUIRE(mtret == mt);
				}
			}

			WHEN("sample format name"){
				std::vector<ffmpegcpp::sample_format> sfvec;
				sfvec.push_back(ffmpegcpp::sample_format::dbl);
				sfvec.push_back(ffmpegcpp::sample_format::dblp);
				sfvec.push_back(ffmpegcpp::sample_format::flt);
				sfvec.push_back(ffmpegcpp::sample_format::fltp);
				sfvec.push_back(ffmpegcpp::sample_format::s16);
				sfvec.push_back(ffmpegcpp::sample_format::s16p);
				sfvec.push_back(ffmpegcpp::sample_format::s32);
				sfvec.push_back(ffmpegcpp::sample_format::s32p);
				sfvec.push_back(ffmpegcpp::sample_format::u8);
				sfvec.push_back(ffmpegcpp::sample_format::u8p);

				for (auto sf : sfvec){
					auto sfname = ffmpegcpp::getSampleFormatName(sf);
					REQUIRE(sfname != "");
					INFO(sfname);
					auto sfret = ffmpegcpp::getSampleFormatByName(sfname);
					REQUIRE(sfret == sf);
				}

			}

			WHEN("sample format name empty/none"){
				REQUIRE("" == ffmpegcpp::getSampleFormatName(ffmpegcpp::sample_format::none));
				REQUIRE(ffmpegcpp::sample_format::none == ffmpegcpp::getSampleFormatByName(""));
			}

			WHEN("sample format is planar"){
				std::vector<std::pair<ffmpegcpp::sample_format,bool>> sfvec;
				sfvec.push_back(std::pair<ffmpegcpp::sample_format,bool>(ffmpegcpp::sample_format::dbl,false));
				sfvec.push_back(std::pair<ffmpegcpp::sample_format,bool>(ffmpegcpp::sample_format::dblp,true));
				sfvec.push_back(std::pair<ffmpegcpp::sample_format,bool>(ffmpegcpp::sample_format::flt,false));
				sfvec.push_back(std::pair<ffmpegcpp::sample_format,bool>(ffmpegcpp::sample_format::fltp,true));
				sfvec.push_back(std::pair<ffmpegcpp::sample_format,bool>(ffmpegcpp::sample_format::s16,false));
				sfvec.push_back(std::pair<ffmpegcpp::sample_format,bool>(ffmpegcpp::sample_format::s16p,true));
				sfvec.push_back(std::pair<ffmpegcpp::sample_format,bool>(ffmpegcpp::sample_format::s32,false));
				sfvec.push_back(std::pair<ffmpegcpp::sample_format,bool>(ffmpegcpp::sample_format::s32p,true));
				sfvec.push_back(std::pair<ffmpegcpp::sample_format,bool>(ffmpegcpp::sample_format::u8,false));
				sfvec.push_back(std::pair<ffmpegcpp::sample_format,bool>(ffmpegcpp::sample_format::u8p,true));

				for (auto sf : sfvec){
					INFO(ffmpegcpp::getSampleFormatName(sf.first))
					REQUIRE(sf.second == ffmpegcpp::isSampleFormatPlanar(sf.first));
				}

			}

			WHEN("channel layout name"){
				std::vector<ffmpegcpp::channel_layout> clvec;
				clvec.push_back(ffmpegcpp::channel_layout::STEREO);
				clvec.push_back(ffmpegcpp::channel_layout::L7POINT0);
				clvec.push_back(ffmpegcpp::channel_layout::L5POINT1);
				clvec.push_back(ffmpegcpp::channel_layout::MONO);
				clvec.push_back(ffmpegcpp::channel_layout::QUAD);
				clvec.push_back(ffmpegcpp::channel_layout::SURROUND);

				for (auto cl : clvec){
					auto clname = ffmpegcpp::getChannelLayoutName(cl);
					REQUIRE(clname != "");
					INFO(clname);
					auto clret = ffmpegcpp::getChannelLayoutByName(clname);
					REQUIRE(clret == cl);
				}
			}

			WHEN("channel layout number of channels"){
				std::vector<std::pair<ffmpegcpp::channel_layout,int>> clvec;
				clvec.push_back(std::pair<ffmpegcpp::channel_layout,int>(ffmpegcpp::channel_layout::STEREO,2));
				clvec.push_back(std::pair<ffmpegcpp::channel_layout,int>(ffmpegcpp::channel_layout::L7POINT0,7));
				clvec.push_back(std::pair<ffmpegcpp::channel_layout,int>(ffmpegcpp::channel_layout::L5POINT1,6));
				clvec.push_back(std::pair<ffmpegcpp::channel_layout,int>(ffmpegcpp::channel_layout::MONO,1));
				clvec.push_back(std::pair<ffmpegcpp::channel_layout,int>(ffmpegcpp::channel_layout::QUAD,4));
				clvec.push_back(std::pair<ffmpegcpp::channel_layout,int>(ffmpegcpp::channel_layout::SURROUND,3));

				for (auto cl : clvec){
					INFO(ffmpegcpp::getChannelLayoutName(cl.first));
					REQUIRE(cl.second == ffmpegcpp::getChannelLayoutNumberOfChannels(cl.first));
				}
			}

			WHEN("pixel format name"){
				std::vector<ffmpegcpp::pixel_format> pfvec;
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV420P);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUYV422);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_RGB24);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_BGR24);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV422P);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV444P);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV410P);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV411P);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_GRAY8);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_MONOWHITE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_MONOBLACK);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_PAL8);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVJ420P);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVJ422P);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVJ444P);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_XVMC_MPEG2_MC);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_XVMC_MPEG2_IDCT);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_UYVY422);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_UYYVYY411);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_BGR8);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_BGR4);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_BGR4_BYTE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_RGB8);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_RGB4);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_RGB4_BYTE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_NV12);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_NV16);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_NV21);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_ARGB);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_RGBA);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_ABGR);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_BGRA);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_GRAY16BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_GRAY16LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV440P);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVJ440P);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVA420P);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_VDPAU_H264);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_VDPAU_MPEG1);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_VDPAU_MPEG2);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_VDPAU_WMV3);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_VDPAU_VC1);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_RGB48BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_RGB48LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_RGB565BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_RGB565LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_RGB555BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_RGB555LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_BGR565BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_BGR565LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_BGR555BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_BGR555LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_VAAPI_MOCO);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_VAAPI_IDCT);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_VAAPI_VLD);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV420P16LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV420P16BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV422P16LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV422P16BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV444P16LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV444P16BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_VDPAU_MPEG4);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_DXVA2_VLD);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_RGB444LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_RGB444BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_BGR444LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_BGR444BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_GRAY8A);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_BGR48BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_BGR48LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV420P9BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV420P9LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV420P10BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV420P10LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV422P10BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV422P10LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV444P9BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV444P9LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV444P10BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV444P10LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV422P9BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV422P9LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_VDA_VLD);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_RGBA64BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_RGBA64LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_BGRA64BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_BGRA64LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_GBRP);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_GBRP9BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_GBRP9LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_GBRP10BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_GBRP10LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_GBRP16BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_GBRP16LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVA420P9BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVA420P9LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVA422P9BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVA422P9LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVA444P9BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVA444P9LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVA420P10BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVA420P10LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVA422P10BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVA422P10LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVA444P10BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVA444P10LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVA420P16BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVA420P16LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVA422P16BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVA422P16LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVA444P16BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVA444P16LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_VDPAU);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_XYZ12LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_XYZ12BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_0RGB);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_RGB0);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_0BGR);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_BGR0);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVA444P);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUVA422P);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV420P12BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV420P12LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV420P14BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV420P14LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV422P12BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV422P12LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV422P14BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV422P14LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV444P12BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV444P12LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV444P14BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YUV444P14LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_GBRP12BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_GBRP12LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_GBRP14BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_GBRP14LE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_NB);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YA16BE);
				pfvec.push_back(ffmpegcpp::pixel_format::PIX_FMT_YA16LE);

				for (auto pf : pfvec){
					std::string pfname;
					REQUIRE_NOTHROW(pfname = ffmpegcpp::getPixelFormatName(pf));
					REQUIRE("" != pfname);
					INFO(pfname);
					ffmpegcpp::pixel_format pfret;
					REQUIRE_NOTHROW(pfret = ffmpegcpp::getPixelFormatByName(pfname));
					REQUIRE(pf == pfret);
				}
			}

			WHEN("pixel format name empty/none"){
				REQUIRE("" == ffmpegcpp::getPixelFormatName(ffmpegcpp::pixel_format::PIX_FMT_NONE));
				REQUIRE(ffmpegcpp::pixel_format::PIX_FMT_NONE == ffmpegcpp::getPixelFormatByName(""));
			}
			
			WHEN("rational from to"){
				ffmpegcpp::rational r1(1, 2);
				REQUIRE(r1.toDouble() == 0.5f);
				REQUIRE(ffmpegcpp::rational::fromDouble(0.5f,1).toDouble() == r1.toDouble());
			}

			WHEN("variant conversions"){
				REQUIRE(ffmpegcpp::variant("1").toi64() == 1);
				REQUIRE(ffmpegcpp::variant("1").todbl() == 1.0f);
				REQUIRE(ffmpegcpp::variant("1").tostr() == "1");
				REQUIRE(ffmpegcpp::variant("1").toq().toDouble() == 1.0f);
				REQUIRE(ffmpegcpp::variant("1").source_type() == ffmpegcpp::variant_source_type::str);
				
				REQUIRE(ffmpegcpp::variant((int64_t)1).toi64() == 1);
				REQUIRE(ffmpegcpp::variant((int64_t)1).todbl() == 1.0f);
				REQUIRE(ffmpegcpp::variant((int64_t)1).tostr() == "1");
				REQUIRE(ffmpegcpp::variant((int64_t)1).toq().toDouble() == 1.0f);
				REQUIRE(ffmpegcpp::variant((int64_t)1).source_type() == ffmpegcpp::variant_source_type::i64);

				REQUIRE(ffmpegcpp::variant((double)1).toi64() == 1);
				REQUIRE(ffmpegcpp::variant((double)1).todbl() == 1.0f);
				REQUIRE(ffmpegcpp::variant((double)1).tostr() == "1");
				REQUIRE(ffmpegcpp::variant((double)1).toq().toDouble() == 1.0f);
				REQUIRE(ffmpegcpp::variant((double)1).source_type() == ffmpegcpp::variant_source_type::dbl);

				REQUIRE(ffmpegcpp::variant((double)1.5f).toi64() == 1);
				REQUIRE(ffmpegcpp::variant((double)1.5f).todbl() == 1.5f);
				REQUIRE(ffmpegcpp::variant((double)1.5f).tostr() == "1.5");
				REQUIRE(ffmpegcpp::variant((double)1.5f).toq().toDouble() == 1.5f);
			}

			WHEN("option type name"){
				std::vector<ffmpegcpp::option_type> otvec;

				otvec.push_back(ffmpegcpp::option_type::FLAGS);
				otvec.push_back(ffmpegcpp::option_type::INT);
				otvec.push_back(ffmpegcpp::option_type::INT64);
				otvec.push_back(ffmpegcpp::option_type::DOUBLE);
				otvec.push_back(ffmpegcpp::option_type::FLOAT);
				otvec.push_back(ffmpegcpp::option_type::STRING);
				otvec.push_back(ffmpegcpp::option_type::RATIONAL);
				otvec.push_back(ffmpegcpp::option_type::BINARY);
				otvec.push_back(ffmpegcpp::option_type::REAL_CONST);
				otvec.push_back(ffmpegcpp::option_type::IMAGE_SIZE);
				otvec.push_back(ffmpegcpp::option_type::PIXEL_FMT);
				otvec.push_back(ffmpegcpp::option_type::SAMPLE_FMT);
				otvec.push_back(ffmpegcpp::option_type::VIDEO_RATE);
				otvec.push_back(ffmpegcpp::option_type::DURATION);
				otvec.push_back(ffmpegcpp::option_type::COLOR);
				otvec.push_back(ffmpegcpp::option_type::VOPTIONS);


				for (auto ot : otvec){
					std::string otname;
					REQUIRE_NOTHROW(otname = ffmpegcpp::getOptionTypeName(ot));
					REQUIRE("" != otname);
					INFO(otname);
					ffmpegcpp::option_type otret;
					REQUIRE_NOTHROW(otret = ffmpegcpp::getOptionTypeByName(otname));
					REQUIRE(ot == otret);
				}
			}

			
			

			WHEN("component flags name"){
				std::vector<ffmpegcpp::component_flags> cfvec;

				cfvec.push_back(ffmpegcpp::component_flags::NOFILE				);
				cfvec.push_back(ffmpegcpp::component_flags::NEEDNUMBER 			);
				cfvec.push_back(ffmpegcpp::component_flags::SHOW_IDS 			);
				cfvec.push_back(ffmpegcpp::component_flags::RAWPICTURE			);
				cfvec.push_back(ffmpegcpp::component_flags::GLOBALHEADER		);
				cfvec.push_back(ffmpegcpp::component_flags::NOTIMESTAMPS 		);
				cfvec.push_back(ffmpegcpp::component_flags::GENERIC_INDEX 		);
				cfvec.push_back(ffmpegcpp::component_flags::TS_DISCONT 			);
				cfvec.push_back(ffmpegcpp::component_flags::VARIABLE_FPS 		);
				cfvec.push_back(ffmpegcpp::component_flags::NODIMENSIONS		);
				cfvec.push_back(ffmpegcpp::component_flags::NOSTREAMS			);
				cfvec.push_back(ffmpegcpp::component_flags::NOBINSEARCH			);
				cfvec.push_back(ffmpegcpp::component_flags::NOGENSEARCH 		);
				cfvec.push_back(ffmpegcpp::component_flags::NO_BYTE_SEEK		);
				cfvec.push_back(ffmpegcpp::component_flags::ALLOW_FLUSH			);
				cfvec.push_back(ffmpegcpp::component_flags::TS_NONSTRICT 		);
				cfvec.push_back(ffmpegcpp::component_flags::TS_NEGATIVE 		);
				cfvec.push_back(ffmpegcpp::component_flags::SEEK_TO_PTS 		);


				for (auto cf : cfvec){
					std::string cfname;
					REQUIRE_NOTHROW(cfname = ffmpegcpp::getComponentFlags(cf));
					REQUIRE("" != cfname);
					INFO(cfname);
					ffmpegcpp::component_flags cfret;
					REQUIRE_NOTHROW(cfret = ffmpegcpp::getComponentFlagsByName(cfname));
					REQUIRE(cf == cfret);
				}
			}

			WHEN("component flags multiple"){
				std::string cfname;
				REQUIRE_NOTHROW(cfname = ffmpegcpp::getComponentFlags(ffmpegcpp::component_flags::ALLOW_FLUSH | ffmpegcpp::component_flags::GENERIC_INDEX));
				REQUIRE("allow_flush, generic_index" == cfname);
				ffmpegcpp::component_flags cfret;
				REQUIRE_NOTHROW(cfret = ffmpegcpp::getComponentFlagsByName(cfname));
				REQUIRE((cfret & ffmpegcpp::component_flags::ALLOW_FLUSH) == ffmpegcpp::component_flags::ALLOW_FLUSH);
				REQUIRE((cfret & ffmpegcpp::component_flags::GENERIC_INDEX) == ffmpegcpp::component_flags::GENERIC_INDEX);
				REQUIRE((cfret & ffmpegcpp::component_flags::GLOBALHEADER) != ffmpegcpp::component_flags::GLOBALHEADER);
			}

			WHEN("component flags and check"){
				REQUIRE(((ffmpegcpp::component_flags::ALLOW_FLUSH | ffmpegcpp::component_flags::GENERIC_INDEX)  & ffmpegcpp::component_flags::GENERIC_INDEX) == ffmpegcpp::component_flags::GENERIC_INDEX);
				REQUIRE(((ffmpegcpp::component_flags::ALLOW_FLUSH | ffmpegcpp::component_flags::GENERIC_INDEX)  & ffmpegcpp::component_flags::NOBINSEARCH) != ffmpegcpp::component_flags::GENERIC_INDEX);
			}


			//cap
			WHEN("codec capabilities name"){
				std::vector<ffmpegcpp::codec_capabilities> ccvec;

				ccvec.push_back(ffmpegcpp::codec_capabilities::DRAW_HORIZ_BAND);
				ccvec.push_back(ffmpegcpp::codec_capabilities::DR1);
				ccvec.push_back(ffmpegcpp::codec_capabilities::TRUNCATED);
				ccvec.push_back(ffmpegcpp::codec_capabilities::HWACCEL);
				ccvec.push_back(ffmpegcpp::codec_capabilities::DELAY);
				ccvec.push_back(ffmpegcpp::codec_capabilities::SMALL_LAST_FRAME);
				ccvec.push_back(ffmpegcpp::codec_capabilities::HWACCEL_VDPAU);
				ccvec.push_back(ffmpegcpp::codec_capabilities::SUBFRAMES);
				ccvec.push_back(ffmpegcpp::codec_capabilities::EXPERIMENTAL);
				ccvec.push_back(ffmpegcpp::codec_capabilities::CHANNEL_CONF);
				ccvec.push_back(ffmpegcpp::codec_capabilities::NEG_LINESIZES);
				ccvec.push_back(ffmpegcpp::codec_capabilities::FRAME_THREADS);
				ccvec.push_back(ffmpegcpp::codec_capabilities::SLICE_THREADS);
				ccvec.push_back(ffmpegcpp::codec_capabilities::PARAM_CHANGE);
				ccvec.push_back(ffmpegcpp::codec_capabilities::AUTO_THREADS);
				ccvec.push_back(ffmpegcpp::codec_capabilities::VARIABLE_FRAME_SIZE);
				ccvec.push_back(ffmpegcpp::codec_capabilities::INTRA_ONLY);
				ccvec.push_back(ffmpegcpp::codec_capabilities::LOSSLESS);

				for (auto cc : ccvec){
					std::string ccname;
					REQUIRE_NOTHROW(ccname = ffmpegcpp::getCodecCapabilities(cc));
					REQUIRE("" != ccname);
					INFO(ccname);
					ffmpegcpp::codec_capabilities ccret;
					REQUIRE_NOTHROW(ccret = ffmpegcpp::getCodecCapabilitiesByName(ccname));
					REQUIRE(cc == ccret);
				}
			}

			WHEN("codec capabilities multiple vector"){
				std::vector<std::string> ccnames;
				REQUIRE_NOTHROW(ccnames = ffmpegcpp::getCodecCapabilitiesVector(ffmpegcpp::codec_capabilities::FRAME_THREADS | ffmpegcpp::codec_capabilities::SMALL_LAST_FRAME));
				REQUIRE_VECTOR_HAS(ccnames, "frame_threads");
				REQUIRE_VECTOR_HAS(ccnames, "small_last_frame");
			}

			WHEN("component flags multiple"){
				std::string ccname;
				REQUIRE_NOTHROW(ccname = ffmpegcpp::getCodecCapabilities(ffmpegcpp::codec_capabilities::FRAME_THREADS | ffmpegcpp::codec_capabilities::SMALL_LAST_FRAME));
				REQUIRE("frame_threads, small_last_frame" == ccname);
				ffmpegcpp::codec_capabilities ccret;
				REQUIRE_NOTHROW(ccret = ffmpegcpp::getCodecCapabilitiesByName(ccname));
				REQUIRE((ccret & ffmpegcpp::codec_capabilities::FRAME_THREADS) == ffmpegcpp::codec_capabilities::FRAME_THREADS);
				REQUIRE((ccret & ffmpegcpp::codec_capabilities::SMALL_LAST_FRAME) == ffmpegcpp::codec_capabilities::SMALL_LAST_FRAME);
				REQUIRE((ccret & ffmpegcpp::codec_capabilities::DELAY) != ffmpegcpp::codec_capabilities::DELAY);
			}

			WHEN("component flags and check"){
				REQUIRE(((ffmpegcpp::codec_capabilities::FRAME_THREADS | ffmpegcpp::codec_capabilities::SMALL_LAST_FRAME)  & ffmpegcpp::codec_capabilities::SMALL_LAST_FRAME) == ffmpegcpp::codec_capabilities::SMALL_LAST_FRAME);
				REQUIRE(((ffmpegcpp::codec_capabilities::FRAME_THREADS | ffmpegcpp::codec_capabilities::SMALL_LAST_FRAME)  & ffmpegcpp::codec_capabilities::DELAY) != ffmpegcpp::codec_capabilities::DELAY);
			}



		}
	}

}
#endif