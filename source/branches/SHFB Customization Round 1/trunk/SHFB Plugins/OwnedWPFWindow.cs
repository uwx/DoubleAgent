using System;
using System.Windows;

namespace SandcastleBuilder.PlugIns.CinSoft
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
			System.Windows.Window lActiveWindow = null;
			System.Windows.Forms.Form lActiveForm = null;
			System.Windows.Interop.WindowInteropHelper lInteropHelper = null;

			if (System.Windows.Application.Current != null)
			{
				lActiveWindow = System.Windows.Application.Current.MainWindow;

				if (lActiveWindow == null)
				{
					foreach (System.Windows.Window lWindow in System.Windows.Application.Current.Windows)
					{
						if (lWindow.IsActive)
						{
							lActiveWindow = lWindow;
							break;
						}
					}
				}
				while (lActiveWindow != null)
				{
					foreach (System.Windows.Window lWindow in lActiveWindow.OwnedWindows)
					{
						if (lWindow.IsActive)
						{
							lActiveWindow = lWindow;
							break;
						}
					}
				}
			}

			if (lActiveWindow != null)
			{
				Owner = lActiveWindow;
			}
			else
			{
				lActiveForm = System.Windows.Forms.Form.ActiveForm;
				if (lActiveForm == null)
				{
					foreach (System.Windows.Forms.Form lForm in System.Windows.Forms.Application.OpenForms)
					{
						if (lForm.Enabled)
						{
							lActiveForm = lForm;
							break;
						}
					}
				}
				if (lActiveForm != null)
				{
					lInteropHelper = new System.Windows.Interop.WindowInteropHelper (this);
					lInteropHelper.Owner = lActiveForm.Handle;
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
			System.Windows.Window lActiveWindow = null;
			System.Windows.Forms.Form lActiveForm = null;
			System.Windows.Interop.WindowInteropHelper lInteropHelper = null;

			if (System.Windows.Application.Current != null)
			{
				lActiveWindow = System.Windows.Application.Current.MainWindow;

				if (lActiveWindow == null)
				{
					foreach (System.Windows.Window lWindow in System.Windows.Application.Current.Windows)
					{
						if (lWindow.IsActive)
						{
							lActiveWindow = lWindow;
							break;
						}
					}
				}
				while (lActiveWindow != null)
				{
					foreach (System.Windows.Window lWindow in lActiveWindow.OwnedWindows)
					{
						if (lWindow.IsActive)
						{
							lActiveWindow = lWindow;
							break;
						}
					}
				}
			}

			if (lActiveWindow != null)
			{
				lInteropHelper = new System.Windows.Interop.WindowInteropHelper (lActiveWindow);
				return lInteropHelper.Handle;
			}
			else
			{
				lActiveForm = System.Windows.Forms.Form.ActiveForm;
				if (lActiveForm == null)
				{
					foreach (System.Windows.Forms.Form lForm in System.Windows.Forms.Application.OpenForms)
					{
						if (lForm.Enabled)
						{
							lActiveForm = lForm;
							break;
						}
					}
				}
				if (lActiveForm != null)
				{
					return lActiveForm.Handle;
				}
			}

			return (IntPtr)0;
		}
	}
}
