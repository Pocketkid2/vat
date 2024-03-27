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
find_path(SWSCALE_INCLUDE_DIR libswscale/swscale.h)
find_library(SWSCALE_LIBRARY swscale)
include_directories(${AVCODEC_INCLUDE_DIR} ${AVFORMAT_INCLUDE_DIR} ${AVUTIL_INCLUDE_DIR} ${SWSCALE_INCLUDE_DIR})

# Load Cairo library
find_package(PkgConfig REQUIRED)
pkg_check_modules(CAIRO REQUIRED cairo)
include_directories(${CAIRO_INCLUDE_DIRS})

# Add modules
include_directories(include modules)
add_subdirectory(modules/bitrate)
add_subdirectory(modules/brightness)
add_subdirectory(modules/colorspace)

# Add utils
add_subdirectory(utils)

# Define executable
add_executable(vat main.c)
target_link_libraries(vat 

    module_bitrate 
    module_brightness
    module_colorspace
    
    utils_timing 
    utils_graph
    utils_parameter
    
    ${CAIRO_LIBRARIES} 
    ${AVCODEC_LIBRARY} 
    ${AVFORMAT_LIBRARY} 
    ${AVUTIL_LIBRARY}
    ${SWSCALE_LIBRARY}
    
    m)

# Define tests
add_executable(test_graph tests/test_graph.c)
target_link_libraries(test_graph utils_graph m ${CAIRO_LIBRARIES})