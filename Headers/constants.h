/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define APP_NAME                            "Gatherers (C) V12.0"
#define PI                                  3.1416f
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Resource constants.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ICO_MAIN                            100
#define IMG_MAIN                            200
#define DLG_LOAD                            300
#define DLG_LOAD_IMG                        400
#define DLG_LOAD_STATUS                     500
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Menu constants.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAIN_MENU                           600
#define MENU_SIMULATION_TOGGLE_SIMULATION   700
#define MENU_SIMULATION_FREE                800
#define MENU_SIMULATION_EXIT                900
#define MENU_SIMULATION_PRINT               1000
#define MENU_SIMULATION_TOGGLE_SCATTER      1100
#define MENU_DELETE_SELECTED                1200
#define MENU_DELETE_WORKER                  1300
#define MENU_DELETE_COMMAND                 1400
#define MENU_DELETE_MINERAL                 1500
#define MENU_DELETE_SUPPLY                  1600
#define MENU_DELETE_REFINERY                1700
#define MENU_DELETE_ALL                     1800
#define MENU_BUILD_NONE                     1900
#define MENU_BUILD_WORKER                   2000
#define MENU_BUILD_COMMAND                  2100
#define MENU_BUILD_MINERAL                  2200
#define MENU_BUILD_SUPPLY                   2300
#define MENU_BUILD_REFINERY                 2400
#define MENU_OPTIONS_ALL_DIAGNOSTICS        2500
#define MENU_OPTIONS_IDS                    2600
#define MENU_OPTIONS_STATS                  2700
#define MENU_OPTIONS_MASKING                2800
#define MENU_OPTIONS_RESOURCES              2900
#define MENU_OPTIONS_STATUSES               3000
#define MENU_OPTIONS_FULLSCREEN             3100
#define MENU_OPTIONS_SHOW_MINOR             3200
#define MENU_OPTIONS_SHOW_MAJOR             3300
#define MENU_OPTIONS_ENABLE_TRANSLATIONS    3400
#define MENU_OPTIONS_TOPMOST                3500
#define MENU_OPTIONS_PATH_RAYCAST           3550
#define MENU_OPTIONS_PATH_BISECTION         3560
#define MENU_CLEAR_SELECTED                 3600
#define MENU_CLEAR_MINERAL_COUNT            3700
#define MENU_CLEAR_GAS_COUNT                3800
#define MENU_CLEAR_ALL_COUNT                3900
#define MENU_SORT_WORKERS                   4000
#define MENU_SORT_COMMANDS                  4100
#define MENU_SORT_MINERALS                  4200
#define MENU_SORT_SUPPLY                    4300
#define MENU_SORT_REFINERIES                4400
#define MENU_SORT_OBSTACLES                 4500
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// List sorting constants.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define WORKERS_TO_FRONT                    4600
#define MINERALS_TO_FRONT                   4700
#define COMMANDS_TO_FRONT                   4800
#define SUPPLIES_TO_FRONT                   4900
#define REFINERIES_TO_FRONT                 5000
#define OBSTACLES_TO_FRONT                  5100
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Entity type constants.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ENTITY_NONE                         5200
#define ENTITY_WORKER                       5300
#define ENTITY_MINERAL                      5400
#define ENTITY_COMMAND                      5500
#define ENTITY_SUPPLY                       5600
#define ENTITY_REFINERY                     5700
#define ENTITY_OBSTACLE                     5800
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Animation state constants.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ANIMATE_WORKER_NORMAL               5900
#define ANIMATE_WORKER_MINERALS             6000
#define ANIMATE_WORKER_GAS                  6100
#define ANIMATE_COMMAND_NORMAL              6200
#define ANIMATE_COMMAND_RECEIVE             6300
#define ANIMATE_COMMAND_CREATING            6400
#define ANIMATE_SUPPLY_NORMAL               6500
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Matrix rendering constants.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RENDER_ROTATE                       6600
#define RENDER_SCALE                        6700
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Message type constants.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MESSAGE_GAIN_MINERALS               6800
#define MESSAGE_GAIN_GAS                    6900
#define MESSAGE_BONUS_MINERALS              7000
#define MESSAGE_BONUS_GAS                   7100
#define MESSAGE_GENERAL_WARNING             7200
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Command card constants.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CARD_BLANK                          7300
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Volatile entity constants.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define WORKER_MOVE_SPEED                   7.0f
#define WORKER_MINERAL_DIVISOR              5.0f
#define WORKER_GAS_DIVISOR                  6.0f
#define MINERAL_MOVE_SPEED                  1.0f
#define COMMAND_MOVE_SPEED                  1.0f
#define SUPPLY_MOVE_SPEED                   1.0f
#define REFINERY_MOVE_SPEED                 1.0f
#define MINERALS_PAUSE                      1500.0f
#define MINERALS_COMMAND_PAUSE              1500.0f
#define REFINERY_PAUSE                      2000.0f
#define REFINERY_COMMAND_PAUSE              4000.0f
#define MINERALS_PER_GRAB                   1
#define MINERALS_MULTIPLIER                 3
#define GAS_PER_GRAB                        1
#define GAS_MULTIPLIER                      3
#define MINERALS_ON_CREATION                100000
#define GAS_ON_CREATION                     100000
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Volatile building constants.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SUPPLY_FROM_COMMAND                 5
#define COST_OF_COMMAND_MINERALS            100
#define COST_OF_COMMAND_GAS                 100
#define SUPPLY_FROM_DEPOT                   3
#define COST_OF_DEPOT_MINERALS              75
#define COST_OF_DEPOT_GAS                   75
#define SUPPLY_USED_WORKER                  1
#define COST_OF_WORKER_MINERALS             25
#define COST_OF_WORKER_GAS                  0
#define COST_OF_REFINERY_MINERALS           50
#define COST_OF_REFINERY_GAS                0
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Volatile drawing constants.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ENGINE_FPS                          60.0f
#define ANIMATE_FPS                         30.0f
#define INITIAL_CLIENT_WIDTH                1650
#define INITIAL_CLIENT_HEIGHT               850
#define TRANSLATION_STEP_AMOUNT             35.0f
#define AI_MAX_SEARCH_RANGE                 12000.0f
#define MINIMAP_SIZE                        220.0f
#define MAP_SIZE                            3000.0f
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Volatile AI constants.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SUFFICIENTLY_CLOSE                  5.0f
#define COLLISION_BUFFER                    5.0f
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
