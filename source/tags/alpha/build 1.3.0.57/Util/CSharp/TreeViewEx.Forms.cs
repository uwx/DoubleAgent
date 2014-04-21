﻿/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
	/// A <see cref="System.Windows.Forms.TreeView"/> with a double-buffering enabled
	/// </summary>
	public partial class TreeViewEx : System.Windows.Forms.TreeView
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		/// <summary>
		/// Enables double buffering.
		/// </summary>
		protected override void OnCreateControl ()
		{
			this.DoubleBuffered = true;
			base.OnCreateControl ();
		}

		#endregion
	}
}
