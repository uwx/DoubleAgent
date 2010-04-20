// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAgentCharacterEx wrapper class

class CAgentCharacterEx : public COleDispatchDriver
{
public:
	CAgentCharacterEx(){} // Calls COleDispatchDriver default constructor
	CAgentCharacterEx(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAgentCharacterEx(const CAgentCharacterEx& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IAgentCharacterEx methods
public:
	void GetVisible(long * pbVisible)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbVisible);
	}
	void SetPosition(long lLeft, long lTop)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lLeft, lTop);
	}
	void GetPosition(long * plLeft, long * plTop)
	{
		static BYTE parms[] = VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, plLeft, plTop);
	}
	void SetSize(long lWidth, long lHeight)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lWidth, lHeight);
	}
	void GetSize(long * plWidth, long * plHeight)
	{
		static BYTE parms[] = VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, plWidth, plHeight);
	}
	void GetName(BSTR * pbszName)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbszName);
	}
	void GetDescription(BSTR * pbszDescription)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbszDescription);
	}
	void GetTTSSpeed(long * pdwSpeed)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pdwSpeed);
	}
	void GetTTSPitch(short * pwPitch)
	{
		static BYTE parms[] = VTS_PI2 ;
		InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pwPitch);
	}
	void Activate(short sState)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sState);
	}
	void SetIdleOn(long bOn)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bOn);
	}
	void GetIdleOn(long * pbOn)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbOn);
	}
	void Prepare(long dwType, LPCTSTR bszName, long bQueue, long * pdwReqID)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_I4 VTS_PI4 ;
		InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwType, bszName, bQueue, pdwReqID);
	}
	void Play(LPCTSTR bszAnimation, long * pdwReqID)
	{
		static BYTE parms[] = VTS_BSTR VTS_PI4 ;
		InvokeHelper(0x6002000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszAnimation, pdwReqID);
	}
	void Stop(long dwReqID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6002000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwReqID);
	}
	void StopAll(long lTypes)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6002000f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lTypes);
	}
	void Wait(long dwReqID, long * pdwReqID)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0x60020010, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwReqID, pdwReqID);
	}
	void Interrupt(long dwReqID, long * pdwReqID)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0x60020011, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwReqID, pdwReqID);
	}
	void Show(long bFast, long * pdwReqID)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0x60020012, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bFast, pdwReqID);
	}
	void Hide(long bFast, long * pdwReqID)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0x60020013, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bFast, pdwReqID);
	}
	void Speak(LPCTSTR bszText, LPCTSTR bszUrl, long * pdwReqID)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_PI4 ;
		InvokeHelper(0x60020014, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszText, bszUrl, pdwReqID);
	}
	void MoveTo(short x, short y, long lSpeed, long * pdwReqID)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x60020015, DISPATCH_METHOD, VT_EMPTY, NULL, parms, x, y, lSpeed, pdwReqID);
	}
	void GestureAt(short x, short y, long * pdwReqID)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_PI4 ;
		InvokeHelper(0x60020016, DISPATCH_METHOD, VT_EMPTY, NULL, parms, x, y, pdwReqID);
	}
	void GetMoveCause(long * pdwCause)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020017, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pdwCause);
	}
	void GetVisibilityCause(long * pdwCause)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020018, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pdwCause);
	}
	void HasOtherClients(long * plNumOtherClients)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020019, DISPATCH_METHOD, VT_EMPTY, NULL, parms, plNumOtherClients);
	}
	void SetSoundEffectsOn(long bOn)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6002001a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bOn);
	}
	void GetSoundEffectsOn(long * pbOn)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x6002001b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbOn);
	}
	void SetName(LPCTSTR bszName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6002001c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszName);
	}
	void SetDescription(LPCTSTR bszDescription)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6002001d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszDescription);
	}
	void GetExtraData(BSTR * pbszExtraData)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x6002001e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbszExtraData);
	}
	void ShowPopupMenu(short x, short y)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, x, y);
	}
	void SetAutoPopupMenu(long bAutoPopupMenu)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bAutoPopupMenu);
	}
	void GetAutoPopupMenu(long * pbAutoPopupMenu)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbAutoPopupMenu);
	}
	void GetHelpFileName(BSTR * pbszName)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60030003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbszName);
	}
	void SetHelpFileName(LPCTSTR bszName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszName);
	}
	void SetHelpModeOn(long bHelpModeOn)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030005, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bHelpModeOn);
	}
	void GetHelpModeOn(long * pbHelpModeOn)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60030006, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbHelpModeOn);
	}
	void SetHelpContextID(long ulID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030007, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ulID);
	}
	void GetHelpContextID(long * pulID)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60030008, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pulID);
	}
	void GetActive(short * psState)
	{
		static BYTE parms[] = VTS_PI2 ;
		InvokeHelper(0x60030009, DISPATCH_METHOD, VT_EMPTY, NULL, parms, psState);
	}
	void Listen(long bListen)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bListen);
	}
	void SetLanguageID(long langid)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6003000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, langid);
	}
	void GetLanguageID(long * plangid)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x6003000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, plangid);
	}
	void GetTTSModeID(BSTR * pbszModeID)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x6003000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbszModeID);
	}
	void SetTTSModeID(LPCTSTR bszModeID)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6003000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszModeID);
	}
	void GetSRModeID(BSTR * pbszModeID)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x6003000f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbszModeID);
	}
	void SetSRModeID(LPCTSTR bszModeID)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030010, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszModeID);
	}
	void GetGUID(BSTR * pbszID)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60030011, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbszID);
	}
	void GetOriginalSize(long * plWidth, long * plHeight)
	{
		static BYTE parms[] = VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x60030012, DISPATCH_METHOD, VT_EMPTY, NULL, parms, plWidth, plHeight);
	}
	void Think(LPCTSTR bszText, long * pdwReqID)
	{
		static BYTE parms[] = VTS_BSTR VTS_PI4 ;
		InvokeHelper(0x60030013, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszText, pdwReqID);
	}
	void GetVersion(short * psMajor, short * psMinor)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x60030014, DISPATCH_METHOD, VT_EMPTY, NULL, parms, psMajor, psMinor);
	}
	void GetAnimationNames(LPUNKNOWN * punkEnum)
	{
		static BYTE parms[] = VTS_PUNKNOWN ;
		InvokeHelper(0x60030015, DISPATCH_METHOD, VT_EMPTY, NULL, parms, punkEnum);
	}
	void GetSRStatus(long * plStatus)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60030016, DISPATCH_METHOD, VT_EMPTY, NULL, parms, plStatus);
	}

	// IAgentCharacterEx properties
public:

};
