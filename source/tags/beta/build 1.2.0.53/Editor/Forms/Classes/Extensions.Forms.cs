/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using DoubleAgent.Character;
using AgentCharacterEditor;

namespace AgentCharacterEditor.Global
{
	public static partial class StringExtensions
	{
		/// <summary>
		/// Updates the mnemonic prefix indicator ('&amp;') in a string.
		/// </summary>
		/// <param name="pString">The initial string value.</param>
		/// <returns>The string value with any mnemonic prefix updated for the current application build.</returns>
		static public String FixMenuPrefix (this String pString)
		{
			return String.IsNullOrEmpty (pString) ? String.Empty : pString;
		}

		/// <summary>
		/// Strips the mnemonic prefix indicator ('&amp;') from a string.
		/// </summary>
		/// <param name="pString">The initial string value.</param>
		/// <returns>The string value with any mnemonic prefix removed.</returns>
		static public String NoMenuPrefix (this String pString)
		{
			return String.IsNullOrEmpty (pString) ? String.Empty : pString.Replace ("&", "");
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public static partial class ListViewExtensions
	{
		/// <summary>
		/// Updates the <see cref="System.Windows.Forms.ListView.Items"/> collection to contain a specific number of items.
		/// </summary>
		/// <param name="pItemCount">The number of items required.</param>
		/// <returns>True if items were added or removed.</returns>
		public static Boolean UpdateItemCount (this ListView pListView, int pItemCount)
		{
			Boolean lRet = false;

			while (pListView.Items.Count > pItemCount)
			{
				pListView.Items.RemoveAt (pListView.Items.Count - 1);
				lRet = true;
			}
			while (pListView.Items.Count < pItemCount)
			{
				pListView.Items.Add (new ListViewItemCommon (""));
				lRet = true;
			}
			return lRet;
		}

		/// <summary>
		/// Updates a <see cref="ListViewItem"/> to contain a subitem for each column.
		/// </summary>
		/// <param name="pItem">The <see cref="ListViewItem"/> to update.</param>
		/// <seealso cref="UpdateSubItemCount (ListViewItem, int)"/>
		public static Boolean UpdateSubItemCount (this ListView pListView, ListViewItem pItem)
		{
			return UpdateSubItemCount (pItem, pListView.Columns.Count);
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
	}
}
