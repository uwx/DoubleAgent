#include <msidefs.h>
#include <msiquery.h>
#include <msi.h>
#pragma once

/////////////////////////////////////////////////////////////////////////////

class CMsiCustomAction
{
public:
	CMsiCustomAction (MSIHANDLE pInstall);
	virtual ~CMsiCustomAction ();

	bool IsInstallMode () const;
	bool IsCommitMode () const;
	bool IsRollbackMode () const;
	bool IsScheduledMode () const;

	LPTSTR GetMsiProperty (LPCTSTR pName) const;
	bool PutMsiProperty (LPCTSTR pName, LPCTSTR pProperty) const;

	LPTSTR GetActionData () const;
	LPTSTR GetActionDataPart (LPCTSTR pActionData, int pPart, TCHAR pDelim = _T('|')) const;
	void ActionStart (LPCTSTR pActionName, LPCTSTR pActionDescription, LPCTSTR pActionText = NULL, UINT pTotalTicks = 0);
	void ActionProgress (LPCTSTR pActionText, UINT pTicks = 1);

	void ResetProgress (UINT pTotalTicks);
	void AddToProgress (UINT pTotalTicks);
	void AdvanceProgress (UINT pTicks = 1);
	void SetAutoProgress (UINT pTicks);

	void EnableCancel (bool pEnable);

	bool IsLogEnabled () const;
	void LogInfoMessage (LPCTSTR pMessage);
	int ShowWarningMessage (LPCTSTR pMessage, UINT pErrorNum = 2);
	int ShowErrorMessage (LPCTSTR pMessage, UINT pErrorNum = 1);
	int ShowUserMessage (LPCTSTR pMessage, UINT pMessageType = MB_YESNO, UINT pErrorNum = 1);

	friend CAtlString FormatString (LPCTSTR pFormat, ...);

public:
	const UINT	mTickIncrement;
protected:
	MSIHANDLE	mInstall;
};
