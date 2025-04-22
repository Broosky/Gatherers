/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/assets.h"
#include "../Headers/common.h"
#include "../Headers/constants.h"
#include "../Headers/globals.h"
#include <stdlib.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_Zero(ASSETS_T* p_Assets) {
    ZeroMemory(p_Assets, sizeof(ASSETS_T));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASSETS_T* __cdecl ASSETS_Create(GLOBALS_T* p_Globals) {
    ASSETS_T* p_Assets = (ASSETS_T*)malloc(sizeof(ASSETS_T));
    p_Globals->iRunningHeap += sizeof(ASSETS_T);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ASSETS_Zero(p_Assets);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return p_Assets;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_InitFromFile(ASSETS_T* p_Assets, HWND hWnd) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Blitter images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading blitter image(s)...");
    PICTURE_Load(&p_Assets->Blitter[0], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/BL/BB0.bmp", "../../Assets/BL/B_M.bmp"); // Light blue.
    PICTURE_Load(&p_Assets->Blitter[1], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/BL/BB1.bmp", "../../Assets/BL/B_M.bmp"); // Dark blue.
    PICTURE_Load(&p_Assets->Blitter[2], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/BL/BG0.bmp", "../../Assets/BL/B_M.bmp"); // Green.
    PICTURE_Load(&p_Assets->Blitter[3], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/BL/BP0.bmp", "../../Assets/BL/B_M.bmp"); // Purple.
    PICTURE_Load(&p_Assets->Blitter[4], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/BL/BR0.bmp", "../../Assets/BL/B_M.bmp"); // Red.
    PICTURE_Load(&p_Assets->Blitter[5], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/BL/BW0.bmp", "../../Assets/BL/B_M.bmp"); // White.
    PICTURE_Load(&p_Assets->Blitter[6], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/BL/BY0.bmp", "../../Assets/BL/B_M.bmp"); // Yellow.
    PICTURE_Load(&p_Assets->Blitter[7], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/BL/BO0.bmp", "../../Assets/BL/B_M.bmp"); // Orange.
    PICTURE_Load(&p_Assets->Blitter[8], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/BL/BP1.bmp", "../../Assets/BL/B_M.bmp"); // Pink.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Worker images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading worker image(s)...");
    PICTURE_Load(&p_Assets->Worker[0], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/WO/WO0.bmp", "../../Assets/WO/WO0_M.bmp"); // Not carrying.
    PICTURE_Load(&p_Assets->Worker[1], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/WO/WO1.bmp", "../../Assets/WO/WO0_M.bmp"); // Carrying minerals.
    PICTURE_Load(&p_Assets->Worker[2], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/WO/WO2.bmp", "../../Assets/WO/WO0_M.bmp"); // Carrying gas.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Command center images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Receiving minerals.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading command center image(s)...");
    PICTURE_Load(&p_Assets->Command[0], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC0.bmp", "../../Assets/CC/CC0_M.bmp"); // Original.
    PICTURE_Load(&p_Assets->Command[1], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC1.bmp", "../../Assets/CC/CC0_M.bmp"); // Bright.
    PICTURE_Load(&p_Assets->Command[2], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC2.bmp", "../../Assets/CC/CC0_M.bmp"); // Brighter.
    PICTURE_Load(&p_Assets->Command[3], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC3.bmp", "../../Assets/CC/CC0_M.bmp"); // Brightest.
    PICTURE_Load(&p_Assets->Command[4], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC4.bmp", "../../Assets/CC/CC0_M.bmp"); // Brighter.
    PICTURE_Load(&p_Assets->Command[5], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC5.bmp", "../../Assets/CC/CC0_M.bmp"); // Bright.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Building a unit.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // East corner green.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Load(&p_Assets->Command[6], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC6.bmp", "../../Assets/CC/CC0_M.bmp"); // Bright.
    PICTURE_Load(&p_Assets->Command[7], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC7.bmp", "../../Assets/CC/CC0_M.bmp"); // Brighter.
    PICTURE_Load(&p_Assets->Command[8], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC8.bmp", "../../Assets/CC/CC0_M.bmp"); // Brightest.
    PICTURE_Load(&p_Assets->Command[9], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC9.bmp", "../../Assets/CC/CC0_M.bmp"); // Brighter.
    PICTURE_Load(&p_Assets->Command[10], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC10.bmp", "../../Assets/CC/CC0_M.bmp"); // Bright.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // South corner green.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Load(&p_Assets->Command[11], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC11.bmp", "../../Assets/CC/CC0_M.bmp"); // Bright.
    PICTURE_Load(&p_Assets->Command[12], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC12.bmp", "../../Assets/CC/CC0_M.bmp"); // Brighter.
    PICTURE_Load(&p_Assets->Command[13], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC13.bmp", "../../Assets/CC/CC0_M.bmp"); // Brightest.
    PICTURE_Load(&p_Assets->Command[14], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC14.bmp", "../../Assets/CC/CC0_M.bmp"); // Brighter.
    PICTURE_Load(&p_Assets->Command[15], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC15.bmp", "../../Assets/CC/CC0_M.bmp"); // Bright.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // West corner green.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Load(&p_Assets->Command[16], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC16.bmp", "../../Assets/CC/CC0_M.bmp"); // Bright.
    PICTURE_Load(&p_Assets->Command[17], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC17.bmp", "../../Assets/CC/CC0_M.bmp"); // Brighter.
    PICTURE_Load(&p_Assets->Command[18], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC18.bmp", "../../Assets/CC/CC0_M.bmp"); // Brightest.
    PICTURE_Load(&p_Assets->Command[19], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC19.bmp", "../../Assets/CC/CC0_M.bmp"); // Brighter.
    PICTURE_Load(&p_Assets->Command[20], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC20.bmp", "../../Assets/CC/CC0_M.bmp"); // Bright.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Center lights green.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Load(&p_Assets->Command[21], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC21.bmp", "../../Assets/CC/CC0_M.bmp"); // Bright.
    PICTURE_Load(&p_Assets->Command[22], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC22.bmp", "../../Assets/CC/CC0_M.bmp"); // Brighter.
    PICTURE_Load(&p_Assets->Command[23], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC23.bmp", "../../Assets/CC/CC0_M.bmp"); // Brightest.
    PICTURE_Load(&p_Assets->Command[24], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC24.bmp", "../../Assets/CC/CC0_M.bmp"); // Brighter.
    PICTURE_Load(&p_Assets->Command[25], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CC/CC25.bmp", "../../Assets/CC/CC0_M.bmp"); // Bright.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Mineral field images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading mineral field image(s)...");
    PICTURE_Load(&p_Assets->Mineral[0], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/MF/MF0.bmp", "../../Assets/MF/MF0_M.bmp"); // 100%
    PICTURE_Load(&p_Assets->Mineral[1], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/MF/MF1.bmp", "../../Assets/MF/MF1_M.bmp"); // 75%
    PICTURE_Load(&p_Assets->Mineral[2], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/MF/MF2.bmp", "../../Assets/MF/MF2_M.bmp"); // 50%
    PICTURE_Load(&p_Assets->Mineral[3], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/MF/MF3.bmp", "../../Assets/MF/MF3_M.bmp"); // 25%
    PICTURE_Load(&p_Assets->Mineral[4], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/MF/MF4.bmp", "../../Assets/MF/MF4_M.bmp"); // 0%
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Supply depot images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading supply depot image(s)...");
    PICTURE_Load(&p_Assets->Supply[0], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/SU/SU0.bmp", "../../Assets/SU/SU0_M.bmp"); // Original.
    PICTURE_Load(&p_Assets->Supply[1], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/SU/SU1.bmp", "../../Assets/SU/SU0_M.bmp"); // Bright.
    PICTURE_Load(&p_Assets->Supply[2], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/SU/SU2.bmp", "../../Assets/SU/SU0_M.bmp"); // Brighter.
    PICTURE_Load(&p_Assets->Supply[3], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/SU/SU3.bmp", "../../Assets/SU/SU0_M.bmp"); // Brightest.
    PICTURE_Load(&p_Assets->Supply[4], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/SU/SU4.bmp", "../../Assets/SU/SU0_M.bmp"); // Brighter.
    PICTURE_Load(&p_Assets->Supply[5], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/SU/SU5.bmp", "../../Assets/SU/SU0_M.bmp"); // Bright.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Refinery images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading refinery image(s)...");
    PICTURE_Load(&p_Assets->Refinery[0], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/RE/RE0.bmp", "../../Assets/RE/RE0_M.bmp"); // Original.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // HUD images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading HUD image(s)...");
    PICTURE_Load(&p_Assets->HUD[0], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/HU/HU0.bmp", "../../Assets/HU/HU0_M.bmp"); // Resource background.
    PICTURE_Load(&p_Assets->HUD[1], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/HU/HU1.bmp", "../../Assets/HU/HU1.bmp"); // Taskbar and minimap background.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Minimap images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading minimap image(s)...");
    PICTURE_Load(&p_Assets->Minimap[0], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/MI/MI0.bmp", "../../Assets/MI/MI0.bmp"); // Regular grass.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Gas images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading gas image(s)...");
    PICTURE_Load(&p_Assets->Gas[0], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/GA/GA0.bmp", "../../Assets/GA/GA0.bmp"); // Original.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Terrain images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading terrain image(s)...");
    PICTURE_Load(&p_Assets->Terrain[0], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/TE/TE0.bmp", "../../Assets/TE/TE0.bmp"); // Regular grass.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Command card images.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading command card image(s)...");
    PICTURE_Load(&p_Assets->Card[0], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CA/CA0.bmp", "../../Assets/CA/CA0.bmp"); // Blank card.
    PICTURE_Load(&p_Assets->Card[1], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CA/CA1.bmp", "../../Assets/CA/CA1.bmp"); // Stop and move card.
    PICTURE_Load(&p_Assets->Card[2], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CA/CA2.bmp", "../../Assets/CA/CA2.bmp"); // Specific structure.
    PICTURE_Load(&p_Assets->Card[3], (FPOINT_T) { 0.0f, 0.0f }, "../../Assets/CA/CA3.bmp", "../../Assets/CA/CA3.bmp"); // Specific worker.
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ASSETS_Kill(ASSETS_T* p_Assets, GLOBALS_T* p_Globals) {
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
    for (ubI = 0; ubI < 2; ubI++) {
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
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    free(p_Assets);
    p_Globals->iRunningHeap -= sizeof(ASSETS_T);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
