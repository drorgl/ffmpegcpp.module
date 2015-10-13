/*
* dictionary tests
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _TEST_DICTIONARY_
#define _TEST_DICTIONARY_

#include <ffmpeg.h>
#include <memory>


SCENARIO("ffmpeg dictionary", "[dictionary]"){
	GIVEN("ffmpeg dictionary creation/deletion"){
		WHEN("new dictionary"){
			std::shared_ptr<ffmpegcpp::dictionary> dict = std::make_shared<ffmpegcpp::dictionary>();
			REQUIRE(dict->count() == 0);
		}

		WHEN("new dictionary from map"){
			std::shared_ptr<std::map<std::string, std::string>> map = std::make_shared<std::map<std::string, std::string>>();
			map->insert(std::pair<std::string, std::string>("one", "1"));

			std::shared_ptr<ffmpegcpp::dictionary> dict = std::make_shared<ffmpegcpp::dictionary>(map);

			REQUIRE(dict->count() == 1);
			REQUIRE(dict->get("one") == "1");

			REQUIRE(map.use_count() == 1); //verify map is not being held by dictionary
		}

		WHEN("add one"){
			std::shared_ptr<ffmpegcpp::dictionary> dict = std::make_shared<ffmpegcpp::dictionary>();
			dict->set("one", "1");
			REQUIRE(dict->count() == 1);
			REQUIRE(dict->get("one") == "1");
		}

		WHEN("add one case sensitive"){
			std::shared_ptr<ffmpegcpp::dictionary> dict = std::make_shared<ffmpegcpp::dictionary>();
			dict->set("One", "1");
			REQUIRE(dict->count() == 1);
			REQUIRE(dict->get("one") == "");
		}

		WHEN("add one extra space at the end"){
			std::shared_ptr<ffmpegcpp::dictionary> dict = std::make_shared<ffmpegcpp::dictionary>();
			dict->set("one ", "1space");
			dict->set("one", "1");
			REQUIRE(dict->count() == 2);
			REQUIRE(dict->get("one") == "1");
			REQUIRE(dict->get("one ") == "1space");
		}

		WHEN("add two"){
			std::shared_ptr<ffmpegcpp::dictionary> dict = std::make_shared<ffmpegcpp::dictionary>();
			dict->set("one", "1");
			dict->set("two", "2");
			REQUIRE(dict->count() == 2);
			REQUIRE(dict->get("one") == "1");
			REQUIRE(dict->get("two") == "2");
		}

		WHEN("clear"){
			std::shared_ptr<ffmpegcpp::dictionary> dict = std::make_shared<ffmpegcpp::dictionary>();
			dict->set("one", "1");
			REQUIRE(dict->count() == 1);
			dict->clear();
			REQUIRE(dict->get("one") == "");
			REQUIRE(dict->count() == 0);
		}
	}
}



#endif