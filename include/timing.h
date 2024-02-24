#ifndef VAT_TIMING_H
#define VAT_TIMING_H

#include <time.h>

typedef struct {
    clock_t start_time;
    clock_t end_time;
} stopwatch_t;

void stopwatch_start(stopwatch_t *stopwatch);
void stopwatch_stop(stopwatch_t *stopwatch);
double stopwatch_get(stopwatch_t *stopwatch);
void stopwatch_print(stopwatch_t *stopwatch);

#endif