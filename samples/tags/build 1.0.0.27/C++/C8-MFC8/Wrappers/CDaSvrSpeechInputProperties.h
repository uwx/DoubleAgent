// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaSvrSpeechInputProperties wrapper class

class CDaSvrSpeechInputProperties : public COleDispatchDriver
{
public:
	CDaSvrSpeechInputProperties(){} // Calls COleDispatchDriver default constructor
	CDaSvrSpeechInputProperties(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaSvrSpeechInputProperties(const CDaSvrSpeechInputProperties& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaSvrSpeechInputProperties methods
public:
	void GetInstalled(long * pbInstalled)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbInstalled);
	}
	void GetEnabled(long * pbEnabled)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbEnabled);
	}
	void GetHotKey(BSTR * pbszHotCharKey)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbszHotCharKey);
	}
	void GetLCID(unsigned long * plcidCurrent)
	{
		static BYTE parms[] = VTS_PUI4 ;
		InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, plcidCurrent);
	}
	void GetEngine(BSTR * pbszEngine)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbszEngine);
	}
	void SetEngine(LPCTSTR bszEngine)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszEngine);
	}
	void GetListeningTip(long * pbListeningTip)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbListeningTip);
	}

	// IDaSvrSpeechInputProperties properties
public:

};
