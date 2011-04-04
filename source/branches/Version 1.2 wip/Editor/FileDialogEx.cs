using System;
using System.Windows.Forms;
using System.ComponentModel;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;
using Microsoft.Win32;

namespace AgentCharacterEditor
{
	public class OpenFileDialogEx
	{
		static public bool OpenCharacterFile (ref String pFilePath)
		{
			OpenFileDialog	lDialog = new OpenFileDialog ();
			StringBuilder	lFilter = new StringBuilder ();

			lFilter.Append (FileExtTypeName (".acd", "Character Definition Files") + " (*.acd)|*.acd");
			lFilter.Append ("|" + FileExtTypeName (".acs", "Character Files") + " (*.acs)|*.acs");
			lFilter.Append ("|All Files (*.*)|*.*");
			lDialog.Filter = lFilter.ToString ();
			lDialog.DefaultExt = ".acd";

			InitFilePath (lDialog, pFilePath);
			if (lDialog.ShowDialog () == DialogResult.OK)
			{
				pFilePath = lDialog.FileName;
				return true;
			}
			return false;
		}

		static public bool OpenImageFile (ref String pFilePath)
		{
			OpenFileDialog	lDialog = new OpenFileDialog ();
			StringBuilder	lFilter = new StringBuilder ();
			
			lFilter.Append (FileExtTypeName (".bmp", "Bitmap Files") + " (*.bmp)|*.bmp");
			lFilter.Append ("|" + FileExtTypeName (".gif", "GIF Files") + " (*.gif)|*.gif");
			lFilter.Append ("|" + FileExtTypeName (".jpg", "Jpeg Files") + " (*.jpg,*.jpeg)|*.jpg;*.jpeg");
			lFilter.Append ("|" + FileExtTypeName (".png", "PNG Files") + " (*.png)|*.png");
			lFilter.Append ("|All Image Files (*.bmp,*.gif,*.jpg,*.jpeg,*.png)|*.bmp;*.gif;*.jpg;*.jpeg;*.png");
			lFilter.Append ("|All Files (*.*)|*.*");
			lDialog.Filter = lFilter.ToString ();
			lDialog.DefaultExt = ".bmp";

			InitFilePath (lDialog, pFilePath);
			if (lDialog.ShowDialog () == DialogResult.OK)
			{
				pFilePath = lDialog.FileName;
				return true;
			}
			return false;
		}

		static public bool OpenIconFile (ref String pFilePath)
		{
			OpenFileDialog	lDialog = new OpenFileDialog ();
			StringBuilder	lFilter = new StringBuilder ();

			lFilter.Append (FileExtTypeName (".ico", "Icon Files") + " (*.ico)|*.ico");
			lFilter.Append ("|All Files (*.*)|*.*");
			lDialog.Filter = lFilter.ToString();
			lDialog.DefaultExt = ".ico";

			InitFilePath (lDialog, pFilePath);
			if (lDialog.ShowDialog () == DialogResult.OK)
			{
				pFilePath = lDialog.FileName;
				return true;
			}
			return false;
		}

		static public bool OpenSoundFile (ref String pFilePath)
		{
			OpenFileDialog	lDialog = new OpenFileDialog ();
			StringBuilder	lFilter = new StringBuilder ();

			lFilter.Append (FileExtTypeName (".wav", "Sound Files") + " (*.wav)|*.wav");
			lFilter.Append ("|All Files (*.*)|*.*");
			lDialog.Filter = lFilter.ToString ();
			lDialog.DefaultExt = ".wav";

			InitFilePath (lDialog, pFilePath);
			if (lDialog.ShowDialog () == DialogResult.OK)
			{
				pFilePath = lDialog.FileName;
				return true;
			}
			return false;
		}

		static public bool OpenPaletteFile (ref String pFilePath)
		{
			OpenFileDialog	lDialog = new OpenFileDialog ();
			StringBuilder	lFilter = new StringBuilder ();

			lFilter.Append (FileExtTypeName (".bmp", "Bitmap Files") + " (*.bmp)|*.bmp");
			lFilter.Append ("|" + FileExtTypeName (".gif", "GIF Files") + " (*.gif)|*.gif");
			lFilter.Append ("|All Files (*.*)|*.*");
			lDialog.Filter = lFilter.ToString ();
			lDialog.DefaultExt = ".bmp";

			InitFilePath (lDialog, pFilePath);
			if (lDialog.ShowDialog () == DialogResult.OK)
			{
				Bitmap			lBitmap = null;
				ColorPalette	lPalette = null;

				try
				{
					lBitmap = new Bitmap (lDialog.FileName);
					if (lBitmap != null)
					{
						lPalette = lBitmap.Palette;
					}
				}
				catch
				{
				}

				if ((lPalette == null) || (lPalette.Entries.Length != 256))
				{
					MessageBox.Show (String.Format (Properties.Resources.MsgNoFilePalette, lDialog.FileName), Program.AssemblyTitle, MessageBoxButtons.OK, MessageBoxIcon.Warning);
				}
				else
				{
					pFilePath = lDialog.FileName;
				}
				return true;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////

		static public void InitFilePath (FileDialog pFileDialog, String pFilePath)
		{
			pFileDialog.FileName = pFilePath;

			if (!String.IsNullOrEmpty (pFilePath))
			{
				try
				{
					pFileDialog.FileName = System.IO.Path.GetFullPath (pFilePath);
					pFileDialog.InitialDirectory = System.IO.Path.GetDirectoryName (pFileDialog.FileName);
					if (pFileDialog.FileName.StartsWith (pFileDialog.InitialDirectory))
					{
						pFileDialog.FileName = System.IO.Path.GetFileName (pFileDialog.FileName);
					}
				}
				catch
				{
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		static public String FileExtTypeName (String pFileExt)
		{
			String	lFileType = pFileExt;

			if (lFileType.StartsWith ("."))
			{
				lFileType = lFileType.Substring (1);
			}
			lFileType = lFileType.ToUpper () + " File";

			return FileExtTypeName (pFileExt, lFileType);
		}

		static public String FileExtTypeName (String pFileExt, String pDefault)
		{
			String	lTypeName = pDefault;

			try
			{
				Microsoft.Win32.RegistryKey	lFileExtKey = Microsoft.Win32.Registry.ClassesRoot.OpenSubKey (pFileExt, false);
				Microsoft.Win32.RegistryKey	lProgIdKey = null;

				if (lFileExtKey != null)
				{
					lProgIdKey = Microsoft.Win32.Registry.ClassesRoot.OpenSubKey (lFileExtKey.GetValue (String.Empty).ToString ());
				}
				if (lProgIdKey != null)
				{
					lTypeName = lProgIdKey.GetValue (String.Empty).ToString ();
				}
				if (String.IsNullOrEmpty (lTypeName))
				{
					lTypeName = pDefault;
				}
			}
			catch
			{
			}

			return lTypeName;
		}
	}
}
