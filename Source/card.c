/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/assets.h"
#include "../Headers/card.h"
#include "../Headers/constants.h"
#include "../Headers/entity.h"
#include "../Headers/enums.h"
#include "../Headers/globals.h"
#include "../Headers/log.h"
#include "../Headers/misc.h"
#include <stdlib.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl CARD_Zero(CARD_T* p_Card) {
    ZeroMemory(p_Card, sizeof(CARD_T));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CARD_T* __cdecl CARD_Create(GLOBALS_T* p_Globals, ASSETS_T* p_Assets, LOG_T* p_Log) {
    size_t stAllocation = sizeof(CARD_T);
    CARD_T* p_Card = malloc(stAllocation);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!p_Card) {
        MISC_WriteOutParams(p_Log, LOG_SEVERITY_FATAL, "CARD_Create(): Malloc failed for size: %zu bytes\n", stAllocation);
        return NULL;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->stAllocations += stAllocation;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CARD_Zero(p_Card);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Card->usState = COMMAND_CARD_BLANK;
    p_Card->p_Picture = &p_Assets->Card[0];
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return p_Card;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl CARD_Kill(CARD_T** pp_Card, GLOBALS_T* p_Globals) {
    CARD_T* p_Card = *pp_Card;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_Card) {
        PICTURE_Kill(p_Card->p_Picture);
        free(p_Card);
        p_Globals->stAllocations -= sizeof(CARD_T);
        *pp_Card = NULL;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl CARD_EvaluateSelected(CARD_T* p_Card, GLOBALS_T* p_Globals, ASSETS_T* p_Assets) {
    USHORT usStructureCount = 0;
    USHORT usWorkerCount = 0;
    ENTITY_T* p_Current = p_Globals->p_RootEntity;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (p_Current) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Keep track of how many structures are selected.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Current->ubIsSelected && p_Current->eType == ENTITY_TYPE_COMMAND) {
            usStructureCount++;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Keep track of how many movable entities are selected.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Current->ubIsSelected && p_Current->ubIsMovable) {
            usWorkerCount++;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Current = (ENTITY_T*)p_Current->p_Next;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Display the correct command card for what was evaluated.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // We have selected one worker. (Allow the worker to build something.)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (usWorkerCount == 1) {
        p_Card->p_Picture = &p_Assets->Card[3];
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // We have selected multiple workers. (Only allow move and stop.)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else if (usWorkerCount > 1) {
        p_Card->p_Picture = &p_Assets->Card[1];
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // We have selected one structure. (Allow the building to create something.)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else if (usStructureCount == 1) {
        p_Card->p_Picture = &p_Assets->Card[2];
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // We have selected multiple structures. (Do not allow anything at all.)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else if (usStructureCount > 1) {
        p_Card->p_Picture = &p_Assets->Card[0];
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // We have selected nothing of importance.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else {
        p_Card->p_Picture = &p_Assets->Card[0];
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
