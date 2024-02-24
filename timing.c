#include <stdio.h>
#include "timing.h"

void stopwatch_start(stopwatch_t *sw) {
    sw->start_time = clock();
}

void stopwatch_stop(stopwatch_t *sw) {
    sw->end_time = clock();
}

double stopwatch_get(stopwatch_t *sw) {
    return (double)(sw->end_time - sw->start_time) / CLOCKS_PER_SEC;
}

void stopwatch_print(stopwatch_t *sw) {
    printf("Elapsed time: %f seconds\n", stopwatch_get(sw));
}