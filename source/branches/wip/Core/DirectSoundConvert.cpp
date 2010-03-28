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
#include "DirectSoundConvert.h"
#include <mmreg.h>
#include <MsAcm.h>
#include "MmSysError.h"
#ifdef	_DEBUG
#include "DebugProcess.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msacm32.lib")

#ifdef	_DEBUG
//#define	_DEBUG_CONVERT		LogNormal|LogHighVolume
//#define	_DEBUG_FORMATS		LogNormal|LogHighVolume
//#define	_TRACE_RESOURCES	(GetProfileDebugInt(_T("TraceResources"),LogVerbose,true)&0xFFFF|LogHighVolume)
#endif

/////////////////////////////////////////////////////////////////////////////

class tHandleTypeAcmStream : public tHandleType <HACMSTREAM>
{
private:
	virtual bool __Close (HACMSTREAM & pHandle) {return (acmStreamClose (pHandle, 0) == MMSYSERR_NOERROR);}
};
typedef tHandle <HACMSTREAM, tHandleTypeAcmStream>	CAcmStreamHandle;

/////////////////////////////////////////////////////////////////////////////

CDirectSoundConvert::CDirectSoundConvert ()
:	mInputBuffer (NULL),
	mInputSize (0),
	mOutputSize (0),
	mOutputLength (0)
{
}

CDirectSoundConvert::~CDirectSoundConvert ()
{
}

/////////////////////////////////////////////////////////////////////////////

void CDirectSoundConvert::SetInputFormat (const LPWAVEFORMATEX pFormat)
{
	CSingleLock	lLock (&mLock, TRUE);
	try
	{
		mInputFormat = DuplicateFormat (pFormat);
	}
	catch AnyExceptionSilent
}

bool CDirectSoundConvert::GetInputFormat (LPWAVEFORMATEX & pFormat)
{
	bool		lRet = false;
	CSingleLock	lLock (&mLock, TRUE);
	try
	{
		pFormat = mInputFormat;
		if	(pFormat)
		{
			lRet = true;
		}
	}
	catch AnyExceptionSilent
	return lRet;
}

bool CDirectSoundConvert::GetOutputFormat (LPWAVEFORMATEX & pFormat)
{
	bool		lRet = false;
	CSingleLock	lLock (&mLock, TRUE);
	try
	{
		pFormat = mOutputFormat;
		if	(pFormat)
		{
			lRet = true;
		}
	}
	catch AnyExceptionSilent
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void CDirectSoundConvert::SetInputBuffer (LPCVOID pBuffer, ULONG pBufferSize)
{
	CSingleLock	lLock (&mLock, TRUE);
	try
	{
		mInputBuffer = pBuffer;
		mInputSize = pBufferSize;
	}
	catch AnyExceptionSilent
}

bool CDirectSoundConvert::GetInputBuffer (LPCVOID & pBuffer, ULONG & pBufferSize)
{
	bool		lRet = false;
	CSingleLock	lLock (&mLock, TRUE);
	try
	{
		pBuffer = mInputBuffer;
		pBufferSize = mInputSize;
		if	(
				(pBuffer)
			&&	(pBufferSize)
			)
		{
			lRet = true;
		}
	}
	catch AnyExceptionSilent
	return lRet;
}

bool CDirectSoundConvert::GetOutputBuffer (LPVOID & pBuffer, ULONG & pBufferSize, ULONG & pDataLength)
{
	bool		lRet = false;
	CSingleLock	lLock (&mLock, TRUE);
	try
	{
		pBuffer = GlobalLock (mOutputBuffer);
		pBufferSize = mOutputSize;
		pDataLength = mOutputLength;
		if	(
				(pBuffer)
			&&	(pBufferSize)
			&&	(pDataLength)
			)
		{
			lRet = true;
		}
	}
	catch AnyExceptionSilent
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectSoundConvert::ConvertSound ()
{
	HRESULT		lResult = E_UNEXPECTED;
	CSingleLock	lLock (&mLock, TRUE);

	try
	{
#ifdef	_TRACE_RESOURCES
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectSoundConvert::ConvertSound"), this);
#endif
		if	(
				(mInputBuffer)
			&&	(mInputSize)
			&&	(mInputFormat)
			&&	(mOutputFormat)
			)
		{
			MMRESULT						lMmResult;
			CAcmStreamHandle				lAcmStream;
			tSS <ACMSTREAMHEADER, DWORD>	lStreamHeader;

#ifdef	_DEBUG_CONVERT
			LogWaveFormat (_DEBUG_CONVERT, *mInputFormat, _T("Convert From"));
			LogWaveFormat (_DEBUG_CONVERT, *mOutputFormat, _T("Convert To  "));
#endif
			if	(LogMmSysErr (LogNormal, lMmResult = acmStreamOpen (&lAcmStream, NULL, mInputFormat, mOutputFormat, NULL, NULL, NULL, ACM_STREAMOPENF_NONREALTIME)) == MMSYSERR_NOERROR)
			{
				try
				{
					HMMIO			lSrcIo = NULL;
					HMMIO			lTrgIo = NULL;
					tS <MMIOINFO>	lSrcInfo;
					tS <MMIOINFO>	lTrgInfo;
					tS <MMCKINFO>	lRiffChunk;
					tS <MMCKINFO>	lWavChunk;
					tS <MMCKINFO>	lDataChunk;

					lSrcInfo.fccIOProc = FOURCC_MEM;
					lSrcInfo.pchBuffer = (HPSTR) mInputBuffer;
					lSrcInfo.cchBuffer = mInputSize;

					lWavChunk.ckid = mmioFOURCC('f','m','t',' ');
					lDataChunk.ckid = mmioFOURCC('d','a','t','a');

					if	(
							(lSrcIo = mmioOpen (NULL, &lSrcInfo, MMIO_READ))
						&&	(LogMmSysErr (LogDetails, mmioDescend (lSrcIo, &lRiffChunk, NULL, MMIO_FINDRIFF)) == MMSYSERR_NOERROR)
						&&	(LogMmSysErr (LogDetails, mmioDescend (lSrcIo, &lWavChunk, &lRiffChunk, MMIO_FINDCHUNK)) == MMSYSERR_NOERROR)
						)
					{
						mmioAscend (lSrcIo, &lWavChunk, 0);
						if	(LogMmSysErr (LogDetails, mmioDescend (lSrcIo, &lDataChunk, &lRiffChunk, MMIO_FINDCHUNK)) == MMSYSERR_NOERROR)
						{
							mmioAscend (lSrcIo, &lDataChunk, 0);
						}
						mmioAscend (lSrcIo, &lRiffChunk, 0);
					}
					if	(lSrcIo)
					{
						mmioClose (lSrcIo, 0);
					}
					else
					{
						LogMmSysErr (LogNormal, lMmResult = lSrcInfo.wErrorRet);
					}

					if	(
							(lStreamHeader.cbSrcLength = lDataChunk.cksize)
						&&	(lStreamHeader.pbSrc = (LPBYTE)(lSrcInfo.pchBuffer + lDataChunk.dwDataOffset))
						&&	(LogMmSysErr (LogDetails, lMmResult = acmStreamSize (lAcmStream, lSrcInfo.cchBuffer, &lStreamHeader.cbDstLength, ACM_STREAMSIZEF_SOURCE)) == MMSYSERR_NOERROR)
						&&	((mOutputBuffer = GlobalAlloc (GPTR, lStreamHeader.cbDstLength)).SafeIsValid())
						&&	(lStreamHeader.pbDst = (LPBYTE) GlobalLock (mOutputBuffer))
						)
					{
						memset (lStreamHeader.pbDst, 0, lStreamHeader.cbDstLength);

						lTrgInfo.fccIOProc = FOURCC_MEM;
						lTrgInfo.pchBuffer = (HPSTR)lStreamHeader.pbDst;
						lTrgInfo.cchBuffer = lStreamHeader.cbDstLength;

						lRiffChunk.Clear();
						lWavChunk.Clear();
						lDataChunk.Clear();
						lRiffChunk.fccType = mmioFOURCC('W','A','V','E');
						lWavChunk.ckid = mmioFOURCC('f','m','t',' ');
						lDataChunk.ckid = mmioFOURCC('d','a','t','a');

						if	(
								(lTrgIo = mmioOpen (NULL, &lTrgInfo, MMIO_CREATE|MMIO_READWRITE))
							&&	(LogMmSysErr (LogDetails, mmioCreateChunk (lTrgIo, &lRiffChunk, MMIO_CREATERIFF)) == MMSYSERR_NOERROR)
							&&	(LogMmSysErr (LogDetails, mmioCreateChunk (lTrgIo, &lWavChunk, 0)) == MMSYSERR_NOERROR)
							)
						{
							mmioWrite (lTrgIo, (HPSTR)mOutputFormat.Ptr(), sizeof(WAVEFORMATEX) + mOutputFormat->cbSize);
							mmioAscend (lTrgIo, &lWavChunk, 0);
							mmioGetInfo (lTrgIo, &lTrgInfo,	0);
							mmioCreateChunk (lTrgIo, &lDataChunk, 0);
							mmioAscend (lTrgIo, &lDataChunk, 0);
							mmioAscend (lTrgIo, &lRiffChunk, 0);

							lStreamHeader.pbDst += lDataChunk.dwDataOffset;
							lStreamHeader.cbDstLength -= lDataChunk.dwDataOffset;

							if	(mOutputFormat->wBitsPerSample == 8)
							{
								memset (lStreamHeader.pbDst, 0x80, lStreamHeader.cbDstLength);
							}
							else
							{
								memset (lStreamHeader.pbDst, 0x00, lStreamHeader.cbDstLength);
							}

							if	(LogMmSysErr (LogNormal, lMmResult = acmStreamPrepareHeader (lAcmStream, &lStreamHeader, 0)) == MMSYSERR_NOERROR)
							{
								if	(LogMmSysErr (LogNormal, lMmResult = acmStreamConvert (lAcmStream, &lStreamHeader, 0)) == MMSYSERR_NOERROR)
								{
									((LPDWORD)lTrgInfo.pchBuffer)[1] += lStreamHeader.cbDstLengthUsed;
									((LPDWORD)lTrgInfo.pchNext)[1] = lStreamHeader.cbDstLengthUsed;

									mOutputSize = lDataChunk.dwDataOffset + lStreamHeader.cbDstLengthUsed;
									mOutputLength = lStreamHeader.cbDstLengthUsed;
									lResult = S_OK;
								}
								else
								{
									((LPDWORD)lTrgInfo.pchNext)[1] = 0;
								}
								LogMmSysErr (LogNormal, acmStreamUnprepareHeader (lAcmStream, &lStreamHeader, 0));
							}
						}
						if	(lTrgIo)
						{
							mmioClose (lTrgIo, 0);
						}
						else
						{
							LogMmSysErr (LogNormal, lMmResult = lTrgInfo.wErrorRet);
						}
					}
				}
				catch AnyExceptionDebug
			}

			if	(lResult != S_OK)
			{
				mOutputBuffer.Close ();

				if	(lMmResult == MMSYSERR_NOERROR)
				{
					lResult = E_FAIL;
				}
				else
				{
					lResult = HRESULT_FROM_WIN32 (lMmResult);
				}
			}
		}

#ifdef	_DEBUG_CONVERT
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONVERT, _T("Converted sound [%p] [%u] to [%p] [%u %u]"), mInputBuffer, mInputSize, GlobalLock(mOutputBuffer), mOutputSize, mOutputLength);
		}
		else
		{
			LogMessage (_DEBUG_CONVERT, _T("Convert failed"));
		}
#endif
#ifdef	_TRACE_RESOURCES
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectSoundConvert::ConvertSound Done"), this);
#endif
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectSoundConvert::StashSound ()
{
	HRESULT		lResult = E_FAIL;
	CSingleLock	lLock (&mLock, TRUE);

	try
	{
#ifdef	_TRACE_RESOURCES
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectSoundConvert::StashSound"), this);
#endif
		if	(
				(mInputBuffer)
			&&	(mInputSize)
			&&	(mInputFormat)
			)
		{
			HMMIO			lSrcIo = NULL;
			HMMIO			lTrgIo = NULL;
			tS <MMIOINFO>	lSrcInfo;
			tS <MMCKINFO>	lRiffChunk;
			tS <MMCKINFO>	lWavChunk;
			tS <MMCKINFO>	lDataChunk;
			LPBYTE			lOutputBuffer;

			lSrcInfo.fccIOProc = FOURCC_MEM;
			lSrcInfo.pchBuffer = (HPSTR) mInputBuffer;
			lSrcInfo.cchBuffer = mInputSize;

			lWavChunk.ckid = mmioFOURCC('f','m','t',' ');
			lDataChunk.ckid = mmioFOURCC('d','a','t','a');

			if	(lSrcIo = mmioOpen (NULL, &lSrcInfo, MMIO_READ))
			{
				if	(
						(mmioDescend (lSrcIo, &lRiffChunk, NULL, MMIO_FINDRIFF) == MMSYSERR_NOERROR)
					&&	(mmioDescend (lSrcIo, &lWavChunk, &lRiffChunk, MMIO_FINDCHUNK) == MMSYSERR_NOERROR)
					)
				{
					mmioAscend (lSrcIo, &lWavChunk, 0);
					if	(mmioDescend (lSrcIo, &lDataChunk, &lRiffChunk, MMIO_FINDCHUNK) == MMSYSERR_NOERROR)
					{
						mOutputBuffer = GlobalAlloc (GPTR, lDataChunk.cksize);
						if	(
								(mOutputBuffer.SafeIsValid ())
							&&	(lOutputBuffer = (LPBYTE) GlobalLock (mOutputBuffer))
							)
						{
							memcpy (lOutputBuffer, (LPBYTE)mInputBuffer+lDataChunk.dwDataOffset, lDataChunk.cksize);
							GlobalUnlock (mOutputBuffer);
							mOutputSize = lDataChunk.cksize;
							mOutputLength = lDataChunk.cksize;
							mOutputFormat = DuplicateFormat (mInputFormat);
							lResult = S_OK;
						}
						else
						{
							mOutputBuffer.Close ();
						}
						mmioAscend (lSrcIo, &lDataChunk, 0);
					}
					mmioAscend (lSrcIo, &lRiffChunk, 0);
					mmioClose (lSrcIo, 0);
				}
			}
		}
#ifdef	_TRACE_RESOURCES
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CDirectSoundConvert::StashSound Done"), this);
#endif
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectSoundConvert::DeriveOutputFormat ()
{
	HRESULT		lResult = E_UNEXPECTED;
	CSingleLock	lLock (&mLock, TRUE);

	try
	{
		tPtr <WAVEFORMATEX>			lOutputFormat;
		COwnPtrArray <WAVEFORMATEX>	lRankedFormats;
		INT_PTR						lFormatNdx;
		DWORD						lFormatSize = 0;

		if	(mInputFormat)
		{
#ifdef	_DEBUG_FORMATS
			LogWaveFormat (_DEBUG_FORMATS, *mInputFormat, _T("Input    "));
#endif
			if	(SUCCEEDED (lResult = EnumAcmFormats (mInputFormat, lRankedFormats)))
			{
				for	(lFormatNdx = 0; lFormatNdx <= lRankedFormats.GetUpperBound(); lFormatNdx++)
				{
					if	(IsValidOutputFormat (mInputFormat, lRankedFormats[lFormatNdx]))
					{
						lOutputFormat = lRankedFormats [lFormatNdx];
						lRankedFormats [lFormatNdx] = NULL;
						break;
					}
				}

				if	(
						(!lOutputFormat)
					&&	(SUCCEEDED (LogMmSysErr (LogNormal, acmMetrics (NULL, ACM_METRIC_MAX_SIZE_FORMAT, &lFormatSize))))
					)
				{
					for	(lFormatNdx = 0; lFormatNdx <= lRankedFormats.GetUpperBound(); lFormatNdx++)
					{
						if	(SuggestOutputFormat (mInputFormat, lRankedFormats[lFormatNdx], lOutputFormat.Free(), lFormatSize))
						{
							break;
						}
					}
				}
			}

			if	(lOutputFormat)
			{
				mOutputFormat = lOutputFormat.Detach ();
#ifdef	_DEBUG_FORMATS
				LogWaveFormat (_DEBUG_FORMATS, *mOutputFormat, _T("Output   "));
#endif
				lResult = S_OK;
			}
			else
			{
				lResult = S_FALSE;
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

bool CDirectSoundConvert::IsValidOutputFormat (const LPWAVEFORMATEX pInputFormat, const LPWAVEFORMATEX pOutputFormat)
{
	bool				lRet = false;
	CAcmStreamHandle	lAcmStream;

	if	(acmStreamOpen (&lAcmStream, NULL, pInputFormat, pOutputFormat, NULL, NULL, NULL, ACM_STREAMOPENF_QUERY) == MMSYSERR_NOERROR)
	{
		lRet = true;
	}
	else
	if	(acmStreamOpen (&lAcmStream, NULL, pInputFormat, pOutputFormat, NULL, NULL, NULL, ACM_STREAMOPENF_QUERY|ACM_STREAMOPENF_NONREALTIME) == MMSYSERR_NOERROR)
	{
		lRet = true;
	}
	return lRet;
}

bool CDirectSoundConvert::SuggestOutputFormat (const LPWAVEFORMATEX pInputFormat, const LPWAVEFORMATEX pFilterFormat, LPWAVEFORMATEX * pOutputFormat, DWORD pFormatSize)
{
	bool				lRet = false;
	tPtr <WAVEFORMATEX>	lOutputFormat;

	lOutputFormat = DuplicateFormat (pFilterFormat, pFormatSize);
#ifdef	_DEBUG_FORMATS
	LogWaveFormat (_DEBUG_FORMATS, *lOutputFormat, _T("  Filter   "));
#endif

	if	(
			(acmFormatSuggest (NULL, pInputFormat, lOutputFormat, pFormatSize, ACM_FORMATSUGGESTF_NCHANNELS|ACM_FORMATSUGGESTF_NSAMPLESPERSEC|ACM_FORMATSUGGESTF_WBITSPERSAMPLE) == MMSYSERR_NOERROR)
		||	(acmFormatSuggest (NULL, pInputFormat, lOutputFormat, pFormatSize, ACM_FORMATSUGGESTF_NCHANNELS|ACM_FORMATSUGGESTF_WBITSPERSAMPLE) == MMSYSERR_NOERROR)
		||	(acmFormatSuggest (NULL, pInputFormat, lOutputFormat, pFormatSize, ACM_FORMATSUGGESTF_NCHANNELS|ACM_FORMATSUGGESTF_NSAMPLESPERSEC) == MMSYSERR_NOERROR)
		||	(acmFormatSuggest (NULL, pInputFormat, lOutputFormat, pFormatSize, ACM_FORMATSUGGESTF_NSAMPLESPERSEC|ACM_FORMATSUGGESTF_WBITSPERSAMPLE) == MMSYSERR_NOERROR)
		||	(acmFormatSuggest (NULL, pInputFormat, lOutputFormat, pFormatSize, ACM_FORMATSUGGESTF_WBITSPERSAMPLE) == MMSYSERR_NOERROR)
		||	(acmFormatSuggest (NULL, pInputFormat, lOutputFormat, pFormatSize, ACM_FORMATSUGGESTF_NSAMPLESPERSEC) == MMSYSERR_NOERROR)
		||	(acmFormatSuggest (NULL, pInputFormat, lOutputFormat, pFormatSize, 0) == MMSYSERR_NOERROR)
		)
	{
#ifdef	_DEBUG_FORMATS
		LogWaveFormat (_DEBUG_FORMATS, *lOutputFormat, _T("    Suggest"));
#endif
		*pOutputFormat = lOutputFormat.Detach();
		lRet = true;
	}
	return lRet;
}


/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDirectSoundConvert::EnumAcmFormats (const LPWAVEFORMATEX pInputFormat, COwnPtrArray <WAVEFORMATEX> & pRankedFormats)
{
	HRESULT				lResult;
	IBaseFilterPtr		lACMFilter;
	IEnumPinsPtr		lEnumPins;
	IPinPtr				lPin;
	PIN_DIRECTION		lPinDir;
	IEnumMediaTypesPtr	lEnumMediaTypes;
	tMediaTypePtr		lMediaType;
	LPWAVEFORMATEX		lMediaFormat;
	CTypeArray <int>	lRanks;
	INT_PTR				lRankNdx;

	if	(
			(SUCCEEDED (lResult = LogComErr (LogNormal, CoCreateInstance (CLSID_ACMWrapper, NULL, CLSCTX_INPROC, __uuidof (IBaseFilter), (void **) &lACMFilter))))
		&&	(SUCCEEDED (lResult = lACMFilter->EnumPins (&lEnumPins)))
		)
	{
		while (lEnumPins->Next (1, &lPin, NULL) == S_OK)
		{
			if	(
					(SUCCEEDED (lPin->QueryDirection (&lPinDir)))
				&&	(lPinDir == PINDIR_OUTPUT)
				&&	(SUCCEEDED (lPin->EnumMediaTypes (&lEnumMediaTypes)))
				)
			{
				while (lEnumMediaTypes->Next (1, lMediaType.Free(), NULL) == S_OK)
				{
					if	(
							(IsEqualGUID (lMediaType->formattype, FORMAT_WaveFormatEx))
						&&	(lMediaFormat = (LPWAVEFORMATEX) lMediaType->pbFormat)
						)
					{
						lRankNdx = lRanks.AddSortedQS (RankFormatMatch (pInputFormat, lMediaFormat), NULL, false);
						pRankedFormats.InsertAt (lRankNdx, DuplicateFormat (lMediaFormat));
					}
				}
			}
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

LPWAVEFORMATEX CDirectSoundConvert::DuplicateFormat (const LPWAVEFORMATEX pFormat, DWORD pFormatSize)
{
	LPWAVEFORMATEX	lFormat;
	UINT			lFormatAlloc = max (pFormatSize, sizeof(WAVEFORMATEX) + pFormat->cbSize);
	UINT			lFormatSize = min (lFormatAlloc, sizeof(WAVEFORMATEX) + pFormat->cbSize);

	if	(lFormat = (LPWAVEFORMATEX) new BYTE [lFormatAlloc])
	{
		memcpy (lFormat, pFormat, lFormatSize);
	}
	return lFormat;
}

int CDirectSoundConvert::RankFormatMatch (const LPWAVEFORMATEX pInputFormat, const LPWAVEFORMATEX pOutputFormat)
{
	int	lRet = 0;

	if	(
			(pInputFormat->nChannels)
		&&	(pOutputFormat->nChannels)
		)
	{
		lRet += MulDiv ((int)max(pInputFormat->nChannels,pOutputFormat->nChannels), 2, (int)min(pInputFormat->nChannels,pOutputFormat->nChannels));
	}
	if	(
			(pInputFormat->nSamplesPerSec)
		&&	(pOutputFormat->nSamplesPerSec)
		)
	{
		lRet += MulDiv ((int)max(pInputFormat->nSamplesPerSec,pOutputFormat->nSamplesPerSec), 8, (int)min(pInputFormat->nSamplesPerSec,pOutputFormat->nSamplesPerSec));
	}
	if	(
			(pInputFormat->wBitsPerSample)
		&&	(pOutputFormat->wBitsPerSample)
		)
	{
		lRet += MulDiv ((int)max(pInputFormat->wBitsPerSample,pOutputFormat->wBitsPerSample), 8, (int)min(pInputFormat->wBitsPerSample,pOutputFormat->wBitsPerSample));
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDirectSoundConvertCache::CDirectSoundConvertCache ()
{
}

CDirectSoundConvertCache::~CDirectSoundConvertCache ()
{
}

INT_PTR CDirectSoundConvertCache::GetSize () const
{
	CSingleLock	lLock (&mLock, TRUE);
	return mCache.GetCount();
}

/////////////////////////////////////////////////////////////////////////////

CDirectSoundConvert * CDirectSoundConvertCache::GetCachedConvert (UINT pSoundNdx)
{
	CDirectSoundConvert *	lRet = NULL;
	CSingleLock				lLock (&mLock, TRUE);

	try
	{
		mCache.Lookup (pSoundNdx, lRet);
	}
	catch AnyExceptionDebug

	return lRet;
}

bool CDirectSoundConvertCache::CacheSoundConvert (CDirectSoundConvert * pConvert, UINT pSoundNdx)
{
	bool		lRet = false;
	CSingleLock	lLock (&mLock, TRUE);

	try
	{
		if	(pConvert)
		{
			CDirectSoundConvert *	lConvert;

			if	(
					(!mCache.Lookup (pSoundNdx, lConvert))
				||	(!lConvert)
				)
			{
				mCache.SetAt (pSoundNdx, pConvert);
				lRet = true;
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

bool CDirectSoundConvertCache::RemovedCachedConvert (UINT pSoundNdx)
{
	bool		lRet = false;
	CSingleLock	lLock (&mLock, TRUE);

	try
	{
		if	(mCache.RemoveKey (pSoundNdx))
		{
			lRet = true;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}
