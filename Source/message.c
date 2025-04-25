/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/common.h"
#include "../Headers/globals.h"
#include "../Headers/log.h"
#include "../Headers/main.h"
#include "../Headers/message.h"
#include <stdio.h>
#include <stdlib.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MESSAGE_Zero(MESSAGE_T* p_Message) {
    ZeroMemory(p_Message, sizeof(MESSAGE_T));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MESSAGE_Create(char* p_szMessage, FPOINT_T Location, USHORT usType, GLOBALS_T* p_Globals, LOG_T* p_Log) {
    size_t stAllocation = sizeof(MESSAGE_T);
    MESSAGE_T* p_Message = malloc(stAllocation);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!p_Message) {
        LOG_AppendParams(p_Log, "MESSAGE_Create(): malloc failed for size: %zu bytes\n", stAllocation);
        UINT8 _discard = MAIN_FailFast(p_Globals, p_Log);
        return;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Globals->stAllocations += stAllocation;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MESSAGE_Zero(p_Message);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Message->sLifespan = 35;
    p_Message->fScale = 1.0f;
    p_Message->fAngle = 0.0f;
    p_Message->Location = Location;
    p_Message->usType = usType;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    snprintf(p_Message->szMessage, sizeof(p_Message->szMessage), "%s", p_szMessage);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!p_Globals->p_RootMessage) {
        p_Message->p_Next = NULL;
        p_Globals->p_RootMessage = p_Message;
    }
    else {
        p_Message->p_Next = (struct MESSAGE_T*)p_Globals->p_RootMessage;
        p_Globals->p_RootMessage = p_Message;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Deleting one specific message. When it is found, break out of the loop. Quick sort and binary search could be beneficial.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MESSAGE_DeleteSpecific(MESSAGE_T* p_Message, GLOBALS_T* p_Globals) {
    MESSAGE_T* p_Previous = NULL;
    MESSAGE_T* p_Current = p_Globals->p_RootMessage;
    while (p_Current) {
        if (p_Current == p_Message) {
            if (p_Current == p_Globals->p_RootMessage) {
                p_Globals->p_RootMessage = (MESSAGE_T*)p_Current->p_Next;
                free(p_Current);
                p_Globals->stAllocations -= sizeof(MESSAGE_T);
                break;
            }
            else {
                p_Previous->p_Next = (struct MESSAGE_T*)p_Current->p_Next;
                free(p_Current);
                p_Globals->stAllocations -= sizeof(MESSAGE_T);
                break;
            }
        }
        else {
            p_Previous = p_Current;
            p_Current = (MESSAGE_T*)p_Current->p_Next;
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MESSAGE_DeleteAll(GLOBALS_T* p_Globals) {
    MESSAGE_T* p_Current = p_Globals->p_RootMessage;
    while (p_Current) {
        MESSAGE_T* p_Temp = p_Current;
        p_Current = (MESSAGE_T*)p_Current->p_Next;
        free(p_Temp);
        p_Globals->stAllocations -= sizeof(MESSAGE_T);
    }
    p_Globals->p_RootMessage = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
