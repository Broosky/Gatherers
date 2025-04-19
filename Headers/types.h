/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _TYPES_H_
#define _TYPES_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/main.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct IPOINT_T {
    int iX;
    int iY;
} IPOINT;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct IDELTA_T {
    int iDx;
    int iDy;
} IDELTA;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct FPOINT_T {
    float fX;
    float fY;
} FPOINT;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct FDELTA_T {
    float fDX;
    float fDY;
} FDELTA;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct PICTURE_T {
    FPOINT Location;
    HBITMAP hBmp;
    HBITMAP hBmpMask;
    BITMAP Bitmap;
} PICTURE;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct SETTINGS_T {
    const float fEngineFps;
    const float fAnimateFps;
    const int iClientWidth;
    const int iClientHeight;
    const int iSupplyFromCommand;
    const int iCommandMineralsCost;
    const int iCommandGasCost;
    const int iSupplyFromDepot;
    const int iSupplyMineralsCost;
    const int iSupplyGasCost;
    const int iSupplyUsedWorker;
    const int iWorkerMineralsCost;
    const int iWorkerGasCost;
    const int iRefineryMineralsCost;
    const int iRefineryGasCost;
    const float fWorkerMoveSpeed;
    const float fMineralSlowdownDivisor;
    const float fGasSlowdownDivisor;
    const float fMineralMoveSpeed;
    const float fSupplyMoveSpeed;
    const float fCommandMoveSpeed;
    const float fRefineryMoveSpeed;
    const float fMineralsPause;
    const float fMineralsCommandPause;
    const float fRefineryPause;
    const float fRefineryCommandPause;
    const float fGoldenMineralBitShift;
    const float fGoldenGasBitShift;
    const float fMinimapSize;
    const int iMineralsStart;
    const int iGasStart;
    const int iMineralsPerGrab;
    const int iGasPerGrab;
} SETTINGS;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct ENTITY_T {
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
    FPOINT Size;
    FPOINT HalfSize;
    FPOINT Location;
    FPOINT CenterPoint;
    FPOINT MovementSpeed;
    FPOINT MinorVector;
    FPOINT MinorUnitVector;
    FPOINT MinorDestinationCenterPoint;
    FPOINT MajorVector;
    FPOINT MajorUnitVector;
    FPOINT MajorDestinationCenterPoint;
    PICTURE* p_Picture;
    struct ENTITY* p_Next;
    struct ENTITY* p_Operating;
} ENTITY;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct CARD_T {
    USHORT usState;
    PICTURE* p_Picture;
} CARD;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct MESSAGE_T {
    USHORT usType;
    short sLifespan;
    char szMessage[128];
    float fScale;
    float fAngle;
    struct MESSAGE* p_Next;
    FPOINT Location;
} MESSAGE;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct IMAGES_T {
    PICTURE Blitter[9];
    PICTURE Worker[3];
    PICTURE Command[26];
    PICTURE Mineral[5];
    PICTURE Supply[6];
    PICTURE Refinery[1];
    PICTURE HUD[2];
    PICTURE Minimap[1];
    PICTURE Gas[1];
    PICTURE Terrain[1];
    PICTURE Card[5];
} IMAGES;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct DBLBUF_T {
    char szBlitter[128];
    HDC hDC;
    HWND hWnd;
    HDC hDCMem;
    HDC hDCBmp;
    XFORM XForm;
    HBITMAP hCanvas;
    RECT ClientArea;
    HGDIOBJ hStorage;
    PICTURE* p_Blitter;
} DBLBUF;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct TIMEBASE_T {
    float fMsPerFrame;
    float fDifference;
    __int64 iFreq;
    __int64 iStartCount;
    __int64 iCountNow;
} TIMEBASE;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct MENU_T {
    UINT8 ubDrawMinor;
    UINT8 ubDrawMajor;
    UINT8 ubDrawStatuses;
    UINT8 ubDrawIds;
    UINT8 ubDrawResources;
    UINT8 ubEnableTranslations;
    UINT8 ubIsTopmostWindow;
    UINT8 ubEnableMasking;
    UINT8 ubToggleSimulation;
    UINT8 ubToggleScatter;
    UINT8 ubAllDiagnosticsToggle;
    UINT8 ubDrawDiagnostics;
    UINT8 ubIsFullScreen;
    UINT8 ubUseRaycast;
    UINT8 ubUseBisection;
    HMENU hMenu;
} MENU;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct SELECTED_COUNT_T {
    USHORT usSelectedWorkersCount;
    USHORT usSelectedOthersCount;
    USHORT usSelectedAllCount;
} SELECTED_COUNT;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct AI_CLOSEST_T {
    float fDistance;
    ENTITY* p_Entity;
} AI_CLOSEST;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct GLOBALS_T {
    USHORT usIdStamp;
    USHORT usMapIndex;
    USHORT usThreshold;
    int iCurrentSupply;
    int iTotalSupply;
    ULONG ulFrameCount;
    UINT uiAnimateCount;
    int iMineralCount;
    int iGasCount;
    int iOldMouseX;
    int iOldMouseY;
    int iMouseX;
    int iMouseY;
    int iCaptureStartX;
    int iCaptureStartY;
    int iCaptureCurrentX;
    int iCaptureCurrentY;
    USHORT usBuildType;
    USHORT usEntityCount;
    USHORT usMessageCount;
    int iRunningHeap;
    ULONG ulSecondsTick;
    UINT8 ubAnimate;
    UINT8 ubDrawSelectionRect;
    UINT8 ubClickOriginFromMinimap;
    UINT8 ubCreate;
    char szBuffer[128];
    float fEngineTime;
    float fEngineTimeSum;
    float fEngineTimeAverage;
    float fFramesPerSecond;
    float fClientBottomY;
    float fAnimationTick;
    float fLateralTranslation;
    float fVerticalTranslation;
    HINSTANCE hInstance;
    MESSAGE* p_RootMessage;
    ENTITY* p_RootEntity;
} GLOBALS;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct LOG_T {
    FILE* p_LogFile;
} LOG;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Externs for WinMain, WindowProc, and DlgLoad.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern GLOBALS* p_Globals;
extern MENU* p_Menu;
extern DBLBUF* p_DblBuf;
extern IMAGES* p_Images;
extern SETTINGS* p_Settings;
extern LOG* p_Log;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
