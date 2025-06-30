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
#include "enums.h"
#include "Windows/windows_minified.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declares:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct LOG LOG_T;
typedef struct CARD CARD_T;
typedef struct MENU MENU_T;
typedef struct ASSETS ASSETS_T;
typedef struct ENTITY ENTITY_T;
typedef struct GLOBALS GLOBALS_T;
typedef struct SETTINGS SETTINGS_T;
typedef struct RENDERER RENDERER_T;
typedef struct CONSTANTS CONSTANTS_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void                __cdecl     PROCESS_DrawSelectionArea                       (RENDERER_T*, GLOBALS_T*, ASSETS_T*);
void                __cdecl     PROCESS_CaptureBorderTranslations               (RENDERER_T*, GLOBALS_T*, ASSETS_T*, MENU_T*, SETTINGS_T*, LOG_T*);
FDELTA_T            __cdecl     PROCESS_ApplyTranslations                       (RENDERER_T*, GLOBALS_T*, ASSETS_T*, CONSTANTS_T*);
void                __cdecl     PROCESS_ResetTranslations                       (RENDERER_T*);
void                __cdecl     PROCESS_UpdateAnimations                        (GLOBALS_T*);
void                __cdecl     PROCESS_PrepareFrame                            (RENDERER_T*, GLOBALS_T*, ASSETS_T*, MENU_T*, CONSTANTS_T*);
void                __cdecl     PROCESS_HandlePreProcessing                     (RENDERER_T*, MENU_T*, ASSETS_T*, GLOBALS_T*, LOG_T*, CONSTANTS_T*);
void                __cdecl     PROCESS_ProcessEntities                         (RENDERER_T*, GLOBALS_T*, ASSETS_T*, MENU_T*, LOG_T*, SETTINGS_T*, CONSTANTS_T*);
void                __cdecl     PROCESS_ProcessMessages                         (RENDERER_T*, GLOBALS_T*, ASSETS_T*, MENU_T*, CONSTANTS_T*, LOG_T*);
void                __cdecl     PROCESS_HandleDiagnostics                       (RENDERER_T*, GLOBALS_T*, ASSETS_T*, MENU_T*, SETTINGS_T*);
void                __cdecl     PROCESS_HandleResourceBar                       (RENDERER_T*, GLOBALS_T*, ASSETS_T*, MENU_T*);
void                __cdecl     PROCESS_DrawCommandCard                         (RENDERER_T*, GLOBALS_T*, ASSETS_T*, CARD_T*, MENU_T*);
void                __cdecl     PROCESS_HandleMinimap                           (RENDERER_T*, GLOBALS_T*, ASSETS_T*, SETTINGS_T*, MENU_T*, CONSTANTS_T*, LOG_T*);
void                __cdecl     PROCESS_DrawMinimapTerrain                      (RENDERER_T*, GLOBALS_T*, ASSETS_T*, CONSTANTS_T*, FPOINT_T);
void                __cdecl     PROCESS_DrawMinimapEntities                     (RENDERER_T*, GLOBALS_T*, FPOINT_T, ASSETS_T*, SETTINGS_T*, MENU_T*, LOG_T*);
void                __cdecl     PROCESS_DrawMinimapViewport                     (RENDERER_T*, GLOBALS_T*, ASSETS_T*, FPOINT_T, SETTINGS_T*);
void                __cdecl     PROCESS_DrawMinimapSelectionArea                (RENDERER_T*, GLOBALS_T*, FPOINT_T, ASSETS_T*, SETTINGS_T*);
void                __cdecl     PROCESS_HandleHud                               (RENDERER_T*, GLOBALS_T*, ASSETS_T*, CARD_T*, MENU_T*, SETTINGS_T*, CONSTANTS_T*, LOG_T*);
void                __cdecl     PROCESS_DrawBuildLimits                         (RENDERER_T*, ENTITY_T*, ASSETS_T*);
void                __cdecl     PROCESS_DrawBuildType                           (RENDERER_T*, GLOBALS_T*, ASSETS_T*, MENU_T*, LOG_T*);
void                __cdecl     PROCESS_PopulateViewportRelativity              (FPOINT_T*, FDELTA_T*, FPOINT_T*, FDELTA_T*, RENDERER_T*, GLOBALS_T*, SETTINGS_T*);
void                __cdecl     PROCESS_PopulateMinimapDimensions               (FPOINT_T*, FDELTA_T*, RENDERER_T*, ASSETS_T*, SETTINGS_T*);
UINT8               __cdecl     PROCESS_IsMinimapClicked                        (RENDERER_T*, ASSETS_T*, GLOBALS_T*, SETTINGS_T*);
void                __cdecl     PROCESS_AdjustMinimapViewport                   (IPOINT_T, GLOBALS_T*, RENDERER_T*, ASSETS_T*, SETTINGS_T*);
UINT8               __cdecl     PROCESS_ConsiderFrustum                         (void*, PROCESS_TYPE_T, RENDERER_T*, LOG_T*);
UINT8               __cdecl     PROCESS_ConsiderTranslationZone                 (TRANSLATION_ZONE_T, IPOINT_T, LONG, LONG, USHORT, LOG_T*);
FDELTA_T            __cdecl     PROCESS_CaptureTranslationAmount                (TRANSLATION_ZONE_T, FDELTA_T, float, LONG, LONG, LONG, LONG, USHORT, LOG_T*);
UINT8               __cdecl     PROCESS_IsVisible                               (FPOINT_T, RENDERER_T*);
void                __cdecl     PROCESS_RenderDiagnostic                        (FPOINT_T*, RENDERER_T*, UINT8);
void                __cdecl     PROCESS_DrawBuildMode                           (FPOINT_T*, RENDERER_T*, UINT8, float);
void                __cdecl     PROCESS_DrawTranslations                        (FPOINT_T*, RENDERER_T*, UINT8, GLOBALS_T*, float);
void                __cdecl     PROCESS_DrawMouseCoordinates                    (FPOINT_T*, RENDERER_T*, UINT8, GLOBALS_T*, float);
void                __cdecl     PROCESS_DrawSelectionCapture                    (FPOINT_T*, RENDERER_T*, UINT8, GLOBALS_T*, float);
void                __cdecl     PROCESS_DrawClientSize                          (FPOINT_T*, RENDERER_T*, UINT8, float);
void                __cdecl     PROCESS_DrawEngineFrameTime                     (FPOINT_T*, RENDERER_T*, UINT8, GLOBALS_T*, float);
void                __cdecl     PROCESS_DrawEngineFps                           (FPOINT_T*, RENDERER_T*, UINT8, GLOBALS_T*, SETTINGS_T*, float);
void                __cdecl     PROCESS_DrawEngineFpsEstimate                   (RENDERER_T*, FPOINT_T, GLOBALS_T*, UINT8);
void                __cdecl     PROCESS_DrawUptime                              (FPOINT_T*, RENDERER_T*, UINT8, GLOBALS_T*, float);
void                __cdecl     PROCESS_DrawHeapAllocations                     (FPOINT_T*, RENDERER_T*, UINT8, GLOBALS_T*, float);
void                __cdecl     PROCESS_DrawWorkersSelected                     (FPOINT_T*, RENDERER_T*, UINT8, GLOBALS_T*, float);
void                __cdecl     PROCESS_DrawEntitiesSelected                    (FPOINT_T*, RENDERER_T*, UINT8, GLOBALS_T*, float);
void                __cdecl     PROCESS_DrawAllocators                          (FPOINT_T*, RENDERER_T*, UINT8, GLOBALS_T*, float);
void                __cdecl     PROCESS_DrawTranslationThreshold                (RENDERER_T*, ASSETS_T*, GLOBALS_T*, MENU_T*);
void                __cdecl     PROCESS_DrawHarvestedMinerals                   (RENDERER_T*, GLOBALS_T*, ASSETS_T*, MENU_T*);
void                __cdecl     PROCESS_DrawHarvestedGas                        (RENDERER_T*, GLOBALS_T*, ASSETS_T*, MENU_T*);
void                __cdecl     PROCESS_DrawSupplyAmounts                       (RENDERER_T*, GLOBALS_T*, ASSETS_T*, MENU_T*);
void                __cdecl     PROCESS_DrawDirtyZones                          (RENDERER_T*, GLOBALS_T*, ASSETS_T*, MENU_T*);
void                __cdecl     PROCESS_DrawTerrainGrid                         (RENDERER_T*, FPOINT_T, FPOINT_T, ASSETS_T*, GLOBALS_T*, MENU_T*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
