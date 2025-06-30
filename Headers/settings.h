/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SETTINGS_H_
#define _SETTINGS_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "Windows/windows_minified.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declares:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct LOG LOG_T;
typedef struct ENTITY ENTITY_T;
typedef struct GLOBALS GLOBALS_T;
typedef struct CONSTANTS CONSTANTS_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Types:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct SETTINGS {
    // [BuildingCosts]
    USHORT usSupplyFromCommand;
    USHORT usCostOfCommandMinerals;
    USHORT usCostOfCommandGas;
    USHORT usSupplyFromDepot;
    USHORT usCostOfDepotMinerals;
    USHORT usCostOfDepotGas;
    USHORT usSupplyUsedWorker;
    USHORT usCostOfWorkerMinerals;
    USHORT usCostOfWorkerGas;
    USHORT usCostOfRefineryMinerals;
    USHORT usCostOfRefineryGas;
    // [AppSettings]
    float fEngineFpsTarget;
    float fAnimateFpsTarget;
    float fMaintenanceTimebase;
    USHORT usInitialClientWidth;
    USHORT usInitialClientHeight;
    float fTranslationStepAmount;
    float fAiMaxSearchRange;
    float fMinimapSize;
    float fMapSize;
    USHORT usWindowRegionRounding;
    float fPathBoundingBoxPadding;
    // [AllocatorSettings]
    USHORT usHeapAllocatorMaxEntities;
    USHORT usHeapAllocatorMaxPathPoints;
    USHORT usHeapAllocatorMaxPathObstacles;
    USHORT usHeapAllocatorMaxMessages;
    USHORT usHeapAllocatorAiClosest;
    USHORT usHeapAllocatorFPoint;
    USHORT usHeapAllocatorFRect;
    // [AISettings]
    float fSufficientlyClose;
    float fCollisionBuffer;
    // [MovementSettings]
    float fWorkerMoveSpeed;
    float fMineralMoveSpeed;
    float fCommandMoveSpeed;
    float fSupplyMoveSpeed;
    float fRefineryMoveSpeed;
    float fWorkerMineralDivisor;
    float fWorkerGasDivisor;
    // [PauseSettings]
    float fMineralsPause;
    float fMineralsCommandPause;
    float fRefineryPause;
    float fRefineryCommandPause;
    // [ResourceSettings]
    USHORT usMineralsPerGrab;
    USHORT usMineralsMultiplier;
    USHORT usGasPerGrab;
    USHORT usGasMultiplier;
    UINT uiMineralsOnCreation;
    UINT uiGasOnCreation;
} SETTINGS_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void                __cdecl     SETTINGS_Zero                                   (SETTINGS_T*);
SETTINGS_T*         __cdecl     SETTINGS_Create                                 (GLOBALS_T*, LOG_T*);
UINT8               __cdecl     SETTINGS_InitFromFile                           (SETTINGS_T*, CONSTANTS_T*, LOG_T*, const char* const);
UINT8               __cdecl     SETTINGS_Match                                  (const char* const, const char* const);
void                __cdecl     SETTINGS_Print                                  (SETTINGS_T*, LOG_T*);
void                __cdecl     SETTINGS_PropagateHotReload                     (SETTINGS_T*, CONSTANTS_T*, LOG_T*, char*, GLOBALS_T*);
void                __cdecl     SETTINGS_Kill                                   (SETTINGS_T**, GLOBALS_T*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
