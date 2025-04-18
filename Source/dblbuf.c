/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _DBLBUF_C_
#define _DBLBUF_C_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/constants.h"
#include "../Headers/types.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_Zero(DBLBUF* p_DblBuf) {
    ZeroMemory(p_DblBuf, sizeof(DBLBUF));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DBLBUF* __cdecl DBLBUF_Create(HWND hWnd, GLOBALS* p_Globals) {
    DBLBUF* p_DblBuf = (DBLBUF*)malloc(sizeof(DBLBUF));
    p_Globals->iRunningHeap += sizeof(DBLBUF);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DBLBUF_Zero(p_DblBuf);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GetClientRect(hWnd, &p_DblBuf->ClientArea);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_DblBuf->hWnd = hWnd;
    p_DblBuf->hDC = GetDC(p_DblBuf->hWnd);
    p_DblBuf->hDCMem = CreateCompatibleDC(p_DblBuf->hDC);
    p_DblBuf->hDCBmp = CreateCompatibleDC(p_DblBuf->hDC);
    p_DblBuf->hCanvas = CreateCompatibleBitmap(
        p_DblBuf->hDC,
        p_DblBuf->ClientArea.right,
        p_DblBuf->ClientArea.bottom
    );
    p_DblBuf->hStorage = SelectObject(p_DblBuf->hDCMem, p_DblBuf->hCanvas);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return p_DblBuf;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_Resize(DBLBUF* p_DblBuf, HWND hWnd, COLORREF BrushColour, GLOBALS* p_Globals) {
    SelectObject(p_DblBuf->hDCMem, p_DblBuf->hStorage);
    DeleteObject(p_DblBuf->hCanvas);
    GetClientRect(hWnd, &p_DblBuf->ClientArea);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_DblBuf->hCanvas = CreateCompatibleBitmap(
        p_DblBuf->hDC,
        p_DblBuf->ClientArea.right,
        p_DblBuf->ClientArea.bottom
    );
    p_DblBuf->hStorage = SelectObject(p_DblBuf->hDCMem, p_DblBuf->hCanvas);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_Kill(DBLBUF* p_DblBuf, GLOBALS* p_Globals) {
    SelectObject(p_DblBuf->hDCMem, p_DblBuf->hStorage);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DeleteObject(p_DblBuf->hCanvas);
    DeleteDC(p_DblBuf->hDCBmp);
    DeleteDC(p_DblBuf->hDCMem);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ReleaseDC(p_DblBuf->hWnd, p_DblBuf->hDC);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    free(p_DblBuf);
    p_Globals->iRunningHeap -= sizeof(DBLBUF);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IPOINT __cdecl DBLBUF_FindBlitterPoint(char cChar) {
    UINT8 ubI;
    IPOINT CurrentPoint = { 0, 0 };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // abcdefghijklmnopqrstuvwxyz
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 97; ubI <= 122; ubI++) {
        if (cChar == (char)ubI) {
            return CurrentPoint;
        }
        CurrentPoint.iX += 1;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // ABCDEFGHIJKLMNOPQRSTUVWXYZ
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CurrentPoint.iX = 0;
    CurrentPoint.iY = 1;
    for (ubI = 65; ubI <= 90; ubI++) {
        if (cChar == (char)ubI) {
            return CurrentPoint;
        }
        CurrentPoint.iX += 1;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 0123456789
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CurrentPoint.iX = 0;
    CurrentPoint.iY = 2;
    for (ubI = 48; ubI <= 57; ubI++) {
        if (cChar == (char)ubI) {
            return CurrentPoint;
        }
        CurrentPoint.iX += 1;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  !"#$%&'()*+,-./ (The first character is a space.)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CurrentPoint.iX = 0;
    CurrentPoint.iY = 3;
    for (ubI = 32; ubI <= 47; ubI++) {
        if (cChar == (char)ubI) {
            return CurrentPoint;
        }
        CurrentPoint.iX += 1;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // :;<=>?@
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CurrentPoint.iX = 0;
    CurrentPoint.iY = 4;
    for (ubI = 58; ubI <= 64; ubI++) {
        if (cChar == (char)ubI) {
            return CurrentPoint;
        }
        CurrentPoint.iX += 1;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // [\]^_`
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CurrentPoint.iX = 0;
    CurrentPoint.iY = 5;
    for (ubI = 91; ubI <= 96; ubI++) {
        if (cChar == (char)ubI) {
            return CurrentPoint;
        }
        CurrentPoint.iX += 1;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // {|}~
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CurrentPoint.iX = 0;
    CurrentPoint.iY = 6;
    for (ubI = 123; ubI <= 126; ubI++) {
        if (cChar == (char)ubI) {
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
    p_DblBuf->p_Blitter = p_Picture;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_Blitter(DBLBUF* p_DblBuf, char* p_szText, FPOINT Location, UINT8 ubMask) {
    const UINT8 ubCharWid = 11;
    const UINT8 ubCharHgt = 22;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Compensate for rendering translations.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Location.fX -= p_DblBuf->XForm.eDx;
    Location.fY -= p_DblBuf->XForm.eDy;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT SavedLocation = Location;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (ubMask) {
        HGDIOBJ hMaskTemp = SelectObject(p_DblBuf->hDCBmp, (*p_DblBuf->p_Blitter).hBmpMask);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        USHORT usI;
        for (usI = 0; usI < strlen(p_szText); usI++) {
            IPOINT BlitterPoint = DBLBUF_FindBlitterPoint(p_szText[usI]);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            BitBlt(
                p_DblBuf->hDCMem,
                Location.fX,
                Location.fY,
                10,
                16,
                p_DblBuf->hDCBmp,
                BlitterPoint.iX * ubCharWid,
                BlitterPoint.iY * ubCharHgt,
                SRCAND
            );
            Location.fX += ubCharWid;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SelectObject(p_DblBuf->hDCBmp, hMaskTemp);
        DeleteObject(hMaskTemp);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Location = SavedLocation;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HGDIOBJ hPicTemp = SelectObject(p_DblBuf->hDCBmp, (*p_DblBuf->p_Blitter).hBmp);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    USHORT usI;
    for (usI = 0; usI < strlen(p_szText); usI++) {
        IPOINT BlitterPoint = DBLBUF_FindBlitterPoint(p_szText[usI]);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        BitBlt(
            p_DblBuf->hDCMem,
            Location.fX,
            Location.fY,
            10,
            16,
            p_DblBuf->hDCBmp,
            BlitterPoint.iX * ubCharWid,
            BlitterPoint.iY * ubCharHgt,
            ubMask ? SRCPAINT : SRCCOPY
        );
        Location.fX += ubCharWid;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SelectObject(p_DblBuf->hDCBmp, hPicTemp);
    DeleteObject(hPicTemp);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_Clear(DBLBUF* p_DblBuf, COLORREF BrushColour) {
    HBRUSH hBrush = CreateSolidBrush(BrushColour);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FillRect(p_DblBuf->hDCMem, &p_DblBuf->ClientArea, hBrush);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DeleteObject(hBrush);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_DrawEntityMinorVector(DBLBUF* p_DblBuf, ENTITY* p_Entity, COLORREF PenColour) {
    HPEN hPen = CreatePen(PS_SOLID, 1, PenColour);
    HPEN hPenTemp = SelectObject(p_DblBuf->hDCMem, hPen);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MoveToEx(p_DblBuf->hDCMem, p_Entity->CenterPoint.fX, p_Entity->CenterPoint.fY, NULL);
    LineTo(p_DblBuf->hDCMem, p_Entity->MinorDestinationCenterPoint.fX, p_Entity->MinorDestinationCenterPoint.fY);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SelectObject(p_DblBuf->hDCMem, hPenTemp);
    DeleteObject(hPen);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_DrawEntityMajorVector(DBLBUF* p_DblBuf, ENTITY* p_Entity, COLORREF PenColour) {
    HPEN hPen = CreatePen(PS_SOLID, 1, PenColour);
    HPEN hPenTemp = SelectObject(p_DblBuf->hDCMem, hPen);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MoveToEx(p_DblBuf->hDCMem, p_Entity->CenterPoint.fX, p_Entity->CenterPoint.fY, NULL);
    LineTo(p_DblBuf->hDCMem, p_Entity->MajorDestinationCenterPoint.fX, p_Entity->MajorDestinationCenterPoint.fY);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SelectObject(p_DblBuf->hDCMem, hPenTemp);
    DeleteObject(hPen);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_DrawEntityEllipse(DBLBUF* p_DblBuf, ENTITY* p_Entity, COLORREF PenColour, COLORREF BrushColour) {
    HPEN hPen = CreatePen(PS_SOLID, 1, PenColour);
    HBRUSH hBrush = CreateSolidBrush(BrushColour);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HGDIOBJ hPenTemp = SelectObject(p_DblBuf->hDCMem, hPen);
    HGDIOBJ hBrushTemp = SelectObject(p_DblBuf->hDCMem, hBrush);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Ellipse(
        p_DblBuf->hDCMem,
        p_Entity->CenterPoint.fX - p_Entity->fRadius - COLLISION_BUFFER,
        p_Entity->CenterPoint.fY - p_Entity->fRadius - COLLISION_BUFFER,
        p_Entity->CenterPoint.fX + p_Entity->fRadius + COLLISION_BUFFER,
        p_Entity->CenterPoint.fY + p_Entity->fRadius + COLLISION_BUFFER
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SelectObject(p_DblBuf->hDCMem, hPenTemp);
    SelectObject(p_DblBuf->hDCMem, hBrushTemp);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DeleteObject(hBrush);
    DeleteObject(hPen);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Remember: The transform is translated to the center point of the entity to be drawn.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_DrawEntity(DBLBUF* p_DblBuf, ENTITY* p_Entity, UINT8 ubMask) {
    if (ubMask) {
        HGDIOBJ hMaskTemp = SelectObject(p_DblBuf->hDCBmp, (*p_Entity->p_Picture).hBmpMask);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        BitBlt(
            p_DblBuf->hDCMem,
            p_Entity->CenterPoint.fX - p_Entity->HalfSize.fX - p_DblBuf->XForm.eDx,
            p_Entity->CenterPoint.fY - p_Entity->HalfSize.fY - p_DblBuf->XForm.eDy,
            p_Entity->Size.fX,
            p_Entity->Size.fY,
            p_DblBuf->hDCBmp,
            0,
            0,
            SRCAND
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SelectObject(p_DblBuf->hDCBmp, hMaskTemp);
        DeleteObject(hMaskTemp);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HGDIOBJ hPicTemp = SelectObject(p_DblBuf->hDCBmp, (*p_Entity->p_Picture).hBmp);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BitBlt(
        p_DblBuf->hDCMem,
        p_Entity->CenterPoint.fX - p_Entity->HalfSize.fX - p_DblBuf->XForm.eDx,
        p_Entity->CenterPoint.fY - p_Entity->HalfSize.fY - p_DblBuf->XForm.eDy,
        p_Entity->Size.fX,
        p_Entity->Size.fY,
        p_DblBuf->hDCBmp,
        0,
        0,
        ubMask ? SRCPAINT : SRCCOPY
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SelectObject(p_DblBuf->hDCBmp, hPicTemp);
    DeleteObject(hPicTemp);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_ClearEntity(DBLBUF* p_DblBuf, ENTITY* p_Entity, COLORREF ClearColour) {
    RECT Area = {
        p_Entity->Location.fX,
        p_Entity->Location.fY,
        p_Entity->Location.fX + p_Entity->Size.fX,
        p_Entity->Location.fY + p_Entity->Size.fY
    };
    HBRUSH hBrush = CreateSolidBrush(ClearColour);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FillRect(p_DblBuf->hDCMem, &Area, hBrush);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DeleteObject(hBrush);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_FlipEntity(DBLBUF* p_DblBuf, ENTITY* p_Entity) {
    BitBlt(
        p_DblBuf->hDC,
        p_Entity->Location.fX,
        p_Entity->Location.fY,
        p_Entity->Size.fX,
        p_Entity->Size.fY,
        p_DblBuf->hDCMem,
        p_Entity->Location.fX,
        p_Entity->Location.fY,
        SRCCOPY
    );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_Flip(DBLBUF* p_DblBuf) {
    BitBlt(
        p_DblBuf->hDC,
        0,
        0,
        p_DblBuf->ClientArea.right,
        p_DblBuf->ClientArea.bottom,
        p_DblBuf->hDCMem,
        0,
        0,
        SRCCOPY
    );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_FlipArea(DBLBUF* p_DblBuf, int iX, int iY, int iWidth, int iHeight) {
    BitBlt(p_DblBuf->hDC, iX, iY, iWidth, iHeight, p_DblBuf->hDCMem, iX, iY, SRCCOPY);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_DrawPicture(DBLBUF* p_DblBuf, PICTURE* p_Picture, UINT8 ubMask) {
    if (ubMask) {
        HGDIOBJ hMaskTemp = SelectObject(p_DblBuf->hDCBmp, p_Picture->hBmpMask);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        BitBlt(
            p_DblBuf->hDCMem,
            p_Picture->Location.fX,
            p_Picture->Location.fY,
            p_Picture->Bitmap.bmWidth,
            p_Picture->Bitmap.bmHeight,
            p_DblBuf->hDCBmp,
            0,
            0,
            SRCAND
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SelectObject(p_DblBuf->hDCBmp, hMaskTemp);
        DeleteObject(hMaskTemp);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HGDIOBJ hPicTemp = SelectObject(p_DblBuf->hDCBmp, p_Picture->hBmp);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BitBlt(
        p_DblBuf->hDCMem,
        p_Picture->Location.fX,
        p_Picture->Location.fY,
        p_Picture->Bitmap.bmWidth,
        p_Picture->Bitmap.bmHeight,
        p_DblBuf->hDCBmp,
        0,
        0,
        ubMask ? SRCPAINT : SRCCOPY
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SelectObject(p_DblBuf->hDCBmp, hPicTemp);
    DeleteObject(hPicTemp);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_DrawPictureAt(DBLBUF* p_DblBuf, PICTURE* p_Picture, FPOINT Location, UINT8 ubMask) {
    if (ubMask) {
        HGDIOBJ hMaskTemp = SelectObject(p_DblBuf->hDCBmp, p_Picture->hBmpMask);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        BitBlt(
            p_DblBuf->hDCMem,
            Location.fX,
            Location.fY,
            p_Picture->Bitmap.bmWidth,
            p_Picture->Bitmap.bmHeight,
            p_DblBuf->hDCBmp,
            0,
            0,
            SRCAND
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SelectObject(p_DblBuf->hDCBmp, hMaskTemp);
        DeleteObject(hMaskTemp);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HGDIOBJ hPicTemp = SelectObject(p_DblBuf->hDCBmp, p_Picture->hBmp);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BitBlt(
        p_DblBuf->hDCMem,
        Location.fX,
        Location.fY,
        p_Picture->Bitmap.bmWidth,
        p_Picture->Bitmap.bmHeight,
        p_DblBuf->hDCBmp,
        0,
        0,
        ubMask ? SRCPAINT : SRCCOPY
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SelectObject(p_DblBuf->hDCBmp, hPicTemp);
    DeleteObject(hPicTemp);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl DBLBUF_CropDrawPictureAt(DBLBUF* p_DblBuf, PICTURE* p_Picture, FPOINT CropStart, FDELTA CropDelta, UINT8 ubMask) {
    if (ubMask) {
        HGDIOBJ hMaskTemp = SelectObject(p_DblBuf->hDCBmp, p_Picture->hBmpMask);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        BitBlt(
            p_DblBuf->hDCMem,
            CropStart.fX,
            CropStart.fY,
            CropDelta.fDX,
            CropDelta.fDY,
            p_DblBuf->hDCBmp,
            0,
            0,
            SRCAND
        );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SelectObject(p_DblBuf->hDCBmp, hMaskTemp);
        DeleteObject(hMaskTemp);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HGDIOBJ hPicTemp = SelectObject(p_DblBuf->hDCBmp, p_Picture->hBmp);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BitBlt(
        p_DblBuf->hDCMem,
        CropStart.fX,
        CropStart.fY,
        CropDelta.fDX,
        CropDelta.fDY,
        p_DblBuf->hDCBmp,
        0,
        0,
        ubMask ? SRCPAINT : SRCCOPY
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SelectObject(p_DblBuf->hDCBmp, hPicTemp);
    DeleteObject(hPicTemp);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
