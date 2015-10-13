#ifndef _ALSA_WRAPPER_
#define _ALSA_WRAPPER_

#include <vector>
#include <string>

#include <functional>

#include <memory>
#include "exports.h"
#include "datastructures.h"
#include "../../stringutils.h"

//struct cardinfo
//{
//	/* card number */
//	int card;
//
//	/* ID of card (user selectable) */
//	std::string id;
//
//	/* Driver name */
//	std::string driver;	
//
//	/* Short name of soundcard */
//	std::string name;	
//
//	/* name + info text about soundcard */
//	std::string longname;
//
//	/* visual mixer identification */
//	std::string mixername;	
//
//	/* card components / fine identification, delimited with one space (AC97 etc..) */
//	std::string components;	
//};

class alsa_exception : public std::runtime_error
{
	std::string getsnd_strerror(int errnum);
	int _alsaerrornumber;
public:
	alsa_exception(std::string message)
		: std::runtime_error(message)
	{}

	alsa_exception(int alsaerrnum)
		: std::runtime_error(getsnd_strerror(alsaerrnum))
	{
		_alsaerrornumber = alsaerrnum;
	}

	alsa_exception(int alsaerrnum, std::string message)
		: std::runtime_error(getsnd_strerror(alsaerrnum) + " " + message)
	{
		_alsaerrornumber = alsaerrnum;
	}
};

class alsa_wrapper
{
private:
	static std::function<void(std::string, int, std::string)> _logger;

	static void log(std::string module, int level, std::string message);
	
	static void alsa_error_handler (const char *file,int line,const char *function,int err,const char *fmt,...);

	static std::vector<std::shared_ptr<ffmpegcpp::deviceinfo>> getDeviceInfos(std::shared_ptr<ffmpegcpp::device> dev);
public:
	alsa_wrapper(const alsa_wrapper& that) = delete;
	alsa_wrapper& operator=(const alsa_wrapper&) = delete;
	
	static void setLogger(std::function<void(std::string, int, std::string)> logger);

	//static std::vector<cardinfo> getCards();

	//static void getCapabilities(cardinfo cardInfo);

	static std::vector<std::shared_ptr<ffmpegcpp::device>> listDevices();

};

#endif