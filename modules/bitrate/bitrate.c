#include <stdio.h>
#include <assert.h>

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>

#include "bitrate.h"
#include "graph.h"

#define BUFFER_ALLOCATION_SIZE 4096

int bitrate_function(int argc, char **argv);

module_t bitrate_module = {
    .long_name = "bitrate",
    .short_name = "bt",
    .function = bitrate_function};

int bitrate_function(int argc, char **argv)
{

    // Make sure filename argument exists
    if (argc < 3)
    {
        printf("Missing filename argument\n");
        printf("%s %s <filename>\n", argv[0], argv[1]);
        return 1;
    }
    char *filename = argv[2];
    printf("Calculating bitrate for file %s\n", argv[2]);

    // Allocate buffer for bitrate data
    int *frame_size_data_buffer = (int *)malloc(BUFFER_ALLOCATION_SIZE * sizeof(int));
    if (!frame_size_data_buffer)
    {
        printf("Could not allocate buffer for frame size data\n");
        return -1;
    }
    int frame_size_data_buffer_max = BUFFER_ALLOCATION_SIZE; // Current limit
    int frame_size_data_buffer_size = 0; // Current size

    AVFormatContext *pFormatCtx = NULL;
    int videoStream;
    int min_frame_bytes = INT_MAX;
    int max_frame_bytes = INT_MIN;
    long total_bytes = 0;
    int frame_count = 0;
    assert(sizeof(long) == 8);

    if (avformat_open_input(&pFormatCtx, filename, NULL, NULL) != 0)
        return -1; // Couldn't open file

    if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
        return -1; // Couldn't find stream information

    videoStream = -1;
    AVCodecParameters *pCodecParameters = NULL;
    AVCodecContext *pCodecContext = NULL;

    for (int i = 0; i < pFormatCtx->nb_streams; i++)
    {
        pCodecParameters = pFormatCtx->streams[i]->codecpar;
        if (pCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStream = i;
            pCodecContext = avcodec_alloc_context3(NULL);
            if (!pCodecContext)
            {
                printf("Could not allocate a codec context\n");
                return -1;
            }
            if (avcodec_parameters_to_context(pCodecContext, pCodecParameters) < 0)
            {
                printf("Could not copy codec parameters to codec context\n");
                return -1;
            }
            break;
        }
    }

    if (videoStream == -1)
        return -1; // Didn't find a video stream

    AVCodec *pCodec = avcodec_find_decoder(pCodecParameters->codec_id);
    if (!pCodec)
    {
        printf("Unsupported codec!\n");
        return -1;
    }

    if (avcodec_open2(pCodecContext, pCodec, NULL) < 0)
    {
        printf("Could not open codec\n");
        return -1;
    }

    AVFrame *pFrame = av_frame_alloc();
    if (!pFrame)
    {
        printf("Could not allocate video frame\n");
        return -1;
    }

    AVPacket packet;

    while (av_read_frame(pFormatCtx, &packet) >= 0)
    {
        if (packet.stream_index == videoStream)
        {
            int response = avcodec_send_packet(pCodecContext, &packet);

            while (response >= 0)
            {
                response = avcodec_receive_frame(pCodecContext, pFrame);

                if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
                    break;
                else if (response < 0)
                {
                    printf("Error while receiving a frame from the decoder\n");
                    return -1;
                }

                if (response >= 0)
                {
                    // Set the min and max bytes per frame if applicable
                    if (packet.size > max_frame_bytes)
                        max_frame_bytes = packet.size;
                    if (packet.size < min_frame_bytes)
                        min_frame_bytes = packet.size;

                    // Add to the running totals
                    total_bytes += packet.size;
                    frame_count++;

                    // Add to the buffer
                    if (frame_size_data_buffer_size >= frame_size_data_buffer_max)
                    {
                        frame_size_data_buffer_max += BUFFER_ALLOCATION_SIZE;
                        frame_size_data_buffer = (int *)realloc(frame_size_data_buffer, frame_size_data_buffer_max * sizeof(int));
                        if (!frame_size_data_buffer)
                        {
                            printf("Could not reallocate buffer for frame size data\n");
                            return -1;
                        }
                    }
                    frame_size_data_buffer[frame_size_data_buffer_size++] = packet.size;

                    if (frame_count % 1000 == 0)
                        printf("Processed %d frames\n", frame_count);
                }
            }
        }
        av_packet_unref(&packet);
    }

    double frame_rate = av_q2d(pFormatCtx->streams[videoStream]->avg_frame_rate);

    double avg_frame_bytes = ((double)total_bytes) / ((double)frame_count);

    double min_bitrate_kbps = ((double)min_frame_bytes * 8.0 * frame_rate) / 1000.0;
    double max_bitrate_kbps = ((double)max_frame_bytes * 8.0 * frame_rate) / 1000.0;
    double avg_bitrate_kbps = ((double)avg_frame_bytes * 8.0 * frame_rate) / 1000.0;

    printf("Min bitrate: %.2f kbps\n", min_bitrate_kbps);
    printf("Max bitrate: %.2f kbps\n", max_bitrate_kbps);
    printf("Avg bitrate: %.2f kbps\n", avg_bitrate_kbps);

    printf("Creating graph\n");

    double *bitrate_data = (double *)malloc(frame_size_data_buffer_size * sizeof(double));

    for (int i = 0; i < frame_size_data_buffer_size; i++)
    {
        bitrate_data[i] = ((double)frame_size_data_buffer[i] * 8.0 * frame_rate) / 1000.0;
    }

    create_graph_bitrate_v_time(bitrate_data, frame_size_data_buffer_size, frame_rate, 1280, 720, "bitrate_data.png");

    avformat_close_input(&pFormatCtx);
    avcodec_free_context(&pCodecContext);

    return 0;
}
