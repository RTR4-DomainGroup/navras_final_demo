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
#include "../inc/scenes/scene11_ShringarRas.h"
#include "../inc/scenes/scene07_Raudra.h"

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


extern bool mouseLeftClickActive;
extern float mouseX;
extern float mouseY;

bool firstMouse = true;
float yaw = -180.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0f;
float lastY = 600.0f / 2.0f;

int winWidth;
int winHeight;

static scene_types_t currentScene = SCENE7_RAUDRA_RAS;

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
		default:
			// LOG("keypress : %d\n", wParam);
			break;
		}
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
			// LOG("keypressed : %d\n", wParam);
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
    scenePush(SCENE11_SHRINGAR_RAS);
    scenePush(SCENE10_ADBHUT_RAS);
	scenePush(SCENE7_RAUDRA_RAS);

	// samples
    //initializeTriangle();
    //initializeSphere();
	

	// currentScene = scenePop();
	// Debug
	// currentScene = SCENE_PLACEHOLDER_INDOOR;
	//currentScene = SCENE7_RAUDRA_RAS;
	// currentScene = SCENE10_ADBHUT_RAS;
	//  currentScene = SCENE11_SHRINGAR_RAS;
	currentScene = CURRENT_SCENE;

	// Scene0 - Astromedicomp video
#ifdef ENABLE_VIDEO_RENDER
	initializeQuadForVideo(); 
	if(initializeVideoEffect("res\\videos\\AMCBanner_60fps.mp4")
	) {
		LOG("initializeVideoEffect() FAILED !!!\n");
        return (-8);
	}
#endif // ENABLE_VIDEO_RENDER

	if(initializeScene_PlaceHolderOutdoor() != 0)
	{
		LOG("initializeScene_PlaceHolderOutdoor() FAILED !!!\n");
        return (-8);
	}

	if (
		SCENE_PLACEHOLDER_INDOOR == currentScene && 
		initializeScene_PlaceHolderIndoor() != 0)
	{
		LOG("initializeScene_PlaceHolderIndoor() FAILED !!!\n");
		return (-8);
	}

	if(
		SCENE7_RAUDRA_RAS == currentScene && 
		initializeScene07_Raudra() != 0)
	{
		LOG("initializeScene7_Raudra() FAILED !!!\n");
        return (-8);
	}

	if (
		SCENE10_ADBHUT_RAS == currentScene && 
		initializeScene10_AdbhutRas() != 0)
	{
		LOG("initializeScene10_AdbhutRas() FAILED !!!\n");
		return (-8);
	}

	if(
		SCENE11_SHRINGAR_RAS == currentScene && 
		initializeScene11_ShringarRas() != 0)
	{
		LOG("initializeScene11_ShringarRas() FAILED !!!\n");
        return (-8);
	}

	if (initializeParticle() != 0)
	{
		LOG("initializeParticle() FAILED !!!\n");
		return (-8);
	}


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

	return(0);
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
	if(currentScene == SCENE0_AMC_BANNER)
	{
#ifdef ENABLE_VIDEO_RENDER
		extern struct FSQuadUniform fsqUniform;

		fsqUniform = useFSQuadShader();
		displayVideoEffect(&fsqUniform);
		glUseProgram(0);
#endif	
	}
	else if (currentScene == SCENE11_SHRINGAR_RAS)
	{
		isGodRequired = true;
		isWaterRequired = false;
		isGaussianBlurRequired = false;
		displayScene_PlaceHolderOutdoor(displayScene11_ShringarRas, isGodRequired, isWaterRequired, isGaussianBlurRequired);
	}
	else if(currentScene == SCENE10_ADBHUT_RAS)
	{
		isGodRequired = true;
		isWaterRequired = false;
		isGaussianBlurRequired = false;
		displayScene_PlaceHolderOutdoor(displayScene10_Passes, isGodRequired, isWaterRequired, isGaussianBlurRequired);
	}
	else if(currentScene == SCENE7_RAUDRA_RAS)
	{
		displayScene07_Raudra();
	}
	else if (currentScene == SCENE_PLACEHOLDER_INDOOR)
	{
		displayScene_PlaceHolderIndoor();
	}
	else if (currentScene == SCENE14_PARTICLE)
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
	if(currentScene == SCENE10_ADBHUT_RAS)
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

	debug_tranformation();

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
	uninitializeScene11_ShringarRas();
	uninitializeScene10_AdbhutRas();
	uninitializeScene07_Raudra();
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

