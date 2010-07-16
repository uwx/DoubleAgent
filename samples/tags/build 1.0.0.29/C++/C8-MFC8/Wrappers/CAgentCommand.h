// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAgentCommand wrapper class

class CAgentCommand : public COleDispatchDriver
{
public:
	CAgentCommand(){} // Calls COleDispatchDriver default constructor
	CAgentCommand(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAgentCommand(const CAgentCommand& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IAgentCommand methods
public:
	void SetCaption(LPCTSTR bszCaption)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszCaption);
	}
	void GetCaption(BSTR * pbszCaption)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbszCaption);
	}
	void SetVoice(LPCTSTR bszVoice)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszVoice);
	}
	void GetVoice(BSTR * pbszVoice)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbszVoice);
	}
	void SetEnabled(long bEnabled)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bEnabled);
	}
	void GetEnabled(long * pbEnabled)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbEnabled);
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
	void SetConfidenceThreshold(long lThreshold)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lThreshold);
	}
	void GetConfidenceThreshold(long * plThreshold)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms, plThreshold);
	}
	void SetConfidenceText(LPCTSTR bszTipText)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszTipText);
	}
	void GetConfidenceText(BSTR * pbszTipText)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbszTipText);
	}
	void GetID(long * pdwID)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pdwID);
	}

	// IAgentCommand properties
public:

};
