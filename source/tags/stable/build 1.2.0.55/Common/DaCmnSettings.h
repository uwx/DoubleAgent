/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software:
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
#pragma once
#include "DaServerOdl.h"
#include "AgentFileCache.h"

/////////////////////////////////////////////////////////////////////////////

class CDaCmnSettings
{
public:
	CDaCmnSettings (class CGlobalAnchor* pAnchor);
	virtual ~CDaCmnSettings ();

// Interfaces
public:
	// ICDaSettings
	HRESULT get_SoundEffectsEnabled (VARIANT_BOOL *SoundEffectsEnabled);
	HRESULT get_BalloonEnabled (VARIANT_BOOL *BalloonEnabled);
	HRESULT get_BalloonFont (IFontDisp **BalloonFont);
	HRESULT get_TTSEnabled (VARIANT_BOOL *TTSEnabled);
	HRESULT get_TTSSpeed (short *TTSSpeed);
	HRESULT get_SREnabled (VARIANT_BOOL *SREnabled);
	HRESULT get_SRHotKey (BSTR *SRHotKey);
	HRESULT get_SRHotKeyTime (short *SRHotKeyTime);
	HRESULT get_SRListeningTip (VARIANT_BOOL *SRListeningTip);
	HRESULT get_SRListeningPrompt (VARIANT_BOOL *SRListeningPrompt);
	HRESULT get_AudioStatus (AudioStatusType *AudioStatus);

// Implementation
protected:
	AudioStatusType GetAudioStatus ();

protected:
	class CGlobalAnchor*	mAnchor;
};

/////////////////////////////////////////////////////////////////////////////
