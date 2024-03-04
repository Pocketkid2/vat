#include "graph.h"

void create_graph(double* data, int data_count, int width, int height, const char* filename) {
    // Define margin
    int margin = (width < height) ? width / 10 : height / 10;

    // Calculate the number of data points per pixel
    int points_per_pixel = data_count / (width - 2 * margin);
    if (points_per_pixel == 0) points_per_pixel = 1;

    // Create the graph
    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t* cr = cairo_create(surface);

    // Set background to white
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    // Set line color to black
    cairo_set_source_rgb(cr, 0, 0, 0);

    // Draw graph
    for (int i = 0; i < width - 2 * margin; i++) {
        // Calculate the average of the data points for this pixel
        double sum = 0;
        for (int j = 0; j < points_per_pixel; j++) {
            sum += data[i * points_per_pixel + j];
        }
        double average = sum / points_per_pixel;

        // Draw the point on the graph
        cairo_line_to(cr, i + margin, height - margin - (average * (height - 2 * margin)));
    }
    cairo_stroke(cr);

    // Label axes
    cairo_move_to(cr, margin, height - margin - 10);
    cairo_show_text(cr, "0");
    cairo_move_to(cr, width - margin - 10, height - margin - 10);
    cairo_show_text(cr, "1");

    // Save to PNG
    cairo_surface_write_to_png(surface, filename);

    // Clean up
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}