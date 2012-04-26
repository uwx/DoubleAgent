// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaSvrCommand wrapper class

class CDaSvrCommand : public COleDispatchDriver
{
public:
	CDaSvrCommand(){} // Calls COleDispatchDriver default constructor
	CDaSvrCommand(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaSvrCommand(const CDaSvrCommand& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaSvrCommand2 methods
public:
	void SetCaption(LPCTSTR Caption)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Caption);
	}
	void GetCaption(BSTR * Caption)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Caption);
	}
	void SetVoice(LPCTSTR Voice)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Voice);
	}
	void GetVoice(BSTR * Voice)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Voice);
	}
	void SetEnabled(long Enabled)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Enabled);
	}
	void GetEnabled(long * Enabled)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Enabled);
	}
	void SetVisible(long Visible)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Visible);
	}
	void GetVisible(long * Visible)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Visible);
	}
	void SetConfidenceThreshold(long Threshold)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Threshold);
	}
	void GetConfidenceThreshold(long * Threshold)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Threshold);
	}
	void SetConfidenceText(LPCTSTR TipText)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, TipText);
	}
	void GetConfidenceText(BSTR * TipText)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, TipText);
	}
	void GetID(long * CommandID)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, CommandID);
	}
	void SetHelpContextID(long ContextID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ContextID);
	}
	void GetHelpContextID(long * ContextID)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ContextID);
	}
	void SetVoiceCaption(LPCTSTR VoiceCaption)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, VoiceCaption);
	}
	void GetVoiceCaption(BSTR * pbszVoiceCaption)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60030003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbszVoiceCaption);
	}
	CString get_Caption()
	{
		CString result;
		InvokeHelper(0x60030004, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Caption(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_Enabled()
	{
		BOOL result;
		InvokeHelper(0x60030005, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Enabled(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60030005, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_Visible()
	{
		BOOL result;
		InvokeHelper(0x60030006, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Visible(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60030006, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_CommandID()
	{
		long result;
		InvokeHelper(0x60030007, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString get_VoiceCaption()
	{
		CString result;
		InvokeHelper(0x60030008, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_VoiceCaption(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030008, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_VoiceGrammar()
	{
		CString result;
		InvokeHelper(0x60030009, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_VoiceGrammar(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030009, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_ConfidenceThreshold()
	{
		long result;
		InvokeHelper(0x6003000a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ConfidenceThreshold(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6003000a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_ConfidenceText()
	{
		CString result;
		InvokeHelper(0x6003000b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_ConfidenceText(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6003000b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// IDaSvrCommand2 properties
public:

};
