using System;
using System.Windows.Media.Animation;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Previews
{
	public class AnimationPreviewAnimation : ObjectAnimationUsingKeyFrames
	{
		public AnimationPreviewAnimation (CharacterFile pCharacterFile, FileAnimation pFileAnimation)
		{
			TimeSpan lStartTime = new TimeSpan ();
			FileAnimationFrame lLastFrame = null;

#if DEBUG
			System.Diagnostics.Debug.Print ("{0} [{1}] frames", pFileAnimation.Name, pFileAnimation.FrameCount);
#endif
			Name = pFileAnimation.Name;
			FillBehavior = FillBehavior.HoldEnd;

			foreach (FileAnimationFrame lFileFrame in pFileAnimation.Frames)
			{
#if DEBUG
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

#if DEBUG
			CurrentStateInvalidated += new EventHandler (OnCurrentStateInvalidated);
			CurrentTimeInvalidated += new EventHandler (OnCurrentTimeInvalidated);
			Completed += new EventHandler (OnCompleted);
#endif
		}

		///////////////////////////////////////////////////////////////////////////////

		public TimeSpan? TimeFromFrame (FileAnimationFrame pFrame)
		{
			if (pFrame != null)
			{
				foreach (AnimationPreviewFrame lKeyFrame in KeyFrames)
				{
					if (Object.ReferenceEquals (lKeyFrame.FileFrame, pFrame))
					{
						return lKeyFrame.KeyTime.TimeSpan;
					}
				}
			}
			return null;
		}

		public TimeSpan? TimeFromFrameIndex (int pFrameIndex, FileAnimation pFileAnimation)
		{
			if ((pFileAnimation != null) && (pFrameIndex >= 0) && (pFrameIndex < pFileAnimation.FrameCount))
			{
				return TimeFromFrame (pFileAnimation.Frames[pFrameIndex]);
			}
			return null;
		}

		///////////////////////////////////////////////////////////////////////////////

		public FileAnimationFrame FrameFromTime (TimeSpan pTime)
		{
			FileAnimationFrame lRet = null;

			foreach (AnimationPreviewFrame lKeyFrame in KeyFrames)
			{
				if (lKeyFrame.KeyTime.TimeSpan <= pTime)
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

		public int FrameIndexFromTime (TimeSpan pTime)
		{
			FileAnimationFrame lFrame = FrameFromTime (pTime);

			if ((lFrame != null) && (lFrame.Container != null) && lFrame.Container.Contains (lFrame))
			{
				return lFrame.Container.IndexOf (lFrame);
			}
			return -1;
		}

		public int FrameIndexFromTime (TimeSpan pTime, FileAnimation pFileAnimation)
		{
			if ((pFileAnimation != null) && (pFileAnimation.Frames != null))
			{
				FileAnimationFrame lFrame = FrameFromTime (pTime);

				if ((lFrame != null) && pFileAnimation.Frames.Contains (lFrame))
				{
					return pFileAnimation.Frames.IndexOf (lFrame);
				}
			}
			return -1;
		}

		///////////////////////////////////////////////////////////////////////////////

		public FileAnimationFrame FrameFromTime (TimeSpan? pTime)
		{
			return (pTime.HasValue) ? FrameFromTime (pTime.Value) : null;
		}

		public int FrameIndexFromTime (TimeSpan? pTime)
		{
			return (pTime.HasValue) ? FrameIndexFromTime (pTime.Value) : -1;
		}

		public int FrameIndexFromTime (TimeSpan? pTime, FileAnimation pFileAnimation)
		{
			return (pTime.HasValue) ? FrameIndexFromTime (pTime.Value, pFileAnimation) : -1;
		}

		///////////////////////////////////////////////////////////////////////////////

#if DEBUG
		private void OnCurrentStateInvalidated (object sender, EventArgs e)
		{
			System.Windows.Media.Animation.AnimationClock lClock = sender as System.Windows.Media.Animation.AnimationClock;
			//System.Diagnostics.Debug.Print ("[{0}] OnCurrentStateInvalidated [{1}] Time [{2} of {3}] Progress [{4}] Iteration [{5}]", Name, lClock.CurrentState, lClock.CurrentTime, GetNaturalDuration (lClock), lClock.CurrentProgress, lClock.CurrentIteration);
			System.Diagnostics.Debug.Print ("{0} {1}", Name, lClock.CurrentState);
		}

		private void OnCurrentTimeInvalidated (object sender, EventArgs e)
		{
			System.Windows.Media.Animation.AnimationClock lClock = sender as System.Windows.Media.Animation.AnimationClock;
			//System.Diagnostics.Debug.Print ("[{0}] OnCurrentTimeInvalidated  [{1}] Time [{2} of {3}] Progress [{4}] Iteration [{5}]", Name, lClock.CurrentState, lClock.CurrentTime, GetNaturalDuration (lClock), lClock.CurrentProgress, lClock.CurrentIteration);
			if (lClock.CurrentTime.HasValue)
			{
				System.Diagnostics.Debug.Print ("{0} Frame {1} at {2}", Name, FrameIndexFromTime(lClock.CurrentTime.Value), lClock.CurrentTime);
			}
		}

		private void OnCompleted (object sender, EventArgs e)
		{
			System.Windows.Media.Animation.AnimationClock lClock = sender as System.Windows.Media.Animation.AnimationClock;
			//System.Diagnostics.Debug.Print ("[{0}] OnCompleted               [{1}] Time [{2} of {3}] Progress [{4}] Iteration [{5}]", Name, lClock.CurrentState, lClock.CurrentTime, GetNaturalDuration (lClock), lClock.CurrentProgress, lClock.CurrentIteration);
			System.Diagnostics.Debug.Print ("{0} Completed", Name);
		}
#endif
	}
}
