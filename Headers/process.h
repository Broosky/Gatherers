/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _PROCESS_H_
#define _PROCESS_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "common_types.h"
#include "windows_macros.h"
#include <windows.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declares:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct GLOBALS GLOBALS_T;
typedef struct DOUBLE_BUFFER DOUBLE_BUFFER_T;
typedef struct ASSETS ASSETS_T;
typedef struct CARD CARD_T;
typedef struct MENU MENU_T;
typedef struct ENTITY ENTITY_T;
typedef struct LOG LOG_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void    __cdecl     PROCESS_DrawSelectionArea               (DOUBLE_BUFFER_T*, GLOBALS_T*, ASSETS_T*);
void    __cdecl     PROCESS_CaptureAndApplyTranslations     (DOUBLE_BUFFER_T*, GLOBALS_T*, ASSETS_T*, MENU_T*);
void    __cdecl     PROCESS_ApplyTranslations               (float, float, GLOBALS_T*, ASSETS_T*);
void    __cdecl     PROCESS_UpdateAnimations                (GLOBALS_T*);
void    __cdecl     PROCESS_DrawBackground                  (DOUBLE_BUFFER_T*, GLOBALS_T*, ASSETS_T*);
void    __cdecl     PROCESS_ProcessEntities                 (DOUBLE_BUFFER_T*, GLOBALS_T*, ASSETS_T*, MENU_T*, LOG_T*);
void    __cdecl     PROCESS_ProcessMessages                 (DOUBLE_BUFFER_T*, GLOBALS_T*, ASSETS_T*, MENU_T*);
void    __cdecl     PROCESS_DrawDiagnostics                 (DOUBLE_BUFFER_T*, GLOBALS_T*, ASSETS_T*, MENU_T*);
void    __cdecl     PROCESS_DrawResourceBar                 (DOUBLE_BUFFER_T*, GLOBALS_T*, ASSETS_T*, MENU_T*);
void    __cdecl     PROCESS_DrawTaskbar                     (DOUBLE_BUFFER_T*, GLOBALS_T*, ASSETS_T*, CARD_T*, MENU_T*);
void    __cdecl     PROCESS_HandleMinimap                   (DOUBLE_BUFFER_T*, GLOBALS_T*, ASSETS_T*);
void    __cdecl     PROCESS_DrawMinimapPicture              (DOUBLE_BUFFER_T*, GLOBALS_T*, ASSETS_T*, FPOINT_T);
void    __cdecl     PROCESS_DrawMinimapEntities             (DOUBLE_BUFFER_T*, GLOBALS_T*, FPOINT_T, ASSETS_T*);
void    __cdecl     PROCESS_DrawMinimapViewport             (DOUBLE_BUFFER_T*, GLOBALS_T*, ASSETS_T*, FPOINT_T);
void    __cdecl     PROCESS_DrawMinimapSelectionArea        (DOUBLE_BUFFER_T*, GLOBALS_T*, FPOINT_T, ASSETS_T*);
void    __cdecl     PROCESS_HandleHud                       (DOUBLE_BUFFER_T*, GLOBALS_T*, ASSETS_T*, CARD_T*, MENU_T*);
void    __cdecl     PROCESS_DrawBuildLimits                 (DOUBLE_BUFFER_T*, ENTITY_T*, ASSETS_T*);
void    __cdecl     PROCESS_DrawBuildType                   (DOUBLE_BUFFER_T*, GLOBALS_T*, ASSETS_T*, MENU_T*);
void    __cdecl     PROCESS_PopulateViewportRelativity      (FPOINT_T*, FDELTA_T*, FPOINT_T*, FDELTA_T*, DOUBLE_BUFFER_T*, ASSETS_T*, GLOBALS_T*);
void    __cdecl     PROCESS_PopulateMinimapDimensions       (FPOINT_T*, FDELTA_T*, DOUBLE_BUFFER_T*, ASSETS_T*);
UINT8   __cdecl     PROCESS_IsMinimapClicked                (DOUBLE_BUFFER_T*, ASSETS_T*, GLOBALS_T*);
void    __cdecl     PROCESS_AdjustMinimapViewport           (GLOBALS_T*, DOUBLE_BUFFER_T*, ASSETS_T*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
