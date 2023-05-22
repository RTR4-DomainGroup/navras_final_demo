/**
 * @file main_xwindows.cpp
 * @author Prashant (adlinge.pa@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

 // standard headers
#include <stdio.h>  // for standard io
#include <stdlib.h> // for exit()
#include <memory.h> // for memset()

// X11 headers - 11th version of x-server
#include <X11/Xlib.h> // similar to Wndoes.h- 
#include <X11/Xutil.h> // XVisualInfo
#include <X11/XKBlib.h>

// OpenGL header files
#include <GL/glew.h>    // for OpenGL functionality
#include <GL/gl.h>    // for OpenGL functionality
#include <GL/glx.h>   // for Bridging APIs

// internal headers
#include "../../inc/helper/common.h"
#include "../../inc/Navras.h"

// macros


// global variables
Display* display = NULL;
XVisualInfo* visualInfo = NULL; // 10 members struct
Colormap colormap;
Window window;
int screenWidth;
int screenHeight;
Bool fullscreen = False;

// keys
GLbyte charPressed;
GLuint keyPressed;

// mouse Interaction 
Bool mouseLeftClickActive = False;
// mouse position
float mouseX;
float mouseY;

// OpenGL related variables
// ARB = arch review board
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;
GLXFBConfig glxFBConfig; 
GLXContext glxContext;

// DefaultScreen *defaultScreen = NULL;
int main(void)
{
    // function declarations
    void toggleFullscreen(void);
    // openGL 
    int initialize(void);
    void resize(int, int);
    void update(void);
    void draw(void);
    void uninitialize(void);


    // local variables
    int defaultScreen;
    int defaultDepth;
    GLXFBConfig* glxFBConfigs = NULL;
    GLXFBConfig bestGLXFBConfigs;
    XVisualInfo *tmpXVisualInfo = NULL;
    int numFBConfigs;
    // Status status;
    XSetWindowAttributes windowAttributes;
    int styleMask;
    Atom wm_delete_window_atom;
    XEvent event;
    KeySym keySym;
    char keys[26];
    Bool bActiveWindow = True;
    static int winWidth, winHeight;
    int retval;

    // OpenGL 
    // added PP related FB attributes
    static int frameBufferAttributes[] = // similar to pixelFormatAttributes in Windows
    {
        GLX_X_RENDERABLE, True,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT, // glxcreatecontext cha second param
        GLX_RENDER_TYPE, GLX_RGBA_BIT, 
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_STENCIL_SIZE, 8,
        GLX_DEPTH_SIZE, 24,
        GLX_DOUBLEBUFFER, True,
        None // XLib style 0 / null termination 
    };
    Bool bDone = False;


    // code

    // first xlib call
    display = XOpenDisplay(NULL); // XServer cha display agent avaialble karun deto
    if(display == NULL)
    {
        LOG("Error: XOpenDisplay() failed");
        uninitialize();
        exit(1);
    }

    // 
    defaultScreen = XDefaultScreen(display);

    defaultDepth = XDefaultDepth(display, defaultScreen);

    // OpenGL - choose the visualinfo closest matching with frameBufferAttributes
    // There are 3 visual xmatchvisualinfo, glXChooseVisual, 
    // we are doing the code of glXChooseVisual, but for to match the PP
    glxFBConfigs = glXChooseFBConfig(
        display,
        defaultScreen,
        frameBufferAttributes, // FB atrrib mazya fb la support karanara pahije
        &numFBConfigs
    );
    if(glxFBConfigs == NULL)
    {
        LOG("glXChooseFBConfig() failed.\n");
        uninitialize();
        exit(1);
    }
    LOG("found numFBConfigs are %d\n", numFBConfigs);

    // find the best FB config from the array
    int bestFrameBufferConfig = -1;
    int worstFrameBufferConfig = -1;
    int bestNumberOfSamples = -1;
    int worstNumberOfSamples = 999; // 999 just a number
    for(int i = 0; i < numFBConfigs; i++)
    {
        tmpXVisualInfo = glXGetVisualFromFBConfig(display, glxFBConfigs[i]);
        if(tmpXVisualInfo != NULL)
        {
            int samples, samplebuffers;
            glXGetFBConfigAttrib(
                display,
                glxFBConfigs[i],
                GLX_SAMPLE_BUFFERS,
                &samplebuffers
            );
            glXGetFBConfigAttrib(
                display,
                glxFBConfigs[i],
                GLX_SAMPLES,
                &samples
            );

            LOG("VisualInfo:0x%0lX found samplebuffers: %d samples: %d\n", tmpXVisualInfo->visualid, samplebuffers, samples);

            if(bestFrameBufferConfig < 0 || samplebuffers && samples > bestNumberOfSamples)
            {
                bestFrameBufferConfig = i;
                bestNumberOfSamples = samples;
            }
            if(worstFrameBufferConfig < 0 || !samplebuffers || samples < worstNumberOfSamples)
            {
                worstFrameBufferConfig = i;
                worstNumberOfSamples = samples;
            }
        }
        XFree(tmpXVisualInfo);
        tmpXVisualInfo = NULL;
    }

    bestGLXFBConfigs = glxFBConfigs[bestFrameBufferConfig];
    glxFBConfig = bestGLXFBConfigs;
    XFree(glxFBConfigs);
    glxFBConfigs = NULL;

    // retrive the visualinfo
    LOG("Visual id of best visualinfo\n");
    visualInfo = glXGetVisualFromFBConfig(display, bestGLXFBConfigs);
    if(visualInfo == NULL)
    {
        LOG("Error: glXChooseVisual() failed \n");
        uninitialize();
        exit(1);
    }
    LOG("Visual id of best visualinfo is :0x%lu\n", visualInfo->visualid);


    memset(&windowAttributes, 0, sizeof(XSetWindowAttributes));
    windowAttributes.border_pixel = 0; // default
    windowAttributes.background_pixel = XBlackPixel(display, defaultScreen);
    windowAttributes.background_pixmap = 0; // background picture
    windowAttributes.colormap = XCreateColormap(
        display,
        RootWindow(display, visualInfo->screen), // default screen was also but we not sure default screen color is TrueColor 
        visualInfo->visual,
        AllocNone
    ); // 2^32 colors

    windowAttributes.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask | 
                FocusChangeMask|ButtonPressMask | Button1MotionMask;

    // initail
    colormap = windowAttributes.colormap;

    styleMask = CWBorderPixel | CWBackPixel | CWColormap | CWEventMask;

    window = XCreateWindow(
        display,
        RootWindow(display, visualInfo->screen),
        100,
        100,
        WIN_WIDTH,
        WIN_HEIGHT,
        0, // border width
        visualInfo->depth,
        InputOutput,
        visualInfo->visual,
        styleMask,
        &windowAttributes
    );

    if(!window)
    {
        printf("Error: XCreateWindow() failed \n");
        uninitialize();
        exit(1);
    }

    XStoreName(display, window, "PAA: OGL PP");

    wm_delete_window_atom = XInternAtom(
        display,
        "WM_DELETE_WINDOW", // Window Manage (Not Window Mwssage)
        True
    );

    XSetWMProtocols(
        display,
        window,
        &wm_delete_window_atom,
        1
    );

    XMapWindow(display, window);


    // centering of Window
    screenWidth = XWidthOfScreen(
        XScreenOfDisplay(display, defaultScreen)
    );

    screenHeight = XHeightOfScreen(
        XScreenOfDisplay(display, defaultScreen)
    );

    XMoveWindow(
        display,
        window,
        (screenWidth / 2 - WIN_WIDTH / 2),
        (screenHeight / 2 - WIN_HEIGHT / 2)
    );

    // call to initialize
    retval = initialize();
    if(retval != 0)
    {
        LOG("Initialize OpenGL failed\n");
        exit(1);
    }
    else
    {
        LOG("Initialize OpenGL successfull\n");
    }

    // message loop
    while(bDone == False)
    {
        // XPending - PeekMessage in Windows - only when there is message
        while(XPending(display))
        {
            // GetNextEvent - in Windows
            XNextEvent(display, &event);
            switch(event.type)
            {
            case MapNotify: // similiar to WM_Create
                break;
            case ButtonPress:
                switch (event.xbutton.button)
                {
                case Button1: // XK_Pointer_Button1:
                    // left mouse button
                    mouseLeftClickActive = true;
                    mouseX = (float)event.xbutton.x;
                    mouseY = (float)event.xbutton.y;
                    printf("ButtonPress: left mouse button clicked at (%.02f,%.02f)\n", 
                    mouseX, mouseY);
                    break;
                case Button2:
                    // middle mouse button clicked
                    printf("ButtonPress: middle mouse button clicked \n");
                    break;
                case Button3:
                    // right mouse button clicked
                    printf("ButtonPress: right mouse button clicked \n");
                    break;
                }
                break;
            case ButtonRelease:
                switch (event.xbutton.button)
                {
                case Button1:
                    mouseLeftClickActive = false;
                    break;
                
                default:
                    break;
                }    
                break;
            case MotionNotify:
                break;
            case KeyPress:
                keySym = XkbKeycodeToKeysym(
                    display,
                    event.xkey.keycode,
                    0, // not key code group
                    0 // pressing along with shift
                ); // 
                switch(keySym)
                {
                case XK_Escape:
                    bDone = True;
                    break;
                default:
                    eventHandlerNavras(WM_KEYDOWN, keySym);
                    break;    
                }
                keyPressed = keySym;

                // fills the array of 
                XLookupString(&event.xkey, keys, sizeof(keys), NULL, NULL);
                switch(keys[0])
                {
                case 'F':
                case 'f':
                    if(False == fullscreen)
                    {
                        toggleFullscreen();
                        fullscreen = True;
                    }
                    else
                    {
                        toggleFullscreen();
                        fullscreen = False;
                    }
                    break;
                default:
                    LOG("keypressed : %c\n", keys[0]);
                    eventHandlerNavras(WM_CHAR, keys[0]);
                    break;
                }
                charPressed = keys[0];
                break;
                // WM_SIZE
            case ConfigureNotify:
                winWidth = event.xconfigure.width;
                winHeight = event.xconfigure.height;
                resize(winWidth, winHeight);
                break;
            case Expose:
                break;
                // wm activate / set focus
            case FocusIn:
                // window has got focus
                bActiveWindow = True;
                break;
                // wm activate / set focus
            case FocusOut:
                // window has loose focus
                bActiveWindow = False;
                break;
            case 33:
                bDone = True;
                break;
            } // switch
        } // while XPending

        if(bActiveWindow == True)
        {
            draw();

            // Here there should be call for update();
            update();
        }
    } // outer while

    uninitialize();

    return (0);
}

void toggleFullscreen(void)
{
    // local variables
    Atom wm_current_state_atom;
    Atom wm_fullscreen_state_atom;
    XEvent event;

    // code
    wm_current_state_atom = XInternAtom(
        display,
        "_NET_WM_STATE", // Network complient and Window Manager complient
        False   // do not create if already exist
    );

    wm_fullscreen_state_atom = XInternAtom(
        display,
        "_NET_WM_STATE_FULLSCREEN",
        False
    );

    memset(&event, 0, sizeof(XEvent));
    event.type = ClientMessage;
    event.xclient.window = window;
    event.xclient.message_type = wm_current_state_atom;
    event.xclient.format = 32;
    event.xclient.data.l[0] = fullscreen ? 0 : 1;
    event.xclient.data.l[1] = wm_fullscreen_state_atom;

    XSendEvent(
        display,
        RootWindow(display, visualInfo->screen),
        False, // not to send to childrens
        SubstructureNotifyMask,
        &event
    );
}

int initialize(void)
{
    // Function declararions 
    void resize(int, int);
    void uninitialize(void);
    void printGLInfo(void);

    //code
    // will give the address of function 
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((GLubyte*)"glXCreateContextAttribsARB");
    if(glXCreateContextAttribsARB == NULL)
    {
        LOG("glXGetProcAddressARB() failed\n");
        uninitialize();
        exit(-1);
    }

    GLint contextAttributes[] = {
        // OpenGL version
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3, 
        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
        None
    };

    glxContext = glXCreateContextAttribsARB(
        display,
        glxFBConfig,
        NULL, // glxContext* - This is for sharing glxContext in case multiple displays connected
        True, // Hardware rendring support from native driver -(not SW renderer Vesa / novau) (WARP - Windows Advance Rasterization Platform 
        contextAttributes
    );
    if(!glxContext)
    {
        // fallback code
        GLint contextAttributes[] = {
            // OpenGL version
            GLX_CONTEXT_MAJOR_VERSION_ARB, 1,
            GLX_CONTEXT_MINOR_VERSION_ARB, 0,
            None
        };

        glxContext = glXCreateContextAttribsARB(
        display,
            glxFBConfig,
        NULL, // glxContext* - This is for sharing glxContext in case multiple displays connected
            True, // Hardware rendring support from native driver -(not SW renderer Vesa / novau) (WARP - Windows Advance Rasterization Platform 
            contextAttributes
    );
        LOG("cannot support 4.6, hence falling back to default version\n");
    }
    else
    {
        LOG("found the support to 4.6 version\n");
    }

    // checking whether direct(hardware) rendering is supported
    if(!glXIsDirect(display, glxContext))
    {
        LOG("Direct(HW) rendering is not supported\n");
    }
    else
    {
        LOG("Direct(HW) rendering is supported\n");
    }

    glXMakeCurrent(
        display,
        window,
        glxContext
    );

    // here starts OpenGL code
    int retVal = initializeNavras();
    if(retVal < 0)
    {
        uninitialize();
        return(-1);
    }

	// warm-up resize()
	resize(WIN_WIDTH, WIN_HEIGHT);

    toggleFullscreen();

    // Get the drawable
    GLXDrawable drawable = glXGetCurrentDrawable();

    // Get the GLX extension function pointer
    PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddress((const GLubyte*)"glXSwapIntervalEXT");

    if (glXSwapIntervalEXT) {
        // Set the swap interval to 1 (enable vertical sync)
        glXSwapIntervalEXT(display, drawable, 1);
    } 

    glXSwapIntervalEXT(display, drawable, 1);

    return (retVal);
}


void set_title(char* title)
{
    char str[255] = {};
    sprintf(str, "PAA: %s", title);
    XStoreName(display, window, str);
}

void resize(int width, int height)
{
    // code
    resizeNavras(width, height);
}

void draw(void)
{
    // code
    displayNavras();

    glXSwapBuffers(display, window);
}

void update(void)
{
    // function declarations

    // code
    updateNavras();
}

void uninitialize(void)
{
    // code
    LOG("%s(%d): %s: \n", __FILE__, __LINE__, __FUNCTION__);
    GLXContext currentContext;
    currentContext = glXGetCurrentContext();
    if(currentContext && currentContext == glxContext)
    {
        glXMakeCurrent(display, 0, 0);
    }
    if(fullscreen)
    {
        toggleFullscreen();
    }

    // openGl
    uninitializeNavras();
    if(glxContext)
    {
        glXDestroyContext(display, glxContext);
        glxContext = NULL;
    }

    if(visualInfo)
    {
        free(visualInfo);
        visualInfo = NULL;
    }

    if(window)
    {
        XDestroyWindow(display, window);
    }

    if(colormap)
    {
        XFreeColormap(display, colormap);
    }

    if(display)
    {
        XCloseDisplay(display);
        display = NULL;
    }

    LOG("%s(%d): %s: Successful\n", __FILE__, __LINE__, __FUNCTION__);

    LOG("log file successfully closed.\n");
    log_close();
}

