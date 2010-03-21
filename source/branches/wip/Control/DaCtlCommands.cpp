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
#include "DaControl.h"
#include "DaCtlCommands.h"
#include "DaCtlCommand.h"
#include "ErrorInfo.h"
#include "Registry.h"
#include "OleVariantEx.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

CDaCtlCommands::CDaCtlCommands ()
:	mOwner (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::CDaCtlCommands (%d) [%p]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add ((LPDISPATCH)this);
#endif
}

CDaCtlCommands::~CDaCtlCommands ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::~CDaCtlCommands (%d) [%p]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
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
			LogMessage (LogNormal, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands Attached [%p]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, mOwner->mCommands.GetInterfacePtr());
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

void CDaCtlCommands::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::FinalRelease (%d) [%p]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
}

void CDaCtlCommands::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::Terminate [%u] [%p]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr());
		}
#endif
#endif
		try
		{
			int				lNdx;
			CDaCtlCommand *	lCommand;

			for	(lNdx = 0; lNdx <= mCommands.GetUpperBound(); lNdx++)
			{
				try
				{
					if	(lCommand = dynamic_cast <CDaCtlCommand *> (mCommands [lNdx].GetInterfacePtr()))
					{
						lCommand->Terminate (pFinal);
					}
				}
				catch AnyExceptionSilent

				if	(pFinal)
				{
					mCommands [lNdx] = NULL;
				}
			}
		}
		catch AnyExceptionDebug

		if	(pFinal)
		{
			mCommands.RemoveAll ();
		}
		if	(pFinal)
		{
			mOwner = NULL;
			mServerObject.Detach ();
		}
		else
		{
			SafeFreeSafePtr (mServerObject);
		}
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::Terminate [%u] Done [%d]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, pFinal, AfxOleCanExitApp());
		}
#endif
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////

void CDaCtlCommands::SetOwner (CDaCtlCharacter * pOwner)
{
	if	(mOwner = pOwner)
	{
		mServerObject = mOwner->mServerObject;
	}
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::SetOwner (%d) [%p]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
}

CDaCtlCharacter * CDaCtlCommands::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int CDaCtlCommands::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDaCtlCommands::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaCtlCommands), riid))
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

CDaCtlCommand * CDaCtlCommands::GetCommand (LPCTSTR pCommandName)
{
	CDaCtlCommand *	lCommand = NULL;
	INT_PTR			lNdx = mCommands.FindKey (pCommandName);

	if	(lNdx >= 0)
	{
		lCommand = dynamic_cast <CDaCtlCommand *> (mCommands [lNdx].GetInterfacePtr());
	}
	return lCommand;
}

CDaCtlCommand * CDaCtlCommands::GetCommand (long pCommandId)
{
	CDaCtlCommand *	lCommand = NULL;
	int				lNdx;

	if	(pCommandId > 0)
	{
		for	(lNdx = 0; lNdx <= mCommands.GetUpperBound(); lNdx++)
		{
			if	(
					(lCommand = dynamic_cast <CDaCtlCommand *> (mCommands [lNdx].GetInterfacePtr()))
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

CString CDaCtlCommands::GetCommandName (long pCommandId)
{
	CString			lCommandName;
	CDaCtlCommand *	lCommand;
	int				lNdx;

	if	(pCommandId > 0)
	{
		for	(lNdx = 0; lNdx <= mCommands.GetUpperBound(); lNdx++)
		{
			if	(
					(lCommand = dynamic_cast <CDaCtlCommand *> (mCommands [lNdx].GetInterfacePtr()))
				&&	(lCommand->mServerId == pCommandId)
				)
			{
				lCommandName = mCommands.KeyAt (lNdx);
				break;
			}
		}
	}
	return lCommandName;
}

long CDaCtlCommands::GetCommandId (LPCTSTR pCommandName)
{
	long			lCommandId = 0;
	CDaCtlCommand *	lCommand;
	INT_PTR			lNdx = mCommands.FindKey (pCommandName);

	if	(
			(lNdx >= 0)
		&&	(lCommand = dynamic_cast <CDaCtlCommand *> (mCommands [lNdx].GetInterfacePtr()))
		)
	{
		lCommandId = lCommand->mServerId;
	}
	return lCommandId;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCommands::get_Item (BSTR Name, IDaCtlCommand **Item)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get_Item"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	CString				lItemName (Name);
	IDispatchPtr		lItemDispatch;
	IDaCtlCommandPtr	lItem;

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

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get_Item"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCommands::Command (BSTR Name, IDaCtlCommand **Item)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::Command"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif

	HRESULT	lResult = get_Item (Name, Item);

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::Command"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCommands::get_Count (long *Count)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get_Count"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Count)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Count) = (long)mCommands.GetSize();
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get_Count"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCommands::get_Caption (BSTR *Caption)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get_Caption"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Caption)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Caption) = NULL;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetCaption (Caption);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get_Caption"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCommands::put_Caption (BSTR Caption)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::put_Caption"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->SetCaption (Caption);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::put_Caption"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCommands::get_Voice (BSTR *Voice)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get_Voice"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Voice)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Voice) = NULL;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetVoice (Voice);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get_Voice"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCommands::put_Voice (BSTR Voice)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::put_Voice"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->SetVoice (Voice);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::put_Voice"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCommands::get_Visible (VARIANT_BOOL *Visible)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get_Visible"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lVisible = 0;
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GetVisible (&lVisible);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}
	if	(Visible)
	{
		(*Visible) = lVisible ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get_Visible"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCommands::put_Visible (VARIANT_BOOL Visible)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::put_Visible"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->SetVisible (Visible!=VARIANT_FALSE);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::put_Visible"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCommands::get__NewEnum (IUnknown **ppunkEnum)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get__NewEnum"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT					lResult = S_OK;
	tPtr <CEnumVARIANT>		lEnum;
	tArrayPtr <_variant_t>	lArray;
	IEnumVARIANTPtr			lInterface;
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
			&&	(lArray = new _variant_t [mCommands.GetSize()+1])
			)
		{
			for	(lNdx = 0; lNdx <= mCommands.GetUpperBound(); lNdx++)
			{
				lArray [lNdx] = mCommands [lNdx].GetInterfacePtr();
			}
			
			if	(SUCCEEDED (lResult = lEnum->Init (&(lArray[0]), &(lArray[mCommands.GetSize()]), (LPDISPATCH)this, AtlFlagCopy)))
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

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get__NewEnum"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCommands::Add (BSTR Name, VARIANT Caption, VARIANT Voice, VARIANT Enabled, VARIANT Visible, IDaCtlCommand **Command)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::Add"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT								lResult = S_OK;
	CString								lName (Name);
	tPtr <CComObject <CDaCtlCommand> >	lCommand;
	_bstr_t								lCaption;
	_bstr_t								lVoice;
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
	if	(IsEmptyParm (&Voice))
	{
		lVoice = lName;
	}
	else
	{
		try
		{
			lVoice = (bstr_t)_variant_t(Voice);
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
	if	(mCommands.FindKey (lName) >= 0)
	{
		lResult = AGENTERR_COMMANDALREADYINUSE;
	}
	else
	if	(!Command)
	{
		lResult = E_POINTER;
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (CComObject <CDaCtlCommand>::CreateInstance (lCommand.Free())))
			{
				lResult = mServerObject->Add (lCaption, lVoice, lEnabled, lVisible, &lCommand->mServerId);
				if	(SUCCEEDED (lResult))
				{
					lResult = mServerObject->GetCommandEx (lCommand->mServerId, &lCommand->mServerObject);
				}
				if	(
						(lCommand->mServerObject != NULL)
					&&	(lCommand->mServerId != 0)
					)
				{
					lCommand->SetOwner (mOwner);
					mCommands.SetAt (lName, (LPDISPATCH) lCommand.Detach());
				}
				else
				{
					if	(lCommand->mServerId != 0)
					{
						mServerObject->Remove (lCommand->mServerId);
					}
					if	(SUCCEEDED (lResult))
					{
						lResult = E_FAIL;
					}
				}
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::Add"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCommands::Insert (BSTR Name, BSTR RefName, VARIANT Before, VARIANT Caption, VARIANT Voice, VARIANT Enabled, VARIANT Visible, IDaCtlCommand **Command)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::Insert"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT								lResult = S_OK;
	CString								lName (Name);
	CString								lRefName (RefName);
	CDaCtlCommand *						lRefCommand = NULL;
	tPtr <CComObject <CDaCtlCommand> >	lCommand;
	_bstr_t								lCaption;
	_bstr_t								lVoice;
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
	if	(IsEmptyParm (&Voice))
	{
		lVoice = lName;
	}
	else
	{
		try
		{
			lVoice = (bstr_t)_variant_t(Voice);
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
	if	(mCommands.FindKey (lName) >= 0)
	{
		lResult = AGENTERR_COMMANDALREADYINUSE;
	}
	else
	if	((lRefCommand = GetCommand (lRefName)) == NULL)
	{
		lResult = AGENTERR_COMMANDNOTFOUND;
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (CComObject <CDaCtlCommand>::CreateInstance (lCommand.Free())))
			{
				lResult = mServerObject->Insert (lCaption, lVoice, lEnabled, lVisible, lRefCommand->mServerId, lBefore, &lCommand->mServerId);
				if	(SUCCEEDED (lResult))
				{
					lResult = mServerObject->GetCommandEx (lCommand->mServerId, &lCommand->mServerObject);
				}
				if	(
						(lCommand->mServerObject != NULL)
					&&	(lCommand->mServerId != 0)
					)
				{
					lCommand->SetOwner (mOwner);
					mCommands.SetAt (lName, (LPDISPATCH) lCommand.Detach());
				}
				else
				{
					if	(lCommand->mServerId != 0)
					{
						mServerObject->Remove (lCommand->mServerId);
					}
					if	(SUCCEEDED (lResult))
					{
						lResult = E_FAIL;
					}
				}
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::Insert"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCommands::Remove (BSTR Name)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::Remove"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT			lResult = S_FALSE;
	CString			lName (Name);
	CDaCtlCommand *	lCommand = NULL;

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

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::Remove"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCommands::RemoveAll (void)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::RemoveAll"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	try
	{
		mCommands.RemoveAll ();
	}
	catch AnyExceptionDebug

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->RemoveAll ();
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::RemoveAll"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCommands::get_DefaultCommand (BSTR *Name)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get_DefaultCommand"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lDefaultId = 0;
	CString	lDefaultName;

	if	(!Name)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetDefaultID (&lDefaultId);
				lDefaultName = GetCommandName (lDefaultId);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}

		(*Name) = lDefaultName.AllocSysString ();
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get_DefaultCommand"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCommands::put_DefaultCommand (BSTR Name)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::put_DefaultCommand"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	CString	lDefaultName (Name);
	long	lDefaultId;

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
	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		)
	{
		try
		{
			lResult = mServerObject->SetDefaultID (lDefaultId);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::put_DefaultCommand"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCommands::get_HelpContextID (long *ID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get_HelpContextID"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!ID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ID) = 0;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetHelpContextID (ID);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get_HelpContextID"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCommands::put_HelpContextID (long ID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::put_HelpContextID"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->SetHelpContextID (ID);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::put_HelpContextID"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCommands::get_FontName (BSTR *FontName)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get_FontName"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!FontName)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*FontName) = NULL;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetFontName (FontName);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get_FontName"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCommands::put_FontName (BSTR FontName)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::put_FontName"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->SetFontName (FontName);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::put_FontName"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCommands::get_FontSize (long *FontSize)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get_FontSize"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!FontSize)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*FontSize) = 0;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetFontSize (FontSize);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get_FontSize"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCommands::put_FontSize (long FontSize)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::put_FontSize"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->SetFontSize (FontSize);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::put_FontSize"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCommands::get_VoiceCaption (BSTR *VoiceCaption)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get_VoiceCaption"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!VoiceCaption)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*VoiceCaption) = NULL;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetVoiceCaption (VoiceCaption);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get_VoiceCaption"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCommands::put_VoiceCaption (BSTR VoiceCaption)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::put_VoiceCaption"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->SetVoiceCaption (VoiceCaption);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::put_VoiceCaption"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCommands::get_GlobalVoiceCommandsEnabled (VARIANT_BOOL *Enable)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get_GlobalVoiceCommandsEnabled"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lEnable = FALSE;
	HRESULT	lResult;

	if	(!Enable)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetGlobalVoiceCommandsEnabled (&lEnable);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
		(*Enable) = lEnable ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::get_GlobalVoiceCommandsEnabled"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCommands::put_GlobalVoiceCommandsEnabled (VARIANT_BOOL Enable)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::put_GlobalVoiceCommandsEnabled"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->SetGlobalVoiceCommandsEnabled (Enable!=VARIANT_FALSE);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] CDaCtlCommands::put_GlobalVoiceCommandsEnabled"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}
