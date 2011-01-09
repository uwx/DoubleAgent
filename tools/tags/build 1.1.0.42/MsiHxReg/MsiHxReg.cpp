#include "StdAfx.h"
#include "DaVersion.h"
#include "Registry.h"
#include "HxRegHelper.h"
#include "MsiCustomAction.h"

/////////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
#define _LOG_LEVEL_DEBUG				LogNormal
#endif
#define	_LOG_ROOT_PATH					_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME				_T("MsiHxReg")
#define _LOG_DEF_LOGNAME				_LOG_SECTION_NAME _T(".log")
#include "Log.inl"
/////////////////////////////////////////////////////////////////////////////

class CMsiHxRegModule : public CAtlDllModuleT <CMsiHxRegModule>
{
public:
	CMsiHxRegModule ();
	virtual ~CMsiHxRegModule ();

// Operations
};

/////////////////////////////////////////////////////////////////////////////

CMsiHxRegModule::CMsiHxRegModule ()
{
#ifdef	_DEBUG
	LogStart (false);
	LogDebugRuntime ();
#else
	LogStart (false);
#endif
	CoInitialize (NULL);
}

CMsiHxRegModule::~CMsiHxRegModule ()
{
	CoUninitialize ();
	LogStop (LogIfActive);
}

CMsiHxRegModule	_AtlModule;

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllexport) int __stdcall StartTransaction (MSIHANDLE pInstall)
{
	int	lRet = ERROR_SUCCESS;

	try
	{
		CMsiCustomAction	lCustomAction (pInstall);
		tArrayPtr <TCHAR>	lCustomData;

		if	(LogIsActive ())
		{
			lCustomData = lCustomAction.GetActionData ();
			LogMessage (LogNormal, _T("StartTransaction [%u] [%u] [%s]"), lCustomAction.IsInstallMode (), lCustomAction.IsScheduledMode (), (LPCTSTR) lCustomData);
		}

		if	(
				(lCustomAction.IsInstallMode ())
			&&	(lCustomData = lCustomAction.GetActionData ())
			)
		{
			CHxRegHelper		lRegHelper;
			tArrayPtr <TCHAR>	lSessionToken;

			if	(
					(SUCCEEDED (lRegHelper.GetLastResult ()))
				&&	(SUCCEEDED (lRegHelper.CreateTransaction (lSessionToken = lCustomAction.GetActionDataPart (lCustomData, 0))))
				&&	(SUCCEEDED (lRegHelper.PostponeTransaction ()))
				)
			{
				if	(lCustomAction.IsLogEnabled ())
				{
					lCustomAction.LogInfoMessage (FormatString (_T(">StartTransaction %1"), lRegHelper.GetSessionToken()));
				}
			}
			else
			{
				lRet = HRESULT_CODE (lRegHelper.GetLastResult ());
			}
		}
		else
		{
			lRet = ERROR_INVALID_FUNCTION;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllexport) int __stdcall CommitTransaction (MSIHANDLE pInstall)
{
	int	lRet = ERROR_SUCCESS;

	try
	{
		CMsiCustomAction	lCustomAction (pInstall);
		tArrayPtr <TCHAR>	lCustomData;

		if	(LogIsActive ())
		{
			lCustomData = lCustomAction.GetActionData ();
			LogMessage (LogNormal, _T("CommitTransaction [%u] [%u] [%u] [%s]"), lCustomAction.IsInstallMode (), lCustomAction.IsCommitMode (), lCustomAction.IsScheduledMode (), (LPCTSTR) lCustomData);
		}

		if	(
				(
					(lCustomAction.IsInstallMode ())
				||	(lCustomAction.IsCommitMode ())
				)
			&&	(lCustomData = lCustomAction.GetActionData ())
			)
		{
			CHxRegHelper		lRegHelper;
			tArrayPtr <TCHAR>	lSessionToken;

			if	(
					(SUCCEEDED (lRegHelper.GetLastResult ()))
				&&	(SUCCEEDED (lRegHelper.SetSessionToken (lSessionToken = lCustomAction.GetActionDataPart (lCustomData, 0))))
				&&	(SUCCEEDED (lRegHelper.CommitTransaction ()))
				)
			{
				if	(lCustomAction.IsLogEnabled ())
				{
					lCustomAction.LogInfoMessage (FormatString (_T(">CommitTransaction %1"), (BSTR)lSessionToken));
				}
			}
			else
			{
				lRet = HRESULT_CODE (lRegHelper.GetLastResult ());
			}
		}
		else
		{
			lRet = ERROR_INVALID_FUNCTION;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllexport) int __stdcall RollbackTransaction (MSIHANDLE pInstall)
{
	int	lRet = ERROR_SUCCESS;

	try
	{
		CMsiCustomAction	lCustomAction (pInstall);
		tArrayPtr <TCHAR>	lCustomData;

		if	(LogIsActive ())
		{
			lCustomData = lCustomAction.GetActionData ();
			LogMessage (LogNormal, _T("RollbackTransaction [%u] [%u] [%s]"), lCustomAction.IsRollbackMode (), lCustomAction.IsScheduledMode (), (LPCTSTR) lCustomData);
		}

		if	(
				(lCustomAction.IsRollbackMode ())
			&&	(lCustomData = lCustomAction.GetActionData ())
			)
		{
			CHxRegHelper		lRegHelper;
			tArrayPtr <TCHAR>	lSessionToken;

			if	(
					(SUCCEEDED (lRegHelper.GetLastResult ()))
				&&	(SUCCEEDED (lRegHelper.SetSessionToken (lSessionToken = lCustomAction.GetActionDataPart (lCustomData, 0))))
				&&	(SUCCEEDED (lRegHelper.RevertTransaction ()))
				)
			{
				if	(lCustomAction.IsLogEnabled ())
				{
					lCustomAction.LogInfoMessage (FormatString (_T(">RollbackTransaction %1"), (BSTR)lSessionToken));
				}
			}
			else
			{
				lRet = HRESULT_CODE (lRegHelper.GetLastResult ());
			}
		}
		else
		{
			lRet = ERROR_INVALID_FUNCTION;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllexport) int __stdcall RegisterNamespace (MSIHANDLE pInstall)
{
	int	lRet = ERROR_SUCCESS;

	try
	{
		CMsiCustomAction	lCustomAction (pInstall);
		tArrayPtr <TCHAR>	lCustomData;

		if	(LogIsActive ())
		{
			lCustomData = lCustomAction.GetActionData ();
			LogMessage (LogNormal, _T("RegisterNamespace [%u] [%u] [%s]"), lCustomAction.IsInstallMode (), lCustomAction.IsScheduledMode (), (LPCTSTR) lCustomData);
		}

		if	(
				(lCustomAction.IsInstallMode ())
			&&	(lCustomData = lCustomAction.GetActionData ())
			)
		{
			int					lDataPart = 0;
			tArrayPtr <TCHAR>	lNamespaceName;
			tArrayPtr <TCHAR>	lNamespaceCollection;
			tArrayPtr <TCHAR>	lNamespaceFile;
			tArrayPtr <TCHAR>	lNamespaceDescription;
			LANGID				lNamespaceLangID = 1033;

			if	(
					(lNamespaceName = lCustomAction.GetActionDataPart (lCustomData, ++lDataPart))
				&&	(lNamespaceCollection = lCustomAction.GetActionDataPart (lCustomData, ++lDataPart))
				&&	(lNamespaceFile = lCustomAction.GetActionDataPart (lCustomData, ++lDataPart))
				)
			{
				CHxRegHelper		lRegHelper;
				tArrayPtr <TCHAR>	lSessionToken;

				if	(lNamespaceDescription = lCustomAction.GetActionDataPart (lCustomData, ++lDataPart))
				{
					lNamespaceLangID = (LANGID) _tcstoul (lNamespaceDescription, NULL, 10);
				}
				lNamespaceDescription = lCustomAction.GetActionDataPart (lCustomData, ++lDataPart);

				if	(
						(SUCCEEDED (lRegHelper.GetLastResult ()))
					&&	(SUCCEEDED (lRegHelper.SetSessionToken (lSessionToken = lCustomAction.GetActionDataPart (lCustomData, 0))))
					)
				{
					if	(SUCCEEDED (lRegHelper.RegisterNamespace (lNamespaceName, lNamespaceDescription, lNamespaceCollection, lNamespaceFile, lNamespaceLangID)))
					{
						if	(lCustomAction.IsLogEnabled ())
						{
							lCustomAction.LogInfoMessage (FormatString (_T(">RegisterNamespace %1 \"%2\" Collection %3"), (LPCTSTR)lNamespaceName, (BSTR)lNamespaceDescription, (LPCTSTR)lNamespaceCollection));
						}
					}
					lRegHelper.PostponeTransaction ();
				}
				else
				{
					lRet = HRESULT_CODE (lRegHelper.GetLastResult ());
				}
			}
			else
			{
				lRet = ERROR_INVALID_PARAMETER;
			}
		}
		else
		{
			lRet = ERROR_INVALID_FUNCTION;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllexport) int __stdcall UnregisterNamespace (MSIHANDLE pInstall)
{
	int	lRet = ERROR_SUCCESS;

	try
	{
		CMsiCustomAction	lCustomAction (pInstall);
		tArrayPtr <TCHAR>	lCustomData;
		tArrayPtr <TCHAR>	lNamespaceName;

		if	(LogIsActive ())
		{
			lCustomData = lCustomAction.GetActionData ();
			LogMessage (LogNormal, _T("UnregisterNamespace [%u] [%u] [%s]"), lCustomAction.IsInstallMode (), lCustomAction.IsScheduledMode (), (LPCTSTR) lCustomData);
		}

		if	(
				(lCustomAction.IsInstallMode ())
			&&	(lCustomData = lCustomAction.GetActionData ())
			)
		{
			if	(lNamespaceName = lCustomAction.GetActionDataPart (lCustomData, 1))
			{
				CHxRegHelper		lRegHelper;
				tArrayPtr <TCHAR>	lSessionToken;

				if	(
						(SUCCEEDED (lRegHelper.GetLastResult ()))
					&&	(SUCCEEDED (lRegHelper.SetSessionToken (lSessionToken = lCustomAction.GetActionDataPart (lCustomData, 0))))
					)
				{
					if	(SUCCEEDED (lRegHelper.UnregisterNamespace (lNamespaceName)))
					{
						if	(lCustomAction.IsLogEnabled ())
						{
							lCustomAction.LogInfoMessage (FormatString (_T(">UnregisterNamespace %1"), (LPCTSTR)lNamespaceName));
						}
					}
					lRegHelper.PostponeTransaction ();
				}
				else
				{
					lRet = HRESULT_CODE (lRegHelper.GetLastResult ());
				}
			}
			else
			{
				lRet = ERROR_INVALID_PARAMETER;
			}
		}
		else
		{
			lRet = ERROR_INVALID_FUNCTION;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllexport) int __stdcall RegisterFilter (MSIHANDLE pInstall)
{
	int	lRet = ERROR_SUCCESS;

	try
	{
		CMsiCustomAction	lCustomAction (pInstall);
		tArrayPtr <TCHAR>	lCustomData;
		tArrayPtr <TCHAR>	lNamespaceName;

		if	(LogIsActive ())
		{
			lCustomData = lCustomAction.GetActionData ();
			LogMessage (LogNormal, _T("RegisterFilters [%u] [%u] [%s]"), lCustomAction.IsInstallMode (), lCustomAction.IsScheduledMode (), (LPCTSTR) lCustomData);
		}

		if	(
				(lCustomAction.IsInstallMode ())
			&&	(lCustomData = lCustomAction.GetActionData ())
			)
		{
			if	(lNamespaceName = lCustomAction.GetActionDataPart (lCustomData, 1))
			{
				CHxRegHelper		lRegHelper;
				tArrayPtr <TCHAR>	lSessionToken;

				if	(
						(SUCCEEDED (lRegHelper.GetLastResult ()))
					&&	(SUCCEEDED (lRegHelper.SetSessionToken (lSessionToken = lCustomAction.GetActionDataPart (lCustomData, 0))))
					)
				{
					try
					{
						int					lDataPart = 2;
						tArrayPtr <TCHAR>	lFilterName;
						tArrayPtr <TCHAR>	lFilterQuery;

						while	(
									(lFilterName = lCustomAction.GetActionDataPart (lCustomData, lDataPart++))
								&&	(lFilterQuery = lCustomAction.GetActionDataPart (lCustomData, lDataPart++))
								)
						{
							if	(SUCCEEDED (lRegHelper.RegisterFilter (lNamespaceName, lFilterName, lFilterQuery)))
							{
							}
						}
					}
					catch AnyExceptionDebug

					lRegHelper.PostponeTransaction ();
				}
				else
				{
					lRet = HRESULT_CODE (lRegHelper.GetLastResult ());
				}
			}
			else
			{
				lRet = ERROR_INVALID_PARAMETER;
			}
		}
		else
		{
			lRet = ERROR_INVALID_FUNCTION;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllexport) int __stdcall RegisterPlugin (MSIHANDLE pInstall)
{
	int	lRet = ERROR_SUCCESS;

	try
	{
		CMsiCustomAction	lCustomAction (pInstall);
		tArrayPtr <TCHAR>	lCustomData;

		if	(LogIsActive ())
		{
			lCustomData = lCustomAction.GetActionData ();
			LogMessage (LogNormal, _T("RegisterPlugin [%u] [%u] [%s]"), lCustomAction.IsInstallMode (), lCustomAction.IsScheduledMode (), (LPCTSTR) lCustomData);
		}

		if	(
				(lCustomAction.IsInstallMode ())
			&&	(lCustomData = lCustomAction.GetActionData ())
			)
		{
			CHxRegHelper		lRegHelper;
			tArrayPtr <TCHAR>	lSessionToken;

			if	(
					(SUCCEEDED (lRegHelper.GetLastResult ()))
				&&	(SUCCEEDED (lRegHelper.SetSessionToken (lSessionToken = lCustomAction.GetActionDataPart (lCustomData, 0))))
				)
			{
				try
				{
					int					lDataPart = 1;
					tArrayPtr <TCHAR>	lParentName;
					tArrayPtr <TCHAR>	lParentFile;
					tArrayPtr <TCHAR>	lChildName;
					tArrayPtr <TCHAR>	lChildFile;

					while	(
								(lParentName = lCustomAction.GetActionDataPart (lCustomData, lDataPart++))
							&&	(lParentFile = lCustomAction.GetActionDataPart (lCustomData, lDataPart++))
							&&	(lChildName = lCustomAction.GetActionDataPart (lCustomData, lDataPart++))
							&&	(lChildFile = lCustomAction.GetActionDataPart (lCustomData, lDataPart++))
							)
					{
						if	(SUCCEEDED (lRegHelper.RegisterPlugin (lParentName, lParentFile, lChildName, lChildFile)))
						{
							if	(lCustomAction.IsLogEnabled ())
							{
								lCustomAction.LogInfoMessage (FormatString (_T(">RegisterPlugin %1 in %2"), (LPCTSTR)lChildName, (LPCTSTR)lParentName));
							}
						}
					}
				}
				catch AnyExceptionDebug

				lRegHelper.PostponeTransaction ();
			}
			else
			{
				lRet = HRESULT_CODE (lRegHelper.GetLastResult ());
			}
		}
		else
		{
			lRet = ERROR_INVALID_FUNCTION;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllexport) int __stdcall UnregisterPlugin (MSIHANDLE pInstall)
{
	int	lRet = ERROR_SUCCESS;

	try
	{
		CMsiCustomAction	lCustomAction (pInstall);
		tArrayPtr <TCHAR>	lCustomData;

		if	(LogIsActive ())
		{
			lCustomData = lCustomAction.GetActionData ();
			LogMessage (LogNormal, _T("UnregisterPlugin [%u] [%u] [%s]"), lCustomAction.IsInstallMode (), lCustomAction.IsScheduledMode (), (LPCTSTR) lCustomData);
		}

		if	(
				(lCustomAction.IsInstallMode ())
			&&	(lCustomData = lCustomAction.GetActionData ())
			)
		{
			CHxRegHelper		lRegHelper;
			tArrayPtr <TCHAR>	lSessionToken;

			if	(
					(SUCCEEDED (lRegHelper.GetLastResult ()))
				&&	(SUCCEEDED (lRegHelper.SetSessionToken (lSessionToken = lCustomAction.GetActionDataPart (lCustomData, 0))))
				)
			{
				try
				{
					int					lDataPart = 1;
					tArrayPtr <TCHAR>	lParentName;
					tArrayPtr <TCHAR>	lChildName;

					while	(
								(lParentName = lCustomAction.GetActionDataPart (lCustomData, lDataPart++))
							&&	(lChildName = lCustomAction.GetActionDataPart (lCustomData, lDataPart++))
							)
					{
						if	(SUCCEEDED (lRegHelper.UnregisterPlugin (lParentName, lChildName)))
						{
							if	(lCustomAction.IsLogEnabled ())
							{
								lCustomAction.LogInfoMessage (FormatString (_T(">UnregisterPlugin %1 in %2"), (LPCTSTR)lChildName, (LPCTSTR)lParentName));
							}
						}
					}
				}
				catch AnyExceptionDebug

				lRegHelper.PostponeTransaction ();
			}
			else
			{
				lRet = HRESULT_CODE (lRegHelper.GetLastResult ());
			}
		}
		else
		{
			lRet = ERROR_INVALID_FUNCTION;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}
