cmake_minimum_required(VERSION 3.1)
project(danmaku2ass_native)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    danmaku2ass.cpp
    danmaku2ass.h)

add_executable(danmaku2ass_native ${SOURCE_FILES})