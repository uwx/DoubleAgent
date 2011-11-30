// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaCtlRequest wrapper class

class CDaCtlRequest : public COleDispatchDriver
{
public:
	CDaCtlRequest(){} // Calls COleDispatchDriver default constructor
	CDaCtlRequest(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaCtlRequest(const CDaCtlRequest& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaCtlRequest methods
public:
	long get_ID()
	{
		long result;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_Status()
	{
		long result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString get_Description()
	{
		CString result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long get_Number()
	{
		long result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}

	// IDaCtlRequest properties
public:

};
