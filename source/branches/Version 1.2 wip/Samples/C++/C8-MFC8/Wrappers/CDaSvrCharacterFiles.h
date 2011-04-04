// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaSvrCharacterFiles wrapper class

class CDaSvrCharacterFiles : public COleDispatchDriver
{
public:
	CDaSvrCharacterFiles(){} // Calls COleDispatchDriver default constructor
	CDaSvrCharacterFiles(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaSvrCharacterFiles(const CDaSvrCharacterFiles& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaSvrCharacterFiles methods
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
	long get_Filter()
	{
		long result;
		InvokeHelper(0x60020003, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Filter(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020003, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_DefaultFilePath()
	{
		CString result;
		InvokeHelper(0x60020004, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_DefaultFileName()
	{
		CString result;
		InvokeHelper(0x60020005, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}

	// IDaSvrCharacterFiles properties
public:

};
