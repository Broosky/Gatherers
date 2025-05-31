/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _RENDERER_DIRECT_2D_H_
#define _RENDERER_DIRECT_2D_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../common.h"
#include "../common_types.h"
#include "../enums.h"
#include "../Windows/windows_minified.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declares:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct LOG LOG_T;
typedef struct MENU MENU_T;
typedef struct ENTITY ENTITY_T;
typedef struct ASSETS ASSETS_T;
typedef struct GLOBALS GLOBALS_T;
typedef struct PICTURE PICTURE_T;
typedef struct RENDERER RENDERER_T;
typedef struct SETTINGS SETTINGS_T;
typedef struct CONSTANTS CONSTANTS_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Types:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct DIRECT_2D {
    // Implementation of renderer.
    UINT8 ubDummyMember;
} DIRECT_2D_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void                __cdecl     DIRECT_2D_Zero                                  (DIRECT_2D_T*);
DIRECT_2D_T*        __cdecl     DIRECT_2D_Create                                (RENDERER_T*, GLOBALS_T*, LOG_T*);
DIRECT_2D_T*        __cdecl     DIRECT_2D_Cast                                  (RENDERER_T*);
void                __cdecl     DIRECT_2D_Kill                                  (RENDERER_T*, GLOBALS_T*);
RECT                __cdecl     DIRECT_2D_GetClientArea                         (RENDERER_T*);
void                __cdecl     DIRECT_2D_Resize                                (RENDERER_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     DIRECT_2D_SetBlitter                            (RENDERER_T*, PICTURE_T*);
void                __cdecl     DIRECT_2D_Blitter                               (RENDERER_T*, char*, FPOINT_T, UINT8);
void                __cdecl     DIRECT_2D_Clear                                 (RENDERER_T*, ASSETS_T*);
void                __cdecl     DIRECT_2D_ClearEntity                           (RENDERER_T*, ENTITY_T*, ASSETS_T*);
void                __cdecl     DIRECT_2D_FlipEntity                            (RENDERER_T*, ENTITY_T*);
void                __cdecl     DIRECT_2D_PresentFrame                          (RENDERER_T*);
void                __cdecl     DIRECT_2D_EnablingPostProcessing                (RENDERER_T*, MENU_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     DIRECT_2D_DisablingPostProcessing               (RENDERER_T*, MENU_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     DIRECT_2D_HandlePostProcessing                  (RENDERER_T*, MENU_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     DIRECT_2D_HandlePreProcessing                   (RENDERER_T*, MENU_T*, ASSETS_T*, GLOBALS_T*, LOG_T*);
FRECT_T             __cdecl     DIRECT_2D_CaptureDirtyZone                      (RENDERER_T*, ENTITY_T*, SETTINGS_T*, CONSTANTS_T*);
void                __cdecl     DIRECT_2D_PrepareDirtyZone                      (RENDERER_T*, FRECT_T, GLOBALS_T*, LOG_T*);
void                __cdecl     DIRECT_2D_FlipArea                              (RENDERER_T*, IRECT_T);
void                __cdecl     DIRECT_2D_ApplyWorldTransform                   (RENDERER_T*, TRANSFORM_TYPE_T, CONSTANTS_T*, float, FPOINT_T, LOG_T*);
void                __cdecl     DIRECT_2D_ResetWorldTransform                   (RENDERER_T*);
void                __cdecl     DIRECT_2D_InitWorldTransform                    (RENDERER_T*);
void                __cdecl     DIRECT_2D_DrawMinimapEntities                   (RENDERER_T*, GLOBALS_T*, FPOINT_T, ASSETS_T*, SETTINGS_T*, LOG_T*);
void                __cdecl     DIRECT_2D_DrawMinimapViewport                   (RENDERER_T*, ASSETS_T*, FPOINT_T, FPOINT_T, FPOINT_T, FPOINT_T);
void                __cdecl     DIRECT_2D_DrawMinimapSelectionArea              (RENDERER_T*, ASSETS_T*, FPOINT_T, FPOINT_T, FPOINT_T, FPOINT_T);
void                __cdecl     DIRECT_2D_DrawBuildLimits                       (RENDERER_T*, ASSETS_T*, FPOINT_T, FPOINT_T, FPOINT_T, FPOINT_T);
void                __cdecl     DIRECT_2D_DrawBuildType                         (RENDERER_T*, MENU_T*, ASSETS_T*, PICTURE_T*, FPOINT_T, IRECT_T);
void                __cdecl     DIRECT_2D_DrawSelectionArea                     (RENDERER_T*, GLOBALS_T*, ASSETS_T*);
void                __cdecl     DIRECT_2D_DrawTranslationThreshold              (RENDERER_T*, ASSETS_T*, GLOBALS_T*);
void                __cdecl     DIRECT_2D_DrawPicture                           (RENDERER_T*, PICTURE_T*, UINT8);
void                __cdecl     DIRECT_2D_DrawPictureAt                         (RENDERER_T*, PICTURE_T*, FPOINT_T, UINT8);
void                __cdecl     DIRECT_2D_CropDrawPictureAt                     (RENDERER_T*, PICTURE_T*, FPOINT_T, FDELTA_T, FPOINT_T, UINT8);
void                __cdecl     DIRECT_2D_DrawEntityMinorVector                 (RENDERER_T*, ENTITY_T*, ASSETS_T*);
void                __cdecl     DIRECT_2D_DrawEntityMajorVector                 (RENDERER_T*, ENTITY_T*, ASSETS_T*);
void                __cdecl     DIRECT_2D_DrawEntityEllipse                     (RENDERER_T*, ENTITY_T*, HPEN, HBRUSH, SETTINGS_T*);
void                __cdecl     DIRECT_2D_DrawEntity                            (RENDERER_T*, ENTITY_T*, UINT8);
void                __cdecl     DIRECT_2D_DrawDirtyZones                        (RENDERER_T*, ASSETS_T*, GLOBALS_T*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
