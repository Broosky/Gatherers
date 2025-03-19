/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Program Name: Gatherers (C)                                                                                            //
//  Author: Jeffrey Bednar                                                                                                 //
//  Copyright: Illusion Interactive, 2011                                                                                  //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _DBLBUF_C_
#define _DBLBUF_C_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "..\Headers\main.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_Zero(DBLBUF* p_DblBuf) {
    ZeroMemory(p_DblBuf, sizeof(DBLBUF));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DBLBUF* __cdecl DBLBUF_Create(HWND hWnd, GLOBALS* p_Globals) {
    DBLBUF* p_DblBuf = (DBLBUF*)malloc(sizeof(DBLBUF));
    (*p_Globals).iRunningHeap += sizeof(DBLBUF);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DBLBUF_Zero(p_DblBuf);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GetClientRect(hWnd, &(*p_DblBuf).ClientArea);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    (*p_DblBuf).hWnd = hWnd;
    (*p_DblBuf).hDC = GetDC((*p_DblBuf).hWnd);
    (*p_DblBuf).hDCMem = CreateCompatibleDC((*p_DblBuf).hDC);
    (*p_DblBuf).hDCBmp = CreateCompatibleDC((*p_DblBuf).hDC);
    (*p_DblBuf).hCanvas = CreateCompatibleBitmap(
        (*p_DblBuf).hDC,
        (*p_DblBuf).ClientArea.right,
        (*p_DblBuf).ClientArea.bottom
    );
    (*p_DblBuf).hStorage = SelectObject((*p_DblBuf).hDCMem, (*p_DblBuf).hCanvas);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return p_DblBuf;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_Resize(DBLBUF* p_DblBuf, HWND hWnd, COLORREF BrushColour, GLOBALS* p_Globals) {
    SelectObject((*p_DblBuf).hDCMem, (*p_DblBuf).hStorage);
    DeleteObject((*p_DblBuf).hCanvas);
    GetClientRect(hWnd, &(*p_DblBuf).ClientArea);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    (*p_DblBuf).hCanvas = CreateCompatibleBitmap(
        (*p_DblBuf).hDC,
        (*p_DblBuf).ClientArea.right,
        (*p_DblBuf).ClientArea.bottom
    );
    (*p_DblBuf).hStorage = SelectObject((*p_DblBuf).hDCMem, (*p_DblBuf).hCanvas);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_Kill(DBLBUF* p_DblBuf, GLOBALS* p_Globals) {
    SelectObject((*p_DblBuf).hDCMem, (*p_DblBuf).hStorage);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DeleteObject((*p_DblBuf).hCanvas);
    DeleteDC((*p_DblBuf).hDCBmp);
    DeleteDC((*p_DblBuf).hDCMem);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ReleaseDC((*p_DblBuf).hWnd, (*p_DblBuf).hDC);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    free(p_DblBuf);
    (*p_Globals).iRunningHeap -= sizeof(DBLBUF);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IPOINT __cdecl DBLBUF_FindBlitterPoint(char cChar) {
    int iI;
    IPOINT CurrentPoint = {0, 0};
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // abcdefghijklmnopqrstuvwxyz
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for(iI = 97; iI <= 122; iI++) {
        if(cChar == (char)iI) {
            return CurrentPoint;
        }
        CurrentPoint.iX += 1;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // ABCDEFGHIJKLMNOPQRSTUVWXYZ
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CurrentPoint.iX = 0;
    CurrentPoint.iY = 1;
    for(iI = 65; iI <= 90; iI++) {
        if(cChar == (char)iI) {
            return CurrentPoint;
        }
        CurrentPoint.iX += 1;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 0123456789
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CurrentPoint.iX = 0;
    CurrentPoint.iY = 2;
    for(iI = 48; iI <= 57; iI++) {
        if(cChar == (char)iI) {
            return CurrentPoint;
        }
        CurrentPoint.iX += 1;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  !"#$%&'()*+,-./ (The first character is a space.)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CurrentPoint.iX = 0;
    CurrentPoint.iY = 3;
    for(iI = 32; iI <= 47; iI++) {
        if(cChar == (char)iI) {
            return CurrentPoint;
        }
        CurrentPoint.iX += 1;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // :;<=>?@
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CurrentPoint.iX = 0;
    CurrentPoint.iY = 4;
    for(iI = 58; iI <= 64; iI++) {
        if(cChar == (char)iI) {
            return CurrentPoint;
        }
        CurrentPoint.iX += 1;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // [\]^_`
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CurrentPoint.iX = 0;
    CurrentPoint.iY = 5;
    for(iI = 91; iI <= 96; iI++) {
        if(cChar == (char)iI) {
            return CurrentPoint;
        }
        CurrentPoint.iX += 1;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // {|}~
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CurrentPoint.iX = 0;
    CurrentPoint.iY = 6;
    for(iI = 123; iI <= 126; iI++) {
        if(cChar == (char)iI) {
            return CurrentPoint;
        }
        CurrentPoint.iX += 1;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Character not found.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CurrentPoint.iX = 0;
    CurrentPoint.iY = 0;
    return CurrentPoint;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_SetBlitter(DBLBUF* p_DblBuf, PICTURE* p_Picture) {
    (*p_DblBuf).p_Blitter = p_Picture;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_Blitter(DBLBUF* p_DblBuf, char* p_szText, FPOINT Location, int bMask) {
    int iCharWid = 11;
    int iCharHgt = 22;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Compensate for rendering translations.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Location.fX -= (*p_DblBuf).XForm.eDx;
    Location.fY -= (*p_DblBuf).XForm.eDy;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT SavedLocation = Location;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(bMask) {
        HGDIOBJ hMaskTemp = SelectObject((*p_DblBuf).hDCBmp, (*(*p_DblBuf).p_Blitter).hBmpMask);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int iI;
        for(iI = 0; iI < strlen(p_szText); iI++) {
            IPOINT BlitterPoint = DBLBUF_FindBlitterPoint(p_szText[iI]);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            BitBlt(
                (*p_DblBuf).hDCMem,
                Location.fX,
                Location.fY,
                10,
                16,
                (*p_DblBuf).hDCBmp,
                BlitterPoint.iX * iCharWid,
                BlitterPoint.iY * iCharHgt,
                SRCAND
            );
            Location.fX += iCharWid;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SelectObject((*p_DblBuf).hDCBmp, hMaskTemp);
        DeleteObject(hMaskTemp);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Location = SavedLocation;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HGDIOBJ hPicTemp = SelectObject((*p_DblBuf).hDCBmp, (*(*p_DblBuf).p_Blitter).hBmp);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int iI;
    for(iI = 0; iI < strlen(p_szText); iI++) {
        IPOINT BlitterPoint = DBLBUF_FindBlitterPoint(p_szText[iI]);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        BitBlt(
            (*p_DblBuf).hDCMem,
            Location.fX,
            Location.fY,
            10,
            16,
            (*p_DblBuf).hDCBmp,
            BlitterPoint.iX * iCharWid,
            BlitterPoint.iY * iCharHgt,
            bMask ? SRCPAINT : SRCCOPY
        );
        Location.fX += iCharWid;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SelectObject((*p_DblBuf).hDCBmp, hPicTemp);
    DeleteObject(hPicTemp);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_Clear(DBLBUF* p_DblBuf, COLORREF BrushColour) {
    HBRUSH hBrush = CreateSolidBrush(BrushColour);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FillRect((*p_DblBuf).hDCMem, &(*p_DblBuf).ClientArea, hBrush);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DeleteObject(hBrush);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_DrawEntityMinorVector(DBLBUF* p_DblBuf, ENTITY* p_Entity, COLORREF PenColour) {
    HPEN hPen = CreatePen(PS_SOLID, 1, PenColour);
    HPEN hPenTemp  = SelectObject((*p_DblBuf).hDCMem, hPen);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MoveToEx((*p_DblBuf).hDCMem, (*p_Entity).CenterPoint.fX, (*p_Entity).CenterPoint.fY, NULL);
    LineTo((*p_DblBuf).hDCMem, (*p_Entity).MinorDestinationCenterPoint.fX, (*p_Entity).MinorDestinationCenterPoint.fY);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SelectObject((*p_DblBuf).hDCMem, hPenTemp);
    DeleteObject(hPen);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_DrawEntityMajorVector(DBLBUF* p_DblBuf, ENTITY* p_Entity, COLORREF PenColour) {
    HPEN hPen = CreatePen(PS_SOLID, 1, PenColour);
    HPEN hPenTemp  = SelectObject((*p_DblBuf).hDCMem, hPen);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MoveToEx((*p_DblBuf).hDCMem, (*p_Entity).CenterPoint.fX, (*p_Entity).CenterPoint.fY, NULL);
    LineTo((*p_DblBuf).hDCMem, (*p_Entity).MajorDestinationCenterPoint.fX, (*p_Entity).MajorDestinationCenterPoint.fY);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SelectObject((*p_DblBuf).hDCMem, hPenTemp);
    DeleteObject(hPen);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_DrawEntityEllipse(DBLBUF* p_DblBuf, ENTITY* p_Entity, COLORREF PenColour, COLORREF BrushColour) {
    HPEN hPen = CreatePen(PS_SOLID, 1, PenColour);
    HBRUSH hBrush = CreateSolidBrush(BrushColour);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HGDIOBJ hPenTemp = SelectObject((*p_DblBuf).hDCMem, hPen);
    HGDIOBJ hBrushTemp  = SelectObject((*p_DblBuf).hDCMem, hBrush);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Ellipse(
        (*p_DblBuf).hDCMem,
        (*p_Entity).CenterPoint.fX - (*p_Entity).fRadius - COLLISION_BUFFER,
        (*p_Entity).CenterPoint.fY - (*p_Entity).fRadius - COLLISION_BUFFER,
        (*p_Entity).CenterPoint.fX + (*p_Entity).fRadius + COLLISION_BUFFER,
        (*p_Entity).CenterPoint.fY + (*p_Entity).fRadius + COLLISION_BUFFER
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SelectObject((*p_DblBuf).hDCMem, hPenTemp);
    SelectObject((*p_DblBuf).hDCMem, hBrushTemp);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DeleteObject(hBrush);
    DeleteObject(hPen);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Remember: The transform is translated to the center point of the entity to be drawn.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_DrawEntity(DBLBUF* p_DblBuf, ENTITY* p_Entity, int bMask) {
    if(bMask) {
        HGDIOBJ hMaskTemp = SelectObject((*p_DblBuf).hDCBmp, (*(*p_Entity).p_Picture).hBmpMask);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        BitBlt(
            (*p_DblBuf).hDCMem,
            (*p_Entity).CenterPoint.fX - (*p_Entity).HalfSize.fX - (*p_DblBuf).XForm.eDx,
            (*p_Entity).CenterPoint.fY - (*p_Entity).HalfSize.fY - (*p_DblBuf).XForm.eDy,
            (*p_Entity).Size.fX,
            (*p_Entity).Size.fY,
            (*p_DblBuf).hDCBmp,
            0,
            0,
            SRCAND
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SelectObject((*p_DblBuf).hDCBmp, hMaskTemp);
        DeleteObject(hMaskTemp);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HGDIOBJ hPicTemp = SelectObject((*p_DblBuf).hDCBmp, (*(*p_Entity).p_Picture).hBmp);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BitBlt(
        (*p_DblBuf).hDCMem,
        (*p_Entity).CenterPoint.fX - (*p_Entity).HalfSize.fX - (*p_DblBuf).XForm.eDx,
        (*p_Entity).CenterPoint.fY - (*p_Entity).HalfSize.fY - (*p_DblBuf).XForm.eDy,
        (*p_Entity).Size.fX,
        (*p_Entity).Size.fY,
        (*p_DblBuf).hDCBmp,
        0,
        0,
        bMask ? SRCPAINT : SRCCOPY
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SelectObject((*p_DblBuf).hDCBmp, hPicTemp);
    DeleteObject(hPicTemp);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_ClearEntity(DBLBUF* p_DblBuf, ENTITY* p_Entity, COLORREF ClearColour) {
    RECT Area = {
        (*p_Entity).Location.fX,
        (*p_Entity).Location.fY,
        (*p_Entity).Location.fX + (*p_Entity).Size.fX,
        (*p_Entity).Location.fY + (*p_Entity).Size.fY
    };
    HBRUSH hBrush = CreateSolidBrush(ClearColour);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FillRect((*p_DblBuf).hDCMem, &Area, hBrush);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DeleteObject(hBrush);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_FlipEntity(DBLBUF* p_DblBuf, ENTITY* p_Entity) {
    BitBlt(
        (*p_DblBuf).hDC,
        (*p_Entity).Location.fX,
        (*p_Entity).Location.fY,
        (*p_Entity).Size.fX,
        (*p_Entity).Size.fY,
        (*p_DblBuf).hDCMem,
        (*p_Entity).Location.fX,
        (*p_Entity).Location.fY,
        SRCCOPY
    );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_Flip(DBLBUF* p_DblBuf) {
    BitBlt(
        (*p_DblBuf).hDC,
        0,
        0,
        (*p_DblBuf).ClientArea.right,
        (*p_DblBuf).ClientArea.bottom,
        (*p_DblBuf).hDCMem,
        0,
        0,
        SRCCOPY
    );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_FlipArea(DBLBUF* p_DblBuf, int iX, int iY, int iWidth, int iHeight) {
    BitBlt((*p_DblBuf).hDC, iX, iY, iWidth, iHeight, (*p_DblBuf).hDCMem, iX, iY, SRCCOPY);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_DrawPicture(DBLBUF* p_DblBuf, PICTURE* p_Picture, int bMask) {
    if(bMask) {
        HGDIOBJ hMaskTemp = SelectObject((*p_DblBuf).hDCBmp, (*p_Picture).hBmpMask);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        BitBlt(
            (*p_DblBuf).hDCMem,
            (*p_Picture).Location.fX,
            (*p_Picture).Location.fY,
            (*p_Picture).Bitmap.bmWidth,
            (*p_Picture).Bitmap.bmHeight,
            (*p_DblBuf).hDCBmp,
            0,
            0,
            SRCAND
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SelectObject((*p_DblBuf).hDCBmp, hMaskTemp);
        DeleteObject(hMaskTemp);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HGDIOBJ hPicTemp = SelectObject((*p_DblBuf).hDCBmp, (*p_Picture).hBmp);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BitBlt(
        (*p_DblBuf).hDCMem,
        (*p_Picture).Location.fX,
        (*p_Picture).Location.fY,
        (*p_Picture).Bitmap.bmWidth,
        (*p_Picture).Bitmap.bmHeight,
        (*p_DblBuf).hDCBmp,
        0,
        0,
        bMask ? SRCPAINT : SRCCOPY
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SelectObject((*p_DblBuf).hDCBmp, hPicTemp);
    DeleteObject(hPicTemp);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_DrawPictureAt(DBLBUF* p_DblBuf, PICTURE* p_Picture, FPOINT Location, int bMask) {
    if(bMask) {
        HGDIOBJ hMaskTemp = SelectObject((*p_DblBuf).hDCBmp, (*p_Picture).hBmpMask);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        BitBlt(
            (*p_DblBuf).hDCMem,
            Location.fX,
            Location.fY,
            (*p_Picture).Bitmap.bmWidth,
            (*p_Picture).Bitmap.bmHeight,
            (*p_DblBuf).hDCBmp,
            0,
            0,
            SRCAND
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SelectObject((*p_DblBuf).hDCBmp, hMaskTemp);
        DeleteObject(hMaskTemp);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HGDIOBJ hPicTemp = SelectObject((*p_DblBuf).hDCBmp, (*p_Picture).hBmp);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BitBlt(
        (*p_DblBuf).hDCMem,
        Location.fX,
        Location.fY,
        (*p_Picture).Bitmap.bmWidth,
        (*p_Picture).Bitmap.bmHeight,
        (*p_DblBuf).hDCBmp,
        0,
        0,
        bMask ? SRCPAINT : SRCCOPY
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SelectObject((*p_DblBuf).hDCBmp, hPicTemp);
    DeleteObject(hPicTemp);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
