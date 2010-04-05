/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include <mmreg.h>
#include "DirectShowUtils.h"
#include "AgentStreamUtils.h"
#include "MmSysError.h"
#include "MallocPtr.h"
#include "GuidStr.h"
#ifdef	_DEBUG
#include "DebugStr.h"
#include "DebugWin.h"
#include "MmSysError.h"
#endif

#pragma comment(lib, "msdmo.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowUtils::GetMonikerFilter (LPCTSTR pMonikerName, IBaseFilter ** pFilter, CString & pFilterName)
{
	HRESULT	lResult = S_FALSE;

	try
	{
		IBindCtxPtr				lBindCtx;
		IMonikerPtr				lMoniker;
		ULONG					lEaten = 0;
		tMallocPtr <OLECHAR>	lDisplayName;
		IPropertyBagPtr			lPropBag;
		COleVariant				lFriendlyName;

		if	(
				(pMonikerName)
			&&	(*pMonikerName)
			&&	(SUCCEEDED (CreateBindCtx (0, &lBindCtx)))
			&&	(SUCCEEDED (MkParseDisplayName (lBindCtx, lDisplayName=AfxAllocTaskOleString (pMonikerName), &lEaten, &lMoniker)))
			)
		{
			if	(
					(SUCCEEDED (lMoniker->BindToStorage (NULL, NULL, IID_IPropertyBag, (void **) &lPropBag)))
				&&	(SUCCEEDED (lPropBag->Read (L"FriendlyName", &lFriendlyName, NULL)))
				&&	(V_VT (&lFriendlyName) == VT_BSTR)
				)
			{
				lResult = lMoniker->BindToObject (NULL, NULL, IID_IBaseFilter, (void **) pFilter);

				if	(lResult == S_OK)
				{
					pFilterName = V_BSTR (&lFriendlyName);
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowUtils::GetDefaultFilter (const GUID & pCategory, IBaseFilter ** pFilter, CString & pFilterName)
{
	HRESULT	lResult = S_FALSE;

	try
	{
		IBindCtxPtr				lBindCtx;
		IMonikerPtr				lMoniker;
		ULONG					lEaten = 0;
		CString					lMonikerName;
		tMallocPtr <OLECHAR>	lDisplayName;
		IPropertyBagPtr			lPropBag;
		COleVariant				lFriendlyName;

		lMonikerName.Format (_T("@device:*:%s"), (CString) CGuidStr (pCategory));

		if	(
				(SUCCEEDED (CreateBindCtx (0, &lBindCtx)))
			&&	(SUCCEEDED (MkParseDisplayName (lBindCtx, lDisplayName=AfxAllocTaskOleString (lMonikerName), &lEaten, &lMoniker)))
			&&	(SUCCEEDED (lMoniker->GetDisplayName (NULL, NULL, lDisplayName.Free ())))
			&&	(SUCCEEDED (MkParseDisplayName (lBindCtx, lDisplayName, &lEaten, &lMoniker)))
			)
		{
			if	(
					(SUCCEEDED (lMoniker->BindToStorage (NULL, NULL, IID_IPropertyBag, (void **) &lPropBag)))
				&&	(SUCCEEDED (lPropBag->Read (L"FriendlyName", &lFriendlyName, NULL)))
				&&	(V_VT (&lFriendlyName) == VT_BSTR)
				)
			{
				lResult = lMoniker->BindToObject (NULL, NULL, IID_IBaseFilter, (void **) pFilter);

				if	(lResult == S_OK)
				{
					pFilterName = V_BSTR (&lFriendlyName);
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowUtils::ConnectFilters (IGraphBuilder * pGraphBuilder, IBaseFilter * pDownstreamFilter, IBaseFilter * pUpstreamFilter, bool pDirect, const AM_MEDIA_TYPE * pDirectMediaType, IPin ** pDownstreamPin, IPin ** pUpstreamPin)
{
	HRESULT			lResult = E_FAIL;
	IEnumPinsPtr	lUpstreamPins;
	IPinPtr			lUpstreamPin;
	IPinPtr			lDownstreamPin;
	PIN_DIRECTION	lPinDir;

	if	(pDownstreamPin)
	{
		(*pDownstreamPin) = NULL;
	}
	if	(pUpstreamPin)
	{
		(*pUpstreamPin) = NULL;
	}

	if	(
			(SUCCEEDED (lResult = pUpstreamFilter->EnumPins (&lUpstreamPins)))
		&&	(SUCCEEDED (lResult = GetFilterPins (pDownstreamFilter, &lDownstreamPin, NULL)))
		)
	{
		while (lUpstreamPins->Next (1, &lUpstreamPin, NULL) == S_OK)
		{
			IPinPtr			lConnected;
			IAsyncReaderPtr	lAsyncReader (lUpstreamPin);

			if	(
					(SUCCEEDED (lUpstreamPin->QueryDirection (&lPinDir)))
				&&	(lPinDir == PINDIR_OUTPUT)
				&&	(
						(FAILED (lUpstreamPin->ConnectedTo (&lConnected)))
					||	(lConnected == NULL)
					)
				)
			{
				if	(pDirect)
				{
					if	(lAsyncReader == NULL)
					{
						if	(SUCCEEDED (lResult = pGraphBuilder->ConnectDirect (lUpstreamPin, lDownstreamPin, pDirectMediaType)))
						{
							break;
						}
					}
					else
					{
						if	(SUCCEEDED (lResult = pGraphBuilder->ConnectDirect (lDownstreamPin, lUpstreamPin, pDirectMediaType)))
						{
							break;
						}
					}
				}
				else
				{
					if	(lAsyncReader == NULL)
					{
						if	(SUCCEEDED (lResult = pGraphBuilder->Connect (lUpstreamPin, lDownstreamPin)))
						{
							break;
						}
					}
					else
					{
						if	(SUCCEEDED (lResult = pGraphBuilder->Connect (lDownstreamPin, lUpstreamPin)))
						{
							break;
						}
					}
				}
			}
		}
		if	(
				(SUCCEEDED (lResult))
			&&	(lUpstreamPin == NULL)
			)
		{
			lResult = E_FAIL;
		}
	}
	if	(SUCCEEDED (lResult))
	{
		if	(pDownstreamPin)
		{
			(*pDownstreamPin) = lDownstreamPin.Detach();
		}
		if	(pUpstreamPin)
		{
			(*pUpstreamPin) = lUpstreamPin.Detach();
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowUtils::GetFilterPins (IBaseFilter * pFilter, IPin ** pInputPin, IPin ** pOutputPin)
{
	HRESULT			lResult;
	IEnumPinsPtr	lPins;
	IPinPtr			lPin;
	PIN_DIRECTION	lPinDir;

	if	(pInputPin)
	{
		*pInputPin = NULL;
	}
	if	(pOutputPin)
	{
		*pOutputPin = NULL;
	}

	if	(SUCCEEDED (lResult = pFilter->EnumPins (&lPins)))
	{
		while	(lPins->Next (1, &lPin, NULL) == S_OK)
		{
			if	(SUCCEEDED (lPin->QueryDirection (&lPinDir)))
			{
				if	(
						(lPinDir == PINDIR_INPUT)
					&&	(pInputPin)
					&&	(*pInputPin == NULL)
					)
				{
					*pInputPin = lPin.Detach();
				}
				else
				if	(
						(lPinDir == PINDIR_OUTPUT)
					&&	(pOutputPin)
					&&	(*pOutputPin == NULL)
					)
				{
					*pOutputPin = lPin.Detach();
				}
			}
			if	(
					(
						(!pInputPin)
					||	(*pInputPin != NULL)
					)
				&&	(
						(!pOutputPin)
					||	(*pOutputPin != NULL)
					)
				)
			{
				break;
			}
		}
	}

	if	(
			(SUCCEEDED (lResult))
		&&	(
				(
					(pInputPin)
				&&	(*pInputPin == NULL)
				)
			||	(
					(pOutputPin)
				&&	(*pOutputPin == NULL)
				)
			)
		)
	{
		lResult = E_FAIL;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowUtils::GetFilterPin (IBaseFilter * pFilter, PIN_DIRECTION pPinDir, REFGUID pMajorMediaType, IPin ** pPin, AM_MEDIA_TYPE** pPinMediaType)
{
	HRESULT				lResult;
	IEnumPinsPtr		lPins;
	IPinPtr				lPin;
	PIN_DIRECTION		lPinDir;
	IEnumMediaTypesPtr	lMediaTypes;
	AM_MEDIA_TYPE_Safe	lMediaType;
	tMediaTypePtr		lPinMediaType;

	if	(pPin)
	{
		*pPin = NULL;
	}
	if	(pPinMediaType)
	{
		*pPinMediaType = NULL;
	}

	if	(SUCCEEDED (lResult = pFilter->EnumPins (&lPins)))
	{
		while	(lPins->Next (1, &lPin, NULL) == S_OK)
		{
			if	(
					(SUCCEEDED (lPin->QueryDirection (&lPinDir)))
				&&	(lPinDir == pPinDir)
				)
			{
				if	(SUCCEEDED (lPin->ConnectionMediaType (&lMediaType)))
				{
					if	(IsEqualGUID (lMediaType.majortype, pMajorMediaType))
					{
						MoDuplicateMediaType ((DMO_MEDIA_TYPE **)lPinMediaType.Free(), lMediaType);
						break;
					}
				}
				else
				if	(SUCCEEDED (lPin->EnumMediaTypes (&lMediaTypes)))
				{
					while (lMediaTypes->Next (1, lPinMediaType.Free(), NULL) == S_OK)
					{
						if	(IsEqualGUID (lPinMediaType->majortype, pMajorMediaType))
						{
							break;
						}
					}
					if	(lPinMediaType.Ptr())
					{
						break;
					}
				}
			}
		}
	}

	if	(
			(SUCCEEDED (lResult))
		&&	(lPin != NULL)
		)
	{
		if	(pPin)
		{
			*pPin = lPin.Detach();
		}
		if	(pPinMediaType)
		{
			*pPinMediaType = lPinMediaType.Detach ();
		}
	}
	else
	{
		lResult = VFW_E_NOT_FOUND;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowUtils::GetRenderType (IPin * pPin, GUID & pRenderType)
{
	HRESULT	lResult = E_FAIL;

	pRenderType = GUID_NULL;

	try
	{
		AM_MEDIA_TYPE_Safe	lPinMediaType;

		if	(
				(pPin)
			&&	(SUCCEEDED (lResult = pPin->ConnectionMediaType (&lPinMediaType)))
			)
		{
			pRenderType = lPinMediaType.subtype;
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowUtils::EmptyFilterCache (IFilterGraph * pFilterGraph)
{
	IGraphConfigPtr	lGraphConfig (pFilterGraph);

	return EmptyFilterCache (lGraphConfig);
}

HRESULT CDirectShowUtils::EmptyFilterCache (IGraphConfig * pGraphConfig)
{
	HRESULT	lResult = E_FAIL;

	try
	{
		IEnumFiltersPtr								lEnumFilters;
		IBaseFilterPtr								lFilter;
		CArrayEx <IBaseFilterPtr, IBaseFilter *>	lFilters;
		INT_PTR										lFilterNdx;

		if	(
				(pGraphConfig)
			&&	(SUCCEEDED (lResult = pGraphConfig->EnumCacheFilter (&lEnumFilters)))
			)
		{
			lResult = S_FALSE;

			while (lEnumFilters->Next (1, &lFilter, NULL) == S_OK)
			{
				lFilters.Add (lFilter);
			}
			for	(lFilterNdx = 0; lFilterNdx <= lFilters.GetUpperBound (); lFilterNdx++)
			{
				lFilter = NULL;
				lFilter.Attach (lFilters [lFilterNdx].Detach ());
				if	(SUCCEEDED (LogVfwErr (LogNormal, pGraphConfig->RemoveFilterFromCache (lFilter))))
				{
					lResult = S_OK;
				}
				lFilter = NULL;
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowUtils::MoveFilterToCache (IBaseFilter * pFilter, IFilterGraph * pFilterGraph)
{
	HRESULT	lResult = E_FAIL;

	try
	{
		FILTER_INFO_Safe	lFilterInfo;

		if	(
				(pFilter)
			&&	(!pFilterGraph)
			&&	(SUCCEEDED (pFilter->QueryFilterInfo (&lFilterInfo)))
			)
		{
			pFilterGraph = lFilterInfo.pGraph;
		}
		if	(
				(pFilter)
			&&	(pFilterGraph)
			&&	(SUCCEEDED (lResult = pFilterGraph->RemoveFilter (pFilter)))
			)
		{
			lResult = AddFilterToCache (pFilter, pFilterGraph);
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowUtils::AddFilterToCache (IBaseFilter * pFilter, IFilterGraph * pFilterGraph)
{
	IGraphConfigPtr	lGraphConfig (pFilterGraph);

	return AddFilterToCache (pFilter, lGraphConfig);
}

HRESULT CDirectShowUtils::AddFilterToCache (IBaseFilter * pFilter, IGraphConfig * pGraphConfig)
{
	HRESULT	lResult = E_FAIL;

	try
	{
		if	(
				(pFilter != NULL)
			&&	(pGraphConfig != NULL)
			)
		{
			lResult = pGraphConfig->AddFilterToCache (pFilter);
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowUtils::MoveFiltersToCache (IBaseFilter * pStartFilter, IBaseFilter * pEndFilter, IFilterGraph * pFilterGraph)
{
	HRESULT	lResult = E_FAIL;

	try
	{
		FILTER_INFO_Safe	lFilterInfo;
		IFilterChainPtr		lFilterChain (pFilterGraph);

		if	(
				(pStartFilter)
			&&	(!pFilterGraph)
			&&	(SUCCEEDED (pStartFilter->QueryFilterInfo (&lFilterInfo)))
			)
		{
			lFilterChain = lFilterInfo.pGraph;
		}

		lResult = MoveFiltersToCache (pStartFilter, pEndFilter, lFilterChain);
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT CDirectShowUtils::MoveFiltersToCache (IBaseFilter * pStartFilter, IBaseFilter * pEndFilter, IFilterChain * pFilterChain, IBaseFilter * pExcludeFilter, ...)
{
	HRESULT	lResult = E_FAIL;

	try
	{
		IGraphConfigPtr								lGraphConfig (pFilterChain);
		CPtrTypeArray <IBaseFilter>					lExcludeFilters;
		CArrayEx <IBaseFilterPtr, IBaseFilter *>	lFilters;
		INT_PTR										lFilterNdx;

		if	(pExcludeFilter != NULL)
		{
			va_list			lArgPtr;
			IBaseFilter *	lExcludeFilter;

			va_start (lArgPtr, pExcludeFilter);
			for	(lExcludeFilter = pExcludeFilter; lExcludeFilter; lExcludeFilter = va_arg (lArgPtr, IBaseFilter *))
			{
				lExcludeFilters.Add (lExcludeFilter);
			}
			va_end (lArgPtr);
		}

		if	(
				(lGraphConfig != NULL)
			&&	(pStartFilter != NULL)
			&&	(pEndFilter != NULL)
			)
		{
			IBaseFilterPtr	lFilter (pStartFilter);
			IPinPtr			lInputPin;
			IPinPtr			lOutputPin;
			PIN_INFO_Safe	lPinInfo;

			if	(lExcludeFilters.Find (pStartFilter) < 0)
			{
				lFilters.Add (pStartFilter);
			}

			while	(
						(lFilter != NULL)
					&&	(SUCCEEDED (GetFilterPins (lFilter, NULL, &lOutputPin)))
					&&	(lOutputPin != NULL)
					&&	(SUCCEEDED (lOutputPin->ConnectedTo (&lInputPin)))
					&&	(lInputPin != NULL)
					&&	(SUCCEEDED (lInputPin->QueryPinInfo (lPinInfo.Clear())))
					)
			{
				if	(lExcludeFilters.Find (lPinInfo.pFilter) < 0)
				{
					lFilters.Add (lPinInfo.pFilter);
				}
				if	(lPinInfo.pFilter == pEndFilter)
				{
					break;
				}
				else
				{
					lFilter = lPinInfo.pFilter;
				}
			}

			if	(SUCCEEDED (lResult = pFilterChain->RemoveChain (pStartFilter, pEndFilter)))
			{
				lResult = S_FALSE;

				for	(lFilterNdx = 0; lFilterNdx <= lFilters.GetUpperBound (); lFilterNdx++)
				{
					if	(SUCCEEDED (lGraphConfig->AddFilterToCache (lFilters [lFilterNdx])))
					{
						lResult = S_OK;
					}
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowUtils::FindFilterInCache (REFGUID pFilterClass, IFilterGraph * pFilterGraph, IBaseFilter ** pFilter)
{
	IGraphConfigPtr	lGraphConfig (pFilterGraph);
	return FindFilterInCache (pFilterClass, lGraphConfig, pFilter);
}

HRESULT CDirectShowUtils::FindFilterInCache (REFGUID pFilterClass, IGraphConfig * pGraphConfig, IBaseFilter ** pFilter)
{
	HRESULT	lResult = E_FAIL;

	try
	{
		IEnumFiltersPtr	lEnumFilters;
		IBaseFilterPtr	lFilter;
		GUID			lFilterClass;

		(*pFilter) = NULL;

		if	(
				(pGraphConfig)
			&&	(SUCCEEDED (lResult = pGraphConfig->EnumCacheFilter (&lEnumFilters)))
			)
		{
			lResult = S_FALSE;

			while (lEnumFilters->Next (1, &lFilter, NULL) == S_OK)
			{
				if	(
						(SUCCEEDED (lFilter->GetClassID (&lFilterClass)))
					&&	(IsEqualGUID (lFilterClass, pFilterClass))
					)
				{
					(*pFilter) = lFilter.Detach();
					lResult = S_OK;
					break;
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowUtils::GetFilterFromCache (REFGUID pFilterClass, IFilterGraph * pFilterGraph, IBaseFilter ** pFilter)
{
	IGraphConfigPtr	lGraphConfig (pFilterGraph);
	return GetFilterFromCache (pFilterClass, lGraphConfig, pFilter);
}

HRESULT CDirectShowUtils::GetFilterFromCache (REFGUID pFilterClass, IGraphConfig * pGraphConfig, IBaseFilter ** pFilter)
{
	HRESULT	lResult = E_FAIL;

	try
	{
		lResult = FindFilterInCache (pFilterClass, pGraphConfig, pFilter);
		if	(lResult == S_OK)
		{
			lResult = pGraphConfig->RemoveFilterFromCache (*pFilter);
		}
	}

	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowUtils::ConnectNullRenderer (IGraphBuilder * pGraphBuilder, IBaseFilter * pUpstreamFilter, IPin * pUpstreamPin, AM_MEDIA_TYPE* pMediaType)
{
	HRESULT	lResult = E_FAIL;

	try
	{
		IPinPtr			lUpstreamPin (pUpstreamPin);
		IBaseFilterPtr	lNullRenderer;

		if	(lUpstreamPin == NULL)
		{
			if	(pMediaType == NULL)
			{
				lResult = GetFilterPins (pUpstreamFilter, NULL, &lUpstreamPin);
			}
			else
			{
				lResult = GetFilterPin (pUpstreamFilter, PINDIR_OUTPUT, pMediaType->majortype, &lUpstreamPin);
			}
		}

		if	(
				(lUpstreamPin != NULL)
			&&	(SUCCEEDED (lResult = LogComErr (LogNormal, CoCreateInstance (__uuidof(NullRenderer), NULL, CLSCTX_INPROC, __uuidof (IBaseFilter), (void **) &lNullRenderer))))
			&&	(SUCCEEDED (lResult = LogVfwErr (LogNormal, pGraphBuilder->AddFilter (lNullRenderer, L"Null Render"))))
			)
		{
			lResult = LogComErr (LogNormal, ConnectFilters (pGraphBuilder, lNullRenderer, pUpstreamFilter, true, pMediaType));
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowUtils::InitPinAllocator (IPin * pInputPin, AM_MEDIA_TYPE* pPinMediaType)
{
	HRESULT	lResult = E_FAIL;

	try
	{
		AM_MEDIA_TYPE_Safe			lPinMediaType;
		IMemInputPinPtr				lTranport (pInputPin);
		IMemAllocatorPtr			lAllocator;
		tS <ALLOCATOR_PROPERTIES>	lRequiredMem;
		tS <ALLOCATOR_PROPERTIES>	lActualMem;

		if	(pInputPin)
		{
			if	(
					(!pPinMediaType)
				&&	(SUCCEEDED (pInputPin->ConnectionMediaType (&lPinMediaType)))
				)
			{
				pPinMediaType = &lPinMediaType;
			}
			if	(
					(pPinMediaType != NULL)
				&&	(lTranport != NULL)
				)
			{
				lTranport->GetAllocatorRequirements (lRequiredMem.Clear());

				lRequiredMem.cBuffers = max (lRequiredMem.cBuffers, 1);
				if	(lRequiredMem.cbAlign <= 1)
				{
					lRequiredMem.cbAlign = 8;
				}
				if	(lRequiredMem.cbBuffer == 0)
				{
					lRequiredMem.cbBuffer = pPinMediaType->lSampleSize;
				}

				if	(
						(SUCCEEDED (lResult = CoCreateInstance (CLSID_MemoryAllocator, NULL, CLSCTX_SERVER, __uuidof(IMemAllocator), (void**)&lAllocator)))
					&&	(SUCCEEDED (lResult = lAllocator->SetProperties (&lRequiredMem, lActualMem.Clear())))
					)
				{
					lResult = lTranport->NotifyAllocator (lAllocator, FALSE);
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectShowUtils::CopySample (IMediaSample * pSource, IMediaSample * pTarget)
{
	HRESULT	lResult = E_FAIL;

	if	(
			(pSource != NULL)
		&&	(pTarget != NULL)
		)
	{
		try
		{
			IMediaSample2Ptr	lSourceSample2 (pSource);
			IMediaSample2Ptr	lTargetSample2 (pTarget);
			long				lSourceSize;
			long				lSourceLength;
			LPBYTE				lSourceBuffer;
			long				lTargetSize;
			LPBYTE				lTargetBuffer;

			if	(
					(lSourceSample2 != NULL)
				&&	(lTargetSample2 != NULL)
				)
			{
				tSS <AM_SAMPLE2_PROPERTIES, DWORD>	lSourceProps;
				tSS <AM_SAMPLE2_PROPERTIES, DWORD>	lTargetProps;

				if	(
						(SUCCEEDED (lResult = lSourceSample2->GetProperties (sizeof(lSourceProps), (LPBYTE)lSourceProps.Clear())))
					&&	(SUCCEEDED (lResult = lTargetSample2->GetProperties (sizeof(lTargetProps), (LPBYTE)lTargetProps.Clear())))
					)
				{
					lTargetProps.dwTypeSpecificFlags = lSourceProps.dwTypeSpecificFlags;
					lTargetProps.dwSampleFlags = lSourceProps.dwSampleFlags;
					lTargetProps.tStart = lSourceProps.tStart;
					lTargetProps.tStop = lSourceProps.tStop;
					lTargetProps.dwStreamId = lSourceProps.dwStreamId;
					lResult = LogVfwErr (LogNormal, lTargetSample2->SetProperties (sizeof(lTargetProps), (LPBYTE)&lTargetProps));
				}
			}
			else
			{
				REFERENCE_TIME	lStartTime;
				REFERENCE_TIME	lEndTime;
				REFERENCE_TIME	lMediaStartTime;
				REFERENCE_TIME	lMediaEndTime;
				tMediaTypePtr	lMediaType;

				if	(
						(SUCCEEDED (lResult = pSource->GetTime (&lStartTime, &lEndTime)))
					&&	(SUCCEEDED (lResult = pSource->GetMediaType (lMediaType.Free())))
					&&	(SUCCEEDED (lResult = pTarget->SetTime (&lStartTime, &lEndTime)))
					&&	(SUCCEEDED (lResult = pTarget->SetMediaType (lMediaType)))
					)
				{
					if	(SUCCEEDED (pSource->GetMediaTime (&lMediaStartTime, &lMediaEndTime)))
					{
						pTarget->SetMediaTime (&lMediaStartTime, &lMediaEndTime);
					}
					pTarget->SetDiscontinuity (pSource->IsDiscontinuity () == S_OK);
					pTarget->SetPreroll (pSource->IsPreroll () == S_OK);
					pTarget->SetSyncPoint (pSource->IsSyncPoint () == S_OK);
				}
			}

			if	(
					(SUCCEEDED (lResult))
				&&	(lSourceSize = pSource->GetSize ())
				&&	(lTargetSize = pTarget->GetSize ())
				&&	(lTargetSize >= lSourceSize)
				&&	(lSourceLength = pSource->GetActualDataLength ())
				&&	(SUCCEEDED (lResult = pSource->GetPointer (&lSourceBuffer)))
				&&	(SUCCEEDED (lResult = pTarget->GetPointer (&lTargetBuffer)))
				)
			{
				memcpy (lTargetBuffer, lSourceBuffer, lSourceLength);
				lResult = pTarget->SetActualDataLength (lSourceLength);
			}
			else
			if	(SUCCEEDED (lResult))
			{
				lResult = E_OUTOFMEMORY;
			}
		}
		catch AnyExceptionDebug
	}
	return lResult;
}

HRESULT CDirectShowUtils::DuplicateSample (IMediaSample * pSource, IMemAllocator * pAllocator, IMediaSample ** pDuplicate, DWORD pFlags)
{
	HRESULT	lResult = E_FAIL;

	if	(
			(pSource != NULL)
		&&	(pDuplicate != NULL)
		&&	(pAllocator != NULL)
		)
	{
		try
		{
			REFERENCE_TIME	lStartTime;
			REFERENCE_TIME	lEndTime;

			if	(
					(SUCCEEDED (pSource->GetTime (&lStartTime, &lEndTime)))
				&&	(SUCCEEDED (lResult = pAllocator->GetBuffer (pDuplicate, &lStartTime, &lEndTime, pFlags&AM_GBF_NOWAIT)))
				)
			{
				lResult = CopySample (pSource, *pDuplicate);
			}
		}
		catch AnyExceptionDebug
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LPWAVEFORMATEX CDirectShowUtils::GetSoundFormat (LPCVOID pSoundData, long pSoundSize, long * pFormatSize, UINT pLogLevel)
{
	tPtr <WAVEFORMATEX>	lSoundFormat;

	if	(pFormatSize)
	{
		(*pFormatSize) = 0;
	}

	if	(
			(pSoundData)
		&&	(pSoundSize > 0)
		)
	{
		try
		{
			HMMIO			lSoundHandle = NULL;
			tS <MMIOINFO>	lSoundInfo;

			lSoundInfo.fccIOProc = FOURCC_MEM;
			lSoundInfo.pchBuffer = (HPSTR) pSoundData;
			lSoundInfo.cchBuffer = pSoundSize;

			if	(lSoundHandle = mmioOpen (NULL, &lSoundInfo, MMIO_READ))
			{
				lSoundFormat = GetSoundFormat (lSoundHandle, pFormatSize, pLogLevel);
				LogMmSysErr (pLogLevel, mmioClose (lSoundHandle, 0));
			}
			else
			{
				LogMmSysErr (pLogLevel, lSoundInfo.wErrorRet);
			}
		}
		catch AnyExceptionDebug
	}
	return lSoundFormat.Detach ();
}

LPWAVEFORMATEX CDirectShowUtils::GetSoundFormat (LPCTSTR pSoundFileName, long * pFormatSize, UINT pLogLevel)
{
	tPtr <WAVEFORMATEX>	lSoundFormat;

	if	(pFormatSize)
	{
		(*pFormatSize) = 0;
	}

	if	(
			(pSoundFileName)
		&&	(*pSoundFileName)
		)
	{
		try
		{
			HMMIO			lSoundHandle = NULL;
			tS <MMIOINFO>	lSoundInfo;
			CString			lSoundFileName (pSoundFileName);

			if	(lSoundHandle = mmioOpen (lSoundFileName.GetBuffer(MAX_PATH), &lSoundInfo, MMIO_READ|MMIO_DENYNONE))
			{
				lSoundFormat = GetSoundFormat (lSoundHandle, pFormatSize, pLogLevel);
				LogMmSysErr (pLogLevel, mmioClose (lSoundHandle, 0));
			}
			else
			{
				LogMmSysErr (pLogLevel, lSoundInfo.wErrorRet);
			}
		}
		catch AnyExceptionDebug
	}
	return lSoundFormat.Detach ();
}

LPWAVEFORMATEX CDirectShowUtils::GetSoundFormat (HMMIO pSound, long * pFormatSize, UINT pLogLevel)
{
	tPtr <WAVEFORMATEX>	lSoundFormat;

	if	(pFormatSize)
	{
		(*pFormatSize) = 0;
	}

	if	(pSound)
	{
		try
		{
			tS <MMCKINFO>	lRiffChunk;
			tS <MMCKINFO>	lFmtChunk;

			lFmtChunk.ckid = mmioFOURCC('f','m','t',' ');

			if	(
					(LogMmSysErr (pLogLevel, mmioDescend (pSound, &lRiffChunk, NULL, MMIO_FINDRIFF)) == MMSYSERR_NOERROR)
				&&	(LogMmSysErr (pLogLevel, mmioDescend (pSound, &lFmtChunk, &lRiffChunk, MMIO_FINDCHUNK)) == MMSYSERR_NOERROR)
				)
			{
				mmioAscend (pSound, &lFmtChunk, 0);
				mmioAscend (pSound, &lRiffChunk, 0);
				mmioSeek (pSound,	lFmtChunk.dwDataOffset, SEEK_SET);

				if	(lSoundFormat = (LPWAVEFORMATEX) new BYTE [max (lFmtChunk.cksize, sizeof(WAVEFORMATEX))])
				{
					memset (lSoundFormat.Ptr(), 0, sizeof(WAVEFORMATEX));
					mmioRead (pSound, (HPSTR)lSoundFormat.Ptr(), lFmtChunk.cksize);
#ifdef	_DEBUG
					if	((int)lSoundFormat->cbSize != max((int)lFmtChunk.cksize - (int)sizeof(WAVEFORMATEX), 0))
					{
						LogMessage (LogIfActive, _T("--- SoundFormat [%d]->[%d]!=[%d] ---"), (int)lSoundFormat->cbSize, (int)lSoundFormat->cbSize+(int)sizeof(WAVEFORMATEX), lFmtChunk.cksize);
					}
#endif
					lSoundFormat->cbSize = (WORD) max (min ((int)lSoundFormat->cbSize, (int)lFmtChunk.cksize - (int)sizeof(WAVEFORMATEX)), 0);
				}
				if	(pFormatSize)
				{
					(*pFormatSize) = lFmtChunk.cksize;
				}
			}
		}
		catch AnyExceptionDebug
	}
	return lSoundFormat.Detach ();
}

/////////////////////////////////////////////////////////////////////////////

AM_MEDIA_TYPE * CDirectShowUtils::GetSoundMediaType (LPCVOID pSoundData, long pSoundSize, UINT pLogLevel)
{
	tMediaTypePtr		lMediaType;
	long				lSoundFormatSize = 0;
	tPtr <WAVEFORMATEX>	lSoundFormat;

	if	(
			(lSoundFormat = GetSoundFormat (pSoundData, pSoundSize, &lSoundFormatSize, pLogLevel))
		&&	(SUCCEEDED (LogVfwErr (pLogLevel, MoCreateMediaType (lMediaType.Free(), lSoundFormatSize))))
		)
	{
		lMediaType->majortype = MEDIATYPE_Stream;
		lMediaType->subtype = MEDIASUBTYPE_WAVE;
		lMediaType->formattype = FORMAT_WaveFormatEx;
		lMediaType->bFixedSizeSamples = FALSE;
		lMediaType->bTemporalCompression = FALSE;
		lMediaType->lSampleSize = pSoundSize;

		memcpy (lMediaType->pbFormat, lSoundFormat, lSoundFormatSize);
	}
	return lMediaType.Detach ();
}

AM_MEDIA_TYPE * CDirectShowUtils::GetSoundMediaType (LPCTSTR pSoundFileName, UINT pLogLevel)
{
	tMediaTypePtr		lMediaType;
	long				lSoundFormatSize = 0;
	tPtr <WAVEFORMATEX>	lSoundFormat;

	if	(
			(lSoundFormat = GetSoundFormat (pSoundFileName, &lSoundFormatSize, pLogLevel))
		&&	(SUCCEEDED (LogVfwErr (pLogLevel, MoCreateMediaType (lMediaType.Free(), lSoundFormatSize))))
		)
	{
		lMediaType->majortype = MEDIATYPE_Stream;
		lMediaType->subtype = MEDIASUBTYPE_WAVE;
		lMediaType->formattype = FORMAT_WaveFormatEx;
		lMediaType->bFixedSizeSamples = FALSE;
		lMediaType->bTemporalCompression = FALSE;
		lMediaType->lSampleSize = max (lSoundFormat->wBitsPerSample / 8, 1);

		memcpy (lMediaType->pbFormat, lSoundFormat, lSoundFormatSize);
	}
	return lMediaType.Detach ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#include <ks.h>
#include <ksmedia.h>

CString MediaTypeStr (const AM_MEDIA_TYPE & pMediaType)
{
	CString	lTypeStr;

	if	(IsEqualGUID (pMediaType.majortype, MEDIATYPE_Audio))
	{
		lTypeStr += _T("Audio ");
	}
	else
	if	(IsEqualGUID (pMediaType.majortype, MEDIATYPE_Video))
	{
		lTypeStr += _T("Video ");
	}
	else
	if	(IsEqualGUID (pMediaType.majortype, MEDIATYPE_Text))
	{
		lTypeStr += _T("Text ");
	}
	else
	if	(IsEqualGUID (pMediaType.majortype, MEDIATYPE_Midi))
	{
		lTypeStr += _T("Midi ");
	}
	else
	if	(IsEqualGUID (pMediaType.majortype, MEDIATYPE_Stream))
	{
		lTypeStr += _T("Stream ");
	}
	else
	if	(IsEqualGUID (pMediaType.majortype, MEDIATYPE_Interleaved))
	{
		lTypeStr += _T("Interleaved ");
	}
	else
	if	(IsEqualGUID (pMediaType.majortype, MEDIATYPE_File))
	{
		lTypeStr += _T("File ");
	}
	else
	if	(IsEqualGUID (pMediaType.majortype, MEDIATYPE_File))
	{
		lTypeStr += (CString)CGuidStr(pMediaType.majortype);
		lTypeStr += _T(" ");
	}

	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_CLPL))
	{
		lTypeStr += _T("CLPL ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_YUYV))
	{
		lTypeStr += _T("YUYV ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_IYUV))
	{
		lTypeStr += _T("IYUV ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_YVU9))
	{
		lTypeStr += _T("YVU9 ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_Y411))
	{
		lTypeStr += _T("Y411 ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_Y41P))
	{
		lTypeStr += _T("Y41P ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_YUY2))
	{
		lTypeStr += _T("YUY2 ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_YVYU))
	{
		lTypeStr += _T("YVYU ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_UYVY))
	{
		lTypeStr += _T("UYVY ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_Y211))
	{
		lTypeStr += _T("Y211 ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_CLJR))
	{
		lTypeStr += _T("CLJR ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_MJPG))
	{
		lTypeStr += _T("MJPG ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_TVMJ))
	{
		lTypeStr += _T("TVMJ ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_WAKE))
	{
		lTypeStr += _T("WAKE ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_IJPG))
	{
		lTypeStr += _T("IJPG ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_RGB1))
	{
		lTypeStr += _T("RGB1 ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_RGB4))
	{
		lTypeStr += _T("RGB4 ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_RGB8))
	{
		lTypeStr += _T("RGB8 ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_RGB565))
	{
		lTypeStr += _T("RGB565 ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_RGB555))
	{
		lTypeStr += _T("RGB555 ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_RGB24))
	{
		lTypeStr += _T("RGB24 ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_RGB32))
	{
		lTypeStr += _T("RGB32 ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_ARGB1555))
	{
		lTypeStr += _T("ARGB1555 ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_ARGB4444))
	{
		lTypeStr += _T("ARGB4444 ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_ARGB32))
	{
		lTypeStr += _T("ARGB32 ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_A2R10G10B10))
	{
		lTypeStr += _T("A2R10G10B10 ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_A2B10G10R10))
	{
		lTypeStr += _T("A2B10G10R10 ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_PCM))
	{
		lTypeStr += _T("PCM ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, KSDATAFORMAT_SUBTYPE_ADPCM))
	{
		lTypeStr += _T("ADPCM ");
	}
	else
	if	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_WAVE))
	{
		lTypeStr += _T("WAVE ");
	}
	else
	if	(!IsEqualGUID (pMediaType.subtype, GUID_NULL))
	{
		lTypeStr += (CString)CGuidStr(pMediaType.subtype);
		lTypeStr += _T(" ");
	}

	if	(IsEqualGUID (pMediaType.formattype, FORMAT_None))
	{
		lTypeStr += _T("None ");
	}
	else
	if	(IsEqualGUID (pMediaType.formattype, FORMAT_VideoInfo))
	{
		lTypeStr += _T("VideoInfo ");
	}
	else
	if	(IsEqualGUID (pMediaType.formattype, FORMAT_VideoInfo2))
	{
		lTypeStr += _T("VideoInfo2 ");
	}
	else
	if	(IsEqualGUID (pMediaType.formattype, FORMAT_WaveFormatEx))
	{
		lTypeStr += _T("WaveFormatEx ");
	}
	else
	if	(IsEqualGUID (pMediaType.formattype, FORMAT_MPEGVideo))
	{
		lTypeStr += _T("MPEGVideo ");
	}
	else
	if	(IsEqualGUID (pMediaType.formattype, FORMAT_MPEGStreams))
	{
		lTypeStr += _T("MPEGStreams ");
	}
	else
	if	(IsEqualGUID (pMediaType.formattype, FORMAT_DvInfo))
	{
		lTypeStr += _T("DvInfo ");
	}
	else
	if	(IsEqualGUID (pMediaType.formattype, FORMAT_525WSS))
	{
		lTypeStr += _T("525WSS ");
	}
	else
	if	(!IsEqualGUID (pMediaType.formattype, GUID_NULL))
	{
		lTypeStr += (CString)CGuidStr(pMediaType.formattype);
		lTypeStr += _T(" ");
	}

	lTypeStr.TrimRight ();
	return lTypeStr;
}

/////////////////////////////////////////////////////////////////////////////

CString WaveFormatStr (WORD pFormatTag)
{
	CString	lFormatStr;

	lFormatStr.Format (_T("%4.4X "), pFormatTag);

	switch (pFormatTag)
	{
		case WAVE_FORMAT_UNKNOWN					: lFormatStr += _T("UNKNOWN"); break;
		case WAVE_FORMAT_PCM						: lFormatStr += _T("PCM"); break;
		case WAVE_FORMAT_ADPCM						: lFormatStr += _T("ADPCM"); break;
		case WAVE_FORMAT_IEEE_FLOAT             	: lFormatStr += _T("IEEE_FLOAT"); break;
		case WAVE_FORMAT_VSELP                  	: lFormatStr += _T("VSELP"); break;
		case WAVE_FORMAT_IBM_CVSD               	: lFormatStr += _T("IBM_CVSD"); break;
		case WAVE_FORMAT_ALAW                   	: lFormatStr += _T("ALAW"); break;
		case WAVE_FORMAT_MULAW                  	: lFormatStr += _T("MULAW"); break;
		case WAVE_FORMAT_DTS                    	: lFormatStr += _T("DTS"); break;
		case WAVE_FORMAT_DRM                    	: lFormatStr += _T("DRM"); break;
		case WAVE_FORMAT_WMAVOICE9              	: lFormatStr += _T("WMAVOICE9"); break;
		case WAVE_FORMAT_WMAVOICE10             	: lFormatStr += _T("WMAVOICE10"); break;
		case WAVE_FORMAT_OKI_ADPCM              	: lFormatStr += _T("OKI_ADPCM"); break;
		case WAVE_FORMAT_DVI_ADPCM              	: lFormatStr += _T("DVI_ADPCM"); break;
		case WAVE_FORMAT_MEDIASPACE_ADPCM       	: lFormatStr += _T("MEDIASPACE_ADPCM"); break;
		case WAVE_FORMAT_SIERRA_ADPCM           	: lFormatStr += _T("SIERRA_ADPCM"); break;
		case WAVE_FORMAT_G723_ADPCM             	: lFormatStr += _T("G723_ADPCM"); break;
		case WAVE_FORMAT_DIGISTD                	: lFormatStr += _T("DIGISTD"); break;
		case WAVE_FORMAT_DIGIFIX                	: lFormatStr += _T("DIGIFIX"); break;
		case WAVE_FORMAT_DIALOGIC_OKI_ADPCM     	: lFormatStr += _T("DIALOGIC_OKI_ADPCM"); break;
		case WAVE_FORMAT_MEDIAVISION_ADPCM      	: lFormatStr += _T("MEDIAVISION_ADPCM"); break;
		case WAVE_FORMAT_CU_CODEC               	: lFormatStr += _T("CU_CODEC"); break;
		case WAVE_FORMAT_YAMAHA_ADPCM           	: lFormatStr += _T("YAMAHA_ADPCM"); break;
		case WAVE_FORMAT_SONARC                 	: lFormatStr += _T("SONARC"); break;
		case WAVE_FORMAT_DSPGROUP_TRUESPEECH    	: lFormatStr += _T("DSPGROUP_TRUESPEECH"); break;
		case WAVE_FORMAT_ECHOSC1                	: lFormatStr += _T("ECHOSC1"); break;
		case WAVE_FORMAT_AUDIOFILE_AF36         	: lFormatStr += _T("AUDIOFILE_AF36"); break;
		case WAVE_FORMAT_APTX                   	: lFormatStr += _T("APTX"); break;
		case WAVE_FORMAT_AUDIOFILE_AF10         	: lFormatStr += _T("AUDIOFILE_AF10"); break;
		case WAVE_FORMAT_PROSODY_1612           	: lFormatStr += _T("PROSODY_1612"); break;
		case WAVE_FORMAT_LRC                    	: lFormatStr += _T("LRC"); break;
		case WAVE_FORMAT_DOLBY_AC2              	: lFormatStr += _T("DOLBY_AC2"); break;
		case WAVE_FORMAT_GSM610                 	: lFormatStr += _T("GSM610"); break;
		case WAVE_FORMAT_MSNAUDIO               	: lFormatStr += _T("MSNAUDIO"); break;
		case WAVE_FORMAT_ANTEX_ADPCME           	: lFormatStr += _T("ANTEX_ADPCME"); break;
		case WAVE_FORMAT_CONTROL_RES_VQLPC      	: lFormatStr += _T("CONTROL_RES_VQLPC"); break;
		case WAVE_FORMAT_DIGIREAL               	: lFormatStr += _T("DIGIREAL"); break;
		case WAVE_FORMAT_DIGIADPCM              	: lFormatStr += _T("DIGIADPCM"); break;
		case WAVE_FORMAT_CONTROL_RES_CR10       	: lFormatStr += _T("CONTROL_RES_CR10"); break;
		case WAVE_FORMAT_NMS_VBXADPCM           	: lFormatStr += _T("NMS_VBXADPCM"); break;
		case WAVE_FORMAT_CS_IMAADPCM            	: lFormatStr += _T("CS_IMAADPCM"); break;
		case WAVE_FORMAT_ECHOSC3                	: lFormatStr += _T("ECHOSC3"); break;
		case WAVE_FORMAT_ROCKWELL_ADPCM         	: lFormatStr += _T("ROCKWELL_ADPCM"); break;
		case WAVE_FORMAT_ROCKWELL_DIGITALK      	: lFormatStr += _T("ROCKWELL_DIGITALK"); break;
		case WAVE_FORMAT_XEBEC                  	: lFormatStr += _T("XEBEC"); break;
		case WAVE_FORMAT_G721_ADPCM             	: lFormatStr += _T("G721_ADPCM"); break;
		case WAVE_FORMAT_G728_CELP              	: lFormatStr += _T("G728_CELP"); break;
		case WAVE_FORMAT_MSG723                 	: lFormatStr += _T("MSG723"); break;
		case WAVE_FORMAT_MPEG                   	: lFormatStr += _T("MPEG"); break;
		case WAVE_FORMAT_RT24                   	: lFormatStr += _T("RT24"); break;
		case WAVE_FORMAT_PAC                    	: lFormatStr += _T("PAC"); break;
		case WAVE_FORMAT_MPEGLAYER3             	: lFormatStr += _T("MPEGLAYER3"); break;
		case WAVE_FORMAT_LUCENT_G723            	: lFormatStr += _T("LUCENT_G723"); break;
		case WAVE_FORMAT_CIRRUS                 	: lFormatStr += _T("CIRRUS"); break;
		case WAVE_FORMAT_ESPCM                  	: lFormatStr += _T("ESPCM"); break;
		case WAVE_FORMAT_VOXWARE                	: lFormatStr += _T("VOXWARE"); break;
		case WAVE_FORMAT_CANOPUS_ATRAC          	: lFormatStr += _T("CANOPUS_ATRAC"); break;
		case WAVE_FORMAT_G726_ADPCM             	: lFormatStr += _T("G726_ADPCM"); break;
		case WAVE_FORMAT_G722_ADPCM             	: lFormatStr += _T("G722_ADPCM"); break;
		case WAVE_FORMAT_DSAT_DISPLAY           	: lFormatStr += _T("DSAT_DISPLAY"); break;
		case WAVE_FORMAT_VOXWARE_BYTE_ALIGNED   	: lFormatStr += _T("VOXWARE_BYTE_ALIGNED"); break;
		case WAVE_FORMAT_VOXWARE_AC8            	: lFormatStr += _T("VOXWARE_AC8"); break;
		case WAVE_FORMAT_VOXWARE_AC10           	: lFormatStr += _T("VOXWARE_AC10"); break;
		case WAVE_FORMAT_VOXWARE_AC16           	: lFormatStr += _T("VOXWARE_AC16"); break;
		case WAVE_FORMAT_VOXWARE_AC20           	: lFormatStr += _T("VOXWARE_AC20"); break;
		case WAVE_FORMAT_VOXWARE_RT24           	: lFormatStr += _T("VOXWARE_RT24"); break;
		case WAVE_FORMAT_VOXWARE_RT29           	: lFormatStr += _T("VOXWARE_RT29"); break;
		case WAVE_FORMAT_VOXWARE_RT29HW         	: lFormatStr += _T("VOXWARE_RT29HW"); break;
		case WAVE_FORMAT_VOXWARE_VR12           	: lFormatStr += _T("VOXWARE_VR12"); break;
		case WAVE_FORMAT_VOXWARE_VR18           	: lFormatStr += _T("VOXWARE_VR18"); break;
		case WAVE_FORMAT_VOXWARE_TQ40           	: lFormatStr += _T("VOXWARE_TQ40"); break;
		case WAVE_FORMAT_SOFTSOUND              	: lFormatStr += _T("SOFTSOUND"); break;
		case WAVE_FORMAT_VOXWARE_TQ60           	: lFormatStr += _T("VOXWARE_TQ60"); break;
		case WAVE_FORMAT_MSRT24                 	: lFormatStr += _T("MSRT24"); break;
		case WAVE_FORMAT_G729A                  	: lFormatStr += _T("G729A"); break;
		case WAVE_FORMAT_MVI_MVI2               	: lFormatStr += _T("MVI_MVI2"); break;
		case WAVE_FORMAT_DF_G726                	: lFormatStr += _T("DF_G726"); break;
		case WAVE_FORMAT_DF_GSM610              	: lFormatStr += _T("DF_GSM610"); break;
		case WAVE_FORMAT_ISIAUDIO               	: lFormatStr += _T("ISIAUDIO"); break;
		case WAVE_FORMAT_ONLIVE                 	: lFormatStr += _T("ONLIVE"); break;
		case WAVE_FORMAT_SBC24                  	: lFormatStr += _T("SBC24"); break;
		case WAVE_FORMAT_DOLBY_AC3_SPDIF        	: lFormatStr += _T("DOLBY_AC3_SPDIF"); break;
		case WAVE_FORMAT_MEDIASONIC_G723        	: lFormatStr += _T("MEDIASONIC_G723"); break;
		case WAVE_FORMAT_PROSODY_8KBPS          	: lFormatStr += _T("PROSODY_8KBPS"); break;
		case WAVE_FORMAT_ZYXEL_ADPCM            	: lFormatStr += _T("ZYXEL_ADPCM"); break;
		case WAVE_FORMAT_PHILIPS_LPCBB          	: lFormatStr += _T("PHILIPS_LPCBB"); break;
		case WAVE_FORMAT_PACKED                 	: lFormatStr += _T("PACKED"); break;
		case WAVE_FORMAT_MALDEN_PHONYTALK       	: lFormatStr += _T("MALDEN_PHONYTALK"); break;
		case WAVE_FORMAT_RHETOREX_ADPCM         	: lFormatStr += _T("RHETOREX_ADPCM"); break;
		case WAVE_FORMAT_IRAT                   	: lFormatStr += _T("IRAT"); break;
		case WAVE_FORMAT_VIVO_G723              	: lFormatStr += _T("VIVO_G723"); break;
		case WAVE_FORMAT_VIVO_SIREN             	: lFormatStr += _T("VIVO_SIREN"); break;
		case WAVE_FORMAT_DIGITAL_G723           	: lFormatStr += _T("DIGITAL_G723"); break;
		case WAVE_FORMAT_SANYO_LD_ADPCM         	: lFormatStr += _T("SANYO_LD_ADPCM"); break;
		case WAVE_FORMAT_SIPROLAB_ACEPLNET      	: lFormatStr += _T("SIPROLAB_ACEPLNET"); break;
		case WAVE_FORMAT_SIPROLAB_ACELP4800     	: lFormatStr += _T("SIPROLAB_ACELP4800"); break;
		case WAVE_FORMAT_SIPROLAB_ACELP8V3      	: lFormatStr += _T("SIPROLAB_ACELP8V3"); break;
		case WAVE_FORMAT_SIPROLAB_G729          	: lFormatStr += _T("SIPROLAB_G729"); break;
		case WAVE_FORMAT_SIPROLAB_G729A         	: lFormatStr += _T("SIPROLAB_G729A"); break;
		case WAVE_FORMAT_SIPROLAB_KELVIN        	: lFormatStr += _T("SIPROLAB_KELVIN"); break;
		case WAVE_FORMAT_G726ADPCM              	: lFormatStr += _T("G726ADPCM"); break;
		case WAVE_FORMAT_QUALCOMM_PUREVOICE     	: lFormatStr += _T("QUALCOMM_PUREVOICE"); break;
		case WAVE_FORMAT_QUALCOMM_HALFRATE      	: lFormatStr += _T("QUALCOMM_HALFRATE"); break;
		case WAVE_FORMAT_TUBGSM                 	: lFormatStr += _T("TUBGSM"); break;
		case WAVE_FORMAT_MSAUDIO1               	: lFormatStr += _T("MSAUDIO1"); break;
		case WAVE_FORMAT_WMAUDIO2               	: lFormatStr += _T("WMAUDIO2"); break;
		case WAVE_FORMAT_WMAUDIO3               	: lFormatStr += _T("WMAUDIO3"); break;
		case WAVE_FORMAT_WMAUDIO_LOSSLESS       	: lFormatStr += _T("WMAUDIO_LOSSLESS"); break;
		case WAVE_FORMAT_WMASPDIF               	: lFormatStr += _T("WMASPDIF"); break;
		case WAVE_FORMAT_UNISYS_NAP_ADPCM       	: lFormatStr += _T("UNISYS_NAP_ADPCM"); break;
		case WAVE_FORMAT_UNISYS_NAP_ULAW        	: lFormatStr += _T("UNISYS_NAP_ULAW"); break;
		case WAVE_FORMAT_UNISYS_NAP_ALAW        	: lFormatStr += _T("UNISYS_NAP_ALAW"); break;
		case WAVE_FORMAT_UNISYS_NAP_16K         	: lFormatStr += _T("UNISYS_NAP_16K"); break;
		case WAVE_FORMAT_CREATIVE_ADPCM         	: lFormatStr += _T("CREATIVE_ADPCM"); break;
		case WAVE_FORMAT_CREATIVE_FASTSPEECH8   	: lFormatStr += _T("CREATIVE_FASTSPEECH8"); break;
		case WAVE_FORMAT_CREATIVE_FASTSPEECH10  	: lFormatStr += _T("CREATIVE_FASTSPEECH10"); break;
		case WAVE_FORMAT_UHER_ADPCM             	: lFormatStr += _T("UHER_ADPCM"); break;
		case WAVE_FORMAT_QUARTERDECK            	: lFormatStr += _T("QUARTERDECK"); break;
		case WAVE_FORMAT_ILINK_VC               	: lFormatStr += _T("ILINK_VC"); break;
		case WAVE_FORMAT_RAW_SPORT              	: lFormatStr += _T("RAW_SPORT"); break;
		case WAVE_FORMAT_ESST_AC3               	: lFormatStr += _T("ESST_AC3"); break;
		case WAVE_FORMAT_GENERIC_PASSTHRU       	: lFormatStr += _T("GENERIC_PASSTHRU"); break;
		case WAVE_FORMAT_IPI_HSX                	: lFormatStr += _T("IPI_HSX"); break;
		case WAVE_FORMAT_IPI_RPELP              	: lFormatStr += _T("IPI_RPELP"); break;
		case WAVE_FORMAT_CS2                    	: lFormatStr += _T("CS2"); break;
		case WAVE_FORMAT_SONY_SCX               	: lFormatStr += _T("SONY_SCX"); break;
		case WAVE_FORMAT_FM_TOWNS_SND           	: lFormatStr += _T("FM_TOWNS_SND"); break;
		case WAVE_FORMAT_BTV_DIGITAL            	: lFormatStr += _T("BTV_DIGITAL"); break;
		case WAVE_FORMAT_QDESIGN_MUSIC          	: lFormatStr += _T("QDESIGN_MUSIC"); break;
		case WAVE_FORMAT_VME_VMPCM              	: lFormatStr += _T("VME_VMPCM"); break;
		case WAVE_FORMAT_TPC                    	: lFormatStr += _T("TPC"); break;
		case WAVE_FORMAT_OLIGSM                 	: lFormatStr += _T("OLIGSM"); break;
		case WAVE_FORMAT_OLIADPCM               	: lFormatStr += _T("OLIADPCM"); break;
		case WAVE_FORMAT_OLICELP                	: lFormatStr += _T("OLICELP"); break;
		case WAVE_FORMAT_OLISBC                 	: lFormatStr += _T("OLISBC"); break;
		case WAVE_FORMAT_OLIOPR                 	: lFormatStr += _T("OLIOPR"); break;
		case WAVE_FORMAT_LH_CODEC               	: lFormatStr += _T("LH_CODEC"); break;
		case WAVE_FORMAT_NORRIS                 	: lFormatStr += _T("NORRIS"); break;
		case WAVE_FORMAT_SOUNDSPACE_MUSICOMPRESS	: lFormatStr += _T("SOUNDSPACE_MUSICOMPRESS"); break;
		case WAVE_FORMAT_MPEG_ADTS_AAC          	: lFormatStr += _T("MPEG_ADTS_AAC"); break;
		case WAVE_FORMAT_MPEG_RAW_AAC           	: lFormatStr += _T("MPEG_RAW_AAC"); break;
		case WAVE_FORMAT_NOKIA_MPEG_ADTS_AAC    	: lFormatStr += _T("NOKIA_MPEG_ADTS_AAC"); break;
		case WAVE_FORMAT_NOKIA_MPEG_RAW_AAC     	: lFormatStr += _T("NOKIA_MPEG_RAW_AAC"); break;
		case WAVE_FORMAT_VODAFONE_MPEG_ADTS_AAC 	: lFormatStr += _T("VODAFONE_MPEG_ADTS_AAC"); break;
		case WAVE_FORMAT_VODAFONE_MPEG_RAW_AAC  	: lFormatStr += _T("VODAFONE_MPEG_RAW_AAC"); break;
		case WAVE_FORMAT_DVM                    	: lFormatStr += _T("DVM"); break;
#ifdef	WAVE_FORMAT_EXTENSIBLE
		case WAVE_FORMAT_EXTENSIBLE             	: lFormatStr += _T("EXTENSIBLE"); break;
#endif
#ifdef	WAVE_FORMAT_DEVELOPMENT
		case WAVE_FORMAT_DEVELOPMENT            	: lFormatStr += _T("DEVELOPMENT"); break;
#endif
	}
	lFormatStr.TrimLeft ();
	return lFormatStr;
}

/////////////////////////////////////////////////////////////////////////////

CString FilterStateStr (OAFilterState pFilterState, bool pStatePending)
{
	CString	lStateStr;

	switch (pFilterState)
	{
		case State_Stopped:	lStateStr = pStatePending ? _T("Stopping") : _T("Stopped"); break;
		case State_Running:	lStateStr = pStatePending ? _T("Starting") : _T("Running"); break;
		case State_Paused:	lStateStr = pStatePending ? _T("Pausing") : _T("Paused"); break;
		default:			lStateStr.Format (_T("%u"), pFilterState);
	}

	return lStateStr;
}

/////////////////////////////////////////////////////////////////////////////

CString SeekingFlagsStr (DWORD pSeekingFlags)
{
	CString	lFlagsStr;

	if	((pSeekingFlags & AM_SEEKING_PositioningBitsMask) == AM_SEEKING_AbsolutePositioning)
	{
		lFlagsStr += _T("AbsolutePositioning ");
	}
	else
	if	((pSeekingFlags & AM_SEEKING_PositioningBitsMask) == AM_SEEKING_RelativePositioning)
	{
		lFlagsStr += _T("RelativePositioning ");
	}
	else
	if	((pSeekingFlags & AM_SEEKING_PositioningBitsMask) == AM_SEEKING_IncrementalPositioning)
	{
		lFlagsStr += _T("IncrementalPositioning ");
	}
	else
	{
		lFlagsStr += _T("NoPositioning ");
	}

	if	(pSeekingFlags & AM_SEEKING_SeekToKeyFrame)
	{
		lFlagsStr += _T("SeekToKeyFrame ");
	}
	if	(pSeekingFlags & AM_SEEKING_ReturnTime)
	{
		lFlagsStr += _T("ReturnTime ");
	}
	if	(pSeekingFlags & AM_SEEKING_Segment)
	{
		lFlagsStr += _T("Segment ");
	}
	if	(pSeekingFlags & AM_SEEKING_NoFlush)
	{
		lFlagsStr += _T("NoFlush ");
	}

	lFlagsStr.TrimRight ();
	return lFlagsStr;
}

/////////////////////////////////////////////////////////////////////////////

CString SeekingCapsStr (DWORD pSeekingCaps)
{
	CString	lCapsStr;

	if	(pSeekingCaps & AM_SEEKING_CanSeekAbsolute)
	{
		lCapsStr += _T("CanSeekAbsolute ");
	}
	if	(pSeekingCaps & AM_SEEKING_CanSeekForwards)
	{
		lCapsStr += _T("CanSeekForwards ");
	}
	if	(pSeekingCaps & AM_SEEKING_CanSeekBackwards)
	{
		lCapsStr += _T("CanSeekBackwards ");
	}
	if	(pSeekingCaps & AM_SEEKING_CanGetCurrentPos)
	{
		lCapsStr += _T("CanGetCurrentPos ");
	}
	if	(pSeekingCaps & AM_SEEKING_CanGetStopPos)
	{
		lCapsStr += _T("CanGetStopPos ");
	}
	if	(pSeekingCaps & AM_SEEKING_CanGetDuration)
	{
		lCapsStr += _T("CanGetDuration ");
	}
	if	(pSeekingCaps & AM_SEEKING_CanPlayBackwards)
	{
		lCapsStr += _T("CanPlayBackwards ");
	}
	if	(pSeekingCaps & AM_SEEKING_CanDoSegments)
	{
		lCapsStr += _T("CanDoSegments ");
	}

	lCapsStr.TrimRight ();
	return lCapsStr;
}

/////////////////////////////////////////////////////////////////////////////

CString SampleIdStr (DWORD pSampleId)
{
	CString	lIdStr;

	if	(pSampleId == AM_STREAM_MEDIA)
	{
		lIdStr = _T("Media");
	}
	else
	if	(pSampleId == AM_STREAM_CONTROL)
	{
		lIdStr = _T("Control");
	}
	else
	{
		lIdStr.Format (_T("%u"), pSampleId);
	}
	return lIdStr;
}

CString SampleFlagsStr (DWORD pSampleFlags)
{
	CString	lFlagsStr;

	lFlagsStr.Format (_T("%8.8X "), pSampleFlags);

	if	(pSampleFlags & AM_SAMPLE_SPLICEPOINT)
	{
		lFlagsStr += _T("SPLICEPOINT ");
	}
	if	(pSampleFlags & AM_SAMPLE_PREROLL)
	{
		lFlagsStr += _T("PREROLL ");
	}
	if	(pSampleFlags & AM_SAMPLE_DATADISCONTINUITY)
	{
		lFlagsStr += _T("DATADISCONTINUITY ");
	}
	if	(pSampleFlags & AM_SAMPLE_TYPECHANGED)
	{
		lFlagsStr += _T("TYPECHANGED ");
	}
	if	(pSampleFlags & AM_SAMPLE_FLUSH_ON_PAUSE)
	{
		lFlagsStr += _T("FLUSH_ON_PAUSE ");
	}
	if	(pSampleFlags & AM_SAMPLE_ENDOFSTREAM)
	{
		lFlagsStr += _T("ENDOFSTREAM ");
	}
	if	(pSampleFlags & AM_SAMPLE_TIMEVALID)
	{
		lFlagsStr += _T("TIMEVALID ");
	}
	if	(pSampleFlags & AM_SAMPLE_STOPVALID)
	{
		lFlagsStr += _T("STOPVALID ");
	}

	lFlagsStr.TrimRight ();
	return lFlagsStr;
}

/////////////////////////////////////////////////////////////////////////////

CString PinIdStr (IPin * pPin, bool pIncludeFilter)
{
	CString	lRet;

	if	(pPin)
	{
#ifdef	_DEBUG
		try
		{
			tMallocPtr <WCHAR>	lPinId;
			PIN_INFO_Safe		lPinInfo;

			pPin->QueryId (lPinId.Free ());
			pPin->QueryPinInfo (&lPinInfo);

			if	(CString ((LPWSTR)lPinId) == CString (lPinInfo.achName))
			{
				lRet.Format (_T("%s:%ls"), ((lPinInfo.dir==PINDIR_INPUT)?_T("INPUT"):(lPinInfo.dir==PINDIR_OUTPUT)?_T("OUTPUT"):_T("<error>")), lPinInfo.achName);
			}
			else
			{
				lRet.Format (_T("%s:%ls (%ls)"), ((lPinInfo.dir==PINDIR_INPUT)?_T("INPUT"):(lPinInfo.dir==PINDIR_OUTPUT)?_T("OUTPUT"):_T("<error>")), (LPWSTR)lPinId, lPinInfo.achName);
			}
			lRet.TrimLeft ();
			lRet.TrimRight ();

			if	(
					(pIncludeFilter)
				&&	(lPinInfo.pFilter)
				)
			{
				try
				{
					FILTER_INFO_Safe	lFilterInfo;

					if	(SUCCEEDED (lPinInfo.pFilter->QueryFilterInfo (&lFilterInfo)))
					{
						lRet.Format (_T("%ls:%s"), lFilterInfo.achName, CString((LPCTSTR)lRet));
					}
				}
				catch AnyExceptionSilent
			}
		}
		catch AnyExceptionSilent
#else
		lRet.Format (_T("%p"), pPin);
#endif
	}
	else
	{
		lRet = _T("<no pin>");
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#define InitLogTitle(pFormat, pTitle, pIndent, pDefaultTitle) \
if	(pFormat) \
{ \
	va_list	lArgPtr; \
	va_start (lArgPtr, pFormat); \
	_vsntprintf (pTitle.GetBuffer(2048), 2048, pFormat, lArgPtr); \
	pTitle.ReleaseBuffer (); \
	pIndent = pTitle; \
	pTitle.TrimLeft (); \
	pIndent = CString (_T(' '), pIndent.GetLength()-pTitle.GetLength()); \
	if	(pTitle.IsEmpty()) \
	{ \
		pTitle = _T(" "); \
	} \
} \
else \
{ \
	pTitle = pDefaultTitle; \
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void LogFilters (UINT pLogLevel, IFilterGraph * pFilterGraph, bool pEnumPinTypes, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString				lTitle;
			CString				lIndent;
			IEnumFiltersPtr		lEnumFilters;
			IBaseFilterPtr		lFilter;

			InitLogTitle (pFormat, lTitle, lIndent, _T("Filters"));
			lTitle.TrimLeft ();

			if	(
					(pFilterGraph != NULL)
				&&	(SUCCEEDED (LogVfwErr (pLogLevel, pFilterGraph->EnumFilters (&lEnumFilters))))
				&&	(lEnumFilters != NULL)
				)
			{
				if	(!lTitle.IsEmpty())
				{
					LogMessage (pLogLevel, _T("%s%s"), lIndent, lTitle);
				}

				while (lEnumFilters->Next (1, &lFilter, NULL) == S_OK)
				{
					LogFilter (pLogLevel, lFilter, pEnumPinTypes, lIndent + _T("  Filter"));
				}
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

void LogFilter (UINT pLogLevel, IBaseFilter * pFilter, bool pEnumPinTypes, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString				lTitle;
			CString				lIndent;
			GUID				lFilterClass = GUID_NULL;
			FILTER_INFO_Safe	lFilterInfo;
			tMallocPtr <WCHAR>	lVendorName;

			InitLogTitle (pFormat, lTitle, lIndent, _T("Filter"));

			pFilter->GetClassID (&lFilterClass);
			pFilter->QueryFilterInfo (&lFilterInfo);
			pFilter->QueryVendorInfo (lVendorName.Free ());

			LogMessage (pLogLevel, _T("%s%s [%ls] Vendor [%ls] Class [%s]"), lIndent, lTitle, lFilterInfo.achName, (LPWSTR) lVendorName, CGuidStr::GuidName(lFilterClass));
			LogFilterPins (pLogLevel, pFilter, pEnumPinTypes, lIndent);
		}
		catch AnyExceptionDebug
	}
#endif
}

void LogFilterPins (UINT pLogLevel, IBaseFilter * pFilter, bool pEnumTypes, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString			lTitle;
			CString			lIndent;
			IEnumPinsPtr	lEnumPins;
			IPinPtr			lPin;

			InitLogTitle (pFormat, lTitle, lIndent, _T("Pins"));
			lTitle.TrimLeft ();

			if	(SUCCEEDED (pFilter->EnumPins (&lEnumPins)))
			{
				if	(!lTitle.IsEmpty())
				{
					LogMessage (pLogLevel, _T("%s%s"), lIndent, lTitle);
				}

				while (lEnumPins->Next (1, &lPin, NULL) == S_OK)
				{
					LogFilterPin (pLogLevel, lPin, pEnumTypes, lIndent + _T("  Pin"));
				}
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

void LogFilterPin (UINT pLogLevel, IPin * pPin, bool pEnumTypes, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString				lTitle;
			CString				lIndent;
			IPinPtr				lConnection;
			AM_MEDIA_TYPE_Safe	lPinMediaType;
			IMemInputPinPtr		lTranport (pPin);

			InitLogTitle (pFormat, lTitle, lIndent, _T("Pin"));

			LogMessage (pLogLevel, _T("%s%s [%s]"), lIndent, lTitle, PinIdStr (pPin ,false));

			if	(SUCCEEDED (pPin->ConnectedTo (&lConnection)))
			{
				LogMessage (pLogLevel, _T("%s  Connected [%s]"), lIndent, PinIdStr (lConnection));
			}

			if	(lTranport != NULL)
			{
				LogMemAllocator (pLogLevel, lTranport, _T("%s  Allocator"), lIndent);
			}

			if	(SUCCEEDED (pPin->ConnectionMediaType (&lPinMediaType)))
			{
				LogMediaType (pLogLevel, lPinMediaType, _T("%s  Type"), lIndent);
			}

			if	(pEnumTypes)
			{
				IEnumMediaTypesPtr	lEnumMediaTypes;
				tMediaTypePtr		lMediaType = NULL;

				if	(SUCCEEDED (pPin->EnumMediaTypes (&lEnumMediaTypes)))
				{
					LogMessage (pLogLevel, _T("%s  Types"), lIndent);

					while (lEnumMediaTypes->Next (1, lMediaType.Free(), NULL) == S_OK)
					{
						LogMediaType (pLogLevel, *lMediaType, _T("%s  "), lIndent);
					}
				}
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void LogFilterStates (UINT pLogLevel, IFilterGraph * pFilterGraph, bool pEnumPins, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString				lTitle;
			CString				lIndent;
			IEnumFiltersPtr		lEnumFilters;
			IBaseFilterPtr		lFilter;

			InitLogTitle (pFormat, lTitle, lIndent, _T("Filter States"));
			lTitle.TrimLeft ();

			if	(
					(pFilterGraph != NULL)
				&&	(SUCCEEDED (pFilterGraph->EnumFilters (&lEnumFilters)))
				&&	(lEnumFilters != NULL)
				)
			{
				if	(!lTitle.IsEmpty())
				{
					LogMessage (pLogLevel, _T("%s%s"), lIndent, lTitle);
				}
				while (lEnumFilters->Next (1, &lFilter, NULL) == S_OK)
				{
					LogFilterState (pLogLevel, lFilter, pEnumPins, lIndent + _T("  Filter"));
				}
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

void LogFilterState (UINT pLogLevel, IBaseFilter * pFilter, bool pEnumPins, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString				lTitle;
			CString				lIndent;
			HRESULT				lResult;
			FILTER_INFO_Safe	lFilterInfo;
			FILTER_STATE		lFilterState;
			IMediaSeekingPtr	lMediaSeeking (pFilter);

			InitLogTitle (pFormat, lTitle, lIndent, _T("Filter State"));

			LogVfwErr (pLogLevel, pFilter->QueryFilterInfo (&lFilterInfo));
			LogVfwErr (pLogLevel, lResult = pFilter->GetState (10, &lFilterState));

			if	(
					(lMediaSeeking != NULL)
				||	(pEnumPins)
				)
			{
				LogMessage (pLogLevel, _T("%s%s [%ls]"), lIndent, lTitle, lFilterInfo.achName);
				if	(SUCCEEDED (lResult))
				{
					LogMessage (pLogLevel, _T("%s  %s"), lIndent, FilterStateStr (lFilterState, (lResult == VFW_S_STATE_INTERMEDIATE)));
				}
			}
			else
			{
				LogMessage (pLogLevel, _T("%s%s [%ls] <%s>"), lIndent, lTitle, lFilterInfo.achName, FilterStateStr (lFilterState, (lResult == VFW_S_STATE_INTERMEDIATE)));
			}

			if	(lMediaSeeking != NULL)
			{
				LogMediaSeeking (pLogLevel, lMediaSeeking, lIndent + _T("  "));
			}

			if	(pEnumPins)
			{
				IEnumPinsPtr	lEnumPins;
				IPinPtr			lPin;

				if	(SUCCEEDED (pFilter->EnumPins (&lEnumPins)))
				{
					while (lEnumPins->Next (1, &lPin, NULL) == S_OK)
					{
						LogFilterPinState (pLogLevel, lPin, lIndent+_T("  Pin"));
					}
				}
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

void LogFilterPinState (UINT pLogLevel, IPin * pPin, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString				lTitle;
			CString				lIndent;
			IPinPtr				lConnection;
			AM_MEDIA_TYPE_Safe	lPinMediaType;
			IMemInputPinPtr		lTranport (pPin);
			IMediaSeekingPtr	lMediaSeeking (pPin);

			InitLogTitle (pFormat, lTitle, lIndent, _T("Pin State"));

			LogMessage (pLogLevel, _T("%s%s [%s]"), lIndent, lTitle, PinIdStr (pPin, false));

			if	(SUCCEEDED (pPin->ConnectedTo (&lConnection)))
			{
				LogMessage (pLogLevel, _T("%s  Connected [%s]"), lIndent, PinIdStr (lConnection));
			}

			if	(lTranport != NULL)
			{
				LogMemAllocator (pLogLevel, lTranport, _T("%s  Allocator"), lIndent);
			}

			if	(SUCCEEDED (pPin->ConnectionMediaType (&lPinMediaType)))
			{
				LogMediaType (pLogLevel, lPinMediaType, _T("%s  Type"), lIndent);
			}

			if	(lMediaSeeking != NULL)
			{
				LogMediaSeeking (pLogLevel, lMediaSeeking, lIndent + _T("  "));
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void LogFilterCache (UINT pLogLevel, IFilterGraph * pFilterGraph, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString			lTitle;
			CString			lIndent;
			IGraphConfigPtr	lGraphConfig (pFilterGraph);
			IEnumFiltersPtr	lEnumFilters;
			IBaseFilterPtr	lFilter;

			InitLogTitle (pFormat, lTitle, lIndent, _T("Filter Cache"));
			lTitle.TrimLeft ();

			if	(
					(lGraphConfig != NULL)
				&&	(SUCCEEDED (LogVfwErr (pLogLevel, lGraphConfig->EnumCacheFilter (&lEnumFilters))))
				&&	(lEnumFilters != NULL)
				)
			{
				if	(!lTitle.IsEmpty())
				{
					LogMessage (pLogLevel, _T("%s%s"), lIndent, lTitle);
				}
				while (lEnumFilters->Next (1, &lFilter, NULL) == S_OK)
				{
					LogFilter (pLogLevel, lFilter, false, lIndent + _T("  Filter"));
				}
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void LogMediaSeeking (UINT pLogLevel, IMediaSeeking * pMediaSeeking, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString		lTitle;
			CString		lIndent;
			LONGLONG	lDuration = -1;
			LONGLONG	lCurrPos = -1;
			LONGLONG	lStopPos = -1;
			LONGLONG	lEarliest = -1;
			LONGLONG	lLatest = -1;
			DWORD		lCapFlags = 0;
			GUID		lTimeFormat = GUID_NULL;

			InitLogTitle (pFormat, lTitle, lIndent, _T("Media Seeking"));

			lTitle.TrimRight ();
			if	(!lTitle.IsEmpty ())
			{
				LogMessage (pLogLevel, _T("%s%s [%p]"), lIndent, lTitle, pMediaSeeking);
			}

			if	(pMediaSeeking != NULL)
			{
				pMediaSeeking->GetDuration (&lDuration);
				pMediaSeeking->GetPositions (&lCurrPos, &lStopPos);
				pMediaSeeking->GetTimeFormat (&lTimeFormat);

				LogMessage (pLogLevel, _T("%s  Time Format   [%s]"), lIndent, CGuidStr::GuidName(lTimeFormat));
				LogMessage (pLogLevel, _T("%s  Duration      [%f (%I64d)] Current [%f (%I64d)] Stop [%f (%I64d)]"), lIndent, RefTimeSec(lDuration), lDuration, RefTimeSec(lCurrPos), lCurrPos, RefTimeSec(lStopPos), lStopPos);
				if	(SUCCEEDED (pMediaSeeking->GetAvailable (&lEarliest, &lLatest)))
				{
					LogMessage (pLogLevel, _T("%s  Earliest      [%f (%I64d)] Latest [%f (%I64d)]"), lIndent, RefTimeSec(lEarliest), lEarliest, RefTimeSec(lLatest), lLatest);
				}
				if	(SUCCEEDED (pMediaSeeking->GetCapabilities (&lCapFlags)))
				{
					LogMessage (pLogLevel, _T("%s  Seeking Caps  [%8.8X] [%s]"), lIndent, lCapFlags, SeekingCapsStr(lCapFlags));
				}
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

void LogMediaSeekingPos (UINT pLogLevel, IMediaSeeking * pMediaSeeking, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString		lTitle;
			CString		lIndent;
			LONGLONG	lDuration = -1;
			LONGLONG	lCurrPos = -1;
			LONGLONG	lStopPos = -1;

			InitLogTitle (pFormat, lTitle, lIndent, _T("Media Seeking"));

			if	(pMediaSeeking != NULL)
			{
				pMediaSeeking->GetDuration (&lDuration);
				pMediaSeeking->GetPositions (&lCurrPos, &lStopPos);

				LogMessage (pLogLevel, _T("%s%s [%f - %f] of [%f]"), lTitle, lIndent, RefTimeSec(lCurrPos), RefTimeSec(lStopPos), RefTimeSec(lDuration));
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void LogPinAllocator (UINT pLogLevel, IPin * pPin, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString			lTitle;
			CString			lIndent;
			IMemInputPinPtr	lTranport (pPin);

			InitLogTitle (pFormat, lTitle, lIndent, _T("Pin Allocator"));

			if	(lTranport != NULL)
			{
				lTitle.TrimLeft ();
				LogMemAllocator (pLogLevel, lTranport, lIndent+lTitle);
			}
			else
			{
				LogMessage (pLogLevel, _T("%s%s <not MemInput transport> [%p]"), lIndent, lTitle, pPin);
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

void LogMemAllocator (UINT pLogLevel, IMemInputPin * pInputPin, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString						lTitle;
			CString						lIndent;
			IMemAllocatorPtr			lAllocator;
			tS <ALLOCATOR_PROPERTIES>	lRequirements;

			InitLogTitle (pFormat, lTitle, lIndent, _T("Pin Allocator"));

			if	(
					(pInputPin)
				&&	(SUCCEEDED (LogVfwErr (pLogLevel, pInputPin->GetAllocator (&lAllocator))))
				&&	(lAllocator != NULL)
				)
			{
				lTitle.TrimLeft ();
				LogMemAllocator (pLogLevel, lAllocator, lIndent+lTitle);
			}
			else
			{
				LogMessage (pLogLevel, _T("%s%s <no allocator> [%p]"), lIndent, lTitle, pInputPin);
			}
			if	(
					(pInputPin)
				&&	(SUCCEEDED (pInputPin->GetAllocatorRequirements (lRequirements.Clear())))
				)
			{
				lTitle.TrimLeft ();
				LogAllocatorProps (pLogLevel, lRequirements, lIndent+lTitle+_T(" (Required)"));
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

void LogMemAllocator (UINT pLogLevel, IMemAllocator * pAllocator, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString						lTitle;
			CString						lIndent;
			tS <ALLOCATOR_PROPERTIES>	lAllocatorProps;

			InitLogTitle (pFormat, lTitle, lIndent, _T("Allocator"));

			if	(pAllocator)
			{
				lTitle.TrimLeft ();
				lTitle.Format (_T("%s [%p]"), CString((LPCTSTR)lTitle), pAllocator);
				LogVfwErr (pLogLevel, pAllocator->GetProperties (lAllocatorProps.Clear()));
				LogAllocatorProps (pLogLevel, lAllocatorProps, lIndent+lTitle);
			}
			else
			{
				LogMessage (pLogLevel, _T("%s%s <null>"), lIndent, lTitle);
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

void LogAllocatorProps (UINT pLogLevel, const ALLOCATOR_PROPERTIES & pProperties, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString						lTitle;
			CString						lIndent;
			tS <ALLOCATOR_PROPERTIES>	lAllocatorProps;

			InitLogTitle (pFormat, lTitle, lIndent, _T("Allocator Properties"));

			LogMessage (pLogLevel, _T("%s%s Buffers [%d] Size [%d] Align [%d] Prefix [%d]"), lIndent, lTitle, pProperties.cBuffers, pProperties.cbBuffer, pProperties.cbAlign, pProperties.cbPrefix);
		}
		catch AnyExceptionDebug
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void LogMediaSample (UINT pLogLevel, IMediaSample * pMediaSample, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString								lTitle;
			CString								lIndent;
			long								lSampleSize;
			long								lSampleLength;
			REFERENCE_TIME						lSampleStart = -1;
			REFERENCE_TIME						lSampleEnd = -1;
			REFERENCE_TIME						lMediaStart = 0;
			REFERENCE_TIME						lMediaEnd = 0;
			IMediaSample2Ptr					lSample2 (pMediaSample);
			tSS <AM_SAMPLE2_PROPERTIES, DWORD>	lSample2Props;

			InitLogTitle (pFormat, lTitle, lIndent, _T("Sample"));

			if	(
					(lSample2 != NULL)
				&&	(SUCCEEDED (lSample2->GetProperties (sizeof(lSample2Props), (LPBYTE)lSample2Props.Clear())))
				)
			{
				pMediaSample->GetMediaTime (&lMediaStart, &lMediaEnd);
				LogMessage (pLogLevel, _T("%s%s [%p]"), lIndent, lTitle, pMediaSample);
				LogMessage (pLogLevel, _T("%s  Id [%s] Flags [%s] Type [%p]"), lIndent, SampleIdStr(lSample2Props.dwStreamId), SampleFlagsStr(lSample2Props.dwSampleFlags), lSample2Props.pMediaType);
				LogMessage (pLogLevel, _T("%s  Size/Length [%d %d]"), lIndent, lSample2Props.cbBuffer, lSample2Props.lActual);
				LogMessage (pLogLevel, _T("%s  Time [%f - %f] (%I64d - %I64d)"), lIndent, ((lSample2Props.dwSampleFlags & AM_SAMPLE_TIMEVALID) ? RefTimeSec(lSample2Props.tStart) : -1.0), ((lSample2Props.dwSampleFlags & AM_SAMPLE_TIMEVALID) ? RefTimeSec(lSample2Props.tStop) : -1.0), lMediaStart, lMediaEnd);
			}
			else
			if	(pMediaSample)
			{
				lSampleSize = pMediaSample->GetSize ();
				lSampleLength = pMediaSample->GetActualDataLength ();
				pMediaSample->GetTime (&lSampleStart, &lSampleEnd);
				pMediaSample->GetMediaTime (&lMediaStart, &lMediaEnd);
				LogMessage (pLogLevel, _T("%s%s [%p] Size/Length [%d %d] Time [%f - %f] (%I64d - %I64d)"), lIndent, lTitle, pMediaSample, lSampleSize, lSampleLength, RefTimeSec(lSampleStart), RefTimeSec(lSampleEnd), lMediaStart, lMediaEnd);
			}
			else
			{
				LogMessage (pLogLevel, _T("%s%s [%p]"), lIndent, lTitle, pMediaSample);
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

void LogMediaSampleId (UINT pLogLevel, IMediaSample * pMediaSample, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString						lTitle;
			CString						lIndent;
			long						lSampleSize;
			long						lSampleLength;
			REFERENCE_TIME				lSampleStart = -1;
			REFERENCE_TIME				lSampleEnd = -1;

			InitLogTitle (pFormat, lTitle, lIndent, _T("Sample"));

			if	(pMediaSample)
			{
				lSampleSize = pMediaSample->GetSize ();
				lSampleLength = pMediaSample->GetActualDataLength ();
				pMediaSample->GetTime (&lSampleStart, &lSampleEnd);
				LogMessage (pLogLevel, _T("%s%s [%p] Size/Length [%d %d] Time [%f - %f]"), lIndent, lTitle, pMediaSample, lSampleSize, lSampleLength, RefTimeSec(lSampleStart), RefTimeSec(lSampleEnd));
			}
			else
			{
				LogMessage (pLogLevel, _T("%s%s [%p]"), lIndent, lTitle, pMediaSample);
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

void LogMediaType (UINT pLogLevel, const AM_MEDIA_TYPE & pMediaType, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle;
			CString	lIndent;

			InitLogTitle (pFormat, lTitle, lIndent, _T("Media Type"));

			LogMessage (pLogLevel, _T("%s%s [%s] Sample size [%d]"), lIndent, lTitle, MediaTypeStr (pMediaType), pMediaType.lSampleSize);

			if	(
					(
						(IsEqualGUID (pMediaType.majortype, MEDIATYPE_Audio))
					||	(IsEqualGUID (pMediaType.majortype, MEDIATYPE_Stream))
					)
				&&	(
						(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_WAVE))
					||	(IsEqualGUID (pMediaType.subtype, MEDIASUBTYPE_PCM))
					||	(IsEqualGUID (pMediaType.formattype, FORMAT_WaveFormatEx))
					)
				&&	(pMediaType.pbFormat)
				)
			{
				if	(pMediaType.cbFormat >= sizeof (WAVEFORMATEX))
				{
					LogWaveFormat (pLogLevel, *(LPWAVEFORMATEX)pMediaType.pbFormat, lIndent+_T("  Format"));
				}
				else
				if	(pMediaType.cbFormat >= sizeof (WAVEFORMAT))
				{
					LogWaveFormat (pLogLevel, *(LPWAVEFORMAT)pMediaType.pbFormat, lIndent+_T("  Format"));
				}
			}
			else
			if	(
					(IsEqualGUID (pMediaType.majortype, MEDIATYPE_Video))
				&&	(IsEqualGUID (pMediaType.formattype, FORMAT_VideoInfo))
				&&	(pMediaType.pbFormat)
				&&	(pMediaType.cbFormat >= sizeof (VIDEOINFOHEADER))
				)
			{
				LogVideoFormat (pLogLevel, *(VIDEOINFOHEADER*)pMediaType.pbFormat, lIndent+_T("  Format"));
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

void LogWaveFormat (UINT pLogLevel, const WAVEFORMAT & pWaveFormat, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lIndent;
			CString	lTitle;

			InitLogTitle (pFormat, lTitle, lIndent, _T("WAVEFORMAT"));

			LogMessage (pLogLevel, _T("%s%s [%s] Channels [%hu] Samples/sec [%u] Bytes/sec [%u] Align [%hu] FormatSize [%u]"), lIndent, lTitle, WaveFormatStr(pWaveFormat.wFormatTag), pWaveFormat.nChannels, pWaveFormat.nSamplesPerSec, pWaveFormat.nAvgBytesPerSec, pWaveFormat.nBlockAlign, sizeof(WAVEFORMAT));
		}
		catch AnyExceptionDebug
	}
#endif
}

void LogWaveFormat (UINT pLogLevel, const WAVEFORMATEX & pWaveFormat, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lIndent;
			CString	lTitle;

			InitLogTitle (pFormat, lTitle, lIndent, _T("WAVEFORMATEX"));

			LogMessage (pLogLevel, _T("%s%s [%s] Channels [%hu] Samples/sec [%u] Bytes/sec [%u] Bits/sample [%hu] Align [%hu] FormatSize [%u (%u+%hd)]"), lIndent, lTitle, WaveFormatStr(pWaveFormat.wFormatTag), pWaveFormat.nChannels, pWaveFormat.nSamplesPerSec, pWaveFormat.nAvgBytesPerSec, pWaveFormat.wBitsPerSample, pWaveFormat.nBlockAlign, sizeof(WAVEFORMATEX)+pWaveFormat.cbSize, sizeof(WAVEFORMATEX), pWaveFormat.cbSize);
		}
		catch AnyExceptionDebug
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

void LogVideoFormat (UINT pLogLevel, const VIDEOINFOHEADER & pVideoFormat, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle;
			CString	lIndent;

			InitLogTitle (pFormat, lTitle, lIndent, _T("VIDEOINFOHEADER"));

			LogMessage (pLogLevel, _T("%s%s"), lIndent, lTitle);
			LogMessage (pLogLevel, _T("%s  SourceRect [%s]"), lIndent, FormatRect(pVideoFormat.rcSource));
			LogMessage (pLogLevel, _T("%s  TargetRect [%s]"), lIndent, FormatRect(pVideoFormat.rcTarget));
			LogMessage (pLogLevel, _T("%s  BitRate    [%u] BitErrorRate [%u] AvgTimePerFrame [%I64d]"), lIndent, pVideoFormat.dwBitRate, pVideoFormat.dwBitErrorRate, pVideoFormat.AvgTimePerFrame);
			LogMessage (pLogLevel, _T("%s  Bitmap     [%d x %d] [%hu bpp] [%u bytes] [%u colors]"), lIndent, pVideoFormat.bmiHeader.biWidth, pVideoFormat.bmiHeader.biHeight, pVideoFormat.bmiHeader.biBitCount, pVideoFormat.bmiHeader.biSizeImage, pVideoFormat.bmiHeader.biClrUsed);
		}
		catch AnyExceptionDebug
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void LogWaveSound (UINT pLogLevel, LPCVOID pSoundData, long pSoundSize, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString			lTitle;
			CString			lIndent;
			HMMIO			lSoundHandle = NULL;
			tS <MMIOINFO>	lSoundInfo;

			InitLogTitle (pFormat, lTitle, lIndent, _T("Sound"));

			lSoundInfo.fccIOProc = FOURCC_MEM;
			lSoundInfo.pchBuffer = (HPSTR) pSoundData;
			lSoundInfo.cchBuffer = pSoundSize;

			if	(lSoundHandle = mmioOpen (NULL, &lSoundInfo, MMIO_READ))
			{
				LogWaveSound (pLogLevel, lSoundHandle, _T("%s%s [%8.8X] [%u]"), lIndent, lTitle, pSoundData, pSoundSize);
				mmioClose (lSoundHandle, 0);
			}
			else
			{
				LogMmSysErrAnon (pLogLevel, lSoundInfo.wErrorRet, _T("%s%s [%8.8X] [%u]"), lIndent, lTitle, pSoundData, pSoundSize);
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

void LogWaveSound (UINT pLogLevel, LPCTSTR pSoundFileName, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString			lTitle;
			CString			lIndent;
			CString			lSoundFileName (pSoundFileName);
			HMMIO			lSoundHandle = NULL;
			tS <MMIOINFO>	lSoundInfo;

			InitLogTitle (pFormat, lTitle, lIndent, _T("Sound"));

			if	(lSoundHandle = mmioOpen (lSoundFileName.GetBuffer(MAX_PATH), &lSoundInfo, MMIO_READ|MMIO_DENYNONE))
			{
				LogWaveSound (pLogLevel, lSoundHandle, _T("%s%s [%s]"), lIndent, lTitle, pSoundFileName);
				mmioClose (lSoundHandle, 0);
			}
			else
			{
				LogMmSysErrAnon (pLogLevel, lSoundInfo.wErrorRet, _T("%s%s [%s]"), lIndent, lTitle, pSoundFileName);
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

void LogWaveSound (UINT pLogLevel, HMMIO pSound, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString			lTitle;
			CString			lIndent;
			tS <MMCKINFO>	lRiffChunk;
			tS <MMCKINFO>	lChunk [10];
			int				lLevel = 0;

			InitLogTitle (pFormat, lTitle, lIndent, _T("Sound"));

			if	(pSound)
			{
				if	(LogMmSysErr (pLogLevel, mmioDescend (pSound, &lRiffChunk, NULL, MMIO_FINDRIFF)) == MMSYSERR_NOERROR)
				{
					LogMessage (pLogLevel, _T("%s%s"), lIndent, lTitle);
					LogMessage (pLogLevel, _T("%s  RIFF  [%6u] [%6u] [%4.4hs] [%4.4hs]"), lIndent, lRiffChunk.dwDataOffset, lRiffChunk.cksize, &lRiffChunk, &lRiffChunk.ckid);

					for	(;;)
					{
						if	(mmioDescend (pSound, &lChunk[lLevel], (lLevel > 0) ? &lChunk[lLevel-1] : &lRiffChunk, 0) == MMSYSERR_NOERROR)
						{
							LogMessage (pLogLevel, _T("%s  %sChunk [%6u] [%6u] [%4.4hs] [%4.4hs]"), lIndent, CString(_T(' '),lLevel*2), lChunk[lLevel].dwDataOffset, lChunk[lLevel].cksize, &lChunk[lLevel].fccType, &lChunk[lLevel].ckid);
							if	(lLevel < 10)
							{
								mmioSeek (pSound, lChunk[lLevel].dwDataOffset+lChunk[lLevel].cksize, SEEK_SET);
								lLevel++;
							}
							else
							{
								mmioAscend (pSound, &lChunk[lLevel], 0);
							}
						}
						else
						if	(lLevel > 0)
						{
							lLevel--;
							mmioAscend (pSound, &lChunk[lLevel], 0);
						}
						else
						{
							break;
						}
					}
					mmioAscend (pSound, &lRiffChunk, 0);
				}
			}
			else
			{
				LogMessage (pLogLevel, _T("%s%s [null]"), lIndent, lTitle, pSound);
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void LogMediaEvent (UINT pLogLevel, IMediaEvent * pMediaEvent, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString		lTitle;
			CString		lIndent;
			long		lEventCode;
			LONG_PTR	lEventParam1 = NULL;
			LONG_PTR	lEventParam2 = NULL;

			InitLogTitle (pFormat, lTitle, lIndent, _T("MediaEvent"));

			if	(pMediaEvent)
			{
				while (pMediaEvent->GetEvent (&lEventCode, &lEventParam1, &lEventParam2, 0) == S_OK)
				{
					LogMediaEvent (pLogLevel, lEventCode, lEventParam1, lEventParam2, lIndent+lTitle);
					pMediaEvent->FreeEventParams (lEventCode, lEventParam1, lEventParam2);
				}
			}
			else
			{
				LogMessage (pLogLevel, _T("%s %s [%p]"), lIndent, lTitle, pMediaEvent);
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

void LogMediaEvent (UINT pLogLevel, long pEventCode, LONG_PTR pEventParam1, LONG_PTR pEventParam2, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle;
			CString	lIndent;
			CString	lEventName;

			InitLogTitle (pFormat, lTitle, lIndent, _T("MediaEvent"));

			switch (pEventCode)
			{
				case EC_COMPLETE:					lEventName = _T("COMPLETE"); break;
				case EC_USERABORT:					lEventName = _T("USERABORT"); break;
				case EC_ERRORABORT:					lEventName = _T("ERRORABORT"); break;
				case EC_TIME:						lEventName = _T("TIME"); break;
				case EC_STREAM_ERROR_STOPPED:		lEventName = _T("STREAM_ERROR_STOPPED"); break;
				case EC_STREAM_ERROR_STILLPLAYING:	lEventName = _T("STREAM_ERROR_STILLPLAYING"); break;
				case EC_ERROR_STILLPLAYING:			lEventName = _T("ERROR_STILLPLAYING"); break;
				case EC_PALETTE_CHANGED:			lEventName = _T("PALETTE_CHANGED"); break;
				case EC_VIDEO_SIZE_CHANGED:			lEventName = _T("VIDEO_SIZE_CHANGED"); break;
				case EC_QUALITY_CHANGE:				lEventName = _T("QUALITY_CHANGE"); break;
				case EC_SHUTTING_DOWN:				lEventName = _T("SHUTTING_DOWN"); break;
				case EC_CLOCK_CHANGED:				lEventName = _T("CLOCK_CHANGED"); break;
				case EC_PAUSED:						lEventName = _T("PAUSED"); break;
				case EC_OPENING_FILE:				lEventName = _T("OPENING_FILE"); break;
				case EC_BUFFERING_DATA:				lEventName = _T("BUFFERING_DATA"); break;
				case EC_FULLSCREEN_LOST:			lEventName = _T("FULLSCREEN_LOST"); break;
				case EC_ACTIVATE:					lEventName = _T("ACTIVATE"); break;
				case EC_NEED_RESTART:				lEventName = _T("NEED_RESTART"); break;
				case EC_WINDOW_DESTROYED:			lEventName = _T("WINDOW_DESTROYED"); break;
				case EC_DISPLAY_CHANGED:			lEventName = _T("DISPLAY_CHANGED"); break;
				case EC_STARVATION:					lEventName = _T("STARVATION"); break;
				case EC_OLE_EVENT:					lEventName = _T("OLE_EVENT"); break;
				case EC_NOTIFY_WINDOW:				lEventName = _T("NOTIFY_WINDOW"); break;
				case EC_STREAM_CONTROL_STOPPED:		lEventName = _T("STREAM_CONTROL_STOPPED"); break;
				case EC_STREAM_CONTROL_STARTED:		lEventName = _T("STREAM_CONTROL_STARTED"); break;
				case EC_END_OF_SEGMENT:				lEventName = _T("END_OF_SEGMENT"); break;
				case EC_SEGMENT_STARTED:			lEventName = _T("SEGMENT_STARTED"); break;
				case EC_LENGTH_CHANGED:				lEventName = _T("LENGTH_CHANGED"); break;
				case EC_DEVICE_LOST:				lEventName = _T("DEVICE_LOST"); break;
				case EC_STEP_COMPLETE:				lEventName = _T("STEP_COMPLETE"); break;
				case EC_TIMECODE_AVAILABLE:			lEventName = _T("TIMECODE_AVAILABLE"); break;
				case EC_EXTDEVICE_MODE_CHANGE:		lEventName = _T("EXTDEVICE_MODE_CHANGE"); break;
				case EC_STATE_CHANGE:				lEventName = _T("STATE_CHANGE"); break;
				case EC_GRAPH_CHANGED:				lEventName = _T("GRAPH_CHANGED"); break;
				case EC_CLOCK_UNSET:				lEventName = _T("CLOCK_UNSET"); break;
				case EC_VMR_RENDERDEVICE_SET:		lEventName = _T("VMR_RENDERDEVICE_SET"); break;
				case EC_VMR_SURFACE_FLIPPED:		lEventName = _T("VMR_SURFACE_FLIPPED"); break;
				case EC_VMR_RECONNECTION_FAILED:	lEventName = _T("VMR_RECONNECTION_FAILED"); break;
//				case EC_PREPROCESS_COMPLETE:		lEventName = _T("PREPROCESS_COMPLETE"); break;
//				case EC_CODECAPI_EVENT:				lEventName = _T("CODECAPI_EVENT"); break;
				case EC_WMT_INDEX_EVENT:			lEventName = _T("WMT_INDEX_EVENT"); break;
				case EC_WMT_EVENT:					lEventName = _T("WMT_EVENT"); break;
				case EC_BUILT:						lEventName = _T("BUILT"); break;
				case EC_UNBUILT:					lEventName = _T("UNBUILT"); break;
				case EC_SNDDEV_IN_ERROR:			lEventName = _T("SNDDEV_IN_ERROR"); break;
				case EC_SNDDEV_OUT_ERROR:			lEventName = _T("SNDDEV_OUT_ERROR"); break;
				default:
				{
					if	(pEventCode >= EC_USER)
					{
						lEventName.Format (_T("USER+%u"), pEventCode-EC_USER);
					}
				}
			}

			LogMessage (pLogLevel, _T("%s%s [%4.4X] %-30.30s [%u (%8.8X)] [%u (%8.8X)]"), lIndent, lTitle, pEventCode, lEventName, pEventParam1, pEventParam1, pEventParam2, pEventParam2);
		}
		catch AnyExceptionDebug
	}
#endif
}
