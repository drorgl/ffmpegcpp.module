#include "alsawrapper.h"

extern "C" {
#include "alsa/asoundlib.h"
}

#include <iostream>

std::string alsa_exception::getsnd_strerror(int errnum)
{
	return snd_strerror(errnum);
}

std::function<void(std::string, int, std::string)>  alsa_wrapper::_logger;

void alsa_wrapper::log(std::string module, int level, std::string message)
{
	//std::cout << module << " " << level << " " << message << std::endl;
	if (_logger){
		_logger(module, level, message);
	}
}

void alsa_wrapper::alsa_error_handler (const char *file,int line,const char *function,int err,const char *fmt,...)
{
	va_list args;
	va_start(args, fmt);
	char buf[1024];
	memset(&buf,'\0', sizeof(char) * 1024);
	vsprintf(buf,fmt, args);
	va_end(args);
	
	//snd_strerror(err), err,
	std::string message = format("%s:%i:%s:%s",file,line,function,  buf);
		
	log("alsa-lib",1,message);
}

void alsa_wrapper::setLogger(std::function<void(std::string, int, std::string)> logger)
{
	_logger = logger;
}


#define ARRAY_SIZE(a) (sizeof(a) / sizeof *(a))

static const snd_pcm_access_t accesses[] = {
	SND_PCM_ACCESS_MMAP_INTERLEAVED,
	SND_PCM_ACCESS_MMAP_NONINTERLEAVED,
	SND_PCM_ACCESS_MMAP_COMPLEX,
	SND_PCM_ACCESS_RW_INTERLEAVED,
	SND_PCM_ACCESS_RW_NONINTERLEAVED,
};

struct fmt_map {
	enum ffmpegcpp::sample_format ff_fmt;
	std::string codec_id;
	uint32_t alsa_fmt;
};

const struct fmt_map alsa_fmt_conversion_table[] = {
	//ff_fmt						codec_id	bits alsa_fmt
	{ ffmpegcpp::sample_format::u8, "PCM_S8", SND_PCM_FORMAT_S8},
	{ ffmpegcpp::sample_format::u8, "PCM_U8", SND_PCM_FORMAT_U8 },
	{ ffmpegcpp::sample_format::s16, "PCM_S16LE", SND_PCM_FORMAT_S16_LE },
	{ ffmpegcpp::sample_format::s16, "PCM_S16BE", SND_PCM_FORMAT_S16_BE },
	{ ffmpegcpp::sample_format::s16, "PCM_U16LE", SND_PCM_FORMAT_U16_LE },
	{ ffmpegcpp::sample_format::s16, "PCM_U16BE", SND_PCM_FORMAT_U16_BE },
	//{ ffmpegcpp::sample_format::s24, "", SND_PCM_FORMAT_S24_LE },
	//{ ffmpegcpp::sample_format::s24, "", SND_PCM_FORMAT_S24_BE },
	//{ ffmpegcpp::sample_format::s24, "", SND_PCM_FORMAT_U24_LE },
	//{ ffmpegcpp::sample_format::s24, "", SND_PCM_FORMAT_U24_BE },
	{ ffmpegcpp::sample_format::s32, "PCM_S32LE", SND_PCM_FORMAT_S32_LE },
	{ ffmpegcpp::sample_format::s32, "PCM_S32BE", SND_PCM_FORMAT_S32_BE },
	{ ffmpegcpp::sample_format::s32, "PCM_U32LE", SND_PCM_FORMAT_U32_LE },
	{ ffmpegcpp::sample_format::s32, "PCM_U32BE", SND_PCM_FORMAT_U32_BE },
	{ ffmpegcpp::sample_format::flt, "PCM_F32LE", SND_PCM_FORMAT_FLOAT_LE },
	{ ffmpegcpp::sample_format::flt, "PCM_F32BE", SND_PCM_FORMAT_FLOAT_BE },
	{ ffmpegcpp::sample_format::dbl, "PCM_F64LE", SND_PCM_FORMAT_FLOAT64_LE },
	{ ffmpegcpp::sample_format::dbl, "PCM_F64BE", SND_PCM_FORMAT_FLOAT64_BE },
	//{ ffmpegcpp::sample_format:, "", SND_PCM_FORMAT_IEC958_SUBFRAME_LE },
	//{ ffmpegcpp::sample_format:, "", SND_PCM_FORMAT_IEC958_SUBFRAME_BE },
	{ ffmpegcpp::sample_format::none, "PCM_MULAW", SND_PCM_FORMAT_MU_LAW },
	{ ffmpegcpp::sample_format::none, "PCM_ALAW", SND_PCM_FORMAT_A_LAW },
	{ ffmpegcpp::sample_format::none, "IMA_ADPCM", SND_PCM_FORMAT_IMA_ADPCM },
	{ ffmpegcpp::sample_format::none, "MPEG", SND_PCM_FORMAT_MPEG },
	{ ffmpegcpp::sample_format::none, "GSM", SND_PCM_FORMAT_GSM },
	{ ffmpegcpp::sample_format::none, "SPECIAL", SND_PCM_FORMAT_SPECIAL }
	//{ ffmpegcpp::sample_format::s24, "", SND_PCM_FORMAT_S24_3LE },
	//{ ffmpegcpp::sample_format::s24, "", SND_PCM_FORMAT_S24_3BE },
	//{ ffmpegcpp::sample_format::s24, "", SND_PCM_FORMAT_U24_3LE },
	//{ ffmpegcpp::sample_format::s24, "", SND_PCM_FORMAT_U24_3BE },
	//{ ffmpegcpp::sample_format::s20, "", SND_PCM_FORMAT_S20_3LE },
	//{ ffmpegcpp::sample_format::s20, "", SND_PCM_FORMAT_S20_3BE },
	//{ ffmpegcpp::sample_format::s20, "", SND_PCM_FORMAT_U20_3LE },
	//{ ffmpegcpp::sample_format::s20, "", SND_PCM_FORMAT_U20_3BE },
	//{ ffmpegcpp::sample_format::s18, "", SND_PCM_FORMAT_S18_3LE },
	//{ ffmpegcpp::sample_format::s18, "", SND_PCM_FORMAT_S18_3BE },
	//{ ffmpegcpp::sample_format::s18, "", SND_PCM_FORMAT_U18_3LE },
	//{ ffmpegcpp::sample_format::s18, "", SND_PCM_FORMAT_U18_3BE },

};

static const snd_pcm_format_t formats[] = {
	SND_PCM_FORMAT_S8,
	SND_PCM_FORMAT_U8,
	SND_PCM_FORMAT_S16_LE,
	SND_PCM_FORMAT_S16_BE,
	SND_PCM_FORMAT_U16_LE,
	SND_PCM_FORMAT_U16_BE,
	SND_PCM_FORMAT_S24_LE,
	SND_PCM_FORMAT_S24_BE,
	SND_PCM_FORMAT_U24_LE,
	SND_PCM_FORMAT_U24_BE,
	SND_PCM_FORMAT_S32_LE,
	SND_PCM_FORMAT_S32_BE,
	SND_PCM_FORMAT_U32_LE,
	SND_PCM_FORMAT_U32_BE,
	SND_PCM_FORMAT_FLOAT_LE,
	SND_PCM_FORMAT_FLOAT_BE,
	SND_PCM_FORMAT_FLOAT64_LE,
	SND_PCM_FORMAT_FLOAT64_BE,
	SND_PCM_FORMAT_IEC958_SUBFRAME_LE,
	SND_PCM_FORMAT_IEC958_SUBFRAME_BE,
	SND_PCM_FORMAT_MU_LAW,
	SND_PCM_FORMAT_A_LAW,
	SND_PCM_FORMAT_IMA_ADPCM,
	SND_PCM_FORMAT_MPEG,
	SND_PCM_FORMAT_GSM,
	SND_PCM_FORMAT_SPECIAL,
	SND_PCM_FORMAT_S24_3LE,
	SND_PCM_FORMAT_S24_3BE,
	SND_PCM_FORMAT_U24_3LE,
	SND_PCM_FORMAT_U24_3BE,
	SND_PCM_FORMAT_S20_3LE,
	SND_PCM_FORMAT_S20_3BE,
	SND_PCM_FORMAT_U20_3LE,
	SND_PCM_FORMAT_U20_3BE,
	SND_PCM_FORMAT_S18_3LE,
	SND_PCM_FORMAT_S18_3BE,
	SND_PCM_FORMAT_U18_3LE,
	SND_PCM_FORMAT_U18_3BE,
};

static const unsigned int rates[] = {
	5512,
	8000,
	11025,
	16000,
	22050,
	32000,
	44100,
	48000,
	64000,
	88200,
	96000,
	176400,
	192000,
};

std::vector<std::shared_ptr<ffmpegcpp::deviceinfo>> alsa_wrapper::getDeviceInfos(std::shared_ptr<ffmpegcpp::device> dev)
{
	std::vector<std::shared_ptr<ffmpegcpp::deviceinfo>> devinfos;

	const char *device_name = dev->name.c_str(); //hw:0
	snd_pcm_t *pcm;
	snd_pcm_hw_params_t *hw_params;
	unsigned int i;
	unsigned int min, max;
	int any_rate;
	int err;

	int minChannels;
	int maxChannels;
	int minRate;
	int maxRate;
	std::vector<fmt_map> supportedformats;


	

	err = snd_pcm_open(&pcm, device_name, SND_PCM_STREAM_CAPTURE, SND_PCM_NONBLOCK);
	if (err < 0) {
		log("alsa_wrapper",1, format("cannot open device '%s' as capture: %s", device_name, snd_strerror(err)));
		return devinfos;
	}

	snd_pcm_hw_params_alloca(&hw_params);
	err = snd_pcm_hw_params_any(pcm, hw_params);
	if (err < 0) {
		log("alsa_wrapper", 1, format("cannot get hardware parameters: %s", snd_strerror(err)));
		snd_pcm_close(pcm);
		return devinfos;
	}

	log("alsa_wrapper", 32, format("Device: %s (type: %s)\n", device_name, snd_pcm_type_name(snd_pcm_type(pcm))));

	log("alsa_wrapper", 32, "Access types:");
	for (i = 0; i < ARRAY_SIZE(accesses); ++i) {
		if (!snd_pcm_hw_params_test_access(pcm, hw_params, accesses[i]))
			log("alsa_wrapper", 32, format(" %s", snd_pcm_access_name(accesses[i])));
	}

	

	log("alsa_wrapper", 32, "Formats:");
	for (i = 0; i < ARRAY_SIZE(formats); ++i) {
		if (!snd_pcm_hw_params_test_format(pcm, hw_params, formats[i]))
		{
			log("alsa_wrapper", 32, format(" %s", snd_pcm_format_name(formats[i])));

			for (auto sfmt : alsa_fmt_conversion_table)
			{
				if (sfmt.alsa_fmt == formats[i]){
					
					supportedformats.push_back(sfmt);
				}
			}

		}
	}
	

	err = snd_pcm_hw_params_get_channels_min(hw_params, &min);
	if (err < 0) {
		log("alsa_wrapper", 1, format("cannot get minimum channels count: %s", snd_strerror(err)));
		snd_pcm_close(pcm);
		return devinfos;
	}

	minChannels = min;

	

	err = snd_pcm_hw_params_get_channels_max(hw_params, &max);
	if (err < 0) {
		log("alsa_wrapper", 1, format("cannot get maximum channels count: %s", snd_strerror(err)));
		snd_pcm_close(pcm);
		return devinfos;
	}

	maxChannels = max;
	
	
	log("alsa_wrapper", 32, "Channels:");
	for (i = min; i <= max; ++i) {
		if (!snd_pcm_hw_params_test_channels(pcm, hw_params, i))
			log("alsa_wrapper", 32, format(" %u", i));
	}
	

	err = snd_pcm_hw_params_get_rate_min(hw_params, &min, NULL);
	if (err < 0) {
		log("alsa_wrapper", 1, format("cannot get minimum rate: %s\n", snd_strerror(err)));
		snd_pcm_close(pcm);
		return devinfos;
	}
	
	minRate = min;
	

	err = snd_pcm_hw_params_get_rate_max(hw_params, &max, NULL);
	if (err < 0) {
		log("alsa_wrapper", 1, format("cannot get maximum rate: %s\n", snd_strerror(err)));
		snd_pcm_close(pcm);
		return devinfos;
	}

	maxRate = max;

	

	log("alsa_wrapper", 32, "Sample rates:");
	if (min == max)
		log("alsa_wrapper", 32, format(" %u", min));
	else if (!snd_pcm_hw_params_test_rate(pcm, hw_params, min + 1, 0))
		log("alsa_wrapper", 32, format(" %u-%u", min, max));
	else {
		any_rate = 0;
		for (i = 0; i < ARRAY_SIZE(rates); ++i) {
			if (!snd_pcm_hw_params_test_rate(pcm, hw_params, rates[i], 0)) {
				any_rate = 1;
				log("alsa_wrapper", 32, format(" %u", rates[i]));
			}
		}
		if (!any_rate)
			log("alsa_wrapper", 32, format(" %u-%u", min, max));
	}
	

	err = snd_pcm_hw_params_get_period_time_min(hw_params, &min, NULL);
	if (err < 0) {
		log("alsa_wrapper", 1, format("cannot get minimum period time: %s\n", snd_strerror(err)));
		snd_pcm_close(pcm);
		return devinfos;
	}
	err = snd_pcm_hw_params_get_period_time_max(hw_params, &max, NULL);
	if (err < 0) {
		log("alsa_wrapper", 1, format("cannot get maximum period time: %s\n", snd_strerror(err)));
		snd_pcm_close(pcm);
		return devinfos;
	}
	
	log("alsa_wrapper", 32, format("Interrupt interval: %u-%u us\n", min, max));

	err = snd_pcm_hw_params_get_buffer_time_min(hw_params, &min, NULL);
	if (err < 0) {
		log("alsa_wrapper", 1, format("cannot get minimum buffer time: %s\n", snd_strerror(err)));
		snd_pcm_close(pcm);
		return devinfos;
	}
	err = snd_pcm_hw_params_get_buffer_time_max(hw_params, &max, NULL);
	if (err < 0) {
		log("alsa_wrapper", 1, format("cannot get maximum buffer time: %s\n", snd_strerror(err)));
		snd_pcm_close(pcm);
		return devinfos;
	}
	log("alsa_wrapper", 32, format("Buffer size: %u-%u us\n", min, max));

	snd_pcm_close(pcm);

	
	
	for (auto fmt : supportedformats){
		std::shared_ptr<ffmpegcpp::deviceinfoaudio> dia = std::make_shared<ffmpegcpp::deviceinfoaudio>();
		int bits = snd_pcm_format_width((snd_pcm_format_t)fmt.alsa_fmt);
		dia->minBits = bits;
		dia->maxBits = bits;
		
		dia->minChannels = minChannels;
		dia->maxChannels = maxChannels;
		dia->minRate = minRate;
		dia->maxRate = maxRate;

		log("alsa_wrapper", 32, "pushing dia");
		devinfos.push_back(dia);
	}

	log("alsa_wrapper", 32, "returning devinfos");
	return devinfos;
}

std::vector<std::shared_ptr<ffmpegcpp::device>> alsa_wrapper::listDevices()
{
	//set alsa error handler
	snd_lib_error_set_handler(alsa_error_handler);
	
	
	std::vector<std::shared_ptr<ffmpegcpp::device>> devlist;
	int rc;

	snd_ctl_card_info_t *info;

	snd_ctl_t *handle;

	int card = -1;

	snd_ctl_card_info_alloca(&info);

	for (rc = snd_card_next(&card); !rc && (card >=0);
		rc = snd_card_next(&card)) 
	{
		char name[32];
		sprintf((char*)&name, "hw:%i", card);
		//std::cout << "card: " << name << std::endl;
		int err;

		if ((err = snd_ctl_open(&handle, name, 0)) < 0) {
			log("alsa_wrapper",1, format("cannot open card '%s': %s (snd_ctl_open)", &name, snd_strerror(err)));
			continue;
			//throw alsa_exception(err, "snd_ctl_open");
		}
		if ((err = snd_ctl_card_info(handle, info)) < 0) {
			snd_ctl_close(handle);
			log("alsa_wrapper",1, format("cannot get card info '%s': %s (snd_ctl_card_info)", &name, snd_strerror(err)));
			continue;
			//throw alsa_exception(err, "snd_ctl_card_info");
		}

		std::shared_ptr<ffmpegcpp::device> dev = std::make_shared<ffmpegcpp::device>();
		dev->name = name;
		dev->full_name = snd_ctl_card_info_get_longname(info);
		dev->format = "alsa";
		dev->type = ffmpegcpp::devicetype::audio;
		
		log("alsa_wrapper", 32, "replacing devInfos");
		
		dev->deviceInfos = getDeviceInfos(dev);

		log("alsa_wrapper", 32, "pushing dev");
		
		devlist.push_back(dev);
		
		//cardinfo ci;
		//ci.id = snd_ctl_card_info_get_card(info);
		//ci.driver = snd_ctl_card_info_get_driver(info);
		//ci.name = snd_ctl_card_info_get_name(info);
		//ci.longname = snd_ctl_card_info_get_longname(info);
		//ci.mixername = snd_ctl_card_info_get_mixername(info);
		//ci.components = snd_ctl_card_info_get_components(info);

		snd_ctl_close(handle);
		
		log("alsa_wrapper", 32, "snd_ctl_close");
	}



	return devlist;
}


