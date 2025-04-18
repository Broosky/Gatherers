/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _PROCESS_C_
#define _PROCESS_C_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/constants.h"
#include "../Headers/functions.h"
#include "../Headers/types.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Future:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// - Put frequently used variables that are accessed via dereferencing onto the stack.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROC_DrawSelectionArea(DBLBUF* p_DblBuf, GLOBALS* p_Globals, COLORREF PenColour) {
    if (p_Globals->ubDrawSelectionRect && !p_Globals->ubCreate) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        HPEN hPen = CreatePen(PS_SOLID, 1, PenColour);
        HGDIOBJ hPenTemp = SelectObject(p_DblBuf->hDCMem, hPen);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        IPOINT OldMousePosition = { p_Globals->iOldMouseX, p_Globals->iOldMouseY };
        IPOINT CurrentMousePosition = { p_Globals->iMouseX, p_Globals->iMouseY };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Top, right, bottom, left.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        MoveToEx(p_DblBuf->hDCMem, OldMousePosition.iX, OldMousePosition.iY, NULL);
        LineTo(p_DblBuf->hDCMem, CurrentMousePosition.iX, OldMousePosition.iY);
        MoveToEx(p_DblBuf->hDCMem, CurrentMousePosition.iX, OldMousePosition.iY, NULL);
        LineTo(p_DblBuf->hDCMem, CurrentMousePosition.iX, CurrentMousePosition.iY);
        MoveToEx(p_DblBuf->hDCMem, CurrentMousePosition.iX, CurrentMousePosition.iY, NULL);
        LineTo(p_DblBuf->hDCMem, OldMousePosition.iX, CurrentMousePosition.iY);
        MoveToEx(p_DblBuf->hDCMem, OldMousePosition.iX, CurrentMousePosition.iY, NULL);
        LineTo(p_DblBuf->hDCMem, OldMousePosition.iX, OldMousePosition.iY);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SelectObject(p_DblBuf->hDCMem, hPenTemp);
        DeleteObject(hPen);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Future:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// - Put frequently used variables that are accessed via dereferencing onto the stack.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROC_CaptureAndApplyTranslations(DBLBUF* p_DblBuf, GLOBALS* p_Globals, IMAGES* p_Images, MENU* p_Menu) {
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
        else if (p_Globals->iMouseX <= p_Globals->usThreshold && p_Globals->iMouseY >= p_DblBuf->ClientArea.bottom - p_Globals->usThreshold) {
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
            if (p_Globals->fVerticalTranslation > -(p_Images->Terrain[p_Globals->usMapIndex].Bitmap.bmHeight - p_DblBuf->ClientArea.bottom)) {
                fVerticalBoundaryTranslation = -fAmount;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // We translated past a boundary. Reset the translation with the difference.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            else {
                fVerticalBoundaryTranslation = 0.0f - (p_Globals->fVerticalTranslation + (p_Images->Terrain[p_Globals->usMapIndex].Bitmap.bmHeight - p_DblBuf->ClientArea.bottom));
            }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Bottom right.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (p_Globals->iMouseX >= p_DblBuf->ClientArea.right - p_Globals->usThreshold && p_Globals->iMouseY >= p_DblBuf->ClientArea.bottom - p_Globals->usThreshold) {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Allow lateral translation.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Globals->fLateralTranslation > -(p_Images->Terrain[p_Globals->usMapIndex].Bitmap.bmWidth - p_DblBuf->ClientArea.right)) {
                fLateralBoundaryTranslation = -fAmount;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // We translated past a boundary. Reset the translation with the difference.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            else {
                fLateralBoundaryTranslation = 0.0f - (p_Globals->fLateralTranslation + (p_Images->Terrain[p_Globals->usMapIndex].Bitmap.bmWidth - p_DblBuf->ClientArea.right));
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Allow vertical translation.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Globals->fVerticalTranslation > -(p_Images->Terrain[p_Globals->usMapIndex].Bitmap.bmHeight - p_DblBuf->ClientArea.bottom)) {
                fVerticalBoundaryTranslation = -fAmount;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // We translated past a boundary. Reset the translation with the difference.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            else {
                fVerticalBoundaryTranslation = 0.0f - (p_Globals->fVerticalTranslation + (p_Images->Terrain[p_Globals->usMapIndex].Bitmap.bmHeight - p_DblBuf->ClientArea.bottom));
            }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Top right.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (p_Globals->iMouseX >= p_DblBuf->ClientArea.right - p_Globals->usThreshold && p_Globals->iMouseY <= p_Globals->usThreshold) {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Allow lateral translation.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Globals->fLateralTranslation > -(p_Images->Terrain[p_Globals->usMapIndex].Bitmap.bmWidth - p_DblBuf->ClientArea.right)) {
                fLateralBoundaryTranslation = -fAmount;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // We translated past a boundary. Reset the translation with the difference.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            else {
                fLateralBoundaryTranslation = 0.0f - (p_Globals->fLateralTranslation + (p_Images->Terrain[p_Globals->usMapIndex].Bitmap.bmWidth - p_DblBuf->ClientArea.right));
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
        else if (p_Globals->iMouseY >= p_DblBuf->ClientArea.bottom - p_Globals->usThreshold) {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Allow translation.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Globals->fVerticalTranslation > -(p_Images->Terrain[p_Globals->usMapIndex].Bitmap.bmHeight - p_DblBuf->ClientArea.bottom)) {
                fVerticalBoundaryTranslation = -fAmount;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // We translated past a boundary. Reset the translation with the difference.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            else {
                fVerticalBoundaryTranslation = 0.0f - (p_Globals->fVerticalTranslation + (p_Images->Terrain[p_Globals->usMapIndex].Bitmap.bmHeight - p_DblBuf->ClientArea.bottom));
            }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Right.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (p_Globals->iMouseX >= p_DblBuf->ClientArea.right - p_Globals->usThreshold) {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Allow translation.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Globals->fLateralTranslation > -(p_Images->Terrain[p_Globals->usMapIndex].Bitmap.bmWidth - p_DblBuf->ClientArea.right)) {
                fLateralBoundaryTranslation = -fAmount;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // We translated past a boundary. Reset the translation with the difference.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            else {
                fLateralBoundaryTranslation = 0.0f - (p_Globals->fLateralTranslation + (p_Images->Terrain[p_Globals->usMapIndex].Bitmap.bmWidth - p_DblBuf->ClientArea.right));
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
        // Accumulate the global translations when interacting with the boundaries. Translations can also be modified by
        // clicking directly on the minimap.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fLateralTranslation += fLateralBoundaryTranslation;
        p_Globals->fVerticalTranslation += fVerticalBoundaryTranslation;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Apply only the delta to everything present on the main view.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        PROC_ApplyTranslations(fLateralBoundaryTranslation, fVerticalBoundaryTranslation, p_Globals, p_Images);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROC_ApplyTranslations(float fLateralTranslation, float fVerticalTranslation, GLOBALS* p_Globals, IMAGES* p_Images) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Apply translations.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Images->Terrain[p_Globals->usMapIndex].Location.fX += fLateralTranslation;
    p_Images->Terrain[p_Globals->usMapIndex].Location.fY += fVerticalTranslation;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Incase we are selecting entities (via mouse drag) and we enter the translation border.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->iOldMouseX += fLateralTranslation;
    p_Globals->iOldMouseY += fVerticalTranslation;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ENTITY* p_Entity = p_Globals->p_RootEntity;
    while (p_Entity) {
        p_Entity->Location.fX += fLateralTranslation;
        p_Entity->Location.fY += fVerticalTranslation;
        p_Entity->CenterPoint.fX += fLateralTranslation;
        p_Entity->CenterPoint.fY += fVerticalTranslation;
        p_Entity->MinorDestinationCenterPoint.fX += fLateralTranslation;
        p_Entity->MinorDestinationCenterPoint.fY += fVerticalTranslation;
        p_Entity->MajorDestinationCenterPoint.fX += fLateralTranslation;
        p_Entity->MajorDestinationCenterPoint.fY += fVerticalTranslation;
        p_Entity = (ENTITY*)p_Entity->p_Next;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MESSAGE* p_Message = p_Globals->p_RootMessage;
    while (p_Message) {
        p_Message->Location.fX += fLateralTranslation;
        p_Message->Location.fY += fVerticalTranslation;
        p_Message = (MESSAGE*)p_Message->p_Next;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROC_UpdateAnimations(GLOBALS* p_Globals) {
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
void __cdecl PROC_DrawBackground(DBLBUF* p_DblBuf, GLOBALS* p_Globals, IMAGES* p_Images) {
    FPOINT CropStart = {
        p_Globals->fLateralTranslation,
        p_Globals->fVerticalTranslation,
    };
    FDELTA CropDelta = {
        -CropStart.fX + p_DblBuf->ClientArea.right,
        -CropStart.fY + p_DblBuf->ClientArea.bottom,
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DBLBUF_CropDrawPictureAt(p_DblBuf, &p_Images->Terrain[p_Globals->usMapIndex], CropStart, CropDelta, 0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROC_ProcessEntities(DBLBUF* p_DblBuf, GLOBALS* p_Globals, IMAGES* p_Images, MENU* p_Menu) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ENTITY* p_Current = p_Globals->p_RootEntity;
    while (p_Current) {
        p_Globals->usEntityCount++;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        switch (p_Current->usType) {
        case ENTITY_WORKER: {
            if (p_Menu->ubToggleSimulation) {
                if (p_Current->p_Operating) {
                    AI_HandleWorkers(p_Current, p_Globals);
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
            printf("PROC_ProcessEntities(): Unknown entity type.\n");
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
                    printf("PROC_ProcessEntities(): Unknown entity type.\n");
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
            p_Current->CenterPoint.fX <= p_DblBuf->ClientArea.right &&
            p_Current->CenterPoint.fY <= p_DblBuf->ClientArea.bottom
            ) {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Menu->ubDrawStatuses) {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                if (p_Current->ubIsPaused) {
                    DBLBUF_DrawEntityEllipse(p_DblBuf, p_Current, RGB(255, 255, 255), RGB(100, 0, 0));
                }
                else if (!p_Current->ubIsAlive) {
                    DBLBUF_DrawEntityEllipse(p_DblBuf, p_Current, RGB(255, 255, 255), RGB(0, 100, 0));
                }
                else if (!p_Current->p_Next) {
                    DBLBUF_DrawEntityEllipse(p_DblBuf, p_Current, RGB(255, 255, 255), RGB(0, 0, 100));
                }
                else if (p_Current == p_Globals->p_RootEntity) {
                    DBLBUF_DrawEntityEllipse(p_DblBuf, p_Current, RGB(255, 255, 255), RGB(100, 100, 0));
                }
                else if (p_Current->ubIsCarrying) {
                    DBLBUF_DrawEntityEllipse(p_DblBuf, p_Current, RGB(255, 255, 255), RGB(0, 100, 100));
                }
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Entity highlighting has priority, with regards to statuses.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (ENTITY_WithinPoint(p_Current, (FPOINT) { p_Globals->iMouseX, p_Globals->iMouseY })) {
                p_Current->ubIsHighlighted = 1;
                DBLBUF_DrawEntityEllipse(p_DblBuf, p_Current, RGB(255, 255, 255), RGB(100, 0, 100));
            }
            else {
                p_Current->ubIsHighlighted = 0;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Menu->ubDrawMinor) {
                DBLBUF_DrawEntityMinorVector(p_DblBuf, p_Current, RGB(255, 127, 255));
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Menu->ubDrawMajor) {
                DBLBUF_DrawEntityMajorVector(p_DblBuf, p_Current, RGB(0, 255, 127));
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Entity selection has priority.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Current->ubIsSelected) {
                DBLBUF_DrawEntityEllipse(p_DblBuf, p_Current, RGB(255, 255, 255), RGB(100, 100, 100));
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Menu->ubDrawResources) {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Light blue font colour.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                DBLBUF_SetBlitter(p_DblBuf, &(p_Images->Blitter[0]));
                snprintf(p_DblBuf->szBlitter, sizeof(p_DblBuf->szBlitter), "%d", p_Current->iMineralCount);
                DBLBUF_Blitter(
                    p_DblBuf,
                    p_DblBuf->szBlitter,
                    (FPOINT) {
                    p_Current->Location.fX, p_Current->Location.fY - 20.0f
                },
                    p_Menu->ubEnableMasking
                );
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Green font colour.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                DBLBUF_SetBlitter(p_DblBuf, &(p_Images->Blitter[2]));
                snprintf(p_DblBuf->szBlitter, sizeof(p_DblBuf->szBlitter), "%d", p_Current->iGasCount);
                DBLBUF_Blitter(
                    p_DblBuf,
                    p_DblBuf->szBlitter,
                    (FPOINT) {
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
                DBLBUF_SetBlitter(p_DblBuf, &(p_Images->Blitter[7]));
                snprintf(p_DblBuf->szBlitter, sizeof(p_DblBuf->szBlitter), "%d", p_Current->usId);
                DBLBUF_Blitter(
                    p_DblBuf,
                    p_DblBuf->szBlitter,
                    (FPOINT) {
                    p_Current->Location.fX, p_Current->Location.fY - 60.0f
                },
                    p_Menu->ubEnableMasking
                );
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Update the picture.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Globals->ubAnimate) {
                ENTITY_Animate(p_Current, p_Images);
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            DBLBUF_DrawEntity(p_DblBuf, p_Current, p_Menu->ubEnableMasking);
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Current = (ENTITY*)p_Current->p_Next;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROC_ProcessMessages(DBLBUF* p_DblBuf, GLOBALS* p_Globals, IMAGES* p_Images, MENU* p_Menu) {
    MESSAGE* p_Current = p_Globals->p_RootMessage;
    MESSAGE* p_Dead = NULL;
    while (p_Current) {
        p_Globals->usMessageCount++;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Only render the message, or information about the message, if it's in the viewport.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (
            p_Current->Location.fX >= 0.0f &&
            p_Current->Location.fY >= 0.0f &&
            p_Current->Location.fX <= p_DblBuf->ClientArea.right &&
            p_Current->Location.fY <= p_DblBuf->ClientArea.bottom
            ) {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            switch (p_Current->usType) {
            case MESSAGE_GAIN_MINERALS: {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Light blue font colour.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                DBLBUF_SetBlitter(p_DblBuf, &(p_Images->Blitter[0]));
                break;
            }
            case MESSAGE_BONUS_MINERALS: {
                RENDER_ApplyTransform(p_DblBuf, RENDER_SCALE, p_Current->fScale, p_Current->Location);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Light blue font colour.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                DBLBUF_SetBlitter(p_DblBuf, &(p_Images->Blitter[0]));
                break;
            }
            case MESSAGE_GAIN_GAS: {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Green font colour.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                DBLBUF_SetBlitter(p_DblBuf, &(p_Images->Blitter[2]));
                break;
            }
            case MESSAGE_BONUS_GAS: {
                RENDER_ApplyTransform(p_DblBuf, RENDER_SCALE, p_Current->fScale, p_Current->Location);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Green font colour.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                DBLBUF_SetBlitter(p_DblBuf, &(p_Images->Blitter[2]));
                break;
            }
            case MESSAGE_GENERAL_WARNING: {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Yellow font colour.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                DBLBUF_SetBlitter(p_DblBuf, &(p_Images->Blitter[6]));
                break;
            }
            default: {
                printf("PROC_ProcessMessages(): Unknown message type.\n");
            }
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            DBLBUF_Blitter(
                p_DblBuf,
                p_Current->szMessage,
                (FPOINT) {
                p_Current->Location.fX, p_Current->Location.fY
            },
                p_Menu->ubEnableMasking
            );
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            RENDER_ResetTransform(p_DblBuf);
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
            p_Current = (MESSAGE*)p_Current->p_Next;
            MESSAGE_DeleteSpecific(p_Dead, p_Globals);
            p_Dead = NULL;
        }
        else {
            p_Current = (MESSAGE*)p_Current->p_Next;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROC_DrawDiagnostics(DBLBUF* p_DblBuf, GLOBALS* p_Globals, IMAGES* p_Images, MENU* p_Menu) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->ulFrameCount++;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_Menu->ubDrawDiagnostics) {
        p_Globals->fClientBottomY = p_DblBuf->ClientArea.bottom;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Pink font colour.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        DBLBUF_SetBlitter(p_DblBuf, &(p_Images->Blitter[8]));
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
        snprintf(p_DblBuf->szBlitter, sizeof(p_DblBuf->szBlitter), "BUILD: %s @ %s %s", szMode, __DATE__, __TIME__);
        DBLBUF_Blitter(
            p_DblBuf,
            p_DblBuf->szBlitter,
            (FPOINT) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DblBuf->szBlitter, sizeof(p_DblBuf->szBlitter), "(Tx, Ty): (%0.2f, %0.2f)", p_Globals->fLateralTranslation, p_Globals->fVerticalTranslation);
        DBLBUF_Blitter(
            p_DblBuf,
            p_DblBuf->szBlitter,
            (FPOINT) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DblBuf->szBlitter, sizeof(p_DblBuf->szBlitter), "(Mx, My): (%d, %d)", p_Globals->iMouseX, p_Globals->iMouseY);
        DBLBUF_Blitter(
            p_DblBuf,
            p_DblBuf->szBlitter,
            (FPOINT) {
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
        snprintf(p_DblBuf->szBlitter, sizeof(p_DblBuf->szBlitter), "(Sw, Sh): (%d, %d)", iCaptureWidth, iCaptureHeight);
        DBLBUF_Blitter(
            p_DblBuf,
            p_DblBuf->szBlitter,
            (FPOINT) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DblBuf->szBlitter, sizeof(p_DblBuf->szBlitter), "(Sx, Sy): (%d, %d) -> (%d, %d)", p_Globals->iCaptureStartX, p_Globals->iCaptureStartY, p_Globals->iCaptureCurrentX, p_Globals->iCaptureCurrentY);
        DBLBUF_Blitter(
            p_DblBuf,
            p_DblBuf->szBlitter,
            (FPOINT) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DblBuf->szBlitter, sizeof(p_DblBuf->szBlitter), "R/B: (%ld, %ld)", p_DblBuf->ClientArea.right, p_DblBuf->ClientArea.bottom);
        DBLBUF_Blitter(
            p_DblBuf,
            p_DblBuf->szBlitter,
            (FPOINT) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DblBuf->szBlitter, sizeof(p_DblBuf->szBlitter), "E(ft/s): %0.2f ms/s", p_Globals->fEngineTimeAverage);
        DBLBUF_Blitter(
            p_DblBuf,
            p_DblBuf->szBlitter,
            (FPOINT) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DblBuf->szBlitter, sizeof(p_DblBuf->szBlitter), "E(ft): %0.2f ms", p_Globals->fEngineTime * 1000.0f);
        DBLBUF_Blitter(
            p_DblBuf,
            p_DblBuf->szBlitter,
            (FPOINT) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DblBuf->szBlitter, sizeof(p_DblBuf->szBlitter), "E(fps): %0.2f @ %0.2f", p_Globals->fFramesPerSecond, ENGINE_FPS);
        DBLBUF_Blitter(
            p_DblBuf,
            p_DblBuf->szBlitter,
            (FPOINT) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        char szFormattedTime[32];
        MISC_FormatTime(p_Globals->ulSecondsTick, szFormattedTime, sizeof(szFormattedTime));
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        snprintf(p_DblBuf->szBlitter, sizeof(p_DblBuf->szBlitter), "UP: %s", szFormattedTime);
        DBLBUF_Blitter(
            p_DblBuf,
            p_DblBuf->szBlitter,
            (FPOINT) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DblBuf->szBlitter, sizeof(p_DblBuf->szBlitter), "H: %d B", p_Globals->iRunningHeap);
        DBLBUF_Blitter(
            p_DblBuf,
            p_DblBuf->szBlitter,
            (FPOINT) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DblBuf->szBlitter, sizeof(p_DblBuf->szBlitter), "W(s): %d", ENTITY_GetSelectedEntityCounts(p_Globals).usSelectedWorkersCount);
        DBLBUF_Blitter(
            p_DblBuf,
            p_DblBuf->szBlitter,
            (FPOINT) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DblBuf->szBlitter, sizeof(p_DblBuf->szBlitter), "E(s): %d", ENTITY_GetSelectedEntityCounts(p_Globals).usSelectedAllCount);
        DBLBUF_Blitter(
            p_DblBuf,
            p_DblBuf->szBlitter,
            (FPOINT) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DblBuf->szBlitter, sizeof(p_DblBuf->szBlitter), "E(t): %d", p_Globals->usEntityCount);
        DBLBUF_Blitter(
            p_DblBuf,
            p_DblBuf->szBlitter,
            (FPOINT) {
            15.0f, p_Globals->fClientBottomY
        },
            p_Menu->ubEnableMasking
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->fClientBottomY -= 20.0f;
        snprintf(p_DblBuf->szBlitter, sizeof(p_DblBuf->szBlitter), "M(t): %d", p_Globals->usMessageCount);
        DBLBUF_Blitter(
            p_DblBuf,
            p_DblBuf->szBlitter,
            (FPOINT) {
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
        p_Globals->fClientBottomY = p_DblBuf->ClientArea.bottom;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Draw the translation threshold boundaries.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
        HGDIOBJ hPenTemp = SelectObject(p_DblBuf->hDCMem, hPen);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Left, bottom, right, top.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        MoveToEx(p_DblBuf->hDCMem, p_Globals->usThreshold, 0, NULL);
        LineTo(p_DblBuf->hDCMem, p_Globals->usThreshold, p_DblBuf->ClientArea.bottom);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        MoveToEx(p_DblBuf->hDCMem, 0, p_DblBuf->ClientArea.bottom - p_Globals->usThreshold, NULL);
        LineTo(p_DblBuf->hDCMem, p_DblBuf->ClientArea.right, p_DblBuf->ClientArea.bottom - p_Globals->usThreshold);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        MoveToEx(
            p_DblBuf->hDCMem,
            p_DblBuf->ClientArea.right - p_Globals->usThreshold,
            p_DblBuf->ClientArea.bottom, NULL
        );
        LineTo(p_DblBuf->hDCMem, p_DblBuf->ClientArea.right - p_Globals->usThreshold, 0);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        MoveToEx(p_DblBuf->hDCMem, 0, p_Globals->usThreshold, NULL);
        LineTo(p_DblBuf->hDCMem, p_DblBuf->ClientArea.right, p_Globals->usThreshold);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SelectObject(p_DblBuf->hDCMem, hPenTemp);
        DeleteObject(hPen);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROC_DrawResourceBar(DBLBUF* p_DblBuf, GLOBALS* p_Globals, IMAGES* p_Images, MENU* p_Menu) {
    p_Globals->fClientBottomY = p_DblBuf->ClientArea.bottom;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Resources background.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DBLBUF_DrawPictureAt(
        p_DblBuf,
        &p_Images->HUD[0],
        (FPOINT) {
        0.0f, p_Globals->fClientBottomY - p_Images->HUD[0].Bitmap.bmHeight
    },
        p_Menu->ubEnableMasking
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Mineral picture.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DBLBUF_DrawPictureAt(
        p_DblBuf,
        &p_Images->Mineral[0],
        (FPOINT) {
        20.0f, p_Globals->fClientBottomY - 50.0f
    },
        p_Menu->ubEnableMasking
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Mineral text with blue font colour.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DBLBUF_SetBlitter(p_DblBuf, &(p_Images->Blitter[0]));
    snprintf(p_DblBuf->szBlitter, sizeof(p_DblBuf->szBlitter), "%d", p_Globals->iMineralCount);
    DBLBUF_Blitter(
        p_DblBuf,
        p_DblBuf->szBlitter,
        (FPOINT) {
        60.0f, p_Globals->fClientBottomY - 40.0f
    },
        p_Menu->ubEnableMasking
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Gas picture.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DBLBUF_DrawPictureAt(
        p_DblBuf,
        &p_Images->Gas[0],
        (FPOINT) {
        135.0f, p_Globals->fClientBottomY - 50.0f
    },
        p_Menu->ubEnableMasking
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Gas text with green font colour.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DBLBUF_SetBlitter(p_DblBuf, &(p_Images->Blitter[2]));
    snprintf(p_DblBuf->szBlitter, sizeof(p_DblBuf->szBlitter), "%d", p_Globals->iGasCount);
    DBLBUF_Blitter(
        p_DblBuf,
        p_DblBuf->szBlitter, (FPOINT) { 175.0f, p_Globals->fClientBottomY - 40.0f },
        p_Menu->ubEnableMasking
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Supply picture.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DBLBUF_DrawPictureAt(
        p_DblBuf,
        &p_Images->Worker[0],
        (FPOINT) {
        250.0f, p_Globals->fClientBottomY - 43.0f
    },
        p_Menu->ubEnableMasking
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Supply text with yellow font colour.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DBLBUF_SetBlitter(p_DblBuf, &(p_Images->Blitter[6]));
    snprintf(p_DblBuf->szBlitter, sizeof(p_DblBuf->szBlitter), "%d/%d (%d)", p_Globals->iCurrentSupply, p_Globals->iTotalSupply, p_Globals->iTotalSupply - p_Globals->iCurrentSupply);
    DBLBUF_Blitter(
        p_DblBuf,
        p_DblBuf->szBlitter,
        (FPOINT) {
        280.0f, p_Globals->fClientBottomY - 40.0f
    },
        p_Menu->ubEnableMasking
    );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROC_DrawTaskbar(DBLBUF* p_DblBuf, GLOBALS* p_Globals, IMAGES* p_Images, CARD* p_Card, MENU* p_Menu) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Taskbar background.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DBLBUF_DrawPictureAt(
        p_DblBuf,
        &p_Images->HUD[1],
        (FPOINT) {
        p_DblBuf->ClientArea.right - p_Images->HUD[1].Bitmap.bmWidth - p_Images->HUD[1].Bitmap.bmWidth,
            p_DblBuf->ClientArea.bottom - p_Images->HUD[1].Bitmap.bmHeight
    },
        p_Menu->ubEnableMasking
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Command card.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DBLBUF_DrawPictureAt(
        p_DblBuf,
        p_Card->p_Picture,
        (FPOINT) {
        p_DblBuf->ClientArea.right - p_Images->HUD[1].Bitmap.bmWidth - p_Images->HUD[1].Bitmap.bmWidth + 15.0f,
            p_DblBuf->ClientArea.bottom - p_Images->HUD[1].Bitmap.bmHeight + 15.0f
    },
        p_Menu->ubEnableMasking
    );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Future:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// - Change to FPOINT and IPOINT structures.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROC_HandleMinimap(DBLBUF* p_DblBuf, GLOBALS* p_Globals, IMAGES* p_Images) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Minimap origin.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT MinimapOrigin;
    FPOINT MinimapDelta;
    PROC_PopulateMinimapDimensions(&MinimapOrigin, &MinimapDelta, p_DblBuf, p_Images);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROC_DrawMinimapPicture(p_DblBuf, p_Globals, p_Images, MinimapOrigin);
    PROC_DrawMinimapEntities(p_DblBuf, p_Globals, MinimapOrigin);
    PROC_DrawMinimapViewport(p_DblBuf, p_Globals, p_Images, MinimapOrigin);
    PROC_DrawMinimapSelectionArea(p_DblBuf, p_Globals, MinimapOrigin);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROC_DrawMinimapPicture(DBLBUF* p_DblBuf, GLOBALS* p_Globals, IMAGES* p_Images, FPOINT MinimapOrigin) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Minimap background.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DBLBUF_DrawPictureAt(
        p_DblBuf,
        &p_Images->HUD[1],
        (FPOINT) {
        p_DblBuf->ClientArea.right - p_Images->HUD[1].Bitmap.bmWidth,
            p_DblBuf->ClientArea.bottom - p_Images->HUD[1].Bitmap.bmHeight
    },
        0
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DBLBUF_DrawPictureAt(p_DblBuf, &p_Images->Minimap[p_Globals->usMapIndex], MinimapOrigin, 0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROC_DrawMinimapEntities(DBLBUF* p_DblBuf, GLOBALS* p_Globals, FPOINT MinimapOrigin) {
    HPEN hPen = NULL;
    HBRUSH hBrush = NULL;
    HGDIOBJ hPenTemp = NULL;
    HGDIOBJ hBrushTemp = NULL;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Render the entities.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ENTITY* p_Current = p_Globals->p_RootEntity;
    while (p_Current) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // The maps are square, and the locations are a ratio the map size!
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT EntityLocation = {
            (p_Current->CenterPoint.fX - p_Current->HalfSize.fX - p_Globals->fLateralTranslation) / MAP_SIZE,
            (p_Current->CenterPoint.fY - p_Current->HalfSize.fY - p_Globals->fVerticalTranslation) / MAP_SIZE
        };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT EntityDelta = {
            (p_Current->CenterPoint.fX + p_Current->HalfSize.fX - p_Globals->fLateralTranslation) / MAP_SIZE,
            (p_Current->CenterPoint.fY + p_Current->HalfSize.fY - p_Globals->fVerticalTranslation) / MAP_SIZE
        };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT MinimapEntityLocation = { MINIMAP_SIZE * EntityLocation.fX, MINIMAP_SIZE * EntityLocation.fY };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT MinimapEntityDelta = { MINIMAP_SIZE * EntityDelta.fX, MINIMAP_SIZE * EntityDelta.fY };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        switch (p_Current->usType) {
        case ENTITY_WORKER: {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Current->ubIsSelected) {
                hBrush = CreateSolidBrush(RGB(0, 255, 0));
                hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
            }
            else if (p_Current->ubIsHighlighted) {
                hBrush = CreateSolidBrush(RGB(100, 0, 100));
                hPen = CreatePen(PS_SOLID, 1, RGB(100, 0, 100));
            }
            else {
                hBrush = CreateSolidBrush(RGB(255, 255, 255));
                hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            hBrushTemp = SelectObject(p_DblBuf->hDCMem, hBrush);
            hPenTemp = SelectObject(p_DblBuf->hDCMem, hPen);
            break;
        }
        case ENTITY_COMMAND: {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Current->ubIsSelected) {
                hBrush = CreateSolidBrush(RGB(0, 255, 0));
                hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
            }
            else if (p_Current->ubIsHighlighted) {
                hBrush = CreateSolidBrush(RGB(100, 0, 100));
                hPen = CreatePen(PS_SOLID, 1, RGB(100, 0, 100));
            }
            else {
                hBrush = CreateSolidBrush(RGB(255, 0, 0));
                hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            hBrushTemp = SelectObject(p_DblBuf->hDCMem, hBrush);
            hPenTemp = SelectObject(p_DblBuf->hDCMem, hPen);
            break;
        }
        case ENTITY_MINERAL: {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Current->ubIsSelected) {
                hBrush = CreateSolidBrush(RGB(0, 255, 0));
                hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
            }
            else if (p_Current->ubIsHighlighted) {
                hBrush = CreateSolidBrush(RGB(100, 0, 100));
                hPen = CreatePen(PS_SOLID, 1, RGB(100, 0, 100));
            }
            else {
                hBrush = CreateSolidBrush(RGB(110, 175, 225));
                hPen = CreatePen(PS_SOLID, 1, RGB(110, 175, 225));
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            hBrushTemp = SelectObject(p_DblBuf->hDCMem, hBrush);
            hPenTemp = SelectObject(p_DblBuf->hDCMem, hPen);
            break;
        }
        case ENTITY_SUPPLY: {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Current->ubIsSelected) {
                hBrush = CreateSolidBrush(RGB(0, 255, 0));
                hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
            }
            else if (p_Current->ubIsHighlighted) {
                hBrush = CreateSolidBrush(RGB(100, 0, 100));
                hPen = CreatePen(PS_SOLID, 1, RGB(100, 0, 100));
            }
            else {
                hBrush = CreateSolidBrush(RGB(255, 255, 0));
                hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            hBrushTemp = SelectObject(p_DblBuf->hDCMem, hBrush);
            hPenTemp = SelectObject(p_DblBuf->hDCMem, hPen);
            break;
        }
        case ENTITY_REFINERY: {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Current->ubIsSelected) {
                hBrush = CreateSolidBrush(RGB(0, 255, 0));
                hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
            }
            else if (p_Current->ubIsHighlighted) {
                hBrush = CreateSolidBrush(RGB(100, 0, 100));
                hPen = CreatePen(PS_SOLID, 1, RGB(100, 0, 100));
            }
            else {
                hBrush = CreateSolidBrush(RGB(0, 175, 0));
                hPen = CreatePen(PS_SOLID, 1, RGB(0, 175, 0));
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            hBrushTemp = SelectObject(p_DblBuf->hDCMem, hBrush);
            hPenTemp = SelectObject(p_DblBuf->hDCMem, hPen);
            break;
        }
        default: {
            printf("PROC_DrawMinimapEntities(): Unknown entity type.\n");
        }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Rectangle(
            p_DblBuf->hDCMem,
            MinimapOrigin.fX + MinimapEntityLocation.fX,
            MinimapOrigin.fY + MinimapEntityLocation.fY,
            MinimapOrigin.fX + MinimapEntityDelta.fX,
            MinimapOrigin.fY + MinimapEntityDelta.fY
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SelectObject(p_DblBuf->hDCMem, hBrushTemp);
        SelectObject(p_DblBuf->hDCMem, hPenTemp);
        DeleteObject(hBrush);
        DeleteObject(hPen);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Current = (ENTITY*)p_Current->p_Next;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROC_PopulateViewportRelativity(FPOINT* p_ViewportRelativeLocation, FPOINT* p_ViewportRelativeDelta, FPOINT* p_ViewportActualLocation, FPOINT* p_ViewportActualDelta, DBLBUF* p_DblBuf, IMAGES* p_Images, GLOBALS* p_Globals) {
    FPOINT ViewportRelativeLocation = {
        (0.0f - p_Globals->fLateralTranslation) / p_Images->Terrain[p_Globals->usMapIndex].Bitmap.bmWidth,
        (0.0f - p_Globals->fVerticalTranslation) / p_Images->Terrain[p_Globals->usMapIndex].Bitmap.bmHeight
    };
    FPOINT ViewportRelativeDelta = {
        (float)p_DblBuf->ClientArea.right / p_Images->Terrain[p_Globals->usMapIndex].Bitmap.bmWidth,
        (float)p_DblBuf->ClientArea.bottom / p_Images->Terrain[p_Globals->usMapIndex].Bitmap.bmHeight
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT ViewportActualLocation = {
        ViewportRelativeLocation.fX * MINIMAP_SIZE,
        ViewportRelativeLocation.fY * MINIMAP_SIZE
    };
    FPOINT ViewportActualDelta = {
        ViewportRelativeDelta.fX * MINIMAP_SIZE,
        ViewportRelativeDelta.fY * MINIMAP_SIZE
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    *p_ViewportRelativeLocation = ViewportRelativeLocation;
    *p_ViewportRelativeDelta = ViewportRelativeDelta;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    *p_ViewportActualLocation = ViewportActualLocation;
    *p_ViewportActualDelta = ViewportActualDelta;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROC_PopulateMinimapDimensions(FPOINT* p_MinimapOrigin, FPOINT* p_MinimapDelta, DBLBUF* p_DblBuf, IMAGES* p_Images) {
    FPOINT MinimapOrigin = {
        p_DblBuf->ClientArea.right - p_Images->HUD[1].Bitmap.bmWidth + 15.0f,
        p_DblBuf->ClientArea.bottom - p_Images->HUD[1].Bitmap.bmHeight + 15.0f
    };
    FPOINT MinimapDelta = {
        MINIMAP_SIZE,
        MINIMAP_SIZE
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    *p_MinimapOrigin = MinimapOrigin;
    *p_MinimapDelta = MinimapDelta;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 __cdecl PROC_IsMinimapClicked(DBLBUF* p_DblBuf, IMAGES* p_Images, GLOBALS* p_Globals) {
    FPOINT MinimapOrigin;
    FPOINT MinimapDelta;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROC_PopulateMinimapDimensions(&MinimapOrigin, &MinimapDelta, p_DblBuf, p_Images);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_Globals->iMouseX >= MinimapOrigin.fX && p_Globals->iMouseX <= MinimapOrigin.fX + MinimapDelta.fX &&
        p_Globals->iMouseY >= MinimapOrigin.fY && p_Globals->iMouseY <= MinimapOrigin.fY + MinimapDelta.fY) {
        return 1;
    }
    else {
        return 0;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROC_AdjustMinimapViewport(GLOBALS* p_Globals, IMAGES* p_Images) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT MinimapOrigin;
    FPOINT MinimapDelta;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROC_PopulateMinimapDimensions(&MinimapOrigin, &MinimapDelta, p_DblBuf, p_Images);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT MinimapClickOrigin = {
        (p_Globals->iMouseX - MinimapOrigin.fX),
        (p_Globals->iMouseY - MinimapOrigin.fY),
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT ViewportRelativeLocation;
    FPOINT ViewportRelativeDelta;
    FPOINT ViewportActualLocation;
    FPOINT ViewportActualDelta;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROC_PopulateViewportRelativity(&ViewportRelativeLocation, &ViewportRelativeDelta, &ViewportActualLocation, &ViewportActualDelta, p_DblBuf, p_Images, p_Globals);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // If we moved the viewport to be centered around the minimap click origin, how far would we translate in the actual view?
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Capture the new actual and relative dimensions.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT NewViewportActualLocation = {
         MinimapClickOrigin.fX - (ViewportActualDelta.fX / 2),
         MinimapClickOrigin.fY - (ViewportActualDelta.fY / 2),
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
    if (NewViewportActualLocation.fX + ViewportActualDelta.fX >= MINIMAP_SIZE) {
        NewViewportActualLocation.fX = MINIMAP_SIZE - ViewportActualDelta.fX;
    }
    if (NewViewportActualLocation.fY + ViewportActualDelta.fY >= MINIMAP_SIZE) {
        NewViewportActualLocation.fY = MINIMAP_SIZE - ViewportActualDelta.fY;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Relative dimensions to upscale.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT NewViewportRelativeLocation = {
         NewViewportActualLocation.fX / MINIMAP_SIZE,
         NewViewportActualLocation.fY / MINIMAP_SIZE
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Actual upscaled dimensions.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT NewMainViewActualLocation = {
        NewViewportRelativeLocation.fX * MAP_SIZE,
        NewViewportRelativeLocation.fY * MAP_SIZE,
    };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Determine the delta and apply the change to the global translations and the entities on the screen.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    float fNewMainViewLateralTranslationDelta = -NewMainViewActualLocation.fX - p_Globals->fLateralTranslation;
    float fNewMainViewVerticalTranslationDelta = -NewMainViewActualLocation.fY - p_Globals->fVerticalTranslation;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->fLateralTranslation += fNewMainViewLateralTranslationDelta;
    p_Globals->fVerticalTranslation += fNewMainViewVerticalTranslationDelta;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROC_ApplyTranslations(fNewMainViewLateralTranslationDelta, fNewMainViewVerticalTranslationDelta, p_Globals, p_Images);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROC_DrawMinimapViewport(DBLBUF* p_DblBuf, GLOBALS* p_Globals, IMAGES* p_Images, FPOINT MinimapOrigin) {
    HPEN hPen = NULL;
    HGDIOBJ hPenTemp = NULL;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Render the viewport box.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT ViewportRelativeLocation;
    FPOINT ViewportRelativeDelta;
    FPOINT ViewportActualLocation;
    FPOINT ViewportActualDelta;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROC_PopulateViewportRelativity(&ViewportRelativeLocation, &ViewportRelativeDelta, &ViewportActualLocation, &ViewportActualDelta, p_DblBuf, p_Images, p_Globals);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    hPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
    hPenTemp = SelectObject(p_DblBuf->hDCMem, hPen);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Top, right, bottom, left.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MoveToEx(
        p_DblBuf->hDCMem,
        MinimapOrigin.fX + ViewportActualLocation.fX,
        MinimapOrigin.fY + ViewportActualLocation.fY,
        NULL
    );
    LineTo(
        p_DblBuf->hDCMem,
        MinimapOrigin.fX + ViewportActualLocation.fX + ViewportActualDelta.fX,
        MinimapOrigin.fY + ViewportActualLocation.fY
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MoveToEx(
        p_DblBuf->hDCMem,
        MinimapOrigin.fX + ViewportActualLocation.fX + ViewportActualDelta.fX,
        MinimapOrigin.fY + ViewportActualLocation.fY,
        NULL
    );
    LineTo(
        p_DblBuf->hDCMem,
        MinimapOrigin.fX + ViewportActualLocation.fX + ViewportActualDelta.fX,
        MinimapOrigin.fY + ViewportActualLocation.fY + ViewportActualDelta.fY
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MoveToEx(
        p_DblBuf->hDCMem,
        MinimapOrigin.fX + ViewportActualLocation.fX + ViewportActualDelta.fX,
        MinimapOrigin.fY + ViewportActualLocation.fY + ViewportActualDelta.fY,
        NULL
    );
    LineTo(
        p_DblBuf->hDCMem,
        MinimapOrigin.fX + ViewportActualLocation.fX,
        MinimapOrigin.fY + ViewportActualLocation.fY + ViewportActualDelta.fY
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MoveToEx(
        p_DblBuf->hDCMem,
        MinimapOrigin.fX + ViewportActualLocation.fX,
        MinimapOrigin.fY + ViewportActualLocation.fY + ViewportActualDelta.fY,
        NULL
    );
    LineTo(
        p_DblBuf->hDCMem,
        MinimapOrigin.fX + ViewportActualLocation.fX,
        MinimapOrigin.fY + ViewportActualLocation.fY
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SelectObject(p_DblBuf->hDCMem, hPenTemp);
    DeleteObject(hPen);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROC_DrawMinimapSelectionArea(DBLBUF* p_DblBuf, GLOBALS* p_Globals, FPOINT MinimapOrigin) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Prevent rendering of the minimap selection area during the initial mouse down coordinates.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_Globals->iCaptureCurrentX > 0 || p_Globals->iCaptureCurrentY > 0) {
        HPEN hPen = NULL;
        HGDIOBJ hPenTemp = NULL;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Render the selection area. Note: lateral and vertical translations are already inclusive to the selection coordinates.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT SelectionAreaStartLocation = {
            p_Globals->iCaptureStartX / MAP_SIZE,
            p_Globals->iCaptureStartY / MAP_SIZE
        };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT SelectionAreaEndLocation = {
            p_Globals->iCaptureCurrentX / MAP_SIZE,
            p_Globals->iCaptureCurrentY / MAP_SIZE
        };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT MinimapSelectionAreaStartLocation = { MINIMAP_SIZE * SelectionAreaStartLocation.fX, MINIMAP_SIZE * SelectionAreaStartLocation.fY };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT MinimapSelectionAreaEndLocation = { MINIMAP_SIZE * SelectionAreaEndLocation.fX, MINIMAP_SIZE * SelectionAreaEndLocation.fY };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
        hPenTemp = SelectObject(p_DblBuf->hDCMem, hPen);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Top, right, bottom, left.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        MoveToEx(
            p_DblBuf->hDCMem,
            MinimapOrigin.fX + MinimapSelectionAreaStartLocation.fX,
            MinimapOrigin.fY + MinimapSelectionAreaStartLocation.fY,
            NULL
        );
        LineTo(
            p_DblBuf->hDCMem,
            MinimapOrigin.fX + MinimapSelectionAreaEndLocation.fX,
            MinimapOrigin.fY + MinimapSelectionAreaStartLocation.fY
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        MoveToEx(
            p_DblBuf->hDCMem,
            MinimapOrigin.fX + MinimapSelectionAreaEndLocation.fX,
            MinimapOrigin.fY + MinimapSelectionAreaStartLocation.fY,
            NULL
        );
        LineTo(
            p_DblBuf->hDCMem,
            MinimapOrigin.fX + MinimapSelectionAreaEndLocation.fX,
            MinimapOrigin.fY + MinimapSelectionAreaEndLocation.fY
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        MoveToEx(
            p_DblBuf->hDCMem,
            MinimapOrigin.fX + MinimapSelectionAreaEndLocation.fX,
            MinimapOrigin.fY + MinimapSelectionAreaEndLocation.fY,
            NULL
        );
        LineTo(
            p_DblBuf->hDCMem,
            MinimapOrigin.fX + MinimapSelectionAreaStartLocation.fX,
            MinimapOrigin.fY + MinimapSelectionAreaEndLocation.fY
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        MoveToEx(
            p_DblBuf->hDCMem,
            MinimapOrigin.fX + MinimapSelectionAreaStartLocation.fX,
            MinimapOrigin.fY + MinimapSelectionAreaEndLocation.fY,
            NULL
        );
        LineTo(
            p_DblBuf->hDCMem,
            MinimapOrigin.fX + MinimapSelectionAreaStartLocation.fX,
            MinimapOrigin.fY + MinimapSelectionAreaStartLocation.fY
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SelectObject(p_DblBuf->hDCMem, hPenTemp);
        DeleteObject(hPen);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROC_HandleHud(DBLBUF* p_DblBuf, GLOBALS* p_Globals, IMAGES* p_Images, CARD* p_Card, MENU* p_Menu) {
    PROC_DrawResourceBar(p_DblBuf, p_Globals, p_Images, p_Menu);
    PROC_DrawTaskbar(p_DblBuf, p_Globals, p_Images, p_Card, p_Menu);
    PROC_HandleMinimap(p_DblBuf, p_Globals, p_Images);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROC_DrawBuildLimits(DBLBUF* p_DblBuf, ENTITY* p_Entity, IMAGES* p_Images) {
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    HGDIOBJ hPenTemp = SelectObject(p_DblBuf->hDCMem, hPen);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT Buffer = { p_Images->Worker[0].Bitmap.bmWidth, p_Images->Worker[0].Bitmap.bmHeight };
    FPOINT Location = { p_Entity->Location.fX, p_Entity->Location.fY };
    FPOINT Delta = { p_Entity->Size.fX, p_Entity->Size.fY };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Top, right, bottom, left.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MoveToEx(p_DblBuf->hDCMem, Location.fX - Buffer.fX, Location.fY - Buffer.fY, NULL);
    LineTo(p_DblBuf->hDCMem, Location.fX + Delta.fX + Buffer.fX, Location.fY - Buffer.fY);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MoveToEx(p_DblBuf->hDCMem, Location.fX + Delta.fX + Buffer.fX, Location.fY - Buffer.fY, NULL);
    LineTo(p_DblBuf->hDCMem, Location.fX + Delta.fX + Buffer.fX, Location.fY + Delta.fY + Buffer.fY);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MoveToEx(p_DblBuf->hDCMem, Location.fX + Delta.fX + Buffer.fX, Location.fY + Delta.fY + Buffer.fY, NULL);
    LineTo(p_DblBuf->hDCMem, Location.fX - Buffer.fX, Location.fY + Delta.fY + Buffer.fY);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MoveToEx(p_DblBuf->hDCMem, Location.fX - Buffer.fX, Location.fY + Delta.fY + Buffer.fY, NULL);
    LineTo(p_DblBuf->hDCMem, Location.fX - Buffer.fX, Location.fY - Buffer.fY);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SelectObject(p_DblBuf->hDCMem, hPenTemp);
    DeleteObject(hPen);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PROC_DrawBuildType(DBLBUF* p_DblBuf, GLOBALS* p_Globals, IMAGES* p_Images, MENU* p_Menu) {
    if (p_Globals->ubCreate) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Draw the red barriers that display the build limits for all entities.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY* p_Current = p_Globals->p_RootEntity;
        while (p_Current) {
            PROC_DrawBuildLimits(p_DblBuf, p_Current, p_Images);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Current = (ENTITY*)p_Current->p_Next;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT Location = { 0.0f, 0.0f };
        FPOINT Delta = { 0.0f, 0.0f };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Draw the buildings image, then the placement restrictions.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        switch (p_Globals->usBuildType) {
        case ENTITY_COMMAND: {
            Location.fX = p_Globals->iMouseX - (p_Images->Command[0].Bitmap.bmWidth >> 1);
            Location.fY = p_Globals->iMouseY - (p_Images->Command[0].Bitmap.bmHeight >> 1);
            Delta.fX = p_Images->Command[0].Bitmap.bmWidth;
            Delta.fY = p_Images->Command[0].Bitmap.bmHeight;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            DBLBUF_DrawPictureAt(p_DblBuf, &p_Images->Command[0], Location, p_Menu->ubEnableMasking);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            break;
        }
        case ENTITY_WORKER: {
            Location.fX = p_Globals->iMouseX - (p_Images->Worker[0].Bitmap.bmWidth >> 1);
            Location.fY = p_Globals->iMouseY - (p_Images->Worker[0].Bitmap.bmHeight >> 1);
            Delta.fX = p_Images->Worker[0].Bitmap.bmWidth;
            Delta.fY = p_Images->Worker[0].Bitmap.bmHeight;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            DBLBUF_DrawPictureAt(p_DblBuf, &p_Images->Worker[0], Location, p_Menu->ubEnableMasking);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            break;
        }
        case ENTITY_MINERAL: {
            Location.fX = p_Globals->iMouseX - (p_Images->Mineral[0].Bitmap.bmWidth >> 1);
            Location.fY = p_Globals->iMouseY - (p_Images->Mineral[0].Bitmap.bmHeight >> 1);
            Delta.fX = p_Images->Mineral[0].Bitmap.bmWidth;
            Delta.fY = p_Images->Mineral[0].Bitmap.bmHeight;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            DBLBUF_DrawPictureAt(p_DblBuf, &p_Images->Mineral[0], Location, p_Menu->ubEnableMasking);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            break;
        }
        case ENTITY_SUPPLY: {
            Location.fX = p_Globals->iMouseX - (p_Images->Supply[0].Bitmap.bmWidth >> 1);
            Location.fY = p_Globals->iMouseY - (p_Images->Supply[0].Bitmap.bmHeight >> 1);
            Delta.fX = p_Images->Supply[0].Bitmap.bmWidth;
            Delta.fY = p_Images->Supply[0].Bitmap.bmHeight;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            DBLBUF_DrawPictureAt(p_DblBuf, &p_Images->Supply[0], Location, p_Menu->ubEnableMasking);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            break;
        }
        case ENTITY_REFINERY: {
            Location.fX = p_Globals->iMouseX - (p_Images->Refinery[0].Bitmap.bmWidth >> 1);
            Location.fY = p_Globals->iMouseY - (p_Images->Refinery[0].Bitmap.bmHeight >> 1);
            Delta.fX = p_Images->Refinery[0].Bitmap.bmWidth;
            Delta.fY = p_Images->Refinery[0].Bitmap.bmHeight;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            DBLBUF_DrawPictureAt(p_DblBuf, &p_Images->Refinery[0], Location, p_Menu->ubEnableMasking);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            break;
        }
        default: {
            printf("PROC_DrawBuildType(): Unknown entity type.\n");
        }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
        HGDIOBJ hPenTemp = SelectObject(p_DblBuf->hDCMem, hPen);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Top, right, bottom, left.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        MoveToEx(p_DblBuf->hDCMem, Location.fX, Location.fY, NULL);
        LineTo(p_DblBuf->hDCMem, Location.fX + Delta.fX, Location.fY);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        MoveToEx(p_DblBuf->hDCMem, Location.fX + Delta.fX, Location.fY, NULL);
        LineTo(p_DblBuf->hDCMem, Location.fX + Delta.fX, Location.fY + Delta.fY);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        MoveToEx(p_DblBuf->hDCMem, Location.fX + Delta.fX, Location.fY + Delta.fY, NULL);
        LineTo(p_DblBuf->hDCMem, Location.fX, Location.fY + Delta.fY);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        MoveToEx(p_DblBuf->hDCMem, Location.fX, Location.fY + Delta.fY, NULL);
        LineTo(p_DblBuf->hDCMem, Location.fX, Location.fY);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SelectObject(p_DblBuf->hDCMem, hPenTemp);
        DeleteObject(hPen);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
