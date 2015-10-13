/*
* memory context tests
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _TEST_MEMORYCONTEXT_
#define _TEST_MEMORYCONTEXT_

#include <ffmpeg.h>
#include <memory>

namespace memorycontext{

	SCENARIO("ffmpeg memorycontext", "[memorycontext]"){
		GIVEN("ffmpeg memorycontext creation/deletion/actions"){
			WHEN("memorycontext ctor"){
				std::shared_ptr<ffmpegcpp::memorycontext> memctx = std::make_shared<ffmpegcpp::memorycontext>(10, 20);

				WHEN("empty"){
					REQUIRE(memctx->size() == 0);
					REQUIRE(memctx->blocks() == 0);
				}

				WHEN("write 1"){
					uint8_t x = '1';
					memctx->write_next_block(&x, 1);
					REQUIRE(memctx->size() == 1);
					REQUIRE(memctx->blocks() == 1);

					WHEN("write 2"){
						memctx->write_next_block(&x, 1);
						REQUIRE(memctx->size() == 2);
						REQUIRE(memctx->blocks() == 2);

						WHEN("clear"){
							memctx->clear();
							REQUIRE(memctx->size() == 0);
							REQUIRE(memctx->blocks() == 0);
						}
					}

					WHEN("read 1"){
						x = '0';
						auto actuallyRead = memctx->read_next_block(&x, 1);
						REQUIRE(memctx->size() == 0);
						REQUIRE(memctx->blocks() == 0);
						REQUIRE(actuallyRead == 1);
						REQUIRE(x == '1');

					}
				}

				WHEN("flush"){
					memctx->flush();
				}

				WHEN("memorycontext dtor"){
					memctx = nullptr;
					REQUIRE((memctx == nullptr) == true);
				}
			}
		}
	}

}
#endif