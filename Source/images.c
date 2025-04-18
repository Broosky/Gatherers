/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _IMAGES_C_
#define _IMAGES_C_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/constants.h"
#include "../Headers/functions.h"
#include "../Headers/types.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl IMAGES_Zero(IMAGES* p_Images) {
	ZeroMemory(p_Images, sizeof(IMAGES));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMAGES* __cdecl IMAGES_Create(GLOBALS* p_Globals) {
	IMAGES* p_Images = (IMAGES*)malloc(sizeof(IMAGES));
	p_Globals->iRunningHeap += sizeof(IMAGES);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IMAGES_Zero(p_Images);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return p_Images;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl IMAGES_InitFromFile(IMAGES* p_Images, HWND hWnd) {
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Blitter images.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading blitter image(s)...");
	PICTURE_Load(&p_Images->Blitter[0], (FPOINT) { 0.0f, 0.0f }, "../../Images/BL/BB0.bmp", "../../Images/BL/B_M.bmp"); // Light blue.
	PICTURE_Load(&p_Images->Blitter[1], (FPOINT) { 0.0f, 0.0f }, "../../Images/BL/BB1.bmp", "../../Images/BL/B_M.bmp"); // Dark blue.
	PICTURE_Load(&p_Images->Blitter[2], (FPOINT) { 0.0f, 0.0f }, "../../Images/BL/BG0.bmp", "../../Images/BL/B_M.bmp"); // Green.
	PICTURE_Load(&p_Images->Blitter[3], (FPOINT) { 0.0f, 0.0f }, "../../Images/BL/BP0.bmp", "../../Images/BL/B_M.bmp"); // Purple.
	PICTURE_Load(&p_Images->Blitter[4], (FPOINT) { 0.0f, 0.0f }, "../../Images/BL/BR0.bmp", "../../Images/BL/B_M.bmp"); // Red.
	PICTURE_Load(&p_Images->Blitter[5], (FPOINT) { 0.0f, 0.0f }, "../../Images/BL/BW0.bmp", "../../Images/BL/B_M.bmp"); // White.
	PICTURE_Load(&p_Images->Blitter[6], (FPOINT) { 0.0f, 0.0f }, "../../Images/BL/BY0.bmp", "../../Images/BL/B_M.bmp"); // Yellow.
	PICTURE_Load(&p_Images->Blitter[7], (FPOINT) { 0.0f, 0.0f }, "../../Images/BL/BO0.bmp", "../../Images/BL/B_M.bmp"); // Orange.
	PICTURE_Load(&p_Images->Blitter[8], (FPOINT) { 0.0f, 0.0f }, "../../Images/BL/BP1.bmp", "../../Images/BL/B_M.bmp"); // Pink.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Worker images.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading worker image(s)...");
	PICTURE_Load(&p_Images->Worker[0], (FPOINT) { 0.0f, 0.0f }, "../../Images/WO/WO0.bmp", "../../Images/WO/WO0_M.bmp"); // Not carrying.
	PICTURE_Load(&p_Images->Worker[1], (FPOINT) { 0.0f, 0.0f }, "../../Images/WO/WO1.bmp", "../../Images/WO/WO0_M.bmp"); // Carrying minerals.
	PICTURE_Load(&p_Images->Worker[2], (FPOINT) { 0.0f, 0.0f }, "../../Images/WO/WO2.bmp", "../../Images/WO/WO0_M.bmp"); // Carrying gas.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Command center images.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Receiving minerals.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading command center image(s)...");
	PICTURE_Load(&p_Images->Command[0], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC0.bmp", "../../Images/CC/CC0_M.bmp"); // Original.
	PICTURE_Load(&p_Images->Command[1], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC1.bmp", "../../Images/CC/CC0_M.bmp"); // Bright.
	PICTURE_Load(&p_Images->Command[2], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC2.bmp", "../../Images/CC/CC0_M.bmp"); // Brighter.
	PICTURE_Load(&p_Images->Command[3], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC3.bmp", "../../Images/CC/CC0_M.bmp"); // Brightest.
	PICTURE_Load(&p_Images->Command[4], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC4.bmp", "../../Images/CC/CC0_M.bmp"); // Brighter.
	PICTURE_Load(&p_Images->Command[5], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC5.bmp", "../../Images/CC/CC0_M.bmp"); // Bright.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Building a unit.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// East corner green.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PICTURE_Load(&p_Images->Command[6], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC6.bmp", "../../Images/CC/CC0_M.bmp"); // Bright.
	PICTURE_Load(&p_Images->Command[7], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC7.bmp", "../../Images/CC/CC0_M.bmp"); // Brighter.
	PICTURE_Load(&p_Images->Command[8], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC8.bmp", "../../Images/CC/CC0_M.bmp"); // Brightest.
	PICTURE_Load(&p_Images->Command[9], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC9.bmp", "../../Images/CC/CC0_M.bmp"); // Brighter.
	PICTURE_Load(&p_Images->Command[10], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC10.bmp", "../../Images/CC/CC0_M.bmp"); // Bright.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// South corner green.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PICTURE_Load(&p_Images->Command[11], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC11.bmp", "../../Images/CC/CC0_M.bmp"); // Bright.
	PICTURE_Load(&p_Images->Command[12], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC12.bmp", "../../Images/CC/CC0_M.bmp"); // Brighter.
	PICTURE_Load(&p_Images->Command[13], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC13.bmp", "../../Images/CC/CC0_M.bmp"); // Brightest.
	PICTURE_Load(&p_Images->Command[14], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC14.bmp", "../../Images/CC/CC0_M.bmp"); // Brighter.
	PICTURE_Load(&p_Images->Command[15], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC15.bmp", "../../Images/CC/CC0_M.bmp"); // Bright.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// West corner green.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PICTURE_Load(&p_Images->Command[16], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC16.bmp", "../../Images/CC/CC0_M.bmp"); // Bright.
	PICTURE_Load(&p_Images->Command[17], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC17.bmp", "../../Images/CC/CC0_M.bmp"); // Brighter.
	PICTURE_Load(&p_Images->Command[18], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC18.bmp", "../../Images/CC/CC0_M.bmp"); // Brightest.
	PICTURE_Load(&p_Images->Command[19], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC19.bmp", "../../Images/CC/CC0_M.bmp"); // Brighter.
	PICTURE_Load(&p_Images->Command[20], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC20.bmp", "../../Images/CC/CC0_M.bmp"); // Bright.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Center lights green.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PICTURE_Load(&p_Images->Command[21], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC21.bmp", "../../Images/CC/CC0_M.bmp"); // Bright.
	PICTURE_Load(&p_Images->Command[22], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC22.bmp", "../../Images/CC/CC0_M.bmp"); // Brighter.
	PICTURE_Load(&p_Images->Command[23], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC23.bmp", "../../Images/CC/CC0_M.bmp"); // Brightest.
	PICTURE_Load(&p_Images->Command[24], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC24.bmp", "../../Images/CC/CC0_M.bmp"); // Brighter.
	PICTURE_Load(&p_Images->Command[25], (FPOINT) { 0.0f, 0.0f }, "../../Images/CC/CC25.bmp", "../../Images/CC/CC0_M.bmp"); // Bright.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Mineral field images.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading mineral field image(s)...");
	PICTURE_Load(&p_Images->Mineral[0], (FPOINT) { 0.0f, 0.0f }, "../../Images/MF/MF0.bmp", "../../Images/MF/MF0_M.bmp"); // 100%
	PICTURE_Load(&p_Images->Mineral[1], (FPOINT) { 0.0f, 0.0f }, "../../Images/MF/MF1.bmp", "../../Images/MF/MF1_M.bmp"); // 75%
	PICTURE_Load(&p_Images->Mineral[2], (FPOINT) { 0.0f, 0.0f }, "../../Images/MF/MF2.bmp", "../../Images/MF/MF2_M.bmp"); // 50%
	PICTURE_Load(&p_Images->Mineral[3], (FPOINT) { 0.0f, 0.0f }, "../../Images/MF/MF3.bmp", "../../Images/MF/MF3_M.bmp"); // 25%
	PICTURE_Load(&p_Images->Mineral[4], (FPOINT) { 0.0f, 0.0f }, "../../Images/MF/MF4.bmp", "../../Images/MF/MF4_M.bmp"); // 0%
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Supply depot images.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading supply depot image(s)...");
	PICTURE_Load(&p_Images->Supply[0], (FPOINT) { 0.0f, 0.0f }, "../../Images/SU/SU0.bmp", "../../Images/SU/SU0_M.bmp"); // Original.
	PICTURE_Load(&p_Images->Supply[1], (FPOINT) { 0.0f, 0.0f }, "../../Images/SU/SU1.bmp", "../../Images/SU/SU0_M.bmp"); // Bright.
	PICTURE_Load(&p_Images->Supply[2], (FPOINT) { 0.0f, 0.0f }, "../../Images/SU/SU2.bmp", "../../Images/SU/SU0_M.bmp"); // Brighter.
	PICTURE_Load(&p_Images->Supply[3], (FPOINT) { 0.0f, 0.0f }, "../../Images/SU/SU3.bmp", "../../Images/SU/SU0_M.bmp"); // Brightest.
	PICTURE_Load(&p_Images->Supply[4], (FPOINT) { 0.0f, 0.0f }, "../../Images/SU/SU4.bmp", "../../Images/SU/SU0_M.bmp"); // Brighter.
	PICTURE_Load(&p_Images->Supply[5], (FPOINT) { 0.0f, 0.0f }, "../../Images/SU/SU5.bmp", "../../Images/SU/SU0_M.bmp"); // Bright.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Refinery images.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading refinery image(s)...");
	PICTURE_Load(&p_Images->Refinery[0], (FPOINT) { 0.0f, 0.0f }, "../../Images/RE/RE0.bmp", "../../Images/RE/RE0_M.bmp"); // Original.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// HUD images.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading HUD image(s)...");
	PICTURE_Load(&p_Images->HUD[0], (FPOINT) { 0.0f, 0.0f }, "../../Images/HU/HU0.bmp", "../../Images/HU/HU0_M.bmp"); // Resource background.
	PICTURE_Load(&p_Images->HUD[1], (FPOINT) { 0.0f, 0.0f }, "../../Images/HU/HU1.bmp", "../../Images/HU/HU1.bmp"); // Taskbar and minimap background.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Minimap images.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading minimap image(s)...");
	PICTURE_Load(&p_Images->Minimap[0], (FPOINT) { 0.0f, 0.0f }, "../../Images/MI/MI0.bmp", "../../Images/MI/MI0.bmp"); // Regular grass.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Gas images.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading gas image(s)...");
	PICTURE_Load(&p_Images->Gas[0], (FPOINT) { 0.0f, 0.0f }, "../../Images/GA/GA0.bmp", "../../Images/GA/GA0.bmp"); // Original.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Terrain images.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading terrain image(s)...");
	PICTURE_Load(&p_Images->Terrain[0], (FPOINT) { 0.0f, 0.0f }, "../../Images/TE/TE0.bmp", "../../Images/TE/TE0.bmp"); // Regular grass.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Command card images.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading command card image(s)...");
	PICTURE_Load(&p_Images->Card[0], (FPOINT) { 0.0f, 0.0f }, "../../Images/CA/CA0.bmp", "../../Images/CA/CA0.bmp"); // Blank card.
	PICTURE_Load(&p_Images->Card[1], (FPOINT) { 0.0f, 0.0f }, "../../Images/CA/CA1.bmp", "../../Images/CA/CA1.bmp"); // Stop and move card.
	PICTURE_Load(&p_Images->Card[2], (FPOINT) { 0.0f, 0.0f }, "../../Images/CA/CA2.bmp", "../../Images/CA/CA2.bmp"); // Specific structure.
	PICTURE_Load(&p_Images->Card[3], (FPOINT) { 0.0f, 0.0f }, "../../Images/CA/CA3.bmp", "../../Images/CA/CA3.bmp"); // Specific worker.
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl IMAGES_Kill(IMAGES* p_Images, GLOBALS* p_Globals) {
	UINT8 ubI;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (ubI = 0; ubI < 9; ubI++) {
		PICTURE_Kill(&p_Images->Blitter[ubI]);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (ubI = 0; ubI < 3; ubI++) {
		PICTURE_Kill(&p_Images->Worker[ubI]);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (ubI = 0; ubI < 26; ubI++) {
		PICTURE_Kill(&p_Images->Command[ubI]);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (ubI = 0; ubI < 5; ubI++) {
		PICTURE_Kill(&p_Images->Mineral[ubI]);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (ubI = 0; ubI < 6; ubI++) {
		PICTURE_Kill(&p_Images->Supply[ubI]);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (ubI = 0; ubI < 1; ubI++) {
		PICTURE_Kill(&p_Images->Refinery[ubI]);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (ubI = 0; ubI < 2; ubI++) {
		PICTURE_Kill(&p_Images->HUD[ubI]);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (ubI = 0; ubI < 1; ubI++) {
		PICTURE_Kill(&p_Images->Minimap[ubI]);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (ubI = 0; ubI < 1; ubI++) {
		PICTURE_Kill(&p_Images->Gas[ubI]);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (ubI = 0; ubI < 1; ubI++) {
		PICTURE_Kill(&p_Images->Terrain[ubI]);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (ubI = 0; ubI < 5; ubI++) {
		PICTURE_Kill(&p_Images->Card[ubI]);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	free(p_Images);
	p_Globals->iRunningHeap -= sizeof(IMAGES);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
