// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaCtlAnimationNames wrapper class

class CDaCtlAnimationNames : public COleDispatchDriver
{
public:
	CDaCtlAnimationNames(){} // Calls COleDispatchDriver default constructor
	CDaCtlAnimationNames(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaCtlAnimationNames(const CDaCtlAnimationNames& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaCtlAnimationNames methods
public:
	LPUNKNOWN get_Enum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}

	// IDaCtlAnimationNames properties
public:

};
