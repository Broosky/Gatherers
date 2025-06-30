/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "common_types.h"
#include "Windows/windows_minified.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Resource constants:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Max: LOWORD(lParam) = WORD = ushort = 0 - 65,535
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ICO_MAIN                                        100
#define IMG_MAIN                                        200
#define DLG_LOAD                                        300
#define DLG_LOAD_IMG                                    400
#define DLG_LOAD_STATUS                                 500
#define MAIN_MENU                                       600
// Simulation ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MENU_SIMULATION_TOGGLE_SIMULATION               5000
#define MENU_SIMULATION_TOGGLE_SCATTER                  5100
#define MENU_SIMULATION_FREE                            5200
#define MENU_SIMULATION_PRINT                           5300
#define MENU_SIMULATION_EXIT                            5400
#define MENU_SIMULATION_SAVE                            5500
#define MENU_SIMULATION_LOAD                            5600
// Delete ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MENU_DELETE_SELECTED                            10000
#define MENU_DELETE_WORKER                              10100
#define MENU_DELETE_COMMAND                             10200
#define MENU_DELETE_MINERAL                             10300
#define MENU_DELETE_SUPPLY                              10400
#define MENU_DELETE_REFINERY                            10500
#define MENU_DELETE_ALL                                 10600
// Build ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MENU_BUILD_NONE                                 15000
#define MENU_BUILD_WORKER                               15100
#define MENU_BUILD_COMMAND                              15200
#define MENU_BUILD_MINERAL                              15300
#define MENU_BUILD_SUPPLY                               15400
#define MENU_BUILD_REFINERY                             15500
// Options //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MENU_OPTIONS_MASKING                            20000
#define MENU_OPTIONS_ALL_DIAGNOSTICS                    20100
#define MENU_OPTIONS_IDS                                20200
#define MENU_OPTIONS_STATISTICS                         20300
#define MENU_OPTIONS_RESOURCES                          20400
#define MENU_OPTIONS_STATUSES                           20500
#define MENU_OPTIONS_SHOW_MINOR_VECTORS                 20600
#define MENU_OPTIONS_SHOW_MAJOR_VECTORS                 20700
#define MENU_OPTIONS_PATH_FINDING_ENABLE                20800
#define MENU_OPTIONS_PATH_FINDING_RAYCAST               20900
#define MENU_OPTIONS_PATH_FINDING_BISECTION             21000
#define MENU_OPTIONS_PATH_FINDING_INTERPOLATE_CURVES    21100
#define MENU_OPTIONS_ENABLE_TRANSLATIONS                21200
#define MENU_OPTIONS_TOPMOST                            21300
#define MENU_OPTIONS_FULLSCREEN                         21400
#define MENU_OPTIONS_DIRTY_ZONES                        21500
#define MENU_OPTIONS_SHOW_TERRAIN_GRID                  21600
#define MENU_OPTIONS_SHOW_TRANSLATION_THRESHOLD         21700
// Renderer /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MENU_RENDERER_WIREFRAME                         25000
#define MENU_RENDERER_SINGLE_BUFFER                     25100
#define MENU_RENDERER_DOUBLE_BUFFER                     25200
#define MENU_RENDERER_SDL                               25300
#define MENU_RENDERER_DIRECT2D                          25400
#define MENU_RENDERER_POST_PROCESSING_ENABLE            25500
#define MENU_RENDERER_POST_PROCESSING_BOX_BLUR          25600
#define MENU_RENDERER_POST_PROCESSING_GREEN_MASK        25700    
#define MENU_RENDERER_POST_PROCESSING_DARKEN            25800
// Log //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MENU_LOG_OPEN                                   30000
// Settings /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MENU_SETTINGS_OPEN                              35000
#define MENU_SETTINGS_RELOAD                            35100
// Clear ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MENU_CLEAR_SELECTED                             40000
#define MENU_CLEAR_MINERAL_COUNT                        40100
#define MENU_CLEAR_GAS_COUNT                            40200
#define MENU_CLEAR_ALL_COUNT                            40300
// Sort /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MENU_SORT_WORKERS                               45000
#define MENU_SORT_COMMANDS                              45100
#define MENU_SORT_MINERALS                              45200
#define MENU_SORT_SUPPLY                                45300
#define MENU_SORT_REFINERIES                            45400
#define MENU_SORT_OBSTACLES                             45500
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declares:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct LOG LOG_T;
typedef struct GLOBALS GLOBALS_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Types:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct CONSTANTS {
    char szAppName[64];
    char szDefaultSettingsFile[64];
    char szDefaultLogFile[64];
    char szDefaultSaveFile[64];
    float fPi;
    float fDirtyZoneBuffer;
    short sMessageLifespan;
    UIDELTA_T MaxClientSize;
    UIDELTA_T MinClientSize;
    UINT uiTerrainTilesX;
    UINT uiTerrainTilesY;
    // List sorting constants:
    USHORT usWorkersToFront;
    USHORT usMineralsToFront;
    USHORT usCommandsToFront;
    USHORT usSuppliesToFront;
    USHORT usRefineriesToFront;
    USHORT usObstaclesToFront;
    // Entity type constants:
    USHORT usEntityNone;
    USHORT usEntityWorker;
    USHORT usEntityMineral;
    USHORT usEntityCommand;
    USHORT usEntitySupply;
    USHORT usEntityRefinery;
    USHORT usEntityObstacle;
    // Animation state constants:
    USHORT usAnimateWorkerNormal;
    USHORT usAnimateWorkerMinerals;
    USHORT usAnimateWorkerGas;
    USHORT usAnimateCommandNormal;
    USHORT usAnimateCommandReceive;
    USHORT usAnimateCommandCreating;
    USHORT usAnimateSupplyNormal;
    // Matrix rendering constants:
    USHORT usTransformRotate;
    USHORT usTransformScale;
    // Message type constants:
    USHORT usMessageGainMinerals;
    USHORT usMessageGainGas;
    USHORT usMessageBonusMinerals;
    USHORT usMessageBonusGas;
    USHORT usMessageGeneralWarning;
    // Command card constants:
    USHORT usCardBlank;
} CONSTANTS_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void                __cdecl     CONSTANTS_Zero                                  (CONSTANTS_T*);
CONSTANTS_T*        __cdecl     CONSTANTS_Create                                (GLOBALS_T*, LOG_T*);
void                __cdecl     CONSTANTS_Kill                                  (CONSTANTS_T**, GLOBALS_T*);
void                __cdecl     CONSTANTS_Init                                  (CONSTANTS_T*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
