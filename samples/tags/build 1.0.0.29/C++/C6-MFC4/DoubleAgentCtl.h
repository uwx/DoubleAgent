// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// IDaCtlCharacters wrapper class

class IDaCtlCharacters : public COleDispatchDriver
{
public:
	IDaCtlCharacters() {}		// Calls COleDispatchDriver default constructor
	IDaCtlCharacters(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlCharacters(const IDaCtlCharacters& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
// IDaCtlCharacter wrapper class

class IDaCtlCharacter : public COleDispatchDriver
{
public:
	IDaCtlCharacter() {}		// Calls COleDispatchDriver default constructor
	IDaCtlCharacter(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlCharacter(const IDaCtlCharacter& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
	short GetActive();
	BOOL Listen(BOOL Listen);
	void SetLanguageID(long nNewValue);
	long GetLanguageID();
	CString GetSRModeID();
	void SetSRModeID(LPCTSTR lpszNewValue);
	CString GetTTSModeID();
	void SetTTSModeID(LPCTSTR lpszNewValue);
	CString GetGuid();
	short GetOriginalHeight();
	short GetOriginalWidth();
	LPDISPATCH Think(LPCTSTR Text);
	CString GetVersion();
	LPDISPATCH GetAnimationNames();
	long GetSRStatus();
};
/////////////////////////////////////////////////////////////////////////////
// IDaCtlBalloon wrapper class

class IDaCtlBalloon : public COleDispatchDriver
{
public:
	IDaCtlBalloon() {}		// Calls COleDispatchDriver default constructor
	IDaCtlBalloon(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlBalloon(const IDaCtlBalloon& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
// IDaCtlCommands wrapper class

class IDaCtlCommands : public COleDispatchDriver
{
public:
	IDaCtlCommands() {}		// Calls COleDispatchDriver default constructor
	IDaCtlCommands(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlCommands(const IDaCtlCommands& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
	CString GetDefaultCommand();
	void SetDefaultCommand(LPCTSTR lpszNewValue);
	CString GetFontName();
	void SetFontName(LPCTSTR lpszNewValue);
	long GetFontSize();
	void SetFontSize(long nNewValue);
	CString GetVoiceCaption();
	void SetVoiceCaption(LPCTSTR lpszNewValue);
	BOOL GetGlobalVoiceCommandsEnabled();
	void SetGlobalVoiceCommandsEnabled(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// IDaCtlCommand wrapper class

class IDaCtlCommand : public COleDispatchDriver
{
public:
	IDaCtlCommand() {}		// Calls COleDispatchDriver default constructor
	IDaCtlCommand(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlCommand(const IDaCtlCommand& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
	CString GetVoiceCaption();
	void SetVoiceCaption(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// IDaCtlRequest wrapper class

class IDaCtlRequest : public COleDispatchDriver
{
public:
	IDaCtlRequest() {}		// Calls COleDispatchDriver default constructor
	IDaCtlRequest(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlRequest(const IDaCtlRequest& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetStatus();
	CString GetDescription();
	long GetNumber();
};
/////////////////////////////////////////////////////////////////////////////
// IDaCtlAnimationNames wrapper class

class IDaCtlAnimationNames : public COleDispatchDriver
{
public:
	IDaCtlAnimationNames() {}		// Calls COleDispatchDriver default constructor
	IDaCtlAnimationNames(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlAnimationNames(const IDaCtlAnimationNames& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// IDaCtlAudioObject wrapper class

class IDaCtlAudioObject : public COleDispatchDriver
{
public:
	IDaCtlAudioObject() {}		// Calls COleDispatchDriver default constructor
	IDaCtlAudioObject(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlAudioObject(const IDaCtlAudioObject& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetEnabled();
	BOOL GetSoundEffects();
};
/////////////////////////////////////////////////////////////////////////////
// IDaCtlSpeechInput wrapper class

class IDaCtlSpeechInput : public COleDispatchDriver
{
public:
	IDaCtlSpeechInput() {}		// Calls COleDispatchDriver default constructor
	IDaCtlSpeechInput(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlSpeechInput(const IDaCtlSpeechInput& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetEnabled();
	CString GetHotKey();
	BOOL GetListeningTip();
};
/////////////////////////////////////////////////////////////////////////////
// IDaCtlPropertySheet wrapper class

class IDaCtlPropertySheet : public COleDispatchDriver
{
public:
	IDaCtlPropertySheet() {}		// Calls COleDispatchDriver default constructor
	IDaCtlPropertySheet(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlPropertySheet(const IDaCtlPropertySheet& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
// IDaCtlUserInput wrapper class

class IDaCtlUserInput : public COleDispatchDriver
{
public:
	IDaCtlUserInput() {}		// Calls COleDispatchDriver default constructor
	IDaCtlUserInput(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlUserInput(const IDaCtlUserInput& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
// IDaCtlCommandsWindow wrapper class

class IDaCtlCommandsWindow : public COleDispatchDriver
{
public:
	IDaCtlCommandsWindow() {}		// Calls COleDispatchDriver default constructor
	IDaCtlCommandsWindow(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlCommandsWindow(const IDaCtlCommandsWindow& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
// IDaControl wrapper class

class IDaControl : public CWnd
{
protected:
	DECLARE_DYNCREATE(IDaControl)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x1147E530, 0xA208, 0x11DE, { 0xAB, 0xF2, 0x00, 0x24, 0x21, 0x11, 0x6F, 0xB2 } };
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
	void ShowDefaultCharacterProperties(const VARIANT& x, const VARIANT& y);
	BOOL GetRaiseRequestErrors();
	void SetRaiseRequestErrors(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// _DaCtlEvents wrapper class

class _DaCtlEvents : public COleDispatchDriver
{
public:
	_DaCtlEvents() {}		// Calls COleDispatchDriver default constructor
	_DaCtlEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_DaCtlEvents(const _DaCtlEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
	void Bookmark(long BookmarkID);
	void Command(LPDISPATCH UserInput);
	void IdleStart(LPCTSTR CharacterID);
	void IdleComplete(LPCTSTR CharacterID);
	void Move(LPCTSTR CharacterID, short x, short y, short Cause);
	void Size(LPCTSTR CharacterID, short Width, short Height);
	void BalloonShow(LPCTSTR CharacterID);
	void BalloonHide(LPCTSTR CharacterID);
	void ListenStart(LPCTSTR CharacterID);
	void ListenComplete(LPCTSTR CharacterID, short Cause);
	void DefaultCharacterChange(LPCTSTR GUID);
	void AgentPropertyChange();
	void ActiveClientChange(LPCTSTR CharacterID, BOOL Active);
};
