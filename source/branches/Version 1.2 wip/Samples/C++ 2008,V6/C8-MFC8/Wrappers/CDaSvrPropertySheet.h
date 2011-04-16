// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaSvrPropertySheet wrapper class

class CDaSvrPropertySheet : public COleDispatchDriver
{
public:
	CDaSvrPropertySheet(){} // Calls COleDispatchDriver default constructor
	CDaSvrPropertySheet(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaSvrPropertySheet(const CDaSvrPropertySheet& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaSvrPropertySheet2 methods
public:
	void GetVisible(long * Visible)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Visible);
	}
	void SetVisible(long Visible)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Visible);
	}
	void GetPosition(long * Left, long * Top)
	{
		static BYTE parms[] = VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Left, Top);
	}
	void GetSize(long * Width, long * Height)
	{
		static BYTE parms[] = VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Width, Height);
	}
	void GetPage(BSTR * Page)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Page);
	}
	void SetPage(LPCTSTR Page)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Page);
	}
	void put_Left(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short get_Left()
	{
		short result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	void put_Top(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
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
	void SetPosition(long Left, long Top)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Left, Top);
	}

	// IDaSvrPropertySheet2 properties
public:

};
