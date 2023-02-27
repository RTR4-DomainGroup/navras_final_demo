// Header Files
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include"include/OGL.h"			// For Icon
#include"include/Sphere.h"
#include"include/common.h"

// OpenGL Header Files
#include<GL/glew.h>		// THIS MUST BE BEFORE gl.h
#include<GL/gl.h>
#include"include/vmath.h"
using namespace vmath;

// OpenGL Libraries
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "../lib/Sphere.lib")

#define WIN_WIDTH  800
#define WIN_HEIGHT  600

// Global Function Declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Variable Declarations
HWND ghwnd = NULL;
BOOL gbFullScreen = FALSE;
BOOL gbActiveWindow = FALSE;
FILE* gpFile = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	// Function Declarations

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
	if (fopen_s(&gpFile, "Log.txt", "w") != 0) {

		MessageBox(NULL, TEXT("Creation Of Log.txt File Failed. Exiting..."), TEXT("File I/O Error."), MB_OK);
		exit(0);

	}
	else {

		fprintf(gpFile, "Log File SuccessFully Created!!!\n");
	}

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
		TEXT("OPEN GL OM MANOHAR WARKE!!!"),
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

		case 27:
			DestroyWindow(hwnd);
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

void resize(int width, int height) {

	// Code
	if (height == 0)			// To Avoid Divided by 0(in Future)
		height = 1;

}
