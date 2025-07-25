/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/card.h"
#include "../Headers/constants.h"
#include "../Headers/engine.h"
#include "../Headers/globals.h"
#include "../Headers/menu.h"
#include "../Headers/process.h"
#include "../Headers/Renderer/renderer.h"
#include "../Headers/timebase.h"
#include "../Headers/Windows/windows_main.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENGINE_ProcessScene(RENDERER_T* p_Renderer,
    TIMEBASE_T* p_FrameTime,
    GLOBALS_T* p_Globals,
    ASSETS_T* p_Assets,
    CARD_T* p_Card,
    MENU_T* p_Menu,
    LOG_T* p_Log,
    SETTINGS_T* p_Settings,
    CONSTANTS_T* p_Constants) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_CaptureBorderTranslations(p_Renderer,
        p_Globals,
        p_Assets,
        p_Menu,
        p_Settings,
        p_Log);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FDELTA_T AppliedTranslations = PROCESS_ApplyTranslations(p_Renderer,
        p_Globals,
        p_Assets,
        p_Constants);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_ResetTranslations(p_Renderer);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ENGINE_ConsiderFullFrameRender(p_Renderer,
        p_Menu,
        p_Globals,
        AppliedTranslations);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Pre-process the current frame with data from the previous frame.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_HandlePreProcessing(p_Renderer,
        p_Menu,
        p_Assets,
        p_Globals,
        p_Log,
        p_Constants);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Process and pass through to the renderer.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_UpdateAnimations(p_Globals);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_ProcessEntities(p_Renderer,
        p_Globals,
        p_Assets,
        p_Menu,
        p_Log,
        p_Settings,
        p_Constants);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_DrawSelectionArea(p_Renderer,
        p_Globals,
        p_Assets);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_DrawBuildType(p_Renderer,
        p_Globals,
        p_Assets,
        p_Menu,
        p_Log);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_ProcessMessages(p_Renderer,
        p_Globals,
        p_Assets,
        p_Menu,
        p_Constants,
        p_Log);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CARD_EvaluateSelected(p_Card,
        p_Globals,
        p_Assets);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_HandleHud(p_Renderer,
        p_Globals,
        p_Assets,
        p_Card,
        p_Menu,
        p_Settings,
        p_Constants,
        p_Log);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_HandleDiagnostics(p_Renderer,
        p_Globals,
        p_Assets,
        p_Menu,
        p_Settings);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->HandlePostProcessing(p_Renderer,
        p_Menu,
        p_Globals,
        p_Log);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->PresentFrame(p_Renderer);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->fEngineTime = TIMEBASE_EndTimer(p_FrameTime);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ENGINE_CaptureSmoothedFps(p_Globals->fEngineTime, p_Globals);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->fEngineTimeSum += p_Globals->fEngineTime;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TIMEBASE_ResetTimer(p_FrameTime);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENGINE_ConsiderFullFrameRender(RENDERER_T* p_Renderer, MENU_T* p_Menu, GLOBALS_T* p_Globals, FDELTA_T AppliedTranslations) {
    static UINT8 ubFullFrameRenderLast = 0;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Conditions determining if we will re-render the full frame.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Re-render everything if we're panning. Otherwise, we can be selective in what we draw instead of always additive. This
    // is only worth it if diagnostics and post-processing are disabled, otherwise the overhead can exceed the benefit. The 
    // same can happen in the minimap.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Consider global transitional states.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Renderer->ubIsFullFrameRender = (p_Globals->ubForceFullFrameRender |
        p_Menu->ubEnablePostProcessing |
        p_Menu->ubDrawIds |
        p_Menu->ubDrawStatistics |
        p_Menu->ubDrawResources |
        p_Menu->ubDrawStatuses |
        p_Menu->ubDrawMinorVectors |
        p_Menu->ubDrawMajorVectors |
        p_Menu->ubDrawDirtyZones |
        p_Menu->ubDrawTerrainGrid |
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Globals->ubCreate |
        p_Globals->ubIsSelecting) ||
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        AppliedTranslations.fDx != 0.0f ||
        AppliedTranslations.fDy != 0.0f;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Reset global transitional states.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->ubForceFullFrameRender = 0;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Before pre-processing, blip the renderer to draw a clean frame if we are entering selective drawing.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (ubFullFrameRenderLast && !p_Renderer->ubIsFullFrameRender) {
        ubFullFrameRenderLast = 0;
        p_Renderer->ubIsFullFrameRender = 1;
    }
    else if (p_Renderer->ubIsFullFrameRender) {
        ubFullFrameRenderLast = 1;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENGINE_CaptureSmoothedFps(float fFrameTime, GLOBALS_T* p_Globals) {
    static float fFrameTimes[SMOOTHED_FPS_WINDOW] = { 0.0f };
    static int iIndex = 0;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fFrameTimes[iIndex] = fFrameTime;
    iIndex = (iIndex + 1) % SMOOTHED_FPS_WINDOW;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    float fSum = 0.0f;
    int iI;
    for (iI = 0; iI < SMOOTHED_FPS_WINDOW; iI++) {
        fSum += fFrameTimes[iI];
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    float fAverageTime = fSum / SMOOTHED_FPS_WINDOW;
    if (fAverageTime > 0.0f) {
        p_Globals->fFpsEstimate = 1.0f / fAverageTime;
    }
    else {
        p_Globals->fFpsEstimate = 0.0f;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
