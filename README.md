# Danmaku2ass native

[![Build Status](https://travis-ci.org/typcn/danmaku2ass_native.svg?branch=master)](https://travis-ci.org/typcn/danmaku2ass_native)
[![Coverage Status](https://coveralls.io/repos/typcn/danmaku2ass_native/badge.svg)](https://coveralls.io/r/typcn/danmaku2ass_native)

# Features

Convrt comments to ASS subtitle，you can play it with most of media players.

Written in C ++, convert ten thousand comments just 0.05 seconds, not depend on any third-party libraries, easily to embed.


# Supported Website

Only support bilibili format , other websites will be added in soon.

# Screenshot

img src="http://blog-cdn.eqoe.cn/images/1428559449093.png!mini">

# Complie

CMake 2.6 or higher and C++ 11 compiler required.

    git clone https://github.com/typcn/danmaku2ass_native.git
    mkdir Build
    cd Build
    cmake ..
    make

# Usage

## Command Line

    ./danmaku2ass -in=弹幕文件 -out=输出文件 -w=视频宽度 -h=视频高度 -font="字体名称" -fontsize=字体大小 -alpha=透明度(0-1之间) -dm=滚动弹幕持续时间 -ds=渐隐弹幕持续时间

## Use in software

Complie as library or Add all files to project，and delete main function in danmaku2ass.cpp

### API

    #include "danmaku2ass.h"
    
    void danmaku2ass(const char *infile,const char *outfile,int width,int height,const char *font,float fontsize,float alpha,float duration_marquee,float duration_still);

* infile - Input file
* outfile - Output file
* width - Video width
* height - Video height
* font - Font name
* fontsize - Font Size
* alpha - Transparency(0-1)
* duration_marquee - Scroll comment display time
* duration_still - Still comment display time

# Render comment into video

You can use ffmpeg to do that.

Example usage:

ffmpeg -i xxx.flv -vf ass=ASS文件 -vcodec libx264 -acodec copy xxx_cm.flv

# About

Thanks for https://github.com/m13253/danmaku2ass

License under GPLv2