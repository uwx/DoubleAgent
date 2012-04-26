/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
#pragma once

class CDaSvrEventDispatch
{
public:
	int FireCommand(long CommandID, IDaSvrUserInput2* UserInput);
	int FireActivateInputState(long CharacterID, long Activated);
	int FireVisibleState(long CharacterID, long Visible, long Cause);
	int FireClick(long CharacterID, short Keys, long X, long Y);
	int FireDblClick(long CharacterID, short Keys, long X, long Y);
	int FireDragStart(long CharacterID, short Keys, long X, long Y);
	int FireDragComplete(long CharacterID, short Keys, long X, long Y);
	int FireRequestStart(long RequestID);
	int FireRequestComplete(long RequestID, long Result);
	int FireBookMark(long BookMarkID);
	int FireIdle(long CharacterID, long Start);
	int FireMove(long CharacterID, long X, long Y, long Cause);
	int FireSize(long CharacterID, long Width, long Height);
	int FireBalloonVisibleState(long CharacterID, long Visible);
	int FireListeningState(long CharacterID, long Listening, long Cause);
	int FireDefaultCharacterChange(BSTR CharGUID);
	int FireAgentPropertyChange();
	int FireActiveClientChange(long CharacterID, long Status);

	int FireSpeechStart(long CharacterID, IDaSvrFormattedText* FormattedText);
	int FireSpeechEnd(long CharacterID, IDaSvrFormattedText* FormattedText, VARIANT_BOOL Stopped);
	int FireSpeechWord(long CharacterID, IDaSvrFormattedText* FormattedText, long WordNdx);

public:
	CDaSvrEventDispatch (CComDynamicUnkArray& pUnkArray, CAtlTypeArray <long>& pUnregisterDelayed) : mUnkArray (pUnkArray), mUnregisterDelayed (pUnregisterDelayed) {}
protected:
	CComDynamicUnkArray&	mUnkArray;
	CAtlTypeArray <long>&	mUnregisterDelayed;
};

/////////////////////////////////////////////////////////////////////////////

template <typename T>
class CDaSvrEventCaller
{
public:
	int FireCommand(long CommandID, IDaSvrUserInput2* UserInput);
	int FireActivateInputState(long CharacterID, long Activated);
	int FireVisibleState(long CharacterID, long Visible, long Cause);
	int FireClick(long CharacterID, short Keys, long X, long Y);
	int FireDblClick(long CharacterID, short Keys, long X, long Y);
	int FireDragStart(long CharacterID, short Keys, long X, long Y);
	int FireDragComplete(long CharacterID, short Keys, long X, long Y);
	int FireRequestStart(long RequestID);
	int FireRequestComplete(long RequestID, long Result);
	int FireBookMark(long BookMarkID);
	int FireIdle(long CharacterID, long Start);
	int FireMove(long CharacterID, long X, long Y, long Cause);
	int FireSize(long CharacterID, long Width, long Height);
	int FireBalloonVisibleState(long CharacterID, long Visible);
__if_exists(T::ListeningState)
{
	int FireListeningState(long CharacterID, long Listening, long Cause);
	int FireDefaultCharacterChange(BSTR CharGUID);
	int FireAgentPropertyChange();
	int FireActiveClientChange(long CharacterID, long Status);
}
__if_exists(T::SpeechStart)
{
	int FireSpeechStart(long CharacterID, IDaSvrFormattedText* FormattedText);
	int FireSpeechEnd(long CharacterID, IDaSvrFormattedText* FormattedText, VARIANT_BOOL Stopped);
	int FireSpeechWord(long CharacterID, IDaSvrFormattedText* FormattedText, long WordNdx);
}

public:
	CDaSvrEventCaller (CComDynamicUnkArray& pUnkArray, CAtlTypeArray <long>& pUnregisterDelayed) : mUnkArray (pUnkArray), mUnregisterDelayed (pUnregisterDelayed) {}
protected:
	CComDynamicUnkArray&	mUnkArray;
	CAtlTypeArray <long>&	mUnregisterDelayed;
};

/////////////////////////////////////////////////////////////////////////////

template <class T>
class CProxy_DaSvrEvents2 : public IConnectionPointImpl<T, &__uuidof(_DaSvrEvents2), CComDynamicUnkArray>, public CDaSvrEventDispatch
{
public:
	CProxy_DaSvrEvents2 (CAtlTypeArray <long>& pUnregisterDelayed) : CDaSvrEventDispatch (m_vec, pUnregisterDelayed) {}
};

template <class T>
class CProxyIDaSvrNotifySink : public IConnectionPointImpl<T, &__uuidof(IDaSvrNotifySink), CComDynamicUnkArray>, public CDaSvrEventCaller<IDaSvrNotifySink>
{
public:
	CProxyIDaSvrNotifySink (CAtlTypeArray <long>& pUnregisterDelayed) : CDaSvrEventCaller<IDaSvrNotifySink> (m_vec, pUnregisterDelayed) {}
};

template <class T>
class CProxyIDaSvrNotifySink2 : public IConnectionPointImpl<T, &__uuidof(IDaSvrNotifySink2), CComDynamicUnkArray>, public CDaSvrEventCaller<IDaSvrNotifySink2>
{
public:
	CProxyIDaSvrNotifySink2 (CAtlTypeArray <long>& pUnregisterDelayed) : CDaSvrEventCaller<IDaSvrNotifySink2> (m_vec, pUnregisterDelayed) {}
};

template <class T>
class CProxyIAgentNotifySink : public IConnectionPointImpl<T, &__uuidof(IAgentNotifySink), CComDynamicUnkArray>, public CDaSvrEventCaller<IAgentNotifySink>
{
public:
	CProxyIAgentNotifySink (CAtlTypeArray <long>& pUnregisterDelayed) : CDaSvrEventCaller<IAgentNotifySink> (m_vec, pUnregisterDelayed) {}
};

template <class T>
class CProxyIAgentNotifySinkEx : public IConnectionPointImpl<T, &__uuidof(IAgentNotifySinkEx), CComDynamicUnkArray>, public CDaSvrEventCaller<IAgentNotifySinkEx>
{
public:
	CProxyIAgentNotifySinkEx (CAtlTypeArray <long>& pUnregisterDelayed) : CDaSvrEventCaller<IAgentNotifySinkEx> (m_vec, pUnregisterDelayed) {}
};

/////////////////////////////////////////////////////////////////////////////

template <class T>
int CDaSvrEventCaller<T>::FireCommand(long CommandID, IDaSvrUserInput2* UserInput)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		int				lNdx;
		IUnknown *		lUnknown;
		CComQIPtr <T>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink->Command (CommandID, UserInput));
				lCount++;
			}
		}
	}
	return lCount;
}

template <class T>
int CDaSvrEventCaller<T>::FireActivateInputState(long CharacterID, long Activated)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		int				lNdx;
		IUnknown *		lUnknown;
		CComQIPtr <T>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink->ActivateInputState (CharacterID, Activated));
				lCount++;
			}
		}
	}
	return lCount;
}

template <class T>
int CDaSvrEventCaller<T>::FireVisibleState(long CharacterID, long Visible, long Cause)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		int				lNdx;
		IUnknown *		lUnknown;
		CComQIPtr <T>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink->VisibleState ( CharacterID, Visible, Cause));
				lCount++;
			}
		}
	}
	return lCount;
}

template <class T>
int CDaSvrEventCaller<T>::FireClick(long CharacterID, short Keys, long X, long Y)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		int				lNdx;
		IUnknown *		lUnknown;
		CComQIPtr <T>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink->Click (CharacterID, Keys, X, Y));
				lCount++;
			}
		}
	}
	return lCount;
}

template <class T>
int CDaSvrEventCaller<T>::FireDblClick(long CharacterID, short Keys, long X, long Y)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		int				lNdx;
		IUnknown *		lUnknown;
		CComQIPtr <T>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink->DblClick (CharacterID, Keys, X, Y));
				lCount++;
			}
		}
	}
	return lCount;
}

template <class T>
int CDaSvrEventCaller<T>::FireDragStart(long CharacterID, short Keys, long X, long Y)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		int				lNdx;
		IUnknown *		lUnknown;
		CComQIPtr <T>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink->DragStart (CharacterID, Keys, X, Y));
				lCount++;
			}
		}
	}
	return lCount;
}

template <class T>
int CDaSvrEventCaller<T>::FireDragComplete(long CharacterID, short Keys, long X, long Y)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		int				lNdx;
		IUnknown *		lUnknown;
		CComQIPtr <T>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink->DragComplete (CharacterID, Keys, X, Y));
				lCount++;
			}
		}
	}
	return lCount;
}

template <class T>
int CDaSvrEventCaller<T>::FireRequestStart(long RequestID)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		int				lNdx;
		IUnknown *		lUnknown;
		CComQIPtr <T>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink->RequestStart (RequestID));
				lCount++;
			}
		}
	}
	return lCount;
}

template <class T>
int CDaSvrEventCaller<T>::FireRequestComplete(long RequestID, long Result)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		int				lNdx;
		IUnknown *		lUnknown;
		CComQIPtr <T>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink->RequestComplete (RequestID, Result));
				lCount++;
			}
		}
	}
	return lCount;
}

template <class T>
int CDaSvrEventCaller<T>::FireBookMark(long BookMarkID)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		int				lNdx;
		IUnknown *		lUnknown;
		CComQIPtr <T>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink->BookMark (BookMarkID));
				lCount++;
			}
		}
	}
	return lCount;
}

template <class T>
int CDaSvrEventCaller<T>::FireIdle(long CharacterID, long Start)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		int				lNdx;
		IUnknown *		lUnknown;
		CComQIPtr <T>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink->Idle (CharacterID, Start));
				lCount++;
			}
		}
	}
	return lCount;
}

template <class T>
int CDaSvrEventCaller<T>::FireMove(long CharacterID, long X, long Y, long Cause)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		int				lNdx;
		IUnknown *		lUnknown;
		CComQIPtr <T>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink->Move (CharacterID, X, Y, Cause));
				lCount++;
			}
		}
	}
	return lCount;
}

template <class T>
int CDaSvrEventCaller<T>::FireSize(long CharacterID, long Width, long Height)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		int				lNdx;
		IUnknown *		lUnknown;
		CComQIPtr <T>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink->Size (CharacterID, Width, Height));
				lCount++;
			}
		}
	}
	return lCount;
}

template <class T>
int CDaSvrEventCaller<T>::FireBalloonVisibleState(long CharacterID, long Visible)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		int				lNdx;
		IUnknown *		lUnknown;
		CComQIPtr <T>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink->BalloonVisibleState (CharacterID, Visible));
				lCount++;
			}
		}
	}
	return lCount;
}

template <class T>
int CDaSvrEventCaller<T>::FireListeningState(long CharacterID, long Listening, long Cause)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		int				lNdx;
		IUnknown *		lUnknown;
		CComQIPtr <T>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink->ListeningState (CharacterID, Listening, Cause));
				lCount++;
			}
		}
	}
	return lCount;
}

template <class T>
int CDaSvrEventCaller<T>::FireDefaultCharacterChange(BSTR CharGUID)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		int				lNdx;
		IUnknown *		lUnknown;
		CComQIPtr <T>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink->DefaultCharacterChange (CharGUID));
				lCount++;
			}
		}
	}
	return lCount;
}

template <class T>
int CDaSvrEventCaller<T>::FireAgentPropertyChange()
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		int				lNdx;
		IUnknown *		lUnknown;
		CComQIPtr <T>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink->AgentPropertyChange ());
				lCount++;
			}
		}
	}
	return lCount;
}

template <class T>
int CDaSvrEventCaller<T>::FireActiveClientChange(long CharacterID, long Status)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		int				lNdx;
		IUnknown *		lUnknown;
		CComQIPtr <T>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink->ActiveClientChange (CharacterID, Status));
				lCount++;
			}
		}
	}
	return lCount;
}

template <class T>
int CDaSvrEventCaller<T>::FireSpeechStart(long CharacterID, IDaSvrFormattedText* FormattedText)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		int				lNdx;
		IUnknown *		lUnknown;
		CComQIPtr <T>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink->SpeechStart (CharacterID, FormattedText));
				lCount++;
			}
		}
	}
	return lCount;
}

template <class T>
int CDaSvrEventCaller<T>::FireSpeechEnd(long CharacterID, IDaSvrFormattedText* FormattedText, VARIANT_BOOL Stopped)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		int				lNdx;
		IUnknown *		lUnknown;
		CComQIPtr <T>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink->SpeechEnd (CharacterID, FormattedText, Stopped));
				lCount++;
			}
		}
	}
	return lCount;
}

template <class T>
int CDaSvrEventCaller<T>::FireSpeechWord(long CharacterID, IDaSvrFormattedText* FormattedText, long WordNdx)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		int				lNdx;
		IUnknown *		lUnknown;
		CComQIPtr <T>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink->SpeechWord (CharacterID, FormattedText, WordNdx));
				lCount++;
			}
		}
	}
	return lCount;
}

/////////////////////////////////////////////////////////////////////////////
