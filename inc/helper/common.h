#pragma once

// standard headers
#include <stdio.h>
#include <stdlib.h>

// Open GL headers
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../../inc/helper/assimp_glm_helpers.h"

#include "resources.h"
#include "vmath.h"
#include "constants.h"

using namespace vmath;

enum {

	DOMAIN_ATTRIBUTE_POSITION = 0,
	DOMAIN_ATTRIBUTE_INSTANCE_POSITION,
	DOMAIN_ATTRIBUTE_COLOR,
	DOMAIN_ATTRIBUTE_NORMAL,
	DOMAIN_ATTRIBUTE_TEXTURE0,
	DOMAIN_ATTRIBUTE_TANGENT,
	DOMAIN_ATTRIBUTE_BITANGENT,
	DOMAIN_ATTRIBUTE_BONE_ID,
	DOMAIN_ATTRIBUTE_BONE_WEIGHT
};

struct TextureVariables {

	GLuint albedo;				// Albedo is Diffuse
	GLuint normal;
	GLuint ao;
	GLuint metallic;
	GLuint roughness;
	GLuint displacement;

	char* albedoPath;
	char* normalPath;
	char* aoPath;
	char* metallicPath;
	char* roughnessPath;
	char* displacementPath;

};


#ifdef __linux__ 

//linux code goes here

// X11 headers - 11th version of x-server
#include <X11/Xutil.h> // XVisualInfo

// Open GL headers
#include <GL/gl.h>
// dummy macro
#define MAKEINTRESOURCE(a) (int *)a
#define TCHAR int
#define MB_OK
#define TEXT(a) a

#define TEXTURE_DIR   "res/textures/"

#define AUDIO_DIR     "res/audios/"

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


#elif _WIN32

// windows code goes here
// windows header
#include <Windows.h>
#include <strsafe.h>

#define TEXTURE_DIR "res\\textures\\"
#define AUDIO_DIR "res\\audios\\"

// #define PATH_SEPARATOR '\\'
#define PATH_SEPARATOR '/'

#else

#endif

// macros
// #define file_printf fprintf
// #define file_close fclose

// internal headers
// #include "types.h"
// #include "debug_transformation.h"

// logging helpers
int log_open(char const* FileName, char const* Mode);
int log_printf(char const* const filewithpath, char const* const funcname, int linenum, char const* const format, ...);
int log_close();

char* currentDateTime(char* log_buffer);
char* vararg2string(const char* format, ...);

// #define LOG(print_buff) \
// 	log_printf_novararg( __FILE__, __FUNCTION__, __LINE__,  vararg2string(print_buff))

#define LOG(format, ...) \
	log_printf(__FILE__, __FUNCTION__, __LINE__, format,  ##__VA_ARGS__)
	
