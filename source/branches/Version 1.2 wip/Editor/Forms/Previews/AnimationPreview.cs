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
using System.Windows.Media;
using System.Windows.Media.Animation;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Previews
{
	public partial class AnimationPreview : System.Windows.Forms.UserControl
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public AnimationPreview ()
		{
			InitializeComponent ();
		}

		protected override void OnHandleCreated (EventArgs e)
		{
			base.OnHandleCreated (e);

			System.Drawing.Graphics lGraphics = System.Drawing.Graphics.FromHwnd (this.Handle);
			this.WPFTarget.LayoutTransform = new ScaleTransform (96.0 / (double)lGraphics.DpiX, 96.0 / (double)lGraphics.DpiY);
			this.WPFTarget.UpdateLayout ();
			this.WPFTarget.Image.Changed += new EventHandler (WPFTarget_ImageChanged);
		}

		protected override void OnHandleDestroyed (EventArgs e)
		{
			base.OnHandleDestroyed (e);
			DeleteAnimation ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public AgentCharacterEditor.Previews.AnimationPreviewTimeline MasterTimeline
		{
			get;
			protected set;
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public System.Windows.Media.Animation.ClockGroup MasterClock
		{
			get;
			protected set;
		}

		protected AgentCharacterEditor.Previews.AnimationPreviewFrames ImageTimeline
		{
			get;
			set;
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public System.Windows.FrameworkElement AnimationTarget
		{
			get
			{
				return this.WPFTarget;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Boolean IsAnimated
		{
			get
			{
				return (MasterTimeline != null) && (ImageTimeline != null) && (ImageTimeline.KeyFrames.Count > 0);
			}
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Boolean IsPlaying
		{
			get
			{
				if (MasterClock != null)
				{
					try
					{
						return (this.MasterClock.CurrentState == ClockState.Active);
					}
					catch
					{
					}
				}
				return false;
			}
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Boolean IsPaused
		{
			get
			{
				if (MasterClock != null)
				{
					try
					{
						return this.MasterClock.IsPaused;
					}
					catch
					{
					}
				}
				return false;
			}
			set
			{
				if (MasterClock != null)
				{
					try
					{
						if (value)
						{
							this.MasterClock.Controller.Pause ();
						}
						else
						{
							this.MasterClock.Controller.Resume ();
						}
					}
					catch (Exception pException)
					{
						System.Diagnostics.Debug.Print (pException.Message);
					}
				}
			}
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Boolean IsRepeating
		{
			get
			{
				if (MasterTimeline != null)
				{
					try
					{
						return (MasterTimeline.RepeatBehavior == RepeatBehavior.Forever);
					}
					catch
					{
					}
				}
				return false;
			}
			set
			{
				if (MasterTimeline != null)
				{
					try
					{
						if (value)
						{
							MasterTimeline.RepeatBehavior = RepeatBehavior.Forever;
						}
						else
						{
							MasterTimeline.ClearValue (Timeline.RepeatBehaviorProperty);
						}
					}
					catch (Exception pException)
					{
						System.Diagnostics.Debug.Print (pException.Message);
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Double AnimationRate
		{
			get
			{
				if (MasterClock != null)
				{
					try
					{
						return Math.Log (this.MasterClock.Controller.SpeedRatio, 2.0);
					}
					catch (Exception pException)
					{
						System.Diagnostics.Debug.Print (pException.Message);
					}
				}
				else if (MasterTimeline != null)
				{
					try
					{
						return Math.Log (this.MasterTimeline.SpeedRatio, 2.0);
					}
					catch (Exception pException)
					{
						System.Diagnostics.Debug.Print (pException.Message);
					}
				}
				return 0.0;
			}
			set
			{
				if (MasterClock != null)
				{
					try
					{
						this.MasterClock.Controller.SpeedRatio = Math.Pow (2.0, value);
					}
					catch (Exception pException)
					{
						System.Diagnostics.Debug.Print (pException.Message);
					}
				}
				if (MasterTimeline != null)
				{
					try
					{
						this.MasterTimeline.SpeedRatio = Math.Pow (2.0, value);
					}
					catch (Exception pException)
					{
						System.Diagnostics.Debug.Print (pException.Message);
					}
				}
			}
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public TimeSpan? CurrentTime
		{
			get
			{
				if (MasterClock != null)
				{
					try
					{
						return this.MasterClock.CurrentTime;
					}
					catch (Exception pException)
					{
						System.Diagnostics.Debug.Print (pException.Message);
					}
				}
				return null;
			}
			set
			{
				if (MasterClock != null)
				{
					try
					{
						this.MasterClock.Controller.SeekAlignedToLastTick (value.Value, TimeSeekOrigin.BeginTime);
					}
					catch (Exception pException)
					{
						System.Diagnostics.Debug.Print (pException.Message);
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public int CurrentFrameIndex
		{
			get
			{
				int lFrameNdx = -1;

				if (IsAnimated)
				{
					TimeSpan? lTime = CurrentTime;

					if (lTime.HasValue)
					{
						lFrameNdx = ImageTimeline.TimeToFrameIndex (lTime.Value);
					}
				}
				return lFrameNdx;
			}
			set
			{
				if (IsAnimated)
				{
					TimeSpan? lTime = ImageTimeline.FrameIndexToTime (value);

					if (lTime.HasValue)
					{
						CurrentTime = lTime;
					}
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		public Boolean CreateAnimation (CharacterFile pCharacterFile, FileAnimation pAnimation)
		{
			return CreateAnimation (pCharacterFile, pAnimation, true);
		}

		public Boolean CreateAnimation (CharacterFile pCharacterFile, FileAnimation pAnimation, Boolean pIncludeSound)
		{
			Boolean lRet = false;

			DeleteAnimation ();
			InitImageTarget (pCharacterFile);

			if ((pCharacterFile != null) && (pAnimation != null))
			{
				try
				{
					MasterTimeline = new AnimationPreviewTimeline ();
					MasterTimeline.Name = pAnimation.Name;
					MasterTimeline.FillBehavior = FillBehavior.Stop;
					MasterTimeline.SlipBehavior = SlipBehavior.Grow;

					ImageTimeline = new AnimationPreviewFrames (pCharacterFile, pAnimation, WPFTarget.Image);
					MasterTimeline.Children.Add (ImageTimeline);

					if (pIncludeSound)
					{
						foreach (AnimationPreviewFrame lImageFrame in ImageTimeline.KeyFrames)
						{
							if ((lImageFrame.FileFrame != null) && (lImageFrame.FileFrame.SoundNdx >= 0))
							{
								try
								{
									AnimationPreviewSound lSound = new AnimationPreviewSound (pCharacterFile, lImageFrame.FileFrame, lImageFrame.BeginTime);
									MasterTimeline.Children.Add (lSound);
								}
								catch (Exception pException)
								{
									System.Diagnostics.Debug.Print (pException.Message);
								}
							}
						}
					}

					lRet = true;
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
			}

			WPFTarget.UpdateLayout ();
			return lRet;
		}

		public Boolean DeleteAnimation ()
		{
			Boolean lRet = false;

			StopAnimation ();
			WPFTarget.StopImageAnimation ();

			this.ImageTimeline = null;
			this.MasterTimeline = null;
			this.MasterClock = null;

			return lRet;
		}

		///////////////////////////////////////////////////////////////////////////////

		public Boolean StartAnimation ()
		{
			Boolean lRet = false;

			if (IsAnimated)
			{
#if false // Recreating the clock each time is more reliable
				if (MasterClock != null)
				{
					try
					{
						this.MasterClock.Controller.Begin ();
						lRet = true;
					}
					catch (Exception pException)
					{
						System.Diagnostics.Debug.Print (pException.Message);
					}
				}
				else
#endif
				{
					try
					{
						this.MasterClock = MasterTimeline.CreateClock (true) as ClockGroup;
						this.MasterClock.CurrentStateInvalidated += new EventHandler (MasterClock_CurrentStateInvalidated);

#if false // Let the timelines take care of this - it's more accurate for the sounds
						foreach (System.Windows.Media.Animation.Clock lClock in MasterClock.Children)
						{
							System.Windows.Media.Animation.AnimationClock lAnimationClock = lClock as System.Windows.Media.Animation.AnimationClock;
							System.Windows.Media.MediaClock lMediaClock = lClock as System.Windows.Media.MediaClock;

							if (lAnimationClock != null)
							{
								AnimationPreviewFrames lFrames = lAnimationClock.Timeline as AnimationPreviewFrames;
								if (lFrames != null)
								{
									lFrames.Target.ApplyAnimationClock (System.Windows.Media.ImageDrawing.ImageSourceProperty, lAnimationClock);
								}
							}
							if (lMediaClock != null)
							{
								AnimationPreviewSound lSound = lMediaClock.Timeline as AnimationPreviewSound;
								if (lSound != null)
								{
									lSound.Player.Clock = lMediaClock;
								}
							}
						}
#endif
						lRet = true;
					}
					catch (Exception pException)
					{
						System.Diagnostics.Debug.Print (pException.Message);
					}
				}
			}
			return lRet;
		}

		public Boolean StopAnimation ()
		{
			Boolean lRet = false;

			if (MasterClock != null)
			{
				try
				{
					this.MasterClock.Controller.Stop ();
					lRet = true;
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
			}
#if true
			MasterClock = null;
#endif

			return lRet;
		}

		///////////////////////////////////////////////////////////////////////////////

		public void ShowAnimationFrame (CharacterFile pCharacterFile, FileAnimationFrame pFrame)
		{
			StopAnimation ();
			InitImageTarget (pCharacterFile);

			if (pCharacterFile != null)
			{
				WPFTarget.Image.ImageSource = AnimationPreviewFrame.MakeImageSource (pCharacterFile, pFrame);
			}
			else
			{
				WPFTarget.Image.ImageSource = null;
			}

			WPFTarget.UpdateLayout ();
			WPFTarget.InvalidateVisual ();
		}

		private void InitImageTarget (CharacterFile pCharacterFile)
		{
			WPFTarget.StopImageAnimation ();

			if (pCharacterFile != null)
			{
				WPFHost.Size = pCharacterFile.Header.ImageSize;
			}
			else
			{
				WPFHost.Size = FrameSample.DefaultImageSize;
			}

			WPFTarget.Image.Rect = new System.Windows.Rect (0, 0, WPFHost.Size.Width, WPFHost.Size.Height);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public event EventHandler AnimationStateChanged;
		public event EventHandler AnimationImageChanged;

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region EventHandlers

		private void MasterClock_CurrentStateInvalidated (object sender, EventArgs e)
		{
			if (AnimationStateChanged != null)
			{
				try
				{
					AnimationStateChanged (this, e);
				}
				catch
				{
				}
			}
		}

		private void WPFTarget_ImageChanged (object sender, EventArgs e)
		{
			if (AnimationImageChanged != null)
			{
				try
				{
					AnimationImageChanged (this, e);
				}
				catch
				{
				}
			}
		}

		#endregion
	}
}
