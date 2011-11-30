[helpstring("Speech output started"), helpcontext(APIHELP_Ctl_Event_SpeechStart),
	id(DISPID_DaCtlEvents2_SpeechStart)] void SpeechStart ([in] BSTR CharacterID, [in] IDaCtlFormattedText* FormattedText);
[helpstring("Speech output ended"), helpcontext(APIHELP_Ctl_Event_SpeechEnd),
	id(DISPID_DaCtlEvents2_SpeechEnd)] void SpeechEnd ([in] BSTR CharacterID, [in] IDaCtlFormattedText* FormattedText, [in] VARIANT_BOOL Stopped);
[helpstring("Speech output reached a word boundary"), helpcontext(APIHELP_Ctl_Event_SpeechWord),
	id(DISPID_DaCtlEvents2_SpeechWord)] void SpeechWord ([in] BSTR CharacterID, [in] IDaCtlFormattedText* FormattedText, [in] long WordIndex);
