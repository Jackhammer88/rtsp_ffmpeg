cmake_minimum_required(VERSION 3.25)
project(rtsp C)

# Пути к заголовкам и библиотекам
set(ffmpeg_libs_DIR /usr/local/lib)
set(ffmpeg_headers_DIR /usr/local/include)
set(jpeg_libs_DIR /usr/lib/x86_64-linux-gnu)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories( ${ffmpeg_headers_DIR} )
link_directories(${ffmpeg_libs_DIR} )

add_library( avcodec SHARED IMPORTED)
add_library( avfilter SHARED IMPORTED )
add_library( swresample SHARED IMPORTED )
add_library( swscale SHARED IMPORTED )
add_library( avformat SHARED IMPORTED )
add_library( avutil SHARED IMPORTED )
add_library( jpeg SHARED IMPORTED )

set_target_properties( avcodec PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libavcodec.so )
set_target_properties( avfilter PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libavfilter.so )
set_target_properties( swresample PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libswresample.so )
set_target_properties( swscale PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libswscale.so )
set_target_properties( avformat PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libavformat.so )
set_target_properties( avutil PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libavutil.so )
set_target_properties( jpeg PROPERTIES IMPORTED_LOCATION ${jpeg_libs_DIR}/libjpeg.so )

set(CMAKE_C_STANDARD 17)

add_executable(rtsp
        src/main.c
        include/rtsp_listener.h
        src/rtsp_listener.c
        include/frame_saver.h
        src/frame_saver.c
        include/error-handler.h
        src/error-handler.c)
target_link_libraries(${PROJECT_NAME} avcodec avformat avutil swresample swscale swscale avfilter jpeg )
