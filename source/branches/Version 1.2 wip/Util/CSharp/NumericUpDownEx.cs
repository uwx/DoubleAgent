/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2011 Cinnamon Software Inc.
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
	public class NumericUpDownEx : System.Windows.Forms.NumericUpDown
	{
		private const	int WM_KEYDOWN = 0x0100;
		private TextBox	mTextBox = null;
		private Color	mDefaultBackColor = SystemColors.Window;
		private Timer	mWheelTimer = null;

		public NumericUpDownEx ()
		{
			this.MouseWheelSingle = true;
		}

		[System.ComponentModel.Browsable (false)]
		public System.Windows.Forms.TextBox TextBox
		{
			get
			{
				return mTextBox;
			}
		}

		public new System.Drawing.Color DefaultBackColor
		{
			get
			{
				return mDefaultBackColor;
			}
			set
			{
				mDefaultBackColor = value;
			}
		}

		[System.ComponentModel.Category ("Behavior")]
		[System.ComponentModel.DefaultValue (true)]
		public Boolean MouseWheelSingle
		{
			get;
			set;
		}

		///////////////////////////////////////////////////////////////////////////////

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
					BackColor = Color.Pink;
				}
				else if (BackColor != DefaultBackColor)
				{
					BackColor = DefaultBackColor;
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

		///////////////////////////////////////////////////////////////////////////////

		protected override bool ProcessCmdKey (ref Message pMessage, Keys pKeyData)
		{
			if ((pMessage.Msg == WM_KEYDOWN) && (pKeyData == Keys.Return))
			{
				ValidateNow ();
				return true;
			}
			return base.ProcessCmdKey (ref pMessage, pKeyData);
		}

		private bool ValidateNow ()
		{
			if (mWheelTimer != null)
			{
				mWheelTimer.Stop ();
				mWheelTimer = null;
			}
			if (CausesValidation)
			{
				CancelEventArgs	lEventArgs = new CancelEventArgs ();
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

		///////////////////////////////////////////////////////////////////////////////

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
					System.Type					lControlType = e.Control.GetType ();
					System.Reflection.EventInfo lControlEvent = lControlType.GetEvent ("UpDown");

					if (lControlEvent != null)
					{
						System.Reflection.MethodInfo	lAddHandlerMethod = lControlEvent.GetAddMethod ();
						System.Object[]					lAddHandlerParms = new System.Object[1];

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

		protected override void OnMouseWheel (MouseEventArgs e)
		{
			if (this.MouseWheelSingle)
			{
				this.Value += e.Delta / System.Windows.Forms.SystemInformation.MouseWheelScrollDelta;
			}
			else
			{
				base.OnMouseWheel (e);
			}
			//if (Math.Abs (e.Delta) > System.Windows.Forms.SystemInformation.MouseWheelScrollDelta)
			{
				if (mWheelTimer == null)
				{
					mWheelTimer = new Timer();
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

		void WheelTimer_Tick (object sender, EventArgs e)
		{
			ValidateNow ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void UpDownButtons_MouseCaptureChanged (object sender, EventArgs e)
		{
			Control	lControl = (Control)sender;
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
	}
}
