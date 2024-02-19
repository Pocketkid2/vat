#include "analyze_picture_brightness.h"

#include <stdio.h>

int analyze_picture_brightness(int argc, char **argv);

module_t analyze_picture_brightness_module = {
    .long_name = "analyze_picture_brightness",
    .short_name = "apb",
    .function = analyze_picture_brightness
};

int analyze_picture_brightness(int argc, char **argv) {
    printf("analyze_picture_brightness\n");
    return 0;
}