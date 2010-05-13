/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent ActiveX Control.

    The Double Agent ActiveX Control is free software:
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
#include "DaControlMod.h"
#include "DaCtlCommands.h"
#include "DaCtlCommand.h"
#include "ErrorInfo.h"
#include "Registry.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaCtlCommands::DaCtlCommands ()
:	mOwner (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::DaCtlCommands (%d) [%p]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add ((LPDISPATCH)this);
#endif
}

DaCtlCommands::~DaCtlCommands ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::~DaCtlCommands (%d) [%p]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	try
	{
		if	(
				(mOwner)
			&&	(mOwner->mCommands != NULL)
			)
		{
			LogMessage (LogNormal, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands Attached [%p]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, mOwner->mCommands.GetInterfacePtr());
		}
	}
	catch AnyExceptionSilent
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Remove ((LPDISPATCH)this);
#endif

	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

void DaCtlCommands::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::FinalRelease (%d) [%p]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
}

void DaCtlCommands::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::Terminate [%u] [%p(%u)]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr(), CoIsHandlerConnected(mServerObject));
		}
#endif
#endif
		try
		{
			POSITION		lPos;
			DaCtlCommand *	lCommand;

			for	(lPos = mCommands.GetStartPosition(); lPos;)
			{
				try
				{
					if	(lCommand = dynamic_cast <DaCtlCommand *> (mCommands.GetValueAt(lPos).GetInterfacePtr()))
					{
						lCommand->Terminate (pFinal);
					}
				}
				catch AnyExceptionSilent

				if	(pFinal)
				{
					mCommands.SetValueAt (lPos, NULL);
				}
				mCommands.GetNext (lPos);
			}
		}
		catch AnyExceptionDebug

		if	(pFinal)
		{
			mCommands.RemoveAll ();
		}
		if	(pFinal)
		{
			mServerObject.Detach ();
		}
		else
		{
			SafeFreeSafePtr (mServerObject);
		}

		SafeFreeSafePtr (mLocalObject);
		mOwner = NULL;
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::Terminate [%u] Done [%d]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, pFinal, _AtlModule.GetLockCount());
		}
#endif
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaCtlCommands::SetOwner (DaCtlCharacter * pOwner)
{
	HRESULT	lResult = S_OK;

	if	(mOwner = pOwner)
	{
		if	(mOwner->mServerObject)
		{
			mServerObject = mOwner->mServerObject;
			if	(!mServerObject)
			{
				lResult = E_FAIL;
			}
		}
		else
		if	(mOwner->mLocalObject)
		{
			if	(mLocalObject = new CDaCmnCommands)
			{
				mLocalObject->Initialize (mOwner->mLocalObject->GetCharID(), mOwner->mLocalObject->mNotify);
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
		else
		{
			lResult = E_FAIL;
		}
	}
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::SetOwner (%d) [%p]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
	return lResult;
}

DaCtlCharacter * DaCtlCommands::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int DaCtlCommands::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaCtlCommands::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaCtlCommands2), riid))
		||	(InlineIsEqualGUID (__uuidof(IDaCtlCommands), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCtlCommands), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCtlCommandsEx), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

DaCtlCommand * DaCtlCommands::GetCommand (LPCTSTR pCommandName)
{
	DaCtlCommand *	lCommand = NULL;
	IDispatchPtr	lDispatch;

	if	(mCommands.Lookup (pCommandName, lDispatch))
	{
		lCommand = dynamic_cast <DaCtlCommand *> (lDispatch.GetInterfacePtr());
	}
	return lCommand;
}

DaCtlCommand * DaCtlCommands::GetCommand (long pCommandId)
{
	DaCtlCommand *	lCommand = NULL;
	POSITION		lPos;

	if	(pCommandId > 0)
	{
		for	(lPos = mCommands.GetStartPosition(); lPos;)
		{
			if	(
					(lCommand = dynamic_cast <DaCtlCommand *> (mCommands.GetNextValue(lPos).GetInterfacePtr()))
				&&	(lCommand->mServerId == pCommandId)
				)
			{
				break;
			}
			lCommand = NULL;
		}
	}
	return lCommand;
}

/////////////////////////////////////////////////////////////////////////////

CAtlString DaCtlCommands::GetCommandName (long pCommandId)
{
	CAtlString		lCommandName;
	DaCtlCommand *	lCommand;
	POSITION		lPos;

	if	(pCommandId > 0)
	{
		for	(lPos = mCommands.GetStartPosition(); lPos;)
		{
			if	(
					(lCommand = dynamic_cast <DaCtlCommand *> (mCommands.GetValueAt(lPos).GetInterfacePtr()))
				&&	(lCommand->mServerId == pCommandId)
				)
			{
				lCommandName = mCommands.GetKeyAt (lPos);
				break;
			}
			mCommands.GetNext (lPos);
		}
	}
	return lCommandName;
}

long DaCtlCommands::GetCommandId (LPCTSTR pCommandName)
{
	long			lCommandId = 0;
	DaCtlCommand *	lCommand;
	IDispatchPtr	lDispatch;

	if	(
			(mCommands.Lookup (pCommandName, lDispatch))
		&&	(lCommand = dynamic_cast <DaCtlCommand *> (lDispatch.GetInterfacePtr()))
		)
	{
		lCommandId = lCommand->mServerId;
	}
	return lCommandId;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCommands::get_Item (BSTR Name, IDaCtlCommand2 **Item)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_Item"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	CAtlString			lItemName (Name);
	IDispatchPtr		lItemDispatch;
	IDaCtlCommand2Ptr	lItem;

	if	(!Item)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Item) = NULL;

		if	(mCommands.Lookup (lItemName, lItemDispatch))
		{
			lItem = lItemDispatch;
			(*Item) = lItem.Detach();
		}
		else
		{
			lResult = AGENTERR_COMMANDNOTFOUND;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_Item"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCommands::Command (BSTR Name, IDaCtlCommand2 **Item)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::Command"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif

	HRESULT	lResult = get_Item (Name, Item);

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::Command"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCommands::get_Index (long Index, IDaCtlCommand2 **Command)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_Index"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	IDispatchPtr		lCommandDispatch;
	IDaCtlCommand2Ptr	lCommand;

	if	(!Command)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Command) = NULL;

		if	(
				(Index >= 0)
			&&	(Index < (long)mCommands.GetCount())
			)
		{
			POSITION	lPos;

			for	(lPos = mCommands.GetStartPosition(); lPos;)
			{
				if	(Index-- <= 0)
				{
					lCommandDispatch = mCommands.GetValueAt (lPos);
					break;
				}
				mCommands.GetNextValue (lPos);
			}
		}
		if	(lCommandDispatch == NULL)
		{
			lResult = E_INVALIDARG;
		}
		else
		{
			lCommand = lCommandDispatch.GetInterfacePtr();
			(*Command) = lCommand.Detach();
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_Index"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCommands::get_Count (long *Count)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_Count"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Count)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Count) = (long)mCommands.GetCount();
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_Count"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCommands::get_Caption (BSTR *Caption)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_Caption"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Caption)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Caption) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_Caption (Caption);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_Caption (Caption);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_Caption"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCommands::put_Caption (BSTR Caption)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::put_Caption"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_Caption (Caption);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_Caption (Caption);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::put_Caption"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCommands::get_Voice (BSTR *Voice)
{
	return get_VoiceGrammar (Voice);
}

HRESULT STDMETHODCALLTYPE DaCtlCommands::put_Voice (BSTR Voice)
{
	return put_VoiceGrammar (Voice);
}

HRESULT STDMETHODCALLTYPE DaCtlCommands::get_VoiceGrammar (BSTR *VoiceGrammar)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_VoiceGrammar"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!VoiceGrammar)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*VoiceGrammar) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_VoiceGrammar (VoiceGrammar);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_VoiceGrammar (VoiceGrammar);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_VoiceGrammar"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCommands::put_VoiceGrammar (BSTR VoiceGrammar)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::put_VoiceGrammar"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_VoiceGrammar (VoiceGrammar);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_VoiceGrammar (VoiceGrammar);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::put_VoiceGrammar"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCommands::get_Visible (VARIANT_BOOL *Visible)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_Visible"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->get_Visible (Visible);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->get_Visible (Visible);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_Visible"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCommands::put_Visible (VARIANT_BOOL Visible)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::put_Visible"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_Visible (Visible!=VARIANT_FALSE);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_Visible (Visible!=VARIANT_FALSE);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::put_Visible"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCommands::get__NewEnum (IUnknown **ppunkEnum)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get__NewEnum"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT					lResult = S_OK;
	tPtr <CEnumVARIANT>		lEnum;
	tArrayPtr <CComVariant>	lArray;
	IEnumVARIANTPtr			lInterface;
	POSITION				lPos;
	INT_PTR					lNdx;

	if	(!ppunkEnum)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ppunkEnum) = NULL;

		if	(
				(lEnum = new CComObject <CEnumVARIANT>)
			&&	(lArray = new CComVariant [mCommands.GetCount()+1])
			)
		{
			for	(lPos = mCommands.GetStartPosition(), lNdx = 0; lPos; lNdx++)
			{
				lArray [lNdx] = mCommands.GetNextValue(lPos).GetInterfacePtr();
			}

			if	(SUCCEEDED (lResult = lEnum->Init (&(lArray[0]), &(lArray[(INT_PTR)mCommands.GetCount()]), (LPDISPATCH)this, AtlFlagCopy)))
			{
				lInterface = lEnum.Detach ();
				(*ppunkEnum) = lInterface.Detach ();
			}
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get__NewEnum"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCommands::Add (BSTR Name, VARIANT Caption, VARIANT VoiceGrammar, VARIANT Enabled, VARIANT Visible, IDaCtlCommand2 **Command)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::Add"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT								lResult = S_OK;
	CAtlString							lName (Name);
	tPtr <CComObject <DaCtlCommand> >	lCommand;
	_bstr_t								lCaption;
	_bstr_t								lVoiceGrammar;
	long								lEnabled = TRUE;
	long								lVisible = TRUE;

	if	(IsEmptyParm (&Caption))
	{
		lCaption = lName;
	}
	else
	{
		try
		{
			lCaption = (bstr_t)_variant_t(Caption);
		}
		catch AnyExceptionSilent
	}
	if	(IsEmptyParm (&VoiceGrammar))
	{
		lVoiceGrammar = lName;
	}
	else
	{
		try
		{
			lVoiceGrammar = (bstr_t)_variant_t(VoiceGrammar);
		}
		catch AnyExceptionSilent
	}
	if	(!IsEmptyParm (&Enabled))
	{
		try
		{
			lEnabled = (long)(bool)_variant_t(Enabled);
		}
		catch AnyExceptionSilent
	}
	if	(!IsEmptyParm (&Visible))
	{
		try
		{
			lVisible = (long)(bool)_variant_t(Visible);
		}
		catch AnyExceptionSilent
	}

	if	(lName.IsEmpty ())
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(mCommands.Lookup (lName))
	{
		lResult = AGENTERR_COMMANDALREADYINUSE;
	}
	else
	if	(!Command)
	{
		lResult = E_POINTER;
	}
	else
	if	(mLocalObject)
	{
		try
		{
			if	(
					(SUCCEEDED (lResult = CComObject <DaCtlCommand>::CreateInstance (lCommand.Free())))
				&&	(SUCCEEDED (lResult = lCommand->SetOwner (mOwner)))
				&&	(SUCCEEDED (lResult = mLocalObject->Add (lCaption, lVoiceGrammar, lCaption, lEnabled, lVisible, &lCommand->mServerId)))
				)
			{
				mCommands.SetAt (lName, (LPDISPATCH) lCommand.Detach());
			}
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(
					(SUCCEEDED (lResult = CComObject <DaCtlCommand>::CreateInstance (lCommand.Free())))
				&&	(SUCCEEDED (lResult = mServerObject->Add (lCaption, lVoiceGrammar, lEnabled, lVisible, &lCommand->mServerId)))
				&&	(SUCCEEDED (lResult = mServerObject->get_Command (lCommand->mServerId, &lCommand->mServerObject)))
				&&	(SUCCEEDED (lResult = lCommand->SetOwner (mOwner)))
				)
			{
				mCommands.SetAt (lName, (LPDISPATCH)lCommand.Detach());
			}
			else
			if	(
					(lCommand)
				&&	(lCommand->mServerId != 0)
				)
			{
				mServerObject->Remove (lCommand->mServerId);
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::Add"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCommands::Insert (BSTR Name, BSTR RefName, VARIANT Before, VARIANT Caption, VARIANT VoiceGrammar, VARIANT Enabled, VARIANT Visible, IDaCtlCommand2 **Command)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::Insert"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT								lResult = S_OK;
	CAtlString							lName (Name);
	CAtlString							lRefName (RefName);
	DaCtlCommand *						lRefCommand = NULL;
	tPtr <CComObject <DaCtlCommand> >	lCommand;
	_bstr_t								lCaption;
	_bstr_t								lVoiceGrammar;
	long								lEnabled = TRUE;
	long								lVisible = TRUE;
	long								lBefore = TRUE;

	if	(IsEmptyParm (&Caption))
	{
		lCaption = lName;
	}
	else
	{
		try
		{
			lCaption = (bstr_t)_variant_t(Caption);
		}
		catch AnyExceptionSilent
	}
	if	(IsEmptyParm (&VoiceGrammar))
	{
		lVoiceGrammar = lName;
	}
	else
	{
		try
		{
			lVoiceGrammar = (bstr_t)_variant_t(VoiceGrammar);
		}
		catch AnyExceptionSilent
	}
	if	(!IsEmptyParm (&Enabled))
	{
		try
		{
			lEnabled = (long)(bool)_variant_t(Enabled);
		}
		catch AnyExceptionSilent
	}
	if	(!IsEmptyParm (&Visible))
	{
		try
		{
			lVisible = (long)(bool)_variant_t(Visible);
		}
		catch AnyExceptionSilent
	}
	if	(!IsEmptyParm (&Before))
	{
		try
		{
			lBefore = (long)(bool)_variant_t(Before);
		}
		catch AnyExceptionSilent
	}

	if	(
			(lName.IsEmpty ())
		||	(lRefName.IsEmpty ())
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(mCommands.Lookup (lName))
	{
		lResult = AGENTERR_COMMANDALREADYINUSE;
	}
	else
	if	((lRefCommand = GetCommand (lRefName)) == NULL)
	{
		lResult = AGENTERR_COMMANDNOTFOUND;
	}
	else
	if	(mLocalObject)
	{
		try
		{
			if	(
					(SUCCEEDED (lResult = CComObject <DaCtlCommand>::CreateInstance (lCommand.Free())))
				&&	(SUCCEEDED (lResult = lCommand->SetOwner (mOwner)))
				&&	(SUCCEEDED (lResult = mLocalObject->Insert (lCaption, lVoiceGrammar, lCaption, lEnabled, lVisible, lRefCommand->mServerId, lBefore, &lCommand->mServerId)))
				)
			{
				mCommands.SetAt (lName, (LPDISPATCH) lCommand.Detach());
			}
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(
					(SUCCEEDED (lResult = CComObject <DaCtlCommand>::CreateInstance (lCommand.Free())))
				&&	(SUCCEEDED (lResult = mServerObject->Insert (lCaption, lVoiceGrammar, lEnabled, lVisible, lRefCommand->mServerId, lBefore, &lCommand->mServerId)))
				&&	(SUCCEEDED (lResult = mServerObject->get_Command (lCommand->mServerId, &lCommand->mServerObject)))
				&&	(SUCCEEDED (lResult = lCommand->SetOwner (mOwner)))
				)
			{
				mCommands.SetAt (lName, (LPDISPATCH)lCommand.Detach());
			}
			else
			if	(
					(lCommand)
				&&	(lCommand->mServerId != 0)
				)
			{
				mServerObject->Remove (lCommand->mServerId);
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::Insert"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCommands::Remove (BSTR Name)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::Remove"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT			lResult = S_FALSE;
	CAtlString		lName (Name);
	DaCtlCommand *	lCommand = NULL;

	if	(lName.IsEmpty ())
	{
		lResult = E_INVALIDARG;
	}
	else
	if	((lCommand = GetCommand (lName)) == NULL)
	{
		lResult = AGENTERR_COMMANDNOTFOUND;
	}
	else
	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->Remove (lCommand->mServerId);
			mCommands.RemoveKey (lName);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			SafeFreeSafePtr (lCommand->mServerObject);
			lResult = mServerObject->Remove (lCommand->mServerId);
			mCommands.RemoveKey (lName);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::Remove"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCommands::RemoveAll (void)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::RemoveAll"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	try
	{
		mCommands.RemoveAll ();
	}
	catch AnyExceptionDebug

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->RemoveAll ();
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->RemoveAll ();
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::RemoveAll"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCommands::get_DefaultCommand (BSTR *Name)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_DefaultCommand"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT		lResult;
	long		lDefaultId = 0;
	CAtlString	lDefaultName;

	if	(!Name)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Name) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_DefaultCommand (&lDefaultId);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_DefaultCommand (&lDefaultId);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}

		if	(SUCCEEDED (lResult))
		{
			lDefaultName = GetCommandName (lDefaultId);
			(*Name) = lDefaultName.AllocSysString ();
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_DefaultCommand"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCommands::put_DefaultCommand (BSTR Name)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::put_DefaultCommand"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT		lResult = S_OK;
	CAtlString	lDefaultName (Name);
	long		lDefaultId;

	if	(lDefaultName.IsEmpty ())
	{
		lDefaultId = 0;
	}
	else
	{
		lDefaultId = GetCommandId (lDefaultName);
		if	(lDefaultId <= 0)
		{
			lResult = AGENTERR_COMMANDNOTFOUND;
		}
	}

	if	(SUCCEEDED (lResult))
	{
		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->put_DefaultCommand (lDefaultId);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->put_DefaultCommand (lDefaultId);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::put_DefaultCommand"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCommands::get_HelpContextID (long *ID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_HelpContextID"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = E_NOTIMPL;

	if	(ID)
	{
		(*ID) = 0;
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_HelpContextID"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCommands::put_HelpContextID (long ID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::put_HelpContextID"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = E_NOTIMPL;

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::put_HelpContextID"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCommands::get_FontName (BSTR *FontName)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_FontName"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!FontName)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*FontName) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_FontName (FontName);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_FontName (FontName);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_FontName"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCommands::put_FontName (BSTR FontName)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::put_FontName"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_FontName (FontName);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_FontName (FontName);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::put_FontName"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCommands::get_FontSize (long *FontSize)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_FontSize"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!FontSize)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*FontSize) = 0;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_FontSize (FontSize);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_FontSize (FontSize);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_FontSize"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCommands::put_FontSize (long FontSize)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::put_FontSize"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_FontSize (FontSize);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_FontSize (FontSize);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::put_FontSize"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCommands::get_VoiceCaption (BSTR *VoiceCaption)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_VoiceCaption"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!VoiceCaption)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*VoiceCaption) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_VoiceCaption (VoiceCaption);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_VoiceCaption (VoiceCaption);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_VoiceCaption"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCommands::put_VoiceCaption (BSTR VoiceCaption)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::put_VoiceCaption"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_VoiceCaption (VoiceCaption);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_VoiceCaption (VoiceCaption);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::put_VoiceCaption"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCommands::get_GlobalVoiceCommandsEnabled (VARIANT_BOOL *Enabled)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_GlobalVoiceCommandsEnabled"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Enabled)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_GlobalVoiceCommandsEnabled (Enabled);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_GlobalVoiceCommandsEnabled (Enabled);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::get_GlobalVoiceCommandsEnabled"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCommands::put_GlobalVoiceCommandsEnabled (VARIANT_BOOL Enabled)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::put_GlobalVoiceCommandsEnabled"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_GlobalVoiceCommandsEnabled (Enabled);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_GlobalVoiceCommandsEnabled (Enabled);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlCommands::put_GlobalVoiceCommandsEnabled"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}
