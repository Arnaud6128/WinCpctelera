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

#define WINCPCTELERA	1

#ifndef CPCTELERA_H
#define CPCTELERA_H

#define __z88dk_fastcall	
#define __z88dk_callee	
#define __naked 
#define __at(X) \
		/* X */
#define __asm__(X) \
		/* X */

#define __sdcc_heap_init()

#include "types.h"
#include "firmware/firmware.h"
#include "memutils/memutils.h"
#include "keyboard/keyboard.h"
#include "bitarray/bitarray.h"
#include "sprites/sprites.h"
#include "strings/strings.h"
#include "video/videomode.h"
#include "audio/audio.h"
#include "random/random.h"
#include <macros/allmacros.h>
#include "easytilemaps/easytilemaps.h"
#include "compression/compression.h"

#include "_helper/wincpctelera_helper.h"

#endif