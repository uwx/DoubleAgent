using System;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Previews
{
	public class AnimationPreview : System.Windows.Forms.Integration.ElementHost
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public AnimationPreview ()
		{
			this.Margin = new System.Windows.Forms.Padding (0);
			this.MaximumSize = new System.Drawing.Size (256, 256);
			this.MinimumSize = new System.Drawing.Size (16, 16);
			this.Size = new System.Drawing.Size (128, 128);

			this.AnimationImage = new System.Windows.Controls.Image ();
			this.Child = this.AnimationImage;
		}

		protected override void OnHandleCreated (EventArgs e)
		{
			base.OnHandleCreated (e);

			System.Drawing.Graphics lGraphics = System.Drawing.Graphics.FromHwnd (this.Handle);
			this.AnimationImage.LayoutTransform = new System.Windows.Media.ScaleTransform (96.0 / (double)lGraphics.DpiX, 96.0 / (double)lGraphics.DpiY);
			this.AnimationImage.Stretch = System.Windows.Media.Stretch.Uniform;
			this.AnimationImage.UpdateLayout ();
		}

		protected override void OnHandleDestroyed (EventArgs e)
		{
			base.OnHandleDestroyed (e);

			DeleteAnimation ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public Boolean IsAnimating
		{
			get
			{
				return (AnimationStoryboard != null) && (AnimationTimeline != null) && (AnimationTimeline.KeyFrames.Count > 0);
			}
		}

		public System.Windows.Controls.Image AnimationImage
		{
			get;
			protected set;
		}
		public System.Windows.Media.Animation.Storyboard AnimationStoryboard
		{
			get;
			protected set;
		}
		public AgentCharacterEditor.Previews.AnimationPreviewAnimation AnimationTimeline
		{
			get;
			protected set;
		}

		///////////////////////////////////////////////////////////////////////////////

		public Boolean AnimationIsPlaying
		{
			get
			{
				if (AnimationStoryboard != null)
				{
					try
					{
						return (AnimationStoryboard.GetCurrentState (AnimationImage) == System.Windows.Media.Animation.ClockState.Active);
					}
					catch
					{
					}
				}
				return false;
			}
		}

		public Boolean AnimationIsPaused
		{
			get
			{
				if (AnimationStoryboard != null)
				{
					try
					{
						return AnimationStoryboard.GetIsPaused (AnimationImage);
					}
					catch
					{
					}
				}
				return false;
			}
			set
			{
				if (AnimationStoryboard != null)
				{
					try
					{
						if (value)
						{
							AnimationStoryboard.Pause (AnimationImage);
						}
						else
						{
							AnimationStoryboard.Resume (AnimationImage);
						}
					}
					catch (Exception ex)
					{
						System.Diagnostics.Debug.Print (ex.Message);
					}
				}
			}
		}

		public Boolean AnimationIsRepeating
		{
			get
			{
				if (AnimationStoryboard != null)
				{
					try
					{
						return (AnimationStoryboard.RepeatBehavior == System.Windows.Media.Animation.RepeatBehavior.Forever);
					}
					catch
					{
					}
				}
				return false;
			}
			set
			{
				if (AnimationStoryboard != null)
				{
					try
					{
						if (value)
						{
							AnimationStoryboard.RepeatBehavior = System.Windows.Media.Animation.RepeatBehavior.Forever;
						}
						else
						{
							AnimationStoryboard.ClearValue (System.Windows.Media.Animation.Storyboard.RepeatBehaviorProperty);
						}
					}
					catch (Exception e)
					{
						System.Diagnostics.Debug.Print (e.Message);
					}
				}
			}
		}

		public Double AnimationRate
		{
			get
			{
				if (AnimationStoryboard != null)
				{
					try
					{
						return Math.Log (AnimationStoryboard.SpeedRatio, 2.0);
					}
					catch (Exception e)
					{
						System.Diagnostics.Debug.Print (e.Message);
					}
				}
				return 0.0;
			}
			set
			{
				if (AnimationStoryboard != null)
				{
					try
					{
						AnimationStoryboard.SpeedRatio = Math.Pow (2.0, value);
						if (AnimationIsPlaying)
						{
							AnimationStoryboard.SetSpeedRatio (AnimationImage, AnimationStoryboard.SpeedRatio);
						}
					}
					catch (Exception e)
					{
						System.Diagnostics.Debug.Print (e.Message);
					}
				}
			}
		}

		public TimeSpan? AnimationTime
		{
			get
			{
				if (AnimationStoryboard != null)
				{
					try
					{
						return AnimationStoryboard.GetCurrentTime (AnimationImage);
					}
					catch (Exception e)
					{
						System.Diagnostics.Debug.Print (e.Message);
					}
				}
				return null;
			}
			set
			{
				if (AnimationStoryboard != null)
				{
					try
					{
						AnimationStoryboard.SeekAlignedToLastTick (AnimationImage, value.Value, System.Windows.Media.Animation.TimeSeekOrigin.BeginTime);
					}
					catch (Exception e)
					{
						System.Diagnostics.Debug.Print (e.Message);
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		public TimeSpan? TimeFromFrame (FileAnimationFrame pFrame)
		{
			return (AnimationTimeline != null) ? AnimationTimeline.TimeFromFrame (pFrame) : null;
		}
		public TimeSpan? TimeFromFrameIndex (int pFrameIndex, FileAnimation pFileAnimation)
		{
			return (AnimationTimeline != null) ? AnimationTimeline.TimeFromFrameIndex (pFrameIndex, pFileAnimation) : null;
		}

		public FileAnimationFrame FrameFromTime (TimeSpan pTime)
		{
			return (AnimationTimeline != null) ? AnimationTimeline.FrameFromTime (pTime) : null;
		}
		public int FrameIndexFromTime (TimeSpan pTime)
		{
			return (AnimationTimeline != null) ? AnimationTimeline.FrameIndexFromTime (pTime) : -1;
		}
		public int FrameIndexFromTime (TimeSpan pTime, FileAnimation pFileAnimation)
		{
			return (AnimationTimeline != null) ? AnimationTimeline.FrameIndexFromTime (pTime, pFileAnimation) : -1;
		}

		public FileAnimationFrame FrameFromTime (TimeSpan? pTime)
		{
			return (AnimationTimeline != null) ? AnimationTimeline.FrameFromTime (pTime) : null;
		}
		public int FrameIndexFromTime (TimeSpan? pTime)
		{
			return (AnimationTimeline != null) ? AnimationTimeline.FrameIndexFromTime (pTime) : -1;
		}
		public int FrameIndexFromTime (TimeSpan? pTime, FileAnimation pFileAnimation)
		{
			return (AnimationTimeline != null) ? AnimationTimeline.FrameIndexFromTime (pTime, pFileAnimation) : -1;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		public Boolean CreateAnimation (CharacterFile pCharacterFile, FileAnimation pAnimation)
		{
			Boolean lRet = false;

			DeleteAnimation ();

			if ((pCharacterFile != null) && (pAnimation != null))
			{
				try
				{
					AnimationStoryboard = new System.Windows.Media.Animation.Storyboard ();
					AnimationTimeline = new AnimationPreviewAnimation (pCharacterFile, pAnimation);

					AnimationStoryboard.Name = pAnimation.Name;
					AnimationStoryboard.Children.Add (AnimationTimeline);
					System.Windows.Media.Animation.Storyboard.SetTarget (AnimationStoryboard, AnimationImage);
					System.Windows.Media.Animation.Storyboard.SetTargetProperty (AnimationStoryboard, new System.Windows.PropertyPath (System.Windows.Controls.Image.SourceProperty));
					//System.Windows.Media.Animation.Timeline.SetDesiredFrameRate (AnimationStoryboard, 100);

					lRet = true;
				}
				catch (Exception e)
				{
					System.Diagnostics.Debug.Print (e.Message);
				}
			}

			return lRet;
		}

		public Boolean DeleteAnimation ()
		{
			Boolean lRet = false;

			StopAnimation ();

			if (AnimationStoryboard != null)
			{
				try
				{
					AnimationStoryboard.Remove (AnimationImage);
					lRet = true;
				}
				catch (Exception e)
				{
					System.Diagnostics.Debug.Print (e.Message);
				}
			}
			AnimationTimeline = null;
			AnimationStoryboard = null;

			return lRet;
		}

		///////////////////////////////////////////////////////////////////////////////

		public Boolean StartAnimation ()
		{
			Boolean lRet = false;

			if (AnimationStoryboard != null)
			{
				try
				{
					AnimationStoryboard.Begin (AnimationImage, true);
					lRet = true;
				}
				catch (Exception e)
				{
					System.Diagnostics.Debug.Print (e.Message);
				}
			}
			return lRet;
		}

		public Boolean StopAnimation ()
		{
			Boolean lRet = false;

			if (AnimationStoryboard != null)
			{
				try
				{
					AnimationStoryboard.Stop (AnimationImage);
					lRet = true;
				}
				catch (Exception e)
				{
					System.Diagnostics.Debug.Print (e.Message);
				}
			}
			return lRet;
		}

		///////////////////////////////////////////////////////////////////////////////

		public void ShowFramePreview (CharacterFile pCharacterFile, FileAnimationFrame pFrame)
		{
			if (pCharacterFile != null)
			{
				Size = pCharacterFile.Header.ImageSize;
				AnimationImage.Source = AnimationPreviewFrame.MakeImageSource (pCharacterFile, pFrame);
			}
			else
			{
				Size = PictureBoxSample.DefaultImageSize;
				AnimationImage.Source = null;
			}
			AnimationImage.UpdateLayout ();
		}

		#endregion
	}
}
