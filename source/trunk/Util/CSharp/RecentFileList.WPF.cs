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
using System.Collections.Generic;
using System.IO;
using System.Windows.Controls;

namespace DoubleAgent
{
	public partial class RecentFileList : List<String>
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private List<Control> mMenuItems = null;

		public Boolean ShowPaths (MenuItem pFileMenu)
		{
			return ShowPaths (pFileMenu, null);
		}

		public Boolean ShowPaths (MenuItem pFileMenu, MenuItem pAfterItem)
		{
			try
			{
				if (mMenuItems != null)
				{
					foreach (Control lMenuItem in mMenuItems)
					{
						try
						{
							pFileMenu.Items.Remove (lMenuItem);
						}
						catch
						{
						}
					}
				}
				if (this.Count > 0)
				{
					int lItemNdx = 0;

					mMenuItems = new List<Control> ();

					mMenuItems.Add (new Separator ());
					foreach (String lPath in this)
					{
						MenuItem lMenuItem;

						lMenuItem = new MenuItem ();
						if (mShowRelativeMostRecent)
						{
							lMenuItem.Header = (++lItemNdx).ToString () + " " + RelativeMostRecent (lPath);
						}
						else
						{
							lMenuItem.Header = (++lItemNdx).ToString () + " " + RelativeCurrent (lPath);
						}
						lMenuItem.Tag = lPath;
						lMenuItem.Click += new System.Windows.RoutedEventHandler (this.RecentMenuItem_Click);
						mMenuItems.Add (lMenuItem);
					}
					if ((pAfterItem != null) && (pFileMenu.Items.Contains (pAfterItem)))
					{
						lItemNdx = pFileMenu.Items.IndexOf (pAfterItem);
					}

					foreach (Control lMenuItem in mMenuItems)
					{
						pFileMenu.Items.Insert (++lItemNdx, lMenuItem);
					}
					return true;
				}
			}
			catch
			{
			}
			return false;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void RecentMenuItem_Click (object sender, System.Windows.RoutedEventArgs e)
		{
			if (RecentItemClick != null)
			{
				RecentItemClick (this, (String)(e.Source as MenuItem).Tag);
			}
			e.Handled = true;
		}

		#endregion
	}
}
