/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _ASSETS_H_
#define _ASSETS_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "picture.h"
#include "Windows/windows_minified.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declares:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct LOG LOG_T;
typedef struct GLOBALS GLOBALS_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Types:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct ASSETS {
    PICTURE_T Blitter[9],
        Worker[3],
        Command[26],
        Mineral[5],
        Supply[6],
        Refinery[1],
        HUD[5],
        Minimap[1],
        Gas[1],
        Terrain[1],
        Card[5];

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
        hPenWhite;
} ASSETS_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void                __cdecl     ASSETS_Zero                                     (ASSETS_T*);
ASSETS_T*           __cdecl     ASSETS_Create                                   (GLOBALS_T*, LOG_T*);
void                __cdecl     ASSETS_LoadBitmaps                              (ASSETS_T*, HWND, LOG_T*);
void                __cdecl     ASSETS_CreateBrushes                            (ASSETS_T*);
void                __cdecl     ASSETS_KillBrushes                              (ASSETS_T*);
void                __cdecl     ASSETS_KillBitmaps                              (ASSETS_T*);
void                __cdecl     ASSETS_Kill                                     (ASSETS_T**, GLOBALS_T*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
