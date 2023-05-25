#define _USE_MATH_DEFINES 1
#include <math.h>		// for PI

#include "../../inc/effects/videoEffect.h"
#include "../../inc/shaders/FSQuadShader.h"
#include "../../inc/helper/texture_loader.h"


GLuint texture_frame;
int frameWidth, frameHeight;
uint8_t* frame_data = NULL;
struct VideoReaderState vr_state;

int initializeVideoEffect(const char* videoFile)
{
    // Code
    if (!video_reader_open(&vr_state, videoFile))
    {
        LOG("Failed to open video file.\n");
        return -1;
    }
    
    frameWidth = vr_state.width;
    frameHeight = vr_state.height;
    frame_data = new uint8_t[frameWidth * frameHeight * 4];

    if (!video_reader_read_frame(&vr_state, frame_data))
    {
        LOG("Couldn't load video frame.\n");
        exit(-2);
    }
    if(LoadGLTexture(&texture_frame, (GLsizei)frameWidth, (GLsizei)frameHeight, frame_data) == GL_FALSE)
    {
        LOG("Unable to load Texture.\n");
        return -3;
    }
    LOG("Frame Width = %d.\nFrame Height = %d.\n", frameWidth, frameHeight);
   

    return 0;
}

void displayVideoEffect( struct FSQuadUniform* fsqUniform)
{
    // Function declaration

    // Code    
    glActiveTexture(GL_TEXTURE0);
    if (!video_reader_read_frame(&vr_state, frame_data))
    {
        LOG("Couldn't load video frame.\n");
        exit(-1);
    }    
    LoadGLTexture(&texture_frame, (GLsizei)frameWidth, (GLsizei)frameHeight, frame_data);
    glBindTexture(GL_TEXTURE_2D, texture_frame);
    glUniform1i(fsqUniform->textureSamplerUniform1, 0);
    glUniform1i(fsqUniform->textureSamplerUniform2, 1);
    displayQuad();
    glBindTexture(GL_TEXTURE_2D, 0);    
}

