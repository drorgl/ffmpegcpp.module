/*
* main ffmpeg wrapper tests
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _TEST_FFMPEG_
#define _TEST_FFMPEG_

#include <ffmpeg.h>
#include <memory>


#include <string>
namespace ffmpeg{

	void logger(std::string module, int level, std::string message)
	{
		std::cout << module << " [" << level << "] " << message << std::endl;
	}

	SCENARIO("ffmpeg", "[ffmpeg]"){
		GIVEN("ffmpeg creation/deletion"){

			WHEN("ffmpeg ctor"){
				INFO("before ffmpeg instance");
				REQUIRE(0 == ffmpegcpp::ffmpeg::instance_count());

				{
					std::shared_ptr<ffmpegcpp::ffmpeg> ffm = std::make_shared<ffmpegcpp::ffmpeg>();
					INFO("create first instance");
					REQUIRE(1 == ffmpegcpp::ffmpeg::instance_count());

				}

				INFO("remove first instance");
				REQUIRE(0 == ffmpegcpp::ffmpeg::instance_count());

				
			}
			WHEN("ffmpeg logger"){
				std::shared_ptr<ffmpegcpp::ffmpeg> ffm = std::make_shared<ffmpegcpp::ffmpeg>();

				REQUIRE_NOTHROW(ffmpegcpp::ffmpeg::setLogger(logger));

				REQUIRE_NOTHROW(ffmpegcpp::ffmpeg::setLogger(nullptr));
			}

			WHEN("listing Input Formats"){
				std::shared_ptr<ffmpegcpp::ffmpeg> ffm = std::make_shared<ffmpegcpp::ffmpeg>();
				std::vector<ffmpegcpp::component> ilist;
				ffmpegcpp::formatcontext::listInputFormats(ilist);
				REQUIRE(ilist.size() > 0);
				for (auto c : ilist){
					REQUIRE(c.name != "");
				}
			}

			WHEN("listing Output Formats"){
				std::shared_ptr<ffmpegcpp::ffmpeg> ffm = std::make_shared<ffmpegcpp::ffmpeg>();
				std::vector<ffmpegcpp::component> olist;
				ffmpegcpp::formatcontext::listOutputFormats(olist);

				REQUIRE(olist.size() > 0);
				for (auto c : olist){
					REQUIRE(c.name != "");
				}
			}

			WHEN("Listing Input Devices"){
				std::shared_ptr<ffmpegcpp::ffmpeg> ffm = std::make_shared<ffmpegcpp::ffmpeg>();
				auto devices = ffmpegcpp::ffmpeg::ListInputDevices();
				CHECK(devices.size() > 0);
				for (auto dev : devices){
					REQUIRE(dev->name != "");
					INFO(dev->name);
					REQUIRE(dev->type != ffmpegcpp::devicetype::unknown);
					REQUIRE(dev->format != "");

					for (auto di : dev->deviceInfos){
						switch (dev->type)
						{
						case ffmpegcpp::devicetype::audio:
						{
							auto dia = std::static_pointer_cast<ffmpegcpp::deviceinfoaudio>(di);
							REQUIRE(dia->minBits > 0);
							REQUIRE(dia->minRate > 0);
							REQUIRE(dia->minChannels > 0);

							REQUIRE(dia->maxBits > 0);
							REQUIRE(dia->maxRate > 0);
							REQUIRE(dia->maxChannels > 0);

						}
							break;
						case ffmpegcpp::devicetype::video:
						{
							auto div = std::static_pointer_cast<ffmpegcpp::deviceinfovideo>(di);
							std::string pixelFormat;
							REQUIRE_NOTHROW(pixelFormat = ffmpegcpp::getPixelFormatName(div->pixelFormat));
							//std::cout << "\t" << div->codec << pixelFormat << " " << div->minWidth << "x" << div->minHeight << " " << div->minFPS << " " << div->maxWidth << "x" << div->maxHeight << " " << div->maxFPS << std::endl;
							REQUIRE(((div->codec != "") || (pixelFormat != "")) == true);
							REQUIRE(div->minWidth > 0);
							REQUIRE(div->minHeight > 0);
							REQUIRE(div->minFPS > 0);

							REQUIRE(div->maxWidth > 0);
							REQUIRE(div->maxHeight > 0);
							REQUIRE(div->maxFPS > 0);


						}
							break;
						}

					}
				}
			}

		}


	}
}



	//TEST_CASE("Hello", "[world]"){
	//	SECTION("world?"){
	//		REQUIRE(false);
	//	}
	//}

	//SCENARIO("hello", "[vector]"){
	//	GIVEN("hello world"){
	//		
	//		REQUIRE(true);
	//
	//		WHEN("bla"){
	//			THEN("BLA BLA"){
	//				REQUIRE(true);
	//			}
	//		}
	//
	//	}
	//}

	//SCENARIO("Testing BufferService", "[bufferservice]"){
	//	GIVEN("Mat"){
	//		ScriptEngine::BufferService bs(nullptr);
	//
	//		std::string matid = "";
	//
	//		CHECK(bs.getStatistics().totalAllocated == 0);
	//		CHECK(bs.getStatistics().allocatedBuffers == 0);
	//		CHECK(bs.getStatistics().allocatedMats == 0);
	//
	//		WHEN("Create Mat"){
	//			matid = bs.createMat(10, 10, 3);
	//			REQUIRE(matid != "");
	//
	//			CHECK(bs.getStatistics().allocatedMats == 1);
	//			CHECK(bs.getStatistics().totalAllocated == 1);
	//			CHECK(bs.getStatistics().allocatedBuffers == 0);
	//
	//			WHEN("Destroy Mat"){
	//				bs.destroyMat(matid);
	//
	//				CHECK(bs.getStatistics().totalAllocated == 0);
	//				CHECK(bs.getStatistics().allocatedMats == 0);
	//				CHECK(bs.getStatistics().allocatedBuffers == 0);
	//			}
	//		}
	//	}
	//
	//
	//	GIVEN("Block"){
	//		ScriptEngine::BufferService bs(nullptr);
	//
	//		std::string blockid = "";
	//
	//		CHECK(bs.getStatistics().totalAllocated == 0);
	//		CHECK(bs.getStatistics().allocatedBuffers == 0);
	//		CHECK(bs.getStatistics().allocatedMats == 0);
	//
	//		WHEN("Create Block"){
	//			blockid = bs.createBlock(10, 10);
	//			REQUIRE(blockid != "");
	//
	//			CHECK(bs.getStatistics().allocatedMats == 0);
	//			CHECK(bs.getStatistics().totalAllocated == 1);
	//			CHECK(bs.getStatistics().allocatedBuffers == 1);
	//
	//			WHEN("Destroy Block"){
	//				bs.destroyBlock(blockid);
	//
	//				CHECK(bs.getStatistics().totalAllocated == 0);
	//				CHECK(bs.getStatistics().allocatedMats == 0);
	//				CHECK(bs.getStatistics().allocatedBuffers == 0);
	//			}
	//		}
	//	}
	//}


#endif