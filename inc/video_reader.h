#pragma once
extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libswscale/swscale.h"
    #include <inttypes.h>
}


struct VideoReaderState
{
    int width, height;
    uint8_t* frame_buffer;
    AVFormatContext* avFormatContext;
    AVCodecContext* avCodecContext;
    int video_stream_index;
    AVFrame* avFrame;
    AVPacket* avPacket;
    SwsContext* swsScaleContext;
};


bool video_reader_open(VideoReaderState*, const char*);
bool video_reader_read_frame(VideoReaderState* , uint8_t*);
bool video_reader_close(VideoReaderState*);
