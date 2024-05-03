//
// Created by jackhammer on 03.05.24.
//

#include <unistd.h>
#include "rtsp_listener.h"

static volatile sig_atomic_t keepRunning = 1;
AVFormatContext* formatContext;
AVCodecContext* codecContext;
int videoStreamIndex = -1;

void handle_signal(int sig) {
    keepRunning = 0;
    char msg[60];
    sprintf(msg, "Signal #%d received. Terminating...\n", sig);
    size_t len = strlen(msg);
    write(STDERR_FILENO, msg, len);
}

void init(const char* url) {
    avformat_network_init();

    if (avformat_open_input(&formatContext, url, NULL, NULL) != 0) {
        fatal_error("Failed to open RTSP stream", CANT_OPEN_RTSP_STREAM);
    }

    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        fatal_error("Failed to find stream information", CANT_FIND_STREAM_INFORMATION);
    }

    av_dump_format(formatContext, 0, url, 0);

    for (int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1) {
        fatal_error("Failed to find video stream", CANT_FIND_VIDEO_STREAM);
    }

    AVCodecParameters* codecParams = formatContext->streams[videoStreamIndex]->codecpar;
    AVCodec* codec = avcodec_find_decoder(codecParams->codec_id);
    if (!codec) {
        fatal_error("Unsupported codec", UNSUPPORTED_CODEC);
    }

    codecContext = avcodec_alloc_context3(codec);
    if (!codecContext) {
        fatal_error("Failed to allocate codec context", CANT_ALLOCATE_CODEC_CONTEXT);
    }

    if (avcodec_parameters_to_context(codecContext, codecParams) < 0) {
        fatal_error("Failed to copy codec parameters to codec context", CANT_COPY_CODEC_PARAMETERS);
    }

    if (avcodec_open2(codecContext, codec, NULL) < 0) {
        fatal_error("Failed to open codec", CANT_OPEN_CODEC);
    }
}

void cleanup() {
    if (codecContext) {
        avcodec_free_context(&codecContext);
    }
    if (formatContext) {
        avformat_close_input(&formatContext);
    }
}

void de_init() {
    cleanup();
    avformat_network_deinit();
}

void startProcessing(const char* url, int jpegQuality) {
    init(url);

    if (jpegQuality < 0 || jpegQuality > 100) {
        char errorDescription[70];
        sprintf(errorDescription, "Jpeg quality must be between 0 and 100. %d", jpegQuality);
        fatal_error(errorDescription, INVALID_JPEG_QUALITY);
    }

    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    AVPacket packet;
    av_init_packet(&packet);
    const int64_t frameInterval = AV_TIME_BASE / 10;
    int64_t nextFrameTime = 0;
    int32_t frameNumber = 1;

    while (keepRunning && av_read_frame(formatContext, &packet) >= 0) {
        if (packet.stream_index == videoStreamIndex) {
            int response = avcodec_send_packet(codecContext, &packet);
            if (response < 0) {
                av_packet_unref(&packet);
                fatal_error("Error decoding packet", CANT_DECODE_PACKET);
                break;
            }

            AVFrame* frame = av_frame_alloc();
            if (!frame) {
                av_packet_unref(&packet);
                fatal_error("Failed to allocate memory for AVFrame", CANT_ALLOCATE_MEMORY);
                break;
            }

            while (keepRunning && response >= 0) {
                response = avcodec_receive_frame(codecContext, frame);
                if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
                    av_frame_free(&frame);
                    break;
                } else if (response < 0) {
                    av_frame_free(&frame);
                    av_packet_unref(&packet);
                    fatal_error("Error during decoding", ERROR_DURING_DECODING);
                    break;
                }

                if (frame->pts >= nextFrameTime) {
                    saveFrameAsJpeg(frame, frame->width, frame->height, jpegQuality, frameNumber++);
                    nextFrameTime += frameInterval;
                }
            }
            av_frame_free(&frame);
        }
        av_packet_unref(&packet);
    }

    de_init();
}