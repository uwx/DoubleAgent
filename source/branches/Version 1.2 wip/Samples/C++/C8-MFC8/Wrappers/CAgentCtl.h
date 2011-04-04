// CAgentCtl.h  : Declaration of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CAgentCtl

class CAgentCtl : public CWnd
{
protected:
	DECLARE_DYNCREATE(CAgentCtl)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xD45FD31B, 0x5C6E, 0x11D1, { 0x9E, 0xC1, 0x0, 0xC0, 0x4F, 0xD7, 0x8, 0x1F } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// Attributes
public:

// Operations
public:

	LPDISPATCH get_Characters()
	{
		LPDISPATCH result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_AudioOutput()
	{
		LPDISPATCH result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_SpeechInput()
	{
		LPDISPATCH result;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_PropertySheet()
	{
		LPDISPATCH result;
		InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_CommandsWindow()
	{
		LPDISPATCH result;
		InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL get_Connected()
	{
		BOOL result;
		InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Connected(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_Suspended()
	{
		BOOL result;
		InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}


};
