/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent Server.

    The Double Agent Server is free software:
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser Public License for more details.

    You should have received a copy of the GNU Lesser Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DaServer.h"
#include "DaAnimationNames.h"
#ifdef	_DEBUG
#include "Registry.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogVerbose,true)&0xFFFF)
#endif
#include "InterfaceMap.inl"

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDaAnimationNames, CEnumVariant)

CDaAnimationNames::CDaAnimationNames (const CDaAnimationNames & pSource)
:	CEnumVariant (pSource)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAnimationNames::CDaAnimationNames (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp();

	mAnimationNames.Copy (pSource.mAnimationNames);
}

CDaAnimationNames::CDaAnimationNames (CAgentFile & pAgentFile)
{
	const CAgentFileGestures &	lGestures = pAgentFile.GetGestures ();
	int							lNdx;

#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAnimationNames::CDaAnimationNames (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp();

	if	(lGestures.GetSize() <= 0)
	{
		pAgentFile.ReadGestures ();
	}
	for	(lNdx = 0; lNdx <= lGestures.GetUpperBound(); lNdx++)
	{
		mAnimationNames.Add (lGestures.KeyAt (lNdx));
	}
}

CDaAnimationNames::~CDaAnimationNames ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAnimationNames::~CDaAnimationNames (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	Terminate (true);
	AfxOleUnlockApp();
}

void CDaAnimationNames::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
		if	(
				(pFinal)
			&&	(m_dwRef > 0)
			)
		{
			if	(!pAbandonned)
			{
				try
				{
					ExternalDisconnect ();
				}
				catch AnyExceptionDebug
			}
			m_dwRef = 0;
		}
	}
}

void CDaAnimationNames::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAnimationNames::OnFinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
	CEnumVariant::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CDaAnimationNames::GetItemCount ()
{
	return (long)mAnimationNames.GetSize();
}

void CDaAnimationNames::PutItem (long pItemNdx, VARIANT & pItem)
{
	V_VT (&pItem) = VT_BSTR;
	V_BSTR (&pItem) = _bstr_t (mAnimationNames [pItemNdx]).Detach();
}

CEnumVariant * CDaAnimationNames::Clone ()
{
	return new CDaAnimationNames (*this);
}
