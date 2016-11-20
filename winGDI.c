//-----------------------------LICENSE NOTICE------------------------------------
//  This file is screenPart of CPCtelera: An Amstrad CPC Game Engine
//  Copyright (C) 2016 Arnaud Bouche
//  Copyright (C) 2016 ronaldo / Fremos / Cheesetea / ByteRealms (@FranGallegoBR)
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//-------------------------------------------------------------------------------

#include "winCpctelera.h"

#ifdef _USEWINGDI

#pragma comment(lib,"winmm.lib")

#define JOY_MID_VALUE (int)(USHRT_MAX / 2)
#define JOY_LIMIT_VALUE (int)(JOY_MID_VALUE - 10000)

extern COLORREF GetColorHW(int pHW);
extern UCHAR* GetRenderingBuffer();
extern u8 GetCpcKeyPos(u16 pVKeyID);

static BOOL _joystickOK;
static JOYINFOEX _joyState;

static HBITMAP _oldBitmap;
static HBITMAP _doubleBuffer;
static HDC _memDC;
static HDC _hdc;
static HPALETTE _hPal;
static HPALETTE _oldhPal;
static HWND _hWnd;
static DWORD _lastTime;

static HANDLE sMutex;
static HANDLE sStartInterruptEvent;
static HANDLE sVSyncEvent;
static BOOL sRunInterrupt;

static const u8 sInterruptArea[] = { 25, 53, 52, 52, 52, 38 };

static const UCHAR sIconDataFile[] = { 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x20, 0x20, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0xE8, 0x02, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x0A, 0xAA, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xBB, 0xBF, 0xBB, 0x00, 0xAA, 0xAA, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xBB, 0xBF, 0xBB, 0x0A, 0xAA, 0xAA, 0xAA, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xBB, 0xFB, 0xBF, 0xB0, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xBB, 0xBF, 0xBF, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xBB, 0xBB, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xBB, 0xBF, 0xBF, 0xFB, 0xBB, 0xF0, 0x08, 0xF8, 0x0F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xBB, 0xFB, 0xBF, 0xBF, 0xBB, 0xF0, 0xF0, 0x80, 0xF0, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xBB, 0xBF, 0xBB, 0xFF, 0x0F, 0xFF, 0x0F, 0xFF, 0x0F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xBB, 0xBF, 0xBB, 0xF0, 0x0F, 0xFF, 0xF0, 0xF0, 0x80, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x08, 0xF8, 0x0F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF0, 0x80, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x0F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xF0, 0x03, 0xFF, 0xFF, 0xFC, 0x0F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFC, 0x0E, 0x1F, 0xFF, 0xF8, 0x00, 0x0F, 0xFF, 0xF0, 0x08, 0x07, 0xFF, 0xE0, 0x00, 0x03, 0xFF, 0xC0, 0x00, 0x01, 0xFF, 0xC0, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0x7F, 0xC0, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x1F, 0xC0, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x03, 0xF8, 0x06, 0x00, 0x07, 0xFC, 0x0F, 0x00, 0x0F, 0xFF, 0xFF, 0x80, 0x1F, 0xFF, 0xFF, 0xC0, 0x3F, 0xFF, 0xFF, 0xE0, 0x7F, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void Close()
{
	SelectObject(_memDC, _oldBitmap);
	DeleteDC(_memDC);
	ReleaseDC(_hWnd, _hdc);
	DeleteObject(_doubleBuffer);
	DeleteObject(_hPal);

	CloseHandle(sMutex);
	CloseHandle(sStartInterruptEvent);
}

int GetXAxis()
{
	if (_joystickOK)
	{
		int curX = (int)(_joyState.dwXpos) - JOY_MID_VALUE;
		if (curX > JOY_LIMIT_VALUE)
			return 1;

		if (curX < -JOY_LIMIT_VALUE)
			return -1;

		int pov = (int)(_joyState.dwPOV);
		if (pov != JOY_POVCENTERED)
		{
			if (pov > JOY_POVFORWARD && pov < JOY_POVBACKWARD)
				return 1;

			if (pov > JOY_POVBACKWARD)
				return -1;
		}
	}
	return 0;
}

int GetYAxis()
{
	if (_joystickOK)
	{
		int curY = (int)(_joyState.dwYpos) - JOY_MID_VALUE;

		if (curY > JOY_LIMIT_VALUE)
			return -1;

		if (curY < -JOY_LIMIT_VALUE)
			return 1;

		int pov = (int)(_joyState.dwPOV);
		if (pov != JOY_POVCENTERED)
		{
			if (pov > JOY_POVLEFT || pov < JOY_POVRIGHT)
				return 1;

			if (pov > JOY_POVRIGHT && pov < JOY_POVLEFT)
				return -1;
		}
	}
	return 0;
}

int GetJoystickButton()
{
	if (_joystickOK)
	{
		int curBtn = _joyState.dwButtons;

		if (curBtn & 0x01)
			return 1;

		if (curBtn & 0x02)
			return 2;
	}
	return 0;
}

void GetAsyncJoystickState()
{
	if (_joystickOK)
	{
		joyGetPosEx(JOYSTICKID1, &_joyState);

		if (GetJoystickButton() != 0 || GetXAxis() != 0 || GetYAxis() != 0)
			_curKey = TRUE;
	}
}

void GetRectScreen(int screenPart, RECT* pRect)
{
	int pos = 0;
	for (int i = 0; i < INTERRUPT_PER_VBL; i++)
	{
		RECT rect = { 0, pos, FULL_SCREEN_CX, pos + sInterruptArea[i] * COEF };

		pos += sInterruptArea[i] * COEF;

		if (i == screenPart)
			*pRect = rect;
	}
}

LPBITMAPINFO CreateBitmapInfo(int pBitCount, int cx, int cy)
{
	int nbColor = 1;
	for (int i = 0; i < pBitCount; i++)
		nbColor *= 2;

	int sizeBitmapInfo = sizeof(BITMAPINFOHEADER) + nbColor * sizeof(WORD);
	LPBITMAPINFO bitmapInfos = (LPBITMAPINFO)malloc(sizeBitmapInfo);
	memset(bitmapInfos, 0, sizeof(BITMAPINFOHEADER));
	bitmapInfos->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfos->bmiHeader.biWidth = cx;
	bitmapInfos->bmiHeader.biHeight = -cy;
	bitmapInfos->bmiHeader.biPlanes = 1;
	bitmapInfos->bmiHeader.biBitCount = pBitCount;
	bitmapInfos->bmiHeader.biCompression = BI_RGB;
	bitmapInfos->bmiHeader.biClrUsed = nbColor;

	WORD* pal = (WORD*)bitmapInfos->bmiColors;
	for (int i = 0; i < nbColor; i++)
		pal[i] = i;

	return bitmapInfos;
}

void CreatePaletteCpc()
{
	if (_hPal != NULL)
	{
		SelectPalette(_memDC, _oldhPal, FALSE);
		DeleteObject(_hPal);
	}

	NPLOGPALETTE logPalette = (NPLOGPALETTE)malloc(sizeof(LOGPALETTE) + NB_COLORS * sizeof(PALETTEENTRY));

	logPalette->palNumEntries = NB_COLORS;
	logPalette->palVersion = 0x300;

	for (int i = 0; i < NB_COLORS; i++)
	{
		int hw = _amstrad._curVideoConf._palette[i];
		COLORREF rgb = GetColorHW(hw);

		logPalette->palPalEntry[i].peBlue = GetBValue(rgb);
		logPalette->palPalEntry[i].peRed = GetRValue(rgb);
		logPalette->palPalEntry[i].peGreen = GetGValue(rgb);
		logPalette->palPalEntry[i].peFlags = 0;
	}

	_hPal = CreatePalette(logPalette);

	free(logPalette);
}

void FillBorder(RECT* pRect)
{
	int hw = _amstrad._curVideoConf._palette[BORDER_COLOR];
	HBRUSH brush = CreateSolidBrush(GetColorHW(hw));
	FillRect(_memDC, pRect, brush);
	DeleteObject(brush);
}

void RenderScreen(int screenPart)
{
	RECT rectPart;
	GetRectScreen(screenPart, &rectPart);

	RECT drawScreen = { 0, BORDER_UP_CY, WIDTH_SCREEN,  BORDER_UP_CY + HEIGHT_SCREEN };

	IntersectRect(&drawScreen, &drawScreen, &rectPart);

	int y = drawScreen.top / COEF;
	int cy = (drawScreen.bottom - drawScreen.top) / COEF;

	CreatePaletteCpc();

	WaitForSingleObject(sMutex, INFINITE);

	FillBorder(&rectPart);

	if (cy > 0)
	{
		LPBITMAPINFO bitmapInfos = CreateBitmapInfo(8, WIDTH_SCREEN, cy);

		int yBitmap = y - BORDER_UP_CY / COEF;

		_oldhPal = SelectPalette(_memDC, _hPal, FALSE);
		RealizePalette(_memDC);

		StretchDIBits(_memDC,	BORDER_CX, y * COEF, WIDTH_SCREEN, cy * COEF,
								0, 0, WIDTH_SCREEN, cy, GetRenderingBuffer() + yBitmap * WIDTH_SCREEN, bitmapInfos, DIB_PAL_COLORS, SRCCOPY);
		free(bitmapInfos);
	}					

	ReleaseMutex(sMutex);
}

void Redraw()
{
	WaitForSingleObject(sMutex, INFINITE);

	BitBlt(_hdc, 0, 0, FULL_SCREEN_CX, FULL_SCREEN_CY, _memDC, 0, 0, SRCCOPY);

	ReleaseMutex(sMutex);
}

static void KeyEvent(u16 pKey)
{
	_curKey = TRUE;
	int pos = GetCpcKeyPos(pKey);
	cpct_keyboardStatusBuffer[pos / 8] = 0xFF ^ (1 << (pos % 8));
}

u8 GetAsyncJoyState(u16 vKey)
{
	if (vKey == VK_UP && GetYAxis() == 1)
		return 1;

	if (vKey == VK_DOWN && GetYAxis() == -1)
		return 1;

	if (vKey == VK_RIGHT && GetXAxis() == 1)
		return 1;

	if (vKey == VK_LEFT && GetXAxis() == -1)
		return 1;

	if (vKey == VK_SPACE && GetJoystickButton() == 1)
		return 1;

	if (vKey == VK_CONTROL && GetJoystickButton() == 2)
		return 1;

	return 0;
}

LRESULT FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_KEYDOWN:
			KeyEvent(wParam);
		break;

		case WM_DESTROY:
			Close();
			exit(0);
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

HICON createIcon(PBYTE iconData, int iconSize)
{
	HICON hIcon = NULL;
	int offset = LookupIconIdFromDirectoryEx(iconData, TRUE, iconSize, iconSize, LR_DEFAULTCOLOR);

	if (offset != 0)
		hIcon = CreateIconFromResourceEx(iconData + offset, 0, TRUE, 0x30000, iconSize, iconSize, LR_DEFAULTCOLOR);

	return hIcon;
}

RECT CalculateWindowRect(HWND hWindow, SIZE szDesiredClient)
{
	RECT rcDesiredWindowRect;
	RECT rcCurrentWindowRect;
	RECT rcCurrentClientRect;

	GetWindowRect(hWindow, &rcCurrentWindowRect);
	GetClientRect(hWindow, &rcCurrentClientRect);

	/** Get the difference between the current and desired client areas */
	SIZE szClientDifference = { rcCurrentClientRect.right - szDesiredClient.cx, rcCurrentClientRect.bottom - szDesiredClient.cy };

	/** Get the difference between the current window rect and the desired window rect */
	SetRect(&rcDesiredWindowRect, rcCurrentWindowRect.left, rcCurrentWindowRect.top, rcCurrentWindowRect.right - szClientDifference.cx, rcCurrentWindowRect.bottom - szClientDifference.cy);
	return rcDesiredWindowRect;
}

void PosWindow()
{
	SetWindowLong(_hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE);
	MoveWindow(_hWnd, 0, 0, 0, 0, FALSE);

	int posX = (GetSystemMetrics(SM_CXFULLSCREEN) - FULL_SCREEN_CX) / 2;
	int posY = (GetSystemMetrics(SM_CYFULLSCREEN) - FULL_SCREEN_CY) / 2;

	SIZE szDesiredClient = { FULL_SCREEN_CX, FULL_SCREEN_CY };

	RECT rcNewWindowRect = CalculateWindowRect(_hWnd, szDesiredClient);

	SIZE size = { rcNewWindowRect.right - rcNewWindowRect.left,
		rcNewWindowRect.bottom - rcNewWindowRect.top };

	MoveWindow(_hWnd, posX, posY, size.cx, size.cy, TRUE);
}

void InitJoystick()
{
	_joystickOK = TRUE;
	if (joyGetNumDevs() == 0)
		_joystickOK = FALSE;
	else
	{
		JOYCAPS jc;
		if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) != JOYERR_NOERROR)
			_joystickOK = FALSE;
	}

	_joyState.dwSize = sizeof(_joyState);
	_joyState.dwFlags = JOY_RETURNALL | JOY_RETURNPOVCTS | JOY_RETURNCENTERED | JOY_USEDEADZONE;
}

void CreateWindowApp()
{
	#define TITLE	"WinCPCTelera"	

	HINSTANCE instance = GetModuleHandle(NULL);

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instance;
	wc.hIcon = createIcon(sIconDataFile, 32);
	wc.hCursor = NULL;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = TITLE;
	wc.lpszClassName = TITLE;

	RegisterClass(&wc);

	_hWnd = CreateWindow(TITLE,
		NULL,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
		0, 0,
		0, 0,
		NULL,
		NULL,
		instance,
		NULL);

	SetWindowText(_hWnd, TITLE);
	PosWindow();

	_hdc = GetDC(_hWnd);
	_memDC = CreateCompatibleDC(_hdc);
	_doubleBuffer = CreateCompatibleBitmap(_hdc, FULL_SCREEN_CX, FULL_SCREEN_CY);

	_oldBitmap = SelectObject(_memDC, _doubleBuffer);

	_hPal = NULL;
	CreatePaletteCpc();
	InitJoystick();

	MsgLoop();
}

void MsgLoop()
{
	MSG msg;
	memset(&msg, 0, sizeof(msg));

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			return;
	}
}

DWORD WINAPI InterruptFunction(LPVOID lpParam)
{
	SAmstrad* amstrad = (SAmstrad*)lpParam;
	DWORD time = timeGetTime();

	while (sRunInterrupt)
	{
		if (timeGetTime() - time > INTERRUPT_MS)
		{
			time = timeGetTime();

			if (amstrad->_internalTimer == INTERRUPT_PER_VBL)
			{
				amstrad->_internalTimer = 0;
				SetEvent(sVSyncEvent);
				Redraw();

				GetAsyncJoystickState();
			}

			if (amstrad->_interruptFunction != NULL)
				amstrad->_interruptFunction();

			RenderScreen(amstrad->_internalTimer);

			amstrad->_internalTimer++;
		}

		Sleep(1);
	}

	SetEvent(sStartInterruptEvent);

	return 0;
}

void CreateInterruptThread()
{
	_amstrad._internalTimer = 0;
	sRunInterrupt = TRUE;

	CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		InterruptFunction,      // thread function name
		&_amstrad,				// argument to thread function 
		0,                      // use default creation flags 
		NULL);
}

void WaitVSync()
{
	WaitForSingleObject(sVSyncEvent, INFINITE);
}

void SetInterruptFunction(void(*intHandler)(void))
{
	sRunInterrupt = FALSE;
	WaitForSingleObject(sStartInterruptEvent, INFINITE);

	_amstrad._interruptFunction = intHandler;
	CreateInterruptThread();
}

void StartInterrupt()
{
	/* Set timer accuracy to 1 ms */
	TIMECAPS tc;
	timeGetDevCaps(&tc, sizeof(TIMECAPS));
	int timerRes = tc.wPeriodMin > 1 ? tc.wPeriodMin : 1;
	timeBeginPeriod(timerRes);

	sMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex

	sStartInterruptEvent  = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		FALSE,              // initial state is nonsignaled
		TEXT("StartInter")	// object name
	);

	sVSyncEvent = CreateEvent(
		NULL,               // default security attributes
		FALSE,               // auto-reset event
		FALSE,              // initial state is nonsignaled
		TEXT("VSync")		// object name
	);

	CreateInterruptThread();
}

#endif