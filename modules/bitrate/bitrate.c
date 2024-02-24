#include "bitrate.h"

#include <stdio.h>

int bitrate_function(int argc, char **argv);

module_t bitrate_module = {
    .long_name = "bitrate",
    .short_name = "bt",
    .function = bitrate_function
};

int bitrate_function(int argc, char **argv) {
    printf("Bitrate module called\n");
    return 0;
}
