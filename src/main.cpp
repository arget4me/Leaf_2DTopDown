#include <GL/glew.h>
#include <GL/wglew.h>
#include <stdint.h>
#include "platform.h"
#include <iostream>
#include "game.h"
#include "graphics\primitives.h"

struct WindowHandler
{ 
    HINSTANCE hInstance; 
    HWND windowHandle;
    HDC deviceContext;
    HGLRC renderingContext;
};

LRESULT WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT Result = 0;
    switch(message)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            Result = DefWindowProc(hwnd, message, wParam, lParam);
            break;
    };

    return Result;
}


bool CreateDummyGLWindow(const WindowHandler window)
{    
    // Must first create an opengl context to load the gl extensions needed to create a proper opengl context
    DWORD WindowCreateFlags = WS_OVERLAPPEDWINDOW | CS_OWNDC;
    HWND windowHandle = CreateWindow(TEXT("MainWndClass"), TEXT("Sample"), 
        WindowCreateFlags, CW_USEDEFAULT, CW_USEDEFAULT, 
        CW_USEDEFAULT, CW_USEDEFAULT, (HWND) NULL, 
        (HMENU) NULL, window.hInstance, (LPVOID) NULL);

    PIXELFORMATDESCRIPTOR pfd{};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;

    HDC deviceContext = GetDC(windowHandle);
    int pixelFormatNumber = ChoosePixelFormat(deviceContext, &pfd);
    SetPixelFormat(deviceContext, pixelFormatNumber, &pfd);

    HGLRC renderingContext = wglCreateContext(deviceContext);
    wglMakeCurrent (deviceContext, renderingContext);

    if(glewInit() != GLEW_OK)
    {
        OutputDebugStringA("Failed to init glew for dummy context!\n");
        wglDeleteContext(renderingContext);
        return false;
    }
    
    wglDeleteContext(renderingContext);
    return true;
}

bool CreateOpenGLWindow(WindowHandler& window)
{
    DWORD WindowCreateFlags = WS_OVERLAPPEDWINDOW | CS_OWNDC;
    window.windowHandle = CreateWindow(TEXT("MainWndClass"), TEXT("Sample"), 
        WindowCreateFlags, CW_USEDEFAULT, CW_USEDEFAULT, 
        CW_USEDEFAULT, CW_USEDEFAULT, (HWND) NULL, 
        (HMENU) NULL, window.hInstance, (LPVOID) NULL);
    window.deviceContext = GetDC(window.windowHandle);

    const int pixelFormatAttributes[] =
    {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        WGL_SAMPLES_ARB, 4,
        WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
        0, // End
    };

    int pixelFormatNumber;
    UINT numFormats;
    wglChoosePixelFormatARB(window.deviceContext, pixelFormatAttributes, NULL, 1, &pixelFormatNumber, &numFormats);

    PIXELFORMATDESCRIPTOR pfd;
    DescribePixelFormat(window.deviceContext, pixelFormatNumber, sizeof(pfd), &pfd);
    SetPixelFormat(window.deviceContext, pixelFormatNumber, &pfd);

     // Specify that we want to create an OpenGL 3.3 core profile context
    int glContextAttributes[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0,
    };

    window.renderingContext = wglCreateContextAttribsARB(window.deviceContext, 0, glContextAttributes);
    if (!window.renderingContext) {
        OutputDebugStringA("Failed to create Opengl context\n");
        return false;
    }

    wglMakeCurrent (window.deviceContext, window.renderingContext);

    #define FRAMEBUFFER_SRGB 0x8DB9
    #define MULTISAMPLE_ARB  0x809D

    glEnable(MULTISAMPLE_ARB);
    glEnable(FRAMEBUFFER_SRGB);

    return true;
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{ 
    MSG msg;
    BOOL bRet; 
    WNDCLASS wc{};
    WindowHandler window{};
    LEAF::GameState gameState;
    UNREFERENCED_PARAMETER(lpszCmdLine); 
 
    // Register the window class for the main window. 
    if (!hPrevInstance) 
    { 
        wc.lpfnWndProc = (WNDPROC) &WinProc;
        wc.hInstance = hInstance; 
        wc.hIcon = LoadIcon((HINSTANCE) NULL, IDI_APPLICATION); 
        wc.hCursor = LoadCursor((HINSTANCE) NULL, IDC_ARROW);
        wc.lpszMenuName = TEXT("MainMenu"); 
        wc.lpszClassName = TEXT("MainWndClass"); 
 
        if (!RegisterClass(&wc)) 
            return FALSE; 
    } 
 
    window.hInstance = hInstance;
    if(!CreateDummyGLWindow(window))
    {
        return -1;
    }

    if (!CreateOpenGLWindow(window))
    {
        return -1;
    }
 
    // If the main window cannot be created, terminate 
    // the application. 
    if (!window.windowHandle) 
        return FALSE; 
 

    LEAF::InitializeGame(gameState);

    // Show the window and paint its contents. 
    ShowWindow(window.windowHandle, nCmdShow); 
    UpdateWindow(window.windowHandle); 

    LEAF::Quad quad;
 
    // Start the message loop.
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    bool running = true;
    while(running) 
    { 
        // Query windows events
        while(PeekMessage( &msg, NULL, 0, 0, PM_REMOVE) != 0)
        {
            if(msg.message == WM_QUIT)
            {
                running = false;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        // Update the screen and render here
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, sizeof(quad.indices) / sizeof(decltype(quad.indices[0])), GL_UNSIGNED_INT, 0);
        


        SwapBuffers(window.deviceContext);
    } 
 
    // Return the exit code to the system. 
    return msg.wParam; 
} 