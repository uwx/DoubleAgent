/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software:
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

class CDaCmnSREngines
{
public:
	CDaCmnSREngines ();
	virtual ~CDaCmnSREngines ();

// Attributes
public:
	CAtlPtrTypeArray <class CSapi5InputInfo>	mSapi5Inputs;

// Operations
public:
	HRESULT UseAllInputs ();
	HRESULT UseTheseInputs (class CAgentFile* pFile, LANGID pLangId);
};

/////////////////////////////////////////////////////////////////////////////
