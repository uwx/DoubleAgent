/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2012 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is a utility used by Double Agent but not specific to
	Double Agent.  However, it is included as part of the Double Agent
	source code under the following conditions:

    This is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
using System;
using System.Windows.Forms;
using System.Drawing;
using System.ComponentModel;

namespace DoubleAgent
{
	public class ToolStripEx : System.Windows.Forms.ToolStrip
	{
		public ToolStripEx ()
		{
			this.RenderInline = true;
			this.CanOverflow = false;
			this.GripStyle = ToolStripGripStyle.Hidden;
		}

		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Category ("Appearance")]
		[System.ComponentModel.DefaultValue (true)]
		[System.ComponentModel.RefreshProperties (RefreshProperties.Repaint)]
		public Boolean RenderInline
		{
			get
			{
				return (Renderer != null) && (Renderer is InlineToolStripRenderer);
			}
			set
			{
				if (value)
				{
					Renderer = new InlineToolStripRenderer ();
				}
				else
				{
					Renderer = null;
					RenderMode = ToolStripRenderMode.System;
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		public void TipTextChanged ()
		{
			Win32Native.PopThreadToolTips (this.Handle, true);
		}

		#endregion
	}

	///////////////////////////////////////////////////////////////////////////////

	public partial class ToolStripButtonEx : ToolStripButton
	{
		public ToolStripButtonEx ()
		{
			this.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ImageTransparentColor = System.Drawing.Color.Magenta;

			this.RepeatEnabled = false;
			this.ClickRepeatNum = 0;
			this.ClickIsRepeat = false;
		}

		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Category ("Behavior")]
		[System.ComponentModel.DefaultValue (false)]
		[System.ComponentModel.RefreshProperties (RefreshProperties.Repaint)]
		public Boolean RepeatEnabled
		{
			get
			{
				return (this.RepeatSpeed > 0);
			}
			set
			{
				if (value)
				{
					if (this.RepeatDelay <= 0)
					{
						this.RepeatDelay = DefaultRepeatDelay;
					}
					if (this.RepeatSpeed <= 0)
					{
						this.RepeatSpeed = DefaultRepeatSpeed;
					}
				}
				else
				{
					this.RepeatDelay = 0;
					this.RepeatSpeed = 0;
				}
			}
		}
		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		//[System.ComponentModel.Category ("Behavior")]
		//[System.ComponentModel.DefaultValue (0)]
		public int RepeatDelay
		{
			get;
			set;
		}
		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		//[System.ComponentModel.Category ("Behavior")]
		//[System.ComponentModel.DefaultValue (0)]
		public int RepeatSpeed
		{
			get;
			set;
		}

		static public int DefaultRepeatDelay
		{
			get
			{
				return System.Windows.Forms.SystemInformation.DoubleClickTime * 2;
			}
		}
		static public int DefaultRepeatSpeed
		{
			get
			{
				return System.Windows.Forms.SystemInformation.DoubleClickTime / 4;
			}
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public System.Windows.Forms.Timer ClickRepeatTimer
		{
			get;
			protected set;
		}
		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public int ClickRepeatNum
		{
			get;
			protected set;
		}
		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Boolean ClickIsRepeat
		{
			get;
			protected set;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		protected void StartRepeat ()
		{
			if (this.RepeatSpeed > 0)
			{
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("{0} StartRepeat", this.Name);
#endif
				if (this.ClickRepeatTimer == null)
				{
					this.ClickRepeatTimer = new Timer ();
					this.ClickRepeatTimer.Tick += new EventHandler (ClickRepeatTimer_Tick);
				}
				this.ClickRepeatNum = 0;
				this.ClickRepeatTimer.Interval = this.RepeatDelay;
				this.ClickRepeatTimer.Start ();
			}
			else
			{
				StopRepeat (false);
				this.ClickRepeatTimer = null;
			}
		}

		protected void StopRepeat (Boolean pNotify)
		{
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("{0} StopRepeat {1} Notify {2}", this.Name, this.RepeatNum.ToString (), pNotify.ToString());
#endif
			if (pNotify && (this.ClickRepeatNum > 0) && (this.RepeatEnd != null))
			{
				try
				{
					RepeatEnd (this, EventArgs.Empty);
				}
				catch
				{
				}
			}
			this.ClickRepeatNum = 0;
			if (this.ClickRepeatTimer != null)
			{
				this.ClickRepeatTimer.Stop ();
			}
		}

		protected void ContinueRepeat ()
		{
			if (this.ClickRepeatTimer != null)
			{
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("{0} ContinueRepeat {1}", this.Name, this.RepeatNum.ToString ());
#endif
				if (this.ClickRepeatNum <= 0)
				{
					this.ClickRepeatTimer.Interval = this.RepeatSpeed;
				}
				this.ClickRepeatNum++;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public delegate void RepeatEndEventHandler (object sender, EventArgs e);
		public event RepeatEndEventHandler RepeatEnd;

		//=============================================================================

		protected override void OnMouseDown (MouseEventArgs e)
		{
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("{0} OnMouseDown Selected {1}", this.Name, this.Selected.ToString ());
#endif
			StopRepeat (false);
			base.OnMouseDown (e);
			StartRepeat ();
		}

		protected override void OnMouseUp (MouseEventArgs e)
		{
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("{0} OnMouseUp Selected {1} Repeat {2}", this.Name, this.Selected.ToString (), this.RepeatNum.ToString());
#endif
			StopRepeat (true);
			base.OnMouseUp (e);
		}

		protected override void OnClick (EventArgs e)
		{
			if (!this.ClickIsRepeat)
			{
				this.ClickRepeatNum = 0;
			}
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("{0} OnClick RepeatClick {1} RepeatNum {2}", this.Name, this.RepeatClick.ToString (), this.RepeatNum.ToString ());
#endif
			base.OnClick (e);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void ClickRepeatTimer_Tick (object Sender, EventArgs e)
		{
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("Tick Selected {0} Pressed {1} RepeatNum {2} Interval {3}", this.Selected.ToString (), this.Pressed.ToString (), this.RepeatNum.ToString (), this.RepeatTimer.Interval.ToString ());
#endif
			if (this.Selected)
			{
				ContinueRepeat ();
				if (this.Pressed)
				{
					this.ClickIsRepeat = true;
					try
					{
						this.OnClick (EventArgs.Empty);
					}
					catch
					{
					}
					this.ClickIsRepeat = false;
				}
			}
			else
			{
				StopRepeat (true);
			}
		}

		#endregion
	}

	///////////////////////////////////////////////////////////////////////////////

	internal class InlineToolStripRenderer : ToolStripSystemRenderer
	{
		protected override void InitializeItem (ToolStripItem item)
		{
			base.InitializeItem (item);
			if (item is ToolStripButton)
			{
				item.Padding = new System.Windows.Forms.Padding (2, 2, 2, 2);
				item.Size = new System.Drawing.Size (24, 24);
			}
		}

		//=============================================================================

		protected override void OnRenderToolStripBorder (ToolStripRenderEventArgs e)
		{
		}

		protected override void OnRenderToolStripBackground (ToolStripRenderEventArgs e)
		{
			e.Graphics.FillRectangle (new System.Drawing.SolidBrush (System.Drawing.SystemColors.Control), e.AffectedBounds);
		}
	}

}
