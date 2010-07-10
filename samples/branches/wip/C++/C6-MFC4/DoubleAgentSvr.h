// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// IDaSvrUserInput wrapper class

class IDaSvrUserInput : public COleDispatchDriver
{
public:
	IDaSvrUserInput() {}		// Calls COleDispatchDriver default constructor
	IDaSvrUserInput(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaSvrUserInput(const IDaSvrUserInput& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void GetAllItemData(VARIANT* ItemIndices, VARIANT* ItemConfidences, VARIANT* ItemText);
	long GetCount();
	long GetItemCommandID(long ItemIndex);
	long GetItemConfidence(long ItemIndex);
	CString GetItemText(long ItemIndex);
};
/////////////////////////////////////////////////////////////////////////////
// IDaSvrCharacterFiles wrapper class

class IDaSvrCharacterFiles : public COleDispatchDriver
{
public:
	IDaSvrCharacterFiles() {}		// Calls COleDispatchDriver default constructor
	IDaSvrCharacterFiles(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaSvrCharacterFiles(const IDaSvrCharacterFiles& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	// method 'GetFilePaths' not emitted because of invalid return type or parameter type
	CString GetSearchPath();
	void SetSearchPath(LPCTSTR lpszNewValue);
	CString GetDefaultSearchPath();
	long GetFilter();
	void SetFilter(long nNewValue);
	CString GetDefaultFilePath();
	CString GetDefaultFileName();
};
/////////////////////////////////////////////////////////////////////////////
// IDaSvrTTSEngine wrapper class

class IDaSvrTTSEngine : public COleDispatchDriver
{
public:
	IDaSvrTTSEngine() {}		// Calls COleDispatchDriver default constructor
	IDaSvrTTSEngine(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaSvrTTSEngine(const IDaSvrTTSEngine& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString GetTTSModeID();
	CString GetDisplayName();
	CString GetManufacturer();
	void GetVersion(short* MajorVersion, short* MinorVersion);
	long GetGender();
	long GetLanguageID();
	CString GetLanguageName(BOOL EnglishName);
};
/////////////////////////////////////////////////////////////////////////////
// IDaSvrTTSEngines wrapper class

class IDaSvrTTSEngines : public COleDispatchDriver
{
public:
	IDaSvrTTSEngines() {}		// Calls COleDispatchDriver default constructor
	IDaSvrTTSEngines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaSvrTTSEngines(const IDaSvrTTSEngines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetItem(long Index);
	long GetCount();
};
/////////////////////////////////////////////////////////////////////////////
// IDaSvrSREngine wrapper class

class IDaSvrSREngine : public COleDispatchDriver
{
public:
	IDaSvrSREngine() {}		// Calls COleDispatchDriver default constructor
	IDaSvrSREngine(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaSvrSREngine(const IDaSvrSREngine& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString GetSRModeID();
	CString GetDisplayName();
	CString GetManufacturer();
	void GetVersion(short* MajorVersion, short* MinorVersion);
	long GetLanguageID();
	CString GetLanguageName(BOOL EnglishName);
	// method 'GetLanguageIDs' not emitted because of invalid return type or parameter type
	// method 'GetLanguageNames' not emitted because of invalid return type or parameter type
};
/////////////////////////////////////////////////////////////////////////////
// IDaSvrSREngines wrapper class

class IDaSvrSREngines : public COleDispatchDriver
{
public:
	IDaSvrSREngines() {}		// Calls COleDispatchDriver default constructor
	IDaSvrSREngines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaSvrSREngines(const IDaSvrSREngines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetItem(long Index);
	long GetCount();
};
/////////////////////////////////////////////////////////////////////////////
// IDaServer wrapper class

class IDaServer : public COleDispatchDriver
{
public:
	IDaServer() {}		// Calls COleDispatchDriver default constructor
	IDaServer(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaServer(const IDaServer& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void Load(const VARIANT& LoadKey, long* CharacterID, long* RequestID);
	void Unload(long CharacterID);
	void GetVersion(short* MajorVersion, short* MinorVersion);
	LPDISPATCH GetCharacter(long CharacterID);
	LPDISPATCH GetCharacterFiles();
	long GetCharacterStyle();
	void SetCharacterStyle(long nNewValue);
	LPDISPATCH GetTTSEngines();
	LPDISPATCH FindTTSEngines(long LanguageID, short Gender);
	LPDISPATCH GetCharacterTTSEngine(const VARIANT& LoadKey);
	LPDISPATCH FindCharacterTTSEngines(const VARIANT& LoadKey, long LanguageID);
	LPDISPATCH GetSREngines();
	LPDISPATCH FindSREngines(long LanguageID);
	LPDISPATCH GetCharacterSREngine(const VARIANT& LoadKey);
	LPDISPATCH FindCharacterSREngines(const VARIANT& LoadKey, long LanguageID);
	LPDISPATCH GetPropertySheet();
	LPDISPATCH GetCommandsWindow();
	LPDISPATCH GetSettings();
};
/////////////////////////////////////////////////////////////////////////////
// IDaSvrCharacter wrapper class

class IDaSvrCharacter : public COleDispatchDriver
{
public:
	IDaSvrCharacter() {}		// Calls COleDispatchDriver default constructor
	IDaSvrCharacter(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaSvrCharacter(const IDaSvrCharacter& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void SetPosition(long Left, long Top);
	void GetPosition(long* Left, long* Top);
	void SetSize(long Width, long Height);
	void GetSize(long* Width, long* Height);
	void Activate(short State);
	void Prepare(long Type, LPCTSTR Name, long Queue, long* RequestID);
	void Play(LPCTSTR Animation, long* RequestID);
	void Stop(long RequestID);
	void StopAll(long Types);
	void Wait(long WaitForRequestID, long* RequestID);
	void Interrupt(long InterruptRequestID, long* RequestID);
	void Show(long Fast, long* RequestID);
	void Hide(long Fast, long* RequestID);
	void Speak(LPCTSTR Text, LPCTSTR Url, long* RequestID);
	void MoveTo(short x, short y, long Speed, long* RequestID);
	void GestureAt(short x, short y, long* RequestID);
	void ShowPopupMenu(short x, short y);
	void Listen(long Listen);
	void GetOriginalSize(long* Width, long* Height);
	void Think(LPCTSTR Text, long* RequestID);
	void GetVersion(short* MajorVersion, short* MinorVersion);
	void GetAnimationNames(LPUNKNOWN* punkEnum);
	LPDISPATCH GetBalloon();
	LPDISPATCH GetCommands();
	long GetStyle();
	void SetStyle(long nNewValue);
	BOOL GetHasIcon();
	void GenerateIcon(long ClipLeft, long ClipTop, long ClipWidth, long ClipHeight);
	BOOL GetIconVisible();
	CString GetIconIdentity();
	void SetIconIdentity(LPCTSTR lpszNewValue);
	CString GetIconTip();
	void SetIconTip(LPCTSTR lpszNewValue);
	LPDISPATCH GetTTSEngine(BOOL GetDefault);
	LPDISPATCH FindTTSEngines(long LanguageID);
	LPDISPATCH GetSREngine(BOOL GetDefault);
	LPDISPATCH FindSREngines(long LanguageID);
	long GetCharacterID();
	CString GetUniqueID();
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	CString GetDescription();
	void SetDescription(LPCTSTR lpszNewValue);
	CString GetExtraData();
	CString GetFileName();
	CString GetFilePath();
	long GetLanguageID();
	void SetLanguageID(long nNewValue);
	CString GetTTSModeID();
	void SetTTSModeID(LPCTSTR lpszNewValue);
	CString GetSRModeID();
	void SetSRModeID(LPCTSTR lpszNewValue);
	short GetLeft();
	void SetLeft(short nNewValue);
	short GetTop();
	void SetTop(short nNewValue);
	short GetWidth();
	void SetWidth(short nNewValue);
	short GetHeight();
	void SetHeight(short nNewValue);
	short GetOriginalWidth();
	short GetOriginalHeight();
	BOOL GetVisible();
	long GetActiveState();
	void SetActiveState(long nNewValue);
	BOOL GetIdleState();
	long GetOtherClientCount();
	long GetMoveCause();
	long GetVisibilityCause();
	long GetListeningStatus();
	LPDISPATCH GetAnimationNames();
};
/////////////////////////////////////////////////////////////////////////////
// IDaSvrBalloon wrapper class

class IDaSvrBalloon : public COleDispatchDriver
{
public:
	IDaSvrBalloon() {}		// Calls COleDispatchDriver default constructor
	IDaSvrBalloon(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaSvrBalloon(const IDaSvrBalloon& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetEnabled();
	long GetStyle();
	void SetStyle(long nNewValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	long GetNumberOfLines();
	void SetNumberOfLines(long nNewValue);
	long GetCharsPerLine();
	void SetCharsPerLine(long nNewValue);
	long GetTextColor();
	void SetTextColor(long nNewValue);
	long GetBackColor();
	void SetBackColor(long nNewValue);
	long GetBorderColor();
	void SetBorderColor(long nNewValue);
	CString GetFontName();
	void SetFontName(LPCTSTR lpszNewValue);
	long GetFontSize();
	void SetFontSize(long nNewValue);
	BOOL GetFontBold();
	void SetFontBold(BOOL bNewValue);
	BOOL GetFontItalic();
	void SetFontItalic(BOOL bNewValue);
	BOOL GetFontStrikethru();
	void SetFontStrikethru(BOOL bNewValue);
	BOOL GetFontUnderline();
	void SetFontUnderline(BOOL bNewValue);
	short GetFontCharSet();
	void SetFontCharSet(short nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommands wrapper class

class IDaSvrCommands : public COleDispatchDriver
{
public:
	IDaSvrCommands() {}		// Calls COleDispatchDriver default constructor
	IDaSvrCommands(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaSvrCommands(const IDaSvrCommands& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void Add(LPCTSTR Caption, LPCTSTR VoiceGrammar, long Enabled, long Visible, long* CommandID);
	void Insert(LPCTSTR Caption, LPCTSTR VoiceGrammar, long Enabled, long Visible, long RefCommandID, long Before, long* CommandID);
	void Remove(long CommandID);
	void RemoveAll();
	void AddEx(LPCTSTR Caption, LPCTSTR VoiceGrammar, LPCTSTR VoiceCaption, long Enabled, long Visible, long HelpContextID, long* CommandID);
	void InsertEx(LPCTSTR Caption, LPCTSTR VoiceGrammar, LPCTSTR VoiceCaption, long Enabled, long Visible, long HelpContextID, long RefCommandID, long Before, long* CommandID);
	LPDISPATCH GetItem(long Index);
	long GetCount();
	LPDISPATCH GetCommand(long CommandID);
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	CString GetVoiceGrammar();
	void SetVoiceGrammar(LPCTSTR lpszNewValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	long GetDefaultCommand();
	void SetDefaultCommand(long nNewValue);
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
// IDaSvrCommand wrapper class

class IDaSvrCommand : public COleDispatchDriver
{
public:
	IDaSvrCommand() {}		// Calls COleDispatchDriver default constructor
	IDaSvrCommand(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaSvrCommand(const IDaSvrCommand& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	long GetCommandID();
	CString GetVoiceCaption();
	void SetVoiceCaption(LPCTSTR lpszNewValue);
	CString GetVoiceGrammar();
	void SetVoiceGrammar(LPCTSTR lpszNewValue);
	long GetConfidenceThreshold();
	void SetConfidenceThreshold(long nNewValue);
	CString GetConfidenceText();
	void SetConfidenceText(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// IDaSvrPropertySheet wrapper class

class IDaSvrPropertySheet : public COleDispatchDriver
{
public:
	IDaSvrPropertySheet() {}		// Calls COleDispatchDriver default constructor
	IDaSvrPropertySheet(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaSvrPropertySheet(const IDaSvrPropertySheet& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void GetPosition(long* Left, long* Top);
	void GetSize(long* Width, long* Height);
	void SetLeft(short nNewValue);
	short GetLeft();
	void SetTop(short nNewValue);
	short GetTop();
	short GetHeight();
	short GetWidth();
	void SetVisible(BOOL bNewValue);
	BOOL GetVisible();
	void SetPage(LPCTSTR lpszNewValue);
	CString GetPage();
	void SetPosition(long Left, long Top);
};
/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommandsWindow wrapper class

class IDaSvrCommandsWindow : public COleDispatchDriver
{
public:
	IDaSvrCommandsWindow() {}		// Calls COleDispatchDriver default constructor
	IDaSvrCommandsWindow(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaSvrCommandsWindow(const IDaSvrCommandsWindow& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void GetPosition(long* Left, long* Top);
	void GetSize(long* Width, long* Height);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	short GetLeft();
	short GetTop();
	short GetHeight();
	short GetWidth();
};
/////////////////////////////////////////////////////////////////////////////
// IDaSvrSetting wrapper class

class IDaSvrSetting : public COleDispatchDriver
{
public:
	IDaSvrSetting() {}		// Calls COleDispatchDriver default constructor
	IDaSvrSetting(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaSvrSetting(const IDaSvrSetting& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetSoundEffectsEnabled();
	BOOL GetBalloonEnabled();
	LPDISPATCH GetBalloonFont();
	BOOL GetTTSEnabled();
	short GetTTSSpeed();
	BOOL GetSREnabled();
	CString GetSRHotKey();
	short GetSRHotKeyTime();
	BOOL GetSRListeningTip();
	BOOL GetSRListeningPrompt();
	long GetAudioStatus();
};
