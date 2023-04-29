// Header Files
#include "../inc/helper/common.h"
#include "../inc/helper/shaders.h"
#include "../inc/scenes/scenes.h"
#include "../inc/helper/camera.h"
#include "../inc/helper/audioplayer.h"
#include "../inc/scenes/scenePlaceHolder.h"

// OpenGL Libraries
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "assimp-vc142-mtd.lib")

#define WIN_WIDTH  800
#define WIN_HEIGHT  600

// Global Function Declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Variable Declarations
HWND ghwnd = NULL;
BOOL gbFullScreen = FALSE;
BOOL gbActiveWindow = FALSE;

// audio
BOOL gbPlayback = FALSE;

// FILE* gpFile = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;

mat4 perspectiveProjectionMatrix;

// framebuffer related variables
int windowWidth;
int windowHeight;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	// Function Declarations
	int initialize(void);
	void display(void);
	void update(void);
	void uninitialize(void);


	// Variable Declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyWindow!!!");
	BOOL bDone = FALSE;
	int iRetVal = 0;

	int iDesktopWidth, iDesktopHeight;
	int iWCoorx, iWCoory;

	// Code
	//if (fopen_s(&gpFile, "Log.txt", "w") != 0) {

	//	MessageBox(NULL, TEXT("Creation Of Log.txt File Failed. Exiting..."), TEXT("File I/O Error."), MB_OK);
	//	exit(0);

	//}
	//else {

	//	LOG("Log File SuccessFully Created!!!\n");
	//}

	// Initialisation Of WNDCLASSEX Structure
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	// Register WNDCLASSEX
	RegisterClassEx(&wndclass);

	// Full Screen Code
	iDesktopWidth = GetSystemMetrics(SM_CXSCREEN);
	iDesktopHeight = GetSystemMetrics(SM_CYSCREEN);

	iWCoorx = ((iDesktopWidth / 2) - (WIN_WIDTH / 2));
	iWCoory = ((iDesktopHeight / 2) - (WIN_HEIGHT / 2));

	// Create The Window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW, 
		szAppName,
		TEXT("DOMAIN's NAVRAS!!!"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		iWCoorx,
		iWCoory,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;

	// Show Window
	ShowWindow(hwnd, iCmdShow);

	//
	iRetVal = initialize();
	if(iRetVal < 0)
	{

		LOG("Initialize() FAILED!!!\n");
		uninitialize();
		return(-1);

	}

	// Foregrounding And Focusing the Window
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// Game Loop
	while (bDone == FALSE) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

			if (msg.message == WM_QUIT)
				bDone = TRUE;
			else {

				TranslateMessage(&msg);
				DispatchMessage(&msg);

			}

		}
		else {

			if (gbActiveWindow == TRUE) {

				display();

				update();

			}

		}

	}

	return((int)msg.wParam);

}


// CAllBack Function
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	// Function Declarations
	void ToggleFullScreen(void);
	void resize(int, int);
	int playSong(int );
	void togglePlayback();


	// variables
	static int songId; 

	// Code
	switch (iMsg) {

	case WM_SETFOCUS:
		gbActiveWindow = TRUE;
		break;

	case WM_KILLFOCUS:
		gbActiveWindow = FALSE;
		break;

	case WM_ERASEBKGND:
		return(0);

	case WM_KEYDOWN:

		switch (wParam) {

		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;

		case VK_SPACE:
			// playSong(songId);
			togglePlayback();
			break;

		default:
			break;
		}
		break;

	case WM_CHAR:
		switch (wParam) {

		case 'F':
		case 'f':
			ToggleFullScreen();
			break;
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
		default:
			break;

		}
		break;

	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;

	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));

}

int playSong(int songId)
{
	// variable
	static int lastSongId = -1;
 
	// code
	// if(gbPlayback && lastSongId == songId) 
	// {
	// 	pauseAudio();
	// 	gbPlayback = FALSE;
	// }
	// else if (!gbPlayback && lastSongId == songId)
	// {
	// 	resumeAudio();
	// 	gbPlayback = TRUE;
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
		gbPlayback = FALSE;
	}
	else
	{
		resumeAudio();
		gbPlayback = TRUE;
	} 
}

int initialize(void) {

	// Function Declarations
	void ToggleFullScreen(void);
	void resize(int, int);
	BOOL LoadGLTexture(GLuint*, TCHAR[]);
	void printGLInfo(void);
	void uninitialize(void);

	// Variable Declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;

	// Code
	// Initialization of PIXELFORMATDESCRIPTOR
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));		//MemSet((void *)&pfd, NULL, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;

	// GetDC
	ghdc = GetDC(ghwnd);

	// Choose Pixel Format
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPixelFormatIndex == 0) {
	
		return(-1);

	}

	// Set The Choosen Pixel Format
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
		return(-2);

	// Create OpenGL Rendering Context
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
		return(-3);

	// Make The Rendering Context As The Running Context
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
		return(-4);

	// glew initialisation
	if (glewInit() != GLEW_OK)
		return(-5);

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

	// Initialize Scenes


	if(initializeScene_PlaceHolder() != 0)
	{
		LOG("initializeScene_PlaceHolder() FAILED !!!\n");
        return (-8);
	}

	// initialize camera
	cameraEyeX = 0.0f;
	cameraEyeY = 0.0f;
	cameraEyeZ = 6.0f;

	cameraCenterX = 0.0f;
	cameraCenterY = 0.0f;
	cameraCenterZ = 0.0f;

	cameraUpX = 0.0f;
	cameraUpY = 1.0f;
	cameraUpZ = 0.0f;

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

	ToggleFullScreen();

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

void ToggleFullScreen(void) {

	// Variable Declarations
	static DWORD dwStyle;
	static WINDOWPLACEMENT wp;
	MONITORINFO mi;

	// Code
	wp.length = sizeof(WINDOWPLACEMENT);

	if (gbFullScreen == FALSE) {

		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW) {

			mi.cbSize = sizeof(MONITORINFO);
			if (GetWindowPlacement(ghwnd, &wp) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi)) {

				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);

			}

			ShowCursor(FALSE);
			gbFullScreen = TRUE;
		}
	}
	else {

		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wp);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowCursor(TRUE);
		gbFullScreen = FALSE;

	}

}

void display(void)
{
	// Code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set camera
	setCamera();

	// Call Scenes Display Here
	displayScene_PlaceHolder();

	SwapBuffers(ghdc);

}

void update(void)
{

	// Code
	
	// Call Scenes Update Here
	updateScene_PlaceHolder();
}

void resize(int width, int height) {

	// Code
	if (height == 0)			// To Avoid Divided by 0(in Future)
		height = 1;

	windowWidth = width;
	windowHeight = height;

        // 
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / height, 0.1f, 1000.0f);


}

void uninitialize(void) {

	// Function Declarations
	void ToggleFullScreen(void);

	// Code

	// audio
	uninitializeAudio();

	//uninitialize all scenes
	uninitializeScene_PlaceHolder();

	//uninitialize all shaders
	uninitializeAllShaders();

	if (gbFullScreen) {

		ToggleFullScreen();

	}

	// 
	if (wglGetCurrentContext() == ghrc) {

		wglMakeCurrent(NULL, NULL);

	}

	if (ghrc) {

		wglDeleteContext(ghrc);
		ghrc = NULL;

	}

	if (ghdc) {

		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;

	}

	if (ghwnd) {

		DestroyWindow(ghwnd);
		ghwnd = NULL;

	}

	//if (gpFile) {

	//	fprintf(gpFile, "Log File Close!!!\n");
	//	fclose(gpFile);
	//	gpFile = NULL;

	//}

}
