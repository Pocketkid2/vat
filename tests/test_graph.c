#include "graph.h"
#include <math.h>

#define DATA_COUNT 10000

int main() {
    // Generate synthetic data
    double data[DATA_COUNT];
    for (int i = 0; i < DATA_COUNT; i++) {
        data[i] = fabs(sin(2 * M_PI * i / DATA_COUNT));
    }

    // Create graph
    create_graph_bitrate_v_time(data, DATA_COUNT, 24, 800, 600, "graph.png");

    return 0;
}