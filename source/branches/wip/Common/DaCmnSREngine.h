/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
#include "DaServerOdl.h"

/////////////////////////////////////////////////////////////////////////////

class CDaCmnSREngine
{
public:
	CDaCmnSREngine ();
	virtual ~CDaCmnSREngine ();

// Operations
public:
	void Initialize (class CSapi5InputInfo * pInputInfo);
	bool Initialize (class CSapi5Input * pInput);
	bool Initialize (class CAgentFile * pFile);

// Interfaces
public:
	HRESULT get_SRModeID (BSTR *SRModeID);
	HRESULT get_DisplayName (BSTR *DisplayName);
	HRESULT get_Manufacturer (BSTR *Manufacturer);
	HRESULT GetVersion (short *MajorVersion, short *MinorVersion);
	HRESULT get_LanguageID (long *LanguageID);
	HRESULT get_LanguageName (VARIANT_BOOL EnglishName, BSTR *LanguageName);
	HRESULT get_LanguageIDs (SAFEARRAY **LanguageIds);
	HRESULT get_LanguageNames (VARIANT_BOOL EnglishNames, SAFEARRAY **LanguageNames);

// Implementation
protected:
	class CSapi5InputInfo * mSapi5Input;
};

////////////////////////////////////////////////////////////////////////////
