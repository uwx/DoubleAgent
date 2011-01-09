// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaCtlSettings wrapper class

class CDaCtlSettings : public COleDispatchDriver
{
public:
	CDaCtlSettings(){} // Calls COleDispatchDriver default constructor
	CDaCtlSettings(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaCtlSettings(const CDaCtlSettings& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaCtlSettings methods
public:
	BOOL get_SoundEffectsEnabled()
	{
		BOOL result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_BalloonEnabled()
	{
		BOOL result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_BalloonFont()
	{
		LPDISPATCH result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL get_TTSEnabled()
	{
		BOOL result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	short get_TTSSpeed()
	{
		short result;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	BOOL get_SREnabled()
	{
		BOOL result;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	CString get_SRHotKey()
	{
		CString result;
		InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	short get_SRHotKeyTime()
	{
		short result;
		InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	BOOL get_SRListeningTip()
	{
		BOOL result;
		InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_SRListeningPrompt()
	{
		BOOL result;
		InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	long get_AudioStatus()
	{
		long result;
		InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}

	// IDaCtlSettings properties
public:

};
