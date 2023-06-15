#define _USE_MATH_DEFINES 1
#include <math.h>		// for PI

#include "../../inc/effects/videoEffect.h"
#include "../../inc/shaders/FSVQuadShader.h"
#include "../../inc/helper/texture_loader.h"

#include "../../inc/helper/audioplayer.h"
#include "../../inc/helper/constants.h"


GLuint texture_frame;
int frameWidth, frameHeight;
uint8_t* frame_data = NULL;
struct VideoReaderState vr_state;

bool flagAudio = true;

struct FSVQuadUniform fsvqUniform;

int initializeVideoEffect(const char* videoFile)
{
    // Code
    if (!video_reader_open(&vr_state, videoFile))
    {
        LOG("Failed to open video file.\n");
        return -1;
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
    LoadGLTexture(&texture_frame, frameWidth, frameHeight, frame_data);
    LOG("Frame Width = %d.\nFrame Height = %d.\n", frameWidth, frameHeight);
   

    return 0;
}

void displayVideoEffect()
{

#ifdef ENABLE_AUDIO
    if(flagAudio == true){
        playSong(0);
        flagAudio = false;
    }
 #endif

    // Function declaration
    // Code
    fsvqUniform = useFSVQuadShader();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_frame);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameWidth, frameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame_data);
    glUniform1i(fsvqUniform.textureSamplerUniform1, 0);
    glUniform1i(fsvqUniform.textureSamplerUniform2, 1);  
    displayVideoQuad();
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    
}

void updateVideoEffect(void)
{  
    if (!video_reader_read_frame(&vr_state, frame_data))
    {
        LOG("Couldn't load video frame.\n");
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
    uninitializeFSVQuadShader();
}