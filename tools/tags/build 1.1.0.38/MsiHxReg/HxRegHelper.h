#pragma once

interface __declspec(uuid("31411218-a502-11d2-bbca-00c04f8ec294")) IHxRegisterSession;
interface __declspec(uuid("314111ef-a502-11d2-bbca-00c04f8ec294")) IHxRegistryWalker;
interface __declspec(uuid("314111f3-a502-11d2-bbca-00c04f8ec294")) IHxRegNamespaceList;
interface __declspec(uuid("314111f1-a502-11d2-bbca-00c04f8ec294")) IHxRegNamespace;
_COM_SMARTPTR_TYPEDEF(IHxRegisterSession, __uuidof(IHxRegisterSession));
_COM_SMARTPTR_TYPEDEF(IHxRegistryWalker, __uuidof(IHxRegistryWalker));
_COM_SMARTPTR_TYPEDEF(IHxRegNamespaceList, __uuidof(IHxRegNamespaceList));
_COM_SMARTPTR_TYPEDEF(IHxRegNamespace, __uuidof(IHxRegNamespace));

/////////////////////////////////////////////////////////////////////////////

class CHxRegHelper
{
public:
	CHxRegHelper ();
	virtual ~CHxRegHelper ();
	
public:
	HRESULT GetLastResult () const;
	LPCTSTR GetSessionToken () const;
	
public:
	HRESULT SetSessionToken (LPCTSTR pSessionToken);
	
	HRESULT CreateTransaction (LPCTSTR pSessionToken);
	HRESULT CommitTransaction ();		
	HRESULT RevertTransaction ();		
	HRESULT PostponeTransaction ();
	HRESULT ContinueTransaction ();
	
	HRESULT RegisterNamespace (LPCTSTR pNamespace, LPCTSTR pDescription, LPCTSTR pCollection, LPCTSTR pFileName, LANGID pLangID = 1033);
	HRESULT UnregisterNamespace (LPCTSTR pNamespace);

	HRESULT RegisterFilter (LPCTSTR pNamespace, LPCTSTR pFilterName, LPCTSTR pFilterQuery);
	HRESULT UnregisterFilter (LPCTSTR pNamespace, LPCTSTR pFilterName);
	HRESULT UnregisterNamespaceFilters (LPCTSTR pNamespace);

	HRESULT RegisterPlugin (LPCTSTR pParentNamespace, LPCTSTR pParentFile, LPCTSTR pChildNamespace, LPCTSTR pChildFile);
	HRESULT UnregisterPlugin (LPCTSTR pParentNamespace, LPCTSTR pChildNamespace);
	HRESULT UnregisterNamespacePlugins (LPCTSTR pNamespace);

protected:
	IHxRegNamespacePtr GetRegNamespace (LPCTSTR pNamespace);
	bool GetNamespaceCollection (LPCTSTR pNamespace, CAtlString & pCollectionPath, bool pStripPath = false);
	
protected:
	HRESULT					mLastResult;
	CString					mSessionToken;
	IHxRegisterSessionPtr	mRegisterSession;
	IHxRegistryWalkerPtr	mRegistryWalker;
	IHxRegNamespaceListPtr	mNamespaceList;
};
