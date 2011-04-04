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

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Helper Methods

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
		#region Event Delegates

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
