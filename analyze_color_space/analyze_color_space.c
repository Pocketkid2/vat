#include "analyze_color_space.h"

int analyze_color_space(int argc, char **argv);

module_t analyze_color_space_module = {
    .long_name = "analyze_color_space",
    .short_name = "acs",
    .function = analyze_color_space
};

int analyze_color_space(int argc, char **argv) {
    printf("analyze_color_space\n");
    return 0;
}