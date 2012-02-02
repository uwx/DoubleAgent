using System;
using System.Windows;

namespace SandcastleBuilder.PlugIns.CinSoft
{
	public partial class OwnedWPFWindow : Window
	{
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
