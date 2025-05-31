/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/constants.h"
#include "../Headers/globals.h"
#include "../Headers/log.h"
#include "../Headers/menu.h"
#include "../Headers/misc.h"
#include <stdlib.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MENU_Zero(MENU_T* p_Menu) {
    ZeroMemory(p_Menu, sizeof(MENU_T));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MENU_T* __cdecl MENU_Create(GLOBALS_T* p_Globals, LOG_T* p_Log) {
    size_t stAllocation = sizeof(MENU_T);
    MENU_T* p_Menu = malloc(stAllocation);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!p_Menu) {
        MISC_WriteOutParams(p_Log, LOG_SEVERITY_FATAL, "MENU_Create(): Malloc failed for size: %zu bytes\n", stAllocation);
        return NULL;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MENU_Zero(p_Menu);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->stAllocations += stAllocation;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return p_Menu;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MENU_Kill(MENU_T** pp_Menu, GLOBALS_T* p_Globals) {
    MENU_T* p_Menu = *pp_Menu;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_Menu) {
        free(p_Menu);
        p_Globals->stAllocations -= sizeof(MENU_T);
        *pp_Menu = NULL;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Menu settings that only need to be calculated once or toggled on at initial run-time.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MENU_Init(MENU_T* p_Menu) {
    HMENU hMenu = p_Menu->hMenu;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Menu->ubToggleSimulation = 1;
    p_Menu->ubToggleScatter = 1;
    p_Menu->ubDrawStatistics = 1;
    p_Menu->ubEnableMasking = 1;
    p_Menu->ubDrawResources = 1;
    p_Menu->ubDrawDirtyZones = 1;
    p_Menu->ubDrawIds = 1;
    p_Menu->ubDrawStatuses = 1;
    p_Menu->ubDrawMinorVectors = 1;
    p_Menu->ubDrawMajorVectors = 1;
    p_Menu->ubAllDiagnosticsToggle = 1;
    p_Menu->ubEnablePathFinding = 1;
    p_Menu->ubUseRaycast = 1;
    p_Menu->ubInterpolateCurves = 1;
    p_Menu->ubEnableBoxBlur = 1;
    p_Menu->ubEnableGreenMask = 1;
    p_Menu->ubIsDoubleBuffer = 1;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Align settings from above.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CheckMenuItem(hMenu, MENU_OPTIONS_IDS, MF_CHECKED);
    CheckMenuItem(hMenu, MENU_OPTIONS_MASKING, MF_CHECKED);
    CheckMenuItem(hMenu, MENU_OPTIONS_STATUSES, MF_CHECKED);
    CheckMenuItem(hMenu, MENU_OPTIONS_RESOURCES, MF_CHECKED);
    CheckMenuItem(hMenu, MENU_OPTIONS_STATISTICS, MF_CHECKED);
    CheckMenuItem(hMenu, MENU_OPTIONS_DIRTY_ZONES, MF_CHECKED);
    CheckMenuItem(hMenu, MENU_RENDERER_DOUBLE_BUFFER, MF_CHECKED);
    CheckMenuItem(hMenu, MENU_SIMULATION_TOGGLE_SCATTER, MF_CHECKED);
    CheckMenuItem(hMenu, MENU_OPTIONS_SHOW_MINOR_VECTORS, MF_CHECKED);
    CheckMenuItem(hMenu, MENU_OPTIONS_SHOW_MAJOR_VECTORS, MF_CHECKED);
    CheckMenuItem(hMenu, MENU_OPTIONS_PATH_FINDING_ENABLE, MF_CHECKED);
    CheckMenuItem(hMenu, MENU_OPTIONS_PATH_FINDING_RAYCAST, MF_CHECKED);
    CheckMenuItem(hMenu, MENU_SIMULATION_TOGGLE_SIMULATION, MF_CHECKED);
    CheckMenuItem(hMenu, MENU_RENDERER_POST_PROCESSING_BOX_BLUR, MF_CHECKED);
    CheckMenuItem(hMenu, MENU_RENDERER_POST_PROCESSING_GREEN_MASK, MF_CHECKED);
    CheckMenuItem(hMenu, MENU_OPTIONS_PATH_FINDING_INTERPOLATE_CURVES, MF_CHECKED);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Initially disabled.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    EnableMenuItem(hMenu, MENU_RENDERER_POST_PROCESSING_DARKEN, MFS_DISABLED);
    EnableMenuItem(hMenu, MENU_RENDERER_POST_PROCESSING_BOX_BLUR, MFS_DISABLED);
    EnableMenuItem(hMenu, MENU_RENDERER_POST_PROCESSING_GREEN_MASK, MFS_DISABLED);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Always disabled.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    EnableMenuItem(hMenu, MENU_RENDERER_SDL, MFS_DISABLED);
    EnableMenuItem(hMenu, MENU_RENDERER_DIRECT2D, MFS_DISABLED);
    EnableMenuItem(hMenu, MENU_RENDERER_WIREFRAME, MFS_DISABLED);
    EnableMenuItem(hMenu, MENU_RENDERER_SINGLE_BUFFER, MFS_DISABLED);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
