// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAgentCommandsEx wrapper class

class CAgentCommandsEx : public COleDispatchDriver
{
public:
	CAgentCommandsEx(){} // Calls COleDispatchDriver default constructor
	CAgentCommandsEx(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAgentCommandsEx(const CAgentCommandsEx& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IAgentCommandsEx methods
public:
	void GetCommand(long dwCommandID, LPUNKNOWN * ppunkCommand)
	{
		static BYTE parms[] = VTS_I4 VTS_PUNKNOWN ;
		InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwCommandID, ppunkCommand);
	}
	void GetCount(long * pdwCount)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pdwCount);
	}
	void SetCaption(LPCTSTR bszCaption)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszCaption);
	}
	void GetCaption(BSTR * pbszCaption)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbszCaption);
	}
	void SetVoice(LPCTSTR bszVoice)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszVoice);
	}
	void GetVoice(BSTR * pbszVoice)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbszVoice);
	}
	void SetVisible(long bVisible)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bVisible);
	}
	void GetVisible(long * pbVisible)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbVisible);
	}
	void Add(LPCTSTR bszCaption, LPCTSTR bszVoice, long bEnabled, long bVisible, long * pdwID)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszCaption, bszVoice, bEnabled, bVisible, pdwID);
	}
	void Insert(LPCTSTR bszCaption, LPCTSTR bszVoice, long bEnabled, long bVisible, long dwRefID, long bBefore, long * pdwID)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszCaption, bszVoice, bEnabled, bVisible, dwRefID, bBefore, pdwID);
	}
	void Remove(long dwID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwID);
	}
	void RemoveAll()
	{
		InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void GetCommandEx(long dwCommandID, LPDISPATCH * ppCommandEx)
	{
		static BYTE parms[] = VTS_I4 VTS_PDISPATCH ;
		InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwCommandID, ppCommandEx);
	}
	void SetDefaultID(long dwID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwID);
	}
	void GetDefaultID(long * pdwID)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pdwID);
	}
	void SetHelpContextID(long ulHelpID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ulHelpID);
	}
	void GetHelpContextID(long * pulHelpID)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60030004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pulHelpID);
	}
	void SetFontName(LPCTSTR bszFontName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030005, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszFontName);
	}
	void GetFontName(BSTR * pbszFontName)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60030006, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbszFontName);
	}
	void SetFontSize(long lFontSize)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030007, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lFontSize);
	}
	void GetFontSize(long * lFontSize)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60030008, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lFontSize);
	}
	void SetVoiceCaption(LPCTSTR bszVoiceCaption)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030009, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszVoiceCaption);
	}
	void GetVoiceCaption(BSTR * bszVoiceCaption)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszVoiceCaption);
	}
	void AddEx(LPCTSTR bszCaption, LPCTSTR bszVoice, LPCTSTR bszVoiceCaption, long bEnabled, long bVisible, long ulHelpID, long * pdwID)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x6003000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszCaption, bszVoice, bszVoiceCaption, bEnabled, bVisible, ulHelpID, pdwID);
	}
	void InsertEx(LPCTSTR bszCaption, LPCTSTR bszVoice, LPCTSTR bszVoiceCaption, long bEnabled, long bVisible, long ulHelpID, long dwRefID, long bBefore, long * pdwID)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x6003000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszCaption, bszVoice, bszVoiceCaption, bEnabled, bVisible, ulHelpID, dwRefID, bBefore, pdwID);
	}
	void SetGlobalVoiceCommandsEnabled(long bEnable)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6003000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bEnable);
	}
	void GetGlobalVoiceCommandsEnabled(long * pbEnabled)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x6003000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbEnabled);
	}

	// IAgentCommandsEx properties
public:

};
