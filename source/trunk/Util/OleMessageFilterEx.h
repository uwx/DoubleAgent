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
#if !defined(AFX_OLEMESSAGEFILTEREX_H__B027E688_0CFD_469C_9AD8_4DBBF33D2617__INCLUDED_)
#define AFX_OLEMESSAGEFILTEREX_H__B027E688_0CFD_469C_9AD8_4DBBF33D2617__INCLUDED_
#pragma once

//////////////////////////////////////////////////////////////////////

class COleMessageFilterEx : public COleMessageFilter
{
public:
	COleMessageFilterEx (DWORD pNotRespondingTimeout = 8000, DWORD pBusyTimeout = 10000, UINT pLogLevelDlg = LogVerbose, UINT pLogLevelMsg = LogVerbose+1);
	COleMessageFilterEx (const COleMessageFilter & pSource);
	COleMessageFilterEx (const CWinThread * pThread);
	COleMessageFilterEx & operator= (const COleMessageFilter & pSource);
	virtual ~COleMessageFilterEx ();
	DECLARE_DYNAMIC (COleMessageFilterEx)

// Attributes
	UINT	mLogLevelDlg;
	UINT	mLogLevelMsg;

	DWORD NotRespondingTimeout () const;
	DWORD BusyTimeout () const;

// Operations
	COleMessageFilterEx * SafeEnableNotResponding (DWORD pNotRespondingTimeout = 8000);
	COleMessageFilterEx * SafeEnableBusy (DWORD pBusyTimeout = 10000);

	static bool SafeBeginBusyState (CWinThread * pThread = NULL);
	static bool SafeEndBusyState (CWinThread * pThread = NULL);

	static COleMessageFilterEx * SetAppFilter (UINT pLogLevelDlg = LogVerbose, UINT pLogLevelMsg = LogVerbose+1);
	static COleMessageFilterEx * SetThreadFilter (UINT pLogLevelDlg = LogVerbose, UINT pLogLevelMsg = LogVerbose+1);
	static COleMessageFilterEx * SetThreadFilter (CWinThread * pThread, COleMessageFilterEx * pMessageFilter);
	static bool ResetAppFilter (bool pUseDefaultFilter);
	static bool ResetThreadFilter (bool pUseDefaultFilter);
	static bool ResetThreadFilter (CWinThread * pThread, bool pUseDefaultFilter);

// Overrides
	//{{AFX_VIRTUAL(COleMessageFilterEx)
	public:
	virtual BOOL OnMessagePending (const MSG* pMsg);
	virtual BOOL IsSignificantMessage(MSG* pMsg);
	virtual int OnBusyDialog (HTASK htaskBusy);
	virtual int OnNotRespondingDialog (HTASK htaskBusy);
	//}}AFX_VIRTUAL
};

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

class COleMessageFilterAuto
{
public:
	COleMessageFilterAuto ()
	{
		Construct ();
	}
	COleMessageFilterAuto (DWORD pNotRespondingTimeout, DWORD pBusyTimeout)
	{
		Construct ();
		EnableNotResponding (pNotRespondingTimeout);
		EnableBusy (pBusyTimeout);
	}
	~COleMessageFilterAuto ()
	{
		Destruct ();
	}

	bool EnableNotResponding (DWORD pNotRespondingTimeout = 8000)
	{
		if	(mMessageFilter->SafeEnableNotResponding (pNotRespondingTimeout))
		{
			return true;
		}
		return false;
	}
	bool EnableBusy (DWORD pBusyTimeout = 10000)
	{
		if	(mMessageFilter->SafeEnableBusy (pBusyTimeout))
		{
			return true;
		}
		return false;
	}

private:
	void Construct ()
	{
		if	(mOldMessageFilter = AfxOleGetMessageFilter ())
		{
			mMessageFilter = DYNAMIC_DOWNCAST (COleMessageFilterEx, mOldMessageFilter);
		}
		if	(mMessageFilter)
		{
			mOldNotResponding = mMessageFilter->NotRespondingTimeout ();
			mOldBusy = mMessageFilter->BusyTimeout ();
		}
		else
		{
			mMessageFilter = COleMessageFilterEx::SetThreadFilter();
		}
	}

	void Destruct ()
	{
		if	(mOldMessageFilter)
		{
			if	(mMessageFilter == mOldMessageFilter)
			{
				mMessageFilter->SafeEnableNotResponding (mOldNotResponding);
				mMessageFilter->SafeEnableBusy (mOldBusy);
			}
			else
			{
				COleMessageFilterEx::ResetThreadFilter (true);
			}
		}
		else
		{
			COleMessageFilterEx::ResetThreadFilter (false);
		}
	}

private:
	COleMessageFilterEx *	mMessageFilter;
	COleMessageFilter *		mOldMessageFilter;
	DWORD					mOldNotResponding;
	DWORD					mOldBusy;
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_OLEMESSAGEFILTEREX_H__B027E688_0CFD_469C_9AD8_4DBBF33D2617__INCLUDED_)
