/*
	SDL - Simple DirectMedia Layer
	Copyright (C) 1997-2012 Sam Lantinga

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

	Sam Lantinga
	slouken@libsdl.org
*/
#ifndef _SDL_PRIZMEVENTS_C_H
	#define _SDL_PRIZMEVENTS_C_H 1

#include "SDL_config.h"

#include "SDL_prizmvideo.h"

//#include <keyboard.hpp>

enum {
    PZM_KEY_ACON,//   = 10,
    PZM_KEY_DOWN,//   = 37,
    PZM_KEY_EXIT,//   = 47,
    PZM_KEY_F1,//     = 79,
    PZM_KEY_F2,//     = 69,
    PZM_KEY_F3,//     = 59,
    PZM_KEY_F4,//     = 49,
    PZM_KEY_F5,//     = 39,
    PZM_KEY_F6,//     = 29,
    PZM_KEY_LEFT,//   = 38,
    PZM_KEY_RETURN,// = 31,
    PZM_KEY_RIGHT,//  = 27,
    PZM_KEY_UP,//     = 28,
    PZM_KEY_1,//      = 72,
    PZM_KEY_2,//      = 62,
    PZM_KEY_3,//      = 52,
    PZM_KEY_4,//      = 73,
    PZM_KEY_5,//      = 63,
    PZM_KEY_6,//      = 53,
    PZM_KEY_7,//      = 74,
    PZM_KEY_8,//      = 64,
    PZM_KEY_9,//      = 54,
    PZM_KEY_ALPHA,//  = 77,
    PZM_KEY_SHIFT,//  = 78
    PZM_KEY_MENU,//   = 48
    PZM_NUMKEYS
    };

/* Variables and functions exported by SDL_sysevents.c to other parts
   of the native video subsystem (SDL_sysvideo.c)
*/
extern void PZM_InitOSKeymap(_THIS);
extern void PZM_PumpEvents(_THIS);

/* end of SDL_prizmevents_c.h ... */
#endif
