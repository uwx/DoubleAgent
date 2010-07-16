// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAgentCtlAudioObjectEx wrapper class

class CAgentCtlAudioObjectEx : public COleDispatchDriver
{
public:
	CAgentCtlAudioObjectEx(){} // Calls COleDispatchDriver default constructor
	CAgentCtlAudioObjectEx(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAgentCtlAudioObjectEx(const CAgentCtlAudioObjectEx& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IAgentCtlAudioObjectEx methods
public:
	BOOL get_Enabled()
	{
		BOOL result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_SoundEffects()
	{
		BOOL result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	short get_Status()
	{
		short result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}

	// IAgentCtlAudioObjectEx properties
public:

};
