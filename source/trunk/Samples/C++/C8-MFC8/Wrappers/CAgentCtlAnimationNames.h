// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAgentCtlAnimationNames wrapper class

class CAgentCtlAnimationNames : public COleDispatchDriver
{
public:
	CAgentCtlAnimationNames(){} // Calls COleDispatchDriver default constructor
	CAgentCtlAnimationNames(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAgentCtlAnimationNames(const CAgentCtlAnimationNames& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IAgentCtlAnimationNames methods
public:
	LPUNKNOWN get_Enum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}

	// IAgentCtlAnimationNames properties
public:

};
