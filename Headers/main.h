/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Program Name: Gatherers (C)                                                                                            //
//  Author: Jeffrey Bednar                                                                                                 //
//  Copyright: Illusion Interactive, 2011                                                                                  //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MAIN_H_
#define _MAIN_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <commctrl.h>
#include <math.h>
#include <stdio.h>
#include <malloc.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Miscellaneous constants.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define APP_NAME                            "Gatherers (C) V11.0"
#define PI                                  3.1416f
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Non-volatile resource constants.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ICO_MAIN                            100
#define IMG_MAIN                            200
#define DLG_LOAD                            300
#define DLG_LOAD_IMG                        400
#define DLG_LOAD_STATUS                     500
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Non-volatile menu constants.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAIN_MENU                           600
#define MENU_SIMULATION_TOGGLE_SIMULATION   700
#define MENU_SIMULATION_FREE                750
#define MENU_SIMULATION_EXIT                800
#define MENU_SIMULATION_PRINT               900
#define MENU_SIMULATION_TOGGLE_SCATTER      950
#define MENU_DELETE_SELECTED                1000
#define MENU_DELETE_WORKER                  1020
#define MENU_DELETE_COMMAND                 1040
#define MENU_DELETE_MINERAL                 1060
#define MENU_DELETE_SUPPLY                  1070
#define MENU_DELETE_REFINERY                1080
#define MENU_DELETE_ALL                     1100
#define MENU_BUILD_NONE                     1200
#define MENU_BUILD_WORKER                   1300
#define MENU_BUILD_COMMAND                  1400
#define MENU_BUILD_MINERAL                  1500
#define MENU_BUILD_SUPPLY                   1600
#define MENU_BUILD_REFINERY                 1700
#define MENU_OPTIONS_ALL_DIAGNOSTICS        1800
#define MENU_OPTIONS_IDS                    1850
#define MENU_OPTIONS_DIAGNOSTICS            1900
#define MENU_OPTIONS_MASKING                2000
#define MENU_OPTIONS_RESOURCES              2100
#define MENU_OPTIONS_STATUSES               2200
#define MENU_OPTIONS_FULLSCREEN             2300
#define MENU_OPTIONS_SHOW_MINOR             2400
#define MENU_OPTIONS_SHOW_MAJOR             2500
#define MENU_OPTIONS_ENABLE_TRANSLATIONS    2600
#define MENU_OPTIONS_TOPMOST                2650
#define MENU_CLEAR_SELECTED                 2700
#define MENU_CLEAR_MINERAL_COUNT            2800
#define MENU_CLEAR_GAS_COUNT                2900
#define MENU_CLEAR_ALL_COUNT                3000
#define MENU_SORT_WORKERS                   3100
#define MENU_SORT_COMMANDS                  3200
#define MENU_SORT_MINERALS                  3300
#define MENU_SORT_SUPPLY                    3400
#define MENU_SORT_REFINERIES                3450
#define MENU_SORT_OBSTACLES                 3500
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Non-volatile sorting constants.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define WORKERS_TO_FRONT                    3600
#define MINERALS_TO_FRONT                   3700
#define COMMANDS_TO_FRONT                   3800
#define SUPPLIES_TO_FRONT                   3900
#define REFINERIES_TO_FRONT                 3950
#define OBSTACLES_TO_FRONT                  4000
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Non-volatile entity constants.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ENTITY_NONE                         4050
#define ENTITY_WORKER                       4100
#define ENTITY_MINERAL                      4200
#define ENTITY_COMMAND                      4300
#define ENTITY_SUPPLY                       4400
#define ENTITY_REFINERY                     4500
#define ENTITY_OBSTACLE                     4600
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Non-volatile animation constants.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ANIMATE_WORKER_NORMAL               4700
#define ANIMATE_WORKER_MINERALS             4800
#define ANIMATE_WORKER_GAS                  4900
#define ANIMATE_COMMAND_NORMAL              5000
#define ANIMATE_COMMAND_RECEIVE             5100
#define ANIMATE_COMMAND_CREATING            5200
#define ANIMATE_SUPPLY_NORMAL               5300
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Non-volatile rendering constants.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RENDER_ROTATE                       5400
#define RENDER_SCALE                        5500
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Non-volatile message constants.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MESSAGE_GAIN_MINERALS               5600
#define MESSAGE_GAIN_GAS                    5700
#define MESSAGE_BONUS_MINERALS              5800
#define MESSAGE_BONUS_GAS                   5900
#define MESSAGE_GENERAL_WARNING             6000
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Non-volatile command card constants.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CARD_BLANK                          6100
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
#define PROCESS_FPS                         200.0f
#define ANIMATE_FPS                         30.0f
#define INITIAL_CLIENT_WIDTH                1650
#define INITIAL_CLIENT_HEIGHT               850
#define TRANSLATION_AMOUNT                  4.0f
#define AI_MAX_SEARCH_RANGE                 12000.0f
#define MINIMAP_SIZE                        220.0f
#define MAP_SIZE                            3000.0f
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Volatile artificial intelligence constants.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SUFFICIENTLY_CLOSE                  5.0f
#define COLLISION_BUFFER                    5.0f
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
    int iX;
    int iY;
} IPOINT;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
    float fX;
    float fY;
} FPOINT;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
    FPOINT Location;
    HBITMAP hBmp;
    HBITMAP hBmpMask;
    BITMAP Bitmap;
} PICTURE;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
    const float fProcessFPS;
    const float fAnimateFPS;
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
typedef struct {
    int iId;
    int iState;
    int iCurrentFrame;
    int iType;
    int iMineralCount;
    int iGasCount;
    int bIsMovable;
    int bIsObstacle;
    int bIsCarrying;
    int bIsSelected;
    int bIsAlive;
    int bIsInMotion;
    int bIsPaused;
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
typedef struct {
    int iState;
    PICTURE* p_Picture;
} CARD;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
    int iType;
    int iLifespan;
    char szMessage[128];
    float fScale;
    float fAngle;
    struct MESSAGE* p_Next;
    FPOINT Location;
} MESSAGE;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
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
typedef struct {
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
typedef struct {
    float fMSPerFrame;
    float fDifference;
    __int64 iFreq;
    __int64 iStartCount;
    __int64 iCountNow;
} TIMEBASE;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
    int bDrawMinor;
    int bDrawMajor;
    int bDrawStatuses;
    int bDrawIds;
    int bDrawResources;
    int bEnableTranslations;
    int bTopmost;
    int bEnableMasking;
    int bBegin;
    int bToggleScatter;
    int bAllDiagnostics;
    int bDiagnostics;
    int bFullScreen;
    HMENU hMenu;
} MENU;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
    int iSelectedWorkersCount;
    int iSelectedOthersCount;
    int iSelectedAllCount;
} SELECTED_COUNT;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
    float fDistance;
    ENTITY* p_Entity;
    int iAllocations;
} AI_CLOSEST;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
    int iIdStamp;
    int iMapIndex;
    int iThreshold;
    int iCurrentSupply;
    int iTotalSupply;
    int iFrameCount;
    int iAnimateCount;
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
    int iBuildType;
    int iEntityCount;
    int iMessageCount;
    int iRunningHeap;
    int iSecondTick;
    int bAnimate;
    int bDrawSelectionRect;
    int bClickOriginFromMinimap;
    int bCreate;
    char szBuffer[128];
    float fProcessTime;
    float fProcessTimeSum;
    float fProcessTimeAverage;
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
// Global because PROC_WindowProc() uses the double buffer and images structures (immutable arguments).
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern GLOBALS* p_Globals;
extern MENU* p_Menu;
extern DBLBUF* p_DblBuf;
extern IMAGES* p_Images;
extern SETTINGS* p_Settings;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            __cdecl     SETTINGS_Zero                               (SETTINGS*);
SETTINGS*       __cdecl     SETTINGS_Create                             (GLOBALS*);
void            __cdecl     SETTINGS_InitFromFile                       (SETTINGS*);
void            __cdecl     SETTINGS_Kill                               (SETTINGS*, GLOBALS*);
////////////    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            __cdecl     DBLBUF_Zero                                 (DBLBUF*);
DBLBUF*         __cdecl     DBLBUF_Create                               (HWND, GLOBALS*);
void            __cdecl     DBLBUF_Resize                               (DBLBUF*, HWND, COLORREF, GLOBALS*);
void            __cdecl     DBLBUF_Kill                                 (DBLBUF*, GLOBALS*);
IPOINT          __cdecl     DBLBUF_FindBlitterPoint                     (char);
void            __cdecl     DBLBUF_SetBlitter                           (DBLBUF*, PICTURE*);
void            __cdecl     DBLBUF_Blitter                              (DBLBUF*, char*, FPOINT, int);
void            __cdecl     DBLBUF_Clear                                (DBLBUF*, COLORREF);
void            __cdecl     DBLBUF_DrawEntityMinorVector                (DBLBUF*, ENTITY*, COLORREF);
void            __cdecl     DBLBUF_DrawEntityMajorVector                (DBLBUF*, ENTITY*, COLORREF);
void            __cdecl     DBLBUF_DrawEntityEllipse                    (DBLBUF*, ENTITY*, COLORREF, COLORREF);
void            __cdecl     DBLBUF_DrawEntity                           (DBLBUF*, ENTITY*, int);
void            __cdecl     DBLBUF_ClearEntity                          (DBLBUF*, ENTITY*, COLORREF);
void            __cdecl     DBLBUF_FlipEntity                           (DBLBUF*, ENTITY*);
void            __cdecl     DBLBUF_Flip                                 (DBLBUF*);
void            __cdecl     DBLBUF_FlipArea                             (DBLBUF*, int, int, int, int);
void            __cdecl     DBLBUF_DrawPicture                          (DBLBUF*, PICTURE*, int);
void            __cdecl     DBLBUF_DrawPictureAt                        (DBLBUF*, PICTURE*, FPOINT, int);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            __cdecl     TIMEBASE_Zero                               (TIMEBASE*);
TIMEBASE*       __cdecl     TIMEBASE_Create                             (float, GLOBALS*);
int             __cdecl     TIMEBASE_Tick                               (TIMEBASE*);
void            __cdecl     TIMEBASE_StartTimer                         (TIMEBASE*);
float           __cdecl     TIMEBASE_EndTimer                           (TIMEBASE*);
void            __cdecl     TIMEBASE_Kill                               (TIMEBASE*, GLOBALS*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            __cdecl     ENTITY_Zero                                 (ENTITY*);
void            __cdecl     ENTITY_Create                               (FPOINT, int, IMAGES*, GLOBALS*);
int             __cdecl     ENTITY_Restrict                             (int, GLOBALS*);
int             __cdecl     ENTITY_Overlap                              (int, IMAGES*, GLOBALS*);
void            __cdecl     ENTITY_Pause                                (ENTITY*, float);
int             __cdecl     ENTITY_CollidedWith                         (ENTITY*, ENTITY*);
int             __cdecl     ENTITY_WithinPoint                          (ENTITY*, FPOINT);
void            __cdecl     ENTITY_MoveTo                               (ENTITY*, ENTITY*, GLOBALS*);
void            __cdecl     ENTITY_MoveToPoint                          (ENTITY*, FPOINT, GLOBALS*);
void            __cdecl     ENTITY_FindMinorVector                      (ENTITY*, GLOBALS*);
FPOINT          __cdecl     ENTITY_MinorVectorHead                      (ENTITY*, ENTITY*, GLOBALS*);
void            __cdecl     ENTITY_UpdatePosition                       (ENTITY*, GLOBALS*);
void            __cdecl     ENTITY_Redefine                             (int, GLOBALS*);
void            __cdecl     ENTITY_DeleteAll                            (GLOBALS*);
void            __cdecl     ENTITY_DeleteSelected                       (GLOBALS*);
void            __cdecl     ENTITY_DeleteSpecific                       (ENTITY*, GLOBALS*);
void            __cdecl     ENTITY_DeleteEntityType                     (int, GLOBALS*);
void            __cdecl     ENTITY_SortToFront                          (int, GLOBALS*);
void            __cdecl     ENTITY_PrintList                            (GLOBALS*);
void            __cdecl     ENTITY_PrintClosestEntitiesList             (AI_CLOSEST*);
void            __cdecl     ENTITY_Animate                              (ENTITY*, IMAGES*);
SELECTED_COUNT  __cdecl     ENTITY_GetSelectedEntityCounts              (GLOBALS*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GLOBALS*        __cdecl     GLOBALS_Create                              (void);
void            __cdecl     GLOBALS_Zero                                (GLOBALS*);
int             __cdecl     GLOBALS_Kill                                (GLOBALS*);
void            __cdecl     GLOBALS_Init                                (GLOBALS*, HINSTANCE);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ENTITY*         __cdecl     AI_FindClosest                              (ENTITY*, int, GLOBALS*);
void            __cdecl     AI_HandleWorkers                            (ENTITY*, GLOBALS*);
AI_CLOSEST*     __cdecl     AI_FindClosestByDistance                    (ENTITY*, int, int*, int*, GLOBALS*);
int             __cdecl     AI_CompareDistances                         (const void*, const void*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int             __cdecl     MISC_RandomNumber                           (int, int);
float           __cdecl     MISC_CalculateRadians                       (float);
void            __cdecl     MISC_ResizeWindow                           (HWND, int, int);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            __cdecl     PROC_ProcessScene                           (DBLBUF*, GLOBALS*, IMAGES*, CARD*, MENU*);
void            __cdecl     PROC_DrawSelectionArea                      (DBLBUF*, GLOBALS*, COLORREF);
void            __cdecl     PROC_CaptureAndApplyTranslations            (DBLBUF*, GLOBALS*, IMAGES*, MENU*);
void            __cdecl     PROC_ApplyTranslations                      (float, float, GLOBALS*, IMAGES*);
void            __cdecl     PROC_UpdateAnimations                       (GLOBALS*);
void            __cdecl     PROC_DrawBackground                         (DBLBUF*, GLOBALS*, IMAGES*);
void            __cdecl     PROC_ProcessEntities                        (DBLBUF*, GLOBALS*, IMAGES*, MENU*);
void            __cdecl     PROC_ProcessMessages                        (DBLBUF*, GLOBALS*, IMAGES*, MENU*);
void            __cdecl     PROC_DrawDiagnostics                        (DBLBUF*, GLOBALS*, IMAGES*, MENU*);
void            __cdecl     PROC_DrawResourceBar                        (DBLBUF*, GLOBALS*, IMAGES*, MENU*);
void            __cdecl     PROC_DrawTaskbar                            (DBLBUF*, GLOBALS*, IMAGES*, CARD*, MENU*);
void            __cdecl     PROC_HandleMinimap                          (DBLBUF*, GLOBALS*, IMAGES* p_Images);
void            __cdecl     PROC_DrawMinimapPicture                     (DBLBUF*, GLOBALS*, IMAGES* p_Images, FPOINT);
void            __cdecl     PROC_DrawMinimapEntities                    (DBLBUF*, GLOBALS*, FPOINT);
void            __cdecl     PROC_DrawMinimapViewport                    (DBLBUF*, GLOBALS*, IMAGES* p_Images, FPOINT);
void            __cdecl     PROC_DrawMinimapSelectionArea               (DBLBUF*, GLOBALS*, FPOINT);
void            __cdecl     PROC_HandleHud                              (DBLBUF*, GLOBALS*, IMAGES*, CARD*, MENU*);
void            __cdecl     PROC_DrawBuildLimits                        (DBLBUF*, ENTITY*, IMAGES*);
void            __cdecl     PROC_DrawBuildType                          (DBLBUF*, GLOBALS*, IMAGES*, MENU*);
void            __cdecl     PROC_PopulateViewportRelativity             (FPOINT*, FPOINT*, FPOINT*, FPOINT*, DBLBUF*, IMAGES*, GLOBALS*);
void            __cdecl     PROC_PopulateMinimapDimensions              (FPOINT*, FPOINT*, DBLBUF*, IMAGES*);
int             __cdecl     PROC_IsMinimapClicked                       (DBLBUF*, IMAGES*, GLOBALS*);
void            __cdecl     PROC_AdjustMinimapViewport                  (GLOBALS*, IMAGES*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            __cdecl     PICTURE_Zero                                (PICTURE*);
void            __cdecl     PICTURE_Load                                (PICTURE*, FPOINT, char*, char*);
void            __cdecl     PICTURE_Kill                                (PICTURE*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            __cdecl     IMAGES_Zero                                 (IMAGES*);
IMAGES*         __cdecl     IMAGES_Create                               (GLOBALS*);
void            __cdecl     IMAGES_InitFromFile                         (IMAGES*, HWND);
void            __cdecl     IMAGES_Kill                                 (IMAGES*, GLOBALS*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            __cdecl     MESSAGE_Zero                                (MESSAGE*);
void            __cdecl     MESSAGE_Create                              (char*, FPOINT, int, GLOBALS*);
void            __cdecl     MESSAGE_DeleteSpecific                      (MESSAGE*, GLOBALS*);
void            __cdecl     MESSAGE_DeleteAll                           (GLOBALS*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            __cdecl     RENDER_Init                                 (DBLBUF*);
void            __cdecl     RENDER_ApplyTransform                       (DBLBUF*, int, float, FPOINT);
void            __cdecl     RENDER_ResetTransform                       (DBLBUF*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            __cdecl     CARD_Zero                                   (CARD*);
CARD*           __cdecl     CARD_Create                                 (GLOBALS*, IMAGES*);
void            __cdecl     CARD_Kill                                   (CARD*, GLOBALS*);
void            __cdecl     CARD_EvaluateSelected                       (CARD*, GLOBALS*, IMAGES*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
INT_PTR         CALLBACK    WINPROC_DlgLoad                             (HWND, UINT, WPARAM, LPARAM);
LRESULT         CALLBACK    WINPROC_WindowProc                          (HWND, UINT, WPARAM, LPARAM);
void            __cdecl     WINPROC_BuildHelper                         (int, MENU*);
void            __cdecl     WINPROC_SendWorkers                         (GLOBALS*, IMAGES*);
void            __cdecl     WINPROC_DistributeAndSendWorkers            (GLOBALS*, IMAGES*);
void            __cdecl     WINPROC_SelectEntities                      (GLOBALS*);
void            __cdecl     WINPROC_CreateOrCaptureEntities             (GLOBALS*, IMAGES*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            __cdecl     MENU_Zero                                   (MENU*);
MENU*           __cdecl     MENU_Create                                 (GLOBALS*);
void            __cdecl     MENU_Init                                   (MENU*);
void            __cdecl     MENU_Kill                                   (MENU*, GLOBALS*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
