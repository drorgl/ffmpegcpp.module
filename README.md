# ffmpegcpp GYP Module

### **Experimental **

ffmpeg c++ wrapper, designed to be an object oriented wrapper of the popular ffmpeg.

beginner ffmpeg users often struggle, when should each structure initialized, which function should initialize it, which property contains the information you need, etc', while all this exists in either the documentation or in the source code, this wrapper was designed to simplify it. 

```
#include "ffmpeg.h"
```

#### compiling with gyp
```
compiling for win32:
gyp ffmpegcpp.gyp -Dtarget_arch=ia32 -Duse_system_yasm=0 -Dos_posix=0 -Dmsan=0 -Dclang=0 -Dbuildtype=Official --depth=. -f msvs -G msvs_version=2013 --no-duplicate-basename-check --generator-output=./build.vs2013/

compiling for win64:
gyp ffmpegcpp.gyp -Dtarget_arch=x64 --depth=. -f msvs -G msvs_version=2013 --generator-output=./build.vs2013/

compiling for linux32:
gyp ffmpegcpp.gyp -Dtarget_arch=ia32 -DOS=linux --depth=. -f make --generator-output=./build.make/

compiling for linux64:
gyp ffmpegcpp.gyp -Dtarget_arch=x64 -DOS=linux -Duse_system_yasm=1 -Dos_posix=1 -Dmsan=0 --depth=. -f make --generator-output=./build.linux64/

compiling for android/arm:
gyp ffmpegcpp.gyp -Dtarget_arch=arm -DOS=android --depth=. -f make --generator-output=./build.make/

compiling for linux/android with ninja:
replace -f make with -f ninja

```

#### Dependencies

##### ffmpeg.module
>
>[https://github.com/drorgl/ffmpeg.module](https://github.com/drorgl/ffmpeg.module)
>

##### opencv.module
>
>[https://github.com/drorgl/opencv.module](https://github.com/drorgl/opencv.module)
>


### ffmpegcpp license
ffmpegcpp is copyrighted by Dror Gluska and licensed under the 3-clause BSD license, however, its dependent upon multiple software projects with various licenses, hire a lawyer if you have any questions.

#### Tests
Tests are inside the ffmpegcpp.sample project, they use catch ([https://github.com/philsquared/Catch](https://github.com/philsquared/Catch)) and are a mix of unit and integration tests, they perform various sanity tests to make sure ffmpegcpp and its dependent libraries are working properly, if your compiled ffmpegcpp.sample is showing warnings or errors, you should be aware that your configuration is unstable and its best to find the root cause.

#### Developer guide

ffmpegcpp is a wrapper project for ffmpeg, it attempts to simplify how ffmpeg is really working and hide the api which can be a bit cryptc for the first time, for example the distinction between a frame and a packet is not always clear.

###### ffmpegcpp contains the following classes:

**ffmpeg** - which is the main class, it contains functions for opening inputs and outputs, listing input devices, finding out how many ffmpeg instances are currently allocated and setting a static logger for the entire project (there might be a way to separate logs for each session (or context as they are called by ffmpeg), but I have yet to find a way to completely separate them.

When opening an input or an output, a **formatcontext** is returned.

**formatcontext** - a format context is ffmpeg's session object, this specifies the format of the input/output, or container, for example, mkv, mov, avi, it contains the list of **stream**s that the file holds, for example, it can contain h264, aac and other various audio/video and subtitle streams.
formatcontext is expected to read or write a **packet**, which is directly related to a specific stream id.
you can get a list of input formats and output formats from formatcontext.

For writing memory only formatcontexts, you can use the **memorycontext** class and the overloaded open_output function.

A **packet** is the format context actual data, a packet is associated with a stream id and contains a bulk of bytes in the file, a packet can be reused. 

When first opening an input formatcontext, you can get a list of its streams so you will know which stream that packet belongs to and before actually writing anything to an output formatcontext, you should add streams to that formatcontext, so the appropriate header will be written.

Output streams should have their settings set before writing the first packet, that includes a codec name, dimensions, pixel format, sampling rates, channels, etc'.

When you read a packet from formatcontext, that packet needs to be decoded into a a frame or an audio buffer, this is done with the **decoder** and the same applies for writing an output formatcontext, which is done with the **encoder**.

The **decoder** is accepting an input packet and sometimes producing an output frame, since not all data needed to produce a whole frame might be in a single packet, multiple packets can be passed to the decoder and it will return true when an entire frame could be decoded.

The **encoder** operates similarly by encoding multiple frames into packets, sometimes producing one packet, sometimes producing none, a **flush** is provided so you may force the encoder to produce a packet, but it should be done only at the end of the encoding session to flush out last packets of data.

Once you have a **frame**, you can get an **opencv mat** from it by using **swscale**, which will do the conversion for you. the frame's internals are not currently exposed.

**swscale** accepts the source/destination mat settings you created **cv::mat** with, for audio mats its important to note that **planar formats** uses row for each channel while normal format uses the channel (or color) for each channel.

Sample usage is in the tests, so feel free to explore.

Last class to note is the **bitstreamfilter**, which can be applied on either an input stream or an output stream.

ffmpegcpp build is based on gyp, but if you modify it, you can use prebuilt libraries, in the future pkg-config will be an option.



