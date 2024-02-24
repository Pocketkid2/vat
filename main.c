#include <stdio.h>
#include <string.h>

#include "bitrate/bitrate.h"

module_t *modules[] = {
        &bitrate_module
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