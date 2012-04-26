/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
#pragma once
#include "PropertyPage.h"
#include "LayoutTools.h"
#include "RegistrySearch.h"

/////////////////////////////////////////////////////////////////////////////

class CPropPageRegistry : public CAtlPropertyPage, protected CLayoutTools, protected CRegistrySearch
{
public:
	CPropPageRegistry();
	virtual ~CPropPageRegistry();

// Dialog Data
	enum { IDD = IDD_REGISTRY };
	CContainedWindow	mMaStatus;
	CContainedWindow	mDaStatus;
	CContainedWindow	mUseMaButton;
	CContainedWindow	mUseDaButton;
	CContainedWindow	mDaTree;
	CContainedWindow	mMaTree;

// Operations
	bool PrepareElevated (HWND pOwnerWnd);

// Overrides
protected:
	virtual BOOL OnInitDialog ();

// Implementation
protected:
	LRESULT OnCustomDrawTree(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnUseDa(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnUseMa(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnLinkClick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	BEGIN_MSG_MAP(CPropPageRegistry)
		NOTIFY_HANDLER (IDC_DA_REGISTRY, NM_CUSTOMDRAW, OnCustomDrawTree)
		NOTIFY_HANDLER (IDC_MA_REGISTRY, NM_CUSTOMDRAW, OnCustomDrawTree)
		COMMAND_HANDLER(IDC_USE_DA, BN_CLICKED, OnUseDa)
		COMMAND_HANDLER(IDC_USE_MA, BN_CLICKED, OnUseMa)
		NOTIFY_HANDLER (IDC_DA_STATUS, NM_CLICK, OnLinkClick)
		CHAIN_MSG_MAP(CAtlPropertyPage)
	END_MSG_MAP()

protected:
	void InitFonts ();
	void InitLayout (bool pMsAgentInstalled, bool pMsAgentEmulated);
	void ShowDaRegistry ();
	bool ShowMsRegistry ();
	bool ShowInstallStatus ();
	bool ShowDaInstallStatus ();
	bool ShowMsInstallStatus ();
	void ShowElevated ();

	HTREEITEM ShowClassId (CAtlString& pProgName, LPCTSTR pNameFormat, REFGUID pClassId, CWindow& pTree, bool pShowMissing = true, bool pAltPlatform = false, HTREEITEM pParentItem = TVI_ROOT);
	HTREEITEM ShowClassId (CAtlString& pProgName, LPCTSTR pNameFormat, REFGUID pClassId, CWindow& pTree, HKEY pGlobalClasses, HKEY pUserClasses, HKEY pClassesRoot, bool pShowMissing = true, HTREEITEM pParentItem = TVI_ROOT);
	HTREEITEM ShowProgId (LPCTSTR pProgName, LPCTSTR pNameFormat, LPCTSTR pProgId, CWindow& pTree, bool pShowMissing = true, bool pAltPlatform = false, HTREEITEM pParentItem = TVI_ROOT);
	HTREEITEM ShowProgId (LPCTSTR pProgName, LPCTSTR pNameFormat, LPCTSTR pProgId, CWindow& pTree, HKEY pGlobalClasses, HKEY pUserClasses, HKEY pClassesRoot, bool pShowMissing = true, HTREEITEM pParentItem = TVI_ROOT);
	CAtlString GetShellPropertiesExt (LPCTSTR pProgId, HKEY pClassesRoot = HKEY_CLASSES_ROOT);

	UINT ShowTreatAs (HTREEITEM pDaItem, LPCTSTR pDaItemName, HTREEITEM& pDaTreatAs, HTREEITEM pMaItem, LPCTSTR pMaItemName, HTREEITEM& pMaTreatAs, REFGUID pClsId, REFGUID pTreatAsClsId, bool pAltPlatform = false);
	void ShowTreatAs (HTREEITEM pDaItem, LPCTSTR pDaItemName, HTREEITEM& pDaTreatAs, HTREEITEM pMaItem, LPCTSTR pMaItemName, HTREEITEM& pMaTreatAs, UINT pTreatAsStatus);
	bool GetTreatAs (REFGUID pClsId, REFGUID pTreatAsClsId, bool pAltPlatform = false);
	bool GetTreatAs (HKEY pClassesRoot, REFGUID pClsId, REFGUID pTreatAsClsId);
	bool UpdateTreatAs (REFGUID pClsId, REFGUID pTreatAsClsId);
	bool UpdateTreatAs (LPCTSTR pClsId, LPCTSTR pTreatAsClsId);

	HTREEITEM InsertTreeItem (CWindow& pTree, LPCTSTR pItemText, HTREEITEM pParentItem, bool pExpanded = false);
	bool IsHighlightedItem (CWindow& pTree, HTREEITEM pTreeItem, HTREEITEM pTreatAsItem, HTREEITEM pRootItem);

protected:
	struct InstalledProduct
	{
		HTREEITEM			mItems [8];
		HTREEITEM&			mServerItem;
		HTREEITEM&			mServerItemAlt;
		HTREEITEM&			mControlItem;
		HTREEITEM&			mCharPropsItem;
		HTREEITEM&			mServerTreatAs;
		HTREEITEM&			mServerTreatAsAlt;
		HTREEITEM&			mControlTreatAs;
		HTREEITEM&			mCharPropsTreatAs;
		CAtlString			mServerName;
		CAtlString			mServerNameAlt;
		CAtlString			mControlName;
		CAtlString			mCharPropsName;
		UINT				mItemCount;

		InstalledProduct ()
		:	mServerItem (mItems [0]),
			mServerItemAlt (mItems [1]),
			mControlItem (mItems [2]),
			mCharPropsItem (mItems [3]),
			mServerTreatAs (mItems [4]),
			mServerTreatAsAlt (mItems [5]),
			mControlTreatAs (mItems [6]),
			mCharPropsTreatAs (mItems [7])
		{
			Clear ();
		}
		void Clear ()
		{
			memset (mItems, 0, sizeof(mItems));
			mItemCount = 0;
		}
	};

	struct EmulationStatus
	{
		UINT	mServerStatus;
		UINT	mServerStatusAlt;
		UINT	mControlStatus;
		UINT	mCharPropsStatus;
	};

	enum TreatAsStatus
	{
		TreatAsEnabled = 0x0001,
		TreatAsAllUsers = 0x0002,
		TreatAsThisUser = 0x0004
	};

protected:
	CFontHandle				mTitleFont;
	CFontHandle				mBoldFont;
	IOleCommandTargetPtr	mElevatedSettings;
	InstalledProduct		mMaInstalled;
	InstalledProduct		mDaInstalled;
	tS <EmulationStatus>	mEmulationStatus;
#ifdef	_WIN64
	InstalledProduct		mMaInstalled32;
	InstalledProduct		mDaInstalled32;
	tS <EmulationStatus>	mEmulationStatus32;
#endif

protected:
	class CDetailsDlg : public CDialogImpl <CDetailsDlg>
	{
	public:
		enum { IDD = IDD_REGISTRY_DETAILS };

		BEGIN_MSG_MAP(CDetailsDlg)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			MESSAGE_HANDLER(WM_CLOSE, OnClose)
			COMMAND_HANDLER(IDOK, BN_CLICKED, OnOk)
		END_MSG_MAP()

		LRESULT OnInitDialog (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnClose (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnOk (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

		CAxWindow mDetailsText;
		CAtlString mDetailsURL;
	};
};

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{F5BE8BD2-7DE6-11D0-91FE-00C04FD701A5}")) AgentControl15;
class __declspec(uuid("{D6589123-FC70-11D0-AC94-00C04FD97575}")) AgentServerTypeLib2;
class __declspec(uuid("{143A62C8-C33B-11D1-84FE-00C04FA34A14}")) AgentCharacterProps;
class __declspec(uuid("{0FA9F4D5-A173-11D1-AA62-00C04FA34D72}")) AgentVoiceCommands;
class __declspec(uuid("{4BAC124B-78C8-11D1-B9A8-00C04FD97575}")) AgentServerHandler;
class __declspec(uuid("{D45FD31D-5C6E-11D1-9EC1-00C04FD7081F}")) AgentNotifyProxy;
class __declspec(uuid("{D45FD31E-5C6E-11D1-9EC1-00C04FD7081F}")) AgentCharacterProxy;
class __declspec(uuid("{95A893C3-543A-11D0-AC45-00C04FD97575}")) AgentTtsEngine;
class __declspec(uuid("{D45FD31C-5C6E-11D1-9EC1-00C04FD7081F}")) AgentTtsEngine2;
class __declspec(uuid("{D45FD300-5C6E-11D1-9EC1-00C04FD7081F}")) AgentFileProvider;
class __declspec(uuid("{D45FD2FF-5C6E-11D1-9EC1-00C04FD7081F}")) AgentDocFileProvider;
class __declspec(uuid("{D45FD301-5C6E-11D1-9EC1-00C04FD7081F}")) AgentFlatFileProvider;

#define	_MS_AGENT_SERVER_TITLE			"Microsoft Agent Server 2.0"
#define	_MS_AGENT_SERVER_TYPELIB2_TITLE	"Microsoft Agent Server Extensions 2.0"
#define	_MS_AGENT_CONTROL_TITLE			"Microsoft Agent Control 2.0"
#define	_MS_AGENT_CONTROL15				"Agent.Control.1"
#define	_MS_AGENT_CONTROL15_TITLE		"Microsoft Agent Control 1.5"
#define	_MS_AGENT_PROPERTIES_TITLE		"Microsoft Agent Character Property Sheet Handler"
#define	_MS_AGENT_CHARACTER_TITLE		"Microsoft Agent Character File"
#define	_MS_AGENT_CHARACTER_WEB_TITLE	"Microsoft Agent Character File (HTTP Format)"
#define	_MS_AGENT_CHARACTER_PVW_TITLE	"Microsoft Agent Preview File"
#define	_MS_AGENT_VOICE_COMMANDS		"Microsoft Agent Voice Command Module Class"
#define	_MS_AGENT_SERVER_PROXY			"Microsoft Agent Server Proxy"
#define	_MS_AGENT_NOTIFY_PROXY			"Microsoft Agent Notify Proxy"
#define	_MS_AGENT_CHARACTER_PROXY		"Microsoft Agent Character Proxy"
#define	_MS_AGENT_TTS_ENGINE			"Microsoft Agent TTS Engine"
#define	_MS_AGENT_FILE_PROVIDER			"Microsoft Agent File Provider 2.0"
#define	_MS_AGENT_DOCFILE_PROVIDER		"Microsoft Agent DocFile Provider 2.0"
#define	_MS_AGENT_FLATFILE_PROVIDER		"Microsoft Agent Flat File Provider 2.0"

/////////////////////////////////////////////////////////////////////////////
