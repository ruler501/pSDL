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
#include "SDL_config.h"

#include "SDL.h"
#include "../../events/SDL_sysevents.h"
#include "../../events/SDL_events_c.h"
#include "../SDL_cursor_c.h"
#include "SDL_keyboard.h"

#include "SDL_prizmvideo.h"
#include "SDL_prizmevents_c.h"

static int pzmk_keymap[PZM_NUMKEYS];
static SDLKey sdlk_keymap[PZM_NUMKEYS] = {SDLK_UNKNOWN};
static char key_state[PZM_NUMKEYS] = {SDL_RELEASED};

const unsigned short* keyboard_register = (unsigned short*)0xA44B0000;
unsigned short lastkey[8];
unsigned short holdkey[8];

void keyupdate(void) {
    SDL_memcpy(holdkey, lastkey, sizeof(unsigned short)*8);
    SDL_memcpy(lastkey, keyboard_register, sizeof(unsigned short)*8);
}
int keydownlast(int basic_keycode) {
    int row, col, word, bit;
    row = basic_keycode%10;
    col = basic_keycode/10-1;
    word = row>>1;
    bit = col + 8*(row&1);
    return (0 != (lastkey[word] & 1<<bit));
}
int keydownhold(int basic_keycode) {
    int row, col, word, bit;
    row = basic_keycode%10;
    col = basic_keycode/10-1;
    word = row>>1;
    bit = col + 8*(row&1);
    return (0 != (holdkey[word] & 1<<bit));
}


void PZM_PumpEvents(_THIS)
{
	int i;
	keyupdate();
	for ( i = 0; i < PZM_NUMKEYS; ++i ) {
		if ( sdlk_keymap[i] == SDLK_UNKNOWN )
			continue;
		//BOOL key_pressed = isKeyPressed(nspk_keymap[i]);
		int key_pressed = keydownlast(pzmk_keymap[i]);// && keydownhold(pzmk_keymap[i]);
		SDL_keysym keysym;
		keysym.scancode = i;
		keysym.sym = sdlk_keymap[i];
		if ( key_state[i] == SDL_RELEASED ) {
			if ( key_pressed ) {
				SDL_PrivateKeyboard(SDL_PRESSED, &keysym);
				key_state[i] = SDL_PRESSED;
			}
		} else if ( ! key_pressed ) {
			SDL_PrivateKeyboard(SDL_RELEASED, &keysym);
			key_state[i] = SDL_RELEASED;
		}
	}
}

void PZM_InitOSKeymap(_THIS)
{
	/* Enum value -> # */
	pzmk_keymap[PZM_KEY_ACON]    = 10;
	pzmk_keymap[PZM_KEY_DOWN]    = 37;
	pzmk_keymap[PZM_KEY_EXIT]    = 47;
	pzmk_keymap[PZM_KEY_F1]      = 79;
	pzmk_keymap[PZM_KEY_F2]      = 69;
	pzmk_keymap[PZM_KEY_F3]      = 59;
	pzmk_keymap[PZM_KEY_F4]      = 49;
	pzmk_keymap[PZM_KEY_F5]      = 39;
	pzmk_keymap[PZM_KEY_F6]      = 29;
	pzmk_keymap[PZM_KEY_LEFT]    = 38;
	pzmk_keymap[PZM_KEY_RETURN]  = 31;
	pzmk_keymap[PZM_KEY_RIGHT]   = 27;
	pzmk_keymap[PZM_KEY_UP]      = 28;
	pzmk_keymap[PZM_KEY_1]       = 72;
	pzmk_keymap[PZM_KEY_2]       = 62;
	pzmk_keymap[PZM_KEY_3]       = 52;
	pzmk_keymap[PZM_KEY_4]       = 73;
	pzmk_keymap[PZM_KEY_5]       = 63;
	pzmk_keymap[PZM_KEY_6]       = 53;
	pzmk_keymap[PZM_KEY_7]       = 74;
	pzmk_keymap[PZM_KEY_8]       = 64;
	pzmk_keymap[PZM_KEY_9]       = 54;


	/* Enum value -> SDLK_* */
	sdlk_keymap[PZM_KEY_ACON]   = SDLK_POWER;
	sdlk_keymap[PZM_KEY_DOWN]   = SDLK_DOWN;
	sdlk_keymap[PZM_KEY_EXIT]   = SDLK_ESCAPE;
	sdlk_keymap[PZM_KEY_F1]     = SDLK_F1;
	sdlk_keymap[PZM_KEY_F2]     = SDLK_F2;
	sdlk_keymap[PZM_KEY_F3]     = SDLK_F3;
	sdlk_keymap[PZM_KEY_F4]     = SDLK_F4;
	sdlk_keymap[PZM_KEY_F5]     = SDLK_F5;
	sdlk_keymap[PZM_KEY_F6]     = SDLK_F6;
	sdlk_keymap[PZM_KEY_LEFT]   = SDLK_LEFT;
	sdlk_keymap[PZM_KEY_RETURN] = SDLK_RETURN;
	sdlk_keymap[PZM_KEY_RIGHT]  = SDLK_RIGHT;
	sdlk_keymap[PZM_KEY_UP]     = SDLK_UP;
	sdlk_keymap[PZM_KEY_1]      = SDLK_1;
	sdlk_keymap[PZM_KEY_2]      = SDLK_2;
	sdlk_keymap[PZM_KEY_3]      = SDLK_3;
	sdlk_keymap[PZM_KEY_4]      = SDLK_4;
	sdlk_keymap[PZM_KEY_5]      = SDLK_5;
	sdlk_keymap[PZM_KEY_6]      = SDLK_6;
	sdlk_keymap[PZM_KEY_7]      = SDLK_7;
	sdlk_keymap[PZM_KEY_8]      = SDLK_8;
	sdlk_keymap[PZM_KEY_9]      = SDLK_9;
	sdlk_keymap[PZM_KEY_ALPHA]  = SDLK_LCTRL;
	sdlk_keymap[PZM_KEY_SHIFT]  = SDLK_LSHIFT;
	sdlk_keymap[PZM_KEY_MENU]   = SDLK_MENU;

}

/* end of SDL_prizmevents.c ... */

