/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _ASSETS_H_
#define _ASSETS_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "constants.h"
#include "enums.h"
#include "picture.h"
#include "Windows/windows_minified.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declares:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct LOG LOG_T;
typedef struct GLOBALS GLOBALS_T;
typedef struct SETTINGS SETTINGS_T;
typedef struct CONSTANTS CONSTANTS_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Types:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct ASSETS {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Viewport entities:
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_T Blitter[BLITTER_COLOR_COUNT],
        Card[4],
        Command[26],
        Gas[1],
        HUD[2],
        Mineral[5],
        Minimap[1],
        Refinery[1],
        Supply[6],
        Worker[3];
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Minimap entities:
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_T CommandMini,
        MineralMini,
        RefineryMini,
        SupplyMini,
        WorkerMini;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Dynamic terrain:
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_T* Terrain;
    PICTURE_T* TerrainMini;
    size_t stAllocationsTerrain;
    size_t stAllocationsTerrainMini;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Brushes:
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HBRUSH hBrushSelected,
        hBrushHighlighted,
        hBrushWorker,
        hBrushCommand,
        hBrushMineral,
        hBrushSupply,
        hBrushRefinery,
        hBrushBufferClear,
        hBrushClear,
        hBrush100r0g0b,
        hBrush0r100g0b,
        hBrush0r0g100b,
        hBrush100r100g0b,
        hBrush0r100g100b,
        hBrush100r0g100b,
        hBrush100r100g100b,
        hBrushWhite;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Pens:
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HPEN hPenSelected,
        hPenHighlighted,
        hPenWorker,
        hPenCommand,
        hPenMineral,
        hPenSupply,
        hPenRefinery,
        hPenViewport,
        hPenSelectionArea,
        hPenBuildLimits,
        hPenBuildType,
        hPenTranslation,
        hPenMinorVector,
        hPenMajorVector,
        hPenDirtyZone,
        hPenTerrainGrid,
        hPenWhite;
} ASSETS_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void                __cdecl     ASSETS_Zero                                     (ASSETS_T*);
ASSETS_T*           __cdecl     ASSETS_Create                                   (GLOBALS_T*, LOG_T*);
void                __cdecl     ASSETS_HandleBitmaps                            (ASSETS_T*, HWND, CONSTANTS_T*, SETTINGS_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     ASSETS_LoadBlitter                              (ASSETS_T*, FPOINT_T, HWND, LOG_T*);
void                __cdecl     ASSETS_LoadCommandCard                          (ASSETS_T*, FPOINT_T, HWND, LOG_T*);
void                __cdecl     ASSETS_LoadCommandCenter                        (ASSETS_T*, FPOINT_T, HWND, SETTINGS_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     ASSETS_LoadGas                                  (ASSETS_T*, FPOINT_T, HWND, LOG_T*);
void                __cdecl     ASSETS_LoadHud                                  (ASSETS_T*, FPOINT_T, HWND, LOG_T*);
void                __cdecl     ASSETS_LoadMineralField                         (ASSETS_T*, FPOINT_T, HWND, SETTINGS_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     ASSETS_LoadRefinery                             (ASSETS_T*, FPOINT_T, HWND, SETTINGS_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     ASSETS_LoadSupplyDepot                          (ASSETS_T*, FPOINT_T, HWND, SETTINGS_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     ASSETS_LoadTerrain                              (ASSETS_T*, FPOINT_T, GLOBALS_T*, CONSTANTS_T*, HWND, SETTINGS_T*, LOG_T*);
void                __cdecl     ASSETS_LoadWorker                               (ASSETS_T*, FPOINT_T, HWND, SETTINGS_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     ASSETS_CreateBrushes                            (ASSETS_T*);
void                __cdecl     ASSETS_CreatePens                               (ASSETS_T*);
void                __cdecl     ASSETS_KillBrushes                              (ASSETS_T*);
void                __cdecl     ASSETS_KillPens                                 (ASSETS_T*);
void                __cdecl     ASSETS_KillBitmaps                              (ASSETS_T*, GLOBALS_T*, CONSTANTS_T*);
void                __cdecl     ASSETS_Kill                                     (ASSETS_T**, GLOBALS_T*, CONSTANTS_T*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
