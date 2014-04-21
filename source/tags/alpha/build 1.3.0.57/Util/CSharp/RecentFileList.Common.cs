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

namespace DoubleAgent
{
	public partial class RecentFileList : List<String>
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		private int mMaxCount = 16;
		private Boolean mShowRelativeMostRecent = false;

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
		#region Methods
 
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
	}
}
