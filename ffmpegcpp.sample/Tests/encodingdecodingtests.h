/*
* encoding and decoding tests
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/

#ifndef _TEST_ENCODINGDECODING_
#define _TEST_ENCODINGDECODING_

#include <ffmpeg.h>
#include <memory>

#include <opencv2/core/core.hpp>



namespace encodingdecoding{
	static const int MAX_AUDIO_LENGTH = 1024 * 1024;

	void logger(std::string module, int level, std::string message)
	{
		std::cout << module << " [" << level << "] " << message << std::endl;
	}

	SCENARIO("ffmpeg encoding decoding", "[encodingdecoding]"){
		GIVEN("ffmpeg encoding decoding webm"){
			WHEN("opening output and closing before doing anything"){
				ffmpegcpp::ffmpeg ffm;

				std::remove(SAMPLE_OUTPUT_MKV);
				auto outctx = ffm.open_output(SAMPLE_OUTPUT_MKV, "");
				outctx->close();

				REQUIRE_FALSE(file_exists(SAMPLE_OUTPUT_MKV));
			}
			WHEN("opening buffered output and closing before doing anything"){
				ffmpegcpp::ffmpeg ffm;
				std::shared_ptr<ffmpegcpp::memorycontext> memctx = std::make_shared<ffmpegcpp::memorycontext>(1024, 2048);
				REQUIRE_NOTHROW(
				{
					auto outctx = ffm.open_output(memctx, "", "ffm");
					outctx->close();
				});
			}
			WHEN("opening buffered output and closing with one stream"){
				ffmpegcpp::ffmpeg ffm;
				std::shared_ptr<ffmpegcpp::memorycontext> memctx = std::make_shared<ffmpegcpp::memorycontext>(1024, 2048);
				auto outctx = ffm.open_output(memctx,"", "ffm");

				auto videoStream = outctx->addStream("libvpx", ffmpegcpp::rational(1, (int)25));
				auto videoCodec = videoStream->getCodec();
				videoCodec->width(320);
				videoCodec->height(240);
				videoCodec->bit_rate(150000);
				videoCodec->gop_size(12);
				videoCodec->time_base(ffmpegcpp::rational(1, (int)25));
				videoCodec->pix_fmt(ffmpegcpp::pixel_format::PIX_FMT_YUV420P);

				//create encoder and open, possible encoder parameters
				auto contextSettings = std::make_shared<ffmpegcpp::dictionary>();
				contextSettings->set("strict", "experimental");
				videoStream->getEncoder()->open(contextSettings);


				REQUIRE_NOTHROW(outctx->close());
			}

			WHEN("getting SDP"){
				ffmpegcpp::ffmpeg ffm;
				std::shared_ptr<ffmpegcpp::memorycontext> memctx = std::make_shared<ffmpegcpp::memorycontext>(1024, 2048);
				auto outctx = ffm.open_output(memctx,"", "matroska");

				auto videoStream = outctx->addStream("libvpx", ffmpegcpp::rational(1, (int)25));
				auto videoCodec = videoStream->getCodec();
				videoCodec->width(320);
				videoCodec->height(240);
				videoCodec->bit_rate(150000);
				videoCodec->gop_size(12);
				videoCodec->time_base(ffmpegcpp::rational(1, (int)25));
				videoCodec->pix_fmt(ffmpegcpp::pixel_format::PIX_FMT_YUV420P);

				//create encoder and open, possible encoder parameters
				auto contextSettings = std::make_shared<ffmpegcpp::dictionary>();
				contextSettings->set("strict", "experimental");
				videoStream->getEncoder()->open(contextSettings);

				auto sdp = outctx->getSDP();

				CHECK(sdp != "");

				outctx->close();
			}

			WHEN("opening buffered output and adding experimental codec without experimental option and closing with one stream"){
				//Test is less relevant now when this codec is no longer marked as experimental...

				ffmpegcpp::ffmpeg ffm;
				std::shared_ptr<ffmpegcpp::memorycontext> memctx = std::make_shared<ffmpegcpp::memorycontext>(1024, 2048);
				auto outctx = ffm.open_output(memctx,"", "ffm");

				auto videoStream = outctx->addStream("libvpx", ffmpegcpp::rational(1, (int)25));
				auto videoCodec = videoStream->getCodec();
				videoCodec->width(320);
				videoCodec->height(240);
				videoCodec->bit_rate(150000);
				videoCodec->gop_size(12);
				videoCodec->time_base(ffmpegcpp::rational(1, (int)25));
				videoCodec->pix_fmt(ffmpegcpp::pixel_format::PIX_FMT_YUV420P);

				//create encoder and open, possible encoder parameters
				auto contextSettings = std::make_shared<ffmpegcpp::dictionary>();
				//contextSettings->set("strict", "experimental");
				REQUIRE_NOTHROW({
					videoStream->getEncoder()->open(contextSettings);
				});

				REQUIRE_NOTHROW(outctx->close());
			}

			WHEN("opening file without format specified"){
				ffmpegcpp::ffmpeg ffm;
				auto inctx = ffm.open_input(SAMPLE_INPUT_FILENAME, "", nullptr);
				inctx->close();
			}
			WHEN("transcoding to webm"){
				ffmpegcpp::ffmpeg ffm;

				std::map<int, cv::Mat*> mats;

				//open input context
				auto inctx = ffm.open_input(SAMPLE_INPUT_FILENAME, "mp4", nullptr);

				//open output file
				std::remove(SAMPLE_OUTPUT_WEBM);
				auto outctx = ffm.open_output(SAMPLE_OUTPUT_WEBM, "webm");

				auto contextSettings = std::make_shared<ffmpegcpp::dictionary>();
				contextSettings->set("strict", "experimental");

				std::map<int, std::shared_ptr<ffmpegcpp::swscale>> inconverters;
				std::map<int, std::shared_ptr<ffmpegcpp::swscale>> outconverters;

				std::map<int, std::shared_ptr<ffmpegcpp::stream>> matching_output_streams;

				//get input streams
				auto instreams = inctx->getStreams();


				for (auto stream : *instreams)
				{
					auto codec = stream->getCodec();


					switch (stream->getCodec()->mediaType())
					{
					case ffmpegcpp::mediaType::video:
					{
						//open and initialize decoder, possible decoder parameters
						stream->getDecoder()->open();

						int newwidth = codec->width();
						int newheight = codec->height();

						//initialize image mat
						mats[stream->index()] = new cv::Mat(newheight, newwidth, CV_8UC3);



						//initialize video input converter
						inconverters[stream->index()] = ffmpegcpp::swscale::fromVideoCodec(stream->getCodec(), newwidth, newheight);

						//initialize output stream/codec
						auto videoStream = outctx->addStream("libvpx", ffmpegcpp::rational(1, (int)stream->r_frame_rate().toDouble()));
						matching_output_streams[stream->index()] = videoStream;
						auto videoCodec = videoStream->getCodec();
						videoCodec->width(newwidth);
						videoCodec->height(newheight);
						videoCodec->bit_rate(600000);
						videoCodec->gop_size(120);
						videoCodec->time_base(ffmpegcpp::rational(1, (int)stream->r_frame_rate().toDouble()));
						videoCodec->pix_fmt(ffmpegcpp::pixel_format::PIX_FMT_YUV420P);

						//create encoder and open, possible encoder parameters
						//contextSettings = std::make_shared<ffmpegcpp::dictionary>();
						contextSettings->set("strict", "experimental");
						contextSettings->set("quality", "realtime");
						contextSettings->set("threads", "4");

						videoStream->getEncoder()->open(contextSettings);

						//create an out converter, which will convert from our decoder's dimensions to our encoder's dimensions
						outconverters[stream->index()] = ffmpegcpp::swscale::toVideoCodec(videoCodec, newwidth, newheight);
					}
						break;
					case ffmpegcpp::mediaType::audio:
					{
						//open and initialize decoder, possible decoder parameters
						stream->getDecoder()->open();

						//initialize audio mat

						//planar <> mat = use row for each channel
						//normal <> mat = use channel(color) for channel - which is PCM format (interleaved - LRLRLRLRLRLRLRLRLR...)

						//mats[stream->index()] = new cv::Mat(1, (int)MAX_AUDIO_LENGTH, CV_MAKETYPE(CV_16U, 2));
						mats[stream->index()] = new cv::Mat(2, (int)MAX_AUDIO_LENGTH, CV_MAKETYPE(CV_32F, 1)); //planar format

						//initialize audio input converter
						inconverters[stream->index()] = ffmpegcpp::swscale::fromAudioCodec(stream->getCodec(), ffmpegcpp::channel_layout::STEREO, 2, ffmpegcpp::sample_format::fltp, 44100);

						auto audioStream = outctx->addStream("vorbis", ffmpegcpp::rational(1, (int)stream->r_frame_rate().toDouble()));
						matching_output_streams[stream->index()] = audioStream;
						auto audioCodec = audioStream->getCodec();
						audioCodec->bit_rate(64000);
						audioCodec->channels(2);
						audioCodec->sample_fmt(ffmpegcpp::sample_format::fltp);
						audioCodec->sample_rate(44100);
						audioCodec->channels_layout(ffmpegcpp::channel_layout::STEREO);
						audioCodec->time_base(ffmpegcpp::rational(1, 44100));

						//create encoder and open, possible encoder parameters
						//contextSettings = std::make_shared<ffmpegcpp::dictionary>();
						contextSettings->set("strict", "experimental");
						audioStream->getEncoder()->open(contextSettings);

						//create an out converter, which will convert from our decoder's dimensions to our encoder's dimensions
						outconverters[stream->index()] = ffmpegcpp::swscale::toAudioCodec(audioCodec, ffmpegcpp::channel_layout::STEREO, 2, ffmpegcpp::sample_format::fltp, 44100);
					}
						break;
					}

					auto name = codec->name();
				}

				//write header (possible muxer parameters)
				outctx->write_header();

				ffmpegcpp::frame current_input_frame;
				ffmpegcpp::frame current_output_frame;

				ffmpegcpp::packet current_output_packet;

				int limiter = SAMPLE_MAXIMUM_PACKETS;

				std::shared_ptr<ffmpegcpp::packet> packet = std::make_shared<ffmpegcpp::packet>();

				auto timemark1 = std::chrono::high_resolution_clock::now();
				int videoFrames = 0;

				while ((inctx->readPacket(packet) == ffmpegcpp::readPacketState::success) && limiter > 0)
				{
					limiter--;

					auto packet_stream_index = packet->stream_index();
					auto packet_stream = instreams->at(packet_stream_index);
					if (packet_stream->index() != packet_stream_index){
						throw std::runtime_error("programming error");
					}
					auto decoder = packet_stream->getDecoder();

					//reset current frame, free buffers, etc'
					current_input_frame.unref();

					//reset current output frame, free buffers, etc'
					current_output_frame.unref();

					//decode packet to frame
					if (!decoder->decode(*packet, current_input_frame))
					{
						//we didn't get a valid frame, so read the next packet
						continue;
					}

					//convert packet to desired format
					switch (packet_stream->getCodec()->mediaType())
					{
					case ffmpegcpp::mediaType::audio:
					{
						int samplesRead;
						inconverters[packet_stream_index]->scaleAudio(&current_input_frame, *mats[packet_stream_index], samplesRead);

						auto output_codec = matching_output_streams[packet_stream_index]->getCodec();

						//nb_samples should match the size of the frame AFTER the conversion
						current_output_frame.set_audio_parameters(output_codec->channels(), output_codec->sample_fmt(), output_codec->channels_layout(), output_codec->sample_rate(), samplesRead);
						current_output_frame.get_buffer(64);

						//convert the mat to the output codec format
						outconverters[packet_stream_index]->scaleAudio(&current_output_frame, *mats[packet_stream_index], samplesRead);

						current_output_packet.unref();

						if (matching_output_streams[packet_stream_index]->getEncoder()->encode(&current_output_frame, &current_output_packet)){
							outctx->write_packet_interleaved(current_output_packet);
						}

					}
						break;
					case ffmpegcpp::mediaType::video:
					{
						videoFrames++;
						inconverters[packet_stream_index]->scaleVideo(&current_input_frame, *mats[packet_stream_index]);

						auto output_codec = matching_output_streams[packet_stream_index]->getCodec();

						//nb_samples should match the size of the frame AFTER the conversion
						current_output_frame.set_video_parameters(output_codec->width(), output_codec->height(), output_codec->pix_fmt());
						current_output_frame.get_buffer(64);

						//convert the mat to the output codec format
						outconverters[packet_stream_index]->scaleVideo(&current_output_frame, *mats[packet_stream_index]);

						current_output_packet.unref();

						if (matching_output_streams[packet_stream_index]->getEncoder()->encode(&current_output_frame, &current_output_packet)){
							outctx->write_packet_interleaved(current_output_packet);
						}
					}
						break;
					default:
						throw std::runtime_error("not implemented");
					}
				}

				inctx->close();
				outctx->close();

				auto timemark2 = std::chrono::high_resolution_clock::now();
				auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(timemark2 - timemark1).count();
				INFO(tostring(((double)videoFrames / ((double)duration / 1000))) << "fps");
				REQUIRE(file_exists(SAMPLE_OUTPUT_WEBM));
			}

			WHEN("transcoding mkv"){
				ffmpegcpp::ffmpeg ffm;

				std::map<int, cv::Mat*> mats;

				//open input context
				auto inctx = ffm.open_input(SAMPLE_INPUT_FILENAME, "mp4", nullptr);

				//open output file
				std::remove(SAMPLE_OUTPUT_MKV);
				auto outctx = ffm.open_output(SAMPLE_OUTPUT_MKV, "mkv");

				auto contextSettings = std::make_shared<ffmpegcpp::dictionary>();
				contextSettings->set("strict", "experimental");

				std::map<int, std::shared_ptr<ffmpegcpp::swscale>> inconverters;
				std::map<int, std::shared_ptr<ffmpegcpp::swscale>> outconverters;

				std::map<int, std::shared_ptr<ffmpegcpp::stream>> matching_output_streams;

				//get input streams
				auto instreams = inctx->getStreams();


				for (auto stream : *instreams)
				{
					auto codec = stream->getCodec();


					switch (stream->getCodec()->mediaType())
					{
					case ffmpegcpp::mediaType::video:
					{
						//open and initialize decoder, possible decoder parameters
						stream->getDecoder()->open();

						int newwidth = codec->width();
						int newheight = codec->height();

						//initialize image mat
						mats[stream->index()] = new cv::Mat(newheight, newwidth, CV_8UC3);



						//initialize video input converter
						inconverters[stream->index()] = ffmpegcpp::swscale::fromVideoCodec(stream->getCodec(), newwidth, newheight);

						//initialize output stream/codec
						auto videoStream = outctx->addStream("libx264", ffmpegcpp::rational(1, (int)stream->r_frame_rate().toDouble()));
						matching_output_streams[stream->index()] = videoStream;
						auto videoCodec = videoStream->getCodec();
						videoCodec->width(newwidth);
						videoCodec->height(newheight);
						videoCodec->bit_rate(500000);
						videoCodec->gop_size(120);
						videoCodec->time_base(ffmpegcpp::rational(1, (int)stream->r_frame_rate().toDouble()));
						videoCodec->pix_fmt(ffmpegcpp::pixel_format::PIX_FMT_YUV420P);

						//create encoder and open, possible encoder parameters
						//contextSettings = std::make_shared<ffmpegcpp::dictionary>();
						contextSettings->set("strict", "experimental");
						contextSettings->set("tune", "zerolatency");
						contextSettings->set("preset", "ultrafast");
						videoStream->getEncoder()->open(contextSettings);

						//create an out converter, which will convert from our decoder's dimensions to our encoder's dimensions
						outconverters[stream->index()] = ffmpegcpp::swscale::toVideoCodec(videoCodec, newwidth, newheight);
					}
						break;
					case ffmpegcpp::mediaType::audio:
					{
						//open and initialize decoder, possible decoder parameters
						stream->getDecoder()->open();

						//initialize audio mat

						//planar <> mat = use row for each channel
						//normal <> mat = use channel(color) for channel - which is PCM format (interleaved - LRLRLRLRLRLRLRLRLR...)

						mats[stream->index()] = new cv::Mat(1, (int)MAX_AUDIO_LENGTH, CV_MAKETYPE(CV_16U, 2));
						//mats[stream->index()] = new cv::Mat(2, (int)MAX_AUDIO_LENGTH, CV_MAKETYPE(CV_32F, 1)); //planar format

						//initialize audio input converter
						inconverters[stream->index()] = ffmpegcpp::swscale::fromAudioCodec(stream->getCodec(), ffmpegcpp::channel_layout::STEREO, 2, ffmpegcpp::sample_format::s16, 44100);

						auto audioStream = outctx->addStream("libmp3lame", ffmpegcpp::rational(1, (int)stream->r_frame_rate().toDouble()));
						matching_output_streams[stream->index()] = audioStream;
						auto audioCodec = audioStream->getCodec();
						audioCodec->bit_rate(128000);
						audioCodec->channels(2);
						audioCodec->sample_fmt(ffmpegcpp::sample_format::s16p);
						audioCodec->sample_rate(44100);
						audioCodec->channels_layout(ffmpegcpp::channel_layout::STEREO);
						audioCodec->time_base(ffmpegcpp::rational(1, 44100));

						//create encoder and open, possible encoder parameters
						//contextSettings = std::make_shared<ffmpegcpp::dictionary>();
						contextSettings->set("strict", "experimental");
						audioStream->getEncoder()->open(contextSettings);

						//create an out converter, which will convert from our decoder's dimensions to our encoder's dimensions
						outconverters[stream->index()] = ffmpegcpp::swscale::toAudioCodec(audioCodec, ffmpegcpp::channel_layout::STEREO, 2, ffmpegcpp::sample_format::s16, 44100);
					}
						break;
					}

					auto name = codec->name();
				}

				//write header (possible muxer parameters)
				outctx->write_header();

				ffmpegcpp::frame current_input_frame;
				ffmpegcpp::frame current_output_frame;

				ffmpegcpp::packet current_output_packet;

				int limiter = SAMPLE_MAXIMUM_PACKETS;

				std::shared_ptr<ffmpegcpp::packet> packet = std::make_shared<ffmpegcpp::packet>();

				auto timemark1 = std::chrono::high_resolution_clock::now();
				int videoFrames = 0;

				while ((inctx->readPacket(packet) == ffmpegcpp::readPacketState::success) && limiter > 0)
				{
					limiter--;

					auto packet_stream_index = packet->stream_index();
					auto packet_stream = instreams->at(packet_stream_index);
					if (packet_stream->index() != packet_stream_index){
						throw std::runtime_error("programming error");
					}
					auto decoder = packet_stream->getDecoder();

					//reset current frame, free buffers, etc'
					current_input_frame.unref();

					//reset current output frame, free buffers, etc'
					current_output_frame.unref();

					//decode packet to frame
					if (!decoder->decode(*packet, current_input_frame))
					{
						//we didn't get a valid frame, so read the next packet
						continue;
					}

					//convert packet to desired format
					switch (packet_stream->getCodec()->mediaType())
					{
					case ffmpegcpp::mediaType::audio:
					{
						int samplesRead;
						inconverters[packet_stream_index]->scaleAudio(&current_input_frame, *mats[packet_stream_index], samplesRead);

						auto output_codec = matching_output_streams[packet_stream_index]->getCodec();

						//nb_samples should match the size of the frame AFTER the conversion
						current_output_frame.set_audio_parameters(output_codec->channels(), output_codec->sample_fmt(), output_codec->channels_layout(), output_codec->sample_rate(), samplesRead);
						current_output_frame.get_buffer(64);

						//convert the mat to the output codec format
						outconverters[packet_stream_index]->scaleAudio(&current_output_frame, *mats[packet_stream_index], samplesRead);

						current_output_packet.unref();

						if (matching_output_streams[packet_stream_index]->getEncoder()->encode(&current_output_frame, &current_output_packet)){
							outctx->write_packet_interleaved(current_output_packet);
						}

					}
						break;
					case ffmpegcpp::mediaType::video:
					{
						videoFrames++;
						inconverters[packet_stream_index]->scaleVideo(&current_input_frame, *mats[packet_stream_index]);

						auto output_codec = matching_output_streams[packet_stream_index]->getCodec();

						//nb_samples should match the size of the frame AFTER the conversion
						current_output_frame.set_video_parameters(output_codec->width(), output_codec->height(), output_codec->pix_fmt());
						current_output_frame.get_buffer(64);

						//convert the mat to the output codec format
						outconverters[packet_stream_index]->scaleVideo(&current_output_frame, *mats[packet_stream_index]);

						current_output_packet.unref();

						if (matching_output_streams[packet_stream_index]->getEncoder()->encode(&current_output_frame, &current_output_packet)){
							outctx->write_packet_interleaved(current_output_packet);
						}
					}
						break;
					default:
						throw std::runtime_error("not implemented");
					}
				}

				inctx->close();
				outctx->close();

				auto timemark2 = std::chrono::high_resolution_clock::now();
				auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(timemark2 - timemark1).count();
				INFO(tostring(((double)videoFrames / ((double)duration / 1000))) << "fps");
				REQUIRE(file_exists(SAMPLE_OUTPUT_MKV));
			}


			WHEN("copy to mpegts"){
				//ffmpegcpp::ffmpeg::setLogger(logger);
				ffmpegcpp::ffmpeg ffm;

				//open input context
				auto inctx = ffm.open_input(SAMPLE_INPUT_FILENAME, "mp4", nullptr);

				//open output file
				std::remove(SAMPLE_OUTPUT_MPEGTS);
				auto outctx = ffm.open_output(SAMPLE_OUTPUT_MPEGTS, "mpegts");

				auto contextSettings = std::make_shared<ffmpegcpp::dictionary>();
				contextSettings->set("strict", "experimental");

				std::map<int, std::shared_ptr<ffmpegcpp::stream>> matching_output_streams;

				std::map<std::string, std::string> codecmapping;
				codecmapping["h264"] = "libx264";

				//get input streams
				auto instreams = inctx->getStreams();

				for (auto stream : *instreams)
				{
					stream->getDecoder()->open();
					auto codec = stream->getCodec();

					auto codecName = codec->name();
					if (codecmapping.count(codec->name())){
						codecName = codecmapping.at(codec->name());
					}
					

					switch (stream->getCodec()->mediaType())
					{
					case ffmpegcpp::mediaType::video:
					{
						int newwidth = codec->width();
						int newheight = codec->height();

						//initialize output stream/codec

						if (codecName == "libx264"){
							stream->addBitstreamFilter(std::make_shared<ffmpegcpp::bitstreamfilter>("h264_mp4toannexb", "", stream->getCodec()));
						}

						auto videoStream = outctx->addStream(codecName, ffmpegcpp::rational(1, (int)stream->r_frame_rate().toDouble()));
						matching_output_streams[stream->index()] = videoStream;
						auto videoCodec = videoStream->getCodec();
						videoCodec->width(newwidth);
						videoCodec->height(newheight);
						videoCodec->bit_rate(codec->bit_rate());
						videoCodec->gop_size(codec->gop_size());
						videoCodec->time_base(ffmpegcpp::rational(1, (int)stream->r_frame_rate().toDouble()));
						videoCodec->pix_fmt(codec->pix_fmt());

						//create encoder and open, possible encoder parameters
						//contextSettings = std::make_shared<ffmpegcpp::dictionary>();
						contextSettings->set("strict", "experimental");
						videoStream->getEncoder()->open(contextSettings);

						

					}
						break;
					case ffmpegcpp::mediaType::audio:
					{
						auto audioStream = outctx->addStream(codecName, ffmpegcpp::rational(1, (int)stream->r_frame_rate().toDouble()));
						matching_output_streams[stream->index()] = audioStream;
						auto audioCodec = audioStream->getCodec();
						audioCodec->bit_rate(codec->bit_rate());
						audioCodec->channels(codec->channels());
						audioCodec->sample_fmt(codec->sample_fmt());
						audioCodec->sample_rate(codec->sample_rate());
						audioCodec->channels_layout(codec->channels_layout());
						audioCodec->time_base(ffmpegcpp::rational(1, (int)stream->r_frame_rate().toDouble()));

						//create encoder and open, possible encoder parameters
						//contextSettings = std::make_shared<ffmpegcpp::dictionary>();
						contextSettings->set("strict", "experimental");
						audioStream->getEncoder()->open(contextSettings);

					}
						break;
					}

					auto name = codec->name();
				}

				//write header (possible muxer parameters)
				outctx->write_header();

				ffmpegcpp::frame current_input_frame;
				ffmpegcpp::frame current_output_frame;

				ffmpegcpp::packet current_output_packet;

				auto outstreams = outctx->getStreams();

				int limiter = SAMPLE_MAXIMUM_PACKETS;

				std::shared_ptr<ffmpegcpp::packet> packet = std::make_shared<ffmpegcpp::packet>();

				while ((inctx->readPacket(packet) == ffmpegcpp::readPacketState::success) && limiter > 0)
				{
					limiter--;

					auto packet_stream_index = packet->stream_index();
					auto in_packet_stream = instreams->at(packet_stream_index);
					auto out_packet_stream = outstreams->at(packet_stream_index);
					if (in_packet_stream->index() != packet_stream_index){
						throw std::runtime_error("programming error");
					}

					//reset current frame, free buffers, etc'
					current_input_frame.unref();

					//reset current output frame, free buffers, etc'
					current_output_frame.unref();

					//auto codecTimeBase = packet_stream->getCodec()->time_base();
					//auto streamTimeBase = packet_stream->time_base();
					auto inStreamTimeBase = in_packet_stream->time_base();
					auto outStreamTimeBase = out_packet_stream->time_base();

					//packet->rescale_rnd(ffmpegcpp::rational(codecTimeBase.num, codecTimeBase.den), ffmpegcpp::rational(streamTimeBase.num, streamTimeBase.den));
					packet->rescale_rnd(ffmpegcpp::rational(inStreamTimeBase.num, inStreamTimeBase.den), ffmpegcpp::rational(outStreamTimeBase.num, outStreamTimeBase.den));

					outctx->write_packet_interleaved(*packet);
				}

				inctx->close();
				outctx->close();

				REQUIRE(file_exists(SAMPLE_OUTPUT_MPEGTS));

				ffmpegcpp::ffmpeg::setLogger(nullptr);
			}
		}
	}

}
#endif