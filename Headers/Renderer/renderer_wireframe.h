/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _RENDERER_WIREFRAME_H_
#define _RENDERER_WIREFRAME_H_
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
typedef struct WIREFRAME {
    // Implementation of renderer.
    UINT8 ubDummyMember;
} WIREFRAME_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void                __cdecl     WIREFRAME_Zero                                  (WIREFRAME_T*);
WIREFRAME_T*        __cdecl     WIREFRAME_Create                                (RENDERER_T*, GLOBALS_T*, LOG_T*);
WIREFRAME_T*        __cdecl     WIREFRAME_Cast                                  (RENDERER_T*);
void                __cdecl     WIREFRAME_Kill                                  (RENDERER_T*, GLOBALS_T*);
RECT                __cdecl     WIREFRAME_GetClientArea                         (RENDERER_T*);
void                __cdecl     WIREFRAME_Resize                                (RENDERER_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     WIREFRAME_SetBlitter                            (RENDERER_T*, PICTURE_T*);
void                __cdecl     WIREFRAME_Blitter                               (RENDERER_T*, char*, FPOINT_T, UINT8);
void                __cdecl     WIREFRAME_Clear                                 (RENDERER_T*, ASSETS_T*);
void                __cdecl     WIREFRAME_ClearEntity                           (RENDERER_T*, ENTITY_T*, ASSETS_T*);
void                __cdecl     WIREFRAME_FlipEntity                            (RENDERER_T*, ENTITY_T*);
void                __cdecl     WIREFRAME_PresentFrame                          (RENDERER_T*);
void                __cdecl     WIREFRAME_EnablingPostProcessing                (RENDERER_T*, MENU_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     WIREFRAME_DisablingPostProcessing               (RENDERER_T*, MENU_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     WIREFRAME_HandlePostProcessing                  (RENDERER_T*, MENU_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     WIREFRAME_HandlePreProcessing                   (RENDERER_T*, MENU_T*, ASSETS_T*, GLOBALS_T*, LOG_T*);
FRECT_T             __cdecl     WIREFRAME_CaptureDirtyZone                      (RENDERER_T*, ENTITY_T*, SETTINGS_T*, CONSTANTS_T*);
void                __cdecl     WIREFRAME_PrepareDirtyZone                      (RENDERER_T*, FRECT_T, GLOBALS_T*, LOG_T*);
void                __cdecl     WIREFRAME_FlipArea                              (RENDERER_T*, IRECT_T);
void                __cdecl     WIREFRAME_ApplyWorldTransform                   (RENDERER_T*, TRANSFORM_TYPE_T, CONSTANTS_T*, float, FPOINT_T, LOG_T*);
void                __cdecl     WIREFRAME_ResetWorldTransform                   (RENDERER_T*);
void                __cdecl     WIREFRAME_InitWorldTransform                    (RENDERER_T*);
void                __cdecl     WIREFRAME_DrawMinimapEntities                   (RENDERER_T*, GLOBALS_T*, FPOINT_T, ASSETS_T*, SETTINGS_T*, MENU_T*, LOG_T*);
void                __cdecl     WIREFRAME_DrawMinimapViewport                   (RENDERER_T*, ASSETS_T*, FPOINT_T, FPOINT_T, FPOINT_T, FPOINT_T);
void                __cdecl     WIREFRAME_DrawMinimapSelectionArea              (RENDERER_T*, ASSETS_T*, FPOINT_T, FPOINT_T, FPOINT_T, FPOINT_T);
void                __cdecl     WIREFRAME_DrawBuildLimits                       (RENDERER_T*, ASSETS_T*, FPOINT_T, FPOINT_T, FPOINT_T, FPOINT_T);
void                __cdecl     WIREFRAME_DrawBuildType                         (RENDERER_T*, MENU_T*, ASSETS_T*, PICTURE_T*, FPOINT_T, IRECT_T);
void                __cdecl     WIREFRAME_DrawSelectionArea                     (RENDERER_T*, GLOBALS_T*, ASSETS_T*);
void                __cdecl     WIREFRAME_DrawTranslationThreshold              (RENDERER_T*, ASSETS_T*, GLOBALS_T*);
void                __cdecl     WIREFRAME_DrawPicture                           (RENDERER_T*, PICTURE_T*, UINT8);
void                __cdecl     WIREFRAME_DrawPictureAt                         (RENDERER_T*, PICTURE_T*, FPOINT_T, UINT8);
void                __cdecl     WIREFRAME_CropDrawPictureAt                     (RENDERER_T*, PICTURE_T*, FPOINT_T, FDELTA_T, FPOINT_T, UINT8);
void                __cdecl     WIREFRAME_DrawEntityMinorVector                 (RENDERER_T*, ENTITY_T*, ASSETS_T*);
void                __cdecl     WIREFRAME_DrawEntityMajorVector                 (RENDERER_T*, ENTITY_T*, ASSETS_T*);
void                __cdecl     WIREFRAME_DrawEntityEllipse                     (RENDERER_T*, ENTITY_T*, HPEN, HBRUSH, SETTINGS_T*);
void                __cdecl     WIREFRAME_DrawEntity                            (RENDERER_T*, ENTITY_T*, UINT8);
void                __cdecl     WIREFRAME_DrawDirtyZones                        (RENDERER_T*, ASSETS_T*, GLOBALS_T*);
void                __cdecl     WIREFRAME_DrawTerrainGrid                       (RENDERER_T*, FPOINT_T, FPOINT_T, ASSETS_T*, GLOBALS_T*);
void                __cdecl     WIREFRAME_DrawLine                              (RENDERER_T*, FPOINT_T, FPOINT_T, HPEN);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
