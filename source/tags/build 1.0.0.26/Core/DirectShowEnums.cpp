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
#include "DirectShowEnums.h"
#include "DirectShowUtils.h"
#include "VfwErr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_COM		LogNormal|LogHighVolume
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_DirectShow"),LogVerbose,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE	LogVerbose
#endif

/////////////////////////////////////////////////////////////////////////////

#include "InterfaceMap.inl"

BEGIN_INTERFACE_MAP(CEnumPins, CCmdTarget)
	INTERFACE_PART(CEnumPins, __uuidof(IEnumPins), Enum)
END_INTERFACE_MAP()

BEGIN_INTERFACE_MAP(CEnumMediaTypes, CCmdTarget)
	INTERFACE_PART(CEnumMediaTypes, __uuidof(IEnumMediaTypes), Enum)
END_INTERFACE_MAP()

IMPLEMENT_IUNKNOWN(CEnumPins, Enum)
IMPLEMENT_IUNKNOWN(CEnumMediaTypes, Enum)

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CEnumPins, CCmdTarget)

CEnumPins::CEnumPins (const CEnumPins & pSource)
:	mInputPins (pSource.mInputPins),
	mOutputPins (pSource.mOutputPins),
	mCurrNdx (pSource.mCurrNdx),
	mOwnerRef (pSource.mOwnerRef)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CEnumPins::CEnumPins (%d) [%8.8X %8.8X] (Clone)"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	AfxOleLockApp ();

	EnableAggregation();
}

CEnumPins::CEnumPins (CDirectShowPins & pInputPins, CDirectShowPins & pOutputPins, LPUNKNOWN pOwnerRef)
:	mInputPins (pInputPins),
	mOutputPins (pOutputPins),
	mCurrNdx (-1),
	mOwnerRef (pOwnerRef)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CEnumPins::CEnumPins (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	AfxOleLockApp ();

	EnableAggregation();
}

CEnumPins::~CEnumPins ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CEnumPins::~CEnumPins (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	AfxOleUnlockApp ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CEnumPins::XEnum::Next (ULONG cPins, IPin **ppPins, ULONG *pcFetched)
{
	METHOD_PROLOGUE(CEnumPins, Enum)

	HRESULT	lResult = S_FALSE;
	long	lNdx = 0;
	ULONG	lCount = cPins;
	ULONG	lFetched = 0;
	GUID	lPinIID = __uuidof(IPin);

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
					(pThis->mInputPins.GetSize() > 0)
				&&	(pThis->mCurrNdx < pThis->mInputPins.GetUpperBound())
				)
			{
				pThis->mInputPins [++pThis->mCurrNdx]->InternalQueryInterface ((LPCVOID)&lPinIID, (LPVOID*)&lPin);
			}
			else
			if	(
					(pThis->mOutputPins.GetSize() > 0)
				&&	(pThis->mCurrNdx < pThis->mOutputPins.GetUpperBound() + pThis->mInputPins.GetSize())
				)
			{
				pThis->mOutputPins [++pThis->mCurrNdx - pThis->mInputPins.GetSize()]->InternalQueryInterface ((LPCVOID)&lPinIID, (LPVOID*)&lPin);
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

HRESULT STDMETHODCALLTYPE CEnumPins::XEnum::Skip (ULONG cPins)
{
	METHOD_PROLOGUE(CEnumPins, Enum)

	HRESULT	lResult = S_OK;

	pThis->mCurrNdx += (long)cPins;
	if	(pThis->mCurrNdx > pThis->mInputPins.GetSize() + pThis->mOutputPins.GetSize())
	{
		pThis->mCurrNdx = pThis->mInputPins.GetSize() + pThis->mOutputPins.GetSize();
		lResult = S_FALSE;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CEnumPins::XEnum::Reset ()
{
	METHOD_PROLOGUE(CEnumPins, Enum)

	HRESULT	lResult = S_OK;

	if	(pThis->mCurrNdx >= 0)
	{
		pThis->mCurrNdx = -1;
	}
	else
	{
		lResult = S_FALSE;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CEnumPins::XEnum::Clone (IEnumPins **ppEnum)
{
	METHOD_PROLOGUE(CEnumPins, Enum)

	HRESULT		lResult = S_OK;
	CEnumPins *	lClone;

	if	(!ppEnum)
	{
		lResult = E_POINTER;
	}
	else
	if	(lClone = new CEnumPins (*pThis))
	{
		(*ppEnum) = &lClone->m_xEnum;
	}
	else
	{
		lResult = E_OUTOFMEMORY;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CEnumMediaTypes, CCmdTarget)

CEnumMediaTypes::CEnumMediaTypes (const CEnumMediaTypes & pSource)
:	mMediaTypes (pSource.mMediaTypes),
	mCurrNdx (pSource.mCurrNdx),
	mOwnerRef (pSource.mOwnerRef)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CEnumMediaTypes::CEnumMediaTypes (%d) [%8.8X %8.8X] (Clone)"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	AfxOleLockApp ();

	EnableAggregation();
}

CEnumMediaTypes::CEnumMediaTypes (CMediaTypes & pMediaTypes, LPUNKNOWN pOwnerRef)
:	mMediaTypes (pMediaTypes),
	mCurrNdx (-1),
	mOwnerRef (pOwnerRef)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CEnumMediaTypes::CEnumMediaTypes (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	AfxOleLockApp ();

	EnableAggregation();
}

CEnumMediaTypes::~CEnumMediaTypes ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CEnumMediaTypes::~CEnumMediaTypes (%d) [%8.8X %8.8X]"), this, AfxGetModuleState()->m_nObjectCount, GetCurrentProcessId(), GetCurrentThreadId());
	}
#endif
	AfxOleUnlockApp ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CEnumMediaTypes::XEnum::Next (ULONG cMediaTypes, AM_MEDIA_TYPE **ppMediaTypes, ULONG *pcFetched)
{
	METHOD_PROLOGUE(CEnumMediaTypes, Enum)

	HRESULT	lResult = S_FALSE;
	long	lNdx = 0;
	ULONG	lCount = cMediaTypes;
	ULONG	lFetched = 0;
	GUID	lPinIID = __uuidof(IPin);

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
					(pThis->mMediaTypes.GetSize() > 0)
				&&	(pThis->mCurrNdx < pThis->mMediaTypes.GetUpperBound())
				)
			{
				lResult = MoDuplicateMediaType ((DMO_MEDIA_TYPE**)(ppMediaTypes+lNdx), (DMO_MEDIA_TYPE*)pThis->mMediaTypes [++pThis->mCurrNdx]);
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

HRESULT STDMETHODCALLTYPE CEnumMediaTypes::XEnum::Skip (ULONG cMediaTypes)
{
	METHOD_PROLOGUE(CEnumMediaTypes, Enum)

	HRESULT	lResult = S_OK;

	pThis->mCurrNdx += (long)cMediaTypes;
	if	(pThis->mCurrNdx > pThis->mMediaTypes.GetSize())
	{
		pThis->mCurrNdx = pThis->mMediaTypes.GetSize();
		lResult = S_FALSE;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CEnumMediaTypes::XEnum::Reset ()
{
	METHOD_PROLOGUE(CEnumMediaTypes, Enum)

	HRESULT	lResult = S_OK;

	if	(pThis->mCurrNdx >= 0)
	{
		pThis->mCurrNdx = -1;
	}
	else
	{
		lResult = S_FALSE;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CEnumMediaTypes::XEnum::Clone (IEnumMediaTypes **ppEnum)
{
	METHOD_PROLOGUE(CEnumMediaTypes, Enum)

	HRESULT		lResult = S_OK;
	CEnumMediaTypes *	lClone;

	if	(!ppEnum)
	{
		lResult = E_POINTER;
	}
	else
	if	(lClone = new CEnumMediaTypes (*pThis))
	{
		(*ppEnum) = &lClone->m_xEnum;
	}
	else
	{
		lResult = E_OUTOFMEMORY;
	}
	return lResult;
}
