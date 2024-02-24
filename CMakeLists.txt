cmake_minimum_required(VERSION 3.10)
project(VideoAnalysisTool C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

include_directories(include modules)

add_subdirectory(modules/bitrate)

add_executable(vat main.c)
target_link_libraries(vat bitrate_module)