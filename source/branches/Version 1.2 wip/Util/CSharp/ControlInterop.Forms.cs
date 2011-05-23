/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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

namespace DoubleAgent
{
	public partial class TextBoxEx : System.Windows.Forms.TextBox
	{
		public Boolean IsEnabled
		{
			get
			{
				return Enabled;
			}
			set
			{
				Enabled = value;
			}
		}

		public Boolean IsModified
		{
			get
			{
				return Modified;
			}
			set
			{
				Modified = value;
			}
		}

		public Boolean IsReadOnly
		{
			get
			{
				return ReadOnly;
			}
			set
			{
				ReadOnly = value;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public partial class ListViewEx : System.Windows.Forms.ListView
	{
		public Boolean IsEnabled
		{
			get
			{
				return Enabled;
			}
			set
			{
				Enabled = value;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public partial class TreeViewEx : System.Windows.Forms.TreeView
	{
		public Boolean IsEnabled
		{
			get
			{
				return Enabled;
			}
			set
			{
				Enabled = value;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public class ButtonEx : System.Windows.Forms.Button
	{
		public Boolean IsEnabled
		{
			get
			{
				return Enabled;
			}
			set
			{
				Enabled = value;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public class CheckBoxEx : System.Windows.Forms.CheckBox
	{
		public Boolean IsEnabled
		{
			get
			{
				return Enabled;
			}
			set
			{
				Enabled = value;
			}
		}

		public Boolean? IsChecked
		{
			get
			{
				return Checked;
			}
			set
			{
				Checked = value.Value;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public class RadioButtonEx : System.Windows.Forms.RadioButton
	{
		public Boolean IsEnabled
		{
			get
			{
				return Enabled;
			}
			set
			{
				Enabled = value;
			}
		}

		public Boolean? IsChecked
		{
			get
			{
				return Checked;
			}
			set
			{
				Checked = value.Value;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public class LabelEx : System.Windows.Forms.Label
	{
		public Boolean IsEnabled
		{
			get
			{
				return Enabled;
			}
			set
			{
				Enabled = value;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public class GroupBoxEx : System.Windows.Forms.GroupBox
	{
		public Boolean IsEnabled
		{
			get
			{
				return Enabled;
			}
			set
			{
				Enabled = value;
			}
		}
	}
}
