// Header Files

#include "../inc/helper/common.h"
#include "../inc/helper/shaders.h"
#include "../inc/helper/camera.h"
#include "../inc/helper/framebuffer.h"
#include "../inc/helper/sceneStack.h"
#include "../inc/helper/audioplayer.h"

#include "../inc/effects/AtmosphereEffect.h"
#include "../inc/effects/ParticelEffect.h"

#include "../inc/shaders/FSQuadShader.h"
#include "../inc/shaders/ParticleShader.h"

#include "../inc/scenes/scenes.h"
#include "../inc/scenes/scenePlaceHolderOutdoor.h"
#include "../inc/scenes/scenePlaceHolderIndoor.h"
#include "../inc/scenes/scene10_AdbhutRas.h"
#include "../inc/scenes/scene7_Raudra.h"

#include "../inc/Navras.h"

#define _USE_MATH_DEFINES 1
#include <math.h>		// for PI

// Third party Libraries
// OpenGL libraries
// is same as C:\> link.exe Traingle.obj OpenGL32.lib blah.lib ... /SUBSYTEM:WINDOWS
// By law it not mandatory that is should before OpenGL
#pragma comment(lib, "GLEW32.lib") 
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "SOIL/lib/SOIL.lib")
#pragma comment(lib, "AL/lib/OpenAL32.lib")
#pragma comment(lib, "ffmpeg/lib/avformat.lib")
#pragma comment(lib, "ffmpeg/lib/avcodec.lib")
#pragma comment(lib, "ffmpeg/lib/avformat.lib")
#pragma comment(lib, "ffmpeg/lib/avutil.lib")
#pragma comment(lib, "ffmpeg/lib/swscale.lib")
#pragma comment(lib, "Assimp/lib/assimp-vc142-mtd.lib")


// audio
bool gbPlayback = false;

mat4 perspectiveProjectionMatrix;

// framebuffer related variables
int windowWidth;
int windowHeight;

// camera related variables for movement in scene during debugging
float cameraCounterSideWays = 3.2f;
float cameraCounterUpDownWays = 3.2f;

extern bool mouseLeftClickActive;
extern float mouseX;
extern float mouseY;

bool firstMouse = true;
float yaw = -180.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0f;
float lastY = 600.0f / 2.0f;

static scene_t currentScene = SCENE_7;

bool sceneFadeOut = false;

extern AtmosphericVariables atmosVariables;

int eventHandlerNavras(unsigned int iMsg, int wParam) {

	// Function Declarations
	void resize(int, int);
	int playSong(int );
	void togglePlayback();
	void resetCamera(void);


	// variables
	static int songId; 

	// Code
	switch (iMsg) {
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_SPACE:
			// playSong(songId);
			togglePlayback();
			break;
		case VK_UP:	// Up
			cameraCenterY = sin(cameraCounterUpDownWays) * 360.0f;
			cameraCenterZ = cos(cameraCounterUpDownWays) * 360.0f;
			cameraCounterUpDownWays += 0.025f;
			break;
		case VK_DOWN:	// down
			cameraCenterY = sin(cameraCounterUpDownWays) * 360.0f;
			cameraCenterZ = cos(cameraCounterUpDownWays) * 360.0f;
			cameraCounterUpDownWays -= 0.025f;
			break;
		case VK_LEFT:	// left
			//LOG("cameraCounterSideWays : %f\n", cameraCounterSideWays);
			cameraCenterX = sin(cameraCounterSideWays) * 360.0f;
			cameraCenterZ = cos(cameraCounterSideWays) * 360.0f;
			cameraCounterSideWays += 0.025f;
			break;
		case VK_RIGHT:	// right
			cameraCenterX = sin(cameraCounterSideWays) * 360.0f;
			cameraCenterZ = cos(cameraCounterSideWays) * 360.0f;
			cameraCounterSideWays -= 0.025f;
			break;
		default:
			LOG("keypress : %d\n", wParam);
			break;
		}
		break;

	case WM_CHAR:
		switch (wParam) {

		case 'W':
		case 'w':
			cameraEyeZ = cameraEyeZ - 0.25f;
			cameraCenterZ = cameraCenterZ - 0.25f;
			break;
		case 'S':
		case 's':
			cameraEyeZ = cameraEyeZ + 0.25f;
			cameraCenterZ = cameraCenterZ + 0.25f;
			break;
		case 'A':
		case 'a':
			cameraEyeX = cameraEyeX - 0.25f;
			cameraCenterX = cameraCenterX - 0.25f;
			break;
		case 'D':
		case 'd':
			cameraEyeX = cameraEyeX + 0.25f;
			cameraCenterX = cameraCenterX + 0.25f;
			break;
		case 'Q':
		case 'q':
			cameraEyeY = cameraEyeY - 0.25f;
			cameraCenterY = cameraCenterY - 0.25f;
			break;
		case 'E':
		case 'e':
			cameraEyeY = cameraEyeY + 0.25f;
			cameraCenterY = cameraCenterY + 0.25f;
			break;
		case 'R':
		case 'r':
			resetCamera();
			break;
		case 'P':
		case 'p':
			LOG("lookAt([%f, %f, %f], [%f, %f, %f] [%f, %f, %f]", cameraEyeX, cameraEyeY, cameraEyeZ, cameraCenterX, cameraCenterY, cameraCenterZ, cameraUpX, cameraUpY, cameraUpZ);
			break;
		case 'n':
			playSong(songId);
			songId++;
			if(songId > NUM_AUDIO-1)
				songId = 0;
			break;	
		case 'b':
			playSong(songId);
			songId--;
			if(songId < 0)
				songId = NUM_AUDIO-1;
			break;	

		case '1':
		case '!':
			if (wParam == '!')
				atmosVariables.m_Kr = max(0.0f, atmosVariables.m_Kr - 0.0001f);
			else
				atmosVariables.m_Kr += 0.0001f;
			atmosVariables.m_Kr4PI = atmosVariables.m_Kr * 4.0f * M_PI;
			break;

		case '2':
		case '@':
			if (wParam == '@')
				atmosVariables.m_Km = max(0.0f, atmosVariables.m_Km - 0.0001f);
			else
				atmosVariables.m_Km += 0.0001f;
			atmosVariables.m_Km4PI = atmosVariables.m_Km * 4.0f * M_PI;
			break;

		case '3':
		case '#':
			if (wParam == '#')
				atmosVariables.m_g = max(-1.0f, atmosVariables.m_g - 0.001f);
			else
				atmosVariables.m_g = min(1.0f, atmosVariables.m_g + 0.001f);
			break;

		case '4':
		case '$':
			if (wParam == '$')
				atmosVariables.m_ESun = max(0.0f, atmosVariables.m_ESun - 0.1f);
			else
				atmosVariables.m_ESun += 0.1f;
			break;

		case '5':
		case '%':
			if (wParam == '%')
				atmosVariables.m_fWavelength[0] = max(0.001f, atmosVariables.m_fWavelength[0] -= 0.001f);
			else
				atmosVariables.m_fWavelength[0] += 0.001f;
			atmosVariables.m_fWavelength4[0] = powf(atmosVariables.m_fWavelength[0], 4.0f);
			break;

		case '6':
		case '^':
			if (wParam == '^')
				atmosVariables.m_fWavelength[1] = max(0.001f, atmosVariables.m_fWavelength[1] -= 0.001f);
			else
				atmosVariables.m_fWavelength[1] += 0.001f;
			atmosVariables.m_fWavelength4[1] = powf(atmosVariables.m_fWavelength[1], 4.0f);
			break;

		case '7':
		case '&':
			if (wParam == '&')
				atmosVariables.m_fWavelength[2] = max(0.001f, atmosVariables.m_fWavelength[2] -= 0.001f);
			else
				atmosVariables.m_fWavelength[2] += 0.001f;
			atmosVariables.m_fWavelength4[2] = powf(atmosVariables.m_fWavelength[2], 4.0f);
			break;

		case '8':
		case '*':
			if (wParam == '*')
				atmosVariables.m_fExposure = max(0.1f, atmosVariables.m_fExposure - 0.1f);
			else
				atmosVariables.m_fExposure += 0.1f;
			break;

		default:
			LOG("keypressed : %d\n", wParam);
			break;

		}
		break;

	default:
		break;

	}

	return(0);
}

int playSong(int songId)
{
	// variable
	static int lastSongId = -1;
 
	// code
	// if(gbPlayback && lastSongId == songId) 
	// {
	// 	pauseAudio();
	// 	gbPlayback = false;
	// }
	// else if (!gbPlayback && lastSongId == songId)
	// {
	// 	resumeAudio();
	// 	gbPlayback = true;
	// }
	// else
	{
		char audiopath[64] = {0};
		snprintf(audiopath, sizeof(audiopath), "%s%s", AUDIO_DIR, szAudios[songId]);
		if(initializeAudio(audiopath))
		{
			LOG("initializeAudio() failed for file: %s\n", audiopath);
			return (-1);
		}
		playAudio();
	}
	lastSongId = songId;
	return (0);
}

void togglePlayback()
{
	// code
	if(gbPlayback) 
	{
		pauseAudio();
		gbPlayback = false;
	}
	else
	{
		resumeAudio();
		gbPlayback = true;
	} 
}

int initializeNavras(void) {

	// Function Declarations
	void resize(int, int);
	void printGLInfo(void);
	void uninitialize(void);
	void resetCamera(void);

	// Variable Declarations

	// Code
	// Here starts OpenGL code
    // GLEW initialization
    // codes related to PP requires Core profile
    if(glewInit() != GLEW_OK)
    {
        LOG("Error: glewInit() failed\n");
        return (-5);
    }

	// // Print OpenGLInfo
	// printGLInfo();

    // Calling Shaders
    if(initAllShaders())
    {
        LOG("All Shaders were successfull !!!\n");
    }
    else
    {
        LOG("All Shaders FAILED !!!\n");
        return (-6);
    }

	// Initialize Scenes
    scenePush(SCENE_10);
	scenePush(SCENE_7);
    scenePush(SCENE_3);
    scenePush(SCENE_2);
    scenePush(SCENE_1);
    scenePush(SCENE_0);


    //initializeTriangle();
    //initializeSphere();

	// Scene0 - Astromedicomp video
#ifdef ENABLE_VIDEO_RENDER
	initializeQuadForVideo(); 
	if(initializeVideoEffect("res\\videos\\AMCBanner_60fps.mp4")
	) {
		LOG("initializeVideoEffect() FAILED !!!\n");
        return (-8);
	}
#endif // ENABLE_VIDEO_RENDER

	if(initializeScene10_AdbhutRas() != 0)
	{
		LOG("initializeScene10_AdbhutRas() FAILED !!!\n");
        return (-8);
	}

	if(initializeScene_PlaceHolderOutdoor() != 0)
	{
		LOG("initializeScene_PlaceHolderOutdoor() FAILED !!!\n");
        return (-8);
	}

	if(initializeScene7_Raudra() != 0)
	{
		LOG("initializeScene7_Raudra() FAILED !!!\n");
        return (-8);
	}

	if (initializeScene_PlaceHolderIndoor() != 0)
	{
		LOG("initializeScene_PlaceHolderIndoor() FAILED !!!\n");
		return (-8);
	}

	if (initializeParticle() != 0)
	{
		LOG("initializeParticle() FAILED !!!\n");
		return (-8);
	}

	// if(initializeScene_Scene0() != 0)
	// {
	// 	LOG("initializeScene_Scene0() FAILED !!!\n");
    //     return (-8);
	// }


	// currentScene = scenePop();
	// Debug
	// currentScene = SCENE_7;
	currentScene = SCENE_10;
	// currentScene = SCENE_PLACEHOLDER_INDOOR;

	// initialize camera
	//resetCamera();

	// Here Starts OpenGL Code
	// Clear The Screen Using Blue Color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Depth Related Changes
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	// Enabling The Texture
	//glEnable(GL_TEXTURE_2D);

	perspectiveProjectionMatrix = mat4::identity();

	//set fps to system
	wglSwapIntervalEXT(1);   //0 --> will extend beyond 60

	return(0);
}

void resetCamera(void)
{
	cameraEyeX = 0.0f;
	cameraEyeY = 0.0f;
	cameraEyeZ = 6.0f;

	cameraCenterX = 0.0f;
	cameraCenterY = 0.0f;
	cameraCenterZ = 0.0f;

	cameraUpX = 0.0f;
	cameraUpY = 1.0f;
	cameraUpZ = 0.0f;

	cameraCounterSideWays = 3.2f;
	cameraCounterUpDownWays = 3.2f;
}

void printGLInfo(void) {

	// Local Variable Declarations
	GLint numExtensions = 0;

	// Code
	// ***** Writing Graphics Card Related Info in Log File  ***** //
	LOG("\n   **********************************************************\n");
	LOG("   ***** Graphics Card Information Details *****\n");
	LOG("   **********************************************************\n");
	LOG("   OpenGL Vendor	: %s \n", glGetString(GL_VENDOR));
	LOG("   OpenGL Renderer	: %s \n", glGetString(GL_RENDERER));
	LOG("   OpenGL Version	: %s \n", glGetString(GL_VERSION));
	LOG("   GLSL Version	: %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	// GLSL - Graphics Library Shading Language

	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
	LOG("   **********************************************************\n");
	LOG("   Number of Supported Extensions: %d \n", numExtensions);
	LOG("   **********************************************************\n");

	for (int i = 0; i < numExtensions; i++)
	{
		LOG("   %s \n", glGetStringi(GL_EXTENSIONS, i));
	}
	LOG("**********************************************************\n");
}

void resizeNavras(int width, int height) {

	// Code
	if (height == 0)			// To Avoid Divided by 0(in Future)
		height = 1;

	windowWidth = width;
	windowHeight = height;
	// 
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / height, 0.1f, 1000.0f);

}

void displayNavras(void)
{
	bool isGodRequired = false;
	bool isWaterRequired = false;
	bool isGaussianBlurRequired = false;

	// Function declarations
	void resize(int, int);

	// Code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Call Scenes Display Here
	if(currentScene == SCENE_0)
	{
#ifdef ENABLE_VIDEO_RENDER
		extern struct FSQuadUniform fsqUniform;

		fsqUniform = useFSQuadShader();
		displayVideoEffect(&fsqUniform);
		glUseProgram(0);
#endif	
	}
	else if(currentScene == SCENE_1)
	{
		// displayScene_Scene1();
	}
	else if(currentScene == SCENE_10)
	{
		isGodRequired = true;
		isWaterRequired = true;
		isGaussianBlurRequired = false;
		displayScene_PlaceHolderOutdoor(displayScene10_Passes, isGodRequired, isWaterRequired, isGaussianBlurRequired);
	}
	else if(currentScene == SCENE_7)
	{
		displayScene7_Raudra();
	}
	else if (currentScene == SCENE_PLACEHOLDER_INDOOR)
	{
		displayScene_PlaceHolderIndoor();
	}
	else if (currentScene == SCENE_PARTICLE)
	{
		displayParticle();
	}
	else
	{
		currentScene = SCENE_INVALID;
	}

}

void updateNavras(void)
{
	// local function declarations
	void updateMouseMovement(void);

	// Code
	// switch scene
	if(sceneFadeOut == true)
	{
		currentScene = scenePop();
		sceneFadeOut = false;
	} 

	
	// Call Scenes Update Here
	if(currentScene == SCENE_0)
	{
		// updateScene_Scene0();
	}
	else if(currentScene == SCENE_1)
	{
		// updateScene_Scene1();
	}
	else if(currentScene == SCENE_10)
	{
		updateScene_PlaceHolderOutdoor();
		updateScene10_AdbhutRas();
	}
	else if (currentScene == SCENE_PLACEHOLDER_INDOOR)
	{
		updateScene_PlaceHolderIndoor();
	}

	// camera movement related updates
	updateMouseMovement();

}

void uninitializeNavras(void) {

	// Function Declarations

	// Code

	// audio
	uninitializeAudio();

	//uninitialize all scenes
	uninitializeParticle();
	uninitializeScene_PlaceHolderOutdoor();
	uninitializeScene_PlaceHolderIndoor();
	uninitializeScene10_AdbhutRas();
	uninitializeScene7_Raudra();
	// uninitializeScene_Scene0();
	// uninitializeScene_Scene1();


	//uninitialize all shaders
	uninitializeAllShaders();

}

void updateMouseMovement(void)
{
	if (firstMouse)
	{
		lastX = mouseX;
		lastY = mouseY;
		firstMouse = false;
	}

	float xoffset = mouseX - lastX;
	float yoffset = lastY - mouseY; // reversed since y-coordinates go from bottom to top
	lastX = mouseX;
	lastY = mouseY;

	float sensitivity = 0.3f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 90.0f)
		pitch = 90.0f;
	if (pitch < -90.0f)
		pitch = -90.0f;

	if (mouseLeftClickActive == true)
	{
		cameraCenterX = cameraEyeX + cos(yaw * M_PI / 180.0f) * cos(pitch * M_PI / 180.0f);
		cameraCenterY = cameraEyeY + sin(pitch * M_PI / 180.0f);
		cameraCenterZ = cameraEyeZ + sin(yaw * M_PI / 180.0f) * cos(pitch * M_PI / 180.0f);
	}
}

