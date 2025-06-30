/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MISC_H_
#define _MISC_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "enums.h"
#include "Windows/windows_minified.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Types:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef void* (__cdecl* GET_NEXT_NODE)      (void* vp_Node);
typedef void  (__cdecl* COPY_NODE)          (void* vp_DestArrayElement, void* vp_Node);
typedef void  (__cdecl* SET_NEXT_NODE)      (void* vp_Node, void* vp_Next);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declares:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct LOG LOG_T;
typedef struct GLOBALS GLOBALS_T;
typedef struct SETTINGS SETTINGS_T;
typedef struct CONSTANTS CONSTANTS_T;
typedef struct HEAP_ALLOCATOR HEAP_ALLOCATOR_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int                 __cdecl     MISC_RandomNumber                               (int, int);
float               __cdecl     MISC_CalculateRadians                           (CONSTANTS_T*, float);
void                __cdecl     MISC_ResizeWindow                               (HWND, UINT, UINT, GLOBALS_T*);
void                __cdecl     MISC_FormatTime                                 (ULONG, char*, size_t);
void                __cdecl     MISC_FormatUnsignedWithCommas                   (char*, size_t, unsigned long long);
void                __cdecl     MISC_FormatSignedWithCommas                     (char*, size_t, long long);
USHORT              __cdecl     MISC_ClampU16                                   (USHORT, USHORT, USHORT);
ULONG               __cdecl     MISC_ClampU32                                   (ULONG, ULONG, ULONG);
float               __cdecl     MISC_ClampF32                                   (float, float, float);
char*               __cdecl     MISC_Trim                                       (char*);
void                __cdecl     MISC_ReceiveHotReload                           (SETTINGS_T*, GLOBALS_T*);
void                __cdecl     MISC_WriteOut                                   (LOG_T*, LOG_SEVERITY_T, const char*);
void                __cdecl     MISC_WriteOutParams                             (LOG_T*, LOG_SEVERITY_T, const char*, ...);
void                __cdecl     MISC_FormatWithSeverity                         (char*, size_t, LOG_SEVERITY_T, const char*);
char*               __cdecl     MISC_GetFullPath                                (const char[MAX_PATH], char*, LOG_T*);
size_t              __cdecl     MISC_LinkedListToArray                          (void*, size_t, void*, size_t, GET_NEXT_NODE, COPY_NODE);
void                __cdecl     MISC_LinkedListFreeAll                          (void*, HEAP_ALLOCATOR_T*, GET_NEXT_NODE, SET_NEXT_NODE, LOG_T*);
char*               __cdecl     MISC_GetRendererTypeName                        (RENDERER_TYPE_T);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void*               __cdecl     FPOINT_LIST_GetNextNode                         (void*);
void                __cdecl     FPOINT_LIST_CopyNode                            (void*, void*);
void                __cdecl     FPOINT_LIST_SetNextNode                         (void*, void*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void*               __cdecl     FRECT_LIST_GetNextNode                          (void*);
void                __cdecl     FRECT_LIST_CopyNode                             (void*, void*);
void                __cdecl     FRECT_LIST_SetNextNode                          (void*, void*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
