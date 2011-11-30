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
using System.Windows.Forms;

namespace AgentCharacterEditor.About
{
	partial class AboutBox : Form
	{
		public AboutBox ()
		{
			InitializeComponent ();

			//LogoPictureBox.Image = Resources.DoubleAgent.ToBitmap ();
			
			Text = String.Format ("About {0}", Program.AssemblyTitle);
			LabelProductName.Text = Program.AssemblyTitle;
			LabelVersion.Text = String.Format ("Version {0}", Program.AssemblyVersion);
			LabelCopyright.Text = Program.AssemblyCopyright;
			TextBoxLicenseIntro.Rtf = Properties.Resources.LicenseIntro;
		}

		private void LicenseButton_Click (object sender, EventArgs e)
		{
			(new AgentCharacterEditor.About.LicenseBox ()).ShowDialog (this);
		}

		private void TextBoxLicenseIntro_LinkClicked (object sender, LinkClickedEventArgs e)
		{
			System.Diagnostics.Process.Start (e.LinkText);
		}
	}
}
