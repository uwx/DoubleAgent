/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DaCore.h"
#include "AgentStreamUtils.h"
#include "AgentStreamInfo.h"
#include "DebugStr.h"

/////////////////////////////////////////////////////////////////////////////

CAgentStreamUtils::CAgentStreamUtils ()
:	mAgentFile (NULL)
{
}

CAgentStreamUtils::~CAgentStreamUtils()
{
	CLockCS	lLock (mUtilCritSec);

	try
	{
		SafeFreeSafePtr (mStreamInfo);
	}
	catch AnyExceptionSilent
}

/////////////////////////////////////////////////////////////////////////////

CAgentFile* CAgentStreamUtils::GetAgentFile () const
{
	return mAgentFile;
}

void CAgentStreamUtils::SetAgentFile (CAgentFile* pAgentFile, CAgentFileClient* pClient)
{
	CLockCS	lLock (mUtilCritSec);

	try
	{
		if	(
				(mAgentFile)
			&&	(pClient)
			)
		{
			try
			{
				_AtlModule.RemoveFileClient (mAgentFile, pClient);
			}
			catch AnyExceptionSilent
		}

		mAgentFile = pAgentFile;

		if	(
				(mAgentFile)
			&&	(pClient)
			)
		{
			_AtlModule.AddFileClient (mAgentFile, pClient);
		}
	}
	catch AnyExceptionSilent
}

/////////////////////////////////////////////////////////////////////////////

const CAgentFileStates& CAgentStreamUtils::GetFileStates () const
{
	CAgentFile*	lAgentFile;

	if	(lAgentFile = GetAgentFile ())
	{
		return lAgentFile->States;
	}
	return * (const CAgentFileStates *) NULL;
}

const CAgentFileGestures& CAgentStreamUtils::GetFileGestures () const
{
	CAgentFile*	lAgentFile;

	if	(lAgentFile = GetAgentFile ())
	{
		return lAgentFile->Gestures;
	}
	return * (const CAgentFileGestures *) NULL;
}

bool CAgentStreamUtils::GetFileImages () const
{
	CAgentFile*	lAgentFile;

	if	(lAgentFile = GetAgentFile ())
	{
		return (lAgentFile->GetImageCount() > 0);
	}
	return false;
}

bool CAgentStreamUtils::GetFileSounds () const
{
	CAgentFile*	lAgentFile;

	if	(lAgentFile = GetAgentFile ())
	{
		return (lAgentFile->GetSoundCount() > 0);
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentStreamUtils::SetAgentStreamInfo (_IAgentStreamInfo* pStreamInfo)
{
	CLockCS	lLock (mUtilCritSec);

	try
	{
		mStreamInfo = pStreamInfo;
	}
	catch AnyExceptionSilent
}

void CAgentStreamUtils::SetAgentStreamInfo (CAgentStreamInfo* pStreamInfo)
{
	CLockCS	lLock (mUtilCritSec);

	try
	{
		mStreamInfo = pStreamInfo;
	}
	catch AnyExceptionSilent
}

CAgentStreamInfo* CAgentStreamUtils::GetAgentStreamInfo () const
{
	CAgentStreamInfo*	lRet = NULL;
	CLockCS				lLock (mUtilCritSec);

	try
	{
		if	(mStreamInfo != NULL)
		{
			lRet = (CAgentStreamInfo*) mStreamInfo.GetInterfacePtr();
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CAgentStreamUtils::CalcFileFrameCount () const
{
	long			lRet = 0;
	CAgentFile*	lAgentFile;

	if	(lAgentFile = GetAgentFile ())
	{
		if	(lAgentFile->IsAcsFile)
		{
			const CAgentFileGestures&	lGestures = GetFileGestures ();
			INT_PTR						lAnimationNdx;
			const CAgentFileAnimation*	lAnimation;

			for	(lAnimationNdx = 0; lAnimationNdx < (INT_PTR)lGestures.mAnimations.GetCount(); lAnimationNdx++)
			{
				if	(lAnimation = lGestures.mAnimations [lAnimationNdx])
				{
					lRet += (long)(short)lAnimation->FrameCount;
				}
			}
		}
		else
		if	(lAgentFile->IsAcfFile)
		{
			//
			//	For an ACS file, we don't support playing the entire file, and we'll assume a maximum animation frame count.
			//
			lRet = 500;
		}
	}
	return lRet;
}

long CAgentStreamUtils::CalcFileDuration () const
{
	long			lRet = 0;
	CAgentFile*	lAgentFile;

	if	(lAgentFile = GetAgentFile ())
	{
		if	(lAgentFile->IsAcsFile)
		{
			const CAgentFileGestures&	lGestures = GetFileGestures ();
			INT_PTR						lAnimationNdx;
			const CAgentFileAnimation*	lAnimation;
			INT_PTR						lFrameNdx;

			for	(lAnimationNdx = 0; lAnimationNdx < (INT_PTR)lGestures.mAnimations.GetCount(); lAnimationNdx++)
			{
				if	(lAnimation = lGestures.mAnimations [lAnimationNdx])
				{
					for	(lFrameNdx = 0; lFrameNdx < (long)(short)lAnimation->FrameCount; lFrameNdx++)
					{
						lRet += (long)(short)lAnimation->Frames [lFrameNdx].Duration;
					}
				}
			}
		}
		else
		if	(lAgentFile->IsAcfFile)
		{
			//
			//	For an ACS file, we don't support playing the entire file, and we'll assume the maximum animation duration is 1 minute.
			//
			lRet = 60000;
#ifdef	_DEBUG
			LogMessage (LogDebug, _T("Assumed duration [%u] for [%ls]"), lRet, (BSTR)lAgentFile->Path);
#endif
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentStreamUtils::SetPaletteBkColor (LPBITMAPINFO pBitmapInfo, BYTE pTransparentNdx, COLORREF pBkColor)
{
	if	(
			(pBitmapInfo)
		&&	(pTransparentNdx < pBitmapInfo->bmiHeader.biClrUsed)
		)
	{
		tS <RGBQUAD>	lOldBkColor;
		tS <RGBQUAD>	lNewBkColor;

		lOldBkColor = pBitmapInfo->bmiColors [pTransparentNdx];
		lNewBkColor.rgbRed = GetRValue(pBkColor);
		lNewBkColor.rgbGreen = GetGValue(pBkColor);
		lNewBkColor.rgbBlue = GetBValue(pBkColor);
		lNewBkColor.rgbReserved = 0xFF;
		pBitmapInfo->bmiColors [pTransparentNdx] = lNewBkColor;

		for	(INT_PTR lNdx = (INT_PTR)pTransparentNdx+1; lNdx < (INT_PTR)pBitmapInfo->bmiHeader.biClrUsed; lNdx++)
		{
			if	(
					(pBitmapInfo->bmiColors [lNdx].rgbRed == lOldBkColor.rgbRed)
				&&	(pBitmapInfo->bmiColors [lNdx].rgbGreen == lOldBkColor.rgbGreen)
				&&	(pBitmapInfo->bmiColors [lNdx].rgbBlue == lOldBkColor.rgbBlue)
				)
			{
				pBitmapInfo->bmiColors [lNdx] = lNewBkColor;
			}
		}
	}
}
