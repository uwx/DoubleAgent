// DaCtlUserInput.cpp : Implementation of CDaCtlUserInput

#include "stdafx.h"
#include "DaCtlUserInput.h"


// CDaCtlUserInput

STDMETHODIMP CDaCtlUserInput::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IDaCtlUserInput
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
