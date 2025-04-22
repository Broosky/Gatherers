/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/common.h"
#include "../Headers/constants.h"
#include "../Headers/globals.h"
#include "../Headers/menu.h"
#include <stdlib.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MENU_Zero(MENU_T* p_Menu) {
    ZeroMemory(p_Menu, sizeof(MENU_T));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MENU_T* __cdecl MENU_Create(GLOBALS_T* p_Globals) {
    MENU_T* p_Menu = (MENU_T*)malloc(sizeof(MENU_T));
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MENU_Zero(p_Menu);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->iRunningHeap += sizeof(MENU_T);
    return p_Menu;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MENU_Kill(MENU_T* p_Menu, GLOBALS_T* p_Globals) {
    free(p_Menu);
    p_Globals->iRunningHeap -= sizeof(MENU_T);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Menu settings that only need to be calculated once or I would like toggled on at run-time.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MENU_Init(MENU_T* p_Menu) {
    p_Menu->ubToggleSimulation = 1;
    p_Menu->ubToggleScatter = 1;
    p_Menu->ubDrawDiagnostics = 1;
    p_Menu->ubEnableMasking = 1;
    p_Menu->ubDrawResources = 1;
    p_Menu->ubDrawIds = 1;
    p_Menu->ubDrawStatuses = 1;
    p_Menu->ubDrawMinor = 1;
    p_Menu->ubDrawMajor = 1;
    p_Menu->ubAllDiagnosticsToggle = 1;
    p_Menu->ubUseRaycast = 1;
    p_Menu->ubEnableTranslations = 1;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_IDS, MF_CHECKED);
    CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_STATS, MF_CHECKED);
    CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_MASKING, MF_CHECKED);
    CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_RESOURCES, MF_CHECKED);
    CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_STATUSES, MF_CHECKED);
    CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_SHOW_MINOR, MF_CHECKED);
    CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_SHOW_MAJOR, MF_CHECKED);
    CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_PATH_RAYCAST, MF_CHECKED);
    CheckMenuItem(p_Menu->hMenu, MENU_SIMULATION_TOGGLE_SCATTER, MF_CHECKED);
    CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_ENABLE_TRANSLATIONS, MF_CHECKED);
    CheckMenuItem(p_Menu->hMenu, MENU_SIMULATION_TOGGLE_SIMULATION, MF_CHECKED);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
