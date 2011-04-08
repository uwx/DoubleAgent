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
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;

namespace DoubleAgent
{
	class RecentFileList : List<String>
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		private int						mMaxCount = 16;
		private bool					mShowRelativeMostRecent = false;
		private List <ToolStripItem>	mMenuItems = null;

		public String MostRecent
		{
			get
			{
				if (this.Count > 0)
				{
					return this[0];
				}
				return null;
			}
			set
			{
				String	lPath = null;

				try
				{
					lPath = Path.GetFullPath (value);
				}
				catch
				{
				}

				if (!String.IsNullOrEmpty (lPath))
				{
					if (this.Contains (lPath))
					{
						this.Remove (lPath);
					}
					this.Insert (0, lPath);
					mShowRelativeMostRecent = true;
				}
				while (this.Count > MaxCount)
				{
					this.RemoveAt (this.Count - 1);
				}
			}
		}

		public int MaxCount
		{
			get
			{
				return mMaxCount;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		public bool ShowPaths (ToolStripDropDown pFileMenu)
		{
			return ShowPaths (pFileMenu, null);
		}

		public bool ShowPaths (ToolStripDropDown pFileMenu, ToolStripItem pAfterItem)
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
				int	lItemNdx = 0;

				mMenuItems = new List<ToolStripItem> ();

				mMenuItems.Add (new ToolStripSeparator ());
				foreach (String lPath in this)
				{
					ToolStripMenuItem	lMenuItem;

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
			return false;
		}
		
		public bool RemovePath (String pPath)
		{
			bool	lRet = false;
			String	lPath = null;

			try
			{
				lPath = Path.GetFullPath (pPath);
			}
			catch
			{
			}

			if (!String.IsNullOrEmpty (lPath))
			{
				for (int lNdx = 0; lNdx < this.Count; lNdx++)
				{
					if (String.Compare (this[lNdx], lPath, true) == 0)
					{
						this.RemoveAt (lNdx);
						lRet = true;
						break;
					}
				}
			}
			return lRet;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		internal String RelativeCurrent (String pPath)
		{
			try
			{
				String	lCurrent = System.IO.Directory.GetCurrentDirectory ();
				String	lPath = Path.GetFullPath (pPath);

				if (!String.IsNullOrEmpty (lCurrent))
				{
					lCurrent = Path.GetFullPath (lCurrent);
					if (Path.GetDirectoryName (lPath).Equals (Path.GetDirectoryName (lCurrent), StringComparison.InvariantCultureIgnoreCase))
					{
						return Path.GetFileName (lPath);
					}
				}
				return lPath;
			}
			catch
			{
				return pPath;
			}
		}

		internal String RelativeMostRecent (String pPath)
		{
			try
			{
				String	lMostRecent = MostRecent;
				String	lPath = Path.GetFullPath (pPath);

				if (!String.IsNullOrEmpty (lMostRecent))
				{
					if (Path.GetDirectoryName (lPath).Equals (Path.GetDirectoryName (lMostRecent), StringComparison.InvariantCultureIgnoreCase))
					{
						return Path.GetFileName (lPath);
					}
				}
				return lPath;
			}
			catch
			{
				return pPath;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public delegate void RecentItemClickEvent (object sender, String e);

		public event RecentItemClickEvent RecentItemClick;

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
