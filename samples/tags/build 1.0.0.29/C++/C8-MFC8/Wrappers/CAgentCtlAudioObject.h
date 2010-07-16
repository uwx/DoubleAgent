// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAgentCtlAudioObject wrapper class

class CAgentCtlAudioObject : public COleDispatchDriver
{
public:
	CAgentCtlAudioObject(){} // Calls COleDispatchDriver default constructor
	CAgentCtlAudioObject(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAgentCtlAudioObject(const CAgentCtlAudioObject& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IAgentCtlAudioObject methods
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

	// IAgentCtlAudioObject properties
public:

};
