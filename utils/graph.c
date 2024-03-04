#include <math.h>

#include "graph.h"

void create_graph(double *data, int data_count, int width, int height, const char *filename)
{
    // Define margin
    int margin = (width < height) ? width / 10 : height / 10;

    // Calculate the number of data points per pixel
    int points_per_pixel = data_count / (width - 2 * margin);
    if (points_per_pixel == 0)
        points_per_pixel = 1;

    // Create the graph
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create(surface);

    // Set background to white
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    // Set line color to black
    cairo_set_source_rgb(cr, 0, 0, 0);

    // Draw graph
    for (int i = 0; i < width - 2 * margin; i++)
    {
        // Calculate the average of the data points for this pixel
        double sum = 0;
        for (int j = 0; j < points_per_pixel; j++)
        {
            sum += data[i * points_per_pixel + j];
        }
        double average = sum / points_per_pixel;

        // Draw the point on the graph
        cairo_line_to(cr, i + margin, height - margin - (average * (height - 2 * margin)));
    }
    cairo_stroke(cr);

    // Draw axes
    cairo_move_to(cr, margin, height - margin);
    cairo_line_to(cr, width - margin, height - margin);
    cairo_move_to(cr, margin, height - margin);
    cairo_line_to(cr, margin, margin);
    cairo_stroke(cr);

    // Draw grid and markers
    int num_horizontal_lines = 10;
    int num_vertical_lines = 10;
    for (int i = 1; i < num_horizontal_lines; i++)
    {
        int y = height - margin - i * (height - 2 * margin) / num_horizontal_lines;
        cairo_move_to(cr, margin, y);
        cairo_line_to(cr, width - margin, y);
    }
    for (int i = 1; i < num_vertical_lines; i++)
    {
        int x = margin + i * (width - 2 * margin) / num_vertical_lines;
        cairo_move_to(cr, x, height - margin);
        cairo_line_to(cr, x, margin);
    }
    cairo_set_source_rgba(cr, 0, 0, 0, 0.2); // Set color to semi-transparent black
    cairo_stroke(cr);

    // Set font size and color
    cairo_set_font_size(cr, 20);       // Set font size to 20
    cairo_set_source_rgb(cr, 1, 0, 0); // Set font color to red

    // Label axes
    cairo_move_to(cr, width / 2, height - margin + 20);
    cairo_show_text(cr, "Time");
    cairo_move_to(cr, margin - 20, height / 2);
    cairo_save(cr);
    cairo_rotate(cr, -M_PI / 2);
    cairo_show_text(cr, "Bitrate");
    cairo_restore(cr);

    // Save to PNG
    cairo_surface_write_to_png(surface, filename);

    // Clean up
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}