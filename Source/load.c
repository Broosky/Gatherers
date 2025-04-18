/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _LOAD_C_
#define _LOAD_C_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/constants.h"
#include "../Headers/functions.h"
#include "../Headers/types.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Define externs.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DBLBUF* p_DblBuf = NULL;
SETTINGS* p_Settings = NULL;
IMAGES* p_Images = NULL;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
INT_PTR CALLBACK WINPROC_DlgLoad(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
	static HBRUSH hBrush;
	switch (uiMsg) {
		case WM_INITDIALOG: {
			PostMessage(hWnd, WM_SETICON, ICON_BIG, lParam);
			PostMessage(hWnd, WM_SETICON, ICON_SMALL, lParam);
			SetTimer(hWnd, 50, 250, NULL);
			break;
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// WinAsm studio does not allow us to change the colour properties of the label. Manually do it.
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case WM_CTLCOLORSTATIC: {
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Set the device context to the label.
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			HDC hDC = (HDC)wParam;
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Change the properties of the labels text (only the text).
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			SetTextColor(hDC, RGB(170, 235, 250));
			SetBkColor(hDC, RGB(0, 0, 0));
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Change the properties of the labels background colour (the entire label).
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			hBrush = CreateSolidBrush(RGB(0, 0, 0));
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// The colour the system will draw the label with.
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			return (INT_PTR)hBrush;
		}
		case WM_TIMER: {
			SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading program settings...");
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			SETTINGS_InitFromFile(p_Settings);
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Ready.");
			SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Loading images...");
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			IMAGES_InitFromFile(p_Images, hWnd);
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			SetDlgItemText(hWnd, DLG_LOAD_STATUS, "Ready.");
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		}
		case WM_CLOSE: {
			DeleteObject(hBrush);
			KillTimer(hWnd, 50);
			EndDialog(hWnd, 0);
			break;
		}
		default: {
			return 0;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// I handled a message.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
