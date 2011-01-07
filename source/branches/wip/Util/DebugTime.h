/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2011 Cinnamon Software Inc.
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
#ifndef	_DEBUGTIME_H_
#define _DEBUGTIME_H_
#pragma once
//////////////////////////////////////////////////////////////////////
#define	DebugTimeDefine \
	DWORD lStartTicks; \
	DWORD lStopTicks = 0; \
	LARGE_INTEGER lPerfFreq = {0, 1}; \
	LARGE_INTEGER lStartTimeLarge = {0, 0}; \
	LARGE_INTEGER lStopTimeLarge = {0, 0}; \
	BOOL lStartTimeLargeValid; \
	BOOL lStopTimeLargeValid = FALSE; \
	QueryPerformanceFrequency (&lPerfFreq); \

#define	DebugTimeStart \
	DebugTimeDefine \
	lStartTicks = GetTickCount (); \
	lStartTimeLargeValid = QueryPerformanceCounter (&lStartTimeLarge); \

#define	DebugTimeRestart \
	lStartTimeLargeValid = QueryPerformanceCounter (&lStartTimeLarge); \
	lStartTicks = GetTickCount (); \

#define DebugTimeStop \
	lStopTimeLargeValid = QueryPerformanceCounter (&lStopTimeLarge); \
	lStopTicks = GetTickCount (); \

#define DebugTimeElapsed ((lStartTimeLargeValid && lStopTimeLargeValid) ? ((double) (lStopTimeLarge.QuadPart - lStartTimeLarge.QuadPart) / (double) lPerfFreq.QuadPart) : (((lStopTicks > lStartTicks) ? (double) (lStopTicks - lStartTicks) : (double) (lStartTicks - lStopTicks)) / 1000.0))

#define DebugTimeLog {DebugTimeStop LogMessage (LogIfActive|LogNoPrefix|LogHighVolume, _T("%f at %d in %hs"), DebugTimeElapsed, __LINE__, __FILE__); DebugTimeRestart}

//////////////////////////////////////////////////////////////////////

#define DebugTimePush(targ) \
	{ \
		if	(lStartTimeLargeValid) \
		{ \
			targ = lStartTimeLarge.QuadPart; \
		} \
		else \
		{ \
			LARGE_INTEGER lTargetTime; \
			lTargetTime.LowPart = lStartTicks; \
			lTargetTime.HighPart = -1; \
			targ = lTargetTime.QuadPart; \
		} \
	} \

#define DebugTimePop(targ) \
	{ \
		LARGE_INTEGER lTargetTime; \
		lTargetTime.QuadPart = targ; \
		if	(lTargetTime.HighPart == -1) \
		{ \
			lStartTicks = lTargetTime.LowPart; \
			lStartTimeLargeValid = FALSE; \
		} \
		else \
		{ \
			lStartTimeLarge.QuadPart = lTargetTime.QuadPart; \
			lStartTimeLargeValid = TRUE; \
		} \
	} \

//////////////////////////////////////////////////////////////////////
#endif // _DEBUGTIME_H_
