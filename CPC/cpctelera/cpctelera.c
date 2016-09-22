#include <windows.h>
#include <cpctelera.h>

#define WINDOW_STYLE	WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE
#define NB_COLORS		16
#define BORDER_COLOR	16
#define TITLE			"WinCPCTelera"			

static HBITMAP _hBitmap;
static HPALETTE _hPal;
static HWND _hWnd;
static BOOL _curKey;
static u8 _curVideo;
static BOOL _isStarted = FALSE;
static u8 _curVideo;
static BOOL _isStarted = FALSE;

typedef void(*TInterrupt)(void);

u8 M0byte2px(u8 pPix);
UCHAR DecodePixel(UCHAR pPix);
void MsgLoop();
void CreatePaletteCpc();
void DisplayBitmap(HDC hdc, int x, int y, int cx, int cy, char* data, BOOL pMasked);
void DrawFont();
void FillBorder();
void SetCurVideo(int pVideoAddr);

typedef struct
{
	UCHAR hw;
	COLORREF rgb;
} SCPCPalette;

const SCPCPalette _palette[27] =
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

<<<<<<< .mine
#define FONT_SIZE		8
#define FONT_NB_LINE	32
#define FONT_NB_COL		3
#define FONT_ROW		(FONT_NB_LINE*FONT_SIZE)
#define FONT_COL		(FONT_NB_COL*FONT_SIZE)
UCHAR fonts[FONT_ROW*FONT_COL] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x80, 0x3B, 0x6E, 0x1E,0x3B, 0x1E, 0x3C, 0x03, 0x73, 0x1E, 0x33, 0x73, 0x1E, 0x63, 0x33, 0x1E, 0x30, 0x06, 0x78, 0x3E, 0x0E, 0x1F, 0x0C, 0x36, 0x63, 0x03, 0x3F, 0x07, 0x0C, 0x38, 0x00, 0x66, 0x00, 0x66, 0x33, 0x33, 0x66, 0x30, 0x18, 0x1F, 0x33, 0x0C, 0x33, 0x36, 0x0C, 0x6B, 0x33, 0x33, 0x3E, 0x3E, 0x30, 0x03, 0x1B, 0x33, 0x1E, 0x7F, 0x36, 0x1F, 0x19, 0x0C, 0x0C, 0x0C, 0x00, 0x19, 0x80, 0x3E, 0x33, 0x30, 0x66, 0x3F, 0x18, 0x33, 0x33, 0x0C, 0x03, 0x3C, 0x0C, 0x6B, 0x33, 0x33, 0x33, 0x66, 0x30, 0x1E, 0x18, 0x33, 0x33, 0x6B, 0x1C, 0x33, 0x0C, 0x0C, 0x0C, 0x0C, 0x00, 0x66, 0x00, 0x06, 0x33, 0x33, 0x66, 0x33, 0x3C, 0x33, 0x3B, 0x0C, 0x03, 0x36, 0x0C, 0x7F, 0x33, 0x33, 0x33, 0x66, 0x3B, 0x30, 0x18, 0x33, 0x33, 0x6B, 0x36, 0x33, 0x26, 0x38, 0x0C, 0x07, 0x00, 0x19, 0x80, 0x3C, 0x3E, 0x1E, 0x3E, 0x1E, 0x18, 0x1F, 0x36, 0x1C, 0x07, 0x33, 0x0C, 0x36, 0x6E, 0x1E, 0x6E, 0x3B, 0x6E, 0x1E, 0x3E, 0x33, 0x33, 0x63, 0x63, 0x33, 0x3F, 0x0C, 0x0C, 0x0C, 0x00, 0x66, 0x00, 0x00, 0x30, 0x00, 0x06, 0x00, 0x1B, 0x00, 0x30, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x0C, 0x6E, 0x19, 0x80, 0x00, 0x70, 0x00, 0x0E, 0x00, 0x0E, 0x00, 0x70, 0x0C, 0x03, 0x70, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0C, 0x38, 0x3B, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x33, 0x7E, 0x1E, 0x7C, 0x7F, 0x78, 0x1F, 0x33, 0x3F, 0x3C, 0x73, 0x7F, 0x63, 0x63, 0x1C, 0x78, 0x3B, 0x73, 0x1E, 0x1E, 0x1E, 0x0C, 0x63, 0x63, 0x1E, 0x7F, 0x1E, 0x01, 0x1E, 0x0C, 0x00, 0x00, 0x33, 0x33, 0x33, 0x36, 0x31, 0x30, 0x33, 0x33, 0x0C, 0x66, 0x33, 0x33, 0x63, 0x63, 0x36, 0x30, 0x66, 0x33, 0x33, 0x0C, 0x33, 0x1E, 0x77, 0x63, 0x0C, 0x33, 0x18, 0x03, 0x06, 0x0C, 0x00, 0x00, 0x3F, 0x33, 0x60, 0x33, 0x34, 0x34, 0x67, 0x33, 0x0C, 0x66, 0x36, 0x31, 0x6B, 0x67, 0x63, 0x30, 0x6D, 0x36, 0x03, 0x0C, 0x33, 0x33, 0x7F, 0x36, 0x0C, 0x19, 0x18, 0x06, 0x06, 0x0C, 0x00, 0x00, 0x33, 0x3E, 0x60, 0x33, 0x3C, 0x3C, 0x60, 0x3F, 0x0C, 0x06, 0x3C, 0x30, 0x7F, 0x6F, 0x63, 0x3E, 0x63, 0x3E, 0x1E, 0x0C, 0x33, 0x33, 0x6B, 0x1C, 0x1E, 0x0C, 0x18, 0x0C, 0x06, 0x0C, 0x00, 0x00, 0x33, 0x33, 0x60, 0x33, 0x34, 0x34, 0x60, 0x33, 0x0C, 0x06, 0x36, 0x30, 0x7F, 0x7B, 0x63, 0x33, 0x63, 0x33, 0x30, 0x0C, 0x33, 0x33, 0x63, 0x1C, 0x33, 0x46, 0x18, 0x18, 0x06, 0x3F, 0x00, 0x06, 0x1E, 0x33, 0x33, 0x36, 0x31, 0x31, 0x33, 0x33, 0x0C, 0x06, 0x33, 0x30, 0x77, 0x73, 0x36, 0x33, 0x36, 0x33, 0x33, 0x2D, 0x33, 0x33, 0x63, 0x36, 0x33, 0x63, 0x18, 0x30, 0x06, 0x1E, 0x00, 0x0C, 0x0C, 0x7E, 0x1E, 0x7C, 0x7F, 0x7F, 0x1E, 0x33, 0x3F, 0x0F, 0x73, 0x78, 0x63, 0x63, 0x1C, 0x7E, 0x1C, 0x7E, 0x1E, 0x3F, 0x33, 0x33, 0x63, 0x63, 0x33, 0x7F, 0x1E, 0x60, 0x1E, 0x0C, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x36, 0x0C, 0x63, 0x3B, 0x00, 0x06, 0x18, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x40, 0x3E, 0x3F, 0x3F, 0x1E, 0x0F, 0x1E, 0x1E, 0x0C, 0x1E, 0x1E, 0x0C, 0x0C, 0x06, 0x00, 0x30, 0x0C, 0x3E, 0x00, 0x00, 0x36, 0x3E, 0x33, 0x66, 0x00, 0x0C, 0x0C, 0x33, 0x0C, 0x0C, 0x00, 0x0C, 0x60, 0x63, 0x0C, 0x33, 0x33, 0x06, 0x33, 0x33, 0x0C, 0x33, 0x33, 0x0C, 0x0C, 0x0C, 0x3F, 0x18, 0x00, 0x60, 0x0C, 0x00, 0x7F, 0x0D, 0x18, 0x6E, 0x00, 0x18, 0x06, 0x1E, 0x0C, 0x00, 0x00, 0x00, 0x30, 0x73, 0x0C, 0x30, 0x03, 0x7F, 0x03, 0x33, 0x0C, 0x33, 0x03, 0x00, 0x00, 0x18, 0x00, 0x0C, 0x0C, 0x6F, 0x0C, 0x00, 0x36, 0x1E, 0x0C, 0x3B, 0x00, 0x18, 0x06, 0x7F, 0xBF, 0x00, 0x3F, 0x00, 0x18, 0x6B, 0x0C, 0x1E, 0x0E, 0x66, 0x3E, 0x3E, 0x06, 0x1E, 0x1F, 0x0C, 0x0C, 0x30, 0x00, 0x06, 0x06, 0x6F, 0x0C, 0x36, 0x7F, 0x2C, 0x66, 0x1C, 0x18, 0x18, 0x06, 0x1E, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x67, 0x0C, 0x03, 0x03, 0x36, 0x30, 0x30, 0x03, 0x33, 0x33, 0x0C, 0x0C, 0x18, 0x3F, 0x0C, 0x33, 0x6F, 0x0C, 0x36, 0x36, 0x1F, 0x63, 0x36, 0x0C, 0x0C, 0x0C, 0x33, 0x0C, 0x00, 0x00, 0x00, 0x06, 0x63, 0x1C, 0x33, 0x33, 0x1E, 0x31, 0x33, 0x33, 0x33, 0x33, 0x00, 0x00, 0x0C, 0x00, 0x18, 0x33, 0x63, 0x0C, 0x36, 0x36, 0x0C, 0x00, 0x1C, 0x0C, 0x06, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x3E, 0x0C, 0x1E, 0x1E, 0x0E, 0x3F, 0x1E, 0x3F, 0x1E, 0x1E, 0x00, 0x00, 0x06, 0x00, 0x30, 0x1E, 0x3E
};

typedef struct
{
	char cpcChara;
	u8 posBitmap;
} SCharMapping;


char charaMapBitmap[] = "!'\\#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\'abcdefghijklmnopqrstuvwxyz{|}~";

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

u16 getVKey(u16 pCpcKeyID)
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

struct SAmstrad
{
	u8 _mode;
	u8 _currentPage;
	u8 _internalTimer;
	TInterrupt _interruptFunction;

	HBITMAP _video[4];

	UCHAR _curPal[NB_COLORS + 1];
	UCHAR _memVideo[200][82];
}
_amstrad;

||||||| .r1110
=======
UCHAR fonts[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x80, 0x3B, 0x6E, 0x1E,0x3B, 0x1E, 0x3C, 0x03, 0x73, 0x1E, 0x33, 0x73, 0x1E, 0x63, 0x33, 0x1E, 0x30, 0x06, 0x78, 0x3E, 0x0E, 0x1F, 0x0C, 0x36, 0x63, 0x03, 0x3F, 0x07, 0x0C, 0x38, 0x00, 0x66, 0x00, 0x66, 0x33, 0x33, 0x66, 0x30, 0x18, 0x1F, 0x33, 0x0C, 0x33, 0x36, 0x0C, 0x6B, 0x33, 0x33, 0x3E, 0x3E, 0x30, 0x03, 0x1B, 0x33, 0x1E, 0x7F, 0x36, 0x1F, 0x19, 0x0C, 0x0C, 0x0C, 0x00, 0x19, 0x80, 0x3E, 0x33, 0x30, 0x66, 0x3F, 0x18, 0x33, 0x33, 0x0C, 0x03, 0x3C, 0x0C, 0x6B, 0x33, 0x33, 0x33, 0x66, 0x30, 0x1E, 0x18, 0x33, 0x33, 0x6B, 0x1C, 0x33, 0x0C, 0x0C, 0x0C, 0x0C, 0x00, 0x66, 0x00, 0x06, 0x33, 0x33, 0x66, 0x33, 0x3C, 0x33, 0x3B, 0x0C, 0x03, 0x36, 0x0C, 0x7F, 0x33, 0x33, 0x33, 0x66, 0x3B, 0x30, 0x18, 0x33, 0x33, 0x6B, 0x36, 0x33, 0x26, 0x38, 0x0C, 0x07, 0x00, 0x19, 0x80, 0x3C, 0x3E, 0x1E, 0x3E, 0x1E, 0x18, 0x1F, 0x36, 0x1C, 0x07, 0x33, 0x0C, 0x36, 0x6E, 0x1E, 0x6E, 0x3B, 0x6E, 0x1E, 0x3E, 0x33, 0x33, 0x63, 0x63, 0x33, 0x3F, 0x0C, 0x0C, 0x0C, 0x00, 0x66, 0x00, 0x00, 0x30, 0x00, 0x06, 0x00, 0x1B, 0x00, 0x30, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x0C, 0x6E, 0x19, 0x80, 0x00, 0x70, 0x00, 0x0E, 0x00, 0x0E, 0x00, 0x70, 0x0C, 0x03, 0x70, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0C, 0x38, 0x3B, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x33, 0x7E, 0x1E, 0x7C, 0x7F, 0x78, 0x1F, 0x33, 0x3F, 0x3C, 0x73, 0x7F, 0x63, 0x63, 0x1C, 0x78, 0x3B, 0x73, 0x1E, 0x1E, 0x1E, 0x0C, 0x63, 0x63, 0x1E, 0x7F, 0x1E, 0x01, 0x1E, 0x0C, 0x00, 0x00, 0x33, 0x33, 0x33, 0x36, 0x31, 0x30, 0x33, 0x33, 0x0C, 0x66, 0x33, 0x33, 0x63, 0x63, 0x36, 0x30, 0x66, 0x33, 0x33, 0x0C, 0x33, 0x1E, 0x77, 0x63, 0x0C, 0x33, 0x18, 0x03, 0x06, 0x0C, 0x00, 0x00, 0x3F, 0x33, 0x60, 0x33, 0x34, 0x34, 0x67, 0x33, 0x0C, 0x66, 0x36, 0x31, 0x6B, 0x67, 0x63, 0x30, 0x6D, 0x36, 0x03, 0x0C, 0x33, 0x33, 0x7F, 0x36, 0x0C, 0x19, 0x18, 0x06, 0x06, 0x0C, 0x00, 0x00, 0x33, 0x3E, 0x60, 0x33, 0x3C, 0x3C, 0x60, 0x3F, 0x0C, 0x06, 0x3C, 0x30, 0x7F, 0x6F, 0x63, 0x3E, 0x63, 0x3E, 0x1E, 0x0C, 0x33, 0x33, 0x6B, 0x1C, 0x1E, 0x0C, 0x18, 0x0C, 0x06, 0x0C, 0x00, 0x00, 0x33, 0x33, 0x60, 0x33, 0x34, 0x34, 0x60, 0x33, 0x0C, 0x06, 0x36, 0x30, 0x7F, 0x7B, 0x63, 0x33, 0x63, 0x33, 0x30, 0x0C, 0x33, 0x33, 0x63, 0x1C, 0x33, 0x46, 0x18, 0x18, 0x06, 0x3F, 0x00, 0x06, 0x1E, 0x33, 0x33, 0x36, 0x31, 0x31, 0x33, 0x33, 0x0C, 0x06, 0x33, 0x30, 0x77, 0x73, 0x36, 0x33, 0x36, 0x33, 0x33, 0x2D, 0x33, 0x33, 0x63, 0x36, 0x33, 0x63, 0x18, 0x30, 0x06, 0x1E, 0x00, 0x0C, 0x0C, 0x7E, 0x1E, 0x7C, 0x7F, 0x7F, 0x1E, 0x33, 0x3F, 0x0F, 0x73, 0x78, 0x63, 0x63, 0x1C, 0x7E, 0x1C, 0x7E, 0x1E, 0x3F, 0x33, 0x33, 0x63, 0x63, 0x33, 0x7F, 0x1E, 0x60, 0x1E, 0x0C, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x36, 0x0C, 0x63, 0x3B, 0x00, 0x06, 0x18, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x40, 0x3E, 0x3F, 0x3F, 0x1E, 0x0F, 0x1E, 0x1E, 0x0C, 0x1E, 0x1E, 0x0C, 0x0C, 0x06, 0x00, 0x30, 0x0C, 0x3E, 0x00, 0x00, 0x36, 0x3E, 0x33, 0x66, 0x00, 0x0C, 0x0C, 0x33, 0x0C, 0x0C, 0x00, 0x0C, 0x60, 0x63, 0x0C, 0x33, 0x33, 0x06, 0x33, 0x33, 0x0C, 0x33, 0x33, 0x0C, 0x0C, 0x0C, 0x3F, 0x18, 0x00, 0x60, 0x0C, 0x00, 0x7F, 0x0D, 0x18, 0x6E, 0x00, 0x18, 0x06, 0x1E, 0x0C, 0x00, 0x00, 0x00, 0x30, 0x73, 0x0C, 0x30, 0x03, 0x7F, 0x03, 0x33, 0x0C, 0x33, 0x03, 0x00, 0x00, 0x18, 0x00, 0x0C, 0x0C, 0x6F, 0x0C, 0x00, 0x36, 0x1E, 0x0C, 0x3B, 0x00, 0x18, 0x06, 0x7F, 0xBF, 0x00, 0x3F, 0x00, 0x18, 0x6B, 0x0C, 0x1E, 0x0E, 0x66, 0x3E, 0x3E, 0x06, 0x1E, 0x1F, 0x0C, 0x0C, 0x30, 0x00, 0x06, 0x06, 0x6F, 0x0C, 0x36, 0x7F, 0x2C, 0x66, 0x1C, 0x18, 0x18, 0x06, 0x1E, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x67, 0x0C, 0x03, 0x03, 0x36, 0x30, 0x30, 0x03, 0x33, 0x33, 0x0C, 0x0C, 0x18, 0x3F, 0x0C, 0x33, 0x6F, 0x0C, 0x36, 0x36, 0x1F, 0x63, 0x36, 0x0C, 0x0C, 0x0C, 0x33, 0x0C, 0x00, 0x00, 0x00, 0x06, 0x63, 0x1C, 0x33, 0x33, 0x1E, 0x31, 0x33, 0x33, 0x33, 0x33, 0x00, 0x00, 0x0C, 0x00, 0x18, 0x33, 0x63, 0x0C, 0x36, 0x36, 0x0C, 0x00, 0x1C, 0x0C, 0x06, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x3E, 0x0C, 0x1E, 0x1E, 0x0E, 0x3F, 0x1E, 0x3F, 0x1E, 0x1E, 0x00, 0x00, 0x06, 0x00, 0x30, 0x1E, 0x3E
};

struct SAmstrad
{
	u8 _mode;
	u8 _currentPage;
	u8 _internalTimer;
	TInterrupt _interruptFunction;

	HBITMAP _video[4];

	UCHAR _curPal[NB_COLORS + 1];
	UCHAR _memVideo[200][82];
}
_amstrad;

>>>>>>> .r1114
COLORREF GetColorHW(int pHW)
{
	for (int i = 0; i < 27; i++)
	{
		if (_palette[i].hw == pHW)
			return _palette[i].rgb;
	}
	return _palette[0].rgb;
}

COLORREF GetColorFW(int pFW)
{
	return _palette[pFW].rgb;
}

u8 cpct_getHWColour(u16 pFW)
{
	return _palette[pFW].hw;
}

<<<<<<< .mine
HBITMAP GetCurVideoBuff()
{
	switch (_curVideo)
	{
	case cpct_page00:
		return _amstrad._video[3];
		break;
||||||| .r1110
	HBITMAP _video;
=======
HBITMAP GetCurVideoBuff()
{
	switch (_curVideo)
	{
		case cpct_page00 :
			return _amstrad._video[3];
		break;
		
		case cpct_page40 :
			return _amstrad._video[2];
			break;
>>>>>>> .r1114

<<<<<<< .mine
	case cpct_page40:
		return _amstrad._video[2];
		break;

	case cpct_page80:
		return _amstrad._video[1];
		break;

	case cpct_pageC0:
	default:
		return _amstrad._video[0];
		break;
	}
||||||| .r1110
	UCHAR _curPal[NB_COLORS];
	UCHAR _mem[0xFFFF];
=======
		case cpct_page80:
			return _amstrad._video[1];
			break;

		case cpct_pageC0:
		default:
			return _amstrad._video[0];
			break;
	}
>>>>>>> .r1114
}

u8* getMemory(int pAddress)
{
	if (pAddress > 0xFFFF)
		return (u8*)pAddress;
	else
		return NULL;
}

void cpct_memcpy(void* to, const void* from, u16 size)
{
	u8* toMem = getMemory((int)to);
	u8* fromMem = getMemory((int)from);

<<<<<<< .mine
	if (toMem != NULL && fromMem != NULL)
		memcpy_s(toMem, size, fromMem, size);
	else
	{


	}
||||||| .r1110
	memcpy_s(toMem, size, fromMem, size);
=======
	if (toMem != NULL && fromMem != NULL)
		memcpy_s(toMem, size, fromMem, size);
	else
	{
		

	}
>>>>>>> .r1114
}

void cpct_memset(void *array, u8  value, u16 size)
{
	u8* arrayMem = getMemory((int)array);

	if (arrayMem != NULL)
		memset(arrayMem, value, size);
	else
	{
		u8 x = 0;
		int addr = (int)array;
		SetCurVideo(addr);

		cpct_drawSolidBox(&x, value, 80, size / 80);
	}
}

void cpct_setStackLocation(void* memory)
{
	CPCTeleraWin();
}

u8 cpct_rand8()
{
	return (u8)rand();
}

VOID CALLBACK InternalTimer(
	HWND hwnd,
	UINT message,
	UINT idTimer,
	DWORD dwTime)
{
<<<<<<< .mine
	if (_amstrad._interruptFunction != NULL)
		_amstrad._interruptFunction();

	_amstrad._internalTimer++;

	MsgLoop();
	InvalidateRect(_hWnd, NULL, FALSE);
||||||| .r1110
	_amstrad._interruptFunction();
=======
	if (_amstrad._interruptFunction != NULL)
		_amstrad._interruptFunction();

	_amstrad._internalTimer++;

	InvalidateRect(_hWnd, NULL, FALSE);
>>>>>>> .r1114
}

void cpct_setInterruptHandler(void(*intHandler)(void))
{
	_amstrad._interruptFunction = intHandler;
}

void cpct_reenableFirmware(u16 firmware_ROM_code)
{
}

u16 cpct_disableFirmware()
{
	srand(GetTickCount());
	CPCTeleraWin();
	return 0;
}

void cpct_scanKeyboard_f()
{
	_curKey = FALSE;
	MsgLoop();
}

void cpct_scanKeyboard_if()
{
	_curKey = FALSE;
	MsgLoop();
}

void cpct_scanKeyboard()
{
	_curKey = FALSE;
	MsgLoop();
}

u8 cpct_isKeyPressed(cpct_keyID key)
{
<<<<<<< .mine
	u16 keyVKey = getVKey(key);
	return GetAsyncKeyState(keyVKey) != 0 ? TRUE : FALSE;
||||||| .r1110
	MsgLoop();
	return (_curKey == key);
=======
	MsgLoop();
	BOOL isKeyPressed = (_curKey == key);
	if (isKeyPressed)
		_curKey = 0;

	return isKeyPressed;
>>>>>>> .r1114
}

u8 cpct_isAnyKeyPressed_f()
{
<<<<<<< .mine
	BOOL isKeyPressed = (_curKey != FALSE);
	return isKeyPressed;
||||||| .r1110
	MsgLoop();
	return (_curKey != 0);
=======
	MsgLoop();
	BOOL isKeyPressed = (_curKey != 0);
	return isKeyPressed;
>>>>>>> .r1114
}

void cpct_fw2hw(void *fw_colour_array, u16 size)
{
	u8* fwPal = (u8*)fw_colour_array;
	for (u16 i = 0; i < size; i++)
		fwPal[i] = cpct_getHWColour(fwPal[i]);
}

void cpct_setPalette(u8* ink_array, u16 ink_array_size)
{
	memcpy(_amstrad._curPal, ink_array, ink_array_size);
	CreatePaletteCpc();
	FillBorder();
}

void cpct_setPALColour(u8 pen, u8 hw_ink)
{
	_amstrad._curPal[pen] = hw_ink;
	CreatePaletteCpc();
	FillBorder();
}

void cpct_drawTileAligned2x4(void* sprite, void* memory)
{
	cpct_drawSprite(sprite, memory, 2, 4);
}

void cpct_drawTileAligned2x4_f(void* sprite, void* memory)
{
	cpct_drawSprite(sprite, memory, 2, 4);
}

void cpct_drawTileAligned2x8(void* sprite, void* memory)
{
	cpct_drawSprite(sprite, memory, 2, 8);
}

<<<<<<< .mine
void cpct_drawTileAligned2x8_f(void* sprite, void* memory)
{
	cpct_drawSprite(sprite, memory, 2, 8);
}

void cpct_drawTileAligned4x4(void* sprite, void* memory)
{
	cpct_drawSprite(sprite, memory, 4, 4);
}

void cpct_drawTileAligned4x8(void* sprite, void* memory)
{
	cpct_drawSprite(sprite, memory, 4, 8);
}

void cpct_drawTileAligned4x8_f(void* sprite, void* memory)
{
	cpct_drawSprite(sprite, memory, 4, 8);
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

void cpct_drawCharM0(void* video_memory, u8 fg_pen, u8 bg_pen, u8 ascii)
{
	u8* x = (u8*)video_memory;
	u8 y = GetCoordY(video_memory);

	DrawFont(*x, y, fg_pen, bg_pen, ascii);
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

void cpct_drawStringM0(void* string, void* video_memory, u8 fg_pen, u8 bg_pen)
{
	char* str = (char*)string;
	u8 x = *((u8*)video_memory);
	u8 y = GetCoordY(video_memory);

	for (UCHAR i = 0; i < strlen(str); i++)
	{
		if (str[i] != ' ')
		{
			DrawFont(x + i * 4, y, fg_pen, bg_pen, str[i]);
		}
	}
}

HBITMAP getVideoBitmap(int pScreenAddr)
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

u8* cpct_getScreenPtr(void* screen_start, u8 x, u8 y)
{
	WORD addr = (WORD)screen_start;
	SetCurVideo(addr);

	return &_amstrad._memVideo[y][x];
}

||||||| .r1110
=======
void cpct_drawTileAligned2x8_f(void* sprite, void* memory)
{
	cpct_drawSprite(sprite, memory, 2, 8);
}

void cpct_drawTileAligned4x4(void* sprite, void* memory)
{
	cpct_drawSprite(sprite, memory, 4, 4);
}

void cpct_drawTileAligned4x8(void* sprite, void* memory)
{
	cpct_drawSprite(sprite, memory, 4, 8);
}

void cpct_drawTileAligned4x8_f(void* sprite, void* memory)
{
	cpct_drawSprite(sprite, memory, 4, 8);
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

void cpct_drawCharM0(void* video_memory, u8 fg_pen, u8 bg_pen, u8 ascii)
{
	u8* x = (u8*)video_memory;
	u8 y = GetCoordY(video_memory);

	DrawFont(*x, y, ascii);
}

void cpct_drawStringM0(void* string, void* video_memory, u8 fg_pen, u8 bg_pen)
{
	char* str = (char*)string;
	u8 x = *((u8*)video_memory);
	u8 y = GetCoordY(video_memory);

	for (UCHAR i = 0; i < strlen(str); i++)
	{
		if (*str != ' ')
		{
			DrawFont(x + i * 4, y, str[i]);
		}
	}
}

HBITMAP getVideoBitmap(int pScreenAddr)
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

u8* cpct_getScreenPtr(void* screen_start, u8 x, u8 y)
{
	WORD addr = (WORD)screen_start;
	SetCurVideo(addr);

	return &_amstrad._memVideo[y][x];
}

>>>>>>> .r1114
void cpct_waitVSYNC()
{
	Sleep(30);
}

u16 cpct_count2VSYNC()
{
	return 0;
}

void SetPalette(int i, UCHAR pHW)
{
	_amstrad._curPal[i] = pHW;
}

void fillBox(HDC hdc, u8 pattern, int x, int y, int cx, int cy)
{
	//u8 pix1 = pattern & 0xF0 >> 4;
	u8 pix0 = pattern & 0x0F;

	int hw = _amstrad._curPal[pix0];
	COLORREF rgb = GetColorHW(hw);
	HBRUSH brush = CreateSolidBrush(rgb);

	if (cx > 80)
		cx = 80;

	if (cy > 200)
		cy = 200;

	if (cx > 80)
		cx = 80;

	if (cy > 200)
		cy = 200;

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

void drawSprite(void *sprite, int x, int y, int cx, int cy, BOOL pMasked)
{
	HDC hdc = GetDC(_hWnd);
	HDC memDC = CreateCompatibleDC(hdc);

<<<<<<< .mine
	HBITMAP oldBitmap = SelectObject(memDC, GetCurVideoBuff());

||||||| .r1110
	HBITMAP oldBitmap = SelectObject(memDC, _amstrad._video);
	
=======
	HBITMAP oldBitmap = SelectObject(memDC, GetCurVideoBuff());
	
>>>>>>> .r1114
	DisplayBitmap(memDC, x, y, cx, cy, sprite, pMasked);

	SelectObject(memDC, oldBitmap);
	DeleteDC(memDC);
	ReleaseDC(_hWnd, hdc);
}

void cpct_drawSprite(void *sprite, void* memory, u8 width, u8 height)
{
	u8* x = (u8*)memory;
	u8 y = GetCoordY(memory);
	drawSprite(sprite, *x, y, width, height, FALSE);
}

void cpct_drawSpriteMasked(void *sprite, void* memory, u8 width, u8 height)
{
	u8* x = (u8*)memory;
	u8 y = GetCoordY(memory);
	drawSprite(sprite, *x, y, width, height, TRUE);
}

void cpct_clearScreen(u8 colour_pattern)
{
	u8 x = 0;

	_curVideo = cpct_pageC0;
	cpct_drawSolidBox(&x, colour_pattern, 80, 200);
}

void cpct_drawSolidBox(void *memory, u8 colour_pattern, u8 width, u8 height)
{
	HDC hdc = GetDC(_hWnd);
	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP oldBitmap = SelectObject(memDC, GetCurVideoBuff());

	u8* x = (u8*)memory;
	u8 y = GetCoordY(memory);

	u8 pix = M0byte2px(colour_pattern);
	fillBox(memDC, pix, *x, y, width, height);

	SelectObject(memDC, oldBitmap);
	DeleteDC(memDC);
	ReleaseDC(_hWnd, hdc);
}

void cpct_setVideoMode(u8 videoMode)
{
	_amstrad._mode = videoMode;
}

void cpct_setVideoMemoryPage(u8 page_6LSb)
{
	_amstrad._currentPage = page_6LSb;
	InvalidateRect(_hWnd, NULL, FALSE);
}

void cpc_UnExo(const u8 *source, u8* dest)
{

}

void Redraw(HWND pWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(pWnd, &ps);
	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP oldBitmap = SelectObject(memDC, GetCurVideoBuff());

	BitBlt(hdc, 0, 0, FULL_SCREEN_CX, FULL_SCREEN_CY, memDC, 0, 0, SRCCOPY);

	SelectObject(memDC, oldBitmap);
	DeleteDC(memDC);

	EndPaint(pWnd, &ps);
}

void Redraw(HWND pWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(pWnd, &ps);
	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP oldBitmap = SelectObject(memDC, GetCurVideoBuff());

	BitBlt(hdc, 0, 0, FULL_SCREEN_CX, FULL_SCREEN_CY, memDC, 0, 0, SRCCOPY);

	SelectObject(memDC, oldBitmap);
	DeleteDC(memDC);

	EndPaint(pWnd, &ps);
}

LRESULT FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
<<<<<<< .mine
	case WM_KEYDOWN:
		_curKey = ((UINT)wParam);
		break;
||||||| .r1110
	case WM_KEYDOWN:
		HandleKeyboard((UINT)wParam);
		break;
=======
		case WM_KEYDOWN:
			HandleKeyboard((UINT)wParam);
			break;
>>>>>>> .r1114

<<<<<<< .mine
	case WM_PAINT:
		Redraw(hWnd);
		break;

	case WM_DESTROY:
		exit(0);
||||||| .r1110
	case WM_DESTROY:
		exit(0);
=======
		case WM_PAINT:
			Redraw(hWnd);
			break;

		case WM_DESTROY:
			exit(0);
>>>>>>> .r1114
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
RECT CalculateWindowRect(HWND hWindow, SIZE szDesiredClient)
{
	// Declare a RECT to hold the results of our calculations
	RECT rcDesiredWindowRect;

	// Get the current window rect and its client rect
	RECT rcCurrentWindowRect;
	RECT rcCurrentClientRect;

	GetWindowRect(hWindow, &rcCurrentWindowRect);
	GetClientRect(hWindow, &rcCurrentClientRect);

	// Get the difference between the current and desired client areas
	SIZE szClientDifference = { rcCurrentClientRect.right - szDesiredClient.cx, rcCurrentClientRect.bottom - szDesiredClient.cy };

	// Get the difference between the current window rect and the desired window rect
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

<<<<<<< .mine
u8 cpct_px2byteM0(u8 px0, u8 px1)
{
	UCHAR bit0 = (px0 & 0x01);
	UCHAR bit1 = (px0 & 0x02) >> 1;
	UCHAR bit2 = (px0 & 0x04) >> 2;
	UCHAR bit3 = (px0 & 0x08) >> 3;
	UCHAR bita = (px1 & 0x01);
	UCHAR bitb = (px1 & 0x02) >> 1;
	UCHAR bitc = (px1 & 0x04) >> 2;
	UCHAR bitd = (px1 & 0x08) >> 3;

	return bit0 << 7 | bita << 6 | bit2 << 5 | bitc << 4 | bit1 << 3 | bitb << 2 | bit3 << 1 | bitd;
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
	int sizeBitmapInfo = sizeof(BITMAPINFOHEADER) + 2 * sizeof(WORD);
	LPBITMAPINFO bitmapInfos = (LPBITMAPINFO)malloc(sizeBitmapInfo);

	if (_amstrad._mode == 0)
	{
		x *= 2;
	}

	memset(bitmapInfos, 0, sizeof(BITMAPINFOHEADER));
	bitmapInfos->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfos->bmiHeader.biWidth = 256;
	bitmapInfos->bmiHeader.biHeight = 24;
	bitmapInfos->bmiHeader.biPlanes = 1;
	bitmapInfos->bmiHeader.biBitCount = 1;
	bitmapInfos->bmiHeader.biCompression = BI_RGB;
	bitmapInfos->bmiHeader.biClrUsed = 2;

	WORD* pal = (WORD*)bitmapInfos->bmiColors;
	pal[0] = bgPen;
	pal[1] = fgPen;

	SelectPalette(hdc, _hPal, FALSE);
	RealizePalette(hdc);

	int index = FindCharaIndex(pChara);
	int fontx = (index % FONT_NB_LINE - 1) * FONT_SIZE;
	int fonty = (FONT_NB_COL - index / FONT_NB_LINE - 1) * FONT_SIZE;

	StretchDIBits(hdc, x * 2 + BORDER_CX, y + BORDER_UP_CY, FONT_SIZE*2, FONT_SIZE, fontx, fonty, FONT_SIZE, FONT_SIZE, fonts, bitmapInfos, DIB_PAL_COLORS, SRCCOPY);
}

void DrawFont(int x, int y, u8 fgPen, u8 bgPen, char chara)
{
	HDC hdc = GetDC(_hWnd);
	HDC memDC = CreateCompatibleDC(hdc);

	HBITMAP oldBitmap = SelectObject(memDC, GetCurVideoBuff());

	DisplayFont(memDC, x, y, fgPen, bgPen, chara);

	SelectObject(memDC, oldBitmap);
	DeleteDC(memDC);
	ReleaseDC(_hWnd, hdc);
}

||||||| .r1110
=======

u8 cpct_px2byteM0(u8 px0, u8 px1)
{
	UCHAR bit0 = (px0 & 0x01);
	UCHAR bit1 = (px0 & 0x02) >> 1;
	UCHAR bit2 = (px0 & 0x04) >> 2;
	UCHAR bit3 = (px0 & 0x08) >> 3;
	UCHAR bita = (px1 & 0x01);
	UCHAR bitb = (px1 & 0x02) >> 1;
	UCHAR bitc = (px1 & 0x04) >> 2;
	UCHAR bitd = (px1 & 0x08) >> 3;

	return bit0 << 7 | bita << 6 | bit2 << 5 | bitc << 4 | bit1 << 3 | bitb << 2 | bit3 << 1 | bitd;
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

void DisplayFont(HDC hdc, int x, int y, int chara)
{
	int sizeBitmapInfo = sizeof(BITMAPINFOHEADER) + 2 * sizeof(WORD);
	LPBITMAPINFO bitmapInfos = (LPBITMAPINFO)malloc(sizeBitmapInfo);

	if (_amstrad._mode == 0)
	{
		x *= 2;
	}

	memset(bitmapInfos, 0, sizeof(BITMAPINFOHEADER));
	bitmapInfos->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfos->bmiHeader.biWidth = 256;
	bitmapInfos->bmiHeader.biHeight = 24;
	bitmapInfos->bmiHeader.biPlanes = 1;
	bitmapInfos->bmiHeader.biBitCount = 1;
	bitmapInfos->bmiHeader.biCompression = BI_RGB;
	bitmapInfos->bmiHeader.biClrUsed = 2;

	WORD* pal = (WORD*)bitmapInfos->bmiColors;
	for (int i = 0; i < 2; i++)
		pal[i] = i;

	SelectPalette(hdc, _hPal, FALSE);
	RealizePalette(hdc);

	//StretchDIBits(hdc, 0, 0, 256, 24, 0, 0, 256, 24, fonts, bitmapInfos, DIB_PAL_COLORS, SRCCOPY);
	
	StretchDIBits(hdc, x * 2 + BORDER_CX, y + BORDER_UP_CY, 8*2, 8, 0, 0, 8, 8, fonts, bitmapInfos, DIB_PAL_COLORS, SRCCOPY);
}

void DrawFont(int x, int y, int chara)
{
	HDC hdc = GetDC(_hWnd);
	HDC memDC = CreateCompatibleDC(hdc);

	HBITMAP oldBitmap = SelectObject(memDC, GetCurVideoBuff());

	DisplayFont(memDC, x, y, chara);

	SelectObject(memDC, oldBitmap);
	DeleteDC(memDC);
	ReleaseDC(_hWnd, hdc);
}

>>>>>>> .r1114
void DisplayBitmap(HDC hdc, int x, int y, int cx, int cy, char* data, BOOL pMasked)
{
	int xi, yi;
	int sizeBitmapInfo = sizeof(BITMAPINFOHEADER) + 16 * sizeof(WORD);
	LPBITMAPINFO bitmapInfos = (LPBITMAPINFO)malloc(sizeBitmapInfo);
	WORD* pal = NULL;

	int widthAlignedDWORD = (((cx) * 8 + 31)  & (~31)) / 8;
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

<<<<<<< .mine
				pix++;
				i++;
||||||| .r1110
				pix++;
				i += (widthAlignedDWORD - cx + 1);
=======
				pix++;	
				i++;
>>>>>>> .r1114
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

	memset(bitmapInfos, 0, sizeof(BITMAPINFOHEADER));
	bitmapInfos->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfos->bmiHeader.biWidth = cx;
	bitmapInfos->bmiHeader.biHeight = -cy;
	bitmapInfos->bmiHeader.biPlanes = 1;
	bitmapInfos->bmiHeader.biBitCount = 4;
	bitmapInfos->bmiHeader.biCompression = BI_RGB;
	bitmapInfos->bmiHeader.biClrUsed = NB_COLORS;

	pal = (WORD*)bitmapInfos->bmiColors;
	for (int i = 0; i < NB_COLORS; i++)
		pal[i] = i;

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

	for (u8 y = 0; y < 200; y++)
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

	for (int i = 0; i < 4; i++)
		_amstrad._video[i] = CreateCompatibleBitmap(hdc, FULL_SCREEN_CX, FULL_SCREEN_CY);

<<<<<<< .mine
	SetTimer(_hWnd, 10000, 15, InternalTimer);
||||||| .r1110
	FillBorder(hdc);
=======
	SetTimer(_hWnd, 10000, 33, MyTimerProc);
>>>>>>> .r1114

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