/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace AgentCharacterEditor.About
{
	public partial class AboutBox : Window
	{
		public AboutBox ()
		{
			InitializeComponent ();

			Title = String.Format ("About {0}", Program.AssemblyTitle);
			LabelProductName.Content = Program.AssemblyTitle;
			LabelVersion.Content = Program.AssemblyVersion;
			LabelCopyright.Content = Program.AssemblyCopyright;
		}

		private void ButtonLicense_Click (object sender, RoutedEventArgs e)
		{
			AgentCharacterEditor.About.LicenseBox lDialog = new AgentCharacterEditor.About.LicenseBox ();

			lDialog.Owner = this;
			lDialog.ShowDialog ();
		}
	}
}
