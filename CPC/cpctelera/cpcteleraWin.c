#include "cpcteleraWin.h"

HWND _hWnd;
BOOL _curKey;
u8 _curVideo;
SAmstrad _amstrad;

static HBITMAP _hBitmap;
static HPALETTE _hPal;
static BOOL _isStarted = FALSE;

#define SCREEN_CX_BYTES	80
#define ALIGNED_DWORD(CX) (((CX) * 8 + 31)  & (~31)) / 8

LRESULT FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

const SCPCPalette _palette[NB_PAL_COLOR] =
{
	HW_BLACK, RGB(0,0,0),
	HW_BLUE, RGB(0,0,128),
	HW_BRIGHT_BLUE, RGB(0,0,255),
	HW_RED, RGB(128,0,0),
	HW_MAGENTA, RGB(128,0,128),
	HW_MAUVE, RGB(128,0,255),
	HW_BRIGHT_RED, RGB(255,0,0),
	HW_PURPLE, RGB(255,0,128),
	HW_BRIGHT_MAGENTA, RGB(255,0,255),
	HW_GREEN, RGB(0,128,0),
	HW_CYAN, RGB(128,0,128),
	HW_SKY_BLUE, RGB(0,128,255),
	HW_YELLOW, RGB(128,128,0),
	HW_WHITE, RGB(128,128,128),
	HW_PASTEL_BLUE, RGB(128,128,255),
	HW_ORANGE, RGB(255,128,0),
	HW_PINK, RGB(255,128,128),
	HW_PASTEL_MAGENTA, RGB(255,128,255),
	HW_BRIGHT_GREEN, RGB(0,255,0),
	HW_SEA_GREEN, RGB(0,255,128),
	HW_BRIGHT_CYAN, RGB(0,255,255),
	HW_LIME, RGB(128,255,0),
	HW_PASTEL_GREEN, RGB(128,255,128),
	HW_PASTEL_CYAN, RGB(128,255,255),
	HW_BRIGHT_YELLOW, RGB(255,255,0),
	HW_PASTEL_YELLOW, RGB(255,255,128),
	HW_BRIGHT_WHITE, RGB(255,255,255)
};

#define FONT_SIZE		8
#define FONT_NB_LINE	32
#define FONT_NB_COL		3
#define FONT_ROW		(FONT_NB_LINE*FONT_SIZE)
#define FONT_COL		(FONT_NB_COL*FONT_SIZE)
const UCHAR fonts[FONT_ROW*FONT_COL] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x80, 0x3B, 0x6E, 0x1E,0x3B, 0x1E, 0x3C, 0x03, 0x73, 0x1E, 0x33, 0x73, 0x1E, 0x63, 0x33, 0x1E, 0x30, 0x06, 0x78, 0x3E, 0x0E, 0x1F, 0x0C, 0x36, 0x63, 0x03, 0x3F, 0x07, 0x0C, 0x38, 0x00, 0x66, 0x00, 0x66, 0x33, 0x33, 0x66, 0x30, 0x18, 0x1F, 0x33, 0x0C, 0x33, 0x36, 0x0C, 0x6B, 0x33, 0x33, 0x3E, 0x3E, 0x30, 0x03, 0x1B, 0x33, 0x1E, 0x7F, 0x36, 0x1F, 0x19, 0x0C, 0x0C, 0x0C, 0x00, 0x19, 0x80, 0x3E, 0x33, 0x30, 0x66, 0x3F, 0x18, 0x33, 0x33, 0x0C, 0x03, 0x3C, 0x0C, 0x6B, 0x33, 0x33, 0x33, 0x66, 0x30, 0x1E, 0x18, 0x33, 0x33, 0x6B, 0x1C, 0x33, 0x0C, 0x0C, 0x0C, 0x0C, 0x00, 0x66, 0x00, 0x06, 0x33, 0x33, 0x66, 0x33, 0x3C, 0x33, 0x3B, 0x0C, 0x03, 0x36, 0x0C, 0x7F, 0x33, 0x33, 0x33, 0x66, 0x3B, 0x30, 0x18, 0x33, 0x33, 0x6B, 0x36, 0x33, 0x26, 0x38, 0x0C, 0x07, 0x00, 0x19, 0x80, 0x3C, 0x3E, 0x1E, 0x3E, 0x1E, 0x18, 0x1F, 0x36, 0x1C, 0x07, 0x33, 0x0C, 0x36, 0x6E, 0x1E, 0x6E, 0x3B, 0x6E, 0x1E, 0x3E, 0x33, 0x33, 0x63, 0x63, 0x33, 0x3F, 0x0C, 0x0C, 0x0C, 0x00, 0x66, 0x00, 0x00, 0x30, 0x00, 0x06, 0x00, 0x1B, 0x00, 0x30, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x0C, 0x6E, 0x19, 0x80, 0x00, 0x70, 0x00, 0x0E, 0x00, 0x0E, 0x00, 0x70, 0x0C, 0x03, 0x70, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0C, 0x38, 0x3B, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x33, 0x7E, 0x1E, 0x7C, 0x7F, 0x78, 0x1F, 0x33, 0x3F, 0x3C, 0x73, 0x7F, 0x63, 0x63, 0x1C, 0x78, 0x3B, 0x73, 0x1E, 0x1E, 0x1E, 0x0C, 0x63, 0x63, 0x1E, 0x7F, 0x1E, 0x01, 0x1E, 0x0C, 0x00, 0x00, 0x33, 0x33, 0x33, 0x36, 0x31, 0x30, 0x33, 0x33, 0x0C, 0x66, 0x33, 0x33, 0x63, 0x63, 0x36, 0x30, 0x66, 0x33, 0x33, 0x0C, 0x33, 0x1E, 0x77, 0x63, 0x0C, 0x33, 0x18, 0x03, 0x06, 0x0C, 0x00, 0x00, 0x3F, 0x33, 0x60, 0x33, 0x34, 0x34, 0x67, 0x33, 0x0C, 0x66, 0x36, 0x31, 0x6B, 0x67, 0x63, 0x30, 0x6D, 0x36, 0x03, 0x0C, 0x33, 0x33, 0x7F, 0x36, 0x0C, 0x19, 0x18, 0x06, 0x06, 0x0C, 0x00, 0x00, 0x33, 0x3E, 0x60, 0x33, 0x3C, 0x3C, 0x60, 0x3F, 0x0C, 0x06, 0x3C, 0x30, 0x7F, 0x6F, 0x63, 0x3E, 0x63, 0x3E, 0x1E, 0x0C, 0x33, 0x33, 0x6B, 0x1C, 0x1E, 0x0C, 0x18, 0x0C, 0x06, 0x0C, 0x00, 0x00, 0x33, 0x33, 0x60, 0x33, 0x34, 0x34, 0x60, 0x33, 0x0C, 0x06, 0x36, 0x30, 0x7F, 0x7B, 0x63, 0x33, 0x63, 0x33, 0x30, 0x0C, 0x33, 0x33, 0x63, 0x1C, 0x33, 0x46, 0x18, 0x18, 0x06, 0x3F, 0x00, 0x06, 0x1E, 0x33, 0x33, 0x36, 0x31, 0x31, 0x33, 0x33, 0x0C, 0x06, 0x33, 0x30, 0x77, 0x73, 0x36, 0x33, 0x36, 0x33, 0x33, 0x2D, 0x33, 0x33, 0x63, 0x36, 0x33, 0x63, 0x18, 0x30, 0x06, 0x1E, 0x00, 0x0C, 0x0C, 0x7E, 0x1E, 0x7C, 0x7F, 0x7F, 0x1E, 0x33, 0x3F, 0x0F, 0x73, 0x78, 0x63, 0x63, 0x1C, 0x7E, 0x1C, 0x7E, 0x1E, 0x3F, 0x33, 0x33, 0x63, 0x63, 0x33, 0x7F, 0x1E, 0x60, 0x1E, 0x0C, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x36, 0x0C, 0x63, 0x3B, 0x00, 0x06, 0x18, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x40, 0x3E, 0x3F, 0x3F, 0x1E, 0x0F, 0x1E, 0x1E, 0x0C, 0x1E, 0x1E, 0x0C, 0x0C, 0x06, 0x00, 0x30, 0x0C, 0x3E, 0x00, 0x00, 0x36, 0x3E, 0x33, 0x66, 0x00, 0x0C, 0x0C, 0x33, 0x0C, 0x0C, 0x00, 0x0C, 0x60, 0x63, 0x0C, 0x33, 0x33, 0x06, 0x33, 0x33, 0x0C, 0x33, 0x33, 0x0C, 0x0C, 0x0C, 0x3F, 0x18, 0x00, 0x60, 0x0C, 0x00, 0x7F, 0x0D, 0x18, 0x6E, 0x00, 0x18, 0x06, 0x1E, 0x0C, 0x00, 0x00, 0x00, 0x30, 0x73, 0x0C, 0x30, 0x03, 0x7F, 0x03, 0x33, 0x0C, 0x33, 0x03, 0x00, 0x00, 0x18, 0x00, 0x0C, 0x0C, 0x6F, 0x0C, 0x00, 0x36, 0x1E, 0x0C, 0x3B, 0x00, 0x18, 0x06, 0x7F, 0xBF, 0x00, 0x3F, 0x00, 0x18, 0x6B, 0x0C, 0x1E, 0x0E, 0x66, 0x3E, 0x3E, 0x06, 0x1E, 0x1F, 0x0C, 0x0C, 0x30, 0x00, 0x06, 0x06, 0x6F, 0x0C, 0x36, 0x7F, 0x2C, 0x66, 0x1C, 0x18, 0x18, 0x06, 0x1E, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x67, 0x0C, 0x03, 0x03, 0x36, 0x30, 0x30, 0x03, 0x33, 0x33, 0x0C, 0x0C, 0x18, 0x3F, 0x0C, 0x33, 0x6F, 0x0C, 0x36, 0x36, 0x1F, 0x63, 0x36, 0x0C, 0x0C, 0x0C, 0x33, 0x0C, 0x00, 0x00, 0x00, 0x06, 0x63, 0x1C, 0x33, 0x33, 0x1E, 0x31, 0x33, 0x33, 0x33, 0x33, 0x00, 0x00, 0x0C, 0x00, 0x18, 0x33, 0x63, 0x0C, 0x36, 0x36, 0x0C, 0x00, 0x1C, 0x0C, 0x06, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x3E, 0x0C, 0x1E, 0x1E, 0x0E, 0x3F, 0x1E, 0x3F, 0x1E, 0x1E, 0x00, 0x00, 0x06, 0x00, 0x30, 0x1E, 0x3E
};

typedef struct
{
	char cpcChara;
	u8 posBitmap;
} SCharMapping;

const char charaMapBitmap[] = "!'\\#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\'abcdefghijklmnopqrstuvwxyz{|}~";

typedef struct
{
	u16 cpcKeyID;
	u16 winKeyID;
} SKeyMapping;

SKeyMapping cpctMapKey[] =
{
	{ Key_CursorUp, VK_UP },
	{ Key_CursorRight, VK_RIGHT },
	{ Key_CursorDown, VK_DOWN },
	{ Key_F9, VK_F9 },
	{ Key_F6, VK_F6 },
	{ Key_F3, VK_F3 },
	{ Key_Enter, VK_RETURN },
	{ Key_FDot, VK_OEM_PERIOD },

	{ Key_CursorLeft, VK_LEFT },
	{ Key_Copy, VK_MENU },
	{ Key_F7, VK_F7 },
	{ Key_F8, VK_F8 },
	{ Key_F5, VK_F5 },
	{ Key_F1, VK_F1 },
	{ Key_F2, VK_F2 },
	{ Key_F0, VK_F10 },

	{ Key_Clr,  VK_CONTROL },
	{ Key_OpenBracket, 0 },
	{ Key_Return, VK_RETURN },
	{ Key_CloseBracket, 0 },
	{ Key_F4, VK_F4 },
	{ Key_Shift, VK_SHIFT },
	{ Key_BackSlash, VK_BACK },
	{ Key_Control, VK_CONTROL },

	{ Key_Caret, 0 },
	{ Key_Hyphen, 0 },
	{ Key_At, 0 },
	{ Key_P, 0x50 },
	{ Key_SemiColon, 0 },
	{ Key_Colon, 0 },
	{ Key_Slash, 0 },
	{ Key_Dot, VK_OEM_PERIOD },

	{ Key_0, 0x30 },
	{ Key_9, 0x39 },
	{ Key_O, 0x39 },
	{ Key_I, 0x39 },
	{ Key_L, 0x4C },
	{ Key_K, 0x4B },
	{ Key_M, 0x4B },
	{ Key_Comma, VK_OEM_COMMA },

	{ Key_8, 0x4B },
	{ Key_7, 0x37 },
	{ Key_U, 0x55 },
	{ Key_Y, 0x59 },
	{ Key_H, 0x48 },
	{ Key_J, 0x4A },
	{ Key_N, 0x4E },
	{ Key_Space, VK_SPACE },

	{ Key_6, 0x36 },
	{ Joy1_Up, VK_UP },
	{ Key_5, 0x35 },
	{ Joy1_Down, VK_DOWN },
	{ Key_R, 0x52 },
	{ Joy1_Left, VK_LEFT },
	{ Key_T, 0x54 },
	{ Joy1_Right, VK_RIGHT },
	{ Key_G, 0x47 },
	{ Joy1_Fire1, VK_SPACE },
	{ Key_F, 0x46 },
	{ Joy1_Fire2, VK_CONTROL },
	{ Key_B, 0x42 },
	{ Joy1_Fire3, VK_CONTROL },
	{ Key_V, 0x56 },

	{ Key_4, 0x34 },
	{ Key_3, 0x33 },
	{ Key_E, 0x45 },
	{ Key_W, 0x57 },
	{ Key_S, 0x53 },
	{ Key_D, 0x44 },
	{ Key_C, 0x43 },
	{ Key_X, 0x58 },

	{ Key_1, 0x31 },
	{ Key_2, 0x32 },
	{ Key_Esc, VK_ESCAPE },
	{ Key_Q, 0x51 },
	{ Key_Tab, VK_TAB },
	{ Key_A, 0x41 },
	{ Key_CapsLock, VK_CAPITAL },
	{ Key_Z, 0x5A },

	{ Joy0_Up, VK_UP },
	{ Joy0_Down, VK_DOWN },
	{ Joy0_Left, VK_LEFT },
	{ Joy0_Right, VK_RIGHT },
	{ Joy0_Fire1, VK_SPACE },
	{ Joy0_Fire2, VK_CONTROL },
	{ Joy0_Fire3, VK_CONTROL },
	{ Key_Del, VK_DELETE },
};

void SetPalette(int i, UCHAR pHW)
{
	_amstrad._curPal[i] = pHW;
}

COLORREF GetColorHW(int pHW)
{
	for (int i = 0; i < NB_PAL_COLOR; i++)
	{
		if (_palette[i].hw == pHW)
			return _palette[i].rgb;
	}
	return _palette[0].rgb;
}

void FillBox(HDC hdc, u8 pattern, int x, int y, int cx, int cy)
{
	//u8 pix1 = pattern & 0xF0 >> 4;
	u8 pix0 = pattern & 0x0F;

	int hw = _amstrad._curPal[pix0];
	COLORREF rgb = GetColorHW(hw);
	HBRUSH brush = CreateSolidBrush(rgb);

	if (cx > SCREEN_CX_BYTES)
		cx = SCREEN_CX_BYTES;

	if (cy > HEIGHT_SCREEN)
		cy = HEIGHT_SCREEN;

	if (_amstrad._mode == 0)
	{
		x *= 4;
		cx *= 4;
	}

	x += BORDER_CX;
	y += BORDER_UP_CY;

	RECT rect = { x, y, x + cx, y + cy };

	FillRect(hdc, &rect, brush);
	DeleteObject(brush);
}


HBITMAP GetVideoBitmap(int pScreenAddr)
{
	switch (pScreenAddr)
	{
	case 0x0000:
		return _amstrad._video[3];
	case 0x4000:
		return _amstrad._video[2];
	case 0x8000:
		return _amstrad._video[1];
	case 0xC000:
	default:
		return _amstrad._video[0];
	}
}

HBITMAP GetCurVideoBitmap()
{
	return GetVideoBitmap(_curVideo);
}

u8* GetVideoBuff(int pScreenAddr)
{
	switch (pScreenAddr)
	{
	case cpct_page00:
		return _amstrad._buffVideo[3];
		break;
	case cpct_page40:
		return _amstrad._buffVideo[2];
		break;
	case cpct_page80:
		return _amstrad._buffVideo[1];
		break;
	case cpct_pageC0:
	default:
		return _amstrad._buffVideo[0];
		break;
	}

}

u8* GetCurVideoBuff()
{
	return GetVideoBuff(_curVideo);
}

void SetCurVideo(int pVideoAddr)
{
	switch (pVideoAddr)
	{
	case 0x0000:
		_curVideo = cpct_page00;
		break;
	case 0x4000:
		_curVideo = cpct_page40;
		break;
	case 0x8000:
		_curVideo = cpct_page80;
		break;
	case 0xC000:
	default:
		_curVideo = cpct_pageC0;
		break;
	}
}

u8 GetCoordY(u8* memory)
{
	for (int i = 0; i < 81; i++)
	{
		if (memory[i] == 0xFF)
			return memory[i + 1];
	}

	return 0;
}

int FindCharaIndex(char pChara)
{
	for (int i = 0; i < sizeof(charaMapBitmap); i++)
	{
		if (charaMapBitmap[i] == pChara)
			return i;
	}
	return 0;
}

u16 GetVKey(u16 pCpcKeyID)
{
	for (int i = 0; i < sizeof(cpctMapKey) / sizeof(SKeyMapping); i++)
	{
		if (cpctMapKey[i].cpcKeyID == pCpcKeyID)
		{
			return cpctMapKey[i].winKeyID;
		}
	}
	return 0;
}

VOID CALLBACK InternalTimer(
	HWND hwnd,
	UINT message,
	UINT idTimer,
	DWORD dwTime)
{
	if (_amstrad._interruptFunction != NULL)
		_amstrad._interruptFunction();

	_amstrad._internalTimer++;

	InvalidateRect(_hWnd, NULL, FALSE);
	MsgLoop();
}

COLORREF GetColorFW(int pFW)
{
	return _palette[pFW].rgb;
}

BOOL IsVideoMem(void* pAddress)
{
	int address = (int)pAddress;
	int memVideoBeg = (int)_amstrad._memVideo;
	int memVideoEnd = (int)(_amstrad._memVideo + sizeof(_amstrad._memVideo));

	return (address >= memVideoBeg && address <= memVideoEnd);
}

void Redraw(HWND pWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(pWnd, &ps);
	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP oldBitmap = SelectObject(memDC, GetCurVideoBitmap());

	BitBlt(hdc, 0, 0, FULL_SCREEN_CX, FULL_SCREEN_CY, memDC, 0, 0, SRCCOPY);

	SelectObject(memDC, oldBitmap);
	DeleteDC(memDC);

	EndPaint(pWnd, &ps);
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
	SetWindowLong(_hWnd, GWL_STYLE, WINDOW_STYLE);
	MoveWindow(_hWnd, 0, 0, 0, 0, FALSE);

	int posX = (GetSystemMetrics(SM_CXFULLSCREEN) - FULL_SCREEN_CX) / 2 + 100;
	int posY = (GetSystemMetrics(SM_CYFULLSCREEN) - FULL_SCREEN_CY) / 2;

	SIZE szDesiredClient = { FULL_SCREEN_CX, FULL_SCREEN_CY };

	RECT rcNewWindowRect = CalculateWindowRect(_hWnd, szDesiredClient);

	SIZE size = { rcNewWindowRect.right - rcNewWindowRect.left,
		rcNewWindowRect.bottom - rcNewWindowRect.top };

	MoveWindow(_hWnd, posX, posY, size.cx, size.cy, TRUE);
}

void CreateWindowApp()
{
	HINSTANCE instance = GetModuleHandle(NULL);

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instance;
	wc.hIcon = NULL;
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

	_hPal = NULL;
	CreatePaletteCpc();
}

LPBITMAPINFO CreateBitmapInfo(int pNbColor, int pBitCount, int cx, int cy)
{
	int sizeBitmapInfo = sizeof(BITMAPINFOHEADER) + pNbColor * sizeof(WORD);
	LPBITMAPINFO bitmapInfos = (LPBITMAPINFO)malloc(sizeBitmapInfo);
	memset(bitmapInfos, 0, sizeof(BITMAPINFOHEADER));
	bitmapInfos->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfos->bmiHeader.biWidth = cx;
	bitmapInfos->bmiHeader.biHeight = -cy;
	bitmapInfos->bmiHeader.biPlanes = 1;
	bitmapInfos->bmiHeader.biBitCount = pBitCount;
	bitmapInfos->bmiHeader.biCompression = BI_RGB;
	bitmapInfos->bmiHeader.biClrUsed = pNbColor;

	WORD* pal = (WORD*)bitmapInfos->bmiColors;
	for (int i = 0; i < pNbColor; i++)
		pal[i] = i;

	return bitmapInfos;
}

void CreatePaletteCpc()
{
	NPLOGPALETTE logPalette = (NPLOGPALETTE)malloc(sizeof(LOGPALETTE) + NB_COLORS * sizeof(PALETTEENTRY));

	logPalette->palNumEntries = NB_COLORS;
	logPalette->palVersion = 0x300;

	for (int i = 0; i < NB_COLORS; i++)
	{
		int hw = _amstrad._curPal[i];
		COLORREF rgb = GetColorHW(hw);

		logPalette->palPalEntry[i].peBlue = GetBValue(rgb);
		logPalette->palPalEntry[i].peRed = GetRValue(rgb);
		logPalette->palPalEntry[i].peGreen = GetGValue(rgb);
		logPalette->palPalEntry[i].peFlags = 0;
	}

	if (_hPal != NULL)
		DeleteObject(_hPal);

	_hPal = CreatePalette(logPalette);

	free(logPalette);
}

void FillBorder()
{
	int hw = _amstrad._curPal[BORDER_COLOR];
	COLORREF rgb = GetColorHW(hw);
	HBRUSH brush = CreateSolidBrush(rgb);

	HDC hdc = GetDC(_hWnd);
	HDC memDC = CreateCompatibleDC(hdc);
	ReleaseDC(_hWnd, hdc);

	for (int i = 0; i < 4; i++)
	{
		HBITMAP oldBitmap = SelectObject(memDC, _amstrad._video[i]);

		RECT top = { 0, 0, BORDER_CX + WIDTH_SCREEN + BORDER_CX, BORDER_UP_CY };
		FillRect(memDC, &top, brush);

		RECT left = { 0, BORDER_UP_CY, BORDER_CX, BORDER_UP_CY + HEIGHT_SCREEN + BORDER_DW_CY };
		FillRect(memDC, &left, brush);

		RECT bottom = { 0, BORDER_UP_CY + HEIGHT_SCREEN, BORDER_CX + WIDTH_SCREEN + BORDER_CX, BORDER_UP_CY + HEIGHT_SCREEN + BORDER_DW_CY };
		FillRect(memDC, &bottom, brush);

		RECT right = { BORDER_CX + WIDTH_SCREEN, BORDER_UP_CY, BORDER_CX + WIDTH_SCREEN + BORDER_CX, BORDER_UP_CY + HEIGHT_SCREEN };
		FillRect(memDC, &right, brush);

		SelectObject(memDC, oldBitmap);
	}

	DeleteObject(brush);
	DeleteDC(memDC);
}

void FillScreen(HDC hdc, u8 pVal)
{
	HBRUSH brush = CreateSolidBrush(_palette[2].rgb);

	RECT rect = { BORDER_CX, BORDER_UP_CY, BORDER_CX + WIDTH_SCREEN, BORDER_UP_CY + HEIGHT_SCREEN };
	FillRect(hdc, &rect, brush);

	DeleteObject(brush);
}

UCHAR DecodePixel(UCHAR pPix)
{
	UCHAR pix0 = (pPix & 0x80) >> 7;
	UCHAR pixa = (pPix & 0x40) >> 6;
	UCHAR pix2 = (pPix & 0x20) >> 5;
	UCHAR pixc = (pPix & 0x10) >> 4;
	UCHAR pix1 = (pPix & 0x08) >> 3;
	UCHAR pixb = (pPix & 0x04) >> 2;
	UCHAR pix3 = (pPix & 0x02) >> 1;
	UCHAR pixd = (pPix & 0x01);

	return (pix3 << 7 | pix2 << 6 | pix1 << 5 | pix0 << 4 | pixd << 3 | pixc << 2 | pixb << 1 | pixa);
}

u8 M0byte2px(u8 pPix)
{
	UCHAR pix0 = (pPix & 0x80) >> 7;
	UCHAR pixa = (pPix & 0x40) >> 6;
	UCHAR pix2 = (pPix & 0x20) >> 5;
	UCHAR pixc = (pPix & 0x10) >> 4;
	UCHAR pix1 = (pPix & 0x08) >> 3;
	UCHAR pixb = (pPix & 0x04) >> 2;
	UCHAR pix3 = (pPix & 0x02) >> 1;
	UCHAR pixd = (pPix & 0x01);

	return (pix3 << 7 | pix2 << 6 | pix1 << 5 | pix0 << 4 | pixd << 3 | pix0 << 4 | pixc << 2 | pixb << 1 | pixa << 0);
}

void DisplayFont(HDC hdc, int x, int y, u8 fgPen, u8 bgPen, char pChara)
{
	if (_amstrad._mode == 0)
	{
		x *= 2;
	}

	LPBITMAPINFO bitmapInfos = CreateBitmapInfo(2, 1, FONT_ROW, FONT_COL);
	WORD* pal = (WORD*)bitmapInfos->bmiColors;
	pal[0] = bgPen;
	pal[1] = fgPen;

	SelectPalette(hdc, _hPal, FALSE);
	RealizePalette(hdc);

	int index = FindCharaIndex(pChara);
	int fontx = (index % FONT_NB_LINE - 1) * FONT_SIZE;
	int fonty = (FONT_NB_COL - index / FONT_NB_LINE - 1) * FONT_SIZE;

	StretchDIBits(hdc, x * 2 + BORDER_CX, y + BORDER_UP_CY, FONT_SIZE * 2, FONT_SIZE, fontx, fonty, FONT_SIZE, FONT_SIZE, fonts, bitmapInfos, DIB_PAL_COLORS, SRCCOPY);

	free(bitmapInfos);
}

void DrawFont(int x, int y, u8 fgPen, u8 bgPen, char chara)
{
	HDC hdc = GetDC(_hWnd);
	HDC memDC = CreateCompatibleDC(hdc);

	HBITMAP oldBitmap = SelectObject(memDC, GetCurVideoBitmap());

	DisplayFont(memDC, x, y, fgPen, bgPen, chara);

	SelectObject(memDC, oldBitmap);
	DeleteDC(memDC);
	ReleaseDC(_hWnd, hdc);
}

void DisplayBitmap(HDC hdc, int x, int y, int cx, int cy, char* data, BOOL pMasked)
{
	int xi, yi;

	int widthAlignedDWORD = ALIGNED_DWORD(cx);
	UCHAR* alignedData = NULL;
	UCHAR* sprite = NULL;
	UCHAR* mask = NULL;

	if (pMasked)
	{
		sprite = malloc(widthAlignedDWORD * cy);
		ZeroMemory(sprite, widthAlignedDWORD * cy);

		mask = malloc(widthAlignedDWORD * cy);
		ZeroMemory(mask, widthAlignedDWORD * cy);

		WORD* pix = (WORD*)data;
		int i = 0;
		for (yi = 0; yi < cy; yi++)
		{
			for (xi = 0; xi < cx; xi++)
			{
				mask[i] = DecodePixel((UCHAR)(*pix));
				sprite[i] = DecodePixel((UCHAR)(*pix >> 8));

				pix++;
				i++;
			}
			i += (widthAlignedDWORD - cx);
		}
	}
	else
	{
		alignedData = malloc(widthAlignedDWORD * cy);
		ZeroMemory(alignedData, widthAlignedDWORD * cy);

		for (yi = 0; yi < cy; yi++)
			memcpy(alignedData + yi * widthAlignedDWORD, data + yi * cx, cx);

		for (yi = 0; yi < cy; yi++)
		{
			UCHAR* pix = alignedData + yi * widthAlignedDWORD;
			for (xi = 0; xi < cx; xi++)
			{
				*pix = DecodePixel(*pix);
				pix++;
			}
		}
	}

	int cxDest = cx;
	int coef = 1;
	if (_amstrad._mode == 0)
	{
		//	coef = 2;
		x *= 2;
		cx *= 2;
	}

	LPBITMAPINFO bitmapInfos = CreateBitmapInfo(NB_COLORS, 4, cx, cy);

	SelectPalette(hdc, _hPal, FALSE);
	RealizePalette(hdc);

	if (!pMasked)
	{
		StretchDIBits(hdc, x * 2 + BORDER_CX, y + BORDER_UP_CY, cx * 2, cy, 0, 0, cx, cy, alignedData, bitmapInfos, DIB_PAL_COLORS, SRCCOPY);
		free(alignedData);
	}
	else
	{
		StretchDIBits(hdc, x * 2 + BORDER_CX, y + BORDER_UP_CY, cx * 2, cy, 0, 0, cx, cy, sprite, bitmapInfos, DIB_PAL_COLORS, SRCINVERT);
		StretchDIBits(hdc, x * 2 + BORDER_CX, y + BORDER_UP_CY, cx * 2, cy, 0, 0, cx, cy, mask, bitmapInfos, DIB_PAL_COLORS, SRCAND);
		StretchDIBits(hdc, x * 2 + BORDER_CX, y + BORDER_UP_CY, cx * 2, cy, 0, 0, cx, cy, sprite, bitmapInfos, DIB_PAL_COLORS, SRCINVERT);

		free(mask);
		free(sprite);
	}

	free(bitmapInfos);
}

void DrawSprite(void *sprite, int x, int y, int cx, int cy, BOOL pMasked)
{
	HDC hdc = GetDC(_hWnd);
	HDC memDC = CreateCompatibleDC(hdc);

	HBITMAP oldBitmap = SelectObject(memDC, GetCurVideoBitmap());

	DisplayBitmap(memDC, x, y, cx, cy, sprite, pMasked);

	SelectObject(memDC, oldBitmap);
	DeleteDC(memDC);
	ReleaseDC(_hWnd, hdc);
}

void MsgLoop()
{
	MSG msg;
	memset(&msg, 0, sizeof(msg));

	//for (int i = 0; i < 10; i++)
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

	Sleep(30);
}

void StartCPC()
{
	ZeroMemory(&_amstrad, sizeof(_amstrad));
	for (int i = 0; i < NB_COLORS; i++)
		_amstrad._curPal[i] = _palette[i].hw;

	for (u8 y = 0; y < HEIGHT_SCREEN; y++)
	{
		for (u8 x = 0; x < 82; x++)
		{
			if (x < 80)
				_amstrad._memVideo[y][x] = x;
			else
			{
				_amstrad._memVideo[y][x++] = 0xFF;
				_amstrad._memVideo[y][x++] = y;
			}
		}
	}

	_amstrad._curPal[0] = HW_BRIGHT_BLUE;
	_amstrad._curPal[1] = HW_BRIGHT_YELLOW;
	_amstrad._currentPage = _curVideo = cpct_pageC0;

	HDC hdc = GetDC(_hWnd);

	LPBITMAPINFO bitmapInfos = CreateBitmapInfo(NB_COLORS, 4, FULL_SCREEN_CX, FULL_SCREEN_CY);

	int widthAlignedDWORD = ALIGNED_DWORD(FULL_SCREEN_CX);

	for (int i = 0; i < 4; i++)
	{
		_amstrad._video[i] = CreateDIBitmap(hdc, &bitmapInfos->bmiHeader, 0, NULL, bitmapInfos, DIB_PAL_COLORS);

		_amstrad._buffVideo[i] = malloc(widthAlignedDWORD*FULL_SCREEN_CY);
		GetDIBits(hdc, _amstrad._video[i], FULL_SCREEN_CX, FULL_SCREEN_CY, &_amstrad._buffVideo[i], bitmapInfos, DIB_PAL_COLORS);
	}

	free(bitmapInfos);

	SetTimer(_hWnd, 10000, 33, InternalTimer);

	ReleaseDC(_hWnd, hdc);
}

void CPCTeleraWin()
{
	if (!_isStarted)
	{
		_isStarted = TRUE;

		StartCPC();

		CreateWindowApp();
		MsgLoop();
	}
}

void Close()
{
	for (int i = 0; i < 4; i++)
	{
		free(_amstrad._buffVideo[i]);
		DeleteObject(_amstrad._video[i]);
	}
}

LRESULT FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		_curKey = TRUE;
		break;

	case WM_PAINT:
		Redraw(hWnd);
		break;

	case WM_DESTROY:
		Close();
		exit(0);
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}