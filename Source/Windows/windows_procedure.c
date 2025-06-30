/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../../Headers/ai.h"
#include "../../Headers/assets.h"
#include "../../Headers/common_types.h"
#include "../../Headers/constants.h"
#include "../../Headers/entity.h"
#include "../../Headers/enums.h"
#include "../../Headers/globals.h"
#include "../../Headers/heap_allocator.h"
#include "../../Headers/load.h"
#include "../../Headers/log.h"
#include "../../Headers/menu.h"
#include "../../Headers/misc.h"
#include "../../Headers/path_finding.h"
#include "../../Headers/process.h"
#include "../../Headers/Renderer/renderer.h"
#include "../../Headers/save.h"
#include "../../Headers/settings.h"
#include "../../Headers/Windows/windows_main.h"
#include "../../Headers/Windows/windows_minified.h"
#include "../../Headers/Windows/windows_procedure.h"
#include <math.h>
#include <shellapi.h>
#include <stdlib.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Statics:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static RECT OriginalWindowArea = { 0 };
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Future:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// - hWnd is unused here in favour of the global handle.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WINDOWS_PROCEDURE_WindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
    switch (uiMsg) {
        // Interferes with WM_SIZE.
        /*
        case WM_WINDOWPOSCHANGED: {
            const WINDOWPOS* p_WindowPos = (const WINDOWPOS*)lParam;
            p_Globals->WindowPosition = *p_WindowPos;
            break;
        }
        //*/
    case WM_CREATE: {
        // Safely transfer the handle.
        p_Globals->hWnd = hWnd;
        break;
    }
    case WM_COMMAND: {
        WINDOWS_PROCEDURE_HandleMenuEvent(wParam, p_Menu, p_Globals, p_Globals->p_ActiveRenderer, p_Log, p_Settings, p_Assets, p_Constants);
        break;
    }
    case WM_SIZE: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Globals->p_ActiveRenderer != NULL) {
            p_Globals->p_ActiveRenderer->Resize(p_Globals->p_ActiveRenderer, p_Globals, p_Log);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Globals->p_ActiveRenderer->InitWorldTransform(p_Globals->p_ActiveRenderer);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Globals->ubForceFullFrameRender = 1;
        }
        break;
    }
    case WM_GETMINMAXINFO: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        MINMAXINFO* p_MinMaxInfo = (MINMAXINFO*)lParam;
        RECT MaxClientSize = { 0, 0, p_Constants->MaxClientSize.uiDx, p_Constants->MaxClientSize.uiDy };
        RECT MinClientSize = { 0, 0, p_Constants->MinClientSize.uiDx, p_Constants->MinClientSize.uiDy };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        HMENU hMenu = GetMenu(p_Globals->hWnd);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        DWORD dwStyle = WS_OVERLAPPEDWINDOW, dwExStyle = WS_EX_OVERLAPPEDWINDOW;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        AdjustWindowRectEx(&MaxClientSize, dwStyle, hMenu != NULL, dwExStyle);
        AdjustWindowRectEx(&MinClientSize, dwStyle, hMenu != NULL, dwExStyle);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_MinMaxInfo->ptMaxTrackSize.x = MaxClientSize.right - MaxClientSize.left;
        p_MinMaxInfo->ptMaxTrackSize.y = MaxClientSize.bottom - MaxClientSize.top;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_MinMaxInfo->ptMinTrackSize.x = MinClientSize.right - MinClientSize.left;
        p_MinMaxInfo->ptMinTrackSize.y = MinClientSize.bottom - MinClientSize.top;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        break;
    }
    case WM_LBUTTONUP: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Reset for subsequent left mouse clicks.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (!p_Globals->ubClickOriginFromMinimap) {
            WINDOWS_PROCEDURE_SelectEntities(p_Globals);
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->ubIsSelecting = 0;
        p_Globals->ubClickOriginFromMinimap = 0;
        break;
    }
    case WM_LBUTTONDOWN: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (PROCESS_IsMinimapClicked(p_Globals->p_ActiveRenderer, p_Assets, p_Globals, p_Settings)) {
            p_Globals->ubClickOriginFromMinimap = 1;
            PROCESS_AdjustMinimapViewport(p_Globals->Mouse, p_Globals, p_Globals->p_ActiveRenderer, p_Assets, p_Settings);
        }
        else {
            p_Globals->ubIsSelecting = 1;
            p_Globals->ubClickOriginFromMinimap = 0;
            WINDOWS_PROCEDURE_CreateOrCaptureEntities(p_Globals, p_Assets, p_Log, p_Globals->p_ActiveRenderer, p_Settings, p_Constants);
        }
        break;
    }
    case WM_RBUTTONDOWN: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        WINDOWS_PROCEDURE_HandleRightClick(p_Globals, p_Menu, p_Assets, p_Log, p_Settings);
        break;
    }
    case WM_MOUSEMOVE: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->Mouse = (IPOINT_T){ LOWORD(lParam), HIWORD(lParam) };
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
                p_Globals->CaptureCurrent.iX = p_Globals->Mouse.iX + -p_Globals->Translation.fDx;
                p_Globals->CaptureCurrent.iY = p_Globals->Mouse.iY + -p_Globals->Translation.fDy;
            }
        }
        else {
            PROCESS_AdjustMinimapViewport(p_Globals->Mouse, p_Globals, p_Globals->p_ActiveRenderer, p_Assets, p_Settings);
        }
        break;
    }
    case WM_KEYDOWN: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        WINDOWS_PROCEDURE_HandleKeyDown(wParam, p_Globals, p_Menu, p_Globals->p_ActiveRenderer, p_Log, p_Settings, p_Constants, p_Assets);
        break;
    }
    case WM_CLOSE: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        PostQuitMessage(EXIT_SUCCESS);
        break;
    }
    default: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Do not use the global window handle.
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
void __cdecl WINDOWS_PROCEDURE_BuildHelper(ENTITY_TYPE_T eType, MENU_T* p_Menu) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Uncheck all of the menu items.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CheckMenuItem(p_Menu->hMenu, MENU_BUILD_NONE, MF_UNCHECKED);
    CheckMenuItem(p_Menu->hMenu, MENU_BUILD_WORKER, MF_UNCHECKED);
    CheckMenuItem(p_Menu->hMenu, MENU_BUILD_COMMAND, MF_UNCHECKED);
    CheckMenuItem(p_Menu->hMenu, MENU_BUILD_MINERAL, MF_UNCHECKED);
    CheckMenuItem(p_Menu->hMenu, MENU_BUILD_SUPPLY, MF_UNCHECKED);
    CheckMenuItem(p_Menu->hMenu, MENU_BUILD_REFINERY, MF_UNCHECKED);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->usBuildType = eType;
    p_Globals->ubCreate = 1;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Check the one we want.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    switch (eType) {
    case ENTITY_TYPE_WORKER: {
        CheckMenuItem(p_Menu->hMenu, MENU_BUILD_WORKER, MF_CHECKED);
        break;
    }
    case ENTITY_TYPE_COMMAND: {
        CheckMenuItem(p_Menu->hMenu, MENU_BUILD_COMMAND, MF_CHECKED);
        break;
    }
    case ENTITY_TYPE_MINERAL: {
        CheckMenuItem(p_Menu->hMenu, MENU_BUILD_MINERAL, MF_CHECKED);
        break;
    }
    case ENTITY_TYPE_SUPPLY: {
        CheckMenuItem(p_Menu->hMenu, MENU_BUILD_SUPPLY, MF_CHECKED);
        break;
    }
    case ENTITY_TYPE_REFINERY: {
        CheckMenuItem(p_Menu->hMenu, MENU_BUILD_REFINERY, MF_CHECKED);
        break;
    }
    default: {
        CheckMenuItem(p_Menu->hMenu, MENU_BUILD_NONE, MF_CHECKED);
    }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINDOWS_PROCEDURE_RendererHelper(RENDERER_TYPE_T eType, GLOBALS_T* p_Globals, MENU_T* p_Menu) {
    HMENU hMenu = p_Menu->hMenu;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Uncheck all of the menu items.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CheckMenuItem(hMenu, MENU_RENDERER_WIREFRAME, MF_UNCHECKED);
    CheckMenuItem(hMenu, MENU_RENDERER_SINGLE_BUFFER, MF_UNCHECKED);
    CheckMenuItem(hMenu, MENU_RENDERER_DOUBLE_BUFFER, MF_UNCHECKED);
    CheckMenuItem(hMenu, MENU_RENDERER_SDL, MF_UNCHECKED);
    CheckMenuItem(hMenu, MENU_RENDERER_DIRECT2D, MF_UNCHECKED);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->eActiveRendererType = eType;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Check the one we want.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    switch (eType) {
    case RENDERER_TYPE_WIREFRAME: {
        CheckMenuItem(hMenu, MENU_RENDERER_WIREFRAME, MF_CHECKED);
        p_Globals->p_ActiveRenderer = p_Globals->p_Renderers[eType];
        break;
    }
    case RENDERER_TYPE_SINGLE_BUFFER: {
        CheckMenuItem(hMenu, MENU_RENDERER_SINGLE_BUFFER, MF_CHECKED);
        p_Globals->p_ActiveRenderer = p_Globals->p_Renderers[eType];
        break;
    }
    case RENDERER_TYPE_DOUBLE_BUFFER: {
        CheckMenuItem(hMenu, MENU_RENDERER_DOUBLE_BUFFER, MF_CHECKED);
        p_Globals->p_ActiveRenderer = p_Globals->p_Renderers[eType];
        break;
    }
    case RENDERER_TYPE_SDL: {
        CheckMenuItem(hMenu, MENU_RENDERER_SDL, MF_CHECKED);
        p_Globals->p_ActiveRenderer = p_Globals->p_Renderers[eType];
        break;
    }
    case RENDERER_TYPE_DIRECT2D: {
        CheckMenuItem(hMenu, MENU_RENDERER_DIRECT2D, MF_CHECKED);
        p_Globals->p_ActiveRenderer = p_Globals->p_Renderers[eType];
        break;
    }
    default: {
        MISC_WriteOut(p_Log, LOG_SEVERITY_WARNING, "WINDOWS_PROCEDURE_RendererHelper(): Uncased renderer type.\n");
        CheckMenuItem(hMenu, MENU_RENDERER_WIREFRAME, MF_CHECKED);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->eActiveRendererType = RENDERER_TYPE_WIREFRAME;
        p_Globals->p_ActiveRenderer = p_Globals->p_Renderers[RENDERER_TYPE_WIREFRAME];
    }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINDOWS_PROCEDURE_HandleRightClick(GLOBALS_T* p_Globals, MENU_T* p_Menu, ASSETS_T* p_Assets, LOG_T* p_Log, SETTINGS_T* p_Settings) {
    if (p_Globals->ubCreate) {
        WINDOWS_PROCEDURE_BuildNone(p_Globals, p_Menu);
    }
    else {
        if (p_Menu->ubToggleScatter) {
            SELECTED_COUNT_T SelectedCount = ENTITY_GetSelectedEntityCounts(p_Globals);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (SelectedCount.usSelectedWorkersCount > 1) {
                WINDOWS_PROCEDURE_DistributeAndSendWorkers(p_Globals, p_Assets, p_Log, p_Menu, p_Settings);
            }
            else {
                WINDOWS_PROCEDURE_SendWorkers(p_Globals, p_Assets, p_Log, p_Menu, p_Settings);
            }
        }
        else {
            WINDOWS_PROCEDURE_SendWorkers(p_Globals, p_Assets, p_Log, p_Menu, p_Settings);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINDOWS_PROCEDURE_BuildNone(GLOBALS_T* p_Globals, MENU_T* p_Menu) {
    WINDOWS_PROCEDURE_BuildHelper(ENTITY_TYPE_NONE, p_Menu);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->usBuildType = ENTITY_TYPE_NONE;
    p_Globals->ubCreate = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINDOWS_PROCEDURE_SendWorkers(GLOBALS_T* p_Globals, ASSETS_T* p_Assets, LOG_T* p_Log, MENU_T* p_Menu, SETTINGS_T* p_Settings) {
    USHORT usMoveableCount = 0;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ENTITY_T* p_Resource = WINDOWS_PROCEDURE_CaptureDestinationData(&usMoveableCount, p_Globals);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // We did not click on a resource, therefore we have to move the selected entities.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!p_Resource) {
        FPOINT_LIST_T* p_DestinationPoints = WINDOWS_PROCEDURE_CaptureDestinationPoints(usMoveableCount, p_Globals, p_Assets, p_Log);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Distribute the workers to their destination points.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT_LIST_T* p_DestinationPointsHead = p_DestinationPoints;
        ENTITY_T* p_Current = p_Globals->p_RootEntity;
        while (p_Current) {
            if (p_Current->ubIsSelected) {
                p_Current->ubIsSelected = 0;
                if (p_Current->ubIsMovable) {
                    p_Current->p_OperatingTarget = NULL;
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////
                    FPOINT_T DestinationPoint = p_DestinationPoints->Point;
                    p_DestinationPoints = p_DestinationPoints->p_Next;
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////
                    PATH_ComputePath(p_Current, p_Current->CenterPoint, DestinationPoint, 0, 0, p_Globals, p_Log, p_Settings);
                    PATH_MoveToPoint(p_Current, DestinationPoint, p_Globals, p_Menu, p_Settings);
                }
            }
            p_Current = (ENTITY_T*)p_Current->p_Next;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_DestinationPointsHead) {
            MISC_LinkedListFreeAll(p_DestinationPointsHead, p_Globals->p_fPointAllocator, FPOINT_LIST_GetNextNode, FPOINT_LIST_SetNextNode, p_Log);
        }
    }
    else {
        ENTITY_T* p_Current = p_Globals->p_RootEntity;
        while (p_Current) {
            if (p_Current->ubIsSelected) {
                p_Current->ubIsSelected = 0;
                if (p_Current->ubIsMovable) {
                    p_Current->p_OperatingTarget = (ENTITY_T*)p_Resource;
                }
            }
            p_Current = (ENTITY_T*)p_Current->p_Next;
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINDOWS_PROCEDURE_DistributeAndSendWorkers(GLOBALS_T* p_Globals, ASSETS_T* p_Assets, LOG_T* p_Log, MENU_T* p_Menu, SETTINGS_T* p_Settings) {
    USHORT usMoveableCount = 0;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ENTITY_T* p_Resource = WINDOWS_PROCEDURE_CaptureDestinationData(&usMoveableCount, p_Globals);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // If we clicked on a resource, get a collection of pointers to available resources of that type, considering their distance.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_Resource) {
        AI_CLOSEST_T* p_ClosestEntities = AI_FindClosestByDistance(p_Resource, p_Resource->eType, p_Globals, p_Log);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        AI_CLOSEST_T* p_ClosestEntitiesHead = p_ClosestEntities;
        ENTITY_T* p_Current = p_Globals->p_RootEntity;
        while (p_Current) {
            if (p_Current->ubIsSelected) {
                p_Current->ubIsSelected = 0;
                if (p_Current->ubIsMovable) {
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // Run through the resources ordered by their closest distance from the inquirer.
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////
                    p_Current->p_OperatingTarget = (ENTITY_T*)p_ClosestEntities->p_Entity;
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // There will most likely be cases where there are more workers than available resources. Let's reset the
                    // index to recycle them.
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////
                    if (p_ClosestEntities->p_Next) {
                        p_ClosestEntities = p_ClosestEntities->p_Next;
                    }
                    else {
                        p_ClosestEntities = p_ClosestEntitiesHead;
                    }
                }
            }
            p_Current = (ENTITY_T*)p_Current->p_Next;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_ClosestEntitiesHead) {
            MISC_LinkedListFreeAll(p_ClosestEntitiesHead, p_Globals->p_AiClosestAllocator, AI_CLOSEST_GetNextNode, AI_CLOSEST_SetNextNode, p_Log);
        }
    }
    else {
        FPOINT_LIST_T* p_DestinationPoints = WINDOWS_PROCEDURE_CaptureDestinationPoints(usMoveableCount, p_Globals, p_Assets, p_Log);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Distribute the workers to their destination points.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT_LIST_T* p_DestinationPointsHead = p_DestinationPoints;
        ENTITY_T* p_Current = p_Globals->p_RootEntity;
        while (p_Current) {
            if (p_Current->ubIsSelected) {
                p_Current->ubIsSelected = 0;
                if (p_Current->ubIsMovable) {
                    p_Current->p_OperatingTarget = NULL;
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////
                    FPOINT_T DestinationPoint = p_DestinationPoints->Point;
                    p_DestinationPoints = p_DestinationPoints->p_Next;
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////
                    PATH_ComputePath(p_Current, p_Current->CenterPoint, DestinationPoint, 0, 0, p_Globals, p_Log, p_Settings);
                    PATH_MoveToPoint(p_Current, DestinationPoint, p_Globals, p_Menu, p_Settings);
                }
            }
            p_Current = (ENTITY_T*)p_Current->p_Next;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_DestinationPointsHead) {
            MISC_LinkedListFreeAll(p_DestinationPointsHead, p_Globals->p_fPointAllocator, FPOINT_LIST_GetNextNode, FPOINT_LIST_SetNextNode, p_Log);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FPOINT_LIST_T* __cdecl WINDOWS_PROCEDURE_CaptureDestinationPoints(USHORT usMoveableCount, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, LOG_T* p_Log) {
    FPOINT_LIST_T* p_DestinationPoints = NULL;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    USHORT usI;
    USHORT usColumn = 1;
    USHORT usMargin = 15;
    USHORT usColumnLimit = sqrt(usMoveableCount);
    FPOINT_T Placement = { p_Globals->Mouse.iX, p_Globals->Mouse.iY };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (usI = 0; usI < usMoveableCount; usI++) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT_LIST_T* p_Node = (FPOINT_LIST_T*)HEAP_ALLOCATOR_Alloc(p_Globals->p_fPointAllocator, p_Log);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (!p_Node) {
            MISC_WriteOut(p_Log, LOG_SEVERITY_FATAL, "WINDOWS_PROCEDURE_CaptureDestinationPoints(): Allocation failed.\n");
            UINT8 ubDiscard = MAIN_FailFast(p_Globals, p_Log);
            return NULL;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Node->Point = Placement;
        p_Node->p_Next = p_DestinationPoints;
        p_DestinationPoints = p_Node;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Adjust for next placement.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (usColumn >= usColumnLimit) {
            Placement.fY += p_Assets->Worker[0].Bitmap.bmHeight + usMargin;
            Placement.fX = p_Globals->Mouse.iX;
            usColumn = 1;
        }
        else {
            Placement.fX += p_Assets->Worker[0].Bitmap.bmWidth + usMargin;
            usColumn++;
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return p_DestinationPoints;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ENTITY_T* __cdecl WINDOWS_PROCEDURE_CaptureDestinationData(USHORT* p_usMoveableCount, GLOBALS_T* p_Globals) {
    ENTITY_T* p_Resource = NULL;
    ENTITY_T* p_Current = p_Globals->p_RootEntity;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Loop through all of the entities and check to see if we right-clicked on a resource, or capture the total moveable count.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (p_Current && !p_Resource) {
        if (p_Current->ubIsMovable) {
            if (p_Current->ubIsSelected) {
                (*p_usMoveableCount)++;
            }
        }
        switch (p_Current->eType) {
        case ENTITY_TYPE_MINERAL: {
            if (PATH_WithinPoint(p_Current, (FPOINT_T) { p_Globals->Mouse.iX, p_Globals->Mouse.iY })) {
                p_Resource = p_Current;
            }
            break;
        }
        case ENTITY_TYPE_REFINERY: {
            if (PATH_WithinPoint(p_Current, (FPOINT_T) { p_Globals->Mouse.iX, p_Globals->Mouse.iY })) {
                p_Resource = p_Current;
            }
        }
        }
        p_Current = (ENTITY_T*)p_Current->p_Next;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return p_Resource;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINDOWS_PROCEDURE_CreateOrCaptureEntities(GLOBALS_T* p_Globals, ASSETS_T* p_Assets, LOG_T* p_Log, RENDERER_T* p_Renderer, SETTINGS_T* p_Settings, CONSTANTS_T* p_Constants) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // For mouse drag selection.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->MouseOld = p_Globals->Mouse;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // If build mode is selected, create an entity at the mouse position.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_Globals->ubCreate) {
        ENTITY_Create((FPOINT_T) { p_Globals->Mouse.iX, p_Globals->Mouse.iY },
            p_Globals->usBuildType,
            p_Assets,
            p_Globals,
            p_Log,
            p_Renderer,
            p_Settings,
            p_Constants
        );
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Build mode is not selected, therefore allow the left mouse click to select entities.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else {
        UINT8 ubIsSomethingThere = 0;
        ENTITY_T* p_Current = p_Globals->p_RootEntity;
        while (p_Current) {
            if (PATH_WithinPoint(p_Current, (FPOINT_T) { p_Globals->Mouse.iX, p_Globals->Mouse.iY })) {
                p_Current->ubIsSelected = !p_Current->ubIsSelected;
                ubIsSomethingThere = 1;
            }
            p_Current = (ENTITY_T*)p_Current->p_Next;
        }
        if (!ubIsSomethingThere) {
            p_Current = p_Globals->p_RootEntity;
            while (p_Current) {
                p_Current->ubIsSelected = 0;
                p_Current = (ENTITY_T*)p_Current->p_Next;
            }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Consider translations when calculating selection area during scrolling.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->CaptureStart.iX = p_Globals->MouseOld.iX + -p_Globals->Translation.fDx;
        p_Globals->CaptureStart.iY = p_Globals->MouseOld.iY + -p_Globals->Translation.fDy;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINDOWS_PROCEDURE_SelectEntities(GLOBALS_T* p_Globals) {
    if (!p_Globals->ubCreate) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Allow the operation to work when the mouse is dragged from all directions.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        IPOINT_T Saved = p_Globals->Mouse;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Globals->Mouse.iX < p_Globals->MouseOld.iX) {
            int iHolder = p_Globals->Mouse.iX;
            p_Globals->Mouse.iX = p_Globals->MouseOld.iX;
            p_Globals->MouseOld.iX = iHolder;
        }
        if (p_Globals->Mouse.iY < p_Globals->MouseOld.iY) {
            int iHolder = p_Globals->Mouse.iY;
            p_Globals->Mouse.iY = p_Globals->MouseOld.iY;
            p_Globals->MouseOld.iY = iHolder;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_T* p_Current = p_Globals->p_RootEntity;
        while (p_Current) {
            if (p_Current->CenterPoint.fX >= p_Globals->MouseOld.iX &&
                p_Current->CenterPoint.fX <= p_Globals->Mouse.iX &&
                p_Current->CenterPoint.fY >= p_Globals->MouseOld.iY &&
                p_Current->CenterPoint.fY <= p_Globals->Mouse.iY) {
                p_Current->ubIsSelected = !p_Current->ubIsSelected;
            }
            p_Current = (ENTITY_T*)p_Current->p_Next;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Incase the points were swapped, reset the original values.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->Mouse.iX = Saved.iX;
        p_Globals->Mouse.iY = Saved.iY;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->ubDrawSelectionRect = 0;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!p_Globals->ubDrawSelectionRect) {
        p_Globals->CaptureStart.iX = 0;
        p_Globals->CaptureStart.iY = 0;
        p_Globals->CaptureCurrent.iX = 0;
        p_Globals->CaptureCurrent.iY = 0;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINDOWS_PROCEDURE_HandleBorderless(GLOBALS_T* p_Globals, MENU_T* p_Menu, RENDERER_T* p_Renderer, SETTINGS_T* p_Settings, ASSETS_T* p_Assets, LOG_T* p_Log) {
    if (p_Menu->ubIsFullScreen) {
        WINDOWS_PROCEDURE_HandleFullscreen(p_Globals, p_Menu, p_Renderer, p_Settings, p_Assets, p_Log);
    }
    else {
        if (p_Globals->ubIsBorderless) {
            WINDOWS_PROCEDURE_ExitBorderless(p_Globals, p_Menu, p_Renderer);
        }
        else {
            WINDOWS_PROCEDURE_EnterBorderless(p_Globals, p_Renderer, p_Settings);
        }
        p_Globals->ubForceFullFrameRender = 1;
        p_Globals->ubIsBorderless = !p_Globals->ubIsBorderless;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINDOWS_PROCEDURE_EnterBorderless(GLOBALS_T* p_Globals, RENDERER_T* p_Renderer, SETTINGS_T* p_Settings) {
    RECT ClientArea = p_Renderer->GetClientArea(p_Renderer);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int iWidth = ClientArea.right - ClientArea.left;
    int iHeight = ClientArea.bottom - ClientArea.top;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GetWindowRect(p_Globals->hWnd, &OriginalWindowArea);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetWindowLongPtr(p_Globals->hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    SetWindowPos(p_Globals->hWnd, NULL, 0, 0, iWidth, iHeight, SWP_NOZORDER | SWP_FRAMECHANGED);
    SetMenu(p_Globals->hWnd, NULL);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Windows cleans up.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HRGN hRegion = CreateRoundRectRgn(0, 0, iWidth, iHeight, p_Settings->usWindowRegionRounding, p_Settings->usWindowRegionRounding);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetWindowRgn(p_Globals->hWnd, hRegion, 1);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Re-center the window.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MISC_ResizeWindow(p_Globals->hWnd, ClientArea.right, ClientArea.bottom, p_Globals);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINDOWS_PROCEDURE_ExitBorderless(GLOBALS_T* p_Globals, MENU_T* p_Menu, RENDERER_T* p_Renderer) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetWindowLongPtr(p_Globals->hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetWindowPos(p_Globals->hWnd, NULL, OriginalWindowArea.left, OriginalWindowArea.top,
        OriginalWindowArea.right - OriginalWindowArea.left,
        OriginalWindowArea.bottom - OriginalWindowArea.top, SWP_NOZORDER | SWP_FRAMECHANGED);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetMenu(p_Globals->hWnd, p_Menu->hMenu);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetWindowRgn(p_Globals->hWnd, NULL, 1);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    RECT ClientArea = p_Renderer->GetClientArea(p_Renderer);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Re-center the window.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MISC_ResizeWindow(p_Globals->hWnd, ClientArea.right, ClientArea.bottom, p_Globals);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINDOWS_PROCEDURE_EnterFullscreen(GLOBALS_T* p_Globals, MENU_T* p_Menu, RENDERER_T* p_Renderer, LOG_T* p_Log) {
    SetWindowRgn(p_Globals->hWnd, NULL, 1);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_FULLSCREEN, MF_CHECKED);
    p_Menu->ubIsFullScreen = !p_Menu->ubIsFullScreen;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetWindowLongPtr(p_Globals->hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    SetWindowPos(p_Globals->hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MoveWindow(p_Globals->hWnd, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 1);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->Resize(p_Renderer, p_Globals, p_Log);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->InitWorldTransform(p_Renderer);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINDOWS_PROCEDURE_ExitFullscreen(GLOBALS_T* p_Globals, MENU_T* p_Menu, RENDERER_T* p_Renderer, SETTINGS_T* p_Settings, LOG_T* p_Log) {
    CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_FULLSCREEN, MF_UNCHECKED);
    p_Menu->ubIsFullScreen = !p_Menu->ubIsFullScreen;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Ensure the menu is always available.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetMenu(p_Globals->hWnd, p_Menu->hMenu);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Change double buffer settings.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetWindowLongPtr(p_Globals->hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
    SetWindowPos(p_Globals->hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MISC_ResizeWindow(p_Globals->hWnd, p_Settings->usInitialClientWidth, p_Settings->usInitialClientHeight, p_Globals);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->Resize(p_Renderer, p_Globals, p_Log);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Reset the graphics mode to allow the rendering engine to work.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->InitWorldTransform(p_Renderer);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINDOWS_PROCEDURE_HandleFullscreen(GLOBALS_T* p_Globals, MENU_T* p_Menu, RENDERER_T* p_Renderer, SETTINGS_T* p_Settings, ASSETS_T* p_Assets, LOG_T* p_Log) {
    if (p_Menu->ubIsFullScreen) {
        WINDOWS_PROCEDURE_ExitFullscreen(p_Globals, p_Menu, p_Renderer, p_Settings, p_Log);
    }
    else {
        WINDOWS_PROCEDURE_EnterFullscreen(p_Globals, p_Menu, p_Renderer, p_Log);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Stay within safe bounds.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_AdjustMinimapViewport((IPOINT_T) { 0, 0 }, p_Globals, p_Globals->p_ActiveRenderer, p_Assets, p_Settings);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->ubForceFullFrameRender = 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINDOWS_PROCEDURE_HandleKeyDown(WPARAM wParam, GLOBALS_T* p_Globals, MENU_T* p_Menu, RENDERER_T* p_Renderer, LOG_T* p_Log, SETTINGS_T* p_Settings, CONSTANTS_T* p_Constants, ASSETS_T* p_Assets) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Arrow keys.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    WINDOWS_PROCEDURE_HandleArrowKeys(wParam, p_Assets, p_Globals, p_Menu, p_Renderer, p_Settings, p_Log);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Chord keys.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (WINDOWS_PROCEDURE_HandleChordKeys(wParam, MOD_CONTROL, 'F')) {
        WINDOWS_PROCEDURE_HandleFullscreen(p_Globals, p_Menu, p_Renderer, p_Settings, p_Assets, p_Log);
    }
    else if (WINDOWS_PROCEDURE_HandleChordKeys(wParam, MOD_CONTROL, 'A')) {
        ENTITY_SelectAll(p_Globals);
    }
    else if (WINDOWS_PROCEDURE_HandleChordKeys(wParam, MOD_CONTROL, 'R')) {
        SETTINGS_PropagateHotReload(p_Settings, p_Constants, p_Log, p_Constants->szDefaultSettingsFile, p_Globals);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Other keys.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    WINDOWS_PROCEDURE_HandleOtherKeys(wParam, p_Globals, p_Menu, p_Renderer, p_Settings, p_Assets, p_Log);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 __cdecl WINDOWS_PROCEDURE_HandleChordKeys(
    WPARAM wParamKey,
    USHORT usRequiredModifiers,
    WPARAM wParamWithKey) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UINT8 ubIsChordPressed = 0;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UINT8 ubCtrlHeld = (GetKeyState(VK_LCONTROL) & 0x8000) || (GetKeyState(VK_RCONTROL) & 0x8000);
    UINT8 ubShiftHeld = (GetKeyState(VK_LSHIFT) & 0x8000) || (GetKeyState(VK_RSHIFT) & 0x8000);
    UINT8 ubAltHeld = (GetKeyState(VK_LMENU) & 0x8000) || (GetKeyState(VK_RMENU) & 0x8000);
    UINT8 ubKeyHeld = ((wParamKey == wParamWithKey) && (GetKeyState((int)wParamKey) & 0x8000));
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UINT8 ubCtrlChord = (usRequiredModifiers & MOD_CONTROL) ? ubCtrlHeld : 1;
    UINT8 ubShiftChord = (usRequiredModifiers & MOD_SHIFT) ? ubShiftHeld : 1;
    UINT8 ubAltChord = (usRequiredModifiers & MOD_ALT) ? ubAltHeld : 1;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (ubKeyHeld && ubCtrlChord && ubShiftChord && ubAltChord) {
        ubIsChordPressed = 1;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return ubIsChordPressed;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINDOWS_PROCEDURE_HandleArrowKeys(WPARAM wParam, ASSETS_T* p_Assets, GLOBALS_T* p_Globals, MENU_T* p_Menu, RENDERER_T* p_Renderer, SETTINGS_T* p_Settings, LOG_T* p_Log) {
    if (p_Menu->ubEnableTranslations) {
        FDELTA_T CapturedTranslations = { 0.0f, 0.0f };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        float fAmount = p_Settings->fTranslationStepAmount;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        LONG lBmWidth = p_Globals->TerrainCombinedSize.ulDx;
        LONG lBmHeight = p_Globals->TerrainCombinedSize.ulDy;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        RECT ClientArea = p_Renderer->GetClientArea(p_Renderer);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        USHORT usThreshold = p_Globals->usThreshold;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TRANSLATION_ZONE_T eRequestedTranslation = TRANSLATION_ZONE_UNKNOWN;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        switch (wParam) {
        case VK_LEFT: {
            eRequestedTranslation = TRANSLATION_ZONE_LEFT;
            break;
        }
        case VK_UP: {
            eRequestedTranslation = TRANSLATION_ZONE_TOP;
            break;
        }
        case VK_RIGHT: {
            eRequestedTranslation = TRANSLATION_ZONE_RIGHT;
            break;
        }
        case VK_DOWN: {
            eRequestedTranslation = TRANSLATION_ZONE_BOTTOM;
            break;
        }
        default:
            break;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (eRequestedTranslation != TRANSLATION_ZONE_UNKNOWN) {
            p_Renderer->CapturedTranslationsKeys = PROCESS_CaptureTranslationAmount(eRequestedTranslation,
                p_Globals->Translation,
                fAmount,
                lBmWidth,
                lBmHeight,
                ClientArea.right,
                ClientArea.bottom,
                usThreshold,
                p_Log);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINDOWS_PROCEDURE_HandleOtherKeys(WPARAM wParam, GLOBALS_T* p_Globals, MENU_T* p_Menu, RENDERER_T* p_Renderer, SETTINGS_T* p_Settings, ASSETS_T* p_Assets, LOG_T* p_Log) {
    switch (wParam) {
    case VK_ESCAPE: {
        if (p_Globals->ubCreate) {
            WINDOWS_PROCEDURE_BuildNone(p_Globals, p_Menu);
        }
        else {
            WINDOWS_PROCEDURE_HandleBorderless(p_Globals, p_Menu, p_Renderer, p_Settings, p_Assets, p_Log);
        }
        break;
    }
    case VK_DELETE: {
        ENTITY_DeleteSelected(p_Globals, p_Settings, p_Log);
        break;
    }
    default:
        break;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl WINDOWS_PROCEDURE_HandleMenuEvent(WPARAM wParam, MENU_T* p_Menu, GLOBALS_T* p_Globals, RENDERER_T* p_Renderer, LOG_T* p_Log, SETTINGS_T* p_Settings, ASSETS_T* p_Assets, CONSTANTS_T* p_Constants) {
    switch (LOWORD(wParam)) {
    case MENU_SIMULATION_FREE: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->iMineralCount += 5000;
        p_Globals->iGasCount += 5000;
        break;
    }
    case MENU_SIMULATION_EXIT: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        PostMessage(p_Globals->hWnd, WM_CLOSE, 0, 0);
        break;
    }
    case MENU_SIMULATION_SAVE: {
        SAVE_SaveSimulation(p_Globals, p_Constants);
        break;
    }
    case MENU_SIMULATION_LOAD: {
        LOAD_LoadSimulation(p_Globals, p_Constants);
        break;
    }
    case MENU_SIMULATION_PRINT: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_PrintList(p_Globals, p_Log);
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
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Stop each entity that was moving.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            ENTITY_T* p_Current = p_Globals->p_RootEntity;
            while (p_Current) {
                p_Current->ubIsInMotion = 0;
                p_Current = (ENTITY_T*)p_Current->p_Next;
            }
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_SIMULATION_TOGGLE_SIMULATION, MF_CHECKED);
        }
        p_Menu->ubToggleSimulation = !p_Menu->ubToggleSimulation;
        break;
    }
    case MENU_DELETE_SELECTED: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_DeleteSelected(p_Globals, p_Settings, p_Log);
        break;
    }
    case MENU_DELETE_ALL: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_DeleteAll(p_Globals, p_Settings, p_Log);
        break;
    }
    case MENU_DELETE_WORKER: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_DeleteEntityType(ENTITY_TYPE_WORKER, p_Globals, p_Settings, p_Log);
        break;
    }
    case MENU_DELETE_COMMAND: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_DeleteEntityType(ENTITY_TYPE_COMMAND, p_Globals, p_Settings, p_Log);
        break;
    }
    case MENU_DELETE_MINERAL: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_DeleteEntityType(ENTITY_TYPE_MINERAL, p_Globals, p_Settings, p_Log);
        break;
    }
    case MENU_DELETE_SUPPLY: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_DeleteEntityType(ENTITY_TYPE_SUPPLY, p_Globals, p_Settings, p_Log);
        break;
    }
    case MENU_DELETE_REFINERY: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_DeleteEntityType(ENTITY_TYPE_REFINERY, p_Globals, p_Settings, p_Log);
        break;
    }
    case MENU_BUILD_NONE: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        WINDOWS_PROCEDURE_BuildNone(p_Globals, p_Menu);
        break;
    }
    case MENU_BUILD_WORKER: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        WINDOWS_PROCEDURE_BuildHelper(ENTITY_TYPE_WORKER, p_Menu);
        break;
    }
    case MENU_BUILD_COMMAND: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        WINDOWS_PROCEDURE_BuildHelper(ENTITY_TYPE_COMMAND, p_Menu);
        break;
    }
    case MENU_BUILD_MINERAL: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        WINDOWS_PROCEDURE_BuildHelper(ENTITY_TYPE_MINERAL, p_Menu);
        break;
    }
    case MENU_BUILD_SUPPLY: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        WINDOWS_PROCEDURE_BuildHelper(ENTITY_TYPE_SUPPLY, p_Menu);
        break;
    }
    case MENU_BUILD_REFINERY: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        WINDOWS_PROCEDURE_BuildHelper(ENTITY_TYPE_REFINERY, p_Menu);
        break;
    }
    case MENU_OPTIONS_ALL_DIAGNOSTICS: {
        long lRelatedItemsChecked = MF_UNCHECKED;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubAllDiagnosticsToggle) {
            lRelatedItemsChecked = MFS_UNCHECKED;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Menu->ubDrawIds =
                p_Menu->ubDrawStatistics =
                p_Menu->ubDrawResources =
                p_Menu->ubDrawStatuses =
                p_Menu->ubDrawMinorVectors =
                p_Menu->ubDrawMajorVectors =
                p_Menu->ubDrawDirtyZones =
                p_Menu->ubDrawTerrainGrid =
                p_Menu->ubDrawTranslationThreshold = 0;
        }
        else {
            lRelatedItemsChecked = MF_CHECKED;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Menu->ubDrawIds =
                p_Menu->ubDrawStatistics =
                p_Menu->ubDrawResources =
                p_Menu->ubDrawStatuses =
                p_Menu->ubDrawMinorVectors =
                p_Menu->ubDrawMajorVectors =
                p_Menu->ubDrawDirtyZones =
                p_Menu->ubDrawTerrainGrid =
                p_Menu->ubDrawTranslationThreshold = 1;
        }
        p_Menu->ubAllDiagnosticsToggle = !p_Menu->ubAllDiagnosticsToggle;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_IDS, lRelatedItemsChecked);
        CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_STATISTICS, lRelatedItemsChecked);
        CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_RESOURCES, lRelatedItemsChecked);
        CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_STATUSES, lRelatedItemsChecked);
        CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_SHOW_MINOR_VECTORS, lRelatedItemsChecked);
        CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_SHOW_MAJOR_VECTORS, lRelatedItemsChecked);
        CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_DIRTY_ZONES, lRelatedItemsChecked);
        CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_SHOW_TERRAIN_GRID, lRelatedItemsChecked);
        CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_SHOW_TRANSLATION_THRESHOLD, lRelatedItemsChecked);
        break;
    }
    case MENU_OPTIONS_STATISTICS: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubDrawStatistics) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_STATISTICS, MF_UNCHECKED);
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_STATISTICS, MF_CHECKED);
        }
        p_Menu->ubDrawStatistics = !p_Menu->ubDrawStatistics;
        break;
    }
    case MENU_OPTIONS_MASKING: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubEnableMasking) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_MASKING, MF_UNCHECKED);
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_MASKING, MF_CHECKED);
        }
        p_Menu->ubEnableMasking = !p_Menu->ubEnableMasking;
        break;
    }
    case MENU_OPTIONS_RESOURCES: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubDrawResources) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_RESOURCES, MF_UNCHECKED);
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_RESOURCES, MF_CHECKED);
        }
        p_Menu->ubDrawResources = !p_Menu->ubDrawResources;
        break;
    }
    case MENU_OPTIONS_DIRTY_ZONES: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubDrawDirtyZones) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_DIRTY_ZONES, MF_UNCHECKED);
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_DIRTY_ZONES, MF_CHECKED);
        }
        p_Menu->ubDrawDirtyZones = !p_Menu->ubDrawDirtyZones;
        break;
    }
    case MENU_OPTIONS_IDS: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubDrawIds) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_IDS, MF_UNCHECKED);
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_IDS, MF_CHECKED);
        }
        p_Menu->ubDrawIds = !p_Menu->ubDrawIds;
        break;
    }
    case MENU_OPTIONS_ENABLE_TRANSLATIONS: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubEnableTranslations) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_ENABLE_TRANSLATIONS, MF_UNCHECKED);
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_ENABLE_TRANSLATIONS, MF_CHECKED);
        }
        p_Menu->ubEnableTranslations = !p_Menu->ubEnableTranslations;
        break;
    }
    case MENU_OPTIONS_TOPMOST: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubIsTopmostWindow) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_TOPMOST, MF_UNCHECKED);
            SetWindowPos(p_Globals->hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
            //LONG_PTR dwExStyle = GetWindowLongPtr(p_Globals->hWnd, GWL_EXSTYLE);
            //dwExStyle &= ~WS_EX_TOPMOST;
            //SetWindowLongPtr(p_Globals->hWnd, GWL_EXSTYLE, dwExStyle);
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_TOPMOST, MF_CHECKED);
            SetWindowPos(p_Globals->hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
            //LONG_PTR dwExStyle = GetWindowLongPtr(p_Globals->hWnd, GWL_EXSTYLE);
            //dwExStyle = WS_EX_TOPMOST;
            //SetWindowLongPtr(p_Globals->hWnd, GWL_EXSTYLE, dwExStyle);
        }
        p_Menu->ubIsTopmostWindow = !p_Menu->ubIsTopmostWindow;
        break;
    }
    case MENU_SETTINGS_RELOAD: {
        SETTINGS_PropagateHotReload(p_Settings, p_Constants, p_Log, p_Constants->szDefaultSettingsFile, p_Globals);
        break;
    }
    case MENU_SETTINGS_OPEN: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        char* p_szFullPath = MISC_GetFullPath(p_Globals->szCurrentWorkingDirectory, p_Constants->szDefaultSettingsFile, p_Log);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_szFullPath) {
            ShellExecuteA(NULL, "open", p_szFullPath, NULL, NULL, SW_SHOWNORMAL);
        }
        break;
    }
    case MENU_LOG_OPEN: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        char* p_szFullPath = MISC_GetFullPath(p_Globals->szCurrentWorkingDirectory, p_Log->szAppendedFileName, p_Log);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_szFullPath) {
            ShellExecuteA(NULL, "open", p_szFullPath, NULL, NULL, SW_SHOWNORMAL);
        }
        break;
    }
    case MENU_OPTIONS_FULLSCREEN: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        WINDOWS_PROCEDURE_HandleFullscreen(p_Globals, p_Menu, p_Renderer, p_Settings, p_Assets, p_Log);
        break;
    }
    case MENU_OPTIONS_STATUSES: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubDrawStatuses) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_STATUSES, MF_UNCHECKED);
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_STATUSES, MF_CHECKED);
        }
        p_Menu->ubDrawStatuses = !p_Menu->ubDrawStatuses;
        break;
    }
    case MENU_OPTIONS_SHOW_MINOR_VECTORS: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubDrawMinorVectors) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_SHOW_MINOR_VECTORS, MF_UNCHECKED);
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_SHOW_MINOR_VECTORS, MF_CHECKED);
        }
        p_Menu->ubDrawMinorVectors = !p_Menu->ubDrawMinorVectors;
        break;
    }
    case MENU_OPTIONS_SHOW_MAJOR_VECTORS: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubDrawMajorVectors) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_SHOW_MAJOR_VECTORS, MF_UNCHECKED);
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_SHOW_MAJOR_VECTORS, MF_CHECKED);
        }
        p_Menu->ubDrawMajorVectors = !p_Menu->ubDrawMajorVectors;
        break;
    }
    case MENU_OPTIONS_PATH_FINDING_ENABLE: {
        long lRelatedItemsEnabled = MFS_DISABLED;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubEnablePathFinding) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_PATH_FINDING_ENABLE, MF_UNCHECKED);
            lRelatedItemsEnabled = MFS_DISABLED;
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_PATH_FINDING_ENABLE, MF_CHECKED);
            lRelatedItemsEnabled = MFS_ENABLED;
        }
        p_Menu->ubEnablePathFinding = !p_Menu->ubEnablePathFinding;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        EnableMenuItem(p_Menu->hMenu, MENU_OPTIONS_PATH_FINDING_RAYCAST, lRelatedItemsEnabled);
        EnableMenuItem(p_Menu->hMenu, MENU_OPTIONS_PATH_FINDING_BISECTION, lRelatedItemsEnabled);
        EnableMenuItem(p_Menu->hMenu, MENU_OPTIONS_PATH_FINDING_INTERPOLATE_CURVES, lRelatedItemsEnabled);
        break;
    }
    case MENU_OPTIONS_PATH_FINDING_RAYCAST: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (!p_Menu->ubUseRaycast) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_PATH_FINDING_RAYCAST, MF_CHECKED);
            p_Menu->ubUseRaycast = 1;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_PATH_FINDING_BISECTION, MF_UNCHECKED);
            p_Menu->ubUseBisection = 0;
        }
        break;
    }
    case MENU_OPTIONS_PATH_FINDING_BISECTION: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (!p_Menu->ubUseBisection) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_PATH_FINDING_BISECTION, MF_CHECKED);
            p_Menu->ubUseBisection = 1;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_PATH_FINDING_RAYCAST, MF_UNCHECKED);
            p_Menu->ubUseRaycast = 0;
        }
        break;
    }
    case MENU_OPTIONS_PATH_FINDING_INTERPOLATE_CURVES: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubInterpolateCurves) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_PATH_FINDING_INTERPOLATE_CURVES, MF_UNCHECKED);
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_PATH_FINDING_INTERPOLATE_CURVES, MF_CHECKED);
        }
        p_Menu->ubInterpolateCurves = !p_Menu->ubInterpolateCurves;
        break;
    }
    case MENU_OPTIONS_SHOW_TERRAIN_GRID: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubDrawTerrainGrid) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_SHOW_TERRAIN_GRID, MF_UNCHECKED);
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_SHOW_TERRAIN_GRID, MF_CHECKED);
        }
        p_Menu->ubDrawTerrainGrid = !p_Menu->ubDrawTerrainGrid;
        break;
    }
    case MENU_OPTIONS_SHOW_TRANSLATION_THRESHOLD: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubDrawTranslationThreshold) {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_SHOW_TRANSLATION_THRESHOLD, MF_UNCHECKED);
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_OPTIONS_SHOW_TRANSLATION_THRESHOLD, MF_CHECKED);
        }
        p_Menu->ubDrawTranslationThreshold = !p_Menu->ubDrawTranslationThreshold;
        break;
    }
    case MENU_CLEAR_SELECTED: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_T* p_Current = p_Globals->p_RootEntity;
        while (p_Current) {
            p_Current->ubIsSelected = 0;
            p_Current = (ENTITY_T*)p_Current->p_Next;
        }
        break;
    }
    case MENU_CLEAR_MINERAL_COUNT: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->iMineralCount = 0;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Clear minerals collected by the command centers.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_T* p_Current = p_Globals->p_RootEntity;
        while (p_Current) {
            switch (p_Current->eType) {
            case ENTITY_TYPE_COMMAND: {
                p_Current->iMineralCount = 0;
                break;
            }
            }
            p_Current = (ENTITY_T*)p_Current->p_Next;
        }
        break;
    }
    case MENU_CLEAR_GAS_COUNT: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->iGasCount = 0;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Clear gas collected by the command centers.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_T* p_Current = p_Globals->p_RootEntity;
        while (p_Current) {
            switch (p_Current->eType) {
            case ENTITY_TYPE_COMMAND: {
                p_Current->iGasCount = 0;
                break;
            }
            }
            p_Current = (ENTITY_T*)p_Current->p_Next;
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
        ENTITY_T* p_Current = p_Globals->p_RootEntity;
        while (p_Current) {
            switch (p_Current->eType) {
            case ENTITY_TYPE_COMMAND: {
                p_Current->iMineralCount = 0;
                p_Current->iGasCount = 0;
                break;
            }
            }
            p_Current = (ENTITY_T*)p_Current->p_Next;
        }
        break;
    }
    case MENU_SORT_WORKERS: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_SortToFront(SORT_ORDER_WORKERS_TO_FRONT, p_Globals, p_Log);
        break;
    }
    case MENU_SORT_MINERALS: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_SortToFront(SORT_ORDER_MINERALS_TO_FRONT, p_Globals, p_Log);
        break;
    }
    case MENU_SORT_COMMANDS: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_SortToFront(SORT_ORDER_COMMANDS_TO_FRONT, p_Globals, p_Log);
        break;
    }
    case MENU_SORT_SUPPLY: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_SortToFront(SORT_ORDER_SUPPLIES_TO_FRONT, p_Globals, p_Log);
        break;
    }
    case MENU_SORT_REFINERIES: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_SortToFront(SORT_ORDER_REFINERIES_TO_FRONT, p_Globals, p_Log);
        break;
    }
    case MENU_SORT_OBSTACLES: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_SortToFront(SORT_ORDER_OBSTACLES_TO_FRONT, p_Globals, p_Log);
        break;
    }
    case MENU_RENDERER_WIREFRAME: {
        WINDOWS_PROCEDURE_RendererHelper(RENDERER_TYPE_WIREFRAME, p_Globals, p_Menu);
        break;
    }
    case MENU_RENDERER_SINGLE_BUFFER: {
        WINDOWS_PROCEDURE_RendererHelper(RENDERER_TYPE_SINGLE_BUFFER, p_Globals, p_Menu);
        break;
    }
    case MENU_RENDERER_DOUBLE_BUFFER: {
        WINDOWS_PROCEDURE_RendererHelper(RENDERER_TYPE_DOUBLE_BUFFER, p_Globals, p_Menu);
        break;
    }
    case MENU_RENDERER_SDL: {
        WINDOWS_PROCEDURE_RendererHelper(RENDERER_TYPE_SDL, p_Globals, p_Menu);
        break;
    }
    case MENU_RENDERER_DIRECT2D: {
        WINDOWS_PROCEDURE_RendererHelper(RENDERER_TYPE_DIRECT2D, p_Globals, p_Menu);
        break;
    }
    case MENU_RENDERER_POST_PROCESSING_ENABLE: {
        long lRelatedItemsEnabled = MFS_DISABLED;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubEnablePostProcessing) {
            CheckMenuItem(p_Menu->hMenu, MENU_RENDERER_POST_PROCESSING_ENABLE, MF_UNCHECKED);
            p_Renderer->DisablingPostProcessing(p_Renderer, p_Menu, p_Globals, p_Log);
            lRelatedItemsEnabled = MFS_DISABLED;
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_RENDERER_POST_PROCESSING_ENABLE, MF_CHECKED);
            p_Renderer->EnablingPostProcessing(p_Renderer, p_Menu, p_Globals, p_Log);
            lRelatedItemsEnabled = MFS_ENABLED;
        }
        p_Menu->ubEnablePostProcessing = !p_Menu->ubEnablePostProcessing;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        EnableMenuItem(p_Menu->hMenu, MENU_RENDERER_POST_PROCESSING_BOX_BLUR, lRelatedItemsEnabled);
        EnableMenuItem(p_Menu->hMenu, MENU_RENDERER_POST_PROCESSING_GREEN_MASK, lRelatedItemsEnabled);
        EnableMenuItem(p_Menu->hMenu, MENU_RENDERER_POST_PROCESSING_DARKEN, lRelatedItemsEnabled);
        break;
    }
    case MENU_RENDERER_POST_PROCESSING_BOX_BLUR: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubEnableBoxBlur) {
            CheckMenuItem(p_Menu->hMenu, MENU_RENDERER_POST_PROCESSING_BOX_BLUR, MF_UNCHECKED);
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_RENDERER_POST_PROCESSING_BOX_BLUR, MF_CHECKED);
        }
        p_Menu->ubEnableBoxBlur = !p_Menu->ubEnableBoxBlur;
        break;
    }
    case MENU_RENDERER_POST_PROCESSING_GREEN_MASK: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubEnableGreenMask) {
            CheckMenuItem(p_Menu->hMenu, MENU_RENDERER_POST_PROCESSING_GREEN_MASK, MF_UNCHECKED);
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_RENDERER_POST_PROCESSING_GREEN_MASK, MF_CHECKED);
        }
        p_Menu->ubEnableGreenMask = !p_Menu->ubEnableGreenMask;
        break;
    }
    case MENU_RENDERER_POST_PROCESSING_DARKEN: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubEnableDarken) {
            CheckMenuItem(p_Menu->hMenu, MENU_RENDERER_POST_PROCESSING_DARKEN, MF_UNCHECKED);
        }
        else {
            CheckMenuItem(p_Menu->hMenu, MENU_RENDERER_POST_PROCESSING_DARKEN, MF_CHECKED);
        }
        p_Menu->ubEnableDarken = !p_Menu->ubEnableDarken;
        break;
    }
    default: {
        MISC_WriteOut(p_Log, LOG_SEVERITY_WARNING, "WINDOWS_PROCEDURE_HandleMenuEvent(): Uncased menu event.\n");
        break;
    }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
