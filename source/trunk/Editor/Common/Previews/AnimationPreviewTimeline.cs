/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
using System;
using System.Windows;
using System.Windows.Media.Animation;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Previews
{
	public class AnimationPreviewTimeline : System.Windows.Media.Animation.ParallelTimeline
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

#if DEBUG
		static private int mNextInstanceNum = 1;
		private int mInstanceNum = mNextInstanceNum++;
#endif

		public AnimationPreviewTimeline ()
		{
#if DEBUG_NOT
			CurrentStateInvalidated += new EventHandler (OnCurrentStateInvalidated);
			CurrentGlobalSpeedInvalidated += new EventHandler (OnCurrentGlobalSpeedInvalidated);
			Completed += new EventHandler (OnCompleted);
#endif
#if DEBUG_NOT
			CurrentTimeInvalidated += new EventHandler (OnCurrentTimeInvalidated);
			RemoveRequested += new EventHandler (OnRemoveRequested);
#endif
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Infrastructure

		protected override System.Windows.Freezable CreateInstanceCore ()
		{
			return new AnimationPreviewTimeline ();
		}

		public new AnimationPreviewTimeline Clone ()
		{
			AnimationPreviewTimeline lClone = CreateInstanceCore () as AnimationPreviewTimeline;
			lClone.CloneCore (this);
			return lClone;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void OnCurrentStateInvalidated (object sender, EventArgs e)
		{
#if DEBUG
			try
			{
				ClockGroup lClock = sender as ClockGroup;
				System.Diagnostics.Debug.Print ("[{0}] {1}(M{2}) State [{3}] Frozen [{4}] Repeat [{5}]", lClock.CurrentTime, Name, mInstanceNum, lClock.CurrentState, IsFrozen, RepeatBehavior);
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
#endif
		}

		void OnCurrentGlobalSpeedInvalidated (object sender, EventArgs e)
		{
#if DEBUG
			try
			{
				ClockGroup lClock = sender as ClockGroup;
				if (lClock.CurrentGlobalSpeed.HasValue)
				{
					System.Diagnostics.Debug.Print ("[{0}] {1}(M{2}) Speed [{3}]", lClock.CurrentTime, Name, mInstanceNum, lClock.CurrentGlobalSpeed);
				}
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
#endif
		}

		private void OnCurrentTimeInvalidated (object sender, EventArgs e)
		{
#if DEBUG
			try
			{
				ClockGroup lClock = sender as ClockGroup;
				if (lClock.CurrentTime.HasValue)
				{
					System.Diagnostics.Debug.Print ("[{0}] {1}(M{2}) Tick", lClock.CurrentTime, Name, mInstanceNum);
				}
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
#endif
		}

		private void OnCompleted (object sender, EventArgs e)
		{
#if DEBUG
			try
			{
				ClockGroup lClock = sender as ClockGroup;
				System.Diagnostics.Debug.Print ("[{0}] {1}(M{2}) Completed [{3}]", lClock.CurrentTime, Name, mInstanceNum, lClock.CurrentState);
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
#endif
		}

		void OnRemoveRequested (object sender, EventArgs e)
		{
#if DEBUG
			System.Diagnostics.Debug.Print ("{0}(M{1}) Remove", Name, mInstanceNum);
#endif
		}
		#endregion
	}
}
