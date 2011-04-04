using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;

namespace AgentCharacterEditor
{
	partial class AboutBox : Form
	{
		public AboutBox ()
		{
			InitializeComponent ();

			//LogoPictureBox.Image = Properties.Resources.DoubleAgent.ToBitmap ();
			
			Text = String.Format ("About {0}", Program.AssemblyTitle);
			LabelProductName.Text = Program.AssemblyTitle;
			LabelVersion.Text = String.Format ("Version {0}", Program.AssemblyVersion);
			LabelCopyright.Text = Program.AssemblyCopyright;
			LabelCompanyName.Text = Program.AssemblyCompany;
			TextBoxDescription.Text = Program.AssemblyDescription;
		}
	}
}
