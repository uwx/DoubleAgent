
[helpstring("A character has become input-active"), helpcontext(APIHELP_Ctl_Event_ActivateInput),
	id(DISPID_AgentEvents_ActivateInput)] void ActivateInput ([in] BSTR CharacterID);
[helpstring("A character is no longer input-active"), helpcontext(APIHELP_Ctl_Event_DeactivateInput),
	id(DISPID_AgentEvents_DeactivateInput)] void DeactivateInput ([in] BSTR CharacterID);
[helpstring("The user has clicked on a character"), helpcontext(APIHELP_Ctl_Event_Click),
	id(DISPID_AgentEvents_Click)] void Click ([in] BSTR CharacterID, [in] short Button, [in] short Shift, [in] short X, [in] short y);
[helpstring("The user has double-clicked on a character"), helpcontext(APIHELP_Ctl_Event_DblClick),
	id(DISPID_AgentEvents_DblClick)] void DblClick ([in] BSTR CharacterID, [in] short Button, [in] short Shift, [in] short X, [in] short y);
[helpstring("The user has started dragging a character"), helpcontext(APIHELP_Ctl_Event_DragStart),
	id(DISPID_AgentEvents_DragStart)] void DragStart ([in] BSTR CharacterID, [in] short Button, [in] short Shift, [in] short X, [in] short y);
[helpstring("The user has finished dragging a character"), helpcontext(APIHELP_Ctl_Event_DragComplete),
	id(DISPID_AgentEvents_DragComplete)] void DragComplete ([in] BSTR CharacterID, [in] short Button, [in] short Shift, [in] short X, [in] short y);
[helpstring("A character was shown"), helpcontext(APIHELP_Ctl_Event_Show),
	id(DISPID_AgentEvents_Show)] void Show ([in] BSTR CharacterID, [in] VisibilityCauseType Cause);
[helpstring("A character was hidden"), helpcontext(APIHELP_Ctl_Event_Hide),
	id(DISPID_AgentEvents_Hide)] void Hide ([in] BSTR CharacterID, [in] VisibilityCauseType Cause);
[helpstring("A server request has started"), helpcontext(APIHELP_Ctl_Event_RequestStart),
	id(DISPID_AgentEvents_RequestStart)] void RequestStart ([in] IDaCtlRequest* Request);
[helpstring("A server request is complete (with or without errors)"), helpcontext(APIHELP_Ctl_Event_RequestComplete),
	id(DISPID_AgentEvents_RequestComplete)] void RequestComplete ([in] IDaCtlRequest* Request);
[helpstring("deprecated"), helpcontext(APIHELP_Ctl_Event_Restart),
	id(DISPID_AgentEvents_Restart), hidden, restricted, nonbrowsable] void Restart ();
[helpstring("deprecated"), helpcontext(APIHELP_Ctl_Event_Shutdown),
	id(DISPID_AgentEvents_Shutdown), hidden, restricted, nonbrowsable] void Shutdown ();
[helpstring("A text-to-speech bookmark was reached"), helpcontext(APIHELP_Ctl_Event_Bookmark),
	id(DISPID_AgentEvents_Bookmark)] void Bookmark ([in] long BookmarkID);
[helpstring("A command was selected"), helpcontext(APIHELP_Ctl_Event_Command),
	id(DISPID_AgentEvents_Command)] void Command ([in] IDaCtlUserInput* UserInput);
[helpstring("A character has started idling"), helpcontext(APIHELP_Ctl_Event_IdleStart),
	id(DISPID_AgentEvents_IdleStart)] void IdleStart ([in] BSTR CharacterID);
[helpstring("A character has stopped idling"), helpcontext(APIHELP_Ctl_Event_IdleComplete),
	id(DISPID_AgentEvents_IdleComplete)] void IdleComplete ([in] BSTR CharacterID);
[helpstring("A character was moved"), helpcontext(APIHELP_Ctl_Event_Move),
	id(DISPID_AgentEvents_Move)] void Move ([in] BSTR CharacterID, [in] short X, [in] short y, [in] MoveCauseType Cause);
[helpstring("A character was resized"), helpcontext(APIHELP_Ctl_Event_Size),
	id(DISPID_AgentEvents_Size)] void Size ([in] BSTR CharacterID, [in] short Width, [in] short Height);
[helpstring("A character's word balloon was shown"), helpcontext(APIHELP_Ctl_Event_BalloonShow),
	id(DISPID_AgentEvents_BalloonShow)] void BalloonShow([in] BSTR CharacterID);
[helpstring("A character's word balloon was hidden"), helpcontext(APIHELP_Ctl_Event_BalloonHide),
	id(DISPID_AgentEvents_BalloonHide)] void BalloonHide([in] BSTR CharacterID);
[helpstring("deprecated"), helpcontext(APIHELP_Ctl_Event_HelpComplete),
	id(DISPID_AgentEvents_HelpComplete), hidden, restricted, nonbrowsable] void HelpComplete ([in] BSTR CharacterID, [in] BSTR Name, [in] short Cause);
[helpstring("A character has started listening for speech recognition"), helpcontext(APIHELP_Ctl_Event_ListenStart),
	id(DISPID_AgentEvents_ListenStart)] void ListenStart ([in] BSTR CharacterID);
[helpstring("A character has stopped listening for speech recognition"), helpcontext(APIHELP_Ctl_Event_ListenComplete),
	id(DISPID_AgentEvents_ListenComplete)] void ListenComplete ([in] BSTR CharacterID, [in] ListenCompleteType Cause);
[helpstring("The default character has changed"), helpcontext(APIHELP_Ctl_Event_DefaultCharacterChange),
	id(DISPID_AgentEvents_DefaultCharacterChange)] void DefaultCharacterChange ([in] BSTR CharGUID);
[helpstring("A global property has changed"), helpcontext(APIHELP_Ctl_Event_AgentPropertyChange),
	id(DISPID_AgentEvents_AgentPropertyChange)] void AgentPropertyChange (void);
[helpstring("A character's active client has changed."), helpcontext(APIHELP_Ctl_Event_ActiveClientChange),
	id(DISPID_AgentEvents_ActiveClientChange)] void ActiveClientChange ([in] BSTR CharacterID, [in] VARIANT_BOOL Active);
