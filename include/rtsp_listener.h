//
// Created by jackhammer on 03.05.24.
//

#ifndef RTSP_RTSP_LISTENER_H
#define RTSP_RTSP_LISTENER_H

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libavutil/log.h>
#include <signal.h>

#include "frame_saver.h"
#include "error-handler.h"

void startProcessing(const char* url, int jpegQuality);

#endif //RTSP_RTSP_LISTENER_H
