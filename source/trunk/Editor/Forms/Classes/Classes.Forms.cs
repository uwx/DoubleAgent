/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
using System.Drawing;
using System.Windows.Forms;

namespace AgentCharacterEditor
{
	public class ListViewItemCommon : System.Windows.Forms.ListViewItem
	{
		public ListViewItemCommon ()
		{
		}
		public ListViewItemCommon (string text)
			: base (text)
		{
		}
		public ListViewItemCommon (string text, int imageIndex)
			: base (text, imageIndex)
		{
		}
		public ListViewItemCommon (string text, string tag)
			: base (text)
		{
			this.Tag = tag;
		}
		public ListViewItemCommon (string[] items)
			: base (items)
		{
		}
		public ListViewItemCommon (string[] items, int imageIndex, Color foreColor, Color backColor, Font font)
			: base (items, imageIndex, foreColor, backColor, font)
		{
		}

		public Boolean IsSelected
		{
			get
			{
				return Selected;
			}
			set
			{
				Selected = value;
			}
		}
	}
}
