/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _RENDERER_DOUBLE_BUFFER_H_
#define _RENDERER_DOUBLE_BUFFER_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../common.h"
#include "../common_types.h"
#include "../enums.h"
#include "../Windows/windows_minified.h"
#include <stdint.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declares:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct LOG LOG_T;
typedef struct MENU MENU_T;
typedef struct ENTITY ENTITY_T;
typedef struct ASSETS ASSETS_T;
typedef struct GLOBALS GLOBALS_T;
typedef struct PICTURE PICTURE_T;
typedef struct RENDERER RENDERER_T;
typedef struct SETTINGS SETTINGS_T;
typedef struct CONSTANTS CONSTANTS_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Types:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct PRE_PROCESSING {
    FRECT_LIST_T* DirtyZones;
} PRE_PROCESSING_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct POST_PROCESSING {
    uint32_t* p_u32;
    size_t stAllocations;
} POST_PROCESSING_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct DOUBLE_BUFFER {
    HDC hDC;
    HWND hWnd;
    HDC hDCMem;
    HDC hDCBmp;
    XFORM XForm;
    HBITMAP hCanvas;
    RECT ClientArea;
    HGDIOBJ hStorage;
    PICTURE_T* p_Blitter;
    BITMAPINFO BitmapInfo;
    void* vp_FrameBuffer;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Implementation specific pre-processing:
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PRE_PROCESSING_T PreProcessing;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Implementation specific post-processing:
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Framebuffer copies
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    POST_PROCESSING_T p_u32Temp;
    POST_PROCESSING_T p_u32Horizontal;
} DOUBLE_BUFFER_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void                __cdecl     DOUBLE_BUFFER_Zero                              (DOUBLE_BUFFER_T*);
DOUBLE_BUFFER_T*    __cdecl     DOUBLE_BUFFER_Create                            (RENDERER_T*, GLOBALS_T*, LOG_T*);
DOUBLE_BUFFER_T*    __cdecl     DOUBLE_BUFFER_Cast                              (RENDERER_T*);
void                __cdecl     DOUBLE_BUFFER_Kill                              (RENDERER_T*, GLOBALS_T*);
RECT                __cdecl     DOUBLE_BUFFER_GetSize                           (RENDERER_T*);
void                __cdecl     DOUBLE_BUFFER_Resize                            (RENDERER_T*, GLOBALS_T*, LOG_T*);
IPOINT_T            __cdecl     DOUBLE_BUFFER_FindBlitterPoint                  (char);
void                __cdecl     DOUBLE_BUFFER_SetBlitter                        (RENDERER_T*, PICTURE_T*);
void                __cdecl     DOUBLE_BUFFER_CreateCompatibleBitmap            (DOUBLE_BUFFER_T*, LOG_T*);
UINT8               __cdecl     DOUBLE_BUFFER_CreateDibSection                  (DOUBLE_BUFFER_T*, LOG_T*);
void                __cdecl     DOUBLE_BUFFER_Blitter                           (RENDERER_T*, char*, FPOINT_T, UINT8);
void                __cdecl     DOUBLE_BUFFER_Clear                             (RENDERER_T*, ASSETS_T*);
void                __cdecl     DOUBLE_BUFFER_DrawEntityMinorVector             (RENDERER_T*, ENTITY_T*, ASSETS_T*);
void                __cdecl     DOUBLE_BUFFER_DrawEntityMajorVector             (RENDERER_T*, ENTITY_T*, ASSETS_T*);
void                __cdecl     DOUBLE_BUFFER_DrawEntityEllipse                 (RENDERER_T*, ENTITY_T*, HPEN, HBRUSH, SETTINGS_T*);
void                __cdecl     DOUBLE_BUFFER_DrawEntity                        (RENDERER_T*, ENTITY_T*, UINT8);
void                __cdecl     DOUBLE_BUFFER_ClearEntity                       (RENDERER_T*, ENTITY_T*, ASSETS_T*);
void                __cdecl     DOUBLE_BUFFER_FlipEntity                        (RENDERER_T*, ENTITY_T*);
void                __cdecl     DOUBLE_BUFFER_PresentFrame                      (RENDERER_T*);
void                __cdecl     DOUBLE_BUFFER_HandlePostProcessing              (RENDERER_T*, MENU_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     DOUBLE_BUFFER_HandlePreProcessing               (RENDERER_T*, MENU_T*, ASSETS_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     DOUBLE_BUFFER_FlipArea                          (RENDERER_T*, int, int, int, int);
RECT                __cdecl     DOUBLE_BUFFER_GetClientArea                     (RENDERER_T*);
void                __cdecl     DOUBLE_BUFFER_DrawPicture                       (RENDERER_T*, PICTURE_T*, UINT8);
void                __cdecl     DOUBLE_BUFFER_DrawPictureAt                     (RENDERER_T*, PICTURE_T*, FPOINT_T, UINT8);
void                __cdecl     DOUBLE_BUFFER_CropDrawPictureAt                 (RENDERER_T*, PICTURE_T*, FPOINT_T, FDELTA_T, UINT8);
void                __cdecl     DOUBLE_BUFFER_InitWorldTransform                (RENDERER_T*);
void                __cdecl     DOUBLE_BUFFER_ApplyWorldTransform               (RENDERER_T*, TRANSFORM_TYPE_T, CONSTANTS_T*, float, FPOINT_T, LOG_T*);
void                __cdecl     DOUBLE_BUFFER_ResetWorldTransform               (RENDERER_T*);
void                __cdecl     DOUBLE_BUFFER_DrawSelectionArea                 (RENDERER_T*, GLOBALS_T*, ASSETS_T*);
void                __cdecl     DOUBLE_BUFFER_DrawTranslationThreshold          (RENDERER_T*, ASSETS_T*, GLOBALS_T*);
void                __cdecl     DOUBLE_BUFFER_DrawMinimapEntities               (RENDERER_T*, GLOBALS_T*, FPOINT_T, ASSETS_T*, SETTINGS_T*, LOG_T*);
void                __cdecl     DOUBLE_BUFFER_DrawMinimapViewport               (RENDERER_T*, ASSETS_T*, FPOINT_T, FPOINT_T, FPOINT_T, FPOINT_T);
void                __cdecl     DOUBLE_BUFFER_DrawMinimapSelectionArea          (RENDERER_T*, ASSETS_T*, FPOINT_T, FPOINT_T, FPOINT_T, FPOINT_T);
void                __cdecl     DOUBLE_BUFFER_DrawBuildType                     (RENDERER_T*, MENU_T*, ASSETS_T*, PICTURE_T*, FPOINT_T, IRECT_T);
void                __cdecl     DOUBLE_BUFFER_DrawBuildLimits                   (RENDERER_T*, ASSETS_T*, FPOINT_T, FPOINT_T, FPOINT_T, FPOINT_T);
void                __cdecl     DOUBLE_BUFFER_EnablingPostProcessing            (RENDERER_T*, MENU_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     DOUBLE_BUFFER_DisablingPostProcessing           (RENDERER_T*, MENU_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     DOUBLE_BUFFER_HandleBoxBlur                     (DOUBLE_BUFFER_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     DOUBLE_BUFFER_HandleGreenMask                   (DOUBLE_BUFFER_T*, uint8_t);
void                __cdecl     DOUBLE_BUFFER_HandleDarken                      (DOUBLE_BUFFER_T*, uint8_t);
void                __cdecl     DOUBLE_BUFFER_KillPostProcessing                (DOUBLE_BUFFER_T*, GLOBALS_T*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
