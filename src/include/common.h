#pragma once

#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include "OGL.h"
#include "vmath.h"

// OpenGL Header Files
#include<GL/glew.h>		// THIS MUST BE BEFORE gl.h
#include<GL/gl.h>

using namespace vmath;

enum {

	DOMAIN_ATTRIBUTE_POSITION = 0,
	DOMAIN_ATTRIBUTE_COLOR,
	DOMAIN_ATTRIBUTE_NORMAL,
	DOMAIN_ATTRIBUTE_TEXTURE0

};

BOOL LoadGLTexture(GLuint*, TCHAR[]);

