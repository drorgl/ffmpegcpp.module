#!/bin/bash

sudo apt-get update

sudo apt-get --force-yes remove opencv

sudo apt-get install build-essential
sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev

wget https://github.com/opencv/opencv/archive/3.2.0.zip -O opencv3.2.0.zip
#wget https://github.com/opencv/opencv_contrib/archive/3.2.0.zip -O opencv_contrib3.2.0.zip
#wget https://github.com/opencv/opencv_extra/archive/3.2.0.zip -O opencv_extra3.2.0.zip

unzip opencv3.2.0.zip
#unzip opencv_contrib3.2.0.zip
#unzip opencv_extra3.2.0.zip

cd opencv-3.2.0
mkdir build
cd build

cmake -D CMAKE_BUILD_TYPE=Release ..

make -j 5

sudo make install
sudo ldconfig

export OPENCV_TEST_DATA_PATH=../../opencv_extra-3.2.0/testdata 

./bin/opencv_test_core
