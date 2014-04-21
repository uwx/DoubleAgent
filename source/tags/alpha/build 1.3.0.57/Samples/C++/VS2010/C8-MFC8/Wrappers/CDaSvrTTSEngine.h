// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaSvrTTSEngine wrapper class

class CDaSvrTTSEngine : public COleDispatchDriver
{
public:
	CDaSvrTTSEngine(){} // Calls COleDispatchDriver default constructor
	CDaSvrTTSEngine(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaSvrTTSEngine(const CDaSvrTTSEngine& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaSvrTTSEngine methods
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
		InvokeHelper(0x60020001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_Manufacturer()
	{
		CString result;
		InvokeHelper(0x60020002, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void GetVersion(short * MajorVersion, short * MinorVersion)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, MajorVersion, MinorVersion);
	}
	long get_Gender()
	{
		long result;
		InvokeHelper(0x60020004, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_LanguageID()
	{
		long result;
		InvokeHelper(0x60020005, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString get_LanguageName(BOOL EnglishName)
	{
		CString result;
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60020006, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, EnglishName);
		return result;
	}

	// IDaSvrTTSEngine properties
public:

};
