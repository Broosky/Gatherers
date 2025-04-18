/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/main.h"
#include <windows.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Be warned of automatic formatting from Visual Studio!
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct GLOBALS_T 		GLOBALS;
typedef struct ENTITY_T 		ENTITY;
typedef struct FPOINT_T 		FPOINT;
typedef struct IMAGES_T 		IMAGES;
typedef struct DBLBUF_T 		DBLBUF;
typedef struct TIMEBASE_T 		TIMEBASE;
typedef struct CARD_T 			CARD;
typedef struct MENU_T 			MENU;
typedef struct SETTINGS_T 		SETTINGS;
typedef struct LOG_T 			LOG;
typedef struct AI_CLOSEST_T 	AI_CLOSEST;
typedef struct PICTURE_T 		PICTURE;
typedef struct FDELTA_T 		FDELTA;
typedef struct IPOINT_T 		IPOINT;
typedef struct SELECTED_COUNT_T SELECTED_COUNT;
typedef struct MESSAGE_T 		MESSAGE;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            __cdecl     	SETTINGS_Zero(SETTINGS*);
SETTINGS*		__cdecl     	SETTINGS_Create(GLOBALS*);
void            __cdecl     	SETTINGS_InitFromFile(SETTINGS*);
void            __cdecl     	SETTINGS_Kill(SETTINGS*, GLOBALS*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            __cdecl     	DBLBUF_Zero(DBLBUF*);
DBLBUF*			__cdecl			DBLBUF_Create(HWND, GLOBALS*);
void            __cdecl			DBLBUF_Resize(DBLBUF*, HWND, COLORREF, GLOBALS*);
void            __cdecl			DBLBUF_Kill(DBLBUF*, GLOBALS*);
IPOINT          __cdecl			DBLBUF_FindBlitterPoint(char);
void            __cdecl			DBLBUF_SetBlitter(DBLBUF*, PICTURE*);
void            __cdecl			DBLBUF_Blitter(DBLBUF*, char*, FPOINT, UINT8);
void            __cdecl			DBLBUF_Clear(DBLBUF*, COLORREF);
void            __cdecl			DBLBUF_DrawEntityMinorVector(DBLBUF*, ENTITY*, COLORREF);
void            __cdecl			DBLBUF_DrawEntityMajorVector(DBLBUF*, ENTITY*, COLORREF);
void            __cdecl			DBLBUF_DrawEntityEllipse(DBLBUF*, ENTITY*, COLORREF, COLORREF);
void            __cdecl			DBLBUF_DrawEntity(DBLBUF*, ENTITY*, UINT8);
void            __cdecl			DBLBUF_ClearEntity(DBLBUF*, ENTITY*, COLORREF);
void            __cdecl			DBLBUF_FlipEntity(DBLBUF*, ENTITY*);
void            __cdecl			DBLBUF_Flip(DBLBUF*);
void            __cdecl			DBLBUF_FlipArea(DBLBUF*, int, int, int, int);
void            __cdecl			DBLBUF_DrawPicture(DBLBUF*, PICTURE*, UINT8);
void            __cdecl			DBLBUF_DrawPictureAt(DBLBUF*, PICTURE*, FPOINT, UINT8);
void            __cdecl			DBLBUF_CropDrawPictureAt(DBLBUF*, PICTURE*, FPOINT, FDELTA, UINT8);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void			__cdecl			LOG_Zero(LOG*);
LOG*			__cdecl			LOG_Create(const char*, GLOBALS*);
void			__cdecl			LOG_Append(LOG*, const char*);
void			__cdecl			LOG_Flush(LOG*, UINT8);
void			__cdecl			LOG_PopulateTimestamp(char*, size_t);
void			__cdecl			LOG_Kill(LOG*, GLOBALS*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            __cdecl			TIMEBASE_Zero(TIMEBASE*);
TIMEBASE*		__cdecl			TIMEBASE_Create(float, GLOBALS*);
UINT8           __cdecl			TIMEBASE_Tick(TIMEBASE*);
void            __cdecl			TIMEBASE_StartTimer(TIMEBASE*);
float           __cdecl			TIMEBASE_EndTimer(TIMEBASE*);
void            __cdecl			TIMEBASE_Kill(TIMEBASE*, GLOBALS*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double          __cdecl			BENCH_Run(void* (*)(void*), void* data);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            __cdecl			ENTITY_Zero(ENTITY*);
void            __cdecl			ENTITY_Create(FPOINT, USHORT, IMAGES*, GLOBALS*);
UINT8           __cdecl			ENTITY_Restrict(USHORT, GLOBALS*);
UINT8           __cdecl			ENTITY_Overlap(USHORT, IMAGES*, GLOBALS*);
void            __cdecl			ENTITY_Pause(ENTITY*, float);
UINT8           __cdecl			ENTITY_CollidedWith(ENTITY*, ENTITY*);
UINT8           __cdecl			ENTITY_WithinPoint(ENTITY*, FPOINT);
void            __cdecl			ENTITY_MoveTo(ENTITY*, ENTITY*, GLOBALS*);
void            __cdecl			ENTITY_MoveToPoint(ENTITY*, FPOINT, GLOBALS*);
void            __cdecl			ENTITY_FindMinorVector(ENTITY*, GLOBALS*);
FPOINT          __cdecl			ENTITY_MinorVectorHead(ENTITY*, ENTITY*, GLOBALS*);
void            __cdecl			ENTITY_UpdatePosition(ENTITY*, GLOBALS*);
void            __cdecl			ENTITY_Redefine(USHORT, GLOBALS*);
void            __cdecl			ENTITY_DeleteAll(GLOBALS*);
void            __cdecl			ENTITY_DeleteSelected(GLOBALS*);
void            __cdecl			ENTITY_DeleteSpecific(ENTITY*, GLOBALS*);
void            __cdecl			ENTITY_DeleteEntityType(USHORT, GLOBALS*);
void            __cdecl			ENTITY_SortToFront(USHORT, GLOBALS*);
void            __cdecl			ENTITY_PrintList(GLOBALS*);
void            __cdecl			ENTITY_PrintClosestEntitiesList(AI_CLOSEST*);
void            __cdecl			ENTITY_Animate(ENTITY*, IMAGES*);
SELECTED_COUNT  __cdecl			ENTITY_GetSelectedEntityCounts(GLOBALS*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GLOBALS*		__cdecl			GLOBALS_Create(void);
void            __cdecl			GLOBALS_Zero(GLOBALS*);
int             __cdecl			GLOBALS_Kill(GLOBALS*);
void            __cdecl			GLOBALS_Init(GLOBALS*, HINSTANCE);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ENTITY*			__cdecl			AI_FindClosest(ENTITY*, USHORT, GLOBALS*);
void            __cdecl			AI_HandleWorkers(ENTITY*, GLOBALS*);
AI_CLOSEST*		__cdecl			AI_FindClosestByDistance(ENTITY*, USHORT, int*, USHORT*, GLOBALS*);
int             __cdecl			AI_CompareDistances(const void*, const void*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int             __cdecl			MISC_RandomNumber(int, int);
float           __cdecl			MISC_CalculateRadians(float);
void            __cdecl			MISC_ResizeWindow(HWND, UINT, UINT);
void            __cdecl			MISC_FormatTime(ULONG, char*, size_t);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int             CALLBACK		WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow);
void            __cdecl			MAIN_LaunchConsole(void);
void			__cdecl			MAIN_ConsiderEngine(TIMEBASE*, TIMEBASE*, CARD*, DBLBUF*, GLOBALS*, IMAGES*, MENU*);
void			__cdecl			MAIN_HandleQuit(DBLBUF*, TIMEBASE*, TIMEBASE*, CARD*, HWND, GLOBALS*, IMAGES*, MENU*, SETTINGS*, LOG*);
void			__cdecl			MAIN_Kill(GLOBALS*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            __cdecl			ENGINE_ProcessScene(DBLBUF*, GLOBALS*, IMAGES*, CARD*, MENU*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            __cdecl			PROC_DrawSelectionArea(DBLBUF*, GLOBALS*, COLORREF);
void            __cdecl			PROC_CaptureAndApplyTranslations(DBLBUF*, GLOBALS*, IMAGES*, MENU*);
void            __cdecl			PROC_ApplyTranslations(float, float, GLOBALS*, IMAGES*);
void            __cdecl			PROC_UpdateAnimations(GLOBALS*);
void            __cdecl			PROC_DrawBackground(DBLBUF*, GLOBALS*, IMAGES*);
void            __cdecl			PROC_ProcessEntities(DBLBUF*, GLOBALS*, IMAGES*, MENU*);
void            __cdecl			PROC_ProcessMessages(DBLBUF*, GLOBALS*, IMAGES*, MENU*);
void            __cdecl			PROC_DrawDiagnostics(DBLBUF*, GLOBALS*, IMAGES*, MENU*);
void            __cdecl			PROC_DrawResourceBar(DBLBUF*, GLOBALS*, IMAGES*, MENU*);
void            __cdecl			PROC_DrawTaskbar(DBLBUF*, GLOBALS*, IMAGES*, CARD*, MENU*);
void            __cdecl			PROC_HandleMinimap(DBLBUF*, GLOBALS*, IMAGES*);
void            __cdecl			PROC_DrawMinimapPicture(DBLBUF*, GLOBALS*, IMAGES*, FPOINT);
void            __cdecl			PROC_DrawMinimapEntities(DBLBUF*, GLOBALS*, FPOINT);
void            __cdecl			PROC_DrawMinimapViewport(DBLBUF*, GLOBALS*, IMAGES*, FPOINT);
void            __cdecl			PROC_DrawMinimapSelectionArea(DBLBUF*, GLOBALS*, FPOINT);
void            __cdecl			PROC_HandleHud(DBLBUF*, GLOBALS*, IMAGES*, CARD*, MENU*);
void            __cdecl			PROC_DrawBuildLimits(DBLBUF*, ENTITY*, IMAGES*);
void            __cdecl			PROC_DrawBuildType(DBLBUF*, GLOBALS*, IMAGES*, MENU*);
void            __cdecl			PROC_PopulateViewportRelativity(FPOINT*, FPOINT*, FPOINT*, FPOINT*, DBLBUF*, IMAGES*, GLOBALS*);
void            __cdecl			PROC_PopulateMinimapDimensions(FPOINT*, FPOINT*, DBLBUF*, IMAGES*);
UINT8           __cdecl			PROC_IsMinimapClicked(DBLBUF*, IMAGES*, GLOBALS*);
void            __cdecl			PROC_AdjustMinimapViewport(GLOBALS*, IMAGES*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            __cdecl			PICTURE_Zero(PICTURE*);
void            __cdecl			PICTURE_Load(PICTURE*, FPOINT, char*, char*);
void            __cdecl			PICTURE_Kill(PICTURE*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            __cdecl			IMAGES_Zero(IMAGES*);
IMAGES*			__cdecl			IMAGES_Create(GLOBALS*);
void            __cdecl			IMAGES_InitFromFile(IMAGES*, HWND);
void            __cdecl			IMAGES_Kill(IMAGES*, GLOBALS*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            __cdecl			MESSAGE_Zero(MESSAGE*);
void            __cdecl			MESSAGE_Create(char*, FPOINT, USHORT, GLOBALS*);
void            __cdecl			MESSAGE_DeleteSpecific(MESSAGE*, GLOBALS*);
void            __cdecl			MESSAGE_DeleteAll(GLOBALS*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////	////////////
void            __cdecl			RENDER_Init(DBLBUF*);
void            __cdecl			RENDER_ApplyTransform(DBLBUF*, USHORT, float, FPOINT);
void            __cdecl			RENDER_ResetTransform(DBLBUF*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            __cdecl			CARD_Zero(CARD*);
CARD*			__cdecl			CARD_Create(GLOBALS*, IMAGES*);
void            __cdecl			CARD_Kill(CARD*, GLOBALS*);
void            __cdecl			CARD_EvaluateSelected(CARD*, GLOBALS*, IMAGES*);
////////////////////////////	/////////////////////////////////////////////////////////////////////////////////////////////////
INT_PTR         CALLBACK		WINPROC_DlgLoad(HWND, UINT, WPARAM, LPARAM);
LRESULT         CALLBACK		WINPROC_WindowProc(HWND, UINT, WPARAM, LPARAM);
void            __cdecl			WINPROC_BuildHelper(USHORT, MENU*);
void            __cdecl			WINPROC_SendWorkers(GLOBALS*, IMAGES*);
void            __cdecl			WINPROC_DistributeAndSendWorkers(GLOBALS*, IMAGES*);
void            __cdecl			WINPROC_SelectEntities(GLOBALS*);
void            __cdecl			WINPROC_CreateOrCaptureEntities(GLOBALS*, IMAGES*);
void            __cdecl			WINPROC_HandleKeyDown(HWND, WPARAM, GLOBALS*, MENU*, DBLBUF*);
void            __cdecl			WINPROC_HandleMenuEvent(HWND, WPARAM, MENU*, GLOBALS*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            __cdecl			MENU_Zero(MENU*);
MENU*			__cdecl			MENU_Create(GLOBALS*);
void            __cdecl			MENU_Init(MENU*);
void            __cdecl			MENU_Kill(MENU*, GLOBALS*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
