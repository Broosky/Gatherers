/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _ENTITY_H_
#define _ENTITY_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "common_types.h"
#include "enums.h"
#include "Windows/windows_minified.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declares:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct LOG LOG_T;
typedef struct PATH PATH_T;
typedef struct ASSETS ASSETS_T;
typedef struct ENTITY ENTITY_T;
typedef struct GLOBALS GLOBALS_T;
typedef struct PICTURE PICTURE_T;
typedef struct SETTINGS SETTINGS_T;
typedef struct RENDERER RENDERER_T;
typedef struct CONSTANTS CONSTANTS_T;
typedef struct AI_CLOSEST AI_CLOSEST_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Types:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct SELECTED_COUNT {
    USHORT usSelectedWorkersCount;
    USHORT usSelectedOthersCount;
    USHORT usSelectedAllCount;
} SELECTED_COUNT_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct ENTITY_REQUIREMENTS {
    int iMinMinerals;
    int iMinGas;
    int iSupplyNeeded;
    int iSupplyProvided;
    int iSupplyDelta;
} ENTITY_REQUIREMENTS_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct ENTITY {
    USHORT usId;
    USHORT usState;
    USHORT usCurrentFrame;
    ENTITY_TYPE_T eType;
    int iMineralCount;
    int iGasCount;
    UINT8 ubIsMovable;
    UINT8 ubIsObstacle;
    UINT8 ubIsCarryingMinerals;
    UINT8 ubIsCarryingGas;
    UINT8 ubIsSelected;
    UINT8 ubIsAlive;
    UINT8 ubIsInMotion;
    UINT8 ubIsPaused;
    UINT8 ubIsHighlighted;
    float fPauseCount;
    float fPauseTime;
    float fRadius;
    FDELTA_T Size;
    FDELTA_T HalfSize;
    FPOINT_T Location;
    FPOINT_T CenterPoint;
    FDELTA_T MovementSpeed;
    FDELTA_T MinorVector;
    FDELTA_T MinorUnitVector;
    FPOINT_T MinorDestinationCenterPoint;
    FDELTA_T MajorVector;
    FDELTA_T MajorUnitVector;
    FPOINT_T MajorDestinationCenterPoint;
    PICTURE_T* p_Picture;
    struct ENTITY* p_Next;
    struct ENTITY* p_OperatingTarget; // Resource interaction traversal.
    struct ENTITY* p_OperatingSubject;
    PATH_T* p_Path;
} ENTITY_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void                __cdecl     ENTITY_Zero                                     (ENTITY_T*);
void                __cdecl     ENTITY_Create                                   (FPOINT_T, ENTITY_TYPE_T, ASSETS_T*, GLOBALS_T*, LOG_T*, RENDERER_T*, SETTINGS_T*, CONSTANTS_T*);
UINT8               __cdecl     ENTITY_BuildResourcesCheck                      (ENTITY_TYPE_T, GLOBALS_T*, LOG_T*, SETTINGS_T*, CONSTANTS_T*);
UINT8               __cdecl     ENTITY_BuildIntersectionCheck                   (ENTITY_TYPE_T, ASSETS_T*, GLOBALS_T*, LOG_T*, RENDERER_T*, CONSTANTS_T*);
void                __cdecl     ENTITY_Pause                                    (ENTITY_T*, float, SETTINGS_T*);
UINT8               __cdecl     ENTITY_CollidedWith                             (ENTITY_T*, ENTITY_T*, SETTINGS_T*);
void                __cdecl     ENTITY_Redefine                                 (ENTITY_TYPE_T, GLOBALS_T*, SETTINGS_T*, LOG_T*);
void                __cdecl     ENTITY_SelectAll                                (GLOBALS_T*);
void                __cdecl     ENTITY_DeleteAll                                (GLOBALS_T*, SETTINGS_T*, LOG_T*);
void                __cdecl     ENTITY_DeleteSelected                           (GLOBALS_T*, SETTINGS_T*, LOG_T*);
void                __cdecl     ENTITY_DeleteSpecific                           (ENTITY_T*, GLOBALS_T*, SETTINGS_T*, LOG_T*);
void                __cdecl     ENTITY_DeleteEntityType                         (ENTITY_TYPE_T, GLOBALS_T*, SETTINGS_T*, LOG_T*);
void                __cdecl     ENTITY_SortToFront                              (SORT_ORDER_T, GLOBALS_T*, LOG_T*);
UINT8               __cdecl     ENTITY_ConsiderSortToFront                      (ENTITY_T*, SORT_ORDER_T, LOG_T*);
void                __cdecl     ENTITY_Sort                                     (ENTITY_T**, ENTITY_T**, GLOBALS_T*);
void                __cdecl     ENTITY_SkipSort                                 (ENTITY_T**, ENTITY_T**);
void                __cdecl     ENTITY_PrintList                                (GLOBALS_T*, LOG_T*);
void                __cdecl     ENTITY_PrintClosestEntitiesList                 (AI_CLOSEST_T*, LOG_T*);
void                __cdecl     ENTITY_HandleAnimation                          (ENTITY_T*, ASSETS_T*, SETTINGS_T*, LOG_T*);
SELECTED_COUNT_T    __cdecl     ENTITY_GetSelectedEntityCounts                  (GLOBALS_T*);
void                __cdecl     ENTITY_ReceiveHotReload                         (SETTINGS_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     ENTITY_AnimateSupply                            (ENTITY_T*, LOG_T*);
void                __cdecl     ENTITY_AnimateRefinery                          (ENTITY_T*, LOG_T*);
void                __cdecl     ENTITY_AnimateWorker                            (ENTITY_T*, LOG_T*);
void                __cdecl     ENTITY_AnimateMineral                           (ENTITY_T*, LOG_T*);
void                __cdecl     ENTITY_AnimateCommand                           (ENTITY_T*, LOG_T*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
