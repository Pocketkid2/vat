cmake_minimum_required(VERSION 3.10)
project(VideoAnalysisTool C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

# Load ffmpeg libraries
find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
find_library(AVCODEC_LIBRARY avcodec)
find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
find_library(AVFORMAT_LIBRARY avformat)
find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
find_library(AVUTIL_LIBRARY avutil)
include_directories(${AVCODEC_INCLUDE_DIR} ${AVFORMAT_INCLUDE_DIR} ${AVUTIL_INCLUDE_DIR})

# Add modules
include_directories(include modules)
add_subdirectory(modules/bitrate)

# Define executable
add_executable(vat main.c)
target_link_libraries(vat bitrate_module ${AVCODEC_LIBRARY} ${AVFORMAT_LIBRARY} ${AVUTIL_LIBRARY} m)