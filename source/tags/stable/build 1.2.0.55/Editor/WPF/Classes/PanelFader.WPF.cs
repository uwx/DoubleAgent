/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
using System.Windows.Controls;
using System.Windows.Media.Animation;

namespace AgentCharacterEditor
{
	public class PanelFader : IDisposable
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public PanelFader (FrameworkElement pPanel)
		{
			Panel = pPanel;
			FadeDuration = new Duration (new TimeSpan (0, 0, 0, 0, 150));
		}

		~PanelFader ()
		{
			Dispose (true);
		}

		public void Dispose ()
		{
			Dispose (false);
			GC.SuppressFinalize (this);
		}

		protected void Dispose (bool disposing)
		{
			if (FadeInStoryboard != null)
			{
				StopStoryboard (FadeInStoryboard);
				FadeInStoryboard = null;
			}
			if (FadeOutStoryboard != null)
			{
				StopStoryboard (FadeOutStoryboard);
				FadeOutStoryboard = null;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public FrameworkElement Panel
		{
			get;
			protected set;
		}

		public Boolean IsPanelVisible
		{
			get
			{
				return (Panel.Visibility == Visibility.Visible) && (Panel.Opacity > 0);
			}
			set
			{
				Storyboard lStoryboard;

				if (value)
				{
					StopStoryboard (FadeOutStoryboard);
					if (StartStoryboard (lStoryboard = GetPanelFadeIn ()))
					{
						FadeInStoryboard = lStoryboard;
					}
					else
					{
						Panel.Opacity = 1;
						Panel.Visibility = Visibility.Visible;
					}
					FadeOutStoryboard = null;
				}
				else
				{
					StopStoryboard (FadeInStoryboard);
					if (StartStoryboard (lStoryboard = GetPanelFadeOut ()))
					{
						FadeOutStoryboard = lStoryboard;
					}
					else
					{
						Panel.Visibility = Visibility.Collapsed;
					}
					FadeInStoryboard = null;
				}
			}
		}

		public Boolean IsPanelFading
		{
			get
			{
				return IsPanelFadingIn || IsPanelFadingOut;
			}
		}

		public Boolean IsPanelFadingIn
		{
			get
			{
				try
				{
					return (FadeInStoryboard != null) && (FadeInStoryboard.GetCurrentState (Panel) == ClockState.Active);
				}
				catch
				{
					return false;
				}
			}
		}

		public Boolean IsPanelFadingOut
		{
			get
			{
				try
				{
					return (FadeOutStoryboard != null) && (FadeOutStoryboard.GetCurrentState (Panel) == ClockState.Active);
				}
				catch
				{
					return false;
				}
			}
		}

		public Duration FadeDuration
		{
			get;
			set;
		}

		//=============================================================================

		public Storyboard FadeInStoryboard
		{
			get;
			set;
		}

		public Storyboard FadeOutStoryboard
		{
			get;
			set;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		public Storyboard GetPanelFadeIn ()
		{
			Storyboard lFadeIn = null;

			if (IsPanelFadingOut || (!IsPanelFadingIn && ((Panel.Visibility != Visibility.Visible) || (Panel.Opacity < 0.1))))
			{
				try
				{
					Storyboard lStoryboard = new Storyboard ();
					DoubleAnimation lOpacityAnimation = new DoubleAnimation (1.0, FadeDuration);
					ObjectAnimationUsingKeyFrames lVisibilityAnimation = new ObjectAnimationUsingKeyFrames ();

					Storyboard.SetTarget (lOpacityAnimation, Panel);
					Storyboard.SetTargetProperty (lOpacityAnimation, new PropertyPath (FrameworkElement.OpacityProperty));
					lOpacityAnimation.From = (Panel.Visibility == Visibility.Visible) ? Panel.Opacity : 0.0;
					lStoryboard.Children.Add (lOpacityAnimation);

					Storyboard.SetTarget (lVisibilityAnimation, Panel);
					Storyboard.SetTargetProperty (lVisibilityAnimation, new PropertyPath (FrameworkElement.VisibilityProperty));
					lVisibilityAnimation.KeyFrames.Add (new DiscreteObjectKeyFrame (Visibility.Visible, KeyTime.FromTimeSpan (new TimeSpan (0))));
					lStoryboard.Children.Add (lVisibilityAnimation);

					lStoryboard.FillBehavior = FillBehavior.HoldEnd;
					lStoryboard.Completed += new EventHandler (FadeInCompleted);
#if DEBUG_NOT
					lStoryboard.Name = "FadeIn";
					System.Diagnostics.Debug.Print ("[{0}] FadeIn [{1} {2}]", Panel.Name, Panel.Visibility, Panel.Opacity);
#endif
					lFadeIn = lStoryboard;
				}
				catch
				{
				}
			}
#if DEBUG_NOT
			else
			{
				System.Diagnostics.Debug.Print ("[{0}] FadeIn [{1} {2}] Skipped", Name, Visibility, Opacity);
			}
#endif
			return lFadeIn;
		}

		public Storyboard GetPanelFadeOut ()
		{
			Storyboard lFadeOut = null;

			if (IsPanelFadingIn || (!IsPanelFadingIn && (Panel.Visibility == Visibility.Visible)))
			{
				try
				{
					Storyboard lStoryboard = new Storyboard ();
					DoubleAnimation lOpacityAnimation = new DoubleAnimation (Panel.Opacity, 0.0, FadeDuration);
					ObjectAnimationUsingKeyFrames lVisibilityAnimation = new ObjectAnimationUsingKeyFrames ();

					Storyboard.SetTarget (lOpacityAnimation, Panel);
					Storyboard.SetTargetProperty (lOpacityAnimation, new PropertyPath (FrameworkElement.OpacityProperty));
					lStoryboard.Children.Add (lOpacityAnimation);

					Storyboard.SetTarget (lVisibilityAnimation, Panel);
					Storyboard.SetTargetProperty (lVisibilityAnimation, new PropertyPath (FrameworkElement.VisibilityProperty));
					lVisibilityAnimation.KeyFrames.Add (new DiscreteObjectKeyFrame (Visibility.Collapsed, KeyTime.FromTimeSpan (FadeDuration.TimeSpan)));
					lStoryboard.Children.Add (lVisibilityAnimation);

					lStoryboard.FillBehavior = FillBehavior.HoldEnd;
					lStoryboard.Completed += new EventHandler (FadeOutCompleted);
#if DEBUG_NOT
					lStoryboard.Name = "FadeOut";
					System.Diagnostics.Debug.Print ("[{0}] FadeOut [{1} {2}]", Panel.Name, Panel.Visibility, Panel.Opacity);
#endif
					lFadeOut = lStoryboard;
				}
				catch
				{
				}
			}
#if DEBUG_NOT
			else
			{
				System.Diagnostics.Debug.Print ("[{0}] FadeOut [{1} {2}] Skipped", Name, Visibility, Opacity);
			}
#endif
			return lFadeOut;
		}

		//=============================================================================

		public Boolean StartStoryboard (Storyboard pStoryboard)
		{
			return StartStoryboard (pStoryboard, Panel);
		}

		public Boolean StopStoryboard (Storyboard pStoryboard)
		{
			return StopStoryboard (pStoryboard, Panel);
		}

		static public Boolean StartStoryboard (Storyboard pStoryboard, FrameworkElement pOwner)
		{
			if (pStoryboard != null)
			{
				try
				{
					pOwner.BeginStoryboard (pStoryboard, HandoffBehavior.SnapshotAndReplace, true);
					return true;
				}
				catch
				{
				}
			}
			return false;
		}

		static public Boolean StopStoryboard (Storyboard pStoryboard, FrameworkElement pOwner)
		{
			if (pStoryboard != null)
			{
				try
				{
					pStoryboard.Stop (pOwner);
					return true;
				}
				catch
				{
				}
			}
			return false;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		void FadeInCompleted (object sender, EventArgs e)
		{
			FadeInStoryboard = null;
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("[{0}] FadeInCompleted [{1} {2}]", Panel.Name, Panel.Visibility, Panel.Opacity);
#endif
		}

		private void FadeOutCompleted (object sender, EventArgs e)
		{
			FadeOutStoryboard = null;
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("[{0}] FadeOutCompleted [{1} {2}]", Panel.Name, Panel.Visibility, Panel.Opacity);
#endif
		}

		#endregion
	}
}
