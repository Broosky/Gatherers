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
#include "windows_macros.h"
#include <windows.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declares:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct PICTURE PICTURE_T;
typedef struct ASSETS ASSETS_T;
typedef struct GLOBALS GLOBALS_T;
typedef struct PATH_NODE PATH_NODE_T;
typedef struct ENTITY ENTITY_T;
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
typedef struct ENTITY {
    USHORT usId;
    USHORT usState;
    USHORT usCurrentFrame;
    USHORT usType;
    int iMineralCount;
    int iGasCount;
    UINT8 ubIsMovable;
    UINT8 ubIsObstacle;
    UINT8 ubIsCarrying;
    UINT8 ubIsSelected;
    UINT8 ubIsAlive;
    UINT8 ubIsInMotion;
    UINT8 ubIsPaused;
    UINT8 ubIsHighlighted;
    float fPauseCount;
    float fPauseTime;
    float fRadius;
    FPOINT_T Size;
    FPOINT_T HalfSize;
    FPOINT_T Location;
    FPOINT_T CenterPoint;
    FPOINT_T MovementSpeed;
    FPOINT_T MinorVector;
    FPOINT_T MinorUnitVector;
    FPOINT_T MinorDestinationCenterPoint;
    FPOINT_T MajorVector;
    FPOINT_T MajorUnitVector;
    FPOINT_T MajorDestinationCenterPoint;
    PICTURE_T* p_Picture;
    struct ENTITY* p_Next;
    struct ENTITY* p_Operating;
    PATH_NODE_T* p_Path;
} ENTITY_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void                __cdecl     ENTITY_Zero                         (ENTITY_T*);
void                __cdecl     ENTITY_Create                       (FPOINT_T, USHORT, ASSETS_T*, GLOBALS_T*);
UINT8               __cdecl     ENTITY_Restrict                     (USHORT, GLOBALS_T*);
UINT8               __cdecl     ENTITY_Overlap                      (USHORT, ASSETS_T*, GLOBALS_T*);
void                __cdecl     ENTITY_Pause                        (ENTITY_T*, float);
UINT8               __cdecl     ENTITY_CollidedWith                 (ENTITY_T*, ENTITY_T*);
UINT8               __cdecl     ENTITY_WithinPoint                  (ENTITY_T*, FPOINT_T);
void                __cdecl     ENTITY_MoveTo                       (ENTITY_T*, ENTITY_T*, GLOBALS_T*);
void                __cdecl     ENTITY_MoveToPoint                  (ENTITY_T*, FPOINT_T, GLOBALS_T*);
void                __cdecl     ENTITY_FindMinorVector              (ENTITY_T*, GLOBALS_T*);
FPOINT_T            __cdecl     ENTITY_MinorVectorHead              (ENTITY_T*, ENTITY_T*, GLOBALS_T*);
void                __cdecl     ENTITY_UpdatePosition               (ENTITY_T*, GLOBALS_T*);
void                __cdecl     ENTITY_Redefine                     (USHORT, GLOBALS_T*);
void                __cdecl     ENTITY_DeleteAll                    (GLOBALS_T*);
void                __cdecl     ENTITY_DeleteSelected               (GLOBALS_T*);
void                __cdecl     ENTITY_DeleteSpecific               (ENTITY_T*, GLOBALS_T*);
void                __cdecl     ENTITY_DeleteEntityType             (USHORT, GLOBALS_T*);
void                __cdecl     ENTITY_SortToFront                  (USHORT, GLOBALS_T*);
void                __cdecl     ENTITY_PrintList                    (GLOBALS_T*);
void                __cdecl     ENTITY_PrintClosestEntitiesList     (AI_CLOSEST_T*);
void                __cdecl     ENTITY_Animate                      (ENTITY_T*, ASSETS_T*);
SELECTED_COUNT_T    __cdecl     ENTITY_GetSelectedEntityCounts      (GLOBALS_T*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
