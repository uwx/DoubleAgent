// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaCtlCharacterFiles wrapper class

class CDaCtlCharacterFiles : public COleDispatchDriver
{
public:
	CDaCtlCharacterFiles(){} // Calls COleDispatchDriver default constructor
	CDaCtlCharacterFiles(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaCtlCharacterFiles(const CDaCtlCharacterFiles& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaCtlCharacterFiles methods
public:
	LPUNKNOWN get__NewEnum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	SAFEARRAY * get_FilePaths()
	{
		InvokeHelper(0x60020000, DISPATCH_PROPERTYGET, VT_EMPTY, NULL, NULL);
	}
	CString get_SearchPath()
	{
		CString result;
		InvokeHelper(0x60020001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_SearchPath(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60020001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_DefaultSearchPath()
	{
		CString result;
		InvokeHelper(0x60020002, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	BOOL get_DoubleAgentFiles()
	{
		BOOL result;
		InvokeHelper(0x60020003, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_DoubleAgentFiles(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60020003, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_MsAgentFiles()
	{
		BOOL result;
		InvokeHelper(0x60020004, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_MsAgentFiles(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60020004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_MsOfficeFiles()
	{
		BOOL result;
		InvokeHelper(0x60020005, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_MsOfficeFiles(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60020005, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_SpeakingCharacters()
	{
		BOOL result;
		InvokeHelper(0x60020006, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_SpeakingCharacters(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60020006, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_NonSpeakingCharacters()
	{
		BOOL result;
		InvokeHelper(0x60020007, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_NonSpeakingCharacters(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60020007, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_VerifyVersion()
	{
		BOOL result;
		InvokeHelper(0x60020008, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_VerifyVersion(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60020008, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_DefaultFilePath()
	{
		CString result;
		InvokeHelper(0x60020009, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_DefaultFileName()
	{
		CString result;
		InvokeHelper(0x6002000a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}

	// IDaCtlCharacterFiles properties
public:

};
