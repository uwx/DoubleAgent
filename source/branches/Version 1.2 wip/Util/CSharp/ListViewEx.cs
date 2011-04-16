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
	public class ListViewEx : System.Windows.Forms.ListView
	{
		public ListViewEx ()
		{
			this.CheckOnActivate = true;
		}

		public Boolean UpdateItemCount (int pItemCount)
		{
			Boolean lRet = false;

			while (Items.Count > pItemCount)
			{
				Items.RemoveAt (Items.Count - 1);
				lRet = true;
			}
			while (Items.Count < pItemCount)
			{
				Items.Add ("");
				lRet = true;
			}
			return lRet;
		}

		public Boolean UpdateSubItemCount (ListViewItem pItem)
		{
			return UpdateSubItemCount (pItem, Columns.Count);
		}

		static public Boolean UpdateSubItemCount (ListViewItem pItem, int pSubItemCount)
		{
			Boolean lRet = false;

			if (pItem != null)
			{
				while (pItem.SubItems.Count > pSubItemCount)
				{
					pItem.SubItems.RemoveAt (pItem.SubItems.Count - 1);
					lRet = true;
				}
				while (pItem.SubItems.Count < pSubItemCount)
				{
					pItem.SubItems.Add ("");
					lRet = true;
				}
			}
			return lRet;
		}

		///////////////////////////////////////////////////////////////////////////////

		[System.ComponentModel.Browsable (false)]
		public ListViewItem SelectedItem
		{
			get
			{
				return GetSelectedItem (true);
			}
			set
			{
				if (value == null)
				{
					SelectedItems.Clear ();
				}
				else if (Items.Contains (value))
				{
					if ((SelectedItems.Count != 1) || (!SelectedItems.Contains (value)))
					{
						SelectedItems.Clear ();
					}
					value.Selected = true;
					value.Focused = true;
				}
			}
		}

		public ListViewItem GetSelectedItem (Boolean pIncludeFocus)
		{
			if (SelectedIndices.Count == 1)
			{
				return SelectedItems[0];
			}
			else if (pIncludeFocus)
			{
				return FocusedItem;
			}
			else
			{
				return null;
			}
		}

		[System.ComponentModel.Browsable (false)]
		public int SelectedIndex
		{
			get
			{
				return GetSelectedIndex (true);
			}
			set
			{
				value = Math.Min (value, Items.Count - 1);
				if ((value >= 0) && (value < Items.Count))
				{
					SelectedItem = Items[value];
				}
				else if (value < 0)
				{
					SelectedItem = null;
				}
			}
		}

		public int GetSelectedIndex (Boolean pIncludeFocus)
		{
			if (SelectedIndices.Count == 1)
			{
				return SelectedIndices[0];
			}
			else if (pIncludeFocus && (FocusedItem != null))
			{
				return FocusedItem.Index;
			}
			else
			{
				return -1;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		public void AutoSizeColumn (ColumnHeader pColumnHeader)
		{
			int	lColumnWidth;

			pColumnHeader.AutoResize (ColumnHeaderAutoResizeStyle.HeaderSize);
			lColumnWidth = pColumnHeader.Width;
			pColumnHeader.AutoResize (ColumnHeaderAutoResizeStyle.ColumnContent);
			lColumnWidth = System.Math.Max (lColumnWidth, pColumnHeader.Width);
			pColumnHeader.Width = lColumnWidth;
		}

		///////////////////////////////////////////////////////////////////////////////

		[System.ComponentModel.Category ("Appearance")]
		[System.ComponentModel.DefaultValue (true)]
		public Boolean CheckOnActivate
		{
			get;
			set;
		}

		private const int WM_LBUTTONDBLCLK = 0x0203;
		private Boolean In_WM_LBUTTONDBLCLK = false;

		protected override void WndProc (ref Message m)
		{
			if (m.Msg == WM_LBUTTONDBLCLK)
			{
				In_WM_LBUTTONDBLCLK = true;
			}
			base.WndProc (ref m);
			if (m.Msg == WM_LBUTTONDBLCLK)
			{
				In_WM_LBUTTONDBLCLK = false;
			}
		}

		protected override void OnItemCheck (ItemCheckEventArgs ice)
		{
			if (!this.CheckOnActivate && this.In_WM_LBUTTONDBLCLK)
			{
				ice.NewValue = ice.CurrentValue;
			}
			base.OnItemCheck (ice);
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override void OnCreateControl ()
		{
			this.DoubleBuffered = true;
			base.OnCreateControl ();
		}
	}
}
