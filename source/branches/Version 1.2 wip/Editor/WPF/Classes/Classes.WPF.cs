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
using System.Windows;
using System.Windows.Controls;

namespace AgentCharacterEditor
{
	public class ListViewItemCommon : System.Windows.Controls.ListViewItem
	{
		public ListViewItemCommon ()
		{
			InitializeComponent ();
		}
		public ListViewItemCommon (string text)
		{
			InitializeComponent ();
			this.Content = text;
		}
		public ListViewItemCommon (string text, int imageIndex)
		{
			InitializeComponent ();
			this.Content = text;
		}

		/////////////////////////////////////////////////////////////////////////////

		protected virtual void InitializeComponent ()
		{
			try
			{
				ResourceDictionary lResources = Program.Current.Resources;
				Style = lResources["ListViewItemCommon"] as Style;
				//Template = lResources["ListViewItemTemplate"] as ControlTemplate;
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
		}
	}
}
