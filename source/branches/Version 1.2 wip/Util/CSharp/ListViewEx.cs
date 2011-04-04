using System;
using System.Windows.Forms;
using System.ComponentModel;

namespace DoubleAgent
{
	public class ListViewEx : System.Windows.Forms.ListView
	{
		public bool UpdateItemCount (int pItemCount)
		{
			bool lRet = false;

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

		public bool UpdateSubItemCount (ListViewItem pItem)
		{
			return UpdateSubItemCount (pItem, Columns.Count);
		}

		static public bool UpdateSubItemCount (ListViewItem pItem, int pSubItemCount)
		{
			bool lRet = false;

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

		public ListViewItem GetSelectedItem (bool pIncludeFocus)
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

		public int GetSelectedIndex (bool pIncludeFocus)
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

		protected override void OnCreateControl ()
		{
			this.DoubleBuffered = true;
			base.OnCreateControl ();
		}
	}
}
