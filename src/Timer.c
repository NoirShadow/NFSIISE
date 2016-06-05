/*
	This is a port of the Windows game Need For Speed™ II Special Edition.
	Copyright (C) 2014-2016  Błażej Szczygieł

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Wrapper.h"

#include <SDL2/SDL_timer.h>

static SDL_TimerID timerID;

typedef void Event;
STDCALL BOOL SetEvent_wrap(Event *event);
extern Event *dword_4DDA70, *dword_5637CC, *dword_5637D8;
extern uint32_t dword_4DB1B0, dword_5637A0;

static uint32_t timerCallback(uint32_t interval, void *param)
{
	static uint32_t dword_5637A8, dword_5637C8;
	int32_t delay;

	if (dword_5637A0 == 655360)
		delay = 1;
	else
	{
		uint32_t val = (dword_5637C8 + dword_5637A0) >> 16 /*/ 61440*/;
		dword_5637C8 = (uint16_t)(dword_5637C8 + dword_5637A0);
		dword_5637A8 += val;

		delay = dword_5637A8 - SDL_GetTicks() - 1;
		if (delay < 1)
		{
			if (delay < -1000)
				dword_5637A8 = SDL_GetTicks() + 1;
			delay = 1;
		}
	}

	if (dword_5637CC)
		SetEvent_wrap(dword_5637CC);
	if (dword_5637D8 && !dword_4DB1B0)
		SetEvent_wrap(dword_5637D8);
	if (dword_4DDA70)
		SetEvent_wrap(dword_4DDA70);

	return delay;
}

REALIGN void startTimer()
{
	timerID = SDL_AddTimer(1, timerCallback, NULL);
}
REALIGN void stopTimer()
{
	SDL_RemoveTimer(timerID);
}
