/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../../Headers/ai.h"
#include "../../Headers/assets.h"
#include "../../Headers/card.h"
#include "../../Headers/constants.h"
#include "../../Headers/engine.h"
#include "../../Headers/entity.h"
#include "../../Headers/globals.h"
#include "../../Headers/heap_allocator.h"
#include "../../Headers/load.h"
#include "../../Headers/log.h"
#include "../../Headers/menu.h"
#include "../../Headers/message.h"
#include "../../Headers/misc.h"
#include "../../Headers/path_finding.h"
#include "../../Headers/Renderer/renderer.h"
#include "../../Headers/settings.h"
#include "../../Headers/timebase.h"
#include "../../Headers/Windows/windows_main.h"
#include "../../Headers/Windows/windows_minified.h"
#include "../../Headers/Windows/windows_procedure.h"
#include <stdlib.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Define externs.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LOG_T* p_Log = NULL;
MENU_T* p_Menu = NULL;
ASSETS_T* p_Assets = NULL;
GLOBALS_T* p_Globals = NULL;
SETTINGS_T* p_Settings = NULL;
CONSTANTS_T* p_Constants = NULL;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
#if DEBUG
    // Ensure the console is available with Visual Studio (Linker: /SUBSYSTEM:WINDOWS).
    MAIN_LaunchConsole();
#endif
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals = GLOBALS_Create(NULL);
    if (!p_Globals) {
        return MAIN_FailFast(p_Globals, p_Log);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Constants = CONSTANTS_Create(p_Globals, p_Log);
    if (!p_Constants) {
        return MAIN_FailFast(p_Globals, p_Log);
    }
    CONSTANTS_Init(p_Constants);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Log = LOG_Create(p_Constants->szDefaultLogFile, p_Globals);
    if (!p_Log) {
        return MAIN_FailFast(p_Globals, p_Log);
    }
    else {
        MISC_WriteOut(p_Log, LOG_SEVERITY_TRACE, "File hook successful...\n");
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (GetCurrentDirectory(MAX_PATH, p_Globals->szCurrentWorkingDirectory)) {
        MISC_WriteOutParams(p_Log, LOG_SEVERITY_TRACE, "Current working directory: %s\n", p_Globals->szCurrentWorkingDirectory);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Settings = SETTINGS_Create(p_Globals, p_Log);
    if (!p_Settings) {
        return MAIN_FailFast(p_Globals, p_Log);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Assets = ASSETS_Create(p_Globals, p_Log);
    if (!p_Assets) {
        return MAIN_FailFast(p_Globals, p_Log);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Menu = MENU_Create(p_Globals, p_Log);
    if (!p_Menu) {
        return MAIN_FailFast(p_Globals, p_Log);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetConsoleTitle(p_Constants->szAppName);
    srand(GetTickCount64());
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    WNDCLASSEX WndClassEx;
    ZeroMemory(&WndClassEx, sizeof(WNDCLASSEX));
    WndClassEx.cbSize = sizeof(WNDCLASSEX);
    WndClassEx.style = 0;
    WndClassEx.lpfnWndProc = WINDOWS_PROCEDURE_WindowProc;
    WndClassEx.cbClsExtra = 0;
    WndClassEx.cbWndExtra = 0;
    WndClassEx.hInstance = hInstance;
    WndClassEx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(ICO_MAIN));
    WndClassEx.hIconSm = WndClassEx.hIcon;
    WndClassEx.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS));
    WndClassEx.hbrBackground = CreateSolidBrush(RGB(100, 100, 100));
    WndClassEx.lpszMenuName = NULL;
    WndClassEx.lpszClassName = p_Constants->szAppName;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    RegisterClassEx(&WndClassEx);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Menu->hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(MAIN_MENU));
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!SETTINGS_InitFromFile(p_Settings, p_Constants, p_Log, p_Constants->szDefaultSettingsFile)) {
        MAIN_FailFast(p_Globals, p_Log);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GLOBALS_Init(p_Globals, hInstance, p_Settings);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MENU_Init(p_Menu);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->hWnd = CreateWindowEx(
        0,
        p_Constants->szAppName,
        p_Constants->szAppName,
        WS_OVERLAPPEDWINDOW,
        100,
        100,
        600,
        400,
        NULL,
        p_Menu->hMenu,
        hInstance,
        NULL
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DialogBoxParam(hInstance, MAKEINTRESOURCE(DLG_LOAD), p_Globals->hWnd, LOAD_LoadAssets, (LPARAM)WndClassEx.hIcon);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MISC_ResizeWindow(p_Globals->hWnd, p_Settings->usInitialClientWidth, p_Settings->usInitialClientHeight, p_Globals);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->p_ActiveRenderer = MAIN_CreateRenderers(p_Globals, p_Log);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    float fEngineTimebase = p_Settings->fEngineFpsTarget;
    p_Globals->p_Engine = TIMEBASE_Create(&fEngineTimebase, p_Globals, p_Log);
    if (!p_Globals->p_Engine) {
        return MAIN_FailFast(p_Globals, p_Log);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    float fSecondsTimebase = 1.0f;
    p_Globals->p_Seconds = TIMEBASE_Create(&fSecondsTimebase, p_Globals, p_Log);
    if (!p_Globals->p_Seconds) {
        return MAIN_FailFast(p_Globals, p_Log);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    float fMaintentanceTimebase = p_Settings->fMaintenanceTimebase;
    p_Globals->p_Maintenance = TIMEBASE_Create(&fMaintentanceTimebase, p_Globals, p_Log);
    if (!p_Globals->p_Maintenance) {
        return MAIN_FailFast(p_Globals, p_Log);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->p_FrameTime = TIMEBASE_Create(NULL, p_Globals, p_Log);
    if (!p_Globals->p_FrameTime) {
        return MAIN_FailFast(p_Globals, p_Log);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CARD_T* p_Card = CARD_Create(p_Globals, p_Assets, p_Log);
    if (!p_Card) {
        return MAIN_FailFast(p_Globals, p_Log);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->p_EntityAllocator = HEAP_ALLOCATOR_Create(sizeof(ENTITY_T), p_Settings->usHeapAllocatorMaxEntities, p_Globals, p_Log);
    if (!p_Globals->p_EntityAllocator) {
        return MAIN_FailFast(p_Globals, p_Log);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->p_PathPointAllocator = HEAP_ALLOCATOR_Create(sizeof(PATH_T), p_Settings->usHeapAllocatorMaxPathPoints, p_Globals, p_Log);
    if (!p_Globals->p_PathPointAllocator) {
        return MAIN_FailFast(p_Globals, p_Log);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->p_PathObstacleAllocator = HEAP_ALLOCATOR_Create(sizeof(PATH_OBSTACLE_T), p_Settings->usHeapAllocatorMaxPathObstacles, p_Globals, p_Log);
    if (!p_Globals->p_PathObstacleAllocator) {
        return MAIN_FailFast(p_Globals, p_Log);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->p_MessagesAllocator = HEAP_ALLOCATOR_Create(sizeof(MESSAGE_T), p_Settings->usHeapAllocatorMaxMessages, p_Globals, p_Log);
    if (!p_Globals->p_MessagesAllocator) {
        return MAIN_FailFast(p_Globals, p_Log);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->p_AiClosestAllocator = HEAP_ALLOCATOR_Create(sizeof(AI_CLOSEST_T), p_Settings->usHeapAllocatorAiClosest, p_Globals, p_Log);
    if (!p_Globals->p_AiClosestAllocator) {
        return MAIN_FailFast(p_Globals, p_Log);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->p_fPointAllocator = HEAP_ALLOCATOR_Create(sizeof(FPOINT_LIST_T), p_Settings->usHeapAllocatorFPoint, p_Globals, p_Log);
    if (!p_Globals->p_fPointAllocator) {
        return MAIN_FailFast(p_Globals, p_Log);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->p_fRectAllocator = HEAP_ALLOCATOR_Create(sizeof(FRECT_LIST_T), p_Settings->usHeapAllocatorFRect, p_Globals, p_Log);
    if (!p_Globals->p_fRectAllocator) {
        return MAIN_FailFast(p_Globals, p_Log);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ShowWindow(p_Globals->hWnd, nCmdShow);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MSG Msg;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (1) {
        if (PeekMessage(&Msg, p_Globals->hWnd, 0, 0, PM_REMOVE)) {
            if (Msg.message == WM_QUIT) {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                MAIN_HandleQuit(p_Globals->p_ActiveRenderer,
                    p_Card,
                    p_Globals,
                    p_Assets,
                    p_Menu,
                    p_Settings,
                    p_Constants,
                    p_Log);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                break;
            }
            else {
                TranslateMessage(&Msg);
                DispatchMessage(&Msg);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                MAIN_ConsiderEngine(p_Globals->p_ActiveRenderer,
                    p_Card,
                    p_Globals,
                    p_Assets,
                    p_Menu,
                    p_Settings,
                    p_Constants,
                    p_Log);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                MAIN_ConsiderMaintenance(&p_Log);
            }
        }
        else {
            MAIN_ConsiderEngine(p_Globals->p_ActiveRenderer,
                p_Card,
                p_Globals,
                p_Assets,
                p_Menu,
                p_Settings,
                p_Constants,
                p_Log);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            MAIN_ConsiderMaintenance(&p_Log);
        }
    }
    return Msg.wParam;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MAIN_ConsiderMaintenance(LOG_T** pp_Log) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (TIMEBASE_Tick(p_Globals->p_Maintenance)) {
        LOG_HandleBookending(pp_Log, p_Constants, p_Globals);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (!*pp_Log) {
            MAIN_FailFast(p_Globals, *pp_Log);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MAIN_ConsiderEngine(RENDERER_T* p_Renderer,
    CARD_T* p_Card,
    GLOBALS_T* p_Globals,
    ASSETS_T* p_Assets,
    MENU_T* p_Menu,
    SETTINGS_T* p_Settings,
    CONSTANTS_T* p_Constants,
    LOG_T* p_Log) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (TIMEBASE_Tick(p_Globals->p_Engine)) {
        ENGINE_ProcessScene(p_Renderer, p_Globals->p_FrameTime, p_Globals, p_Assets, p_Card, p_Menu, p_Log, p_Settings, p_Constants);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (TIMEBASE_Tick(p_Globals->p_Seconds)) {
        p_Globals->ulSecondsTick++;
        p_Globals->fFpsLongTermAverage = (float)p_Globals->ulFrameCount / p_Globals->ulSecondsTick;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main fail-fast function. Any subsequent returns are to silence null pointer warnings.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT __cdecl MAIN_FailFast(GLOBALS_T* p_Globals, LOG_T* p_Log) {
    UINT uiCode = EXIT_FAILURE;
    // Log struct could have been created, but not a log file.
    if (p_Globals && p_Log) {
        LOG_Kill(&p_Log, p_Globals);
    }
    TerminateProcess(GetCurrentProcess(), uiCode);
    // Unreachable; propagate a return to silence null pointer warnings.
    return uiCode;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MAIN_HandleQuit(RENDERER_T* p_Renderer,
    CARD_T* p_Card,
    GLOBALS_T* p_Globals,
    ASSETS_T* p_Assets,
    MENU_T* p_Menu,
    SETTINGS_T* p_Settings,
    CONSTANTS_T* p_Constants,
    LOG_T* p_Log) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Destroy everything the program uses. Note: some pointers may be null depending on what systems were fully initialized.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DestroyMenu(p_Menu->hMenu);
    DestroyWindow(p_Globals->hWnd);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ENTITY_DeleteAll(p_Globals, p_Settings, p_Log);
    MESSAGE_DeleteAll(p_Globals);
    MAIN_KillRenderers(p_Globals);
    ASSETS_Kill(&p_Assets, p_Globals, p_Constants);
    TIMEBASE_Kill(&p_Globals->p_Engine, p_Globals);
    TIMEBASE_Kill(&p_Globals->p_Seconds, p_Globals);
    TIMEBASE_Kill(&p_Globals->p_FrameTime, p_Globals);
    TIMEBASE_Kill(&p_Globals->p_Maintenance, p_Globals);
    CARD_Kill(&p_Card, p_Globals);
    SETTINGS_Kill(&p_Settings, p_Globals);
    MENU_Kill(&p_Menu, p_Globals);
    HEAP_ALLOCATOR_Kill(&p_Globals->p_EntityAllocator, p_Globals);
    HEAP_ALLOCATOR_Kill(&p_Globals->p_PathPointAllocator, p_Globals);
    HEAP_ALLOCATOR_Kill(&p_Globals->p_PathObstacleAllocator, p_Globals);
    HEAP_ALLOCATOR_Kill(&p_Globals->p_MessagesAllocator, p_Globals);
    HEAP_ALLOCATOR_Kill(&p_Globals->p_AiClosestAllocator, p_Globals);
    HEAP_ALLOCATOR_Kill(&p_Globals->p_fPointAllocator, p_Globals);
    HEAP_ALLOCATOR_Kill(&p_Globals->p_fRectAllocator, p_Globals);
    CONSTANTS_Kill(&p_Constants, p_Globals);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Last for flushing.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LOG_Kill(&p_Log, p_Globals);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Last for heap check.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MAIN_Finalize(p_Globals, p_Log);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MAIN_Finalize(GLOBALS_T* p_Globals, LOG_T* p_Log) {
    int iRemainingHeap = GLOBALS_Kill(&p_Globals);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (iRemainingHeap) {
        MISC_WriteOutParams(p_Log, LOG_SEVERITY_WARNING, "Warning: Remaining heap does not equal zero (%d Bytes).\n", iRemainingHeap);
    }
    else {
        MISC_WriteOut(p_Log, LOG_SEVERITY_TRACE, "Heap successfully freed.\n");
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Sleep(2000);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MAIN_LaunchConsole(void) {
    AllocConsole();
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FILE* p_File;
    freopen_s(&p_File, "CONOUT$", "w", stdout);
    freopen_s(&p_File, "CONOUT$", "w", stderr);
    freopen_s(&p_File, "CONIN$", "r", stdin);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RENDERER_T* __cdecl MAIN_CreateRenderers(GLOBALS_T* p_Globals, LOG_T* p_Log) {
    UINT8 ubI;
    for (ubI = 0; ubI < RENDERER_TYPE_COUNT; ubI++) {
        RENDERER_T* p_Renderer = p_Globals->p_Renderers[ubI] = RENDERER_Create((RENDERER_TYPE_T)ubI, p_Globals, p_Log);
        if (!p_Renderer) {
            MAIN_FailFast(p_Globals, p_Log);
            return NULL;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Renderer->SetBlitter(p_Renderer, &p_Assets->Blitter[BLITTER_COLOR_LIGHT_BLUE]);
        p_Renderer->Clear(p_Renderer, p_Assets);
        p_Renderer->InitWorldTransform(p_Renderer);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Use software double buffering as the default.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return p_Globals->p_Renderers[RENDERER_TYPE_DOUBLE_BUFFER];
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MAIN_KillRenderers(GLOBALS_T* p_Globals) {
    UINT8 ubI;
    for (ubI = 0; ubI < RENDERER_TYPE_COUNT; ubI++) {
        RENDERER_Kill(&p_Globals->p_Renderers[(RENDERER_TYPE_T)ubI], p_Globals);
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->eActiveRendererType = RENDERER_TYPE_UNKNOWN;
    p_Globals->p_ActiveRenderer = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
