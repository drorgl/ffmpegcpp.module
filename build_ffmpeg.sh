#!/bin/bash

sudo -E add-apt-repository ppa:kalakris/cmake -y
sudo -E apt-get -yq update

#libsdl2-dev

sudo -E apt-get -yq --no-install-suggests --no-install-recommends --force-yes install autoconf automake build-essential libass-dev libfreetype6-dev \
   libtheora-dev libtool libva-dev libvdpau-dev libvorbis-dev libxcb1-dev libxcb-shm0-dev \
  libxcb-xfixes0-dev pkg-config texinfo zlib1g-dev

mkdir ~/ffmpeg_sources


cd ~/ffmpeg_sources
wget http://www.tortall.net/projects/yasm/releases/yasm-1.3.0.tar.gz
tar xzvf yasm-1.3.0.tar.gz
cd yasm-1.3.0
./configure 
make -j 5
sudo make install


cd ~/ffmpeg_sources
wget http://download.videolan.org/pub/x264/snapshots/last_x264.tar.bz2
tar xjvf last_x264.tar.bz2
cd x264-snapshot*
./configure  --enable-static --disable-opencl
make -j 5
sudo make install


sudo -E apt-get -yq --no-install-suggests --no-install-recommends --force-yes install cmake mercurial

cd ~/ffmpeg_sources
hg clone https://bitbucket.org/multicoreware/x265
cd ~/ffmpeg_sources/x265/build/linux
cmake -G "Unix Makefiles"  -DENABLE_SHARED:bool=off ../../source
make -j 5
sudo make install



sudo -E apt-get -yq --no-install-suggests --no-install-recommends --force-yes install dh-autoreconf
cd ~/ffmpeg_sources
wget -O fdk-aac.tar.gz https://github.com/mstorsjo/fdk-aac/tarball/master
tar xzvf fdk-aac.tar.gz
cd mstorsjo-fdk-aac*
autoreconf -fiv
./configure --disable-shared
make -j 5
sudo make install


sudo -E apt-get -yq --no-install-suggests --no-install-recommends --force-yes install nasm
cd ~/ffmpeg_sources
wget --no-check-certificate http://downloads.sourceforge.net/project/lame/lame/3.99/lame-3.99.5.tar.gz
tar xzvf lame-3.99.5.tar.gz
cd lame-3.99.5
./configure --enable-nasm --disable-shared
make -j 5
sudo make install


cd ~/ffmpeg_sources
wget http://downloads.xiph.org/releases/opus/opus-1.1.4.tar.gz
tar xzvf opus-1.1.4.tar.gz
cd opus-1.1.4
./configure --disable-shared
make -j 5
sudo make install



cd ~/ffmpeg_sources
wget http://storage.googleapis.com/downloads.webmproject.org/releases/webm/libvpx-1.6.1.tar.bz2
tar xjvf libvpx-1.6.1.tar.bz2
cd libvpx-1.6.1
./configure --disable-examples --disable-unit-tests
make -j 5
sudo make install

sudo -E apt-get -yq --no-install-suggests --no-install-recommends --force-yes install libfreetype6-dev libfribidi-dev
cd ~/ffmpeg_sources
wget https://github.com/libass/libass/releases/download/0.13.6/libass-0.13.6.tar.gz
tar -zxvf libass-0.13.6.tar.gz
cd libass-0.13.6
./configure --disable-require-system-font-provider
make -j 5
sudo make install

git clone --depth=1 https://github.com/mstorsjo/fdk-aac.git
cd fdk-aac
./autogen.sh
./configure  --enable-shared --enable-static
make -j 5
sudo make install


cd ~/ffmpeg_sources
wget http://downloads.xiph.org/releases/ogg/libogg-1.3.2.tar.gz
tar xzvf libogg-1.3.2.tar.gz
cd libogg-1.3.2
./configure --disable-shared
make -j 5
sudo make install



cd ~/ffmpeg_sources
wget http://downloads.xiph.org/releases/theora/libtheora-1.1.1.tar.bz2
tar xjvf libtheora-1.1.1.tar.bz2
cd libtheora-1.1.1
./configure
make -j 5
sudo make install

sudo -E apt-get -yq --no-install-suggests --no-install-recommends --force-yes install libvorbis-dev

cd ~/ffmpeg_sources
wget http://ffmpeg.org/releases/ffmpeg-snapshot.tar.bz2
tar xjvf ffmpeg-snapshot.tar.bz2
cd ffmpeg
./configure \
  --enable-gpl \
  --enable-libass \
  --enable-libfdk-aac \
  --enable-libfreetype \
  --enable-libmp3lame \
  --enable-libopus \
  --enable-libtheora \
  --enable-libvorbis \
  --enable-libvpx \
  --enable-libx264 \
  --enable-libx265 \
  --enable-nonfree
make -j 5
sudo make install
hash -r