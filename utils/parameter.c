#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>

int get_filename(int argc, char *argv[], char **full_filename, char **base_filename) {
    if (argc < 3) {
        printf("Error: No video filename supplied\n");
        return 1;
    }

    if (access(argv[2], F_OK) != 0) {
        printf("Error: File does not exist\n");
        return 1;
    }

    *full_filename = argv[2];
    *base_filename = basename(argv[2]);
    return 0;
}