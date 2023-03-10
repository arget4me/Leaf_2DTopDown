#include <Windows.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <stdint.h>
#include <wingdi.h>
#include <iostream>

struct GameHandler
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

bool CreateDummyGLWindow(const GameHandler game)
{    
    // Must first create an opengl context to load the gl extensions needed to create a proper opengl context
    DWORD WindowCreateFlags = WS_OVERLAPPEDWINDOW | CS_OWNDC;
    HWND windowHandle = CreateWindow(TEXT("MainWndClass"), TEXT("Sample"), 
        WindowCreateFlags, CW_USEDEFAULT, CW_USEDEFAULT, 
        CW_USEDEFAULT, CW_USEDEFAULT, (HWND) NULL, 
        (HMENU) NULL, game.hInstance, (LPVOID) NULL);

    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
        PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
        32,                   // Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                   // Number of bits for the depthbuffer
        8,                    // Number of bits for the stencilbuffer
        0,                    // Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    HDC deviceContext = GetDC(windowHandle);
    int pixelFormatNumber = ChoosePixelFormat(deviceContext, &pfd);
    SetPixelFormat(deviceContext, pixelFormatNumber, &pfd);

    HGLRC renderingContext = wglCreateContext(deviceContext);
    wglMakeCurrent (deviceContext, renderingContext);

    struct CallOutOfScope{
        HGLRC renderingContext;
        CallOutOfScope(HGLRC renderingContext) : renderingContext(renderingContext) {}

        ~CallOutOfScope() {
            wglDeleteContext(renderingContext);
        }
    }defer(renderingContext);

    if(glewInit() != GLEW_OK)
    {
        OutputDebugStringA("Failed to init glew for dummy context!\n");
        return false;
    }
    
    return true;
}

bool CreateOpenGLWindow(GameHandler& game)
{
    DWORD WindowCreateFlags = WS_OVERLAPPEDWINDOW | CS_OWNDC;
    game.windowHandle = CreateWindow(TEXT("MainWndClass"), TEXT("Sample"), 
        WindowCreateFlags, CW_USEDEFAULT, CW_USEDEFAULT, 
        CW_USEDEFAULT, CW_USEDEFAULT, (HWND) NULL, 
        (HMENU) NULL, game.hInstance, (LPVOID) NULL);
    game.deviceContext = GetDC(game.windowHandle);

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
    wglChoosePixelFormatARB(game.deviceContext, pixelFormatAttributes, NULL, 1, &pixelFormatNumber, &numFormats);

    PIXELFORMATDESCRIPTOR pfd;
    DescribePixelFormat(game.deviceContext, pixelFormatNumber, sizeof(pfd), &pfd);
    SetPixelFormat(game.deviceContext, pixelFormatNumber, &pfd);

     // Specify that we want to create an OpenGL 3.3 core profile context
    int glContextAttributes[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0,
    };

    game.renderingContext = wglCreateContextAttribsARB(game.deviceContext, 0, glContextAttributes);
    if (!game.renderingContext) {
        OutputDebugStringA("Failed to create Opengl context\n");
        return false;
    }

    wglMakeCurrent (game.deviceContext, game.renderingContext);
    MessageBoxA(0,(char*)glGetString(GL_VERSION), "OPENGL VERSION",0);

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
    GameHandler game{};
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
 
    game.hInstance = hInstance;
    if(!CreateDummyGLWindow(game))
    {
        return -1;
    }

    if (!CreateOpenGLWindow(game))
    {
        return -1;
    }
 
    // If the main window cannot be created, terminate 
    // the application. 
    if (!game.windowHandle) 
        return FALSE; 
 
    // Show the window and paint its contents. 
    ShowWindow(game.windowHandle, nCmdShow); 
    UpdateWindow(game.windowHandle); 
 
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

        SwapBuffers(game.deviceContext);
    } 
 
    // Return the exit code to the system. 
    return msg.wParam; 
} 