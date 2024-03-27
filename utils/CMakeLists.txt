# Load Cairo library
find_package(PkgConfig REQUIRED)
pkg_check_modules(CAIRO REQUIRED cairo)
include_directories(${CAIRO_INCLUDE_DIRS})

# Define utility libraries
add_library(utils_timing timing.c)
add_library(utils_graph graph.c)
add_library(utils_parameter parameter.c)