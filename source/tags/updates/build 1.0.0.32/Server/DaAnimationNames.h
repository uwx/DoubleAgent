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
#ifndef DAANIMATIONNAMES_H_INCLUDED_
#define DAANIMATIONNAMES_H_INCLUDED_
#pragma once

#include "EnumVariant.h"
#include "AgentFile.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E517-A208-11DE-ABF2-002421116FB2}")) CDaAnimationNames : public CEnumVariant
{
protected:
	CDaAnimationNames (const CDaAnimationNames & pSource);
public:
	CDaAnimationNames (CAgentFile & pAgentFile);
	virtual ~CDaAnimationNames ();
	void Terminate (bool pFinal, bool pAbandonned = false);
	DECLARE_DYNAMIC(CDaAnimationNames)

// Attributes
public:

// Operations
public:

// Overrides
	//{{AFX_VIRTUAL(CDaAnimationNames)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual long GetItemCount ();
	virtual void PutItem (long pItemNdx, VARIANT & pItem);
	virtual CEnumVariant * Clone ();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CStringArray	mAnimationNames;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DAANIMATIONNAMES_H_INCLUDED_
