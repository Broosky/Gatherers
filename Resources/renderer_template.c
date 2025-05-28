/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/globals.h"
#include "../Headers/log.h"
#include "../Headers/misc.h"
#include "../Headers/Renderer/renderer.h"
#include "../Headers/Windows/windows_minified.h"
#include <stdlib.h>
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
typedef struct RENDERER_IMPL {
    // Implementation of renderer.
    UINT8 ubDummyMember;
} RENDERER_IMPL_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void                __cdecl     RENDERER_IMPL_Zero                              (RENDERER_IMPL_T*);
RENDERER_IMPL_T*    __cdecl     RENDERER_IMPL_Create                            (RENDERER_T*, GLOBALS_T*, LOG_T*);
RENDERER_IMPL_T*    __cdecl     RENDERER_IMPL_Cast                              (RENDERER_T*);
void                __cdecl     RENDERER_IMPL_Kill                              (RENDERER_T*, GLOBALS_T*);
RECT                __cdecl     RENDERER_IMPL_GetClientArea                     (RENDERER_T*);
void                __cdecl     RENDERER_IMPL_Resize                            (RENDERER_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     RENDERER_IMPL_SetBlitter                        (RENDERER_T*, PICTURE_T*);
void                __cdecl     RENDERER_IMPL_Blitter                           (RENDERER_T*, char*, FPOINT_T, UINT8);
void                __cdecl     RENDERER_IMPL_Clear                             (RENDERER_T*, ASSETS_T*);
void                __cdecl     RENDERER_IMPL_ClearEntity                       (RENDERER_T*, ENTITY_T*, ASSETS_T*);
void                __cdecl     RENDERER_IMPL_FlipEntity                        (RENDERER_T*, ENTITY_T*);
void                __cdecl     RENDERER_IMPL_PresentFrame                      (RENDERER_T*);
void                __cdecl     RENDERER_IMPL_EnablingPostProcessing            (RENDERER_T*, MENU_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     RENDERER_IMPL_DisablingPostProcessing           (RENDERER_T*, MENU_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     RENDERER_IMPL_HandlePostProcessing              (RENDERER_T*, MENU_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     RENDERER_IMPL_HandlePreProcessing               (RENDERER_T*, MENU_T*, ASSETS_T*, GLOBALS_T*, LOG_T*);
void                __cdecl     RENDERER_IMPL_FlipArea                          (RENDERER_T*, int, int, int, int);
void                __cdecl     RENDERER_IMPL_ApplyWorldTransform               (RENDERER_T*, TRANSFORM_TYPE_T, CONSTANTS_T*, float, FPOINT_T, LOG_T*);
void                __cdecl     RENDERER_IMPL_ResetWorldTransform               (RENDERER_T*);
void                __cdecl     RENDERER_IMPL_InitWorldTransform                (RENDERER_T*);
void                __cdecl     RENDERER_IMPL_DrawMinimapEntities               (RENDERER_T*, GLOBALS_T*, FPOINT_T, ASSETS_T*, SETTINGS_T*, LOG_T*);
void                __cdecl     RENDERER_IMPL_DrawMinimapViewport               (RENDERER_T*, ASSETS_T*, FPOINT_T, FPOINT_T, FPOINT_T, FPOINT_T);
void                __cdecl     RENDERER_IMPL_DrawMinimapSelectionArea          (RENDERER_T*, ASSETS_T*, FPOINT_T, FPOINT_T, FPOINT_T, FPOINT_T);
void                __cdecl     RENDERER_IMPL_DrawBuildLimits                   (RENDERER_T*, ASSETS_T*, FPOINT_T, FPOINT_T, FPOINT_T, FPOINT_T);
void                __cdecl     RENDERER_IMPL_DrawBuildType                     (RENDERER_T*, MENU_T*, ASSETS_T*, PICTURE_T*, FPOINT_T, IRECT_T);
void                __cdecl     RENDERER_IMPL_DrawSelectionArea                 (RENDERER_T*, GLOBALS_T*, ASSETS_T*);
void                __cdecl     RENDERER_IMPL_DrawTranslationThreshold          (RENDERER_T*, ASSETS_T*, GLOBALS_T*);
void                __cdecl     RENDERER_IMPL_DrawPicture                       (RENDERER_T*, PICTURE_T*, UINT8);
void                __cdecl     RENDERER_IMPL_DrawPictureAt                     (RENDERER_T*, PICTURE_T*, FPOINT_T, UINT8);
void                __cdecl     RENDERER_IMPL_CropDrawPictureAt                 (RENDERER_T*, PICTURE_T*, FPOINT_T, FDELTA_T, UINT8);
void                __cdecl     RENDERER_IMPL_DrawEntityMinorVector             (RENDERER_T*, ENTITY_T*, ASSETS_T*);
void                __cdecl     RENDERER_IMPL_DrawEntityMajorVector             (RENDERER_T*, ENTITY_T*, ASSETS_T*);
void                __cdecl     RENDERER_IMPL_DrawEntityEllipse                 (RENDERER_T*, ENTITY_T*, HPEN, HBRUSH, SETTINGS_T*);
void                __cdecl     RENDERER_IMPL_DrawEntity                        (RENDERER_T*, ENTITY_T*, UINT8);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Local Only:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_Zero(RENDERER_IMPL_T* p_RendererImpl) {
    ZeroMemory(p_RendererImpl, sizeof(RENDERER_IMPL_T));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RENDERER_IMPL_T* __cdecl RENDERER_IMPL_Create(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, LOG_T* p_Log) {
    size_t stAllocation = sizeof(RENDERER_IMPL_T);
    RENDERER_IMPL_T* p_RendererImpl = malloc(stAllocation);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!p_RendererImpl) {
        MISC_WriteOutParams(p_Log, LOG_SEVERITY_FATAL, "RENDERER_IMPL_Create(): Malloc failed for size: %zu bytes\n", stAllocation);
        return NULL;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->stAllocations += stAllocation;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    RENDERER_IMPL_Zero(p_RendererImpl);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Renderer population.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->vp_Implementation = p_RendererImpl;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->GetClientArea = RENDERER_IMPL_GetClientArea;
    p_Renderer->Resize = RENDERER_IMPL_Resize;
    p_Renderer->Kill = RENDERER_IMPL_Kill;
    p_Renderer->SetBlitter = RENDERER_IMPL_SetBlitter;
    p_Renderer->Blitter = RENDERER_IMPL_Blitter;
    p_Renderer->Clear = RENDERER_IMPL_Clear;
    p_Renderer->ClearEntity = RENDERER_IMPL_ClearEntity;
    p_Renderer->FlipEntity = RENDERER_IMPL_FlipEntity;
    p_Renderer->PresentFrame = RENDERER_IMPL_PresentFrame;
    p_Renderer->HandlePreProcessing = RENDERER_IMPL_HandlePreProcessing;
    p_Renderer->EnablingPostProcessing = RENDERER_IMPL_EnablingPostProcessing;
    p_Renderer->DisablingPostProcessing = RENDERER_IMPL_DisablingPostProcessing;
    p_Renderer->HandlePostProcessing = RENDERER_IMPL_HandlePostProcessing;
    p_Renderer->FlipArea = RENDERER_IMPL_FlipArea;
    p_Renderer->ApplyWorldTransform = RENDERER_IMPL_ApplyWorldTransform;
    p_Renderer->ResetWorldTransform = RENDERER_IMPL_ResetWorldTransform;
    p_Renderer->InitWorldTransform = RENDERER_IMPL_InitWorldTransform;
    p_Renderer->DrawMinimapEntities = RENDERER_IMPL_DrawMinimapEntities;
    p_Renderer->DrawMinimapViewport = RENDERER_IMPL_DrawMinimapViewport;
    p_Renderer->DrawMinimapSelectionArea = RENDERER_IMPL_DrawMinimapSelectionArea;
    p_Renderer->DrawBuildLimits = RENDERER_IMPL_DrawBuildLimits;
    p_Renderer->DrawBuildType = RENDERER_IMPL_DrawBuildType;
    p_Renderer->DrawSelectionArea = RENDERER_IMPL_DrawSelectionArea;
    p_Renderer->DrawTranslationThreshold = RENDERER_IMPL_DrawTranslationThreshold;
    p_Renderer->DrawPicture = RENDERER_IMPL_DrawPicture;
    p_Renderer->DrawPictureAt = RENDERER_IMPL_DrawPictureAt;
    p_Renderer->CropDrawPictureAt = RENDERER_IMPL_CropDrawPictureAt;
    p_Renderer->DrawEntityMinorVector = RENDERER_IMPL_DrawEntityMinorVector;
    p_Renderer->DrawEntityMajorVector = RENDERER_IMPL_DrawEntityMajorVector;
    p_Renderer->DrawEntityEllipse = RENDERER_IMPL_DrawEntityEllipse;
    p_Renderer->DrawEntity = RENDERER_IMPL_DrawEntity;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Unused, retained internally in renderer.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return p_RendererImpl;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RENDERER_IMPL_T* __cdecl RENDERER_IMPL_Cast(RENDERER_T* p_Renderer) {
    return (RENDERER_IMPL_T*)p_Renderer->vp_Implementation;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Accessible via Renderer:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_Kill(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals) {
    RENDERER_IMPL_T* p_RendererImpl = RENDERER_IMPL_Cast(p_Renderer);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (p_RendererImpl) {
        free(p_RendererImpl);
        p_Globals->stAllocations -= sizeof(RENDERER_IMPL_T);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RECT __cdecl RENDERER_IMPL_GetClientArea(RENDERER_T* p_Renderer) {
    return (RECT){ 0, 0, 0 };
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_Resize(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, LOG_T* p_Log) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_SetBlitter(RENDERER_T* p_Renderer, PICTURE_T* p_Picture) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_Blitter(RENDERER_T* p_Renderer, char* p_szText, FPOINT_T Location, UINT8 ubMask) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_Clear(RENDERER_T* p_Renderer, ASSETS_T* p_Assets) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_ClearEntity(RENDERER_T* p_Renderer, ENTITY_T* p_Entity, ASSETS_T* p_Assets) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_FlipEntity(RENDERER_T* p_Renderer, ENTITY_T* p_Entity) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_PresentFrame(RENDERER_T* p_Renderer) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_EnablingPostProcessing(RENDERER_T* p_Renderer, MENU_T* p_Menu, GLOBALS_T* p_Globals, LOG_T* p_Log) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_DisablingPostProcessing(RENDERER_T* p_Renderer, MENU_T* p_Menu, GLOBALS_T* p_Globals, LOG_T* p_Log) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_HandlePostProcessing(RENDERER_T* p_Renderer, MENU_T* p_Menu, GLOBALS_T* p_Globals, LOG_T* p_Log) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_HandlePreProcessing(RENDERER_T* p_Renderer, MENU_T* p_Menu, ASSETS_T* p_Assets, GLOBALS_T* p_Globals, LOG_T* p_Log) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_FlipArea(RENDERER_T* p_Renderer, int iX, int iY, int iWidth, int iHeight) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_ApplyWorldTransform(RENDERER_T* p_Renderer, TRANSFORM_TYPE_T eType, CONSTANTS_T* p_Constants, float fValue, FPOINT_T Pin, LOG_T* p_Log) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_ResetWorldTransform(RENDERER_T* p_Renderer) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_InitWorldTransform(RENDERER_T* p_Renderer) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_DrawMinimapEntities(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, FPOINT_T MinimapOrigin, ASSETS_T* p_Assets, SETTINGS_T* p_Settings, LOG_T* p_Log) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_DrawMinimapViewport(RENDERER_T* p_Renderer, ASSETS_T* p_Assets, FPOINT_T TopLeft, FPOINT_T TopRight, FPOINT_T BottomRight, FPOINT_T BottomLeft) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_DrawMinimapSelectionArea(RENDERER_T* p_Renderer, ASSETS_T* p_Assets, FPOINT_T TopLeft, FPOINT_T TopRight, FPOINT_T BottomRight, FPOINT_T BottomLeft) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_DrawBuildLimits(RENDERER_T* p_Renderer, ASSETS_T* p_Assets, FPOINT_T TopLeft, FPOINT_T TopRight, FPOINT_T BottomRight, FPOINT_T BottomLeft) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_DrawBuildType(RENDERER_T* p_Renderer, MENU_T* p_Menu, ASSETS_T* p_Assets, PICTURE_T* p_Picture, FPOINT_T Location, IRECT_T BarrierArea) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_DrawSelectionArea(RENDERER_T* p_Renderer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_DrawTranslationThreshold(RENDERER_T* p_Renderer, ASSETS_T* p_Assets, GLOBALS_T* p_Globals) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_DrawPicture(RENDERER_T* p_Renderer, PICTURE_T* p_Picture, UINT8 ubMask) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_DrawPictureAt(RENDERER_T* p_Renderer, PICTURE_T* p_Picture, FPOINT_T Location, UINT8 ubMask) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_CropDrawPictureAt(RENDERER_T* p_Renderer, PICTURE_T* p_Picture, FPOINT_T CropStart, FDELTA_T CropDelta, UINT8 ubMask) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_DrawEntityMinorVector(RENDERER_T* p_Renderer, ENTITY_T* p_Entity, ASSETS_T* p_Assets) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_DrawEntityMajorVector(RENDERER_T* p_Renderer, ENTITY_T* p_Entity, ASSETS_T* p_Assets) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_DrawEntityEllipse(RENDERER_T* p_Renderer, ENTITY_T* p_Entity, HPEN hPen, HBRUSH hBrush, SETTINGS_T* p_Settings) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl RENDERER_IMPL_DrawEntity(RENDERER_T* p_Renderer, ENTITY_T* p_Entity, UINT8 ubMask) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
