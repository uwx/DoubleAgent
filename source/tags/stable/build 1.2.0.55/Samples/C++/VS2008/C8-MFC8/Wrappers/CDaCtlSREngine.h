// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaCtlSREngine wrapper class

class CDaCtlSREngine : public COleDispatchDriver
{
public:
	CDaCtlSREngine(){} // Calls COleDispatchDriver default constructor
	CDaCtlSREngine(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaCtlSREngine(const CDaCtlSREngine& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaCtlSREngine methods
public:
	CString get_SRModeID()
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
	long get_LanguageID()
	{
		long result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString get_LanguageName(BOOL EnglishName)
	{
		CString result;
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, EnglishName);
		return result;
	}
	SAFEARRAY * get_LanguageIDs()
	{
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_EMPTY, NULL, NULL);
	}
	SAFEARRAY * get_LanguageNames(BOOL EnglishNames)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_EMPTY, NULL, parms, EnglishNames);
	}

	// IDaCtlSREngine properties
public:

};
