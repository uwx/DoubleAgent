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
#ifndef AGENTFILES_H_INCLUDED_
#define AGENTFILES_H_INCLUDED_
#pragma once

#include "DaCoreExp.h"
#include "AgentFile.h"

//////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable:4251 4275)

class _DACORE_IMPEXP CAgentFiles : public CObject
{
public:
	CAgentFiles();
	virtual ~CAgentFiles();
	DECLARE_DYNAMIC(CAgentFiles)

// Attributes
	const CPtrTypeArray <CAgentFile> & Files () const;

// Operations
	HRESULT Load (LPCTSTR pPath = NULL, UINT pLogLevel = 15);

	int FindDefChar ();
	CAgentFile * GetDefChar ();

	static tBstrPtr GetDefCharPath (const CStringArray * pSearchPath = NULL);
	static HRESULT SetDefCharPath (LPCTSTR pCharPath);

	static tBstrPtr GetAgentPath (bool pAlternatePlatform = false);
	static tBstrPtr GetSystemCharsPath (UINT pPathNum = 0);
	static tBstrPtr GetOfficeCharsPath ();

// Implementation
protected:
	COwnPtrArray <CAgentFile>	mFiles;

// new and delete for DLL object
public:
#pragma auto_inline (off)
	void PASCAL operator delete (void* p) {CObject::operator delete (p);}
#ifdef	_DEBUG
	void PASCAL operator delete (void *p, LPCSTR lpszFileName, int nLine) {CObject::operator delete (p, lpszFileName, nLine);}
#endif
	void* PASCAL operator new(size_t nSize) {return CObject::operator new (nSize);}
#ifdef	_DEBUG
	void* PASCAL operator new(size_t nSize, LPCSTR lpszFileName, int nLine) {return CObject::operator new (nSize, lpszFileName, nLine);}
#endif
#pragma auto_inline ()
};

#pragma warning(pop)
//////////////////////////////////////////////////////////////////////

#endif // AGENTFILES_H_INCLUDED_
