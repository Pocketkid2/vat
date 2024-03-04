#include <math.h>

#include "graph.h"

#define WHITE 1, 1, 1
#define BLACK 0, 0, 0
#define RED 1, 0, 0
#define GREEN 0, 1, 0
#define BLUE 0, 0, 1
#define YELLOW 1, 1, 0
#define CYAN 0, 1, 1
#define MAGENTA 1, 0, 1

void create_graph_bitrate_v_time(double *data, int data_count, double framerate, int width, int height, const char *filename)
{
    // Scan the data to find the maximum value
    double max_value = 0;
    for (int i = 0; i < data_count; i++)
    {
        if (data[i] > max_value)
        {
            max_value = data[i];
        }
    }

    // Define left and right margins
    int margin[2];
    margin[0] = width / 10;  // Left margin
    margin[1] = height / 10; // Right margin

    // Calculate the number of data points per pixel
    int points_per_pixel = data_count / (width - margin[0] - margin[1]);
    if (points_per_pixel == 0)
        points_per_pixel = 1;

    // Create the graph
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create(surface);

    // Set background to white
    cairo_set_source_rgb(cr, WHITE);
    cairo_paint(cr);

    // Set line color to black
    cairo_set_source_rgb(cr, BLUE);

    // Draw graph
    for (int i = 0; i < width - margin[0] - margin[1]; i++)
    {
        // Calculate the average of the data points for this pixel
        double sum = 0;
        for (int j = 0; j < points_per_pixel; j++)
        {
            sum += data[i * points_per_pixel + j];
        }
        double average = sum / points_per_pixel;

        // Scale the average based on the maximum value
        double scaled_average = average / max_value;

        // Move to the horizontal axis for this x point
        cairo_move_to(cr, i + margin[0], height - margin[1]);

        // Draw a line to the y-value for this x point
        cairo_line_to(cr, i + margin[0], height - margin[1] - (scaled_average * (height - margin[0] - margin[1])));
    }
    cairo_stroke(cr);

    // Change color back to black
    cairo_set_source_rgb(cr, BLACK);

    // Draw axes
    cairo_move_to(cr, margin[0], height - margin[1]);
    cairo_line_to(cr, width - margin[1], height - margin[1]);
    cairo_move_to(cr, margin[0], height - margin[1]);
    cairo_line_to(cr, margin[0], margin[0]);
    cairo_stroke(cr);

    // Draw grid and markers
    int num_horizontal_lines = 10;
    int num_vertical_lines = 10;
    double max_label_width = 0;
    for (int i = 1; i < num_horizontal_lines; i++)
    {
        int y = height - margin[1] - i * (height - 2 * margin[1]) / num_horizontal_lines;
        cairo_move_to(cr, margin[0], y);
        cairo_line_to(cr, width - margin[1], y);

        // Label the grid line
        char label[64];
        double value = i * max_value / num_horizontal_lines;
        if (value > 1000.0)
        {
            sprintf(label, "%.2f Mbps", value / 1000.0);
        }
        else
        {
            sprintf(label, "%.2f kbps", value);
        }

        // Calculate text width
        cairo_text_extents_t extents;
        cairo_text_extents(cr, label, &extents);
        if (extents.width > max_label_width)
        {
            max_label_width = extents.width;
        }

        // Move to the position adjusted by the text width
        cairo_move_to(cr, margin[0] - (width / 100) - extents.width, y); // Move labels to the left of the vertical axis

        cairo_show_text(cr, label);
    }
    for (int i = 1; i < num_vertical_lines; i++)
    {
        int x = margin[0] + i * (width - 2 * margin[0]) / num_vertical_lines;
        cairo_move_to(cr, x, height - margin[1]);
        cairo_line_to(cr, x, margin[0]);

        // Calculate the time for this data point
        double time = i * data_count / (num_vertical_lines * framerate);

        // Convert the time to hours, minutes, and seconds
        int hours = (int)time / 3600;
        time -= hours * 3600;
        int minutes = (int)time / 60;
        time -= minutes * 60;
        int seconds = (int)time;

        // Label the grid line
        char label[64];
        sprintf(label, "%02d:%02d:%02d", hours, minutes, seconds);
        cairo_move_to(cr, x, height - margin[1] + (height / 40)); // Move labels below the horizontal axis
        cairo_show_text(cr, label);
    }
    cairo_set_source_rgba(cr, BLACK, 0.2); // Set color to semi-transparent black
    cairo_stroke(cr);

    // Set font size and color
    cairo_set_font_size(cr, 20);     // Set font size to 20
    cairo_set_source_rgb(cr, BLACK); // Set font color to red

    // Label axes
    cairo_move_to(cr, width / 2, height - margin[1] + (width / 30)); // Move x-axis label down
    cairo_show_text(cr, "Time");
    cairo_move_to(cr, margin[0] - (width / 100) - max_label_width, height / 2); // Move y-axis label to the left
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