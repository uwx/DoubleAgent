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
#ifndef DARECOGNITIONENGINESENUM_H_INCLUDED_
#define DARECOGNITIONENGINESENUM_H_INCLUDED_
#pragma once

#include "EnumVariant.h"
#include "DaRecognitionEnginesObj.h"

/////////////////////////////////////////////////////////////////////////////

class CDaRecognitionEnginesEnum : public CEnumVariant
{
protected:
	CDaRecognitionEnginesEnum (const CDaRecognitionEnginesEnum & pSource);
public:
	CDaRecognitionEnginesEnum (CDaRecognitionEnginesObj & pRecognitionEngines);
	virtual ~CDaRecognitionEnginesEnum ();
	DECLARE_DYNAMIC(CDaRecognitionEnginesEnum)

// Attributes
public:

// Operations
public:

// Overrides
	//{{AFX_VIRTUAL(CDaRecognitionEnginesEnum)
	protected:
	virtual long GetItemCount ();
	virtual void PutItem (long pItemNdx, VARIANT & pItem);
	virtual CEnumVariant * Clone ();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void Terminate (bool pFinal);
protected:
	CArrayEx <IDaCtlRecognitionEnginePtr, IDaCtlRecognitionEngine *>	mRecognitionEngines;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DARECOGNITIONENGINESENUM_H_INCLUDED_
