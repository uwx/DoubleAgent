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

class CDaCmnCharacterFiles
{
public:
	CDaCmnCharacterFiles ();
	virtual ~CDaCmnCharacterFiles ();

// Attributes
public:
	const CAtlStringArray & FilePaths () const {return mFilePaths;}

// Operations
public:
	void Initialize ();

// Interfaces
public:
	// ICDaCharacterFiles
	HRESULT get_FilePaths (SAFEARRAY **FilePaths);
	HRESULT get_SearchPath (BSTR *SearchPath);
	HRESULT put_SearchPath (BSTR SearchPath);
	HRESULT get_DefaultSearchPath (BSTR *DefaultSearchPath);
	HRESULT get_Filter (long *Filter);
	HRESULT put_Filter (long Filter);
	HRESULT get_DefaultFilePath (BSTR *DefaultFilePath);
	HRESULT get_DefaultFileName (BSTR *DefaultFileName);

// Implementation
protected:
	void GetDefaultSearch ();
	void GetFilePaths ();
	void UpdateFilter (DWORD pNewFilter);

protected:
	CAtlStringArray	mDefaultSearchPath;
	CAtlStringArray	mSearchPath;
	CAtlStringArray	mFilePaths;
	DWORD			mFilter;
};

/////////////////////////////////////////////////////////////////////////////
