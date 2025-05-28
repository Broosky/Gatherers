/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/entity.h"
#include "../Headers/globals.h"
#include "../Headers/log.h"
#include "../Headers/misc.h"
#include "../Headers/settings.h"
#include "../Headers/timebase.h"
#include <stdint.h>
#include <stdlib.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_LINE_LENGTH 256
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl SETTINGS_Zero(SETTINGS_T* p_Settings) {
    ZeroMemory(p_Settings, sizeof(SETTINGS_T));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SETTINGS_T* __cdecl SETTINGS_Create(GLOBALS_T* p_Globals, LOG_T* p_Log) {
    size_t stAllocation = sizeof(SETTINGS_T);
    SETTINGS_T* p_Settings = malloc(stAllocation);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!p_Settings) {
        MISC_WriteOutParams(p_Log, LOG_SEVERITY_FATAL, "SETTINGS_Create(): Malloc failed for size : %zu bytes\n", stAllocation);
        return NULL;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->stAllocations += stAllocation;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SETTINGS_Zero(p_Settings);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return p_Settings;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 __cdecl SETTINGS_InitFromFile(SETTINGS_T* p_Settings, LOG_T* p_Log, const char* const p_szFileName) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FILE* p_SettingsFile = fopen(p_szFileName, "r");
    if (!p_SettingsFile) {
        MISC_WriteOut(p_Log, LOG_SEVERITY_FATAL, "SETTINGS_InitFromFile(): Failed to open settings file.\n");
        return 0;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    char szLine[MAX_LINE_LENGTH];
    while (fgets(szLine, sizeof(szLine), p_SettingsFile)) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Skip empty lines, lines with only whitespace, or commented lines.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (szLine[0] == '\0' || szLine[0] == '\n' || szLine[0] == '#' || strspn(szLine, " \t\n\r") == strlen(szLine)) {
            continue;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Remove comments...
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        char* p_szTrimmedLine = MISC_Trim(szLine);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_szTrimmedLine[0] == '\0' || p_szTrimmedLine[0] == '#') {
            continue;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        char* p_cComment = strchr(p_szTrimmedLine, '#');
        if (p_cComment) {
            *p_cComment = '\0';
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Find '=' character to separate key and value. Skip lines without the equals.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        char* p_cEquals = strchr(p_szTrimmedLine, '=');
        if (!p_cEquals) continue;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Null-terminate key part, trim and get the key and value.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        *p_cEquals = '\0';
        char* p_szKey = MISC_Trim(p_szTrimmedLine);
        char* p_szValue = MISC_Trim(p_cEquals + 1);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Match settings and assign values.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // [BuildingCosts]
        if (SETTINGS_Match(p_szKey, "SUPPLY_FROM_COMMAND"))                         p_Settings->usSupplyFromCommand = MISC_ClampU16(atoi(p_szValue), 1, 20);
        else if (SETTINGS_Match(p_szKey, "COST_OF_COMMAND_MINERALS"))               p_Settings->usCostOfCommandMinerals = MISC_ClampU16(atoi(p_szValue), 1, 200);
        else if (SETTINGS_Match(p_szKey, "COST_OF_COMMAND_GAS"))                    p_Settings->usCostOfCommandGas = MISC_ClampU16(atoi(p_szValue), 1, 200);
        else if (SETTINGS_Match(p_szKey, "SUPPLY_FROM_DEPOT"))                      p_Settings->usSupplyFromDepot = MISC_ClampU16(atoi(p_szValue), 1, 5);
        else if (SETTINGS_Match(p_szKey, "COST_OF_DEPOT_MINERALS"))                 p_Settings->usCostOfDepotMinerals = MISC_ClampU16(atoi(p_szValue), 75, 150);
        else if (SETTINGS_Match(p_szKey, "COST_OF_DEPOT_GAS"))                      p_Settings->usCostOfDepotGas = MISC_ClampU16(atoi(p_szValue), 75, 150);
        else if (SETTINGS_Match(p_szKey, "SUPPLY_USED_WORKER"))                     p_Settings->usSupplyUsedWorker = MISC_ClampU16(atoi(p_szValue), 1, 5);
        else if (SETTINGS_Match(p_szKey, "COST_OF_WORKER_MINERALS"))                p_Settings->usCostOfWorkerMinerals = MISC_ClampU16(atoi(p_szValue), 1, 50);
        else if (SETTINGS_Match(p_szKey, "COST_OF_WORKER_GAS"))                     p_Settings->usCostOfWorkerGas = MISC_ClampU16(atoi(p_szValue), 1, 50);
        else if (SETTINGS_Match(p_szKey, "COST_OF_REFINERY_MINERALS"))              p_Settings->usCostOfRefineryMinerals = MISC_ClampU16(atoi(p_szValue), 1, 100);
        else if (SETTINGS_Match(p_szKey, "COST_OF_REFINERY_GAS"))                   p_Settings->usCostOfRefineryGas = MISC_ClampU16(atoi(p_szValue), 0, 100);
        // [AppSettings]
        else if (SETTINGS_Match(p_szKey, "ENGINE_FPS"))                             p_Settings->fEngineFPS = MISC_ClampF32(strtof(p_szValue, NULL), 1.0f, 240.0f);
        else if (SETTINGS_Match(p_szKey, "ANIMATION_STATE_FPS"))                    p_Settings->fAnimateFPS = MISC_ClampF32(strtof(p_szValue, NULL), 1.0f, 60.0f);
        else if (SETTINGS_Match(p_szKey, "MAINTENANCE_TIMEBASE"))                   p_Settings->fMaintenanceTimebase = MISC_ClampF32(strtof(p_szValue, NULL), 0.1f, 1.0f);
        else if (SETTINGS_Match(p_szKey, "INITIAL_CLIENT_WIDTH"))                   p_Settings->usInitialClientWidth = MISC_ClampU16(atoi(p_szValue), 640, 1650);
        else if (SETTINGS_Match(p_szKey, "INITIAL_CLIENT_HEIGHT"))                  p_Settings->usInitialClientHeight = MISC_ClampU16(atoi(p_szValue), 480, 850);
        else if (SETTINGS_Match(p_szKey, "TRANSLATION_STEP_AMOUNT"))                p_Settings->fTranslationStepAmount = MISC_ClampF32(strtof(p_szValue, NULL), 1.0f, 50.0f);
        else if (SETTINGS_Match(p_szKey, "MINIMAP_SIZE"))                           p_Settings->fMinimapSize = MISC_ClampF32(strtof(p_szValue, NULL), 220.0f, 220.0f);
        else if (SETTINGS_Match(p_szKey, "MAP_SIZE"))                               p_Settings->fMapSize = MISC_ClampF32(strtof(p_szValue, NULL), 2048.0, 5120.0f);
        else if (SETTINGS_Match(p_szKey, "WINDOW_REGION_ROUNDING"))                 p_Settings->usWindowRegionRounding = MISC_ClampU16(atoi(p_szValue), 0, 100);
        else if (SETTINGS_Match(p_szKey, "PATH_BOUNDING_BOX_PADDING"))              p_Settings->fPathBoundingBoxPadding = MISC_ClampF32(strtof(p_szValue, NULL), 1.0f, 200.0f);
        // [AllocatorSettings]
        else if (SETTINGS_Match(p_szKey, "HEAP_ALLOCATOR_MAX_ENTITIES"))            p_Settings->usHeapAllocatorMaxEntities = MISC_ClampU16(atoi(p_szValue), 16, 2048);
        else if (SETTINGS_Match(p_szKey, "HEAP_ALLOCATOR_MAX_PATH_POINTS"))         p_Settings->usHeapAllocatorMaxPathPoints = MISC_ClampU16(atoi(p_szValue), 16, 2048);
        else if (SETTINGS_Match(p_szKey, "HEAP_ALLOCATOR_MAX_PATH_OBSTACLES"))      p_Settings->usHeapAllocatorMaxPathObstacles = MISC_ClampU16(atoi(p_szValue), 16, 2048);
        else if (SETTINGS_Match(p_szKey, "HEAP_ALLOCATOR_MAX_MESSAGES"))            p_Settings->usHeapAllocatorMaxMessages = MISC_ClampU16(atoi(p_szValue), 16, 2048);
        else if (SETTINGS_Match(p_szKey, "HEAP_ALLOCATOR_MAX_AI_CLOSEST"))          p_Settings->usHeapAllocatorAiClosest = MISC_ClampU16(atoi(p_szValue), 16, 2048);
        else if (SETTINGS_Match(p_szKey, "HEAP_ALLOCATOR_MAX_FPOINT"))              p_Settings->usHeapAllocatorFPoint = MISC_ClampU16(atoi(p_szValue), 16, 2048);
        else if (SETTINGS_Match(p_szKey, "HEAP_ALLOCATOR_MAX_FRECT"))               p_Settings->usHeapAllocatorFRect = MISC_ClampU16(atoi(p_szValue), 16, 2048);
        // [AISettings]
        else if (SETTINGS_Match(p_szKey, "AI_MAX_SEARCH_RANGE"))                    p_Settings->fAiMaxSearchRange = MISC_ClampF32(strtof(p_szValue, NULL), 1.0f, 4250.0f);
        else if (SETTINGS_Match(p_szKey, "SUFFICIENTLY_CLOSE"))                     p_Settings->fSufficientlyClose = MISC_ClampF32(strtof(p_szValue, NULL), 3.0f, 10.0f);
        else if (SETTINGS_Match(p_szKey, "COLLISION_BUFFER"))                       p_Settings->fCollisionBuffer = MISC_ClampF32(strtof(p_szValue, NULL), 3.0f, 10.0f);
        // [MovementSettings]
        else if (SETTINGS_Match(p_szKey, "WORKER_MOVE_SPEED"))                      p_Settings->fWorkerMoveSpeed = MISC_ClampF32(strtof(p_szValue, NULL), 1.0f, 10.0f);
        else if (SETTINGS_Match(p_szKey, "MINERAL_MOVE_SPEED"))                     p_Settings->fMineralMoveSpeed = MISC_ClampF32(strtof(p_szValue, NULL), 1.0f, 10.0f);
        else if (SETTINGS_Match(p_szKey, "COMMAND_MOVE_SPEED"))                     p_Settings->fCommandMoveSpeed = MISC_ClampF32(strtof(p_szValue, NULL), 1.0f, 10.0f);
        else if (SETTINGS_Match(p_szKey, "SUPPLY_MOVE_SPEED"))                      p_Settings->fSupplyMoveSpeed = MISC_ClampF32(strtof(p_szValue, NULL), 1.0f, 10.0f);
        else if (SETTINGS_Match(p_szKey, "REFINERY_MOVE_SPEED"))                    p_Settings->fRefineryMoveSpeed = MISC_ClampF32(strtof(p_szValue, NULL), 1.0f, 10.0f);
        else if (SETTINGS_Match(p_szKey, "WORKER_MINERAL_DIVISOR"))                 p_Settings->fWorkerMineralDivisor = MISC_ClampF32(strtof(p_szValue, NULL), 1.0f, 10.0f);
        else if (SETTINGS_Match(p_szKey, "WORKER_GAS_DIVISOR"))                     p_Settings->fWorkerGasDivisor = MISC_ClampF32(strtof(p_szValue, NULL), 1.0f, 10.0f);
        // [PauseSettings]
        else if (SETTINGS_Match(p_szKey, "MINERALS_PAUSE"))                         p_Settings->fMineralsPause = MISC_ClampF32(strtof(p_szValue, NULL), 1.0f, 10000.0f);
        else if (SETTINGS_Match(p_szKey, "MINERALS_COMMAND_PAUSE"))                 p_Settings->fMineralsCommandPause = MISC_ClampF32(strtof(p_szValue, NULL), 1.0f, 10000.0f);
        else if (SETTINGS_Match(p_szKey, "REFINERY_PAUSE"))                         p_Settings->fRefineryPause = MISC_ClampF32(strtof(p_szValue, NULL), 1.0f, 10000.0f);
        else if (SETTINGS_Match(p_szKey, "REFINERY_COMMAND_PAUSE"))                 p_Settings->fRefineryCommandPause = MISC_ClampF32(strtof(p_szValue, NULL), 1.0f, 10000.0f);
        // [ResourceSettings]
        else if (SETTINGS_Match(p_szKey, "MINERALS_PER_GRAB"))                      p_Settings->usMineralsPerGrab = MISC_ClampU16(atoi(p_szValue), 1, 50);
        else if (SETTINGS_Match(p_szKey, "MINERALS_MULTIPLIER"))                    p_Settings->usMineralsMultiplier = MISC_ClampU16(atoi(p_szValue), 1, 10);
        else if (SETTINGS_Match(p_szKey, "GAS_PER_GRAB"))                           p_Settings->usGasPerGrab = MISC_ClampU16(atoi(p_szValue), 1, 50);
        else if (SETTINGS_Match(p_szKey, "GAS_MULTIPLIER"))                         p_Settings->usGasMultiplier = MISC_ClampU16(atoi(p_szValue), 1, 10);
        else if (SETTINGS_Match(p_szKey, "MINERALS_ON_CREATION"))                   p_Settings->uiMineralsOnCreation = MISC_ClampU32(atoi(p_szValue), 1, 100000);
        else if (SETTINGS_Match(p_szKey, "GAS_ON_CREATION"))                        p_Settings->uiGasOnCreation = MISC_ClampU32(atoi(p_szValue), 1, 100000);
        // Unknown
        else {
            MISC_WriteOutParams(p_Log, LOG_SEVERITY_WARNING, "SETTINGS_InitFromFile(): Unknown setting: %s = %s\n", p_szKey, p_szValue);
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SETTINGS_Print(p_Settings, p_Log);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fclose(p_SettingsFile);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl SETTINGS_Print(SETTINGS_T* p_Settings, LOG_T* p_Log) {
    MISC_WriteOut(p_Log, LOG_SEVERITY_TRACE, "--------------------\n");
    MISC_WriteOutParams(p_Log,
        LOG_SEVERITY_TRACE,
        "[BuildingSettings]\n"
        "    SUPPLY_FROM_COMMAND=%u\n"
        "    COST_OF_COMMAND_MINERALS=%u\n"
        "    COST_OF_COMMAND_GAS=%u\n"
        "    SUPPLY_FROM_DEPOT=%u\n"
        "    COST_OF_DEPOT_MINERALS=%u\n"
        "    COST_OF_DEPOT_GAS=%u\n"
        "    SUPPLY_USED_WORKER=%u\n"
        "    COST_OF_WORKER_MINERALS=%u\n"
        "    COST_OF_WORKER_GAS=%u\n"
        "    COST_OF_REFINERY_MINERALS=%u\n"
        "    COST_OF_REFINERY_GAS=%u\n\n"
        "[AppSettings]\n"
        "    ENGINE_FPS=%.2f\n"
        "    ANIMATION_STATE_FPS=%.2f\n"
        "    MAINTENANCE_TIMEBASE=%.2f\n"
        "    INITIAL_CLIENT_WIDTH=%u\n"
        "    INITIAL_CLIENT_HEIGHT=%u\n"
        "    TRANSLATION_STEP_AMOUNT=%.2f\n"
        "    MINIMAP_SIZE=%.2f\n"
        "    MAP_SIZE=%.2f\n"
        "    WINDOW_REGION_ROUNDING=%u\n"
        "    PATH_BOUNDING_BOX_PADDING=%.2f\n\n"
        "[AllocatorSettings]\n"
        "    HEAP_ALLOCATOR_MAX_ENTITIES=%u\n"
        "    HEAP_ALLOCATOR_MAX_PATH_POINTS=%u\n"
        "    HEAP_ALLOCATOR_MAX_PATH_OBSTACLES=%u\n"
        "    HEAP_ALLOCATOR_MAX_AI_CLOSEST=%u\n"
        "    HEAP_ALLOCATOR_MAX_FPOINT=%u\n"
        "    HEAP_ALLOCATOR_MAX_FRECT=%u\n"
        "    HEAP_ALLOCATOR_MAX_MESSAGES=%u\n\n"
        "[AISettings]\n"
        "    AI_MAX_SEARCH_RANGE=%.2f\n"
        "    SUFFICIENTLY_CLOSE=%.2f\n"
        "    COLLISION_BUFFER=%.2f\n\n"
        "[MovementSettings]\n"
        "    WORKER_MOVE_SPEED=%.2f\n"
        "    MINERAL_MOVE_SPEED=%.2f\n"
        "    COMMAND_MOVE_SPEED=%.2f\n"
        "    SUPPLY_MOVE_SPEED=%.2f\n"
        "    REFINERY_MOVE_SPEED=%.2f\n"
        "    WORKER_MINERAL_DIVISOR=%.2f\n"
        "    WORKER_GAS_DIVISOR=%.2f\n\n"
        "[PauseSettings]\n"
        "    MINERALS_PAUSE=%.2f\n"
        "    MINERALS_COMMAND_PAUSE=%.2f\n"
        "    REFINERY_PAUSE=%.2f\n"
        "    REFINERY_COMMAND_PAUSE=%.2f\n\n"
        "[ResourceSettings]\n"
        "    MINERALS_PER_GRAB=%u\n"
        "    MINERALS_MULTIPLIER=%u\n"
        "    GAS_PER_GRAB=%u\n"
        "    GAS_MULTIPLIER=%u\n"
        "    MINERALS_ON_CREATION=%u\n"
        "    GAS_ON_CREATION=%u\n",
        // [BuildingSettings]
        p_Settings->usSupplyFromCommand,
        p_Settings->usCostOfCommandMinerals,
        p_Settings->usCostOfCommandGas,
        p_Settings->usSupplyFromDepot,
        p_Settings->usCostOfDepotMinerals,
        p_Settings->usCostOfDepotGas,
        p_Settings->usSupplyUsedWorker,
        p_Settings->usCostOfWorkerMinerals,
        p_Settings->usCostOfWorkerGas,
        p_Settings->usCostOfRefineryMinerals,
        p_Settings->usCostOfRefineryGas,
        // [AppSettings]
        p_Settings->fEngineFPS,
        p_Settings->fAnimateFPS,
        p_Settings->fMaintenanceTimebase,
        p_Settings->usInitialClientWidth,
        p_Settings->usInitialClientHeight,
        p_Settings->fTranslationStepAmount,
        p_Settings->fMinimapSize,
        p_Settings->fMapSize,
        p_Settings->usWindowRegionRounding,
        p_Settings->fPathBoundingBoxPadding,
        // [AllocatorSettings]
        p_Settings->usHeapAllocatorMaxEntities,
        p_Settings->usHeapAllocatorMaxPathPoints,
        p_Settings->usHeapAllocatorMaxPathObstacles,
        p_Settings->usHeapAllocatorAiClosest,
        p_Settings->usHeapAllocatorFPoint,
        p_Settings->usHeapAllocatorFRect,
        p_Settings->usHeapAllocatorMaxMessages,
        // [AISettings]
        p_Settings->fAiMaxSearchRange,
        p_Settings->fSufficientlyClose,
        p_Settings->fCollisionBuffer,
        // [MovementSettings]
        p_Settings->fWorkerMoveSpeed,
        p_Settings->fMineralMoveSpeed,
        p_Settings->fCommandMoveSpeed,
        p_Settings->fSupplyMoveSpeed,
        p_Settings->fRefineryMoveSpeed,
        p_Settings->fWorkerMineralDivisor,
        p_Settings->fWorkerGasDivisor,
        // [PauseSettings]
        p_Settings->fMineralsPause,
        p_Settings->fMineralsCommandPause,
        p_Settings->fRefineryPause,
        p_Settings->fRefineryCommandPause,
        // [ResourceSettings]
        p_Settings->usMineralsPerGrab,
        p_Settings->usMineralsMultiplier,
        p_Settings->usGasPerGrab,
        p_Settings->usGasMultiplier,
        p_Settings->uiMineralsOnCreation,
        p_Settings->uiGasOnCreation);
    MISC_WriteOut(p_Log, LOG_SEVERITY_TRACE, "--------------------\n");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl SETTINGS_PropagateHotReload(SETTINGS_T* p_Settings, LOG_T* p_Log, char* p_szFileName, GLOBALS_T* p_Globals) {
    if (SETTINGS_InitFromFile(p_Settings, p_Log, p_szFileName)) {
        // Propagate to existing static properties in other systems:
        ENTITY_ReceiveHotReload(p_Settings, p_Globals, p_Log);
        GLOBALS_ReceiveHotReload(p_Settings, p_Globals);
        MISC_ReceiveHotReload(p_Settings, p_Globals);
        TIMEBASE_ReceiveHotReload(p_Globals->p_Engine, p_Settings->fEngineFPS);
        TIMEBASE_ReceiveHotReload(p_Globals->p_Maintenance, p_Settings->fMaintenanceTimebase);
    }
    else {
        MISC_WriteOut(p_Log, LOG_SEVERITY_ERROR, "SETTINGS_PropagateHotReload(): Failed to hot reload settings file.\n");
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 __cdecl SETTINGS_Match(const char* const p_szA, const char* const p_szB) {
    return strcmp(p_szA, p_szB) == 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl SETTINGS_Kill(SETTINGS_T** pp_Settings, GLOBALS_T* p_Globals) {
    SETTINGS_T* p_Settings = *pp_Settings;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_Settings) {
        free(p_Settings);
        p_Globals->stAllocations -= sizeof(SETTINGS_T);
        *pp_Settings = NULL;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
