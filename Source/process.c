/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/ai.h"
#include "../Headers/assets.h"
#include "../Headers/card.h"
#include "../Headers/common.h"
#include "../Headers/constants.h"
#include "../Headers/double_buffer.h"
#include "../Headers/entity.h"
#include "../Headers/globals.h"
#include "../Headers/menu.h"
#include "../Headers/message.h"
#include "../Headers/misc.h"
#include "../Headers/process.h"
#include "../Headers/transform.h"
#include <stdio.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Future:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// - Put frequently used variables that are accessed via dereferencing onto the stack.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawSelectionArea(DOUBLE_BUFFER_T* p_DoubleBuffer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets) {
    if (p_Globals->ubDrawSelectionRect && !p_Globals->ubCreate) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        HGDIOBJ hPenTemp = SelectObject(p_DoubleBuffer->hDCMem, p_Assets->hPenSelectionArea);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        IPOINT_T OldMousePosition = { p_Globals->iOldMouseX, p_Globals->iOldMouseY };
        IPOINT_T CurrentMousePosition = { p_Globals->iMouseX, p_Globals->iMouseY };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Top, right, bottom, left.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        MoveToEx(p_DoubleBuffer->hDCMem, OldMousePosition.iX, OldMousePosition.iY, NULL);
        LineTo(p_DoubleBuffer->hDCMem, CurrentMousePosition.iX, OldMousePosition.iY);
        LineTo(p_DoubleBuffer->hDCMem, CurrentMousePosition.iX, CurrentMousePosition.iY);
        LineTo(p_DoubleBuffer->hDCMem, OldMousePosition.iX, CurrentMousePosition.iY);
        LineTo(p_DoubleBuffer->hDCMem, OldMousePosition.iX, OldMousePosition.iY);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SelectObject(p_DoubleBuffer->hDCMem, hPenTemp);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Future:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// - Put frequently used variables that are accessed via dereferencing onto the stack.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_CaptureAndApplyTranslations(DOUBLE_BUFFER_T* p_DoubleBuffer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, MENU_T* p_Menu) {
    if (p_Menu->ubEnableTranslations) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        float fAmount = TRANSLATION_STEP_AMOUNT;
        float fLateralBoundaryTranslation = 0.0f;
        float fVerticalBoundaryTranslation = 0.0f;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Top left.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Globals->iMouseX <= p_Globals->usThreshold && p_Globals->iMouseY <= p_Globals->usThreshold) {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Allow lateral translation.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Globals->fLateralTranslation < 0.0f) {
                fLateralBoundaryTranslation = fAmount;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // We translated past a boundary. Reset the translation with the difference.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            else {
                fLateralBoundaryTranslation = -p_Globals->fLateralTranslation;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Allow vertical translation.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Globals->fVerticalTranslation < 0.0f) {
                fVerticalBoundaryTranslation = fAmount;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // We translated past a boundary. Reset the translation with the difference.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            else {
                fVerticalBoundaryTranslation = -p_Globals->fVerticalTranslation;
            }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Bottom left.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (p_Globals->iMouseX <= p_Globals->usThreshold && p_Globals->iMouseY >= p_DoubleBuffer->ClientArea.bottom - p_Globals->usThreshold) {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Allow lateral translation.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Globals->fLateralTranslation < 0.0f) {
                fLateralBoundaryTranslation = fAmount;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // We translated past a boundary. Reset the translation with the difference.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            else {
                fLateralBoundaryTranslation = -p_Globals->fLateralTranslation;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Allow vertical translation.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Globals->fVerticalTranslation > -(p_Assets->Terrain[p_Globals->usMapIndex].Bitmap.bmHeight - p_DoubleBuffer->ClientArea.bottom)) {
                fVerticalBoundaryTranslation = -fAmount;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // We translated past a boundary. Reset the translation with the difference.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            else {
                fVerticalBoundaryTranslation = 0.0f - (p_Globals->fVerticalTranslation + (p_Assets->Terrain[p_Globals->usMapIndex].Bitmap.bmHeight - p_DoubleBuffer->ClientArea.bottom));
            }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Bottom right.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (p_Globals->iMouseX >= p_DoubleBuffer->ClientArea.right - p_Globals->usThreshold && p_Globals->iMouseY >= p_DoubleBuffer->ClientArea.bottom - p_Globals->usThreshold) {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Allow lateral translation.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Globals->fLateralTranslation > -(p_Assets->Terrain[p_Globals->usMapIndex].Bitmap.bmWidth - p_DoubleBuffer->ClientArea.right)) {
                fLateralBoundaryTranslation = -fAmount;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // We translated past a boundary. Reset the translation with the difference.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            else {
                fLateralBoundaryTranslation = 0.0f - (p_Globals->fLateralTranslation + (p_Assets->Terrain[p_Globals->usMapIndex].Bitmap.bmWidth - p_DoubleBuffer->ClientArea.right));
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Allow vertical translation.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Globals->fVerticalTranslation > -(p_Assets->Terrain[p_Globals->usMapIndex].Bitmap.bmHeight - p_DoubleBuffer->ClientArea.bottom)) {
                fVerticalBoundaryTranslation = -fAmount;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // We translated past a boundary. Reset the translation with the difference.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            else {
                fVerticalBoundaryTranslation = 0.0f - (p_Globals->fVerticalTranslation + (p_Assets->Terrain[p_Globals->usMapIndex].Bitmap.bmHeight - p_DoubleBuffer->ClientArea.bottom));
            }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Top right.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (p_Globals->iMouseX >= p_DoubleBuffer->ClientArea.right - p_Globals->usThreshold && p_Globals->iMouseY <= p_Globals->usThreshold) {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Allow lateral translation.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Globals->fLateralTranslation > -(p_Assets->Terrain[p_Globals->usMapIndex].Bitmap.bmWidth - p_DoubleBuffer->ClientArea.right)) {
                fLateralBoundaryTranslation = -fAmount;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // We translated past a boundary. Reset the translation with the difference.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            else {
                fLateralBoundaryTranslation = 0.0f - (p_Globals->fLateralTranslation + (p_Assets->Terrain[p_Globals->usMapIndex].Bitmap.bmWidth - p_DoubleBuffer->ClientArea.right));
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Allow vertical translation.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Globals->fVerticalTranslation < 0.0f) {
                fVerticalBoundaryTranslation = fAmount;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // We translated past a boundary. Reset the translation with the difference.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            else {
                fVerticalBoundaryTranslation = -p_Globals->fVerticalTranslation;
            }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Left.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (p_Globals->iMouseX <= p_Globals->usThreshold) {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Allow translation.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Globals->fLateralTranslation < 0.0f) {
                fLateralBoundaryTranslation = fAmount;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // We translated past a boundary. Reset the translation with the difference.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            else {
                fLateralBoundaryTranslation = -p_Globals->fLateralTranslation;
            }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Bottom.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (p_Globals->iMouseY >= p_DoubleBuffer->ClientArea.bottom - p_Globals->usThreshold) {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Allow translation.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Globals->fVerticalTranslation > -(p_Assets->Terrain[p_Globals->usMapIndex].Bitmap.bmHeight - p_DoubleBuffer->ClientArea.bottom)) {
                fVerticalBoundaryTranslation = -fAmount;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // We translated past a boundary. Reset the translation with the difference.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            else {
                fVerticalBoundaryTranslation = 0.0f - (p_Globals->fVerticalTranslation + (p_Assets->Terrain[p_Globals->usMapIndex].Bitmap.bmHeight - p_DoubleBuffer->ClientArea.bottom));
            }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Right.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (p_Globals->iMouseX >= p_DoubleBuffer->ClientArea.right - p_Globals->usThreshold) {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Allow translation.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Globals->fLateralTranslation > -(p_Assets->Terrain[p_Globals->usMapIndex].Bitmap.bmWidth - p_DoubleBuffer->ClientArea.right)) {
                fLateralBoundaryTranslation = -fAmount;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // We translated past a boundary. Reset the translation with the difference.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            else {
                fLateralBoundaryTranslation = 0.0f - (p_Globals->fLateralTranslation + (p_Assets->Terrain[p_Globals->usMapIndex].Bitmap.bmWidth - p_DoubleBuffer->ClientArea.right));
            }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Top.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (p_Globals->iMouseY <= p_Globals->usThreshold) {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Allow translation.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Globals->fVerticalTranslation < 0.0f) {
                fVerticalBoundaryTranslation = fAmount;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // We translated past a boundary. Reset the translation with the difference.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            else {
                fVerticalBoundaryTranslation = -p_Globals->fVerticalTranslation;
            }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Don't process translations if there's no delta.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (fLateralBoundaryTranslation != 0.0f || fVerticalBoundaryTranslation != 0.0f) {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Accumulate the global translations when interacting with the boundaries. Translations can also be modified by
            // clicking directly on the minimap.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Globals->fLateralTranslation += fLateralBoundaryTranslation;
            p_Globals->fVerticalTranslation += fVerticalBoundaryTranslation;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Apply only the delta to everything present on the main view.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            PROCESS_ApplyTranslations(fLateralBoundaryTranslation, fVerticalBoundaryTranslation, p_Globals, p_Assets);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_ApplyTranslations(float fLateralTranslation, float fVerticalTranslation, GLOBALS_T* p_Globals, ASSETS_T* p_Assets) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Apply translations.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Assets->Terrain[p_Globals->usMapIndex].Location.fX += fLateralTranslation;
    p_Assets->Terrain[p_Globals->usMapIndex].Location.fY += fVerticalTranslation;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Incase we are selecting entities (via mouse drag) and we enter the translation border.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->iOldMouseX += fLateralTranslation;
    p_Globals->iOldMouseY += fVerticalTranslation;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ENTITY_T* p_Entity = p_Globals->p_RootEntity;
    while (p_Entity) {
        p_Entity->Location.fX += fLateralTranslation;
        p_Entity->Location.fY += fVerticalTranslation;
        p_Entity->CenterPoint.fX += fLateralTranslation;
        p_Entity->CenterPoint.fY += fVerticalTranslation;
        p_Entity->MinorDestinationCenterPoint.fX += fLateralTranslation;
        p_Entity->MinorDestinationCenterPoint.fY += fVerticalTranslation;
        p_Entity->MajorDestinationCenterPoint.fX += fLateralTranslation;
        p_Entity->MajorDestinationCenterPoint.fY += fVerticalTranslation;
        p_Entity = (ENTITY_T*)p_Entity->p_Next;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MESSAGE_T* p_Message = p_Globals->p_RootMessage;
    while (p_Message) {
        p_Message->Location.fX += fLateralTranslation;
        p_Message->Location.fY += fVerticalTranslation;
        p_Message = (MESSAGE_T*)p_Message->p_Next;
    }
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
void __cdecl PROCESS_DrawBackground(DOUBLE_BUFFER_T* p_DoubleBuffer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets) {
    FPOINT_T CropStart = {
        p_Globals->fLateralTranslation,
        p_Globals->fVerticalTranslation,
    };
    FDELTA_T CropDelta = {
        -CropStart.fX + p_DoubleBuffer->ClientArea.right,
        -CropStart.fY + p_DoubleBuffer->ClientArea.bottom,
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DOUBLE_BUFFER_CropDrawPictureAt(p_DoubleBuffer, &p_Assets->Terrain[p_Globals->usMapIndex], CropStart, CropDelta, 0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_ProcessEntities(DOUBLE_BUFFER_T* p_DoubleBuffer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, MENU_T* p_Menu, LOG_T* p_Log) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ENTITY_T* p_Current = p_Globals->p_RootEntity;
    while (p_Current) {
        p_Globals->usEntityCount++;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        switch (p_Current->usType) {
        case ENTITY_WORKER: {
            if (p_Menu->ubToggleSimulation) {
                if (p_Current->p_OperatingTarget) {
                    AI_HandleWorkers(p_Current, p_Globals, p_Log);
                }
            }
            break;
        }
        case ENTITY_MINERAL: {
            break;
        }
        case ENTITY_COMMAND: {
            break;
        }
        case ENTITY_SUPPLY: {
            break;
        }
        case ENTITY_REFINERY: {
            break;
        }
        default: {
            printf("PROCESS_ProcessEntities(): Unknown entity type.\n");
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
                switch (p_Current->usType) {
                case ENTITY_COMMAND: {
                    p_Current->usState = ANIMATE_COMMAND_CREATING;
                    break;
                }
                case ENTITY_WORKER: {
                    break;
                }
                case ENTITY_MINERAL: {
                    break;
                }
                case ENTITY_SUPPLY: {
                    break;
                }
                case ENTITY_REFINERY: {
                    break;
                }
                default: {
                    printf("PROCESS_ProcessEntities(): Unknown entity type.\n");
                }
                }
            }
        }
        else {
            if (p_Current->ubIsInMotion) {
                ENTITY_UpdatePosition(p_Current, p_Globals);
            }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Only render the entity, or information about the entity, if it's in the viewport.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (
            p_Current->CenterPoint.fX >= 0.0f &&
            p_Current->CenterPoint.fY >= 0.0f &&
            p_Current->CenterPoint.fX <= p_DoubleBuffer->ClientArea.right &&
            p_Current->CenterPoint.fY <= p_DoubleBuffer->ClientArea.bottom
            ) {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Menu->ubDrawStatuses) {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                if (p_Current->ubIsPaused) {
                    DOUBLE_BUFFER_DrawEntityEllipse(p_DoubleBuffer, p_Current, p_Assets->hPenWhite, p_Assets->hBrush100r0g0b);
                }
                else if (!p_Current->ubIsAlive) {
                    DOUBLE_BUFFER_DrawEntityEllipse(p_DoubleBuffer, p_Current, p_Assets->hPenWhite, p_Assets->hBrush0r100g0b);
                }
                else if (!p_Current->p_Next) {
                    DOUBLE_BUFFER_DrawEntityEllipse(p_DoubleBuffer, p_Current, p_Assets->hPenWhite, p_Assets->hBrush0r0g100b);
                }
                else if (p_Current == p_Globals->p_RootEntity) {
                    DOUBLE_BUFFER_DrawEntityEllipse(p_DoubleBuffer, p_Current, p_Assets->hPenWhite, p_Assets->hBrush100r100g0b);
                }
                else if (p_Current->ubIsCarrying) {
                    DOUBLE_BUFFER_DrawEntityEllipse(p_DoubleBuffer, p_Current, p_Assets->hPenWhite, p_Assets->hBrush0r100g100b);
                }
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Entity highlighting has priority, with regards to statuses.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (ENTITY_WithinPoint(p_Current, (FPOINT_T) { p_Globals->iMouseX, p_Globals->iMouseY })) {
                p_Current->ubIsHighlighted = 1;
                DOUBLE_BUFFER_DrawEntityEllipse(p_DoubleBuffer, p_Current, p_Assets->hPenWhite, p_Assets->hBrush100r0g100b);
            }
            else {
                p_Current->ubIsHighlighted = 0;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Menu->ubDrawMinor) {
                DOUBLE_BUFFER_DrawEntityMinorVector(p_DoubleBuffer, p_Current, p_Assets);
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Menu->ubDrawMajor) {
                DOUBLE_BUFFER_DrawEntityMajorVector(p_DoubleBuffer, p_Current, p_Assets);
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Entity selection has priority.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Current->ubIsSelected) {
                DOUBLE_BUFFER_DrawEntityEllipse(p_DoubleBuffer, p_Current, p_Assets->hPenWhite, p_Assets->hBrush100r100g100b);
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Menu->ubDrawResources) {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Light blue font colour.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                DOUBLE_BUFFER_SetBlitter(p_DoubleBuffer, &(p_Assets->Blitter[0]));
                snprintf(p_DoubleBuffer->szBlitter, sizeof(p_DoubleBuffer->szBlitter), "%d", p_Current->iMineralCount);
                DOUBLE_BUFFER_Blitter(
                    p_DoubleBuffer,
                    p_DoubleBuffer->szBlitter,
                    (FPOINT_T) {
                    p_Current->Location.fX, p_Current->Location.fY - 20.0f
                },
                    p_Menu->ubEnableMasking
                );
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Green font colour.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                DOUBLE_BUFFER_SetBlitter(p_DoubleBuffer, &(p_Assets->Blitter[2]));
                snprintf(p_DoubleBuffer->szBlitter, sizeof(p_DoubleBuffer->szBlitter), "%d", p_Current->iGasCount);
                DOUBLE_BUFFER_Blitter(
                    p_DoubleBuffer,
                    p_DoubleBuffer->szBlitter,
                    (FPOINT_T) {
                    p_Current->Location.fX, p_Current->Location.fY - 40.0f
                },
                    p_Menu->ubEnableMasking
                );
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Menu->ubDrawIds) {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Orange font colour.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                DOUBLE_BUFFER_SetBlitter(p_DoubleBuffer, &(p_Assets->Blitter[7]));
                snprintf(p_DoubleBuffer->szBlitter, sizeof(p_DoubleBuffer->szBlitter), "%d", p_Current->usId);
                DOUBLE_BUFFER_Blitter(
                    p_DoubleBuffer,
                    p_DoubleBuffer->szBlitter,
                    (FPOINT_T) {
                    p_Current->Location.fX, p_Current->Location.fY - 60.0f
                },
                    p_Menu->ubEnableMasking
                );
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Update the picture.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Globals->ubAnimate) {
                ENTITY_Animate(p_Current, p_Assets);
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            DOUBLE_BUFFER_DrawEntity(p_DoubleBuffer, p_Current, p_Menu->ubEnableMasking);
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Current = (ENTITY_T*)p_Current->p_Next;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_ProcessMessages(DOUBLE_BUFFER_T* p_DoubleBuffer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, MENU_T* p_Menu) {
    MESSAGE_T* p_Current = p_Globals->p_RootMessage;
    MESSAGE_T* p_Dead = NULL;
    while (p_Current) {
        p_Globals->usMessageCount++;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Only render the message, or information about the message, if it's in the viewport.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (
            p_Current->Location.fX >= 0.0f &&
            p_Current->Location.fY >= 0.0f &&
            p_Current->Location.fX <= p_DoubleBuffer->ClientArea.right &&
            p_Current->Location.fY <= p_DoubleBuffer->ClientArea.bottom
            ) {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            switch (p_Current->usType) {
            case MESSAGE_GAIN_MINERALS: {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Light blue font colour.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                DOUBLE_BUFFER_SetBlitter(p_DoubleBuffer, &(p_Assets->Blitter[0]));
                break;
            }
            case MESSAGE_BONUS_MINERALS: {
                TRANSFORM_ApplyTransform(p_DoubleBuffer, TRANSFORM_SCALE, p_Current->fScale, p_Current->Location);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Light blue font colour.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                DOUBLE_BUFFER_SetBlitter(p_DoubleBuffer, &(p_Assets->Blitter[0]));
                break;
            }
            case MESSAGE_GAIN_GAS: {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Green font colour.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                DOUBLE_BUFFER_SetBlitter(p_DoubleBuffer, &(p_Assets->Blitter[2]));
                break;
            }
            case MESSAGE_BONUS_GAS: {
                TRANSFORM_ApplyTransform(p_DoubleBuffer, TRANSFORM_SCALE, p_Current->fScale, p_Current->Location);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Green font colour.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                DOUBLE_BUFFER_SetBlitter(p_DoubleBuffer, &(p_Assets->Blitter[2]));
                break;
            }
            case MESSAGE_GENERAL_WARNING: {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Yellow font colour.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                DOUBLE_BUFFER_SetBlitter(p_DoubleBuffer, &(p_Assets->Blitter[6]));
                break;
            }
            default: {
                printf("PROCESS_ProcessMessages(): Unknown message type.\n");
            }
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            DOUBLE_BUFFER_Blitter(
                p_DoubleBuffer,
                p_Current->szMessage,
                (FPOINT_T) {
                p_Current->Location.fX, p_Current->Location.fY
            },
                p_Menu->ubEnableMasking
            );
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            TRANSFORM_ResetTransform(p_DoubleBuffer);
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // If this particular call is updating the entity pictures, update the messages as well.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Globals->ubAnimate) {
            p_Current->sLifespan--;
            p_Current->Location.fY -= 2.0f;
            p_Current->fScale += 0.075f;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // This one's for the graveyard.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (!p_Current->sLifespan) {
                p_Dead = p_Current;
            }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Dead) {
            p_Current = (MESSAGE_T*)p_Current->p_Next;
            MESSAGE_DeleteSpecific(p_Dead, p_Globals);
            p_Dead = NULL;
        }
        else {
            p_Current = (MESSAGE_T*)p_Current->p_Next;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawDiagnostics(DOUBLE_BUFFER_T* p_DoubleBuffer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, MENU_T* p_Menu) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->ulFrameCount++;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_Menu->ubDrawDiagnostics) {
        p_Globals->fClientBottomY = p_DoubleBuffer->ClientArea.bottom;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Pink font colour.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        DOUBLE_BUFFER_SetBlitter(p_DoubleBuffer, &(p_Assets->Blitter[8]));
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Bottom up, above taskbar starting here.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 90.0f;
        char szMode[16] = { 0 };
#ifdef DEBUG
        snprintf(szMode, sizeof(szMode), "Debug");
#elif RELEASE
        snprintf(szMode, sizeof(szMode), "Release");
#else
        snprintf(szMode, sizeof(szMode), "Unknown");
#endif
        snprintf(p_DoubleBuffer->szBlitter, sizeof(p_DoubleBuffer->szBlitter), "BUILD: %s @ %s %s", szMode, __DATE__, __TIME__);
        DOUBLE_BUFFER_Blitter(
            p_DoubleBuffer,
            p_DoubleBuffer->szBlitter,
            (FPOINT_T) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DoubleBuffer->szBlitter, sizeof(p_DoubleBuffer->szBlitter), "(Tx, Ty): (%0.2f, %0.2f)", p_Globals->fLateralTranslation, p_Globals->fVerticalTranslation);
        DOUBLE_BUFFER_Blitter(
            p_DoubleBuffer,
            p_DoubleBuffer->szBlitter,
            (FPOINT_T) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DoubleBuffer->szBlitter, sizeof(p_DoubleBuffer->szBlitter), "(Mx, My): (%d, %d)", p_Globals->iMouseX, p_Globals->iMouseY);
        DOUBLE_BUFFER_Blitter(
            p_DoubleBuffer,
            p_DoubleBuffer->szBlitter,
            (FPOINT_T) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int iCaptureWidth = 0;
        int iCaptureHeight = 0;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Prevent initial width and height to be the initial mouse down coordinates. Translations are considered already so
        // this can be a simple subtraction. This also is in the minimap logic.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Globals->iCaptureCurrentX > 0) {
            iCaptureWidth = p_Globals->iCaptureCurrentX - p_Globals->iCaptureStartX;
        }
        if (p_Globals->iCaptureCurrentY > 0) {
            iCaptureHeight = p_Globals->iCaptureCurrentY - p_Globals->iCaptureStartY;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        snprintf(p_DoubleBuffer->szBlitter, sizeof(p_DoubleBuffer->szBlitter), "(Sw, Sh): (%d, %d)", iCaptureWidth, iCaptureHeight);
        DOUBLE_BUFFER_Blitter(
            p_DoubleBuffer,
            p_DoubleBuffer->szBlitter,
            (FPOINT_T) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DoubleBuffer->szBlitter, sizeof(p_DoubleBuffer->szBlitter), "(Sx, Sy): (%d, %d) -> (%d, %d)", p_Globals->iCaptureStartX, p_Globals->iCaptureStartY, p_Globals->iCaptureCurrentX, p_Globals->iCaptureCurrentY);
        DOUBLE_BUFFER_Blitter(
            p_DoubleBuffer,
            p_DoubleBuffer->szBlitter,
            (FPOINT_T) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DoubleBuffer->szBlitter, sizeof(p_DoubleBuffer->szBlitter), "R/B: (%ld, %ld)", p_DoubleBuffer->ClientArea.right, p_DoubleBuffer->ClientArea.bottom);
        DOUBLE_BUFFER_Blitter(
            p_DoubleBuffer,
            p_DoubleBuffer->szBlitter,
            (FPOINT_T) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DoubleBuffer->szBlitter, sizeof(p_DoubleBuffer->szBlitter), "E(ft/s): %0.2f ms/s", p_Globals->fEngineTimeAverage);
        DOUBLE_BUFFER_Blitter(
            p_DoubleBuffer,
            p_DoubleBuffer->szBlitter,
            (FPOINT_T) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DoubleBuffer->szBlitter, sizeof(p_DoubleBuffer->szBlitter), "E(ft): %0.2f ms", p_Globals->fEngineTime * 1000.0f);
        DOUBLE_BUFFER_Blitter(
            p_DoubleBuffer,
            p_DoubleBuffer->szBlitter,
            (FPOINT_T) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DoubleBuffer->szBlitter, sizeof(p_DoubleBuffer->szBlitter), "E(fps): %0.2f @ %0.2f", p_Globals->fFramesPerSecond, ENGINE_FPS);
        DOUBLE_BUFFER_Blitter(
            p_DoubleBuffer,
            p_DoubleBuffer->szBlitter,
            (FPOINT_T) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        char szFormattedTime[32];
        MISC_FormatTime(p_Globals->ulSecondsTick, szFormattedTime, sizeof(szFormattedTime));
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        snprintf(p_DoubleBuffer->szBlitter, sizeof(p_DoubleBuffer->szBlitter), "UP: %s", szFormattedTime);
        DOUBLE_BUFFER_Blitter(
            p_DoubleBuffer,
            p_DoubleBuffer->szBlitter,
            (FPOINT_T) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DoubleBuffer->szBlitter, sizeof(p_DoubleBuffer->szBlitter), "H: %zu B", p_Globals->stAllocations);
        DOUBLE_BUFFER_Blitter(
            p_DoubleBuffer,
            p_DoubleBuffer->szBlitter,
            (FPOINT_T) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DoubleBuffer->szBlitter, sizeof(p_DoubleBuffer->szBlitter), "W(s): %d", ENTITY_GetSelectedEntityCounts(p_Globals).usSelectedWorkersCount);
        DOUBLE_BUFFER_Blitter(
            p_DoubleBuffer,
            p_DoubleBuffer->szBlitter,
            (FPOINT_T) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DoubleBuffer->szBlitter, sizeof(p_DoubleBuffer->szBlitter), "E(s): %d", ENTITY_GetSelectedEntityCounts(p_Globals).usSelectedAllCount);
        DOUBLE_BUFFER_Blitter(
            p_DoubleBuffer,
            p_DoubleBuffer->szBlitter,
            (FPOINT_T) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DoubleBuffer->szBlitter, sizeof(p_DoubleBuffer->szBlitter), "E(t): %d", p_Globals->usEntityCount);
        DOUBLE_BUFFER_Blitter(
            p_DoubleBuffer,
            p_DoubleBuffer->szBlitter,
            (FPOINT_T) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DoubleBuffer->szBlitter, sizeof(p_DoubleBuffer->szBlitter), "M(t): %d", p_Globals->usMessageCount);
        DOUBLE_BUFFER_Blitter(
            p_DoubleBuffer,
            p_DoubleBuffer->szBlitter,
            (FPOINT_T) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // The counts have already been displayed and were accumulated through different functions. Reset them.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->usEntityCount = 0;
        p_Globals->usMessageCount = 0;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Reset the location that other messages rely on.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY = p_DoubleBuffer->ClientArea.bottom;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Draw the translation threshold boundaries.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        HGDIOBJ hPenTemp = SelectObject(p_DoubleBuffer->hDCMem, p_Assets->hPenTranslation);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Left, bottom, right, top.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        USHORT usLeft = p_Globals->usThreshold;
        USHORT usTop = p_Globals->usThreshold;
        USHORT usRight = p_DoubleBuffer->ClientArea.right - p_Globals->usThreshold;
        USHORT usBottom = p_DoubleBuffer->ClientArea.bottom - p_Globals->usThreshold;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        MoveToEx(p_DoubleBuffer->hDCMem, usLeft, usTop, NULL);
        LineTo(p_DoubleBuffer->hDCMem, usRight, usTop);
        LineTo(p_DoubleBuffer->hDCMem, usRight, usBottom);
        LineTo(p_DoubleBuffer->hDCMem, usLeft, usBottom);
        LineTo(p_DoubleBuffer->hDCMem, usLeft, usTop);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SelectObject(p_DoubleBuffer->hDCMem, hPenTemp);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawResourceBar(DOUBLE_BUFFER_T* p_DoubleBuffer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, MENU_T* p_Menu) {
    p_Globals->fClientBottomY = p_DoubleBuffer->ClientArea.bottom;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Resources background.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DOUBLE_BUFFER_DrawPictureAt(
        p_DoubleBuffer,
        &p_Assets->HUD[0],
        (FPOINT_T) {
        0.0f, p_Globals->fClientBottomY - p_Assets->HUD[0].Bitmap.bmHeight
    },
        p_Menu->ubEnableMasking
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Mineral picture.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DOUBLE_BUFFER_DrawPictureAt(
        p_DoubleBuffer,
        &p_Assets->Mineral[0],
        (FPOINT_T) {
        20.0f, p_Globals->fClientBottomY - 50.0f
    },
        p_Menu->ubEnableMasking
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Mineral text with blue font colour.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DOUBLE_BUFFER_SetBlitter(p_DoubleBuffer, &(p_Assets->Blitter[0]));
    MISC_FormatWithCommas(p_DoubleBuffer->szBlitter, sizeof(p_DoubleBuffer->szBlitter), p_Globals->iMineralCount);
    DOUBLE_BUFFER_Blitter(
        p_DoubleBuffer,
        p_DoubleBuffer->szBlitter,
        (FPOINT_T) {
        60.0f, p_Globals->fClientBottomY - 40.0f
    },
        p_Menu->ubEnableMasking
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Gas picture.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DOUBLE_BUFFER_DrawPictureAt(
        p_DoubleBuffer,
        &p_Assets->Gas[0],
        (FPOINT_T) {
        135.0f, p_Globals->fClientBottomY - 50.0f
    },
        p_Menu->ubEnableMasking
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Gas text with green font colour.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DOUBLE_BUFFER_SetBlitter(p_DoubleBuffer, &(p_Assets->Blitter[2]));
    MISC_FormatWithCommas(p_DoubleBuffer->szBlitter, sizeof(p_DoubleBuffer->szBlitter), p_Globals->iGasCount);
    DOUBLE_BUFFER_Blitter(
        p_DoubleBuffer,
        p_DoubleBuffer->szBlitter, (FPOINT_T) { 175.0f, p_Globals->fClientBottomY - 40.0f },
        p_Menu->ubEnableMasking
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Supply picture.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DOUBLE_BUFFER_DrawPictureAt(
        p_DoubleBuffer,
        &p_Assets->Worker[0],
        (FPOINT_T) {
        250.0f, p_Globals->fClientBottomY - 43.0f
    },
        p_Menu->ubEnableMasking
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Supply text with yellow font colour.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DOUBLE_BUFFER_SetBlitter(p_DoubleBuffer, &(p_Assets->Blitter[6]));
    snprintf(p_DoubleBuffer->szBlitter, sizeof(p_DoubleBuffer->szBlitter), "%d/%d (%d)", p_Globals->iCurrentSupply, p_Globals->iTotalSupply, p_Globals->iTotalSupply - p_Globals->iCurrentSupply);
    DOUBLE_BUFFER_Blitter(
        p_DoubleBuffer,
        p_DoubleBuffer->szBlitter,
        (FPOINT_T) {
        280.0f, p_Globals->fClientBottomY - 40.0f
    },
        p_Menu->ubEnableMasking
    );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawTaskbar(DOUBLE_BUFFER_T* p_DoubleBuffer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, CARD_T* p_Card, MENU_T* p_Menu) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Taskbar background.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DOUBLE_BUFFER_DrawPictureAt(
        p_DoubleBuffer,
        &p_Assets->HUD[1],
        (FPOINT_T) {
        p_DoubleBuffer->ClientArea.right - p_Assets->HUD[1].Bitmap.bmWidth - p_Assets->HUD[1].Bitmap.bmWidth,
            p_DoubleBuffer->ClientArea.bottom - p_Assets->HUD[1].Bitmap.bmHeight
    },
        p_Menu->ubEnableMasking
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Command card.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DOUBLE_BUFFER_DrawPictureAt(
        p_DoubleBuffer,
        p_Card->p_Picture,
        (FPOINT_T) {
        p_DoubleBuffer->ClientArea.right - p_Assets->HUD[1].Bitmap.bmWidth - p_Assets->HUD[1].Bitmap.bmWidth + 15.0f,
            p_DoubleBuffer->ClientArea.bottom - p_Assets->HUD[1].Bitmap.bmHeight + 15.0f
    },
        p_Menu->ubEnableMasking
    );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_HandleMinimap(DOUBLE_BUFFER_T* p_DoubleBuffer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets) {
    FPOINT_T MinimapOrigin;
    FDELTA_T MinimapDelta;
    PROCESS_PopulateMinimapDimensions(&MinimapOrigin, &MinimapDelta, p_DoubleBuffer, p_Assets);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_DrawMinimapPicture(p_DoubleBuffer, p_Globals, p_Assets, MinimapOrigin);
    PROCESS_DrawMinimapEntities(p_DoubleBuffer, p_Globals, MinimapOrigin, p_Assets);
    PROCESS_DrawMinimapViewport(p_DoubleBuffer, p_Globals, p_Assets, MinimapOrigin);
    PROCESS_DrawMinimapSelectionArea(p_DoubleBuffer, p_Globals, MinimapOrigin, p_Assets);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawMinimapPicture(DOUBLE_BUFFER_T* p_DoubleBuffer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, FPOINT_T MinimapOrigin) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Minimap background.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DOUBLE_BUFFER_DrawPictureAt(
        p_DoubleBuffer,
        &p_Assets->HUD[1],
        (FPOINT_T) {
        p_DoubleBuffer->ClientArea.right - p_Assets->HUD[1].Bitmap.bmWidth,
            p_DoubleBuffer->ClientArea.bottom - p_Assets->HUD[1].Bitmap.bmHeight
    },
        0
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DOUBLE_BUFFER_DrawPictureAt(p_DoubleBuffer, &p_Assets->Minimap[p_Globals->usMapIndex], MinimapOrigin, 0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawMinimapEntities(DOUBLE_BUFFER_T* p_DoubleBuffer, GLOBALS_T* p_Globals, FPOINT_T MinimapOrigin, ASSETS_T* p_Assets) {
    HPEN hPen = NULL;
    HBRUSH hBrush = NULL;
    HGDIOBJ hPenTemp = NULL;
    HGDIOBJ hBrushTemp = NULL;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Render the entities.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ENTITY_T* p_Current = p_Globals->p_RootEntity;
    while (p_Current) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // The maps are square, and the locations are a ratio the map size!
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT_T EntityLocation = {
            (p_Current->CenterPoint.fX - p_Current->HalfSize.fDx - p_Globals->fLateralTranslation) / MAP_SIZE,
            (p_Current->CenterPoint.fY - p_Current->HalfSize.fDy - p_Globals->fVerticalTranslation) / MAP_SIZE
        };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FDELTA_T EntityDelta = {
            (p_Current->CenterPoint.fX + p_Current->HalfSize.fDx - p_Globals->fLateralTranslation) / MAP_SIZE,
            (p_Current->CenterPoint.fY + p_Current->HalfSize.fDy - p_Globals->fVerticalTranslation) / MAP_SIZE
        };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT_T MinimapEntityLocation = { MINIMAP_SIZE * EntityLocation.fX, MINIMAP_SIZE * EntityLocation.fY };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FDELTA_T MinimapEntityDelta = { MINIMAP_SIZE * EntityDelta.fDx, MINIMAP_SIZE * EntityDelta.fDy };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Current->ubIsSelected) {
            hBrush = p_Assets->hBrushSelected;
            hPen = p_Assets->hPenSelected;
        }
        else if (p_Current->ubIsHighlighted) {
            hBrush = p_Assets->hBrushHighlighted;
            hPen = p_Assets->hPenHighlighted;
        }
        else {
            switch (p_Current->usType) {
            case ENTITY_WORKER:
                hBrush = p_Assets->hBrushWorker;
                hPen = p_Assets->hPenWorker;
                break;
            case ENTITY_COMMAND:
                hBrush = p_Assets->hBrushCommand;
                hPen = p_Assets->hPenCommand;
                break;
            case ENTITY_MINERAL:
                hBrush = p_Assets->hBrushMineral;
                hPen = p_Assets->hPenMineral;
                break;
            case ENTITY_SUPPLY:
                hBrush = p_Assets->hBrushSupply;
                hPen = p_Assets->hPenSupply;
                break;
            case ENTITY_REFINERY:
                hBrush = p_Assets->hBrushRefinery;
                hPen = p_Assets->hPenRefinery;
                break;
            default: {
                printf("PROCESS_DrawMinimapEntities(): Unknown entity type.\n");
                return;
            }
            }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        hBrushTemp = SelectObject(p_DoubleBuffer->hDCMem, hBrush);
        hPenTemp = SelectObject(p_DoubleBuffer->hDCMem, hPen);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Rectangle(
            p_DoubleBuffer->hDCMem,
            MinimapOrigin.fX + MinimapEntityLocation.fX,
            MinimapOrigin.fY + MinimapEntityLocation.fY,
            MinimapOrigin.fX + MinimapEntityDelta.fDx,
            MinimapOrigin.fY + MinimapEntityDelta.fDy
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SelectObject(p_DoubleBuffer->hDCMem, hBrushTemp);
        SelectObject(p_DoubleBuffer->hDCMem, hPenTemp);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Current = (ENTITY_T*)p_Current->p_Next;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawMinimapViewport(DOUBLE_BUFFER_T* p_DoubleBuffer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, FPOINT_T MinimapOrigin) {
    HGDIOBJ hPenTemp = NULL;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Render the viewport box.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T ViewportRelativeLocation;
    FDELTA_T ViewportRelativeDelta;
    FPOINT_T ViewportActualLocation;
    FDELTA_T ViewportActualDelta;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_PopulateViewportRelativity(&ViewportRelativeLocation, &ViewportRelativeDelta, &ViewportActualLocation, &ViewportActualDelta, p_DoubleBuffer, p_Assets, p_Globals);
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
    hPenTemp = SelectObject(p_DoubleBuffer->hDCMem, p_Assets->hPenViewport);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Top, right, bottom, left.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MoveToEx(p_DoubleBuffer->hDCMem, TopLeft.fX, TopLeft.fY, NULL);
    LineTo(p_DoubleBuffer->hDCMem, TopRight.fX, TopRight.fY);
    LineTo(p_DoubleBuffer->hDCMem, BottomRight.fX, BottomRight.fY);
    LineTo(p_DoubleBuffer->hDCMem, BottomLeft.fX, BottomLeft.fY);
    LineTo(p_DoubleBuffer->hDCMem, TopLeft.fX, TopLeft.fY);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SelectObject(p_DoubleBuffer->hDCMem, hPenTemp);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawMinimapSelectionArea(DOUBLE_BUFFER_T* p_DoubleBuffer, GLOBALS_T* p_Globals, FPOINT_T MinimapOrigin, ASSETS_T* p_Assets) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Prevent rendering of the minimap selection area during the initial mouse down coordinates.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_Globals->iCaptureCurrentX > 0 || p_Globals->iCaptureCurrentY > 0) {
        HGDIOBJ hPenTemp = NULL;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Render the selection area. Note: lateral and vertical translations are already inclusive to the selection coordinates.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT_T SelectionAreaStartLocation = {
            p_Globals->iCaptureStartX / MAP_SIZE,
            p_Globals->iCaptureStartY / MAP_SIZE
        };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT_T SelectionAreaEndLocation = {
            p_Globals->iCaptureCurrentX / MAP_SIZE,
            p_Globals->iCaptureCurrentY / MAP_SIZE
        };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Downscale the coordinates to the minimap size.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT_T MinimapSelectionAreaStartLocation = {
            MINIMAP_SIZE * SelectionAreaStartLocation.fX,
            MINIMAP_SIZE * SelectionAreaStartLocation.fY };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT_T MinimapSelectionAreaEndLocation = {
            MINIMAP_SIZE * SelectionAreaEndLocation.fX,
            MINIMAP_SIZE * SelectionAreaEndLocation.fY };
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
        hPenTemp = SelectObject(p_DoubleBuffer->hDCMem, p_Assets->hPenSelectionArea);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        MoveToEx(p_DoubleBuffer->hDCMem, TopLeft.fX, TopLeft.fY, NULL);
        LineTo(p_DoubleBuffer->hDCMem, TopRight.fX, TopRight.fY);
        LineTo(p_DoubleBuffer->hDCMem, BottomRight.fX, BottomRight.fY);
        LineTo(p_DoubleBuffer->hDCMem, BottomLeft.fX, BottomLeft.fY);
        LineTo(p_DoubleBuffer->hDCMem, TopLeft.fX, TopLeft.fY);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SelectObject(p_DoubleBuffer->hDCMem, hPenTemp);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_PopulateViewportRelativity(FPOINT_T* p_ViewportRelativeLocation, FDELTA_T* p_ViewportRelativeDelta, FPOINT_T* p_ViewportActualLocation, FDELTA_T* p_ViewportActualDelta, DOUBLE_BUFFER_T* p_DoubleBuffer, ASSETS_T* p_Assets, GLOBALS_T* p_Globals) {
    FPOINT_T ViewportRelativeLocation = {
        (0.0f - p_Globals->fLateralTranslation) / p_Assets->Terrain[p_Globals->usMapIndex].Bitmap.bmWidth,
        (0.0f - p_Globals->fVerticalTranslation) / p_Assets->Terrain[p_Globals->usMapIndex].Bitmap.bmHeight
    };
    FDELTA_T ViewportRelativeDelta = {
        (float)p_DoubleBuffer->ClientArea.right / p_Assets->Terrain[p_Globals->usMapIndex].Bitmap.bmWidth,
        (float)p_DoubleBuffer->ClientArea.bottom / p_Assets->Terrain[p_Globals->usMapIndex].Bitmap.bmHeight
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T ViewportActualLocation = {
        ViewportRelativeLocation.fX * MINIMAP_SIZE,
        ViewportRelativeLocation.fY * MINIMAP_SIZE
    };
    FDELTA_T ViewportActualDelta = {
        ViewportRelativeDelta.fDx * MINIMAP_SIZE,
        ViewportRelativeDelta.fDy * MINIMAP_SIZE
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    *p_ViewportRelativeLocation = ViewportRelativeLocation;
    *p_ViewportRelativeDelta = ViewportRelativeDelta;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    *p_ViewportActualLocation = ViewportActualLocation;
    *p_ViewportActualDelta = ViewportActualDelta;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_PopulateMinimapDimensions(FPOINT_T* p_MinimapOrigin, FDELTA_T* p_MinimapDelta, DOUBLE_BUFFER_T* p_DoubleBuffer, ASSETS_T* p_Assets) {
    FPOINT_T MinimapOrigin = {
        p_DoubleBuffer->ClientArea.right - p_Assets->HUD[1].Bitmap.bmWidth + 15.0f,
        p_DoubleBuffer->ClientArea.bottom - p_Assets->HUD[1].Bitmap.bmHeight + 15.0f
    };
    FDELTA_T MinimapDelta = {
        MINIMAP_SIZE,
        MINIMAP_SIZE
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    *p_MinimapOrigin = MinimapOrigin;
    *p_MinimapDelta = MinimapDelta;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 __cdecl PROCESS_IsMinimapClicked(DOUBLE_BUFFER_T* p_DoubleBuffer, ASSETS_T* p_Assets, GLOBALS_T* p_Globals) {
    FPOINT_T MinimapOrigin;
    FDELTA_T MinimapDelta;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_PopulateMinimapDimensions(&MinimapOrigin, &MinimapDelta, p_DoubleBuffer, p_Assets);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_Globals->iMouseX >= MinimapOrigin.fX && p_Globals->iMouseX <= MinimapOrigin.fX + MinimapDelta.fDx &&
        p_Globals->iMouseY >= MinimapOrigin.fY && p_Globals->iMouseY <= MinimapOrigin.fY + MinimapDelta.fDy) {
        return 1;
    }
    else {
        return 0;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_AdjustMinimapViewport(GLOBALS_T* p_Globals, DOUBLE_BUFFER_T* p_DoubleBuffer, ASSETS_T* p_Assets) {
    FPOINT_T MinimapOrigin;
    FDELTA_T MinimapDelta;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_PopulateMinimapDimensions(&MinimapOrigin, &MinimapDelta, p_DoubleBuffer, p_Assets);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T MinimapClickOrigin = {
        (p_Globals->iMouseX - MinimapOrigin.fX),
        (p_Globals->iMouseY - MinimapOrigin.fY),
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T ViewportRelativeLocation;
    FDELTA_T ViewportRelativeDelta;
    FPOINT_T ViewportActualLocation;
    FDELTA_T ViewportActualDelta;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_PopulateViewportRelativity(&ViewportRelativeLocation, &ViewportRelativeDelta, &ViewportActualLocation, &ViewportActualDelta, p_DoubleBuffer, p_Assets, p_Globals);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // If we moved the viewport to be centered around the minimap click origin, how far would we translate in the actual view?
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Capture the new actual and relative dimensions.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T NewViewportActualLocation = {
         MinimapClickOrigin.fX - (ViewportActualDelta.fDx / 2),
         MinimapClickOrigin.fY - (ViewportActualDelta.fDy / 2),
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
    if (NewViewportActualLocation.fX + ViewportActualDelta.fDx >= MINIMAP_SIZE) {
        NewViewportActualLocation.fX = MINIMAP_SIZE - ViewportActualDelta.fDx;
    }
    if (NewViewportActualLocation.fY + ViewportActualDelta.fDy >= MINIMAP_SIZE) {
        NewViewportActualLocation.fY = MINIMAP_SIZE - ViewportActualDelta.fDy;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Relative dimensions to upscale.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T NewViewportRelativeLocation = {
         NewViewportActualLocation.fX / MINIMAP_SIZE,
         NewViewportActualLocation.fY / MINIMAP_SIZE
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Actual upscaled dimensions.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T NewMainViewActualLocation = {
        NewViewportRelativeLocation.fX * MAP_SIZE,
        NewViewportRelativeLocation.fY * MAP_SIZE,
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Determine the delta and apply the change to the global translations and the entities on the screen.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    float fNewMainViewLateralTranslationDelta = -NewMainViewActualLocation.fX - p_Globals->fLateralTranslation;
    float fNewMainViewVerticalTranslationDelta = -NewMainViewActualLocation.fY - p_Globals->fVerticalTranslation;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (fNewMainViewLateralTranslationDelta != 0.0f || fNewMainViewVerticalTranslationDelta != 0.0f) {
        p_Globals->fLateralTranslation += fNewMainViewLateralTranslationDelta;
        p_Globals->fVerticalTranslation += fNewMainViewVerticalTranslationDelta;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        PROCESS_ApplyTranslations(fNewMainViewLateralTranslationDelta, fNewMainViewVerticalTranslationDelta, p_Globals, p_Assets);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_HandleHud(DOUBLE_BUFFER_T* p_DoubleBuffer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, CARD_T* p_Card, MENU_T* p_Menu) {
    PROCESS_DrawResourceBar(p_DoubleBuffer, p_Globals, p_Assets, p_Menu);
    PROCESS_DrawTaskbar(p_DoubleBuffer, p_Globals, p_Assets, p_Card, p_Menu);
    PROCESS_HandleMinimap(p_DoubleBuffer, p_Globals, p_Assets);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawBuildLimits(DOUBLE_BUFFER_T* p_DoubleBuffer, ENTITY_T* p_Entity, ASSETS_T* p_Assets) {
    HGDIOBJ hPenTemp = SelectObject(p_DoubleBuffer->hDCMem, p_Assets->hPenBuildLimits);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T Buffer = { p_Assets->Worker[0].Bitmap.bmWidth, p_Assets->Worker[0].Bitmap.bmHeight };
    FPOINT_T Location = { p_Entity->Location.fX, p_Entity->Location.fY };
    FDELTA_T Delta = { p_Entity->Size.fDx, p_Entity->Size.fDy };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T TopLeft = { Location.fX - Buffer.fX, Location.fY - Buffer.fY };
    FPOINT_T TopRight = { Location.fX + Delta.fDx + Buffer.fX, Location.fY - Buffer.fY };
    FPOINT_T BottomRight = { Location.fX + Delta.fDx + Buffer.fX, Location.fY + Delta.fDy + Buffer.fY };
    FPOINT_T BottomLeft = { Location.fX - Buffer.fX, Location.fY + Delta.fDy + Buffer.fY };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Top, right, bottom, left.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MoveToEx(p_DoubleBuffer->hDCMem, TopLeft.fX, TopLeft.fY, NULL);
    LineTo(p_DoubleBuffer->hDCMem, TopRight.fX, TopRight.fY);
    LineTo(p_DoubleBuffer->hDCMem, BottomRight.fX, BottomRight.fY);
    LineTo(p_DoubleBuffer->hDCMem, BottomLeft.fX, BottomLeft.fY);
    LineTo(p_DoubleBuffer->hDCMem, TopLeft.fX, TopLeft.fY);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SelectObject(p_DoubleBuffer->hDCMem, hPenTemp);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROCESS_DrawBuildType(DOUBLE_BUFFER_T* p_DoubleBuffer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, MENU_T* p_Menu) {
    if (p_Globals->ubCreate) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Draw the red barriers that display the build limits for all entities.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_T* p_Current = p_Globals->p_RootEntity;
        while (p_Current) {
            PROCESS_DrawBuildLimits(p_DoubleBuffer, p_Current, p_Assets);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Current = (ENTITY_T*)p_Current->p_Next;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT_T Location = { 0.0f, 0.0f };
        PICTURE_T* p_Picture = NULL;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Draw the buildings image, then the placement restrictions.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        switch (p_Globals->usBuildType) {
        case ENTITY_COMMAND: { p_Picture = &p_Assets->Command[0]; break; }
        case ENTITY_WORKER: { p_Picture = &p_Assets->Worker[0]; break; }
        case ENTITY_MINERAL: { p_Picture = &p_Assets->Mineral[0]; break; }
        case ENTITY_SUPPLY: { p_Picture = &p_Assets->Supply[0]; break; }
        case ENTITY_REFINERY: { p_Picture = &p_Assets->Refinery[0]; break; }
        default: {
            printf("PROCESS_DrawBuildType(): Unknown entity type.\n");
        }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Picture) {
            LONG width = p_Picture->Bitmap.bmWidth;
            LONG height = p_Picture->Bitmap.bmHeight;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            Location.fX = p_Globals->iMouseX - (width >> 1);
            Location.fY = p_Globals->iMouseY - (height >> 1);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            DOUBLE_BUFFER_DrawPictureAt(p_DoubleBuffer, p_Picture, Location, p_Menu->ubEnableMasking);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            HGDIOBJ hPenTemp = SelectObject(p_DoubleBuffer->hDCMem, p_Assets->hPenBuildType);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            int iLeft = (int)Location.fX;
            int iTop = (int)Location.fY;
            int iRight = (int)(Location.fX + width);
            int iBottom = (int)(Location.fY + height);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Top, right, bottom, left.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            MoveToEx(p_DoubleBuffer->hDCMem, iLeft, iTop, NULL);
            LineTo(p_DoubleBuffer->hDCMem, iRight, iTop);
            LineTo(p_DoubleBuffer->hDCMem, iRight, iBottom);
            LineTo(p_DoubleBuffer->hDCMem, iLeft, iBottom);
            LineTo(p_DoubleBuffer->hDCMem, iLeft, iTop);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            SelectObject(p_DoubleBuffer->hDCMem, hPenTemp);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
