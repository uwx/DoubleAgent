

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sat Feb 20 13:59:34 2010
 */
/* Compiler settings for .\Core\DaCore.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
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

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __DaCoreOdl_h__
#define __DaCoreOdl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDaPreview_FWD_DEFINED__
#define __IDaPreview_FWD_DEFINED__
typedef interface IDaPreview IDaPreview;
#endif 	/* __IDaPreview_FWD_DEFINED__ */


#ifndef __DaPreview_FWD_DEFINED__
#define __DaPreview_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaPreview DaPreview;
#else
typedef struct DaPreview DaPreview;
#endif /* __cplusplus */

#endif 	/* __DaPreview_FWD_DEFINED__ */


#ifndef ___IAgentStreamInfo_FWD_DEFINED__
#define ___IAgentStreamInfo_FWD_DEFINED__
typedef interface _IAgentStreamInfo _IAgentStreamInfo;
#endif 	/* ___IAgentStreamInfo_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IDaPreview_INTERFACE_DEFINED__
#define __IDaPreview_INTERFACE_DEFINED__

/* interface IDaPreview */
/* [object][uuid] */ 


EXTERN_C const IID IID_IDaPreview;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E560-A208-11DE-ABF2-002421116FB2")
    IDaPreview : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE PlayAnimation( 
            /* [in] */ BSTR pAnimationName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PlayState( 
            /* [in] */ BSTR pStateName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIsPlaying( 
            /* [out] */ boolean *pIsPlaying) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSoundsEnabled( 
            /* [out] */ boolean *pEnabled) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnableSounds( 
            /* [in] */ boolean pEnabled) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIdleEnabled( 
            /* [out] */ boolean *pEnabled) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnableIdle( 
            /* [in] */ boolean pEnabled) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetBkColor( 
            /* [in] */ COLORREF pBkColor) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFrameSize( 
            /* [out] */ SIZE *pFrameSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCharacterFrameSize( 
            /* [in] */ BSTR pCharacterPath,
            /* [out] */ SIZE *pFrameSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFrameFormat( 
            /* [out] */ BYTE **pFrameFormat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCharacterFrameFormat( 
            /* [in] */ BSTR pCharacterPath,
            /* [out] */ BYTE **pFrameFormat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnActivateApp( 
            /* [in] */ boolean pActivated) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RenderFrame( 
            /* [in] */ HDC pDC,
            /* [in] */ POINT *pPosition) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RenderAnimationFrame( 
            /* [in] */ BSTR pAnimationName,
            /* [in] */ USHORT pFrameNum,
            /* [in] */ HDC pDC,
            /* [in] */ POINT *pPosition) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RenderCharacterFrame( 
            /* [in] */ BSTR pCharacterPath,
            /* [in] */ BSTR pAnimationName,
            /* [in] */ USHORT pFrameNum,
            /* [in] */ HDC pDC,
            /* [in] */ POINT *pPosition) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Open( 
            /* [in] */ BSTR pCharacterPath,
            /* [in] */ HWND pParentWnd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCharacterPath( 
            /* [out] */ BSTR *pCharacterPath) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCharacterName( 
            /* [out] */ BSTR *pCharacterName,
            /* [defaultvalue][in] */ USHORT pLangID = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCharacterDescription( 
            /* [out] */ BSTR *pCharacterDescription,
            /* [defaultvalue][in] */ USHORT pLangID = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCharacterGuid( 
            /* [out] */ GUID *pGuid) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaPreviewVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaPreview * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaPreview * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaPreview * This);
        
        HRESULT ( STDMETHODCALLTYPE *PlayAnimation )( 
            IDaPreview * This,
            /* [in] */ BSTR pAnimationName);
        
        HRESULT ( STDMETHODCALLTYPE *PlayState )( 
            IDaPreview * This,
            /* [in] */ BSTR pStateName);
        
        HRESULT ( STDMETHODCALLTYPE *GetIsPlaying )( 
            IDaPreview * This,
            /* [out] */ boolean *pIsPlaying);
        
        HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IDaPreview * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetSoundsEnabled )( 
            IDaPreview * This,
            /* [out] */ boolean *pEnabled);
        
        HRESULT ( STDMETHODCALLTYPE *EnableSounds )( 
            IDaPreview * This,
            /* [in] */ boolean pEnabled);
        
        HRESULT ( STDMETHODCALLTYPE *GetIdleEnabled )( 
            IDaPreview * This,
            /* [out] */ boolean *pEnabled);
        
        HRESULT ( STDMETHODCALLTYPE *EnableIdle )( 
            IDaPreview * This,
            /* [in] */ boolean pEnabled);
        
        HRESULT ( STDMETHODCALLTYPE *SetBkColor )( 
            IDaPreview * This,
            /* [in] */ COLORREF pBkColor);
        
        HRESULT ( STDMETHODCALLTYPE *GetFrameSize )( 
            IDaPreview * This,
            /* [out] */ SIZE *pFrameSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetCharacterFrameSize )( 
            IDaPreview * This,
            /* [in] */ BSTR pCharacterPath,
            /* [out] */ SIZE *pFrameSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetFrameFormat )( 
            IDaPreview * This,
            /* [out] */ BYTE **pFrameFormat);
        
        HRESULT ( STDMETHODCALLTYPE *GetCharacterFrameFormat )( 
            IDaPreview * This,
            /* [in] */ BSTR pCharacterPath,
            /* [out] */ BYTE **pFrameFormat);
        
        HRESULT ( STDMETHODCALLTYPE *OnActivateApp )( 
            IDaPreview * This,
            /* [in] */ boolean pActivated);
        
        HRESULT ( STDMETHODCALLTYPE *RenderFrame )( 
            IDaPreview * This,
            /* [in] */ HDC pDC,
            /* [in] */ POINT *pPosition);
        
        HRESULT ( STDMETHODCALLTYPE *RenderAnimationFrame )( 
            IDaPreview * This,
            /* [in] */ BSTR pAnimationName,
            /* [in] */ USHORT pFrameNum,
            /* [in] */ HDC pDC,
            /* [in] */ POINT *pPosition);
        
        HRESULT ( STDMETHODCALLTYPE *RenderCharacterFrame )( 
            IDaPreview * This,
            /* [in] */ BSTR pCharacterPath,
            /* [in] */ BSTR pAnimationName,
            /* [in] */ USHORT pFrameNum,
            /* [in] */ HDC pDC,
            /* [in] */ POINT *pPosition);
        
        HRESULT ( STDMETHODCALLTYPE *Open )( 
            IDaPreview * This,
            /* [in] */ BSTR pCharacterPath,
            /* [in] */ HWND pParentWnd);
        
        HRESULT ( STDMETHODCALLTYPE *GetCharacterPath )( 
            IDaPreview * This,
            /* [out] */ BSTR *pCharacterPath);
        
        HRESULT ( STDMETHODCALLTYPE *GetCharacterName )( 
            IDaPreview * This,
            /* [out] */ BSTR *pCharacterName,
            /* [defaultvalue][in] */ USHORT pLangID);
        
        HRESULT ( STDMETHODCALLTYPE *GetCharacterDescription )( 
            IDaPreview * This,
            /* [out] */ BSTR *pCharacterDescription,
            /* [defaultvalue][in] */ USHORT pLangID);
        
        HRESULT ( STDMETHODCALLTYPE *GetCharacterGuid )( 
            IDaPreview * This,
            /* [out] */ GUID *pGuid);
        
        END_INTERFACE
    } IDaPreviewVtbl;

    interface IDaPreview
    {
        CONST_VTBL struct IDaPreviewVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaPreview_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaPreview_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaPreview_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaPreview_PlayAnimation(This,pAnimationName)	\
    ( (This)->lpVtbl -> PlayAnimation(This,pAnimationName) ) 

#define IDaPreview_PlayState(This,pStateName)	\
    ( (This)->lpVtbl -> PlayState(This,pStateName) ) 

#define IDaPreview_GetIsPlaying(This,pIsPlaying)	\
    ( (This)->lpVtbl -> GetIsPlaying(This,pIsPlaying) ) 

#define IDaPreview_Stop(This)	\
    ( (This)->lpVtbl -> Stop(This) ) 

#define IDaPreview_GetSoundsEnabled(This,pEnabled)	\
    ( (This)->lpVtbl -> GetSoundsEnabled(This,pEnabled) ) 

#define IDaPreview_EnableSounds(This,pEnabled)	\
    ( (This)->lpVtbl -> EnableSounds(This,pEnabled) ) 

#define IDaPreview_GetIdleEnabled(This,pEnabled)	\
    ( (This)->lpVtbl -> GetIdleEnabled(This,pEnabled) ) 

#define IDaPreview_EnableIdle(This,pEnabled)	\
    ( (This)->lpVtbl -> EnableIdle(This,pEnabled) ) 

#define IDaPreview_SetBkColor(This,pBkColor)	\
    ( (This)->lpVtbl -> SetBkColor(This,pBkColor) ) 

#define IDaPreview_GetFrameSize(This,pFrameSize)	\
    ( (This)->lpVtbl -> GetFrameSize(This,pFrameSize) ) 

#define IDaPreview_GetCharacterFrameSize(This,pCharacterPath,pFrameSize)	\
    ( (This)->lpVtbl -> GetCharacterFrameSize(This,pCharacterPath,pFrameSize) ) 

#define IDaPreview_GetFrameFormat(This,pFrameFormat)	\
    ( (This)->lpVtbl -> GetFrameFormat(This,pFrameFormat) ) 

#define IDaPreview_GetCharacterFrameFormat(This,pCharacterPath,pFrameFormat)	\
    ( (This)->lpVtbl -> GetCharacterFrameFormat(This,pCharacterPath,pFrameFormat) ) 

#define IDaPreview_OnActivateApp(This,pActivated)	\
    ( (This)->lpVtbl -> OnActivateApp(This,pActivated) ) 

#define IDaPreview_RenderFrame(This,pDC,pPosition)	\
    ( (This)->lpVtbl -> RenderFrame(This,pDC,pPosition) ) 

#define IDaPreview_RenderAnimationFrame(This,pAnimationName,pFrameNum,pDC,pPosition)	\
    ( (This)->lpVtbl -> RenderAnimationFrame(This,pAnimationName,pFrameNum,pDC,pPosition) ) 

#define IDaPreview_RenderCharacterFrame(This,pCharacterPath,pAnimationName,pFrameNum,pDC,pPosition)	\
    ( (This)->lpVtbl -> RenderCharacterFrame(This,pCharacterPath,pAnimationName,pFrameNum,pDC,pPosition) ) 

#define IDaPreview_Open(This,pCharacterPath,pParentWnd)	\
    ( (This)->lpVtbl -> Open(This,pCharacterPath,pParentWnd) ) 

#define IDaPreview_GetCharacterPath(This,pCharacterPath)	\
    ( (This)->lpVtbl -> GetCharacterPath(This,pCharacterPath) ) 

#define IDaPreview_GetCharacterName(This,pCharacterName,pLangID)	\
    ( (This)->lpVtbl -> GetCharacterName(This,pCharacterName,pLangID) ) 

#define IDaPreview_GetCharacterDescription(This,pCharacterDescription,pLangID)	\
    ( (This)->lpVtbl -> GetCharacterDescription(This,pCharacterDescription,pLangID) ) 

#define IDaPreview_GetCharacterGuid(This,pGuid)	\
    ( (This)->lpVtbl -> GetCharacterGuid(This,pGuid) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaPreview_INTERFACE_DEFINED__ */


#ifndef ___IAgentStreamInfo_INTERFACE_DEFINED__
#define ___IAgentStreamInfo_INTERFACE_DEFINED__

/* interface _IAgentStreamInfo */
/* [hidden][object][uuid] */ 


EXTERN_C const IID IID__IAgentStreamInfo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E562-A208-11DE-ABF2-002421116FB2")
    _IAgentStreamInfo : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetAnimationIndex( 
            /* [out] */ long *pAnimationNdx) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetAnimationIndex( 
            /* [in] */ long pAnimationNdx) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAnimationName( 
            /* [out] */ BSTR *pAnimationName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetAnimationName( 
            /* [in] */ BSTR pAnimationName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAnimationSource( 
            /* [out] */ BSTR *pAnimationSource) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetAnimationSource( 
            /* [in] */ BSTR pAnimationSource) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CalcAnimationFrameCount( 
            /* [out] */ long *pAnimationFrameCount,
            /* [in] */ long pAnimationNdx) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CalcAnimationDuration( 
            /* [out] */ long *pAnimationDuration,
            /* [in] */ long pAnimationNdx) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FindAnimationSpeakingFrame( 
            /* [out] */ long *pSpeakingFrameNdx,
            /* [in] */ long pAnimationNdx,
            /* [defaultvalue][in] */ long pStartFrameNdx = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE NewAnimationSequence( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EndAnimationSequence( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ClearAnimationSequences( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSequenceFrameCount( 
            /* [out] */ long *pSequenceFrameCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSequenceDuration( 
            /* [out] */ long *pSequenceDuration) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSequenceLoop( 
            /* [out] */ long *pLoopDuration) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CalcSequenceTimeNdx( 
            /* [out] */ long *pTimeNdx,
            /* [in] */ long pFrameNum,
            /* [defaultvalue][in] */ boolean pClampFrameNum = FALSE) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CalcSequenceFrameNum( 
            /* [out] */ long *pFrameNum,
            /* [in] */ long pTimeNdx,
            /* [defaultvalue][in] */ boolean pClampTimeNdx = FALSE) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CalcSequenceAnimationFrameNdx( 
            /* [out] */ long *pAnimationNdx,
            /* [out] */ long *pFrameNdx,
            /* [in] */ long pTimeNdx,
            /* [defaultvalue][in] */ boolean pClampTimeNdx = FALSE) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SequenceAll( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SequenceAnimation( 
            /* [in] */ long pAnimationNdx,
            /* [defaultvalue][in] */ long pMaxLoopTime = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SequenceAnimationLoop( 
            /* [in] */ long pAnimationNdx,
            /* [defaultvalue][in] */ long pMaxLoopTime = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SequenceAnimationExit( 
            /* [in] */ long pAnimationNdx,
            /* [in] */ long pLastFrameNdx) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CueSequenceAudio( 
            /* [defaultvalue][in] */ long pStartFrameNum = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ClearSequenceAudio( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct _IAgentStreamInfoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IAgentStreamInfo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IAgentStreamInfo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IAgentStreamInfo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetAnimationIndex )( 
            _IAgentStreamInfo * This,
            /* [out] */ long *pAnimationNdx);
        
        HRESULT ( STDMETHODCALLTYPE *SetAnimationIndex )( 
            _IAgentStreamInfo * This,
            /* [in] */ long pAnimationNdx);
        
        HRESULT ( STDMETHODCALLTYPE *GetAnimationName )( 
            _IAgentStreamInfo * This,
            /* [out] */ BSTR *pAnimationName);
        
        HRESULT ( STDMETHODCALLTYPE *SetAnimationName )( 
            _IAgentStreamInfo * This,
            /* [in] */ BSTR pAnimationName);
        
        HRESULT ( STDMETHODCALLTYPE *GetAnimationSource )( 
            _IAgentStreamInfo * This,
            /* [out] */ BSTR *pAnimationSource);
        
        HRESULT ( STDMETHODCALLTYPE *SetAnimationSource )( 
            _IAgentStreamInfo * This,
            /* [in] */ BSTR pAnimationSource);
        
        HRESULT ( STDMETHODCALLTYPE *CalcAnimationFrameCount )( 
            _IAgentStreamInfo * This,
            /* [out] */ long *pAnimationFrameCount,
            /* [in] */ long pAnimationNdx);
        
        HRESULT ( STDMETHODCALLTYPE *CalcAnimationDuration )( 
            _IAgentStreamInfo * This,
            /* [out] */ long *pAnimationDuration,
            /* [in] */ long pAnimationNdx);
        
        HRESULT ( STDMETHODCALLTYPE *FindAnimationSpeakingFrame )( 
            _IAgentStreamInfo * This,
            /* [out] */ long *pSpeakingFrameNdx,
            /* [in] */ long pAnimationNdx,
            /* [defaultvalue][in] */ long pStartFrameNdx);
        
        HRESULT ( STDMETHODCALLTYPE *NewAnimationSequence )( 
            _IAgentStreamInfo * This);
        
        HRESULT ( STDMETHODCALLTYPE *EndAnimationSequence )( 
            _IAgentStreamInfo * This);
        
        HRESULT ( STDMETHODCALLTYPE *ClearAnimationSequences )( 
            _IAgentStreamInfo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetSequenceFrameCount )( 
            _IAgentStreamInfo * This,
            /* [out] */ long *pSequenceFrameCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetSequenceDuration )( 
            _IAgentStreamInfo * This,
            /* [out] */ long *pSequenceDuration);
        
        HRESULT ( STDMETHODCALLTYPE *GetSequenceLoop )( 
            _IAgentStreamInfo * This,
            /* [out] */ long *pLoopDuration);
        
        HRESULT ( STDMETHODCALLTYPE *CalcSequenceTimeNdx )( 
            _IAgentStreamInfo * This,
            /* [out] */ long *pTimeNdx,
            /* [in] */ long pFrameNum,
            /* [defaultvalue][in] */ boolean pClampFrameNum);
        
        HRESULT ( STDMETHODCALLTYPE *CalcSequenceFrameNum )( 
            _IAgentStreamInfo * This,
            /* [out] */ long *pFrameNum,
            /* [in] */ long pTimeNdx,
            /* [defaultvalue][in] */ boolean pClampTimeNdx);
        
        HRESULT ( STDMETHODCALLTYPE *CalcSequenceAnimationFrameNdx )( 
            _IAgentStreamInfo * This,
            /* [out] */ long *pAnimationNdx,
            /* [out] */ long *pFrameNdx,
            /* [in] */ long pTimeNdx,
            /* [defaultvalue][in] */ boolean pClampTimeNdx);
        
        HRESULT ( STDMETHODCALLTYPE *SequenceAll )( 
            _IAgentStreamInfo * This);
        
        HRESULT ( STDMETHODCALLTYPE *SequenceAnimation )( 
            _IAgentStreamInfo * This,
            /* [in] */ long pAnimationNdx,
            /* [defaultvalue][in] */ long pMaxLoopTime);
        
        HRESULT ( STDMETHODCALLTYPE *SequenceAnimationLoop )( 
            _IAgentStreamInfo * This,
            /* [in] */ long pAnimationNdx,
            /* [defaultvalue][in] */ long pMaxLoopTime);
        
        HRESULT ( STDMETHODCALLTYPE *SequenceAnimationExit )( 
            _IAgentStreamInfo * This,
            /* [in] */ long pAnimationNdx,
            /* [in] */ long pLastFrameNdx);
        
        HRESULT ( STDMETHODCALLTYPE *CueSequenceAudio )( 
            _IAgentStreamInfo * This,
            /* [defaultvalue][in] */ long pStartFrameNum);
        
        HRESULT ( STDMETHODCALLTYPE *ClearSequenceAudio )( 
            _IAgentStreamInfo * This);
        
        END_INTERFACE
    } _IAgentStreamInfoVtbl;

    interface _IAgentStreamInfo
    {
        CONST_VTBL struct _IAgentStreamInfoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IAgentStreamInfo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IAgentStreamInfo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IAgentStreamInfo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IAgentStreamInfo_GetAnimationIndex(This,pAnimationNdx)	\
    ( (This)->lpVtbl -> GetAnimationIndex(This,pAnimationNdx) ) 

#define _IAgentStreamInfo_SetAnimationIndex(This,pAnimationNdx)	\
    ( (This)->lpVtbl -> SetAnimationIndex(This,pAnimationNdx) ) 

#define _IAgentStreamInfo_GetAnimationName(This,pAnimationName)	\
    ( (This)->lpVtbl -> GetAnimationName(This,pAnimationName) ) 

#define _IAgentStreamInfo_SetAnimationName(This,pAnimationName)	\
    ( (This)->lpVtbl -> SetAnimationName(This,pAnimationName) ) 

#define _IAgentStreamInfo_GetAnimationSource(This,pAnimationSource)	\
    ( (This)->lpVtbl -> GetAnimationSource(This,pAnimationSource) ) 

#define _IAgentStreamInfo_SetAnimationSource(This,pAnimationSource)	\
    ( (This)->lpVtbl -> SetAnimationSource(This,pAnimationSource) ) 

#define _IAgentStreamInfo_CalcAnimationFrameCount(This,pAnimationFrameCount,pAnimationNdx)	\
    ( (This)->lpVtbl -> CalcAnimationFrameCount(This,pAnimationFrameCount,pAnimationNdx) ) 

#define _IAgentStreamInfo_CalcAnimationDuration(This,pAnimationDuration,pAnimationNdx)	\
    ( (This)->lpVtbl -> CalcAnimationDuration(This,pAnimationDuration,pAnimationNdx) ) 

#define _IAgentStreamInfo_FindAnimationSpeakingFrame(This,pSpeakingFrameNdx,pAnimationNdx,pStartFrameNdx)	\
    ( (This)->lpVtbl -> FindAnimationSpeakingFrame(This,pSpeakingFrameNdx,pAnimationNdx,pStartFrameNdx) ) 

#define _IAgentStreamInfo_NewAnimationSequence(This)	\
    ( (This)->lpVtbl -> NewAnimationSequence(This) ) 

#define _IAgentStreamInfo_EndAnimationSequence(This)	\
    ( (This)->lpVtbl -> EndAnimationSequence(This) ) 

#define _IAgentStreamInfo_ClearAnimationSequences(This)	\
    ( (This)->lpVtbl -> ClearAnimationSequences(This) ) 

#define _IAgentStreamInfo_GetSequenceFrameCount(This,pSequenceFrameCount)	\
    ( (This)->lpVtbl -> GetSequenceFrameCount(This,pSequenceFrameCount) ) 

#define _IAgentStreamInfo_GetSequenceDuration(This,pSequenceDuration)	\
    ( (This)->lpVtbl -> GetSequenceDuration(This,pSequenceDuration) ) 

#define _IAgentStreamInfo_GetSequenceLoop(This,pLoopDuration)	\
    ( (This)->lpVtbl -> GetSequenceLoop(This,pLoopDuration) ) 

#define _IAgentStreamInfo_CalcSequenceTimeNdx(This,pTimeNdx,pFrameNum,pClampFrameNum)	\
    ( (This)->lpVtbl -> CalcSequenceTimeNdx(This,pTimeNdx,pFrameNum,pClampFrameNum) ) 

#define _IAgentStreamInfo_CalcSequenceFrameNum(This,pFrameNum,pTimeNdx,pClampTimeNdx)	\
    ( (This)->lpVtbl -> CalcSequenceFrameNum(This,pFrameNum,pTimeNdx,pClampTimeNdx) ) 

#define _IAgentStreamInfo_CalcSequenceAnimationFrameNdx(This,pAnimationNdx,pFrameNdx,pTimeNdx,pClampTimeNdx)	\
    ( (This)->lpVtbl -> CalcSequenceAnimationFrameNdx(This,pAnimationNdx,pFrameNdx,pTimeNdx,pClampTimeNdx) ) 

#define _IAgentStreamInfo_SequenceAll(This)	\
    ( (This)->lpVtbl -> SequenceAll(This) ) 

#define _IAgentStreamInfo_SequenceAnimation(This,pAnimationNdx,pMaxLoopTime)	\
    ( (This)->lpVtbl -> SequenceAnimation(This,pAnimationNdx,pMaxLoopTime) ) 

#define _IAgentStreamInfo_SequenceAnimationLoop(This,pAnimationNdx,pMaxLoopTime)	\
    ( (This)->lpVtbl -> SequenceAnimationLoop(This,pAnimationNdx,pMaxLoopTime) ) 

#define _IAgentStreamInfo_SequenceAnimationExit(This,pAnimationNdx,pLastFrameNdx)	\
    ( (This)->lpVtbl -> SequenceAnimationExit(This,pAnimationNdx,pLastFrameNdx) ) 

#define _IAgentStreamInfo_CueSequenceAudio(This,pStartFrameNum)	\
    ( (This)->lpVtbl -> CueSequenceAudio(This,pStartFrameNum) ) 

#define _IAgentStreamInfo_ClearSequenceAudio(This)	\
    ( (This)->lpVtbl -> ClearSequenceAudio(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ___IAgentStreamInfo_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  HDC_UserSize(     unsigned long *, unsigned long            , HDC * ); 
unsigned char * __RPC_USER  HDC_UserMarshal(  unsigned long *, unsigned char *, HDC * ); 
unsigned char * __RPC_USER  HDC_UserUnmarshal(unsigned long *, unsigned char *, HDC * ); 
void                      __RPC_USER  HDC_UserFree(     unsigned long *, HDC * ); 

unsigned long             __RPC_USER  HWND_UserSize(     unsigned long *, unsigned long            , HWND * ); 
unsigned char * __RPC_USER  HWND_UserMarshal(  unsigned long *, unsigned char *, HWND * ); 
unsigned char * __RPC_USER  HWND_UserUnmarshal(unsigned long *, unsigned char *, HWND * ); 
void                      __RPC_USER  HWND_UserFree(     unsigned long *, HWND * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


