#define _USE_MATH_DEFINES 1
#include <math.h>		// for PI

#include "../../inc/effects/videoEffect.h"
#include "../../inc/shaders/FSVQuadShader.h"
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
   
    if(initializeFSVQuadShader() != 0)
    {
        LOG("Failed to initializeFSVQuadShader().\n");
        return -2;
    }
    initializeQuadForVideo();
    frameWidth = vr_state.width;
    frameHeight = vr_state.height;
    frame_data = new uint8_t[frameWidth * frameHeight * 4];

    if (!video_reader_read_frame(&vr_state, frame_data))
    {
        LOG("Couldn't load video frame.\n");
        return -2;
    }
    
    if(LoadGLTexture(&texture_frame, (GLsizei)frameWidth, (GLsizei)frameHeight, frame_data) == GL_FALSE)
    {
        LOG("Unable to load Texture.\n");
        return -3;
    }
    LOG("Frame Width = %d.\nFrame Height = %d.\n", frameWidth, frameHeight);
   

    return 0;
}

void displayVideoEffect( struct FSVQuadUniform* fsvqUniform)
{
    // Function declaration
    static bool myFlag = true;
    // Code
    if (&vr_state)
    {
        int i = 0;
        while (i <= 240)
        {
            i++;
        }
        
        if (!video_reader_read_frame(&vr_state, frame_data))
        {
            LOG("Couldn't load video frame.\n");
        }

        if (texture_frame)
        {
            LoadGLTexture(&texture_frame, (GLsizei)frameWidth, (GLsizei)frameHeight, frame_data);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_frame);
            glUniform1i(fsvqUniform->textureSamplerUniform1, 0);
            glUniform1i(fsvqUniform->textureSamplerUniform2, 1);  
            displayVideoQuad();
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void uninitializeVideoEffect(void)
{
    if (texture_frame)
	{
		glDeleteTextures(1, &texture_frame);
		texture_frame = 0;
	}
    video_reader_close(&vr_state);
    uninitializeVideoQuad();
}