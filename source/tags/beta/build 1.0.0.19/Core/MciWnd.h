#if !defined(AFX_MCIWND_H__BD3A2000_B36E_4252_9B11_EB6172C94E47__INCLUDED_)
#define AFX_MCIWND_H__BD3A2000_B36E_4252_9B11_EB6172C94E47__INCLUDED_
#pragma once

#include "DaCoreExp.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251 4275)

class _DACORE_IMPEXP CMciWnd : public CWnd
{
protected:
	CMciWnd();
public:
	virtual ~CMciWnd();
	DECLARE_DYNCREATE(CMciWnd)

// Attributes
public:

// Operations
public:
	virtual bool Open (LPCTSTR pFileName);
	virtual void Close ();

	virtual bool Start ();
	virtual bool Stop ();
	virtual bool Pause ();
	virtual bool Resume ();

	virtual bool IsPlaying ();

	bool HandleMciNotify (UINT pMsgId, WPARAM pWparam, LPARAM pLparam, LRESULT & pResult);

	void LogMciDevice (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
	void LogMciMode (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
	void LogMciMetrics (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
	void LogMciDrivers (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
	void LogAviFilters (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;

// Overrides
	//{{AFX_VIRTUAL(CMciWnd)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CMciWnd)
	afx_msg void OnDestroy();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void Opening (LPCTSTR pFileName);
	virtual void Opened ();
	virtual void Closing ();
	virtual void Closed ();

public:
	friend CString MciStyleStr (DWORD pMciStyle);
};

#pragma warning(pop)
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCIWND_H__BD3A2000_B36E_4252_9B11_EB6172C94E47__INCLUDED_)
