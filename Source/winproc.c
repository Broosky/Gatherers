/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _WINPROC_C_
#define _WINPROC_C_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/constants.h"
#include "../Headers/functions.h"
#include "../Headers/types.h"
#include <math.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Future:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// - Put the menu handle into a stack variable because we use it many times.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WINPROC_WindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
    switch (uiMsg) {
    case WM_COMMAND: {
        WINPROC_HandleMenuEvent(hWnd, wParam, p_Menu, p_Globals);
        break;
    }
    case WM_SIZE: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_DblBuf != NULL) {
            DBLBUF_Resize(p_DblBuf, hWnd, RGB(0, 0, 0), p_Globals);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Remember to reset the graphics mode to allow the rendering engine to work.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            RENDER_Init(p_DblBuf);
        }
        break;
    }
    case WM_LBUTTONUP: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Reset for subsequent left mouse clicks.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (!p_Globals->ubClickOriginFromMinimap) {
            WINPROC_SelectEntities(p_Globals);
        }
        p_Globals->ubClickOriginFromMinimap = 0;
        break;
    }
    case WM_LBUTTONDOWN: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (PROC_IsMinimapClicked(p_DblBuf, p_Images, p_Globals) && p_Menu->ubEnableTranslations) {
            p_Globals->ubClickOriginFromMinimap = 1;
            PROC_AdjustMinimapViewport(p_Globals, p_Images);
        }
        else {
            p_Globals->ubClickOriginFromMinimap = 0;
            WINPROC_CreateOrCaptureEntities(p_Globals, p_Images);
        }
        break;
    }
    case WM_RBUTTONDOWN: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubToggleScatter) {
            SELECTED_COUNT SelectedCount = ENTITY_GetSelectedEntityCounts(p_Globals);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (SelectedCount.usSelectedWorkersCount > 1) {
                WINPROC_DistributeAndSendWorkers(p_Globals, p_Images);
            }
            else {
                WINPROC_SendWorkers(p_Globals, p_Images);
            }
        }
        else {
            WINPROC_SendWorkers(p_Globals, p_Images);
        }
        break;
    }
    case WM_MOUSEMOVE: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->iMouseX = LOWORD(lParam);
        p_Globals->iMouseY = HIWORD(lParam);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // If we've clicked in the minimap area, we will translate the view to match the adjustments to the viewport. So, we 
        // don't have to do anything with the selection area.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (!p_Globals->ubClickOriginFromMinimap) {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // For mouse drag selection.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (wParam == MK_LBUTTON) {
                p_Globals->ubDrawSelectionRect = 1;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Consider translations when calculating selection area during scrolling.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Globals->ubDrawSelectionRect && !p_Globals->ubCreate) {
                p_Globals->iCaptureCurrentX = p_Globals->iMouseX + -p_Globals->fLateralTranslation;
                p_Globals->iCaptureCurrentY = p_Globals->iMouseY + -p_Globals->fVerticalTranslation;
            }
        }
        else {
            PROC_AdjustMinimapViewport(p_Globals, p_Images);
        }
        break;
    }
    case WM_KEYDOWN: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        WINPROC_HandleKeyDown(hWnd, wParam, p_Globals, p_Menu, p_DblBuf);
        break;
    }
    case WM_CLOSE: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        PostQuitMessage(0);
        break;
    }
    default: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        return DefWindowProc(hWnd, uiMsg, wParam, lParam);
    }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // I handled a message.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINPROC_BuildHelper(USHORT usType, MENU* p_Menu) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Uncheck all of the menu items
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CheckMenuItem(p_Menu->hMenu, MENU_BUILD_NONE, MF_UNCHECKED);
    CheckMenuItem(p_Menu->hMenu, MENU_BUILD_WORKER, MF_UNCHECKED);
    CheckMenuItem(p_Menu->hMenu, MENU_BUILD_COMMAND, MF_UNCHECKED);
    CheckMenuItem(p_Menu->hMenu, MENU_BUILD_MINERAL, MF_UNCHECKED);
    CheckMenuItem(p_Menu->hMenu, MENU_BUILD_SUPPLY, MF_UNCHECKED);
    CheckMenuItem(p_Menu->hMenu, MENU_BUILD_REFINERY, MF_UNCHECKED);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Check the one we want.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    switch (usType) {
    case ENTITY_WORKER: {
        CheckMenuItem(p_Menu->hMenu, MENU_BUILD_WORKER, MF_CHECKED);
        break;
    }
    case ENTITY_COMMAND: {
        CheckMenuItem(p_Menu->hMenu, MENU_BUILD_COMMAND, MF_CHECKED);
        break;
    }
    case ENTITY_MINERAL: {
        CheckMenuItem(p_Menu->hMenu, MENU_BUILD_MINERAL, MF_CHECKED);
        break;
    }
    case ENTITY_SUPPLY: {
        CheckMenuItem(p_Menu->hMenu, MENU_BUILD_SUPPLY, MF_CHECKED);
        break;
    }
    case ENTITY_REFINERY: {
        CheckMenuItem(p_Menu->hMenu, MENU_BUILD_REFINERY, MF_CHECKED);
        break;
    }
    default: {
        CheckMenuItem(p_Menu->hMenu, MENU_BUILD_NONE, MF_CHECKED);
    }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINPROC_SendWorkers(GLOBALS* p_Globals, IMAGES* p_Images) {
    USHORT usMoveableCount = 0;
    ENTITY* p_Resource = NULL;
    ENTITY* p_Current = p_Globals->p_RootEntity;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Loop through all of the entities and check to see if we right-clicked on a resource.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (p_Current && !p_Resource) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Hijack this loop to keep track of how many moveable entities are selected.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Current->ubIsMovable) {
            if (p_Current->ubIsSelected) {
                usMoveableCount++;
            }
        }
        switch (p_Current->usType) {
        case ENTITY_MINERAL: {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // True, if I right clicked on a mineral field.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (ENTITY_WithinPoint(p_Current, (FPOINT) { p_Globals->iMouseX, p_Globals->iMouseY })) {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // We want to make all the selected entities harvest THAT particular mineral field.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                p_Resource = p_Current;
            }
            break;
        }
        case ENTITY_REFINERY: {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // True, if I right clicked on a refinery.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (ENTITY_WithinPoint(p_Current, (FPOINT) { p_Globals->iMouseX, p_Globals->iMouseY })) {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // We want to make all the selected entities harvest THAT particular refinery.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                p_Resource = p_Current;
            }
        }
        }
        p_Current = (ENTITY*)p_Current->p_Next;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    USHORT usI;
    USHORT usAllocation;
    FPOINT* p_DestinationPoints = NULL;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // We did not click on a resource, therefore we have to move the selected entities.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!p_Resource) {
        usAllocation = sizeof(FPOINT) * usMoveableCount;
        p_DestinationPoints = (FPOINT*)malloc(usAllocation);
        p_Globals->iRunningHeap += usAllocation;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Future:
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // - Figure out the average size of the entities to be moved.
        // - Do not create destination points if we are harvesting a resource. There is no need.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        USHORT usColumn = 1;
        USHORT usColumnLimit = sqrt(usMoveableCount);
        FPOINT Placement = { p_Globals->iMouseX, p_Globals->iMouseY };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (usI = 0; usI < usMoveableCount; usI++) {
            p_DestinationPoints[usI] = Placement;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (usColumn >= usColumnLimit) {
                Placement.fY += p_Images->Worker[0].Bitmap.bmHeight;
                Placement.fX = p_Globals->iMouseX;
                usColumn = 1;
            }
            else {
                Placement.fX += p_Images->Worker[0].Bitmap.bmWidth;
                usColumn++;
            }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        usI = 0;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Loop through all of the entities and move them if they are selected.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Current = p_Globals->p_RootEntity;
    while (p_Current) {
        if (p_Current->ubIsSelected) {
            p_Current->ubIsSelected = 0;
            if (p_Current->ubIsMovable) {
                if (p_Resource) {
                    p_Current->p_Operating = (struct ENTITY*)p_Resource;
                }
                else {
                    p_Current->p_Operating = NULL;
                    ENTITY_MoveToPoint(p_Current, p_DestinationPoints[usI++], p_Globals);
                }
            }
        }
        p_Current = (ENTITY*)p_Current->p_Next;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_DestinationPoints) {
        free(p_DestinationPoints);
        p_Globals->iRunningHeap -= usAllocation;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINPROC_DistributeAndSendWorkers(GLOBALS* p_Globals, IMAGES* p_Images) {
    USHORT usMoveableCount = 0;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ENTITY* p_Resource = NULL;
    ENTITY* p_Current = p_Globals->p_RootEntity;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Loop through all of the entities and check to see if we right-clicked on a resource.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (p_Current && !p_Resource) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Hijack this loop to keep track of how many moveable entities are selected.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Current->ubIsMovable) {
            if (p_Current->ubIsSelected) {
                usMoveableCount++;
            }
        }
        switch (p_Current->usType) {
        case ENTITY_MINERAL: {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // True, if I right clicked on a mineral field.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (ENTITY_WithinPoint(p_Current, (FPOINT) { p_Globals->iMouseX, p_Globals->iMouseY })) {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // We want to make all the selected entities harvest THAT particular mineral field.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                p_Resource = p_Current;
            }
            break;
        }
        case ENTITY_REFINERY: {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // True, if I right clicked on a refinery.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (ENTITY_WithinPoint(p_Current, (FPOINT) { p_Globals->iMouseX, p_Globals->iMouseY })) {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // We want to make all the selected entities harvest THAT particular refinery.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                p_Resource = p_Current;
            }
        }
        }
        p_Current = (ENTITY*)p_Current->p_Next;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // If we clicked on a resource, get a collection of pointers to available resources of that type, considering their distance.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    USHORT usFound = 0;
    int iRollingAllocationHeap = 0;
    AI_CLOSEST* p_ClosestEntities = NULL;
    if (p_Resource) {
        p_ClosestEntities = AI_FindClosestByDistance(p_Resource, p_Resource->usType, &iRollingAllocationHeap, &usFound, p_Globals);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Data for moving entities to a specific destination (not gathering resources).
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    USHORT usI = 0;
    USHORT usDestinationPointsAllocation = 0;
    FPOINT* p_DestinationPoints = NULL;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // We did not click on a resource, therefore we have to move the selected entities.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!p_Resource) {
        usDestinationPointsAllocation = sizeof(FPOINT) * usMoveableCount;
        p_DestinationPoints = (FPOINT*)malloc(usDestinationPointsAllocation);
        p_Globals->iRunningHeap += usDestinationPointsAllocation;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Future:
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // - Figure out the average size of the entities to be moved.
        // - Do not create destination points if we are harvesting a resource. There is no need.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        USHORT usColumn = 1;
        USHORT usColumnLimit = sqrt(usMoveableCount);
        FPOINT Placement = { p_Globals->iMouseX, p_Globals->iMouseY };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (usI = 0; usI < usMoveableCount; usI++) {
            p_DestinationPoints[usI] = Placement;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (usColumn >= usColumnLimit) {
                Placement.fY += p_Images->Worker[0].Bitmap.bmHeight + 20.0f;
                Placement.fX = p_Globals->iMouseX;
                usColumn = 1;
            }
            else {
                Placement.fX += p_Images->Worker[0].Bitmap.bmWidth + 20.0f;
                usColumn++;
            }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        usI = 0;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Loop through all of the entities and either move them to the organized destination points, or the resources to gather.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    USHORT usDestinationResourceIndex = 0;
    p_Current = p_Globals->p_RootEntity;
    while (p_Current) {
        if (p_Current->ubIsSelected) {
            p_Current->ubIsSelected = 0;
            if (p_Current->ubIsMovable) {
                if (p_ClosestEntities) {
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // Run through the resources ordered by their closest distance from the inquirer.
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////
                    p_Current->p_Operating = (struct ENTITY*)p_ClosestEntities[usDestinationResourceIndex++].p_Entity;
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // There will most likely be cases where there are more workers than available resources. Let's reset the
                    // index to recycle them.
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////
                    if (usDestinationResourceIndex >= usFound) {
                        usDestinationResourceIndex = 0;
                    }
                }
                else {
                    p_Current->p_Operating = NULL;
                    ENTITY_MoveToPoint(p_Current, p_DestinationPoints[usI++], p_Globals);
                }
            }
        }
        p_Current = (ENTITY*)p_Current->p_Next;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_DestinationPoints) {
        free(p_DestinationPoints);
        p_Globals->iRunningHeap -= usDestinationPointsAllocation;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_ClosestEntities) {
        free(p_ClosestEntities);
        p_Globals->iRunningHeap -= iRollingAllocationHeap;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINPROC_CreateOrCaptureEntities(GLOBALS* p_Globals, IMAGES* p_Images) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // For mouse drag selection.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->iOldMouseX = p_Globals->iMouseX;
    p_Globals->iOldMouseY = p_Globals->iMouseY;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // If build mode is selected, create an entity at the mouse position.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_Globals->ubCreate) {
        ENTITY_Create(
            (FPOINT) {
            p_Globals->iMouseX, p_Globals->iMouseY
        },
            p_Globals->usBuildType,
            p_Images,
            p_Globals
        );
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Build mode is not selected, therefore allow the left mouse click to select entities.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else {
        UINT8 ubIsSomethingThere = 0;
        ENTITY* p_Current = p_Globals->p_RootEntity;
        while (p_Current) {
            if (ENTITY_WithinPoint(p_Current, (FPOINT) { p_Globals->iMouseX, p_Globals->iMouseY })) {
                p_Current->ubIsSelected = !p_Current->ubIsSelected;
                ubIsSomethingThere = 1;
            }
            p_Current = (ENTITY*)p_Current->p_Next;
        }
        if (!ubIsSomethingThere) {
            p_Current = p_Globals->p_RootEntity;
            while (p_Current) {
                p_Current->ubIsSelected = 0;
                p_Current = (ENTITY*)p_Current->p_Next;
            }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Consider translations when calculating selection area during scrolling.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->iCaptureStartX = p_Globals->iOldMouseX + -p_Globals->fLateralTranslation;
        p_Globals->iCaptureStartY = p_Globals->iOldMouseY + -p_Globals->fVerticalTranslation;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINPROC_SelectEntities(GLOBALS* p_Globals) {
    if (!p_Globals->ubCreate) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Allow the operation to work when the mouse is dragged from all directions.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int iSavedX = p_Globals->iMouseX;
        int iSavedY = p_Globals->iMouseY;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Globals->iMouseX < p_Globals->iOldMouseX) {
            int iHolder = p_Globals->iMouseX;
            p_Globals->iMouseX = p_Globals->iOldMouseX;
            p_Globals->iOldMouseX = iHolder;
        }
        if (p_Globals->iMouseY < p_Globals->iOldMouseY) {
            int iHolder = p_Globals->iMouseY;
            p_Globals->iMouseY = p_Globals->iOldMouseY;
            p_Globals->iOldMouseY = iHolder;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY* p_Current = p_Globals->p_RootEntity;
        while (p_Current) {
            if (
                p_Current->CenterPoint.fX >= p_Globals->iOldMouseX &&
                p_Current->CenterPoint.fX <= p_Globals->iMouseX &&
                p_Current->CenterPoint.fY >= p_Globals->iOldMouseY &&
                p_Current->CenterPoint.fY <= p_Globals->iMouseY
                ) {
                p_Current->ubIsSelected = !p_Current->ubIsSelected;
            }
            p_Current = (ENTITY*)p_Current->p_Next;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Incase the points were swapped, reset the original values.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->iMouseX = iSavedX;
        p_Globals->iMouseY = iSavedY;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->ubDrawSelectionRect = 0;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!p_Globals->ubDrawSelectionRect) {
        p_Globals->iCaptureStartX = 0;
        p_Globals->iCaptureStartY = 0;
        p_Globals->iCaptureCurrentX = 0;
        p_Globals->iCaptureCurrentY = 0;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINPROC_HandleKeyDown(HWND hWnd, WPARAM wParam, GLOBALS* p_Globals, MENU* p_Menu, DBLBUF* p_DblBuf) {
    const float fLateralStep = TRANSLATION_STEP_AMOUNT;
    const float fVerticalStep = TRANSLATION_STEP_AMOUNT;
    float fAdjusted;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Arrow keys.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_Menu->ubEnableTranslations) {
        LONG lBmWidth = p_Images->Terrain[p_Globals->usMapIndex].Bitmap.bmWidth;
        LONG lBmHeight = p_Images->Terrain[p_Globals->usMapIndex].Bitmap.bmHeight;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        switch (wParam) {
        case VK_LEFT: {
            if (p_Globals->fLateralTranslation <= -fLateralStep) {
                fAdjusted = fLateralStep;
            }
            else {
                fAdjusted = -p_Globals->fLateralTranslation;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Globals->fLateralTranslation += fAdjusted;
            PROC_ApplyTranslations(fAdjusted, 0.0f, p_Globals, p_Images);
            break;
        }
        case VK_UP: {
            if (p_Globals->fVerticalTranslation <= -fVerticalStep) {
                fAdjusted = fVerticalStep;
            }
            else {
                fAdjusted = -p_Globals->fVerticalTranslation;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Globals->fVerticalTranslation += fAdjusted;
            PROC_ApplyTranslations(0.0f, fAdjusted, p_Globals, p_Images);
            break;
        }
        case VK_RIGHT: {
            if (p_Globals->fLateralTranslation >= -(lBmWidth - p_DblBuf->ClientArea.right - fLateralStep)) {
                fAdjusted = -fLateralStep;
            }
            else {
                fAdjusted = -(p_Globals->fLateralTranslation + (lBmWidth - p_DblBuf->ClientArea.right));
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Globals->fLateralTranslation += fAdjusted;
            PROC_ApplyTranslations(fAdjusted, 0.0f, p_Globals, p_Images);
            break;
        }
        case VK_DOWN: {
            if (p_Globals->fVerticalTranslation >= -(lBmHeight - p_DblBuf->ClientArea.bottom - fVerticalStep)) {
                fAdjusted = -fVerticalStep;
            }
            else {
                fAdjusted = -(p_Globals->fVerticalTranslation + (lBmHeight - p_DblBuf->ClientArea.bottom));
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Globals->fVerticalTranslation += fAdjusted;
            PROC_ApplyTranslations(0.0f, fAdjusted, p_Globals, p_Images);
            break;
        }
        default:
            break;
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Other keys.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    switch (wParam) {
    case VK_ESCAPE: {
        PostMessage(hWnd, WM_CLOSE, 0, 0);
        break;
    }
    case VK_DELETE: {
        ENTITY_DeleteSelected(p_Globals);
        break;
    }
    default:
        break;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINPROC_HandleMenuEvent(HWND hWnd, WPARAM wParam, MENU* p_Menu, GLOBALS* p_Globals) {
    switch (LOWORD(wParam)) {
    case MENU_SIMULATION_FREE: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->iMineralCount += 4000;
        p_Globals->iGasCount += 4000;
        break;
    }
    case MENU_SIMULATION_EXIT: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        PostMessage(hWnd, WM_CLOSE, 0, 0);
        break;
    }
    case MENU_SIMULATION_PRINT: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_PrintList(p_Globals);
        break;
    }
    case MENU_SIMULATION_TOGGLE_SCATTER: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubToggleScatter) {
            CheckMenuItem(p_Menu->hMenu, MENU_SIMULATION_TOGGLE_SCATTER, MF_UNCHECKED);
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_SIMULATION_TOGGLE_SCATTER, MF_CHECKED);
        }
        p_Menu->ubToggleScatter = !p_Menu->ubToggleScatter;
        break;
    }
    case MENU_SIMULATION_TOGGLE_SIMULATION: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubToggleSimulation) {
            CheckMenuItem(p_Menu->hMenu, MENU_SIMULATION_TOGGLE_SIMULATION, MF_UNCHECKED);
            p_Menu->ubToggleSimulation = !p_Menu->ubToggleSimulation;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Stop each entity that was moving.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            ENTITY* p_Current = p_Globals->p_RootEntity;
            while (p_Current) {
                p_Current->ubIsInMotion = 0;
                p_Current = (ENTITY*)p_Current->p_Next;
            }
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_SIMULATION_TOGGLE_SIMULATION, MF_CHECKED);
            p_Menu->ubToggleSimulation = !p_Menu->ubToggleSimulation;
        }
        break;
    }
    case MENU_DELETE_SELECTED: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_DeleteSelected(p_Globals);
        break;
    }
    case MENU_DELETE_ALL: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_DeleteAll(p_Globals);
        break;
    }
    case MENU_DELETE_WORKER: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_DeleteEntityType(ENTITY_WORKER, p_Globals);
        break;
    }
    case MENU_DELETE_COMMAND: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_DeleteEntityType(ENTITY_COMMAND, p_Globals);
        break;
    }
    case MENU_DELETE_MINERAL: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_DeleteEntityType(ENTITY_MINERAL, p_Globals);
        break;
    }
    case MENU_DELETE_SUPPLY: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_DeleteEntityType(ENTITY_SUPPLY, p_Globals);
        break;
    }
    case MENU_DELETE_REFINERY: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_DeleteEntityType(ENTITY_REFINERY, p_Globals);
        break;
    }
    case MENU_BUILD_NONE: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        WINPROC_BuildHelper(ENTITY_NONE, p_Menu);
        p_Globals->usBuildType = ENTITY_WORKER;
        p_Globals->ubCreate = 0;
        break;
    }
    case MENU_BUILD_WORKER: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        WINPROC_BuildHelper(ENTITY_WORKER, p_Menu);
        p_Globals->usBuildType = ENTITY_WORKER;
        p_Globals->ubCreate = 1;
        break;
    }
    case MENU_BUILD_COMMAND: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        WINPROC_BuildHelper(ENTITY_COMMAND, p_Menu);
        p_Globals->usBuildType = ENTITY_COMMAND;
        p_Globals->ubCreate = 1;
        break;
    }
    case MENU_BUILD_MINERAL: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        WINPROC_BuildHelper(ENTITY_MINERAL, p_Menu);
        p_Globals->usBuildType = ENTITY_MINERAL;
        p_Globals->ubCreate = 1;
        break;
    }
    case MENU_BUILD_SUPPLY: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        WINPROC_BuildHelper(ENTITY_SUPPLY, p_Menu);
        p_Globals->usBuildType = ENTITY_SUPPLY;
        p_Globals->ubCreate = 1;
        break;
    }
    case MENU_BUILD_REFINERY: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        WINPROC_BuildHelper(ENTITY_REFINERY, p_Menu);
        p_Globals->usBuildType = ENTITY_REFINERY;
        p_Globals->ubCreate = 1;
        break;
    }
    case MENU_OPTIONS_ALL_DIAGNOSTICS: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubAllDiagnosticsToggle) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_IDS, MF_UNCHECKED);
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_STATS, MF_UNCHECKED);
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_RESOURCES, MF_UNCHECKED);
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_STATUSES, MF_UNCHECKED);
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_SHOW_MINOR, MF_UNCHECKED);
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_SHOW_MAJOR, MF_UNCHECKED);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Menu->ubDrawIds = 0;
            p_Menu->ubDrawDiagnostics = 0;
            p_Menu->ubDrawResources = 0;
            p_Menu->ubDrawStatuses = 0;
            p_Menu->ubDrawMinor = 0;
            p_Menu->ubDrawMajor = 0;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Menu->ubAllDiagnosticsToggle = 0;
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_IDS, MF_CHECKED);
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_STATS, MF_CHECKED);
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_RESOURCES, MF_CHECKED);
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_STATUSES, MF_CHECKED);
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_SHOW_MINOR, MF_CHECKED);
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_SHOW_MAJOR, MF_CHECKED);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Menu->ubDrawIds = 1;
            p_Menu->ubDrawDiagnostics = 1;
            p_Menu->ubDrawResources = 1;
            p_Menu->ubDrawStatuses = 1;
            p_Menu->ubDrawMinor = 1;
            p_Menu->ubDrawMajor = 1;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Menu->ubAllDiagnosticsToggle = 1;
        }
        break;
    }
    case MENU_OPTIONS_STATS: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubDrawDiagnostics) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_STATS, MF_UNCHECKED);
            p_Menu->ubDrawDiagnostics = !p_Menu->ubDrawDiagnostics;
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_STATS, MF_CHECKED);
            p_Menu->ubDrawDiagnostics = !p_Menu->ubDrawDiagnostics;
        }
        break;
    }
    case MENU_OPTIONS_MASKING: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubEnableMasking) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_MASKING, MF_UNCHECKED);
            p_Menu->ubEnableMasking = !p_Menu->ubEnableMasking;
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_MASKING, MF_CHECKED);
            p_Menu->ubEnableMasking = !p_Menu->ubEnableMasking;
        }
        break;
    }
    case MENU_OPTIONS_RESOURCES: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubDrawResources) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_RESOURCES, MF_UNCHECKED);
            p_Menu->ubDrawResources = !p_Menu->ubDrawResources;
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_RESOURCES, MF_CHECKED);
            p_Menu->ubDrawResources = !p_Menu->ubDrawResources;
        }
        break;
    }
    case MENU_OPTIONS_IDS: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubDrawIds) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_IDS, MF_UNCHECKED);
            p_Menu->ubDrawIds = !p_Menu->ubDrawIds;
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_IDS, MF_CHECKED);
            p_Menu->ubDrawIds = !p_Menu->ubDrawIds;
        }
        break;
    }
    case MENU_OPTIONS_ENABLE_TRANSLATIONS: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubEnableTranslations) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_ENABLE_TRANSLATIONS, MF_UNCHECKED);
            p_Menu->ubEnableTranslations = !p_Menu->ubEnableTranslations;
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_ENABLE_TRANSLATIONS, MF_CHECKED);
            p_Menu->ubEnableTranslations = !p_Menu->ubEnableTranslations;
        }
        break;
    }
    case MENU_OPTIONS_TOPMOST: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubIsTopmostWindow) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_TOPMOST, MF_UNCHECKED);
            p_Menu->ubIsTopmostWindow = !p_Menu->ubIsTopmostWindow;
            SetWindowPos(hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
            //LONG_PTR dwExStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
            //dwExStyle &= ~WS_EX_TOPMOST;
            //SetWindowLongPtr(hWnd, GWL_EXSTYLE, dwExStyle);
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_TOPMOST, MF_CHECKED);
            p_Menu->ubIsTopmostWindow = !p_Menu->ubIsTopmostWindow;
            SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
            //LONG_PTR dwExStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
            //dwExStyle = WS_EX_TOPMOST;
            //SetWindowLongPtr(hWnd, GWL_EXSTYLE, dwExStyle);
        }
        break;
    }
    case MENU_OPTIONS_FULLSCREEN: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubIsFullScreen) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_FULLSCREEN, MF_UNCHECKED);
            p_Menu->ubIsFullScreen = !p_Menu->ubIsFullScreen;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Change double buffer settings.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
            SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            MISC_ResizeWindow(hWnd, INITIAL_CLIENT_WIDTH, INITIAL_CLIENT_HEIGHT);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            DBLBUF_Resize(p_DblBuf, hWnd, RGB(0, 0, 0), p_Globals);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Remember to reset the graphics mode to allow the rendering engine to work.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            RENDER_Init(p_DblBuf);
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_FULLSCREEN, MF_CHECKED);
            p_Menu->ubIsFullScreen = !p_Menu->ubIsFullScreen;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Change double buffer settings.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Last argument is 1 because the menu needs to be redrawn when the window area resizes.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
            SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            MoveWindow(hWnd, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 1);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            DBLBUF_Resize(p_DblBuf, hWnd, RGB(0, 0, 0), p_Globals);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Remember to reset the graphics mode to allow the rendering engine to work.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            RENDER_Init(p_DblBuf);
        }
        break;
    }
    case MENU_OPTIONS_STATUSES: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubDrawStatuses) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_STATUSES, MF_UNCHECKED);
            p_Menu->ubDrawStatuses = !p_Menu->ubDrawStatuses;
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_STATUSES, MF_CHECKED);
            p_Menu->ubDrawStatuses = !p_Menu->ubDrawStatuses;
        }
        break;
    }
    case MENU_OPTIONS_SHOW_MINOR: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubDrawMinor) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_SHOW_MINOR, MF_UNCHECKED);
            p_Menu->ubDrawMinor = !p_Menu->ubDrawMinor;
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_SHOW_MINOR, MF_CHECKED);
            p_Menu->ubDrawMinor = !p_Menu->ubDrawMinor;
        }
        break;
    }
    case MENU_OPTIONS_SHOW_MAJOR: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubDrawMajor) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_SHOW_MAJOR, MF_UNCHECKED);
            p_Menu->ubDrawMajor = !p_Menu->ubDrawMajor;
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_SHOW_MAJOR, MF_CHECKED);
            p_Menu->ubDrawMajor = !p_Menu->ubDrawMajor;
        }
        break;
    }
    case MENU_OPTIONS_PATH_RAYCAST: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (!p_Menu->ubUseRaycast) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_PATH_RAYCAST, MF_CHECKED);
            p_Menu->ubUseRaycast = 1;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_PATH_BISECTION, MF_UNCHECKED);
            p_Menu->ubUseBisection = 0;
        }
        break;
    }
    case MENU_OPTIONS_PATH_BISECTION: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (!p_Menu->ubUseBisection) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_PATH_BISECTION, MF_CHECKED);
            p_Menu->ubUseBisection = 1;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_PATH_RAYCAST, MF_UNCHECKED);
            p_Menu->ubUseRaycast = 0;
        }
        break;
    }
    case MENU_CLEAR_SELECTED: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY* p_Current = p_Globals->p_RootEntity;
        while (p_Current) {
            p_Current->ubIsSelected = 0;
            p_Current = (ENTITY*)p_Current->p_Next;
        }
        break;
    }
    case MENU_CLEAR_MINERAL_COUNT: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->iMineralCount = 0;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Clear minerals collected by the command centers.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY* p_Current = p_Globals->p_RootEntity;
        while (p_Current) {
            switch (p_Current->usType) {
            case ENTITY_COMMAND: {
                p_Current->iMineralCount = 0;
                break;
            }
            }
            p_Current = (ENTITY*)p_Current->p_Next;
        }
        break;
    }
    case MENU_CLEAR_GAS_COUNT: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->iGasCount = 0;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Clear gas collected by the command centers.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY* p_Current = p_Globals->p_RootEntity;
        while (p_Current) {
            switch (p_Current->usType) {
            case ENTITY_COMMAND: {
                p_Current->iGasCount = 0;
                break;
            }
            }
            p_Current = (ENTITY*)p_Current->p_Next;
        }
        break;
    }
    case MENU_CLEAR_ALL_COUNT: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->iMineralCount = 0;
        p_Globals->iGasCount = 0;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Clear resources collected by the command centers.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY* p_Current = p_Globals->p_RootEntity;
        while (p_Current) {
            switch (p_Current->usType) {
            case ENTITY_COMMAND: {
                p_Current->iMineralCount = 0;
                p_Current->iGasCount = 0;
                break;
            }
            }
            p_Current = (ENTITY*)p_Current->p_Next;
        }
        break;
    }
    case MENU_SORT_WORKERS: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_SortToFront(WORKERS_TO_FRONT, p_Globals);
        break;
    }
    case MENU_SORT_MINERALS: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_SortToFront(MINERALS_TO_FRONT, p_Globals);
        break;
    }
    case MENU_SORT_COMMANDS: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_SortToFront(COMMANDS_TO_FRONT, p_Globals);
        break;
    }
    case MENU_SORT_SUPPLY: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_SortToFront(SUPPLIES_TO_FRONT, p_Globals);
        break;
    }
    case MENU_SORT_REFINERIES: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_SortToFront(REFINERIES_TO_FRONT, p_Globals);
        break;
    }
    case MENU_SORT_OBSTACLES: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_SortToFront(OBSTACLES_TO_FRONT, p_Globals);
    }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
