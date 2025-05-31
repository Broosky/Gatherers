/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _PATH_FINDING_H_
#define _PATH_FINDING_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "common_types.h"
#include "Windows/windows_minified.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declares:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct LOG LOG_T;
typedef struct MENU MENU_T;
typedef struct ENTITY ENTITY_T;
typedef struct GLOBALS GLOBALS_T;
typedef struct SETTINGS SETTINGS_T;
typedef struct RENDERER RENDERER_T;
typedef struct CONSTANTS CONSTANT_T;
typedef struct PATH_NODE PATH_NODE_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Types:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct PATH_OBSTACLE {
    ENTITY_T* p_Entity;
    float fDistance;
    struct PATH_OBSTACLE* p_Next;
} PATH_OBSTACLE_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct PATH_NODE {
    FPOINT_T Location;
    struct PATH_NODE* p_Next;
    struct PATH_NODE* p_Previous;
} PATH_NODE_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct PATH {
    UINT8 ubIncludesReturn;
    UINT8 ubIncludesInterpolatedCurves;
    PATH_NODE_T* p_PathNodes;
} PATH_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PATH_T*             __cdecl     PATH_ComputePath                                (ENTITY_T*, FPOINT_T, FPOINT_T, UINT8, UINT8, GLOBALS_T*, LOG_T*, SETTINGS_T*);
PATH_T*             __cdecl     PATH_RecomputePath                              (ENTITY_T*, UINT8, GLOBALS_T*, LOG_T*);
FRECT_T             __cdecl     PATH_ComputeBoundingBox                         (FPOINT_T, FPOINT_T, FDELTA_T*);
UINT8               __cdecl     PATH_IsWithinBoundingBox                        (ENTITY_T*, FRECT_T);
UINT8               __cdecl     PATH_CheckLineIntersection                      (FPOINT_T, FPOINT_T, FPOINT_T, FPOINT_T);
UINT8               __cdecl     PATH_CheckEntityIntersection                    (FPOINT_T, FPOINT_T, ENTITY_T*);
UINT8               __cdecl     PATH_CheckRectangleIntersection                 (FPOINT_T, FPOINT_T, FRECT_T);
PATH_NODE_T*        __cdecl     PATH_InterpolateCurve                           (FPOINT_T, FDELTA_T, FDELTA_T, float);
FDELTA_T            __cdecl     PATH_CalculateVector                            (FPOINT_T, FPOINT_T);
FDELTA_T            __cdecl     PATH_CalculateUnitVector                        (FDELTA_T);
UINT8               __cdecl     PATH_WithinPoint                                (ENTITY_T*, FPOINT_T);
void                __cdecl     PATH_MoveTo                                     (ENTITY_T*, ENTITY_T*, GLOBALS_T*, MENU_T*, SETTINGS_T*);
void                __cdecl     PATH_MoveToPoint                                (ENTITY_T*, FPOINT_T, GLOBALS_T*, MENU_T*, SETTINGS_T*);
void                __cdecl     PATH_FindMinorVector                            (ENTITY_T*, GLOBALS_T*, MENU_T*, SETTINGS_T*);
FPOINT_T            __cdecl     PATH_FindMinorVectorHead                        (ENTITY_T*, ENTITY_T*, GLOBALS_T*, SETTINGS_T*);
void                __cdecl     PATH_UpdatePosition                             (RENDERER_T*, ENTITY_T*, GLOBALS_T*, MENU_T*, SETTINGS_T*, CONSTANTS_T*, LOG_T*);
void                __cdecl     PATH_SnapToPoint                                (ENTITY_T*, FPOINT_T);
float               __cdecl     PATH_CalculateDistance                          (ENTITY_T*, ENTITY_T*);
UINT8               __cdecl     PATH_IsSufficientlyCloseMinorVector             (ENTITY_T*, SETTINGS_T*);
UINT8               __cdecl     PATH_IsSufficientlyCloseMajorVector             (ENTITY_T*, SETTINGS_T*);
UINT8               __cdecl     PATH_IsSufficientlyClosePoint                   (FPOINT_T, FPOINT_T, SETTINGS_T*);
UINT8               __cdecl     PATH_IsNotSufficientlyClosePoint                (FPOINT_T, FPOINT_T, SETTINGS_T*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void                __cdecl     PATH_OBSTACLE_Zero                              (PATH_OBSTACLE_T*);
PATH_OBSTACLE_T*    __cdecl     PATH_OBSTACLE_Create                            (GLOBALS_T*, LOG_T*);
void                __cdecl     PATH_OBSTACLE_KillAll                           (PATH_OBSTACLE_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     PATH_OBSTACLE_AddAndAdvance                     (ENTITY_T*, PATH_OBSTACLE_T**, GLOBALS_T*, LOG_T*);
void                __cdecl     PATH_OBSTACLE_PrintObstaclesList                (PATH_OBSTACLE_T*, LOG_T*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
