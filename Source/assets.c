/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/assets.h"
#include "../Headers/constants.h"
#include "../Headers/globals.h"
#include "../Headers/log.h"
#include "../Headers/misc.h"
#include "../Headers/settings.h"
#include <stdlib.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_Zero(ASSETS_T* p_Assets) {
    ZeroMemory(p_Assets, sizeof(ASSETS_T));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASSETS_T* __cdecl ASSETS_Create(GLOBALS_T* p_Globals, LOG_T* p_Log) {
    size_t stAllocation = sizeof(ASSETS_T);
    ASSETS_T* p_Assets = malloc(stAllocation);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!p_Assets) {
        MISC_WriteOutParams(p_Log, LOG_SEVERITY_FATAL, "ASSETS_Create(): Malloc failed for size: %zu bytes\n", stAllocation);
        return NULL;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->stAllocations += stAllocation;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ASSETS_Zero(p_Assets);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return p_Assets;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_LoadBlitter(ASSETS_T* p_Assets, FPOINT_T Location, HWND hWnd, LOG_T* p_Log) {
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading blitter...");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    char* p_szSharedMask = "../../Assets/BL/B_M.bmp";
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Load(&p_Assets->Blitter[BLITTER_COLOR_LIGHT_BLUE], Location, "../../Assets/BL/BB0.bmp", p_szSharedMask, p_Log);
    PICTURE_Load(&p_Assets->Blitter[BLITTER_COLOR_BLUE], Location, "../../Assets/BL/BB1.bmp", p_szSharedMask, p_Log);
    PICTURE_Load(&p_Assets->Blitter[BLITTER_COLOR_GREEN], Location, "../../Assets/BL/BG0.bmp", p_szSharedMask, p_Log);
    PICTURE_Load(&p_Assets->Blitter[BLITTER_COLOR_ORANGE], Location, "../../Assets/BL/BO0.bmp", p_szSharedMask, p_Log);
    PICTURE_Load(&p_Assets->Blitter[BLITTER_COLOR_PURPLE], Location, "../../Assets/BL/BP0.bmp", p_szSharedMask, p_Log);
    PICTURE_Load(&p_Assets->Blitter[BLITTER_COLOR_PINK], Location, "../../Assets/BL/BP1.bmp", p_szSharedMask, p_Log);
    PICTURE_Load(&p_Assets->Blitter[BLITTER_COLOR_RED], Location, "../../Assets/BL/BR0.bmp", p_szSharedMask, p_Log);
    PICTURE_Load(&p_Assets->Blitter[BLITTER_COLOR_WHITE], Location, "../../Assets/BL/BW0.bmp", p_szSharedMask, p_Log);
    PICTURE_Load(&p_Assets->Blitter[BLITTER_COLOR_YELLOW], Location, "../../Assets/BL/BY0.bmp", p_szSharedMask, p_Log);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_LoadCommandCard(ASSETS_T* p_Assets, FPOINT_T Location, HWND hWnd, LOG_T* p_Log) {
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading command card...");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Load(&p_Assets->Card[0], Location, "../../Assets/CA/CA0.bmp", NULL, p_Log); // Blank card.
    PICTURE_Load(&p_Assets->Card[1], Location, "../../Assets/CA/CA1.bmp", NULL, p_Log); // Stop and move card.
    PICTURE_Load(&p_Assets->Card[2], Location, "../../Assets/CA/CA2.bmp", NULL, p_Log); // Specific structure.
    PICTURE_Load(&p_Assets->Card[3], Location, "../../Assets/CA/CA3.bmp", NULL, p_Log); // Specific worker.
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_LoadCommandCenter(ASSETS_T* p_Assets, FPOINT_T Location, HWND hWnd, SETTINGS_T* p_Settings, GLOBALS_T* p_Globals, LOG_T* p_Log) {
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading command center...");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    char* p_szSharedMask = "../../Assets/CC/CC0_M.bmp";
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // White lighting.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Load(&p_Assets->Command[0], Location, "../../Assets/CC/CC0.bmp", p_szSharedMask, p_Log); // Original.
    PICTURE_Load(&p_Assets->Command[1], Location, "../../Assets/CC/CC1.bmp", p_szSharedMask, p_Log); // Bright.
    PICTURE_Load(&p_Assets->Command[2], Location, "../../Assets/CC/CC2.bmp", p_szSharedMask, p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Command[3], Location, "../../Assets/CC/CC3.bmp", p_szSharedMask, p_Log); // Brightest.
    PICTURE_Load(&p_Assets->Command[4], Location, "../../Assets/CC/CC4.bmp", p_szSharedMask, p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Command[5], Location, "../../Assets/CC/CC5.bmp", p_szSharedMask, p_Log); // Bright.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Green lighting.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Load(&p_Assets->Command[6], Location, "../../Assets/CC/CC6.bmp", p_szSharedMask, p_Log); // Bright.
    PICTURE_Load(&p_Assets->Command[7], Location, "../../Assets/CC/CC7.bmp", p_szSharedMask, p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Command[8], Location, "../../Assets/CC/CC8.bmp", p_szSharedMask, p_Log); // Brightest.
    PICTURE_Load(&p_Assets->Command[9], Location, "../../Assets/CC/CC9.bmp", p_szSharedMask, p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Command[10], Location, "../../Assets/CC/CC10.bmp", p_szSharedMask, p_Log); // Bright.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Load(&p_Assets->Command[11], Location, "../../Assets/CC/CC11.bmp", p_szSharedMask, p_Log); // Bright.
    PICTURE_Load(&p_Assets->Command[12], Location, "../../Assets/CC/CC12.bmp", p_szSharedMask, p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Command[13], Location, "../../Assets/CC/CC13.bmp", p_szSharedMask, p_Log); // Brightest.
    PICTURE_Load(&p_Assets->Command[14], Location, "../../Assets/CC/CC14.bmp", p_szSharedMask, p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Command[15], Location, "../../Assets/CC/CC15.bmp", p_szSharedMask, p_Log); // Bright.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Load(&p_Assets->Command[16], Location, "../../Assets/CC/CC16.bmp", p_szSharedMask, p_Log); // Bright.
    PICTURE_Load(&p_Assets->Command[17], Location, "../../Assets/CC/CC17.bmp", p_szSharedMask, p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Command[18], Location, "../../Assets/CC/CC18.bmp", p_szSharedMask, p_Log); // Brightest.
    PICTURE_Load(&p_Assets->Command[19], Location, "../../Assets/CC/CC19.bmp", p_szSharedMask, p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Command[20], Location, "../../Assets/CC/CC20.bmp", p_szSharedMask, p_Log); // Bright.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Load(&p_Assets->Command[21], Location, "../../Assets/CC/CC21.bmp", p_szSharedMask, p_Log); // Bright.
    PICTURE_Load(&p_Assets->Command[22], Location, "../../Assets/CC/CC22.bmp", p_szSharedMask, p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Command[23], Location, "../../Assets/CC/CC23.bmp", p_szSharedMask, p_Log); // Brightest.
    PICTURE_Load(&p_Assets->Command[24], Location, "../../Assets/CC/CC24.bmp", p_szSharedMask, p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Command[25], Location, "../../Assets/CC/CC25.bmp", p_szSharedMask, p_Log); // Bright.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_T ScaleFrom = p_Assets->Command[0];
    FDELTA_T ScaledSize = {
        p_Settings->fMinimapSize * (ScaleFrom.Bitmap.bmWidth / (float)p_Globals->TerrainCombinedSize.ulDx),
        p_Settings->fMinimapSize * (ScaleFrom.Bitmap.bmHeight / (float)p_Globals->TerrainCombinedSize.ulDy)
    };
    PICTURE_Scale(&ScaleFrom, &p_Assets->CommandMini, hWnd, ScaledSize, p_Log);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_LoadGas(ASSETS_T* p_Assets, FPOINT_T Location, HWND hWnd, LOG_T* p_Log) {
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading gas...");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Load(&p_Assets->Gas[0], Location, "../../Assets/GA/GA0.bmp", NULL, p_Log);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_LoadHud(ASSETS_T* p_Assets, FPOINT_T Location, HWND hWnd, LOG_T* p_Log) {
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading HUD...");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Load(&p_Assets->HUD[0], Location, "../../Assets/HU/HU0.bmp", "../../Assets/HU/HU0_M.bmp", p_Log); // Resource background.
    PICTURE_Load(&p_Assets->HUD[1], Location, "../../Assets/HU/HU1.bmp", NULL, p_Log); // Taskbar and minimap background.
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_LoadMineralField(ASSETS_T* p_Assets, FPOINT_T Location, HWND hWnd, SETTINGS_T* p_Settings, GLOBALS_T* p_Globals, LOG_T* p_Log) {
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading mineral field...");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Load(&p_Assets->Mineral[0], Location, "../../Assets/MF/MF0.bmp", "../../Assets/MF/MF0_M.bmp", p_Log); // 100%
    PICTURE_Load(&p_Assets->Mineral[1], Location, "../../Assets/MF/MF1.bmp", "../../Assets/MF/MF1_M.bmp", p_Log); // 75%
    PICTURE_Load(&p_Assets->Mineral[2], Location, "../../Assets/MF/MF2.bmp", "../../Assets/MF/MF2_M.bmp", p_Log); // 50%
    PICTURE_Load(&p_Assets->Mineral[3], Location, "../../Assets/MF/MF3.bmp", "../../Assets/MF/MF3_M.bmp", p_Log); // 25%
    PICTURE_Load(&p_Assets->Mineral[4], Location, "../../Assets/MF/MF4.bmp", "../../Assets/MF/MF4_M.bmp", p_Log); // 0%
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_T ScaleFrom = p_Assets->Mineral[0];
    FDELTA_T ScaledSize = {
        p_Settings->fMinimapSize * (ScaleFrom.Bitmap.bmWidth / (float)p_Globals->TerrainCombinedSize.ulDx),
        p_Settings->fMinimapSize * (ScaleFrom.Bitmap.bmHeight / (float)p_Globals->TerrainCombinedSize.ulDy)
    };
    PICTURE_Scale(&ScaleFrom, &p_Assets->MineralMini, hWnd, ScaledSize, p_Log);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_LoadRefinery(ASSETS_T* p_Assets, FPOINT_T Location, HWND hWnd, SETTINGS_T* p_Settings, GLOBALS_T* p_Globals, LOG_T* p_Log) {
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading refinery...");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Load(&p_Assets->Refinery[0], Location, "../../Assets/RE/RE0.bmp", "../../Assets/RE/RE0_M.bmp", p_Log);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_T ScaleFrom = p_Assets->Refinery[0];
    FDELTA_T ScaledSize = {
        p_Settings->fMinimapSize * (ScaleFrom.Bitmap.bmWidth / (float)p_Globals->TerrainCombinedSize.ulDx),
        p_Settings->fMinimapSize * (ScaleFrom.Bitmap.bmHeight / (float)p_Globals->TerrainCombinedSize.ulDy)
    };
    PICTURE_Scale(&ScaleFrom, &p_Assets->RefineryMini, hWnd, ScaledSize, p_Log);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_LoadSupplyDepot(ASSETS_T* p_Assets, FPOINT_T Location, HWND hWnd, SETTINGS_T* p_Settings, GLOBALS_T* p_Globals, LOG_T* p_Log) {
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading supply depot...");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    char* p_szSharedMask = "../../Assets/SU/SU0_M.bmp";
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Load(&p_Assets->Supply[0], Location, "../../Assets/SU/SU0.bmp", p_szSharedMask, p_Log); // Original.
    PICTURE_Load(&p_Assets->Supply[1], Location, "../../Assets/SU/SU1.bmp", p_szSharedMask, p_Log); // Bright.
    PICTURE_Load(&p_Assets->Supply[2], Location, "../../Assets/SU/SU2.bmp", p_szSharedMask, p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Supply[3], Location, "../../Assets/SU/SU3.bmp", p_szSharedMask, p_Log); // Brightest.
    PICTURE_Load(&p_Assets->Supply[4], Location, "../../Assets/SU/SU4.bmp", p_szSharedMask, p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Supply[5], Location, "../../Assets/SU/SU5.bmp", p_szSharedMask, p_Log); // Bright.

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_T ScaleFrom = p_Assets->Supply[0];
    FDELTA_T ScaledSize = {
        p_Settings->fMinimapSize * (ScaleFrom.Bitmap.bmWidth / (float)p_Globals->TerrainCombinedSize.ulDx),
        p_Settings->fMinimapSize * (ScaleFrom.Bitmap.bmHeight / (float)p_Globals->TerrainCombinedSize.ulDy)
    };
    PICTURE_Scale(&ScaleFrom, &p_Assets->SupplyMini, hWnd, ScaledSize, p_Log);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_LoadTerrain(ASSETS_T* p_Assets, FPOINT_T Location, GLOBALS_T* p_Globals, CONSTANTS_T* p_Constants, HWND hWnd, SETTINGS_T* p_Settings, LOG_T* p_Log) {
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading terrain...");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UINT uiAllocations = p_Constants->uiTerrainTilesX * p_Constants->uiTerrainTilesY * sizeof(PICTURE_T);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Assets->stAllocationsTerrain = uiAllocations;
    p_Assets->Terrain = (PICTURE_T*)malloc(p_Assets->stAllocationsTerrain);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!p_Assets->Terrain) {
        MISC_WriteOutParams(p_Log, LOG_SEVERITY_ERROR, "ASSETS_LoadTerrain(): Malloc failed for size: %zu bytes\n", p_Assets->stAllocationsTerrain);
        return;
    }
    p_Globals->stAllocations += p_Assets->stAllocationsTerrain;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Assets->stAllocationsTerrainMini = uiAllocations;
    p_Assets->TerrainMini = (PICTURE_T*)malloc(p_Assets->stAllocationsTerrainMini);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!p_Assets->TerrainMini) {
        MISC_WriteOutParams(p_Log, LOG_SEVERITY_ERROR, "ASSETS_LoadTerrain(): Malloc failed for size: %zu bytes\n", p_Assets->stAllocationsTerrainMini);
        return;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->stAllocations += p_Assets->stAllocationsTerrainMini;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UINT uiX, uiY, uiRowOffset;
    for (uiY = 0; uiY < p_Constants->uiTerrainTilesY; uiY++) {
        uiRowOffset = uiY * p_Constants->uiTerrainTilesX;
        for (uiX = 0; uiX < p_Constants->uiTerrainTilesX; uiX++) {
            PICTURE_Load(&p_Assets->Terrain[uiRowOffset + uiX], Location, "../../Assets/TE/TE0.bmp", NULL, p_Log);
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Common main terrain tile size.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BITMAP Tile = p_Assets->Terrain[0].Bitmap;
    p_Globals->TerrainTileSize.uiDx = Tile.bmWidth;
    p_Globals->TerrainTileSize.uiDy = Tile.bmHeight;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->TerrainCombinedSize.ulDx = p_Constants->uiTerrainTilesX * Tile.bmWidth;
    p_Globals->TerrainCombinedSize.ulDy = p_Constants->uiTerrainTilesY * Tile.bmHeight;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // These terrain loading loops can be combined but it's clearer to have the tile sizes and full map size present at this time.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UINT uiIndex;
    for (uiY = 0; uiY < p_Constants->uiTerrainTilesY; uiY++) {
        uiRowOffset = uiY * p_Constants->uiTerrainTilesX;
        for (uiX = 0; uiX < p_Constants->uiTerrainTilesX; uiX++) {
            uiIndex = uiRowOffset + uiX;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            PICTURE_T* p_ScaleFrom = &p_Assets->Terrain[uiIndex];
            FDELTA_T MinimapRelativeSize = {
                p_Settings->fMinimapSize * (p_ScaleFrom->Bitmap.bmWidth / (float)p_Globals->TerrainCombinedSize.ulDx),
                p_Settings->fMinimapSize * (p_ScaleFrom->Bitmap.bmHeight / (float)p_Globals->TerrainCombinedSize.ulDy)
            };
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            PICTURE_Scale(p_ScaleFrom, &p_Assets->TerrainMini[uiIndex], hWnd, MinimapRelativeSize, p_Log);
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Common mini terrain tile size.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BITMAP MinimapTile = p_Assets->TerrainMini[0].Bitmap;
    p_Globals->TerrainMinimapTileSize.uiDx = MinimapTile.bmWidth;
    p_Globals->TerrainMinimapTileSize.uiDy = MinimapTile.bmHeight;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_LoadWorker(ASSETS_T* p_Assets, FPOINT_T Location, HWND hWnd, SETTINGS_T* p_Settings, GLOBALS_T* p_Globals, LOG_T* p_Log) {
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading worker image(s)...");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    char* p_szSharedMask = "../../Assets/WO/WO0_M.bmp";
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Load(&p_Assets->Worker[0], Location, "../../Assets/WO/WO0.bmp", p_szSharedMask, p_Log); // Not carrying.
    PICTURE_Load(&p_Assets->Worker[1], Location, "../../Assets/WO/WO1.bmp", p_szSharedMask, p_Log); // Carrying minerals.
    PICTURE_Load(&p_Assets->Worker[2], Location, "../../Assets/WO/WO2.bmp", p_szSharedMask, p_Log); // Carrying gas.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_T ScaleFrom = p_Assets->Worker[0];
    FDELTA_T ScaledSize = {
        p_Settings->fMinimapSize * (ScaleFrom.Bitmap.bmWidth / (float)p_Globals->TerrainCombinedSize.ulDx),
        p_Settings->fMinimapSize * (ScaleFrom.Bitmap.bmHeight / (float)p_Globals->TerrainCombinedSize.ulDy)
    };
    PICTURE_Scale(&ScaleFrom, &p_Assets->WorkerMini, hWnd, ScaledSize, p_Log);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HWND is the main window handle.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_HandleBitmaps(ASSETS_T* p_Assets, HWND hWnd, CONSTANTS_T* p_Constants, SETTINGS_T* p_Settings, GLOBALS_T* p_Globals, LOG_T* p_Log) {
    FPOINT_T Location = { 0.0f };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // First.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ASSETS_LoadTerrain(p_Assets, Location, p_Globals, p_Constants, hWnd, p_Settings, p_Log);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ASSETS_LoadBlitter(p_Assets, Location, hWnd, p_Log);
    ASSETS_LoadCommandCard(p_Assets, Location, hWnd, p_Log);
    ASSETS_LoadCommandCenter(p_Assets, Location, hWnd, p_Settings, p_Globals, p_Log);
    ASSETS_LoadGas(p_Assets, Location, hWnd, p_Log);
    ASSETS_LoadHud(p_Assets, Location, hWnd, p_Log);
    ASSETS_LoadMineralField(p_Assets, Location, hWnd, p_Settings, p_Globals, p_Log);
    ASSETS_LoadRefinery(p_Assets, Location, hWnd, p_Settings, p_Globals, p_Log);
    ASSETS_LoadSupplyDepot(p_Assets, Location, hWnd, p_Settings, p_Globals, p_Log);
    ASSETS_LoadWorker(p_Assets, Location, hWnd, p_Settings, p_Globals, p_Log);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_CreateBrushes(ASSETS_T* p_Assets) {
    p_Assets->hBrushSelected = CreateSolidBrush(RGB(0, 255, 0));
    p_Assets->hBrushHighlighted = CreateSolidBrush(RGB(100, 0, 100));
    p_Assets->hBrushWorker = CreateSolidBrush(RGB(255, 255, 255));
    p_Assets->hBrushCommand = CreateSolidBrush(RGB(255, 0, 0));
    p_Assets->hBrushMineral = CreateSolidBrush(RGB(110, 175, 225));
    p_Assets->hBrushSupply = CreateSolidBrush(RGB(255, 255, 0));
    p_Assets->hBrushRefinery = CreateSolidBrush(RGB(0, 175, 0));
    p_Assets->hBrushBufferClear = CreateSolidBrush(RGB(0, 0, 0));
    p_Assets->hBrushClear = CreateSolidBrush(RGB(0, 0, 0));
    p_Assets->hBrush100r0g0b = CreateSolidBrush(RGB(100, 0, 0));
    p_Assets->hBrush0r100g0b = CreateSolidBrush(RGB(0, 100, 0));
    p_Assets->hBrush0r0g100b = CreateSolidBrush(RGB(0, 0, 100));
    p_Assets->hBrush100r100g0b = CreateSolidBrush(RGB(100, 100, 0));
    p_Assets->hBrush0r100g100b = CreateSolidBrush(RGB(0, 100, 100));
    p_Assets->hBrush100r0g100b = CreateSolidBrush(RGB(100, 0, 100));
    p_Assets->hBrush100r100g100b = CreateSolidBrush(RGB(100, 100, 100));
    p_Assets->hBrushWhite = CreateSolidBrush(RGB(255, 255, 255));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_CreatePens(ASSETS_T* p_Assets) {
    p_Assets->hPenSelected = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    p_Assets->hPenHighlighted = CreatePen(PS_SOLID, 1, RGB(100, 0, 100));
    p_Assets->hPenWorker = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    p_Assets->hPenCommand = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    p_Assets->hPenMineral = CreatePen(PS_SOLID, 1, RGB(110, 175, 225));
    p_Assets->hPenSupply = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
    p_Assets->hPenRefinery = CreatePen(PS_SOLID, 1, RGB(0, 175, 0));
    p_Assets->hPenViewport = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
    p_Assets->hPenSelectionArea = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    p_Assets->hPenBuildLimits = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    p_Assets->hPenBuildType = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    p_Assets->hPenTranslation = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
    p_Assets->hPenMinorVector = CreatePen(PS_SOLID, 1, RGB(255, 165, 0));
    p_Assets->hPenMajorVector = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
    p_Assets->hPenDirtyZone = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
    p_Assets->hPenTerrainGrid = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
    p_Assets->hPenWhite = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_KillBrushes(ASSETS_T* p_Assets) {
    DeleteObject(p_Assets->hBrushSelected);
    DeleteObject(p_Assets->hBrushHighlighted);
    DeleteObject(p_Assets->hBrushWorker);
    DeleteObject(p_Assets->hBrushCommand);
    DeleteObject(p_Assets->hBrushMineral);
    DeleteObject(p_Assets->hBrushSupply);
    DeleteObject(p_Assets->hBrushRefinery);
    DeleteObject(p_Assets->hBrushBufferClear);
    DeleteObject(p_Assets->hBrushClear);
    DeleteObject(p_Assets->hBrush100r0g0b);
    DeleteObject(p_Assets->hBrush0r100g0b);
    DeleteObject(p_Assets->hBrush0r0g100b);
    DeleteObject(p_Assets->hBrush100r100g0b);
    DeleteObject(p_Assets->hBrush0r100g100b);
    DeleteObject(p_Assets->hBrush100r0g100b);
    DeleteObject(p_Assets->hBrush100r100g100b);
    DeleteObject(p_Assets->hBrushWhite);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_KillPens(ASSETS_T* p_Assets) {
    DeleteObject(p_Assets->hPenSelected);
    DeleteObject(p_Assets->hPenHighlighted);
    DeleteObject(p_Assets->hPenWorker);
    DeleteObject(p_Assets->hPenCommand);
    DeleteObject(p_Assets->hPenMineral);
    DeleteObject(p_Assets->hPenSupply);
    DeleteObject(p_Assets->hPenRefinery);
    DeleteObject(p_Assets->hPenViewport);
    DeleteObject(p_Assets->hPenSelectionArea);
    DeleteObject(p_Assets->hPenBuildLimits);
    DeleteObject(p_Assets->hPenBuildType);
    DeleteObject(p_Assets->hPenTranslation);
    DeleteObject(p_Assets->hPenMinorVector);
    DeleteObject(p_Assets->hPenMajorVector);
    DeleteObject(p_Assets->hPenDirtyZone);
    DeleteObject(p_Assets->hPenTerrainGrid);
    DeleteObject(p_Assets->hPenWhite);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_KillBitmaps(ASSETS_T* p_Assets, GLOBALS_T* p_Globals, CONSTANTS_T* p_Constants) {
    UINT8 ubI;
    UINT uiX, uiY, uiRowOffset, uiIndex;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Main images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 0; ubI < BLITTER_COLOR_COUNT; ubI++) {
        PICTURE_Kill(&p_Assets->Blitter[ubI]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 0; ubI < 5; ubI++) {
        PICTURE_Kill(&p_Assets->Card[ubI]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 0; ubI < 26; ubI++) {
        PICTURE_Kill(&p_Assets->Command[ubI]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 0; ubI < 1; ubI++) {
        PICTURE_Kill(&p_Assets->Gas[ubI]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 0; ubI < 2; ubI++) {
        PICTURE_Kill(&p_Assets->HUD[ubI]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 0; ubI < 5; ubI++) {
        PICTURE_Kill(&p_Assets->Mineral[ubI]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 0; ubI < 1; ubI++) {
        PICTURE_Kill(&p_Assets->Minimap[ubI]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 0; ubI < 1; ubI++) {
        PICTURE_Kill(&p_Assets->Refinery[ubI]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 0; ubI < 6; ubI++) {
        PICTURE_Kill(&p_Assets->Supply[ubI]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (uiY = 0; uiY < p_Constants->uiTerrainTilesY; uiY++) {
        uiRowOffset = uiY * p_Constants->uiTerrainTilesX;
        for (uiX = 0; uiX < p_Constants->uiTerrainTilesX; uiX++) {
            uiIndex = uiRowOffset + uiX;
            PICTURE_Kill(&p_Assets->Terrain[uiIndex]);
            PICTURE_Kill(&p_Assets->TerrainMini[uiIndex]);
        }
    }
    free(p_Assets->Terrain);
    p_Globals->stAllocations -= p_Assets->stAllocationsTerrain;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    free(p_Assets->TerrainMini);
    p_Globals->stAllocations -= p_Assets->stAllocationsTerrain;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 0; ubI < 3; ubI++) {
        PICTURE_Kill(&p_Assets->Worker[ubI]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Minimap images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Kill(&p_Assets->CommandMini);
    PICTURE_Kill(&p_Assets->SupplyMini);
    PICTURE_Kill(&p_Assets->WorkerMini);
    PICTURE_Kill(&p_Assets->MineralMini);
    PICTURE_Kill(&p_Assets->RefineryMini);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_Kill(ASSETS_T** pp_Assets, GLOBALS_T* p_Globals, CONSTANTS_T* p_Constants) {
    ASSETS_T* p_Assets = *pp_Assets;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_Assets) {
        ASSETS_KillBrushes(p_Assets);
        ASSETS_KillPens(p_Assets);
        ASSETS_KillBitmaps(p_Assets, p_Globals, p_Constants);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        free(p_Assets);
        p_Globals->stAllocations -= sizeof(ASSETS_T);
        *pp_Assets = NULL;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
