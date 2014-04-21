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

namespace DoubleAgent
{
	/// <summary>
	/// A <see cref="System.Windows.Forms.ListView"/> with a bit of extra functionality an double-buffering enabled
	/// </summary>
	public partial class ListViewEx : System.Windows.Forms.ListView
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		/// <summary>
		/// Constructor
		/// </summary>
		public ListViewEx ()
		{
			this.CheckOnActivate = true;
		}

		/// <summary>
		/// Enables double buffering.
		/// </summary>
		protected override void OnCreateControl ()
		{
			this.DoubleBuffered = true;
			base.OnCreateControl ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Single selection

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
				return GetSelectedItem ();
			}
			set
			{
				SetSelectedItem (value);
			}
		}

		/// <summary>
		/// Retrieves the selected <see cref="ListViewItem"/> in single-selection mode.
		/// </summary>
		/// <returns>The selected item, or null if not found.</returns>
		public ListViewItem GetSelectedItem ()
		{
			if (SelectedIndices.Count == 1)
			{
				return SelectedItems[0];
			}
			else
			{
				return null;
			}
		}

		public void SetSelectedItem (ListViewItem pSelectedItem)
		{
			if (pSelectedItem == null)
			{
				SelectedItems.Clear ();
			}
			else if (Items.Contains (pSelectedItem))
			{
				if ((SelectedItems.Count != 1) || (!SelectedItems.Contains (pSelectedItem)))
				{
					SelectedItems.Clear ();
				}
				pSelectedItem.Selected = true;
				pSelectedItem.Focused = true;
			}
		}

		//=============================================================================

		/// <summary>
		/// Gets or sets the index of the selected <see cref="ListViewItem"/> in single-selection mode.
		/// </summary>
		/// <remarks>Setting the selected item also sets the <see cref="ListView.FocusedItem"/></remarks>
		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public int SelectedIndex
		{
			get
			{
				return GetSelectedIndex ();
			}
			set
			{
				SetSelectedIndex (value);
			}
		}

		/// <summary>
		/// Retrieves the index of the selected <see cref="ListViewItem"/> in single-selection mode.
		/// </summary>
		/// <returns>The index of the selected item, or -1 if not found.</returns>
		public int GetSelectedIndex ()
		{
			if (SelectedIndices.Count == 1)
			{
				return SelectedIndices[0];
			}
			else
			{
				return -1;
			}
		}

		public void SetSelectedIndex (int pSelectedIndex)
		{
			pSelectedIndex = Math.Min (pSelectedIndex, Items.Count - 1);
			if (pSelectedIndex >= 0)
			{
				SelectedItem = Items[pSelectedIndex];
			}
			else
			{
				SelectedItem = null;
			}
		}

		//=============================================================================

		/// <summary>
		/// Gets the index of the <see cref="ListView.FocusedItem"/>.
		/// </summary>
		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public int FocusedIndex
		{
			get
			{
				ListViewItem lFocusedItem = FocusedItem;
				return (lFocusedItem == null) ? -1 : lFocusedItem.Index;
			}
			set
			{
				if ((value >= 0) && (value < Items.Count))
				{
					FocusedItem = Items[value];
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Column sizing

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

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Pseudo-readonly

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

		#endregion
	}
}
