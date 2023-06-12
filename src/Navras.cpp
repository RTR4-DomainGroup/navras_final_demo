// Header Files
#include <time.h>

#include "../inc/helper/common.h"
#include "../inc/helper/geometry.h"
#include "../inc/helper/shaders.h"
#include "../inc/helper/camera.h"
#include "../inc/helper/framebuffer.h"
#include "../inc/helper/sceneStack.h"
#include "../inc/helper/audioplayer.h"

#include "../inc/effects/AtmosphereEffect.h"
#include "../inc/effects/ParticelEffect.h"

#include "../inc/shaders/FSQuadShader.h"
#include "../inc/shaders/ParticleShader.h"
#include "../inc/shaders/FontShader.h"

#include "../inc/scenes/scenes.h"
#include "../inc/scenes/scenePlaceHolderOutdoor.h"
#include "../inc/scenes/scenePlaceHolderIndoor.h"
#include "../inc/scenes/scene02_EarthAndSpace.h"
#include "../inc/scenes/scene05_karun.h"
#include "../inc/scenes/scene06_BhayanakRas.h"
#include "../inc/scenes/scene07_Raudra.h"
#include "../inc/scenes/scene08_BibhatsaRas.h"
#include "../inc/scenes/scene09_VeerRas.h"
#include "../inc/scenes/scene10_AdbhutRas.h"
#include "../inc/scenes/scene11_ShringarRas.h"
#include "../inc/scenes/scene12_Hasya.h"
#include "../inc/scenes/scene13_Shant.h"
#include "../inc/scenes/fontRendering.h"
#include "../inc/effects/videoEffect.h"

#include "../inc/Navras.h"

#include "../inc/debug/debug_transformation.h"

#define _USE_MATH_DEFINES 1
#include <math.h>		// for PI

// audio
bool gbPlayback = false;

mat4 perspectiveProjectionMatrix;

// framebuffer related variables
int windowWidth;
int windowHeight;

// extern GLbyte charPressed;
// extern GLuint keyPressed;
extern bool mouseLeftClickActive;
extern float mouseX;
extern float mouseY;

float yaw = -180.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0f;
float lastY = 600.0f / 2.0f;

int winWidth;
int winHeight;


// Indoor Gaussian Blur
bool shouldSceneRaudraMaskAppear = false;

// Time
bool timeFlag = true;
time_t now;
time_t then;

#ifdef SHORTS
int time_scene1 = 5;
int time_scene2 = 10;
int time_scene3 = 10;
int time_scene4 = 10;
int time_scene5 = 10;
int time_scene6 = 10;
int time_scene7 = 10;
int time_scene8 = 10;
int time_scene9 = 10;
int time_scene10 = 10;
int time_scene11 = 10;
int time_scene12 = 10;
int time_scene13 = 10;
int time_scene14 = 10;

int blurTime = 3;
#else
int time_scene1 = 5;
int time_scene2 = 40;
int time_scene3 = 40;
int time_scene4 = 40;
int time_scene5 = 40;
int time_scene6 = 40;
int time_scene7 = 40;
int time_scene8 = 40;
int time_scene9 = 40;
int time_scene10 = 40;
int time_scene11 = 40;
int time_scene12 = 40;
int time_scene13 = 40;
int time_scene14 = 40;
int blurTime = 10;
#endif


// Audio
static bool audioFlag = true;

//
static scene_types_t currentScene = SCENE_INVALID;

bool sceneFadeOut = false;

#ifdef ENABLE_ATMOSPHERE
extern AtmosphericVariables atmosVariables;
extern AtmosphericVariables atmosVariables_11;
#endif

void QuitApplication(void);

scene_types_t  getCurrentScene(void)
{
	return (currentScene);
}

int eventHandlerNavras(unsigned int iMsg, int wParam) {

	// Function Declarations
	void resize(int, int);
	int playSong(int);
	void togglePlayback();
	void resetCamera(void);

	// variables
	static int songId; 
    GLbyte charPressed = 0;
    GLuint keyPressed = 0;
	
	// Code
	switch (iMsg) {
	case WM_SETFOCUS:
		togglePlayback();
		break;
	case WM_KILLFOCUS:
		togglePlayback();
		break;		
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_SPACE:
			// playSong(songId);
			togglePlayback();
			break;
		default:
			// LOG("keypress : %d\n", wParam);
			break;
		}
		keyPressed = wParam;
		debug_tranformation(charPressed, keyPressed);
		break;

	case WM_CHAR:
		switch (wParam) {

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

		case '[':
			currentScene = scenePrev();
			LOG("current scene changed: %d\n", currentScene);
			break;	
		case ']':	
			currentScene = sceneNext();
			LOG("current scene changed: %d\n", currentScene);
			break;

#ifdef ENABLE_ATMOSPHERE
		 case '1':
		 case '!':
		 	if (wParam == '!')
		 		atmosVariables_11.m_Kr = max(0.0f, atmosVariables_11.m_Kr - 0.0001f);
		 	else
		 		atmosVariables_11.m_Kr += 0.0001f;
		 	atmosVariables_11.m_Kr4PI = atmosVariables_11.m_Kr * 4.0f * M_PI;
		 	break;

		 case '2':
		 case '@':
		 	if (wParam == '@')
		 		atmosVariables_11.m_Km = max(0.0f, atmosVariables_11.m_Km - 0.0001f);
		 	else
		 		atmosVariables_11.m_Km += 0.0001f;
		 	atmosVariables_11.m_Km4PI = atmosVariables_11.m_Km * 4.0f * M_PI;
		 	break;

		 case '3':
		 case '#':
		 	if (wParam == '#')
		 		atmosVariables_11.m_g = max(-1.0f, atmosVariables_11.m_g - 0.001f);
		 	else
		 		atmosVariables_11.m_g = min(1.0f, atmosVariables_11.m_g + 0.001f);
		 	break;

		 case '4':
		 case '$':
		 	if (wParam == '$')
		 		atmosVariables_11.m_ESun = max(0.0f, atmosVariables_11.m_ESun - 0.1f);
		 	else
		 		atmosVariables_11.m_ESun += 0.1f;
		 	break;

		 case '5':
		 case '%':
		 	if (wParam == '%')
		 		atmosVariables_11.m_fWavelength[0] = max(0.001f, atmosVariables_11.m_fWavelength[0] -= 0.001f);
		 	else
		 		atmosVariables_11.m_fWavelength[0] += 0.001f;
		 	atmosVariables_11.m_fWavelength4[0] = powf(atmosVariables_11.m_fWavelength[0], 4.0f);
		 	break;

		 case '6':
		 case '^':
		 	if (wParam == '^')
		 		atmosVariables_11.m_fWavelength[1] = max(0.001f, atmosVariables_11.m_fWavelength[1] -= 0.001f);
		 	else
		 		atmosVariables_11.m_fWavelength[1] += 0.001f;
		 	atmosVariables_11.m_fWavelength4[1] = powf(atmosVariables_11.m_fWavelength[1], 4.0f);
		 	break;

		 case '7':
		 case '&':
		 	if (wParam == '&')
		 		atmosVariables_11.m_fWavelength[2] = max(0.001f, atmosVariables_11.m_fWavelength[2] -= 0.001f);
		 	else
		 		atmosVariables_11.m_fWavelength[2] += 0.001f;
		 	atmosVariables_11.m_fWavelength4[2] = powf(atmosVariables_11.m_fWavelength[2], 4.0f);
		 	break;

		 case '8':
		 case '*':
		 	if (wParam == '*')
		 		atmosVariables_11.m_fExposure = max(0.1f, atmosVariables_11.m_fExposure - 0.1f);
		 	else
		 		atmosVariables_11.m_fExposure += 0.1f;
		 	break;

		case '/':
			LOG("-----------------------------------------------------------------------------------------------\n");
			LOG("m_nSamples = %d\n", atmosVariables_11.m_nSamples);
			LOG("m_Kr = %f\n", atmosVariables_11.m_Kr);
			LOG("m_Kr4PI = %f\n", atmosVariables_11.m_Kr4PI);
			LOG("m_Km = %f\n", atmosVariables_11.m_Km);
			LOG("m_Km4PI = %f\n", atmosVariables_11.m_Km4PI);
			LOG("m_ESun = %f\n", atmosVariables_11.m_ESun);
			LOG("m_g = %f\n", atmosVariables_11.m_g);
			LOG("m_fExposure = %f\n", atmosVariables_11.m_fExposure);
			
			LOG("m_fInnerRadius = %f\n", atmosVariables_11.m_fInnerRadius);
			LOG("m_fOuterRadius = %f\n", atmosVariables_11.m_fOuterRadius);
			LOG("m_fScale = %f\n", atmosVariables_11.m_fScale);
			
			LOG("m_fWavelength[0] = %f\n", atmosVariables_11.m_fWavelength[0]);
			LOG("m_fWavelength[1] = %f\n", atmosVariables_11.m_fWavelength[1]);
			LOG("m_fWavelength[2] = %f\n", atmosVariables_11.m_fWavelength[2]);
			LOG("m_fWavelength4[0] = %f\n", atmosVariables_11.m_fWavelength4[0]);
			LOG("m_fWavelength4[1] = %f\n", atmosVariables_11.m_fWavelength4[1]);
			LOG("m_fWavelength4[2] = %f\n", atmosVariables_11.m_fWavelength4[2]);

			LOG("m_fRayleighScaleDepth = %f\n", atmosVariables_11.m_fRayleighScaleDepth);
			LOG("m_fMieScaleDepth = %f\n", atmosVariables_11.m_fMieScaleDepth);
			LOG("-----------------------------------------------------------------------------------------------\n");
			break;

#endif

		default:
			// LOG("keypressed : %d\n", wParam);
			break;
		}
		charPressed = wParam;
		debug_tranformation(charPressed, keyPressed);
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
	void sceneTime(int);

	// Variable Declarations

	// Code


	// Here starts OpenGL code
    // GLEW initialization
    // codes related to PP requires Core profile
    // if(glewInit() != GLEW_OK)
    // {
    //     LOG("Error: glewInit() failed\n");
    //     return (-5);
    // }

	// Print OpenGLInfo
	printGLInfo();

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
	

	if(initializeScene_PlaceHolderOutdoor() != 0)
	{
		LOG("initializeScene_PlaceHolderOutdoor() FAILED !!!\n");
        return (-8);
	}

	if (initializeScene_PlaceHolderIndoor() != 0)
	{
		LOG("initializeScene_PlaceHolderIndoor() FAILED !!!\n");
		return (-8);
	}

#ifdef ENABLE_SINGLE_SCENE

	switch (CURRENT_SCENE) {
		case SCENE02_EARTH_AND_SPACE:
			if(initializeScene02_EarthAndSpace() != 0)
			{
				LOG("initializeScene02_EarthAndSpace() FAILED !!!\n");
				return (-8);
			}
			break;	
		case SCENE05_KARUN_RAS:
			if(initializeScene5_karun() != 0)
			{
				LOG("initializeScene5_karun() FAILED !!!\n");
				return (-8);
			}
			break;	
		case SCENE06_BHAYANK_RAS:
			if(initializeScene06_BhayanakRas() != 0)
			{
				LOG("initializeScene02_EarthAndSpace() FAILED !!!\n");
				return (-8);
			}
			break;	
		case SCENE07_RAUDRA_RAS:
			if(	initializeScene07_Raudra() != 0)
			{
				LOG("initializeScene7_Raudra() FAILED !!!\n");
				return (-8);
			}
			break;	
		case SCENE08_BIBHATSA_RAS:
			if(		initializeScene08_BibhatsaRas() != 0)
			{
				LOG("initializeScene08_BibhatsaRas() FAILED !!!\n");
				return (-8);
			}
			break;	
		case SCENE09_VEER_RAS:
			if(initializeScene09_VeerRas() != 0)
			{
				LOG("initializeScene09_VeerRas() FAILED !!!\n");
				return (-8);
			}
			break;	
		case SCENE10_ADBHUT_RAS:
			if(	initializeScene10_AdbhutRas() != 0)
			{
				LOG("initializeScene10_AdbhutRas() FAILED !!!\n");
				return (-8);
			}
			break;	
		case SCENE11_SHRINGAR_RAS:
			if(initializeScene11_ShringarRas() != 0)
			{
				LOG("initializeScene11_ShringarRas() FAILED !!!\n");
				return (-8);
			}
			break;	
		case SCENE12_HASYA_RAS:
			if(initializeScene12_Hasya() != 0)
			{
				LOG("initializeScene12_Hasya() FAILED !!!\n");
				return (-8);
			}
			break;	
		case SCENE13_SHANT_RAS:
			if(initializeScene13_Shant() != 0)
			{
				LOG("initializeScene13_Shant() FAILED !!!\n");
				return (-8);
			}
			break;	
		case SCENE14_PARTICLE:
			if(initializeParticle() != 0)
			{
				LOG("initializeParticle() FAILED !!!\n");
				return (-8);
			}
			break;	
		default:
			LOG("invalid scene %d !!!\n", CURRENT_SCENE);
			return (-8);
	}

	scenePush(CURRENT_SCENE);

	currentScene = scenePop();
	LOG("current scene changed: %d\n", currentScene);

#else

	// SCENE02
	if (initializeScene02_EarthAndSpace() != 0)
	{
		LOG("initializeScene02_EarthAndSpace() FAILED !!!\n");
		return (-8);
	}

	// SCENE05
	if (initializeScene5_karun() != 0)
	{
		LOG("initializeScene5_karun() FAILED !!!\n");
		return (-8);
	}

	// SCENE06
	if (initializeScene06_BhayanakRas() != 0)
	{
		LOG("initializeScene02_EarthAndSpace() FAILED !!!\n");
		return (-8);
	}

	//// SCENE07
	if (initializeScene07_Raudra() != 0)
	{
		LOG("initializeScene7_Raudra() FAILED !!!\n");
		return (-8);
	}

	//// SCENE08
	if (initializeScene08_BibhatsaRas() != 0)
	{
		LOG("initializeScene08_BibhatsaRas() FAILED !!!\n");
		return (-8);
	}

	// SCENE09
	if (initializeScene09_VeerRas() != 0)
	{
		LOG("initializeScene09_VeerRas() FAILED !!!\n");
		return (-8);
	}

	// SCENE10
	if (initializeScene10_AdbhutRas() != 0)
	{
		LOG("initializeScene10_AdbhutRas() FAILED !!!\n");
		return (-8);
	}

	// SCENE11
	if (initializeScene11_ShringarRas() != 0)
	{
		LOG("initializeScene11_ShringarRas() FAILED !!!\n");
		return (-8);
	}

	// SCENE12
	if (initializeScene12_Hasya() != 0)
	{
		LOG("initializeScene12_Hasya() FAILED !!!\n");
		return (-8);
	}

	// SCENE13
	if (initializeScene13_Shant() != 0)
	{
		LOG("initializeScene13_Shant() FAILED !!!\n");
		return (-8);
	}

	////LOG("initializeScene13_Shant() DONE !!!\n");

	// SCENE14
	 if (initializeParticle() != 0)
	 {
	 	LOG("initializeParticle() FAILED !!!\n");
	 	return (-8);
	 }

	scenePush(MAX_SCENES);
	scenePush(SCENE14_PARTICLE);
	scenePush(SCENE13_SHANT_RAS);
	////scenePush(SCENE12_HASYA_RAS);
	scenePush(SCENE11_SHRINGAR_RAS);
	scenePush(SCENE10_ADBHUT_RAS);
	scenePush(SCENE09_VEER_RAS);
	scenePush(SCENE08_BIBHATSA_RAS);

	scenePush(SCENE07_RAUDRA_RAS);
	scenePush(SCENE06_BHAYANK_RAS);
	scenePush(SCENE05_KARUN_RAS);
	scenePush(SCENE02_EARTH_AND_SPACE);

	currentScene = scenePop();
	LOG("current scene changed: %d\n", currentScene);

#endif


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

	sceneTime(time_scene2);

	return(0);
}

void printGLInfo(void) {

	// Local Variable Declarations
	GLint numExtensions = 0;

	// Code
	// ***** Writing Graphics Card Related Info in Log File  ***** //
	LOG("   **********************************************************\n");
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

	// for (int i = 0; i < numExtensions; i++)
	// {
	// 	LOG("   %s \n", glGetStringi(GL_EXTENSIONS, i));
	// }
	// LOG("**********************************************************\n");
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
	// Function Declarations 
	void sceneTime(int);
	void audio(int);
	
	bool isGodRequired = false;
	bool isWaterRequired = false;
	bool isGaussianBlurRequired = false;

	// Function declarations
	void resize(int, int);

	// Code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Call Scenes Display Here
	if ( currentScene == SCENE02_EARTH_AND_SPACE)
	{
		audio(SCENE02_EARTH_AND_SPACE);

		isGodRequired = true;
		isWaterRequired = false;
		isGaussianBlurRequired = false;
		shouldSceneRaudraMaskAppear = now >= ((then + time_scene2) - blurTime);
		if (shouldSceneRaudraMaskAppear)
		{
			isGaussianBlurRequired = true;
		}
		displayScene_PlaceHolderOutdoor(setCameraScene02_EarthAndSpace, displayScene02_EarthAndSpace, isGodRequired, isWaterRequired, isGaussianBlurRequired);
		sceneTime(time_scene2);
	}
	else if (currentScene == SCENE05_KARUN_RAS)
	{
		isGaussianBlurRequired = false;
		shouldSceneRaudraMaskAppear = now >= ((then + time_scene5) - blurTime);
		if (shouldSceneRaudraMaskAppear)
		{
			isGaussianBlurRequired = true;
		}

		audio(SCENE05_KARUN_RAS);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		displayScene_PlaceHolderIndoor(setCameraScene05_karun, displayScene5_karun, shouldSceneRaudraMaskAppear);
		sceneTime(time_scene5);
	}
	else if (currentScene == SCENE06_BHAYANK_RAS)
	{
		audio(SCENE06_BHAYANK_RAS);
		isGaussianBlurRequired = false;
		shouldSceneRaudraMaskAppear = now >= ((then + time_scene6) - blurTime);
		if (shouldSceneRaudraMaskAppear)
		{
			isGaussianBlurRequired = true;
		}
		
		isGodRequired = false;
		isWaterRequired = true;
		
		displayScene_PlaceHolderOutdoor(setCameraScene06_BhyanakRas, displayScene06_BhayanakRas, isGodRequired, isWaterRequired, isGaussianBlurRequired);
		sceneTime(time_scene6);
	}
	else if (currentScene == SCENE07_RAUDRA_RAS)
	{
		shouldSceneRaudraMaskAppear = now >= ((then + time_scene7) - blurTime);
		
		audio(SCENE07_RAUDRA_RAS);
		displayScene_PlaceHolderIndoor(setCameraScene07_RaudraRas, displayScene07_Raudra, shouldSceneRaudraMaskAppear);
		//displayScene07_Raudra();
		sceneTime(time_scene7);
	}
	else if (currentScene == SCENE08_BIBHATSA_RAS)
	{
		audio(SCENE08_BIBHATSA_RAS);
		isGaussianBlurRequired = false;
		shouldSceneRaudraMaskAppear = now >= ((then + time_scene8) - blurTime);
		if (shouldSceneRaudraMaskAppear)
		{
			isGaussianBlurRequired = true;
		}
		isGodRequired = false;
		isWaterRequired = true;
		displayScene_PlaceHolderOutdoor(setCameraScene08, displayScene08_Passes, isGodRequired, isWaterRequired, isGaussianBlurRequired);
		sceneTime(time_scene8);
	}
	else if (currentScene == SCENE09_VEER_RAS)
	{
		audio(SCENE09_VEER_RAS);
		isGaussianBlurRequired = false;
		shouldSceneRaudraMaskAppear = now >= ((then + time_scene9) - blurTime);
		if (shouldSceneRaudraMaskAppear)
		{
			isGaussianBlurRequired = true;
		}

		isGodRequired = false;
		isWaterRequired = false;
		displayScene_PlaceHolderOutdoor(setCameraScene09_VeerRas, displayScene09_VeerRas, isGodRequired, isWaterRequired, isGaussianBlurRequired);
		sceneTime(time_scene9);
	}
	else if (currentScene == SCENE10_ADBHUT_RAS)
	{
		audio(SCENE10_ADBHUT_RAS);
		isGaussianBlurRequired = false;
		shouldSceneRaudraMaskAppear = now >= ((then + time_scene10) - blurTime);
		if (shouldSceneRaudraMaskAppear)
		{
			isGaussianBlurRequired = true;
		}
		isGodRequired = true;
		isWaterRequired = true;
		displayScene_PlaceHolderOutdoor(setCameraScene10, displayScene10_Passes, isGodRequired, isWaterRequired, isGaussianBlurRequired);
		sceneTime(time_scene10);
	}
	else if (currentScene == SCENE11_SHRINGAR_RAS)
	{
		audio(SCENE11_SHRINGAR_RAS);
		isGaussianBlurRequired = false;
		shouldSceneRaudraMaskAppear = now >= ((then + time_scene11) - blurTime);
		if (shouldSceneRaudraMaskAppear)
		{
			isGaussianBlurRequired = true;
		}
		
		isGodRequired = false;
		isWaterRequired = true;
		
		displayScene_PlaceHolderOutdoor(setCameraScene11_ShringarRas, displayScene11_ShringarRas, isGodRequired, isWaterRequired, isGaussianBlurRequired);
		sceneTime(time_scene11);
	}
	else if (currentScene == SCENE12_HASYA_RAS)
	{
		audio(SCENE12_HASYA_RAS);
		isGaussianBlurRequired = false;
		shouldSceneRaudraMaskAppear = now >= ((then + time_scene12) - blurTime);
		if (shouldSceneRaudraMaskAppear)
		{
			isGaussianBlurRequired = true;
		}
		
		displayScene12_Hasya();
		sceneTime(time_scene12);
	}
	else if (currentScene == SCENE13_SHANT_RAS)
	{
		shouldSceneRaudraMaskAppear = false;
		
		audio(SCENE13_SHANT_RAS);
		displayScene_PlaceHolderIndoor(setCameraScene13_ShantRas, displayScene13_Shant, shouldSceneRaudraMaskAppear);
		//displayScene13_Shant();
		sceneTime(time_scene13);
	}
	else if (currentScene == SCENE14_PARTICLE)
	{
		audio(SCENE14_PARTICLE);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		displayParticle();
		sceneTime(time_scene14);
	}
	else if (currentScene == SCENE_PLACEHOLDER_INDOOR)
	{
		//displayScene_PlaceHolderIndoor();
	}
	else
	{
		audio(SCENE_INVALID);
		LOG("current scene changed: %d\n", currentScene);
		currentScene = SCENE_INVALID;
		QuitApplication();
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
		LOG("current scene changed: %d\n", currentScene);
		sceneFadeOut = false;
	} 

	// Call Scenes Update Here
	if (currentScene == SCENE02_EARTH_AND_SPACE)
	{
		updateScene_PlaceHolderOutdoor();
		updateScene02_EarthAndSpace();
	}
	else if (currentScene == SCENE05_KARUN_RAS)
	{
		updateScene5_karun();
		updateScene_PlaceHolderIndoor();
	}
	else if (currentScene == SCENE06_BHAYANK_RAS)
	{
		updateScene_PlaceHolderOutdoor();
		updateScene06_BhayanakRas();
	}
	else if (currentScene == SCENE07_RAUDRA_RAS)
	{
		updateScene07_RaudraRas();
		updateScene_PlaceHolderIndoor();
	}
	else if (currentScene == SCENE08_BIBHATSA_RAS)
	{
		updateScene_PlaceHolderOutdoor();
		updateScene08_BibhatsaRas();
	}
	else if (currentScene == SCENE09_VEER_RAS)
	{
		updateScene_PlaceHolderOutdoor();
		updateScene09_VeerRas();
	}
	else if(currentScene == SCENE10_ADBHUT_RAS)
	{
		updateScene_PlaceHolderOutdoor();
		updateScene10_AdbhutRas();
	}
	else if (currentScene == SCENE11_SHRINGAR_RAS)
	{
		updateScene_PlaceHolderOutdoor();
		updateScene11_ShringarRas();
	}
	else if (currentScene == SCENE12_HASYA_RAS)
	{
		updateScene_PlaceHolderIndoor();
	}
	else if (currentScene == SCENE13_SHANT_RAS)
	{
		updateScene13_ShantRas();
	}	
	else if (currentScene == SCENE_PLACEHOLDER_INDOOR)
	{
		updateScene_PlaceHolderIndoor();
	}
	

	// camera movement related updates
	updateMouseMovement();

}

void sceneTime(int scenetime){

	// Code
#ifndef ENABLE_SINGLE_SCENE

	if (timeFlag == true) {
		then = time(NULL);
		timeFlag = false;
	}

#ifdef AUTOSWITCH_SCENE
	now = time(NULL);
#endif

	if (now == (then + scenetime))
	{
		then = time(NULL);
		currentScene = scenePop();
		LOG("current scene changed: %d\n", currentScene);

		audioFlag = true;
	}
	
#endif // !ENABLE_SINGLE_SCENE

}

void audio(int scene){

	// function declaration
	int playSong(int);


	// code
#ifdef ENABLE_AUDIO

	if (audioFlag) {
		playSong(scene);
		audioFlag = false;
	}

#endif // ENABLE_AUDIO

}

void uninitializeNavras(void) {

	// Function Declarations

	// Code
	//LOG("Enter\n");

	// audio
	uninitializeAudio();
	LOG("Audio uninitialized\n");

#ifdef ENABLE_SINGLE_SCENE
	// only single scene pushed
	currentScene = scenePop();
#else
	while (SCENE_INVALID != currentScene)
	{
		currentScene = scenePop();
	}
	
#endif // !ENABLE_SINGLE_SCENE

	//uninitialize all scenes
	uninitializeParticle();
	uninitializeScene11_ShringarRas();
	uninitializeScene09_VeerRas();
	uninitializeScene10_AdbhutRas();
	uninitializeScene08_BibhatsaRas();
	uninitializeScene07_Raudra();
	uninitializeScene06_BhayanakRas();
	uninitializeScene5_karun();
	uninitializeScene02_EarthAndSpace();
	uninitializeScene_PlaceHolderIndoor();
	uninitializeScene_PlaceHolderOutdoor();
	LOG("All scenes uninitialized\n");


	//uninitialize all shaders
	uninitializeAllShaders();
	LOG("All shaders uninitialized\n");

}

void updateMouseMovement(void)
{
	static bool firstMouse = true;
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
