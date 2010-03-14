// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaCtlPropertySheet wrapper class

class CDaCtlPropertySheet : public COleDispatchDriver
{
public:
	CDaCtlPropertySheet(){} // Calls COleDispatchDriver default constructor
	CDaCtlPropertySheet(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaCtlPropertySheet(const CDaCtlPropertySheet& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaCtlPropertySheet methods
public:
	short get_Left()
	{
		short result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	short get_Top()
	{
		short result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	short get_Height()
	{
		short result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	short get_Width()
	{
		short result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	void put_Visible(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_Visible()
	{
		BOOL result;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Page(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_Page()
	{
		CString result;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}

	// IDaCtlPropertySheet properties
public:

};
