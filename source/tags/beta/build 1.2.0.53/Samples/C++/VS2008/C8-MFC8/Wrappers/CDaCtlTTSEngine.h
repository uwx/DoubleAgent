// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaCtlTTSEngine wrapper class

class CDaCtlTTSEngine : public COleDispatchDriver
{
public:
	CDaCtlTTSEngine(){} // Calls COleDispatchDriver default constructor
	CDaCtlTTSEngine(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaCtlTTSEngine(const CDaCtlTTSEngine& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaCtlTTSEngine methods
public:
	CString get_TTSModeID()
	{
		CString result;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_DisplayName()
	{
		CString result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_Manufacturer()
	{
		CString result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void GetVersion(short * MajorVersion, short * MinorVersion)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, MajorVersion, MinorVersion);
	}
	long get_Gender()
	{
		long result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_LanguageID()
	{
		long result;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString get_LanguageName(BOOL EnglishName)
	{
		CString result;
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, EnglishName);
		return result;
	}

	// IDaCtlTTSEngine properties
public:

};
