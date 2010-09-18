/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent Server.

    The Double Agent Server is free software:
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser Public License for more details.

    You should have received a copy of the GNU Lesser Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DaSvrCommands.h"
#include "AgentPopupWnd.h"
#include "VoiceCommandsWnd.h"
#include "Sapi5Input.h"
#include "Sapi5Err.h"
#include "Localize.h"
#ifdef	_DEBUG
#include "GuidStr.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogVerbose,true)&0xFFFF)
#define	_LOG_ABANDONED		MinLogLevel(GetProfileDebugInt(_T("LogAbandoned"),LogDetails,true)&0xFFFF,_LOG_INSTANCE)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrCommands::DaSvrCommands ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)(%d)] DaSvrCommands::DaSvrCommands (%d)"), this, mCharID, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
}

DaSvrCommands::~DaSvrCommands ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)(%d)] DaSvrCommands::~DaSvrCommands (%d)"), this, mCharID, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrCommands * DaSvrCommands::CreateInstance (long pCharID, CEventNotify * pNotify, LPCTSTR pClientMutexName)
{
	CComObject<DaSvrCommands> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrCommands>::CreateInstance (&lInstance))))
	{
		lInstance->Initialize (pCharID, pNotify);
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrCommands::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
		DaSvrCommand *	lCommand;
		INT_PTR			lNdx;

		SafeFreeSafePtr (mCachedEnum);

		if	(pFinal)
		{
			UnmanageObjectLifetime (this);
		}

		if	(
				(pFinal)
			&&	(m_dwRef > 0)
			)
		{
			if	(!pAbandonned)
			{
				try
				{
					CoDisconnectObject (GetUnknown(), 0);
				}
				catch AnyExceptionDebug
			}
			m_dwRef = 0;
		}

		try
		{
			for	(lNdx = mCommands.GetCount()-1; lNdx >= 0; lNdx--)
			{
				lCommand = dynamic_cast <DaSvrCommand*> (mCommands (lNdx));
				if	(pFinal)
				{
					mCommands.RemoveAt (lNdx);
				}
				if	(lCommand)
				{
					lCommand->Terminate (pFinal, pAbandonned);
					if	(pFinal)
					{
						try
						{
							delete lCommand;
						}
						catch AnyExceptionSilent
					}
				}
			}
		}
		catch AnyExceptionSilent

		try
		{
			for	(lNdx = mRemoved.GetCount()-1; lNdx >= 0; lNdx--)
			{
				lCommand = dynamic_cast <DaSvrCommand*> (mRemoved (lNdx));
				mCommands.RemoveAt (lNdx);
				if	(lCommand)
				{
					lCommand->Terminate (pFinal, pAbandonned);
					try
					{
						delete lCommand;
					}
					catch AnyExceptionSilent
				}
			}
		}
		catch AnyExceptionSilent
	}
}

void DaSvrCommands::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)(%d)] DaSvrCommands::FinalRelease"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	Terminate (false);
}

void DaSvrCommands::OnClientEnded()
{
#ifdef	_LOG_ABANDONED
	if	(LogIsActive (_LOG_ABANDONED))
	{
		LogMessage (_LOG_ABANDONED, _T("[%p(%d)(%d)] DaSvrCommands::OnClientEnded"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	Terminate (false, true);
	Terminate (true, true);
	try
	{
		delete this;
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaSvrCommands::InitEnumVariant (CEnumVARIANTImpl * pEnum)
{
	HRESULT					lResult = S_FALSE;
	tArrayPtr <CComVariant>	lArray;
	INT_PTR					lNdx;

	if	(lArray = new CComVariant [mCommands.GetCount()+1])
	{
		for	(lNdx = 0; lNdx < (INT_PTR)mCommands.GetCount(); lNdx++)
		{
			lArray [lNdx] = (LPDISPATCH) dynamic_cast <DaSvrCommand*> (mCommands [lNdx]);
		}
		lResult = pEnum->Init (&(lArray[0]), &(lArray[(INT_PTR)mCommands.GetCount()]), (LPDISPATCH)this, AtlFlagCopy);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaSvrCommands::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaSvrCommands2), riid))
		||	(InlineIsEqualGUID (__uuidof(IDaSvrCommands), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCommands), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCommandsEx), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long DaSvrCommands::GetCharID () const
{
	return mCharID;
}

LANGID DaSvrCommands::GetLangID () const
{
	return mLangID;
}

/////////////////////////////////////////////////////////////////////////////

//DaSvrCommand * DaSvrCommands::GetDefaultCommand ()
//{
//	if	(mDefaultId)
//	{
//		return mCommands (FindCommand (mDefaultId));
//	}
//	return NULL;
//}
//
//DaSvrCommand * DaSvrCommands::GetCommand (USHORT pCommandId)
//{
//	INT_PTR			lCommandNdx;
//	DaSvrCommand *	lCommand;
//
//	for	(lCommandNdx = 0; lCommand = mCommands (lCommandNdx); lCommandNdx++)
//	{
//		if	(lCommand->mCommandId == pCommandId)
//		{
//			return lCommand;
//		}
//	}
//	return NULL;
//}

/////////////////////////////////////////////////////////////////////////////

CDaCmnCommand * DaSvrCommands::NewCommand (LPCTSTR pCaption, LPCTSTR pVoice, LPCTSTR pVoiceCaption, bool pEnabled, bool pVisible)
{
	DaSvrCommand *	lCommand;

	if	(lCommand = DaSvrCommand::CreateInstance (this, mClientMutexName))
	{
		lCommand->mCommandId = mNextCommandId++;
		lCommand->mCaption = pCaption;
		lCommand->mVoiceGrammar = pVoice;
		lCommand->mVoiceCaption = pVoiceCaption;
		lCommand->mEnabled = pEnabled;
		lCommand->mVisible = pVisible;

		lCommand->AddRef ();
	}
	return lCommand;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetCaption (BSTR Caption)
{
	return put_Caption (Caption);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetCaption (BSTR *Caption)
{
	return get_Caption (Caption);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetVoice (BSTR Voice)
{
	return put_VoiceGrammar (Voice);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetVoice (BSTR *Voice)
{
	return get_VoiceGrammar (Voice);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetVoiceCaption (BSTR VoiceCaption)
{
	return put_VoiceCaption (VoiceCaption);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetVoiceCaption (BSTR *VoiceCaption)
{
	return get_VoiceCaption (VoiceCaption);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetVisible (long Visible)
{
	return put_Visible (Visible ? VARIANT_TRUE : VARIANT_FALSE);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetVisible (long *Visible)
{
	VARIANT_BOOL	lVisible = VARIANT_FALSE;
	HRESULT			lResult = get_Visible (&lVisible);

	if	(Visible)
	{
		(*Visible) = (lVisible != VARIANT_FALSE);
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetDefaultID (long CommandID)
{
	return put_DefaultCommand (CommandID);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetDefaultID (long *CommandID)
{
	return get_DefaultCommand (CommandID);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetHelpContextID (long HelpContextID)
{
	HRESULT	lResult = E_NOTIMPL;

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::SetHelpContextID"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetHelpContextID (long *HelpContextID)
{
	HRESULT	lResult = E_NOTIMPL;

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::GetHelpContextID"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetFontName (BSTR FontName)
{
	return put_FontName (FontName);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetFontName (BSTR *FontName)
{
	return get_FontName (FontName);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetFontSize (long FontSize)
{
	return put_FontSize (FontSize);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetFontSize (long *FontSize)
{
	return get_FontSize (FontSize);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::SetGlobalVoiceCommandsEnabled (long Enabled)
{
	return put_GlobalVoiceCommandsEnabled (Enabled ? VARIANT_TRUE : VARIANT_FALSE);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetGlobalVoiceCommandsEnabled (long *Enabled)
{
	VARIANT_BOOL	lEnabled = VARIANT_FALSE;
	HRESULT			lResult = get_GlobalVoiceCommandsEnabled (&lEnabled);

	if	(Enabled)
	{
		(*Enabled) = (lEnabled != VARIANT_FALSE);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetCount (long *Count)
{
	return get_Count (Count);
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetCommand (long CommandID, IUnknown **Command)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::GetCommand"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT			lResult = S_OK;
	DaSvrCommand *	lCommand;

	if	(!Command)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Command) = NULL;

		if	(lCommand = dynamic_cast <DaSvrCommand*> (CDaCmnCommands::GetCommand ((USHORT) CommandID)))
		{
			IUnknownPtr lInterface (lCommand->GetControllingUnknown ());
			(*Command) = lInterface.Detach();
		}
		else
		{
			lResult = S_FALSE;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::GetCommand"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::GetCommandEx (long CommandID, IDaSvrCommand **Command)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::GetCommandEx"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT			lResult = S_OK;
	DaSvrCommand *	lCommand;

	if	(!Command)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Command) = NULL;

		if	(lCommand = dynamic_cast <DaSvrCommand*> (CDaCmnCommands::GetCommand ((USHORT) CommandID)))
		{
			IDaSvrCommandPtr lInterface (lCommand->GetControllingUnknown ());
			(*Command) = lInterface.Detach ();
		}
		else
		{
			lResult = S_FALSE;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::GetCommandEx"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::Add (BSTR Caption, BSTR VoiceGrammar, long Enabled, long Visible, long *CommandID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::Add [%ls] [%ls] [%d] [%d]"), this, mCharID, max(m_dwRef,-1), Caption, VoiceGrammar, Enabled, Visible);
#endif
	HRESULT	lResult = CDaCmnCommands::Add (Caption, VoiceGrammar, Caption, Enabled, Visible, CommandID);

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::Add"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::AddEx (BSTR Caption, BSTR VoiceGrammar, BSTR VoiceCaption, long Enabled, long Visible, long HelpContextID, long *CommandID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::AddEx"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommands::Add (Caption, VoiceGrammar, VoiceCaption, Enabled, Visible, CommandID);

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::AddEx"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::Insert (BSTR Caption, BSTR VoiceGrammar, long Enabled, long Visible, long RefCommandID, long Before, long *CommandID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::Insert"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommands::Insert (Caption, VoiceGrammar, Caption, Enabled, Visible, RefCommandID, Before, CommandID);

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::Insert"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::InsertEx (BSTR Caption, BSTR VoiceGrammar, BSTR VoiceCaption, long Enabled, long Visible, long HelpContextID, long RefCommandID, long Before, long *CommandID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::InsertEx"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommands::Insert (Caption, VoiceGrammar, VoiceCaption, Enabled, Visible, RefCommandID, Before, CommandID);

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::InsertEx"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::Remove (long CommandID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::Remove"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommands::Remove (CommandID);

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::Remove"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::RemoveAll (void)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::RemoveAll"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommands::RemoveAll ();

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::RemoveAll"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::get_Item (long Index, IDaSvrCommand2 **Command)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::get_Item"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT			lResult = S_OK;
	DaSvrCommand *	lCommand;

	if	(!Command)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Command) = NULL;

		if	(lCommand = dynamic_cast <DaSvrCommand*> (mCommands (Index)))
		{
			IDaSvrCommand2Ptr lInterface (lCommand->GetControllingUnknown ());
			(*Command) = lInterface.Detach();
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (((lResult==E_INVALIDARG)?MaxLogLevel(_LOG_RESULTS,LogDetails):_LOG_RESULTS), lResult, _T("[%p(%d)(%d)] DaSvrCommands::get_Item"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::get_Count (long *Count)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::get_Count"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommands::get_Count (Count);

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::get_Count"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::get_Command (long CommandID, IDaSvrCommand2 **Command)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::get_Command"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT			lResult = S_OK;
	DaSvrCommand *	lCommand;

	if	(!Command)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Command) = NULL;

		if	(lCommand = dynamic_cast <DaSvrCommand*> (CDaCmnCommands::GetCommand ((USHORT) CommandID)))
		{
			IDaSvrCommand2Ptr lInterface (lCommand->GetControllingUnknown ());
			(*Command) = lInterface.Detach();
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::get_Command"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::get_Caption (BSTR *Caption)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::get_Caption"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommands::get_Caption (Caption);

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::get_Caption"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::put_Caption (BSTR Caption)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::put_Caption [%ls]"), this, mCharID, max(m_dwRef,-1), Caption);
#endif
	HRESULT	lResult = CDaCmnCommands::put_Caption (Caption);

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::put_Caption"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::get_VoiceGrammar (BSTR *VoiceGrammar)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::get_VoiceGrammar"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommands::get_VoiceGrammar (VoiceGrammar);

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::get_VoiceGrammar"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::put_VoiceGrammar (BSTR VoiceGrammar)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::put_VoiceGrammar"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommands::put_VoiceGrammar (VoiceGrammar);

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::put_VoiceGrammar"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::get_Visible (VARIANT_BOOL *Visible)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::get_Visible"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommands::get_Visible (Visible);

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::get_Visible"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::put_Visible (VARIANT_BOOL Visible)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::SetVisible [%d]"), this, mCharID, max(m_dwRef,-1), Visible);
#endif
	HRESULT	lResult = CDaCmnCommands::put_Visible (Visible);

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::SetVisible"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::get_DefaultCommand (long *CommandID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::get_DefaultCommand"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommands::get_DefaultCommand (CommandID);

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::get_DefaultCommand"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::put_DefaultCommand (long CommandID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::put_DefaultCommand"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommands::put_DefaultCommand (CommandID);

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::put_DefaultCommand"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::get_FontName (BSTR *FontName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::get_FontName"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommands::get_FontName (FontName);

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::get_FontName"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::put_FontName (BSTR FontName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::SetFontName"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommands::put_FontName (FontName);

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::SetFontName"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::get_FontSize (long *FontSize)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::get_FontSize"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommands::get_FontSize (FontSize);

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::get_FontSize"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::put_FontSize (long FontSize)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::put_FontSize"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommands::put_FontSize (FontSize);

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::put_FontSize"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::get_VoiceCaption (BSTR *VoiceCaption)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::get_VoiceCaption"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommands::get_VoiceCaption (VoiceCaption);

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::get_VoiceCaption"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::put_VoiceCaption (BSTR VoiceCaption)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::put_VoiceCaption"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommands::put_VoiceCaption (VoiceCaption);

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::put_VoiceCaption"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::get_GlobalVoiceCommandsEnabled (VARIANT_BOOL *Enabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::get_GlobalVoiceCommandsEnabled"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommands::get_GlobalVoiceCommandsEnabled (Enabled);

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::get_GlobalVoiceCommandsEnabled"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCommands::put_GlobalVoiceCommandsEnabled (VARIANT_BOOL Enabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::put_GlobalVoiceCommandsEnabled"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnCommands::put_GlobalVoiceCommandsEnabled (Enabled);

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::put_GlobalVoiceCommandsEnabled"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCommands::get__NewEnum (IUnknown **EnumVariant)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrCommands::get__NewEnum"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT								lResult = S_OK;
	tPtr <CComObject<CEnumVARIANT> >	lEnumVariant;
	IEnumVARIANTPtr						lInterface;

	if	(!EnumVariant)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*EnumVariant) = NULL;

		if	(
				(SUCCEEDED (lResult = CComObject<CEnumVARIANT>::CreateInstance (lEnumVariant.Free())))
			&&	(SUCCEEDED (lResult = InitEnumVariant (lEnumVariant)))
			)
		{
			lInterface = lEnumVariant.Detach();
			(*EnumVariant) = lInterface.Detach();
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrCommands::get__NewEnum"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}
