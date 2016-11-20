//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of CPCtelera: An Amstrad CPC Game Engine
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

#include <winCpctelera.h>

extern void DrawSprite(void *sprite, void *memory, int cx, int cy, u8 pSpriteMode);
extern u8* GetVideoBufferFromAddress(int pScreenAddr);

#define TILE_CX	2
#define TILE_CY	4

static u8** _curTilset;

void cpct_etm_setTileset2x4(const void* ptileset)
{
	_curTilset = (u8**)ptileset;
}

void cpct_etm_drawTileBox2x4(u8 x, u8 y, u8 w, u8 h, u8 map_width, void* pvideomem, const void* ptilemap)
{
	u8* videoAddress = (u8*)pvideomem + 0x50 * (y / 2) + 0x2000 * (y % 2) + 2 * x;

	u8* screen = GetVideoBufferFromAddress((int)videoAddress);
	u8* tilemap = ((u8*)ptilemap) + y*map_width + x;

	for (int iy = 0; iy < h; iy++)
	{
		for (int ix = 0; ix < w; ix++)
		{
			u8* tileSprite = _curTilset[*tilemap++];
			DrawSprite(tileSprite, screen, TILE_CX, TILE_CY, SPRITE_NORMAL);

			screen += TILE_CX;
		}
		tilemap += map_width - w;
		screen += CPC_SCR_CX_BYTES * TILE_CY - w * TILE_CX;
	}

	MsgLoop();
}

void cpct_etm_drawTilemap2x4_f(u8 map_width, u8 map_height, u8* pvideomem, u8* ptilemap)
{
	cpct_etm_drawTilemap2x4(map_width, map_height, pvideomem, ptilemap);
}

void cpct_etm_drawTileRow2x4(u8 numtiles, void* pvideomem, const void* ptilemap_row)
{
	u8* screen = (u8*)pvideomem;
	u8* tilemap = (u8*)ptilemap_row;

	for (int ix = 0; ix < numtiles; ix++)
	{
		u8 tileIndex = *tilemap++;
		u8* tileSprite = _curTilset[tileIndex];
		cpct_drawSprite(tileSprite, screen, TILE_CX, TILE_CY);

		screen += TILE_CX;
	}
}