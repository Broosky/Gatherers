/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Program Name: Gatherers (C)                                                                                            //
//  Author: Jeffrey Bednar                                                                                                 //
//  Copyright: Illusion Interactive, 2011                                                                                  //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MESSAGE_C_
#define _MESSAGE_C_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/main.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MESSAGE_Zero(MESSAGE* p_Message) {
    ZeroMemory(p_Message, sizeof(MESSAGE));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MESSAGE_Create(char* p_szMessage, FPOINT Location, USHORT usType, GLOBALS* p_Globals) {
    MESSAGE* p_Message = (MESSAGE*)malloc(sizeof(MESSAGE));
    (*p_Globals).iRunningHeap += sizeof(MESSAGE);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MESSAGE_Zero(p_Message);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    (*p_Message).sLifespan = 35;
    (*p_Message).fScale = 1.0f;
    (*p_Message).fAngle = 0.0f;
    (*p_Message).Location = Location;
    (*p_Message).usType = usType;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    strcpy((*p_Message).szMessage, p_szMessage);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(!(*p_Globals).p_RootMessage) {
        (*p_Message).p_Next = NULL;
        (*p_Globals).p_RootMessage = p_Message;
    }
    else {
        (*p_Message).p_Next = (struct MESSAGE*)(*p_Globals).p_RootMessage;
        (*p_Globals).p_RootMessage = p_Message;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Deleting one specific message. When it is found, break out of the loop. Quick sort and binary search could be beneficial.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MESSAGE_DeleteSpecific(MESSAGE* p_Message, GLOBALS* p_Globals) {
    MESSAGE* p_Previous;
    MESSAGE* p_Current = (*p_Globals).p_RootMessage;
    while(p_Current) {
        if(p_Current == p_Message) {
            if(p_Current == (*p_Globals).p_RootMessage) {
                (*p_Globals).p_RootMessage = (MESSAGE*)(*p_Current).p_Next;
                free(p_Current);
                (*p_Globals).iRunningHeap -= sizeof(MESSAGE);
                break;
            }
            else {
                (*p_Previous).p_Next = (struct MESSAGE*)(*p_Current).p_Next;
                free(p_Current);
                (*p_Globals).iRunningHeap -= sizeof(MESSAGE);
                break;
            }
        }
        else {
            p_Previous = p_Current;
            p_Current = (MESSAGE*)(*p_Current).p_Next;
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl MESSAGE_DeleteAll(GLOBALS* p_Globals) {
    MESSAGE* p_Current = (*p_Globals).p_RootMessage;
    while(p_Current) {
        MESSAGE* p_Temp = p_Current;
        p_Current = (MESSAGE*)(*p_Current).p_Next;
        free(p_Temp);
        (*p_Globals).iRunningHeap -= sizeof(MESSAGE);
    }
    (*p_Globals).p_RootMessage = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
