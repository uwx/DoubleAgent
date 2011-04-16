// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAgentCommands wrapper class

class CAgentCommands : public COleDispatchDriver
{
public:
	CAgentCommands(){} // Calls COleDispatchDriver default constructor
	CAgentCommands(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAgentCommands(const CAgentCommands& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IAgentCommands methods
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

	// IAgentCommands properties
public:

};
