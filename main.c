#include <stdio.h>
#include <string.h>

#include "bitrate/bitrate.h"
#include "brightness/brightness.h"
#include "colorspace/colorspace.h"

#include "timing.h"

module_t *modules[] = {
        &bitrate_module,
        &brightness_module,
        &colorspace_module
};

int main(int argc, char **argv) {
    if (argc <= 1) {
        printf("Usage: %s <module> <args>\n", argv[0]);
        printf("Available modules:\n");
        for (int i = 0; i < sizeof(modules) / sizeof(module_t *); i++) {
            printf("\t[%s] %s\t- %s\n", modules[i]->short_name, modules[i]->long_name, modules[i]->description);
        }
        return 1;
    }
    for (int i = 0; i < sizeof(modules) / sizeof(module_t *); i++) {
        if (strcmp(modules[i]->short_name, argv[1]) == 0 || strcmp(modules[i]->long_name, argv[1]) == 0) {
            stopwatch_t sw;
            stopwatch_start(&sw);
            int status = modules[i]->function(argc, argv);
            stopwatch_stop(&sw);
            stopwatch_print(&sw);
            return status;
        }
    }
}