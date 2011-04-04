using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Imaging;
using System.Reflection;

namespace AgentCharacterEditor
{
	static class Program
	{
		[STAThread]
		static void Main ()
		{
			Application.EnableVisualStyles ();
			Application.SetCompatibleTextRenderingDefault (false);
			Application.Run (sMainForm = new MainForm ());
		}

		static public MainForm MainForm
		{
			get
			{
				return sMainForm;
			}
		}

		static private AgentCharacterEditor.MainForm sMainForm = null;

		///////////////////////////////////////////////////////////////////////////////
		#region Utility Functions - PictureBox

		static internal void ResizeImageControl (PictureBox pImageControl)
		{
			ResizeImageControl (pImageControl.Image.Size, pImageControl, pImageControl);
		}

		static internal void ResizeImageControl (PictureBox pImageControl, Control pResizeControl)
		{
			ResizeImageControl (pImageControl.Image.Size, pImageControl, pResizeControl);
		}

		static internal void ResizeImageControl (Size pImageSize, Control pImageControl)
		{
			ResizeImageControl (pImageSize, pImageControl, pImageControl);
		}

		static internal void ResizeImageControl (Size pImageSize, Control pImageControl, Control pResizeControl)
		{
			Size	lImageSize = pImageControl.ClientSize;
			Size	lControlSize = pResizeControl.Size;

			lControlSize.Width += pImageSize.Width - lImageSize.Width;
			lControlSize.Height += pImageSize.Height - lImageSize.Height;
			pResizeControl.Size = lControlSize;

			if ((pResizeControl != pImageControl) && (pImageControl.ClientSize != pImageSize))
			{
				lImageSize = pImageControl.ClientSize;
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("ResizeImage [{0:D}x{1:D}] with [{2:D}x{3:D}] gives [{4:D}x{5:D}]", pImageSize.Width, pImageSize.Height, lControlSize.Width, lControlSize.Height, lImageSize.Width, lImageSize.Height);
#endif
				lControlSize.Width += lImageSize.Width - pImageSize.Width;
				lControlSize.Height += lImageSize.Height - pImageSize.Height;
				pResizeControl.Size = lControlSize;
#if DEBUG_NOT
				lImageSize = pImageControl.ClientSize;
				System.Diagnostics.Debug.Print ("ResizeImage [{0:D}x{1:D}] with [{2:D}x{3:D}] gives [{4:D}x{5:D}]", pImageSize.Width, pImageSize.Height, lControlSize.Width, lControlSize.Height, lImageSize.Width, lImageSize.Height);
#endif
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		static internal double ScaleImageControl (PictureBox pImageControl)
		{
			return ScaleImageControl (pImageControl.Image.Size, pImageControl);
		}

		static internal double ScaleImageControl (PictureBox pImageControl, Control pScaleToControl)
		{
			return ScaleImageControl (pImageControl.Image.Size, pImageControl, pScaleToControl);
		}

		static internal double ScaleImageControl (Size pImageSize, PictureBox pImageControl)
		{
			return ScaleImageControl (pImageSize, pImageControl, pImageControl);
		}

		static internal double ScaleImageControl (Size pImageSize, PictureBox pImageControl, Control pScaleToControl)
		{
			double	lScale = 1.0;
			Size	lImageSize = pScaleToControl.ClientSize;

			if (lImageSize != pImageSize)
			{
				lScale = Math.Min ((double)lImageSize.Width / (double)pImageSize.Width, (double)lImageSize.Height / (double)pImageSize.Height);

				lImageSize = Size.Round (new SizeF ((float)pImageControl.Image.Width * (float)lScale, (float)pImageControl.Image.Height * (float)lScale));
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("ScaleImage [{0:D}x{1:D}] from [{2:D}x{3:D}] to [{4:D}x{5:D}]", pImageSize.Width, pImageSize.Height, pImageControl.ClientSize.Width, pImageControl.ClientSize.Height, lImageSize.Width, lImageSize.Height);
#endif
				lImageSize.Width += pImageControl.Size.Width - pImageControl.ClientSize.Width;
				lImageSize.Height += pImageControl.Size.Height - pImageControl.ClientSize.Height;
				pImageControl.Size = lImageSize;
			}
			return lScale;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Assembly Attribute Accessors

		static public string AssemblyTitle
		{
			get
			{
				object[] attributes = Assembly.GetExecutingAssembly ().GetCustomAttributes (typeof (AssemblyTitleAttribute), false);
				if (attributes.Length > 0)
				{
					AssemblyTitleAttribute titleAttribute = (AssemblyTitleAttribute)attributes[0];
					if (titleAttribute.Title != "")
					{
						return titleAttribute.Title;
					}
				}
				return System.IO.Path.GetFileNameWithoutExtension (Assembly.GetExecutingAssembly ().CodeBase);
			}
		}

		static public string AssemblyVersion
		{
			get
			{
				return Assembly.GetExecutingAssembly ().GetName ().Version.ToString ();
			}
		}

		static public string AssemblyDescription
		{
			get
			{
				object[] attributes = Assembly.GetExecutingAssembly ().GetCustomAttributes (typeof (AssemblyDescriptionAttribute), false);
				if (attributes.Length == 0)
				{
					return "";
				}
				return ((AssemblyDescriptionAttribute)attributes[0]).Description;
			}
		}

		static public string AssemblyProduct
		{
			get
			{
				object[] attributes = Assembly.GetExecutingAssembly ().GetCustomAttributes (typeof (AssemblyProductAttribute), false);
				if (attributes.Length == 0)
				{
					return "";
				}
				return ((AssemblyProductAttribute)attributes[0]).Product;
			}
		}

		static public string AssemblyCopyright
		{
			get
			{
				object[] attributes = Assembly.GetExecutingAssembly ().GetCustomAttributes (typeof (AssemblyCopyrightAttribute), false);
				if (attributes.Length == 0)
				{
					return "";
				}
				return ((AssemblyCopyrightAttribute)attributes[0]).Copyright;
			}
		}

		static public string AssemblyCompany
		{
			get
			{
				object[] attributes = Assembly.GetExecutingAssembly ().GetCustomAttributes (typeof (AssemblyCompanyAttribute), false);
				if (attributes.Length == 0)
				{
					return "";
				}
				return ((AssemblyCompanyAttribute)attributes[0]).Company;
			}
		}
		#endregion
		///////////////////////////////////////////////////////////////////////////////
	}
}
