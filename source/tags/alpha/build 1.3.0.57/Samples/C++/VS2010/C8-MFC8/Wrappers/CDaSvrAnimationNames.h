// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaSvrAnimationNames wrapper class

class CDaSvrAnimationNames : public COleDispatchDriver
{
public:
	CDaSvrAnimationNames(){} // Calls COleDispatchDriver default constructor
	CDaSvrAnimationNames(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaSvrAnimationNames(const CDaSvrAnimationNames& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaSvrAnimationNames methods
public:
	LPUNKNOWN get__NewEnum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	CString get_Item(long Index)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, Index);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0xfffffff8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}

	// IDaSvrAnimationNames properties
public:

};
