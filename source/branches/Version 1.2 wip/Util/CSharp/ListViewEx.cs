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

namespace DoubleAgent
{
	/// <summary>
	/// A <see cref="System.Windows.Forms.ListView"/> with a bit of extra functionality an double-buffering enabled
	/// </summary>
	public class ListViewEx : System.Windows.Forms.ListView
	{
		/// <summary>
		/// Constructor
		/// </summary>
		public ListViewEx ()
		{
			this.CheckOnActivate = true;
		}

		/// <summary>
		/// Updates the <see cref="System.Windows.Forms.ListView.Items"/> collection to contain a specific number of items.
		/// </summary>
		/// <param name="pItemCount">The number of items required.</param>
		/// <returns>True if items were added or removed.</returns>
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

		/// <summary>
		/// Updates a <see cref="ListViewItem"/> to contain a subitem for each column.
		/// </summary>
		/// <param name="pItem">The <see cref="ListViewItem"/> to update.</param>
		/// <seealso cref="UpdateSubItemCount (ListViewItem, int)"/>
		public Boolean UpdateSubItemCount (ListViewItem pItem)
		{
			return UpdateSubItemCount (pItem, Columns.Count);
		}

		/// <summary>
		/// Updates the <see cref="ListViewItem.SubItems"/> collection to contain a specific number of subitems.
		/// </summary>
		/// <param name="pItem">The <see cref="ListViewItem"/> to update.</param>
		/// <param name="pSubItemCount">The number of subitems required.</param>
		/// <returns>True if subitems were added or removed.</returns>
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

		/// <summary>
		/// Gets or sets the selected <see cref="ListViewItem"/> in single-selection mode.
		/// </summary>
		/// <remarks>Setting the selected item also sets the <see cref="ListView.FocusedItem"/></remarks>
		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
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

		/// <summary>
		/// Retrieves the selected <see cref="ListViewItem"/> in single-selection mode.
		/// </summary>
		/// <param name="pIncludeFocus">True if the <see cref="ListView.FocusedItem"/> should be returned when there is no selected item.</param>
		/// <returns>The selected (or focused) item, or null if not found.</returns>
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

		/// <summary>
		/// Gets or sets the index of the selected <see cref="ListViewItem"/> in single-selection mode.
		/// </summary>
		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
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

		/// <summary>
		/// Retrieves the index of the selected <see cref="ListViewItem"/> in single-selection mode.
		/// </summary>
		/// <param name="pIncludeFocus">True if the <see cref="ListView.FocusedItem"/> should be returned when there is no selected item.</param>
		/// <returns>The index of the selected (or focused) item, or -1 if not found.</returns>
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

		/// <summary>
		/// Updates a column's width to fully show both the header and content (whichever is wider).
		/// </summary>
		/// <param name="pColumnHeader">The <see cref="ColumnHeader"/> to update.</param>
		public void AutoSizeColumn (ColumnHeader pColumnHeader)
		{
			int lColumnWidth;

			pColumnHeader.AutoResize (ColumnHeaderAutoResizeStyle.HeaderSize);
			lColumnWidth = pColumnHeader.Width;
			pColumnHeader.AutoResize (ColumnHeaderAutoResizeStyle.ColumnContent);
			lColumnWidth = System.Math.Max (lColumnWidth, pColumnHeader.Width);
			pColumnHeader.Width = lColumnWidth;
		}

		///////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Determines if the checked state is changed when a list item is activated.
		/// </summary>
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

		/// <summary>
		/// Enables double buffering.
		/// </summary>
		protected override void OnCreateControl ()
		{
			this.DoubleBuffered = true;
			base.OnCreateControl ();
		}
	}
}
