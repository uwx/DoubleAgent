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

namespace DoubleAgent
{
	public class TextBoxEx : System.Windows.Forms.TextBox
	{
		private const int WM_KEYDOWN = 0x0100;

		public TextBoxEx ()
		{
			AcceptsReturn = true;
		}

		protected override bool ProcessCmdKey (ref Message pMessage, Keys pKeyData)
		{
			if ((pMessage.Msg == WM_KEYDOWN) && (pKeyData == Keys.Return) && (this.AcceptsReturn))
			{
				ValidateNow ();
				return true;
			}
			return base.ProcessCmdKey (ref pMessage, pKeyData);
		}

		private Boolean ValidateNow ()
		{
			if (CausesValidation)
			{
				CancelEventArgs	lEventArgs = new CancelEventArgs ();
				OnValidating (lEventArgs);
				if (!lEventArgs.Cancel)
				{
					OnValidated (new EventArgs ());
					return true;
				}
			}
			return false;
		}
	}
}
