/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2014 Cinnamon Software Inc.
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
using System.ComponentModel;
using System.Drawing;

namespace DoubleAgent
{
	/// <summary>
	/// A <see cref="System.Windows.Forms.NumericUpDown"/> with a bit of extra functionality.
	/// </summary>
	/// <remarks>Validation is triggered after the mouse wheel is used and after the up/down buttons are used.</remarks>
	public partial class NumericUpDownEx : System.Windows.Forms.NumericUpDown
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		private const int WM_KEYDOWN = 0x0100;
		private TextBox mTextBox = null;
		private Timer mWheelTimer = null;

		/// <summary>
		/// Constructor
		/// </summary>
		public NumericUpDownEx ()
		{
			this.DefaultBackColor = SystemColors.Window;
			this.HighlightBackColor = Color.Pink;
			this.MouseWheelSingle = true;
			this.ValidateOnReturnKey = true;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		/// <summary>
		/// The <see cref="System.Windows.Forms.TextBox"/> contained by this control.
		/// </summary>
		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public System.Windows.Forms.TextBox TextBox
		{
			get
			{
				return mTextBox;
			}
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Boolean Modified
		{
			get
			{
				return TextBox.Modified;
			}
			set
			{
				TextBox.Modified = value;
			}
		}

		/// <summary>
		/// Indicates if validation should be triggered when the return key is pressed.
		/// </summary>
		[System.ComponentModel.Category ("Behavior")]
		[System.ComponentModel.DefaultValue (true)]
		public Boolean ValidateOnReturnKey
		{
			get;
			set;
		}

		/// <summary>
		/// Indicates if mouse wheel increments and decrements should be limited to +/- 1.
		/// </summary>
		[System.ComponentModel.Category ("Behavior")]
		[System.ComponentModel.DefaultValue (true)]
		public Boolean MouseWheelSingle
		{
			get;
			set;
		}

		//=============================================================================

		/// <summary>
		/// The default background color.
		/// </summary>
		/// <seealso cref="HighlightBackColor"/>
		public new System.Drawing.Color DefaultBackColor
		{
			get;
			set;
		}

		/// <summary>
		/// The highlighted background color.
		/// </summary>
		/// <remarks>The highlighted background color is used when the control's value is not in range.</remarks>
		/// <seealso cref="IsHighlighted"/>
		/// <seealso cref="DefaultBackColor"/>
		public System.Drawing.Color HighlightBackColor
		{
			get;
			set;
		}

		/// <summary>
		/// Indicates if the control is highlighted.
		/// </summary>
		/// <remarks>If the control is highlighted, it will use the <see cref="HighlightBackColor"/>. Otherwise, it will use the <see cref="DefaultBackColor"/></remarks>
		/// <seealso cref="HighlightBackColor"/>
		/// <seealso cref="NumericUpDown.Minimum"/>
		/// <seealso cref="NumericUpDown.Maximum"/>
		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Boolean IsHighlighted
		{
			get
			{
				return (this.BackColor == this.HighlightBackColor);
			}
			set
			{
				if (value)
				{
					if (this.BackColor != this.HighlightBackColor)
					{
						this.BackColor = this.HighlightBackColor;
					}
				}
				else
				{
					if (this.BackColor != this.DefaultBackColor)
					{
						this.BackColor = this.DefaultBackColor;
					}
				}
			}
		}

		//=============================================================================

		/// <summary>
		/// This is an override of <see cref="NumericUpDown.Value"/>.  Instead of throwing an exception when the value is out of range, it highlights the control.
		/// </summary>
		/// <seealso cref="NumericUpDown.Minimum"/>
		/// <seealso cref="NumericUpDown.Maximum"/>
		/// <seealso cref="IsHighlighted"/>
		public new decimal Value
		{
			get
			{
				return base.Value;
			}
			set
			{
				if ((value < Minimum) || (value > Maximum))
				{
					value = Math.Min (Math.Max (value, Minimum), Maximum);
					this.IsHighlighted = true;
				}
				else
				{
					this.IsHighlighted = false;
				}
				if (base.Value != value) // Setting the same value causes to control to blank out
				{
					base.Value = value;
				}
				else if (String.IsNullOrEmpty (base.Text))
				{
					base.Text = "x"; // Setting the same value causes to control to blank out
					base.Value = value;
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Implementation

		protected override bool ProcessCmdKey (ref Message pMessage, Keys pKeyData)
		{
			if ((pMessage.Msg == WM_KEYDOWN) && (pKeyData == Keys.Return) && ValidateOnReturnKey)
			{
				ValidateNow ();
				return true;
			}
			return base.ProcessCmdKey (ref pMessage, pKeyData);
		}

		private Boolean ValidateNow ()
		{
			if (mWheelTimer != null)
			{
				mWheelTimer.Stop ();
				mWheelTimer = null;
			}
			if (CausesValidation)
			{
				CancelEventArgs lEventArgs = new CancelEventArgs ();
				OnValidating (lEventArgs);
				if (!lEventArgs.Cancel)
				{
					if (Validate ())
					{
						OnValidated (new EventArgs ());
						return true;
					}
				}
			}
			return false;
		}

		protected override void OnValidated (EventArgs e)
		{
			if (mWheelTimer != null)
			{
				mWheelTimer.Stop ();
				mWheelTimer = null;
			}
			if ((base.Value >= Minimum) && (base.Value <= Maximum) && (BackColor != DefaultBackColor))
			{
				BackColor = DefaultBackColor;
			}
			base.OnValidated (e);
		}

		//=============================================================================

		/// <summary>
		/// Sets up the internal event handlers and retrieves the contained TextBox.
		/// </summary>
		protected override void OnControlAdded (ControlEventArgs e)
		{
			if (e.Control is TextBox)
			{
				mTextBox = e.Control as TextBox;
			}
			else
			{
				try
				{
					System.Type lControlType = e.Control.GetType ();
					System.Reflection.EventInfo lControlEvent = lControlType.GetEvent ("UpDown");

					if (lControlEvent != null)
					{
						System.Reflection.MethodInfo lAddHandlerMethod = lControlEvent.GetAddMethod ();
						System.Object[] lAddHandlerParms = new System.Object[1];

						lAddHandlerParms[0] = new UpDownEventHandler (UpDownButtons_UpDown);
						lAddHandlerMethod.Invoke (e.Control, lAddHandlerParms);

						e.Control.MouseCaptureChanged += new System.EventHandler (UpDownButtons_MouseCaptureChanged);
					}
				}
				catch
				{
				}
			}

			base.OnControlAdded (e);
		}

		/// <summary>
		/// Overrides mouse wheel processing.
		/// </summary>
		protected override void OnMouseWheel (MouseEventArgs e)
		{
			if (this.MouseWheelSingle)
			{
				this.Value = Math.Min (Math.Max (this.Value + (e.Delta / System.Windows.Forms.SystemInformation.MouseWheelScrollDelta), this.Minimum), this.Maximum);
			}
			else
			{
				base.OnMouseWheel (e);
			}
			//if (Math.Abs (e.Delta) > System.Windows.Forms.SystemInformation.MouseWheelScrollDelta)
			{
				if (mWheelTimer == null)
				{
					mWheelTimer = new Timer ();
					mWheelTimer.Tick += new EventHandler (WheelTimer_Tick);
					mWheelTimer.Interval = System.Windows.Forms.SystemInformation.DoubleClickTime;
				}
				mWheelTimer.Stop ();
				mWheelTimer.Start ();
			}
			//else
			//{
			//    if (mWheelTimer == null)
			//    {
			//        ValidateNow ();
			//    }
			//}
		}

		/// <summary>
		/// Causes validation when the mouse wheel has stopped turning.
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		void WheelTimer_Tick (object sender, EventArgs e)
		{
			ValidateNow ();
		}

		//=============================================================================

		private void UpDownButtons_MouseCaptureChanged (object sender, EventArgs e)
		{
			Control lControl = (Control)sender;
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("UpDownButtons_MouseCaptureChanged {0:B}", lControl.Capture);
#endif
			if (!lControl.Capture)
			{
				ValidateNow ();
			}
		}

		private void UpDownButtons_UpDown (object sender, UpDownEventArgs e)
		{
#if DEBUG_NOT
			Control	lControl = (Control)sender;
			System.Diagnostics.Debug.Print ("UpDownButtons_UpDown {0:B} {1:D}", lControl.Capture, e.ButtonID);
#endif
		}

		#endregion
	}
}
