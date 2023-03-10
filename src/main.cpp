#include <Windows.h>
#include <GL/glew.h>
#include <stdint.h>

struct GameHandler
{ 
    HINSTANCE hInstance; 
    HWND windowHandle;
}Game{};

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

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{ 
    MSG msg;
    BOOL bRet; 
    WNDCLASS wc{}; 
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
 
    Game.hInstance = hInstance;
 
    // Create the main window.
    uint32_t WindowCreateFlags = WS_OVERLAPPEDWINDOW;
    Game.windowHandle = CreateWindow(TEXT("MainWndClass"), TEXT("Sample"), 
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
        CW_USEDEFAULT, CW_USEDEFAULT, (HWND) NULL, 
        (HMENU) NULL, Game.hInstance, (LPVOID) NULL); 
 
    // If the main window cannot be created, terminate 
    // the application. 
    if (!Game.windowHandle) 
        return FALSE; 
 
    // Show the window and paint its contents. 
    ShowWindow(Game.windowHandle, nCmdShow); 
    UpdateWindow(Game.windowHandle); 
 
    // Start the message loop. 
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

    } 
 
    // Return the exit code to the system. 
    return msg.wParam; 
} 