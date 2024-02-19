#include <stdio.h>
#include <string.h>

#include "analyze_audio_track/analyze_audio_track.h"
#include "analyze_color_space/analyze_color_space.h"
#include "analyze_picture_brightness/analyze_picture_brightness.h"

module_t *modules[] = {
        &analyze_audio_track_module,
        &analyze_color_space_module,
        &analyze_picture_brightness_module
};

int main(int argc, char **argv) {
    if (argc <= 1) {
        printf("Usage: %s <module> <args>\n", argv[0]);
        printf("Available modules:\n");
        for (int i = 0; i < sizeof(modules) / sizeof(module_t *); i++) {
            printf("\t[%s] %s\n", modules[i]->short_name, modules[i]->long_name);
        }
        return 1;
    }
    for (int i = 0; i < sizeof(modules) / sizeof(module_t *); i++) {
        if (strcmp(modules[i]->short_name, argv[1]) == 0 || strcmp(modules[i]->long_name, argv[1]) == 0) {
            return modules[i]->function(argc - 1, argv + 1);
        }
    }
}