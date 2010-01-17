/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is a utility used by Double Agent but not specific to
	Double Agent.  However, it is included as part of the Double Agent
	source code under the following conditions:

    This is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include <afxadv.h>
#include <shlwapi.h>
#include "CmdLineInfoEx.h"
#include "StringArrayEx.h"
#include "Log.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib, "shlwapi.lib")

//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC (CCmdLineInfoEx, CObject)

CCmdLineInfoEx::CCmdLineInfoEx ()
:	mLogLevelDebug (LogVerbose)
{
}

CCmdLineInfoEx::~CCmdLineInfoEx ()
{
}

//////////////////////////////////////////////////////////////////////

void CCmdLineInfoEx::ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast)
{
	CCommandLineInfo::ParseParam (pszParam, bFlag, bLast);

#if	(_MFC_VER < 0x0800)
	if	(
			(bFlag)
		&&	(bLast)
		&&	(m_nShellCommand == FileNew)
		&&	(
				(_tcsicmp (pszParam, _T("Register")) == 0)
			||	(_tcsicmp (pszParam, _T("RegServer")) == 0)
			)
		)
	{
		m_nShellCommand = FileNothing;
	}
#endif
}

//////////////////////////////////////////////////////////////////////

bool CCmdLineInfoEx::ParseOpenRecent (CRecentFileList * pRecent, bool pDefaultFileNew, bool pDefaultFileOpen, bool pDefaultOpenRecent)
{
	bool	lRet = false;

	if	(
			(
				(pDefaultOpenRecent)
			&&	(m_nShellCommand == FileNew)
			&&	(m_strFileName.IsEmpty ())
			)
		||	(
				(!pDefaultOpenRecent)
			&&	(m_nShellCommand == FileOpen)
			&&	(m_strFileName == _T("*"))
			)
		)
	{
		if	(
				(pRecent)
			&&	(pRecent->GetSize () > 0)
			)
		{
			m_strFileName = pRecent->m_arrNames [0];

			if	(
					(!m_strFileName.IsEmpty ())
				&&	(!PathFileExists (m_strFileName))
				)
			{
				m_strFileName.Empty ();
			}
		}

		if	(m_strFileName.IsEmpty ())
		{
			m_nShellCommand = FileNothing;
		}
		else
		{
			m_nShellCommand = FileOpen;
			lRet = true;
		}
	}
	else
	if	(
			(pDefaultOpenRecent)
		&&	(m_nShellCommand == FileOpen)
		&&	(m_strFileName == _T("*"))
		)
	{
		m_strFileName.Empty ();
		m_nShellCommand = FileNothing;
	}

	if	(!lRet)
	{
		if	(
				(pDefaultFileNew)
			&&	(
					(m_nShellCommand == FileNew)
				||	(m_nShellCommand == FileNothing)
				)
			&&	(m_strFileName.IsEmpty ())
			)
		{
			m_nShellCommand = FileNew;
		}
		else
		if	(
				(pDefaultFileOpen)
			&&	(
					(m_nShellCommand == FileOpen)
				||	(m_nShellCommand == FileNothing)
				)
			&&	(m_strFileName.IsEmpty ())
			)
		{
			m_nShellCommand = FileOpen;
		}
		else
		if	(
				(m_nShellCommand == FileOpen)
			&&	(m_strFileName == _T("?"))
			)
		{
			m_strFileName.Empty ();
		}
		else
		if	(
				(
					(m_nShellCommand == FileNew)
				||	(m_nShellCommand == FileOpen)
				)
			&&	(m_strFileName.IsEmpty ())
			)
		{
			m_nShellCommand = FileNothing;
		}
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CCmdLineInfoEx::RequiresOpenDlg () const
{
	if	(
			(
				(m_nShellCommand == FileOpen)
			||	(m_nShellCommand == FileNew)
			)
		&&	(m_strFileName.IsEmpty ())
		)
	{
		return true;
	}
	return false;
}


//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

int CCmdLineInfoEx::SplitCmdLine (CStringArray & pCmdLineParts)
{
	return SplitCmdLine (GetCommandLine(), pCmdLineParts);
}

int CCmdLineInfoEx::SplitCmdLine (LPCTSTR pCmdLine, CStringArray & pCmdLineParts)
{
	CString				lCmdLineRaw (pCmdLine);
	CStringArray		lCmdLine;
	int					lCmdNdx;
	bool				lCmdQuoted;

	pCmdLineParts.RemoveAll();

	if	(!lCmdLineRaw.IsEmpty())
	{
		MakeStringArray (lCmdLineRaw, lCmdLine, _T("\""), true);
		lCmdQuoted = (lCmdLineRaw [0] != _T('\"'));

		for	(lCmdNdx = 0; lCmdNdx <= lCmdLine.GetUpperBound(); lCmdNdx++)
		{
			CString &	lCmdPart = lCmdLine [lCmdNdx];

			lCmdPart.TrimLeft ();
			lCmdPart.TrimRight ();
			if	(!lCmdPart.IsEmpty ())
			{
				if	(lCmdQuoted)
				{
					pCmdLineParts.Add (lCmdPart);
				}
				else
				{
					MakeStringArray (lCmdPart, pCmdLineParts, _T(" \t\r\n"));
				}
			}
			lCmdQuoted = !lCmdQuoted;
		}
	}
	return (int)pCmdLineParts.GetSize();
}
