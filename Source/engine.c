/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/card.h"
#include "../Headers/common.h"
#include "../Headers/double_buffer.h"
#include "../Headers/globals.h"
#include "../Headers/menu.h"
#include "../Headers/process.h"
#include "../Headers/timebase.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main engine processing. All operations are centralized in this one, timed, function.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENGINE_ProcessScene(DOUBLE_BUFFER_T* p_DoubleBuffer, GLOBALS_T* p_Globals, ASSETS_T* p_Assets, CARD_T* p_Card, MENU_T* p_Menu) {
    TIMEBASE_T* p_Timer = TIMEBASE_Create(0.0f, p_Globals);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PROCESS_CaptureAndApplyTranslations(p_DoubleBuffer, p_Globals, p_Assets, p_Menu);
    PROCESS_DrawBackground(p_DoubleBuffer, p_Globals, p_Assets);
    PROCESS_UpdateAnimations(p_Globals);
    PROCESS_ProcessEntities(p_DoubleBuffer, p_Globals, p_Assets, p_Menu);
    PROCESS_DrawSelectionArea(p_DoubleBuffer, p_Globals, RGB(0, 255, 0));
    PROCESS_DrawBuildType(p_DoubleBuffer, p_Globals, p_Assets, p_Menu);
    PROCESS_ProcessMessages(p_DoubleBuffer, p_Globals, p_Assets, p_Menu);
    CARD_EvaluateSelected(p_Card, p_Globals, p_Assets);
    PROCESS_HandleHud(p_DoubleBuffer, p_Globals, p_Assets, p_Card, p_Menu);
    PROCESS_DrawDiagnostics(p_DoubleBuffer, p_Globals, p_Assets, p_Menu);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DOUBLE_BUFFER_Flip(p_DoubleBuffer);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->fEngineTime = TIMEBASE_EndTimer(p_Timer);
    p_Globals->fEngineTimeSum += p_Globals->fEngineTime;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TIMEBASE_Kill(p_Timer, p_Globals);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
