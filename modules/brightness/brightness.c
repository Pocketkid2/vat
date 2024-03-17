#include <stdio.h>

#include "brightness.h"

int brightness_function(int argc, char **argv);

module_t brightness_module = {
    .long_name = "brightness",
    .short_name = "br",
    .description = "HDR brightness analysis.",
    .function = brightness_function};

int brightness_function(int argc, char **argv)
{
    printf("Brightness function called\n");
}