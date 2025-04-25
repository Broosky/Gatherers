/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _WINDOWS_PROCEDURE_H_
#define _WINDOWS_PROCEDURE_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "windows_macros.h"
#include <windows.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declares:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct DOUBLE_BUFFER DOUBLE_BUFFER_T;
typedef struct ASSETS ASSETS_T;
typedef struct GLOBALS GLOBALS_T;
typedef struct MENU MENU_T;
typedef struct LOG LOG_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Types:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef void (__cdecl *CHORD_CALLBACK)(HWND, GLOBALS_T*, MENU_T*, DOUBLE_BUFFER_T*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
INT_PTR     CALLBACK    WINDOWS_PROCEDURE_DlgLoad                       (HWND, UINT, WPARAM, LPARAM);
LRESULT     CALLBACK    WINDOWS_PROCEDURE_WindowProc                    (HWND, UINT, WPARAM, LPARAM);
void        __cdecl     WINDOWS_PROCEDURE_BuildHelper                   (USHORT, MENU_T*);
void        __cdecl     WINDOWS_PROCEDURE_BuildNone                     (GLOBALS_T*, MENU_T*);
void        __cdecl     WINDOWS_PROCEDURE_SendWorkers                   (GLOBALS_T*, ASSETS_T*);
void        __cdecl     WINDOWS_PROCEDURE_DistributeAndSendWorkers      (GLOBALS_T*, ASSETS_T*, LOG_T*);
void        __cdecl     WINDOWS_PROCEDURE_SelectEntities                (GLOBALS_T*);
void        __cdecl     WINDOWS_PROCEDURE_CreateOrCaptureEntities       (GLOBALS_T*, ASSETS_T*, LOG_T*, DOUBLE_BUFFER_T* p_DoubleBuffer);
void        __cdecl     WINDOWS_PROCEDURE_HandleKeyDown                 (HWND, WPARAM, GLOBALS_T*, MENU_T*, DOUBLE_BUFFER_T*);
UINT8       __cdecl     WINDOWS_PROCEDURE_HandleChordKeys               (HWND, WPARAM, USHORT, WPARAM);
void        __cdecl     WINDOWS_PROCEDURE_HandleArrowKeys               (HWND, WPARAM, GLOBALS_T*, MENU_T*, DOUBLE_BUFFER_T*);
void        __cdecl     WINDOWS_PROCEDURE_HandleOtherKeys               (HWND, WPARAM, GLOBALS_T*, MENU_T*, DOUBLE_BUFFER_T*);
void        __cdecl     WINDOWS_PROCEDURE_HandleMenuEvent               (HWND, WPARAM, MENU_T*, GLOBALS_T*, DOUBLE_BUFFER_T*);
void        __cdecl     WINDOWS_PROCEDURE_HandleFullscreen              (HWND, GLOBALS_T*, MENU_T*, DOUBLE_BUFFER_T*);
void        __cdecl     WINDOWS_PROCEDURE_HandleBorderless              (HWND, GLOBALS_T*, MENU_T*, DOUBLE_BUFFER_T*);        
void        __cdecl     WINDOWS_PROCEDURE_EnterFullscreen               (HWND, GLOBALS_T*, MENU_T*, DOUBLE_BUFFER_T*);
void        __cdecl     WINDOWS_PROCEDURE_ExitFullscreen                (HWND, GLOBALS_T*, MENU_T*, DOUBLE_BUFFER_T*);
void        __cdecl     WINDOWS_PROCEDURE_EnterBorderless               (HWND);
void        __cdecl     WINDOWS_PROCEDURE_ExitBorderless                (HWND);
void        __cdecl     WINDOWS_PROCEDURE_HandleRightClick              (GLOBALS_T*, MENU_T*);   
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
