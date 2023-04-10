#include "../../inc/video_reader.h"

#pragma comment(lib, "lib\\ffmpeg\\avformat.lib")
#pragma comment(lib, "lib\\ffmpeg\\avcodec.lib")
#pragma comment(lib, "lib\\ffmpeg\\avformat.lib")
#pragma comment(lib, "lib\\ffmpeg\\avutil.lib")
#pragma comment(lib, "lib\\ffmpeg\\swscale.lib")

extern FILE *gpFile;

bool video_reader_open(VideoReaderState* state, const char* fileName)
{
    state->avFormatContext = avformat_alloc_context();
    
    if (!state->avFormatContext)
    {
        fprintf(gpFile, "Unable to Create AVFormatContext. \n");
        return false;
    }
    
    if(avformat_open_input(&state->avFormatContext, fileName, NULL, NULL) != 0)
    {
        fprintf(gpFile, "Unable to open input file. \n");
        return false;
    }

    state->video_stream_index = -1;
    AVCodecParameters* avCodecParams;
    const AVCodec* avCodec;

    for (int i = 0; i < state->avFormatContext->nb_streams; i++)
    {
        avCodecParams = state->avFormatContext->streams[i]->codecpar;
        avCodec = avcodec_find_decoder(avCodecParams->codec_id);

        if (!avCodec)
        {
            continue;
        }

        if (avCodecParams->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            state->video_stream_index = i;
            state->width = avCodecParams->width;
            state->height = avCodecParams->height;
            break;
        }        
    }
    if (state->video_stream_index == -1)
    {
        fprintf(gpFile, "Unable to find video stream index. \n");
        return false;
    }
    
    state->avCodecContext = avcodec_alloc_context3(avCodec);
    if (!state->avCodecContext)
    {
        fprintf(gpFile, "Unable to allocate codec context. \n");
        return false;
    }
    
    if (avcodec_parameters_to_context(state->avCodecContext, avCodecParams) < 0)
    {
        fprintf(gpFile, "Unable to initialize AV codec context. \n");
        return false;
    }
    
    if (avcodec_open2(state->avCodecContext, avCodec, NULL) < 0)
    {
        fprintf(gpFile, "Unable to open AV codec. \n");
        return false;
    }
    
    
    state->avFrame = av_frame_alloc();
    if (!state->avFrame)
    {
        fprintf(gpFile, "Unable to allocate memory AV Frame. \n");
        return false;
    }
    
    state->avPacket = av_packet_alloc();
    if (!state->avPacket)
    {
        fprintf(gpFile, "Unable to allocate memory AV Packet. \n");
        return false;
    }
    
    return true;
}
bool video_reader_read_frame(VideoReaderState* state, uint8_t* frame_buffer)
{
    int response;
    while (av_read_frame(state->avFormatContext, state->avPacket) >= 0)
    {
        if (state->avPacket->stream_index != state->video_stream_index)
        {
            continue;
        }

        response = avcodec_send_packet(state->avCodecContext, state->avPacket);
        if (response < 0)
        {
            fprintf(gpFile, "Failed to Decode AV Packet.\n");
        }
        response = avcodec_receive_frame(state->avCodecContext, state->avFrame);
        
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
        {
            continue;
        }
        else if (response < 0)
        {
            fprintf(gpFile, "Failed to Decode AV Packet.\n");
            return false;
        }
        av_packet_unref(state->avPacket);
        break;    
    }

    state->swsScaleContext = sws_getContext(state->width, state->height, state->avCodecContext->pix_fmt, 
                                state->width, state->height, AV_PIX_FMT_RGB0, SWS_BILINEAR, NULL, NULL, NULL);
    
    if (!state->swsScaleContext)
    {
        fprintf(gpFile, "Failed to get scale context.\n");
        return false;
    }
    
    uint8_t* dest[4] = { frame_buffer, NULL, NULL, NULL };
    int dest_lineSize[4] = {state->width * 4, 0, 0, 0 };
    sws_scale(state->swsScaleContext, state->avFrame->data, state->avFrame->linesize, 0, state->avFrame->height, dest, dest_lineSize);

    return true;

}

bool video_reader_close(VideoReaderState* state)
{
    if (state)
    {
        if (state->swsScaleContext)
        {
            sws_freeContext(state->swsScaleContext);
        }
        if (state->avFormatContext)
        {
            avformat_close_input(&state->avFormatContext);
        }
        if (state->avFormatContext)
        {
            avformat_free_context(state->avFormatContext);
        }
        if (state->avFrame)
        {
            av_frame_free(&state->avFrame);
        }
            
        
        av_packet_free(&state->avPacket);
        avcodec_free_context(&state->avCodecContext);
    }

    return true;
}