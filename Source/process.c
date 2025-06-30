/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/ai.h"
#include "../Headers/assets.h"
#include "../Headers/card.h"
#include "../Headers/constants.h"
#include "../Headers/entity.h"
#include "../Headers/enums.h"
#include "../Headers/globals.h"
#include "../Headers/heap_allocator.h"
#include "../Headers/menu.h"
#include "../Headers/message.h"
#include "../Headers/misc.h"
#include "../Headers/path_finding.h"
#include "../Headers/process.h"
#include "../Headers/Renderer/renderer.h"
#include "../Headers/settings.h"
#include <stdio.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Future:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// - Put frequently used variables that are accessed via dereferencing onto the stack.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawSelectionArea(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets) {
    p_Renderer->DrawSelectionArea(p_Renderer, p_Globals, p_Assets);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 __cdecl PROCESS_ConsiderTranslationZone(TRANSLATION_ZONE_T eTranslationZone, IPOINT_T Mouse, LONG lClientRight, LONG lClientBottom, USHORT usThreshold, LOG_T* p_Log) {
    UINT8 ubShouldTranslate = 0;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    switch (eTranslationZone)
    {
    case TRANSLATION_ZONE_TOP_LEFT: {
        ubShouldTranslate = Mouse.iX <= usThreshold && Mouse.iY <= usThreshold;
        break;
    }
    case TRANSLATION_ZONE_TOP: {
        ubShouldTranslate = Mouse.iY <= usThreshold;
        break;
    }
    case TRANSLATION_ZONE_TOP_RIGHT: {
        ubShouldTranslate = Mouse.iX >= lClientRight - usThreshold && Mouse.iY <= usThreshold;
        break;
    }
    case TRANSLATION_ZONE_RIGHT: {
        ubShouldTranslate = Mouse.iX >= lClientRight - usThreshold;
        break;
    }
    case TRANSLATION_ZONE_BOTTOM_RIGHT: {
        ubShouldTranslate = Mouse.iX >= lClientRight - usThreshold && Mouse.iY >= lClientBottom - usThreshold;
        break;
    }
    case TRANSLATION_ZONE_BOTTOM: {
        ubShouldTranslate = Mouse.iY >= lClientBottom - usThreshold;
        break;
    }
    case TRANSLATION_ZONE_BOTTOM_LEFT: {
        ubShouldTranslate = Mouse.iX <= usThreshold && Mouse.iY >= lClientBottom - usThreshold;
        break;
    }
    case TRANSLATION_ZONE_LEFT: {
        ubShouldTranslate = Mouse.iX <= usThreshold;
        break;
    }
    default: {
        MISC_WriteOut(p_Log, LOG_SEVERITY_WARNING, "PROCESS_ConsiderTranslationZone(): Unknown case.\n");
    }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return ubShouldTranslate;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Advances the translation by the step amount or resets the difference if there is excess.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FDELTA_T __cdecl PROCESS_CaptureTranslationAmount(TRANSLATION_ZONE_T eTranslationZone,
    FDELTA_T fCurrentTranslation,
    float fTranslationStepAmount,
    LONG lBmWidth,
    LONG lBmHeight,
    LONG lClientRight,
    LONG lClientBottom,
    USHORT usThreshold,
    LOG_T* p_Log) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FDELTA_T AllowedTranslation = { 0.0f, 0.0f };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    switch (eTranslationZone)
    {
    case TRANSLATION_ZONE_TOP_LEFT: {
        if (fCurrentTranslation.fDx < 0.0f) {
            AllowedTranslation.fDx = fTranslationStepAmount;
        }
        else {
            AllowedTranslation.fDx = -fCurrentTranslation.fDx;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (fCurrentTranslation.fDy < 0.0f) {
            AllowedTranslation.fDy = fTranslationStepAmount;
        }
        else {
            AllowedTranslation.fDy = -fCurrentTranslation.fDy;
        }
        break;
    }
    case TRANSLATION_ZONE_TOP: {
        if (fCurrentTranslation.fDy < 0.0f) {
            AllowedTranslation.fDy = fTranslationStepAmount;
        }
        else {
            AllowedTranslation.fDy = -fCurrentTranslation.fDy;
        }
        break;
    }
    case TRANSLATION_ZONE_TOP_RIGHT: {
        if (fCurrentTranslation.fDx > -(lBmWidth - lClientRight)) {
            AllowedTranslation.fDx = -fTranslationStepAmount;
        }
        else {
            AllowedTranslation.fDx = 0.0f - (fCurrentTranslation.fDx + (lBmWidth - lClientRight));
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (fCurrentTranslation.fDy < 0.0f) {
            AllowedTranslation.fDy = fTranslationStepAmount;
        }
        else {
            AllowedTranslation.fDy = -fCurrentTranslation.fDy;
        }
        break;
    }
    case TRANSLATION_ZONE_RIGHT: {
        if (fCurrentTranslation.fDx > -(lBmWidth - lClientRight)) {
            AllowedTranslation.fDx = -fTranslationStepAmount;
        }
        else {
            AllowedTranslation.fDx = 0.0f - (fCurrentTranslation.fDx + (lBmWidth - lClientRight));
        }
        break;
    }
    case TRANSLATION_ZONE_BOTTOM_RIGHT: {
        if (fCurrentTranslation.fDx > -(lBmWidth - lClientRight)) {
            AllowedTranslation.fDx = -fTranslationStepAmount;
        }
        else {
            AllowedTranslation.fDx = 0.0f - (fCurrentTranslation.fDx + (lBmWidth - lClientRight));
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (fCurrentTranslation.fDy > -(lBmHeight - lClientBottom)) {
            AllowedTranslation.fDy = -fTranslationStepAmount;
        }
        else {
            AllowedTranslation.fDy = 0.0f - (fCurrentTranslation.fDy + (lBmHeight - lClientBottom));
        }
        break;
    }
    case TRANSLATION_ZONE_BOTTOM: {
        if (fCurrentTranslation.fDy > -(lBmHeight - lClientBottom)) {
            AllowedTranslation.fDy = -fTranslationStepAmount;
        }
        else {
            AllowedTranslation.fDy = 0.0f - (fCurrentTranslation.fDy + (lBmHeight - lClientBottom));
        }
        break;
    }
    case TRANSLATION_ZONE_BOTTOM_LEFT: {
        if (fCurrentTranslation.fDx < 0.0f) {
            AllowedTranslation.fDx = fTranslationStepAmount;
        }
        else {
            AllowedTranslation.fDx = -fCurrentTranslation.fDx;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (fCurrentTranslation.fDy > -(lBmHeight - lClientBottom)) {
            AllowedTranslation.fDy = -fTranslationStepAmount;
        }

        else {
            AllowedTranslation.fDy = 0.0f - (fCurrentTranslation.fDy + (lBmHeight - lClientBottom));
        }
        break;
    }
    case TRANSLATION_ZONE_LEFT: {
        if (fCurrentTranslation.fDx < 0.0f) {
            AllowedTranslation.fDx = fTranslationStepAmount;
        }
        else {
            AllowedTranslation.fDx = -fCurrentTranslation.fDx;
        }
        break;
    }
    default: {
        MISC_WriteOut(p_Log, LOG_SEVERITY_WARNING, "PROCESS_CaptureTranslationAmount(): Unknown case.\n");
    }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return AllowedTranslation;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TRANSLATION_ZONE_T __cdecl PROCESS_SafeToTranslation(USHORT usValue) {
    if (usValue >= TRANSLATION_ZONE_TOP_LEFT && usValue < TRANSLATION_ZONE_COUNT) {
        return (TRANSLATION_ZONE_T)usValue;
    }
    else {
        return TRANSLATION_ZONE_UNKNOWN;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_CaptureBorderTranslations(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, MENU_T* p_Menu, SETTINGS_T* p_Settings, LOG_T* p_Log) {
    if (p_Menu->ubEnableTranslations) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        float fAmount = p_Settings->fTranslationStepAmount;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        LONG lBmWidth = p_Globals->TerrainCombinedSize.ulDx;
        LONG lBmHeight = p_Globals->TerrainCombinedSize.ulDy;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        RECT ClientArea = p_Renderer->GetClientArea(p_Renderer);
        LONG lClientRight = ClientArea.right;
        LONG lClientBottom = ClientArea.bottom;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        USHORT usThreshold = p_Globals->usThreshold;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        UINT8 ubI = 0;
        for (ubI = 0; ubI < TRANSLATION_ZONE_COUNT; ubI++) {
            TRANSLATION_ZONE_T eTranslationZone = PROCESS_SafeToTranslation(ubI);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (PROCESS_ConsiderTranslationZone(eTranslationZone, p_Globals->Mouse, lClientRight, lClientBottom, usThreshold, p_Log)) {
                p_Renderer->CapturedTranslationsBorder = PROCESS_CaptureTranslationAmount(eTranslationZone, p_Globals->Translation, fAmount, lBmWidth, lBmHeight, lClientRight, lClientBottom, usThreshold, p_Log);
                break;
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FDELTA_T __cdecl PROCESS_ApplyTranslations(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, CONSTANTS_T* p_Constants) {
    FDELTA_T CapturedTranslationsTotal = {
        p_Renderer->CapturedTranslationsBorder.fDx + p_Renderer->CapturedTranslationsMinimap.fDx + p_Renderer->CapturedTranslationsKeys.fDx,
        p_Renderer->CapturedTranslationsBorder.fDy + p_Renderer->CapturedTranslationsMinimap.fDy + p_Renderer->CapturedTranslationsKeys.fDy,
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Don't process translations if there's no delta.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (CapturedTranslationsTotal.fDx != 0.0f || CapturedTranslationsTotal.fDy != 0.0f) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->Translation.fDx += CapturedTranslationsTotal.fDx;
        p_Globals->Translation.fDy += CapturedTranslationsTotal.fDy;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Apply translations to all terrain tiles.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        UINT uiX, uiY, uiRowOffset, uiIndex;
        for (uiY = 0; uiY < p_Constants->uiTerrainTilesY; uiY++) {
            uiRowOffset = uiY * p_Constants->uiTerrainTilesX;
            for (uiX = 0; uiX < p_Constants->uiTerrainTilesX; uiX++) {
                uiIndex = uiRowOffset + uiX;
                p_Assets->Terrain[uiIndex].Location.fX += CapturedTranslationsTotal.fDx;
                p_Assets->Terrain[uiIndex].Location.fY += CapturedTranslationsTotal.fDy;
            }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Incase we are selecting entities (via mouse drag) and we enter the translation border.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->MouseOld.iX += CapturedTranslationsTotal.fDx;
        p_Globals->MouseOld.iY += CapturedTranslationsTotal.fDy;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_T* p_Entity = p_Globals->p_RootEntity;
        while (p_Entity) {
            p_Entity->Location.fX += CapturedTranslationsTotal.fDx;
            p_Entity->Location.fY += CapturedTranslationsTotal.fDy;
            p_Entity->CenterPoint.fX += CapturedTranslationsTotal.fDx;
            p_Entity->CenterPoint.fY += CapturedTranslationsTotal.fDy;
            p_Entity->MinorDestinationCenterPoint.fX += CapturedTranslationsTotal.fDx;
            p_Entity->MinorDestinationCenterPoint.fY += CapturedTranslationsTotal.fDy;
            p_Entity->MajorDestinationCenterPoint.fX += CapturedTranslationsTotal.fDx;
            p_Entity->MajorDestinationCenterPoint.fY += CapturedTranslationsTotal.fDy;
            p_Entity = (ENTITY_T*)p_Entity->p_Next;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        MESSAGE_T* p_Message = p_Globals->p_RootMessage;
        while (p_Message) {
            p_Message->Location.fX += CapturedTranslationsTotal.fDx;
            p_Message->Location.fY += CapturedTranslationsTotal.fDy;
            p_Message = (MESSAGE_T*)p_Message->p_Next;
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return CapturedTranslationsTotal;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_ResetTranslations(RENDERER_T* p_Renderer) {
    FDELTA_T Zero = { 0.0f, 0.0f };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->CapturedTranslationsBorder = Zero;
    p_Renderer->CapturedTranslationsMinimap = Zero;
    p_Renderer->CapturedTranslationsKeys = Zero;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_UpdateAnimations(GLOBALS_T* p_Globals) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // This function determines if the picture of all the entities needs to be updated.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->uiAnimateCount++;
    if (p_Globals->uiAnimateCount >= p_Globals->fAnimationTick) {
        p_Globals->ubAnimate = 1;
        p_Globals->uiAnimateCount = 0;
    }
    else {
        p_Globals->ubAnimate = 0;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Re-render the frame.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_PrepareFrame(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, MENU_T* p_Menu, CONSTANTS_T* p_Constants) {
    RECT ClientArea = p_Renderer->GetClientArea(p_Renderer);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UINT uiTileIndex = 0,
        uiCurrentTileIndexX = 0,
        uiCurrentTileIndexY = 0;
    UIPOINT_T CurrentTilePoint = { 0 },
        NextTilePoint = { 0 };
    UIDELTA_T CurrentTranslation = {
        -p_Globals->Translation.fDx,
        -p_Globals->Translation.fDy
    }, RelativeTileTranslation = {
        CurrentTranslation.uiDx,
        CurrentTranslation.uiDy
    }, CurrentClientViewport = {
        CurrentTranslation.uiDx + ClientArea.right,
        CurrentTranslation.uiDy + ClientArea.bottom
    }, TerrainTileSize = {
        p_Globals->TerrainTileSize.uiDx,
        p_Globals->TerrainTileSize.uiDy
    }, CropLimit = { 0 };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T CropToClientArea = { 0.0f }, CropFromBitmap = { 0.0f };
    FDELTA_T CropDelta = { 0.0f };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Optimize with:
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // - Last known tile positions as starting point: this could be indexable by the viewport translation with some arithmetic
    // since the tile size is constant.
    // - Combine iterations with minimap terrain rendering.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (; CurrentTilePoint.uiY < CurrentClientViewport.uiDy; CurrentTilePoint.uiY += TerrainTileSize.uiDy, uiCurrentTileIndexY++) {
        NextTilePoint.uiY = CurrentTilePoint.uiY + TerrainTileSize.uiDy;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // We're not in this vertical tile; we could be in the next.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (CurrentTranslation.uiDy >= NextTilePoint.uiY) {
            RelativeTileTranslation.uiDy = CurrentTranslation.uiDy - NextTilePoint.uiY;
            continue;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Determine terrain bitmap Y.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        CropFromBitmap.fY = RelativeTileTranslation.uiDy;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Determine terrain bitmap Y delta.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if ((UINT)ClientArea.bottom <= TerrainTileSize.uiDy) {
            CropLimit.uiDy = ClientArea.bottom;
        }
        else {
            CropLimit.uiDy = TerrainTileSize.uiDy - CropFromBitmap.fY;
        }
        CropDelta.fDy = CropLimit.uiDy;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (; CurrentTilePoint.uiX < CurrentClientViewport.uiDx; CurrentTilePoint.uiX += TerrainTileSize.uiDx, uiCurrentTileIndexX++) {
            NextTilePoint.uiX = CurrentTilePoint.uiX + TerrainTileSize.uiDx;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // We're not in this horizontal tile; we could be in the next.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (CurrentTranslation.uiDx >= NextTilePoint.uiX) {
                RelativeTileTranslation.uiDx = CurrentTranslation.uiDx - NextTilePoint.uiX;
                continue;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Determine terrain bitmap X.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            CropFromBitmap.fX = RelativeTileTranslation.uiDx;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Determine terrain bitmap X delta.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if ((UINT)ClientArea.right <= TerrainTileSize.uiDx) {
                CropLimit.uiDx = ClientArea.right;
            }
            else {
                CropLimit.uiDx = TerrainTileSize.uiDx - CropFromBitmap.fX;
            }
            CropDelta.fDx = CropLimit.uiDx;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            uiTileIndex = uiCurrentTileIndexY * p_Constants->uiTerrainTilesX + uiCurrentTileIndexX;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Renderer->CropDrawPictureAt(p_Renderer, &p_Assets->Terrain[uiTileIndex], CropToClientArea, CropDelta, CropFromBitmap, 0);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Render grid and diagnostics, if enabled.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Menu->ubDrawTerrainGrid) {
                float fPadding = 17.0f;
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                FPOINT_T ClientFromX = {
                    CropToClientArea.fX,
                    CropToClientArea.fY
                }, ClientToX = {
                    ClientFromX.fX + CropDelta.fDx,
                    ClientFromX.fY
                }, ClientFromY = {
                    CropToClientArea.fX,
                    CropToClientArea.fY
                }, ClientToY = {
                    CropToClientArea.fX,
                    CropToClientArea.fY + CropDelta.fDy
                }, Line = {
                    ClientFromX.fX + fPadding,
                    ClientFromY.fY + fPadding
                };
                PROCESS_DrawTerrainGrid(p_Renderer, ClientFromX, ClientToX, p_Assets, p_Globals, p_Menu);
                PROCESS_DrawTerrainGrid(p_Renderer, ClientFromY, ClientToY, p_Assets, p_Globals, p_Menu);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                p_Renderer->SetBlitter(p_Renderer, &(p_Assets->Blitter[BLITTER_COLOR_ORANGE]));
                snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "I: (%u, %u)", uiCurrentTileIndexX, uiCurrentTileIndexY);
                p_Renderer->Blitter(p_Renderer, p_Renderer->szBlitter, Line, p_Menu->ubEnableMasking);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                Line.fY += fPadding;
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////                
                p_Renderer->SetBlitter(p_Renderer, &(p_Assets->Blitter[BLITTER_COLOR_YELLOW]));
                snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "V: (%0.2f, %0.2f)", ClientFromX.fX, ClientFromY.fY);
                p_Renderer->Blitter(p_Renderer, p_Renderer->szBlitter, Line, p_Menu->ubEnableMasking);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                Line.fY += fPadding;
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                p_Renderer->SetBlitter(p_Renderer, &(p_Assets->Blitter[BLITTER_COLOR_LIGHT_BLUE]));
                snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "A: (%u, %u)", CurrentTilePoint.uiX, CurrentTilePoint.uiY);
                p_Renderer->Blitter(p_Renderer, p_Renderer->szBlitter, Line, p_Menu->ubEnableMasking);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                Line.fY += fPadding;
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                p_Renderer->SetBlitter(p_Renderer, &(p_Assets->Blitter[BLITTER_COLOR_GREEN]));
                snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "S: (%u, %u)", TerrainTileSize.uiDx, TerrainTileSize.uiDy);
                p_Renderer->Blitter(p_Renderer, p_Renderer->szBlitter, Line, p_Menu->ubEnableMasking);
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Prep next X tile.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            CropFromBitmap.fX = 0.0f;
            RelativeTileTranslation.uiDx = 0;
            CropToClientArea.fX += CropDelta.fDx; // Advance viewport by remaining or the tile size.
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Prep next Y and X tile.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        CurrentTilePoint.uiX = 0;
        uiCurrentTileIndexX = 0;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        CropFromBitmap.fX = CurrentTranslation.uiDx;
        CropFromBitmap.fY = 0.0f;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        RelativeTileTranslation.uiDy = 0;
        RelativeTileTranslation.uiDx = CurrentTranslation.uiDx;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        CropToClientArea.fX = 0.0f;
        CropToClientArea.fY += CropDelta.fDy; // Advance viewport by remaining or the tile size.
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// If the view has remained static since the last frame, we can be selective in what we re-render.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_HandlePreProcessing(RENDERER_T* p_Renderer, MENU_T* p_Menu, ASSETS_T* p_Assets, GLOBALS_T* p_Globals, LOG_T* p_Log, CONSTANTS_T* p_Constants) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Process level:
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_Renderer->ubIsFullFrameRender) {
        PROCESS_PrepareFrame(p_Renderer, p_Globals, p_Assets, p_Menu, p_Constants);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Dirty zones from last frame.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        PROCESS_DrawDirtyZones(p_Renderer, p_Globals, p_Assets, p_Menu);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Renderer level:
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->HandlePreProcessing(p_Renderer, p_Menu, p_Assets, p_Globals, p_Log);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_ProcessEntities(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, MENU_T* p_Menu, LOG_T* p_Log, SETTINGS_T* p_Settings, CONSTANTS_T* p_Constants) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ENTITY_T* p_Current = p_Globals->p_RootEntity;
    while (p_Current) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        switch (p_Current->eType) {
        case ENTITY_TYPE_WORKER: {
            if (p_Menu->ubToggleSimulation) {
                if (p_Current->p_OperatingTarget) {
                    AI_HandleWorkers(p_Current, p_Globals, p_Log, p_Menu, p_Constants, p_Settings);
                }
            }
            break;
        }
        case ENTITY_TYPE_MINERAL: {
            break;
        }
        case ENTITY_TYPE_COMMAND: {
            break;
        }
        case ENTITY_TYPE_SUPPLY: {
            break;
        }
        case ENTITY_TYPE_REFINERY: {
            break;
        }
        default: {
            MISC_WriteOut(p_Log, LOG_SEVERITY_WARNING, "PROCESS_ProcessEntities(): Unknown entity type.\n");
        }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // This entity is being interacted with.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Current->ubIsPaused) {
            p_Current->fPauseCount += 1.0f;
            if (p_Current->fPauseCount >= p_Current->fPauseTime) {
                p_Current->ubIsPaused = 0;
                p_Current->fPauseCount = 0.0f;
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // The command center is no longer being interacted with.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                switch (p_Current->eType) {
                case ENTITY_TYPE_COMMAND: {
                    p_Current->usState = ANIMATION_STATE_COMMAND_CREATING;
                    break;
                }
                case ENTITY_TYPE_WORKER: {
                    break;
                }
                case ENTITY_TYPE_MINERAL: {
                    break;
                }
                case ENTITY_TYPE_SUPPLY: {
                    break;
                }
                case ENTITY_TYPE_REFINERY: {
                    break;
                }
                default: {
                    MISC_WriteOut(p_Log, LOG_SEVERITY_WARNING, "PROCESS_ProcessEntities(): Unknown entity type.\n");
                }
                }
            }
        }
        else {
            if (p_Current->ubIsInMotion) {
                PATH_UpdatePosition(p_Renderer, p_Current, p_Globals, p_Menu, p_Settings, p_Constants, p_Log);
            }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (PROCESS_ConsiderFrustum(p_Current, PROCESS_TYPE_ENTITY, p_Renderer, p_Log)) {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Menu->ubDrawDirtyZones | !p_Renderer->ubIsFullFrameRender) {
                p_Renderer->PrepareDirtyZone(p_Renderer,
                    p_Renderer->CaptureDirtyZone(p_Renderer, p_Current, p_Settings, p_Constants), p_Globals, p_Log);
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Menu->ubDrawStatuses) {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                if (p_Current->ubIsPaused) {
                    p_Renderer->DrawEntityEllipse(p_Renderer, p_Current, p_Assets->hPenWhite, p_Assets->hBrush100r0g0b, p_Settings);
                }
                else if (!p_Current->ubIsAlive) {
                    p_Renderer->DrawEntityEllipse(p_Renderer, p_Current, p_Assets->hPenWhite, p_Assets->hBrush0r100g0b, p_Settings);
                }
                else if (!p_Current->p_Next) {
                    p_Renderer->DrawEntityEllipse(p_Renderer, p_Current, p_Assets->hPenWhite, p_Assets->hBrush0r0g100b, p_Settings);
                }
                else if (p_Current == p_Globals->p_RootEntity) {
                    p_Renderer->DrawEntityEllipse(p_Renderer, p_Current, p_Assets->hPenWhite, p_Assets->hBrush100r100g0b, p_Settings);
                }
                else if (p_Current->ubIsCarryingMinerals || p_Current->ubIsCarryingGas) {
                    p_Renderer->DrawEntityEllipse(p_Renderer, p_Current, p_Assets->hPenWhite, p_Assets->hBrush0r100g100b, p_Settings);
                }
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Entity highlighting has priority, with regards to statuses.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (PATH_WithinPoint(p_Current, (FPOINT_T) { p_Globals->Mouse.iX, p_Globals->Mouse.iY })) {
                p_Current->ubIsHighlighted = 1;
                p_Renderer->DrawEntityEllipse(p_Renderer, p_Current, p_Assets->hPenWhite, p_Assets->hBrush100r0g100b, p_Settings);
            }
            else {
                p_Current->ubIsHighlighted = 0;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Menu->ubDrawMinorVectors) {
                p_Renderer->DrawEntityMinorVector(p_Renderer, p_Current, p_Assets);
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Menu->ubDrawMajorVectors) {
                p_Renderer->DrawEntityMajorVector(p_Renderer, p_Current, p_Assets);
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Entity selection has priority.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Current->ubIsSelected) {
                p_Renderer->DrawEntityEllipse(p_Renderer, p_Current, p_Assets->hPenWhite, p_Assets->hBrush100r100g100b, p_Settings);
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Menu->ubDrawResources) {
                p_Renderer->SetBlitter(p_Renderer, &(p_Assets->Blitter[BLITTER_COLOR_LIGHT_BLUE]));
                snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "%d", p_Current->iMineralCount);
                p_Renderer->Blitter(p_Renderer, p_Renderer->szBlitter, (FPOINT_T) { p_Current->Location.fX, p_Current->Location.fY - 20.0f }, p_Menu->ubEnableMasking);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                p_Renderer->SetBlitter(p_Renderer, &(p_Assets->Blitter[BLITTER_COLOR_GREEN]));
                snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "%d", p_Current->iGasCount);
                p_Renderer->Blitter(p_Renderer, p_Renderer->szBlitter, (FPOINT_T) { p_Current->Location.fX, p_Current->Location.fY - 40.0f }, p_Menu->ubEnableMasking);
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Menu->ubDrawIds) {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                p_Renderer->SetBlitter(p_Renderer, &(p_Assets->Blitter[BLITTER_COLOR_ORANGE]));
                snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "%hu", p_Current->usId);
                p_Renderer->Blitter(p_Renderer, p_Renderer->szBlitter, (FPOINT_T) { p_Current->Location.fX, p_Current->Location.fY - 60.0f }, p_Menu->ubEnableMasking);
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Update the picture.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Globals->ubAnimate) {
                ENTITY_HandleAnimation(p_Current, p_Assets, p_Settings, p_Log);
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Renderer->DrawEntity(p_Renderer, p_Current, p_Menu->ubEnableMasking);
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Current = (ENTITY_T*)p_Current->p_Next;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_ProcessMessages(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, MENU_T* p_Menu, CONSTANTS_T* p_Constants, LOG_T* p_Log) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MESSAGE_T* p_Current = p_Globals->p_RootMessage;
    if (p_Current) {
        MESSAGE_T* p_Dead = NULL;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Menu->ubDrawDirtyZones | !p_Renderer->ubIsFullFrameRender) {
            FRECT_T GlobalMessageZone = {
                .Location = {.fX = 15.0f, .fY = 15.0f },
                .Size = {.fDx = 240.0f, .fDy = 85.0f }
            };
            p_Renderer->PrepareDirtyZone(p_Renderer, GlobalMessageZone, p_Globals, p_Log);
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        while (p_Current) {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (PROCESS_ConsiderFrustum(p_Current, PROCESS_TYPE_MESSAGE, p_Renderer, p_Log)) {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                switch (p_Current->eType) {
                case MESSAGE_TYPE_GAIN_MINERALS: {
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////
                    p_Renderer->SetBlitter(p_Renderer, &(p_Assets->Blitter[BLITTER_COLOR_LIGHT_BLUE]));
                    break;
                }
                case MESSAGE_TYPE_BONUS_MINERALS: {
                    p_Renderer->ApplyWorldTransform(p_Renderer, TRANSFORM_TYPE_SCALE, p_Constants, p_Current->fScale, p_Current->Location, p_Log);
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////
                    p_Renderer->SetBlitter(p_Renderer, &(p_Assets->Blitter[BLITTER_COLOR_LIGHT_BLUE]));
                    break;
                }
                case MESSAGE_TYPE_GAIN_GAS: {
                    p_Renderer->SetBlitter(p_Renderer, &(p_Assets->Blitter[BLITTER_COLOR_GREEN]));
                    break;
                }
                case MESSAGE_TYPE_BONUS_GAS: {
                    p_Renderer->ApplyWorldTransform(p_Renderer, TRANSFORM_TYPE_SCALE, p_Constants, p_Current->fScale, p_Current->Location, p_Log);
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////
                    p_Renderer->SetBlitter(p_Renderer, &(p_Assets->Blitter[BLITTER_COLOR_GREEN]));
                    break;
                }
                case MESSAGE_TYPE_GENERAL_WARNING: {
                    p_Renderer->SetBlitter(p_Renderer, &(p_Assets->Blitter[BLITTER_COLOR_YELLOW]));
                    break;
                }
                default: {
                    MISC_WriteOut(p_Log, LOG_SEVERITY_WARNING, "PROCESS_ProcessMessages(): Unknown message type.\n");
                }
                }
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                p_Renderer->Blitter(p_Renderer, p_Current->szMessage, (FPOINT_T) { p_Current->Location.fX, p_Current->Location.fY }, p_Menu->ubEnableMasking);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                p_Renderer->ResetWorldTransform(p_Renderer);
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // If this particular call is updating the entity pictures, update the messages as well.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Globals->ubAnimate) {
                p_Current->sLifespan--;
                p_Current->Location.fY -= 2.0f;
                p_Current->fScale += 0.075f;
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // This one's for the graveyard.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                if (!p_Current->sLifespan) {
                    p_Dead = p_Current;
                    MESSAGE_DeleteSpecific(p_Dead, p_Globals);
                }
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Current = (MESSAGE_T*)p_Current->p_Next;
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_RenderDiagnostic(FPOINT_T* Location, RENDERER_T* p_Renderer, UINT8 ubEnableMasking) {
    p_Renderer->Blitter(p_Renderer, p_Renderer->szBlitter, *Location, ubEnableMasking);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawBuildMode(FPOINT_T* Location, RENDERER_T* p_Renderer, UINT8 ubEnableMasking, float fLineAdvance) {
    char szMode[16] = { 0 };
#ifdef DEBUG
    snprintf(szMode, sizeof(szMode), "Debug");
#elif RELEASE
    snprintf(szMode, sizeof(szMode), "Release");
#else
    snprintf(szMode, sizeof(szMode), "Unknown");
#endif
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "%s (%s @ %s)", szMode, __DATE__, __TIME__);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_RenderDiagnostic(Location, p_Renderer, ubEnableMasking);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Location->fY -= fLineAdvance;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawTranslations(FPOINT_T* Location, RENDERER_T* p_Renderer, UINT8 ubEnableMasking, GLOBALS_T* p_Globals, float fLineAdvance) {
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "(Tx, Ty): (%0.2f, %0.2f)", p_Globals->Translation.fDx, p_Globals->Translation.fDy);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_RenderDiagnostic(Location, p_Renderer, ubEnableMasking);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Location->fY -= fLineAdvance;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawMouseCoordinates(FPOINT_T* Location, RENDERER_T* p_Renderer, UINT8 ubEnableMasking, GLOBALS_T* p_Globals, float fLineAdvance) {
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "(Mx, My): (%d, %d)", p_Globals->Mouse.iX, p_Globals->Mouse.iY);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_RenderDiagnostic(Location, p_Renderer, ubEnableMasking);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Location->fY -= fLineAdvance;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawSelectionCapture(FPOINT_T* Location, RENDERER_T* p_Renderer, UINT8 ubEnableMasking, GLOBALS_T* p_Globals, float fLineAdvance) {
    int iCaptureWidth = 0;
    int iCaptureHeight = 0;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Prevent initial width and height to be the initial mouse down coordinates. Translations are considered already so
    // this can be a simple subtraction. This also is in the minimap logic.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_Globals->CaptureCurrent.iX > 0) {
        iCaptureWidth = p_Globals->CaptureCurrent.iX - p_Globals->CaptureStart.iX;
    }
    if (p_Globals->CaptureCurrent.iY > 0) {
        iCaptureHeight = p_Globals->CaptureCurrent.iY - p_Globals->CaptureStart.iY;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "(Sw, Sh): (%d, %d)", iCaptureWidth, iCaptureHeight);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_RenderDiagnostic(Location, p_Renderer, ubEnableMasking);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Location->fY -= fLineAdvance;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "(Sx, Sy): (%d, %d) -> (%d, %d)",
        p_Globals->CaptureStart.iX, p_Globals->CaptureStart.iY, p_Globals->CaptureCurrent.iX, p_Globals->CaptureCurrent.iY);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_RenderDiagnostic(Location, p_Renderer, ubEnableMasking);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Location->fY -= fLineAdvance;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawClientSize(FPOINT_T* Location, RENDERER_T* p_Renderer, UINT8 ubEnableMasking, float fLineAdvance) {
    RECT ClientArea = p_Renderer->GetClientArea(p_Renderer);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "R/B: (%ld, %ld)", ClientArea.right, ClientArea.bottom);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_RenderDiagnostic(Location, p_Renderer, ubEnableMasking);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Location->fY -= fLineAdvance;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawEngineFrameTime(FPOINT_T* Location, RENDERER_T* p_Renderer, UINT8 ubEnableMasking, GLOBALS_T* p_Globals, float fLineAdvance) {
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "E(ft): %0.2f ms", p_Globals->fEngineTime * 1000.0f);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_RenderDiagnostic(Location, p_Renderer, ubEnableMasking);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Location->fY -= fLineAdvance;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawEngineFps(FPOINT_T* Location, RENDERER_T* p_Renderer, UINT8 ubEnableMasking, GLOBALS_T* p_Globals, SETTINGS_T* p_Settings, float fLineAdvance) {
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "E(fps/a): %0.2f", p_Globals->fFpsLongTermAverage);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_RenderDiagnostic(Location, p_Renderer, ubEnableMasking);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Location->fY -= fLineAdvance;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "E(fps/x): %0.2f", p_Settings->fEngineFpsTarget);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_RenderDiagnostic(Location, p_Renderer, ubEnableMasking);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Location->fY -= fLineAdvance;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "A(fps/x): %0.2f", p_Settings->fAnimateFpsTarget);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_RenderDiagnostic(Location, p_Renderer, ubEnableMasking);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Location->fY -= fLineAdvance;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawUptime(FPOINT_T* Location, RENDERER_T* p_Renderer, UINT8 ubEnableMasking, GLOBALS_T* p_Globals, float fLineAdvance) {
    char szFormattedTime[32];
    MISC_FormatTime(p_Globals->ulSecondsTick, szFormattedTime, sizeof(szFormattedTime));
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "U: %s", szFormattedTime);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_RenderDiagnostic(Location, p_Renderer, ubEnableMasking);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Location->fY -= fLineAdvance;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawHeapAllocations(FPOINT_T* Location, RENDERER_T* p_Renderer, UINT8 ubEnableMasking, GLOBALS_T* p_Globals, float fLineAdvance) {
    char szHeap[32] = { 0 };
    MISC_FormatUnsignedWithCommas(szHeap, sizeof(szHeap), p_Globals->stAllocations);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "H: %s B", szHeap);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_RenderDiagnostic(Location, p_Renderer, ubEnableMasking);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Location->fY -= fLineAdvance;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawWorkersSelected(FPOINT_T* Location, RENDERER_T* p_Renderer, UINT8 ubEnableMasking, GLOBALS_T* p_Globals, float fLineAdvance) {
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "W(s): %hu", ENTITY_GetSelectedEntityCounts(p_Globals).usSelectedWorkersCount);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_RenderDiagnostic(Location, p_Renderer, ubEnableMasking);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Location->fY -= fLineAdvance;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawEntitiesSelected(FPOINT_T* Location, RENDERER_T* p_Renderer, UINT8 ubEnableMasking, GLOBALS_T* p_Globals, float fLineAdvance) {
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "E(s): %hu", ENTITY_GetSelectedEntityCounts(p_Globals).usSelectedAllCount);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_RenderDiagnostic(Location, p_Renderer, ubEnableMasking);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Location->fY -= fLineAdvance;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawAllocators(FPOINT_T* Location, RENDERER_T* p_Renderer, UINT8 ubEnableMasking, GLOBALS_T* p_Globals, float fLineAdvance) {
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "A(e-): %zu-%zu=%zu ^%zu",
        p_Globals->p_EntityAllocator->stCapacity,
        p_Globals->p_EntityAllocator->stAllocCount,
        p_Globals->p_EntityAllocator->stFreeCount,
        p_Globals->p_EntityAllocator->stPeakAllocCount
    );
    PROCESS_RenderDiagnostic(Location, p_Renderer, ubEnableMasking);
    Location->fY -= fLineAdvance;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "A(m-): %zu-%zu=%zu ^%zu",
        p_Globals->p_MessagesAllocator->stCapacity,
        p_Globals->p_MessagesAllocator->stAllocCount,
        p_Globals->p_MessagesAllocator->stFreeCount,
        p_Globals->p_MessagesAllocator->stPeakAllocCount
    );
    PROCESS_RenderDiagnostic(Location, p_Renderer, ubEnableMasking);
    Location->fY -= fLineAdvance;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "A(o-): %zu-%zu=%zu ^%zu",
        p_Globals->p_PathObstacleAllocator->stCapacity,
        p_Globals->p_PathObstacleAllocator->stAllocCount,
        p_Globals->p_PathObstacleAllocator->stFreeCount,
        p_Globals->p_PathObstacleAllocator->stPeakAllocCount
    );
    PROCESS_RenderDiagnostic(Location, p_Renderer, ubEnableMasking);
    Location->fY -= fLineAdvance;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "A(p-): %zu-%zu=%zu ^%zu",
        p_Globals->p_PathPointAllocator->stCapacity,
        p_Globals->p_PathPointAllocator->stAllocCount,
        p_Globals->p_PathPointAllocator->stFreeCount,
        p_Globals->p_PathPointAllocator->stPeakAllocCount
    );
    PROCESS_RenderDiagnostic(Location, p_Renderer, ubEnableMasking);
    Location->fY -= fLineAdvance;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "A(c-): %zu-%zu=%zu ^%zu",
        p_Globals->p_AiClosestAllocator->stCapacity,
        p_Globals->p_AiClosestAllocator->stAllocCount,
        p_Globals->p_AiClosestAllocator->stFreeCount,
        p_Globals->p_AiClosestAllocator->stPeakAllocCount
    );
    PROCESS_RenderDiagnostic(Location, p_Renderer, ubEnableMasking);
    Location->fY -= fLineAdvance;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "A(fP): %zu-%zu=%zu ^%zu",
        p_Globals->p_fPointAllocator->stCapacity,
        p_Globals->p_fPointAllocator->stAllocCount,
        p_Globals->p_fPointAllocator->stFreeCount,
        p_Globals->p_fPointAllocator->stPeakAllocCount
    );
    PROCESS_RenderDiagnostic(Location, p_Renderer, ubEnableMasking);
    Location->fY -= fLineAdvance;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "A(fR): %zu-%zu=%zu ^%zu",
        p_Globals->p_fRectAllocator->stCapacity,
        p_Globals->p_fRectAllocator->stAllocCount,
        p_Globals->p_fRectAllocator->stFreeCount,
        p_Globals->p_fRectAllocator->stPeakAllocCount
    );
    PROCESS_RenderDiagnostic(Location, p_Renderer, ubEnableMasking);
    Location->fY -= fLineAdvance;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawTranslationThreshold(RENDERER_T* p_Renderer, ASSETS_T* p_Assets, GLOBALS_T* p_Globals, MENU_T* p_Menu) {
    if (p_Menu->ubDrawTranslationThreshold) {
        p_Renderer->DrawTranslationThreshold(p_Renderer, p_Assets, p_Globals);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawDirtyZones(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, MENU_T* p_Menu) {
    if (p_Menu->ubDrawDirtyZones) {
        p_Renderer->DrawDirtyZones(p_Renderer, p_Assets, p_Globals);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawTerrainGrid(RENDERER_T* p_Renderer, FPOINT_T From, FPOINT_T To, ASSETS_T* p_Assets, GLOBALS_T* p_Globals, MENU_T* p_Menu) {
    p_Renderer->DrawTerrainGrid(p_Renderer, From, To, p_Assets, p_Globals);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawEngineFpsEstimate(RENDERER_T* p_Renderer, FPOINT_T Location, GLOBALS_T* p_Globals, UINT8 ubEnableMasking) {
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "%.0f", p_Globals->fFpsEstimate);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_RenderDiagnostic(&Location, p_Renderer, ubEnableMasking);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Render diagnostics on equally spaced lines starting from an offset and going bottom to top.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_HandleDiagnostics(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, MENU_T* p_Menu, SETTINGS_T* p_Settings) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->ulFrameCount++;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UINT8 ubEnableMasking = p_Menu->ubEnableMasking;
    RECT ClientArea = p_Renderer->GetClientArea(p_Renderer);
    float fLineAdvance = 17.0f;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_Menu->ubDrawStatistics) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Bottom up, above taskbar starting here.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT_T Offset = { 15.0f, ClientArea.bottom - 160.0f };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Renderer->SetBlitter(p_Renderer, &(p_Assets->Blitter[BLITTER_COLOR_PINK]));
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        PROCESS_DrawBuildMode(&Offset, p_Renderer, ubEnableMasking, fLineAdvance);
        PROCESS_DrawTranslations(&Offset, p_Renderer, ubEnableMasking, p_Globals, fLineAdvance);
        PROCESS_DrawMouseCoordinates(&Offset, p_Renderer, ubEnableMasking, p_Globals, fLineAdvance);
        PROCESS_DrawSelectionCapture(&Offset, p_Renderer, ubEnableMasking, p_Globals, fLineAdvance);
        PROCESS_DrawClientSize(&Offset, p_Renderer, ubEnableMasking, fLineAdvance);
        PROCESS_DrawEngineFrameTime(&Offset, p_Renderer, ubEnableMasking, p_Globals, fLineAdvance);
        PROCESS_DrawEngineFps(&Offset, p_Renderer, ubEnableMasking, p_Globals, p_Settings, fLineAdvance);
        PROCESS_DrawUptime(&Offset, p_Renderer, ubEnableMasking, p_Globals, fLineAdvance);
        PROCESS_DrawHeapAllocations(&Offset, p_Renderer, ubEnableMasking, p_Globals, fLineAdvance);
        PROCESS_DrawWorkersSelected(&Offset, p_Renderer, ubEnableMasking, p_Globals, fLineAdvance);
        PROCESS_DrawEntitiesSelected(&Offset, p_Renderer, ubEnableMasking, p_Globals, fLineAdvance);
        PROCESS_DrawAllocators(&Offset, p_Renderer, ubEnableMasking, p_Globals, fLineAdvance);
    }
    PROCESS_DrawTranslationThreshold(p_Renderer, p_Assets, p_Globals, p_Menu);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->SetBlitter(p_Renderer, &(p_Assets->Blitter[BLITTER_COLOR_GREEN]));
    PROCESS_DrawEngineFpsEstimate(p_Renderer, (FPOINT_T) { ClientArea.right - 50, ClientArea.top + fLineAdvance }, p_Globals, ubEnableMasking);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawHarvestedMinerals(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, MENU_T* p_Menu) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Mineral picture.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->DrawPictureAt(p_Renderer, &p_Assets->Mineral[0], (FPOINT_T) { 20.0f, p_Globals->fClientBottomY - 120.0f }, p_Menu->ubEnableMasking);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->SetBlitter(p_Renderer, &(p_Assets->Blitter[BLITTER_COLOR_LIGHT_BLUE]));
    MISC_FormatSignedWithCommas(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), p_Globals->iMineralCount);
    p_Renderer->Blitter(p_Renderer, p_Renderer->szBlitter, (FPOINT_T) { 70.0f, p_Globals->fClientBottomY - 110.0f }, p_Menu->ubEnableMasking);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawHarvestedGas(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, MENU_T* p_Menu) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Gas picture.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->DrawPictureAt(p_Renderer, &p_Assets->Gas[0], (FPOINT_T) { 20.0f, p_Globals->fClientBottomY - 80.0f }, p_Menu->ubEnableMasking);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->SetBlitter(p_Renderer, &(p_Assets->Blitter[BLITTER_COLOR_GREEN]));
    MISC_FormatSignedWithCommas(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), p_Globals->iGasCount);
    p_Renderer->Blitter(p_Renderer, p_Renderer->szBlitter, (FPOINT_T) { 70.0f, p_Globals->fClientBottomY - 70.0f }, p_Menu->ubEnableMasking);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawSupplyAmounts(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, MENU_T* p_Menu) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Supply picture.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->DrawPictureAt(p_Renderer, &p_Assets->Worker[0], (FPOINT_T) { 25.0f, p_Globals->fClientBottomY - 40.0f }, p_Menu->ubEnableMasking);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->SetBlitter(p_Renderer, &(p_Assets->Blitter[BLITTER_COLOR_YELLOW]));
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    char szNumerator[32] = { 0 }, szDenominator[32] = { 0 }, szRemaining[32] = { 0 };
    MISC_FormatSignedWithCommas(szNumerator, sizeof(szNumerator), p_Globals->iCurrentSupply);
    MISC_FormatSignedWithCommas(szDenominator, sizeof(szDenominator), p_Globals->iTotalSupply);
    MISC_FormatSignedWithCommas(szRemaining, sizeof(szRemaining), p_Globals->iTotalSupply - p_Globals->iCurrentSupply);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    snprintf(p_Renderer->szBlitter, sizeof(p_Renderer->szBlitter), "%s/%s (%s)", szNumerator, szDenominator, szRemaining);
    p_Renderer->Blitter(p_Renderer, p_Renderer->szBlitter, (FPOINT_T) { 70.0f, p_Globals->fClientBottomY - 35.0f }, p_Menu->ubEnableMasking);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_HandleResourceBar(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, MENU_T* p_Menu) {
    RECT ClientArea = p_Renderer->GetClientArea(p_Renderer);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->fClientBottomY = ClientArea.bottom;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_T* p_Hud = &p_Assets->HUD[0];
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Resources background.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->DrawPictureAt(p_Renderer, p_Hud, (FPOINT_T) { 0.0f, p_Globals->fClientBottomY - p_Hud->Bitmap.bmHeight }, p_Menu->ubEnableMasking);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_DrawHarvestedMinerals(p_Renderer, p_Globals, p_Assets, p_Menu);
    PROCESS_DrawHarvestedGas(p_Renderer, p_Globals, p_Assets, p_Menu);
    PROCESS_DrawSupplyAmounts(p_Renderer, p_Globals, p_Assets, p_Menu);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawCommandCard(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, CARD_T* p_Card, MENU_T* p_Menu) {
    RECT ClientArea = p_Renderer->GetClientArea(p_Renderer);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Taskbar background.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_T Hud = p_Assets->HUD[1];
    LONG lRightOffset = Hud.Bitmap.bmWidth << 1;
    float fBorderPadding = 15.0f;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->DrawPictureAt(p_Renderer, &Hud, (FPOINT_T) { ClientArea.right - lRightOffset, ClientArea.bottom - Hud.Bitmap.bmHeight }, 0);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Command card.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->DrawPictureAt(p_Renderer, p_Card->p_Picture, (FPOINT_T) { ClientArea.right - lRightOffset + fBorderPadding, ClientArea.bottom - Hud.Bitmap.bmHeight + fBorderPadding }, 0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_HandleMinimap(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, SETTINGS_T* p_Settings, MENU_T* p_Menu, CONSTANTS_T* p_Constants, LOG_T* p_Log) {
    FPOINT_T MinimapOrigin;
    FDELTA_T MinimapDelta;
    PROCESS_PopulateMinimapDimensions(&MinimapOrigin, &MinimapDelta, p_Renderer, p_Assets, p_Settings);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_DrawMinimapTerrain(p_Renderer, p_Globals, p_Assets, p_Constants, MinimapOrigin);
    PROCESS_DrawMinimapEntities(p_Renderer, p_Globals, MinimapOrigin, p_Assets, p_Settings, p_Menu, p_Log);
    PROCESS_DrawMinimapViewport(p_Renderer, p_Globals, p_Assets, MinimapOrigin, p_Settings);
    PROCESS_DrawMinimapSelectionArea(p_Renderer, p_Globals, MinimapOrigin, p_Assets, p_Settings);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawMinimapTerrain(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, CONSTANTS_T* p_Constants, FPOINT_T MinimapOrigin) {
    RECT ClientArea = p_Renderer->GetClientArea(p_Renderer);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Minimap background.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_T Hud = p_Assets->HUD[1];
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->DrawPictureAt(p_Renderer, &Hud, (FPOINT_T) { ClientArea.right - Hud.Bitmap.bmWidth, ClientArea.bottom - Hud.Bitmap.bmHeight }, 0);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UINT uiY, uiX, uiRowOffset;
    UIPOINT_T CurrentTilePoint = { 0 };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (uiY = 0; uiY < p_Constants->uiTerrainTilesY; CurrentTilePoint.uiY += p_Globals->TerrainMinimapTileSize.uiDy, uiY++) {
        uiRowOffset = uiY * p_Constants->uiTerrainTilesX;
        for (uiX = 0; uiX < p_Constants->uiTerrainTilesX; CurrentTilePoint.uiX += p_Globals->TerrainMinimapTileSize.uiDx, uiX++) {
            FPOINT_T Location = {
                MinimapOrigin.fX + CurrentTilePoint.uiX,
                MinimapOrigin.fY + CurrentTilePoint.uiY
            };
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Renderer->DrawPictureAt(p_Renderer, &p_Assets->TerrainMini[uiRowOffset + uiX], Location, 0);
        }
        CurrentTilePoint.uiX = 0;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawMinimapEntities(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, FPOINT_T MinimapOrigin, ASSETS_T* p_Assets, SETTINGS_T* p_Settings, MENU_T* p_Menu, LOG_T* p_Log) {
    p_Renderer->DrawMinimapEntities(p_Renderer, p_Globals, MinimapOrigin, p_Assets, p_Settings, p_Menu, p_Log);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawMinimapViewport(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, FPOINT_T MinimapOrigin, SETTINGS_T* p_Settings) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Render the viewport box.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T ViewportRelativeLocation;
    FDELTA_T ViewportRelativeDelta;
    FPOINT_T ViewportActualLocation;
    FDELTA_T ViewportActualDelta;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_PopulateViewportRelativity(&ViewportRelativeLocation, &ViewportRelativeDelta, &ViewportActualLocation, &ViewportActualDelta, p_Renderer, p_Globals, p_Settings);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T TopLeft = {
        MinimapOrigin.fX + ViewportActualLocation.fX,
        MinimapOrigin.fY + ViewportActualLocation.fY
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T TopRight = {
        MinimapOrigin.fX + ViewportActualLocation.fX + ViewportActualDelta.fDx,
        MinimapOrigin.fY + ViewportActualLocation.fY
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T BottomRight = {
        MinimapOrigin.fX + ViewportActualLocation.fX + ViewportActualDelta.fDx,
        MinimapOrigin.fY + ViewportActualLocation.fY + ViewportActualDelta.fDy
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T BottomLeft = {
        MinimapOrigin.fX + ViewportActualLocation.fX,
        MinimapOrigin.fY + ViewportActualLocation.fY + ViewportActualDelta.fDy
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->DrawMinimapViewport(p_Renderer, p_Assets, TopLeft, TopRight, BottomRight, BottomLeft);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawMinimapSelectionArea(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, FPOINT_T MinimapOrigin, ASSETS_T* p_Assets, SETTINGS_T* p_Settings) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Prevent rendering of the minimap selection area during the initial mouse down coordinates.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_Globals->CaptureCurrent.iX > 0 || p_Globals->CaptureCurrent.iY > 0) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Render the selection area. Note: lateral and vertical translations are already included to the selection coordinates.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT_T SelectionAreaStartLocation = {
            p_Globals->CaptureStart.iX / (float)p_Globals->TerrainCombinedSize.ulDx,
            p_Globals->CaptureStart.iY / (float)p_Globals->TerrainCombinedSize.ulDy
        };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT_T SelectionAreaEndLocation = {
            p_Globals->CaptureCurrent.iX / (float)p_Globals->TerrainCombinedSize.ulDx,
            p_Globals->CaptureCurrent.iY / (float)p_Globals->TerrainCombinedSize.ulDy
        };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Downscale the coordinates to the minimap size.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT_T MinimapSelectionAreaStartLocation = {
            p_Settings->fMinimapSize * SelectionAreaStartLocation.fX,
            p_Settings->fMinimapSize * SelectionAreaStartLocation.fY
        };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT_T MinimapSelectionAreaEndLocation = {
            p_Settings->fMinimapSize * SelectionAreaEndLocation.fX,
            p_Settings->fMinimapSize * SelectionAreaEndLocation.fY
        };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Create rectangle coordinates.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT_T TopLeft = {
            MinimapOrigin.fX + MinimapSelectionAreaStartLocation.fX,
            MinimapOrigin.fY + MinimapSelectionAreaStartLocation.fY
        };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT_T TopRight = {
            MinimapOrigin.fX + MinimapSelectionAreaEndLocation.fX,
            MinimapOrigin.fY + MinimapSelectionAreaStartLocation.fY
        };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT_T BottomRight = {
            MinimapOrigin.fX + MinimapSelectionAreaEndLocation.fX,
            MinimapOrigin.fY + MinimapSelectionAreaEndLocation.fY
        };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT_T BottomLeft = {
            MinimapOrigin.fX + MinimapSelectionAreaStartLocation.fX,
            MinimapOrigin.fY + MinimapSelectionAreaEndLocation.fY
        };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Renderer->DrawMinimapSelectionArea(p_Renderer, p_Assets, TopLeft, TopRight, BottomRight, BottomLeft);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_PopulateViewportRelativity(FPOINT_T* p_ViewportRelativeLocation,
    FDELTA_T* p_ViewportRelativeDelta,
    FPOINT_T* p_ViewportActualLocation,
    FDELTA_T* p_ViewportActualDelta,
    RENDERER_T* p_Renderer,
    GLOBALS_T* p_Globals,
    SETTINGS_T* p_Settings) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    RECT ClientArea = p_Renderer->GetClientArea(p_Renderer);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T ViewportRelativeLocation = {
        (0.0f - p_Globals->Translation.fDx) / p_Globals->TerrainCombinedSize.ulDx,
        (0.0f - p_Globals->Translation.fDy) / p_Globals->TerrainCombinedSize.ulDy
    };
    FDELTA_T ViewportRelativeDelta = {
        (float)ClientArea.right / p_Globals->TerrainCombinedSize.ulDx,
        (float)ClientArea.bottom / p_Globals->TerrainCombinedSize.ulDy
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T ViewportActualLocation = {
        ViewportRelativeLocation.fX * p_Settings->fMinimapSize,
        ViewportRelativeLocation.fY * p_Settings->fMinimapSize
    };
    FDELTA_T ViewportActualDelta = {
        ViewportRelativeDelta.fDx * p_Settings->fMinimapSize,
        ViewportRelativeDelta.fDy * p_Settings->fMinimapSize
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    *p_ViewportRelativeLocation = ViewportRelativeLocation;
    *p_ViewportRelativeDelta = ViewportRelativeDelta;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    *p_ViewportActualLocation = ViewportActualLocation;
    *p_ViewportActualDelta = ViewportActualDelta;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_PopulateMinimapDimensions(FPOINT_T* p_MinimapOrigin, FDELTA_T* p_MinimapDelta, RENDERER_T* p_Renderer, ASSETS_T* p_Assets, SETTINGS_T* p_Settings) {
    RECT ClientArea = p_Renderer->GetClientArea(p_Renderer);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_T Hud = p_Assets->HUD[1];
    float fBorderPadding = 15.0f;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T MinimapOrigin = {
        ClientArea.right - Hud.Bitmap.bmWidth + fBorderPadding,
        ClientArea.bottom - Hud.Bitmap.bmHeight + fBorderPadding
    };
    FDELTA_T MinimapDelta = {
        p_Settings->fMinimapSize,
        p_Settings->fMinimapSize
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    *p_MinimapOrigin = MinimapOrigin;
    *p_MinimapDelta = MinimapDelta;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 __cdecl PROCESS_IsMinimapClicked(RENDERER_T* p_Renderer, ASSETS_T* p_Assets, GLOBALS_T* p_Globals, SETTINGS_T* p_Settings) {
    UINT8 ubIsClicked = 0;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T MinimapOrigin;
    FDELTA_T MinimapDelta;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_PopulateMinimapDimensions(&MinimapOrigin, &MinimapDelta, p_Renderer, p_Assets, p_Settings);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_Globals->Mouse.iX >= MinimapOrigin.fX && p_Globals->Mouse.iX <= MinimapOrigin.fX + MinimapDelta.fDx &&
        p_Globals->Mouse.iY >= MinimapOrigin.fY && p_Globals->Mouse.iY <= MinimapOrigin.fY + MinimapDelta.fDy) {
        ubIsClicked = 1;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return ubIsClicked;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_AdjustMinimapViewport(IPOINT_T Origin, GLOBALS_T* p_Globals, RENDERER_T* p_Renderer, ASSETS_T* p_Assets, SETTINGS_T* p_Settings) {
    FPOINT_T MinimapOrigin = { 0.0f };
    FDELTA_T MinimapDelta = { 0.0f };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_PopulateMinimapDimensions(&MinimapOrigin, &MinimapDelta, p_Renderer, p_Assets, p_Settings);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T MinimapClickOrigin = {
        (Origin.iX - MinimapOrigin.fX),
        (Origin.iY - MinimapOrigin.fY),
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T ViewportRelativeLocation = { 0.0f };
    FDELTA_T ViewportRelativeDelta = { 0.0f };
    FPOINT_T ViewportActualLocation = { 0.0f };
    FDELTA_T ViewportActualDelta = { 0.0f };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_PopulateViewportRelativity(&ViewportRelativeLocation, &ViewportRelativeDelta, &ViewportActualLocation, &ViewportActualDelta, p_Renderer, p_Globals, p_Settings);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // If we moved the viewport to be centered around the minimap click origin, how far would we translate in the actual view?
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Capture the new actual and relative dimensions.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T NewViewportActualLocation = {
        MinimapClickOrigin.fX - (ViewportActualDelta.fDx / 2.0f),
        MinimapClickOrigin.fY - (ViewportActualDelta.fDy / 2.0f),
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Ensuring we're within bounds in the minimap will also reflect we're in bounds in the main view.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (NewViewportActualLocation.fX <= 0.0f) {
        NewViewportActualLocation.fX = 0.0f;
    }
    if (NewViewportActualLocation.fY <= 0.0f) {
        NewViewportActualLocation.fY = 0.0f;
    }
    if (NewViewportActualLocation.fX + ViewportActualDelta.fDx >= p_Settings->fMinimapSize) {
        NewViewportActualLocation.fX = p_Settings->fMinimapSize - ViewportActualDelta.fDx;
    }
    if (NewViewportActualLocation.fY + ViewportActualDelta.fDy >= p_Settings->fMinimapSize) {
        NewViewportActualLocation.fY = p_Settings->fMinimapSize - ViewportActualDelta.fDy;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Relative dimensions to upscale.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T NewViewportRelativeLocation = {
        NewViewportActualLocation.fX / p_Settings->fMinimapSize,
        NewViewportActualLocation.fY / p_Settings->fMinimapSize
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Actual upscaled dimensions.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T NewMainViewActualLocation = {
        NewViewportRelativeLocation.fX * p_Globals->TerrainCombinedSize.ulDx,
        NewViewportRelativeLocation.fY * p_Globals->TerrainCombinedSize.ulDy
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Determine the delta and apply the change to the global translations and the entities on the screen.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->CapturedTranslationsMinimap = (FDELTA_T){
        -NewMainViewActualLocation.fX - p_Globals->Translation.fDx,
        -NewMainViewActualLocation.fY - p_Globals->Translation.fDy
    };
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_HandleHud(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, CARD_T* p_Card, MENU_T* p_Menu, SETTINGS_T* p_Settings, CONSTANTS_T* p_Constants, LOG_T* p_Log) {
    PROCESS_HandleResourceBar(p_Renderer, p_Globals, p_Assets, p_Menu);
    PROCESS_DrawCommandCard(p_Renderer, p_Globals, p_Assets, p_Card, p_Menu);
    PROCESS_HandleMinimap(p_Renderer, p_Globals, p_Assets, p_Settings, p_Menu, p_Constants, p_Log);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawBuildLimits(RENDERER_T* p_Renderer, ENTITY_T* p_Entity, ASSETS_T* p_Assets) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FDELTA_T AdditionalBuffer = { 0.0f, 0.0f };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_Entity->eType == ENTITY_TYPE_COMMAND) {
        AdditionalBuffer.fDx = p_Entity->HalfSize.fDx;
        AdditionalBuffer.fDy = p_Entity->HalfSize.fDy;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FDELTA_T WorkerBuffer = { p_Assets->Worker[0].Bitmap.bmWidth, p_Assets->Worker[0].Bitmap.bmHeight };
    FPOINT_T Location = { p_Entity->Location.fX, p_Entity->Location.fY };
    FDELTA_T Delta = { p_Entity->Size.fDx, p_Entity->Size.fDy };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T TopLeft = {
        Location.fX - (WorkerBuffer.fDx + AdditionalBuffer.fDx),
        Location.fY - (WorkerBuffer.fDy + AdditionalBuffer.fDy)
    };
    FPOINT_T TopRight = {
        Location.fX + Delta.fDx + WorkerBuffer.fDx + AdditionalBuffer.fDx,
        Location.fY - (WorkerBuffer.fDy + AdditionalBuffer.fDy)
    };
    FPOINT_T BottomRight = {
        Location.fX + Delta.fDx + WorkerBuffer.fDx + AdditionalBuffer.fDx,
        Location.fY + Delta.fDy + WorkerBuffer.fDy + AdditionalBuffer.fDy
    };
    FPOINT_T BottomLeft = {
        Location.fX - (WorkerBuffer.fDx + AdditionalBuffer.fDx),
        Location.fY + Delta.fDy + WorkerBuffer.fDy + AdditionalBuffer.fDy
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->DrawBuildLimits(p_Renderer, p_Assets, TopLeft, TopRight, BottomRight, BottomLeft);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawBuildType(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, MENU_T* p_Menu, LOG_T* p_Log) {
    if (p_Globals->ubCreate) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Draw the red barriers that display the build limits for all entities.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_T* p_Current = p_Globals->p_RootEntity;
        while (p_Current) {
            PROCESS_DrawBuildLimits(p_Renderer, p_Current, p_Assets);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Current = (ENTITY_T*)p_Current->p_Next;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT_T Location = { 0.0f, 0.0f };
        FDELTA_T AdditionalEntityRestrictions = { 0.0f, 0.0f };
        PICTURE_T* p_Picture = NULL;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Draw the buildings image, then the placement restrictions.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        switch (p_Globals->usBuildType) {
        case ENTITY_TYPE_COMMAND: {
            p_Picture = &p_Assets->Command[0];
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            AdditionalEntityRestrictions.fDx = p_Picture->Bitmap.bmWidth >> 1;
            AdditionalEntityRestrictions.fDy = p_Picture->Bitmap.bmHeight >> 1;
            break;
        }
        case ENTITY_TYPE_WORKER: {
            p_Picture = &p_Assets->Worker[0];
            break;
        }
        case ENTITY_TYPE_MINERAL: {
            p_Picture = &p_Assets->Mineral[0];
            break;
        }
        case ENTITY_TYPE_SUPPLY: {
            p_Picture = &p_Assets->Supply[0];
            break;
        }
        case ENTITY_TYPE_REFINERY: {
            p_Picture = &p_Assets->Refinery[0];
            break;
        }
        default: {
            MISC_WriteOut(p_Log, LOG_SEVERITY_WARNING, "PROCESS_DrawBuildType(): Unknown entity type.\n");
        }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Picture) {
            LONG lBmWidth = p_Picture->Bitmap.bmWidth;
            LONG lBmHeight = p_Picture->Bitmap.bmHeight;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            Location.fX = p_Globals->Mouse.iX - (lBmWidth >> 1);
            Location.fY = p_Globals->Mouse.iY - (lBmHeight >> 1);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            IDELTA_T BarrierSize = {
                (int)Location.fX + lBmWidth + AdditionalEntityRestrictions.fDx,
                (int)Location.fY + lBmHeight + AdditionalEntityRestrictions.fDy
            };
            IPOINT_T BarrierLocation = {
                (int)Location.fX - AdditionalEntityRestrictions.fDx,
                (int)Location.fY - AdditionalEntityRestrictions.fDy
            };
            IRECT_T BarrierArea = {
                BarrierLocation,
                BarrierSize
            };
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Renderer->DrawBuildType(p_Renderer, p_Menu, p_Assets, p_Picture, Location, BarrierArea);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 __cdecl PROCESS_ConsiderFrustum(void* vp_Object, PROCESS_TYPE_T eProcessType, RENDERER_T* p_Renderer, LOG_T* p_Log) {
    UINT8 ubIsVisible = 0;
    switch (eProcessType) {
    case PROCESS_TYPE_ENTITY: {
        ENTITY_T* p_Cast = (ENTITY_T*)vp_Object;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ubIsVisible = PROCESS_IsVisible(p_Cast->CenterPoint, p_Renderer);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        break;
    }
    case PROCESS_TYPE_MESSAGE: {
        MESSAGE_T* p_Cast = (MESSAGE_T*)vp_Object;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ubIsVisible = PROCESS_IsVisible(p_Cast->Location, p_Renderer);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        break;
    }
    default: {
        MISC_WriteOut(p_Log, LOG_SEVERITY_WARNING, "PROCESS_ConsiderFrustum(): Unknown process type.\n");
    }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return ubIsVisible;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 __cdecl PROCESS_IsVisible(FPOINT_T Location, RENDERER_T* p_Renderer) {
    RECT ClientArea = p_Renderer->GetClientArea(p_Renderer);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return Location.fX >= 0.0f &&
        Location.fY >= 0.0f &&
        Location.fX <= ClientArea.right &&
        Location.fY <= ClientArea.bottom;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
