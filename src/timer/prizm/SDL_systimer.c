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

#ifdef SDL_TIMER_PRIZM

#include <SYSTEM_syscalls.h>
#include <RTC_syscalls.h>

#include "SDL_timer.h"
#include "../SDL_timer_c.h"

Uint32 start = 0;
volatile unsigned char *R64CNT = (unsigned char*)0xA413FEC0;
volatile unsigned char *RSECCNT = (unsigned char*)0xA413FEC2;
volatile unsigned char *RMINCNT = (unsigned char*)0xA413FEC4;
volatile unsigned char *RHRCNT = (unsigned char*)0xA413FEC6;
volatile unsigned char *RWKCNT = (unsigned char*)0xA413FEC8;
volatile unsigned char *RDAYCNT = (unsigned char*)0xA413FECA;
volatile unsigned char *RMONCNT = (unsigned char*)0xA413FECC;
volatile unsigned short *RYRCNT = (unsigned short*)0xA413FECE;
volatile unsigned char *RCR1 = (unsigned char*)0xA413FEDC;
volatile unsigned char *RCR2 = (unsigned char*)0xA413FEDE;

void SDL_StartTicks(void)
{
/*	unsigned char m8 = '0' + (*R64CNT & 0b111);
	unsigned char s1 = '0' + (*RSECCNT & 0b1111);
    unsigned char s2 = '0' + ((*RSECCNT >> 4) & 0b111);
    unsigned char m1 = '0' + (*RMINCNT & 0b1111);
    unsigned char m2 = '0' + ((*RMINCNT >> 4) & 0b111);
    unsigned char h1 = '0' + (*RHRCNT & 0b1111);
    unsigned char h2 = '0' + ((*RHRCNT >> 4) & 0b11);
    start = m8*8+s1*1000+s2*10000+m1*60000+m2*600000+h1*3600000+h2*36000000*/
    start=RTC_GetTicks();
}

Uint32 SDL_GetTicks (void)
{
	/*Uint32 diff;
	unsigned char m8 = '0' + (*R64CNT & 0b111);
	unsigned char s1 = '0' + (*RSECCNT & 0b1111);
    unsigned char s2 = '0' + ((*RSECCNT >> 4) & 0b111);
    unsigned char m1 = '0' + (*RMINCNT & 0b1111);
    unsigned char m2 = '0' + ((*RMINCNT >> 4) & 0b111);
    unsigned char h1 = '0' + (*RHRCNT & 0b1111);
    unsigned char h2 = '0' + ((*RHRCNT >> 4) & 0b11);
    diff = (m8*8+s1*1000+s2*10000+m1*6000+m2*60000+h1*3600000+h2*36000000) - start*/
    return 7.8125*(RTC_GetTicks()-start);
}

void SDL_Delay (Uint32 ms)
{
	int times = ms/1000, i;
	OS_InnerWait_ms(ms%1000);
	for( i=0; i<times; i++)
	{
		OS_InnerWait_ms(990);//reduce it a little for possible call delay
	}
}

static int RunTimer(void *unused)
{
	SDL_SetError("Timer not yet implemented on the Prizm");
	return(-1);
}

int SDL_SYS_TimerInit(void)
{
	SDL_SetError("Timer not yet implemented on the Prizm");
	return(-1);
}

void SDL_SYS_TimerQuit(void)
{
	return;
}

int SDL_SYS_StartTimer(void)
{
	SDL_SetError("Timers not implemented on the Prizm");
	return(-1);
}

void SDL_SYS_StopTimer(void)
{
	return;
}

#endif /* SDL_TIMER_PRIZM */
