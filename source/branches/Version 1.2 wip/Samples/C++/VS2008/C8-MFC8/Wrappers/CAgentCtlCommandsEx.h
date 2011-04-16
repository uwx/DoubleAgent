// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAgentCtlCommandsEx wrapper class

class CAgentCtlCommandsEx : public COleDispatchDriver
{
public:
	CAgentCtlCommandsEx(){} // Calls COleDispatchDriver default constructor
	CAgentCtlCommandsEx(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAgentCtlCommandsEx(const CAgentCtlCommandsEx& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IAgentCtlCommandsEx methods
public:
	LPDISPATCH get_Item(LPCTSTR Name)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Name);
		return result;
	}
	LPDISPATCH Command(LPCTSTR Name)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Name);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString get_Caption()
	{
		CString result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Caption(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_Voice()
	{
		CString result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Voice(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_Visible()
	{
		BOOL result;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Visible(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPUNKNOWN get_Enum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH Add(LPCTSTR Name, VARIANT& Caption, VARIANT& Voice, VARIANT& Enabled, VARIANT& Visible)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Name, &Caption, &Voice, &Enabled, &Visible);
		return result;
	}
	LPDISPATCH Insert(LPCTSTR Name, LPCTSTR RefName, VARIANT& Before, VARIANT& Caption, VARIANT& Voice, VARIANT& Enabled, VARIANT& Visible)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Name, RefName, &Before, &Caption, &Voice, &Enabled, &Visible);
		return result;
	}
	void Remove(LPCTSTR Name)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Name);
	}
	void RemoveAll()
	{
		InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void put_DefaultCommand(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_DefaultCommand()
	{
		CString result;
		InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_HelpContextID(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_HelpContextID()
	{
		long result;
		InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_FontName(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_FontName()
	{
		CString result;
		InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long get_FontSize()
	{
		long result;
		InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_FontSize(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void put_VoiceCaption(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x16, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_VoiceCaption()
	{
		CString result;
		InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_GlobalVoiceCommandsEnabled(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_GlobalVoiceCommandsEnabled()
	{
		BOOL result;
		InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}

	// IAgentCtlCommandsEx properties
public:

};
