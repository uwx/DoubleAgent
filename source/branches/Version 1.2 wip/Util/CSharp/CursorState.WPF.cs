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
using System.Windows;
using System.Windows.Input;

namespace DoubleAgent
{
	/// <summary>
	/// For use as a local variable to save/restore a <see cref="System.Windows.Forms.Form"/>'s current <see cref="System.Windows.Forms.Cursor"/>.
	/// </summary>
	public class CursorState : IDisposable
	{
		/// <summary>
		/// The <see cref="System.Windows.Window"/> whose <see cref="System.Windows.Forms.Cursor"/> is being managed.
		/// </summary>
		public System.Windows.Window Window
		{
			get;
			set;
		}

		/// <summary>
		/// The <see cref="System.Windows.Window"/>'s <see cref="System.Windows.Input.Cursor"/> at the time this object was constructed.
		/// </summary>
		public System.Windows.Input.Cursor SavedCursor
		{
			get;
			set;
		}

		//=============================================================================

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="pForm">The <see cref="System.Windows.Window"/> whose <see cref="System.Windows.Input.Cursor"/> is being managed.</param>
		/// <seealso cref="RestoreCursor"/>
		/// <seealso cref="Form"/>
		/// <seealso cref="SavedCursor"/>
		public CursorState (System.Windows.Window pWindow)
		{
			this.Window = pWindow;
			this.SavedCursor = pWindow.Cursor;
		}
		~CursorState ()
		{
			Dispose (false);
		}

		public void Dispose ()
		{
			Dispose (true);
			GC.SuppressFinalize (this);
		}
		protected virtual void Dispose (bool disposing)
		{
			try
			{
				if (this.Window != null)
				{
					Mouse.OverrideCursor = null;
					this.Window.Cursor = this.SavedCursor;
					this.Window = null;
				}
			}
			catch
			{
			}
		}

		//=============================================================================

		/// <summary>
		/// Sets the managed <see cref="Window"/>'s current <see cref="System.Windows.Input.Cursor"/>.
		/// </summary>
		/// <remarks>This method will fail if <see cref="RestoreCursor"/> has already been called.</remarks>
		/// <param name="pCursor">The <see cref="System.Windows.Input.Cursor"/> to show.</param>
		/// <returns>True of successful</returns>
		public Boolean ShowCursor (System.Windows.Input.Cursor pCursor)
		{
			if (this.Window != null)
			{
				try
				{
					this.Window.Cursor = pCursor;
				}
				catch
				{
				}
				return true;
			}
			return false;
		}

		/// <summary>
		/// Restores the managed <see cref="Window"/>'s <see cref="System.Windows.Input.Cursor"/> to the <see cref="SavedCursor"/>
		/// </summary>
		/// <remarks>This method should be called once-and-only-once.</remarks>
		/// <returns>True if successful</returns>
		public Boolean RestoreCursor ()
		{
			Boolean lRet = false;
			if (this.Window != null)
			{
				lRet = true;
			}
			Dispose (false);
			return lRet;
		}

		//=============================================================================

		/// <summary>
		/// Sets the managed <see cref="Window"/>'s current <see cref="System.Windows.Input.Cursor"/> to <see cref="System.Windows.Input.Cursors.Wait"/>.
		/// </summary>
		/// <returns>True if successful</returns>
		/// <seealso cref="ShowCursor"/>
		public Boolean ShowWait ()
		{
			if (ShowCursor (System.Windows.Input.Cursors.Wait))
			{
				Mouse.OverrideCursor = this.Window.Cursor;
				return true;
			}
			return false;
		}
	}
}
