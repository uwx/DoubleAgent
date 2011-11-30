// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCharacters wrapper class

class IAgentCtlCharacters : public COleDispatchDriver
{
public:
	IAgentCtlCharacters() {}		// Calls COleDispatchDriver default constructor
	IAgentCtlCharacters(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCtlCharacters(const IAgentCtlCharacters& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetItem(LPCTSTR CharacterID);
	LPDISPATCH Character(LPCTSTR CharacterID);
	void Unload(LPCTSTR CharacterID);
	LPDISPATCH Load(LPCTSTR CharacterID, const VARIANT& LoadKey);
};
/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCharacterEx wrapper class

class IAgentCtlCharacterEx : public COleDispatchDriver
{
public:
	IAgentCtlCharacterEx() {}		// Calls COleDispatchDriver default constructor
	IAgentCtlCharacterEx(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCtlCharacterEx(const IAgentCtlCharacterEx& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetBalloon();
	LPDISPATCH GetCommands();
	CString GetName();
	CString GetDescription();
	BOOL GetVisible();
	void SetLeft(short nNewValue);
	short GetLeft();
	void SetTop(short nNewValue);
	short GetTop();
	void SetHeight(short nNewValue);
	short GetHeight();
	void SetWidth(short nNewValue);
	short GetWidth();
	long GetSpeed();
	long GetPitch();
	void SetIdleOn(BOOL bNewValue);
	BOOL GetIdleOn();
	BOOL Activate(const VARIANT& State);
	LPDISPATCH Play(LPCTSTR Animation);
	LPDISPATCH Get(LPCTSTR Type, LPCTSTR Name, const VARIANT& Queue);
	void Stop(const VARIANT& Request);
	LPDISPATCH Wait(LPDISPATCH WaitForRequest);
	LPDISPATCH Interrupt(LPDISPATCH InterruptRequest);
	LPDISPATCH Speak(const VARIANT& Text, const VARIANT& Url);
	LPDISPATCH GestureAt(short x, short y);
	LPDISPATCH MoveTo(short x, short y, const VARIANT& Speed);
	LPDISPATCH Hide(const VARIANT& Fast);
	LPDISPATCH Show(const VARIANT& Fast);
	void StopAll(const VARIANT& Types);
	short GetMoveCause();
	short GetVisibilityCause();
	BOOL GetHasOtherClients();
	void SetSoundEffectsOn(BOOL bNewValue);
	BOOL GetSoundEffectsOn();
	void SetName(LPCTSTR lpszNewValue);
	void SetDescription(LPCTSTR lpszNewValue);
	CString GetExtraData();
	BOOL ShowPopupMenu(short x, short y);
	void SetAutoPopupMenu(BOOL bNewValue);
	BOOL GetAutoPopupMenu();
	void SetHelpModeOn(BOOL bNewValue);
	BOOL GetHelpModeOn();
	void SetHelpContextID(long nNewValue);
	long GetHelpContextID();
	short GetActive();
	BOOL Listen(BOOL Listen);
	void SetLanguageID(long nNewValue);
	long GetLanguageID();
	CString GetSRModeID();
	void SetSRModeID(LPCTSTR lpszNewValue);
	CString GetTTSModeID();
	void SetTTSModeID(LPCTSTR lpszNewValue);
	CString GetHelpFile();
	void SetHelpFile(LPCTSTR lpszNewValue);
	CString GetGuid();
	short GetOriginalHeight();
	short GetOriginalWidth();
	LPDISPATCH Think(LPCTSTR Text);
	CString GetVersion();
	LPDISPATCH GetAnimationNames();
	long GetSRStatus();
};
/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCharacter wrapper class

class IAgentCtlCharacter : public COleDispatchDriver
{
public:
	IAgentCtlCharacter() {}		// Calls COleDispatchDriver default constructor
	IAgentCtlCharacter(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCtlCharacter(const IAgentCtlCharacter& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetBalloon();
	LPDISPATCH GetCommands();
	CString GetName();
	CString GetDescription();
	BOOL GetVisible();
	void SetLeft(short nNewValue);
	short GetLeft();
	void SetTop(short nNewValue);
	short GetTop();
	void SetHeight(short nNewValue);
	short GetHeight();
	void SetWidth(short nNewValue);
	short GetWidth();
	long GetSpeed();
	long GetPitch();
	void SetIdleOn(BOOL bNewValue);
	BOOL GetIdleOn();
	BOOL Activate(const VARIANT& State);
	LPDISPATCH Play(LPCTSTR Animation);
	LPDISPATCH Get(LPCTSTR Type, LPCTSTR Name, const VARIANT& Queue);
	void Stop(const VARIANT& Request);
	LPDISPATCH Wait(LPDISPATCH WaitForRequest);
	LPDISPATCH Interrupt(LPDISPATCH InterruptRequest);
	LPDISPATCH Speak(const VARIANT& Text, const VARIANT& Url);
	LPDISPATCH GestureAt(short x, short y);
	LPDISPATCH MoveTo(short x, short y, const VARIANT& Speed);
	LPDISPATCH Hide(const VARIANT& Fast);
	LPDISPATCH Show(const VARIANT& Fast);
	void StopAll(const VARIANT& Types);
	short GetMoveCause();
	short GetVisibilityCause();
	BOOL GetHasOtherClients();
	void SetSoundEffectsOn(BOOL bNewValue);
	BOOL GetSoundEffectsOn();
	void SetName(LPCTSTR lpszNewValue);
	void SetDescription(LPCTSTR lpszNewValue);
	CString GetExtraData();
};
/////////////////////////////////////////////////////////////////////////////
// IAgentCtlBalloonEx wrapper class

class IAgentCtlBalloonEx : public COleDispatchDriver
{
public:
	IAgentCtlBalloonEx() {}		// Calls COleDispatchDriver default constructor
	IAgentCtlBalloonEx(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCtlBalloonEx(const IAgentCtlBalloonEx& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetEnabled();
	long GetNumberOfLines();
	long GetCharsPerLine();
	CString GetFontName();
	long GetFontSize();
	BOOL GetFontBold();
	BOOL GetFontItalic();
	BOOL GetFontStrikethru();
	BOOL GetFontUnderline();
	long GetForeColor();
	long GetBackColor();
	long GetBorderColor();
	void SetVisible(BOOL bNewValue);
	BOOL GetVisible();
	void SetFontName(LPCTSTR lpszNewValue);
	void SetFontSize(long nNewValue);
	void SetFontCharSet(short nNewValue);
	short GetFontCharSet();
	void SetStyle(long nNewValue);
	long GetStyle();
};
/////////////////////////////////////////////////////////////////////////////
// IAgentCtlBalloon wrapper class

class IAgentCtlBalloon : public COleDispatchDriver
{
public:
	IAgentCtlBalloon() {}		// Calls COleDispatchDriver default constructor
	IAgentCtlBalloon(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCtlBalloon(const IAgentCtlBalloon& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetEnabled();
	long GetNumberOfLines();
	long GetCharsPerLine();
	CString GetFontName();
	long GetFontSize();
	BOOL GetFontBold();
	BOOL GetFontItalic();
	BOOL GetFontStrikethru();
	BOOL GetFontUnderline();
	long GetForeColor();
	long GetBackColor();
	long GetBorderColor();
	void SetVisible(BOOL bNewValue);
	BOOL GetVisible();
	void SetFontName(LPCTSTR lpszNewValue);
	void SetFontSize(long nNewValue);
	void SetFontCharSet(short nNewValue);
	short GetFontCharSet();
};
/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCommandsEx wrapper class

class IAgentCtlCommandsEx : public COleDispatchDriver
{
public:
	IAgentCtlCommandsEx() {}		// Calls COleDispatchDriver default constructor
	IAgentCtlCommandsEx(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCtlCommandsEx(const IAgentCtlCommandsEx& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetItem(LPCTSTR Name);
	LPDISPATCH Command(LPCTSTR Name);
	long GetCount();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	CString GetVoice();
	void SetVoice(LPCTSTR lpszNewValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	LPDISPATCH Add(LPCTSTR Name, const VARIANT& Caption, const VARIANT& Voice, const VARIANT& Enabled, const VARIANT& Visible);
	LPDISPATCH Insert(LPCTSTR Name, LPCTSTR RefName, const VARIANT& Before, const VARIANT& Caption, const VARIANT& Voice, const VARIANT& Enabled, const VARIANT& Visible);
	void Remove(LPCTSTR Name);
	void RemoveAll();
	void SetDefaultCommand(LPCTSTR lpszNewValue);
	CString GetDefaultCommand();
	void SetHelpContextID(long nNewValue);
	long GetHelpContextID();
	void SetFontName(LPCTSTR lpszNewValue);
	CString GetFontName();
	long GetFontSize();
	void SetFontSize(long nNewValue);
	void SetVoiceCaption(LPCTSTR lpszNewValue);
	CString GetVoiceCaption();
	void SetGlobalVoiceCommandsEnabled(BOOL bNewValue);
	BOOL GetGlobalVoiceCommandsEnabled();
};
/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCommands wrapper class

class IAgentCtlCommands : public COleDispatchDriver
{
public:
	IAgentCtlCommands() {}		// Calls COleDispatchDriver default constructor
	IAgentCtlCommands(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCtlCommands(const IAgentCtlCommands& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetItem(LPCTSTR Name);
	LPDISPATCH Command(LPCTSTR Name);
	long GetCount();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	CString GetVoice();
	void SetVoice(LPCTSTR lpszNewValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	LPDISPATCH Add(LPCTSTR Name, const VARIANT& Caption, const VARIANT& Voice, const VARIANT& Enabled, const VARIANT& Visible);
	LPDISPATCH Insert(LPCTSTR Name, LPCTSTR RefName, const VARIANT& Before, const VARIANT& Caption, const VARIANT& Voice, const VARIANT& Enabled, const VARIANT& Visible);
	void Remove(LPCTSTR Name);
	void RemoveAll();
};
/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCommandEx wrapper class

class IAgentCtlCommandEx : public COleDispatchDriver
{
public:
	IAgentCtlCommandEx() {}		// Calls COleDispatchDriver default constructor
	IAgentCtlCommandEx(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCtlCommandEx(const IAgentCtlCommandEx& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString GetVoice();
	void SetVoice(LPCTSTR lpszNewValue);
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	long GetConfidence();
	void SetConfidence(long nNewValue);
	CString GetConfidenceText();
	void SetConfidenceText(LPCTSTR lpszNewValue);
	void SetHelpContextID(long nNewValue);
	long GetHelpContextID();
	void SetVoiceCaption(LPCTSTR lpszNewValue);
	CString GetVoiceCaption();
};
/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCommand wrapper class

class IAgentCtlCommand : public COleDispatchDriver
{
public:
	IAgentCtlCommand() {}		// Calls COleDispatchDriver default constructor
	IAgentCtlCommand(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCtlCommand(const IAgentCtlCommand& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString GetVoice();
	void SetVoice(LPCTSTR lpszNewValue);
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	long GetConfidence();
	void SetConfidence(long nNewValue);
	CString GetConfidenceText();
	void SetConfidenceText(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// IAgentCtlRequest wrapper class

class IAgentCtlRequest : public COleDispatchDriver
{
public:
	IAgentCtlRequest() {}		// Calls COleDispatchDriver default constructor
	IAgentCtlRequest(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCtlRequest(const IAgentCtlRequest& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetStatus();
	CString GetDescription();
	long GetNumber();
};
/////////////////////////////////////////////////////////////////////////////
// IAgentCtlAnimationNames wrapper class

class IAgentCtlAnimationNames : public COleDispatchDriver
{
public:
	IAgentCtlAnimationNames() {}		// Calls COleDispatchDriver default constructor
	IAgentCtlAnimationNames(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCtlAnimationNames(const IAgentCtlAnimationNames& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// IAgentCtlAudioObject wrapper class

class IAgentCtlAudioObject : public COleDispatchDriver
{
public:
	IAgentCtlAudioObject() {}		// Calls COleDispatchDriver default constructor
	IAgentCtlAudioObject(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCtlAudioObject(const IAgentCtlAudioObject& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetEnabled();
	BOOL GetSoundEffects();
};
/////////////////////////////////////////////////////////////////////////////
// IAgentCtlAudioObjectEx wrapper class

class IAgentCtlAudioObjectEx : public COleDispatchDriver
{
public:
	IAgentCtlAudioObjectEx() {}		// Calls COleDispatchDriver default constructor
	IAgentCtlAudioObjectEx(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCtlAudioObjectEx(const IAgentCtlAudioObjectEx& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetEnabled();
	BOOL GetSoundEffects();
	short GetStatus();
};
/////////////////////////////////////////////////////////////////////////////
// IAgentCtlSpeechInput wrapper class

class IAgentCtlSpeechInput : public COleDispatchDriver
{
public:
	IAgentCtlSpeechInput() {}		// Calls COleDispatchDriver default constructor
	IAgentCtlSpeechInput(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCtlSpeechInput(const IAgentCtlSpeechInput& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetEnabled();
	CString GetLanguage();
	CString GetHotKey();
	BOOL GetInstalled();
	CString GetEngine();
	void SetEngine(LPCTSTR lpszNewValue);
	BOOL GetListeningTip();
};
/////////////////////////////////////////////////////////////////////////////
// IAgentCtlPropertySheet wrapper class

class IAgentCtlPropertySheet : public COleDispatchDriver
{
public:
	IAgentCtlPropertySheet() {}		// Calls COleDispatchDriver default constructor
	IAgentCtlPropertySheet(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCtlPropertySheet(const IAgentCtlPropertySheet& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	short GetLeft();
	short GetTop();
	short GetHeight();
	short GetWidth();
	void SetVisible(BOOL bNewValue);
	BOOL GetVisible();
	void SetPage(LPCTSTR lpszNewValue);
	CString GetPage();
};
/////////////////////////////////////////////////////////////////////////////
// IAgentCtlUserInput wrapper class

class IAgentCtlUserInput : public COleDispatchDriver
{
public:
	IAgentCtlUserInput() {}		// Calls COleDispatchDriver default constructor
	IAgentCtlUserInput(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCtlUserInput(const IAgentCtlUserInput& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	short GetCount();
	CString GetName();
	CString GetCharacterID();
	long GetConfidence();
	CString GetVoice();
	CString GetAlt1Name();
	long GetAlt1Confidence();
	CString GetAlt1Voice();
	CString GetAlt2Name();
	long GetAlt2Confidence();
	CString GetAlt2Voice();
};
/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCommandsWindow wrapper class

class IAgentCtlCommandsWindow : public COleDispatchDriver
{
public:
	IAgentCtlCommandsWindow() {}		// Calls COleDispatchDriver default constructor
	IAgentCtlCommandsWindow(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCtlCommandsWindow(const IAgentCtlCommandsWindow& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	short GetLeft();
	short GetTop();
	short GetHeight();
	short GetWidth();
};
/////////////////////////////////////////////////////////////////////////////
// IAgentCtlEx wrapper class

class IAgentCtlEx : public CWnd
{
protected:
	DECLARE_DYNCREATE(IAgentCtlEx)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xD45FD31B, 0x5C6E, 0x11D1, { 0x9E, 0xC1, 0x00, 0xC0, 0x4F, 0xD7, 0x08, 0x1F } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attributes
public:

// Operations
public:
	LPDISPATCH GetCharacters();
	LPDISPATCH GetAudioOutput();
	LPDISPATCH GetSpeechInput();
	LPDISPATCH GetPropertySheet();
	LPDISPATCH GetCommandsWindow();
	BOOL GetConnected();
	void SetConnected(BOOL bNewValue);
	BOOL GetSuspended();
	void ShowDefaultCharacterProperties(const VARIANT& x, const VARIANT& y);
	BOOL GetRaiseRequestErrors();
	void SetRaiseRequestErrors(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// _AgentEvents wrapper class

class _AgentEvents : public COleDispatchDriver
{
public:
	_AgentEvents() {}		// Calls COleDispatchDriver default constructor
	_AgentEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_AgentEvents(const _AgentEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void ActivateInput(LPCTSTR CharacterID);
	void DeactivateInput(LPCTSTR CharacterID);
	void Click(LPCTSTR CharacterID, short Button, short Shift, short x, short y);
	void DblClick(LPCTSTR CharacterID, short Button, short Shift, short x, short y);
	void DragStart(LPCTSTR CharacterID, short Button, short Shift, short x, short y);
	void DragComplete(LPCTSTR CharacterID, short Button, short Shift, short x, short y);
	void Show(LPCTSTR CharacterID, short Cause);
	void Hide(LPCTSTR CharacterID, short Cause);
	void RequestStart(LPDISPATCH Request);
	void RequestComplete(LPDISPATCH Request);
	void Restart();
	void Shutdown();
	void Bookmark(long BookmarkID);
	void Command(LPDISPATCH UserInput);
	void IdleStart(LPCTSTR CharacterID);
	void IdleComplete(LPCTSTR CharacterID);
	void Move(LPCTSTR CharacterID, short x, short y, short Cause);
	void Size(LPCTSTR CharacterID, short Width, short Height);
	void BalloonShow(LPCTSTR CharacterID);
	void BalloonHide(LPCTSTR CharacterID);
	void HelpComplete(LPCTSTR CharacterID, LPCTSTR Name, short Cause);
	void ListenStart(LPCTSTR CharacterID);
	void ListenComplete(LPCTSTR CharacterID, short Cause);
	void DefaultCharacterChange(LPCTSTR GUID);
	void AgentPropertyChange();
	void ActiveClientChange(LPCTSTR CharacterID, BOOL Active);
};
