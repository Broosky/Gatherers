/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Program Name: Gatherers (C)                                                                                            //
//  Author: Jeffrey Bednar                                                                                                 //
//  Copyright: Illusion Interactive, 2011                                                                                  //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Development notes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// - Windows XP does not like the progress bar control created by WinAsm studio. I replaced it with a general label system.
// - Put simple variables that are accessed via dereferencing onto the stack.
// - I no longer use the original taskbar image. Remove it from the loading process.
// - When the process terminates, the operating system will destroy the image objects automatically.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MAIN_C_
#define _MAIN_C_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "..\Headers\main.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    char szCurrentWorkingDirectory[MAX_PATH];
    if (GetCurrentDirectory(MAX_PATH, szCurrentWorkingDirectory)) {
        printf("Current working directory: %s\n", szCurrentWorkingDirectory);
    }
    p_Globals = (GLOBALS*)GLOBALS_Create();
    p_Settings = (SETTINGS*)SETTINGS_Create(p_Globals);
    p_Images = (IMAGES*)IMAGES_Create(p_Globals);
    p_Menu = (MENU*)MENU_Create(p_Globals);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetConsoleTitle(APP_NAME);
    srand(GetTickCount());
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    WNDCLASSEX WndClassEx;
    ZeroMemory(&WndClassEx, sizeof(WNDCLASSEX));
    WndClassEx.cbSize = sizeof(WNDCLASSEX);
    WndClassEx.style = 0;
    WndClassEx.lpfnWndProc = WINPROC_WindowProc;
    WndClassEx.cbClsExtra = 0;
    WndClassEx.cbWndExtra = 0;
    WndClassEx.hInstance = hInstance;
    WndClassEx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(ICO_MAIN));
    WndClassEx.hIconSm = WndClassEx.hIcon;
    WndClassEx.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS));
    WndClassEx.hbrBackground = CreateSolidBrush(RGB(100, 100, 100));
    WndClassEx.lpszMenuName = NULL;
    WndClassEx.lpszClassName = APP_NAME;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    RegisterClassEx(&WndClassEx);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DialogBoxParam(hInstance, MAKEINTRESOURCE(DLG_LOAD), NULL, WINPROC_DlgLoad, (LPARAM)WndClassEx.hIcon);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    (*p_Menu).hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(MAIN_MENU));
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GLOBALS_Init(p_Globals, hInstance);
    MENU_Init(p_Menu);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HWND hWnd = CreateWindowEx(
        0,
        APP_NAME,
        APP_NAME,
        WS_OVERLAPPEDWINDOW,
        100,
        100,
        600,
        400,
        NULL,
        (*p_Menu).hMenu,
        hInstance,
        NULL
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MISC_ResizeWindow(hWnd, INITIAL_CLIENT_WIDTH, INITIAL_CLIENT_HEIGHT);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_DblBuf = DBLBUF_Create(hWnd, p_Globals);
    DBLBUF_SetBlitter(p_DblBuf, &(*p_Images).Blitter[0]);
    DBLBUF_Clear(p_DblBuf, RGB(0, 0, 0));
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ShowWindow(hWnd, nCmdShow);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TIMEBASE* p_Process = TIMEBASE_Create(PROCESS_FPS, p_Globals);
    TIMEBASE* p_ProcessAverage = TIMEBASE_Create(1.0f, p_Globals);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    RENDER_Init(p_DblBuf);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CARD* p_Card = (CARD*)CARD_Create(p_Globals, p_Images);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MSG Msg;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while(1) {
        if(PeekMessage(&Msg, hWnd, 0, 0, PM_REMOVE)) {
            if(Msg.message == WM_QUIT) {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Destroy everything the program uses.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                ENTITY_DeleteAll(p_Globals);
                MESSAGE_DeleteAll(p_Globals);
                DBLBUF_Kill(p_DblBuf, p_Globals);
                IMAGES_Kill(p_Images, p_Globals);
                TIMEBASE_Kill(p_Process, p_Globals);
                TIMEBASE_Kill(p_ProcessAverage, p_Globals);
                CARD_Kill(p_Card, p_Globals);
                SETTINGS_Kill(p_Settings, p_Globals);
                MENU_Kill(p_Menu, p_Globals);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                DestroyMenu((*p_Menu).hMenu);
                DestroyWindow(hWnd);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                int iRemainingHeap = GLOBALS_Kill(p_Globals);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                if(iRemainingHeap) {
                    printf("Warning: Remaining heap does not equal zero (%d Bytes).\n", iRemainingHeap);
                }
                else {
                    printf("Heap successfully freed.\n");
                }
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                Sleep(1500);
                break;
            }
            else {
                TranslateMessage(&Msg);
                DispatchMessage(&Msg);
            }
        }
        else {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if(TIMEBASE_Tick(p_Process)) {
                PROC_ProcessScene(p_DblBuf, p_Globals, p_Images, p_Card, p_Menu);
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if(TIMEBASE_Tick(p_ProcessAverage)) {
                (*p_Globals).iSecondTick++;
                (*p_Globals).fFramesPerSecond = (float)(*p_Globals).iFrameCount / (*p_Globals).iSecondTick;
                (*p_Globals).fProcessTimeAverage = (*p_Globals).fProcessTimeSum * 1000.0f / (*p_Globals).iSecondTick;
            }
        }
    }
    return Msg.wParam;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
