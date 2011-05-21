﻿/////////////////////////////////////////////////////////////////////////////
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
#if WPF
using System.Windows.Controls;
#else
using System.Windows.Forms;
#endif

namespace DoubleAgent
{
	class RecentFileList : List<String>
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		private int mMaxCount = 16;
		private Boolean mShowRelativeMostRecent = false;
#if WPF
		private List<Control> mMenuItems = null;
#else
		private List <ToolStripItem>	mMenuItems = null;
#endif

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
				String lPath = null;

				try
				{
					lPath = Path.GetFullPath (value);
				}
				catch
				{
				}

				if (!String.IsNullOrEmpty (lPath))
				{
					for (int lNdx = this.Count - 1; lNdx >= 0; lNdx--)
					{
						if (String.Compare (this[lNdx], lPath, true) == 0)
						{
							this.RemoveAt (lNdx);
						}
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

#if WPF
		public Boolean ShowPaths (MenuItem pFileMenu)
		{
			return ShowPaths (pFileMenu, null);
		}
#else
		public Boolean ShowPaths (ToolStripDropDown pFileMenu)
		{
			return ShowPaths (pFileMenu, null);
		}
#endif

#if WPF
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
#else
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
#endif

		public Boolean RemovePath (String pPath)
		{
			Boolean lRet = false;
			String lPath = null;

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
				String lCurrent = System.IO.Directory.GetCurrentDirectory ();
				String lPath = Path.GetFullPath (pPath);

				if (!String.IsNullOrEmpty (lCurrent))
				{
					lCurrent = Path.GetFullPath (lCurrent);
					if (String.Compare (Path.GetDirectoryName (lPath), Path.GetDirectoryName (lCurrent), true) == 0)
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
				String lMostRecent = MostRecent;
				String lPath = Path.GetFullPath (pPath);

				if (!String.IsNullOrEmpty (lMostRecent))
				{
					if (String.Compare (Path.GetDirectoryName (lPath), Path.GetDirectoryName (lMostRecent), true) == 0)
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

		public delegate void RecentItemClickEventHandler (object sender, String e);
		public event RecentItemClickEventHandler RecentItemClick;

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

#if WPF
		private void RecentMenuItem_Click (object sender, System.Windows.RoutedEventArgs e)
		{
			if (RecentItemClick != null)
			{
				RecentItemClick (this, (String)(e.Source as MenuItem).Tag);
			}
			e.Handled = true;
		}
#else
		private void RecentMenuItem_Click (object sender, EventArgs e)
		{
			if (RecentItemClick != null)
			{
				RecentItemClick (this, (String)((ToolStripItem)sender).Tag);
			}
		}
#endif

		#endregion
	}
}
