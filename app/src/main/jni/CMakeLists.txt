cmake_minimum_required(VERSION 3.4.1)

set(
	PATH_TO_SUPERPOWERED
	CACHE STRING ""
)

message(${ANDROID_ABI})

file(GLOB CPP_FILES "*.cpp")

add_library(
	SuperpoweredExample
	SHARED
	${CPP_FILES}
	${CMAKE_SOURCE_DIR}/Superpowered/AndroidIO/SuperpoweredAndroidAudioIO.cpp
	${CMAKE_SOURCE_DIR}/SuperpoweredExample.cpp
	${CMAKE_SOURCE_DIR}/AudioPlayer.cpp
)

include_directories(src/main/jni)
include_directories({CMAKE_SOURCE_DIR}/Superpowered)

target_link_libraries(
    SuperpoweredExample
    log
    android
    OpenSLES
    ${CMAKE_SOURCE_DIR}/Superpowered/libSuperpoweredAndroid${ANDROID_ABI}.a
)