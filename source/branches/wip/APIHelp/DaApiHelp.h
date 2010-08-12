/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#ifndef _DAAPIHELP_H
#define _DAAPIHELP_H
/////////////////////////////////////////////////////////////////////////////

#define	APIHELP_Svr_Server									0x00010000
#define	APIHELP_Svr_Server_Load								0x00010001
#define	APIHELP_Svr_Server_Unload							0x00010002
#define	APIHELP_Svr_Server_Register							0x00010003
#define	APIHELP_Svr_Server_Unregister						0x00010004
#define	APIHELP_Svr_Server_GetCharacter						0x00010005
#define	APIHELP_Svr_Server_GetSuspended						0x00010006
#define	APIHELP_Svr_Server_GetCharacterEx					0x00010007
#define	APIHELP_Svr_Server_GetVersion						0x00010008
#define	APIHELP_Svr_Server_ShowDefaultCharacterProperties	0x00010009
#define	APIHELP_Svr_Server_Character						0x0001000A
#define	APIHELP_Svr_Server_CharacterFiles					0x0001000B
#define	APIHELP_Svr_Server_CharacterStyle					0x0001000C
#define	APIHELP_Svr_Server_TTSEngines						0x0001000D
#define	APIHELP_Svr_Server_FindTTSEngines					0x0001000E
#define	APIHELP_Svr_Server_GetCharacterTTSEngine			0x0001000F
#define	APIHELP_Svr_Server_FindCharacterTTSEngines			0x00010010
#define	APIHELP_Svr_Server_SREngines						0x00010011
#define	APIHELP_Svr_Server_FindSREngines					0x00010012
#define	APIHELP_Svr_Server_GetCharacterSREngine				0x00010013
#define	APIHELP_Svr_Server_FindCharacterSREngines			0x00010014
#define	APIHELP_Svr_Server_PropertySheet					0x00010015
#define	APIHELP_Svr_Server_CommandsWindow					0x00010016
#define	APIHELP_Svr_Server_Settings							0x00010017

#define	APIHELP_Svr_Event									0x00020000
#define	APIHELP_Svr_Event_Command							0x00020001
#define	APIHELP_Svr_Event_ActivateInputState				0x00020002
#define	APIHELP_Svr_Event_Restart							0x00020003
#define	APIHELP_Svr_Event_Shutdown							0x00020004
#define	APIHELP_Svr_Event_VisibleState						0x00020005
#define	APIHELP_Svr_Event_Click								0x00020006
#define	APIHELP_Svr_Event_DblClick							0x00020007
#define	APIHELP_Svr_Event_DragStart							0x00020008
#define	APIHELP_Svr_Event_DragComplete						0x00020009
#define	APIHELP_Svr_Event_RequestStart						0x0002000A
#define	APIHELP_Svr_Event_RequestComplete					0x0002000B
#define	APIHELP_Svr_Event_BookMark							0x0002000C
#define	APIHELP_Svr_Event_Idle								0x0002000D
#define	APIHELP_Svr_Event_Move								0x0002000E
#define	APIHELP_Svr_Event_Size								0x0002000F
#define	APIHELP_Svr_Event_BalloonVisibleState				0x00020010
#define	APIHELP_Svr_Event_HelpComplete						0x00020011
#define	APIHELP_Svr_Event_ListeningState					0x00020012
#define	APIHELP_Svr_Event_DefaultCharacterChange			0x00020013
#define	APIHELP_Svr_Event_AgentPropertyChange				0x00020014
#define	APIHELP_Svr_Event_ActiveClientChange				0x00020015
#define	APIHELP_Svr_Event_SpeechStart						0x00020016
#define	APIHELP_Svr_Event_SpeechEnd							0x00020017
#define	APIHELP_Svr_Event_SpeechWord						0x00020018

#define	APIHELP_Svr_Character								0x00030000
#define	APIHELP_Svr_Character_GetVisible					0x00030001
#define	APIHELP_Svr_Character_SetPosition					0x00030002
#define	APIHELP_Svr_Character_GetPosition					0x00030003
#define	APIHELP_Svr_Character_SetSize						0x00030004
#define	APIHELP_Svr_Character_GetSize						0x00030005
#define	APIHELP_Svr_Character_GetName						0x00030006
#define	APIHELP_Svr_Character_GetDescription 				0x00030007
#define	APIHELP_Svr_Character_GetTTSSpeed					0x00030008
#define	APIHELP_Svr_Character_GetTTSPitch					0x00030009
#define	APIHELP_Svr_Character_Activate						0x0003000A
#define	APIHELP_Svr_Character_SetIdleOn						0x0003000B
#define	APIHELP_Svr_Character_GetIdleOn						0x0003000C
#define	APIHELP_Svr_Character_Prepare						0x0003000D
#define	APIHELP_Svr_Character_Play							0x0003000E
#define	APIHELP_Svr_Character_Stop							0x0003000F
#define	APIHELP_Svr_Character_StopAll						0x00030010
#define	APIHELP_Svr_Character_Wait							0x00030011
#define	APIHELP_Svr_Character_Interrupt						0x00030012
#define	APIHELP_Svr_Character_Show							0x00030013
#define	APIHELP_Svr_Character_Hide							0x00030014
#define	APIHELP_Svr_Character_Speak							0x00030015
#define	APIHELP_Svr_Character_MoveTo						0x00030016
#define	APIHELP_Svr_Character_GestureAt						0x00030017
#define	APIHELP_Svr_Character_GetMoveCause					0x00030018
#define	APIHELP_Svr_Character_GetVisibilityCause			0x00030019
#define	APIHELP_Svr_Character_HasOtherClients				0x0003001A
#define	APIHELP_Svr_Character_SetSoundEffectsOn				0x0003001B
#define	APIHELP_Svr_Character_GetSoundEffectsOn				0x0003001C
#define	APIHELP_Svr_Character_SetName						0x0003001D
#define	APIHELP_Svr_Character_SetDescription				0x0003001E
#define	APIHELP_Svr_Character_GetExtraData					0x0003001F
#define	APIHELP_Svr_Character_ShowPopupMenu					0x00030020
#define	APIHELP_Svr_Character_SetAutoPopupMenu				0x00030021
#define	APIHELP_Svr_Character_GetAutoPopupMenu				0x00030022
#define	APIHELP_Svr_Character_GetHelpFileName				0x00030023
#define	APIHELP_Svr_Character_SetHelpFileName				0x00030024
#define	APIHELP_Svr_Character_SetHelpModeOn					0x00030025
#define	APIHELP_Svr_Character_GetHelpModeOn					0x00030026
#define	APIHELP_Svr_Character_SetHelpContextID				0x00030027
#define	APIHELP_Svr_Character_GetHelpContextID				0x00030028
#define	APIHELP_Svr_Character_GetActive						0x00030029
#define	APIHELP_Svr_Character_Listen						0x0003002A
#define	APIHELP_Svr_Character_SetLanguageID					0x0003002B
#define	APIHELP_Svr_Character_GetLanguageID					0x0003002C
#define	APIHELP_Svr_Character_GetTTSModeID					0x0003002D
#define	APIHELP_Svr_Character_SetTTSModeID					0x0003002E
#define	APIHELP_Svr_Character_GetSRModeID					0x0003002F
#define	APIHELP_Svr_Character_SetSRModeID					0x00030030
#define	APIHELP_Svr_Character_GetGUID						0x00030031
#define	APIHELP_Svr_Character_GetOriginalSize				0x00030032
#define	APIHELP_Svr_Character_Think							0x00030033
#define	APIHELP_Svr_Character_GetVersion					0x00030034
#define	APIHELP_Svr_Character_GetAnimationNames				0x00030035
#define	APIHELP_Svr_Character_GetSRStatus					0x00030036
#define	APIHELP_Svr_Character_Balloon						0x00030037
#define	APIHELP_Svr_Character_Commands						0x00030038
#define	APIHELP_Svr_Character_Style							0x00030039
#define	APIHELP_Svr_Character_HasIcon						0x0003003A
#define	APIHELP_Svr_Character_GenerateIcon					0x0003003B
#define	APIHELP_Svr_Character_IconVisible					0x0003003C
#define	APIHELP_Svr_Character_GetIconIdentity				0x0003003D
#define	APIHELP_Svr_Character_SetIconIdentity				0x0003003E
#define	APIHELP_Svr_Character_IconIdentity					0x0003003F
#define	APIHELP_Svr_Character_IconTip						0x00030040
#define	APIHELP_Svr_Character_TTSEngine						0x00030041
#define	APIHELP_Svr_Character_FindTTSEngines				0x00030042
#define	APIHELP_Svr_Character_SREngine						0x00030043
#define	APIHELP_Svr_Character_FindSREngines					0x00030044
#define	APIHELP_Svr_Character_CharacterID					0x00030045
#define	APIHELP_Svr_Character_UniqueID						0x00030046
#define	APIHELP_Svr_Character_Name							0x00030047
#define	APIHELP_Svr_Character_Description					0x00030048
#define	APIHELP_Svr_Character_ExtraData						0x00030049
#define	APIHELP_Svr_Character_FileName						0x0003004A
#define	APIHELP_Svr_Character_FilePath						0x0003004B
#define	APIHELP_Svr_Character_LanguageID					0x0003004C
#define	APIHELP_Svr_Character_TTSModeID						0x0003004D
#define	APIHELP_Svr_Character_SRModeID						0x0003004E
#define	APIHELP_Svr_Character_Left							0x0003004F
#define	APIHELP_Svr_Character_Top							0x00030050
#define	APIHELP_Svr_Character_Width							0x00030051
#define	APIHELP_Svr_Character_Height						0x00030052
#define	APIHELP_Svr_Character_OriginalWidth					0x00030053
#define	APIHELP_Svr_Character_OriginalHeight				0x00030054
#define	APIHELP_Svr_Character_Visible						0x00030055
#define	APIHELP_Svr_Character_ActiveState					0x00030056
#define	APIHELP_Svr_Character_IdleState						0x00030057
#define	APIHELP_Svr_Character_OtherClientCount				0x00030058
#define	APIHELP_Svr_Character_MoveCause						0x00030059
#define	APIHELP_Svr_Character_VisibilityCause				0x0003005A
#define	APIHELP_Svr_Character_ListeningStatus				0x0003005B
#define	APIHELP_Svr_Character_Animations					0x0003005C
#define	APIHELP_Svr_Character_States						0x0003005D
#define	APIHELP_Svr_Character_SpeakFormatted				0x0003005E
#define	APIHELP_Svr_Character_ThinkFormatted				0x0003005F
#define	APIHELP_Svr_Character_NewFormattedText				0x00030060

#define	APIHELP_Svr_Balloon									0x00040000
#define	APIHELP_Svr_Balloon_GetEnabled						0x00040001
#define	APIHELP_Svr_Balloon_GetNumLines						0x00040002
#define	APIHELP_Svr_Balloon_GetNumCharsPerLine				0x00040003
#define	APIHELP_Svr_Balloon_GetFontName						0x00040004
#define	APIHELP_Svr_Balloon_GetFontSize						0x00040005
#define	APIHELP_Svr_Balloon_GetFontBold						0x00040006
#define	APIHELP_Svr_Balloon_GetFontItalic					0x00040007
#define	APIHELP_Svr_Balloon_GetFontStrikethru				0x00040008
#define	APIHELP_Svr_Balloon_GetFontUnderline				0x00040009
#define	APIHELP_Svr_Balloon_GetForeColor					0x0004000A
#define	APIHELP_Svr_Balloon_GetBackColor					0x0004000B
#define	APIHELP_Svr_Balloon_GetBorderColor					0x0004000C
#define	APIHELP_Svr_Balloon_SetVisible						0x0004000D
#define	APIHELP_Svr_Balloon_GetVisible						0x0004000E
#define	APIHELP_Svr_Balloon_SetFontName						0x0004000F
#define	APIHELP_Svr_Balloon_SetFontSize						0x00040010
#define	APIHELP_Svr_Balloon_SetFontCharSet					0x00040011
#define	APIHELP_Svr_Balloon_GetFontCharSet					0x00040012
#define	APIHELP_Svr_Balloon_SetStyle						0x00040013
#define	APIHELP_Svr_Balloon_GetStyle						0x00040014
#define	APIHELP_Svr_Balloon_SetNumLines						0x00040015
#define	APIHELP_Svr_Balloon_SetNumCharsPerLine				0x00040016
#define	APIHELP_Svr_Balloon_Enabled							0x00040017
#define	APIHELP_Svr_Balloon_Style							0x00040018
#define	APIHELP_Svr_Balloon_Visible							0x00040019
#define	APIHELP_Svr_Balloon_NumberOfLines					0x0004001A
#define	APIHELP_Svr_Balloon_CharsPerLine					0x0004001B
#define	APIHELP_Svr_Balloon_TextColor						0x0004001C
#define	APIHELP_Svr_Balloon_BackColor						0x0004001D
#define	APIHELP_Svr_Balloon_BorderColor						0x0004001E
#define	APIHELP_Svr_Balloon_FontName						0x0004001F
#define	APIHELP_Svr_Balloon_FontSize						0x00040020
#define	APIHELP_Svr_Balloon_FontBold						0x00040021
#define	APIHELP_Svr_Balloon_FontItalic						0x00040022
#define	APIHELP_Svr_Balloon_FontStrikethru					0x00040023
#define	APIHELP_Svr_Balloon_FontUnderline					0x00040024
#define	APIHELP_Svr_Balloon_FontCharSet						0x00040025

#define	APIHELP_Svr_Commands								0x00050000

#define	APIHELP_Svr_Command									0x00060000

#define	APIHELP_Svr_UserInput								0x00070000

#define	APIHELP_Svr_CommandsWindow							0x00080000

#define	APIHELP_Svr_PropertySheet							0x00090000

#define	APIHELP_Svr_AudioOutput								0x000A0000
#define	APIHELP_Svr_SpeechInput								0x000B0000
#define	APIHELP_Svr_AnimationNames							0x000C0000

#define	APIHELP_Svr_Settings								0x000D0000

#define	APIHELP_Svr_CharacterFiles							0x000E0000

#define	APIHELP_Svr_FormattedText								0x000F0000

#define	APIHELP_Svr_TTSEngines								0x00100000

#define	APIHELP_Svr_TTSEngine								0x00110000

#define	APIHELP_Svr_SREngines								0x00120000

#define	APIHELP_Svr_SREngine								0x00130000

/////////////////////////////////////////////////////////////////////////////

#define	APIHELP_Ctl_Control									0x01010000
#define	APIHELP_Ctl_Control_Characters						0x01010001
#define	APIHELP_Ctl_Control_AudioOutput						0x01010002
#define	APIHELP_Ctl_Control_SpeechInput						0x01010003
#define	APIHELP_Ctl_Control_PropertySheet					0x01010004
#define	APIHELP_Ctl_Control_Connected						0x01010005
#define	APIHELP_Ctl_Control_CommandsWindow					0x01010006
#define	APIHELP_Ctl_Control_Suspended						0x01010007
#define	APIHELP_Ctl_Control_ShowDefaultCharacterProperties	0x01010008
#define	APIHELP_Ctl_Control_RaiseRequestErrors				0x01010009
#define	APIHELP_Ctl_Control_CharacterStyle					0x0101000A
#define	APIHELP_Ctl_Control_CharacterFiles					0x0101000B
#define	APIHELP_Ctl_Control_TTSEngines						0x0101000C
#define	APIHELP_Ctl_Control_FindTTSEngines					0x0101000D
#define	APIHELP_Ctl_Control_GetCharacterTTSEngine			0x0101000E
#define	APIHELP_Ctl_Control_FindCharacterTTSEngines			0x0101000F
#define	APIHELP_Ctl_Control_SREngines						0x01010010
#define	APIHELP_Ctl_Control_FindSREngines					0x01010011
#define	APIHELP_Ctl_Control_GetCharacterSREngine			0x01010012
#define	APIHELP_Ctl_Control_FindCharacterSREngines			0x01010013
#define	APIHELP_Ctl_Control_Settings						0x01010014
#define	APIHELP_Ctl_Control_AutoConnect						0x01010015
#define	APIHELP_Ctl_Control_ControlCharacter				0x01010016

#define	APIHELP_Ctl_Event									0x01020000
#define	APIHELP_Ctl_Event_ActivateInput						0x01020001
#define	APIHELP_Ctl_Event_Click								0x01020002
#define	APIHELP_Ctl_Event_DeactivateInput					0x01020003
#define	APIHELP_Ctl_Event_DblClick							0x01020004
#define	APIHELP_Ctl_Event_DragStart							0x01020005
#define	APIHELP_Ctl_Event_DragComplete						0x01020006
#define	APIHELP_Ctl_Event_Hide								0x01020007
#define	APIHELP_Ctl_Event_RequestStart						0x01020008
#define	APIHELP_Ctl_Event_RequestComplete					0x01020009
#define	APIHELP_Ctl_Event_Shutdown							0x0102000A
#define	APIHELP_Ctl_Event_Show								0x0102000B
#define	APIHELP_Ctl_Event_Bookmark							0x0102000C
#define	APIHELP_Ctl_Event_Command							0x0102000D
#define	APIHELP_Ctl_Event_IdleStart							0x0102000E
#define	APIHELP_Ctl_Event_IdleComplete						0x0102000F
#define	APIHELP_Ctl_Event_Restart							0x01020010
#define	APIHELP_Ctl_Event_Move								0x01020011
#define	APIHELP_Ctl_Event_Size								0x01020012
#define	APIHELP_Ctl_Event_BalloonShow						0x01020013
#define	APIHELP_Ctl_Event_BalloonHide						0x01020014
#define	APIHELP_Ctl_Event_HelpComplete						0x01020015
#define	APIHELP_Ctl_Event_ListenStart						0x01020016
#define	APIHELP_Ctl_Event_ListenComplete					0x01020017
#define	APIHELP_Ctl_Event_DefaultCharacterChange			0x01020018
#define	APIHELP_Ctl_Event_AgentPropertyChange				0x01020019
#define	APIHELP_Ctl_Event_ActiveClientChange				0x01020020
#define	APIHELP_Ctl_Event_SpeechStart						0x01020021
#define	APIHELP_Ctl_Event_SpeechEnd							0x01020022
#define	APIHELP_Ctl_Event_SpeechWord							0x01020023

#define	APIHELP_Ctl_Characters								0x01030000
#define	APIHELP_Ctl_Characters_Load							0x01030001
#define	APIHELP_Ctl_Characters_Unload						0x01030002
#define	APIHELP_Ctl_Characters_Character					0x01030003
#define	APIHELP_Ctl_Characters_Count						0x01030004
#define	APIHELP_Ctl_Characters_Index						0x01030005
#define	APIHELP_Ctl_Characters_Item							0x01030006

#define	APIHELP_Ctl_Character								0x01040000
#define	APIHELP_Ctl_Character_Visible						0x01040001
#define	APIHELP_Ctl_Character_Left							0x01040002
#define	APIHELP_Ctl_Character_Top							0x01040003
#define	APIHELP_Ctl_Character_Height						0x01040004
#define	APIHELP_Ctl_Character_Width							0x01040005
#define	APIHELP_Ctl_Character_Speed							0x01040006
#define	APIHELP_Ctl_Character_Pitch							0x01040007
#define	APIHELP_Ctl_Character_Play							0x01040008
#define	APIHELP_Ctl_Character_Stop							0x01040009
#define	APIHELP_Ctl_Character_Speak							0x0104000A
#define	APIHELP_Ctl_Character_GestureAt						0x0104000B
#define	APIHELP_Ctl_Character_MoveTo						0x0104000C
#define	APIHELP_Ctl_Character_Hide							0x0104000D
#define	APIHELP_Ctl_Character_Show							0x0104000E
#define	APIHELP_Ctl_Character_Interrupt						0x0104000F
#define	APIHELP_Ctl_Character_Wait							0x01040010
#define	APIHELP_Ctl_Character_Balloon						0x01040011
#define	APIHELP_Ctl_Character_Name							0x01040012
#define	APIHELP_Ctl_Character_Commands						0x01040013
#define	APIHELP_Ctl_Character_Activate						0x01040014
#define	APIHELP_Ctl_Character_Get							0x01040015
#define	APIHELP_Ctl_Character_Description					0x01040016
#define	APIHELP_Ctl_Character_IdleOn						0x01040017
#define	APIHELP_Ctl_Character_StopAll						0x01040018
#define	APIHELP_Ctl_Character_MoveCause						0x01040019
#define	APIHELP_Ctl_Character_VisibilityCause				0x0104001A
#define	APIHELP_Ctl_Character_HasOtherClients				0x0104001B
#define	APIHELP_Ctl_Character_SoundEffectsOn				0x0104001C
#define	APIHELP_Ctl_Character_ExtraData						0x0104001D
#define	APIHELP_Ctl_Character_ShowPopupMenu					0x0104001E
#define	APIHELP_Ctl_Character_AutoPopupMenu					0x0104001F
#define	APIHELP_Ctl_Character_HelpModeOn					0x01040020
#define	APIHELP_Ctl_Character_HelpContextID					0x01040021
#define	APIHELP_Ctl_Character_Active						0x01040022
#define	APIHELP_Ctl_Character_Listen						0x01040023
#define	APIHELP_Ctl_Character_LanguageID					0x01040024
#define	APIHELP_Ctl_Character_SRModeID						0x01040025
#define	APIHELP_Ctl_Character_TTSModeID						0x01040026
#define	APIHELP_Ctl_Character_HelpFile						0x01040027
#define	APIHELP_Ctl_Character_GUID							0x01040028
#define	APIHELP_Ctl_Character_OriginalHeight				0x01040029
#define	APIHELP_Ctl_Character_OriginalWidth					0x0104002A
#define	APIHELP_Ctl_Character_Think							0x0104002B
#define	APIHELP_Ctl_Character_Version						0x0104002C
#define	APIHELP_Ctl_Character_AnimationNames				0x0104002D
#define	APIHELP_Ctl_Character_SRStatus						0x0104002E
#define	APIHELP_Ctl_Character_Style							0x0104002F
#define	APIHELP_Ctl_Character_HasIcon						0x01040030
#define	APIHELP_Ctl_Character_GenerateIcon					0x01040031
#define	APIHELP_Ctl_Character_IconShown						0x01040032
#define	APIHELP_Ctl_Character_IconVisible					0x01040033
#define	APIHELP_Ctl_Character_IconIdentity					0x01040034
#define	APIHELP_Ctl_Character_IconTip						0x01040035
#define	APIHELP_Ctl_Character_TTSEngine						0x01040036
#define	APIHELP_Ctl_Character_FindTTSEngines				0x01040037
#define	APIHELP_Ctl_Character_SREngine						0x01040038
#define	APIHELP_Ctl_Character_FindSREngines					0x01040039
#define	APIHELP_Ctl_Character_UniqueID						0x0104003A
#define	APIHELP_Ctl_Character_CharacterID					0x0104003B
#define	APIHELP_Ctl_Character_Connected						0x0104003C
#define	APIHELP_Ctl_Character_FileName						0x0104003D
#define	APIHELP_Ctl_Character_FilePath						0x0104003E
#define	APIHELP_Ctl_Character_ActiveState					0x0104003F
#define	APIHELP_Ctl_Character_IdleState						0x01040040
#define	APIHELP_Ctl_Character_IdleEnabled					0x01040041
#define	APIHELP_Ctl_Character_SoundEffectsEnabled			0x01040042
#define	APIHELP_Ctl_Character_Prepare						0x01040043
#define	APIHELP_Ctl_Character_ListeningStatus				0x01040044
#define	APIHELP_Ctl_Character_Smoothed						0x01040045
#define	APIHELP_Ctl_Character_SmoothEdges					0x01040046
#define	APIHELP_Ctl_Character_SpeakFormatted				0x01040047
#define	APIHELP_Ctl_Character_ThinkFormatted				0x01040048
#define	APIHELP_Ctl_Character_NewFormattedText				0x01040049
#define	APIHELP_Ctl_Character_SuspendPause					0x0104004A
#define	APIHELP_Ctl_Character_SuspendStop					0x0104004B
#define	APIHELP_Ctl_Character_SuspendHide					0x0104004C

#define	APIHELP_Ctl_Request									0x01050000
#define	APIHELP_Ctl_Request_ID								0x01050001
#define	APIHELP_Ctl_Request_Status							0x01050002
#define	APIHELP_Ctl_Request_Description						0x01050003
#define	APIHELP_Ctl_Request_Number							0x01050004

#define	APIHELP_Ctl_Balloon									0x01060000
#define	APIHELP_Ctl_Balloon_Enabled							0x01060001
#define	APIHELP_Ctl_Balloon_NumberOfLines					0x01060002
#define	APIHELP_Ctl_Balloon_CharsPerLine					0x01060003
#define	APIHELP_Ctl_Balloon_FontName						0x01060004
#define	APIHELP_Ctl_Balloon_FontSize						0x01060005
#define	APIHELP_Ctl_Balloon_ForeColor						0x01060006
#define	APIHELP_Ctl_Balloon_BackColor						0x01060007
#define	APIHELP_Ctl_Balloon_BorderColor						0x01060008
#define	APIHELP_Ctl_Balloon_FontBold						0x01060009
#define	APIHELP_Ctl_Balloon_FontItalic						0x0106000A
#define	APIHELP_Ctl_Balloon_FontStrikethru					0x0106000B
#define	APIHELP_Ctl_Balloon_FontUnderline					0x0106000C
#define	APIHELP_Ctl_Balloon_Visible							0x0106000D
#define	APIHELP_Ctl_Balloon_FontCharSet						0x0106000E
#define	APIHELP_Ctl_Balloon_Style							0x0106000F
#define	APIHELP_Ctl_Balloon_TextColor						0x01060010
#define	APIHELP_Ctl_Balloon_SizeToText						0x01060011
#define	APIHELP_Ctl_Balloon_AutoHide						0x01060012
#define	APIHELP_Ctl_Balloon_AutoPace						0x01060013
#define	APIHELP_Ctl_Balloon_ShowPartialLines				0x01060014

#define	APIHELP_Ctl_Commands								0x01070000
#define	APIHELP_Ctl_Commands_Count							0x01070001
#define	APIHELP_Ctl_Commands_Caption						0x01070002
#define	APIHELP_Ctl_Commands_Voice							0x01070003
#define	APIHELP_Ctl_Commands_Visible						0x01070004
#define	APIHELP_Ctl_Commands_Add							0x01070005
#define	APIHELP_Ctl_Commands_Insert							0x01070006
#define	APIHELP_Ctl_Commands_Remove							0x01070007
#define	APIHELP_Ctl_Commands_RemoveAll						0x01070008
#define	APIHELP_Ctl_Commands_Command						0x01070009
#define	APIHELP_Ctl_Commands_Commands_DefaultCommand		0x0107000A
#define	APIHELP_Ctl_Commands_HelpContextID					0x0107000B
#define	APIHELP_Ctl_Commands_FontName						0x0107000C
#define	APIHELP_Ctl_Commands_FontSize						0x0107000D
#define	APIHELP_Ctl_Commands_VoiceCaption					0x0107000E
#define	APIHELP_Ctl_Commands_GlobalVoiceCommandsEnabled		0x0107000F
#define	APIHELP_Ctl_Commands_Commands_VoiceGrammar			0x01070000
#define	APIHELP_Ctl_Commands_Index							0x01070011

#define	APIHELP_Ctl_Command									0x01080000
#define	APIHELP_Ctl_Command_Voice							0x01080001
#define	APIHELP_Ctl_Command_Caption							0x01080002
#define	APIHELP_Ctl_Command_Confidence						0x01080003
#define	APIHELP_Ctl_Command_ConfidenceText					0x01080004
#define	APIHELP_Ctl_Command_Enabled							0x01080005
#define	APIHELP_Ctl_Command_Visible							0x01080006
#define	APIHELP_Ctl_Command_HelpContextID					0x01080007
#define	APIHELP_Ctl_Command_VoiceCaption					0x01080008
#define	APIHELP_Ctl_Command_VoiceGrammar					0x01080009
#define	APIHELP_Ctl_Command_ConfidenceThreshold				0x0108000A
#define	APIHELP_Ctl_Command_Name							0x0108000B

#define	APIHELP_Ctl_UserInput								0x01090000
#define	APIHELP_Ctl_UserInput_Count							0x01090001
#define	APIHELP_Ctl_UserInput_Name							0x01090002
#define	APIHELP_Ctl_UserInput_CharacterID					0x01090003
#define	APIHELP_Ctl_UserInput_Confidence					0x01090004
#define	APIHELP_Ctl_UserInput_Voice							0x01090005
#define	APIHELP_Ctl_UserInput_Alt1Name						0x01090006
#define	APIHELP_Ctl_UserInput_Alt1Confidence				0x01090007
#define	APIHELP_Ctl_UserInput_Alt1Voice						0x01090008
#define	APIHELP_Ctl_UserInput_Alt2Name						0x01090009
#define	APIHELP_Ctl_UserInput_Alt2Confidence				0x0109000A
#define	APIHELP_Ctl_UserInput_Alt2Voice						0x0109000B

#define	APIHELP_Ctl_CommandsWindow							0x010A0000
#define	APIHELP_Ctl_CommandsWindow_Visible					0x010A0001
#define	APIHELP_Ctl_CommandsWindow_Left						0x010A0002
#define	APIHELP_Ctl_CommandsWindow_Top						0x010A0003
#define	APIHELP_Ctl_CommandsWindow_Height					0x010A0004
#define	APIHELP_Ctl_CommandsWindow_Width					0x010A0005

#define	APIHELP_Ctl_AudioOutput								0x010B0000
#define	APIHELP_Ctl_SpeechInput								0x010C0000
#define	APIHELP_Ctl_AnimationNames							0x010F0000

#define	APIHELP_Ctl_Settings								0x010D0000
#define	APIHELP_Ctl_Settings_SoundEffectsEnabled			0x010D0001
#define	APIHELP_Ctl_Settings_BalloonEnabled					0x010D0002
#define	APIHELP_Ctl_Settings_BalloonFont					0x010D0003
#define	APIHELP_Ctl_Settings_TTSEnabled						0x010D0004
#define	APIHELP_Ctl_Settings_TTSSpeed						0x010D0005
#define	APIHELP_Ctl_Settings_SREnabled						0x010D0006
#define	APIHELP_Ctl_Settings_SRHotKey						0x010D0007
#define	APIHELP_Ctl_Settings_SRHotKeyTime					0x010D0008
#define	APIHELP_Ctl_Settings_SRListeningTip					0x010D0009
#define	APIHELP_Ctl_Settings_SRListeningPrompt				0x010D000A
#define	APIHELP_Ctl_Settings_AudioStatus					0x010D000B

#define	APIHELP_Ctl_PropertySheet							0x010E0000
#define	APIHELP_Ctl_PropertySheet_Left						0x010E0001
#define	APIHELP_Ctl_PropertySheet_Top						0x010E0002
#define	APIHELP_Ctl_PropertySheet_Height					0x010E0003
#define	APIHELP_Ctl_PropertySheet_Width						0x010E0004
#define	APIHELP_Ctl_PropertySheet_Page						0x010E0005
#define	APIHELP_Ctl_PropertySheet_Visible					0x010E0006

#define	APIHELP_Ctl_CharacterFiles							0x01100000
#define	APIHELP_Ctl_CharacterFiles_FilePaths				0x01100001
#define	APIHELP_Ctl_CharacterFiles_SearchPath				0x01100002
#define	APIHELP_Ctl_CharacterFiles_DefaultSearchPath		0x01100003
#define	APIHELP_Ctl_CharacterFiles_DoubleAgentFiles			0x01100004
#define	APIHELP_Ctl_CharacterFiles_MsAgentFiles				0x01100005
#define	APIHELP_Ctl_CharacterFiles_MsOfficeFiles			0x01100006
#define	APIHELP_Ctl_CharacterFiles_SpeakingCharacters		0x01100007
#define	APIHELP_Ctl_CharacterFiles_NonSpeakingCharacters	0x01100008
#define	APIHELP_Ctl_CharacterFiles_VerifyVersion			0x01100009
#define	APIHELP_Ctl_CharacterFiles_DefaultFilePath			0x0110000A
#define	APIHELP_Ctl_CharacterFiles_DefaultFileName			0x0110000B

#define	APIHELP_Ctl_FormattedText							0x01110000
#define	APIHELP_Ctl_FormattedText_RawText					0x01110001
#define	APIHELP_Ctl_FormattedText_ParsedText				0x01110002
#define	APIHELP_Ctl_FormattedText_SpeechText				0x01110003
#define	APIHELP_Ctl_FormattedText_WordCount					0x01110004
#define	APIHELP_Ctl_FormattedText_DisplayWord				0x01110005
#define	APIHELP_Ctl_FormattedText_WordIndex					0x01110006
#define	APIHELP_Ctl_FormattedText_DisplayText				0x01110007
#define	APIHELP_Ctl_FormattedText_Parse						0x01110008
#define	APIHELP_Ctl_FormattedText_Append					0x01110009

#define	APIHELP_Ctl_TTSEngine								0x01120000
#define	APIHELP_Ctl_TTSEngine_TTSModeID						0x01120001
#define	APIHELP_Ctl_TTSEngine_DisplayName					0x01120002
#define	APIHELP_Ctl_TTSEngine_Manufacturer					0x01120003
#define	APIHELP_Ctl_TTSEngine_GetVersion					0x01120004
#define	APIHELP_Ctl_TTSEngine_Gender						0x01120005
#define	APIHELP_Ctl_TTSEngine_LanguageID					0x01120006
#define	APIHELP_Ctl_TTSEngine_LanguageName					0x01120007

#define	APIHELP_Ctl_TTSEngines								0x01130000
#define	APIHELP_Ctl_TTSEngines_Index						0x01130001
#define	APIHELP_Ctl_TTSEngines_Count						0x01130002

#define	APIHELP_Ctl_SREngine								0x01140000
#define	APIHELP_Ctl_SREngine_SRModeID						0x01140001
#define	APIHELP_Ctl_SREngine_DisplayName					0x01140002
#define	APIHELP_Ctl_SREngine_Manufacturer					0x01140003
#define	APIHELP_Ctl_SREngine_GetVersion						0x01140004
#define	APIHELP_Ctl_SREngine_LanguageID						0x01140005
#define	APIHELP_Ctl_SREngine_LanguageName					0x01140006
#define	APIHELP_Ctl_SREngine_LanguageIDs					0x01140007
#define	APIHELP_Ctl_SREngine_LanguageNames					0x01140008

#define	APIHELP_Ctl_SREngines								0x01150000
#define	APIHELP_Ctl_SREngines_Index							0x01150001
#define	APIHELP_Ctl_SREngines_Count							0x01150002

/////////////////////////////////////////////////////////////////////////////
#endif // _DAAPIHELP_H
