/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/common.h"
#include "../Headers/constants.h"
#include "../Headers/double_buffer.h"
#include "../Headers/misc.h"
#include <math.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl TRANSFORM_Init(DOUBLE_BUFFER_T* p_DoubleBuffer) {
    SetGraphicsMode(p_DoubleBuffer->hDCMem, GM_ADVANCED);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl TRANSFORM_ApplyTransform(DOUBLE_BUFFER_T* p_DoubleBuffer, USHORT usType, float fValue, FPOINT_T Pin) {
    switch (usType) {
    case TRANSFORM_ROTATE: {
        float fRadians = MISC_CalculateRadians(fValue);
        float fSin = sin(fRadians);
        float fCos = cos(fRadians);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Possible lookup table?
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_DoubleBuffer->XForm.eM11 = fCos;
        p_DoubleBuffer->XForm.eM12 = fSin;
        p_DoubleBuffer->XForm.eM21 = -fSin;
        p_DoubleBuffer->XForm.eM22 = fCos;
        p_DoubleBuffer->XForm.eDx = Pin.fX;
        p_DoubleBuffer->XForm.eDy = Pin.fY;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        break;
    }
    case TRANSFORM_SCALE: {
        p_DoubleBuffer->XForm.eM11 = fValue;
        p_DoubleBuffer->XForm.eM12 = 0.0f;
        p_DoubleBuffer->XForm.eM21 = 0.0f;
        p_DoubleBuffer->XForm.eM22 = fValue;
        p_DoubleBuffer->XForm.eDx = Pin.fX;
        p_DoubleBuffer->XForm.eDy = Pin.fY;
    }
    }
    SetWorldTransform(p_DoubleBuffer->hDCMem, &p_DoubleBuffer->XForm);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl TRANSFORM_ResetTransform(DOUBLE_BUFFER_T* p_DoubleBuffer) {
    p_DoubleBuffer->XForm.eDx = 0.0f;
    p_DoubleBuffer->XForm.eDy = 0.0f;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ModifyWorldTransform(p_DoubleBuffer->hDCMem, NULL, MWT_IDENTITY);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
