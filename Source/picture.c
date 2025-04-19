/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _PICTURE_C_
#define _PICTURE_C_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/types.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PICTURE_Zero(PICTURE* p_Picture) {
    ZeroMemory(p_Picture, sizeof(PICTURE));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PICTURE_Load(PICTURE* p_Picture, FPOINT Location, char* p_szFileName, char* p_szFileNameMask) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PICTURE_Zero(p_Picture);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Picture->hBmp = LoadImage(NULL, p_szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!p_Picture->hBmp) {
        printf("PICTURE_Load(): Picture file not found (%s).\n", p_szFileName);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Picture->hBmpMask = LoadImage(NULL, p_szFileNameMask, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!p_Picture->hBmpMask) {
        printf("PICTURE_Load(): Mask file not found (%s).\n", p_szFileNameMask);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Picture->Location = Location;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GetObject(p_Picture->hBmp, sizeof(BITMAP), &p_Picture->Bitmap);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl PICTURE_Kill(PICTURE* p_Picture) {
    DeleteObject(p_Picture->hBmp);
    DeleteObject(p_Picture->hBmpMask);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
