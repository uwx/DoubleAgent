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
#include "DirectShowEnums.h"
#include "DirectShowUtils.h"
#include "VfwErr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	_LOG_INSTANCE	(GetProfileDebugInt(_T("LogInstance_DirectShowEnum"),LogVerbose,true)&0xFFFF)
#define	_LOG_RESULTS	(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE	LogVerbose
#endif

/////////////////////////////////////////////////////////////////////////////

CEnumPins::CEnumPins ()
:	mInputPins (NULL),
	mOutputPins (NULL),
	mCurrNdx (-1)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CEnumPins::CEnumPins (%d) [%8.8X %8.8X]"), this, m_dwRef, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

CEnumPins::~CEnumPins ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CEnumPins::~CEnumPins (%d) [%8.8X %8.8X]"), this, m_dwRef, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CEnumPins::Initialize (CDirectShowPins & pInputPins, CDirectShowPins & pOutputPins, LPUNKNOWN pOwnerRef)
{
	mInputPins = &pInputPins;
	mOutputPins = &pOutputPins;
	mOwnerRef = pOwnerRef;

#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CEnumPins::Initialize (%d) [%8.8X %8.8X]"), this, m_dwRef, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

CEnumPins & CEnumPins::operator= (const CEnumPins & pSource)
{
	mInputPins = pSource.mInputPins;
	mOutputPins = pSource.mOutputPins;
	mCurrNdx = pSource.mCurrNdx;
	mOwnerRef = pSource.mOwnerRef;

#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CEnumPins::operator= (%d) [%8.8X %8.8X]"), this, m_dwRef, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CEnumPins::Next (ULONG cPins, IPin **ppPins, ULONG *pcFetched)
{
	HRESULT	lResult = S_FALSE;
	long	lNdx = 0;
	ULONG	lCount = cPins;
	ULONG	lFetched = 0;

	if	(!ppPins)
	{
		lResult = E_POINTER;
	}
	else
	{
		while (lCount > 0)
		{
			IPinPtr	lPin;

			if	(
					(mInputPins->GetSize() > 0)
				&&	(mCurrNdx < mInputPins->GetUpperBound())
				)
			{
				lPin = (*mInputPins) [++mCurrNdx];
			}
			else
			if	(
					(mOutputPins->GetSize() > 0)
				&&	(mCurrNdx < mOutputPins->GetUpperBound() + mInputPins->GetSize())
				)
			{
				lPin = (*mOutputPins) [++mCurrNdx - mInputPins->GetSize()];
			}

			if	(lPin == NULL)
			{
				lResult = S_FALSE;
			}
			else
			{
				lFetched++;
				lResult = S_OK;
			}
			ppPins [lNdx] = lPin.Detach ();
			lNdx++;
			lCount--;
		}
	}
	if	(pcFetched)
	{
		(*pcFetched) = lFetched;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CEnumPins::Skip (ULONG cPins)
{
	HRESULT	lResult = S_OK;

	mCurrNdx += (long)cPins;
	if	(mCurrNdx > mInputPins->GetSize() + mOutputPins->GetSize())
	{
		mCurrNdx = mInputPins->GetSize() + mOutputPins->GetSize();
		lResult = S_FALSE;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CEnumPins::Reset ()
{
	HRESULT	lResult = S_OK;

	if	(mCurrNdx >= 0)
	{
		mCurrNdx = -1;
	}
	else
	{
		lResult = S_FALSE;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CEnumPins::Clone (IEnumPins **ppEnum)
{
	HRESULT						lResult = S_OK;
	CComObject <CEnumPins> *	lClone = NULL;
	IEnumPinsPtr				lInterface;

	if	(!ppEnum)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ppEnum) = NULL;

		if	(SUCCEEDED (lResult = CComObject <CEnumPins>::CreateInstance (&lClone)))
		{
			lClone->CEnumPins::operator= (*this);
			lInterface = lClone;
			(*ppEnum) = lInterface.Detach ();
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CEnumMediaTypes::CEnumMediaTypes ()
:	mMediaTypes (NULL),
	mCurrNdx (-1)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CEnumMediaTypes::CEnumMediaTypes (%d) [%8.8X %8.8X]"), this, m_dwRef, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

CEnumMediaTypes::~CEnumMediaTypes ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CEnumMediaTypes::~CEnumMediaTypes (%d) [%8.8X %8.8X]"), this, m_dwRef, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CEnumMediaTypes::Initialize (CMediaTypes & pMediaTypes, LPUNKNOWN pOwnerRef)
{
	mMediaTypes = &pMediaTypes;
	mOwnerRef = pOwnerRef;

#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CEnumMediaTypes::Initialize (%d) [%8.8X %8.8X]"), this, m_dwRef, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
}

CEnumMediaTypes & CEnumMediaTypes::operator= (const CEnumMediaTypes & pSource)
{
	mMediaTypes = pSource.mMediaTypes;
	mCurrNdx = pSource.mCurrNdx;
	mOwnerRef = pSource.mOwnerRef;

#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CEnumMediaTypes::operator= (%d) [%8.8X %8.8X]"), this, m_dwRef, _AtlModule.GetLockCount(), GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CEnumMediaTypes::Next (ULONG cMediaTypes, AM_MEDIA_TYPE **ppMediaTypes, ULONG *pcFetched)
{
	HRESULT	lResult = S_FALSE;
	long	lNdx = 0;
	ULONG	lCount = cMediaTypes;
	ULONG	lFetched = 0;

	if	(!ppMediaTypes)
	{
		lResult = E_POINTER;
	}
	else
	{
		while (lCount > 0)
		{
			ppMediaTypes [lNdx] = NULL;

			if	(
					(mMediaTypes->GetSize() > 0)
				&&	(mCurrNdx < mMediaTypes->GetUpperBound())
				)
			{
				lResult = MoDuplicateMediaType ((DMO_MEDIA_TYPE**)(ppMediaTypes+lNdx), (DMO_MEDIA_TYPE*)(*mMediaTypes) [++mCurrNdx]);
				if	(SUCCEEDED (lResult))
				{
					lFetched++;
					lResult = S_OK;
				}
				else
				{
					break;
				}
			}
			else
			{
				lResult = S_FALSE;
			}
			lNdx++;
			lCount--;
		}
	}
	if	(pcFetched)
	{
		(*pcFetched) = lFetched;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CEnumMediaTypes::Skip (ULONG cMediaTypes)
{
	HRESULT	lResult = S_OK;

	mCurrNdx += (long)cMediaTypes;
	if	(mCurrNdx > mMediaTypes->GetSize())
	{
		mCurrNdx = mMediaTypes->GetSize();
		lResult = S_FALSE;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CEnumMediaTypes::Reset ()
{
	HRESULT	lResult = S_OK;

	if	(mCurrNdx >= 0)
	{
		mCurrNdx = -1;
	}
	else
	{
		lResult = S_FALSE;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CEnumMediaTypes::Clone (IEnumMediaTypes **ppEnum)
{
	HRESULT							lResult = S_OK;
	CComObject <CEnumMediaTypes> *	lClone = NULL;
	IEnumMediaTypesPtr				lInterface;

	if	(!ppEnum)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ppEnum) = NULL;

		if	(SUCCEEDED (lResult = CComObject <CEnumMediaTypes>::CreateInstance (&lClone)))
		{
			lClone->CEnumMediaTypes::operator= (*this);
			lInterface = lClone;
			(*ppEnum) = lInterface.Detach ();
		}
	}
	return lResult;
}
