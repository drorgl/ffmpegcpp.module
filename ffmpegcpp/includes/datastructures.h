/*
* ffmpegcpp data structures
* Copyright (c) 2015 Dror Gluska, All rights reserved.
*
* This file is part of ffmpegcpp
*
* ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
*
* ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
*/


#ifndef _DATA_STRUCTURES_
#define _DATA_STRUCTURES_

#include "exports.h"

#include <string>
#include <vector>
#include <stdint.h>
#include <assert.h>
#include <memory>

#include <stdlib.h>

#include <sstream>

namespace ffmpegcpp
{
	enum mediaType
	{
		unknown = -1,
		video,
		audio,
		data,
		subtitle,
		attachment,
		nb
	};

	FFMPEGCPP_API std::string getMediaTypeName(mediaType media);
	FFMPEGCPP_API mediaType getMediaTypeByName(std::string media);

	enum sample_format
	{
		none,
		u8,   // unsigned 8 bits
		s16,  // signed 16 bits
		s32,  // signed 32 bits
		flt,  // float
		dbl,  // double

		u8p,  // unsigned 8 bits, planar
		s16p, // signed 16 bits, planar
		s32p, // signed 32 bits, planar
		fltp, // float, planar
		dblp  // double, planar
	};

	FFMPEGCPP_API std::string getSampleFormatName(sample_format samplefmt);
	FFMPEGCPP_API sample_format getSampleFormatByName(std::string samplefmt_name);
	FFMPEGCPP_API bool isSampleFormatPlanar(sample_format samplefmt);
	


	enum channels : uint64_t
	{
		FRONT_LEFT =  0x00000001,
		FRONT_RIGHT =  0x00000002,
		FRONT_CENTER =  0x00000004,
		LOW_FREQUENCY =  0x00000008,
		BACK_LEFT =  0x00000010,
		BACK_RIGHT =  0x00000020,
		FRONT_LEFT_OF_CENTER  = 0x00000040,
		FRONT_RIGHT_OF_CENTER =  0x00000080,
		BACK_CENTER =  0x00000100,
		SIDE_LEFT =  0x00000200,
		SIDE_RIGHT =  0x00000400,
		TOP_CENTER  = 0x00000800,
		TOP_FRONT_LEFT =  0x00001000,
		TOP_FRONT_CENTER =  0x00002000,
		TOP_FRONT_RIGHT =  0x00004000,
		TOP_BACK_LEFT =  0x00008000,
		TOP_BACK_CENTER =  0x00010000,
		TOP_BACK_RIGHT  = 0x00020000,
		STEREO_LEFT  = 0x20000000,
		STEREO_RIGHT  = 0x40000000,
		WIDE_LEFT =  0x0000000080000000ULL,
		WIDE_RIGHT =  0x0000000100000000ULL,
		SURROUND_DIRECT_LEFT =  0x0000000200000000ULL,
		SURROUND_DIRECT_RIGHT =  0x0000000400000000ULL,
		LOW_FREQUENCY_2 =  0x0000000800000000ULL,
		LAYOUT_NATIVE  = 0x8000000000000000ULL, 
	};
	
	enum channel_layout : __int64
	{
		MONO = (channels::FRONT_CENTER),
		STEREO =  (channels::FRONT_LEFT|channels::FRONT_RIGHT),
		L2POINT1 = (channel_layout::STEREO | channels::LOW_FREQUENCY),
		L2_1 = (channel_layout::STEREO | channels::BACK_CENTER),
		SURROUND = (channel_layout::STEREO | channels::FRONT_CENTER),
		L3POINT1 = (channel_layout::SURROUND | channels::LOW_FREQUENCY),
		L4POINT0 = (channel_layout::SURROUND | channels::BACK_CENTER),
		L4POINT1 = (channel_layout::L4POINT0 | channels::LOW_FREQUENCY),
		L2_2 = (channel_layout::STEREO | channels::SIDE_LEFT | channels::SIDE_RIGHT),
		QUAD = (channel_layout::STEREO | channels::BACK_LEFT | channels::BACK_RIGHT),
		L5POINT0 = (channel_layout::SURROUND | channels::SIDE_LEFT | channels::SIDE_RIGHT),
		L5POINT1 = (channel_layout::L5POINT0 | channels::LOW_FREQUENCY),
		L5POINT0_BACK = (channel_layout::SURROUND | channels::BACK_LEFT | channels::BACK_RIGHT),
		L5POINT1_BACK = (channel_layout::L5POINT0_BACK | channels::LOW_FREQUENCY),
		L6POINT0 = (channel_layout::L5POINT0 | channels::BACK_CENTER),
		L6POINT0_FRONT = (channel_layout::L2_2 | channels::FRONT_LEFT_OF_CENTER | channels::FRONT_RIGHT_OF_CENTER),
		HEXAGONAL = (channel_layout::L5POINT0_BACK | channels::BACK_CENTER),
		L6POINT1 = (channel_layout::L5POINT1 | channels::BACK_CENTER),
		L6POINT1_BACK = (channel_layout::L5POINT1_BACK | channels::BACK_CENTER),
		L6POINT1_FRONT = (channel_layout::L6POINT0_FRONT | channels::LOW_FREQUENCY),
		L7POINT0 = (channel_layout::L5POINT0 | channels::BACK_LEFT | channels::BACK_RIGHT),
		L7POINT0_FRONT = (channel_layout::L5POINT0 | channels::FRONT_LEFT_OF_CENTER | channels::FRONT_RIGHT_OF_CENTER),
		L7POINT1 = (channel_layout::L5POINT1 | channels::BACK_LEFT | channels::BACK_RIGHT),
		L7POINT1_WIDE = (channel_layout::L5POINT1 | channels::FRONT_LEFT_OF_CENTER | channels::FRONT_RIGHT_OF_CENTER),
		L7POINT1_WIDE_BACK = (channel_layout::L5POINT1_BACK | channels::FRONT_LEFT_OF_CENTER | channels::FRONT_RIGHT_OF_CENTER),
		OCTAGONAL = (channel_layout::L5POINT0 | channels::BACK_LEFT | channels::BACK_CENTER | channels::BACK_RIGHT),
		STEREO_DOWNMIX = (channels::STEREO_LEFT | channels::STEREO_RIGHT)
	};

	FFMPEGCPP_API std::string getChannelLayoutName(channel_layout channellayout);
	FFMPEGCPP_API channel_layout getChannelLayoutByName(std::string channellayout_name);
	FFMPEGCPP_API int getChannelLayoutNumberOfChannels(channel_layout channellayout);

	enum pixel_format
	{
		PIX_FMT_NONE ,
		PIX_FMT_YUV420P,   
		PIX_FMT_YUYV422,   
		PIX_FMT_RGB24,     
		PIX_FMT_BGR24,     
		PIX_FMT_YUV422P,   
		PIX_FMT_YUV444P,   
		PIX_FMT_YUV410P,   
		PIX_FMT_YUV411P,   
		PIX_FMT_GRAY8,     
		PIX_FMT_MONOWHITE, 
		PIX_FMT_MONOBLACK, 
		PIX_FMT_PAL8,      
		PIX_FMT_YUVJ420P,  
		PIX_FMT_YUVJ422P,  
		PIX_FMT_YUVJ444P,  
		PIX_FMT_XVMC_MPEG2_MC,
		PIX_FMT_XVMC_MPEG2_IDCT,
		PIX_FMT_UYVY422,   
		PIX_FMT_UYYVYY411, 
		PIX_FMT_BGR8,      
		PIX_FMT_BGR4,      
		PIX_FMT_BGR4_BYTE, 
		PIX_FMT_RGB8,      
		PIX_FMT_RGB4,      
		PIX_FMT_RGB4_BYTE, 
		PIX_FMT_NV12, 
		PIX_FMT_NV16,
		PIX_FMT_NV21,      
		PIX_FMT_ARGB,      
		PIX_FMT_RGBA,      
		PIX_FMT_ABGR,      
		PIX_FMT_BGRA,      
		PIX_FMT_GRAY16BE,  
		PIX_FMT_GRAY16LE,  
		PIX_FMT_YUV440P,   
		PIX_FMT_YUVJ440P,  
		PIX_FMT_YUVA420P,  
		PIX_FMT_VDPAU_H264,
		PIX_FMT_VDPAU_MPEG1,
		PIX_FMT_VDPAU_MPEG2,
		PIX_FMT_VDPAU_WMV3,
		PIX_FMT_VDPAU_VC1, 
		PIX_FMT_RGB48BE,   
		PIX_FMT_RGB48LE,   
		PIX_FMT_RGB565BE,  
		PIX_FMT_RGB565LE,  
		PIX_FMT_RGB555BE,  
		PIX_FMT_RGB555LE,  
		PIX_FMT_BGR565BE,  
		PIX_FMT_BGR565LE,  
		PIX_FMT_BGR555BE,  
		PIX_FMT_BGR555LE,  
		PIX_FMT_VAAPI_MOCO, 
		PIX_FMT_VAAPI_IDCT, 
		PIX_FMT_VAAPI_VLD,  
		PIX_FMT_YUV420P16LE,
		PIX_FMT_YUV420P16BE,
		PIX_FMT_YUV422P16LE,
		PIX_FMT_YUV422P16BE,
		PIX_FMT_YUV444P16LE,
		PIX_FMT_YUV444P16BE,
		PIX_FMT_VDPAU_MPEG4,
		PIX_FMT_DXVA2_VLD,  
		PIX_FMT_RGB444LE, 
		PIX_FMT_RGB444BE, 
		PIX_FMT_BGR444LE, 
		PIX_FMT_BGR444BE, 
		PIX_FMT_GRAY8A,   
		PIX_FMT_BGR48BE,  
		PIX_FMT_BGR48LE,  
		PIX_FMT_YUV420P9BE, 
		PIX_FMT_YUV420P9LE, 
		PIX_FMT_YUV420P10BE,
		PIX_FMT_YUV420P10LE,
		PIX_FMT_YUV422P10BE,
		PIX_FMT_YUV422P10LE,
		PIX_FMT_YUV444P9BE, 
		PIX_FMT_YUV444P9LE, 
		PIX_FMT_YUV444P10BE,
		PIX_FMT_YUV444P10LE,
		PIX_FMT_YUV422P9BE, 
		PIX_FMT_YUV422P9LE, 
		PIX_FMT_VDA_VLD,    
		PIX_FMT_RGBA64BE, 
		PIX_FMT_RGBA64LE, 
		PIX_FMT_BGRA64BE, 
		PIX_FMT_BGRA64LE, 
		PIX_FMT_GBRP,      
		PIX_FMT_GBRP9BE,   
		PIX_FMT_GBRP9LE,   
		PIX_FMT_GBRP10BE,  
		PIX_FMT_GBRP10LE,  
		PIX_FMT_GBRP16BE,  
		PIX_FMT_GBRP16LE,  
		//PIX_FMT_YUVA422P_LIBAV, 
		//PIX_FMT_YUVA444P_LIBAV, 
		PIX_FMT_YUVA420P9BE, 
		PIX_FMT_YUVA420P9LE, 
		PIX_FMT_YUVA422P9BE, 
		PIX_FMT_YUVA422P9LE, 
		PIX_FMT_YUVA444P9BE, 
		PIX_FMT_YUVA444P9LE, 
		PIX_FMT_YUVA420P10BE,
		PIX_FMT_YUVA420P10LE,
		PIX_FMT_YUVA422P10BE,
		PIX_FMT_YUVA422P10LE,
		PIX_FMT_YUVA444P10BE,
		PIX_FMT_YUVA444P10LE,
		PIX_FMT_YUVA420P16BE,
		PIX_FMT_YUVA420P16LE,
		PIX_FMT_YUVA422P16BE,
		PIX_FMT_YUVA422P16LE,
		PIX_FMT_YUVA444P16BE,
		PIX_FMT_YUVA444P16LE,
		PIX_FMT_VDPAU,   
		PIX_FMT_XYZ12LE, 
		PIX_FMT_XYZ12BE, 
		/*PIX_FMT_RGBA64BE, 
		PIX_FMT_RGBA64LE,
		PIX_FMT_BGRA64BE,
		PIX_FMT_BGRA64LE,*/
		PIX_FMT_0RGB ,
		PIX_FMT_RGB0,     
		PIX_FMT_0BGR,     
		PIX_FMT_BGR0,     
		PIX_FMT_YUVA444P, 
		PIX_FMT_YUVA422P, 
		PIX_FMT_YUV420P12BE, 
		PIX_FMT_YUV420P12LE, 
		PIX_FMT_YUV420P14BE, 
		PIX_FMT_YUV420P14LE, 
		PIX_FMT_YUV422P12BE, 
		PIX_FMT_YUV422P12LE, 
		PIX_FMT_YUV422P14BE, 
		PIX_FMT_YUV422P14LE, 
		PIX_FMT_YUV444P12BE, 
		PIX_FMT_YUV444P12LE, 
		PIX_FMT_YUV444P14BE, 
		PIX_FMT_YUV444P14LE, 
		PIX_FMT_GBRP12BE,    
		PIX_FMT_GBRP12LE,    
		PIX_FMT_GBRP14BE,    
		PIX_FMT_GBRP14LE,    
		//PIX_FMT_GBRAP,       
		//PIX_FMT_GBRAP16BE,   
		//PIX_FMT_GBRAP16LE,   
		PIX_FMT_YUVJ411P,    
		PIX_FMT_NB,
		PIX_FMT_YA16BE,
		PIX_FMT_YA16LE
	};

	FFMPEGCPP_API std::string getPixelFormatName(pixel_format pixfmt);
	FFMPEGCPP_API pixel_format getPixelFormatByName(std::string pixfmt_name);

	struct rational{
		int num, den;
		rational(int num_, int den_)
		{
			num = num_;
			den = den_;
		}

		rational(){}

		FFMPEGCPP_API double toDouble() const;

		FFMPEGCPP_API static rational fromDouble(double avq, int max);
	};


	enum class variant_source_type
	{
		i64,
		dbl,
		str,
		q
	};

	class variant
	{
	private:

		variant_source_type _src_type;

		int64_t _src_i64 = 0;
		double _src_dbl = 0;
		std::string _src_str;
		rational _src_q = rational(0, 0);
	public:
		variant() : variant("")
		{
			
		}

		variant(int64_t from_int64_t)
		{
			_src_i64 = from_int64_t;
			_src_type = variant_source_type::i64;
		}

		variant(double from_dbl)
		{
			_src_dbl = from_dbl;
			_src_type = variant_source_type::dbl;
		}

		variant(std::string from_string)
		{
			_src_str = from_string;
			_src_type = variant_source_type::str;
		}

		variant(rational from_rational)
		{
			_src_q = from_rational;
			_src_type = variant_source_type::q;
		}

		variant_source_type source_type()
		{
			return _src_type;
		}

		variant& operator = (int64_t from_int64_t)
		{
			*this = variant(from_int64_t);
			return *this;
		}

		variant& operator = (double from_dbl)
		{
			*this = variant(from_dbl);
			return *this;
		}

		variant& operator = (std::string from_string)
		{
			*this = variant(from_string);
			return *this;
		}

		variant& operator = (rational from_rational)
		{
			*this = variant(from_rational);
			return *this;
		}

		int64_t toi64()
		{
			switch (_src_type)
			{
			case variant_source_type::i64:
				return _src_i64;
				break;
			case variant_source_type::dbl:
				return static_cast<int64_t>(_src_dbl);
			case variant_source_type::str:
			{
				char* endptr;
				return strtoll(_src_str.c_str(), &endptr, 10);
				//return std::stoll(_src_str); //problem with ndk, revert to strtoll
			}
			case variant_source_type::q:
				return static_cast<int64_t>(_src_q.toDouble());
			default:
				assert(false && "variant::toi64 variant_source_type is invalid");
				throw std::logic_error("variant::toi64 variant_source_type is invalid");
			}
		}

		double todbl()
		{
			switch (_src_type)
			{
			case variant_source_type::i64:
				return static_cast<double>(_src_i64);
				break;
			case variant_source_type::dbl:
				return _src_dbl;
			case variant_source_type::str:
			{
				char* endptr;
				return strtod(_src_str.c_str(), &endptr);
				//return std::stod(_src_str); //not part of ndk
			}
			case variant_source_type::q:
				return _src_q.toDouble();
			default:
				assert(false && "variant::todbl variant_source_type is invalid");
				throw std::logic_error("variant::todbl variant_source_type is invalid");
			}
		}

		template <typename T>
		static std::string tostring(T val)
		{
			std::stringstream ss;
			ss.unsetf(std::ios::fixed | std::ios::scientific);
			ss << val;
			return ss.str();
		}

		std::string tostr()
		{
			switch (_src_type)
			{
			case variant_source_type::i64:
				return variant::tostring(_src_i64); //not part of ndk
				break;
			case variant_source_type::dbl:
				return variant::tostring(_src_dbl); //not part of ndk
			case variant_source_type::str:
				return _src_str;
			case variant_source_type::q:
				return variant::tostring(_src_q.toDouble()); //not part of ndk
			default:
				assert(false && "variant::tostr variant_source_type is invalid");
				throw std::logic_error("variant::tostr variant_source_type is invalid");
			}
		}

		rational toq()
		{
			switch (_src_type)
			{
			case variant_source_type::i64:
				return rational::fromDouble(static_cast<double>(_src_i64), 10);
				break;
			case variant_source_type::dbl:
				return rational::fromDouble(_src_dbl,10);
			case variant_source_type::str:
			{
				char * endptr;
				auto tdbl = strtod(_src_str.c_str(), &endptr);
				//return rational::fromDouble(std::stod(_src_str),10); //missing in ndk
				return rational::fromDouble(tdbl, 10);
			}
			case variant_source_type::q:
				return _src_q;
			default:
				assert(false && "variant::toq variant_source_type is invalid");
				throw std::logic_error("variant::toq variant_source_type is invalid");
			}
		}
	};

	enum class option_type
	{
		FLAGS,
		INT,
		INT64,
		DOUBLE,
		FLOAT,
		STRING,
		RATIONAL,
		BINARY,
		REAL_CONST,
		IMAGE_SIZE,
		PIXEL_FMT,
		SAMPLE_FMT,
		VIDEO_RATE,
		DURATION,
		COLOR,
		VOPTIONS
	};
	FFMPEGCPP_API std::string getOptionTypeName(option_type opttype);
	FFMPEGCPP_API option_type getOptionTypeByName(std::string opttype);


	struct option
	{
		std::string name;
		std::string help;
		option_type type = option_type::INT;
		variant default_val = (int64_t)0;
		double min = 0;
		double max = 0;
		int flags = 0;
		std::string unit;
	};

	enum class component_flags : int
	{
		NOFILE      =  0x0001,
		/**< Needs '%d' in filename. */
		NEEDNUMBER  =  0x0002, 
		/**< Show format stream IDs numbers. */
		SHOW_IDS    =  0x0008,
		/**< Format wants AVPicture structure for raw picture data. */
		RAWPICTURE  =  0x0020,
		/**< Format wants global header. */
		GLOBALHEADER  = 0x0040, 
		/**< Format does not need / have any timestamps. */
		NOTIMESTAMPS  = 0x0080, 
		/**< Use generic index building code. */
		GENERIC_INDEX = 0x0100, 
		/**< Format allows timestamp discontinuities. Note, muxers always require valid (monotone) timestamps */
		TS_DISCONT    = 0x0200, 
		/**< Format allows variable fps. */
		VARIABLE_FPS  = 0x0400, 
		/**< Format does not need width/height */
		NODIMENSIONS  = 0x0800, 
		/**< Format does not require any streams */
		NOSTREAMS     = 0x1000, 
		/**< Format does not allow to fall back on binary search via read_timestamp */
		NOBINSEARCH   = 0x2000, 
		/**< Format does not allow to fall back on generic search */
		NOGENSEARCH   = 0x4000, 
		/**< Format does not allow seeking by bytes */
		NO_BYTE_SEEK  = 0x8000, 
		/**< Format allows flushing. If not set, the muxer will not receive a NULL packet in the write_packet function. */
		ALLOW_FLUSH  = 0x10000, 
		/**< Format does not require strictly increasing timestamps, but they must still be monotonic */
		TS_NONSTRICT = 0x20000,

		/**< Format allows muxing negative
		timestamps. If not set the timestamp
		will be shifted in av_write_frame and
		av_interleaved_write_frame so they
		start from 0.
		The user or muxer can override this through
		AVFormatContext.avoid_negative_ts
		*/
		TS_NEGATIVE  = 0x40000, 
		/**< Seeking is based on PTS */
		SEEK_TO_PTS  = 0x4000000 
	};

	FFMPEGCPP_API component_flags operator | (component_flags x, component_flags y);
	FFMPEGCPP_API component_flags operator & (component_flags x, component_flags y);
	FFMPEGCPP_API std::string getComponentFlags(component_flags codec_cap);
	FFMPEGCPP_API component_flags getComponentFlagsByName(std::string codec_cap);

	struct component
	{
		std::string name;
		std::string long_name;
		component_flags flags; 
		std::string mime_type;
		std::string extensions;
		std::vector<option> options;
	};

	struct codec_profile
	{
		int profile;
		std::string name;

		codec_profile(int profile_, std::string name_)
		{
			profile = profile_;
			name = name_;
		}
	};
	
	enum class codec_capabilities : uint32_t
	{
		/*Decoder can use draw_horiz_band callback.*/
		DRAW_HORIZ_BAND = 0x0001,
		/*Codec uses get_buffer() for allocating buffers and supports custom allocators.*/
		DR1 = 0x0002,
		TRUNCATED = 0x0008,
		/* Codec can export data for HW decoding (XvMC).*/
		HWACCEL = 0x0010,
		/*encoder must set the pts and duration for each output packet. If this flag is not set, the pts and duration will be determined by libavcodec from the input frame.*/
		DELAY = 0x0020,
		/*Codec can be fed a final frame with a smaller size.
		 * This can be used to prevent truncation of the last audio samples.*/
		SMALL_LAST_FRAME = 0x0040,
		/*Codec can export data for HW decoding (VDPAU).*/
		HWACCEL_VDPAU = 0x0080,
		/*Packets may contain more than one frame*/
		SUBFRAMES = 0x0100,
		/*Codec is experimental*/
		EXPERIMENTAL = 0x0200,
		/*Codec should fill in channel configuration and samplerate instead of container*/
		CHANNEL_CONF = 0x0400,
		/* Codec is able to deal with negative linesizes*/
		NEG_LINESIZES = 0x0800,
		/* Codec supports frame-level multithreading.*/
		FRAME_THREADS = 0x1000,
		/*Codec supports slice-based (or partition-based) multithreading.*/
		SLICE_THREADS = 0x2000,
		/*Codec supports changed parameters at any point.*/
		PARAM_CHANGE = 0x4000,
		/*Codec supports avctx->thread_count == 0 (auto).*/
		AUTO_THREADS = 0x8000,
		/* Audio encoder supports receiving a different number of samples in each call.*/
		VARIABLE_FRAME_SIZE = 0x10000,
		/*Codec is intra only.*/
		INTRA_ONLY = 0x40000000,
		/*Codec is lossless.*/
		LOSSLESS = 0x80000000,
	};

	FFMPEGCPP_API codec_capabilities operator | (codec_capabilities x, codec_capabilities y);
	FFMPEGCPP_API codec_capabilities operator & (codec_capabilities x, codec_capabilities y);

	FFMPEGCPP_API std::string getCodecCapabilities(codec_capabilities codec_cap);
	FFMPEGCPP_API std::vector<std::string> getCodecCapabilitiesVector(codec_capabilities codec_cap);
	FFMPEGCPP_API codec_capabilities getCodecCapabilitiesByName(std::string codec_cap);

	struct codecinfo
	{
		std::string name;
		std::string long_name;
		bool isEncoder;
		bool isDecoder;
		enum mediaType mediaType;
		codec_capabilities capabilities; 
		std::vector<rational> frameRates;
		std::vector<pixel_format> pixelFormats;
		std::vector<int> sampleRates;
		std::vector<sample_format> sampleFormats;
		std::vector<channel_layout> channelLayouts;
		uint8_t max_lowres; //av_codec_get_max_lowres()
		std::vector<codec_profile> profiles;
	};

	
	enum class devicetype
	{
		unknown,
		video,
		audio
	};

	struct deviceinfo{
		std::string pin;
	};

	struct deviceinfovideo : deviceinfo
	{
		enum pixel_format pixelFormat;
		std::string codec;
		int minWidth;
		int minHeight;
		double minFPS;
		int maxWidth;
		int maxHeight;
		double maxFPS;
	};

	struct deviceinfoaudio : deviceinfo
	{
		int minChannels;
		int minBits;
		int minRate;
		int maxChannels;
		int maxBits;
		int maxRate;
	};

	struct device
	{
		std::string name;
		std::string full_name;
		std::string format;
		devicetype type;
		std::vector<std::shared_ptr<deviceinfo>> deviceInfos;
	};

};

#endif