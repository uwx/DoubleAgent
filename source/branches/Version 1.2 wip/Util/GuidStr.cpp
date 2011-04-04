/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2011 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is a utility used by Double Agent but not specific to
	Double Agent.  However, it is included as part of the Double Agent
	source code under the following conditions:

    This is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "GuidStr.h"

#ifdef	__AFX_H__
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

//////////////////////////////////////////////////////////////////////

CGuidStr::CGuidStr (REFGUID pGuid)
{
	OLECHAR	lGuidStr [50];

	StringFromGUID2 (pGuid, lGuidStr, sizeof (lGuidStr) / sizeof (OLECHAR));
	mValue = lGuidStr;
	mValue.MakeUpper ();
}

GUID CGuidStr::Parse (LPCTSTR pGuidStr)
{
	GUID	lGuid;

	if	(pGuidStr)
	{
#ifdef	_UNICODE
		if	(IIDFromString ((LPOLESTR) pGuidStr, &lGuid) != S_OK)
		{
			lGuid = GUID_NULL;
		}
#else
		OLECHAR	lGuidStr [50];
		MultiByteToWideChar (CP_ACP, 0, pGuidStr, -1, lGuidStr, sizeof (lGuidStr) / sizeof (OLECHAR));
		if	(IIDFromString (lGuidStr, &lGuid) != S_OK)
		{
			lGuid = GUID_NULL;
		}
#endif
	}
	else
	{
		lGuid = GUID_NULL;
	}

	return lGuid;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG
#include <ComDef.h>
#include <ShObjIdl.h>
#include <ShlGuid.h>
#include <ExDisp.h>
#include <OleAcc.h>
#include <MsHtml.h>
#include <UrlMon.h>
#include <uuids.h>
#include <AgtCtl.h>
#include <AgtSvr.h>
#include <mmreg.h>
#include <mmsystem.h>
#include <strmif.h>
#include <control.h>
#include <ks.h>
#include <ksmedia.h>
#include <InitGuid.h>
#include <vfw.h>
#if	(_MSC_VER >= 1400)
#include <shdeprecated.h>
#endif

#if	(_WIN32_IE >= 0x600)
interface __declspec(uuid("{90B4135A-95BA-46EA-8CAA-E05B45CD801E}")) ILayoutModifier;
interface __declspec(uuid("{2539E31C-857F-43C4-8872-45BD6A024892}")) IFileDialogPrivate;
interface __declspec(uuid("{F5A24314-5B8B-44FA-BC2E-31285544B520}")) IShellBrowserService;
interface __declspec(uuid("{711B2CFD-93D1-422B-BDF4-69BE923F2449}")) IShellFolder3;
interface __declspec(uuid("{321A6A6A-D61F-4BF3-97AE-14BE2986BB36}")) IObjectWithBackReferences;
interface __declspec(uuid("{6A9D9026-0E6E-464C-B000-42ECC07DE673}")) IObjectWithFolderEnumMode;
interface __declspec(uuid("{9D264146-A94F-4195-9F9F-3BB12CE0C955}")) IViewStateIdentityItem;
interface __declspec(uuid("{527832F6-5FB1-414D-86CC-5BC1DA0F4ED9}")) IObjectWithGITCookie;
interface __declspec(uuid("{86187C37-E662-4D1E-A122-7478676D7E6E}")) ILibraryDescription;
interface __declspec(uuid("{8279FEB8-5CA4-45C4-BE27-770DCDEA1DEB}")) ITopViewAwareItem;
interface __declspec(uuid("{176C11B1-4302-4164-8430-D5A9F0EEACDB}")) IFrameLayoutDefinition;
interface __declspec(uuid("{7E734121-F3B4-45F9-AD43-2FBE39E533E2}")) IFrameLayoutDefinitionFactory;
interface __declspec(uuid("{32AE3A1F-D90E-4417-9DD9-23B0DFA4621D}")) IItemSetOperations;
interface __declspec(uuid("{24D16EE5-10F5-4DE3-8766-D23779BA7A6D}")) INewItemAdvisor;
interface __declspec(uuid("{A08CE4D0-FA25-44AB-B57C-C7B1C323E0B9}")) IExplorerCommand;
interface __declspec(uuid("{BDDACB60-7657-47AE-8445-D23E1ACF82AE}")) IExplorerCommandState;
interface __declspec(uuid("{64961751-0835-43C0-8FFE-D57686530E64}")) IExplorerCommandProvider;
interface __declspec(uuid("{A88826F8-186F-4987-AADE-EA0CEF8FBFE8}")) IEnumExplorerCommand;
interface __declspec(uuid("{85075ACF-231F-40EA-9610-D26B7B58F638}")) IInitializeCommand;
interface __declspec(uuid("{1C9CD5BB-98E9-4491-A60F-31AACC72B83C}")) IObjectWithSelection;
interface __declspec(uuid("{36149969-0A8F-49C8-8B00-4AECB20222FB}")) IPreviewItem;
interface __declspec(uuid("{7F73BE3F-FB79-493C-A6C7-7EE14E245841}")) IInitializeWithItem;
interface __declspec(uuid("{B824B49D-22AC-4161-AC8A-9916E8FA3F7F}")) IInitializeWithStream;
interface __declspec(uuid("{B7D14566-0509-4CCE-A71F-0A554233BD9B}")) IInitializeWithFile;
#endif

#pragma comment(lib, "strmiids.lib")

#ifdef	__AFXCOLL_H__
inline UINT AFXAPI HashKey<> (const GUID& key)
{
	return ((UINT)key.Data1) >> 4;
}
#endif
#endif

//////////////////////////////////////////////////////////////////////

CString CGuidStr::GuidName (REFGUID pGuid)
{
	CString lRet;

#ifdef	_DEBUG
#ifdef	__AFXCOLL_H__
	static CMap <GUID, const GUID&, CString, LPCTSTR>	lGuidMap;
#else
#ifdef	__ATLCOLL_H__
	static CAtlMap <GUID, CString> lGuidMap;
#endif
#endif

	if	(lGuidMap.IsEmpty ())
	{
		lGuidMap.SetAt (GUID_NULL, _T("GUID_NULL"));
		lGuidMap.SetAt (__uuidof (IUnknown), _T("IID_IUnknown"));
		lGuidMap.SetAt (__uuidof (IDispatch), _T("IID_IDispatch"));
//		lGuidMap.SetAt (__uuidof (IDispatchEx), _T("IID_IDispatchEx"));
		lGuidMap.SetAt (__uuidof (IProvideClassInfo), _T("IID_IProvideClassInfo"));
		lGuidMap.SetAt (__uuidof (IProvideClassInfo2), _T("IID_IProvideClassInfo2"));
		lGuidMap.SetAt (IID_IDataObject, _T("IID_IDataObject"));
		lGuidMap.SetAt (IID_IDropSource, _T("IID_IDropSource"));
		lGuidMap.SetAt (IID_IDropTarget, _T("IID_IDropTarget"));
		lGuidMap.SetAt (IID_IMarshal, _T("IID_IMarshal"));
		lGuidMap.SetAt (IID_IStdMarshalInfo, _T("IID_IStdMarshalInfo"));
//		lGuidMap.SetAt (__uuidof (IdentityUnmarshal), _T("IID_IdentityUnmarshal"));
		lGuidMap.SetAt (IID_IExternalConnection, _T("IID_IExternalConnection"));
		lGuidMap.SetAt (IID_IMultiQI, _T("IID_IMultiQI"));
#ifdef	_UNICODE
		lGuidMap.SetAt (IID_IClientSecurity, _T("IID_IClientSecurity"));
#endif
		lGuidMap.SetAt (IID_IObjectWithSite, _T("IID_IObjectWithSite"));
		lGuidMap.SetAt (IID_IOleCommandTarget, _T("IID_IOleCommandTarget"));
		lGuidMap.SetAt (IID_IOleWindow, _T("IID_IOleWindow"));
		lGuidMap.SetAt (IID_IOleObject, _T("IID_IOleObject"));
		lGuidMap.SetAt (IID_IOleControl, _T("IID_IOleControl"));
		lGuidMap.SetAt (IID_IQuickActivate, _T("IID_IQuickActivate"));
		lGuidMap.SetAt (IID_IViewObject2, _T("IID_IViewObject2"));
		lGuidMap.SetAt (IID_IOleInPlaceObject, _T("IID_IOleInPlaceObject"));
		lGuidMap.SetAt (IID_IRunnableObject, _T("IID_IRunnableObject"));
		lGuidMap.SetAt (IID_IConnectionPointContainer, _T("IID_IConnectionPointContainer"));
		lGuidMap.SetAt (IID_ICallFactory, _T("IID_ICallFactory"));
		lGuidMap.SetAt (IID_IAccIdentity, _T("IID_IAccIdentity"));
		lGuidMap.SetAt (IID_IAccessible, _T("IID_IAccessible"));
		lGuidMap.SetAt (IID_IEnumVARIANT, _T("IID_IEnumVARIANT"));
		lGuidMap.SetAt (IID_IStream, _T("IID_IStream"));
		lGuidMap.SetAt (IID_IStorage, _T("IID_IStorage"));
		lGuidMap.SetAt (IID_IPersist, _T("IID_IPersist"));
		lGuidMap.SetAt (IID_IPersistFile, _T("IID_IPersistFile"));
		lGuidMap.SetAt (IID_IPersistStream, _T("IID_IPersistStream"));
		lGuidMap.SetAt (IID_IPersistStreamInit, _T("IID_IPersistStreamInit"));
		lGuidMap.SetAt (IID_IPersistFolder, _T("IID_IPersistFolder"));
		lGuidMap.SetAt (IID_IPersistFolder2, _T("IID_IPersistFolder2"));
#if	(_WIN32_IE >= 0x600)
		lGuidMap.SetAt (IID_IPersistFolder3, _T("IID_IPersistFolder3"));
		lGuidMap.SetAt (IID_IPersistIDList, _T("IID_IPersistIDList"));
		lGuidMap.SetAt (__uuidof (ILayoutModifier), _T("IID_ILayoutModifier"));
		lGuidMap.SetAt (__uuidof (IFileDialogPrivate), _T("IFileDialogPrivate"));
		lGuidMap.SetAt (__uuidof (IShellBrowserService), _T("IID_IShellBrowserService"));
		lGuidMap.SetAt (__uuidof (IShellFolder3), _T("IID_IShellFolder3"));
		lGuidMap.SetAt (__uuidof (IObjectWithBackReferences), _T("IID_IObjectWithBackReferences"));
		lGuidMap.SetAt (__uuidof (IObjectWithFolderEnumMode), _T("IID_IObjectWithFolderEnumMode"));
		lGuidMap.SetAt (__uuidof (IViewStateIdentityItem), _T("IID_IViewStateIdentityItem"));
		lGuidMap.SetAt (__uuidof (IObjectWithGITCookie), _T("IID_IObjectWithGITCookie"));
		lGuidMap.SetAt (__uuidof (ILibraryDescription), _T("IID_ILibraryDescription"));
		lGuidMap.SetAt (__uuidof (ITopViewAwareItem), _T("IID_ITopViewAwareItem"));
		lGuidMap.SetAt (__uuidof (IFrameLayoutDefinition), _T("IID_IFrameLayoutDefinition"));
		lGuidMap.SetAt (__uuidof (IFrameLayoutDefinitionFactory), _T("IID_IFrameLayoutDefinitionFactory"));
		lGuidMap.SetAt (__uuidof (IItemSetOperations), _T("IID_IItemSetOperations"));
		lGuidMap.SetAt (__uuidof (INewItemAdvisor), _T("IID_INewItemAdvisor"));
		lGuidMap.SetAt (__uuidof (IExplorerCommand), _T("IID_IExplorerCommand"));
		lGuidMap.SetAt (__uuidof (IExplorerCommandState), _T("IID_IExplorerCommandState"));
		lGuidMap.SetAt (__uuidof (IExplorerCommandProvider), _T("IID_IExplorerCommandProvider"));
		lGuidMap.SetAt (__uuidof (IEnumExplorerCommand), _T("IID_IEnumExplorerCommand"));
		lGuidMap.SetAt (__uuidof (IInitializeCommand), _T("IID_IInitializeCommand"));
		lGuidMap.SetAt (__uuidof (IObjectWithSelection), _T("IID_IObjectWithSelection"));
		lGuidMap.SetAt (__uuidof (IPreviewItem), _T("IID_IPreviewItem"));
		lGuidMap.SetAt (__uuidof (IInitializeWithItem), _T("IID_IInitializeWithItem"));
		lGuidMap.SetAt (__uuidof (IInitializeWithStream), _T("IID_IInitializeWithStream"));
		lGuidMap.SetAt (__uuidof (IInitializeWithFile), _T("IID_IInitializeWithFile"));
#endif
//		lGuidMap.SetAt (__uuidof (IPersistHistory), _T("IID_IPersistHistory"));
		lGuidMap.SetAt (__uuidof (IPropertyBag), _T("IID_IPropertyBag"));
		lGuidMap.SetAt (__uuidof (IPersistPropertyBag), _T("IID_IPersistPropertyBag"));
		lGuidMap.SetAt (__uuidof (IPropertyStorage), _T("IID_IPropertyStorage"));
		lGuidMap.SetAt (IID_IPropertySetStorage, _T("IID_IPropertySetStorage"));
		lGuidMap.SetAt (IID_IShellFolder, _T("IID_IShellFolder"));
		lGuidMap.SetAt (IID_IShellFolder2, _T("IID_IShellFolder2"));
		lGuidMap.SetAt (IID_IShellFolderBand, _T("IID_IShellFolderBand"));
		lGuidMap.SetAt (IID_IShellView, _T("IID_IShellView"));
		lGuidMap.SetAt (IID_IShellBrowser, _T("IID_IShellBrowser"));
		lGuidMap.SetAt (IID_ICommDlgBrowser, _T("IID_ICommDlgBrowser"));
		lGuidMap.SetAt (IID_ICommDlgBrowser2, _T("IID_ICommDlgBrowser2"));
		lGuidMap.SetAt (IID_IShellFolderViewCB, _T("IID_IShellFolderViewCB"));
		lGuidMap.SetAt (IID_IShellDetails, _T("IID_IShellDetails"));
		lGuidMap.SetAt (IID_IShellLinkA, _T("IID_IShellLinkA"));
		lGuidMap.SetAt (IID_IShellLinkW, _T("IID_IShellLinkW"));
		lGuidMap.SetAt (IID_IResolveShellLink, _T("IID_IResolveShellLink"));
		lGuidMap.SetAt (IID_IShellItem, _T("IID_IShellItem"));
		lGuidMap.SetAt (IID_IShellIcon, _T("IID_IShellIcon"));
		lGuidMap.SetAt (IID_IShellIconOverlay, _T("IID_IShellIconOverlay"));
		lGuidMap.SetAt (IID_IShellExtInit, _T("IID_IShellExtInit"));
		lGuidMap.SetAt (IID_IContextMenu, _T("IID_IContextMenu"));
		lGuidMap.SetAt (IID_IExtractIconA, _T("IID_IExtractIconA"));
		lGuidMap.SetAt (IID_IExtractIconW, _T("IID_IExtractIconW"));
		lGuidMap.SetAt (IID_IQueryInfo, _T("IID_IQueryInfo"));
		lGuidMap.SetAt (IID_IQueryAssociations, _T("IID_IQueryAssociations"));
#if (_WIN32_IE >= 0x0500)
		lGuidMap.SetAt (IID_ICategoryProvider, _T("IID_ICategoryProvider"));
#endif
#if	(_WIN32_IE >= 0x0600)
		lGuidMap.SetAt (IID_IBrowserFrameOptions, _T("IID_IBrowserFrameOptions"));
		lGuidMap.SetAt (IID_ITaskbarList, _T("IID_ITaskbarList"));
		lGuidMap.SetAt (IID_ITaskbarList2, _T("IID_ITaskbarList2"));
		lGuidMap.SetAt (IID_IFolderView, _T("IID_IFolderView"));
		lGuidMap.SetAt (IID_IFolderFilter, _T("IID_IFolderFilter"));
		lGuidMap.SetAt (IID_IFolderFilterSite, _T("IID_IFolderFilterSite"));
		lGuidMap.SetAt (CLSID_ISFBand, _T("CLSID_ISFBand"));
		lGuidMap.SetAt (IID_IInternetSecurityManager, _T("IID_IInternetSecurityManager"));
#endif
#if	(_WIN32_IE >= 0x0700)
		lGuidMap.SetAt (IID_IFolderView2, _T("IID_IFolderView2"));
		lGuidMap.SetAt (IID_IShellItemFilter, _T("IID_IShellItemFilter"));
		lGuidMap.SetAt (IID_IIdentityName, _T("IID_IIdentityName"));
		lGuidMap.SetAt (IID_IParentAndItem, _T("IID_IParentAndItem"));
		lGuidMap.SetAt (IID_IExplorerBrowser, _T("IID_IExplorerBrowser"));
		lGuidMap.SetAt (IID_IExplorerPaneVisibility, _T("IID_IExplorerPaneVisibility"));
		lGuidMap.SetAt (IID_ITransferDestination, _T("IID_ITransferDestination"));
		lGuidMap.SetAt (IID_IPropertyStore, _T("IID_IPropertyStore"));
		lGuidMap.SetAt (IID_IPropertyStoreCache, _T("IID_IPropertyStoreCache"));
		lGuidMap.SetAt (IID_IPropertyStoreFactory, _T("IID_IPropertyStoreFactory"));
#endif
		lGuidMap.SetAt (IID_IViewObject, _T("IID_IViewObject"));
		lGuidMap.SetAt (IID_IServiceProvider, _T("IID_IServiceProvider"));
		lGuidMap.SetAt (IID_IProfferService, _T("IID_IProfferService"));
		lGuidMap.SetAt (IID_IDeskBand, _T("IID_IDeskBand"));
		lGuidMap.SetAt (IID_IBandSite, _T("IID_IBandSite"));
		lGuidMap.SetAt (IID_IDockingWindow, _T("IID_IDockingWindow"));
		lGuidMap.SetAt (IID_IDockingWindowSite, _T("IID_IDockingWindowSite"));
		lGuidMap.SetAt (IID_IInputObject, _T("IID_IInputObject"));
		lGuidMap.SetAt (IID_IInputObjectSite, _T("IID_IInputObjectSite"));
		lGuidMap.SetAt (CGID_Explorer, _T("CGID_Explorer"));
		lGuidMap.SetAt (CGID_ShellDocView, _T("CGID_ShellDocView"));
		lGuidMap.SetAt (CGID_ShellServiceObject, _T("CGID_ShellServiceObject"));
		lGuidMap.SetAt (CGID_ExplorerBarDoc, _T("CGID_ExplorerBarDoc"));
		lGuidMap.SetAt (SID_STopLevelBrowser, _T("SID_STopLevelBrowser"));
		lGuidMap.SetAt (SID_SMenuBandParent, _T("SID_SMenuBandParent"));
		lGuidMap.SetAt (SID_SMenuPopup, _T("SID_SMenuPopup"));
		lGuidMap.SetAt (IID_IShellMenu, _T("IID_IShellMenu"));
		//lGuidMap.SetAt (IID_IBrowserService, _T("IID_IBrowserService"));
		lGuidMap.SetAt (IID_IWebBrowserApp, _T("IID_IWebBrowserApp"));
		lGuidMap.SetAt (IID_IWebBrowser2, _T("IID_IWebBrowser2"));
		lGuidMap.SetAt (DIID_DWebBrowserEvents2, _T("DIID_DWebBrowserEvents2"));

		lGuidMap.SetAt (BHID_SFObject, _T("BHID_SFObject"));
		lGuidMap.SetAt (BHID_SFUIObject, _T("BHID_SFUIObject"));
		lGuidMap.SetAt (BHID_SFViewObject, _T("BHID_SFViewObject"));
		lGuidMap.SetAt (BHID_Storage, _T("BHID_Storage"));
		lGuidMap.SetAt (BHID_Stream, _T("BHID_Stream"));
		lGuidMap.SetAt (BHID_LinkTargetItem, _T("BHID_LinkTargetItem"));
		lGuidMap.SetAt (BHID_StorageEnum, _T("BHID_StorageEnum"));
		lGuidMap.SetAt (BHID_Transfer, _T("BHID_Transfer"));
		lGuidMap.SetAt (BHID_PropertyStore, _T("BHID_PropertyStore"));
		lGuidMap.SetAt (BHID_ThumbnailHandler, _T("BHID_ThumbnailHandler"));
		lGuidMap.SetAt (BHID_EnumItems, _T("BHID_EnumItems"));
		lGuidMap.SetAt (BHID_DataObject, _T("BHID_DataObject"));
		lGuidMap.SetAt (BHID_AssociationArray, _T("BHID_AssociationArray"));
		lGuidMap.SetAt (BHID_Filter, _T("BHID_Filter"));

#if	(_WIN32_IE >= 0x0700)
		lGuidMap.SetAt (EP_NavPane, _T("EP_NavPane"));
		lGuidMap.SetAt (EP_Commands, _T("EP_Commands"));
		lGuidMap.SetAt (EP_Commands_Organize, _T("EP_Commands_Organize"));
		lGuidMap.SetAt (EP_Commands_View, _T("EP_Commands_View"));
		lGuidMap.SetAt (EP_DetailsPane, _T("EP_DetailsPane"));
		lGuidMap.SetAt (EP_PreviewPane, _T("EP_PreviewPane"));
		lGuidMap.SetAt (EP_QueryPane, _T("EP_QueryPane"));
		lGuidMap.SetAt (EP_AdvQueryPane, _T("EP_AdvQueryPane"));
#endif

		lGuidMap.SetAt (IID_IHTMLFiltersCollection, _T("IID_IHTMLFiltersCollection"));
		lGuidMap.SetAt (IID_IHTMLEventObj, _T("IID_IHTMLEventObj"));
		lGuidMap.SetAt (IID_IElementBehaviorSite, _T("IID_IElementBehaviorSite"));
		lGuidMap.SetAt (IID_IElementBehavior, _T("IID_IElementBehavior"));
		lGuidMap.SetAt (IID_IElementBehaviorFactory, _T("IID_IElementBehaviorFactory"));
		lGuidMap.SetAt (IID_IElementBehaviorSiteOM, _T("IID_IElementBehaviorSiteOM"));
		lGuidMap.SetAt (IID_IElementBehaviorRender, _T("IID_IElementBehaviorRender"));
		lGuidMap.SetAt (IID_IElementBehaviorSiteRender, _T("IID_IElementBehaviorSiteRender"));
		lGuidMap.SetAt (IID_IHTMLStyle, _T("IID_IHTMLStyle"));
		lGuidMap.SetAt (IID_IHTMLStyle2, _T("IID_IHTMLStyle2"));
		lGuidMap.SetAt (IID_IHTMLStyle3, _T("IID_IHTMLStyle3"));
		lGuidMap.SetAt (IID_IHTMLStyle4, _T("IID_IHTMLStyle4"));
		lGuidMap.SetAt (IID_IHTMLRuleStyle, _T("IID_IHTMLRuleStyle"));
		lGuidMap.SetAt (IID_IHTMLRuleStyle2, _T("IID_IHTMLRuleStyle2"));
		lGuidMap.SetAt (IID_IHTMLRuleStyle3, _T("IID_IHTMLRuleStyle3"));
		lGuidMap.SetAt (IID_IHTMLRuleStyle4, _T("IID_IHTMLRuleStyle4"));
		lGuidMap.SetAt (DIID_DispHTMLStyle, _T("DIID_DispHTMLStyle"));
		lGuidMap.SetAt (DIID_DispHTMLRuleStyle, _T("DIID_DispHTMLRuleStyle"));
		lGuidMap.SetAt (IID_IHTMLRenderStyle, _T("IID_IHTMLRenderStyle"));
		lGuidMap.SetAt (DIID_DispHTMLRenderStyle, _T("DIID_DispHTMLRenderStyle"));
		lGuidMap.SetAt (IID_IHTMLCurrentStyle, _T("IID_IHTMLCurrentStyle"));
		lGuidMap.SetAt (IID_IHTMLCurrentStyle2, _T("IID_IHTMLCurrentStyle2"));
		lGuidMap.SetAt (IID_IHTMLCurrentStyle3, _T("IID_IHTMLCurrentStyle3"));
		lGuidMap.SetAt (DIID_DispHTMLCurrentStyle, _T("DIID_DispHTMLCurrentStyle"));
		lGuidMap.SetAt (IID_IHTMLElement, _T("IID_IHTMLElement"));
		lGuidMap.SetAt (IID_IHTMLRect, _T("IID_IHTMLRect"));
		lGuidMap.SetAt (IID_IHTMLRectCollection, _T("IID_IHTMLRectCollection"));
		lGuidMap.SetAt (IID_IHTMLDOMNode, _T("IID_IHTMLDOMNode"));
		lGuidMap.SetAt (IID_IHTMLDOMNode2, _T("IID_IHTMLDOMNode2"));
		lGuidMap.SetAt (IID_IHTMLDOMAttribute, _T("IID_IHTMLDOMAttribute"));
		lGuidMap.SetAt (IID_IHTMLDOMAttribute2, _T("IID_IHTMLDOMAttribute2"));
		lGuidMap.SetAt (IID_IHTMLDOMTextNode, _T("IID_IHTMLDOMTextNode"));
		lGuidMap.SetAt (IID_IHTMLDOMTextNode2, _T("IID_IHTMLDOMTextNode2"));
		lGuidMap.SetAt (IID_IHTMLDOMImplementation, _T("IID_IHTMLDOMImplementation"));
		lGuidMap.SetAt (DIID_DispHTMLDOMAttribute, _T("DIID_DispHTMLDOMAttribute"));
		lGuidMap.SetAt (DIID_DispHTMLDOMTextNode, _T("DIID_DispHTMLDOMTextNode"));
		lGuidMap.SetAt (DIID_DispHTMLDOMImplementation, _T("DIID_DispHTMLDOMImplementation"));
		lGuidMap.SetAt (IID_IHTMLAttributeCollection, _T("IID_IHTMLAttributeCollection"));
		lGuidMap.SetAt (IID_IHTMLAttributeCollection2, _T("IID_IHTMLAttributeCollection2"));
		lGuidMap.SetAt (IID_IHTMLDOMChildrenCollection, _T("IID_IHTMLDOMChildrenCollection"));
		lGuidMap.SetAt (DIID_DispHTMLAttributeCollection, _T("DIID_DispHTMLAttributeCollection"));
		lGuidMap.SetAt (DIID_DispDOMChildrenCollection, _T("DIID_DispDOMChildrenCollection"));
		lGuidMap.SetAt (DIID_HTMLElementEvents2, _T("DIID_HTMLElementEvents2"));
		lGuidMap.SetAt (DIID_HTMLElementEvents, _T("DIID_HTMLElementEvents"));
		lGuidMap.SetAt (IID_IHTMLElementCollection, _T("IID_IHTMLElementCollection"));
		lGuidMap.SetAt (IID_IHTMLElement2, _T("IID_IHTMLElement2"));
		lGuidMap.SetAt (IID_IHTMLElement3, _T("IID_IHTMLElement3"));
		lGuidMap.SetAt (IID_IHTMLElement4, _T("IID_IHTMLElement4"));
		lGuidMap.SetAt (IID_IHTMLElementRender, _T("IID_IHTMLElementRender"));
		lGuidMap.SetAt (IID_IHTMLUniqueName, _T("IID_IHTMLUniqueName"));
		lGuidMap.SetAt (IID_IHTMLDatabinding, _T("IID_IHTMLDatabinding"));
		lGuidMap.SetAt (IID_IHTMLDocument, _T("IID_IHTMLDocument"));
		lGuidMap.SetAt (IID_IHTMLElementDefaults, _T("IID_IHTMLElementDefaults"));
		lGuidMap.SetAt (DIID_DispHTMLDefaults, _T("DIID_DispHTMLDefaults"));
		lGuidMap.SetAt (IID_IHTCDefaultDispatch, _T("IID_IHTCDefaultDispatch"));
		lGuidMap.SetAt (IID_IHTCPropertyBehavior, _T("IID_IHTCPropertyBehavior"));
		lGuidMap.SetAt (IID_IHTCMethodBehavior, _T("IID_IHTCMethodBehavior"));
		lGuidMap.SetAt (IID_IHTCEventBehavior, _T("IID_IHTCEventBehavior"));
		lGuidMap.SetAt (IID_IHTCAttachBehavior, _T("IID_IHTCAttachBehavior"));
		lGuidMap.SetAt (IID_IHTCAttachBehavior2, _T("IID_IHTCAttachBehavior2"));
		lGuidMap.SetAt (IID_IHTCDescBehavior, _T("IID_IHTCDescBehavior"));
		lGuidMap.SetAt (DIID_DispHTCDefaultDispatch, _T("DIID_DispHTCDefaultDispatch"));
		lGuidMap.SetAt (DIID_DispHTCPropertyBehavior, _T("DIID_DispHTCPropertyBehavior"));
		lGuidMap.SetAt (DIID_DispHTCMethodBehavior, _T("DIID_DispHTCMethodBehavior"));
		lGuidMap.SetAt (DIID_DispHTCEventBehavior, _T("DIID_DispHTCEventBehavior"));
		lGuidMap.SetAt (DIID_DispHTCAttachBehavior, _T("DIID_DispHTCAttachBehavior"));
		lGuidMap.SetAt (DIID_DispHTCDescBehavior, _T("DIID_DispHTCDescBehavior"));
		lGuidMap.SetAt (IID_IHTMLUrnCollection, _T("IID_IHTMLUrnCollection"));
		lGuidMap.SetAt (IID_IHTMLGenericElement, _T("IID_IHTMLGenericElement"));
		lGuidMap.SetAt (DIID_DispHTMLGenericElement, _T("DIID_DispHTMLGenericElement"));
		lGuidMap.SetAt (IID_IHTMLStyleSheetRule, _T("IID_IHTMLStyleSheetRule"));
		lGuidMap.SetAt (IID_IHTMLStyleSheetRulesCollection, _T("IID_IHTMLStyleSheetRulesCollection"));
		lGuidMap.SetAt (IID_IHTMLStyleSheetPage, _T("IID_IHTMLStyleSheetPage"));
		lGuidMap.SetAt (IID_IHTMLStyleSheetPagesCollection, _T("IID_IHTMLStyleSheetPagesCollection"));
		lGuidMap.SetAt (IID_IHTMLStyleSheetsCollection, _T("IID_IHTMLStyleSheetsCollection"));
		lGuidMap.SetAt (IID_IHTMLStyleSheet, _T("IID_IHTMLStyleSheet"));
		lGuidMap.SetAt (IID_IHTMLStyleSheet2, _T("IID_IHTMLStyleSheet2"));
		lGuidMap.SetAt (DIID_DispHTMLStyleSheet, _T("DIID_DispHTMLStyleSheet"));
		lGuidMap.SetAt (DIID_HTMLLinkElementEvents2, _T("DIID_HTMLLinkElementEvents2"));
		lGuidMap.SetAt (DIID_HTMLLinkElementEvents, _T("DIID_HTMLLinkElementEvents"));
		lGuidMap.SetAt (IID_IHTMLLinkElement, _T("IID_IHTMLLinkElement"));
		lGuidMap.SetAt (IID_IHTMLLinkElement2, _T("IID_IHTMLLinkElement2"));
		lGuidMap.SetAt (IID_IHTMLLinkElement3, _T("IID_IHTMLLinkElement3"));
		lGuidMap.SetAt (DIID_DispHTMLLinkElement, _T("DIID_DispHTMLLinkElement"));
		lGuidMap.SetAt (IID_IHTMLTxtRange, _T("IID_IHTMLTxtRange"));
		lGuidMap.SetAt (IID_IHTMLTextRangeMetrics, _T("IID_IHTMLTextRangeMetrics"));
		lGuidMap.SetAt (IID_IHTMLTextRangeMetrics2, _T("IID_IHTMLTextRangeMetrics2"));
		lGuidMap.SetAt (IID_IHTMLTxtRangeCollection, _T("IID_IHTMLTxtRangeCollection"));
		lGuidMap.SetAt (DIID_HTMLFormElementEvents2, _T("DIID_HTMLFormElementEvents2"));
		lGuidMap.SetAt (DIID_HTMLFormElementEvents, _T("DIID_HTMLFormElementEvents"));
		lGuidMap.SetAt (IID_IHTMLFormElement, _T("IID_IHTMLFormElement"));
		lGuidMap.SetAt (IID_IHTMLFormElement2, _T("IID_IHTMLFormElement2"));
		lGuidMap.SetAt (IID_IHTMLFormElement3, _T("IID_IHTMLFormElement3"));
		lGuidMap.SetAt (IID_IHTMLSubmitData, _T("IID_IHTMLSubmitData"));
		lGuidMap.SetAt (DIID_DispHTMLFormElement, _T("DIID_DispHTMLFormElement"));
		lGuidMap.SetAt (DIID_HTMLControlElementEvents2, _T("DIID_HTMLControlElementEvents2"));
		lGuidMap.SetAt (DIID_HTMLControlElementEvents, _T("DIID_HTMLControlElementEvents"));
		lGuidMap.SetAt (IID_IHTMLControlElement, _T("IID_IHTMLControlElement"));
		lGuidMap.SetAt (IID_IHTMLTextElement, _T("IID_IHTMLTextElement"));
		lGuidMap.SetAt (DIID_DispHTMLTextElement, _T("DIID_DispHTMLTextElement"));
		lGuidMap.SetAt (DIID_HTMLTextContainerEvents2, _T("DIID_HTMLTextContainerEvents2"));
		lGuidMap.SetAt (DIID_HTMLTextContainerEvents, _T("DIID_HTMLTextContainerEvents"));
		lGuidMap.SetAt (IID_IHTMLTextContainer, _T("IID_IHTMLTextContainer"));
		lGuidMap.SetAt (IID_IHTMLControlRange, _T("IID_IHTMLControlRange"));
		lGuidMap.SetAt (IID_IHTMLControlRange2, _T("IID_IHTMLControlRange2"));
		lGuidMap.SetAt (DIID_HTMLImgEvents2, _T("DIID_HTMLImgEvents2"));
		lGuidMap.SetAt (DIID_HTMLImgEvents, _T("DIID_HTMLImgEvents"));
		lGuidMap.SetAt (IID_IHTMLImgElement, _T("IID_IHTMLImgElement"));
		lGuidMap.SetAt (IID_IHTMLImgElement2, _T("IID_IHTMLImgElement2"));
		lGuidMap.SetAt (IID_IHTMLImageElementFactory, _T("IID_IHTMLImageElementFactory"));
		lGuidMap.SetAt (DIID_DispHTMLImg, _T("DIID_DispHTMLImg"));
		lGuidMap.SetAt (IID_IHTMLBodyElement, _T("IID_IHTMLBodyElement"));
		lGuidMap.SetAt (IID_IHTMLBodyElement2, _T("IID_IHTMLBodyElement2"));
		lGuidMap.SetAt (DIID_DispHTMLBody, _T("DIID_DispHTMLBody"));
		lGuidMap.SetAt (IID_IHTMLFontElement, _T("IID_IHTMLFontElement"));
		lGuidMap.SetAt (DIID_DispHTMLFontElement, _T("DIID_DispHTMLFontElement"));
		lGuidMap.SetAt (DIID_HTMLAnchorEvents2, _T("DIID_HTMLAnchorEvents2"));
		lGuidMap.SetAt (DIID_HTMLAnchorEvents, _T("DIID_HTMLAnchorEvents"));
		lGuidMap.SetAt (IID_IHTMLAnchorElement, _T("IID_IHTMLAnchorElement"));
		lGuidMap.SetAt (IID_IHTMLAnchorElement2, _T("IID_IHTMLAnchorElement2"));
		lGuidMap.SetAt (DIID_DispHTMLAnchorElement, _T("DIID_DispHTMLAnchorElement"));
		lGuidMap.SetAt (DIID_HTMLLabelEvents2, _T("DIID_HTMLLabelEvents2"));
		lGuidMap.SetAt (DIID_HTMLLabelEvents, _T("DIID_HTMLLabelEvents"));
		lGuidMap.SetAt (IID_IHTMLLabelElement, _T("IID_IHTMLLabelElement"));
		lGuidMap.SetAt (IID_IHTMLLabelElement2, _T("IID_IHTMLLabelElement2"));
		lGuidMap.SetAt (DIID_DispHTMLLabelElement, _T("DIID_DispHTMLLabelElement"));
		lGuidMap.SetAt (IID_IHTMLListElement, _T("IID_IHTMLListElement"));
		lGuidMap.SetAt (IID_IHTMLListElement2, _T("IID_IHTMLListElement2"));
		lGuidMap.SetAt (DIID_DispHTMLListElement, _T("DIID_DispHTMLListElement"));
		lGuidMap.SetAt (IID_IHTMLUListElement, _T("IID_IHTMLUListElement"));
		lGuidMap.SetAt (DIID_DispHTMLUListElement, _T("DIID_DispHTMLUListElement"));
		lGuidMap.SetAt (IID_IHTMLOListElement, _T("IID_IHTMLOListElement"));
		lGuidMap.SetAt (DIID_DispHTMLOListElement, _T("DIID_DispHTMLOListElement"));
		lGuidMap.SetAt (IID_IHTMLLIElement, _T("IID_IHTMLLIElement"));
		lGuidMap.SetAt (DIID_DispHTMLLIElement, _T("DIID_DispHTMLLIElement"));
		lGuidMap.SetAt (IID_IHTMLBlockElement, _T("IID_IHTMLBlockElement"));
		lGuidMap.SetAt (IID_IHTMLBlockElement2, _T("IID_IHTMLBlockElement2"));
		lGuidMap.SetAt (DIID_DispHTMLBlockElement, _T("DIID_DispHTMLBlockElement"));
		lGuidMap.SetAt (IID_IHTMLDivElement, _T("IID_IHTMLDivElement"));
		lGuidMap.SetAt (DIID_DispHTMLDivElement, _T("DIID_DispHTMLDivElement"));
		lGuidMap.SetAt (IID_IHTMLDDElement, _T("IID_IHTMLDDElement"));
		lGuidMap.SetAt (DIID_DispHTMLDDElement, _T("DIID_DispHTMLDDElement"));
		lGuidMap.SetAt (IID_IHTMLDTElement, _T("IID_IHTMLDTElement"));
		lGuidMap.SetAt (DIID_DispHTMLDTElement, _T("DIID_DispHTMLDTElement"));
		lGuidMap.SetAt (IID_IHTMLBRElement, _T("IID_IHTMLBRElement"));
		lGuidMap.SetAt (DIID_DispHTMLBRElement, _T("DIID_DispHTMLBRElement"));
		lGuidMap.SetAt (IID_IHTMLDListElement, _T("IID_IHTMLDListElement"));
		lGuidMap.SetAt (DIID_DispHTMLDListElement, _T("DIID_DispHTMLDListElement"));
		lGuidMap.SetAt (IID_IHTMLHRElement, _T("IID_IHTMLHRElement"));
		lGuidMap.SetAt (DIID_DispHTMLHRElement, _T("DIID_DispHTMLHRElement"));
		lGuidMap.SetAt (IID_IHTMLParaElement, _T("IID_IHTMLParaElement"));
		lGuidMap.SetAt (DIID_DispHTMLParaElement, _T("DIID_DispHTMLParaElement"));
		lGuidMap.SetAt (IID_IHTMLElementCollection2, _T("IID_IHTMLElementCollection2"));
		lGuidMap.SetAt (IID_IHTMLElementCollection3, _T("IID_IHTMLElementCollection3"));
		lGuidMap.SetAt (DIID_DispHTMLElementCollection, _T("DIID_DispHTMLElementCollection"));
		lGuidMap.SetAt (IID_IHTMLHeaderElement, _T("IID_IHTMLHeaderElement"));
		lGuidMap.SetAt (DIID_DispHTMLHeaderElement, _T("DIID_DispHTMLHeaderElement"));
		lGuidMap.SetAt (DIID_HTMLSelectElementEvents2, _T("DIID_HTMLSelectElementEvents2"));
		lGuidMap.SetAt (DIID_HTMLSelectElementEvents, _T("DIID_HTMLSelectElementEvents"));
		lGuidMap.SetAt (IID_IHTMLSelectElement, _T("IID_IHTMLSelectElement"));
		lGuidMap.SetAt (IID_IHTMLSelectElement2, _T("IID_IHTMLSelectElement2"));
		lGuidMap.SetAt (IID_IHTMLSelectElement4, _T("IID_IHTMLSelectElement4"));
		lGuidMap.SetAt (DIID_DispHTMLSelectElement, _T("DIID_DispHTMLSelectElement"));
		lGuidMap.SetAt (IID_IHTMLSelectionObject, _T("IID_IHTMLSelectionObject"));
		lGuidMap.SetAt (IID_IHTMLSelectionObject2, _T("IID_IHTMLSelectionObject2"));
		lGuidMap.SetAt (IID_IHTMLOptionElement, _T("IID_IHTMLOptionElement"));
		lGuidMap.SetAt (IID_IHTMLOptionElement3, _T("IID_IHTMLOptionElement3"));
		lGuidMap.SetAt (IID_IHTMLOptionElementFactory, _T("IID_IHTMLOptionElementFactory"));
		lGuidMap.SetAt (DIID_DispHTMLOptionElement, _T("DIID_DispHTMLOptionElement"));
		lGuidMap.SetAt (DIID_HTMLButtonElementEvents2, _T("DIID_HTMLButtonElementEvents2"));
		lGuidMap.SetAt (DIID_HTMLButtonElementEvents, _T("DIID_HTMLButtonElementEvents"));
		lGuidMap.SetAt (DIID_HTMLInputTextElementEvents2, _T("DIID_HTMLInputTextElementEvents2"));
		lGuidMap.SetAt (DIID_HTMLOptionButtonElementEvents2, _T("DIID_HTMLOptionButtonElementEvents2"));
		lGuidMap.SetAt (DIID_HTMLInputFileElementEvents2, _T("DIID_HTMLInputFileElementEvents2"));
		lGuidMap.SetAt (DIID_HTMLInputImageEvents2, _T("DIID_HTMLInputImageEvents2"));
		lGuidMap.SetAt (DIID_HTMLInputTextElementEvents, _T("DIID_HTMLInputTextElementEvents"));
		lGuidMap.SetAt (DIID_HTMLOptionButtonElementEvents, _T("DIID_HTMLOptionButtonElementEvents"));
		lGuidMap.SetAt (DIID_HTMLInputFileElementEvents, _T("DIID_HTMLInputFileElementEvents"));
		lGuidMap.SetAt (DIID_HTMLInputImageEvents, _T("DIID_HTMLInputImageEvents"));
		lGuidMap.SetAt (IID_IHTMLInputElement, _T("IID_IHTMLInputElement"));
		lGuidMap.SetAt (IID_IHTMLInputElement2, _T("IID_IHTMLInputElement2"));
		lGuidMap.SetAt (IID_IHTMLInputButtonElement, _T("IID_IHTMLInputButtonElement"));
		lGuidMap.SetAt (IID_IHTMLInputHiddenElement, _T("IID_IHTMLInputHiddenElement"));
		lGuidMap.SetAt (IID_IHTMLInputTextElement, _T("IID_IHTMLInputTextElement"));
		lGuidMap.SetAt (IID_IHTMLInputFileElement, _T("IID_IHTMLInputFileElement"));
		lGuidMap.SetAt (IID_IHTMLOptionButtonElement, _T("IID_IHTMLOptionButtonElement"));
		lGuidMap.SetAt (IID_IHTMLInputImage, _T("IID_IHTMLInputImage"));
		lGuidMap.SetAt (DIID_DispHTMLInputElement, _T("DIID_DispHTMLInputElement"));
		lGuidMap.SetAt (IID_IHTMLTextAreaElement, _T("IID_IHTMLTextAreaElement"));
		lGuidMap.SetAt (DIID_DispHTMLTextAreaElement, _T("DIID_DispHTMLTextAreaElement"));
		lGuidMap.SetAt (DIID_DispHTMLRichtextElement, _T("DIID_DispHTMLRichtextElement"));
		lGuidMap.SetAt (IID_IHTMLButtonElement, _T("IID_IHTMLButtonElement"));
		lGuidMap.SetAt (DIID_DispHTMLButtonElement, _T("DIID_DispHTMLButtonElement"));
		lGuidMap.SetAt (DIID_HTMLMarqueeElementEvents2, _T("DIID_HTMLMarqueeElementEvents2"));
		lGuidMap.SetAt (DIID_HTMLMarqueeElementEvents, _T("DIID_HTMLMarqueeElementEvents"));
		lGuidMap.SetAt (IID_IHTMLMarqueeElement, _T("IID_IHTMLMarqueeElement"));
		lGuidMap.SetAt (DIID_DispHTMLMarqueeElement, _T("DIID_DispHTMLMarqueeElement"));
		lGuidMap.SetAt (IID_IHTMLHtmlElement, _T("IID_IHTMLHtmlElement"));
		lGuidMap.SetAt (IID_IHTMLHeadElement, _T("IID_IHTMLHeadElement"));
		lGuidMap.SetAt (IID_IHTMLTitleElement, _T("IID_IHTMLTitleElement"));
		lGuidMap.SetAt (IID_IHTMLMetaElement, _T("IID_IHTMLMetaElement"));
		lGuidMap.SetAt (IID_IHTMLMetaElement2, _T("IID_IHTMLMetaElement2"));
		lGuidMap.SetAt (IID_IHTMLBaseElement, _T("IID_IHTMLBaseElement"));
		lGuidMap.SetAt (IID_IHTMLIsIndexElement, _T("IID_IHTMLIsIndexElement"));
		lGuidMap.SetAt (IID_IHTMLIsIndexElement2, _T("IID_IHTMLIsIndexElement2"));
		lGuidMap.SetAt (IID_IHTMLNextIdElement, _T("IID_IHTMLNextIdElement"));
		lGuidMap.SetAt (DIID_DispHTMLHtmlElement, _T("DIID_DispHTMLHtmlElement"));
		lGuidMap.SetAt (DIID_DispHTMLHeadElement, _T("DIID_DispHTMLHeadElement"));
		lGuidMap.SetAt (DIID_DispHTMLTitleElement, _T("DIID_DispHTMLTitleElement"));
		lGuidMap.SetAt (DIID_DispHTMLMetaElement, _T("DIID_DispHTMLMetaElement"));
		lGuidMap.SetAt (DIID_DispHTMLBaseElement, _T("DIID_DispHTMLBaseElement"));
		lGuidMap.SetAt (DIID_DispHTMLIsIndexElement, _T("DIID_DispHTMLIsIndexElement"));
		lGuidMap.SetAt (DIID_DispHTMLNextIdElement, _T("DIID_DispHTMLNextIdElement"));
		lGuidMap.SetAt (IID_IHTMLBaseFontElement, _T("IID_IHTMLBaseFontElement"));
		lGuidMap.SetAt (DIID_DispHTMLBaseFontElement, _T("DIID_DispHTMLBaseFontElement"));
		lGuidMap.SetAt (IID_IHTMLUnknownElement, _T("IID_IHTMLUnknownElement"));
		lGuidMap.SetAt (DIID_DispHTMLUnknownElement, _T("DIID_DispHTMLUnknownElement"));
		lGuidMap.SetAt (IID_IOmHistory, _T("IID_IOmHistory"));
		lGuidMap.SetAt (IID_IHTMLMimeTypesCollection, _T("IID_IHTMLMimeTypesCollection"));
		lGuidMap.SetAt (IID_IHTMLPluginsCollection, _T("IID_IHTMLPluginsCollection"));
		lGuidMap.SetAt (IID_IHTMLOpsProfile, _T("IID_IHTMLOpsProfile"));
		lGuidMap.SetAt (IID_IOmNavigator, _T("IID_IOmNavigator"));
		lGuidMap.SetAt (IID_IHTMLLocation, _T("IID_IHTMLLocation"));
		lGuidMap.SetAt (IID_IHTMLBookmarkCollection, _T("IID_IHTMLBookmarkCollection"));
		lGuidMap.SetAt (IID_IHTMLDataTransfer, _T("IID_IHTMLDataTransfer"));
		lGuidMap.SetAt (IID_IHTMLEventObj2, _T("IID_IHTMLEventObj2"));
		lGuidMap.SetAt (IID_IHTMLEventObj3, _T("IID_IHTMLEventObj3"));
		lGuidMap.SetAt (IID_IHTMLEventObj4, _T("IID_IHTMLEventObj4"));
		lGuidMap.SetAt (DIID_DispCEventObj, _T("DIID_DispCEventObj"));
		lGuidMap.SetAt (DIID_HTMLWindowEvents2, _T("DIID_HTMLWindowEvents2"));
		lGuidMap.SetAt (DIID_HTMLWindowEvents, _T("DIID_HTMLWindowEvents"));
		lGuidMap.SetAt (IID_IHTMLDocument2, _T("IID_IHTMLDocument2"));
		lGuidMap.SetAt (IID_IHTMLFramesCollection2, _T("IID_IHTMLFramesCollection2"));
		lGuidMap.SetAt (IID_IHTMLWindow2, _T("IID_IHTMLWindow2"));
		lGuidMap.SetAt (IID_IHTMLWindow3, _T("IID_IHTMLWindow3"));
		lGuidMap.SetAt (IID_IHTMLFrameBase, _T("IID_IHTMLFrameBase"));
		lGuidMap.SetAt (IID_IHTMLScreen, _T("IID_IHTMLScreen"));
		lGuidMap.SetAt (IID_IHTMLScreen2, _T("IID_IHTMLScreen2"));
		lGuidMap.SetAt (IID_IHTMLWindow4, _T("IID_IHTMLWindow4"));
		lGuidMap.SetAt (DIID_DispHTMLScreen, _T("DIID_DispHTMLScreen"));
		lGuidMap.SetAt (DIID_DispHTMLWindow2, _T("DIID_DispHTMLWindow2"));
		lGuidMap.SetAt (DIID_DispHTMLWindowProxy, _T("DIID_DispHTMLWindowProxy"));
		lGuidMap.SetAt (DIID_HTMLDocumentEvents2, _T("DIID_HTMLDocumentEvents2"));
		lGuidMap.SetAt (DIID_HTMLDocumentEvents, _T("DIID_HTMLDocumentEvents"));
		lGuidMap.SetAt (IID_IHTMLDocument3, _T("IID_IHTMLDocument3"));
		lGuidMap.SetAt (IID_IHTMLDocument4, _T("IID_IHTMLDocument4"));
		lGuidMap.SetAt (IID_IHTMLDocument5, _T("IID_IHTMLDocument5"));
		lGuidMap.SetAt (DIID_DispHTMLDocument, _T("DIID_DispHTMLDocument"));
		lGuidMap.SetAt (DIID_DWebBridgeEvents, _T("DIID_DWebBridgeEvents"));
		lGuidMap.SetAt (IID_IWebBridge, _T("IID_IWebBridge"));
		lGuidMap.SetAt (IID_IWBScriptControl, _T("IID_IWBScriptControl"));
		lGuidMap.SetAt (IID_IHTMLEmbedElement, _T("IID_IHTMLEmbedElement"));
		lGuidMap.SetAt (DIID_DispHTMLEmbed, _T("DIID_DispHTMLEmbed"));
		lGuidMap.SetAt (DIID_HTMLMapEvents2, _T("DIID_HTMLMapEvents2"));
		lGuidMap.SetAt (DIID_HTMLMapEvents, _T("DIID_HTMLMapEvents"));
		lGuidMap.SetAt (IID_IHTMLAreasCollection, _T("IID_IHTMLAreasCollection"));
		lGuidMap.SetAt (IID_IHTMLAreasCollection2, _T("IID_IHTMLAreasCollection2"));
		lGuidMap.SetAt (IID_IHTMLAreasCollection3, _T("IID_IHTMLAreasCollection3"));
		lGuidMap.SetAt (IID_IHTMLMapElement, _T("IID_IHTMLMapElement"));
		lGuidMap.SetAt (DIID_DispHTMLAreasCollection, _T("DIID_DispHTMLAreasCollection"));
		lGuidMap.SetAt (DIID_DispHTMLMapElement, _T("DIID_DispHTMLMapElement"));
		lGuidMap.SetAt (DIID_HTMLAreaEvents2, _T("DIID_HTMLAreaEvents2"));
		lGuidMap.SetAt (DIID_HTMLAreaEvents, _T("DIID_HTMLAreaEvents"));
		lGuidMap.SetAt (IID_IHTMLAreaElement, _T("IID_IHTMLAreaElement"));
		lGuidMap.SetAt (DIID_DispHTMLAreaElement, _T("DIID_DispHTMLAreaElement"));
		lGuidMap.SetAt (IID_IHTMLTableCaption, _T("IID_IHTMLTableCaption"));
		lGuidMap.SetAt (DIID_DispHTMLTableCaption, _T("DIID_DispHTMLTableCaption"));
		lGuidMap.SetAt (IID_IHTMLCommentElement, _T("IID_IHTMLCommentElement"));
		lGuidMap.SetAt (IID_IHTMLCommentElement2, _T("IID_IHTMLCommentElement2"));
		lGuidMap.SetAt (DIID_DispHTMLCommentElement, _T("DIID_DispHTMLCommentElement"));
		lGuidMap.SetAt (IID_IHTMLPhraseElement, _T("IID_IHTMLPhraseElement"));
		lGuidMap.SetAt (IID_IHTMLPhraseElement2, _T("IID_IHTMLPhraseElement2"));
		lGuidMap.SetAt (IID_IHTMLSpanElement, _T("IID_IHTMLSpanElement"));
		lGuidMap.SetAt (DIID_DispHTMLPhraseElement, _T("DIID_DispHTMLPhraseElement"));
		lGuidMap.SetAt (DIID_DispHTMLSpanElement, _T("DIID_DispHTMLSpanElement"));
		lGuidMap.SetAt (DIID_HTMLTableEvents2, _T("DIID_HTMLTableEvents2"));
		lGuidMap.SetAt (DIID_HTMLTableEvents, _T("DIID_HTMLTableEvents"));
		lGuidMap.SetAt (IID_IHTMLTableSection, _T("IID_IHTMLTableSection"));
		lGuidMap.SetAt (IID_IHTMLTable, _T("IID_IHTMLTable"));
		lGuidMap.SetAt (IID_IHTMLTable2, _T("IID_IHTMLTable2"));
		lGuidMap.SetAt (IID_IHTMLTable3, _T("IID_IHTMLTable3"));
		lGuidMap.SetAt (IID_IHTMLTableCol, _T("IID_IHTMLTableCol"));
		lGuidMap.SetAt (IID_IHTMLTableCol2, _T("IID_IHTMLTableCol2"));
		lGuidMap.SetAt (IID_IHTMLTableSection2, _T("IID_IHTMLTableSection2"));
		lGuidMap.SetAt (IID_IHTMLTableSection3, _T("IID_IHTMLTableSection3"));
		lGuidMap.SetAt (IID_IHTMLTableRow, _T("IID_IHTMLTableRow"));
		lGuidMap.SetAt (IID_IHTMLTableRow2, _T("IID_IHTMLTableRow2"));
		lGuidMap.SetAt (IID_IHTMLTableRow3, _T("IID_IHTMLTableRow3"));
		lGuidMap.SetAt (IID_IHTMLTableRowMetrics, _T("IID_IHTMLTableRowMetrics"));
		lGuidMap.SetAt (IID_IHTMLTableCell, _T("IID_IHTMLTableCell"));
		lGuidMap.SetAt (IID_IHTMLTableCell2, _T("IID_IHTMLTableCell2"));
		lGuidMap.SetAt (DIID_DispHTMLTable, _T("DIID_DispHTMLTable"));
		lGuidMap.SetAt (DIID_DispHTMLTableCol, _T("DIID_DispHTMLTableCol"));
		lGuidMap.SetAt (DIID_DispHTMLTableSection, _T("DIID_DispHTMLTableSection"));
		lGuidMap.SetAt (DIID_DispHTMLTableRow, _T("DIID_DispHTMLTableRow"));
		lGuidMap.SetAt (DIID_DispHTMLTableCell, _T("DIID_DispHTMLTableCell"));
		lGuidMap.SetAt (DIID_HTMLScriptEvents2, _T("DIID_HTMLScriptEvents2"));
		lGuidMap.SetAt (DIID_HTMLScriptEvents, _T("DIID_HTMLScriptEvents"));
		lGuidMap.SetAt (IID_IHTMLScriptElement, _T("IID_IHTMLScriptElement"));
		lGuidMap.SetAt (IID_IHTMLScriptElement2, _T("IID_IHTMLScriptElement2"));
		lGuidMap.SetAt (DIID_DispHTMLScriptElement, _T("DIID_DispHTMLScriptElement"));
		lGuidMap.SetAt (IID_IHTMLNoShowElement, _T("IID_IHTMLNoShowElement"));
		lGuidMap.SetAt (DIID_DispHTMLNoShowElement, _T("DIID_DispHTMLNoShowElement"));
		lGuidMap.SetAt (DIID_HTMLObjectElementEvents2, _T("DIID_HTMLObjectElementEvents2"));
		lGuidMap.SetAt (DIID_HTMLObjectElementEvents, _T("DIID_HTMLObjectElementEvents"));
		lGuidMap.SetAt (IID_IHTMLObjectElement, _T("IID_IHTMLObjectElement"));
		lGuidMap.SetAt (IID_IHTMLObjectElement2, _T("IID_IHTMLObjectElement2"));
		lGuidMap.SetAt (IID_IHTMLObjectElement3, _T("IID_IHTMLObjectElement3"));	 // Unfinished and fed up

		lGuidMap.SetAt (CLSID_ShellDesktop, _T("CLSID_ShellDesktop"));
		lGuidMap.SetAt (CLSID_ShellLink, _T("CLSID_ShellLink"));
		lGuidMap.SetAt (CLSID_NetworkPlaces, _T("CLSID_NetworkPlaces"));
//		lGuidMap.SetAt (CLSID_NetworkExplorerFolder, _T("CLSID_NetworkExplorerFolder"));
		lGuidMap.SetAt (CLSID_NetworkDomain, _T("CLSID_NetworkDomain"));
		lGuidMap.SetAt (CLSID_NetworkServer, _T("CLSID_NetworkServer"));
		lGuidMap.SetAt (CLSID_NetworkShare, _T("CLSID_NetworkShare"));
		lGuidMap.SetAt (CLSID_MyComputer, _T("CLSID_MyComputer"));
		lGuidMap.SetAt (CLSID_Internet, _T("CLSID_Internet"));
		lGuidMap.SetAt (CLSID_ShellFSFolder, _T("CLSID_ShellFSFolder"));
		lGuidMap.SetAt (CLSID_RecycleBin, _T("CLSID_RecycleBin"));
		lGuidMap.SetAt (CLSID_ControlPanel, _T("CLSID_ControlPanel"));
		lGuidMap.SetAt (CLSID_Printers, _T("CLSID_Printers"));
		lGuidMap.SetAt (CLSID_MyDocuments, _T("CLSID_MyDocuments"));

		lGuidMap.SetAt (__uuidof(IAgentUserInput), _T("IID_IAgentUserInput"));
		lGuidMap.SetAt (__uuidof(IAgentCommand), _T("IID_IAgentCommand"));
		lGuidMap.SetAt (__uuidof(IAgentCommandEx), _T("IID_IAgentCommandEx"));
		lGuidMap.SetAt (__uuidof(IAgentCommands), _T("IID_IAgentCommands"));
		lGuidMap.SetAt (__uuidof(IAgentCommandsEx), _T("IID_IAgentCommandsEx"));
		lGuidMap.SetAt (__uuidof(IAgentCommandWindow), _T("IID_ISvrCommandWindow"));
		lGuidMap.SetAt (__uuidof(IAgentSpeechInputProperties), _T("IID_IAgentSpeechInputProperties"));
		lGuidMap.SetAt (__uuidof(IAgentAudioOutputProperties), _T("IID_IAgentAudioOutputProperties"));
		lGuidMap.SetAt (__uuidof(IAgentAudioOutputPropertiesEx), _T("IID_IAgentAudioOutputPropertiesEx"));
		lGuidMap.SetAt (__uuidof(IAgentPropertySheet), _T("IID_ISvrPropertySheet"));
		lGuidMap.SetAt (__uuidof(IAgentBalloon), _T("IID_IAgentBalloon"));
		lGuidMap.SetAt (__uuidof(IAgentBalloonEx), _T("IID_IAgentBalloonEx"));
		lGuidMap.SetAt (__uuidof(IAgentCharacter), _T("IID_IAgentCharacter"));
		lGuidMap.SetAt (__uuidof(IAgentCharacterEx), _T("IID_IAgentCharacterEx"));
		lGuidMap.SetAt (__uuidof(IAgent), _T("IID_IAgent"));
		lGuidMap.SetAt (__uuidof(IAgentEx), _T("IID_IAgentEx"));
		lGuidMap.SetAt (__uuidof(IAgentNotifySink), _T("IID_IAgentNotifySink"));
		lGuidMap.SetAt (__uuidof(IAgentNotifySinkEx), _T("IID_IAgentNotifySinkEx"));
		lGuidMap.SetAt (__uuidof(IAgentPrivateNotifySink), _T("IID_IAgentPrivateNotifySink"));
		lGuidMap.SetAt (__uuidof(IAgentCustomMarshalMaker), _T("IID_IAgentCustomMarshalMaker"));
		lGuidMap.SetAt (__uuidof(IAgentClientStatus), _T("IID_IAgentClientStatus"));
		lGuidMap.SetAt (__uuidof(AgentServer), _T("CLSID_AgentServer"));

		lGuidMap.SetAt (__uuidof(IAgentCtlRequest), _T("IID_IAgentCtlRequest"));
		lGuidMap.SetAt (__uuidof(IAgentCtlUserInput), _T("IID_IAgentCtlUserInput"));
		lGuidMap.SetAt (__uuidof(IAgentCtlBalloon), _T("IID_IAgentCtlBalloon"));
		lGuidMap.SetAt (__uuidof(IAgentCtlBalloonEx), _T("IID_IAgentCtlBalloonEx"));
		lGuidMap.SetAt (__uuidof(IAgentCtlCommand), _T("IID_IAgentCtlCommand"));
		lGuidMap.SetAt (__uuidof(IAgentCtlCommandEx), _T("IID_IAgentCtlCommandEx"));
		lGuidMap.SetAt (__uuidof(IAgentCtlCommands), _T("IID_IAgentCtlCommands"));
		lGuidMap.SetAt (__uuidof(IAgentCtlCommandsEx), _T("IID_IAgentCtlCommandsEx"));
		lGuidMap.SetAt (__uuidof(IAgentCtlCharacter), _T("IID_IAgentCtlCharacter"));
		lGuidMap.SetAt (__uuidof(IAgentCtlAnimationNames), _T("IID_IAgentCtlAnimationNames"));
		lGuidMap.SetAt (__uuidof(IAgentCtlCharacterEx), _T("IID_IAgentCtlCharacterEx"));
		lGuidMap.SetAt (__uuidof(IAgentCtlCharacters), _T("IID_IAgentCtlCharacters"));
		lGuidMap.SetAt (__uuidof(IAgentCtlAudioObject), _T("IID_IAgentCtlAudioObject"));
		lGuidMap.SetAt (__uuidof(IAgentCtlAudioObjectEx), _T("IID_IAgentCtlAudioObjectEx"));
		lGuidMap.SetAt (__uuidof(IAgentCtlSpeechInput), _T("IID_IAgentCtlSpeechInput"));
		lGuidMap.SetAt (__uuidof(IAgentCtlPropertySheet), _T("IID_IAgentCtlPropertySheet"));
		lGuidMap.SetAt (__uuidof(IAgentCtlCommandsWindow), _T("IID_IAgentCtlCommandsWindow"));
		lGuidMap.SetAt (__uuidof(IAgentCtl), _T("IID_IAgentCtl"));
		lGuidMap.SetAt (__uuidof(IAgentCtlEx), _T("IID_IAgentCtlEx"));
		lGuidMap.SetAt (__uuidof(_AgentEvents), _T("DIID__AgentEvents"));
		lGuidMap.SetAt (__uuidof(Agent), _T("CLSID_Agent"));

		lGuidMap.SetAt (IID_IAVIFile, _T("IID_IAVIFile"));
		lGuidMap.SetAt (IID_IAVIStream, _T("IID_IAVIStream"));
		lGuidMap.SetAt (IID_IAVIStreaming, _T("IID_IAVIStreaming"));
		lGuidMap.SetAt (IID_IGetFrame, _T("IID_IGetFrame"));
		lGuidMap.SetAt (IID_IAVIEditStream, _T("IID_IAVIEditStream"));
		lGuidMap.SetAt (IID_IAVIPersistFile, _T("IID_IAVIPersistFile"));
		lGuidMap.SetAt (CLSID_AVIFile, _T("CLSID_AVIFile"));

		lGuidMap.SetAt (IID_IBaseFilter, _T("IID_IBaseFilter"));
		lGuidMap.SetAt (IID_IFilterGraph, _T("IID_IFilterGraph"));
		lGuidMap.SetAt (IID_IPin, _T("IID_IPin"));
		lGuidMap.SetAt (IID_IEnumPins, _T("IID_IEnumPins"));
		lGuidMap.SetAt (IID_IEnumMediaTypes, _T("IID_IEnumMediaTypes"));
		lGuidMap.SetAt (IID_IEnumFilters, _T("IID_IEnumFilters"));
		lGuidMap.SetAt (IID_IMediaFilter, _T("IID_IMediaFilter"));
		lGuidMap.SetAt (IID_IReferenceClock, _T("IID_IReferenceClock"));
		lGuidMap.SetAt (IID_IMediaSample, _T("IID_IMediaSample"));
		lGuidMap.SetAt (IID_IMediaSample2, _T("IID_IMediaSample2"));
		lGuidMap.SetAt (IID_IMemAllocator, _T("IID_IMemAllocator"));
		lGuidMap.SetAt (IID_IMemInputPin, _T("IID_IMemInputPin"));
		lGuidMap.SetAt (IID_IAsyncReader, _T("IID_IAsyncReader"));
		lGuidMap.SetAt (IID_IMediaSeeking, _T("IID_IMediaSeeking"));
		lGuidMap.SetAt (IID_IMediaPosition, _T("IID_IMediaPosition"));
		lGuidMap.SetAt (IID_IMediaEventSink, _T("IID_IMediaEventSink"));
		lGuidMap.SetAt (IID_IFileSourceFilter, _T("IID_IFileSourceFilter"));
		lGuidMap.SetAt (IID_IVideoWindow, _T("IID_IVideoWindow"));
		lGuidMap.SetAt (IID_IBasicAudio, _T("IID_IBasicAudio"));
		lGuidMap.SetAt (IID_IBasicVideo, _T("IID_IBasicVideo"));
		lGuidMap.SetAt (IID_IAMFilterMiscFlags, _T("IID_IAMFilterMiscFlags"));
		lGuidMap.SetAt (IID_IAMOpenProgress, _T("IID_IAMOpenProgress"));
		lGuidMap.SetAt (IID_IAMDeviceRemoval, _T("IID_IAMDeviceRemoval"));
		lGuidMap.SetAt (IID_IAMStreamControl, _T("IID_IAMStreamControl"));
		lGuidMap.SetAt (IID_IAMPushSource, _T("IID_IAMPushSource"));
		lGuidMap.SetAt (IID_IKsPropertySet, _T("IID_IKsPropertySet"));

		lGuidMap.SetAt (MEDIATYPE_AnalogAudio, _T("MEDIATYPE_AnalogAudio"));
		lGuidMap.SetAt (MEDIATYPE_AnalogVideo, _T("MEDIATYPE_AnalogVideo"));
		lGuidMap.SetAt (MEDIATYPE_Audio, _T("MEDIATYPE_Audio"));
		lGuidMap.SetAt (MEDIATYPE_AUXLine21Data, _T("MEDIATYPE_AUXLine21Data"));
		lGuidMap.SetAt (MEDIATYPE_File, _T("MEDIATYPE_File"));
		lGuidMap.SetAt (MEDIATYPE_Interleaved, _T("MEDIATYPE_Interleaved"));
		lGuidMap.SetAt (MEDIATYPE_ScriptCommand, _T("MEDIATYPE_ScriptCommand"));
		lGuidMap.SetAt (MEDIATYPE_VBI, _T("MEDIATYPE_VBI"));
		lGuidMap.SetAt (MEDIATYPE_LMRT, _T("MEDIATYPE_LMRT"));
		lGuidMap.SetAt (MEDIATYPE_Timecode, _T("MEDIATYPE_Timecode"));
		lGuidMap.SetAt (MEDIATYPE_Stream, _T("MEDIATYPE_Stream"));
		lGuidMap.SetAt (MEDIATYPE_URL_STREAM, _T("MEDIATYPE_URL_STREAM"));
		lGuidMap.SetAt (MEDIATYPE_Video, _T("MEDIATYPE_Video"));
		lGuidMap.SetAt (MEDIATYPE_ScriptCommand, _T("MEDIATYPE_ScriptCommand"));
		lGuidMap.SetAt (MEDIATYPE_MPEG1SystemStream, _T("MEDIATYPE_MPEG1SystemStream"));

		lGuidMap.SetAt (MEDIASUBTYPE_CLPL, _T("MEDIASUBTYPE_CLPL"));
		lGuidMap.SetAt (MEDIASUBTYPE_YUYV, _T("MEDIASUBTYPE_YUYV"));
		lGuidMap.SetAt (MEDIASUBTYPE_IYUV, _T("MEDIASUBTYPE_IYUV"));
		lGuidMap.SetAt (MEDIASUBTYPE_YVU9, _T("MEDIASUBTYPE_YVU9"));
		lGuidMap.SetAt (MEDIASUBTYPE_Y411, _T("MEDIASUBTYPE_Y411"));
		lGuidMap.SetAt (MEDIASUBTYPE_Y41P, _T("MEDIASUBTYPE_Y41P"));
		lGuidMap.SetAt (MEDIASUBTYPE_YUY2, _T("MEDIASUBTYPE_YUY2"));
		lGuidMap.SetAt (MEDIASUBTYPE_YVYU, _T("MEDIASUBTYPE_YVYU"));
		lGuidMap.SetAt (MEDIASUBTYPE_UYVY, _T("MEDIASUBTYPE_UYVY"));
		lGuidMap.SetAt (MEDIASUBTYPE_Y211, _T("MEDIASUBTYPE_Y211"));
		lGuidMap.SetAt (MEDIASUBTYPE_CLJR, _T("MEDIASUBTYPE_CLJR"));
		lGuidMap.SetAt (MEDIASUBTYPE_IF09, _T("MEDIASUBTYPE_IF09"));
		lGuidMap.SetAt (MEDIASUBTYPE_CPLA, _T("MEDIASUBTYPE_CPLA"));
		lGuidMap.SetAt (MEDIASUBTYPE_MJPG, _T("MEDIASUBTYPE_MJPG"));
		lGuidMap.SetAt (MEDIASUBTYPE_TVMJ, _T("MEDIASUBTYPE_TVMJ"));
		lGuidMap.SetAt (MEDIASUBTYPE_WAKE, _T("MEDIASUBTYPE_WAKE"));
		lGuidMap.SetAt (MEDIASUBTYPE_CFCC, _T("MEDIASUBTYPE_CFCC"));
		lGuidMap.SetAt (MEDIASUBTYPE_IJPG, _T("MEDIASUBTYPE_IJPG"));
		lGuidMap.SetAt (MEDIASUBTYPE_Plum, _T("MEDIASUBTYPE_Plum"));
		lGuidMap.SetAt (MEDIASUBTYPE_DVCS, _T("MEDIASUBTYPE_DVCS"));
		lGuidMap.SetAt (MEDIASUBTYPE_DVSD, _T("MEDIASUBTYPE_DVSD"));
		lGuidMap.SetAt (MEDIASUBTYPE_MDVF, _T("MEDIASUBTYPE_MDVF"));
		lGuidMap.SetAt (MEDIASUBTYPE_RGB1, _T("MEDIASUBTYPE_RGB1"));
		lGuidMap.SetAt (MEDIASUBTYPE_RGB4, _T("MEDIASUBTYPE_RGB4"));
		lGuidMap.SetAt (MEDIASUBTYPE_RGB8, _T("MEDIASUBTYPE_RGB8"));
		lGuidMap.SetAt (MEDIASUBTYPE_RGB565, _T("MEDIASUBTYPE_RGB565"));
		lGuidMap.SetAt (MEDIASUBTYPE_RGB555, _T("MEDIASUBTYPE_RGB555"));
		lGuidMap.SetAt (MEDIASUBTYPE_RGB24, _T("MEDIASUBTYPE_RGB24"));
		lGuidMap.SetAt (MEDIASUBTYPE_RGB32, _T("MEDIASUBTYPE_RGB32"));
		lGuidMap.SetAt (MEDIASUBTYPE_ARGB1555, _T("MEDIASUBTYPE_ARGB1555"));
		lGuidMap.SetAt (MEDIASUBTYPE_ARGB4444, _T("MEDIASUBTYPE_ARGB4444"));
		lGuidMap.SetAt (MEDIASUBTYPE_ARGB32, _T("MEDIASUBTYPE_ARGB32"));
		lGuidMap.SetAt (MEDIASUBTYPE_A2R10G10B10, _T("MEDIASUBTYPE_A2R10G10B10"));
		lGuidMap.SetAt (MEDIASUBTYPE_A2B10G10R10, _T("MEDIASUBTYPE_A2B10G10R10"));
		lGuidMap.SetAt (MEDIASUBTYPE_AYUV, _T("MEDIASUBTYPE_AYUV"));
		lGuidMap.SetAt (MEDIASUBTYPE_AI44, _T("MEDIASUBTYPE_AI44"));
		lGuidMap.SetAt (MEDIASUBTYPE_IA44, _T("MEDIASUBTYPE_IA44"));
		lGuidMap.SetAt (MEDIASUBTYPE_RGB32_D3D_DX7_RT, _T("MEDIASUBTYPE_RGB32_D3D_DX7_RT"));
		lGuidMap.SetAt (MEDIASUBTYPE_RGB16_D3D_DX7_RT, _T("MEDIASUBTYPE_RGB16_D3D_DX7_RT"));
		lGuidMap.SetAt (MEDIASUBTYPE_ARGB4444_D3D_DX7_RT, _T("MEDIASUBTYPE_ARGB4444_D3D_DX7_RT"));
		lGuidMap.SetAt (MEDIASUBTYPE_ARGB1555_D3D_DX7_RT, _T("MEDIASUBTYPE_ARGB1555_D3D_DX7_RT"));
		lGuidMap.SetAt (MEDIASUBTYPE_RGB32_D3D_DX9_RT, _T("MEDIASUBTYPE_RGB32_D3D_DX9_RT"));
		lGuidMap.SetAt (MEDIASUBTYPE_RGB16_D3D_DX9_RT, _T("MEDIASUBTYPE_RGB16_D3D_DX9_RT"));
		lGuidMap.SetAt (MEDIASUBTYPE_ARGB32_D3D_DX9_RT, _T("MEDIASUBTYPE_ARGB32_D3D_DX9_RT"));
		lGuidMap.SetAt (MEDIASUBTYPE_ARGB4444_D3D_DX9_RT, _T("MEDIASUBTYPE_ARGB4444_D3D_DX9_RT"));
		lGuidMap.SetAt (MEDIASUBTYPE_ARGB1555_D3D_DX9_RT, _T("MEDIASUBTYPE_ARGB1555_D3D_DX9_RT"));
		lGuidMap.SetAt (MEDIASUBTYPE_YV12, _T("MEDIASUBTYPE_YV12"));
		lGuidMap.SetAt (FORMAT_None, _T("MEDIASUBTYPE_NV12"));
		lGuidMap.SetAt (MEDIASUBTYPE_NV12, _T("FORMAT_None"));
		lGuidMap.SetAt (MEDIASUBTYPE_IMC1, _T("MEDIASUBTYPE_IMC1"));
		lGuidMap.SetAt (MEDIASUBTYPE_IMC2, _T("MEDIASUBTYPE_IMC2"));
		lGuidMap.SetAt (MEDIASUBTYPE_IMC3, _T("MEDIASUBTYPE_IMC3"));
		lGuidMap.SetAt (MEDIASUBTYPE_IMC4, _T("MEDIASUBTYPE_IMC4"));
		lGuidMap.SetAt (MEDIASUBTYPE_S340, _T("MEDIASUBTYPE_S340"));
		lGuidMap.SetAt (MEDIASUBTYPE_S342, _T("MEDIASUBTYPE_S342"));
		lGuidMap.SetAt (MEDIASUBTYPE_Overlay, _T("MEDIASUBTYPE_Overlay"));
		lGuidMap.SetAt (MEDIASUBTYPE_MPEG1Packet, _T("MEDIASUBTYPE_MPEG1Packet"));
		lGuidMap.SetAt (MEDIASUBTYPE_MPEG1Payload, _T("MEDIASUBTYPE_MPEG1Payload"));
		lGuidMap.SetAt (MEDIASUBTYPE_MPEG1AudioPayload, _T("MEDIASUBTYPE_MPEG1AudioPayload"));
		lGuidMap.SetAt (MEDIASUBTYPE_MPEG1System, _T("MEDIASUBTYPE_MPEG1System"));
		lGuidMap.SetAt (MEDIASUBTYPE_MPEG1VideoCD, _T("MEDIASUBTYPE_MPEG1VideoCD"));
		lGuidMap.SetAt (MEDIASUBTYPE_MPEG1Video, _T("MEDIASUBTYPE_MPEG1Video"));
		lGuidMap.SetAt (MEDIASUBTYPE_MPEG1Audio, _T("MEDIASUBTYPE_MPEG1Audio"));
		lGuidMap.SetAt (MEDIASUBTYPE_Avi, _T("MEDIASUBTYPE_Avi"));
		lGuidMap.SetAt (MEDIASUBTYPE_Asf, _T("MEDIASUBTYPE_Asf"));
		lGuidMap.SetAt (MEDIASUBTYPE_QTMovie, _T("MEDIASUBTYPE_QTMovie"));
		lGuidMap.SetAt (MEDIASUBTYPE_QTRpza, _T("MEDIASUBTYPE_QTRpza"));
		lGuidMap.SetAt (MEDIASUBTYPE_QTSmc, _T("MEDIASUBTYPE_QTSmc"));
		lGuidMap.SetAt (MEDIASUBTYPE_QTRle, _T("MEDIASUBTYPE_QTRle"));
		lGuidMap.SetAt (MEDIASUBTYPE_QTJpeg, _T("MEDIASUBTYPE_QTJpeg"));
		lGuidMap.SetAt (MEDIASUBTYPE_PCMAudio_Obsolete, _T("MEDIASUBTYPE_PCMAudio_Obsolete"));
		lGuidMap.SetAt (MEDIASUBTYPE_PCM, _T("MEDIASUBTYPE_PCM"));
		lGuidMap.SetAt (MEDIASUBTYPE_WAVE, _T("MEDIASUBTYPE_WAVE"));
		lGuidMap.SetAt (MEDIASUBTYPE_AU, _T("MEDIASUBTYPE_AU"));
		lGuidMap.SetAt (MEDIASUBTYPE_AIFF, _T("MEDIASUBTYPE_AIFF"));
		lGuidMap.SetAt (MEDIASUBTYPE_dvsd, _T("MEDIASUBTYPE_dvsd"));
		lGuidMap.SetAt (MEDIASUBTYPE_dvhd, _T("MEDIASUBTYPE_dvhd"));
		lGuidMap.SetAt (MEDIASUBTYPE_dvsl, _T("MEDIASUBTYPE_dvsl"));
		lGuidMap.SetAt (MEDIASUBTYPE_dv25, _T("MEDIASUBTYPE_dv25"));
		lGuidMap.SetAt (MEDIASUBTYPE_dv50, _T("MEDIASUBTYPE_dv50"));
		lGuidMap.SetAt (MEDIASUBTYPE_dvh1, _T("MEDIASUBTYPE_dvh1"));
		lGuidMap.SetAt (MEDIASUBTYPE_Line21_BytePair, _T("MEDIASUBTYPE_Line21_BytePair"));
		lGuidMap.SetAt (MEDIASUBTYPE_Line21_GOPPacket, _T("MEDIASUBTYPE_Line21_GOPPacket"));
		lGuidMap.SetAt (MEDIASUBTYPE_Line21_VBIRawData, _T("MEDIASUBTYPE_Line21_VBIRawData"));
		lGuidMap.SetAt (MEDIASUBTYPE_TELETEXT, _T("MEDIASUBTYPE_TELETEXT"));
		lGuidMap.SetAt (MEDIASUBTYPE_DRM_Audio, _T("MEDIASUBTYPE_DRM_Audio"));
		lGuidMap.SetAt (MEDIASUBTYPE_IEEE_FLOAT, _T("MEDIASUBTYPE_IEEE_FLOAT"));
		lGuidMap.SetAt (MEDIASUBTYPE_DOLBY_AC3_SPDIF, _T("MEDIASUBTYPE_DOLBY_AC3_SPDIF"));
		lGuidMap.SetAt (MEDIASUBTYPE_RAW_SPORT, _T("MEDIASUBTYPE_RAW_SPORT"));
		lGuidMap.SetAt (MEDIASUBTYPE_SPDIF_TAG_241h, _T("MEDIASUBTYPE_SPDIF_TAG_241h"));
		lGuidMap.SetAt (MEDIASUBTYPE_DssVideo, _T("MEDIASUBTYPE_DssVideo"));
		lGuidMap.SetAt (MEDIASUBTYPE_DssAudio, _T("MEDIASUBTYPE_DssAudio"));
		lGuidMap.SetAt (MEDIASUBTYPE_VPVideo, _T("MEDIASUBTYPE_VPVideo"));
		lGuidMap.SetAt (MEDIASUBTYPE_VPVBI, _T("MEDIASUBTYPE_VPVBI"));
		lGuidMap.SetAt (MEDIASUBTYPE_AnalogVideo_NTSC_M, _T("MEDIASUBTYPE_AnalogVideo_NTSC_M"));
		lGuidMap.SetAt (MEDIASUBTYPE_AnalogVideo_PAL_B, _T("MEDIASUBTYPE_AnalogVideo_PAL_B"));
		lGuidMap.SetAt (MEDIASUBTYPE_AnalogVideo_PAL_D, _T("MEDIASUBTYPE_AnalogVideo_PAL_D"));
		lGuidMap.SetAt (MEDIASUBTYPE_AnalogVideo_PAL_G, _T("MEDIASUBTYPE_AnalogVideo_PAL_G"));
		lGuidMap.SetAt (MEDIASUBTYPE_AnalogVideo_PAL_H, _T("MEDIASUBTYPE_AnalogVideo_PAL_H"));
		lGuidMap.SetAt (MEDIASUBTYPE_AnalogVideo_PAL_I, _T("MEDIASUBTYPE_AnalogVideo_PAL_I"));
		lGuidMap.SetAt (MEDIASUBTYPE_AnalogVideo_PAL_M, _T("MEDIASUBTYPE_AnalogVideo_PAL_M"));
		lGuidMap.SetAt (MEDIASUBTYPE_AnalogVideo_PAL_N, _T("MEDIASUBTYPE_AnalogVideo_PAL_N"));
		lGuidMap.SetAt (MEDIASUBTYPE_AnalogVideo_PAL_N_COMBO, _T("MEDIASUBTYPE_AnalogVideo_PAL_N_COMBO"));
		lGuidMap.SetAt (MEDIASUBTYPE_AnalogVideo_SECAM_B, _T("MEDIASUBTYPE_AnalogVideo_SECAM_B"));
		lGuidMap.SetAt (MEDIASUBTYPE_AnalogVideo_SECAM_D, _T("MEDIASUBTYPE_AnalogVideo_SECAM_D"));
		lGuidMap.SetAt (MEDIASUBTYPE_AnalogVideo_SECAM_G, _T("MEDIASUBTYPE_AnalogVideo_SECAM_G"));
		lGuidMap.SetAt (MEDIASUBTYPE_AnalogVideo_SECAM_H, _T("MEDIASUBTYPE_AnalogVideo_SECAM_H"));
		lGuidMap.SetAt (MEDIASUBTYPE_AnalogVideo_SECAM_K, _T("MEDIASUBTYPE_AnalogVideo_SECAM_K"));
		lGuidMap.SetAt (MEDIASUBTYPE_AnalogVideo_SECAM_K1, _T("MEDIASUBTYPE_AnalogVideo_SECAM_K1"));
		lGuidMap.SetAt (MEDIASUBTYPE_AnalogVideo_SECAM_L, _T("MEDIASUBTYPE_AnalogVideo_SECAM_L"));
/**/	lGuidMap.SetAt (KSDATAFORMAT_TYPE_VIDEO, _T("KSDATAFORMAT_TYPE_VIDEO"));
/**/	lGuidMap.SetAt (KSDATAFORMAT_TYPE_AUDIO, _T("KSDATAFORMAT_TYPE_AUDIO"));
/**/	lGuidMap.SetAt (KSDATAFORMAT_TYPE_TEXT, _T("KSDATAFORMAT_TYPE_TEXT"));
/**/	lGuidMap.SetAt (KSDATAFORMAT_TYPE_MUSIC, _T("KSDATAFORMAT_TYPE_MUSIC"));
/**/	lGuidMap.SetAt (KSDATAFORMAT_TYPE_MIDI, _T("KSDATAFORMAT_TYPE_MIDI"));
/**/	lGuidMap.SetAt (KSDATAFORMAT_SUBTYPE_PCM, _T("KSDATAFORMAT_SUBTYPE_PCM"));
/**/	lGuidMap.SetAt (KSDATAFORMAT_SUBTYPE_IEEE_FLOAT, _T("KSDATAFORMAT_SUBTYPE_IEEE_FLOAT"));
		lGuidMap.SetAt (KSDATAFORMAT_SUBTYPE_ANALOG, _T("KSDATAFORMAT_SUBTYPE_ANALOG"));
		lGuidMap.SetAt (KSDATAFORMAT_SUBTYPE_WAVEFORMATEX, _T("KSDATAFORMAT_SUBTYPE_WAVEFORMATEX"));
		lGuidMap.SetAt (KSDATAFORMAT_SUBTYPE_ALAW, _T("KSDATAFORMAT_SUBTYPE_ALAW"));
		lGuidMap.SetAt (KSDATAFORMAT_SUBTYPE_MULAW, _T("KSDATAFORMAT_SUBTYPE_MULAW"));
		lGuidMap.SetAt (KSDATAFORMAT_SUBTYPE_ADPCM, _T("KSDATAFORMAT_SUBTYPE_ADPCM"));
		lGuidMap.SetAt (KSDATAFORMAT_SUBTYPE_MPEG, _T("KSDATAFORMAT_SUBTYPE_MPEG"));
		lGuidMap.SetAt (KSDATAFORMAT_SUBTYPE_MIDI, _T("KSDATAFORMAT_SUBTYPE_MIDI"));
		lGuidMap.SetAt (KSDATAFORMAT_SUBTYPE_MIDI_BUS, _T("KSDATAFORMAT_SUBTYPE_MIDI_BUS"));
		lGuidMap.SetAt (KSDATAFORMAT_SUBTYPE_RIFFMIDI, _T("KSDATAFORMAT_SUBTYPE_RIFFMIDI"));
		lGuidMap.SetAt (KSDATAFORMAT_SPECIFIER_VC_ID, _T("KSDATAFORMAT_SPECIFIER_VC_ID"));
		lGuidMap.SetAt (KSDATAFORMAT_SPECIFIER_WAVEFORMATEX, _T("KSDATAFORMAT_SPECIFIER_WAVEFORMATEX"));
		lGuidMap.SetAt (KSDATAFORMAT_SPECIFIER_DSOUND, _T("KSDATAFORMAT_SPECIFIER_DSOUND"));

		lGuidMap.SetAt (FORMAT_None, _T("FORMAT_None"));
		lGuidMap.SetAt (FORMAT_VideoInfo, _T("FORMAT_VideoInfo"));
		lGuidMap.SetAt (FORMAT_VideoInfo2, _T("FORMAT_VideoInfo2"));
		lGuidMap.SetAt (FORMAT_WaveFormatEx, _T("FORMAT_WaveFormatEx"));
		lGuidMap.SetAt (FORMAT_MPEGVideo, _T("FORMAT_MPEGVideo"));
		lGuidMap.SetAt (FORMAT_MPEGStreams, _T("FORMAT_MPEGStreams"));
		lGuidMap.SetAt (FORMAT_DvInfo, _T("FORMAT_DvInfo"));

		lGuidMap.SetAt (TIME_FORMAT_FRAME, _T("TIME_FORMAT_FRAME"));
		lGuidMap.SetAt (TIME_FORMAT_BYTE, _T("TIME_FORMAT_BYTE"));
		lGuidMap.SetAt (TIME_FORMAT_SAMPLE, _T("TIME_FORMAT_SAMPLE"));
		lGuidMap.SetAt (TIME_FORMAT_FIELD, _T("TIME_FORMAT_FIELD"));
		lGuidMap.SetAt (TIME_FORMAT_MEDIA_TIME, _T("TIME_FORMAT_MEDIA_TIME"));
	}

	if	(!lGuidMap.Lookup (pGuid, lRet))
#endif
	{
		lRet = (CString&) CGuidStr (pGuid);
	}
	return lRet;
}
