/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _WINDOWS_MAIN_H_
#define _WINDOWS_MAIN_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../common.h"
#include "windows_minified.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declares:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct LOG LOG_T;
typedef struct CARD CARD_T;
typedef struct MENU MENU_T;
typedef struct ASSETS ASSETS_T;
typedef struct GLOBALS GLOBALS_T;
typedef struct SETTINGS SETTINGS_T;
typedef struct RENDERER RENDERER_T;
typedef struct CONSTANTS CONSTANTS_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Externs for WinMain, WindowProc, and DlgLoad:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern LOG_T* p_Log;
extern MENU_T* p_Menu;
extern ASSETS_T* p_Assets;
extern GLOBALS_T* p_Globals;
extern SETTINGS_T* p_Settings;
extern CONSTANTS_T* p_Constants;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void                __cdecl     MAIN_LaunchConsole                              (void);
int                 CALLBACK    WinMain                                         (_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow);
void                __cdecl     MAIN_ConsiderEngine                             (RENDERER_T*, CARD_T*, GLOBALS_T*, ASSETS_T*, MENU_T*, SETTINGS_T*, CONSTANTS_T*, LOG_T*);
void                __cdecl     MAIN_HandleQuit                                 (RENDERER_T*, CARD_T*, GLOBALS_T*, ASSETS_T*, MENU_T*, SETTINGS_T*, CONSTANTS_T*, LOG_T*);
UINT                __cdecl     MAIN_FailFast                                   (GLOBALS_T*, LOG_T*);
void                __cdecl     MAIN_Finalize                                   (GLOBALS_T*, LOG_T*);
RENDERER_T*         __cdecl     MAIN_CreateRenderers                            (GLOBALS_T*, LOG_T*);
void                __cdecl     MAIN_KillRenderers                              (GLOBALS_T*);
void                __cdecl     MAIN_ConsiderMaintenance                        (LOG_T**);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
