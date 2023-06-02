#pragma once
#include<windows.h>
#include<stdio.h>
#include <stdlib.h>
#include "../helper/vmath.h"
#include "../helper/common.h"

#include <map>
#include <string>

int initializeFont(void);
void displayFont(void);
void RenderText(std::string, float, float, float, vec4);
void uninitializeFont(void);

