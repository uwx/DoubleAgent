/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
#pragma once
#include "DaCoreExp.h"
#include "DaCoreRes.h"
#include "PropertyPage.h"
#include "LayoutTools.h"
#include "AgentFile.h"
#include "AgentPreviewWnd.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CPropPageCharacterObj :
	public CComObjectRootEx<CComMultiThreadModel>,
	public IUnknown
{
// Declarations
public:
	DECLARE_NOT_AGGREGATABLE(CPropPageCharacterObj)

	BEGIN_COM_MAP(CPropPageCharacterObj)
		COM_INTERFACE_ENTRY(IUnknown)
	END_COM_MAP()
};

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CPropPageCharacter :
	public CPropPageCharacterObj,
	public CAtlPropertyPage,
	protected CLayoutTools
{
	DECLARE_DLL_OBJECT_EX(CPropPageCharacter, _DACORE_IMPEXP)
protected:
	CPropPageCharacter();
public:
	_DACORE_IMPEXP virtual ~CPropPageCharacter();
	_DACORE_IMPEXP static CPropPageCharacter * CreateInstance ();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_CHARACTER };
	CContainedWindow	mCharTtsModeID;
	CContainedWindow	mCharStandard;
	CContainedWindow	mCharPreview;
	CContainedWindow	mCharName;
	CContainedWindow	mCharInvalid;
	CContainedWindow	mCharGuid;
	CContainedWindow	mCharFileVer;
	CContainedWindow	mCharDesc;

// Attributes
	tPtr <CAgentFile>	mAgentFile;

// Operations
	_DACORE_IMPEXP void FinalRelease();
	_DACORE_IMPEXP HRESULT AddSheetPage (LPFNSVADDPROPSHEETPAGE pAddPageFunc, LPARAM pLparam);

// Overrides
protected:
	_DACORE_IMPEXP virtual BOOL OnInitDialog();

// Implementation
protected:
	LRESULT OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnShowWindow (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCtlColor (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_MSG_MAP(CPropPageCharacter)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColor)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColor)
		CHAIN_MSG_MAP(CAtlPropertyPage)
	END_MSG_MAP()

protected:
	static UINT CALLBACK PropPageCallback (HWND pWnd, UINT pMsgId, LPPROPSHEETPAGE pPropPage);

	void NoCharacter ();
	void ShowCharacter ();
	void ShowPreview ();
	void StartPreview ();
	void StopPreview ();

protected:
	CGlobalHandle			mTemplate;
	tPtr <CAgentPreviewWnd>	mPreviewWnd;
};

/////////////////////////////////////////////////////////////////////////////
