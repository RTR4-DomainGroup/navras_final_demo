/**
 * @file audioplayer.c
 * @author adlinge.pa (adlinge.pa@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-12-25
 *
 * @copyright Copyright (c) 2021
 *
 */

// standard headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include <unistd.h>

// Alsa header files
#include <AL/include/al.h>
#include <AL/include/alc.h>

#include "../../inc/helper/common.h"

// #pragma comment(lib, "AL/lib/OpenAL32.lib")


extern FILE* gpFile;

unsigned int sourceid;
ALCdevice* device = NULL;
ALCcontext* context = NULL;

static unsigned char playing_stated = 0;

int initializeAudio(const char* audio_file)
{
    // function declaration 
    char* loadWAV(const char* , int* , int* , int*  , int* );
    void uninitializeAudio(void);

    int ret = -1;

    // code
    // check and stop, if already initialized
    uninitializeAudio();

    if(NULL == (device = alcOpenDevice(NULL)))
    {
        LOG("open Audio device failed\n");
        uninitializeAudio();
        return (ret);

    }
    else if(NULL == (context = alcCreateContext(device, NULL)))
    {
        LOG("alcCreateContext() failed\n");
        uninitializeAudio();
        return (ret);
    }
    else if(ALC_FALSE == alcMakeContextCurrent(context))
    {
        LOG("alcMakeContextCurrent() failed\n");
        uninitializeAudio();
        return (ret);
    }
    else
    {
        unsigned int bufferid, format;
        int channel, samplerate, bitspersecond, size;
        char* data = loadWAV(audio_file, &channel, &samplerate, &bitspersecond, &size);
        if(!data)
        {
            LOG("wav loading failed\n");
            uninitializeAudio();
            return (ret);
        }

        alGenBuffers(1, &bufferid);
        if(channel == 1)
        {
            if(bitspersecond == 8)
            {
                format = AL_FORMAT_MONO8;
            }
            else
            {
                format = AL_FORMAT_MONO16;
            }
        }
        else // channel 2
        {
            if(bitspersecond == 8)
            {
                format = AL_FORMAT_STEREO8;
            }
            else
            {
                format = AL_FORMAT_STEREO16;
            }
        }

        alBufferData(bufferid, format, data, size, samplerate);
        // NOTE: deleting sourceid result in stopping playback, declare this global scope
        // unsigned int sourceid;
        alGenSources(1, &sourceid);
        alSourcei(sourceid, AL_BUFFER, bufferid);
        // alSourcePlay(sourceid);
        // alDeleteSources(1, &sourceid);
        alDeleteBuffers(1, &bufferid);
        free(data);
        ret = 0;
    }

    return (ret);
}

char* loadWAV(const char* filename, int* channel, int* samplerate, int*  bitspersec, int* size)
{
    // function declaration
    int convertToInt(char* , int );

    // code
    char buffer[4] = {0};
    FILE* fp = fopen(filename, "rb");
    if(NULL == fp)
        LOG("file open failed %s\n", filename);
    else if(4 != fread(buffer, 1, 4, fp)) // RIFF
        LOG("file read failed\n");
    else if(0 != strncmp("RIFF", buffer, 4))
        LOG("invalid read\n");
    else if(4 != fread(buffer, 1, 4, fp)) // 
        LOG("line: %d read failed\n", __LINE__);
    else if(4 != fread(buffer, 1, 4, fp)) // WAVE
        LOG("wav read failed\n");
    else if(4 != fread(buffer, 1, 4, fp)) // "fmt "
        LOG("fmt read failed\n");
    else if(4 != fread(buffer, 1, 4, fp)) // "10 00"
        LOG("16 read failed\n");
    else if(2 != fread(buffer, 1, 2, fp)) // "00 00"
        LOG("1 read failed\n");
    else if(2 != fread(buffer, 1, 2, fp)) // "01 00"
        LOG("1-2 read failed\n");
    else if(0 == (*channel = convertToInt(buffer, 2))) // get channel
        LOG("channel get failed: %d\n", *channel);
    else if(4 != fread(buffer, 1, 4, fp)) 
        LOG("wav read failed\n");
    else if(0 == (*samplerate = convertToInt(buffer, 4))) 
        LOG("samplerate conversion failed: %d\n", *samplerate );
    else if(4 != fread(buffer, 1, 4, fp)) // fmt
        LOG("line: %d read failed\n", __LINE__);
    else if(2 != fread(buffer, 1, 2, fp)) // 16
        LOG("line: %d read failed\n", __LINE__);
    else if(2 != fread(buffer, 1, 2, fp)) // 1
        LOG("line: %d read failed\n", __LINE__);
    else if(0 == (*bitspersec = convertToInt(buffer, 2))) // 1
        LOG("bitspersec conversion failed: %d\n", *bitspersec);
    else if(4 != fread(buffer, 1, 4, fp)) // data
        LOG("line: %d read failed\n", __LINE__);
    else if(4 != fread(buffer, 1, 4, fp)) // 
        LOG("line: %d read failed\n", __LINE__);
    else if(0 == (*size = convertToInt(buffer, 4))) // 1
        LOG("size conversion failed: %d\n", *size);
    else{
        char* data = (char*)malloc(*size);
        if(*size != fread(data, 1, *size, fp)) // 
            LOG("line: %d read failed\n", __LINE__);
        fclose(fp);    
        return data;
    }
    return (NULL);
}

unsigned char isBigEndian(void)
{
    int a = 1;
    return !((char*)&a)[0];
}

int convertToInt(char* buffer, int length)
{
    // function declaration
    unsigned char isBigEndian(void);

    // code
    int a = 0;
    if(!isBigEndian())
    {
        for(int i = 0; i < length; i++)
        {
            ((char*)&a)[i] = buffer[i];
        }
    }
    else
    {
        for(int i = 0; i < length; i++)
        {
            ((char*)&a)[3-i] = buffer[i];
        }
    }
    return a;
}

void playAudio()
{
    playing_stated = 1;
    alSourcePlay(sourceid);
}

void pauseAudio()
{
    if(playing_stated)
        alSourcePause(sourceid);
    playing_stated = 0;
}

void resumeAudio()
{
    if(playing_stated)
        alSourcePlay(sourceid);
}

void stopAudio()
{
    alSourceStop(sourceid);
    playing_stated = 0;
}

void uninitializeAudio(void)
{
    if(sourceid)
    {
        alDeleteSources(1, &sourceid);
        sourceid = 0;
    }
    if(context)
    {
        alcDestroyContext(context);
        context = NULL;
    }
    if(device)
    {
        alcCloseDevice(device);
        device = NULL;
    }
}

