/**
 * @file main_windows.c
 * @author Prashant Adlinge (adlinge.pa@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <windowsx.h>	// for mouse move x and y coordinates
#include <condition_variable>
// #include "../../inc/template_ogl.h"

// OpenGL header files
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/gl.h>

#include "../../inc/helper/constants.h"
#include "../../inc/helper/resources.h"
#include "../../inc/helper/common.h"
#include "../../inc/effects/videoEffect.h"
#include "../../inc/Navras.h"

struct FSVQuadUniform videoUniform;

// OpenGL Libraries
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

// Global Function Declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Variable Declarations
HWND ghwnd = NULL;
BOOL gbFullscreen = FALSE;
BOOL gbActiveWindow = FALSE;
HDC ghdc = NULL;
HDC ghdc_video = NULL;
HGLRC ghrc = NULL;
HGLRC ghrc_AMC_Video = NULL;

// keys
GLbyte charPressed;
GLuint keyPressed;

// mouse Interaction 
bool mouseLeftClickActive = false;

// mouse position
float mouseX;
float mouseY;

int iScrWdth;
int iScrHght;

// Multithreading

std::atomic<bool> gTaskFinished(false);
mat4 perspectiveProjectionMatrix_AMCBanner;
static int windowWidth;
static int windowHeight;
// external variables

// entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	// Function Declarations
	int initialize(void);
	int initializeForVideo(void);
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

	// code

    // Variables for Center Window
    int iScrWdth = GetSystemMetrics(SM_CXSCREEN);
    int iScrHght = GetSystemMetrics(SM_CYSCREEN);

    // Window Position
    // Calculation of left top corner of our Window
    int iPosX = (iScrWdth - WIN_WIDTH) / 2;
    int iPosY = (iScrHght - WIN_HEIGHT) / 2;

	// initialization of WNDCLASSEX structure
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
#ifdef ENABLE_MULTI_THREADING
	// initialize()
	iRetVal = initializeForVideo();
	if(iRetVal < 0)
	{
		LOG("initializeForVideo FAILED!!!\n");
		uninitialize();
		return(-1);
	}
	LOG("initializeForVideo() Success starting Background Load !!!\n");
	std::thread first (initialize);
#else
	iRetVal = initialize();
	if(iRetVal < 0)
	{

		LOG("Initialize() FAILED!!!\n");
		uninitialize();
		return(-1);
	}
#endif
	
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

				// Render the scene
				display();				
				// update the scene
				update();
			}
		}
	}
	
	#ifdef ENABLE_MULTI_THREADING
	first.join();
	#endif
	
	uninitialize();
	return((int)msg.wParam);
}

void QuitApplication(void)
{
	PostQuitMessage(0);
}

// CAllBack Function
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	// Function Declarations
	void ToggleFullscreen(void);
	void resize(int, int);
	void uninitialize(void);

	// code
	switch(iMsg)
	{

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
		default:
			eventHandlerNavras(iMsg, wParam);
			break;
		}
		keyPressed = wParam;
		break;

	case WM_CHAR:
		switch(wParam) {
		case 'F':
		case 'f':
			ToggleFullscreen();
			break;
		default:
			eventHandlerNavras(iMsg, wParam);
			break;
		}
		charPressed = wParam;
		break;
	case WM_MOUSEMOVE:
		// variables
		mouseX = GET_X_LPARAM(lParam);
		mouseY = GET_Y_LPARAM(lParam);
		break;
	case WM_LBUTTONDOWN:
		mouseLeftClickActive = true;
		break;
	case WM_LBUTTONUP:
		mouseLeftClickActive = false;
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

int initialize(void)
{
	// function declarations
	void uninitialize(void);
	void resize(int, int);
	void ToggleFullscreen(void);

	// variable declarations
	int initializeReturnValue;
#ifndef ENABLE_MULTI_THREADING
	// variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;

	// code
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	// initialization of PIXELFORMATDESCRIPTOR structure
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

	// choose pixel format
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if(iPixelFormatIndex == 0)
	{
		return(-1);
	}

	// set the chosen pixel format
	if(SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		return(-2);
	}
			
#endif
	// create OpenGL rendering context
	ghrc = wglCreateContext(ghdc);
	if(ghrc == NULL)
	{
		return(-3);
	}

	// make the rendering context as the current context
	if(wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		return(-4);
	}
#ifndef ENABLE_MULTI_THREADING
	// glew initalization
    if (glewInit() != GLEW_OK)
    {
        return (-5);
    }
#endif
	// here starts OpenGL code
	initializeReturnValue = initializeNavras();
	if(initializeReturnValue < 0)
	{
		LOG("Calling uninitialize()");
		uninitialize();
	}

	// warm-up resize()
	resize(WIN_WIDTH, WIN_HEIGHT);

	// ToggleFullscreen();

	//set fps to system
	wglSwapIntervalEXT(1);   //0 --> will extend beyond 60
#ifdef ENABLE_MULTI_THREADING
	LOG("Setting Task finished to True \n");
	gTaskFinished = true;
#endif

	return(0);
}

int initializeForVideo(void)
{
	// function declarations
	void uninitialize(void);
	void resizeAMCBaneer(int width, int height);
	void ToggleFullscreen(void);

	// variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;
	int initializeReturnValue;

	// code
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	// initialization of PIXELFORMATDESCRIPTOR structure
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

	// choose pixel format
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if(iPixelFormatIndex == 0)
	{
		return(-1);
	}

	// set the chosen pixel format
	if(SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		return(-2);
	}

	// create OpenGL rendering context
	ghrc_AMC_Video = wglCreateContext(ghdc);
	if(ghrc_AMC_Video == NULL)
	{
		return(-3);
	}

	// make the rendering context as the current context
	if(wglMakeCurrent(ghdc, ghrc_AMC_Video) == FALSE)
	{
		return(-4);
	}

	// glew initalization
    if (glewInit() != GLEW_OK)
    {
        return (-5);
    }
	LOG("Reading VIdeo File.  \n");
	// here starts OpenGL code
	initializeVideoEffect("res\\videos\\AMCBanner_60fps.mp4");
	
	// warm-up resize()
	resizeAMCBaneer(WIN_WIDTH, WIN_HEIGHT);

	// ToggleFullscreen();

	//set fps to system
	wglSwapIntervalEXT(1);   //0 --> will extend beyond 60

	perspectiveProjectionMatrix_AMCBanner = mat4::identity();

	return(0);
}

void ToggleFullscreen(void) {

	// Variable Declarations
	static DWORD dwStyle;
	static WINDOWPLACEMENT wp;
	MONITORINFO mi;

	// Code
	wp.length = sizeof(WINDOWPLACEMENT);

	if (gbFullscreen == FALSE) {

		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW) {

			mi.cbSize = sizeof(MONITORINFO);
			if (GetWindowPlacement(ghwnd, &wp) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi)) {

				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);

			}

			ShowCursor(TRUE);		//usually kept false
			gbFullscreen = TRUE;
		}
	}
	else {

		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wp);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowCursor(TRUE);
		gbFullscreen = FALSE;

	}

}

void set_title(const char* const title)
{
	TCHAR str[MAX_LOG_LENGTH] = {};

	wsprintf(str, TEXT("%s"), title);
	SetWindowText(ghwnd, str);
}

void resize(int width, int height)
{
	// code
	resizeNavras(width, height);
}

void resizeAMCBaneer(int width, int height) {

	// Code
	if (height == 0)			// To Avoid Divided by 0(in Future)
		height = 1;

	windowWidth = width;
	windowHeight = height;
	// 
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	perspectiveProjectionMatrix_AMCBanner = vmath::perspective(45.0f, (GLfloat)width / height, 0.1f, 1000.0f);

}

void display(void)
{
	// function declarations

	// code
#ifdef ENABLE_MULTI_THREADING
	if (!gTaskFinished.load())
    {
		videoUniform = useFSVQuadShader();
		displayVideoEffect(&videoUniform);
		glUseProgram(0);
	}
	else
	{
		if (ghrc_AMC_Video)
		{
			wglDeleteContext(ghrc_AMC_Video);
			ghrc_AMC_Video = NULL;
			if (ghrc)
        	{
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				wglMakeCurrent(ghdc, ghrc);
			}		
			uninitializeVideoEffect();			
		}
		displayNavras();
	}
#else
	displayNavras();
#endif
	SwapBuffers(ghdc);
}

void update(void)
{
	// function declarations

	// code
	updateNavras();
}

void uninitialize(void)
{
	// function declarations
	void ToggleFullscreen(void);

	// code
	if(gbFullscreen)
	{
		ToggleFullscreen();
	}

	LOG("Enter\n");
	uninitializeNavras();
	if(wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	if(ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	if(ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	if(ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;
	}

	LOG("Log File closed successfully!");
	log_close();
}

