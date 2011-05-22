/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
	public class AnimationPreviewFrames : ObjectAnimationBase, IKeyFrameAnimation
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

#if DEBUG
		static private int mNextInstanceNum = 1;
		private int mInstanceNum = mNextInstanceNum++;
#endif

		public AnimationPreviewFrames (CharacterFile pCharacterFile, FileAnimation pFileAnimation, System.Windows.Media.ImageDrawing pTarget)
		{
			TimeSpan lStartTime = new TimeSpan ();
			FileAnimationFrame lLastFrame = null;

#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("{0} [{1}] frames", pFileAnimation.Name, pFileAnimation.FrameCount);
#endif
			foreach (FileAnimationFrame lFileFrame in pFileAnimation.Frames)
			{
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("  Frame [{0}] at [{1}]", pFileAnimation.Frames.IndexOf (lFileFrame), lStartTime);
#endif
				if ((lFileFrame.ImageCount > 0) || (lLastFrame == null))
				{
					KeyFrames.Add (new AnimationPreviewFrame (pCharacterFile, lFileFrame, lStartTime));
				}
				else
				{
					KeyFrames.Add (new AnimationPreviewFrame (AnimationPreviewFrame.MakeImageSource (pCharacterFile, lLastFrame), lFileFrame, lStartTime));
				}
				if (lFileFrame.ImageCount > 0)
				{
					lLastFrame = lFileFrame;
				}
				lStartTime += TimeSpan.FromMilliseconds (lFileFrame.Duration * 10);
			}
			if (lLastFrame != null)
			{
				// Add a final frame to complete the duration
				KeyFrames.Add (new AnimationPreviewFrame (AnimationPreviewFrame.MakeImageSource (pCharacterFile, lLastFrame), null, lStartTime));
			}

			Target = pTarget;
			Name = pFileAnimation.Name;
			FillBehavior = FillBehavior.HoldEnd;
		}

		private AnimationPreviewFrames ()
		{
			//
			//	Only trap events on frozen clones
			//
			CurrentStateInvalidated += new EventHandler (OnCurrentStateInvalidated);
#if DEBUG_NOT
			Completed += new EventHandler (OnCompleted);
			CurrentGlobalSpeedInvalidated += new EventHandler (OnCurrentGlobalSpeedInvalidated);
#endif
#if DEBUG_NOT
			CurrentTimeInvalidated += new EventHandler (OnCurrentTimeInvalidated);
			RemoveRequested += new EventHandler (OnRemoveRequested);
#endif
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public System.Windows.Media.ImageDrawing Target
		{
			get;
			protected set;
		}

		private ObjectKeyFrameCollection mKeyFrames = new ObjectKeyFrameCollection ();
		public ObjectKeyFrameCollection KeyFrames
		{
			get
			{
				return mKeyFrames;
			}
			set
			{
				if (IsFrozen)
				{
					throw new InvalidOperationException ();
				}
				else
				{
					mKeyFrames = value;
				}
			}

		}

		System.Collections.IList IKeyFrameAnimation.KeyFrames
		{
			get
			{
				return mKeyFrames;
			}
			set
			{
				if (IsFrozen)
				{
					throw new InvalidOperationException ();
				}
				else if (value is ObjectKeyFrameCollection)
				{
					mKeyFrames = value as ObjectKeyFrameCollection;
				}
				else
				{
					throw new InvalidCastException ();
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		public TimeSpan? FrameToTime (FileAnimationFrame pFrame)
		{
			if (pFrame != null)
			{
				foreach (AnimationPreviewFrame lKeyFrame in KeyFrames)
				{
					if (Object.ReferenceEquals (lKeyFrame.FileFrame, pFrame))
					{
						return lKeyFrame.BeginTime;
					}
				}
			}
			return null;
		}

		public TimeSpan? FrameIndexToTime (int pFrameIndex)
		{
			if ((pFrameIndex >= 0) && (pFrameIndex < KeyFrames.Count))
			{
				return KeyFrames[pFrameIndex].KeyTime.TimeSpan;
			}
			return null;
		}

		///////////////////////////////////////////////////////////////////////////////

		public FileAnimationFrame TimeToFrame (TimeSpan pTime)
		{
			FileAnimationFrame lRet = null;

			foreach (AnimationPreviewFrame lKeyFrame in KeyFrames)
			{
				if (lKeyFrame.BeginTime <= pTime)
				{
					lRet = lKeyFrame.FileFrame;
				}
				else
				{
					break;
				}
			}
			return lRet;
		}

		public int TimeToFrameIndex (TimeSpan pTime)
		{
			FileAnimationFrame lFrame = TimeToFrame (pTime);

			if ((lFrame != null) && (lFrame.Container != null) && lFrame.Container.Contains (lFrame))
			{
				return lFrame.Container.IndexOf (lFrame);
			}
			return -1;
		}

		///////////////////////////////////////////////////////////////////////////////

		public FileAnimationFrame TimeToFrame (TimeSpan? pTime)
		{
			return (pTime.HasValue) ? TimeToFrame (pTime.Value) : null;
		}

		public int TimeToFrameIndex (TimeSpan? pTime)
		{
			return (pTime.HasValue) ? TimeToFrameIndex (pTime.Value) : -1;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Implementation

		private Duration mDuration = Duration.Automatic;

		protected override void OnChanged ()
		{
			base.OnChanged ();
			mDuration = Duration.Automatic;
		}

		protected override Duration GetNaturalDurationCore (Clock clock)
		{
			return mDuration;
		}

		protected override object GetCurrentValueCore (object defaultOriginValue, object defaultDestinationValue, AnimationClock animationClock)
		{
			object lRet = null;

			if (animationClock.CurrentTime.HasValue)
			{
				int lNdx;
				AnimationPreviewFrame lKeyFrame;

				for (lNdx = KeyFrames.Count - 1; lNdx >= 0; lNdx--)
				{
					lKeyFrame = KeyFrames[lNdx] as AnimationPreviewFrame;

					if (lKeyFrame.BeginTime <= animationClock.CurrentTime.Value)
					{
						lRet = lKeyFrame.Value;

						if (lKeyFrame.EndTime <= animationClock.CurrentTime.Value)
						{
							if (mDuration == Duration.Automatic)
							{
								mDuration = new Duration (lKeyFrame.EndTime);
#if DEBUG_NOT
								System.Diagnostics.Debug.Print ("{0} {1}(I{2}) Duration -> {3}", animationClock.CurrentTime, Name, mInstanceNum, mDuration);
#endif
							}
						}
						else
						{
							mDuration = Duration.Automatic;
						}
						break;
					}
				}
			}
			return lRet;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Infrastructure

		protected override System.Windows.Freezable CreateInstanceCore ()
		{
			return new AnimationPreviewFrames ();
		}

		public new AnimationPreviewFrames Clone ()
		{
			AnimationPreviewFrames lClone = CreateInstanceCore () as AnimationPreviewFrames;
			lClone.CloneCore (this);
			return lClone;
		}

		protected override void CloneCore (Freezable sourceFreezable)
		{
			base.CloneCore (sourceFreezable);
			if (sourceFreezable is AnimationPreviewFrames)
			{
				AnimationPreviewFrames lSource = (sourceFreezable as AnimationPreviewFrames);
				this.Target = lSource.Target;
				this.mKeyFrames = lSource.KeyFrames.Clone ();
			}
		}

		protected override void CloneCurrentValueCore (System.Windows.Freezable sourceFreezable)
		{
			base.CloneCurrentValueCore (sourceFreezable);
			if (sourceFreezable is AnimationPreviewFrames)
			{
				AnimationPreviewFrames lSource = (sourceFreezable as AnimationPreviewFrames);
				this.Target = lSource.Target;
				mKeyFrames = lSource.KeyFrames.CloneCurrentValue () as ObjectKeyFrameCollection;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override bool FreezeCore (bool isChecking)
		{
			if (base.FreezeCore (isChecking))
			{
				if (isChecking)
				{
					return KeyFrames.CanFreeze;
				}
				else
				{
					KeyFrames.Freeze ();
					return true;
				}
			}
			return false;
		}

		protected override void GetAsFrozenCore (System.Windows.Freezable source)
		{
			base.GetAsFrozenCore (source);
			if (source is AnimationPreviewFrames)
			{
				AnimationPreviewFrames lSource = (source as AnimationPreviewFrames);
				this.Target = lSource.Target;

				if (lSource.KeyFrames.IsFrozen)
				{
					mKeyFrames = lSource.KeyFrames;
				}
				else if (lSource.KeyFrames.CanFreeze)
				{
					mKeyFrames = lSource.KeyFrames.GetAsFrozen () as ObjectKeyFrameCollection;
				}
				else
				{
					mKeyFrames = lSource.KeyFrames.Clone ();
					if (mKeyFrames.CanFreeze)
					{
						mKeyFrames.Freeze ();
					}
				}
			}
		}

		protected override void GetCurrentValueAsFrozenCore (System.Windows.Freezable source)
		{
			base.GetCurrentValueAsFrozenCore (source);
			if (source is AnimationPreviewFrames)
			{
				AnimationPreviewFrames lSource = (source as AnimationPreviewFrames);
				this.Target = lSource.Target;

				if (lSource.KeyFrames.IsFrozen)
				{
					mKeyFrames = lSource.KeyFrames;
				}
				else if (lSource.KeyFrames.CanFreeze)
				{
					mKeyFrames = lSource.KeyFrames.GetCurrentValueAsFrozen () as ObjectKeyFrameCollection;
				}
				else
				{
					mKeyFrames = lSource.KeyFrames.CloneCurrentValue () as ObjectKeyFrameCollection;
					if (mKeyFrames.CanFreeze)
					{
						mKeyFrames.Freeze ();
					}
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void OnCurrentStateInvalidated (object sender, EventArgs e)
		{
			AnimationClock lClock = sender as AnimationClock;
#if DEBUG_NOT
			try
			{
				System.Diagnostics.Debug.Print ("[{0}] {1}(I{2}) State [{3}] Frozen [{4}]", lClock.CurrentTime, Name, mInstanceNum, lClock.CurrentState, IsFrozen);
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
#endif
			try
			{
				if (lClock.CurrentState == ClockState.Active)
				{
					Target.ApplyAnimationClock (System.Windows.Media.ImageDrawing.ImageSourceProperty, lClock);
				}
				else if (lClock.CurrentState == ClockState.Stopped)
				{
					Target.ApplyAnimationClock (System.Windows.Media.ImageDrawing.ImageSourceProperty, null);
				}
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
		}

		void OnCurrentGlobalSpeedInvalidated (object sender, EventArgs e)
		{
#if DEBUG
			try
			{
				AnimationClock lClock = sender as AnimationClock;
				if (lClock.CurrentGlobalSpeed.HasValue)
				{
					System.Diagnostics.Debug.Print ("[{0}] {1}(I{2}) Speed [{3}]", lClock.CurrentTime, Name, mInstanceNum, lClock.CurrentGlobalSpeed);
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
				AnimationClock lClock = sender as AnimationClock;
				if (lClock.CurrentTime.HasValue)
				{
					System.Diagnostics.Debug.Print ("[{0}] {1}(I{2}) Frame [{3}] Duration [{4}]", lClock.CurrentTime, Name, mInstanceNum, TimeToFrameIndex (lClock.CurrentTime.Value), GetNaturalDuration (lClock));
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
				AnimationClock lClock = sender as AnimationClock;
				System.Diagnostics.Debug.Print ("[{0}] {1}(I{2}) Completed [{3}]", lClock.CurrentTime, Name, mInstanceNum, lClock.CurrentState);
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
			System.Diagnostics.Debug.Print ("{0}(I{2}) Remove", Name, mInstanceNum);
#endif
		}
		#endregion
	}
}
