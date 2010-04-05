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
#include <vfw.h>
#include <vfwmsgs.h>
#include "VfwErr.h"
////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
////////////////////////////////////////////////////////////////////////

LPCTSTR _GetVfwErr (HRESULT pError)
{
	LPCTSTR	lError = NULL;

	switch (pError)
	{
		case VFW_E_INVALIDMEDIATYPE:					lError = _T("An invalid media type was specified."); break;
		case VFW_E_INVALIDSUBTYPE:						lError = _T("An invalid media subtype was specified."); break;
		case VFW_E_NEED_OWNER:							lError = _T("This object can only be created as an aggregated object."); break;
		case VFW_E_ENUM_OUT_OF_SYNC:					lError = _T("The enumerator has become invalid."); break;
		case VFW_E_ALREADY_CONNECTED:					lError = _T("At least one of the pins involved in the operation is already connected."); break;
		case VFW_E_FILTER_ACTIVE:						lError = _T("This operation cannot be performed because the filter is active."); break;
		case VFW_E_NO_TYPES:							lError = _T("One of the specified pins supports no media types."); break;
		case VFW_E_NO_ACCEPTABLE_TYPES:					lError = _T("There is no common media type between these pins."); break;
		case VFW_E_INVALID_DIRECTION:					lError = _T("Two pins of the same direction cannot be connected together."); break;
		case VFW_E_NOT_CONNECTED:						lError = _T("The operation cannot be performed because the pins are not connected."); break;
		case VFW_E_NO_ALLOCATOR:						lError = _T("No sample buffer allocator is available."); break;
		case VFW_E_RUNTIME_ERROR:						lError = _T("A run-time error occurred."); break;
		case VFW_E_BUFFER_NOTSET:						lError = _T("No buffer space has been set."); break;
		case VFW_E_BUFFER_OVERFLOW:						lError = _T("The buffer is not big enough."); break;
		case VFW_E_BADALIGN:							lError = _T("An invalid alignment was specified."); break;
		case VFW_E_ALREADY_COMMITTED:					lError = _T("Cannot change allocated memory while the filter is active."); break;
		case VFW_E_BUFFERS_OUTSTANDING:					lError = _T("One or more buffers are still active."); break;
		case VFW_E_NOT_COMMITTED:						lError = _T("Cannot allocate a sample when the allocator is not active."); break;
		case VFW_E_SIZENOTSET:							lError = _T("Cannot allocate memory because no size has been set."); break;
		case VFW_E_NO_CLOCK:							lError = _T("Cannot lock for synchronization because no clock has been defined."); break;
		case VFW_E_NO_SINK:								lError = _T("Quality messages could not be sent because no quality sink has been defined."); break;
		case VFW_E_NO_INTERFACE:						lError = _T("A required interface has not been implemented."); break;
		case VFW_E_NOT_FOUND:							lError = _T("An object or name was not found."); break;
		case VFW_E_CANNOT_CONNECT:						lError = _T("No combination of intermediate filters could be found to make the connection."); break;
		case VFW_E_CANNOT_RENDER:						lError = _T("No combination of filters could be found to render the stream."); break;
		case VFW_E_CHANGING_FORMAT:						lError = _T("Could not change formats dynamically."); break;
		case VFW_E_NO_COLOR_KEY_SET:					lError = _T("No color key has been set."); break;
		case VFW_E_NOT_OVERLAY_CONNECTION:				lError = _T("Current pin connection is not using the IOverlay transport."); break;
		case VFW_E_NOT_SAMPLE_CONNECTION:				lError = _T("Current pin connection is not using the IMemInputPin transport."); break;
		case VFW_E_PALETTE_SET:							lError = _T("Setting a color key would conflict with the palette already set."); break;
		case VFW_E_COLOR_KEY_SET:						lError = _T("Setting a palette would conflict with the color key already set."); break;
		case VFW_E_NO_COLOR_KEY_FOUND:					lError = _T("No matching color key is available."); break;
		case VFW_E_NO_PALETTE_AVAILABLE:				lError = _T("No palette is available."); break;
		case VFW_E_NO_DISPLAY_PALETTE:					lError = _T("Display does not use a palette."); break;
		case VFW_E_TOO_MANY_COLORS:						lError = _T("Too many colors for the current display settings."); break;
		case VFW_E_STATE_CHANGED:						lError = _T("The state changed while waiting to process the sample."); break;
		case VFW_E_NOT_STOPPED:							lError = _T("The operation could not be performed because the filter is not stopped."); break;
		case VFW_E_NOT_PAUSED:							lError = _T("The operation could not be performed because the filter is not paused."); break;
		case VFW_E_NOT_RUNNING:							lError = _T("The operation could not be performed because the filter is not running."); break;
		case VFW_E_WRONG_STATE:							lError = _T("The operation could not be performed because the filter is in the wrong state."); break;
		case VFW_E_START_TIME_AFTER_END:				lError = _T("The sample start time is after the sample end time."); break;
		case VFW_E_INVALID_RECT:						lError = _T("The supplied rectangle is invalid."); break;
		case VFW_E_TYPE_NOT_ACCEPTED:					lError = _T("This pin cannot use the supplied media type."); break;
		case VFW_E_SAMPLE_REJECTED:						lError = _T("This sample cannot be rendered."); break;
		case VFW_E_SAMPLE_REJECTED_EOS:					lError = _T("This sample cannot be rendered because the end of the stream has been reached."); break;
		case VFW_E_DUPLICATE_NAME:						lError = _T("An attempt to add a filter with a duplicate name failed."); break;
		case VFW_S_DUPLICATE_NAME:						lError = _T("An attempt to add a filter with a duplicate name succeeded with a modified name."); break;
		case VFW_E_TIMEOUT:								lError = _T("A time-out has expired."); break;
		case VFW_E_INVALID_FILE_FORMAT:					lError = _T("The file format is invalid."); break;
		case VFW_E_ENUM_OUT_OF_RANGE:					lError = _T("The list has already been exhausted."); break;
		case VFW_E_CIRCULAR_GRAPH:						lError = _T("The filter graph is circular."); break;
		case VFW_E_NOT_ALLOWED_TO_SAVE:					lError = _T("Updates are not allowed in this state."); break;
		case VFW_E_TIME_ALREADY_PASSED:					lError = _T("An attempt was made to queue a command for a time in the past."); break;
		case VFW_E_ALREADY_CANCELLED:					lError = _T("The queued command has already been canceled."); break;
		case VFW_E_CORRUPT_GRAPH_FILE:					lError = _T("Cannot render the file because it is corrupt."); break;
		case VFW_E_ADVISE_ALREADY_SET:					lError = _T("An overlay advise link already exists."); break;
		case VFW_S_STATE_INTERMEDIATE:					lError = _T("The state transition has not completed."); break;
		case VFW_E_NO_MODEX_AVAILABLE:					lError = _T("No full-screen modes are available."); break;
		case VFW_E_NO_ADVISE_SET:						lError = _T("This Advise cannot be canceled because it was not successfully set."); break;
		case VFW_E_NO_FULLSCREEN:						lError = _T("A full-screen mode is not available."); break;
		case VFW_E_IN_FULLSCREEN_MODE:					lError = _T("Cannot call IVideoWindow methods while in full-screen mode."); break;
		case VFW_E_UNKNOWN_FILE_TYPE:					lError = _T("The media type of this file is not recognized."); break;
		case VFW_E_CANNOT_LOAD_SOURCE_FILTER:			lError = _T("The source filter for this file could not be loaded."); break;
		case VFW_S_PARTIAL_RENDER:						lError = _T("Some of the streams in this movie are in an unsupported format."); break;
		case VFW_E_FILE_TOO_SHORT:						lError = _T("A file appeared to be incomplete."); break;
		case VFW_E_INVALID_FILE_VERSION:				lError = _T("The version number of the file is invalid."); break;
		case VFW_S_SOME_DATA_IGNORED:					lError = _T("The file contained some property settings that were not used."); break;
		case VFW_S_CONNECTIONS_DEFERRED:				lError = _T("Some connections have failed and have been deferred."); break;
		case VFW_E_INVALID_CLSID:						lError = _T("This file is corrupt: it contains an invalid class identifier."); break;
		case VFW_E_INVALID_MEDIA_TYPE:					lError = _T("This file is corrupt: it contains an invalid media type."); break;
		case VFW_E_BAD_KEY:								lError = _T("A registry entry is corrupt."); break;
		case VFW_S_NO_MORE_ITEMS:						lError = _T("The end of the list has been reached."); break;
		case VFW_E_SAMPLE_TIME_NOT_SET:					lError = _T("No time stamp has been set for this sample."); break;
		case VFW_S_RESOURCE_NOT_NEEDED:					lError = _T("The resource specified is no longer needed."); break;
		case VFW_E_MEDIA_TIME_NOT_SET:					lError = _T("No media time stamp has been set for this sample."); break;
		case VFW_E_NO_TIME_FORMAT_SET:					lError = _T("No media time format has been selected."); break;
		case VFW_E_MONO_AUDIO_HW:						lError = _T("Cannot change balance because audio device is mono only."); break;
		case VFW_S_MEDIA_TYPE_IGNORED:					lError = _T("A connection could not be made with the media type in the persistent graph, but has been made with a negotiated media type."); break;
		case VFW_E_NO_DECOMPRESSOR:						lError = _T("Cannot play back the video stream: no suitable decompressor could be found."); break;
		case VFW_E_NO_AUDIO_HARDWARE:					lError = _T("Cannot play back the audio stream: no audio hardware is available, or the hardware is not responding."); break;
		case VFW_S_VIDEO_NOT_RENDERED:					lError = _T("Cannot play back the video stream: no suitable decompressor could be found."); break;
		case VFW_S_AUDIO_NOT_RENDERED:					lError = _T("Cannot play back the audio stream: no audio hardware is available."); break;
		case VFW_E_RPZA:								lError = _T("Cannot play back the video stream: format 'RPZA' is not supported."); break;
		case VFW_S_RPZA:								lError = _T("Cannot play back the video stream: format 'RPZA' is not supported."); break;
		case VFW_E_PROCESSOR_NOT_SUITABLE:				lError = _T("ActiveMovie cannot play MPEG movies on this processor."); break;
		case VFW_E_UNSUPPORTED_AUDIO:					lError = _T("Cannot play back the audio stream: the audio format is not supported."); break;
		case VFW_E_UNSUPPORTED_VIDEO:					lError = _T("Cannot play back the video stream: the video format is not supported."); break;
		case VFW_E_MPEG_NOT_CONSTRAINED:				lError = _T("ActiveMovie cannot play this video stream because it falls outside the constrained standard."); break;
		case VFW_E_NOT_IN_GRAPH:						lError = _T("Cannot perform the requested function on an object that is not in the filter graph."); break;
		case VFW_S_ESTIMATED:							lError = _T("The value returned had to be estimated.  It's accuracy can not be guaranteed."); break;
		case VFW_E_NO_TIME_FORMAT:						lError = _T("Cannot get or set time related information on an object that is using a time format of TIME_FORMAT_NONE."); break;
		case VFW_E_READ_ONLY:							lError = _T("The connection cannot be made because the stream is read only and the filter alters the data."); break;
		case VFW_E_BUFFER_UNDERFLOW:					lError = _T("The buffer is not full enough."); break;
		case VFW_E_UNSUPPORTED_STREAM:					lError = _T("Cannot play back the file.  The format is not supported."); break;
		case VFW_E_NO_TRANSPORT:						lError = _T("Pins cannot connect due to not supporting the same transport."); break;
		case VFW_S_STREAM_OFF:							lError = _T("The stream has been turned off."); break;
		case VFW_S_CANT_CUE:							lError = _T("The graph can't be cued because of lack of or corrupt data."); break;
		case VFW_E_BAD_VIDEOCD:							lError = _T("The Video CD can't be read correctly by the device or is the data is corrupt."); break;
		case VFW_S_NO_STOP_TIME:						lError = _T("The stop time for the sample was not set."); break;
		case VFW_E_OUT_OF_VIDEO_MEMORY:					lError = _T("There is not enough Video Memory at this display resolution and number of colors. Reducing resolution might help."); break;
		case VFW_E_VP_NEGOTIATION_FAILED:				lError = _T("The VideoPort connection negotiation process has failed."); break;
		case VFW_E_DDRAW_CAPS_NOT_SUITABLE:				lError = _T("Either DirectDraw has not been installed or the Video Card capabilities are not suitable. Make sure the display is not in 16 color mode or try changing the graphics mode."); break;
		case VFW_E_NO_VP_HARDWARE:						lError = _T("No VideoPort hardware is available, or the hardware is not responding."); break;
		case VFW_E_NO_CAPTURE_HARDWARE:					lError = _T("No Capture hardware is available, or the hardware is not responding."); break;
		case VFW_E_DVD_OPERATION_INHIBITED:				lError = _T("This User Operation is inhibited by DVD Content at this time."); break;
		case VFW_E_DVD_INVALIDDOMAIN:					lError = _T("This Operation is not permitted in the current domain."); break;
		case VFW_E_DVD_NO_BUTTON:						lError = _T("The specified button is invalid or is not present at the current time, or there is no button present at the specified location."); break;
		case VFW_E_DVD_GRAPHNOTREADY:					lError = _T("DVD-Video playback graph has not been built yet."); break;
		case VFW_E_DVD_RENDERFAIL:						lError = _T("DVD-Video playback graph building failed."); break;
		case VFW_E_DVD_DECNOTENOUGH:					lError = _T("DVD-Video playback graph could not be built due to insufficient decoders."); break;
		case VFW_E_DDRAW_VERSION_NOT_SUITABLE:			lError = _T("Version number of DirectDraw not suitable. Make sure to install dx5 or higher version."); break;
		case VFW_E_COPYPROT_FAILED:						lError = _T("Copy protection cannot be enabled. Please make sure any other copy protected content is not being shown now."); break;
		case VFW_S_NOPREVIEWPIN:						lError = _T("There was no preview pin available, so the capture pin output is being split to provide both capture and preview."); break;
		case VFW_E_TIME_EXPIRED:						lError = _T("This object cannot be used anymore as its time has expired."); break;
		case VFW_S_DVD_NON_ONE_SEQUENTIAL:				lError = _T("The current title was not a sequential set of chapters (PGC), and the returned timing information might not be continuous."); break;
		case VFW_E_DVD_WRONG_SPEED:						lError = _T("The operation cannot be performed at the current playback speed."); break;
		case VFW_E_DVD_MENU_DOES_NOT_EXIST:				lError = _T("The specified menu doesn't exist."); break;
		case VFW_E_DVD_CMD_CANCELLED:					lError = _T("The specified command was either cancelled or no longer exists."); break;
		case VFW_E_DVD_STATE_WRONG_VERSION:				lError = _T("The data did not contain a recognized version."); break;
		case VFW_E_DVD_STATE_CORRUPT:					lError = _T("The state data was corrupt."); break;
		case VFW_E_DVD_STATE_WRONG_DISC:				lError = _T("The state data is from a different disc."); break;
		case VFW_E_DVD_INCOMPATIBLE_REGION:				lError = _T("The region was not compatible with the current drive."); break;
		case VFW_E_DVD_NO_ATTRIBUTES:					lError = _T("The requested DVD stream attribute does not exist."); break;
		case VFW_E_DVD_NO_GOUP_PGC:						lError = _T("Currently there is no GoUp (Annex J user function) program chain (PGC)."); break;
		case VFW_E_DVD_LOW_PARENTAL_LEVEL:				lError = _T("The current parental level was too low."); break;
		case VFW_E_DVD_NOT_IN_KARAOKE_MODE:				lError = _T("The current audio is not karaoke content."); break;
		case VFW_S_DVD_CHANNEL_CONTENTS_NOT_AVAILABLE:	lError = _T("The audio stream did not contain sufficient information to determine the contents of each channel."); break;
		case VFW_S_DVD_NOT_ACCURATE:					lError = _T("The seek into the movie was not frame accurate."); break;
		case VFW_E_FRAME_STEP_UNSUPPORTED:				lError = _T("Frame step is not supported on this configuration."); break;
		case VFW_E_DVD_STREAM_DISABLED:					lError = _T("The specified stream is disabled and cannot be selected."); break;
		case VFW_E_DVD_TITLE_UNKNOWN:					lError = _T("The operation depends on the current title number, however the navigator has not yet entered the VTSM or the title domains, so the 'current' title index is unknown."); break;
		case VFW_E_DVD_INVALID_DISC:					lError = _T("The specified path does not point to a valid DVD disc."); break;
		case VFW_E_DVD_NO_RESUME_INFORMATION:			lError = _T("There is currently no resume information."); break;
		case VFW_E_PIN_ALREADY_BLOCKED_ON_THIS_THREAD:	lError = _T("This thread has already blocked this output pin.  There is no need to call IPinFlowControl::Block () again."); break;
		case VFW_E_PIN_ALREADY_BLOCKED:					lError = _T("IPinFlowControl::Block () has been called on another thread.  The current thread cannot make any assumptions about this pin's block state."); break;
		case VFW_E_CERTIFICATION_FAILURE:				lError = _T("An operation failed due to a certification failure."); break;
		case VFW_E_VMR_NOT_IN_MIXER_MODE:				lError = _T("The VMR has not yet created a mixing component.  That is, IVMRFilterConfig::SetNumberofStreams has not yet been called."); break;
		case VFW_E_VMR_NO_AP_SUPPLIED:					lError = _T("The application has not yet provided the VMR filter with a valid allocator-presenter object."); break;
		case VFW_E_VMR_NO_DEINTERLACE_HW:				lError = _T("The VMR could not find any de-interlacing hardware on the current display device."); break;
		case VFW_E_VMR_NO_PROCAMP_HW:					lError = _T("The VMR could not find any ProcAmp hardware on the current display device."); break;
		case VFW_E_DVD_VMR9_INCOMPATIBLEDEC:			lError = _T("VMR9 does not work with VPE-based hardware decoders."); break;
		case E_PROP_SET_UNSUPPORTED:					lError = _T("The Specified property set is not supported."); break;
		case E_PROP_ID_UNSUPPORTED:						lError = _T("The specified property ID is not supported for the specified property set."); break;
	}

	return lError;
}

////////////////////////////////////////////////////////////////////////

#ifdef	_LOG_NOFILELINE
HRESULT LogVfwErr (unsigned int pLogLevel, HRESULT pError, LPCTSTR pFormat, ...)
#else
extern "C" void _LogPrepFileLine (LPTSTR pString, LPCSTR pFile, UINT pLine);
HRESULT _LogVfwErrFL::LogErr (unsigned int pLogLevel, HRESULT pError, LPCTSTR pFormat, ...)
#endif
{
	if	(
			(
				(FAILED (pError))
			||	((pLogLevel & 0x000000FF) == LogAlways)
			)
		&&	(LogIsActive (pLogLevel))
		)
	{
		try
		{
			LPTSTR	lExtra = NULL;
			TCHAR	lFormat [1024];
			LPCTSTR	lError;

			if	(pError == S_OK)
			{
				lError = _T("S_OK");
			}
			else
			if	(pError == S_FALSE)
			{
				lError = _T("S_FALSE");
			}
			else
			{
				lError = _GetVfwErr (pError);
			}

			if	(lError)
			{
				_tcscpy (lFormat, _T("VfwError [%8.8X] [%s]"));
			}
			else
			{
				_tcscpy (lFormat, _T(""));
			}

			if  (pFormat)
			{
				const size_t    lExtraSize = 4096;
				va_list 		lArgPtr;

				lExtra = new TCHAR [lExtraSize];
				if	(lError)
				{
					_tcscpy (lExtra, _T(" => "));
				}
				else
				{
					_tcscpy (lExtra, _T(""));
				}
				va_start (lArgPtr, pFormat);
				_vsntprintf (lExtra+_tcslen(lExtra), lExtraSize-_tcslen(lExtra), pFormat, lArgPtr);
				_tcscat (lFormat, _T("%s"));
			}

			try
			{
				if	(lError)
				{
#ifndef	_LOG_NOFILELINE
					_LogPrepFileLine (lFormat, mFile, mLine);
#endif
					LogMessage (pLogLevel, lFormat, pError, lError, lExtra);
				}
				else
				{
#ifdef	_LOG_NOFILELINE
					LogComErr (MinLogLevel (pLogLevel, LogAlways), pError, lFormat, lExtra);
#else
					_LogComErrFL(mFile,mLine).LogErr (MinLogLevel (pLogLevel, LogAlways), pError, lFormat, lExtra);
#endif
				}
			}
			catch AnyExceptionSilent

			if	(lExtra)
			{
				delete [] lExtra;
			}
		}
		catch AnyExceptionSilent
	}

	return pError;
}

////////////////////////////////////////////////////////////////////////
#endif	// _DEBUG
////////////////////////////////////////////////////////////////////////
