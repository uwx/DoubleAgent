#pragma once
#include "AfxTemplEx.h"
#include "InstanceGate.h"

/////////////////////////////////////////////////////////////////////////////

struct CActionLine
{
	CFileTime		mTime;
	CString			mAction;
	CStringArray	mValues;
};

/////////////////////////////////////////////////////////////////////////////

class CActionFile : public CStdioFile, private CInstanceGate
{
public:
	CActionFile();
	virtual ~CActionFile();

// Attributes
public:
	INT_PTR GetLineCount () const;

// Operations
public:
	bool Open (LPCTSTR pPath);

	CActionLine * GetNextLine ();
	CActionLine * PeekNextLine ();

// Overrides
	//{{AFX_VIRTUAL(CActionFile)
	//}}AFX_VIRTUAL

// Implementation
protected:
	static DWORD WINAPI AsyncThreadProc (PVOID lpParameter);
	INT_PTR ReadLine (CFileTime * pLineTime, CFileTime * pFirstLineTime);
	void ParseLineTime (LPCTSTR pDateStr, LPCTSTR pTimeStr, CFileTime & pLineTime);

protected:
	mutable ::CCriticalSection	mCriticalSection;
	::CEvent					mThreadEndEvent;
	COwnPtrList <CActionLine>	mLines;
};

/////////////////////////////////////////////////////////////////////////////
