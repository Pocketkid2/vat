cmake_minimum_required(VERSION 3.10)
project(VideoAnalysisTool C)
set(CMAKE_C_STANDARD 11)

add_subdirectory(analyze_color_space)
add_subdirectory(analyze_picture_brightness)
add_subdirectory(analyze_audio_track)

add_executable(vat main.c)
target_link_libraries(vat analyze_color_space analyze_picture_brightness analyze_audio_track)