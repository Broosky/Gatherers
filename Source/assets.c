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
// Dialog window handle only.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_LoadBitmaps(ASSETS_T* p_Assets, HWND hWnd, LOG_T* p_Log) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Blitter images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading blitter image(s)...");
    PICTURE_Load(&p_Assets->Blitter[0], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/BL/BB0.bmp", "../../Assets/BL/B_M.bmp", p_Log); // Light blue.
    PICTURE_Load(&p_Assets->Blitter[1], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/BL/BB1.bmp", "../../Assets/BL/B_M.bmp", p_Log); // Dark blue.
    PICTURE_Load(&p_Assets->Blitter[2], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/BL/BG0.bmp", "../../Assets/BL/B_M.bmp", p_Log); // Green.
    PICTURE_Load(&p_Assets->Blitter[3], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/BL/BP0.bmp", "../../Assets/BL/B_M.bmp", p_Log); // Purple.
    PICTURE_Load(&p_Assets->Blitter[4], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/BL/BR0.bmp", "../../Assets/BL/B_M.bmp", p_Log); // Red.
    PICTURE_Load(&p_Assets->Blitter[5], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/BL/BW0.bmp", "../../Assets/BL/B_M.bmp", p_Log); // White.
    PICTURE_Load(&p_Assets->Blitter[6], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/BL/BY0.bmp", "../../Assets/BL/B_M.bmp", p_Log); // Yellow.
    PICTURE_Load(&p_Assets->Blitter[7], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/BL/BO0.bmp", "../../Assets/BL/B_M.bmp", p_Log); // Orange.
    PICTURE_Load(&p_Assets->Blitter[8], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/BL/BP1.bmp", "../../Assets/BL/B_M.bmp", p_Log); // Pink.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Worker images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading worker image(s)...");
    PICTURE_Load(&p_Assets->Worker[0], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/WO/WO0.bmp", "../../Assets/WO/WO0_M.bmp", p_Log); // Not carrying.
    PICTURE_Load(&p_Assets->Worker[1], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/WO/WO1.bmp", "../../Assets/WO/WO0_M.bmp", p_Log); // Carrying minerals.
    PICTURE_Load(&p_Assets->Worker[2], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/WO/WO2.bmp", "../../Assets/WO/WO0_M.bmp", p_Log); // Carrying gas.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Command center images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Receiving minerals.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading command center image(s)...");
    PICTURE_Load(&p_Assets->Command[0], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC0.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Original.
    PICTURE_Load(&p_Assets->Command[1], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC1.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Bright.
    PICTURE_Load(&p_Assets->Command[2], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC2.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Command[3], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC3.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Brightest.
    PICTURE_Load(&p_Assets->Command[4], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC4.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Command[5], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC5.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Bright.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Building a unit.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // East corner green.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Load(&p_Assets->Command[6], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC6.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Bright.
    PICTURE_Load(&p_Assets->Command[7], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC7.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Command[8], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC8.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Brightest.
    PICTURE_Load(&p_Assets->Command[9], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC9.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Command[10], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC10.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Bright.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // South corner green.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Load(&p_Assets->Command[11], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC11.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Bright.
    PICTURE_Load(&p_Assets->Command[12], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC12.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Command[13], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC13.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Brightest.
    PICTURE_Load(&p_Assets->Command[14], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC14.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Command[15], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC15.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Bright.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // West corner green.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Load(&p_Assets->Command[16], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC16.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Bright.
    PICTURE_Load(&p_Assets->Command[17], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC17.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Command[18], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC18.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Brightest.
    PICTURE_Load(&p_Assets->Command[19], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC19.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Command[20], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC20.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Bright.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Center lights green.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Load(&p_Assets->Command[21], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC21.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Bright.
    PICTURE_Load(&p_Assets->Command[22], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC22.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Command[23], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC23.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Brightest.
    PICTURE_Load(&p_Assets->Command[24], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC24.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Command[25], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC25.bmp", "../../Assets/CC/CC0_M.bmp", p_Log); // Bright.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Mineral field images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading mineral field image(s)...");
    PICTURE_Load(&p_Assets->Mineral[0], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/MF/MF0.bmp", "../../Assets/MF/MF0_M.bmp", p_Log); // 100%
    PICTURE_Load(&p_Assets->Mineral[1], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/MF/MF1.bmp", "../../Assets/MF/MF1_M.bmp", p_Log); // 75%
    PICTURE_Load(&p_Assets->Mineral[2], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/MF/MF2.bmp", "../../Assets/MF/MF2_M.bmp", p_Log); // 50%
    PICTURE_Load(&p_Assets->Mineral[3], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/MF/MF3.bmp", "../../Assets/MF/MF3_M.bmp", p_Log); // 25%
    PICTURE_Load(&p_Assets->Mineral[4], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/MF/MF4.bmp", "../../Assets/MF/MF4_M.bmp", p_Log); // 0%
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Supply depot images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading supply depot image(s)...");
    PICTURE_Load(&p_Assets->Supply[0], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/SU/SU0.bmp", "../../Assets/SU/SU0_M.bmp", p_Log); // Original.
    PICTURE_Load(&p_Assets->Supply[1], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/SU/SU1.bmp", "../../Assets/SU/SU0_M.bmp", p_Log); // Bright.
    PICTURE_Load(&p_Assets->Supply[2], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/SU/SU2.bmp", "../../Assets/SU/SU0_M.bmp", p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Supply[3], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/SU/SU3.bmp", "../../Assets/SU/SU0_M.bmp", p_Log); // Brightest.
    PICTURE_Load(&p_Assets->Supply[4], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/SU/SU4.bmp", "../../Assets/SU/SU0_M.bmp", p_Log); // Brighter.
    PICTURE_Load(&p_Assets->Supply[5], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/SU/SU5.bmp", "../../Assets/SU/SU0_M.bmp", p_Log); // Bright.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Refinery images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading refinery image(s)...");
    PICTURE_Load(&p_Assets->Refinery[0], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/RE/RE0.bmp", "../../Assets/RE/RE0_M.bmp", p_Log); // Original.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // HUD images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading HUD image(s)...");
    PICTURE_Load(&p_Assets->HUD[0], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/HU/HU0.bmp", "../../Assets/HU/HU0_M.bmp", p_Log); // Resource background.
    PICTURE_Load(&p_Assets->HUD[1], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/HU/HU1.bmp", "../../Assets/HU/HU1.bmp", p_Log); // Taskbar and minimap background.
    PICTURE_Load(&p_Assets->HUD[2], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/HU/HU2.bmp", "../../Assets/HU/HU2_M.bmp", p_Log); // Resource background.
    PICTURE_Load(&p_Assets->HUD[3], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/HU/HU3.bmp", "../../Assets/HU/HU3_M.bmp", p_Log); // Resource background.
    PICTURE_Load(&p_Assets->HUD[4], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/HU/HU4.bmp", "../../Assets/HU/HU4_M.bmp", p_Log); // Resource background.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Minimap images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading minimap image(s)...");
    PICTURE_Load(&p_Assets->Minimap[0], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/MI/MI0.bmp", "../../Assets/MI/MI0.bmp", p_Log); // Regular sized grass.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Gas images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading gas image(s)...");
    PICTURE_Load(&p_Assets->Gas[0], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/GA/GA0.bmp", "../../Assets/GA/GA0.bmp", p_Log); // Original.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Terrain images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading terrain image(s)...");
    PICTURE_Load(&p_Assets->Terrain[0], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/TE/TE0.bmp", "../../Assets/TE/TE0.bmp", p_Log); // Regular sized grass.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Command card images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading command card image(s)...");
    PICTURE_Load(&p_Assets->Card[0], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CA/CA0.bmp", "../../Assets/CA/CA0.bmp", p_Log); // Blank card.
    PICTURE_Load(&p_Assets->Card[1], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CA/CA1.bmp", "../../Assets/CA/CA1.bmp", p_Log); // Stop and move card.
    PICTURE_Load(&p_Assets->Card[2], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CA/CA2.bmp", "../../Assets/CA/CA2.bmp", p_Log); // Specific structure.
    PICTURE_Load(&p_Assets->Card[3], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CA/CA3.bmp", "../../Assets/CA/CA3.bmp", p_Log); // Specific worker.
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_CreateBrushes(ASSETS_T* p_Assets) {
    p_Assets->hBrushSelected =      CreateSolidBrush(RGB(0,     255,    0));
    p_Assets->hBrushHighlighted =   CreateSolidBrush(RGB(100,   0,      100));
    p_Assets->hBrushWorker =        CreateSolidBrush(RGB(255,   255,    255));
    p_Assets->hBrushCommand =       CreateSolidBrush(RGB(255,   0,      0));
    p_Assets->hBrushMineral =       CreateSolidBrush(RGB(110,   175,    225));
    p_Assets->hBrushSupply =        CreateSolidBrush(RGB(255,   255,    0));
    p_Assets->hBrushRefinery =      CreateSolidBrush(RGB(0,     175,    0));
    p_Assets->hBrushBufferClear =   CreateSolidBrush(RGB(0,     0,      0));
    p_Assets->hBrushClear =         CreateSolidBrush(RGB(0,     0,      0));
    p_Assets->hBrush100r0g0b =      CreateSolidBrush(RGB(100,   0,      0));
    p_Assets->hBrush0r100g0b =      CreateSolidBrush(RGB(0,     100,    0));
    p_Assets->hBrush0r0g100b =      CreateSolidBrush(RGB(0,     0,      100));
    p_Assets->hBrush100r100g0b =    CreateSolidBrush(RGB(100,   100,    0));
    p_Assets->hBrush0r100g100b =    CreateSolidBrush(RGB(0,     100,    100));
    p_Assets->hBrush100r0g100b =    CreateSolidBrush(RGB(100,   0,      100));
    p_Assets->hBrush100r100g100b =  CreateSolidBrush(RGB(100,   100,    100));
    p_Assets->hBrushWhite =         CreateSolidBrush(RGB(255,   255,    255));
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Assets->hPenSelected =        CreatePen(PS_SOLID, 1, RGB(0,       255,    0));
    p_Assets->hPenHighlighted =     CreatePen(PS_SOLID, 1, RGB(100,     0,      100));
    p_Assets->hPenWorker =          CreatePen(PS_SOLID, 1, RGB(255,     255,    255));
    p_Assets->hPenCommand =         CreatePen(PS_SOLID, 1, RGB(255,     0,      0));
    p_Assets->hPenMineral =         CreatePen(PS_SOLID, 1, RGB(110,     175,    225));
    p_Assets->hPenSupply =          CreatePen(PS_SOLID, 1, RGB(255,     255,    0));
    p_Assets->hPenRefinery =        CreatePen(PS_SOLID, 1, RGB(0,       175,    0));
    p_Assets->hPenViewport =        CreatePen(PS_SOLID, 1, RGB(200,     200,    200));
    p_Assets->hPenSelectionArea =   CreatePen(PS_SOLID, 1, RGB(0,       255,    0));
    p_Assets->hPenBuildLimits =     CreatePen(PS_SOLID, 1, RGB(255,     0,      0));
    p_Assets->hPenBuildType =       CreatePen(PS_SOLID, 1, RGB(255,     0,      0));
    p_Assets->hPenTranslation =     CreatePen(PS_SOLID, 1, RGB(200,     200,    200));
    p_Assets->hPenMinorVector =     CreatePen(PS_SOLID, 1, RGB(255,     165,    0));
    p_Assets->hPenMajorVector =     CreatePen(PS_SOLID, 1, RGB(0,       255,    255));
    p_Assets->hPenDirtyZone =       CreatePen(PS_SOLID, 1, RGB(255,     0,      255));
    p_Assets->hPenWhite =           CreatePen(PS_SOLID, 1, RGB(255,     255,    255));
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
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
    DeleteObject(p_Assets->hPenWhite);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_KillBitmaps(ASSETS_T* p_Assets) {
    UINT8 ubI;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 0; ubI < 9; ubI++) {
        PICTURE_Kill(&p_Assets->Blitter[ubI]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 0; ubI < 3; ubI++) {
        PICTURE_Kill(&p_Assets->Worker[ubI]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 0; ubI < 26; ubI++) {
        PICTURE_Kill(&p_Assets->Command[ubI]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 0; ubI < 5; ubI++) {
        PICTURE_Kill(&p_Assets->Mineral[ubI]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 0; ubI < 6; ubI++) {
        PICTURE_Kill(&p_Assets->Supply[ubI]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 0; ubI < 1; ubI++) {
        PICTURE_Kill(&p_Assets->Refinery[ubI]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 0; ubI < 5; ubI++) {
        PICTURE_Kill(&p_Assets->HUD[ubI]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 0; ubI < 1; ubI++) {
        PICTURE_Kill(&p_Assets->Minimap[ubI]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 0; ubI < 1; ubI++) {
        PICTURE_Kill(&p_Assets->Gas[ubI]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 0; ubI < 1; ubI++) {
        PICTURE_Kill(&p_Assets->Terrain[ubI]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 0; ubI < 5; ubI++) {
        PICTURE_Kill(&p_Assets->Card[ubI]);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_Kill(ASSETS_T** pp_Assets, GLOBALS_T* p_Globals) {
    ASSETS_T* p_Assets = *pp_Assets;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_Assets) {
        ASSETS_KillBrushes(p_Assets);
        ASSETS_KillBitmaps(p_Assets);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        free(p_Assets);
        p_Globals->stAllocations -= sizeof(ASSETS_T);
        *pp_Assets = NULL;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
