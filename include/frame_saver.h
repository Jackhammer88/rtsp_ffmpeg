//
// Created by jackhammer on 03.05.24.
//

#ifndef RTSP_FRAME_SAVER_H
#define RTSP_FRAME_SAVER_H

#include <libavfilter/avfilter.h>
#include <libswscale/swscale.h>
#include <jpeglib.h>
#include <sys/stat.h>

void create_directory(const char* name);
void saveFrameAsJpeg(const AVFrame* frame, const int width, const int height, const int jpegQuality, const int frameNumber);

#endif //RTSP_FRAME_SAVER_H
