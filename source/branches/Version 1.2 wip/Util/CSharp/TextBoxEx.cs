using System;
using System.Windows.Forms;
using System.ComponentModel;

namespace DoubleAgent
{
	public class TextBoxEx : System.Windows.Forms.TextBox
	{
		private const int WM_KEYDOWN = 0x0100;

		public TextBoxEx ()
		{
			AcceptsReturn = true;
		}

		protected override bool ProcessCmdKey (ref Message pMessage, Keys pKeyData)
		{
			if ((pMessage.Msg == WM_KEYDOWN) && (pKeyData == Keys.Return) && (this.AcceptsReturn))
			{
				ValidateNow ();
				return true;
			}
			return base.ProcessCmdKey (ref pMessage, pKeyData);
		}

		private bool ValidateNow ()
		{
			if (CausesValidation)
			{
				CancelEventArgs	lEventArgs = new CancelEventArgs ();
				OnValidating (lEventArgs);
				if (!lEventArgs.Cancel)
				{
					OnValidated (new EventArgs ());
					return true;
				}
			}
			return false;
		}
	}
}
