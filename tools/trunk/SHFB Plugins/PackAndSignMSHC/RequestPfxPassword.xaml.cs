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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Security;

namespace SandcastleBuilder.PlugIns.CinSoft
{
	public partial class RequestPfxPassword : OwnedWPFWindow
	{
		public RequestPfxPassword (String pFileName)
		{
			InitializeComponent ();
			PromptLabel.Content = String.Format (PromptLabel.Content as String, pFileName);
		}

		public SecureString SecurePassword { get; protected set; }

		private void OnOK (object sender, RoutedEventArgs e)
		{
			SecurePassword = EnterPasswordBox.SecurePassword;
			DialogResult = true;
			Close ();
		}

		private void OnCancel (object sender, RoutedEventArgs e)
		{
			DialogResult = false;
			Close ();
		}

		private void OnActivated (object sender, EventArgs e)
		{
			EnterPasswordBox.Focus();
		}
	}
}
