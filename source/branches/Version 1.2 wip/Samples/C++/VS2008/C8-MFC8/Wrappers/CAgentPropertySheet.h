// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAgentPropertySheet wrapper class

class CAgentPropertySheet : public COleDispatchDriver
{
public:
	CAgentPropertySheet(){} // Calls COleDispatchDriver default constructor
	CAgentPropertySheet(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAgentPropertySheet(const CAgentPropertySheet& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IAgentPropertySheet methods
public:
	void GetVisible(long * pbVisible)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbVisible);
	}
	void SetVisible(long bVisible)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bVisible);
	}
	void GetPosition(long * plLeft, long * plTop)
	{
		static BYTE parms[] = VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, plLeft, plTop);
	}
	void GetSize(long * plWidth, long * plHeight)
	{
		static BYTE parms[] = VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, plWidth, plHeight);
	}
	void GetPage(BSTR * pbszPage)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbszPage);
	}
	void SetPage(LPCTSTR bszPage)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszPage);
	}

	// IAgentPropertySheet properties
public:

};
