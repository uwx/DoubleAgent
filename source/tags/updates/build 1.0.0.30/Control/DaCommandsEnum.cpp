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
#include "DaCommandsEnum.h"

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

IMPLEMENT_DYNAMIC(CDaCommandsEnum, CEnumVariant)

CDaCommandsEnum::CDaCommandsEnum (const CDaCommandsEnum & pSource)
:	CEnumVariant (pSource)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaCommandsEnum::CDaCommandsEnum (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp ();

	mCommands.Copy (pSource.mCommands);
#ifdef	_DEBUG
	TheControlApp->mComObjects.Add (this);
#endif
}

CDaCommandsEnum::CDaCommandsEnum (CDaCommandsObj & pCommands)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaCommandsEnum::CDaCommandsEnum (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp ();

	mCommands.SetSize (pCommands.mCommands.GetSize());
	for	(int lNdx = 0; lNdx <= pCommands.mCommands.GetUpperBound (); lNdx++)
	{
		mCommands [lNdx] = pCommands.mCommands [lNdx];
	}
#ifdef	_DEBUG
	TheControlApp->mComObjects.Add (this);
#endif
}

CDaCommandsEnum::~CDaCommandsEnum ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaCommandsEnum::~CDaCommandsEnum (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	Terminate (true);
	AfxOleUnlockApp ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Remove (this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CDaCommandsEnum::Terminate (bool pFinal)
{
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CDaCommandsEnum::GetItemCount ()
{
	return (long)mCommands.GetSize();
}

void CDaCommandsEnum::PutItem (long pItemNdx, VARIANT & pItem)
{
	V_VT (&pItem) = VT_DISPATCH;
	V_DISPATCH (&pItem) = mCommands [pItemNdx];
	mCommands [pItemNdx].AddRef ();
}

CEnumVariant * CDaCommandsEnum::Clone ()
{
	return new CDaCommandsEnum (*this);
}
