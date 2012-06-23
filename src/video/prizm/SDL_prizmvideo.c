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

#include "SDL.h"
#include "SDL_config.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"

#include "SDL_prizmvideo.h"
#include "SDL_prizmevents_c.h"

#define SCREEN_WIDTH 384
#define SCREEN_HEIGHT 216

static Uint16 pzm_palette[256] = {0};

/* Initialization/Query functions */
static int PZM_VideoInit(_THIS, SDL_PixelFormat *vformat);
static SDL_Rect **PZM_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags);
static SDL_Surface *PZM_SetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
static int PZM_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors);
static void PZM_VideoQuit(_THIS);

/* etc. */
static void PZM_UpdateRects(_THIS, int numrects, SDL_Rect *rects);

/* PZM driver bootstrap functions */

static int PZM_Available(void)
{
	return(1);
}


static void PZM_DeleteDevice(SDL_VideoDevice *device)
{
	SDL_free(device->hidden);
	SDL_free(device);
}

static SDL_VideoDevice *PZM_CreateDevice(int devindex)
{
	SDL_VideoDevice *device;

	/* Initialize all variables that we clean on shutdown */
	device = (SDL_VideoDevice *)SDL_malloc(sizeof(SDL_VideoDevice));
	if ( device ) {
		SDL_memset(device, 0, (sizeof *device));
		device->hidden = (struct SDL_PrivateVideoData *)
				SDL_malloc((sizeof *device->hidden));
	}
	if ( (device == NULL) || (device->hidden == NULL) ) {
		SDL_OutOfMemory();
		if ( device ) {
			SDL_free(device);
		}
		return(0);
	}
	SDL_memset(device->hidden, 0, (sizeof *device->hidden));

	/* Set the function pointers */
	device->VideoInit = PZM_VideoInit;
	device->ListModes = PZM_ListModes;
	device->SetVideoMode = PZM_SetVideoMode;
	device->SetColors = PZM_SetColors;
	device->UpdateRects = PZM_UpdateRects;
	device->VideoQuit = PZM_VideoQuit;
	device->InitOSKeymap = PZM_InitOSKeymap;
	device->PumpEvents = PZM_PumpEvents;

	device->free = PZM_DeleteDevice;

	return device;
}

VideoBootStrap PZM_bootstrap = {
	"prizm", "SDL Casio Prizm video driver",
	PZM_Available, PZM_CreateDevice
};

static int PZM_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
//	this->hidden->cx = (int)is_cx;

/*	if ( this->hidden->cx ) {
		vformat->BitsPerPixel = 16;
		vformat->Rmask = PZM_RMASK16;
		vformat->Gmask = PZM_GMASK16;
		vformat->Bmask = PZM_BMASK16;
	} else
		vformat->BitsPerPixel = 8;*/
	vformat->BitsPerPixel = 16;

	this->info.current_w = SCREEN_WIDTH;
	this->info.current_h = SCREEN_HEIGHT;

	return(0);
}

static SDL_Rect **PZM_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
	return (SDL_Rect **) -1;
}

static SDL_Surface *PZM_SetVideoMode(_THIS, SDL_Surface *current,
				     int width, int height, int bpp, Uint32 flags)
{
	Uint32 rmask = 0, gmask = 0, bmask = 0;

	width = ( width > SCREEN_WIDTH ) ? SCREEN_WIDTH : width;
	height = ( height > SCREEN_HEIGHT ) ? SCREEN_HEIGHT : height;
	bpp = ( bpp < 16 ) ? 8 : 16;

	PZM_DPRINT("Initializing display (%dx%dx%d)", width, height, bpp);

	this->info.current_w = width;
	this->info.current_h = height;

	if ( width < SCREEN_WIDTH || height < SCREEN_HEIGHT ) {
		int offset_x = (SCREEN_WIDTH - width) / 2;
		int offset_y = (SCREEN_HEIGHT - height) / 2;
		/*this->hidden->offset = this->hidden->cx
				     ? (int)PZM_PIXEL_ADDR(0, offset_x, offset_y, 2 * SCREEN_WIDTH, 2)
				     : (int)PZM_PIXEL_ADDR(0, offset_x / 2, offset_y, SCREEN_WIDTH / 2, 1);
		this->hidden->offset_x = offset_x;
		memset(VRAM, 0, SCREEN_BYTES_SIZE);
	} else
		this->hidden->offset = this->hidden->offset_x = 0;*/
		rmask = PZM_RMASK16;
		gmask = PZM_GMASK16;
		bmask = PZM_BMASK16;
	}

	if ( this->hidden->buffer ) {
		SDL_free( this->hidden->buffer );
	}

	this->hidden->buffer = SDL_malloc((bpp / 8) * width * height);
	if ( ! this->hidden->buffer ) {
		SDL_SetError("Couldn't allocate buffer for requested mode");
		return(NULL);
	}

	memset(this->hidden->buffer, 0, (bpp / 8) * width * height);

	/* Allocate the new pixel format for the screen */
	if ( ! SDL_ReallocFormat(current, bpp, rmask, gmask, bmask, 0) ) {
		SDL_free(this->hidden->buffer);
		this->hidden->buffer = NULL;
		SDL_SetError("Couldn't allocate new pixel format for requested mode");
		return(NULL);
	}

	/* Set up the new mode framebuffer */
	current->flags = flags;
	this->hidden->w = current->w = width;
	this->hidden->h = current->h = height;
	current->pitch = (bpp / 8) * current->w;
	current->pixels = this->hidden->buffer;

	PZM_DPRINT("Done (0x%p)", current);

	/* We're done */
	return(current);
}

#define PZM_DRAW_LOOP(code) do { \
	while ( rows--) { \
		int j = 0, k = 0; \
		code \
		src_addr += src_skip; \
		dst_addr += dst_skip; \
	} \
} while(0)

static void PZM_UpdateRects(_THIS, int numrects, SDL_Rect *rects)
{
	int src_skip = SDL_VideoSurface->pitch;
	int dst_skip = (2 * SCREEN_WIDTH);
	int i;

	for ( i = 0; i < numrects; ++i ) {
		Uint8 *src_addr, *dst_addr;
		int row_bytes, rows;
		int bpp = SDL_VideoSurface->format->BytesPerPixel;
		SDL_bool odd_left, odd_right;
		SDL_Rect *rect = &rects[i];
		if ( ! rect )
			continue;

		row_bytes = bpp * rect->w;
		rows = rect->h;
		//odd_left = (this->hidden->offset_x + rect->x) & 1;
		//odd_right = (this->hidden->offset_x + rect->x + rect->w) & 1;

		src_addr = PZM_SURF_PIXEL(SDL_VideoSurface, rect->x, rect->y);
		dst_addr = PZM_PIXEL_ADDR(VRAM, rect->x, rect->y, 2 * SCREEN_WIDTH, 2);
		//dst_addr += this->hidden->offset;

		if ( SDL_VideoSurface->format->BitsPerPixel == 16 ) {
			/* 16 bpp SW, 16 bpp HW */
			PZM_DRAW_LOOP(
				SDL_memcpy(dst_addr, src_addr, row_bytes);
			);
		} else {
			/* 8 bpp SW, 16 bpp HW */
			row_bytes *= 2;
			PZM_DRAW_LOOP(
				for ( j = 0, k = 0; j < row_bytes; j += 2, ++k )
					*(Uint16 *)(dst_addr + j) = pzm_palette[src_addr[k]];
			);
		}
	}
}

#define PZM_MAP_RGB_PALETTE(r, g, b)	(((r / 8) << 11) | ((g / 4) << 5) | (b / 8))

static int PZM_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
	int i;
	for ( i = firstcolor; i < ncolors; ++i )
		pzm_palette[i] = PZM_MAP_RGB_PALETTE(colors[i].r, colors[i].g, colors[i].b);
	return(1);
}

/* Note:  If we are terminated, this could be called in the middle of
   another SDL video routine -- notably UpdateRects.
*/
static void PZM_VideoQuit(_THIS)
{
	PZM_DPRINT("Closing video");
}
