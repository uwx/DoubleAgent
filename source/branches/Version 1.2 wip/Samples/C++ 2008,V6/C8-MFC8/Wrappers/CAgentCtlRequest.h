// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAgentCtlRequest wrapper class

class CAgentCtlRequest : public COleDispatchDriver
{
public:
	CAgentCtlRequest(){} // Calls COleDispatchDriver default constructor
	CAgentCtlRequest(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAgentCtlRequest(const CAgentCtlRequest& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IAgentCtlRequest methods
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

	// IAgentCtlRequest properties
public:

};
