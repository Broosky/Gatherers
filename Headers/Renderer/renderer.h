/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _RENDERER_H_
#define _RENDERER_H_
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
typedef struct ASSETS ASSETS_T;
typedef struct ENTITY ENTITY_T;
typedef struct GLOBALS GLOBALS_T;
typedef struct PICTURE PICTURE_T;
typedef struct SETTINGS SETTINGS_T;
typedef struct RENDERER RENDERER_T;
typedef struct CONSTANTS CONSTANTS_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef void*       (__cdecl* RENDERER_CREATE)                                  (RENDERER_T*, GLOBALS_T*, LOG_T*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Abstractions:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef RECT        (__cdecl* RENDERER_GET_CLIENT_AREA)                         (RENDERER_T*);
typedef void        (__cdecl* RENDERER_RESIZE)                                  (RENDERER_T*, GLOBALS_T*, LOG_T*);
typedef void        (__cdecl* RENDERER_KILL)                                    (RENDERER_T*, GLOBALS_T*);
typedef void        (__cdecl* RENDERER_SET_BLITTER)                             (RENDERER_T*, PICTURE_T*);
typedef void        (__cdecl* RENDERER_BLITTER)                                 (RENDERER_T*, char*, FPOINT_T, UINT8);
typedef void        (__cdecl* RENDERER_CLEAR)                                   (RENDERER_T*, ASSETS_T*);
typedef void        (__cdecl* RENDERER_CLEAR_ENTITY)                            (RENDERER_T*, ENTITY_T*, ASSETS_T*);
typedef void        (__cdecl* RENDERER_FLIP_ENTITY)                             (RENDERER_T*, ENTITY_T*);
typedef void        (__cdecl* RENDERER_HANDLE_PRE_PROCESSING)                   (RENDERER_T*, MENU_T*, ASSETS_T*, GLOBALS_T*, LOG_T*);
typedef FRECT_T     (__cdecl* RENDERER_CAPTURE_DIRTY_ZONE)                      (RENDERER_T*, ENTITY_T*, SETTINGS_T*, CONSTANTS_T*);
typedef void        (__cdecl* RENDERER_PREPARE_DIRTY_ZONE)                      (RENDERER_T*, FRECT_T, GLOBALS_T*, LOG_T*);
typedef void        (__cdecl* RENDERER_PRESENT_FRAME)                           (RENDERER_T*);
typedef void        (__cdecl* RENDERER_ENABLING_POST_PROCESSING)                (RENDERER_T*, MENU_T*, GLOBALS_T*, LOG_T*);
typedef void        (__cdecl* RENDERER_DISABLING_POST_PROCESSING)               (RENDERER_T*, MENU_T*, GLOBALS_T*, LOG_T*);
typedef void        (__cdecl* RENDERER_HANDLE_POST_PROCESSING)                  (RENDERER_T*, MENU_T*, GLOBALS_T*, LOG_T*);
typedef void        (__cdecl* RENDERER_FLIP_AREA)                               (RENDERER_T*, IRECT_T);
typedef void        (__cdecl* RENDERER_APPLY_WORLD_TRANSFORM)                   (RENDERER_T*, TRANSFORM_TYPE_T, CONSTANTS_T*, float, FPOINT_T, LOG_T*);
typedef void        (__cdecl* RENDERER_RESET_WORLD_TRANSFORM)                   (RENDERER_T*);
typedef void        (__cdecl* RENDERER_INIT_WORLD_TRANSFORM)                    (RENDERER_T*);
typedef void        (__cdecl* RENDERER_DRAW_MINIMAP_ENTITIES)                   (RENDERER_T*, GLOBALS_T*, FPOINT_T, ASSETS_T*, SETTINGS_T*, MENU_T*, LOG_T*);
typedef void        (__cdecl* RENDERER_DRAW_MINIMAP_VIEWPORT)                   (RENDERER_T*, ASSETS_T*, FPOINT_T, FPOINT_T, FPOINT_T, FPOINT_T);
typedef void        (__cdecl* RENDERER_DRAW_MINIMAP_SELECTION_AREA)             (RENDERER_T*, ASSETS_T*, FPOINT_T, FPOINT_T, FPOINT_T, FPOINT_T);
typedef void        (__cdecl* RENDERER_DRAW_BUILD_LIMITS)                       (RENDERER_T*, ASSETS_T*, FPOINT_T, FPOINT_T, FPOINT_T, FPOINT_T);
typedef void        (__cdecl* RENDERER_DRAW_BUILD_TYPE)                         (RENDERER_T*, MENU_T*, ASSETS_T*, PICTURE_T*, FPOINT_T, IRECT_T);
typedef void        (__cdecl* RENDERER_DRAW_SELECTION_AREA)                     (RENDERER_T*, GLOBALS_T*, ASSETS_T*);
typedef void        (__cdecl* RENDERER_DRAW_TRANSLATION_THRESHOLD)              (RENDERER_T*, ASSETS_T*, GLOBALS_T*);
typedef void        (__cdecl* RENDERER_DRAW_DIRTY_ZONES)                        (RENDERER_T*, ASSETS_T*, GLOBALS_T*);
typedef void        (__cdecl* RENDERER_DRAW_TERRAIN_GRID)                       (RENDERER_T*, FPOINT_T, FPOINT_T, ASSETS_T*, GLOBALS_T*);
typedef void        (__cdecl* RENDERER_DRAW_PICTURE)                            (RENDERER_T*, PICTURE_T*, UINT8);
typedef void        (__cdecl* RENDERER_DRAW_PICTURE_AT)                         (RENDERER_T*, PICTURE_T*, FPOINT_T, UINT8);
typedef void        (__cdecl* RENDERER_CROP_DRAW_PICTURE_AT)                    (RENDERER_T*, PICTURE_T*, FPOINT_T, FDELTA_T, FPOINT_T, UINT8);
typedef void        (__cdecl* RENDERER_DRAW_ENTITY_MINOR_VECTOR)                (RENDERER_T*, ENTITY_T*, ASSETS_T*);
typedef void        (__cdecl* RENDERER_DRAW_ENTITY_MAJOR_VECTOR)                (RENDERER_T*, ENTITY_T*, ASSETS_T*);
typedef void        (__cdecl* RENDERER_DRAW_ENTITY_ELLIPSE)                     (RENDERER_T*, ENTITY_T*, HPEN, HBRUSH, SETTINGS_T*);
typedef void        (__cdecl* RENDERER_DRAW_ENTITY)                             (RENDERER_T*, ENTITY_T*, UINT8);
typedef void        (__cdecl* RENDERER_DRAW_LINE)                               (RENDERER_T*, FPOINT_T, FPOINT_T, HPEN);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Types:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct RENDERER {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Opaque to underlying implementation (wireframe, single/double buffer, SDL, Direct2D).
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void* vp_Implementation;
    RENDERER_TYPE_T                         eType;
    RENDERER_GET_CLIENT_AREA                GetClientArea;
    RENDERER_RESIZE                         Resize;
    RENDERER_KILL                           Kill;
    RENDERER_SET_BLITTER                    SetBlitter;
    RENDERER_BLITTER                        Blitter;
    RENDERER_CLEAR                          Clear;
    RENDERER_CLEAR_ENTITY                   ClearEntity;
    RENDERER_FLIP_ENTITY                    FlipEntity;
    RENDERER_PRESENT_FRAME                  PresentFrame;
    RENDERER_HANDLE_PRE_PROCESSING          HandlePreProcessing;
    RENDERER_CAPTURE_DIRTY_ZONE             CaptureDirtyZone;
    RENDERER_PREPARE_DIRTY_ZONE             PrepareDirtyZone;
    RENDERER_ENABLING_POST_PROCESSING       EnablingPostProcessing;
    RENDERER_DISABLING_POST_PROCESSING      DisablingPostProcessing;
    RENDERER_HANDLE_POST_PROCESSING         HandlePostProcessing;
    RENDERER_FLIP_AREA                      FlipArea;
    RENDERER_INIT_WORLD_TRANSFORM           InitWorldTransform;
    RENDERER_APPLY_WORLD_TRANSFORM          ApplyWorldTransform;
    RENDERER_RESET_WORLD_TRANSFORM          ResetWorldTransform;
    RENDERER_DRAW_MINIMAP_ENTITIES          DrawMinimapEntities;
    RENDERER_DRAW_MINIMAP_VIEWPORT          DrawMinimapViewport;
    RENDERER_DRAW_MINIMAP_SELECTION_AREA    DrawMinimapSelectionArea;
    RENDERER_DRAW_BUILD_LIMITS              DrawBuildLimits;
    RENDERER_DRAW_BUILD_TYPE                DrawBuildType;
    RENDERER_DRAW_SELECTION_AREA            DrawSelectionArea;
    RENDERER_DRAW_TRANSLATION_THRESHOLD     DrawTranslationThreshold;
    RENDERER_DRAW_DIRTY_ZONES               DrawDirtyZones;
    RENDERER_DRAW_TERRAIN_GRID              DrawTerrainGrid;
    RENDERER_DRAW_PICTURE                   DrawPicture;
    RENDERER_DRAW_PICTURE_AT                DrawPictureAt;
    RENDERER_CROP_DRAW_PICTURE_AT           CropDrawPictureAt;
    RENDERER_DRAW_ENTITY_MINOR_VECTOR       DrawEntityMinorVector;
    RENDERER_DRAW_ENTITY_MAJOR_VECTOR       DrawEntityMajorVector;
    RENDERER_DRAW_ENTITY_ELLIPSE            DrawEntityEllipse;
    RENDERER_DRAW_ENTITY                    DrawEntity;
    RENDERER_DRAW_LINE                      DrawLine;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    char szBlitter[128];
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FDELTA_T CapturedTranslationsBorder;
    FDELTA_T CapturedTranslationsKeys;
    FDELTA_T CapturedTranslationsMinimap;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UINT8 ubIsFullFrameRender;
} RENDERER_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void                __cdecl     RENDERER_Zero                                   (RENDERER_T*);
RENDERER_T*         __cdecl     RENDERER_Create                                 (RENDERER_TYPE_T, GLOBALS_T*, LOG_T*);
void                __cdecl     RENDERER_Kill                                   (RENDERER_T**, GLOBALS_T*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
