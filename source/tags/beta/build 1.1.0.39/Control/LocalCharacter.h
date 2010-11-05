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
#pragma once
#include "DaControlMod.h"
#include "DaCtlCharacter.h"

/////////////////////////////////////////////////////////////////////////////

class CLocalCharacter : public CDaCmnCharacter
{
public:
	CLocalCharacter (DaCtlCharacter & pOwner);
	virtual ~CLocalCharacter ();

// Overrides
public:
	virtual class CDaCmnCommands * GetCommands (bool pCreateObject);
	virtual class CDaCmnBalloon * GetBalloon (bool pCreateObject);

	virtual DWORD GetStyle () const;
	virtual HRESULT SetStyle (DWORD pRemoveStyle, DWORD pAddStyle);

	virtual short GetActiveState () const;
	virtual HRESULT SetActiveState (short pActiveState);
	virtual void _OnAppActivated (bool pActive);

protected:
	DaCtlCharacter &	mOwner;
	DWORD				mActiveState;
	DWORD				mLocalStyle;
};

/////////////////////////////////////////////////////////////////////////////

#define	LocalCharacterStyle (CharacterStyle_SuspendPause|CharacterStyle_SuspendStop|CharacterStyle_SuspendHide)

/////////////////////////////////////////////////////////////////////////////
