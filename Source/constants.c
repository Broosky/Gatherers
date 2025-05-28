/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/constants.h"
#include "../Headers/enums.h"
#include "../Headers/globals.h"
#include "../Headers/log.h"
#include "../Headers/misc.h"
#include <stdlib.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl CONSTANTS_Zero(CONSTANTS_T* p_Constants) {
    ZeroMemory(p_Constants, sizeof(CONSTANTS_T));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CONSTANTS_T* __cdecl CONSTANTS_Create(GLOBALS_T* p_Globals, LOG_T* p_Log) {
    size_t stAllocation = sizeof(CONSTANTS_T);
    CONSTANTS_T* p_Constants = malloc(stAllocation);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!p_Constants) {
        MISC_WriteOutParams(p_Log, LOG_SEVERITY_FATAL, "CONSTANTS_Create(): Malloc failed for size: %zu bytes\n", stAllocation);
        return NULL;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CONSTANTS_Zero(p_Constants);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->stAllocations += stAllocation;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return p_Constants;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl CONSTANTS_Init(CONSTANTS_T* p_Constants) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    snprintf(p_Constants->szAppName, sizeof(p_Constants->szAppName), "Gatherers (C) V12.0");
    snprintf(p_Constants->szDefaultSettingsFile, sizeof(p_Constants->szDefaultSettingsFile), "Gatherers.cfg");
    snprintf(p_Constants->szDefaultLogFile, sizeof(p_Constants->szDefaultLogFile), "Gatherers.log");
	snprintf(p_Constants->szDefaultSaveFile, sizeof(p_Constants->szDefaultSaveFile), "Gatherers.sav");
    p_Constants->fPi = 3.1416f;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // List sorting constants:
    p_Constants->usWorkersToFront = SORT_ORDER_WORKERS_TO_FRONT;
    p_Constants->usMineralsToFront = SORT_ORDER_MINERALS_TO_FRONT;
    p_Constants->usCommandsToFront = SORT_ORDER_COMMANDS_TO_FRONT;
    p_Constants->usSuppliesToFront = SORT_ORDER_SUPPLIES_TO_FRONT;
    p_Constants->usRefineriesToFront = SORT_ORDER_REFINERIES_TO_FRONT;
    p_Constants->usObstaclesToFront = SORT_ORDER_OBSTACLES_TO_FRONT;
    // Entity type constants:
    p_Constants->usEntityNone = ENTITY_TYPE_NONE;
    p_Constants->usEntityWorker = ENTITY_TYPE_WORKER;
    p_Constants->usEntityMineral = ENTITY_TYPE_MINERAL;
    p_Constants->usEntityCommand = ENTITY_TYPE_COMMAND;
    p_Constants->usEntitySupply = ENTITY_TYPE_SUPPLY;
    p_Constants->usEntityRefinery = ENTITY_TYPE_REFINERY;
    p_Constants->usEntityObstacle = ENTITY_TYPE_OBSTACLE;
    // Animation state constants:
    p_Constants->usAnimateWorkerNormal = ANIMATION_STATE_WORKER_NORMAL;
    p_Constants->usAnimateWorkerMinerals = ANIMATION_STATE_WORKER_MINERALS;
    p_Constants->usAnimateWorkerGas = ANIMATION_STATE_WORKER_GAS;
    p_Constants->usAnimateCommandNormal = ANIMATION_STATE_COMMAND_NORMAL;
    p_Constants->usAnimateCommandReceive = ANIMATION_STATE_COMMAND_RECEIVE;
    p_Constants->usAnimateCommandCreating = ANIMATION_STATE_COMMAND_CREATING;
    p_Constants->usAnimateSupplyNormal = ANIMATION_STATE_SUPPLY_NORMAL;
    // Matrix rendering constants:
    p_Constants->usTransformRotate = TRANSFORM_TYPE_ROTATE;
    p_Constants->usTransformScale = TRANSFORM_TYPE_SCALE;
    // Message type constants:
    p_Constants->usMessageGainMinerals = MESSAGE_TYPE_GAIN_MINERALS;
    p_Constants->usMessageGainGas = MESSAGE_TYPE_GAIN_GAS;
    p_Constants->usMessageBonusMinerals = MESSAGE_TYPE_BONUS_MINERALS;
    p_Constants->usMessageBonusGas = MESSAGE_TYPE_BONUS_GAS;
    p_Constants->usMessageGeneralWarning = MESSAGE_TYPE_GENERAL_WARNING;
    // Command card constants:
    p_Constants->usCardBlank = COMMAND_CARD_BLANK;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl CONSTANTS_Kill(CONSTANTS_T** pp_Constants, GLOBALS_T* p_Globals) {
    CONSTANTS_T* p_Constants = *pp_Constants;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_Constants) {
        free(p_Constants);
        p_Globals->stAllocations -= sizeof(CONSTANTS_T);
        *pp_Constants = NULL;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
