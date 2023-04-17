#include "../../inc/effects/videoEffect.h"
#include "../../inc/shaders/FSQuadShader.h"


GLuint texture_frame;
int frameWidth, frameHeight;
uint8_t* frame_data = NULL;
struct VideoReaderState vr_state;

GLuint vao_cube;
GLuint vbo_cube_position;
GLuint vbo_cube_texcoord;

int initializeVideoEffect(const char* videoFile)
{
    BOOL LoadGLTexture(GLuint *, GLsizei, GLsizei, void*);
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
    if(LoadGLTexture(&texture_frame, (GLsizei)frameWidth, (GLsizei)frameHeight, frame_data) == FALSE)
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
    BOOL LoadGLTexture(GLuint *, GLsizei, GLsizei, void*);

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

BOOL LoadGLTexture(GLuint *texture, GLsizei width, GLsizei height, void* data)
{   
    
    BOOL bResult = TRUE;

    // For better performance at shader level
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glGenTextures(1, texture);

    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return bResult;    
}
