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
	public partial class AnimationPreviewHost : System.Windows.Forms.UserControl
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public AnimationPreviewHost ()
		{
			InitializeComponent ();
		}

		protected override void OnHandleCreated (EventArgs e)
		{
			base.OnHandleCreated (e);

			System.Drawing.Graphics lGraphics = System.Drawing.Graphics.FromHwnd (this.Handle);
			this.WPFTarget.LayoutTransform = new ScaleTransform (96.0 / (double)lGraphics.DpiX, 96.0 / (double)lGraphics.DpiY);
			this.WPFTarget.UpdateLayout ();
			this.WPFTarget.AnimationStateChanged += new EventHandler (Target_StateChanged);
			this.WPFTarget.AnimationImageChanged += new EventHandler (Target_ImageChanged);
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
				return WPFTarget.IsAnimated;
			}
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Boolean IsPlaying
		{
			get
			{
				return WPFTarget.IsPlaying;
			}
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Boolean IsPaused
		{
			get
			{
				return WPFTarget.IsPaused;
			}
			set
			{
				WPFTarget.IsPaused = value;
			}
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Boolean IsRepeating
		{
			get
			{
				return WPFTarget.IsRepeating;
			}
			set
			{
				WPFTarget.IsRepeating = value;
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
				return WPFTarget.AnimationRate;
			}
			set
			{
				WPFTarget.AnimationRate = value;
			}
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public TimeSpan? CurrentTime
		{
			get
			{
				return WPFTarget.CurrentTime;
			}
			set
			{
				WPFTarget.CurrentTime = value;
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
				return WPFTarget.CurrentFrameIndex;
			}
			set
			{
				WPFTarget.CurrentFrameIndex = value;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		public Boolean CreateAnimation (CharacterFile pCharacterFile, FileAnimation pAnimation)
		{
			DeleteAnimation ();
			return WPFTarget.CreateAnimation (pCharacterFile, pAnimation, true);
		}

		public Boolean CreateAnimation (CharacterFile pCharacterFile, FileAnimation pAnimation, Boolean pIncludeSound)
		{
			DeleteAnimation ();
			return WPFTarget.CreateAnimation (pCharacterFile, pAnimation, pIncludeSound);
		}

		public Boolean CreateAnimation (CharacterFile pCharacterFile, FileAnimation pAnimation, System.Drawing.Size pImageSize, Boolean pIncludeSound)
		{
			DeleteAnimation ();
			SetAnimationSize (pImageSize);
			return WPFTarget.CreateAnimation (pCharacterFile, pAnimation, pIncludeSound);
		}

		public Boolean DeleteAnimation ()
		{
			return WPFTarget.DeleteAnimation ();
		}

		public Boolean StartAnimation ()
		{
			return WPFTarget.StartAnimation ();
		}

		public Boolean StopAnimation ()
		{
			return WPFTarget.StopAnimation ();
		}

		///////////////////////////////////////////////////////////////////////////////

		public void ShowAnimationFrame (CharacterFile pCharacterFile, FileAnimationFrame pFrame)
		{
			StopAnimation ();
			WPFTarget.ShowAnimationFrame (pCharacterFile, pFrame);
		}

		public void ShowAnimationFrame (CharacterFile pCharacterFile, FileAnimationFrame pFrame, System.Drawing.Size pImageSize)
		{
			StopAnimation ();
			SetAnimationSize (pImageSize);
			WPFTarget.ShowAnimationFrame (pCharacterFile, pFrame);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void SetAnimationSize ( System.Drawing.Size pImageSize)
		{
			WPFHost.Size = pImageSize;
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

		private void Target_StateChanged (object sender, EventArgs e)
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

		private void Target_ImageChanged (object sender, EventArgs e)
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
