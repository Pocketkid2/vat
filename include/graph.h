#ifndef VAT_GRAPH_H
#define VAT_GRAPH_H

#include <cairo.h>
#include <stdio.h>

void create_graph_bitrate_v_time(double* data, int data_count, double framerate, int width, int height, const char* filename);

#endif