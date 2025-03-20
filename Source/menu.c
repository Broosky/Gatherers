/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Program Name: Gatherers (C)                                                                                            //
//  Author: Jeffrey Bednar                                                                                                 //
//  Copyright: Illusion Interactive, 2011                                                                                  //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MENU_C_
#define _MENU_C_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/main.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MENU_Zero(MENU* p_Menu) {
    ZeroMemory(p_Menu, sizeof(MENU));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MENU* __cdecl MENU_Create(GLOBALS* p_Globals) {
    MENU* p_Menu = (MENU*)malloc(sizeof(MENU));
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MENU_Zero(p_Menu);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    (*p_Globals).iRunningHeap += sizeof(MENU);
    return p_Menu;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MENU_Kill(MENU* p_Menu, GLOBALS* p_Globals) {
    free(p_Menu);
    (*p_Globals).iRunningHeap -= sizeof(MENU);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Menu settings that only need to be calculated once or I would like toggled on at run-time.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MENU_Init(MENU* p_Menu) {
    (*p_Menu).bBegin = 1;
    (*p_Menu).bToggleScatter = 1;
    (*p_Menu).bDiagnostics = 1;
    (*p_Menu).bEnableMasking = 1;
    (*p_Menu).bDrawResources = 1;
    (*p_Menu).bDrawIds = 1;
    (*p_Menu).bDrawStatuses = 1;
    (*p_Menu).bDrawMinor = 1;
    (*p_Menu).bDrawMajor = 1;
    (*p_Menu).bAllDiagnostics = 1;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CheckMenuItem((*p_Menu).hMenu, MENU_SIMULATION_TOGGLE_SIMULATION, MF_CHECKED);
    CheckMenuItem((*p_Menu).hMenu, MENU_SIMULATION_TOGGLE_SCATTER, MF_CHECKED);
    CheckMenuItem((*p_Menu).hMenu, MENU_OPTIONS_IDS, MF_CHECKED);
    CheckMenuItem((*p_Menu).hMenu, MENU_OPTIONS_DIAGNOSTICS, MF_CHECKED);
    CheckMenuItem((*p_Menu).hMenu, MENU_OPTIONS_MASKING, MF_CHECKED);
    CheckMenuItem((*p_Menu).hMenu, MENU_OPTIONS_RESOURCES, MF_CHECKED);
    CheckMenuItem((*p_Menu).hMenu, MENU_OPTIONS_STATUSES, MF_CHECKED);
    CheckMenuItem((*p_Menu).hMenu, MENU_OPTIONS_SHOW_MINOR, MF_CHECKED);
    CheckMenuItem((*p_Menu).hMenu, MENU_OPTIONS_SHOW_MAJOR, MF_CHECKED);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
