/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _DOUBLE_BUFFER_H_
#define _DOUBLE_BUFFER_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "common_types.h"
#include "windows_macros.h"
#include <windows.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declares:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct GLOBALS GLOBALS_T;
typedef struct PICTURE PICTURE_T;
typedef struct ENTITY ENTITY_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Types:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct DOUBLE_BUFFER {
    char szBlitter[128];
    HDC hDC;
    HWND hWnd;
    HDC hDCMem;
    HDC hDCBmp;
    XFORM XForm;
    HBITMAP hCanvas;
    RECT ClientArea;
    HGDIOBJ hStorage;
    PICTURE_T* p_Blitter;
} DOUBLE_BUFFER_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void                __cdecl     DOUBLE_BUFFER_Zero                      (DOUBLE_BUFFER_T*);
DOUBLE_BUFFER_T*    __cdecl     DOUBLE_BUFFER_Create                    (HWND, GLOBALS_T*);
void                __cdecl     DOUBLE_BUFFER_Resize                    (DOUBLE_BUFFER_T*, HWND, COLORREF, GLOBALS_T*);
void                __cdecl     DOUBLE_BUFFER_Kill                      (DOUBLE_BUFFER_T*, GLOBALS_T*);
IPOINT_T            __cdecl     DOUBLE_BUFFER_FindBlitterPoint          (char);
void                __cdecl     DOUBLE_BUFFER_SetBlitter                (DOUBLE_BUFFER_T*, PICTURE_T*);
void                __cdecl     DOUBLE_BUFFER_Blitter                   (DOUBLE_BUFFER_T*, char*, FPOINT_T, UINT8);
void                __cdecl     DOUBLE_BUFFER_Clear                     (DOUBLE_BUFFER_T*, COLORREF);
void                __cdecl     DOUBLE_BUFFER_DrawEntityMinorVector     (DOUBLE_BUFFER_T*, ENTITY_T*, COLORREF);
void                __cdecl     DOUBLE_BUFFER_DrawEntityMajorVector     (DOUBLE_BUFFER_T*, ENTITY_T*, COLORREF);
void                __cdecl     DOUBLE_BUFFER_DrawEntityEllipse         (DOUBLE_BUFFER_T*, ENTITY_T*, COLORREF, COLORREF);
void                __cdecl     DOUBLE_BUFFER_DrawEntity                (DOUBLE_BUFFER_T*, ENTITY_T*, UINT8);
void                __cdecl     DOUBLE_BUFFER_ClearEntity               (DOUBLE_BUFFER_T*, ENTITY_T*, COLORREF);
void                __cdecl     DOUBLE_BUFFER_FlipEntity                (DOUBLE_BUFFER_T*, ENTITY_T*);
void                __cdecl     DOUBLE_BUFFER_Flip                      (DOUBLE_BUFFER_T*);
void                __cdecl     DOUBLE_BUFFER_FlipArea                  (DOUBLE_BUFFER_T*, int, int, int, int);
void                __cdecl     DOUBLE_BUFFER_DrawPicture               (DOUBLE_BUFFER_T*, PICTURE_T*, UINT8);
void                __cdecl     DOUBLE_BUFFER_DrawPictureAt             (DOUBLE_BUFFER_T*, PICTURE_T*, FPOINT_T, UINT8);
void                __cdecl     DOUBLE_BUFFER_CropDrawPictureAt         (DOUBLE_BUFFER_T*, PICTURE_T*, FPOINT_T, FDELTA_T, UINT8);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
