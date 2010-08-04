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
#include "DaServerRes.h"
#include "DaElevatedSettings.h"
#include "ThreadSecurity.h"
#include "SecurityDesc.h"
#include "GuidStr.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Settings"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Settings"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogDetails,true)&0xFFFF)
#endif

#ifndef	_LOG_CHANGES
#define	_LOG_CHANGES		LogNormal
#endif
#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE		LogDetails
#endif
#ifndef	_LOG_RESULTS
#define	_LOG_RESULTS		LogDetails
#endif

/////////////////////////////////////////////////////////////////////////////

CDaElevatedSettings::CDaElevatedSettings ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaElevatedSettings::CDaElevatedSettings (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
		LogProcessIntegrity (GetCurrentProcess(), _LOG_INSTANCE);
	}
#endif
}

CDaElevatedSettings::~CDaElevatedSettings ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaElevatedSettings::~CDaElevatedSettings (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT WINAPI CDaElevatedSettings::UpdateRegistryOverride (BOOL bRegister)
{
	HRESULT		lResult = S_OK;

	if	(
			(!bRegister)
		||	(IsWindowsVista_AtLeast ())
		)
	{
		lResult = UpdateRegistry (bRegister);
	}

	if	(
			(SUCCEEDED (lResult))
		&&	(bRegister)
		&&	(IsWindowsVista_AtLeast ())
		)
	{
//
//	Set the launch and access permissions to allow OTS activation as per the Elevation Moniker documentation in the SDK.
//
		CRegKeyEx		lAppIdKey (CRegKeyEx (HKEY_CLASSES_ROOT, _T("AppID"), true), CGuidStr(__uuidof(CDaElevatedSettings)), false);
		CRegBinary		lLaunchPermission (lAppIdKey, _T("LaunchPermission"), true);
		CRegBinary		lAccessPermission (lAppIdKey, _T("AccessPermission"), true);
		CSecurityDesc	lLaunchDescriptor (_T("O:BAG:BAD:(A;;CCDCSW;;;BA)(A;;CCDCSW;;;IU)(A;;CCDCSW;;;SY)"));
		CSecurityDesc	lAccessDescriptor (_T("O:BAG:BAD:(A;;CCDC;;;IU)(A;;CCDC;;;PS)(A;;CCDC;;;SY)"));
		DWORD			lDescriptorSize;

		lLaunchDescriptor.MakeAbsolute ();
		lDescriptorSize = lLaunchDescriptor.MakeSelfRelative ();
		if	(lDescriptorSize > 0)
		{
			lLaunchPermission.Value().SetCount (lDescriptorSize);
			memcpy (lLaunchPermission.Value().GetData(), (PSECURITY_DESCRIPTOR)lLaunchDescriptor.mDescriptor, lDescriptorSize);
			lLaunchPermission.Update ();

			if	(LogIsActive (LogDetails))
			{
				lLaunchDescriptor.DumpAccess (LogDetails, true, _T("  Set LaunchPermission [%s]"), (CString)lLaunchDescriptor);
			}
			else
			if	(LogIsActive ())
			{
				LogMessage (LogIfActive, _T("  Set LaunchPermission [%s]"), (CString)lLaunchDescriptor);
			}
#ifdef	_DEBUG
			if	(LogIsActive (LogNormal))
			{
				CAtlString	lAccessStr;
				CAtlString	lByteStr;
				INT_PTR		lNdx;

				for	(lNdx = 0; lNdx < (INT_PTR)lLaunchPermission.Value().GetCount(); lNdx++)
				{
					lByteStr.Format (_T("%2.2X"), lLaunchPermission.Value().GetAt(lNdx));
					lAccessStr += lByteStr;
				}
				LogMessage (LogNormal, _T("  LaunchPermission [%s]"), lAccessStr);
			}
#endif
		}
		else
		{
			lLaunchPermission.Delete ();
		}

		lAccessDescriptor.MakeAbsolute ();
		lDescriptorSize = lAccessDescriptor.MakeSelfRelative ();
		if	(lDescriptorSize > 0)
		{
			lAccessPermission.Value().SetCount (lDescriptorSize);
			memcpy (lAccessPermission.Value().GetData(), (PSECURITY_DESCRIPTOR)lAccessDescriptor.mDescriptor, lDescriptorSize);
			lAccessPermission.Update ();

			if	(LogIsActive (LogDetails))
			{
				lAccessDescriptor.DumpAccess (LogDetails, true, _T("  Set AccessPermission [%s]"), (CString)lAccessDescriptor);
			}
			else
			if	(LogIsActive ())
			{
				LogMessage (LogIfActive, _T("  Set AccessPermission [%s]"), (CString)lAccessDescriptor);
			}
#ifdef	_DEBUG
			if	(LogIsActive (LogNormal))
			{
				CAtlString	lAccessStr;
				CAtlString	lByteStr;
				INT_PTR		lNdx;

				for	(lNdx = 0; lNdx < (INT_PTR)lAccessPermission.Value().GetCount(); lNdx++)
				{
					lByteStr.Format (_T("%2.2X"), lAccessPermission.Value().GetAt(lNdx));
					lAccessStr += lByteStr;
				}
				LogMessage (LogNormal, _T("  AccessPermission [%s]"), lAccessStr);
			}
#endif
		}
		else
		{
			lAccessPermission.Delete ();
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaElevatedSettings::SetTreatAs (REFGUID pClassId, REFGUID pTreatAs)
{
	HRESULT		lResult;
	HRESULT		lSubResult;
	CRegKeyEx	lUserClasses;

	lResult = CoTreatAsClass (pClassId, pTreatAs);
#ifdef	_LOG_CHANGES
	if	(LogIsActive (_LOG_CHANGES))
	{
		LogComErrAnon (MinLogLevel(_LOG_CHANGES,LogAlways), lResult, _T("CoTreatAsClass [%s] [%s]"), CGuidStr::GuidName(pClassId), CGuidStr::GuidName(pTreatAs));
	}
#endif

	if	(
			(lResult == REGDB_E_READREGDB)
		||	(lResult == REGDB_E_WRITEREGDB)
		)
	{
		if	(GetUserClassesKey (lUserClasses))
		{
			lResult = SetTreatAs (lUserClasses, pClassId, pTreatAs);
		}
		else
		{
			lResult = REGDB_E_KEYMISSING;
		}
#ifdef	_LOG_CHANGES
		if	(LogIsActive (_LOG_CHANGES))
		{
			LogComErrAnon (MinLogLevel(_LOG_CHANGES,LogAlways), lResult, _T("CoTreatAsClass [%s] [%s] (HKEY_CURRENT_USER)"), CGuidStr::GuidName(pClassId), CGuidStr::GuidName(pTreatAs));
		}
#endif
	}

	if	(IsEqualGUID (pTreatAs, GUID_NULL))
	{
		if	(GetUserClassesKey (lUserClasses))
		{
			lSubResult = SetTreatAs (lUserClasses, pClassId, pTreatAs);
		}
#ifdef	_LOG_CHANGES
		else
		{
			lSubResult = REGDB_E_KEYMISSING;
		}
		if	(LogIsActive (_LOG_CHANGES))
		{
			LogComErrAnon (MinLogLevel(_LOG_CHANGES,LogAlways), lSubResult, _T("CoTreatAsClass [%s] [%s] (HKEY_CURRENT_USER)"), CGuidStr::GuidName(pClassId), CGuidStr::GuidName(pTreatAs));
		}
#endif
	}

#ifdef	_WIN64
	if	(
			(
				(SUCCEEDED (lResult))
			||	(
					(lResult == REGDB_E_CLASSNOTREG)
				&&	(IsEqualGUID (pClassId, __uuidof(AgentServer)))
				)
			)
		&&	(
				(
					(!IsEqualGUID (pClassId, __uuidof(AgentServer64)))
				&&	(!IsEqualGUID (pClassId, __uuidof(AgentCharacterProps)))
				)
			||	(IsEqualGUID (pTreatAs, GUID_NULL))
			)
		)
	{
		CRegKeyEx	lClassesRoot;

		if	(GetClassesRootKey (lClassesRoot, true))
		{
			lSubResult = SetTreatAs (lClassesRoot, pClassId, pTreatAs);
		}
		else
		{
			lSubResult = REGDB_E_KEYMISSING;
		}
#ifdef	_LOG_CHANGES
		if	(LogIsActive (_LOG_CHANGES))
		{
			LogComErrAnon (MinLogLevel(_LOG_CHANGES,LogAlways), lSubResult, _T("CoTreatAsClass [%s] [%s] (HKEY_CLASSES_ROOT\\Wow6432Node)"), CGuidStr::GuidName(pClassId), CGuidStr::GuidName(pTreatAs));
		}
#endif
		if	(
				(FAILED (lSubResult))
			&&	(!IsEqualGUID (pTreatAs, GUID_NULL))
			)
		{
			if	(GetUserClassesKey (lUserClasses, true))
			{
				lSubResult = SetTreatAs (lUserClasses, pClassId, pTreatAs);
			}
			else
			{
				lSubResult = REGDB_E_KEYMISSING;
			}
#ifdef	_LOG_CHANGES
			if	(LogIsActive (_LOG_CHANGES))
			{
				LogComErrAnon (MinLogLevel(_LOG_CHANGES,LogAlways), lSubResult, _T("CoTreatAsClass [%s] [%s] (HKEY_CURRENT_USER\\Wow6432Node)"), CGuidStr::GuidName(pClassId), CGuidStr::GuidName(pTreatAs));
			}
#endif
			if	(
					(lResult == REGDB_E_CLASSNOTREG)
				&&	(IsEqualGUID (pClassId, __uuidof(AgentServer)))
				)
			{
				lResult = lSubResult;
			}
		}
	}

	if	(IsEqualGUID (pTreatAs, GUID_NULL))
	{
		if	(GetUserClassesKey (lUserClasses, true))
		{
			lSubResult = SetTreatAs (lUserClasses, pClassId, pTreatAs);
		}
#ifdef	_LOG_CHANGES
		else
		{
			lSubResult = REGDB_E_KEYMISSING;
		}
		if	(LogIsActive (_LOG_CHANGES))
		{
			LogComErrAnon (MinLogLevel(_LOG_CHANGES,LogAlways), lSubResult, _T("CoTreatAsClass [%s] [%s] (HKEY_CURRENT_USER\\Wow6432Node)"), CGuidStr::GuidName(pClassId), CGuidStr::GuidName(pTreatAs));
		}
#endif
	}
#endif
	return lResult;
}

HRESULT CDaElevatedSettings::SetTreatAs (HKEY pClassesRoot, REFGUID pClassId, REFGUID pTreatAs)
{
	HRESULT		lResult = REGDB_E_KEYMISSING;
	CRegKeyEx	lClassIds (pClassesRoot, _T("CLSID"), true);

	if	(
			(pClassesRoot)
		&&	(lClassIds.IsValid ())
		)
	{
		long		lError;
		CAtlString	lClassIdStr = (CString)CGuidStr (pClassId);
		CAtlString	lTreatAsStr = (CString)CGuidStr (pTreatAs);

		if	(IsEqualGUID (pTreatAs, GUID_NULL))
		{
			CRegKeyEx	lClassId (lClassIds, lClassIdStr, false);
			CRegKeyEx	lTreatAs (lClassId, _T("TreatAs"), false);

			if	(lTreatAs.IsValid ())
			{
				lError = lTreatAs.Delete ();

				if	(lError == ERROR_SUCCESS)
				{
					lResult = S_OK;
				}
				else
				{
#ifdef	_LOG_RESULTS
					if	(LogIsActive (_LOG_RESULTS))
					{
						LogWinErr (_LOG_RESULTS, lError);
					}
#endif
					lResult = HRESULT_FROM_WIN32 (lError);
				}
			}
			else
			{
				lResult = S_FALSE;
			}

			if	(
					(SUCCEEDED (lResult))
				&&	(lClassId.IsValid ())
				&&	(lClassId.IsEmpty ())
				&&	(lClassId.Delete () == ERROR_SUCCESS)
				)
			{
				lResult = S_OK;
			}
		}
		else
		{
			CRegKeyEx	lClassId;
			CRegKeyEx	lTreatAs;

			lError = lClassId.Open (lClassIds, lClassIdStr, false, true);
			if	(lClassId.IsValid ())
			{
				lError = lTreatAs.Open (lClassId, _T("TreatAs"), false, true);
			}
#ifdef	_LOG_RESULTS
			else
			if	(LogIsActive (_LOG_RESULTS))
			{
				LogWinErr (_LOG_RESULTS, lError);
			}
#endif
			if	(lTreatAs.IsValid ())
			{
				CRegString	lTreatAsClass (lTreatAs, (LPCTSTR)NULL, true);

				if	(lTreatAsClass.Value() == lTreatAsStr)
				{
					lResult = S_FALSE;
				}
				else
				{
					lError = lTreatAsClass.Update (lTreatAsStr);
					if	(lError == ERROR_SUCCESS)
					{
						lResult = S_OK;
					}
					else
					{
#ifdef	_LOG_RESULTS
						if	(LogIsActive (_LOG_RESULTS))
						{
							LogWinErr (_LOG_RESULTS, lError);
						}
#endif
						lResult = HRESULT_FROM_WIN32 (lError);
					}
				}
			}
			else
			{
#ifdef	_LOG_RESULTS
				if	(LogIsActive (_LOG_RESULTS))
				{
					LogWinErr (_LOG_RESULTS, lError);
				}
#endif
				if	(lError != ERROR_SUCCESS)
				{
					lResult = HRESULT_FROM_WIN32 (lError);
				}
			}
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaElevatedSettings::QueryStatus (const GUID *pguidCmdGroup, ULONG cCmds, OLECMD prgCmds[], OLECMDTEXT *pCmdText)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaElevatedSettings::QueryStatus"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = E_NOTIMPL;
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaElevatedSettings::QueryStatus"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaElevatedSettings::Exec (const GUID *pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, VARIANT *pvaIn, VARIANT *pvaOut)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaElevatedSettings::Exec [%u] [%u]"), this, max(m_dwRef,-1), nCmdID, nCmdexecopt);
#endif
	HRESULT	lResult = S_OK;
	VARTYPE	lParmType;
	long	lParmNdx;

	if	(
			(nCmdID == OLECMDID_OPEN)
		&&	(pvaIn != NULL)
		&&	(V_VT (pvaIn) == (VT_SAFEARRAY|VT_ARRAY))
		&&	(SUCCEEDED (SafeArrayGetVartype (pvaIn->parray, &lParmType)))
		&&	(lParmType == VT_BSTR)
		)
	{
		_bstr_t	lParm0;
		_bstr_t	lParm1;
		GUID	lClsId = GUID_NULL;
		GUID	lTreatAsClsId = GUID_NULL;

		if	(
				(SUCCEEDED (lResult = SafeArrayGetElement (pvaIn->parray, &(lParmNdx=0), lParm0.GetAddress())))
			&&	(SUCCEEDED (lResult = SafeArrayGetElement (pvaIn->parray, &(lParmNdx=1), lParm1.GetAddress())))
			)
		{
			lClsId = CGuidStr::Parse ((BSTR)lParm0);
			lTreatAsClsId = CGuidStr::Parse ((BSTR)lParm1);
#ifdef	_DEBUG_INTERFACE
			LogMessage (_DEBUG_INTERFACE, _T("  Treat [%ls] as [%ls]"), (BSTR)lParm0, (BSTR)lParm1);
			LogMessage (_DEBUG_INTERFACE, _T("  Treat [%s] as [%s]"), (CString)CGuidStr(lClsId), (CString)CGuidStr(lTreatAsClsId));
#endif
			lResult = SetTreatAs (lClsId, lTreatAsClsId);
		}
	}
	else
	{
		lResult = E_INVALIDARG;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaElevatedSettings::Exec [%u] [%u]"), this, max(m_dwRef,-1), nCmdID, nCmdexecopt);
	}
#endif
	return lResult;
}
