#include <stdio.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <libgen.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libavutil/pixdesc.h>

#include "brightness.h"
#include "parameter.h"

int brightness_function(int argc, char **argv);

module_t brightness_module = {
    .long_name = "brightness",
    .short_name = "br",
    .description = "HDR brightness analysis.",
    .function = brightness_function};



int save_frame_as_png(AVFrame *pFrame, int width, int height, int iFrame, char* folder) {
    FILE *pFile;
    char szFilename[1024];
    int  y;

    // Open file
    sprintf(szFilename, "%s/frame%d.png", folder, iFrame);
    pFile=fopen(szFilename, "wb");
    if(pFile==NULL)
        return -1;

    // Write header
    fprintf(pFile, "P6\n%d %d\n255\n", width, height);

    // Write pixel data
    for(y=0; y<height; y++)
        fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);

    // Close file
    fclose(pFile);

    return 0;
}

int brightness_function(int argc, char **argv)
{
    char *full_filename;
    char *base_filename;
    if (get_filename(argc, argv, &full_filename, &base_filename)) {
        return -1;
    }

    char folder[1024];
    sprintf(folder, "keyframes/%s", base_filename);
    mkdir(folder, 0700);

    AVFormatContext *pFormatCtx = NULL;
    if (avformat_open_input(&pFormatCtx, full_filename, NULL, NULL) != 0) {
        printf("Could not open file\n");
        return -1;
    }

    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        printf("Could not find stream information\n");
        return -1;
    }

    int videoStream = -1;
    AVCodecParameters *codecpar = NULL;
    AVCodecContext *pCodecCtx = NULL;

    for (int i = 0; i < pFormatCtx->nb_streams; i++) {
        codecpar = pFormatCtx->streams[i]->codecpar;
        if (codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            pCodecCtx = avcodec_alloc_context3(NULL);
            if (!pCodecCtx) {
                printf("Could not allocate memory for codec context\n");
                return -1;
            }
            if (avcodec_parameters_to_context(pCodecCtx, codecpar) < 0) {
                printf("Could not copy codec parameters to codec context\n");
                return -1;
            }
            break;
        }
    }

    if (videoStream == -1) {
        printf("Could not find a video stream\n");
        return -1;
    }

    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == NULL) {
        printf("Unsupported codec\n");
        return -1;
    }

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        printf("Could not open codec\n");
        return -1;
    }

    AVFrame *pFrame = av_frame_alloc();
    AVFrame *pFrameRGB = av_frame_alloc();
    if (pFrameRGB == NULL) {
        printf("Could not allocate frame\n");
        return -1;
    }

    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, 1);
    uint8_t *buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

    av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize, buffer, AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, 1);

    struct SwsContext *sws_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);

    int frameFinished;
    AVPacket packet;

    int i = 0;
    while (av_read_frame(pFormatCtx, &packet) >= 0) {
        if (packet.stream_index == videoStream) {
            if (avcodec_send_packet(pCodecCtx, &packet) < 0) {
                printf("Error sending packet for decoding\n");
                return -1;
            }
            while (avcodec_receive_frame(pCodecCtx, pFrame) == 0) {
                if (pFrame->key_frame) {
                    sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);
                    if(save_frame_as_png(pFrameRGB, pCodecCtx->width, pCodecCtx->height, ++i, folder) != 0)
                        return -1;
                }
            }
        }
        av_packet_unref(&packet);
    }

    av_free(buffer);
    av_free(pFrameRGB);
    av_free(pFrame);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);

    printf("Brightness function called\n");
    return 0;
}