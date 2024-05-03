//
// Created by jackhammer on 03.05.24.
//

#include "frame_saver.h"

void create_directory(const char* name) {
    mkdir(name, 0777);
}

void saveFrameAsJpeg(const AVFrame* frame, const int width, const int height, const int jpegQuality, const int frameNumber) {
    create_directory("frames");
    char filename[64];
    sprintf(filename, "frames/frame-%d.jpg", frameNumber);

    FILE *outfile = fopen(filename, "wb");
    if (!outfile) {
        char error[64];
        sprintf(error, "Can't open file %s \n", filename);
        fprintf(stderr, "%s\n", error);
        return;
    }

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 75, TRUE);
    jpeg_start_compress(&cinfo, TRUE);
    int row_stride = width * 3;

    unsigned char* tmp = malloc(sizeof(char) * row_stride * height);

    struct SwsContext *sws_ctx = sws_getContext(width, height, AV_PIX_FMT_YUV420P, width, height, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
    sws_scale(sws_ctx, frame->data, frame->linesize, 0, height, &tmp, &row_stride);
    while (cinfo.next_scanline < cinfo.image_height) {
        JSAMPROW row_pointer[1];
        row_pointer[0] = &tmp[cinfo.next_scanline * row_stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }
    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
    free(tmp);
    sws_freeContext(sws_ctx);
}
