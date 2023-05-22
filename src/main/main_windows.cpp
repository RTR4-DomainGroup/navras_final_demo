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

#include <windowsx.h>	// for mouse move x and y coordinates

// #include "../../inc/template_ogl.h"

// OpenGL header files
#include <GL/glew.h>
#include <GL/gl.h>

#include "../../inc/helper/constants.h"
#include "../../inc/helper/resources.h"
#include "../../inc/helper/common.h"
#include "../../inc/Navras.h"


// OpenGL libraries
// is same as C:\> link.exe Traingle.obj OpenGL32.lib blah.lib ... /SUBSYTEM:WINDOWS
// By law it not mandatory that is should before OpenGL
#pragma comment(lib, "GLEW32.lib") 
#pragma comment(lib, "OpenGL32.lib")
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
// include drawing headers


// global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// global variable declarations
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;
BOOL gbFullscreen = FALSE;
BOOL gbActiveWindow = FALSE;

// menu
BOOL gbLight = FALSE;

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


// external variables

// entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// function declaration
	int initialize(void);
	void display(void);
	void update(void);
	void uninitialize(void);
	void ToggleFullscreen(void);

	// variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("Prashant's Window");
	BOOL bDone = FALSE;
	int iRetVal = 0;

	// code

    // Variables for Center Window
    int iScrWdth = GetSystemMetrics(SM_CXSCREEN);
    int iScrHght = GetSystemMetrics(SM_CYSCREEN);

    // Window Position
    // Calculation of left top corner of our Window
    int iPosX = (iScrWdth - WIN_WIDTH) / 2;
    int iPosY = (iScrHght - WIN_HEIGHT) / 2;

\
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

	// register above wndclass
	RegisterClassEx(&wndclass);

	// create the window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("Title: "),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		iPosX,
		iPosY,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;

	// initialize()
	iRetVal = initialize();

	if(iRetVal == -1)
	{
		LOG("ChoosePixelFormat() Failed.\n");
		uninitialize();
	}
	else if(iRetVal == -2)
	{
		LOG("SetPixelFormat() Failed.\n");
		uninitialize();
	}
	else if(iRetVal == -3)
	{
		LOG("wglCreateContext() Failed.\n");
		uninitialize();
	}
	else if(iRetVal == -4)
	{
		LOG("wglMakeCurrent() Failed.\n");
		uninitialize();
	}
	else
	{
		LOG("initialize() Successful.\n");
	}

	// show window
	ShowWindow(hwnd, iCmdShow);
	// if(gbFullscreen == FALSE)
	// {
	// 	ToggleFullscreen();
	// }

	// foregrounding and focusing the window
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// game loop
	while(bDone == FALSE)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				bDone = TRUE;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if(gbActiveWindow == TRUE)
			{
				// Render the scene
				display();

				// update the scene
				update();
			}
		}
	}

	uninitialize();

	return((int)msg.wParam);

}

// callback function
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// function prototype
	void ToggleFullscreen(void);
	void resize(int, int);
	void ProcessSelection(int, int);
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
		// break;

	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		// escape key
		case VK_ESCAPE: // 27
			DestroyWindow(hwnd);
			break;
		default:
			eventHandlerNavras(iMsg, wParam);
			break;
		}
		keyPressed = wParam;
		break;

	case WM_CHAR:
		switch(wParam)
		{
		case 'F':
		case 'f':
			ToggleFullscreen();
			break;
		case 'Q':
		case 'q':
			DestroyWindow(hwnd);
			break;
		default:
			eventHandlerNavras(iMsg, wParam);
			break;
		}
		charPressed = wParam;
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_LBUTTONDOWN:
		mouseLeftClickActive = true;
		break;
	case WM_LBUTTONUP:
		mouseLeftClickActive = false;
		break;
	case WM_MOUSEMOVE:
		// variables
		if(mouseLeftClickActive == true)
		{
			mouseX = GET_X_LPARAM(lParam);
			mouseY = GET_Y_LPARAM(lParam);
		}
		break;
	default:
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullscreen(void)
{
	// variable declarations
	static DWORD dwStyle;
	static WINDOWPLACEMENT wp;
	MONITORINFO mi;

	wp.length = sizeof(WINDOWPLACEMENT);

	if(gbFullscreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);

		if(dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi.cbSize = sizeof(MONITORINFO);

			if(GetWindowPlacement(ghwnd, &wp) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, (dwStyle & ~WS_OVERLAPPEDWINDOW));
				SetWindowPos(ghwnd,
					HWND_TOP,
					mi.rcMonitor.left,
					mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left,
					mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOZORDER | SWP_FRAMECHANGED);
			}
			ShowCursor(FALSE);
			gbFullscreen = TRUE;
		}

		LOG("fullscreen Successful.\n");
	}
	else
	{
		SetWindowLong(ghwnd, GWL_STYLE, (dwStyle | WS_OVERLAPPEDWINDOW));
		SetWindowPlacement(ghwnd, &wp);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowCursor(TRUE);
		gbFullscreen = FALSE;
		LOG("fullscreen false.\n");
	}
}

int initialize(void)
{
	// function declarations
	void uninitialize(void);
	void resize(int, int);
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


	// here starts OpenGL code
	initializeReturnValue = initializeNavras();
	if(initializeReturnValue < 0)
	{
		uninitialize();
	}

	// warm-up resize()
	resize(WIN_WIDTH, WIN_HEIGHT);

	ToggleFullscreen();

	return(0);
}

void set_title(char* title)
{
	TCHAR str[255] = {};

	size_t newsize = strlen(title) + 1;
	wchar_t * wcstring = new wchar_t[newsize];

	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, title, _TRUNCATE);

    wsprintf(str, TEXT("Domain: %s"), wcstring);
    SetWindowText(ghwnd, str);

	delete[] wcstring;
}

void resize(int width, int height)
{
	// code
	LOG("resize called\n");

	resizeNavras(width, height);
}

void display(void)
{
	// function declarations

	// code
	displayNavras();

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

