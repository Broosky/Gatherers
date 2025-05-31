/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GLOBALS_H_
#define _GLOBALS_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "common_types.h"
#include "enums.h"
#include "Windows/windows_minified.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declares:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct LOG LOG_T;
typedef struct ENTITY ENTITY_T;
typedef struct MESSAGE MESSAGE_T;
typedef struct GLOBALS GLOBALS_T;
typedef struct SETTINGS SETTINGS_T;
typedef struct TIMEBASE TIMEBASE_T;
typedef struct RENDERER RENDERER_T;
typedef struct HEAP_ALLOCATOR HEAP_ALLOCATOR_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Types:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct GLOBALS {
    USHORT usIdStamp;
    USHORT usMapIndex;
    USHORT usThreshold;
    int iCurrentSupply;
    int iTotalSupply;
    ULONG ulFrameCount;
    UINT uiAnimateCount;
    int iMineralCount;
    int iGasCount;
    IPOINT_T Mouse;
    IPOINT_T MouseOld;
    IPOINT_T CaptureStart;
    IPOINT_T CaptureCurrent;
    USHORT usBuildType;
    size_t stAllocations;
    ULONG ulSecondsTick;
    UINT8 ubAnimate;
    UINT8 ubDrawSelectionRect;
    UINT8 ubClickOriginFromMinimap;
    UINT8 ubCreate;
    UINT8 ubIsBorderless;
    UINT8 ubIsSelecting;
    UINT8 ubIsFullscreenToggled;
    UINT8 ubIsBorderlessToggled;
    char szBuffer[128];
    float fEngineTime;
    float fEngineTimeSum;
    float fEngineTimeAverage;
    float fFramesPerSecond;
    float fClientBottomY;
    float fAnimationTick;
    FDELTA_T Translation;
    char szCurrentWorkingDirectory[MAX_PATH];
    SORT_ORDER_T eEntitySortState;
    HINSTANCE hInstance;
    MESSAGE_T* p_RootMessage;
    ENTITY_T* p_RootEntity;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HWND hWnd;
    HWND hWndDialog;
    WINDOWPOS WindowPosition;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TIMEBASE_T* p_Engine;
    TIMEBASE_T* p_Seconds;
    TIMEBASE_T* p_FrameTime;
    TIMEBASE_T* p_Maintenance;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HEAP_ALLOCATOR_T* p_PathPointAllocator;
    HEAP_ALLOCATOR_T* p_PathObstacleAllocator;
    HEAP_ALLOCATOR_T* p_EntityAllocator;
    HEAP_ALLOCATOR_T* p_MessagesAllocator;
    HEAP_ALLOCATOR_T* p_AiClosestAllocator;
    // + General purpose allocators.
    HEAP_ALLOCATOR_T* p_fPointAllocator;
    HEAP_ALLOCATOR_T* p_fRectAllocator;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    RENDERER_TYPE_T eActiveRendererType;
    RENDERER_T* p_ActiveRenderer;
    RENDERER_T* p_Renderers[RENDERER_TYPE_COUNT];
} GLOBALS_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GLOBALS_T*          __cdecl     GLOBALS_Create                                  (LOG_T*);
void                __cdecl     GLOBALS_Zero                                    (GLOBALS_T*);
size_t              __cdecl     GLOBALS_Kill                                    (GLOBALS_T**);
void                __cdecl     GLOBALS_Init                                    (GLOBALS_T*, HINSTANCE, SETTINGS_T*);
void                __cdecl     GLOBALS_ApplyAnimationRate                      (GLOBALS_T*, SETTINGS_T*);
void                __cdecl     GLOBALS_ReceiveHotReload                        (SETTINGS_T*, GLOBALS_T*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
