using System;
using System.Windows;

namespace SandcastleBuilder.PlugIns
{
	/// <summary>
	/// This class is a kludge that allows a WPF window to be shown as a modal dialog
	/// in either a WPF application or a Forms application.
	/// </summary>
	public partial class OwnedWPFWindow : Window
	{
		/// <summary>
		/// This overload checks the current execution environment, and
		/// sets the <see cref="P:System.Windows.Window.Owner" /> property accordingly.
		/// </summary>
		/// <returns>
		/// A <see cref="T:System.Nullable`1"/> value of type <see cref="T:System.Boolean"/> that specifies whether the dialog
		/// was accepted (<b>true</b>) or canceled (<b>false</b>).
		/// The return value is the value of the <see cref="P:System.Windows.Window.DialogResult"/> property before a window closes. 
		/// </returns>
		public new bool? ShowDialog ()
		{
			System.Windows.Window v_activeWindow = null;
			System.Windows.Forms.Form v_activeForm = null;
			System.Windows.Interop.WindowInteropHelper v_interopHelper = null;

			if (System.Windows.Application.Current != null)
			{
				v_activeWindow = System.Windows.Application.Current.MainWindow;

				if (v_activeWindow == null)
				{
					foreach (System.Windows.Window v_window in System.Windows.Application.Current.Windows)
					{
						if (v_window.IsActive)
						{
							v_activeWindow = v_window;
							break;
						}
					}
				}
				while (v_activeWindow != null)
				{
					foreach (System.Windows.Window v_window in v_activeWindow.OwnedWindows)
					{
						if (v_window.IsActive)
						{
							v_activeWindow = v_window;
							break;
						}
					}
				}
			}

			if (v_activeWindow != null)
			{
				Owner = v_activeWindow;
			}
			else
			{
				v_activeForm = System.Windows.Forms.Form.ActiveForm;
				if (v_activeForm == null)
				{
					foreach (System.Windows.Forms.Form lForm in System.Windows.Forms.Application.OpenForms)
					{
						if (lForm.Enabled)
						{
							v_activeForm = lForm;
							break;
						}
					}
				}
				if (v_activeForm != null)
				{
					v_interopHelper = new System.Windows.Interop.WindowInteropHelper (this);
					v_interopHelper.Owner = v_activeForm.Handle;
				}
			}

			return base.ShowDialog ();
		}

		/// <summary>
		/// This method determines if there is either an active WPF window or an active
		/// Form and returns its native handle.  The return value is useful as the owner
		/// window for modal dialogs (like MessageBox).
		/// </summary>
		/// <returns>The native window handle of the application's active window (if any).</returns>
		static public IntPtr GetDialogOwner ()
		{
			System.Windows.Window v_activeWindow = null;
			System.Windows.Forms.Form v_activeForm = null;
			System.Windows.Interop.WindowInteropHelper v_interopHelper = null;

			if (System.Windows.Application.Current != null)
			{
				v_activeWindow = System.Windows.Application.Current.MainWindow;

				if (v_activeWindow == null)
				{
					foreach (System.Windows.Window v_window in System.Windows.Application.Current.Windows)
					{
						if (v_window.IsActive)
						{
							v_activeWindow = v_window;
							break;
						}
					}
				}
				while (v_activeWindow != null)
				{
					foreach (System.Windows.Window v_window in v_activeWindow.OwnedWindows)
					{
						if (v_window.IsActive)
						{
							v_activeWindow = v_window;
							break;
						}
					}
				}
			}

			if (v_activeWindow != null)
			{
				v_interopHelper = new System.Windows.Interop.WindowInteropHelper (v_activeWindow);
				return v_interopHelper.Handle;
			}
			else
			{
				v_activeForm = System.Windows.Forms.Form.ActiveForm;
				if (v_activeForm == null)
				{
					foreach (System.Windows.Forms.Form lForm in System.Windows.Forms.Application.OpenForms)
					{
						if (lForm.Enabled)
						{
							v_activeForm = lForm;
							break;
						}
					}
				}
				if (v_activeForm != null)
				{
					return v_activeForm.Handle;
				}
			}

			return (IntPtr)0;
		}
	}
}
