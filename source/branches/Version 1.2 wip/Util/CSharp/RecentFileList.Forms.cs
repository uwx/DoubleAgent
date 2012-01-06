/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2012 Cinnamon Software Inc.
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
using System.Windows.Forms;

namespace DoubleAgent
{
	public partial class RecentFileList : List<String>
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private List<ToolStripItem> mMenuItems = null;

		public Boolean ShowPaths (ToolStripDropDown pFileMenu)
		{
			return ShowPaths (pFileMenu, null);
		}

		public Boolean ShowPaths (ToolStripDropDown pFileMenu, ToolStripItem pAfterItem)
		{
			try
			{
				if (mMenuItems != null)
				{
					foreach (ToolStripItem lMenuItem in mMenuItems)
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

					mMenuItems = new List<ToolStripItem> ();

					mMenuItems.Add (new ToolStripSeparator ());
					foreach (String lPath in this)
					{
						ToolStripMenuItem lMenuItem;

						lMenuItem = new ToolStripMenuItem ();
						if (mShowRelativeMostRecent)
						{
							lMenuItem.Text = (++lItemNdx).ToString () + " " + RelativeMostRecent (lPath);
						}
						else
						{
							lMenuItem.Text = (++lItemNdx).ToString () + " " + RelativeCurrent (lPath);
						}
						lMenuItem.Tag = lPath;
						lMenuItem.Click += new EventHandler (this.RecentMenuItem_Click);
						mMenuItems.Add (lMenuItem);
					}
					if ((pAfterItem == null) || (!pFileMenu.Items.Contains (pAfterItem)))
					{
						pFileMenu.Items.AddRange (mMenuItems.ToArray ());
					}
					else
					{
						lItemNdx = pFileMenu.Items.IndexOf (pAfterItem);

						foreach (ToolStripItem lMenuItem in mMenuItems)
						{
							pFileMenu.Items.Insert (++lItemNdx, lMenuItem);
						}
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

		private void RecentMenuItem_Click (object sender, EventArgs e)
		{
			if (RecentItemClick != null)
			{
				RecentItemClick (this, (String)((ToolStripItem)sender).Tag);
			}
		}

		#endregion
	}
}
