#ifndef _V4L2_WRAPPER_
#define _V4L2_WRAPPER_

#include <memory>
#include "exports.h"
#include "datastructures.h"
#include "../../stringutils.h"

class v4l2_exception : public std::runtime_error
{
	std::string getstrerror(int errnum);
	int _errornumber;
public:
	v4l2_exception(std::string message)
		: std::runtime_error(message)
	{}

	v4l2_exception(int errnum)
		: std::runtime_error(getstrerror(errnum))
	{
		_errornumber = errnum;
	}

	v4l2_exception(int errnum, std::string message)
		: std::runtime_error(getstrerror(errnum) + " " + message)
	{
		_errornumber = errnum;
	}
};

class v4l2_wrapper
{
public:
	v4l2_wrapper(const v4l2_wrapper& that) = delete;
	v4l2_wrapper & operator=(const v4l2_wrapper&) = delete;

	static std::vector<std::shared_ptr<ffmpegcpp::device>> listDevices();
};

#endif