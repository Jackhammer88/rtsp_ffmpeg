//
// Created by jackhammer on 03.05.24.
//

#ifndef RTSP_ERROR_HANDLER_H
#define RTSP_ERROR_HANDLER_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    CANT_OPEN_RTSP_STREAM = 10,
    CANT_FIND_STREAM_INFORMATION = 20,
    CANT_FIND_VIDEO_STREAM = 30,
    UNSUPPORTED_CODEC = 40,
    CANT_ALLOCATE_CODEC_CONTEXT = 50,
    CANT_COPY_CODEC_PARAMETERS = 60,
    CANT_OPEN_CODEC = 70,
    CANT_DECODE_PACKET = 80,
    ERROR_DURING_DECODING = 90,
    CANT_ALLOCATE_MEMORY = 100,
    INVALID_JPEG_QUALITY = 200
} ErrorCodes;


void fatal_error(const char* message, int code);

#endif //RTSP_ERROR_HANDLER_H
