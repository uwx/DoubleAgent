/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2014 Cinnamon Software Inc.
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
#pragma once
/////////////////////////////////////////////////////////////////////////////
#pragma managed(push,off)
////////////////////////////////////////////////////////////////////////
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
////////////////////////////////////////////////////////////////////////
#ifndef	_DEBUG_GDIPLUS_TOKEN
#define	_DEBUG_GDIPLUS_TOKEN	LogVerbose|LogTime
#endif
////////////////////////////////////////////////////////////////////////

class CUseGdiplus
{
public:
	CUseGdiplus (bool pStartup = true)
	:	mToken (0)
	{
		if	(pStartup)
		{
			Startup ();
		}
	}

	~CUseGdiplus ()
	{
		Shutdown ();
	}

	Gdiplus::Status Startup ()
	{
		Gdiplus::Status	Status = Gdiplus::GenericError;
		if	(!mToken)
		{
			tS <Gdiplus::GdiplusStartupInput>	lInput;
			tS <Gdiplus::GdiplusStartupOutput>	lOutput;

			try
			{
				lInput.GdiplusVersion = 1;
				Status = Gdiplus::GdiplusStartup (&mToken, &lInput, &lOutput);
			}
			catch (...)
			{}

#ifdef	_DEBUG_GDIPLUS_TOKEN
			if	(LogIsActive (_DEBUG_GDIPLUS_TOKEN))
			{
				LogMessage (_DEBUG_GDIPLUS_TOKEN, _T("GdiplusStartup [%p] Status [%u]"), mToken, Status);
			}
#endif
		}
		return Status;
	}

	bool Shutdown ()
	{
		if	(mToken)
		{
			ULONG_PTR	lToken = mToken;

			mToken = NULL;
#ifdef	_DEBUG_GDIPLUS_TOKEN
			if	(LogIsActive (_DEBUG_GDIPLUS_TOKEN))
			{
				LogMessage (_DEBUG_GDIPLUS_TOKEN, _T("GdiplusShutdown [%p]"), lToken);
			}
#endif
			try
			{
				Gdiplus::GdiplusShutdown (lToken);
			}
			catch (...)
			{}

#ifdef	_DEBUG_GDIPLUS_TOKEN
			if	(LogIsActive (_DEBUG_GDIPLUS_TOKEN))
			{
				LogMessage (_DEBUG_GDIPLUS_TOKEN, _T("GdiplusShutdown [%p] done"), lToken);
			}
#endif
			return true;
		}
		return false;
	}

private:
	ULONG_PTR	mToken;
};

/////////////////////////////////////////////////////////////////////////////
#pragma managed(pop)
////////////////////////////////////////////////////////////////////////
