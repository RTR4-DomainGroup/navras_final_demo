/**
 * @file audioplayer.h
 * @author adlinge.pa (adlinge.pa@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-12-25
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

int initializeAudio(const char* );

int playSong(int);

void playAudio(void);

void pauseAudio(void);

void resumeAudio(void);

void stopAudio(void);

void uninitializeAudio(void);

