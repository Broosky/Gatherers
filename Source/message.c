/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/constants.h"
#include "../Headers/globals.h"
#include "../Headers/heap_allocator.h"
#include "../Headers/log.h"
#include "../Headers/message.h"
#include "../Headers/misc.h"
#include "../Headers/Windows/windows_main.h"
#include <stdio.h>
#include <stdlib.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MESSAGE_Zero(MESSAGE_T* p_Message) {
    ZeroMemory(p_Message, sizeof(MESSAGE_T));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MESSAGE_Create(char* p_szMessage, FPOINT_T Location, MESSAGE_TYPE_T eType, GLOBALS_T* p_Globals, CONSTANTS_T* p_Constants, LOG_T* p_Log) {
    MESSAGE_T* p_Message = (MESSAGE_T*)HEAP_ALLOCATOR_Alloc(p_Globals->p_MessagesAllocator, p_Log);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!p_Message) {
        MISC_WriteOut(p_Log, LOG_SEVERITY_FATAL, "MESSAGE_Create(): Allocation failed.\n");
        UINT8 ubDiscard = MAIN_FailFast(p_Globals, p_Log);
        return;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Message->sLifespan = p_Constants->sMessageLifespan;
    p_Message->fScale = 1.0f;
    p_Message->fAngle = 0.0f;
    p_Message->Location = Location;
    p_Message->eType = eType;
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
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                HEAP_ALLOCATOR_Free(p_Globals->p_MessagesAllocator, p_Message, NULL);
                break;
            }
            else {
                p_Previous->p_Next = (struct MESSAGE_T*)p_Current->p_Next;
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                HEAP_ALLOCATOR_Free(p_Globals->p_MessagesAllocator, p_Message, NULL);
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
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        HEAP_ALLOCATOR_Free(p_Globals->p_MessagesAllocator, p_Temp, NULL);
    }
    p_Globals->p_RootMessage = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
