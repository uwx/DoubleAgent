/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is a utility used by Double Agent but not specific to
	Double Agent.  However, it is included as part of the Double Agent
	source code under the following conditions:

    This is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#ifndef	_ELAPSED_H
#define	_ELAPSED_H
////////////////////////////////////////////////////////////////////////

static inline long ElapsedTicks (DWORD pStartTicks)
{
	DWORD	lTicks = GetTickCount ();
	return (lTicks >= pStartTicks) ? lTicks - pStartTicks : (ULONG_MAX - pStartTicks) + lTicks;
}

static inline long ElapsedTicks (DWORD pStartTicks, DWORD pCurrentTicks)
{
	return (pCurrentTicks >= pStartTicks) ? pCurrentTicks - pStartTicks : (ULONG_MAX - pStartTicks) + pCurrentTicks;
}

static inline bool TicksElapsed (DWORD & pStartTicks, DWORD pTickCount)
{
	DWORD	lTicks = GetTickCount ();

	if	((long) ((lTicks >= pStartTicks) ? lTicks - pStartTicks : (ULONG_MAX - pStartTicks) + lTicks) >= (long) pTickCount)
	{
		pStartTicks = lTicks;
		return true;
	}

	return false;
}

static inline bool TicksElapsed (DWORD & pStartTicks, DWORD pCurrentTicks, DWORD pTickCount)
{
	if	((long) ((pCurrentTicks >= pStartTicks) ? pCurrentTicks - pStartTicks : (ULONG_MAX - pStartTicks) + pCurrentTicks) >= (long) pTickCount)
	{
		pStartTicks = pCurrentTicks;
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////
#endif // _ELAPSED_H
