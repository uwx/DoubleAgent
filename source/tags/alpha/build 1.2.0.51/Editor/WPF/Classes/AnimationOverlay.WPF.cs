#if DEBUG
//#define	DEBUG_ANIMATIONS
#endif
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
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using DoubleAgent;

namespace AgentCharacterEditor
{
	internal class AnimationOverlay
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public AnimationOverlay (FrameworkElement pContentElement, Canvas pOverlayElement)
		{
			ContentElement = pContentElement;
			OverlayElement = pOverlayElement;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public FrameworkElement ContentElement
		{
			get;
			private set;
		}
		public Canvas OverlayElement
		{
			get;
			private set;
		}
		public Rectangle BackgroundElement
		{
			get;
			private set;
		}

		public Storyboard Animation
		{
			get;
			private set;
		}
		public Clock AnimationClock
		{
			get;
			private set;
		}

		///////////////////////////////////////////////////////////////////////////////

		private class PartAnimation
		{
			public Storyboard Animation = null;
			public Boolean Enabled = false;
		}
		private List<PartAnimation> mPartAnimations = new List<PartAnimation> ();

		//=============================================================================

		public int SetPartAnimation (int pIndex, Storyboard pAnimation)
		{
			PartAnimation lPartAnimation = new PartAnimation ();

			InitOverlayAnimation (pAnimation);
			lPartAnimation.Animation = pAnimation;

			if (pIndex < mPartAnimations.Count)
			{
				mPartAnimations.Insert (pIndex, lPartAnimation);
				return pIndex;
			}
			else
			{
				mPartAnimations.Add (lPartAnimation);
				return mPartAnimations.Count - 1;
			}
		}
		public Storyboard GetPartAnimation (int pIndex)
		{
			if ((pIndex >= 0) && (pIndex < mPartAnimations.Count))
			{
				return mPartAnimations[pIndex].Animation;
			}
			return null;
		}
		IEnumerable<Storyboard> PartAnimations ()
		{
			foreach (PartAnimation lPartAnimation in mPartAnimations)
			{
				yield return lPartAnimation.Animation;
			}
		}
		IEnumerable<Storyboard> EnabledPartAnimations ()
		{
			foreach (PartAnimation lPartAnimation in mPartAnimations)
			{
				if (lPartAnimation.Enabled)
				{
					yield return lPartAnimation.Animation;
				}
			}
		}

		public Boolean SetPartEnabled (int pIndex, Boolean pEnabled)
		{
			if ((pIndex >= 0) && (pIndex < mPartAnimations.Count))
			{
				mPartAnimations[pIndex].Enabled = pEnabled;
				return true;
			}
			return false;
		}
		public Boolean GetPartEnabled (int pIndex)
		{
			if ((pIndex >= 0) && (pIndex < mPartAnimations.Count))
			{
				return mPartAnimations[pIndex].Enabled;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////

		public static Duration DefaultDuration
		{
			get
			{
				return new Duration (TimeSpan.FromSeconds (5));
			}
		}
		public static Double DefaultStartRest
		{
			get
			{
				return 0.2;
			}
		}
		public static Double DefaultEndRest
		{
			get
			{
				return 0.4;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		public Boolean PrepareOverlay (Rectangle pBackgroundElement)
		{
			if ((OverlayElement.Children.Count <= 0) && (ContentElement.ActualWidth > 0) && (ContentElement.ActualHeight > 0))
			{
				BackgroundElement = MakeAnimationBackground (pBackgroundElement);
				return true;
			}
			return false;
		}

		public Boolean CompleteOverlay (Storyboard pAnimation)
		{
			if ((pAnimation != null) && (Animation == null))
			{
				InitOverlayAnimation (pAnimation);
				pAnimation.RepeatBehavior = RepeatBehavior.Forever;
				pAnimation.Name = String.Format ("{0}_A", ContentElement.Name);
				Animation = pAnimation;
				return true;
			}
			return false;
		}

		private void InitOverlayAnimation (Storyboard pAnimation)
		{
			pAnimation.FillBehavior = FillBehavior.Stop;
			pAnimation.AutoReverse = false;
#if false
			pAnimation.AccelerationRatio = 0.25;
			pAnimation.DecelerationRatio = 0.25;
#endif
		}

		//=============================================================================

		public virtual Boolean StartOverlayAnimation ()
		{
			StopOverlayAnimation ();

			if (Animation != null)
			{
				try
				{
					Clock lClock;
#if DEBUG_ANIMATIONS
					System.Diagnostics.Debug.Print ("Start Animation {0}", Animation.Name);
#endif
					foreach (PartAnimation lPartAnimation in mPartAnimations)
					{
						if (lPartAnimation.Enabled)
						{
							if (!Animation.Children.Contains (lPartAnimation.Animation))
							{
								Animation.Children.Add (lPartAnimation.Animation);
							}
						}
						else
						{
							if (Animation.Children.Contains (lPartAnimation.Animation))
							{
								Animation.Children.Remove (lPartAnimation.Animation);
							}
						}
					}

					lClock = Animation.CreateClock (true);
					Animation.Begin (OverlayElement, HandoffBehavior.SnapshotAndReplace, true);
					AnimationClock = lClock;
					return true;
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
			}
			return false;
		}

		//=============================================================================

		public void StopOverlayAnimation ()
		{
			try
			{
				if (Animation != null)
				{
#if DEBUG_ANIMATIONS
					System.Diagnostics.Debug.Print ("Stop Animation {0}", Animation.Name);
#endif
					Animation.Stop (OverlayElement);
				}
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}

			try
			{
				if ((AnimationClock != null) && (AnimationClock.Controller != null))
				{
					AnimationClock.Controller.Remove ();
				}
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
			AnimationClock = null;
		}

		public void PauseOverlayAnimation ()
		{
			try
			{
				if (Animation != null)
				{
#if DEBUG_ANIMATIONS
					System.Diagnostics.Debug.Print ("Pause Animation {0}", Animation.Name);
#endif
					Animation.Pause (OverlayElement);
				}
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
		}

		public void RemoveOverlayAnimation ()
		{
			try
			{
				if ((AnimationClock != null) && (AnimationClock.Controller != null))
				{
#if DEBUG_ANIMATIONS
					System.Diagnostics.Debug.Print ("Remove Animation {0}", Animation.Name);
#endif
					AnimationClock.Controller.Remove ();
				}
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
			AnimationClock = null;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Implementation

		public Rectangle MakeAnimationBackground (Rectangle pBackgroundElement)
		{
			Rectangle lBackgroundElement = MakeAnimationElement (pBackgroundElement);

			if (lBackgroundElement != null)
			{
				lBackgroundElement.Style = pBackgroundElement.Style;
				lBackgroundElement.Fill = pBackgroundElement.Fill;
				lBackgroundElement.Stroke = pBackgroundElement.Stroke;
				lBackgroundElement.StrokeThickness = pBackgroundElement.StrokeThickness;
			}
			return lBackgroundElement;
		}

		public Rectangle MakeAnimationVisual (FrameworkElement pSourceElement)
		{
			Rectangle lAnimationVisual = MakeAnimationElement (pSourceElement);

			if (lAnimationVisual != null)
			{
				lAnimationVisual.Fill = new VisualBrush (pSourceElement);
			}
			return lAnimationVisual;
		}

		public Rectangle MakeAnimationElement (FrameworkElement pSourceElement)
		{
			Rectangle lAnimationElement = null;

			if (pSourceElement != null)
			{
				lAnimationElement = new Rectangle ();

				lAnimationElement.Name = String.Format ("{0}_A", pSourceElement.Name);
				lAnimationElement.SnapsToDevicePixels = false;
				lAnimationElement.UseLayoutRounding = pSourceElement.UseLayoutRounding;

				LayoutAnimationElement (pSourceElement, lAnimationElement);
				pSourceElement.LayoutUpdated += (object sender, EventArgs e) =>
				{
					LayoutAnimationElement (pSourceElement, lAnimationElement);
				};
				OverlayElement.Children.Add (lAnimationElement);
			}
			return lAnimationElement;
		}

		private void LayoutAnimationElement (FrameworkElement pSourceElement, Rectangle pAnimationElement)
		{
			Point lOrigin = pSourceElement.TranslatePoint (new Point (0, 0), OverlayElement);

			pAnimationElement.Width = pSourceElement.ActualWidth;
			pAnimationElement.Height = pSourceElement.ActualHeight;
			Canvas.SetLeft (pAnimationElement, lOrigin.X);
			Canvas.SetTop (pAnimationElement, lOrigin.Y);
		}

		//=============================================================================

		public AnimationTimeline MakeSwapAnimation (Rectangle pAnimationVisual, FrameworkElement pTargetElement)
		{
			return MakeSwapAnimation (pAnimationVisual, pTargetElement, DefaultStartRest);
		}
		public AnimationTimeline MakeSwapAnimation (Rectangle pAnimationVisual, FrameworkElement pTargetElement, Double? pStartRest)
		{
			return MakeSwapAnimation (pAnimationVisual, pTargetElement, pStartRest, DefaultEndRest);
		}
		public AnimationTimeline MakeSwapAnimation (Rectangle pAnimationVisual, FrameworkElement pTargetElement, Double? pStartRest, Double? pEndRest)
		{
			try
			{
				Double lOrigin = Canvas.GetLeft (pAnimationVisual);
				Point lDestination = pTargetElement.TranslatePoint (new Point (0, 0), OverlayElement);
				DoubleAnimationOverride lAnimation = new DoubleAnimationOverride (lOrigin, lDestination.X);

				lAnimation.Duration = DefaultDuration;
				lAnimation.InitializeRewind (pStartRest, pEndRest);

#if DEBUG_NOT
				lAnimation.CurrentTimeInvalidated += (object sender, EventArgs e) =>
				{
					try
					{
						Clock lClock = sender as Clock;
						Clock lParent;
						for (lParent = lClock; lParent.Parent != null; lParent = lParent.Parent)
						{
						}
						System.Diagnostics.Debug.Print ("{0} TimeInvalidated [{1} ({2})] [{3:F4} {4}] [{5:F4} {6}]", pAnimationVisual.Name, Canvas.GetLeft (pAnimationVisual), lAnimation.GetCurrentValue (lOrigin, lDestination.X, lClock as AnimationClock), lClock.CurrentProgress, lClock.CurrentIteration, lParent.CurrentProgress, lParent.CurrentIteration);
					}
					catch
					{
					}
				};
#endif
#if DEBUG_NOT
				lAnimation.CurrentStateInvalidated += (object sender, EventArgs e) =>
				{
					try
					{
						Clock lClock = sender as Clock;
						System.Diagnostics.Debug.Print ("{0} StateInvalidated [{1}] [{2} ({3})] in [{4} to [{5}]", pAnimationVisual.Name, lClock.CurrentState, Canvas.GetLeft (pAnimationVisual), lAnimation.GetCurrentValue (lOrigin, lDestination.X, lClock as AnimationClock), lOrigin, lDestination.X);
					}
					catch
					{
					}
				};
#endif
#if DEBUG_NOT
				lAnimation.Completed += (object sender, EventArgs e) =>
				{
					try
					{
						Clock lClock = sender as Clock;
						System.Diagnostics.Debug.Print ("{0} Completed [{1}] ({2})]", pAnimationVisual.Name, Canvas.GetLeft (pAnimationVisual), lAnimation.GetCurrentValue (lOrigin, lDestination.X, lClock as AnimationClock));
					}
					catch
					{
					}
				};
#endif

				Storyboard.SetTarget (lAnimation, pAnimationVisual);
				Storyboard.SetTargetProperty (lAnimation, new PropertyPath (Canvas.LeftProperty));
				return lAnimation;
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
			return null;
		}

#if false
		public AnimationTimeline MakeDoubleAnimation (Rectangle pAnimationVisual, DependencyProperty pProperty, Double pMidPoint)
		{
			try
			{
				DoubleAnimationUsingKeyFrames lAnimation = new DoubleAnimationUsingKeyFrames ();
				Double lValue = (Double)pAnimationVisual.GetValue (pProperty);

				lAnimation.KeyFrames.Add (new DiscreteDoubleKeyFrame (lValue, KeyTime.FromPercent (0)));
				lAnimation.KeyFrames.Add (new SplineDoubleKeyFrame (lValue + pMidPoint, KeyTime.FromPercent (0.5), new KeySpline (0.0, 0.8, 0.2, 1.0)));
				lAnimation.KeyFrames.Add (new SplineDoubleKeyFrame (lValue, KeyTime.FromPercent (1.0), new KeySpline (0.8, 0.0, 1.0, 0.2)));
				lAnimation.Duration = DefaultDuration;

				Storyboard.SetTarget (lAnimation, pAnimationVisual);
				Storyboard.SetTargetProperty (lAnimation, new PropertyPath (pProperty));
				return lAnimation;
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
			return null;
		}
#endif

		//=============================================================================

		public AnimationTimeline MakeFadeAnimation (FrameworkElement pText, Boolean pFadeIn)
		{
			return MakeFadeAnimation (pText, pFadeIn, DefaultStartRest);
		}
		public AnimationTimeline MakeFadeAnimation (FrameworkElement pText, Boolean pFadeIn, Double? pStartRest)
		{
			return MakeFadeAnimation (pText, pFadeIn, pStartRest, DefaultEndRest);
		}
		public AnimationTimeline MakeFadeAnimation (FrameworkElement pText, Boolean pFadeIn, Double? pStartRest, Double? pEndRest)
		{
			if (pFadeIn)
			{
				return MakeFadeAnimation (pText, 0.0, 1.0, pStartRest, pEndRest);
			}
			else
			{
				return MakeFadeAnimation (pText, 1.0, 0.0, pStartRest, pEndRest);
			}
		}
		public AnimationTimeline MakeFadeAnimation (FrameworkElement pText, Double pFrom, Double pTo, Double? pStartRest, Double? pEndRest)
		{
			try
			{
				if ((pStartRest.HasValue && pStartRest.Value > 0) || (pEndRest.HasValue && pEndRest.Value > 0))
				{
					DoubleAnimationUsingKeyFrames lAnimation = new DoubleAnimationUsingKeyFrames ();

					lAnimation.Duration = DefaultDuration;
					lAnimation.FillBehavior = FillBehavior.Stop;
					lAnimation.KeyFrames.Add (new DiscreteDoubleKeyFrame (pFrom, KeyTime.FromPercent (0)));
					if (pStartRest.HasValue && pStartRest.Value > 0)
					{
						lAnimation.KeyFrames.Add (new DiscreteDoubleKeyFrame (pFrom, KeyTime.FromPercent (pStartRest.Value)));
					}
					if (pEndRest.HasValue && pEndRest.Value > 0)
					{
						lAnimation.KeyFrames.Add (new LinearDoubleKeyFrame (pTo, KeyTime.FromPercent (1.0 - pEndRest.Value)));
					}
					lAnimation.KeyFrames.Add (new LinearDoubleKeyFrame (pTo, KeyTime.FromPercent (1.0)));

					Storyboard.SetTarget (lAnimation, pText);
					Storyboard.SetTargetProperty (lAnimation, new PropertyPath (FrameworkElement.OpacityProperty));
					return lAnimation;
				}
				else
				{
					DoubleAnimation lAnimation = new DoubleAnimation ();

					lAnimation.Duration = DefaultDuration;
					lAnimation.FillBehavior = FillBehavior.Stop;
					lAnimation.From = pFrom;
					lAnimation.To = pTo;

					Storyboard.SetTarget (lAnimation, pText);
					Storyboard.SetTargetProperty (lAnimation, new PropertyPath (FrameworkElement.OpacityProperty));
					return lAnimation;
				}
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
			return null;
		}

		//=============================================================================

		public AnimationTimeline MakeFlashAnimation (FrameworkElement pAnimationVisual)
		{
			try
			{
				DoubleAnimationUsingKeyFrames lAnimation = new DoubleAnimationUsingKeyFrames ();
				TimeSpan lDuration = DefaultDuration.TimeSpan;
				TimeSpan lRestTime = TimeSpan.FromSeconds ((lDuration.TotalSeconds * DefaultStartRest) - 0.3);

				lAnimation.Duration = DefaultDuration;
				lAnimation.FillBehavior = FillBehavior.Stop;
				lAnimation.KeyFrames.Add (new DiscreteDoubleKeyFrame (0));
				lAnimation.KeyFrames.Add (new DiscreteDoubleKeyFrame (0, KeyTime.FromTimeSpan (lRestTime)));
				lAnimation.KeyFrames.Add (new LinearDoubleKeyFrame (1, KeyTime.FromTimeSpan (TimeSpan.FromSeconds (lRestTime.TotalSeconds + 0.1))));
				lAnimation.KeyFrames.Add (new DiscreteDoubleKeyFrame (1, KeyTime.FromTimeSpan (TimeSpan.FromSeconds (lRestTime.TotalSeconds + 0.3))));
				lAnimation.KeyFrames.Add (new LinearDoubleKeyFrame (0, KeyTime.FromTimeSpan (TimeSpan.FromSeconds (lRestTime.TotalSeconds + 0.8))));
				lAnimation.KeyFrames.Add (new DiscreteDoubleKeyFrame (0, KeyTime.FromTimeSpan (DefaultDuration.TimeSpan)));
#if DEBUG_NOT
				lAnimation.DebugPrint ();
				lAnimation.CurrentTimeInvalidated += (object sender, EventArgs e) =>
				{
					try
					{
						Clock lClock = sender as Clock;
						System.Diagnostics.Debug.Print ("{0} TimeInvalidated [{1:0.000} ({2:0.000})] [{3} {4:0.000} {5}]", pAnimationVisual.Name, pAnimationVisual.Opacity, lAnimation.GetCurrentValue (0, 1, lClock as AnimationClock), lClock.CurrentTime, lClock.CurrentProgress, lClock.CurrentIteration);
					}
					catch
					{
					}
				};
#endif

				Storyboard.SetTarget (lAnimation, pAnimationVisual);
				Storyboard.SetTargetProperty (lAnimation, new PropertyPath (FrameworkElement.OpacityProperty));
				return lAnimation;
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
			return null;
		}

		//=============================================================================

#if DEBUG
		public void DumpAnimations ()
		{
			try
			{
				if (Animation != null)
				{
					Animation.DebugPrint (String.Format ("{0} Animation", OverlayElement.Name), String.Empty);
				}
				foreach (PartAnimation lPartAnimation in mPartAnimations)
				{
					if (lPartAnimation.Animation != null)
					{
						lPartAnimation.Animation.DebugPrint (String.Format ("{0}   Part", OverlayElement.Name), String.Empty);
					}
				}
			}
			catch
			{
			}
		}

		public void DumpClocks ()
		{
			try
			{
				if (AnimationClock != null)
				{
					AnimationClock.DebugPrint (String.Format ("{0} Clock", OverlayElement.Name), String.Empty);
				}
			}
			catch
			{
			}
		}
#endif
		#endregion
	}

	/////////////////////////////////////////////////////////////////////////////
	//
	//	Wrapper for DoubleAnimationUsingKeyFrames.  This should just be a subclass
	//	but the buggers have sealed all the useful methods.
	//
	//	The purpose of this class is to allow the first repeat iteration to
	//	pick up where any previous animation left off (i.e. animate from the 
	//	current position) but force any repeats to animate from the default
	//	origin.
	//
	/////////////////////////////////////////////////////////////////////////////

	[ContentProperty ("KeyFrames")]
	internal class DoubleAnimationOverride : DoubleAnimationBase, IKeyFrameAnimation, IAddChild
	{
		/////////////////////////////////////////////////////////////////////////////
		#region Initialization

		DoubleAnimationUsingKeyFrames mAnimation = new DoubleAnimationUsingKeyFrames ();
		Double mOrigin = 0;
		Double mDestination = 0;
		Boolean mRepeatRewind = false;

		public DoubleAnimationOverride (Double pOrigin, Double pDestination)
		{
			mOrigin = pOrigin;
			mDestination = pDestination;
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("DoubleAnimationOverride [{0} {1}]", mOrigin, mDestination);
#endif
		}
		protected DoubleAnimationOverride ()
		{
		}

		#endregion
		/////////////////////////////////////////////////////////////////////////////
		#region Properties

		public DoubleAnimationUsingKeyFrames InnerAnimation
		{
			get
			{
				return mAnimation;
			}
		}

		public DoubleKeyFrameCollection KeyFrames
		{
			get
			{
				return mAnimation.KeyFrames;
			}
			set
			{
				mAnimation.KeyFrames = value;
			}
		}

		System.Collections.IList IKeyFrameAnimation.KeyFrames
		{
			get
			{
				return mAnimation.KeyFrames;
			}
			set
			{
				mAnimation.KeyFrames = value as DoubleKeyFrameCollection;
			}
		}

		public override bool IsDestinationDefault
		{
			get
			{
				return mAnimation.IsDestinationDefault;
			}
		}

		public static Double RewindRatio
		{
			get
			{
				return 0.025;
			}
		}

		#endregion
		/////////////////////////////////////////////////////////////////////////////
		#region Infrastructure

		public virtual void AddChild (object child)
		{
		}
		public virtual void AddText (string childText)
		{
		}
		public bool ShouldSerializeKeyFrames ()
		{
			return mAnimation.ShouldSerializeKeyFrames ();
		}

		//=============================================================================

		public new DoubleAnimationOverride Clone ()
		{
			DoubleAnimationOverride lClone = CreateInstanceCore () as DoubleAnimationOverride;
			lClone.CloneCore (this);
			return lClone;
		}
		protected override void CloneCore (Freezable sourceFreezable)
		{
			base.CloneCore (sourceFreezable);
			if (sourceFreezable is DoubleAnimationOverride)
			{
				DoubleAnimationOverride lSource = (sourceFreezable as DoubleAnimationOverride);
				this.Duration = lSource.Duration;
				this.mOrigin = lSource.mOrigin;
				this.mDestination = lSource.mDestination;
				this.mRepeatRewind = lSource.mRepeatRewind;
				this.mAnimation = lSource.mAnimation.Clone ();
			}
		}
		public new DoubleAnimationOverride CloneCurrentValue ()
		{
			DoubleAnimationOverride lClone = CreateInstanceCore () as DoubleAnimationOverride;
			lClone.CloneCurrentValueCore (this);
			return lClone;
		}
		protected override void CloneCurrentValueCore (Freezable sourceFreezable)
		{
			base.CloneCurrentValueCore (sourceFreezable);
			if (sourceFreezable is DoubleAnimationOverride)
			{
				DoubleAnimationOverride lSource = (sourceFreezable as DoubleAnimationOverride);
				this.Duration = lSource.Duration;
				this.mOrigin = lSource.mOrigin;
				this.mDestination = lSource.mDestination;
				this.mRepeatRewind = lSource.mRepeatRewind;
				this.mAnimation = lSource.mAnimation.CloneCurrentValue ();
			}
		}
		protected override Freezable CreateInstanceCore ()
		{
			return new DoubleAnimationOverride ();
		}
		protected override bool FreezeCore (bool isChecking)
		{
			if (!isChecking && !mAnimation.IsFrozen)
			{
				mAnimation.Duration = Duration;
			}
			return base.FreezeCore (isChecking);
		}
		protected override void GetAsFrozenCore (Freezable source)
		{
			base.GetAsFrozenCore (source);
			if (source is DoubleAnimationOverride)
			{
				DoubleAnimationOverride lSource = (source as DoubleAnimationOverride);
				this.Duration = lSource.Duration;
				this.mOrigin = lSource.mOrigin;
				this.mDestination = lSource.mDestination;
				this.mRepeatRewind = lSource.mRepeatRewind;
				this.mAnimation = lSource.mAnimation.GetAsFrozen () as DoubleAnimationUsingKeyFrames;
			}
		}
		protected override void GetCurrentValueAsFrozenCore (Freezable source)
		{
			base.GetCurrentValueAsFrozenCore (source);
			if (source is DoubleAnimationOverride)
			{
				DoubleAnimationOverride lSource = (source as DoubleAnimationOverride);
				this.Duration = lSource.Duration;
				this.mOrigin = lSource.mOrigin;
				this.mDestination = lSource.mDestination;
				this.mRepeatRewind = lSource.mRepeatRewind;
				this.mAnimation = lSource.mAnimation.GetCurrentValueAsFrozen () as DoubleAnimationUsingKeyFrames;
			}
		}

		//=============================================================================

		protected override void OnPropertyChanged (DependencyPropertyChangedEventArgs e)
		{
			base.OnPropertyChanged (e);

			if (e.Property == DurationProperty)
			{
				if (!mAnimation.IsFrozen)
				{
					mAnimation.Duration = Duration;
				}
			}
			else if (e.Property == FillBehaviorProperty)
			{
				if (!mAnimation.IsFrozen)
				{
					mAnimation.FillBehavior = FillBehavior;
				}
			}
			else if (e.Property == RepeatBehaviorProperty)
			{
				if (!mAnimation.IsFrozen)
				{
					mAnimation.RepeatBehavior = RepeatBehavior;
				}
			}
			else if (e.Property == AutoReverseProperty)
			{
				if (!mAnimation.IsFrozen)
				{
					mAnimation.AutoReverse = AutoReverse;
				}
			}
			else if (e.Property == Storyboard.TargetProperty)
			{
				if (!mAnimation.IsFrozen)
				{
					Storyboard.SetTarget (mAnimation, Storyboard.GetTarget (this));
				}
			}
			else if (e.Property == Storyboard.TargetNameProperty)
			{
				if (!mAnimation.IsFrozen)
				{
					Storyboard.SetTargetName (mAnimation, Storyboard.GetTargetName (this));
				}
			}
		}

		#endregion
		/////////////////////////////////////////////////////////////////////////////
		#region Implementation

		public void InitializeRewind ()
		{
			InitializeRewind (null, null);
		}
		public void InitializeRewind (Double? pStartRest)
		{
			InitializeRewind (pStartRest, null);
		}
		public void InitializeRewind (Double? pStartRest, Double? pEndRest)
		{
			InitializeRewind (pStartRest, pEndRest, false);
		}
		public void InitializeRewind (Double? pStartRest, Double? pEndRest, Boolean pRepeatRewind)
		{
			mRepeatRewind = pRepeatRewind;
			FillBehavior = FillBehavior.Stop;
			KeyFrames.Add (new LinearDoubleKeyFrame (mOrigin, KeyTime.FromPercent (RewindRatio)));

			if ((pStartRest.HasValue) && (pStartRest.Value > RewindRatio))
			{
				KeyFrames.Add (new DiscreteDoubleKeyFrame (mOrigin, KeyTime.FromPercent (pStartRest.Value)));
			}
			if ((pEndRest.HasValue) && (pEndRest.Value > 0))
			{
				KeyFrames.Add (new LinearDoubleKeyFrame (mDestination, KeyTime.FromPercent (1.0 - pEndRest.Value)));
			}
			KeyFrames.Add (new LinearDoubleKeyFrame (mDestination, KeyTime.FromPercent (1.0)));
		}

		protected override Duration GetNaturalDurationCore (Clock clock)
		{
			return Duration;
		}

		protected override double GetCurrentValueCore (double defaultOriginValue, double defaultDestinationValue, AnimationClock animationClock)
		{
			double lRet;
			Clock lMasterClock;

			for (lMasterClock = animationClock; lMasterClock.Parent != null; lMasterClock = lMasterClock.Parent)
			{
			}
			if (lMasterClock.CurrentIteration > 1)
			{
				if (mRepeatRewind)
				{
					defaultOriginValue = mDestination;
					defaultDestinationValue = mOrigin;
				}
				else
				{
					defaultOriginValue = mOrigin;
					defaultDestinationValue = mDestination;
				}
			}
			lRet = mAnimation.GetCurrentValue (defaultOriginValue, defaultDestinationValue, animationClock);
#if DEBUG_NOT
			try
			{
				System.Diagnostics.Debug.Print ("GCV [{0:000.0000}] [{1:000.0000} {2:000.0000}] [{3} {4::000.0000} {5}] [{6} {7::000.0000} {8}]", lRet, defaultOriginValue, defaultDestinationValue, animationClock.CurrentTime, animationClock.CurrentProgress, animationClock.CurrentIteration, lMasterClock.CurrentTime, lMasterClock.CurrentProgress, lMasterClock.CurrentIteration);
			}
			catch
			{
			}
#endif
			return lRet;
		}

		#endregion
	}
}
