#include <stdio.h>

#include "colorspace.h"

int colorspace_function(int argc, char **argv);

module_t colorspace_module = {
    .long_name = "colorspace",
    .short_name = "cs",
    .description = "WCG colorspace analysis.",
    .function = colorspace_function};

int colorspace_function(int argc, char **argv)
{
    printf("Colorspace function called\n");
}