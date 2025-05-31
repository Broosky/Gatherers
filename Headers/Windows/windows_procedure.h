/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _WINDOWS_PROCEDURE_H_
#define _WINDOWS_PROCEDURE_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../common.h"
#include "../enums.h"
#include "windows_minified.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declares:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct LOG LOG_T;
typedef struct MENU MENU_T;
typedef struct ASSETS ASSETS_T;
typedef struct GLOBALS GLOBALS_T;
typedef struct RENDERER RENDERER_T;
typedef struct SETTINGS SETTINGS_T;
typedef struct CONSTANTS CONSTANTS_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT             CALLBACK    WINDOWS_PROCEDURE_WindowProc                    (HWND, UINT, WPARAM, LPARAM);
void                __cdecl     WINDOWS_PROCEDURE_BuildHelper                   (ENTITY_TYPE_T, MENU_T*);
void                __cdecl     WINDOWS_PROCEDURE_RendererHelper                (RENDERER_TYPE_T, GLOBALS_T*, MENU_T*);
void                __cdecl     WINDOWS_PROCEDURE_BuildNone                     (GLOBALS_T*, MENU_T*);
void                __cdecl     WINDOWS_PROCEDURE_SendWorkers                   (GLOBALS_T*, ASSETS_T*, LOG_T*, MENU_T*, SETTINGS_T*);
void                __cdecl     WINDOWS_PROCEDURE_DistributeAndSendWorkers      (GLOBALS_T*, ASSETS_T*, LOG_T*, MENU_T*, SETTINGS_T*);
void                __cdecl     WINDOWS_PROCEDURE_SelectEntities                (GLOBALS_T*);
void                __cdecl     WINDOWS_PROCEDURE_CreateOrCaptureEntities       (GLOBALS_T*, ASSETS_T*, LOG_T*, RENDERER_T*, SETTINGS_T*, CONSTANTS_T*);
FPOINT_LIST_T*      __cdecl     WINDOWS_PROCEDURE_CaptureDestinationPoints      (USHORT, GLOBALS_T*, ASSETS_T*, LOG_T*);
ENTITY_T*           __cdecl     WINDOWS_PROCEDURE_CaptureDestinationData        (USHORT*, GLOBALS_T*);
void                __cdecl     WINDOWS_PROCEDURE_HandleKeyDown                 (WPARAM, GLOBALS_T*, MENU_T*, RENDERER_T*, LOG_T*, SETTINGS_T*);
UINT8               __cdecl     WINDOWS_PROCEDURE_HandleChordKeys               (WPARAM, USHORT, WPARAM);
void                __cdecl     WINDOWS_PROCEDURE_HandleArrowKeys               (WPARAM, GLOBALS_T*, MENU_T*, RENDERER_T*, SETTINGS_T*, LOG_T*);
void                __cdecl     WINDOWS_PROCEDURE_HandleOtherKeys               (WPARAM, GLOBALS_T*, MENU_T*, RENDERER_T*, SETTINGS_T*, LOG_T*);
void                __cdecl     WINDOWS_PROCEDURE_HandleMenuEvent               (WPARAM, MENU_T*, GLOBALS_T*, RENDERER_T*, LOG_T*, SETTINGS_T*, CONSTANTS_T*);
void                __cdecl     WINDOWS_PROCEDURE_HandleFullscreen              (GLOBALS_T*, MENU_T*, RENDERER_T*, SETTINGS_T*, LOG_T*);
void                __cdecl     WINDOWS_PROCEDURE_HandleBorderless              (GLOBALS_T*, MENU_T*, RENDERER_T*, SETTINGS_T*, LOG_T*);        
void                __cdecl     WINDOWS_PROCEDURE_EnterFullscreen               (GLOBALS_T*, MENU_T*, RENDERER_T*, LOG_T*);
void                __cdecl     WINDOWS_PROCEDURE_ExitFullscreen                (GLOBALS_T*, MENU_T*, RENDERER_T*, SETTINGS_T*, LOG_T*);
void                __cdecl     WINDOWS_PROCEDURE_EnterBorderless               (GLOBALS_T*, RENDERER_T*, SETTINGS_T*);
void                __cdecl     WINDOWS_PROCEDURE_ExitBorderless                (GLOBALS_T*, RENDERER_T*);
void                __cdecl     WINDOWS_PROCEDURE_HandleRightClick              (GLOBALS_T*, MENU_T*, ASSETS_T*, LOG_T*, SETTINGS_T*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
