

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Feb 26 13:37:40 2010
 */
/* Compiler settings for .\Control\DaControl.odl:
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


#ifndef __DaControlOdl_h__
#define __DaControlOdl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDaCtlCharacters_FWD_DEFINED__
#define __IDaCtlCharacters_FWD_DEFINED__
typedef interface IDaCtlCharacters IDaCtlCharacters;
#endif 	/* __IDaCtlCharacters_FWD_DEFINED__ */


#ifndef __IDaCtlCharacter_FWD_DEFINED__
#define __IDaCtlCharacter_FWD_DEFINED__
typedef interface IDaCtlCharacter IDaCtlCharacter;
#endif 	/* __IDaCtlCharacter_FWD_DEFINED__ */


#ifndef __IDaCtlBalloon_FWD_DEFINED__
#define __IDaCtlBalloon_FWD_DEFINED__
typedef interface IDaCtlBalloon IDaCtlBalloon;
#endif 	/* __IDaCtlBalloon_FWD_DEFINED__ */


#ifndef __IDaCtlCommands_FWD_DEFINED__
#define __IDaCtlCommands_FWD_DEFINED__
typedef interface IDaCtlCommands IDaCtlCommands;
#endif 	/* __IDaCtlCommands_FWD_DEFINED__ */


#ifndef __IDaCtlCommand_FWD_DEFINED__
#define __IDaCtlCommand_FWD_DEFINED__
typedef interface IDaCtlCommand IDaCtlCommand;
#endif 	/* __IDaCtlCommand_FWD_DEFINED__ */


#ifndef __IDaCtlRequest_FWD_DEFINED__
#define __IDaCtlRequest_FWD_DEFINED__
typedef interface IDaCtlRequest IDaCtlRequest;
#endif 	/* __IDaCtlRequest_FWD_DEFINED__ */


#ifndef __IDaCtlAudioObject_FWD_DEFINED__
#define __IDaCtlAudioObject_FWD_DEFINED__
typedef interface IDaCtlAudioObject IDaCtlAudioObject;
#endif 	/* __IDaCtlAudioObject_FWD_DEFINED__ */


#ifndef __IDaCtlSpeechInput_FWD_DEFINED__
#define __IDaCtlSpeechInput_FWD_DEFINED__
typedef interface IDaCtlSpeechInput IDaCtlSpeechInput;
#endif 	/* __IDaCtlSpeechInput_FWD_DEFINED__ */


#ifndef __IDaCtlPropertySheet_FWD_DEFINED__
#define __IDaCtlPropertySheet_FWD_DEFINED__
typedef interface IDaCtlPropertySheet IDaCtlPropertySheet;
#endif 	/* __IDaCtlPropertySheet_FWD_DEFINED__ */


#ifndef __IDaCtlUserInput_FWD_DEFINED__
#define __IDaCtlUserInput_FWD_DEFINED__
typedef interface IDaCtlUserInput IDaCtlUserInput;
#endif 	/* __IDaCtlUserInput_FWD_DEFINED__ */


#ifndef __IDaCtlCommandsWindow_FWD_DEFINED__
#define __IDaCtlCommandsWindow_FWD_DEFINED__
typedef interface IDaCtlCommandsWindow IDaCtlCommandsWindow;
#endif 	/* __IDaCtlCommandsWindow_FWD_DEFINED__ */


#ifndef __IDaCtlAnimationNames_FWD_DEFINED__
#define __IDaCtlAnimationNames_FWD_DEFINED__
typedef interface IDaCtlAnimationNames IDaCtlAnimationNames;
#endif 	/* __IDaCtlAnimationNames_FWD_DEFINED__ */


#ifndef __IDaControl_FWD_DEFINED__
#define __IDaControl_FWD_DEFINED__
typedef interface IDaControl IDaControl;
#endif 	/* __IDaControl_FWD_DEFINED__ */


#ifndef ___DaCtlEvents_FWD_DEFINED__
#define ___DaCtlEvents_FWD_DEFINED__
typedef interface _DaCtlEvents _DaCtlEvents;
#endif 	/* ___DaCtlEvents_FWD_DEFINED__ */


#ifndef __DaControl_FWD_DEFINED__
#define __DaControl_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaControl DaControl;
#else
typedef struct DaControl DaControl;
#endif /* __cplusplus */

#endif 	/* __DaControl_FWD_DEFINED__ */


#ifndef __DaCtlCharacters_FWD_DEFINED__
#define __DaCtlCharacters_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlCharacters DaCtlCharacters;
#else
typedef struct DaCtlCharacters DaCtlCharacters;
#endif /* __cplusplus */

#endif 	/* __DaCtlCharacters_FWD_DEFINED__ */


#ifndef __DaCtlCharacter_FWD_DEFINED__
#define __DaCtlCharacter_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlCharacter DaCtlCharacter;
#else
typedef struct DaCtlCharacter DaCtlCharacter;
#endif /* __cplusplus */

#endif 	/* __DaCtlCharacter_FWD_DEFINED__ */


#ifndef __DaCtlBalloon_FWD_DEFINED__
#define __DaCtlBalloon_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlBalloon DaCtlBalloon;
#else
typedef struct DaCtlBalloon DaCtlBalloon;
#endif /* __cplusplus */

#endif 	/* __DaCtlBalloon_FWD_DEFINED__ */


#ifndef __DaCtlCommands_FWD_DEFINED__
#define __DaCtlCommands_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlCommands DaCtlCommands;
#else
typedef struct DaCtlCommands DaCtlCommands;
#endif /* __cplusplus */

#endif 	/* __DaCtlCommands_FWD_DEFINED__ */


#ifndef __DaCtlCommand_FWD_DEFINED__
#define __DaCtlCommand_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlCommand DaCtlCommand;
#else
typedef struct DaCtlCommand DaCtlCommand;
#endif /* __cplusplus */

#endif 	/* __DaCtlCommand_FWD_DEFINED__ */


#ifndef __DaCtlRequest_FWD_DEFINED__
#define __DaCtlRequest_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlRequest DaCtlRequest;
#else
typedef struct DaCtlRequest DaCtlRequest;
#endif /* __cplusplus */

#endif 	/* __DaCtlRequest_FWD_DEFINED__ */


#ifndef __DaCtlAudioOutput_FWD_DEFINED__
#define __DaCtlAudioOutput_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlAudioOutput DaCtlAudioOutput;
#else
typedef struct DaCtlAudioOutput DaCtlAudioOutput;
#endif /* __cplusplus */

#endif 	/* __DaCtlAudioOutput_FWD_DEFINED__ */


#ifndef __DaCtlSpeechInput_FWD_DEFINED__
#define __DaCtlSpeechInput_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlSpeechInput DaCtlSpeechInput;
#else
typedef struct DaCtlSpeechInput DaCtlSpeechInput;
#endif /* __cplusplus */

#endif 	/* __DaCtlSpeechInput_FWD_DEFINED__ */


#ifndef __DaCtlPropertySheet_FWD_DEFINED__
#define __DaCtlPropertySheet_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlPropertySheet DaCtlPropertySheet;
#else
typedef struct DaCtlPropertySheet DaCtlPropertySheet;
#endif /* __cplusplus */

#endif 	/* __DaCtlPropertySheet_FWD_DEFINED__ */


#ifndef __DaCtlUserInput_FWD_DEFINED__
#define __DaCtlUserInput_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlUserInput DaCtlUserInput;
#else
typedef struct DaCtlUserInput DaCtlUserInput;
#endif /* __cplusplus */

#endif 	/* __DaCtlUserInput_FWD_DEFINED__ */


#ifndef __DaCtlCommandsWindow_FWD_DEFINED__
#define __DaCtlCommandsWindow_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlCommandsWindow DaCtlCommandsWindow;
#else
typedef struct DaCtlCommandsWindow DaCtlCommandsWindow;
#endif /* __cplusplus */

#endif 	/* __DaCtlCommandsWindow_FWD_DEFINED__ */


#ifndef __DaCtlAnimationNames_FWD_DEFINED__
#define __DaCtlAnimationNames_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlAnimationNames DaCtlAnimationNames;
#else
typedef struct DaCtlAnimationNames DaCtlAnimationNames;
#endif /* __cplusplus */

#endif 	/* __DaCtlAnimationNames_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __DoubleAgentCtl_LIBRARY_DEFINED__
#define __DoubleAgentCtl_LIBRARY_DEFINED__

/* library DoubleAgentCtl */
/* [control][helpstring][version][uuid] */ 















#define	DISPID_IAgentCtlCharacters_Load	( 1 )

#define	DISPID_IAgentCtlCharacters_Unload	( 2 )

#define	DISPID_IAgentCtlCharacters_CharacterID	( 3 )

#define	DISPID_IAgentCtlCharacter_Visible	( 2 )

#define	DISPID_IAgentCtlCharacter_Left	( 3 )

#define	DISPID_IAgentCtlCharacter_Top	( 4 )

#define	DISPID_IAgentCtlCharacter_Height	( 5 )

#define	DISPID_IAgentCtlCharacter_Width	( 6 )

#define	DISPID_IAgentCtlCharacter_Speed	( 10 )

#define	DISPID_IAgentCtlCharacter_Pitch	( 11 )

#define	DISPID_IAgentCtlCharacter_Play	( 13 )

#define	DISPID_IAgentCtlCharacter_Stop	( 14 )

#define	DISPID_IAgentCtlCharacter_Speak	( 15 )

#define	DISPID_IAgentCtlCharacter_GestureAt	( 17 )

#define	DISPID_IAgentCtlCharacter_MoveTo	( 18 )

#define	DISPID_IAgentCtlCharacter_Hide	( 19 )

#define	DISPID_IAgentCtlCharacter_Show	( 20 )

#define	DISPID_IAgentCtlCharacter_Interrupt	( 21 )

#define	DISPID_IAgentCtlCharacter_Wait	( 22 )

#define	DISPID_IAgentCtlCharacter_Balloon	( 23 )

#define	DISPID_IAgentCtlCharacter_Name	( 24 )

#define	DISPID_IAgentCtlCharacter_Commands	( 25 )

#define	DISPID_IAgentCtlCharacter_Activate	( 26 )

#define	DISPID_IAgentCtlCharacter_Get	( 27 )

#define	DISPID_IAgentCtlCharacter_Description	( 28 )

#define	DISPID_IAgentCtlCharacter_IdleOn	( 29 )

#define	DISPID_IAgentCtlCharacter_StopAll	( 31 )

#define	DISPID_IAgentCtlCharacter_MoveCause	( 32 )

#define	DISPID_IAgentCtlCharacter_VisibilityCause	( 33 )

#define	DISPID_IAgentCtlCharacter_HasOtherClients	( 34 )

#define	DISPID_IAgentCtlCharacter_SoundEffectsOn	( 35 )

#define	DISPID_IAgentCtlCharacter_ExtraData	( 36 )

#define	DISPID_IAgentCtlCharacterEx_ShowPopupMenu	( 37 )

#define	DISPID_IAgentCtlCharacterEx_AutoPopupMenu	( 38 )

#define	DISPID_IAgentCtlCharacterEx_HelpModeOn	( 39 )

#define	DISPID_IAgentCtlCharacterEx_HelpContextID	( 40 )

#define	DISPID_IAgentCtlCharacterEx_Active	( 41 )

#define	DISPID_IAgentCtlCharacterEx_Listen	( 42 )

#define	DISPID_IAgentCtlCharacterEx_LanguageID	( 43 )

#define	DISPID_IAgentCtlCharacterEx_SRModeID	( 46 )

#define	DISPID_IAgentCtlCharacterEx_TTSModeID	( 47 )

#define	DISPID_IAgentCtlCharacterEx_HelpFile	( 48 )

#define	DISPID_IAgentCtlCharacterEx_GUID	( 49 )

#define	DISPID_IAgentCtlCharacterEx_OriginalHeight	( 50 )

#define	DISPID_IAgentCtlCharacterEx_OriginalWidth	( 51 )

#define	DISPID_IAgentCtlCharacterEx_Think	( 52 )

#define	DISPID_IAgentCtlCharacterEx_Version	( 53 )

#define	DISPID_IAgentCtlCharacterEx_AnimationNames	( 54 )

#define	DISPID_IAgentCtlCharacterEx_SRStatus	( 55 )

#define	DISPID_IAgentCtlBalloon_Enabled	( 1 )

#define	DISPID_IAgentCtlBalloon_NumberOfLines	( 2 )

#define	DISPID_IAgentCtlBalloon_CharsPerLine	( 3 )

#define	DISPID_IAgentCtlBalloon_FontName	( 4 )

#define	DISPID_IAgentCtlBalloon_FontSize	( 5 )

#define	DISPID_IAgentCtlBalloon_ForeColor	( 7 )

#define	DISPID_IAgentCtlBalloon_BackColor	( 8 )

#define	DISPID_IAgentCtlBalloon_BorderColor	( 9 )

#define	DISPID_IAgentCtlBalloon_FontBold	( 10 )

#define	DISPID_IAgentCtlBalloon_FontItalic	( 11 )

#define	DISPID_IAgentCtlBalloon_FontStrikethru	( 12 )

#define	DISPID_IAgentCtlBalloon_FontUnderline	( 13 )

#define	DISPID_IAgentCtlBalloon_Visible	( 14 )

#define	DISPID_IAgentCtlBalloon_FontCharSet	( 15 )

#define	DISPID_IAgentCtlBalloonEx_Style	( 16 )

#define	DISPID_IAgentCtlCommands_Count	( 2 )

#define	DISPID_IAgentCtlCommands_Caption	( 3 )

#define	DISPID_IAgentCtlCommands_Voice	( 4 )

#define	DISPID_IAgentCtlCommands_Visible	( 5 )

#define	DISPID_IAgentCtlCommands_Add	( 10 )

#define	DISPID_IAgentCtlCommands_Insert	( 11 )

#define	DISPID_IAgentCtlCommands_Remove	( 13 )

#define	DISPID_IAgentCtlCommands_RemoveAll	( 14 )

#define	DISPID_IAgentCtlCommands_Command	( 15 )

#define	DISPID_IAgentCtlCommandsEx_DefaultCommand	( 16 )

#define	DISPID_IAgentCtlCommandsEx_HelpContextID	( 17 )

#define	DISPID_IAgentCtlCommandsEx_FontName	( 21 )

#define	DISPID_IAgentCtlCommandsEx_FontSize	( 23 )

#define	DISPID_IAgentCtlCommandsEx_VoiceCaption	( 22 )

#define	DISPID_IAgentCtlCommandsEx_GlobalVoiceCommandsEnabled	( 24 )

#define	DISPID_IAgentCtlCommand_Voice	( 1 )

#define	DISPID_IAgentCtlCommand_Caption	( 2 )

#define	DISPID_IAgentCtlCommand_Confidence	( 3 )

#define	DISPID_IAgentCtlCommand_ConfidenceText	( 4 )

#define	DISPID_IAgentCtlCommand_Enabled	( 5 )

#define	DISPID_IAgentCtlCommand_Visible	( 6 )

#define	DISPID_IAgentCtlCommandEx_HelpContextID	( 7 )

#define	DISPID_IAgentCtlCommandEx_VoiceCaption	( 8 )

#define	RequestSuccess	( 0 )

#define	RequestFailed	( 1 )

#define	RequestPending	( 2 )

#define	RequestInterrupted	( 3 )

#define	RequestInProgress	( 4 )

#define	DISPID_IAgentCtlRequest_Status	( 1 )

#define	DISPID_IAgentCtlRequest_Description	( 2 )

#define	DISPID_IAgentCtlRequest_Number	( 3 )

#define	DISPID_IAgentCtlAudioObject_Enabled	( 1 )

#define	DISPID_IAgentCtlAudioObject_SoundEffects	( 2 )

#define	DISPID_IAgentCtlAudioObjectEx_Status	( 3 )

#define	DISPID_IAgentCtlSpeechInput_Enabled	( 1 )

#define	DISPID_IAgentCtlSpeechInput_Language	( 2 )

#define	DISPID_IAgentCtlSpeechInput_HotKey	( 3 )

#define	DISPID_IAgentCtlSpeechInput_Installed	( 4 )

#define	DISPID_IAgentCtlSpeechInput_Engine	( 5 )

#define	DISPID_IAgentCtlSpeechInput_ListeningTip	( 6 )

#define	DISPID_IAgentCtlPropertySheet_Left	( 1 )

#define	DISPID_IAgentCtlPropertySheet_Top	( 2 )

#define	DISPID_IAgentCtlPropertySheet_Height	( 3 )

#define	DISPID_IAgentCtlPropertySheet_Width	( 4 )

#define	DISPID_IAgentCtlPropertySheet_Page	( 5 )

#define	DISPID_IAgentCtlPropertySheet_Visible	( 6 )

#define	DISPID_IAgentCtlUserInput_Count	( 0x60020000 )

#define	DISPID_IAgentCtlUserInput_Name	( 0x60020001 )

#define	DISPID_IAgentCtlUserInput_CharacterID	( 0x60020002 )

#define	DISPID_IAgentCtlUserInput_Confidence	( 0x60020003 )

#define	DISPID_IAgentCtlUserInput_Voice	( 0x60020004 )

#define	DISPID_IAgentCtlUserInput_Alt1Name	( 0x60020005 )

#define	DISPID_IAgentCtlUserInput_Alt1Confidence	( 0x60020006 )

#define	DISPID_IAgentCtlUserInput_Alt1Voice	( 0x60020007 )

#define	DISPID_IAgentCtlUserInput_Alt2Name	( 0x60020008 )

#define	DISPID_IAgentCtlUserInput_Alt2Confidence	( 0x60020009 )

#define	DISPID_IAgentCtlUserInput_Alt2Voice	( 0x6002000a )

#define	DISPID_IAgentCtlCommandsWindow_Visible	( 5 )

#define	DISPID_IAgentCtlCommandsWindow_Left	( 6 )

#define	DISPID_IAgentCtlCommandsWindow_Top	( 7 )

#define	DISPID_IAgentCtlCommandsWindow_Height	( 8 )

#define	DISPID_IAgentCtlCommandsWindow_Width	( 9 )

#define	DISPID_IAgentCtl_Characters	( 3 )

#define	DISPID_IAgentCtl_AudioOutput	( 4 )

#define	DISPID_IAgentCtl_SpeechInput	( 5 )

#define	DISPID_IAgentCtl_PropertySheet	( 8 )

#define	DISPID_IAgentCtl_Connected	( 9 )

#define	DISPID_IAgentCtl_CommandsWindow	( 12 )

#define	DISPID_IAgentCtl_Suspended	( 14 )

#define	DISPID_IAgentCtlEx_ShowDefaultCharacterProperties	( 20 )

#define	DISPID_IAgentCtlEx_RaiseRequestErrors	( 21 )

#define	DISPID_AgentEvents_ActivateInput	( 1 )

#define	DISPID_AgentEvents_Click	( 2 )

#define	DISPID_AgentEvents_DeactivateInput	( 3 )

#define	DISPID_AgentEvents_DblClick	( 4 )

#define	DISPID_AgentEvents_DragStart	( 5 )

#define	DISPID_AgentEvents_DragComplete	( 6 )

#define	DISPID_AgentEvents_Hide	( 7 )

#define	DISPID_AgentEvents_RequestStart	( 9 )

#define	DISPID_AgentEvents_RequestComplete	( 11 )

#define	DISPID_AgentEvents_Shutdown	( 12 )

#define	DISPID_AgentEvents_Show	( 15 )

#define	DISPID_AgentEvents_Bookmark	( 16 )

#define	DISPID_AgentEvents_Command	( 17 )

#define	DISPID_AgentEvents_IdleStart	( 19 )

#define	DISPID_AgentEvents_IdleComplete	( 20 )

#define	DISPID_AgentEvents_Restart	( 21 )

#define	DISPID_AgentEvents_Move	( 22 )

#define	DISPID_AgentEvents_Size	( 23 )

#define	DISPID_AgentEvents_BalloonShow	( 24 )

#define	DISPID_AgentEvents_BalloonHide	( 25 )

#define	DISPID_AgentEvents_HelpComplete	( 26 )

#define	DISPID_AgentEvents_ListenStart	( 27 )

#define	DISPID_AgentEvents_ListenComplete	( 28 )

#define	DISPID_AgentEvents_DefaultCharacterChange	( 30 )

#define	DISPID_AgentEvents_AgentPropertyChange	( 31 )

#define	DISPID_AgentEvents_ActiveClientChange	( 32 )


EXTERN_C const IID LIBID_DoubleAgentCtl;

#ifndef __IDaCtlCharacters_INTERFACE_DEFINED__
#define __IDaCtlCharacters_INTERFACE_DEFINED__

/* interface IDaCtlCharacters */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaCtlCharacters;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E53C-A208-11DE-ABF2-002421116FB2")
    IDaCtlCharacters : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ BSTR CharacterID,
            /* [retval][out] */ IDaCtlCharacter **ppidItem) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Character( 
            /* [in] */ BSTR CharacterID,
            /* [retval][out] */ IDaCtlCharacter **ppidItem) = 0;
        
        virtual /* [restricted][propget][id] */ HRESULT STDMETHODCALLTYPE get_Enum( 
            /* [retval][out] */ IUnknown **ppunkEnum) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Unload( 
            /* [in] */ BSTR CharacterID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Load( 
            /* [in] */ BSTR CharacterID,
            /* [optional][in] */ VARIANT LoadKey,
            /* [retval][out] */ IDaCtlRequest **ppidRequest) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlCharactersVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlCharacters * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlCharacters * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlCharacters * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlCharacters * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlCharacters * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlCharacters * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlCharacters * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IDaCtlCharacters * This,
            /* [in] */ BSTR CharacterID,
            /* [retval][out] */ IDaCtlCharacter **ppidItem);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Character )( 
            IDaCtlCharacters * This,
            /* [in] */ BSTR CharacterID,
            /* [retval][out] */ IDaCtlCharacter **ppidItem);
        
        /* [restricted][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Enum )( 
            IDaCtlCharacters * This,
            /* [retval][out] */ IUnknown **ppunkEnum);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Unload )( 
            IDaCtlCharacters * This,
            /* [in] */ BSTR CharacterID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Load )( 
            IDaCtlCharacters * This,
            /* [in] */ BSTR CharacterID,
            /* [optional][in] */ VARIANT LoadKey,
            /* [retval][out] */ IDaCtlRequest **ppidRequest);
        
        END_INTERFACE
    } IDaCtlCharactersVtbl;

    interface IDaCtlCharacters
    {
        CONST_VTBL struct IDaCtlCharactersVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlCharacters_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlCharacters_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlCharacters_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlCharacters_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlCharacters_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlCharacters_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlCharacters_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlCharacters_get_Item(This,CharacterID,ppidItem)	\
    ( (This)->lpVtbl -> get_Item(This,CharacterID,ppidItem) ) 

#define IDaCtlCharacters_Character(This,CharacterID,ppidItem)	\
    ( (This)->lpVtbl -> Character(This,CharacterID,ppidItem) ) 

#define IDaCtlCharacters_get_Enum(This,ppunkEnum)	\
    ( (This)->lpVtbl -> get_Enum(This,ppunkEnum) ) 

#define IDaCtlCharacters_Unload(This,CharacterID)	\
    ( (This)->lpVtbl -> Unload(This,CharacterID) ) 

#define IDaCtlCharacters_Load(This,CharacterID,LoadKey,ppidRequest)	\
    ( (This)->lpVtbl -> Load(This,CharacterID,LoadKey,ppidRequest) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlCharacters_INTERFACE_DEFINED__ */


#ifndef __IDaCtlCharacter_INTERFACE_DEFINED__
#define __IDaCtlCharacter_INTERFACE_DEFINED__

/* interface IDaCtlCharacter */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaCtlCharacter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E53D-A208-11DE-ABF2-002421116FB2")
    IDaCtlCharacter : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Balloon( 
            /* [retval][out] */ IDaCtlBalloon **ppidBalloon) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Commands( 
            /* [retval][out] */ IDaCtlCommands **ppidCommands) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *Name) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Description( 
            /* [retval][out] */ BSTR *Description) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *Visible) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Left( 
            short Left) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Left( 
            /* [retval][out] */ short *Left) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Top( 
            short Top) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Top( 
            /* [retval][out] */ short *Top) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Height( 
            short Height) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ short *Height) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Width( 
            short Width) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ short *Width) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Speed( 
            /* [retval][out] */ long *Speed) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Pitch( 
            /* [retval][out] */ long *Pitch) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_IdleOn( 
            VARIANT_BOOL On) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_IdleOn( 
            /* [retval][out] */ VARIANT_BOOL *On) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Activate( 
            /* [optional][in] */ VARIANT State,
            /* [retval][out] */ VARIANT_BOOL *Success) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Play( 
            /* [in] */ BSTR Animation,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Get( 
            /* [in] */ BSTR Type,
            /* [in] */ BSTR Name,
            /* [optional][in] */ VARIANT Queue,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Stop( 
            /* [optional][in] */ VARIANT Request) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Wait( 
            /* [in] */ IDaCtlRequest *WaitForRequest,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Interrupt( 
            /* [in] */ IDaCtlRequest *InterruptRequest,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Speak( 
            /* [optional][in] */ VARIANT Text,
            /* [optional][in] */ VARIANT Url,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GestureAt( 
            /* [in] */ short x,
            /* [in] */ short y,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE MoveTo( 
            /* [in] */ short x,
            /* [in] */ short y,
            /* [optional][in] */ VARIANT Speed,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Hide( 
            /* [optional][in] */ VARIANT Fast,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Show( 
            /* [optional][in] */ VARIANT Fast,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE StopAll( 
            /* [optional][in] */ VARIANT Types) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_MoveCause( 
            /* [retval][out] */ short *MoveCause) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_VisibilityCause( 
            /* [retval][out] */ short *VisibilityCause) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_HasOtherClients( 
            /* [retval][out] */ VARIANT_BOOL *HasOtherClients) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_SoundEffectsOn( 
            VARIANT_BOOL On) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_SoundEffectsOn( 
            /* [retval][out] */ VARIANT_BOOL *On) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Name( 
            /* [in] */ BSTR Name) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Description( 
            /* [in] */ BSTR Description) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_ExtraData( 
            /* [retval][out] */ BSTR *ExtraData) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ShowPopupMenu( 
            /* [in] */ short x,
            /* [in] */ short y,
            /* [retval][out] */ VARIANT_BOOL *Showed) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_AutoPopupMenu( 
            /* [in] */ VARIANT_BOOL On) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_AutoPopupMenu( 
            /* [retval][out] */ VARIANT_BOOL *On) = 0;
        
        virtual /* [hidden][propput][id] */ HRESULT STDMETHODCALLTYPE put_HelpModeOn( 
            /* [in] */ VARIANT_BOOL On) = 0;
        
        virtual /* [hidden][propget][id] */ HRESULT STDMETHODCALLTYPE get_HelpModeOn( 
            /* [retval][out] */ VARIANT_BOOL *On) = 0;
        
        virtual /* [hidden][propput][id] */ HRESULT STDMETHODCALLTYPE put_HelpContextID( 
            /* [in] */ long ID) = 0;
        
        virtual /* [hidden][propget][id] */ HRESULT STDMETHODCALLTYPE get_HelpContextID( 
            /* [retval][out] */ long *ID) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Active( 
            /* [retval][out] */ short *State) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Listen( 
            /* [in] */ VARIANT_BOOL Listen,
            /* [retval][out] */ VARIANT_BOOL *StartedListening) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_LanguageID( 
            /* [in] */ long LanguageID) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_LanguageID( 
            /* [retval][out] */ long *LanguageID) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_SRModeID( 
            /* [retval][out] */ BSTR *EngineModeId) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_SRModeID( 
            /* [in] */ BSTR EngineModeId) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_TTSModeID( 
            /* [retval][out] */ BSTR *EngineModeId) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_TTSModeID( 
            /* [in] */ BSTR EngineModeId) = 0;
        
        virtual /* [hidden][propget][id] */ HRESULT STDMETHODCALLTYPE get_HelpFile( 
            /* [retval][out] */ BSTR *File) = 0;
        
        virtual /* [hidden][propput][id] */ HRESULT STDMETHODCALLTYPE put_HelpFile( 
            /* [in] */ BSTR File) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_GUID( 
            /* [retval][out] */ BSTR *GUID) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_OriginalHeight( 
            /* [retval][out] */ short *Height) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_OriginalWidth( 
            /* [retval][out] */ short *Width) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Think( 
            /* [in] */ BSTR Text,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Version( 
            /* [retval][out] */ BSTR *Version) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_AnimationNames( 
            /* [retval][out] */ IDaCtlAnimationNames **Names) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_SRStatus( 
            /* [retval][out] */ long *Status) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlCharacterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlCharacter * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlCharacter * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlCharacter * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlCharacter * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlCharacter * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlCharacter * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlCharacter * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Balloon )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ IDaCtlBalloon **ppidBalloon);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Commands )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ IDaCtlCommands **ppidCommands);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ BSTR *Name);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Description )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ BSTR *Description);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Left )( 
            IDaCtlCharacter * This,
            short Left);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ short *Left);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Top )( 
            IDaCtlCharacter * This,
            short Top);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ short *Top);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Height )( 
            IDaCtlCharacter * This,
            short Height);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ short *Height);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Width )( 
            IDaCtlCharacter * This,
            short Width);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ short *Width);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Speed )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ long *Speed);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Pitch )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ long *Pitch);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_IdleOn )( 
            IDaCtlCharacter * This,
            VARIANT_BOOL On);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_IdleOn )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ VARIANT_BOOL *On);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Activate )( 
            IDaCtlCharacter * This,
            /* [optional][in] */ VARIANT State,
            /* [retval][out] */ VARIANT_BOOL *Success);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Play )( 
            IDaCtlCharacter * This,
            /* [in] */ BSTR Animation,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Get )( 
            IDaCtlCharacter * This,
            /* [in] */ BSTR Type,
            /* [in] */ BSTR Name,
            /* [optional][in] */ VARIANT Queue,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IDaCtlCharacter * This,
            /* [optional][in] */ VARIANT Request);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Wait )( 
            IDaCtlCharacter * This,
            /* [in] */ IDaCtlRequest *WaitForRequest,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Interrupt )( 
            IDaCtlCharacter * This,
            /* [in] */ IDaCtlRequest *InterruptRequest,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Speak )( 
            IDaCtlCharacter * This,
            /* [optional][in] */ VARIANT Text,
            /* [optional][in] */ VARIANT Url,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GestureAt )( 
            IDaCtlCharacter * This,
            /* [in] */ short x,
            /* [in] */ short y,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *MoveTo )( 
            IDaCtlCharacter * This,
            /* [in] */ short x,
            /* [in] */ short y,
            /* [optional][in] */ VARIANT Speed,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Hide )( 
            IDaCtlCharacter * This,
            /* [optional][in] */ VARIANT Fast,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Show )( 
            IDaCtlCharacter * This,
            /* [optional][in] */ VARIANT Fast,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *StopAll )( 
            IDaCtlCharacter * This,
            /* [optional][in] */ VARIANT Types);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_MoveCause )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ short *MoveCause);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_VisibilityCause )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ short *VisibilityCause);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_HasOtherClients )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ VARIANT_BOOL *HasOtherClients);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_SoundEffectsOn )( 
            IDaCtlCharacter * This,
            VARIANT_BOOL On);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_SoundEffectsOn )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ VARIANT_BOOL *On);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Name )( 
            IDaCtlCharacter * This,
            /* [in] */ BSTR Name);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Description )( 
            IDaCtlCharacter * This,
            /* [in] */ BSTR Description);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ExtraData )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ BSTR *ExtraData);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ShowPopupMenu )( 
            IDaCtlCharacter * This,
            /* [in] */ short x,
            /* [in] */ short y,
            /* [retval][out] */ VARIANT_BOOL *Showed);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_AutoPopupMenu )( 
            IDaCtlCharacter * This,
            /* [in] */ VARIANT_BOOL On);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_AutoPopupMenu )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ VARIANT_BOOL *On);
        
        /* [hidden][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_HelpModeOn )( 
            IDaCtlCharacter * This,
            /* [in] */ VARIANT_BOOL On);
        
        /* [hidden][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_HelpModeOn )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ VARIANT_BOOL *On);
        
        /* [hidden][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_HelpContextID )( 
            IDaCtlCharacter * This,
            /* [in] */ long ID);
        
        /* [hidden][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_HelpContextID )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ long *ID);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Active )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ short *State);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Listen )( 
            IDaCtlCharacter * This,
            /* [in] */ VARIANT_BOOL Listen,
            /* [retval][out] */ VARIANT_BOOL *StartedListening);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_LanguageID )( 
            IDaCtlCharacter * This,
            /* [in] */ long LanguageID);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_LanguageID )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ long *LanguageID);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_SRModeID )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ BSTR *EngineModeId);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_SRModeID )( 
            IDaCtlCharacter * This,
            /* [in] */ BSTR EngineModeId);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_TTSModeID )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ BSTR *EngineModeId);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_TTSModeID )( 
            IDaCtlCharacter * This,
            /* [in] */ BSTR EngineModeId);
        
        /* [hidden][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_HelpFile )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ BSTR *File);
        
        /* [hidden][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_HelpFile )( 
            IDaCtlCharacter * This,
            /* [in] */ BSTR File);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_GUID )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ BSTR *GUID);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_OriginalHeight )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ short *Height);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_OriginalWidth )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ short *Width);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Think )( 
            IDaCtlCharacter * This,
            /* [in] */ BSTR Text,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Version )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ BSTR *Version);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_AnimationNames )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ IDaCtlAnimationNames **Names);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_SRStatus )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ long *Status);
        
        END_INTERFACE
    } IDaCtlCharacterVtbl;

    interface IDaCtlCharacter
    {
        CONST_VTBL struct IDaCtlCharacterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlCharacter_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlCharacter_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlCharacter_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlCharacter_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlCharacter_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlCharacter_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlCharacter_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlCharacter_get_Balloon(This,ppidBalloon)	\
    ( (This)->lpVtbl -> get_Balloon(This,ppidBalloon) ) 

#define IDaCtlCharacter_get_Commands(This,ppidCommands)	\
    ( (This)->lpVtbl -> get_Commands(This,ppidCommands) ) 

#define IDaCtlCharacter_get_Name(This,Name)	\
    ( (This)->lpVtbl -> get_Name(This,Name) ) 

#define IDaCtlCharacter_get_Description(This,Description)	\
    ( (This)->lpVtbl -> get_Description(This,Description) ) 

#define IDaCtlCharacter_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaCtlCharacter_put_Left(This,Left)	\
    ( (This)->lpVtbl -> put_Left(This,Left) ) 

#define IDaCtlCharacter_get_Left(This,Left)	\
    ( (This)->lpVtbl -> get_Left(This,Left) ) 

#define IDaCtlCharacter_put_Top(This,Top)	\
    ( (This)->lpVtbl -> put_Top(This,Top) ) 

#define IDaCtlCharacter_get_Top(This,Top)	\
    ( (This)->lpVtbl -> get_Top(This,Top) ) 

#define IDaCtlCharacter_put_Height(This,Height)	\
    ( (This)->lpVtbl -> put_Height(This,Height) ) 

#define IDaCtlCharacter_get_Height(This,Height)	\
    ( (This)->lpVtbl -> get_Height(This,Height) ) 

#define IDaCtlCharacter_put_Width(This,Width)	\
    ( (This)->lpVtbl -> put_Width(This,Width) ) 

#define IDaCtlCharacter_get_Width(This,Width)	\
    ( (This)->lpVtbl -> get_Width(This,Width) ) 

#define IDaCtlCharacter_get_Speed(This,Speed)	\
    ( (This)->lpVtbl -> get_Speed(This,Speed) ) 

#define IDaCtlCharacter_get_Pitch(This,Pitch)	\
    ( (This)->lpVtbl -> get_Pitch(This,Pitch) ) 

#define IDaCtlCharacter_put_IdleOn(This,On)	\
    ( (This)->lpVtbl -> put_IdleOn(This,On) ) 

#define IDaCtlCharacter_get_IdleOn(This,On)	\
    ( (This)->lpVtbl -> get_IdleOn(This,On) ) 

#define IDaCtlCharacter_Activate(This,State,Success)	\
    ( (This)->lpVtbl -> Activate(This,State,Success) ) 

#define IDaCtlCharacter_Play(This,Animation,Request)	\
    ( (This)->lpVtbl -> Play(This,Animation,Request) ) 

#define IDaCtlCharacter_Get(This,Type,Name,Queue,Request)	\
    ( (This)->lpVtbl -> Get(This,Type,Name,Queue,Request) ) 

#define IDaCtlCharacter_Stop(This,Request)	\
    ( (This)->lpVtbl -> Stop(This,Request) ) 

#define IDaCtlCharacter_Wait(This,WaitForRequest,Request)	\
    ( (This)->lpVtbl -> Wait(This,WaitForRequest,Request) ) 

#define IDaCtlCharacter_Interrupt(This,InterruptRequest,Request)	\
    ( (This)->lpVtbl -> Interrupt(This,InterruptRequest,Request) ) 

#define IDaCtlCharacter_Speak(This,Text,Url,Request)	\
    ( (This)->lpVtbl -> Speak(This,Text,Url,Request) ) 

#define IDaCtlCharacter_GestureAt(This,x,y,Request)	\
    ( (This)->lpVtbl -> GestureAt(This,x,y,Request) ) 

#define IDaCtlCharacter_MoveTo(This,x,y,Speed,Request)	\
    ( (This)->lpVtbl -> MoveTo(This,x,y,Speed,Request) ) 

#define IDaCtlCharacter_Hide(This,Fast,Request)	\
    ( (This)->lpVtbl -> Hide(This,Fast,Request) ) 

#define IDaCtlCharacter_Show(This,Fast,Request)	\
    ( (This)->lpVtbl -> Show(This,Fast,Request) ) 

#define IDaCtlCharacter_StopAll(This,Types)	\
    ( (This)->lpVtbl -> StopAll(This,Types) ) 

#define IDaCtlCharacter_get_MoveCause(This,MoveCause)	\
    ( (This)->lpVtbl -> get_MoveCause(This,MoveCause) ) 

#define IDaCtlCharacter_get_VisibilityCause(This,VisibilityCause)	\
    ( (This)->lpVtbl -> get_VisibilityCause(This,VisibilityCause) ) 

#define IDaCtlCharacter_get_HasOtherClients(This,HasOtherClients)	\
    ( (This)->lpVtbl -> get_HasOtherClients(This,HasOtherClients) ) 

#define IDaCtlCharacter_put_SoundEffectsOn(This,On)	\
    ( (This)->lpVtbl -> put_SoundEffectsOn(This,On) ) 

#define IDaCtlCharacter_get_SoundEffectsOn(This,On)	\
    ( (This)->lpVtbl -> get_SoundEffectsOn(This,On) ) 

#define IDaCtlCharacter_put_Name(This,Name)	\
    ( (This)->lpVtbl -> put_Name(This,Name) ) 

#define IDaCtlCharacter_put_Description(This,Description)	\
    ( (This)->lpVtbl -> put_Description(This,Description) ) 

#define IDaCtlCharacter_get_ExtraData(This,ExtraData)	\
    ( (This)->lpVtbl -> get_ExtraData(This,ExtraData) ) 

#define IDaCtlCharacter_ShowPopupMenu(This,x,y,Showed)	\
    ( (This)->lpVtbl -> ShowPopupMenu(This,x,y,Showed) ) 

#define IDaCtlCharacter_put_AutoPopupMenu(This,On)	\
    ( (This)->lpVtbl -> put_AutoPopupMenu(This,On) ) 

#define IDaCtlCharacter_get_AutoPopupMenu(This,On)	\
    ( (This)->lpVtbl -> get_AutoPopupMenu(This,On) ) 

#define IDaCtlCharacter_put_HelpModeOn(This,On)	\
    ( (This)->lpVtbl -> put_HelpModeOn(This,On) ) 

#define IDaCtlCharacter_get_HelpModeOn(This,On)	\
    ( (This)->lpVtbl -> get_HelpModeOn(This,On) ) 

#define IDaCtlCharacter_put_HelpContextID(This,ID)	\
    ( (This)->lpVtbl -> put_HelpContextID(This,ID) ) 

#define IDaCtlCharacter_get_HelpContextID(This,ID)	\
    ( (This)->lpVtbl -> get_HelpContextID(This,ID) ) 

#define IDaCtlCharacter_get_Active(This,State)	\
    ( (This)->lpVtbl -> get_Active(This,State) ) 

#define IDaCtlCharacter_Listen(This,Listen,StartedListening)	\
    ( (This)->lpVtbl -> Listen(This,Listen,StartedListening) ) 

#define IDaCtlCharacter_put_LanguageID(This,LanguageID)	\
    ( (This)->lpVtbl -> put_LanguageID(This,LanguageID) ) 

#define IDaCtlCharacter_get_LanguageID(This,LanguageID)	\
    ( (This)->lpVtbl -> get_LanguageID(This,LanguageID) ) 

#define IDaCtlCharacter_get_SRModeID(This,EngineModeId)	\
    ( (This)->lpVtbl -> get_SRModeID(This,EngineModeId) ) 

#define IDaCtlCharacter_put_SRModeID(This,EngineModeId)	\
    ( (This)->lpVtbl -> put_SRModeID(This,EngineModeId) ) 

#define IDaCtlCharacter_get_TTSModeID(This,EngineModeId)	\
    ( (This)->lpVtbl -> get_TTSModeID(This,EngineModeId) ) 

#define IDaCtlCharacter_put_TTSModeID(This,EngineModeId)	\
    ( (This)->lpVtbl -> put_TTSModeID(This,EngineModeId) ) 

#define IDaCtlCharacter_get_HelpFile(This,File)	\
    ( (This)->lpVtbl -> get_HelpFile(This,File) ) 

#define IDaCtlCharacter_put_HelpFile(This,File)	\
    ( (This)->lpVtbl -> put_HelpFile(This,File) ) 

#define IDaCtlCharacter_get_GUID(This,GUID)	\
    ( (This)->lpVtbl -> get_GUID(This,GUID) ) 

#define IDaCtlCharacter_get_OriginalHeight(This,Height)	\
    ( (This)->lpVtbl -> get_OriginalHeight(This,Height) ) 

#define IDaCtlCharacter_get_OriginalWidth(This,Width)	\
    ( (This)->lpVtbl -> get_OriginalWidth(This,Width) ) 

#define IDaCtlCharacter_Think(This,Text,Request)	\
    ( (This)->lpVtbl -> Think(This,Text,Request) ) 

#define IDaCtlCharacter_get_Version(This,Version)	\
    ( (This)->lpVtbl -> get_Version(This,Version) ) 

#define IDaCtlCharacter_get_AnimationNames(This,Names)	\
    ( (This)->lpVtbl -> get_AnimationNames(This,Names) ) 

#define IDaCtlCharacter_get_SRStatus(This,Status)	\
    ( (This)->lpVtbl -> get_SRStatus(This,Status) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlCharacter_INTERFACE_DEFINED__ */


#ifndef __IDaCtlBalloon_INTERFACE_DEFINED__
#define __IDaCtlBalloon_INTERFACE_DEFINED__

/* interface IDaCtlBalloon */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaCtlBalloon;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E53F-A208-11DE-ABF2-002421116FB2")
    IDaCtlBalloon : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Enabled( 
            /* [retval][out] */ VARIANT_BOOL *Enabled) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_NumberOfLines( 
            /* [retval][out] */ long *Lines) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_CharsPerLine( 
            /* [retval][out] */ long *CharsPerLine) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_FontName( 
            /* [retval][out] */ BSTR *FontName) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_FontSize( 
            /* [retval][out] */ long *FontSize) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_FontBold( 
            /* [retval][out] */ VARIANT_BOOL *FontBold) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_FontItalic( 
            /* [retval][out] */ VARIANT_BOOL *FontItalic) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_FontStrikethru( 
            /* [retval][out] */ VARIANT_BOOL *FontStrikethru) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_FontUnderline( 
            /* [retval][out] */ VARIANT_BOOL *FontUnderline) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_ForeColor( 
            /* [retval][out] */ long *ForeColor) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_BackColor( 
            /* [retval][out] */ long *BackColor) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_BorderColor( 
            /* [retval][out] */ long *BorderColor) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL Visible) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *Visible) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_FontName( 
            /* [in] */ BSTR FontName) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_FontSize( 
            /* [in] */ long FontSize) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_FontCharSet( 
            /* [in] */ short FontCharSet) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_FontCharSet( 
            /* [retval][out] */ short *FontCharSet) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Style( 
            /* [in] */ long Style) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Style( 
            /* [retval][out] */ long *Style) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlBalloonVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlBalloon * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlBalloon * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlBalloon * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlBalloon * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlBalloon * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlBalloon * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlBalloon * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ VARIANT_BOOL *Enabled);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_NumberOfLines )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ long *Lines);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_CharsPerLine )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ long *CharsPerLine);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_FontName )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ BSTR *FontName);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_FontSize )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ long *FontSize);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_FontBold )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ VARIANT_BOOL *FontBold);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_FontItalic )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ VARIANT_BOOL *FontItalic);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_FontStrikethru )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ VARIANT_BOOL *FontStrikethru);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_FontUnderline )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ VARIANT_BOOL *FontUnderline);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ForeColor )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ long *ForeColor);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_BackColor )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ long *BackColor);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_BorderColor )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ long *BorderColor);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IDaCtlBalloon * This,
            /* [in] */ VARIANT_BOOL Visible);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontName )( 
            IDaCtlBalloon * This,
            /* [in] */ BSTR FontName);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontSize )( 
            IDaCtlBalloon * This,
            /* [in] */ long FontSize);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontCharSet )( 
            IDaCtlBalloon * This,
            /* [in] */ short FontCharSet);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_FontCharSet )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ short *FontCharSet);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Style )( 
            IDaCtlBalloon * This,
            /* [in] */ long Style);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Style )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ long *Style);
        
        END_INTERFACE
    } IDaCtlBalloonVtbl;

    interface IDaCtlBalloon
    {
        CONST_VTBL struct IDaCtlBalloonVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlBalloon_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlBalloon_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlBalloon_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlBalloon_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlBalloon_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlBalloon_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlBalloon_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlBalloon_get_Enabled(This,Enabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,Enabled) ) 

#define IDaCtlBalloon_get_NumberOfLines(This,Lines)	\
    ( (This)->lpVtbl -> get_NumberOfLines(This,Lines) ) 

#define IDaCtlBalloon_get_CharsPerLine(This,CharsPerLine)	\
    ( (This)->lpVtbl -> get_CharsPerLine(This,CharsPerLine) ) 

#define IDaCtlBalloon_get_FontName(This,FontName)	\
    ( (This)->lpVtbl -> get_FontName(This,FontName) ) 

#define IDaCtlBalloon_get_FontSize(This,FontSize)	\
    ( (This)->lpVtbl -> get_FontSize(This,FontSize) ) 

#define IDaCtlBalloon_get_FontBold(This,FontBold)	\
    ( (This)->lpVtbl -> get_FontBold(This,FontBold) ) 

#define IDaCtlBalloon_get_FontItalic(This,FontItalic)	\
    ( (This)->lpVtbl -> get_FontItalic(This,FontItalic) ) 

#define IDaCtlBalloon_get_FontStrikethru(This,FontStrikethru)	\
    ( (This)->lpVtbl -> get_FontStrikethru(This,FontStrikethru) ) 

#define IDaCtlBalloon_get_FontUnderline(This,FontUnderline)	\
    ( (This)->lpVtbl -> get_FontUnderline(This,FontUnderline) ) 

#define IDaCtlBalloon_get_ForeColor(This,ForeColor)	\
    ( (This)->lpVtbl -> get_ForeColor(This,ForeColor) ) 

#define IDaCtlBalloon_get_BackColor(This,BackColor)	\
    ( (This)->lpVtbl -> get_BackColor(This,BackColor) ) 

#define IDaCtlBalloon_get_BorderColor(This,BorderColor)	\
    ( (This)->lpVtbl -> get_BorderColor(This,BorderColor) ) 

#define IDaCtlBalloon_put_Visible(This,Visible)	\
    ( (This)->lpVtbl -> put_Visible(This,Visible) ) 

#define IDaCtlBalloon_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaCtlBalloon_put_FontName(This,FontName)	\
    ( (This)->lpVtbl -> put_FontName(This,FontName) ) 

#define IDaCtlBalloon_put_FontSize(This,FontSize)	\
    ( (This)->lpVtbl -> put_FontSize(This,FontSize) ) 

#define IDaCtlBalloon_put_FontCharSet(This,FontCharSet)	\
    ( (This)->lpVtbl -> put_FontCharSet(This,FontCharSet) ) 

#define IDaCtlBalloon_get_FontCharSet(This,FontCharSet)	\
    ( (This)->lpVtbl -> get_FontCharSet(This,FontCharSet) ) 

#define IDaCtlBalloon_put_Style(This,Style)	\
    ( (This)->lpVtbl -> put_Style(This,Style) ) 

#define IDaCtlBalloon_get_Style(This,Style)	\
    ( (This)->lpVtbl -> get_Style(This,Style) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlBalloon_INTERFACE_DEFINED__ */


#ifndef __IDaCtlCommands_INTERFACE_DEFINED__
#define __IDaCtlCommands_INTERFACE_DEFINED__

/* interface IDaCtlCommands */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaCtlCommands;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E540-A208-11DE-ABF2-002421116FB2")
    IDaCtlCommands : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ BSTR Name,
            /* [retval][out] */ IDaCtlCommand **Item) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Command( 
            /* [in] */ BSTR Name,
            /* [retval][out] */ IDaCtlCommand **Item) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *Count) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Caption( 
            /* [retval][out] */ BSTR *Caption) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Caption( 
            /* [in] */ BSTR Caption) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Voice( 
            /* [retval][out] */ BSTR *Voice) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Voice( 
            /* [in] */ BSTR Voice) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *Visible) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL Visible) = 0;
        
        virtual /* [restricted][propget][id] */ HRESULT STDMETHODCALLTYPE get_Enum( 
            /* [retval][out] */ IUnknown **ppunkEnum) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ BSTR Name,
            /* [optional][in] */ VARIANT Caption,
            /* [optional][in] */ VARIANT Voice,
            /* [optional][in] */ VARIANT Enabled,
            /* [optional][in] */ VARIANT Visible,
            /* [retval][out] */ IDaCtlCommand **Command) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Insert( 
            /* [in] */ BSTR Name,
            /* [in] */ BSTR RefName,
            /* [optional][in] */ VARIANT Before,
            /* [optional][in] */ VARIANT Caption,
            /* [optional][in] */ VARIANT Voice,
            /* [optional][in] */ VARIANT Enabled,
            /* [optional][in] */ VARIANT Visible,
            /* [retval][out] */ IDaCtlCommand **Command) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ BSTR Name) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RemoveAll( void) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_DefaultCommand( 
            /* [retval][out] */ BSTR *Name) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_DefaultCommand( 
            /* [in] */ BSTR Name) = 0;
        
        virtual /* [hidden][propget][id] */ HRESULT STDMETHODCALLTYPE get_HelpContextID( 
            /* [retval][out] */ long *ID) = 0;
        
        virtual /* [hidden][propput][id] */ HRESULT STDMETHODCALLTYPE put_HelpContextID( 
            /* [in] */ long ID) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_FontName( 
            /* [retval][out] */ BSTR *FontName) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_FontName( 
            /* [in] */ BSTR FontName) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_FontSize( 
            /* [retval][out] */ long *FontSize) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_FontSize( 
            /* [in] */ long FontSize) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_VoiceCaption( 
            /* [retval][out] */ BSTR *VoiceCaption) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_VoiceCaption( 
            /* [in] */ BSTR VoiceCaption) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_GlobalVoiceCommandsEnabled( 
            /* [retval][out] */ VARIANT_BOOL *Enable) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_GlobalVoiceCommandsEnabled( 
            /* [in] */ VARIANT_BOOL Enable) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlCommandsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlCommands * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlCommands * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlCommands * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlCommands * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlCommands * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlCommands * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlCommands * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IDaCtlCommands * This,
            /* [in] */ BSTR Name,
            /* [retval][out] */ IDaCtlCommand **Item);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Command )( 
            IDaCtlCommands * This,
            /* [in] */ BSTR Name,
            /* [retval][out] */ IDaCtlCommand **Item);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IDaCtlCommands * This,
            /* [retval][out] */ long *Count);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Caption )( 
            IDaCtlCommands * This,
            /* [retval][out] */ BSTR *Caption);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Caption )( 
            IDaCtlCommands * This,
            /* [in] */ BSTR Caption);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Voice )( 
            IDaCtlCommands * This,
            /* [retval][out] */ BSTR *Voice);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Voice )( 
            IDaCtlCommands * This,
            /* [in] */ BSTR Voice);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaCtlCommands * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IDaCtlCommands * This,
            /* [in] */ VARIANT_BOOL Visible);
        
        /* [restricted][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Enum )( 
            IDaCtlCommands * This,
            /* [retval][out] */ IUnknown **ppunkEnum);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IDaCtlCommands * This,
            /* [in] */ BSTR Name,
            /* [optional][in] */ VARIANT Caption,
            /* [optional][in] */ VARIANT Voice,
            /* [optional][in] */ VARIANT Enabled,
            /* [optional][in] */ VARIANT Visible,
            /* [retval][out] */ IDaCtlCommand **Command);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Insert )( 
            IDaCtlCommands * This,
            /* [in] */ BSTR Name,
            /* [in] */ BSTR RefName,
            /* [optional][in] */ VARIANT Before,
            /* [optional][in] */ VARIANT Caption,
            /* [optional][in] */ VARIANT Voice,
            /* [optional][in] */ VARIANT Enabled,
            /* [optional][in] */ VARIANT Visible,
            /* [retval][out] */ IDaCtlCommand **Command);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Remove )( 
            IDaCtlCommands * This,
            /* [in] */ BSTR Name);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RemoveAll )( 
            IDaCtlCommands * This);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_DefaultCommand )( 
            IDaCtlCommands * This,
            /* [retval][out] */ BSTR *Name);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_DefaultCommand )( 
            IDaCtlCommands * This,
            /* [in] */ BSTR Name);
        
        /* [hidden][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_HelpContextID )( 
            IDaCtlCommands * This,
            /* [retval][out] */ long *ID);
        
        /* [hidden][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_HelpContextID )( 
            IDaCtlCommands * This,
            /* [in] */ long ID);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_FontName )( 
            IDaCtlCommands * This,
            /* [retval][out] */ BSTR *FontName);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontName )( 
            IDaCtlCommands * This,
            /* [in] */ BSTR FontName);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_FontSize )( 
            IDaCtlCommands * This,
            /* [retval][out] */ long *FontSize);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontSize )( 
            IDaCtlCommands * This,
            /* [in] */ long FontSize);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_VoiceCaption )( 
            IDaCtlCommands * This,
            /* [retval][out] */ BSTR *VoiceCaption);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_VoiceCaption )( 
            IDaCtlCommands * This,
            /* [in] */ BSTR VoiceCaption);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_GlobalVoiceCommandsEnabled )( 
            IDaCtlCommands * This,
            /* [retval][out] */ VARIANT_BOOL *Enable);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_GlobalVoiceCommandsEnabled )( 
            IDaCtlCommands * This,
            /* [in] */ VARIANT_BOOL Enable);
        
        END_INTERFACE
    } IDaCtlCommandsVtbl;

    interface IDaCtlCommands
    {
        CONST_VTBL struct IDaCtlCommandsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlCommands_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlCommands_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlCommands_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlCommands_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlCommands_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlCommands_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlCommands_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlCommands_get_Item(This,Name,Item)	\
    ( (This)->lpVtbl -> get_Item(This,Name,Item) ) 

#define IDaCtlCommands_Command(This,Name,Item)	\
    ( (This)->lpVtbl -> Command(This,Name,Item) ) 

#define IDaCtlCommands_get_Count(This,Count)	\
    ( (This)->lpVtbl -> get_Count(This,Count) ) 

#define IDaCtlCommands_get_Caption(This,Caption)	\
    ( (This)->lpVtbl -> get_Caption(This,Caption) ) 

#define IDaCtlCommands_put_Caption(This,Caption)	\
    ( (This)->lpVtbl -> put_Caption(This,Caption) ) 

#define IDaCtlCommands_get_Voice(This,Voice)	\
    ( (This)->lpVtbl -> get_Voice(This,Voice) ) 

#define IDaCtlCommands_put_Voice(This,Voice)	\
    ( (This)->lpVtbl -> put_Voice(This,Voice) ) 

#define IDaCtlCommands_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaCtlCommands_put_Visible(This,Visible)	\
    ( (This)->lpVtbl -> put_Visible(This,Visible) ) 

#define IDaCtlCommands_get_Enum(This,ppunkEnum)	\
    ( (This)->lpVtbl -> get_Enum(This,ppunkEnum) ) 

#define IDaCtlCommands_Add(This,Name,Caption,Voice,Enabled,Visible,Command)	\
    ( (This)->lpVtbl -> Add(This,Name,Caption,Voice,Enabled,Visible,Command) ) 

#define IDaCtlCommands_Insert(This,Name,RefName,Before,Caption,Voice,Enabled,Visible,Command)	\
    ( (This)->lpVtbl -> Insert(This,Name,RefName,Before,Caption,Voice,Enabled,Visible,Command) ) 

#define IDaCtlCommands_Remove(This,Name)	\
    ( (This)->lpVtbl -> Remove(This,Name) ) 

#define IDaCtlCommands_RemoveAll(This)	\
    ( (This)->lpVtbl -> RemoveAll(This) ) 

#define IDaCtlCommands_get_DefaultCommand(This,Name)	\
    ( (This)->lpVtbl -> get_DefaultCommand(This,Name) ) 

#define IDaCtlCommands_put_DefaultCommand(This,Name)	\
    ( (This)->lpVtbl -> put_DefaultCommand(This,Name) ) 

#define IDaCtlCommands_get_HelpContextID(This,ID)	\
    ( (This)->lpVtbl -> get_HelpContextID(This,ID) ) 

#define IDaCtlCommands_put_HelpContextID(This,ID)	\
    ( (This)->lpVtbl -> put_HelpContextID(This,ID) ) 

#define IDaCtlCommands_get_FontName(This,FontName)	\
    ( (This)->lpVtbl -> get_FontName(This,FontName) ) 

#define IDaCtlCommands_put_FontName(This,FontName)	\
    ( (This)->lpVtbl -> put_FontName(This,FontName) ) 

#define IDaCtlCommands_get_FontSize(This,FontSize)	\
    ( (This)->lpVtbl -> get_FontSize(This,FontSize) ) 

#define IDaCtlCommands_put_FontSize(This,FontSize)	\
    ( (This)->lpVtbl -> put_FontSize(This,FontSize) ) 

#define IDaCtlCommands_get_VoiceCaption(This,VoiceCaption)	\
    ( (This)->lpVtbl -> get_VoiceCaption(This,VoiceCaption) ) 

#define IDaCtlCommands_put_VoiceCaption(This,VoiceCaption)	\
    ( (This)->lpVtbl -> put_VoiceCaption(This,VoiceCaption) ) 

#define IDaCtlCommands_get_GlobalVoiceCommandsEnabled(This,Enable)	\
    ( (This)->lpVtbl -> get_GlobalVoiceCommandsEnabled(This,Enable) ) 

#define IDaCtlCommands_put_GlobalVoiceCommandsEnabled(This,Enable)	\
    ( (This)->lpVtbl -> put_GlobalVoiceCommandsEnabled(This,Enable) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlCommands_INTERFACE_DEFINED__ */


#ifndef __IDaCtlCommand_INTERFACE_DEFINED__
#define __IDaCtlCommand_INTERFACE_DEFINED__

/* interface IDaCtlCommand */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaCtlCommand;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E541-A208-11DE-ABF2-002421116FB2")
    IDaCtlCommand : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Voice( 
            /* [retval][out] */ BSTR *Voice) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Voice( 
            /* [in] */ BSTR Voice) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Caption( 
            /* [retval][out] */ BSTR *Caption) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Caption( 
            /* [in] */ BSTR Caption) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Enabled( 
            /* [retval][out] */ VARIANT_BOOL *Enabled) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Enabled( 
            /* [in] */ VARIANT_BOOL Enabled) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *Visible) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL Visible) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Confidence( 
            /* [retval][out] */ long *Confidence) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Confidence( 
            /* [in] */ long Confidence) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_ConfidenceText( 
            /* [retval][out] */ BSTR *Text) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_ConfidenceText( 
            /* [in] */ BSTR Text) = 0;
        
        virtual /* [hidden][propget][id] */ HRESULT STDMETHODCALLTYPE get_HelpContextID( 
            /* [retval][out] */ long *ID) = 0;
        
        virtual /* [hidden][propput][id] */ HRESULT STDMETHODCALLTYPE put_HelpContextID( 
            /* [in] */ long ID) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_VoiceCaption( 
            /* [retval][out] */ BSTR *Caption) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_VoiceCaption( 
            /* [in] */ BSTR Caption) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlCommandVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlCommand * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlCommand * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlCommand * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlCommand * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlCommand * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlCommand * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlCommand * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Voice )( 
            IDaCtlCommand * This,
            /* [retval][out] */ BSTR *Voice);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Voice )( 
            IDaCtlCommand * This,
            /* [in] */ BSTR Voice);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Caption )( 
            IDaCtlCommand * This,
            /* [retval][out] */ BSTR *Caption);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Caption )( 
            IDaCtlCommand * This,
            /* [in] */ BSTR Caption);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IDaCtlCommand * This,
            /* [retval][out] */ VARIANT_BOOL *Enabled);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            IDaCtlCommand * This,
            /* [in] */ VARIANT_BOOL Enabled);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaCtlCommand * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IDaCtlCommand * This,
            /* [in] */ VARIANT_BOOL Visible);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Confidence )( 
            IDaCtlCommand * This,
            /* [retval][out] */ long *Confidence);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Confidence )( 
            IDaCtlCommand * This,
            /* [in] */ long Confidence);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ConfidenceText )( 
            IDaCtlCommand * This,
            /* [retval][out] */ BSTR *Text);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ConfidenceText )( 
            IDaCtlCommand * This,
            /* [in] */ BSTR Text);
        
        /* [hidden][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_HelpContextID )( 
            IDaCtlCommand * This,
            /* [retval][out] */ long *ID);
        
        /* [hidden][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_HelpContextID )( 
            IDaCtlCommand * This,
            /* [in] */ long ID);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_VoiceCaption )( 
            IDaCtlCommand * This,
            /* [retval][out] */ BSTR *Caption);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_VoiceCaption )( 
            IDaCtlCommand * This,
            /* [in] */ BSTR Caption);
        
        END_INTERFACE
    } IDaCtlCommandVtbl;

    interface IDaCtlCommand
    {
        CONST_VTBL struct IDaCtlCommandVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlCommand_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlCommand_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlCommand_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlCommand_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlCommand_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlCommand_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlCommand_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlCommand_get_Voice(This,Voice)	\
    ( (This)->lpVtbl -> get_Voice(This,Voice) ) 

#define IDaCtlCommand_put_Voice(This,Voice)	\
    ( (This)->lpVtbl -> put_Voice(This,Voice) ) 

#define IDaCtlCommand_get_Caption(This,Caption)	\
    ( (This)->lpVtbl -> get_Caption(This,Caption) ) 

#define IDaCtlCommand_put_Caption(This,Caption)	\
    ( (This)->lpVtbl -> put_Caption(This,Caption) ) 

#define IDaCtlCommand_get_Enabled(This,Enabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,Enabled) ) 

#define IDaCtlCommand_put_Enabled(This,Enabled)	\
    ( (This)->lpVtbl -> put_Enabled(This,Enabled) ) 

#define IDaCtlCommand_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaCtlCommand_put_Visible(This,Visible)	\
    ( (This)->lpVtbl -> put_Visible(This,Visible) ) 

#define IDaCtlCommand_get_Confidence(This,Confidence)	\
    ( (This)->lpVtbl -> get_Confidence(This,Confidence) ) 

#define IDaCtlCommand_put_Confidence(This,Confidence)	\
    ( (This)->lpVtbl -> put_Confidence(This,Confidence) ) 

#define IDaCtlCommand_get_ConfidenceText(This,Text)	\
    ( (This)->lpVtbl -> get_ConfidenceText(This,Text) ) 

#define IDaCtlCommand_put_ConfidenceText(This,Text)	\
    ( (This)->lpVtbl -> put_ConfidenceText(This,Text) ) 

#define IDaCtlCommand_get_HelpContextID(This,ID)	\
    ( (This)->lpVtbl -> get_HelpContextID(This,ID) ) 

#define IDaCtlCommand_put_HelpContextID(This,ID)	\
    ( (This)->lpVtbl -> put_HelpContextID(This,ID) ) 

#define IDaCtlCommand_get_VoiceCaption(This,Caption)	\
    ( (This)->lpVtbl -> get_VoiceCaption(This,Caption) ) 

#define IDaCtlCommand_put_VoiceCaption(This,Caption)	\
    ( (This)->lpVtbl -> put_VoiceCaption(This,Caption) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlCommand_INTERFACE_DEFINED__ */


#ifndef __IDaCtlRequest_INTERFACE_DEFINED__
#define __IDaCtlRequest_INTERFACE_DEFINED__

/* interface IDaCtlRequest */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaCtlRequest;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E542-A208-11DE-ABF2-002421116FB2")
    IDaCtlRequest : public IDispatch
    {
    public:
        virtual /* [hidden][propget][id] */ HRESULT STDMETHODCALLTYPE get_ID( 
            /* [retval][out] */ long *ID) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Status( 
            /* [retval][out] */ long *Status) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Description( 
            /* [retval][out] */ BSTR *Description) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Number( 
            /* [retval][out] */ long *Number) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlRequestVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlRequest * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlRequest * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlRequest * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlRequest * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlRequest * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlRequest * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlRequest * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [hidden][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ID )( 
            IDaCtlRequest * This,
            /* [retval][out] */ long *ID);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Status )( 
            IDaCtlRequest * This,
            /* [retval][out] */ long *Status);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Description )( 
            IDaCtlRequest * This,
            /* [retval][out] */ BSTR *Description);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Number )( 
            IDaCtlRequest * This,
            /* [retval][out] */ long *Number);
        
        END_INTERFACE
    } IDaCtlRequestVtbl;

    interface IDaCtlRequest
    {
        CONST_VTBL struct IDaCtlRequestVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlRequest_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlRequest_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlRequest_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlRequest_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlRequest_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlRequest_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlRequest_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlRequest_get_ID(This,ID)	\
    ( (This)->lpVtbl -> get_ID(This,ID) ) 

#define IDaCtlRequest_get_Status(This,Status)	\
    ( (This)->lpVtbl -> get_Status(This,Status) ) 

#define IDaCtlRequest_get_Description(This,Description)	\
    ( (This)->lpVtbl -> get_Description(This,Description) ) 

#define IDaCtlRequest_get_Number(This,Number)	\
    ( (This)->lpVtbl -> get_Number(This,Number) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlRequest_INTERFACE_DEFINED__ */


#ifndef __IDaCtlAudioObject_INTERFACE_DEFINED__
#define __IDaCtlAudioObject_INTERFACE_DEFINED__

/* interface IDaCtlAudioObject */
/* [object][hidden][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaCtlAudioObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E543-A208-11DE-ABF2-002421116FB2")
    IDaCtlAudioObject : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Enabled( 
            /* [retval][out] */ VARIANT_BOOL *AudioEnabled) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_SoundEffects( 
            /* [retval][out] */ VARIANT_BOOL *SoundEffects) = 0;
        
        virtual /* [hidden][propget][id] */ HRESULT STDMETHODCALLTYPE get_Status( 
            /* [retval][out] */ short *Available) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlAudioObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlAudioObject * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlAudioObject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlAudioObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlAudioObject * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlAudioObject * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlAudioObject * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlAudioObject * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IDaCtlAudioObject * This,
            /* [retval][out] */ VARIANT_BOOL *AudioEnabled);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_SoundEffects )( 
            IDaCtlAudioObject * This,
            /* [retval][out] */ VARIANT_BOOL *SoundEffects);
        
        /* [hidden][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Status )( 
            IDaCtlAudioObject * This,
            /* [retval][out] */ short *Available);
        
        END_INTERFACE
    } IDaCtlAudioObjectVtbl;

    interface IDaCtlAudioObject
    {
        CONST_VTBL struct IDaCtlAudioObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlAudioObject_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlAudioObject_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlAudioObject_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlAudioObject_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlAudioObject_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlAudioObject_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlAudioObject_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlAudioObject_get_Enabled(This,AudioEnabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,AudioEnabled) ) 

#define IDaCtlAudioObject_get_SoundEffects(This,SoundEffects)	\
    ( (This)->lpVtbl -> get_SoundEffects(This,SoundEffects) ) 

#define IDaCtlAudioObject_get_Status(This,Available)	\
    ( (This)->lpVtbl -> get_Status(This,Available) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlAudioObject_INTERFACE_DEFINED__ */


#ifndef __IDaCtlSpeechInput_INTERFACE_DEFINED__
#define __IDaCtlSpeechInput_INTERFACE_DEFINED__

/* interface IDaCtlSpeechInput */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaCtlSpeechInput;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E544-A208-11DE-ABF2-002421116FB2")
    IDaCtlSpeechInput : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Enabled( 
            /* [retval][out] */ VARIANT_BOOL *VoiceEnabled) = 0;
        
        virtual /* [hidden][propget][id] */ HRESULT STDMETHODCALLTYPE get_Language( 
            /* [retval][out] */ BSTR *Language) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_HotKey( 
            /* [retval][out] */ BSTR *HotKey) = 0;
        
        virtual /* [hidden][propget][id] */ HRESULT STDMETHODCALLTYPE get_Installed( 
            /* [retval][out] */ VARIANT_BOOL *VoiceInstalled) = 0;
        
        virtual /* [hidden][propget][id] */ HRESULT STDMETHODCALLTYPE get_Engine( 
            /* [retval][out] */ BSTR *Engine) = 0;
        
        virtual /* [hidden][propput][id] */ HRESULT STDMETHODCALLTYPE put_Engine( 
            /* [in] */ BSTR Engine) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_ListeningTip( 
            /* [retval][out] */ VARIANT_BOOL *ListeningTip) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlSpeechInputVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlSpeechInput * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlSpeechInput * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlSpeechInput * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlSpeechInput * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlSpeechInput * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlSpeechInput * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlSpeechInput * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IDaCtlSpeechInput * This,
            /* [retval][out] */ VARIANT_BOOL *VoiceEnabled);
        
        /* [hidden][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Language )( 
            IDaCtlSpeechInput * This,
            /* [retval][out] */ BSTR *Language);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_HotKey )( 
            IDaCtlSpeechInput * This,
            /* [retval][out] */ BSTR *HotKey);
        
        /* [hidden][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Installed )( 
            IDaCtlSpeechInput * This,
            /* [retval][out] */ VARIANT_BOOL *VoiceInstalled);
        
        /* [hidden][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Engine )( 
            IDaCtlSpeechInput * This,
            /* [retval][out] */ BSTR *Engine);
        
        /* [hidden][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Engine )( 
            IDaCtlSpeechInput * This,
            /* [in] */ BSTR Engine);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ListeningTip )( 
            IDaCtlSpeechInput * This,
            /* [retval][out] */ VARIANT_BOOL *ListeningTip);
        
        END_INTERFACE
    } IDaCtlSpeechInputVtbl;

    interface IDaCtlSpeechInput
    {
        CONST_VTBL struct IDaCtlSpeechInputVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlSpeechInput_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlSpeechInput_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlSpeechInput_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlSpeechInput_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlSpeechInput_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlSpeechInput_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlSpeechInput_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlSpeechInput_get_Enabled(This,VoiceEnabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,VoiceEnabled) ) 

#define IDaCtlSpeechInput_get_Language(This,Language)	\
    ( (This)->lpVtbl -> get_Language(This,Language) ) 

#define IDaCtlSpeechInput_get_HotKey(This,HotKey)	\
    ( (This)->lpVtbl -> get_HotKey(This,HotKey) ) 

#define IDaCtlSpeechInput_get_Installed(This,VoiceInstalled)	\
    ( (This)->lpVtbl -> get_Installed(This,VoiceInstalled) ) 

#define IDaCtlSpeechInput_get_Engine(This,Engine)	\
    ( (This)->lpVtbl -> get_Engine(This,Engine) ) 

#define IDaCtlSpeechInput_put_Engine(This,Engine)	\
    ( (This)->lpVtbl -> put_Engine(This,Engine) ) 

#define IDaCtlSpeechInput_get_ListeningTip(This,ListeningTip)	\
    ( (This)->lpVtbl -> get_ListeningTip(This,ListeningTip) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlSpeechInput_INTERFACE_DEFINED__ */


#ifndef __IDaCtlPropertySheet_INTERFACE_DEFINED__
#define __IDaCtlPropertySheet_INTERFACE_DEFINED__

/* interface IDaCtlPropertySheet */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaCtlPropertySheet;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E545-A208-11DE-ABF2-002421116FB2")
    IDaCtlPropertySheet : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Left( 
            /* [retval][out] */ short *Left) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Top( 
            /* [retval][out] */ short *Top) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ short *Height) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ short *Width) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL Visible) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *Visible) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Page( 
            /* [in] */ BSTR Page) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Page( 
            /* [retval][out] */ BSTR *Page) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlPropertySheetVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlPropertySheet * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlPropertySheet * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlPropertySheet * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlPropertySheet * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlPropertySheet * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlPropertySheet * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlPropertySheet * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IDaCtlPropertySheet * This,
            /* [retval][out] */ short *Left);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IDaCtlPropertySheet * This,
            /* [retval][out] */ short *Top);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IDaCtlPropertySheet * This,
            /* [retval][out] */ short *Height);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IDaCtlPropertySheet * This,
            /* [retval][out] */ short *Width);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IDaCtlPropertySheet * This,
            /* [in] */ VARIANT_BOOL Visible);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaCtlPropertySheet * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Page )( 
            IDaCtlPropertySheet * This,
            /* [in] */ BSTR Page);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Page )( 
            IDaCtlPropertySheet * This,
            /* [retval][out] */ BSTR *Page);
        
        END_INTERFACE
    } IDaCtlPropertySheetVtbl;

    interface IDaCtlPropertySheet
    {
        CONST_VTBL struct IDaCtlPropertySheetVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlPropertySheet_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlPropertySheet_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlPropertySheet_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlPropertySheet_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlPropertySheet_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlPropertySheet_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlPropertySheet_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlPropertySheet_get_Left(This,Left)	\
    ( (This)->lpVtbl -> get_Left(This,Left) ) 

#define IDaCtlPropertySheet_get_Top(This,Top)	\
    ( (This)->lpVtbl -> get_Top(This,Top) ) 

#define IDaCtlPropertySheet_get_Height(This,Height)	\
    ( (This)->lpVtbl -> get_Height(This,Height) ) 

#define IDaCtlPropertySheet_get_Width(This,Width)	\
    ( (This)->lpVtbl -> get_Width(This,Width) ) 

#define IDaCtlPropertySheet_put_Visible(This,Visible)	\
    ( (This)->lpVtbl -> put_Visible(This,Visible) ) 

#define IDaCtlPropertySheet_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaCtlPropertySheet_put_Page(This,Page)	\
    ( (This)->lpVtbl -> put_Page(This,Page) ) 

#define IDaCtlPropertySheet_get_Page(This,Page)	\
    ( (This)->lpVtbl -> get_Page(This,Page) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlPropertySheet_INTERFACE_DEFINED__ */


#ifndef __IDaCtlUserInput_INTERFACE_DEFINED__
#define __IDaCtlUserInput_INTERFACE_DEFINED__

/* interface IDaCtlUserInput */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaCtlUserInput;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E546-A208-11DE-ABF2-002421116FB2")
    IDaCtlUserInput : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ short *pCount) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pName) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_CharacterID( 
            /* [retval][out] */ BSTR *pCharacterID) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Confidence( 
            /* [retval][out] */ long *pConfidence) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Voice( 
            /* [retval][out] */ BSTR *pVoice) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Alt1Name( 
            /* [retval][out] */ BSTR *pAlt1Name) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Alt1Confidence( 
            /* [retval][out] */ long *pAlt1Confidence) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Alt1Voice( 
            /* [retval][out] */ BSTR *pAlt1Voice) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Alt2Name( 
            /* [retval][out] */ BSTR *pAlt2Name) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Alt2Confidence( 
            /* [retval][out] */ long *pAlt2Confidence) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Alt2Voice( 
            /* [retval][out] */ BSTR *pAlt2Voice) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlUserInputVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlUserInput * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlUserInput * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlUserInput * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlUserInput * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlUserInput * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlUserInput * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlUserInput * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IDaCtlUserInput * This,
            /* [retval][out] */ short *pCount);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IDaCtlUserInput * This,
            /* [retval][out] */ BSTR *pName);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_CharacterID )( 
            IDaCtlUserInput * This,
            /* [retval][out] */ BSTR *pCharacterID);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Confidence )( 
            IDaCtlUserInput * This,
            /* [retval][out] */ long *pConfidence);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Voice )( 
            IDaCtlUserInput * This,
            /* [retval][out] */ BSTR *pVoice);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Alt1Name )( 
            IDaCtlUserInput * This,
            /* [retval][out] */ BSTR *pAlt1Name);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Alt1Confidence )( 
            IDaCtlUserInput * This,
            /* [retval][out] */ long *pAlt1Confidence);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Alt1Voice )( 
            IDaCtlUserInput * This,
            /* [retval][out] */ BSTR *pAlt1Voice);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Alt2Name )( 
            IDaCtlUserInput * This,
            /* [retval][out] */ BSTR *pAlt2Name);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Alt2Confidence )( 
            IDaCtlUserInput * This,
            /* [retval][out] */ long *pAlt2Confidence);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Alt2Voice )( 
            IDaCtlUserInput * This,
            /* [retval][out] */ BSTR *pAlt2Voice);
        
        END_INTERFACE
    } IDaCtlUserInputVtbl;

    interface IDaCtlUserInput
    {
        CONST_VTBL struct IDaCtlUserInputVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlUserInput_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlUserInput_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlUserInput_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlUserInput_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlUserInput_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlUserInput_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlUserInput_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlUserInput_get_Count(This,pCount)	\
    ( (This)->lpVtbl -> get_Count(This,pCount) ) 

#define IDaCtlUserInput_get_Name(This,pName)	\
    ( (This)->lpVtbl -> get_Name(This,pName) ) 

#define IDaCtlUserInput_get_CharacterID(This,pCharacterID)	\
    ( (This)->lpVtbl -> get_CharacterID(This,pCharacterID) ) 

#define IDaCtlUserInput_get_Confidence(This,pConfidence)	\
    ( (This)->lpVtbl -> get_Confidence(This,pConfidence) ) 

#define IDaCtlUserInput_get_Voice(This,pVoice)	\
    ( (This)->lpVtbl -> get_Voice(This,pVoice) ) 

#define IDaCtlUserInput_get_Alt1Name(This,pAlt1Name)	\
    ( (This)->lpVtbl -> get_Alt1Name(This,pAlt1Name) ) 

#define IDaCtlUserInput_get_Alt1Confidence(This,pAlt1Confidence)	\
    ( (This)->lpVtbl -> get_Alt1Confidence(This,pAlt1Confidence) ) 

#define IDaCtlUserInput_get_Alt1Voice(This,pAlt1Voice)	\
    ( (This)->lpVtbl -> get_Alt1Voice(This,pAlt1Voice) ) 

#define IDaCtlUserInput_get_Alt2Name(This,pAlt2Name)	\
    ( (This)->lpVtbl -> get_Alt2Name(This,pAlt2Name) ) 

#define IDaCtlUserInput_get_Alt2Confidence(This,pAlt2Confidence)	\
    ( (This)->lpVtbl -> get_Alt2Confidence(This,pAlt2Confidence) ) 

#define IDaCtlUserInput_get_Alt2Voice(This,pAlt2Voice)	\
    ( (This)->lpVtbl -> get_Alt2Voice(This,pAlt2Voice) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlUserInput_INTERFACE_DEFINED__ */


#ifndef __IDaCtlCommandsWindow_INTERFACE_DEFINED__
#define __IDaCtlCommandsWindow_INTERFACE_DEFINED__

/* interface IDaCtlCommandsWindow */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaCtlCommandsWindow;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E547-A208-11DE-ABF2-002421116FB2")
    IDaCtlCommandsWindow : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *Visible) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL Visible) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Left( 
            /* [retval][out] */ short *Left) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Top( 
            /* [retval][out] */ short *Top) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ short *Height) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ short *Width) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlCommandsWindowVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlCommandsWindow * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlCommandsWindow * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlCommandsWindow * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlCommandsWindow * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlCommandsWindow * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlCommandsWindow * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlCommandsWindow * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaCtlCommandsWindow * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IDaCtlCommandsWindow * This,
            /* [in] */ VARIANT_BOOL Visible);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IDaCtlCommandsWindow * This,
            /* [retval][out] */ short *Left);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IDaCtlCommandsWindow * This,
            /* [retval][out] */ short *Top);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IDaCtlCommandsWindow * This,
            /* [retval][out] */ short *Height);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IDaCtlCommandsWindow * This,
            /* [retval][out] */ short *Width);
        
        END_INTERFACE
    } IDaCtlCommandsWindowVtbl;

    interface IDaCtlCommandsWindow
    {
        CONST_VTBL struct IDaCtlCommandsWindowVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlCommandsWindow_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlCommandsWindow_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlCommandsWindow_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlCommandsWindow_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlCommandsWindow_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlCommandsWindow_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlCommandsWindow_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlCommandsWindow_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaCtlCommandsWindow_put_Visible(This,Visible)	\
    ( (This)->lpVtbl -> put_Visible(This,Visible) ) 

#define IDaCtlCommandsWindow_get_Left(This,Left)	\
    ( (This)->lpVtbl -> get_Left(This,Left) ) 

#define IDaCtlCommandsWindow_get_Top(This,Top)	\
    ( (This)->lpVtbl -> get_Top(This,Top) ) 

#define IDaCtlCommandsWindow_get_Height(This,Height)	\
    ( (This)->lpVtbl -> get_Height(This,Height) ) 

#define IDaCtlCommandsWindow_get_Width(This,Width)	\
    ( (This)->lpVtbl -> get_Width(This,Width) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlCommandsWindow_INTERFACE_DEFINED__ */


#ifndef __IDaCtlAnimationNames_INTERFACE_DEFINED__
#define __IDaCtlAnimationNames_INTERFACE_DEFINED__

/* interface IDaCtlAnimationNames */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaCtlAnimationNames;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E548-A208-11DE-ABF2-002421116FB2")
    IDaCtlAnimationNames : public IDispatch
    {
    public:
        virtual /* [restricted][propget][id] */ HRESULT STDMETHODCALLTYPE get_Enum( 
            /* [retval][out] */ IUnknown **ppunkEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlAnimationNamesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlAnimationNames * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlAnimationNames * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlAnimationNames * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlAnimationNames * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlAnimationNames * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlAnimationNames * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlAnimationNames * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [restricted][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Enum )( 
            IDaCtlAnimationNames * This,
            /* [retval][out] */ IUnknown **ppunkEnum);
        
        END_INTERFACE
    } IDaCtlAnimationNamesVtbl;

    interface IDaCtlAnimationNames
    {
        CONST_VTBL struct IDaCtlAnimationNamesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlAnimationNames_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlAnimationNames_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlAnimationNames_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlAnimationNames_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlAnimationNames_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlAnimationNames_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlAnimationNames_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlAnimationNames_get_Enum(This,ppunkEnum)	\
    ( (This)->lpVtbl -> get_Enum(This,ppunkEnum) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlAnimationNames_INTERFACE_DEFINED__ */


#ifndef __IDaControl_INTERFACE_DEFINED__
#define __IDaControl_INTERFACE_DEFINED__

/* interface IDaControl */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E549-A208-11DE-ABF2-002421116FB2")
    IDaControl : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Characters( 
            /* [retval][out] */ IDaCtlCharacters **Characters) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_AudioOutput( 
            /* [retval][out] */ IDaCtlAudioObject **AudioOutput) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_SpeechInput( 
            /* [retval][out] */ IDaCtlSpeechInput **SpeechInput) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_PropertySheet( 
            /* [retval][out] */ IDaCtlPropertySheet **PropSheet) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_CommandsWindow( 
            /* [retval][out] */ IDaCtlCommandsWindow **CommandsWindow) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Connected( 
            /* [retval][out] */ VARIANT_BOOL *Connected) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Connected( 
            VARIANT_BOOL Connected) = 0;
        
        virtual /* [hidden][propget][id] */ HRESULT STDMETHODCALLTYPE get_Suspended( 
            /* [retval][out] */ VARIANT_BOOL *Suspended) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ShowDefaultCharacterProperties( 
            /* [optional][in] */ VARIANT x,
            /* [optional][in] */ VARIANT y) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_RaiseRequestErrors( 
            /* [retval][out] */ VARIANT_BOOL *RaiseErrors) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_RaiseRequestErrors( 
            /* [in] */ VARIANT_BOOL RaiseErrors) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaControl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaControl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaControl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaControl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaControl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaControl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaControl * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Characters )( 
            IDaControl * This,
            /* [retval][out] */ IDaCtlCharacters **Characters);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_AudioOutput )( 
            IDaControl * This,
            /* [retval][out] */ IDaCtlAudioObject **AudioOutput);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_SpeechInput )( 
            IDaControl * This,
            /* [retval][out] */ IDaCtlSpeechInput **SpeechInput);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_PropertySheet )( 
            IDaControl * This,
            /* [retval][out] */ IDaCtlPropertySheet **PropSheet);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_CommandsWindow )( 
            IDaControl * This,
            /* [retval][out] */ IDaCtlCommandsWindow **CommandsWindow);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Connected )( 
            IDaControl * This,
            /* [retval][out] */ VARIANT_BOOL *Connected);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Connected )( 
            IDaControl * This,
            VARIANT_BOOL Connected);
        
        /* [hidden][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Suspended )( 
            IDaControl * This,
            /* [retval][out] */ VARIANT_BOOL *Suspended);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ShowDefaultCharacterProperties )( 
            IDaControl * This,
            /* [optional][in] */ VARIANT x,
            /* [optional][in] */ VARIANT y);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_RaiseRequestErrors )( 
            IDaControl * This,
            /* [retval][out] */ VARIANT_BOOL *RaiseErrors);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_RaiseRequestErrors )( 
            IDaControl * This,
            /* [in] */ VARIANT_BOOL RaiseErrors);
        
        END_INTERFACE
    } IDaControlVtbl;

    interface IDaControl
    {
        CONST_VTBL struct IDaControlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaControl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaControl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaControl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaControl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaControl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaControl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaControl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaControl_get_Characters(This,Characters)	\
    ( (This)->lpVtbl -> get_Characters(This,Characters) ) 

#define IDaControl_get_AudioOutput(This,AudioOutput)	\
    ( (This)->lpVtbl -> get_AudioOutput(This,AudioOutput) ) 

#define IDaControl_get_SpeechInput(This,SpeechInput)	\
    ( (This)->lpVtbl -> get_SpeechInput(This,SpeechInput) ) 

#define IDaControl_get_PropertySheet(This,PropSheet)	\
    ( (This)->lpVtbl -> get_PropertySheet(This,PropSheet) ) 

#define IDaControl_get_CommandsWindow(This,CommandsWindow)	\
    ( (This)->lpVtbl -> get_CommandsWindow(This,CommandsWindow) ) 

#define IDaControl_get_Connected(This,Connected)	\
    ( (This)->lpVtbl -> get_Connected(This,Connected) ) 

#define IDaControl_put_Connected(This,Connected)	\
    ( (This)->lpVtbl -> put_Connected(This,Connected) ) 

#define IDaControl_get_Suspended(This,Suspended)	\
    ( (This)->lpVtbl -> get_Suspended(This,Suspended) ) 

#define IDaControl_ShowDefaultCharacterProperties(This,x,y)	\
    ( (This)->lpVtbl -> ShowDefaultCharacterProperties(This,x,y) ) 

#define IDaControl_get_RaiseRequestErrors(This,RaiseErrors)	\
    ( (This)->lpVtbl -> get_RaiseRequestErrors(This,RaiseErrors) ) 

#define IDaControl_put_RaiseRequestErrors(This,RaiseErrors)	\
    ( (This)->lpVtbl -> put_RaiseRequestErrors(This,RaiseErrors) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaControl_INTERFACE_DEFINED__ */


#ifndef ___DaCtlEvents_DISPINTERFACE_DEFINED__
#define ___DaCtlEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DaCtlEvents */
/* [hidden][helpstring][uuid] */ 


EXTERN_C const IID DIID__DaCtlEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("1147E54A-A208-11DE-ABF2-002421116FB2")
    _DaCtlEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DaCtlEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DaCtlEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DaCtlEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DaCtlEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DaCtlEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DaCtlEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DaCtlEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DaCtlEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DaCtlEventsVtbl;

    interface _DaCtlEvents
    {
        CONST_VTBL struct _DaCtlEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DaCtlEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DaCtlEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DaCtlEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DaCtlEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DaCtlEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DaCtlEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DaCtlEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DaCtlEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_DaControl;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E530-A208-11DE-ABF2-002421116FB2")
DaControl;
#endif

EXTERN_C const CLSID CLSID_DaCtlCharacters;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E531-A208-11DE-ABF2-002421116FB2")
DaCtlCharacters;
#endif

EXTERN_C const CLSID CLSID_DaCtlCharacter;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E532-A208-11DE-ABF2-002421116FB2")
DaCtlCharacter;
#endif

EXTERN_C const CLSID CLSID_DaCtlBalloon;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E533-A208-11DE-ABF2-002421116FB2")
DaCtlBalloon;
#endif

EXTERN_C const CLSID CLSID_DaCtlCommands;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E534-A208-11DE-ABF2-002421116FB2")
DaCtlCommands;
#endif

EXTERN_C const CLSID CLSID_DaCtlCommand;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E535-A208-11DE-ABF2-002421116FB2")
DaCtlCommand;
#endif

EXTERN_C const CLSID CLSID_DaCtlRequest;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E536-A208-11DE-ABF2-002421116FB2")
DaCtlRequest;
#endif

EXTERN_C const CLSID CLSID_DaCtlAudioOutput;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E537-A208-11DE-ABF2-002421116FB2")
DaCtlAudioOutput;
#endif

EXTERN_C const CLSID CLSID_DaCtlSpeechInput;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E538-A208-11DE-ABF2-002421116FB2")
DaCtlSpeechInput;
#endif

EXTERN_C const CLSID CLSID_DaCtlPropertySheet;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E539-A208-11DE-ABF2-002421116FB2")
DaCtlPropertySheet;
#endif

EXTERN_C const CLSID CLSID_DaCtlUserInput;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E53A-A208-11DE-ABF2-002421116FB2")
DaCtlUserInput;
#endif

EXTERN_C const CLSID CLSID_DaCtlCommandsWindow;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E53B-A208-11DE-ABF2-002421116FB2")
DaCtlCommandsWindow;
#endif

EXTERN_C const CLSID CLSID_DaCtlAnimationNames;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E54B-A208-11DE-ABF2-002421116FB2")
DaCtlAnimationNames;
#endif
#endif /* __DoubleAgentCtl_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


