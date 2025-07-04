/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _RENDERER_SDL_H_
#define _RENDERER_SDL_H_
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
typedef struct SDL {
    // Implementation of renderer.
    UINT8 ubDummyMember;
} SDL_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void                __cdecl     SDL_Zero                                        (SDL_T*);
SDL_T*              __cdecl     SDL_Create                                      (RENDERER_T*, GLOBALS_T*, LOG_T*);
SDL_T*              __cdecl     SDL_Cast                                        (RENDERER_T*);
void                __cdecl     SDL_Kill                                        (RENDERER_T*, GLOBALS_T*);
RECT                __cdecl     SDL_GetClientArea                               (RENDERER_T*);
void                __cdecl     SDL_Resize                                      (RENDERER_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     SDL_SetBlitter                                  (RENDERER_T*, PICTURE_T*);
void                __cdecl     SDL_Blitter                                     (RENDERER_T*, char*, FPOINT_T, UINT8);
void                __cdecl     SDL_Clear                                       (RENDERER_T*, ASSETS_T*);
void                __cdecl     SDL_ClearEntity                                 (RENDERER_T*, ENTITY_T*, ASSETS_T*);
void                __cdecl     SDL_FlipEntity                                  (RENDERER_T*, ENTITY_T*);
void                __cdecl     SDL_PresentFrame                                (RENDERER_T*);
void                __cdecl     SDL_EnablingPostProcessing                      (RENDERER_T*, MENU_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     SDL_DisablingPostProcessing                     (RENDERER_T*, MENU_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     SDL_HandlePostProcessing                        (RENDERER_T*, MENU_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     SDL_HandlePreProcessing                         (RENDERER_T*, MENU_T*, ASSETS_T*, GLOBALS_T*, LOG_T*);
FRECT_T             __cdecl     SDL_CaptureDirtyZone                            (RENDERER_T*, ENTITY_T*, SETTINGS_T*, CONSTANTS_T*);
void                __cdecl     SDL_PrepareDirtyZone                            (RENDERER_T*, FRECT_T, GLOBALS_T*, LOG_T*);
void                __cdecl     SDL_FlipArea                                    (RENDERER_T*, IRECT_T);
void                __cdecl     SDL_ApplyWorldTransform                         (RENDERER_T*, TRANSFORM_TYPE_T, CONSTANTS_T*, float, FPOINT_T, LOG_T*);
void                __cdecl     SDL_ResetWorldTransform                         (RENDERER_T*);
void                __cdecl     SDL_InitWorldTransform                          (RENDERER_T*);
void                __cdecl     SDL_DrawMinimapEntities                         (RENDERER_T*, GLOBALS_T*, FPOINT_T, ASSETS_T*, SETTINGS_T*, MENU_T*, LOG_T*);
void                __cdecl     SDL_DrawMinimapViewport                         (RENDERER_T*, ASSETS_T*, FPOINT_T, FPOINT_T, FPOINT_T, FPOINT_T);
void                __cdecl     SDL_DrawMinimapSelectionArea                    (RENDERER_T*, ASSETS_T*, FPOINT_T, FPOINT_T, FPOINT_T, FPOINT_T);
void                __cdecl     SDL_DrawBuildLimits                             (RENDERER_T*, ASSETS_T*, FPOINT_T, FPOINT_T, FPOINT_T, FPOINT_T);
void                __cdecl     SDL_DrawBuildType                               (RENDERER_T*, MENU_T*, ASSETS_T*, PICTURE_T*, FPOINT_T, IRECT_T);
void                __cdecl     SDL_DrawSelectionArea                           (RENDERER_T*, GLOBALS_T*, ASSETS_T*);
void                __cdecl     SDL_DrawTranslationThreshold                    (RENDERER_T*, ASSETS_T*, GLOBALS_T*);
void                __cdecl     SDL_DrawPicture                                 (RENDERER_T*, PICTURE_T*, UINT8);
void                __cdecl     SDL_DrawPictureAt                               (RENDERER_T*, PICTURE_T*, FPOINT_T, UINT8);
void                __cdecl     SDL_CropDrawPictureAt                           (RENDERER_T*, PICTURE_T*, FPOINT_T, FDELTA_T, FPOINT_T, UINT8);
void                __cdecl     SDL_DrawEntityMinorVector                       (RENDERER_T*, ENTITY_T*, ASSETS_T*);
void                __cdecl     SDL_DrawEntityMajorVector                       (RENDERER_T*, ENTITY_T*, ASSETS_T*);
void                __cdecl     SDL_DrawEntityEllipse                           (RENDERER_T*, ENTITY_T*, HPEN, HBRUSH, SETTINGS_T*);
void                __cdecl     SDL_DrawEntity                                  (RENDERER_T*, ENTITY_T*, UINT8);
void                __cdecl     SDL_DrawDirtyZones                              (RENDERER_T*, ASSETS_T*, GLOBALS_T*);
void                __cdecl     SDL_DrawTerrainGrid                             (RENDERER_T*, FPOINT_T, FPOINT_T, ASSETS_T*, GLOBALS_T*);
void                __cdecl     SDL_DrawLine                                    (RENDERER_T*, FPOINT_T, FPOINT_T, HPEN);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
