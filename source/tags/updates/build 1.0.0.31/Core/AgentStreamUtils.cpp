/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

CAgentStreamUtils::CAgentStreamUtils ()
:	mAgentFile (NULL)
{
}

CAgentStreamUtils::~CAgentStreamUtils()
{
	CSingleLock	lLock (&mUtilCritSec, TRUE);

	try
	{
		SafeFreeSafePtr (mStreamInfo);
	}
	catch AnyExceptionSilent
}

/////////////////////////////////////////////////////////////////////////////

CAgentFile * CAgentStreamUtils::GetAgentFile () const
{
	return mAgentFile;
}

void CAgentStreamUtils::SetAgentFile (CAgentFile * pAgentFile, CObject * pClient)
{
	CSingleLock	lLock (&mUtilCritSec, TRUE);

	try
	{
		if	(
				(mAgentFile)
			&&	(pClient)
			)
		{
			try
			{
				TheCoreApp->RemoveFileClient (mAgentFile, pClient);
			}
			catch AnyExceptionSilent
		}

		mAgentFile = pAgentFile;

		if	(
				(mAgentFile)
			&&	(pClient)
			)
		{
			TheCoreApp->AddFileClient (mAgentFile, pClient);
		}
	}
	catch AnyExceptionSilent
}

/////////////////////////////////////////////////////////////////////////////

const CStringMap <CStringArray> & CAgentStreamUtils::GetFileStates (UINT pLogLevel) const
{
	CAgentFile *	lAgentFile;

	if	(lAgentFile = GetAgentFile ())
	{
		if	(lAgentFile->GetStates().GetSize() <= 0)
		{
			lAgentFile->ReadStates (pLogLevel);
		}
		return lAgentFile->GetStates ();
	}
	return * (const CStringMap <CStringArray> *) NULL;
}

const CAgentFileGestures & CAgentStreamUtils::GetFileGestures (UINT pLogLevel) const
{
	CAgentFile *	lAgentFile;

	if	(lAgentFile = GetAgentFile ())
	{
		if	(lAgentFile->GetGestures().IsEmpty())
		{
			lAgentFile->ReadGestures (pLogLevel);
		}
		return lAgentFile->GetGestures ();
	}
	return * (const CAgentFileGestures *) NULL;
}

bool CAgentStreamUtils::GetFileImages (UINT pLogLevel) const
{
	CAgentFile *	lAgentFile;

	if	(lAgentFile = GetAgentFile ())
	{
		if	(lAgentFile->GetImageCount() <= 0)
		{
			lAgentFile->ReadImageIndex (pLogLevel);
		}
		return (lAgentFile->GetImageCount() > 0);
	}
	return false;
}

bool CAgentStreamUtils::GetFileSounds (UINT pLogLevel) const
{
	CAgentFile *	lAgentFile;

	if	(lAgentFile = GetAgentFile ())
	{
		if	(lAgentFile->GetSoundCount() <= 0)
		{
			lAgentFile->ReadSoundIndex (pLogLevel);
		}
		return (lAgentFile->GetSoundCount() > 0);
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentStreamUtils::SetAgentStreamInfo (_IAgentStreamInfo * pStreamInfo)
{
	CSingleLock	lLock (&mUtilCritSec, TRUE);

	try
	{
		mStreamInfo = pStreamInfo;
	}
	catch AnyExceptionSilent
}

void CAgentStreamUtils::SetAgentStreamInfo (CAgentStreamInfo * pStreamInfo)
{
	CSingleLock	lLock (&mUtilCritSec, TRUE);

	try
	{
		mStreamInfo = pStreamInfo;
	}
	catch AnyExceptionSilent
}

CAgentStreamInfo * CAgentStreamUtils::GetAgentStreamInfo () const
{
	CAgentStreamInfo *	lRet = NULL;
	CSingleLock			lLock (&mUtilCritSec, TRUE);

	try
	{
		if	(mStreamInfo != NULL)
		{
			lRet = (CAgentStreamInfo *) mStreamInfo.GetInterfacePtr();
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
	CAgentFile *	lAgentFile;

	if	(lAgentFile = GetAgentFile ())
	{
		if	(lAgentFile->IsAcsFile ())
		{
			const CAgentFileGestures &	lGestures = GetFileGestures ();
			int							lAnimationNdx;
			const CAgentFileAnimation *	lAnimation;

			for	(lAnimationNdx = 0; lAnimationNdx < lGestures.GetSize(); lAnimationNdx++)
			{
				if	(lAnimation = lGestures [lAnimationNdx])
				{
					lRet += (long)(short)lAnimation->mFrameCount;
				}
			}
		}
		else
		if	(lAgentFile->IsAcfFile ())
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
	CAgentFile *	lAgentFile;

	if	(lAgentFile = GetAgentFile ())
	{
		if	(lAgentFile->IsAcsFile ())
		{
			const CAgentFileGestures &	lGestures = GetFileGestures ();
			int							lAnimationNdx;
			const CAgentFileAnimation *	lAnimation;
			int							lFrameNdx;

			for	(lAnimationNdx = 0; lAnimationNdx < lGestures.GetSize(); lAnimationNdx++)
			{
				if	(lAnimation = lGestures [lAnimationNdx])
				{
					for	(lFrameNdx = 0; lFrameNdx < (long)(short)lAnimation->mFrameCount; lFrameNdx++)
					{
						lRet += (long)(short)lAnimation->mFrames [lFrameNdx].mDuration;
					}
				}
			}
		}
		else
		if	(lAgentFile->IsAcfFile ())
		{
			//
			//	For an ACS file, we don't support playing the entire file, and we'll assume the maximum animation duration is 1 minute.
			//
			lRet = 60000;
#ifdef	_DEBUG
			LogMessage (LogDebug, _T("Assumed duration [%u] for [%ls]"), lRet, (BSTR)lAgentFile->GetPath());
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

		for	(int lNdx = (int)pTransparentNdx+1; lNdx < (int)pBitmapInfo->bmiHeader.biClrUsed; lNdx++)
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
