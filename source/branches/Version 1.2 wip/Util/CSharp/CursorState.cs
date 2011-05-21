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
#if WPF
using System.Windows;
using System.Windows.Input;
#else
using System.Windows.Forms;
#endif

namespace DoubleAgent
{
	/// <summary>
	/// For use as a local variable to save/restore a <see cref="System.Windows.Forms.Form"/>'s current <see cref="System.Windows.Forms.Cursor"/>.
	/// </summary>
	public class CursorState
	{
#if WPF
		/// <summary>
		/// The <see cref="System.Windows.Window"/> whose <see cref="System.Windows.Forms.Cursor"/> is being managed.
		/// </summary>
		public System.Windows.Window Window
		{
			get;
			set;
		}
#else
		/// <summary>
		/// The <see cref="System.Windows.Forms.Form"/> whose <see cref="System.Windows.Forms.Cursor"/> is being managed.
		/// </summary>
		public System.Windows.Forms.Form Form
		{
			get;
			set;
		}
#endif

#if WPF
		/// <summary>
		/// The <see cref="System.Windows.Window"/>'s <see cref="System.Windows.Input.Cursor"/> at the time this object was constructed.
		/// </summary>
		public System.Windows.Input.Cursor SavedCursor
		{
			get;
			set;
		}
#else
		/// <summary>
		/// The <see cref="System.Windows.Forms.Form"/>'s <see cref="System.Windows.Forms.Cursor"/> at the time this object was constructed.
		/// </summary>
		public System.Windows.Forms.Cursor SavedCursor
		{
			get;
			set;
		}
#endif

#if WPF
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
#else
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="pForm">The <see cref="System.Windows.Forms.Form"/> whose <see cref="System.Windows.Forms.Cursor"/> is being managed.</param>
		/// <seealso cref="RestoreCursor"/>
		/// <seealso cref="Form"/>
		/// <seealso cref="SavedCursor"/>
		public CursorState (System.Windows.Forms.Form pForm)
		{
			this.Form = pForm;
			this.SavedCursor = pForm.Cursor;
		}
#endif

		~CursorState ()
		{
			RestoreCursor ();
		}

		///////////////////////////////////////////////////////////////////////////////

#if WPF
		/// <summary>
		/// Sets the managed <see cref="Window"/>'s current <see cref="System.Windows.Input.Cursor"/>.
		/// </summary>
		/// <remarks>This method will fail if <see cref="RestoreCursor"/> has already been called.</remarks>
		/// <param name="pCursor">The <see cref="System.Windows.Input.Cursor"/> to show.</param>
		/// <returns>True of successful</returns>
		public Boolean ShowCursor (System.Windows.Input.Cursor pCursor)
		{
			if ((pCursor != null) && (this.SavedCursor != null) && (this.Window != null))
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
#else
		/// <summary>
		/// Sets the managed <see cref="Form"/>'s current <see cref="System.Windows.Forms.Cursor"/>.
		/// </summary>
		/// <remarks>This method will fail if <see cref="RestoreCursor"/> has already been called.</remarks>
		/// <param name="pCursor">The <see cref="System.Windows.Forms.Cursor"/> to show.</param>
		/// <returns>True of successful</returns>
		public Boolean ShowCursor (System.Windows.Forms.Cursor pCursor)
		{
			if ((pCursor != null) && (this.SavedCursor != null) && (this.Form != null))
			{
				try
				{
					this.Form.Cursor = pCursor;
				}
				catch
				{
				}
				return true;
			}
			return false;
		}
#endif

#if WPF
		/// <summary>
		/// Restores the managed <see cref="Window"/>'s <see cref="System.Windows.Input.Cursor"/> to the <see cref="SavedCursor"/>
		/// </summary>
		/// <remarks>This method should be called once-and-only-once.</remarks>
		/// <returns>True if successful</returns>
		public Boolean RestoreCursor ()
		{
			if ((this.SavedCursor != null) && (this.Window != null))
			{
				try
				{
					this.Window.Cursor = this.SavedCursor;
					this.SavedCursor = null;
				}
				catch
				{
				}
				return true;
			}
			return false;
		}
#else
		/// <summary>
		/// Restores the managed <see cref="Form"/>'s <see cref="System.Windows.Forms.Cursor"/> to the <see cref="SavedCursor"/>
		/// </summary>
		/// <remarks>This method should be called once-and-only-once.</remarks>
		/// <returns>True if successful</returns>
		public Boolean RestoreCursor ()
		{
			if ((this.SavedCursor != null) && (this.Form != null))
			{
				try
				{
					this.Form.Cursor = this.SavedCursor;
					this.SavedCursor = null;
				}
				catch
				{
				}
				return true;
			}
			return false;
		}
#endif

		///////////////////////////////////////////////////////////////////////////////

#if WPF
		/// <summary>
		/// Sets the managed <see cref="Window"/>'s current <see cref="System.Windows.Input.Cursor"/> to <see cref="System.Windows.Input.Cursors.Wait"/>.
		/// </summary>
		/// <returns>True if successful</returns>
		/// <seealso cref="ShowCursor"/>
		public Boolean ShowWait ()
		{
			return ShowCursor (System.Windows.Input.Cursors.Wait);
		}
#else
		/// <summary>
		/// Sets the managed <see cref="Form"/>'s current <see cref="System.Windows.Forms.Cursor"/> to <see cref="System.Windows.Forms.Cursors.WaitCursor"/>.
		/// </summary>
		/// <returns>True if successful</returns>
		/// <seealso cref="ShowCursor"/>
		public Boolean ShowWait ()
		{
			return ShowCursor (System.Windows.Forms.Cursors.WaitCursor);
		}
#endif
	}
}
