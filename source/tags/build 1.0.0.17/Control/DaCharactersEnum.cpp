/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent ActiveX Control.

    The Double Agent ActiveX Control is free software:
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
#include "DaControl.h"
#include "DaCharactersEnum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Enum"),LogVerbose,true)&0xFFFF)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE		LogVerbose
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDaCharactersEnum, CEnumVariant)

CDaCharactersEnum::CDaCharactersEnum (const CDaCharactersEnum & pSource)
:	CEnumVariant (pSource)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaCharactersEnum::CDaCharactersEnum (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp ();

	mCharacters.Copy (pSource.mCharacters);
#ifdef	_DEBUG
	TheControlApp->mComObjects.Add (this);
#endif
}

CDaCharactersEnum::CDaCharactersEnum (CDaCharactersObj & pCharacters)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaCharactersEnum::CDaCharactersEnum (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp ();

	mCharacters.SetSize (pCharacters.mCharacters.GetSize());
	for	(int lNdx = 0; lNdx <= pCharacters.mCharacters.GetUpperBound (); lNdx++)
	{
		mCharacters [lNdx] = pCharacters.mCharacters [lNdx];
	}
#ifdef	_DEBUG
	TheControlApp->mComObjects.Add (this);
#endif
}

CDaCharactersEnum::~CDaCharactersEnum ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaCharactersEnum::~CDaCharactersEnum (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	Terminate (true);
	AfxOleUnlockApp ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Remove (this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CDaCharactersEnum::Terminate (bool pFinal)
{
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CDaCharactersEnum::GetItemCount ()
{
	return (long)mCharacters.GetSize();
}

void CDaCharactersEnum::PutItem (long pItemNdx, VARIANT & pItem)
{
	V_VT (&pItem) = VT_DISPATCH;
	V_DISPATCH (&pItem) = mCharacters [pItemNdx];
	mCharacters [pItemNdx].AddRef ();
}

CEnumVariant * CDaCharactersEnum::Clone ()
{
	return new CDaCharactersEnum (*this);
}
