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
using System.Windows.Media;
using System.Windows.Media.Animation;
using AgentCharacterEditor.Global;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Previews
{
	public partial class AnimationPreview : UserControl
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public AnimationPreview ()
		{
			InitializeComponent ();
			RenderOptions.SetBitmapScalingMode (this, BitmapScalingMode.HighQuality);
			this.Image = new System.Windows.Media.ImageDrawing ();
			this.Image.Changed += new EventHandler (Image_ImageChanged);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public System.Windows.Media.ImageDrawing Image
		{
			get;
			protected set;
		}

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

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public AgentCharacterEditor.Previews.AnimationPreviewFrames ImageTimeline
		{
			get;
			protected set;
		}

		//=============================================================================

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

		//=============================================================================

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Double AnimationRate
		{
			get
			{
				Double lSpeedRatio = 1.0;

				if (IsPlaying)
				{
					try
					{
						lSpeedRatio *= this.MasterClock.Controller.SpeedRatio;
						//System.Diagnostics.Debug.Print ("  GetClockRatio {0}", this.MasterClock.Controller.SpeedRatio);
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
						lSpeedRatio *= this.MasterTimeline.SpeedRatio;
						//System.Diagnostics.Debug.Print ("  GetTimelineRatio {0}", this.MasterTimeline.SpeedRatio);
					}
					catch (Exception pException)
					{
						System.Diagnostics.Debug.Print (pException.Message);
					}
				}
				//System.Diagnostics.Debug.Print ("GetRatio {0} Rate {1}", lSpeedRatio, Math.Log (lSpeedRatio, 2.0));
				return Math.Log (lSpeedRatio, 2.0);
			}
			set
			{
				Double lSpeedRatio = Math.Pow (2.0, value);
				//System.Diagnostics.Debug.Print ("SetRatio {0} Rate {1}", lSpeedRatio, value);

				if (IsPlaying)
				{
					if (MasterTimeline != null)
					{
						try
						{
							//System.Diagnostics.Debug.Print ("  DivTimelineRatio {0}", this.MasterTimeline.SpeedRatio);
							lSpeedRatio /= this.MasterTimeline.SpeedRatio;
						}
						catch (Exception pException)
						{
							System.Diagnostics.Debug.Print (pException.Message);
						}
					}
					try
					{
						this.MasterClock.Controller.SpeedRatio = lSpeedRatio;
						//System.Diagnostics.Debug.Print ("  SetClockRatio {0} {1}", lSpeedRatio, this.MasterClock.Controller.SpeedRatio);
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
						this.MasterTimeline.SpeedRatio = lSpeedRatio;
						//System.Diagnostics.Debug.Print ("  SetTimelineRatio {0} {1}", lSpeedRatio, this.MasterTimeline.SpeedRatio);
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

		//=============================================================================

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

			if ((pCharacterFile != null) && (pAnimation != null))
			{
				try
				{
					MasterTimeline = new AnimationPreviewTimeline ();
					MasterTimeline.Name = pAnimation.Name;

					ImageTimeline = new AnimationPreviewFrames (pCharacterFile, pAnimation, Image);
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

					MasterTimeline.FillBehavior = FillBehavior.HoldEnd;
					MasterTimeline.SlipBehavior = SlipBehavior.Slip;
					MasterTimeline.Duration = Duration.Automatic;
					lRet = true;
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
			}

			UpdateLayout ();
			return lRet;
		}

		public Boolean CreateAnimation (CharacterFile pCharacterFile, FileAnimation pAnimation, System.Windows.Size pImageSize, Boolean pIncludeSound)
		{
			DeleteAnimation ();
			SetImageSize (pImageSize);
			return CreateAnimation (pCharacterFile, pAnimation, pIncludeSound);
		}

		public Boolean DeleteAnimation ()
		{
			Boolean lRet = false;

			StopAnimation ();

			this.ImageTimeline = null;
			this.MasterTimeline = null;
			this.MasterClock = null;

			return lRet;
		}

		//=============================================================================

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
			if (this.Image != null)
			{
				try
				{
					this.Image.ApplyAnimationClock (System.Windows.Media.ImageDrawing.ImageSourceProperty, null);
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
			}
			return lRet;
		}

		//=============================================================================

		public void ShowAnimationFrame (CharacterFile pCharacterFile, FileAnimationFrame pFrame)
		{
			StopAnimation ();

			if (pCharacterFile != null)
			{
				Image.ImageSource = AnimationPreviewFrame.MakeImageSource (pCharacterFile, pFrame);
			}
			else
			{
				Image.ImageSource = null;
			}

			UpdateLayout ();
			InvalidateVisual ();
		}

		public void ShowAnimationFrame (CharacterFile pCharacterFile, FileAnimationFrame pFrame, System.Windows.Size pImageSize)
		{
			StopAnimation ();
			SetImageSize (pImageSize);
			ShowAnimationFrame (pCharacterFile, pFrame);
		}

		private void SetImageSize (System.Windows.Size pImageSize)
		{
			Size lImageSize = new Size (Math.Min (Math.Max (pImageSize.Width, MinWidth), MaxWidth), Math.Min (Math.Max (pImageSize.Height, MinHeight), MaxHeight));
			lImageSize = lImageSize.PreserveAspectRatio (pImageSize);
			Width = lImageSize.Width;
			Height = lImageSize.Height;
			Image.Rect = new System.Windows.Rect (0, 0, lImageSize.Width, lImageSize.Height);
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("SetPreviewnImageSize [{0} {1}] Size [{2} {3}] Min [{4} {5}] Max [{6} {7}] Rect [{8} {9}]", pImageSize.Width, pImageSize.Height, Width, Height, MinWidth, MinHeight, MaxWidth, MaxHeight, Image.Rect.Width, Image.Rect.Height);
#endif
		}

		//=============================================================================

		protected override void OnRender (DrawingContext drawingContext)
		{
			base.OnRender (drawingContext);
			if (this.Image != null)
			{
				drawingContext.DrawImage (this.Image.ImageSource, this.Image.Rect);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public event EventHandler AnimationStateChanged;
		public event EventHandler AnimationImageChanged;

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void MasterClock_CurrentStateInvalidated (object sender, EventArgs e)
		{
			if (AnimationStateChanged != null)
			{
				try
				{
					AnimationStateChanged (sender, e);
				}
				catch
				{
				}
			}
		}

		private void Image_ImageChanged (object sender, EventArgs e)
		{
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("Image_ImageChanged");
#endif
			InvalidateVisual ();

			if (AnimationImageChanged != null)
			{
				try
				{
					AnimationImageChanged (sender, e);
				}
				catch
				{
				}
			}
		}

		#endregion
	}
}
