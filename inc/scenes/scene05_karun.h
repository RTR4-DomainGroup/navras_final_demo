#pragma once

#include "../../inc/helper/camera.h"
#include "../../inc/helper/common.h"
#include "../../inc/helper/framebuffer.h"
#include "../../inc/helper/texture_loader.h"

struct scene5_variables
{

	float x;
	float y;
	float z;

};

int initializeScene5_karun(void);

void setCameraScene05_karun(void);

void displayScene5_karun(void);
void uninitializeScene5_karun(void);
void updateScene5_karun(void);
