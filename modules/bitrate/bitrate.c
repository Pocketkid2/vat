#include "bitrate.h"

#include <stdio.h>

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>

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

    AVFormatContext *pFormatCtx = NULL;
    int videoStream;
    double min_bitrate = INT_MAX;
    double max_bitrate = INT_MIN;
    double total_bitrate = 0;
    int frame_count = 0;

    if (avformat_open_input(&pFormatCtx, filename, NULL, NULL) != 0)
        return -1; // Couldn't open file

    if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
        return -1; // Couldn't find stream information

    videoStream = -1;
    AVCodecParameters *pCodecParameters = NULL;
    AVCodecContext *pCodecContext = NULL;

    for (int i = 0; i < pFormatCtx->nb_streams; i++) {
        pCodecParameters = pFormatCtx->streams[i]->codecpar;
        if (pCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStream = i;
            pCodecContext = avcodec_alloc_context3(NULL);
            if (!pCodecContext) {
                printf("Could not allocate a codec context\n");
                return -1;
            }
            if (avcodec_parameters_to_context(pCodecContext, pCodecParameters) < 0) {
                printf("Could not copy codec parameters to codec context\n");
                return -1;
            }
            break;
        }
    }
        
    if (videoStream == -1)
        return -1; // Didn't find a video stream

    AVPacket packet;
    while (av_read_frame(pFormatCtx, &packet) >= 0)
    {
        if (packet.stream_index == videoStream)
        {
            double bitrate = (double)packet.size * 8 / av_q2d(pFormatCtx->streams[videoStream]->time_base) / 1000.0;
            min_bitrate = fmin(min_bitrate, bitrate);
            max_bitrate = fmax(max_bitrate, bitrate);
            total_bitrate += bitrate;
            frame_count++;
        }
        av_packet_unref(&packet);
    }

    double avg_bitrate = total_bitrate / frame_count;

    printf("Min bitrate: %f kbps\n", min_bitrate);
    printf("Max bitrate: %f kbps\n", max_bitrate);
    printf("Avg bitrate: %f kbps\n", avg_bitrate);

    avformat_close_input(&pFormatCtx);
    avcodec_free_context(&pCodecContext);

    return 0;
}
