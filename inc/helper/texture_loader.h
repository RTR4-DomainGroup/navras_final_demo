#pragma once

// standard headers
#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__ 

//linux code goes here

// X11 headers - 11th version of x-server
#include <X11/Xutil.h> // XVisualInfo

// Open GL headers
#include <GL/glew.h>  // for GLEW functionality
#include <GL/gl.h>
#include <GL/glu.h>
#include "common.h"

// dummy macro
#define MAKEINTRESOURCE(a) (int *)a
#define TCHAR int
#define MB_OK
#define TEXT(a) a

#define TEXTURE_DIR "res/textures/"
#define AUDIO_DIR   "res/audios/"

#define VK_NUMPAD0 XK_KP_0
#define VK_NUMPAD1 XK_KP_1
#define VK_NUMPAD2 XK_KP_2
#define VK_NUMPAD3 XK_KP_3
#define VK_NUMPAD4 XK_KP_4
#define VK_NUMPAD5 XK_KP_5
#define VK_NUMPAD6 XK_KP_6
#define VK_NUMPAD7 XK_KP_7
#define VK_NUMPAD8 XK_KP_8
#define VK_NUMPAD9 XK_KP_9

#define VK_HOME  XK_Home     
#define VK_END   XK_End      
#define VK_LEFT  XK_Left     
#define VK_RIGHT XK_Right    
#define VK_UP    XK_Up       
#define VK_DOWN  XK_Down     
#define VK_PRIOR XK_Prior //XK_Page_Up
#define VK_NEXT  XK_Next // XK_Page_Down    

GLboolean LoadGLTexture(GLuint* a, TCHAR b[]);
// MessageBox(void* , TEXT("2: Cannot Open The Desired File\n"), TEXT("Error"), MB_OK);

Bool Load3DFonts(const char* fontName);

void Draw_Text(const char* text, size_t length);


#elif _WIN32

// windows code goes here
// windows header
#include <Windows.h>
#include <strsafe.h>

// Open GL headers
#include <GL/glew.h>
#include <GL/gl.h>
#include "common.h"

BOOL LoadGLTexture_UsingBitMap(GLuint* texture, TCHAR imageRecId[]);

BOOL LoadGLTexture(GLuint* texture, TCHAR imageResourceID[]);

BOOL Load3DFonts(const char* fontName);

void Draw_Text(const char* text, size_t length);

#else

#endif

// macros

// internal headers
#include "geometrytypes.h"

// typedefs
// typedef VERTEX TRANFORM;

// external variables

// drawing helpers

GLboolean LoadGLTexture_Cubemap(GLuint* textureid, const char* path);

GLboolean LoadGLTexture_UsingSOIL(GLuint* texture, const char* path);
GLboolean LoadGLTextureData_UsingSOIL(TEXTURE* texture, const char* path);

