

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu May 13 13:40:36 2010
 */
/* Compiler settings for .\Server\DaServer.odl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __DaServerOdl_h__
#define __DaServerOdl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDaServer_FWD_DEFINED__
#define __IDaServer_FWD_DEFINED__
typedef interface IDaServer IDaServer;
#endif 	/* __IDaServer_FWD_DEFINED__ */


#ifndef __IDaSvrUserInput_FWD_DEFINED__
#define __IDaSvrUserInput_FWD_DEFINED__
typedef interface IDaSvrUserInput IDaSvrUserInput;
#endif 	/* __IDaSvrUserInput_FWD_DEFINED__ */


#ifndef __IDaSvrCommand_FWD_DEFINED__
#define __IDaSvrCommand_FWD_DEFINED__
typedef interface IDaSvrCommand IDaSvrCommand;
#endif 	/* __IDaSvrCommand_FWD_DEFINED__ */


#ifndef __IDaSvrCommands_FWD_DEFINED__
#define __IDaSvrCommands_FWD_DEFINED__
typedef interface IDaSvrCommands IDaSvrCommands;
#endif 	/* __IDaSvrCommands_FWD_DEFINED__ */


#ifndef __IDaSvrSpeechInputProperties_FWD_DEFINED__
#define __IDaSvrSpeechInputProperties_FWD_DEFINED__
typedef interface IDaSvrSpeechInputProperties IDaSvrSpeechInputProperties;
#endif 	/* __IDaSvrSpeechInputProperties_FWD_DEFINED__ */


#ifndef __IDaSvrAudioOutputProperties_FWD_DEFINED__
#define __IDaSvrAudioOutputProperties_FWD_DEFINED__
typedef interface IDaSvrAudioOutputProperties IDaSvrAudioOutputProperties;
#endif 	/* __IDaSvrAudioOutputProperties_FWD_DEFINED__ */


#ifndef __IDaSvrPropertySheet_FWD_DEFINED__
#define __IDaSvrPropertySheet_FWD_DEFINED__
typedef interface IDaSvrPropertySheet IDaSvrPropertySheet;
#endif 	/* __IDaSvrPropertySheet_FWD_DEFINED__ */


#ifndef __IDaSvrBalloon_FWD_DEFINED__
#define __IDaSvrBalloon_FWD_DEFINED__
typedef interface IDaSvrBalloon IDaSvrBalloon;
#endif 	/* __IDaSvrBalloon_FWD_DEFINED__ */


#ifndef __IDaSvrCharacter_FWD_DEFINED__
#define __IDaSvrCharacter_FWD_DEFINED__
typedef interface IDaSvrCharacter IDaSvrCharacter;
#endif 	/* __IDaSvrCharacter_FWD_DEFINED__ */


#ifndef __IDaSvrCommandWindow_FWD_DEFINED__
#define __IDaSvrCommandWindow_FWD_DEFINED__
typedef interface IDaSvrCommandWindow IDaSvrCommandWindow;
#endif 	/* __IDaSvrCommandWindow_FWD_DEFINED__ */


#ifndef __IDaSvrNotifySink15_FWD_DEFINED__
#define __IDaSvrNotifySink15_FWD_DEFINED__
typedef interface IDaSvrNotifySink15 IDaSvrNotifySink15;
#endif 	/* __IDaSvrNotifySink15_FWD_DEFINED__ */


#ifndef __IDaSvrNotifySink_FWD_DEFINED__
#define __IDaSvrNotifySink_FWD_DEFINED__
typedef interface IDaSvrNotifySink IDaSvrNotifySink;
#endif 	/* __IDaSvrNotifySink_FWD_DEFINED__ */


#ifndef __DaServer_FWD_DEFINED__
#define __DaServer_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaServer DaServer;
#else
typedef struct DaServer DaServer;
#endif /* __cplusplus */

#endif 	/* __DaServer_FWD_DEFINED__ */


#ifndef __DaCharacter_FWD_DEFINED__
#define __DaCharacter_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCharacter DaCharacter;
#else
typedef struct DaCharacter DaCharacter;
#endif /* __cplusplus */

#endif 	/* __DaCharacter_FWD_DEFINED__ */


#ifndef __DaCommand_FWD_DEFINED__
#define __DaCommand_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCommand DaCommand;
#else
typedef struct DaCommand DaCommand;
#endif /* __cplusplus */

#endif 	/* __DaCommand_FWD_DEFINED__ */


#ifndef __DaCommands_FWD_DEFINED__
#define __DaCommands_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCommands DaCommands;
#else
typedef struct DaCommands DaCommands;
#endif /* __cplusplus */

#endif 	/* __DaCommands_FWD_DEFINED__ */


#ifndef __DaCommandWindow_FWD_DEFINED__
#define __DaCommandWindow_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCommandWindow DaCommandWindow;
#else
typedef struct DaCommandWindow DaCommandWindow;
#endif /* __cplusplus */

#endif 	/* __DaCommandWindow_FWD_DEFINED__ */


#ifndef __DaUserInput_FWD_DEFINED__
#define __DaUserInput_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaUserInput DaUserInput;
#else
typedef struct DaUserInput DaUserInput;
#endif /* __cplusplus */

#endif 	/* __DaUserInput_FWD_DEFINED__ */


#ifndef __DaBalloon_FWD_DEFINED__
#define __DaBalloon_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaBalloon DaBalloon;
#else
typedef struct DaBalloon DaBalloon;
#endif /* __cplusplus */

#endif 	/* __DaBalloon_FWD_DEFINED__ */


#ifndef __DaPropertySheet_FWD_DEFINED__
#define __DaPropertySheet_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaPropertySheet DaPropertySheet;
#else
typedef struct DaPropertySheet DaPropertySheet;
#endif /* __cplusplus */

#endif 	/* __DaPropertySheet_FWD_DEFINED__ */


#ifndef __DaAudioOutputProperties_FWD_DEFINED__
#define __DaAudioOutputProperties_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaAudioOutputProperties DaAudioOutputProperties;
#else
typedef struct DaAudioOutputProperties DaAudioOutputProperties;
#endif /* __cplusplus */

#endif 	/* __DaAudioOutputProperties_FWD_DEFINED__ */


#ifndef __DaSpeechInputProperties_FWD_DEFINED__
#define __DaSpeechInputProperties_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaSpeechInputProperties DaSpeechInputProperties;
#else
typedef struct DaSpeechInputProperties DaSpeechInputProperties;
#endif /* __cplusplus */

#endif 	/* __DaSpeechInputProperties_FWD_DEFINED__ */


#ifndef __DaAnimationNames_FWD_DEFINED__
#define __DaAnimationNames_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaAnimationNames DaAnimationNames;
#else
typedef struct DaAnimationNames DaAnimationNames;
#endif /* __cplusplus */

#endif 	/* __DaAnimationNames_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __DoubleAgentSvr_LIBRARY_DEFINED__
#define __DoubleAgentSvr_LIBRARY_DEFINED__

/* library DoubleAgentSvr */
/* [helpstring][version][uuid] */ 













#define	NeverMoved	( 0 )

#define	UserMoved	( 1 )

#define	ProgramMoved	( 2 )

#define	OtherProgramMoved	( 3 )

#define	SystemMoved	( 4 )

#define	NeverShown	( 0 )

#define	UserHid	( 1 )

#define	UserShowed	( 2 )

#define	ProgramHid	( 3 )

#define	ProgramShowed	( 4 )

#define	OtherProgramHid	( 5 )

#define	OtherProgramShowed	( 6 )

#define	UserHidViaCharacterMenu	( 7 )

#define	UserHidViaTaskbarIcon	( UserHid )

#define	CSHELPCAUSE_COMMAND	( 1 )

#define	CSHELPCAUSE_OTHERPROGRAM	( 2 )

#define	CSHELPCAUSE_OPENCOMMANDSWINDOW	( 3 )

#define	CSHELPCAUSE_CLOSECOMMANDSWINDOW	( 4 )

#define	CSHELPCAUSE_SHOWCHARACTER	( 5 )

#define	CSHELPCAUSE_HIDECHARACTER	( 6 )

#define	CSHELPCAUSE_CHARACTER	( 7 )

#define	ACTIVATE_NOTTOPMOST	( 0 )

#define	ACTIVATE_TOPMOST	( 1 )

#define	ACTIVATE_NOTACTIVE	( 0 )

#define	ACTIVATE_ACTIVE	( 1 )

#define	ACTIVATE_INPUTACTIVE	( 2 )

#define	PREPARE_ANIMATION	( 0 )

#define	PREPARE_STATE	( 1 )

#define	PREPARE_WAVE	( 2 )

#define	STOP_TYPE_PLAY	( 0x1 )

#define	STOP_TYPE_MOVE	( 0x2 )

#define	STOP_TYPE_SPEAK	( 0x4 )

#define	STOP_TYPE_PREPARE	( 0x8 )

#define	STOP_TYPE_NONQUEUEDPREPARE	( 0x10 )

#define	STOP_TYPE_VISIBLE	( 0x20 )

#define	STOP_TYPE_ALL	( 0xffffffff )

#define	BALLOON_STYLE_BALLOON_ON	( 0x1 )

#define	BALLOON_STYLE_SIZETOTEXT	( 0x2 )

#define	BALLOON_STYLE_AUTOHIDE	( 0x4 )

#define	BALLOON_STYLE_AUTOPACE	( 0x8 )

#define	AUDIO_STATUS_AVAILABLE	( 0 )

#define	AUDIO_STATUS_NOAUDIO	( 1 )

#define	AUDIO_STATUS_CANTOPENAUDIO	( 2 )

#define	AUDIO_STATUS_USERSPEAKING	( 3 )

#define	AUDIO_STATUS_CHARACTERSPEAKING	( 4 )

#define	AUDIO_STATUS_SROVERRIDEABLE	( 5 )

#define	AUDIO_STATUS_ERROR	( 6 )

#define	LISTEN_STATUS_CANLISTEN	( 0 )

#define	LISTEN_STATUS_NOAUDIO	( 1 )

#define	LISTEN_STATUS_NOTACTIVE	( 2 )

#define	LISTEN_STATUS_CANTOPENAUDIO	( 3 )

#define	LISTEN_STATUS_COULDNTINITIALIZESPEECH	( 4 )

#define	LISTEN_STATUS_SPEECHDISABLED	( 5 )

#define	LISTEN_STATUS_ERROR	( 6 )

#define	LSCOMPLETE_CAUSE_PROGRAMDISABLED	( 1 )

#define	LSCOMPLETE_CAUSE_PROGRAMTIMEDOUT	( 2 )

#define	LSCOMPLETE_CAUSE_USERTIMEDOUT	( 3 )

#define	LSCOMPLETE_CAUSE_USERRELEASEDKEY	( 4 )

#define	LSCOMPLETE_CAUSE_USERUTTERANCEENDED	( 5 )

#define	LSCOMPLETE_CAUSE_CLIENTDEACTIVATED	( 6 )

#define	LSCOMPLETE_CAUSE_DEFAULTCHARCHANGE	( 7 )

#define	LSCOMPLETE_CAUSE_USERDISABLED	( 8 )

#define	MK_ICON	( 0x1000 )

#define	DISPID_IAgent_Load	( 0x60020000 )

#define	DISPID_IAgent_Unload	( 0x60020001 )

#define	DISPID_IAgent_Register	( 0x60020002 )

#define	DISPID_IAgent_Unregister	( 0x60020003 )

#define	DISPID_IAgent_GetCharacter	( 0x60020004 )

#define	DISPID_IAgent_GetSuspended	( 0x60020005 )

#define	DISPID_IAgentEx_GetCharacterEx	( 0x60030000 )

#define	DISPID_IAgentEx_GetVersion	( 0x60030001 )

#define	DISPID_IAgentEx_ShowDefaultCharacterProperties	( 0x60030002 )

#define	DISPID_IAgentUserInput_GetCount	( 0x60020000 )

#define	DISPID_IAgentUserInput_GetItemID	( 0x60020001 )

#define	DISPID_IAgentUserInput_GetItemConfidence	( 0x60020002 )

#define	DISPID_IAgentUserInput_GetItemText	( 0x60020003 )

#define	DISPID_IAgentUserInput_GetAllItemData	( 0x60020004 )

#define	DISPID_IAgentCommand_SetCaption	( 0x60020000 )

#define	DISPID_IAgentCommand_GetCaption	( 0x60020001 )

#define	DISPID_IAgentCommand_SetVoice	( 0x60020002 )

#define	DISPID_IAgentCommand_GetVoice	( 0x60020003 )

#define	DISPID_IAgentCommand_SetEnabled	( 0x60020004 )

#define	DISPID_IAgentCommand_GetEnabled	( 0x60020005 )

#define	DISPID_IAgentCommand_SetVisible	( 0x60020006 )

#define	DISPID_IAgentCommand_GetVisible	( 0x60020007 )

#define	DISPID_IAgentCommand_SetConfidenceThreshold	( 0x60020008 )

#define	DISPID_IAgentCommand_GetConfidenceThreshold	( 0x60020009 )

#define	DISPID_IAgentCommand_SetConfidenceText	( 0x6002000a )

#define	DISPID_IAgentCommand_GetConfidenceText	( 0x6002000b )

#define	DISPID_IAgentCommand_GetID	( 0x6002000c )

#define	DISPID_IAgentCommandEx_SetHelpContextID	( 0x60030000 )

#define	DISPID_IAgentCommandEx_GetHelpContextID	( 0x60030001 )

#define	DISPID_IAgentCommandEx_SetVoiceCaption	( 0x60030002 )

#define	DISPID_IAgentCommandEx_GetVoiceCaption	( 0x60030003 )

#define	DISPID_IAgentCommands_GetCommand	( 0x60020000 )

#define	DISPID_IAgentCommands_GetCount	( 0x60020001 )

#define	DISPID_IAgentCommands_SetCaption	( 0x60020002 )

#define	DISPID_IAgentCommands_GetCaption	( 0x60020003 )

#define	DISPID_IAgentCommands_SetVoice	( 0x60020004 )

#define	DISPID_IAgentCommands_GetVoice	( 0x60020005 )

#define	DISPID_IAgentCommands_SetVisible	( 0x60020006 )

#define	DISPID_IAgentCommands_GetVisible	( 0x60020007 )

#define	DISPID_IAgentCommands_Add	( 0x60020008 )

#define	DISPID_IAgentCommands_Insert	( 0x60020009 )

#define	DISPID_IAgentCommands_Remove	( 0x6002000a )

#define	DISPID_IAgentCommands_RemoveAll	( 0x6002000b )

#define	DISPID_IAgentCommandsEx_GetCommandEx	( 0x60030000 )

#define	DISPID_IAgentCommandsEx_SetDefaultID	( 0x60030001 )

#define	DISPID_IAgentCommandsEx_GetDefaultID	( 0x60030002 )

#define	DISPID_IAgentCommandsEx_SetHelpContextID	( 0x60030003 )

#define	DISPID_IAgentCommandsEx_GetHelpContextID	( 0x60030004 )

#define	DISPID_IAgentCommandsEx_SetFontName	( 0x60030005 )

#define	DISPID_IAgentCommandsEx_GetFontName	( 0x60030006 )

#define	DISPID_IAgentCommandsEx_SetFontSize	( 0x60030007 )

#define	DISPID_IAgentCommandsEx_GetFontSize	( 0x60030008 )

#define	DISPID_IAgentCommandsEx_SetVoiceCaption	( 0x60030009 )

#define	DISPID_IAgentCommandsEx_GetVoiceCaption	( 0x6003000a )

#define	DISPID_IAgentCommandsEx_AddEx	( 0x6003000b )

#define	DISPID_IAgentCommandsEx_InsertEx	( 0x6003000c )

#define	DISPID_IAgentCommandsEx_SetGlobalVoiceCommandsEnabled	( 0x6003000d )

#define	DISPID_IAgentCommandsEx_GetGlobalVoiceCommandsEnabled	( 0x6003000e )

#define	DISPID_IAgentSpeechInputProperties_GetInstalled	( 0x60020000 )

#define	DISPID_IAgentSpeechInputProperties_GetEnabled	( 0x60020001 )

#define	DISPID_IAgentSpeechInputProperties_GetHotKey	( 0x60020002 )

#define	DISPID_IAgentSpeechInputProperties_GetLCID	( 0x60020003 )

#define	DISPID_IAgentSpeechInputProperties_GetEngine	( 0x60020004 )

#define	DISPID_IAgentSpeechInputProperties_SetEngine	( 0x60020005 )

#define	DISPID_IAgentSpeechInputProperties_GetListeningTip	( 0x60020006 )

#define	DISPID_IAgentAudioOutputProperties_GetEnabled	( 0x60020000 )

#define	DISPID_IAgentAudioOutputProperties_GetUsingSoundEffects	( 0x60020001 )

#define	DISPID_IAgentAudioOutputPropertiesEx_GetStatus	( 0x60030000 )

#define	DISPID_IAgentPropertySheet_GetVisible	( 0x60020000 )

#define	DISPID_IAgentPropertySheet_SetVisible	( 0x60020001 )

#define	DISPID_IAgentPropertySheet_GetPosition	( 0x60020002 )

#define	DISPID_IAgentPropertySheet_GetSize	( 0x60020003 )

#define	DISPID_IAgentPropertySheet_GetPage	( 0x60020004 )

#define	DISPID_IAgentPropertySheet_SetPage	( 0x60020005 )

#define	DISPID_IAgentBalloon_GetEnabled	( 0x60020000 )

#define	DISPID_IAgentBalloon_GetNumLines	( 0x60020001 )

#define	DISPID_IAgentBalloon_GetNumCharsPerLine	( 0x60020002 )

#define	DISPID_IAgentBalloon_GetFontName	( 0x60020003 )

#define	DISPID_IAgentBalloon_GetFontSize	( 0x60020004 )

#define	DISPID_IAgentBalloon_GetFontBold	( 0x60020005 )

#define	DISPID_IAgentBalloon_GetFontItalic	( 0x60020006 )

#define	DISPID_IAgentBalloon_GetFontStrikethru	( 0x60020007 )

#define	DISPID_IAgentBalloon_GetFontUnderline	( 0x60020008 )

#define	DISPID_IAgentBalloon_GetForeColor	( 0x60020009 )

#define	DISPID_IAgentBalloon_GetBackColor	( 0x6002000a )

#define	DISPID_IAgentBalloon_GetBorderColor	( 0x6002000b )

#define	DISPID_IAgentBalloon_SetVisible	( 0x6002000c )

#define	DISPID_IAgentBalloon_GetVisible	( 0x6002000d )

#define	DISPID_IAgentBalloon_SetFontName	( 0x6002000e )

#define	DISPID_IAgentBalloon_SetFontSize	( 0x6002000f )

#define	DISPID_IAgentBalloon_SetFontCharSet	( 0x60020010 )

#define	DISPID_IAgentBalloon_GetFontCharSet	( 0x60020011 )

#define	DISPID_IAgentBalloonEx_SetStyle	( 0x60030000 )

#define	DISPID_IAgentBalloonEx_GetStyle	( 0x60030001 )

#define	DISPID_IAgentBalloonEx_SetNumLines	( 0x60030002 )

#define	DISPID_IAgentBalloonEx_SetNumCharsPerLine	( 0x60030003 )

#define	DISPID_IAgentCharacter_GetVisible	( 0x60020000 )

#define	DISPID_IAgentCharacter_SetPosition	( 0x60020001 )

#define	DISPID_IAgentCharacter_GetPosition	( 0x60020002 )

#define	DISPID_IAgentCharacter_SetSize	( 0x60020003 )

#define	DISPID_IAgentCharacter_GetSize	( 0x60020004 )

#define	DISPID_IAgentCharacter_GetName	( 0x60020005 )

#define	DISPID_IAgentCharacter_GetDescription	( 0x60020006 )

#define	DISPID_IAgentCharacter_GetTTSSpeed	( 0x60020007 )

#define	DISPID_IAgentCharacter_GetTTSPitch	( 0x60020008 )

#define	DISPID_IAgentCharacter_Activate	( 0x60020009 )

#define	DISPID_IAgentCharacter_SetIdleOn	( 0x6002000a )

#define	DISPID_IAgentCharacter_GetIdleOn	( 0x6002000b )

#define	DISPID_IAgentCharacter_Prepare	( 0x6002000c )

#define	DISPID_IAgentCharacter_Play	( 0x6002000d )

#define	DISPID_IAgentCharacter_Stop	( 0x6002000e )

#define	DISPID_IAgentCharacter_StopAll	( 0x6002000f )

#define	DISPID_IAgentCharacter_Wait	( 0x60020010 )

#define	DISPID_IAgentCharacter_Interrupt	( 0x60020011 )

#define	DISPID_IAgentCharacter_Show	( 0x60020012 )

#define	DISPID_IAgentCharacter_Hide	( 0x60020013 )

#define	DISPID_IAgentCharacter_Speak	( 0x60020014 )

#define	DISPID_IAgentCharacter_MoveTo	( 0x60020015 )

#define	DISPID_IAgentCharacter_GestureAt	( 0x60020016 )

#define	DISPID_IAgentCharacter_GetMoveCause	( 0x60020017 )

#define	DISPID_IAgentCharacter_GetVisibilityCause	( 0x60020018 )

#define	DISPID_IAgentCharacter_HasOtherClients	( 0x60020019 )

#define	DISPID_IAgentCharacter_SetSoundEffectsOn	( 0x6002001a )

#define	DISPID_IAgentCharacter_GetSoundEffectsOn	( 0x6002001b )

#define	DISPID_IAgentCharacter_SetName	( 0x6002001c )

#define	DISPID_IAgentCharacter_SetDescription	( 0x6002001d )

#define	DISPID_IAgentCharacter_GetExtraData	( 0x6002001e )

#define	DISPID_IAgentCharacterEx_ShowPopupMenu	( 0x60030000 )

#define	DISPID_IAgentCharacterEx_SetAutoPopupMenu	( 0x60030001 )

#define	DISPID_IAgentCharacterEx_GetAutoPopupMenu	( 0x60030002 )

#define	DISPID_IAgentCharacterEx_GetHelpFileName	( 0x60030003 )

#define	DISPID_IAgentCharacterEx_SetHelpFileName	( 0x60030004 )

#define	DISPID_IAgentCharacterEx_SetHelpModeOn	( 0x60030005 )

#define	DISPID_IAgentCharacterEx_GetHelpModeOn	( 0x60030006 )

#define	DISPID_IAgentCharacterEx_SetHelpContextID	( 0x60030007 )

#define	DISPID_IAgentCharacterEx_GetHelpContextID	( 0x60030008 )

#define	DISPID_IAgentCharacterEx_GetActive	( 0x60030009 )

#define	DISPID_IAgentCharacterEx_Listen	( 0x6003000a )

#define	DISPID_IAgentCharacterEx_SetLanguageID	( 0x6003000b )

#define	DISPID_IAgentCharacterEx_GetLanguageID	( 0x6003000c )

#define	DISPID_IAgentCharacterEx_GetTTSModeID	( 0x6003000d )

#define	DISPID_IAgentCharacterEx_SetTTSModeID	( 0x6003000e )

#define	DISPID_IAgentCharacterEx_GetSRModeID	( 0x6003000f )

#define	DISPID_IAgentCharacterEx_SetSRModeID	( 0x60030010 )

#define	DISPID_IAgentCharacterEx_GetGUID	( 0x60030011 )

#define	DISPID_IAgentCharacterEx_GetOriginalSize	( 0x60030012 )

#define	DISPID_IAgentCharacterEx_Think	( 0x60030013 )

#define	DISPID_IAgentCharacterEx_GetVersion	( 0x60030014 )

#define	DISPID_IAgentCharacterEx_GetAnimationNames	( 0x60030015 )

#define	DISPID_IAgentCharacterEx_GetSRStatus	( 0x60030016 )

#define	DISPID_IAgentNotifySink_Command	( 0x60020000 )

#define	DISPID_IAgentNotifySink_ActivateInputState	( 0x60020001 )

#define	DISPID_IAgentNotifySink_Restart	( 0x60020002 )

#define	DISPID_IAgentNotifySink_Shutdown	( 0x60020003 )

#define	DISPID_IAgentNotifySink_VisibleState	( 0x60020004 )

#define	DISPID_IAgentNotifySink_Click	( 0x60020005 )

#define	DISPID_IAgentNotifySink_DblClick	( 0x60020006 )

#define	DISPID_IAgentNotifySink_DragStart	( 0x60020007 )

#define	DISPID_IAgentNotifySink_DragComplete	( 0x60020008 )

#define	DISPID_IAgentNotifySink_RequestStart	( 0x60020009 )

#define	DISPID_IAgentNotifySink_RequestComplete	( 0x6002000a )

#define	DISPID_IAgentNotifySink_BookMark	( 0x6002000b )

#define	DISPID_IAgentNotifySink_Idle	( 0x6002000c )

#define	DISPID_IAgentNotifySink_Move	( 0x6002000d )

#define	DISPID_IAgentNotifySink_Size	( 0x6002000e )

#define	DISPID_IAgentNotifySink_BalloonVisibleState	( 0x6002000f )

#define	DISPID_IAgentNotifySinkEx_HelpComplete	( 0x60030000 )

#define	DISPID_IAgentNotifySinkEx_ListeningState	( 0x60030001 )

#define	DISPID_IAgentNotifySinkEx_DefaultCharacterChange	( 0x60030002 )

#define	DISPID_IAgentNotifySinkEx_AgentPropertyChange	( 0x60030003 )

#define	DISPID_IAgentNotifySinkEx_ActiveClientChange	( 0x60030004 )

#define	DISPID_IAgentCommandWindow_SetVisible	( 0x60020000 )

#define	DISPID_IAgentCommandWindow_GetVisible	( 0x60020001 )

#define	DISPID_IAgentCommandWindow_GetPosition	( 0x60020002 )

#define	DISPID_IAgentCommandWindow_GetSize	( 0x60020003 )


EXTERN_C const IID LIBID_DoubleAgentSvr;

#ifndef __IDaServer_INTERFACE_DEFINED__
#define __IDaServer_INTERFACE_DEFINED__

/* interface IDaServer */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaServer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E502-A208-11DE-ABF2-002421116FB2")
    IDaServer : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Load( 
            /* [in] */ VARIANT vLoadKey,
            /* [out] */ long *pdwCharID,
            /* [out] */ long *pdwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Unload( 
            /* [in] */ long dwCharID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Register( 
            /* [in] */ IUnknown *punkNotifySink,
            /* [out] */ long *pdwSinkID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Unregister( 
            /* [in] */ long dwSinkID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCharacter( 
            /* [in] */ long dwCharID,
            /* [out] */ IDispatch **ppunkCharacter) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE GetSuspended( 
            /* [out] */ long *pbSuspended) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCharacterEx( 
            /* [in] */ long dwCharID,
            /* [out] */ IDaSvrCharacter **ppCharacterEx) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVersion( 
            /* [out] */ short *psMajor,
            /* [out] */ short *psMinor) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ShowDefaultCharacterProperties( 
            /* [in] */ short x,
            /* [in] */ short y,
            /* [in] */ long bUseDefaultPosition) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaServerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaServer * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaServer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaServer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaServer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaServer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaServer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaServer * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Load )( 
            IDaServer * This,
            /* [in] */ VARIANT vLoadKey,
            /* [out] */ long *pdwCharID,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Unload )( 
            IDaServer * This,
            /* [in] */ long dwCharID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Register )( 
            IDaServer * This,
            /* [in] */ IUnknown *punkNotifySink,
            /* [out] */ long *pdwSinkID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Unregister )( 
            IDaServer * This,
            /* [in] */ long dwSinkID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCharacter )( 
            IDaServer * This,
            /* [in] */ long dwCharID,
            /* [out] */ IDispatch **ppunkCharacter);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetSuspended )( 
            IDaServer * This,
            /* [out] */ long *pbSuspended);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCharacterEx )( 
            IDaServer * This,
            /* [in] */ long dwCharID,
            /* [out] */ IDaSvrCharacter **ppCharacterEx);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVersion )( 
            IDaServer * This,
            /* [out] */ short *psMajor,
            /* [out] */ short *psMinor);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ShowDefaultCharacterProperties )( 
            IDaServer * This,
            /* [in] */ short x,
            /* [in] */ short y,
            /* [in] */ long bUseDefaultPosition);
        
        END_INTERFACE
    } IDaServerVtbl;

    interface IDaServer
    {
        CONST_VTBL struct IDaServerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaServer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaServer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaServer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaServer_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaServer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaServer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaServer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaServer_Load(This,vLoadKey,pdwCharID,pdwReqID)	\
    ( (This)->lpVtbl -> Load(This,vLoadKey,pdwCharID,pdwReqID) ) 

#define IDaServer_Unload(This,dwCharID)	\
    ( (This)->lpVtbl -> Unload(This,dwCharID) ) 

#define IDaServer_Register(This,punkNotifySink,pdwSinkID)	\
    ( (This)->lpVtbl -> Register(This,punkNotifySink,pdwSinkID) ) 

#define IDaServer_Unregister(This,dwSinkID)	\
    ( (This)->lpVtbl -> Unregister(This,dwSinkID) ) 

#define IDaServer_GetCharacter(This,dwCharID,ppunkCharacter)	\
    ( (This)->lpVtbl -> GetCharacter(This,dwCharID,ppunkCharacter) ) 

#define IDaServer_GetSuspended(This,pbSuspended)	\
    ( (This)->lpVtbl -> GetSuspended(This,pbSuspended) ) 

#define IDaServer_GetCharacterEx(This,dwCharID,ppCharacterEx)	\
    ( (This)->lpVtbl -> GetCharacterEx(This,dwCharID,ppCharacterEx) ) 

#define IDaServer_GetVersion(This,psMajor,psMinor)	\
    ( (This)->lpVtbl -> GetVersion(This,psMajor,psMinor) ) 

#define IDaServer_ShowDefaultCharacterProperties(This,x,y,bUseDefaultPosition)	\
    ( (This)->lpVtbl -> ShowDefaultCharacterProperties(This,x,y,bUseDefaultPosition) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaServer_INTERFACE_DEFINED__ */


#ifndef __IDaSvrUserInput_INTERFACE_DEFINED__
#define __IDaSvrUserInput_INTERFACE_DEFINED__

/* interface IDaSvrUserInput */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrUserInput;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E508-A208-11DE-ABF2-002421116FB2")
    IDaSvrUserInput : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCount( 
            /* [out] */ long *pdwCount) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetItemID( 
            /* [in] */ long dwItemIndex,
            /* [out] */ long *pdwCommandID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetItemConfidence( 
            /* [in] */ long dwItemIndex,
            /* [out] */ long *plConfidence) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetItemText( 
            /* [in] */ long dwItemIndex,
            /* [out] */ BSTR *pbszText) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetAllItemData( 
            /* [out] */ VARIANT *pdwItemIndices,
            /* [out] */ VARIANT *plConfidences,
            /* [out] */ VARIANT *pbszText) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrUserInputVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrUserInput * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrUserInput * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrUserInput * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrUserInput * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrUserInput * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrUserInput * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrUserInput * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IDaSvrUserInput * This,
            /* [out] */ long *pdwCount);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetItemID )( 
            IDaSvrUserInput * This,
            /* [in] */ long dwItemIndex,
            /* [out] */ long *pdwCommandID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetItemConfidence )( 
            IDaSvrUserInput * This,
            /* [in] */ long dwItemIndex,
            /* [out] */ long *plConfidence);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetItemText )( 
            IDaSvrUserInput * This,
            /* [in] */ long dwItemIndex,
            /* [out] */ BSTR *pbszText);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetAllItemData )( 
            IDaSvrUserInput * This,
            /* [out] */ VARIANT *pdwItemIndices,
            /* [out] */ VARIANT *plConfidences,
            /* [out] */ VARIANT *pbszText);
        
        END_INTERFACE
    } IDaSvrUserInputVtbl;

    interface IDaSvrUserInput
    {
        CONST_VTBL struct IDaSvrUserInputVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrUserInput_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrUserInput_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrUserInput_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrUserInput_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrUserInput_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrUserInput_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrUserInput_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrUserInput_GetCount(This,pdwCount)	\
    ( (This)->lpVtbl -> GetCount(This,pdwCount) ) 

#define IDaSvrUserInput_GetItemID(This,dwItemIndex,pdwCommandID)	\
    ( (This)->lpVtbl -> GetItemID(This,dwItemIndex,pdwCommandID) ) 

#define IDaSvrUserInput_GetItemConfidence(This,dwItemIndex,plConfidence)	\
    ( (This)->lpVtbl -> GetItemConfidence(This,dwItemIndex,plConfidence) ) 

#define IDaSvrUserInput_GetItemText(This,dwItemIndex,pbszText)	\
    ( (This)->lpVtbl -> GetItemText(This,dwItemIndex,pbszText) ) 

#define IDaSvrUserInput_GetAllItemData(This,pdwItemIndices,plConfidences,pbszText)	\
    ( (This)->lpVtbl -> GetAllItemData(This,pdwItemIndices,plConfidences,pbszText) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrUserInput_INTERFACE_DEFINED__ */


#ifndef __IDaSvrCommand_INTERFACE_DEFINED__
#define __IDaSvrCommand_INTERFACE_DEFINED__

/* interface IDaSvrCommand */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrCommand;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E506-A208-11DE-ABF2-002421116FB2")
    IDaSvrCommand : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetCaption( 
            /* [in] */ BSTR bszCaption) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCaption( 
            /* [out] */ BSTR *pbszCaption) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetVoice( 
            /* [in] */ BSTR bszVoice) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVoice( 
            /* [out] */ BSTR *pbszVoice) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetEnabled( 
            /* [in] */ long bEnabled) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetEnabled( 
            /* [out] */ long *pbEnabled) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetVisible( 
            /* [in] */ long bVisible) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVisible( 
            /* [out] */ long *pbVisible) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetConfidenceThreshold( 
            /* [in] */ long lThreshold) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetConfidenceThreshold( 
            /* [out] */ long *plThreshold) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetConfidenceText( 
            /* [in] */ BSTR bszTipText) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetConfidenceText( 
            /* [out] */ BSTR *pbszTipText) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetID( 
            /* [out] */ long *pdwID) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE SetHelpContextID( 
            /* [in] */ long ulID) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE GetHelpContextID( 
            /* [out] */ long *pulID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetVoiceCaption( 
            /* [in] */ BSTR bszVoiceCaption) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVoiceCaption( 
            /* [out] */ BSTR *pbszVoiceCaption) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrCommandVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrCommand * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrCommand * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrCommand * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrCommand * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrCommand * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrCommand * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrCommand * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetCaption )( 
            IDaSvrCommand * This,
            /* [in] */ BSTR bszCaption);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCaption )( 
            IDaSvrCommand * This,
            /* [out] */ BSTR *pbszCaption);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetVoice )( 
            IDaSvrCommand * This,
            /* [in] */ BSTR bszVoice);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVoice )( 
            IDaSvrCommand * This,
            /* [out] */ BSTR *pbszVoice);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetEnabled )( 
            IDaSvrCommand * This,
            /* [in] */ long bEnabled);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetEnabled )( 
            IDaSvrCommand * This,
            /* [out] */ long *pbEnabled);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetVisible )( 
            IDaSvrCommand * This,
            /* [in] */ long bVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrCommand * This,
            /* [out] */ long *pbVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetConfidenceThreshold )( 
            IDaSvrCommand * This,
            /* [in] */ long lThreshold);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetConfidenceThreshold )( 
            IDaSvrCommand * This,
            /* [out] */ long *plThreshold);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetConfidenceText )( 
            IDaSvrCommand * This,
            /* [in] */ BSTR bszTipText);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetConfidenceText )( 
            IDaSvrCommand * This,
            /* [out] */ BSTR *pbszTipText);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetID )( 
            IDaSvrCommand * This,
            /* [out] */ long *pdwID);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetHelpContextID )( 
            IDaSvrCommand * This,
            /* [in] */ long ulID);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetHelpContextID )( 
            IDaSvrCommand * This,
            /* [out] */ long *pulID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetVoiceCaption )( 
            IDaSvrCommand * This,
            /* [in] */ BSTR bszVoiceCaption);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVoiceCaption )( 
            IDaSvrCommand * This,
            /* [out] */ BSTR *pbszVoiceCaption);
        
        END_INTERFACE
    } IDaSvrCommandVtbl;

    interface IDaSvrCommand
    {
        CONST_VTBL struct IDaSvrCommandVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrCommand_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrCommand_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrCommand_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrCommand_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrCommand_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrCommand_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrCommand_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrCommand_SetCaption(This,bszCaption)	\
    ( (This)->lpVtbl -> SetCaption(This,bszCaption) ) 

#define IDaSvrCommand_GetCaption(This,pbszCaption)	\
    ( (This)->lpVtbl -> GetCaption(This,pbszCaption) ) 

#define IDaSvrCommand_SetVoice(This,bszVoice)	\
    ( (This)->lpVtbl -> SetVoice(This,bszVoice) ) 

#define IDaSvrCommand_GetVoice(This,pbszVoice)	\
    ( (This)->lpVtbl -> GetVoice(This,pbszVoice) ) 

#define IDaSvrCommand_SetEnabled(This,bEnabled)	\
    ( (This)->lpVtbl -> SetEnabled(This,bEnabled) ) 

#define IDaSvrCommand_GetEnabled(This,pbEnabled)	\
    ( (This)->lpVtbl -> GetEnabled(This,pbEnabled) ) 

#define IDaSvrCommand_SetVisible(This,bVisible)	\
    ( (This)->lpVtbl -> SetVisible(This,bVisible) ) 

#define IDaSvrCommand_GetVisible(This,pbVisible)	\
    ( (This)->lpVtbl -> GetVisible(This,pbVisible) ) 

#define IDaSvrCommand_SetConfidenceThreshold(This,lThreshold)	\
    ( (This)->lpVtbl -> SetConfidenceThreshold(This,lThreshold) ) 

#define IDaSvrCommand_GetConfidenceThreshold(This,plThreshold)	\
    ( (This)->lpVtbl -> GetConfidenceThreshold(This,plThreshold) ) 

#define IDaSvrCommand_SetConfidenceText(This,bszTipText)	\
    ( (This)->lpVtbl -> SetConfidenceText(This,bszTipText) ) 

#define IDaSvrCommand_GetConfidenceText(This,pbszTipText)	\
    ( (This)->lpVtbl -> GetConfidenceText(This,pbszTipText) ) 

#define IDaSvrCommand_GetID(This,pdwID)	\
    ( (This)->lpVtbl -> GetID(This,pdwID) ) 

#define IDaSvrCommand_SetHelpContextID(This,ulID)	\
    ( (This)->lpVtbl -> SetHelpContextID(This,ulID) ) 

#define IDaSvrCommand_GetHelpContextID(This,pulID)	\
    ( (This)->lpVtbl -> GetHelpContextID(This,pulID) ) 

#define IDaSvrCommand_SetVoiceCaption(This,bszVoiceCaption)	\
    ( (This)->lpVtbl -> SetVoiceCaption(This,bszVoiceCaption) ) 

#define IDaSvrCommand_GetVoiceCaption(This,pbszVoiceCaption)	\
    ( (This)->lpVtbl -> GetVoiceCaption(This,pbszVoiceCaption) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrCommand_INTERFACE_DEFINED__ */


#ifndef __IDaSvrCommands_INTERFACE_DEFINED__
#define __IDaSvrCommands_INTERFACE_DEFINED__

/* interface IDaSvrCommands */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrCommands;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E505-A208-11DE-ABF2-002421116FB2")
    IDaSvrCommands : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCommand( 
            /* [in] */ long dwCommandID,
            /* [out] */ IUnknown **ppunkCommand) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCount( 
            /* [out] */ long *pdwCount) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetCaption( 
            /* [in] */ BSTR bszCaption) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCaption( 
            /* [out] */ BSTR *pbszCaption) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetVoice( 
            /* [in] */ BSTR bszVoice) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVoice( 
            /* [out] */ BSTR *pbszVoice) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetVisible( 
            /* [in] */ long bVisible) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVisible( 
            /* [out] */ long *pbVisible) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ BSTR bszCaption,
            /* [in] */ BSTR bszVoice,
            /* [in] */ long bEnabled,
            /* [in] */ long bVisible,
            /* [out] */ long *pdwID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Insert( 
            /* [in] */ BSTR bszCaption,
            /* [in] */ BSTR bszVoice,
            /* [in] */ long bEnabled,
            /* [in] */ long bVisible,
            /* [in] */ long dwRefID,
            /* [in] */ long bBefore,
            /* [out] */ long *pdwID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ long dwID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RemoveAll( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCommandEx( 
            /* [in] */ long dwCommandID,
            /* [out] */ IDaSvrCommand **ppCommandEx) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetDefaultID( 
            /* [in] */ long dwID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetDefaultID( 
            /* [out] */ long *pdwID) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE SetHelpContextID( 
            /* [in] */ long ulHelpID) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE GetHelpContextID( 
            /* [out] */ long *pulHelpID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetFontName( 
            /* [in] */ BSTR bszFontName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFontName( 
            /* [out] */ BSTR *pbszFontName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetFontSize( 
            /* [in] */ long lFontSize) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFontSize( 
            /* [out] */ long *lFontSize) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetVoiceCaption( 
            /* [in] */ BSTR bszVoiceCaption) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVoiceCaption( 
            /* [out] */ BSTR *bszVoiceCaption) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddEx( 
            /* [in] */ BSTR bszCaption,
            /* [in] */ BSTR bszVoice,
            /* [in] */ BSTR bszVoiceCaption,
            /* [in] */ long bEnabled,
            /* [in] */ long bVisible,
            /* [in] */ long ulHelpID,
            /* [out] */ long *pdwID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE InsertEx( 
            /* [in] */ BSTR bszCaption,
            /* [in] */ BSTR bszVoice,
            /* [in] */ BSTR bszVoiceCaption,
            /* [in] */ long bEnabled,
            /* [in] */ long bVisible,
            /* [in] */ long ulHelpID,
            /* [in] */ long dwRefID,
            /* [in] */ long bBefore,
            /* [out] */ long *pdwID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetGlobalVoiceCommandsEnabled( 
            /* [in] */ long bEnable) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGlobalVoiceCommandsEnabled( 
            /* [out] */ long *pbEnabled) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrCommandsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrCommands * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrCommands * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrCommands * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrCommands * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrCommands * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrCommands * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrCommands * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCommand )( 
            IDaSvrCommands * This,
            /* [in] */ long dwCommandID,
            /* [out] */ IUnknown **ppunkCommand);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IDaSvrCommands * This,
            /* [out] */ long *pdwCount);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetCaption )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR bszCaption);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCaption )( 
            IDaSvrCommands * This,
            /* [out] */ BSTR *pbszCaption);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetVoice )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR bszVoice);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVoice )( 
            IDaSvrCommands * This,
            /* [out] */ BSTR *pbszVoice);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetVisible )( 
            IDaSvrCommands * This,
            /* [in] */ long bVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrCommands * This,
            /* [out] */ long *pbVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR bszCaption,
            /* [in] */ BSTR bszVoice,
            /* [in] */ long bEnabled,
            /* [in] */ long bVisible,
            /* [out] */ long *pdwID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Insert )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR bszCaption,
            /* [in] */ BSTR bszVoice,
            /* [in] */ long bEnabled,
            /* [in] */ long bVisible,
            /* [in] */ long dwRefID,
            /* [in] */ long bBefore,
            /* [out] */ long *pdwID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Remove )( 
            IDaSvrCommands * This,
            /* [in] */ long dwID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RemoveAll )( 
            IDaSvrCommands * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCommandEx )( 
            IDaSvrCommands * This,
            /* [in] */ long dwCommandID,
            /* [out] */ IDaSvrCommand **ppCommandEx);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetDefaultID )( 
            IDaSvrCommands * This,
            /* [in] */ long dwID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetDefaultID )( 
            IDaSvrCommands * This,
            /* [out] */ long *pdwID);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetHelpContextID )( 
            IDaSvrCommands * This,
            /* [in] */ long ulHelpID);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetHelpContextID )( 
            IDaSvrCommands * This,
            /* [out] */ long *pulHelpID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetFontName )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR bszFontName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFontName )( 
            IDaSvrCommands * This,
            /* [out] */ BSTR *pbszFontName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetFontSize )( 
            IDaSvrCommands * This,
            /* [in] */ long lFontSize);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFontSize )( 
            IDaSvrCommands * This,
            /* [out] */ long *lFontSize);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetVoiceCaption )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR bszVoiceCaption);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVoiceCaption )( 
            IDaSvrCommands * This,
            /* [out] */ BSTR *bszVoiceCaption);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AddEx )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR bszCaption,
            /* [in] */ BSTR bszVoice,
            /* [in] */ BSTR bszVoiceCaption,
            /* [in] */ long bEnabled,
            /* [in] */ long bVisible,
            /* [in] */ long ulHelpID,
            /* [out] */ long *pdwID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *InsertEx )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR bszCaption,
            /* [in] */ BSTR bszVoice,
            /* [in] */ BSTR bszVoiceCaption,
            /* [in] */ long bEnabled,
            /* [in] */ long bVisible,
            /* [in] */ long ulHelpID,
            /* [in] */ long dwRefID,
            /* [in] */ long bBefore,
            /* [out] */ long *pdwID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetGlobalVoiceCommandsEnabled )( 
            IDaSvrCommands * This,
            /* [in] */ long bEnable);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetGlobalVoiceCommandsEnabled )( 
            IDaSvrCommands * This,
            /* [out] */ long *pbEnabled);
        
        END_INTERFACE
    } IDaSvrCommandsVtbl;

    interface IDaSvrCommands
    {
        CONST_VTBL struct IDaSvrCommandsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrCommands_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrCommands_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrCommands_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrCommands_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrCommands_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrCommands_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrCommands_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrCommands_GetCommand(This,dwCommandID,ppunkCommand)	\
    ( (This)->lpVtbl -> GetCommand(This,dwCommandID,ppunkCommand) ) 

#define IDaSvrCommands_GetCount(This,pdwCount)	\
    ( (This)->lpVtbl -> GetCount(This,pdwCount) ) 

#define IDaSvrCommands_SetCaption(This,bszCaption)	\
    ( (This)->lpVtbl -> SetCaption(This,bszCaption) ) 

#define IDaSvrCommands_GetCaption(This,pbszCaption)	\
    ( (This)->lpVtbl -> GetCaption(This,pbszCaption) ) 

#define IDaSvrCommands_SetVoice(This,bszVoice)	\
    ( (This)->lpVtbl -> SetVoice(This,bszVoice) ) 

#define IDaSvrCommands_GetVoice(This,pbszVoice)	\
    ( (This)->lpVtbl -> GetVoice(This,pbszVoice) ) 

#define IDaSvrCommands_SetVisible(This,bVisible)	\
    ( (This)->lpVtbl -> SetVisible(This,bVisible) ) 

#define IDaSvrCommands_GetVisible(This,pbVisible)	\
    ( (This)->lpVtbl -> GetVisible(This,pbVisible) ) 

#define IDaSvrCommands_Add(This,bszCaption,bszVoice,bEnabled,bVisible,pdwID)	\
    ( (This)->lpVtbl -> Add(This,bszCaption,bszVoice,bEnabled,bVisible,pdwID) ) 

#define IDaSvrCommands_Insert(This,bszCaption,bszVoice,bEnabled,bVisible,dwRefID,bBefore,pdwID)	\
    ( (This)->lpVtbl -> Insert(This,bszCaption,bszVoice,bEnabled,bVisible,dwRefID,bBefore,pdwID) ) 

#define IDaSvrCommands_Remove(This,dwID)	\
    ( (This)->lpVtbl -> Remove(This,dwID) ) 

#define IDaSvrCommands_RemoveAll(This)	\
    ( (This)->lpVtbl -> RemoveAll(This) ) 

#define IDaSvrCommands_GetCommandEx(This,dwCommandID,ppCommandEx)	\
    ( (This)->lpVtbl -> GetCommandEx(This,dwCommandID,ppCommandEx) ) 

#define IDaSvrCommands_SetDefaultID(This,dwID)	\
    ( (This)->lpVtbl -> SetDefaultID(This,dwID) ) 

#define IDaSvrCommands_GetDefaultID(This,pdwID)	\
    ( (This)->lpVtbl -> GetDefaultID(This,pdwID) ) 

#define IDaSvrCommands_SetHelpContextID(This,ulHelpID)	\
    ( (This)->lpVtbl -> SetHelpContextID(This,ulHelpID) ) 

#define IDaSvrCommands_GetHelpContextID(This,pulHelpID)	\
    ( (This)->lpVtbl -> GetHelpContextID(This,pulHelpID) ) 

#define IDaSvrCommands_SetFontName(This,bszFontName)	\
    ( (This)->lpVtbl -> SetFontName(This,bszFontName) ) 

#define IDaSvrCommands_GetFontName(This,pbszFontName)	\
    ( (This)->lpVtbl -> GetFontName(This,pbszFontName) ) 

#define IDaSvrCommands_SetFontSize(This,lFontSize)	\
    ( (This)->lpVtbl -> SetFontSize(This,lFontSize) ) 

#define IDaSvrCommands_GetFontSize(This,lFontSize)	\
    ( (This)->lpVtbl -> GetFontSize(This,lFontSize) ) 

#define IDaSvrCommands_SetVoiceCaption(This,bszVoiceCaption)	\
    ( (This)->lpVtbl -> SetVoiceCaption(This,bszVoiceCaption) ) 

#define IDaSvrCommands_GetVoiceCaption(This,bszVoiceCaption)	\
    ( (This)->lpVtbl -> GetVoiceCaption(This,bszVoiceCaption) ) 

#define IDaSvrCommands_AddEx(This,bszCaption,bszVoice,bszVoiceCaption,bEnabled,bVisible,ulHelpID,pdwID)	\
    ( (This)->lpVtbl -> AddEx(This,bszCaption,bszVoice,bszVoiceCaption,bEnabled,bVisible,ulHelpID,pdwID) ) 

#define IDaSvrCommands_InsertEx(This,bszCaption,bszVoice,bszVoiceCaption,bEnabled,bVisible,ulHelpID,dwRefID,bBefore,pdwID)	\
    ( (This)->lpVtbl -> InsertEx(This,bszCaption,bszVoice,bszVoiceCaption,bEnabled,bVisible,ulHelpID,dwRefID,bBefore,pdwID) ) 

#define IDaSvrCommands_SetGlobalVoiceCommandsEnabled(This,bEnable)	\
    ( (This)->lpVtbl -> SetGlobalVoiceCommandsEnabled(This,bEnable) ) 

#define IDaSvrCommands_GetGlobalVoiceCommandsEnabled(This,pbEnabled)	\
    ( (This)->lpVtbl -> GetGlobalVoiceCommandsEnabled(This,pbEnabled) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrCommands_INTERFACE_DEFINED__ */


#ifndef __IDaSvrSpeechInputProperties_INTERFACE_DEFINED__
#define __IDaSvrSpeechInputProperties_INTERFACE_DEFINED__

/* interface IDaSvrSpeechInputProperties */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrSpeechInputProperties;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E50C-A208-11DE-ABF2-002421116FB2")
    IDaSvrSpeechInputProperties : public IDispatch
    {
    public:
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE GetInstalled( 
            /* [out] */ long *pbInstalled) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetEnabled( 
            /* [out] */ long *pbEnabled) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetHotKey( 
            /* [out] */ BSTR *pbszHotCharKey) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE GetLCID( 
            /* [out] */ unsigned long *plcidCurrent) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE GetEngine( 
            /* [out] */ BSTR *pbszEngine) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE SetEngine( 
            /* [in] */ BSTR bszEngine) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetListeningTip( 
            /* [out] */ long *pbListeningTip) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrSpeechInputPropertiesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrSpeechInputProperties * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrSpeechInputProperties * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrSpeechInputProperties * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrSpeechInputProperties * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrSpeechInputProperties * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrSpeechInputProperties * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrSpeechInputProperties * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetInstalled )( 
            IDaSvrSpeechInputProperties * This,
            /* [out] */ long *pbInstalled);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetEnabled )( 
            IDaSvrSpeechInputProperties * This,
            /* [out] */ long *pbEnabled);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetHotKey )( 
            IDaSvrSpeechInputProperties * This,
            /* [out] */ BSTR *pbszHotCharKey);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetLCID )( 
            IDaSvrSpeechInputProperties * This,
            /* [out] */ unsigned long *plcidCurrent);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetEngine )( 
            IDaSvrSpeechInputProperties * This,
            /* [out] */ BSTR *pbszEngine);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetEngine )( 
            IDaSvrSpeechInputProperties * This,
            /* [in] */ BSTR bszEngine);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetListeningTip )( 
            IDaSvrSpeechInputProperties * This,
            /* [out] */ long *pbListeningTip);
        
        END_INTERFACE
    } IDaSvrSpeechInputPropertiesVtbl;

    interface IDaSvrSpeechInputProperties
    {
        CONST_VTBL struct IDaSvrSpeechInputPropertiesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrSpeechInputProperties_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrSpeechInputProperties_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrSpeechInputProperties_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrSpeechInputProperties_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrSpeechInputProperties_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrSpeechInputProperties_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrSpeechInputProperties_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrSpeechInputProperties_GetInstalled(This,pbInstalled)	\
    ( (This)->lpVtbl -> GetInstalled(This,pbInstalled) ) 

#define IDaSvrSpeechInputProperties_GetEnabled(This,pbEnabled)	\
    ( (This)->lpVtbl -> GetEnabled(This,pbEnabled) ) 

#define IDaSvrSpeechInputProperties_GetHotKey(This,pbszHotCharKey)	\
    ( (This)->lpVtbl -> GetHotKey(This,pbszHotCharKey) ) 

#define IDaSvrSpeechInputProperties_GetLCID(This,plcidCurrent)	\
    ( (This)->lpVtbl -> GetLCID(This,plcidCurrent) ) 

#define IDaSvrSpeechInputProperties_GetEngine(This,pbszEngine)	\
    ( (This)->lpVtbl -> GetEngine(This,pbszEngine) ) 

#define IDaSvrSpeechInputProperties_SetEngine(This,bszEngine)	\
    ( (This)->lpVtbl -> SetEngine(This,bszEngine) ) 

#define IDaSvrSpeechInputProperties_GetListeningTip(This,pbListeningTip)	\
    ( (This)->lpVtbl -> GetListeningTip(This,pbListeningTip) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrSpeechInputProperties_INTERFACE_DEFINED__ */


#ifndef __IDaSvrAudioOutputProperties_INTERFACE_DEFINED__
#define __IDaSvrAudioOutputProperties_INTERFACE_DEFINED__

/* interface IDaSvrAudioOutputProperties */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrAudioOutputProperties;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E50B-A208-11DE-ABF2-002421116FB2")
    IDaSvrAudioOutputProperties : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetEnabled( 
            /* [out] */ long *pbEnabled) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetUsingSoundEffects( 
            /* [out] */ long *pbUsingSoundEffects) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE GetStatus( 
            /* [out] */ long *plStatus) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrAudioOutputPropertiesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrAudioOutputProperties * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrAudioOutputProperties * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrAudioOutputProperties * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrAudioOutputProperties * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrAudioOutputProperties * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrAudioOutputProperties * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrAudioOutputProperties * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetEnabled )( 
            IDaSvrAudioOutputProperties * This,
            /* [out] */ long *pbEnabled);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetUsingSoundEffects )( 
            IDaSvrAudioOutputProperties * This,
            /* [out] */ long *pbUsingSoundEffects);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetStatus )( 
            IDaSvrAudioOutputProperties * This,
            /* [out] */ long *plStatus);
        
        END_INTERFACE
    } IDaSvrAudioOutputPropertiesVtbl;

    interface IDaSvrAudioOutputProperties
    {
        CONST_VTBL struct IDaSvrAudioOutputPropertiesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrAudioOutputProperties_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrAudioOutputProperties_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrAudioOutputProperties_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrAudioOutputProperties_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrAudioOutputProperties_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrAudioOutputProperties_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrAudioOutputProperties_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrAudioOutputProperties_GetEnabled(This,pbEnabled)	\
    ( (This)->lpVtbl -> GetEnabled(This,pbEnabled) ) 

#define IDaSvrAudioOutputProperties_GetUsingSoundEffects(This,pbUsingSoundEffects)	\
    ( (This)->lpVtbl -> GetUsingSoundEffects(This,pbUsingSoundEffects) ) 

#define IDaSvrAudioOutputProperties_GetStatus(This,plStatus)	\
    ( (This)->lpVtbl -> GetStatus(This,plStatus) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrAudioOutputProperties_INTERFACE_DEFINED__ */


#ifndef __IDaSvrPropertySheet_INTERFACE_DEFINED__
#define __IDaSvrPropertySheet_INTERFACE_DEFINED__

/* interface IDaSvrPropertySheet */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrPropertySheet;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E50A-A208-11DE-ABF2-002421116FB2")
    IDaSvrPropertySheet : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVisible( 
            /* [out] */ long *pbVisible) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetVisible( 
            /* [in] */ long bVisible) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetPosition( 
            /* [out] */ long *plLeft,
            /* [out] */ long *plTop) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSize( 
            /* [out] */ long *plWidth,
            /* [out] */ long *plHeight) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetPage( 
            /* [out] */ BSTR *pbszPage) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetPage( 
            /* [in] */ BSTR bszPage) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrPropertySheetVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrPropertySheet * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrPropertySheet * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrPropertySheet * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrPropertySheet * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrPropertySheet * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrPropertySheet * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrPropertySheet * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrPropertySheet * This,
            /* [out] */ long *pbVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetVisible )( 
            IDaSvrPropertySheet * This,
            /* [in] */ long bVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetPosition )( 
            IDaSvrPropertySheet * This,
            /* [out] */ long *plLeft,
            /* [out] */ long *plTop);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSize )( 
            IDaSvrPropertySheet * This,
            /* [out] */ long *plWidth,
            /* [out] */ long *plHeight);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetPage )( 
            IDaSvrPropertySheet * This,
            /* [out] */ BSTR *pbszPage);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetPage )( 
            IDaSvrPropertySheet * This,
            /* [in] */ BSTR bszPage);
        
        END_INTERFACE
    } IDaSvrPropertySheetVtbl;

    interface IDaSvrPropertySheet
    {
        CONST_VTBL struct IDaSvrPropertySheetVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrPropertySheet_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrPropertySheet_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrPropertySheet_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrPropertySheet_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrPropertySheet_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrPropertySheet_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrPropertySheet_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrPropertySheet_GetVisible(This,pbVisible)	\
    ( (This)->lpVtbl -> GetVisible(This,pbVisible) ) 

#define IDaSvrPropertySheet_SetVisible(This,bVisible)	\
    ( (This)->lpVtbl -> SetVisible(This,bVisible) ) 

#define IDaSvrPropertySheet_GetPosition(This,plLeft,plTop)	\
    ( (This)->lpVtbl -> GetPosition(This,plLeft,plTop) ) 

#define IDaSvrPropertySheet_GetSize(This,plWidth,plHeight)	\
    ( (This)->lpVtbl -> GetSize(This,plWidth,plHeight) ) 

#define IDaSvrPropertySheet_GetPage(This,pbszPage)	\
    ( (This)->lpVtbl -> GetPage(This,pbszPage) ) 

#define IDaSvrPropertySheet_SetPage(This,bszPage)	\
    ( (This)->lpVtbl -> SetPage(This,bszPage) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrPropertySheet_INTERFACE_DEFINED__ */


#ifndef __IDaSvrBalloon_INTERFACE_DEFINED__
#define __IDaSvrBalloon_INTERFACE_DEFINED__

/* interface IDaSvrBalloon */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrBalloon;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E509-A208-11DE-ABF2-002421116FB2")
    IDaSvrBalloon : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetEnabled( 
            /* [out] */ long *pbEnabled) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetNumLines( 
            /* [out] */ long *plLines) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetNumCharsPerLine( 
            /* [out] */ long *plCharsPerLine) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFontName( 
            /* [out] */ BSTR *pbszFontName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFontSize( 
            /* [out] */ long *plFontSize) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFontBold( 
            /* [out] */ long *pbFontBold) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFontItalic( 
            /* [out] */ long *pbFontItalic) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFontStrikethru( 
            /* [out] */ long *pbFontStrikethru) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFontUnderline( 
            /* [out] */ long *pbFontUnderline) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetForeColor( 
            /* [out] */ long *plFGColor) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetBackColor( 
            /* [out] */ long *plBGColor) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetBorderColor( 
            /* [out] */ long *plBorderColor) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetVisible( 
            /* [in] */ long bVisible) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVisible( 
            /* [out] */ long *pbVisible) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetFontName( 
            /* [in] */ BSTR bszFontName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetFontSize( 
            /* [in] */ long lFontSize) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetFontCharSet( 
            /* [in] */ short sFontCharSet) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFontCharSet( 
            /* [out] */ short *psFontCharSet) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetStyle( 
            /* [in] */ long lStyle) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetStyle( 
            /* [out] */ long *plStyle) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetNumLines( 
            /* [in] */ long lLines) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetNumCharsPerLine( 
            /* [in] */ long lCharsPerLine) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrBalloonVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrBalloon * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrBalloon * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrBalloon * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrBalloon * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrBalloon * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrBalloon * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrBalloon * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetEnabled )( 
            IDaSvrBalloon * This,
            /* [out] */ long *pbEnabled);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetNumLines )( 
            IDaSvrBalloon * This,
            /* [out] */ long *plLines);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetNumCharsPerLine )( 
            IDaSvrBalloon * This,
            /* [out] */ long *plCharsPerLine);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFontName )( 
            IDaSvrBalloon * This,
            /* [out] */ BSTR *pbszFontName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFontSize )( 
            IDaSvrBalloon * This,
            /* [out] */ long *plFontSize);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFontBold )( 
            IDaSvrBalloon * This,
            /* [out] */ long *pbFontBold);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFontItalic )( 
            IDaSvrBalloon * This,
            /* [out] */ long *pbFontItalic);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFontStrikethru )( 
            IDaSvrBalloon * This,
            /* [out] */ long *pbFontStrikethru);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFontUnderline )( 
            IDaSvrBalloon * This,
            /* [out] */ long *pbFontUnderline);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetForeColor )( 
            IDaSvrBalloon * This,
            /* [out] */ long *plFGColor);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetBackColor )( 
            IDaSvrBalloon * This,
            /* [out] */ long *plBGColor);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetBorderColor )( 
            IDaSvrBalloon * This,
            /* [out] */ long *plBorderColor);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetVisible )( 
            IDaSvrBalloon * This,
            /* [in] */ long bVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrBalloon * This,
            /* [out] */ long *pbVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetFontName )( 
            IDaSvrBalloon * This,
            /* [in] */ BSTR bszFontName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetFontSize )( 
            IDaSvrBalloon * This,
            /* [in] */ long lFontSize);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetFontCharSet )( 
            IDaSvrBalloon * This,
            /* [in] */ short sFontCharSet);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFontCharSet )( 
            IDaSvrBalloon * This,
            /* [out] */ short *psFontCharSet);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetStyle )( 
            IDaSvrBalloon * This,
            /* [in] */ long lStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetStyle )( 
            IDaSvrBalloon * This,
            /* [out] */ long *plStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetNumLines )( 
            IDaSvrBalloon * This,
            /* [in] */ long lLines);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetNumCharsPerLine )( 
            IDaSvrBalloon * This,
            /* [in] */ long lCharsPerLine);
        
        END_INTERFACE
    } IDaSvrBalloonVtbl;

    interface IDaSvrBalloon
    {
        CONST_VTBL struct IDaSvrBalloonVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrBalloon_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrBalloon_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrBalloon_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrBalloon_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrBalloon_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrBalloon_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrBalloon_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrBalloon_GetEnabled(This,pbEnabled)	\
    ( (This)->lpVtbl -> GetEnabled(This,pbEnabled) ) 

#define IDaSvrBalloon_GetNumLines(This,plLines)	\
    ( (This)->lpVtbl -> GetNumLines(This,plLines) ) 

#define IDaSvrBalloon_GetNumCharsPerLine(This,plCharsPerLine)	\
    ( (This)->lpVtbl -> GetNumCharsPerLine(This,plCharsPerLine) ) 

#define IDaSvrBalloon_GetFontName(This,pbszFontName)	\
    ( (This)->lpVtbl -> GetFontName(This,pbszFontName) ) 

#define IDaSvrBalloon_GetFontSize(This,plFontSize)	\
    ( (This)->lpVtbl -> GetFontSize(This,plFontSize) ) 

#define IDaSvrBalloon_GetFontBold(This,pbFontBold)	\
    ( (This)->lpVtbl -> GetFontBold(This,pbFontBold) ) 

#define IDaSvrBalloon_GetFontItalic(This,pbFontItalic)	\
    ( (This)->lpVtbl -> GetFontItalic(This,pbFontItalic) ) 

#define IDaSvrBalloon_GetFontStrikethru(This,pbFontStrikethru)	\
    ( (This)->lpVtbl -> GetFontStrikethru(This,pbFontStrikethru) ) 

#define IDaSvrBalloon_GetFontUnderline(This,pbFontUnderline)	\
    ( (This)->lpVtbl -> GetFontUnderline(This,pbFontUnderline) ) 

#define IDaSvrBalloon_GetForeColor(This,plFGColor)	\
    ( (This)->lpVtbl -> GetForeColor(This,plFGColor) ) 

#define IDaSvrBalloon_GetBackColor(This,plBGColor)	\
    ( (This)->lpVtbl -> GetBackColor(This,plBGColor) ) 

#define IDaSvrBalloon_GetBorderColor(This,plBorderColor)	\
    ( (This)->lpVtbl -> GetBorderColor(This,plBorderColor) ) 

#define IDaSvrBalloon_SetVisible(This,bVisible)	\
    ( (This)->lpVtbl -> SetVisible(This,bVisible) ) 

#define IDaSvrBalloon_GetVisible(This,pbVisible)	\
    ( (This)->lpVtbl -> GetVisible(This,pbVisible) ) 

#define IDaSvrBalloon_SetFontName(This,bszFontName)	\
    ( (This)->lpVtbl -> SetFontName(This,bszFontName) ) 

#define IDaSvrBalloon_SetFontSize(This,lFontSize)	\
    ( (This)->lpVtbl -> SetFontSize(This,lFontSize) ) 

#define IDaSvrBalloon_SetFontCharSet(This,sFontCharSet)	\
    ( (This)->lpVtbl -> SetFontCharSet(This,sFontCharSet) ) 

#define IDaSvrBalloon_GetFontCharSet(This,psFontCharSet)	\
    ( (This)->lpVtbl -> GetFontCharSet(This,psFontCharSet) ) 

#define IDaSvrBalloon_SetStyle(This,lStyle)	\
    ( (This)->lpVtbl -> SetStyle(This,lStyle) ) 

#define IDaSvrBalloon_GetStyle(This,plStyle)	\
    ( (This)->lpVtbl -> GetStyle(This,plStyle) ) 

#define IDaSvrBalloon_SetNumLines(This,lLines)	\
    ( (This)->lpVtbl -> SetNumLines(This,lLines) ) 

#define IDaSvrBalloon_SetNumCharsPerLine(This,lCharsPerLine)	\
    ( (This)->lpVtbl -> SetNumCharsPerLine(This,lCharsPerLine) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrBalloon_INTERFACE_DEFINED__ */


#ifndef __IDaSvrCharacter_INTERFACE_DEFINED__
#define __IDaSvrCharacter_INTERFACE_DEFINED__

/* interface IDaSvrCharacter */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrCharacter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E504-A208-11DE-ABF2-002421116FB2")
    IDaSvrCharacter : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVisible( 
            /* [out] */ long *pbVisible) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetPosition( 
            /* [in] */ long lLeft,
            /* [in] */ long lTop) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetPosition( 
            /* [out] */ long *plLeft,
            /* [out] */ long *plTop) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetSize( 
            /* [in] */ long lWidth,
            /* [in] */ long lHeight) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSize( 
            /* [out] */ long *plWidth,
            /* [out] */ long *plHeight) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetName( 
            /* [out] */ BSTR *pbszName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetDescription( 
            /* [out] */ BSTR *pbszDescription) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetTTSSpeed( 
            /* [out] */ long *pdwSpeed) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetTTSPitch( 
            /* [out] */ short *pwPitch) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Activate( 
            /* [in] */ short sState) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetIdleOn( 
            /* [in] */ long bOn) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetIdleOn( 
            /* [out] */ long *pbOn) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Prepare( 
            /* [in] */ long dwType,
            /* [in] */ BSTR bszName,
            /* [in] */ long bQueue,
            /* [out] */ long *pdwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Play( 
            /* [in] */ BSTR bszAnimation,
            /* [out] */ long *pdwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Stop( 
            /* [in] */ long dwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE StopAll( 
            /* [in] */ long lTypes) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Wait( 
            /* [in] */ long dwReqID,
            /* [out] */ long *pdwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Interrupt( 
            /* [in] */ long dwReqID,
            /* [out] */ long *pdwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Show( 
            /* [in] */ long bFast,
            /* [out] */ long *pdwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Hide( 
            /* [in] */ long bFast,
            /* [out] */ long *pdwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Speak( 
            /* [in] */ BSTR bszText,
            /* [in] */ BSTR bszUrl,
            /* [out] */ long *pdwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE MoveTo( 
            /* [in] */ short x,
            /* [in] */ short y,
            /* [in] */ long lSpeed,
            /* [out] */ long *pdwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GestureAt( 
            /* [in] */ short x,
            /* [in] */ short y,
            /* [out] */ long *pdwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetMoveCause( 
            /* [out] */ long *pdwCause) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVisibilityCause( 
            /* [out] */ long *pdwCause) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE HasOtherClients( 
            /* [out] */ long *plNumOtherClients) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetSoundEffectsOn( 
            /* [in] */ long bOn) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSoundEffectsOn( 
            /* [out] */ long *pbOn) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetName( 
            /* [in] */ BSTR bszName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetDescription( 
            /* [in] */ BSTR bszDescription) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetExtraData( 
            /* [out] */ BSTR *pbszExtraData) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ShowPopupMenu( 
            /* [in] */ short x,
            /* [in] */ short y) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetAutoPopupMenu( 
            /* [in] */ long bAutoPopupMenu) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetAutoPopupMenu( 
            /* [out] */ long *pbAutoPopupMenu) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE GetHelpFileName( 
            /* [out] */ BSTR *pbszName) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE SetHelpFileName( 
            /* [in] */ BSTR bszName) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE SetHelpModeOn( 
            /* [in] */ long bHelpModeOn) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE GetHelpModeOn( 
            /* [out] */ long *pbHelpModeOn) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE SetHelpContextID( 
            /* [in] */ long ulID) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE GetHelpContextID( 
            /* [out] */ long *pulID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetActive( 
            /* [out] */ short *psState) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Listen( 
            /* [in] */ long bListen) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetLanguageID( 
            /* [in] */ long langid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetLanguageID( 
            /* [out] */ long *plangid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetTTSModeID( 
            /* [out] */ BSTR *pbszModeID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetTTSModeID( 
            /* [in] */ BSTR bszModeID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSRModeID( 
            /* [out] */ BSTR *pbszModeID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetSRModeID( 
            /* [in] */ BSTR bszModeID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGUID( 
            /* [out] */ BSTR *pbszID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetOriginalSize( 
            /* [out] */ long *plWidth,
            /* [out] */ long *plHeight) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Think( 
            /* [in] */ BSTR bszText,
            /* [out] */ long *pdwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVersion( 
            /* [out] */ short *psMajor,
            /* [out] */ short *psMinor) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetAnimationNames( 
            /* [out] */ IUnknown **punkEnum) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSRStatus( 
            /* [out] */ long *plStatus) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrCharacterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrCharacter * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrCharacter * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrCharacter * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrCharacter * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrCharacter * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrCharacter * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrCharacter * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrCharacter * This,
            /* [out] */ long *pbVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetPosition )( 
            IDaSvrCharacter * This,
            /* [in] */ long lLeft,
            /* [in] */ long lTop);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetPosition )( 
            IDaSvrCharacter * This,
            /* [out] */ long *plLeft,
            /* [out] */ long *plTop);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetSize )( 
            IDaSvrCharacter * This,
            /* [in] */ long lWidth,
            /* [in] */ long lHeight);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSize )( 
            IDaSvrCharacter * This,
            /* [out] */ long *plWidth,
            /* [out] */ long *plHeight);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetName )( 
            IDaSvrCharacter * This,
            /* [out] */ BSTR *pbszName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetDescription )( 
            IDaSvrCharacter * This,
            /* [out] */ BSTR *pbszDescription);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetTTSSpeed )( 
            IDaSvrCharacter * This,
            /* [out] */ long *pdwSpeed);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetTTSPitch )( 
            IDaSvrCharacter * This,
            /* [out] */ short *pwPitch);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Activate )( 
            IDaSvrCharacter * This,
            /* [in] */ short sState);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetIdleOn )( 
            IDaSvrCharacter * This,
            /* [in] */ long bOn);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetIdleOn )( 
            IDaSvrCharacter * This,
            /* [out] */ long *pbOn);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Prepare )( 
            IDaSvrCharacter * This,
            /* [in] */ long dwType,
            /* [in] */ BSTR bszName,
            /* [in] */ long bQueue,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Play )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR bszAnimation,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IDaSvrCharacter * This,
            /* [in] */ long dwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *StopAll )( 
            IDaSvrCharacter * This,
            /* [in] */ long lTypes);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Wait )( 
            IDaSvrCharacter * This,
            /* [in] */ long dwReqID,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Interrupt )( 
            IDaSvrCharacter * This,
            /* [in] */ long dwReqID,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Show )( 
            IDaSvrCharacter * This,
            /* [in] */ long bFast,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Hide )( 
            IDaSvrCharacter * This,
            /* [in] */ long bFast,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Speak )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR bszText,
            /* [in] */ BSTR bszUrl,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *MoveTo )( 
            IDaSvrCharacter * This,
            /* [in] */ short x,
            /* [in] */ short y,
            /* [in] */ long lSpeed,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GestureAt )( 
            IDaSvrCharacter * This,
            /* [in] */ short x,
            /* [in] */ short y,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetMoveCause )( 
            IDaSvrCharacter * This,
            /* [out] */ long *pdwCause);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVisibilityCause )( 
            IDaSvrCharacter * This,
            /* [out] */ long *pdwCause);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *HasOtherClients )( 
            IDaSvrCharacter * This,
            /* [out] */ long *plNumOtherClients);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetSoundEffectsOn )( 
            IDaSvrCharacter * This,
            /* [in] */ long bOn);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSoundEffectsOn )( 
            IDaSvrCharacter * This,
            /* [out] */ long *pbOn);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetName )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR bszName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetDescription )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR bszDescription);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetExtraData )( 
            IDaSvrCharacter * This,
            /* [out] */ BSTR *pbszExtraData);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ShowPopupMenu )( 
            IDaSvrCharacter * This,
            /* [in] */ short x,
            /* [in] */ short y);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetAutoPopupMenu )( 
            IDaSvrCharacter * This,
            /* [in] */ long bAutoPopupMenu);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetAutoPopupMenu )( 
            IDaSvrCharacter * This,
            /* [out] */ long *pbAutoPopupMenu);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetHelpFileName )( 
            IDaSvrCharacter * This,
            /* [out] */ BSTR *pbszName);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetHelpFileName )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR bszName);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetHelpModeOn )( 
            IDaSvrCharacter * This,
            /* [in] */ long bHelpModeOn);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetHelpModeOn )( 
            IDaSvrCharacter * This,
            /* [out] */ long *pbHelpModeOn);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetHelpContextID )( 
            IDaSvrCharacter * This,
            /* [in] */ long ulID);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetHelpContextID )( 
            IDaSvrCharacter * This,
            /* [out] */ long *pulID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetActive )( 
            IDaSvrCharacter * This,
            /* [out] */ short *psState);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Listen )( 
            IDaSvrCharacter * This,
            /* [in] */ long bListen);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetLanguageID )( 
            IDaSvrCharacter * This,
            /* [in] */ long langid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetLanguageID )( 
            IDaSvrCharacter * This,
            /* [out] */ long *plangid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetTTSModeID )( 
            IDaSvrCharacter * This,
            /* [out] */ BSTR *pbszModeID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetTTSModeID )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR bszModeID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSRModeID )( 
            IDaSvrCharacter * This,
            /* [out] */ BSTR *pbszModeID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetSRModeID )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR bszModeID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetGUID )( 
            IDaSvrCharacter * This,
            /* [out] */ BSTR *pbszID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetOriginalSize )( 
            IDaSvrCharacter * This,
            /* [out] */ long *plWidth,
            /* [out] */ long *plHeight);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Think )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR bszText,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVersion )( 
            IDaSvrCharacter * This,
            /* [out] */ short *psMajor,
            /* [out] */ short *psMinor);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetAnimationNames )( 
            IDaSvrCharacter * This,
            /* [out] */ IUnknown **punkEnum);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSRStatus )( 
            IDaSvrCharacter * This,
            /* [out] */ long *plStatus);
        
        END_INTERFACE
    } IDaSvrCharacterVtbl;

    interface IDaSvrCharacter
    {
        CONST_VTBL struct IDaSvrCharacterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrCharacter_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrCharacter_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrCharacter_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrCharacter_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrCharacter_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrCharacter_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrCharacter_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrCharacter_GetVisible(This,pbVisible)	\
    ( (This)->lpVtbl -> GetVisible(This,pbVisible) ) 

#define IDaSvrCharacter_SetPosition(This,lLeft,lTop)	\
    ( (This)->lpVtbl -> SetPosition(This,lLeft,lTop) ) 

#define IDaSvrCharacter_GetPosition(This,plLeft,plTop)	\
    ( (This)->lpVtbl -> GetPosition(This,plLeft,plTop) ) 

#define IDaSvrCharacter_SetSize(This,lWidth,lHeight)	\
    ( (This)->lpVtbl -> SetSize(This,lWidth,lHeight) ) 

#define IDaSvrCharacter_GetSize(This,plWidth,plHeight)	\
    ( (This)->lpVtbl -> GetSize(This,plWidth,plHeight) ) 

#define IDaSvrCharacter_GetName(This,pbszName)	\
    ( (This)->lpVtbl -> GetName(This,pbszName) ) 

#define IDaSvrCharacter_GetDescription(This,pbszDescription)	\
    ( (This)->lpVtbl -> GetDescription(This,pbszDescription) ) 

#define IDaSvrCharacter_GetTTSSpeed(This,pdwSpeed)	\
    ( (This)->lpVtbl -> GetTTSSpeed(This,pdwSpeed) ) 

#define IDaSvrCharacter_GetTTSPitch(This,pwPitch)	\
    ( (This)->lpVtbl -> GetTTSPitch(This,pwPitch) ) 

#define IDaSvrCharacter_Activate(This,sState)	\
    ( (This)->lpVtbl -> Activate(This,sState) ) 

#define IDaSvrCharacter_SetIdleOn(This,bOn)	\
    ( (This)->lpVtbl -> SetIdleOn(This,bOn) ) 

#define IDaSvrCharacter_GetIdleOn(This,pbOn)	\
    ( (This)->lpVtbl -> GetIdleOn(This,pbOn) ) 

#define IDaSvrCharacter_Prepare(This,dwType,bszName,bQueue,pdwReqID)	\
    ( (This)->lpVtbl -> Prepare(This,dwType,bszName,bQueue,pdwReqID) ) 

#define IDaSvrCharacter_Play(This,bszAnimation,pdwReqID)	\
    ( (This)->lpVtbl -> Play(This,bszAnimation,pdwReqID) ) 

#define IDaSvrCharacter_Stop(This,dwReqID)	\
    ( (This)->lpVtbl -> Stop(This,dwReqID) ) 

#define IDaSvrCharacter_StopAll(This,lTypes)	\
    ( (This)->lpVtbl -> StopAll(This,lTypes) ) 

#define IDaSvrCharacter_Wait(This,dwReqID,pdwReqID)	\
    ( (This)->lpVtbl -> Wait(This,dwReqID,pdwReqID) ) 

#define IDaSvrCharacter_Interrupt(This,dwReqID,pdwReqID)	\
    ( (This)->lpVtbl -> Interrupt(This,dwReqID,pdwReqID) ) 

#define IDaSvrCharacter_Show(This,bFast,pdwReqID)	\
    ( (This)->lpVtbl -> Show(This,bFast,pdwReqID) ) 

#define IDaSvrCharacter_Hide(This,bFast,pdwReqID)	\
    ( (This)->lpVtbl -> Hide(This,bFast,pdwReqID) ) 

#define IDaSvrCharacter_Speak(This,bszText,bszUrl,pdwReqID)	\
    ( (This)->lpVtbl -> Speak(This,bszText,bszUrl,pdwReqID) ) 

#define IDaSvrCharacter_MoveTo(This,x,y,lSpeed,pdwReqID)	\
    ( (This)->lpVtbl -> MoveTo(This,x,y,lSpeed,pdwReqID) ) 

#define IDaSvrCharacter_GestureAt(This,x,y,pdwReqID)	\
    ( (This)->lpVtbl -> GestureAt(This,x,y,pdwReqID) ) 

#define IDaSvrCharacter_GetMoveCause(This,pdwCause)	\
    ( (This)->lpVtbl -> GetMoveCause(This,pdwCause) ) 

#define IDaSvrCharacter_GetVisibilityCause(This,pdwCause)	\
    ( (This)->lpVtbl -> GetVisibilityCause(This,pdwCause) ) 

#define IDaSvrCharacter_HasOtherClients(This,plNumOtherClients)	\
    ( (This)->lpVtbl -> HasOtherClients(This,plNumOtherClients) ) 

#define IDaSvrCharacter_SetSoundEffectsOn(This,bOn)	\
    ( (This)->lpVtbl -> SetSoundEffectsOn(This,bOn) ) 

#define IDaSvrCharacter_GetSoundEffectsOn(This,pbOn)	\
    ( (This)->lpVtbl -> GetSoundEffectsOn(This,pbOn) ) 

#define IDaSvrCharacter_SetName(This,bszName)	\
    ( (This)->lpVtbl -> SetName(This,bszName) ) 

#define IDaSvrCharacter_SetDescription(This,bszDescription)	\
    ( (This)->lpVtbl -> SetDescription(This,bszDescription) ) 

#define IDaSvrCharacter_GetExtraData(This,pbszExtraData)	\
    ( (This)->lpVtbl -> GetExtraData(This,pbszExtraData) ) 

#define IDaSvrCharacter_ShowPopupMenu(This,x,y)	\
    ( (This)->lpVtbl -> ShowPopupMenu(This,x,y) ) 

#define IDaSvrCharacter_SetAutoPopupMenu(This,bAutoPopupMenu)	\
    ( (This)->lpVtbl -> SetAutoPopupMenu(This,bAutoPopupMenu) ) 

#define IDaSvrCharacter_GetAutoPopupMenu(This,pbAutoPopupMenu)	\
    ( (This)->lpVtbl -> GetAutoPopupMenu(This,pbAutoPopupMenu) ) 

#define IDaSvrCharacter_GetHelpFileName(This,pbszName)	\
    ( (This)->lpVtbl -> GetHelpFileName(This,pbszName) ) 

#define IDaSvrCharacter_SetHelpFileName(This,bszName)	\
    ( (This)->lpVtbl -> SetHelpFileName(This,bszName) ) 

#define IDaSvrCharacter_SetHelpModeOn(This,bHelpModeOn)	\
    ( (This)->lpVtbl -> SetHelpModeOn(This,bHelpModeOn) ) 

#define IDaSvrCharacter_GetHelpModeOn(This,pbHelpModeOn)	\
    ( (This)->lpVtbl -> GetHelpModeOn(This,pbHelpModeOn) ) 

#define IDaSvrCharacter_SetHelpContextID(This,ulID)	\
    ( (This)->lpVtbl -> SetHelpContextID(This,ulID) ) 

#define IDaSvrCharacter_GetHelpContextID(This,pulID)	\
    ( (This)->lpVtbl -> GetHelpContextID(This,pulID) ) 

#define IDaSvrCharacter_GetActive(This,psState)	\
    ( (This)->lpVtbl -> GetActive(This,psState) ) 

#define IDaSvrCharacter_Listen(This,bListen)	\
    ( (This)->lpVtbl -> Listen(This,bListen) ) 

#define IDaSvrCharacter_SetLanguageID(This,langid)	\
    ( (This)->lpVtbl -> SetLanguageID(This,langid) ) 

#define IDaSvrCharacter_GetLanguageID(This,plangid)	\
    ( (This)->lpVtbl -> GetLanguageID(This,plangid) ) 

#define IDaSvrCharacter_GetTTSModeID(This,pbszModeID)	\
    ( (This)->lpVtbl -> GetTTSModeID(This,pbszModeID) ) 

#define IDaSvrCharacter_SetTTSModeID(This,bszModeID)	\
    ( (This)->lpVtbl -> SetTTSModeID(This,bszModeID) ) 

#define IDaSvrCharacter_GetSRModeID(This,pbszModeID)	\
    ( (This)->lpVtbl -> GetSRModeID(This,pbszModeID) ) 

#define IDaSvrCharacter_SetSRModeID(This,bszModeID)	\
    ( (This)->lpVtbl -> SetSRModeID(This,bszModeID) ) 

#define IDaSvrCharacter_GetGUID(This,pbszID)	\
    ( (This)->lpVtbl -> GetGUID(This,pbszID) ) 

#define IDaSvrCharacter_GetOriginalSize(This,plWidth,plHeight)	\
    ( (This)->lpVtbl -> GetOriginalSize(This,plWidth,plHeight) ) 

#define IDaSvrCharacter_Think(This,bszText,pdwReqID)	\
    ( (This)->lpVtbl -> Think(This,bszText,pdwReqID) ) 

#define IDaSvrCharacter_GetVersion(This,psMajor,psMinor)	\
    ( (This)->lpVtbl -> GetVersion(This,psMajor,psMinor) ) 

#define IDaSvrCharacter_GetAnimationNames(This,punkEnum)	\
    ( (This)->lpVtbl -> GetAnimationNames(This,punkEnum) ) 

#define IDaSvrCharacter_GetSRStatus(This,plStatus)	\
    ( (This)->lpVtbl -> GetSRStatus(This,plStatus) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrCharacter_INTERFACE_DEFINED__ */


#ifndef __IDaSvrCommandWindow_INTERFACE_DEFINED__
#define __IDaSvrCommandWindow_INTERFACE_DEFINED__

/* interface IDaSvrCommandWindow */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrCommandWindow;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E507-A208-11DE-ABF2-002421116FB2")
    IDaSvrCommandWindow : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetVisible( 
            /* [in] */ long bVisible) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVisible( 
            /* [out] */ long *pbVisible) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetPosition( 
            /* [out] */ long *plLeft,
            /* [out] */ long *plTop) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSize( 
            /* [out] */ long *plWidth,
            /* [out] */ long *plHeight) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrCommandWindowVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrCommandWindow * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrCommandWindow * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrCommandWindow * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrCommandWindow * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrCommandWindow * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrCommandWindow * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrCommandWindow * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetVisible )( 
            IDaSvrCommandWindow * This,
            /* [in] */ long bVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrCommandWindow * This,
            /* [out] */ long *pbVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetPosition )( 
            IDaSvrCommandWindow * This,
            /* [out] */ long *plLeft,
            /* [out] */ long *plTop);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSize )( 
            IDaSvrCommandWindow * This,
            /* [out] */ long *plWidth,
            /* [out] */ long *plHeight);
        
        END_INTERFACE
    } IDaSvrCommandWindowVtbl;

    interface IDaSvrCommandWindow
    {
        CONST_VTBL struct IDaSvrCommandWindowVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrCommandWindow_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrCommandWindow_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrCommandWindow_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrCommandWindow_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrCommandWindow_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrCommandWindow_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrCommandWindow_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrCommandWindow_SetVisible(This,bVisible)	\
    ( (This)->lpVtbl -> SetVisible(This,bVisible) ) 

#define IDaSvrCommandWindow_GetVisible(This,pbVisible)	\
    ( (This)->lpVtbl -> GetVisible(This,pbVisible) ) 

#define IDaSvrCommandWindow_GetPosition(This,plLeft,plTop)	\
    ( (This)->lpVtbl -> GetPosition(This,plLeft,plTop) ) 

#define IDaSvrCommandWindow_GetSize(This,plWidth,plHeight)	\
    ( (This)->lpVtbl -> GetSize(This,plWidth,plHeight) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrCommandWindow_INTERFACE_DEFINED__ */


#ifndef __IDaSvrNotifySink15_INTERFACE_DEFINED__
#define __IDaSvrNotifySink15_INTERFACE_DEFINED__

/* interface IDaSvrNotifySink15 */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrNotifySink15;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E503-A208-11DE-ABF2-002421116FB2")
    IDaSvrNotifySink15 : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Command( 
            /* [in] */ long dwCommandID,
            /* [in] */ IUnknown *punkUserInput) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ActivateInputState( 
            /* [in] */ long dwCharID,
            /* [in] */ long bActivated) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE Restart( void) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE Shutdown( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE VisibleState( 
            /* [in] */ long dwCharID,
            /* [in] */ long bVisible,
            /* [in] */ long dwCause) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Click( 
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DblClick( 
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DragStart( 
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DragComplete( 
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RequestStart( 
            /* [in] */ long dwRequestID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RequestComplete( 
            /* [in] */ long dwRequestID,
            /* [in] */ long hrStatus) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE BookMark( 
            /* [in] */ long dwBookMarkID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Idle( 
            /* [in] */ long dwCharID,
            /* [in] */ long bStart) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Move( 
            /* [in] */ long dwCharID,
            /* [in] */ long x,
            /* [in] */ long y,
            /* [in] */ long dwCause) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Size( 
            /* [in] */ long dwCharID,
            /* [in] */ long lWidth,
            /* [in] */ long lHeight) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE BalloonVisibleState( 
            /* [in] */ long dwCharID,
            /* [in] */ long bVisible) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrNotifySink15Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrNotifySink15 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrNotifySink15 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrNotifySink15 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Command )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwCommandID,
            /* [in] */ IUnknown *punkUserInput);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ActivateInputState )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwCharID,
            /* [in] */ long bActivated);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *Restart )( 
            IDaSvrNotifySink15 * This);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *Shutdown )( 
            IDaSvrNotifySink15 * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *VisibleState )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwCharID,
            /* [in] */ long bVisible,
            /* [in] */ long dwCause);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Click )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DblClick )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DragStart )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DragComplete )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RequestStart )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwRequestID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RequestComplete )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwRequestID,
            /* [in] */ long hrStatus);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *BookMark )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwBookMarkID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Idle )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwCharID,
            /* [in] */ long bStart);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Move )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwCharID,
            /* [in] */ long x,
            /* [in] */ long y,
            /* [in] */ long dwCause);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Size )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwCharID,
            /* [in] */ long lWidth,
            /* [in] */ long lHeight);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *BalloonVisibleState )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwCharID,
            /* [in] */ long bVisible);
        
        END_INTERFACE
    } IDaSvrNotifySink15Vtbl;

    interface IDaSvrNotifySink15
    {
        CONST_VTBL struct IDaSvrNotifySink15Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrNotifySink15_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrNotifySink15_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrNotifySink15_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrNotifySink15_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrNotifySink15_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrNotifySink15_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrNotifySink15_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrNotifySink15_Command(This,dwCommandID,punkUserInput)	\
    ( (This)->lpVtbl -> Command(This,dwCommandID,punkUserInput) ) 

#define IDaSvrNotifySink15_ActivateInputState(This,dwCharID,bActivated)	\
    ( (This)->lpVtbl -> ActivateInputState(This,dwCharID,bActivated) ) 

#define IDaSvrNotifySink15_Restart(This)	\
    ( (This)->lpVtbl -> Restart(This) ) 

#define IDaSvrNotifySink15_Shutdown(This)	\
    ( (This)->lpVtbl -> Shutdown(This) ) 

#define IDaSvrNotifySink15_VisibleState(This,dwCharID,bVisible,dwCause)	\
    ( (This)->lpVtbl -> VisibleState(This,dwCharID,bVisible,dwCause) ) 

#define IDaSvrNotifySink15_Click(This,dwCharID,fwKeys,x,y)	\
    ( (This)->lpVtbl -> Click(This,dwCharID,fwKeys,x,y) ) 

#define IDaSvrNotifySink15_DblClick(This,dwCharID,fwKeys,x,y)	\
    ( (This)->lpVtbl -> DblClick(This,dwCharID,fwKeys,x,y) ) 

#define IDaSvrNotifySink15_DragStart(This,dwCharID,fwKeys,x,y)	\
    ( (This)->lpVtbl -> DragStart(This,dwCharID,fwKeys,x,y) ) 

#define IDaSvrNotifySink15_DragComplete(This,dwCharID,fwKeys,x,y)	\
    ( (This)->lpVtbl -> DragComplete(This,dwCharID,fwKeys,x,y) ) 

#define IDaSvrNotifySink15_RequestStart(This,dwRequestID)	\
    ( (This)->lpVtbl -> RequestStart(This,dwRequestID) ) 

#define IDaSvrNotifySink15_RequestComplete(This,dwRequestID,hrStatus)	\
    ( (This)->lpVtbl -> RequestComplete(This,dwRequestID,hrStatus) ) 

#define IDaSvrNotifySink15_BookMark(This,dwBookMarkID)	\
    ( (This)->lpVtbl -> BookMark(This,dwBookMarkID) ) 

#define IDaSvrNotifySink15_Idle(This,dwCharID,bStart)	\
    ( (This)->lpVtbl -> Idle(This,dwCharID,bStart) ) 

#define IDaSvrNotifySink15_Move(This,dwCharID,x,y,dwCause)	\
    ( (This)->lpVtbl -> Move(This,dwCharID,x,y,dwCause) ) 

#define IDaSvrNotifySink15_Size(This,dwCharID,lWidth,lHeight)	\
    ( (This)->lpVtbl -> Size(This,dwCharID,lWidth,lHeight) ) 

#define IDaSvrNotifySink15_BalloonVisibleState(This,dwCharID,bVisible)	\
    ( (This)->lpVtbl -> BalloonVisibleState(This,dwCharID,bVisible) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrNotifySink15_INTERFACE_DEFINED__ */


#ifndef __IDaSvrNotifySink_INTERFACE_DEFINED__
#define __IDaSvrNotifySink_INTERFACE_DEFINED__

/* interface IDaSvrNotifySink */
/* [object][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrNotifySink;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E516-A208-11DE-ABF2-002421116FB2")
    IDaSvrNotifySink : public IDaSvrNotifySink15
    {
    public:
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE HelpComplete( 
            /* [in] */ long dwCharID,
            /* [in] */ long dwCommandID,
            /* [in] */ long dwCause) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ListeningState( 
            /* [in] */ long dwCharID,
            /* [in] */ long bListening,
            /* [in] */ long dwCause) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DefaultCharacterChange( 
            /* [in] */ BSTR bszGUID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AgentPropertyChange( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ActiveClientChange( 
            /* [in] */ long dwCharID,
            /* [in] */ long lStatus) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrNotifySinkVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrNotifySink * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrNotifySink * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrNotifySink * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrNotifySink * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrNotifySink * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrNotifySink * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrNotifySink * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Command )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCommandID,
            /* [in] */ IUnknown *punkUserInput);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ActivateInputState )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ long bActivated);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *Restart )( 
            IDaSvrNotifySink * This);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *Shutdown )( 
            IDaSvrNotifySink * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *VisibleState )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ long bVisible,
            /* [in] */ long dwCause);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Click )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DblClick )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DragStart )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DragComplete )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RequestStart )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwRequestID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RequestComplete )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwRequestID,
            /* [in] */ long hrStatus);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *BookMark )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwBookMarkID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Idle )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ long bStart);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Move )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ long x,
            /* [in] */ long y,
            /* [in] */ long dwCause);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Size )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ long lWidth,
            /* [in] */ long lHeight);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *BalloonVisibleState )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ long bVisible);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *HelpComplete )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ long dwCommandID,
            /* [in] */ long dwCause);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ListeningState )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ long bListening,
            /* [in] */ long dwCause);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DefaultCharacterChange )( 
            IDaSvrNotifySink * This,
            /* [in] */ BSTR bszGUID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AgentPropertyChange )( 
            IDaSvrNotifySink * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ActiveClientChange )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ long lStatus);
        
        END_INTERFACE
    } IDaSvrNotifySinkVtbl;

    interface IDaSvrNotifySink
    {
        CONST_VTBL struct IDaSvrNotifySinkVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrNotifySink_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrNotifySink_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrNotifySink_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrNotifySink_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrNotifySink_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrNotifySink_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrNotifySink_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrNotifySink_Command(This,dwCommandID,punkUserInput)	\
    ( (This)->lpVtbl -> Command(This,dwCommandID,punkUserInput) ) 

#define IDaSvrNotifySink_ActivateInputState(This,dwCharID,bActivated)	\
    ( (This)->lpVtbl -> ActivateInputState(This,dwCharID,bActivated) ) 

#define IDaSvrNotifySink_Restart(This)	\
    ( (This)->lpVtbl -> Restart(This) ) 

#define IDaSvrNotifySink_Shutdown(This)	\
    ( (This)->lpVtbl -> Shutdown(This) ) 

#define IDaSvrNotifySink_VisibleState(This,dwCharID,bVisible,dwCause)	\
    ( (This)->lpVtbl -> VisibleState(This,dwCharID,bVisible,dwCause) ) 

#define IDaSvrNotifySink_Click(This,dwCharID,fwKeys,x,y)	\
    ( (This)->lpVtbl -> Click(This,dwCharID,fwKeys,x,y) ) 

#define IDaSvrNotifySink_DblClick(This,dwCharID,fwKeys,x,y)	\
    ( (This)->lpVtbl -> DblClick(This,dwCharID,fwKeys,x,y) ) 

#define IDaSvrNotifySink_DragStart(This,dwCharID,fwKeys,x,y)	\
    ( (This)->lpVtbl -> DragStart(This,dwCharID,fwKeys,x,y) ) 

#define IDaSvrNotifySink_DragComplete(This,dwCharID,fwKeys,x,y)	\
    ( (This)->lpVtbl -> DragComplete(This,dwCharID,fwKeys,x,y) ) 

#define IDaSvrNotifySink_RequestStart(This,dwRequestID)	\
    ( (This)->lpVtbl -> RequestStart(This,dwRequestID) ) 

#define IDaSvrNotifySink_RequestComplete(This,dwRequestID,hrStatus)	\
    ( (This)->lpVtbl -> RequestComplete(This,dwRequestID,hrStatus) ) 

#define IDaSvrNotifySink_BookMark(This,dwBookMarkID)	\
    ( (This)->lpVtbl -> BookMark(This,dwBookMarkID) ) 

#define IDaSvrNotifySink_Idle(This,dwCharID,bStart)	\
    ( (This)->lpVtbl -> Idle(This,dwCharID,bStart) ) 

#define IDaSvrNotifySink_Move(This,dwCharID,x,y,dwCause)	\
    ( (This)->lpVtbl -> Move(This,dwCharID,x,y,dwCause) ) 

#define IDaSvrNotifySink_Size(This,dwCharID,lWidth,lHeight)	\
    ( (This)->lpVtbl -> Size(This,dwCharID,lWidth,lHeight) ) 

#define IDaSvrNotifySink_BalloonVisibleState(This,dwCharID,bVisible)	\
    ( (This)->lpVtbl -> BalloonVisibleState(This,dwCharID,bVisible) ) 


#define IDaSvrNotifySink_HelpComplete(This,dwCharID,dwCommandID,dwCause)	\
    ( (This)->lpVtbl -> HelpComplete(This,dwCharID,dwCommandID,dwCause) ) 

#define IDaSvrNotifySink_ListeningState(This,dwCharID,bListening,dwCause)	\
    ( (This)->lpVtbl -> ListeningState(This,dwCharID,bListening,dwCause) ) 

#define IDaSvrNotifySink_DefaultCharacterChange(This,bszGUID)	\
    ( (This)->lpVtbl -> DefaultCharacterChange(This,bszGUID) ) 

#define IDaSvrNotifySink_AgentPropertyChange(This)	\
    ( (This)->lpVtbl -> AgentPropertyChange(This) ) 

#define IDaSvrNotifySink_ActiveClientChange(This,dwCharID,lStatus)	\
    ( (This)->lpVtbl -> ActiveClientChange(This,dwCharID,lStatus) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrNotifySink_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_DaServer;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E500-A208-11DE-ABF2-002421116FB2")
DaServer;
#endif

EXTERN_C const CLSID CLSID_DaCharacter;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E50D-A208-11DE-ABF2-002421116FB2")
DaCharacter;
#endif

EXTERN_C const CLSID CLSID_DaCommand;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E50E-A208-11DE-ABF2-002421116FB2")
DaCommand;
#endif

EXTERN_C const CLSID CLSID_DaCommands;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E50F-A208-11DE-ABF2-002421116FB2")
DaCommands;
#endif

EXTERN_C const CLSID CLSID_DaCommandWindow;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E510-A208-11DE-ABF2-002421116FB2")
DaCommandWindow;
#endif

EXTERN_C const CLSID CLSID_DaUserInput;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E511-A208-11DE-ABF2-002421116FB2")
DaUserInput;
#endif

EXTERN_C const CLSID CLSID_DaBalloon;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E512-A208-11DE-ABF2-002421116FB2")
DaBalloon;
#endif

EXTERN_C const CLSID CLSID_DaPropertySheet;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E513-A208-11DE-ABF2-002421116FB2")
DaPropertySheet;
#endif

EXTERN_C const CLSID CLSID_DaAudioOutputProperties;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E514-A208-11DE-ABF2-002421116FB2")
DaAudioOutputProperties;
#endif

EXTERN_C const CLSID CLSID_DaSpeechInputProperties;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E515-A208-11DE-ABF2-002421116FB2")
DaSpeechInputProperties;
#endif

EXTERN_C const CLSID CLSID_DaAnimationNames;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E517-A208-11DE-ABF2-002421116FB2")
DaAnimationNames;
#endif
#endif /* __DoubleAgentSvr_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


