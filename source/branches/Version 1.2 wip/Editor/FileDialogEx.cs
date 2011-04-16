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
using System.ComponentModel;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;
using Microsoft.Win32;

namespace AgentCharacterEditor
{
	public class OpenFileDialogEx
	{
		static public Boolean OpenCharacterFile (ref String pFilePath)
		{
			OpenFileDialog	lDialog = new OpenFileDialog ();
			StringBuilder	lFilter = new StringBuilder ();

			lFilter.Append (FileExtTypeName (".acd", "Character Definition Files") + " (*.acd)|*.acd");
			lFilter.Append ("|" + FileExtTypeName (".acs", "Character Files") + " (*.acs)|*.acs");
			lFilter.Append ("|All Files (*.*)|*.*");
			lDialog.Filter = lFilter.ToString ();
			lDialog.CheckFileExists = true;

			InitFilePath (lDialog, pFilePath, ".acd");
			InitFilterIndex (lDialog);
			if (lDialog.ShowDialog () == DialogResult.OK)
			{
				pFilePath = lDialog.FileName;
				return true;
			}
			return false;
		}

		static public Boolean SaveCharacterFile (ref String pFilePath)
		{
			SaveFileDialog	lDialog = new SaveFileDialog ();
			StringBuilder	lFilter = new StringBuilder ();

			lFilter.Append (FileExtTypeName (".acd", "Character Definition Files") + " (*.acd)|*.acd");
			lFilter.Append ("|" + FileExtTypeName (".acs", "Character Files") + " (*.acs)|*.acs");
			lFilter.Append ("|All Files (*.*)|*.*");
			lDialog.Filter = lFilter.ToString ();
			lDialog.CheckPathExists = true;
			lDialog.OverwritePrompt = true;

			InitFilePath (lDialog, pFilePath, ".acd");
			InitFilterIndex (lDialog);
			if (lDialog.ShowDialog () == DialogResult.OK)
			{
				pFilePath = lDialog.FileName;
				return true;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////

		static public Boolean OpenImageFile (ref String pFilePath)
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
			lDialog.CheckFileExists = true;

			InitFilePath (lDialog, pFilePath, ".bmp");
			InitFilterIndex (lDialog);
			if (lDialog.ShowDialog () == DialogResult.OK)
			{
				pFilePath = lDialog.FileName;
				return true;
			}
			return false;
		}

		static public Boolean OpenIconFile (ref String pFilePath)
		{
			OpenFileDialog	lDialog = new OpenFileDialog ();
			StringBuilder	lFilter = new StringBuilder ();

			lFilter.Append (FileExtTypeName (".ico", "Icon Files") + " (*.ico)|*.ico");
			lFilter.Append ("|All Files (*.*)|*.*");
			lDialog.Filter = lFilter.ToString ();
			lDialog.CheckFileExists = true;

			InitFilePath (lDialog, pFilePath, ".ico");
			InitFilterIndex (lDialog);
			if (lDialog.ShowDialog () == DialogResult.OK)
			{
				pFilePath = lDialog.FileName;
				return true;
			}
			return false;
		}

		static public Boolean OpenSoundFile (ref String pFilePath)
		{
			OpenFileDialog	lDialog = new OpenFileDialog ();
			StringBuilder	lFilter = new StringBuilder ();

			lFilter.Append (FileExtTypeName (".wav", "Sound Files") + " (*.wav)|*.wav");
			lFilter.Append ("|All Files (*.*)|*.*");
			lDialog.Filter = lFilter.ToString ();
			lDialog.CheckFileExists = true;

			InitFilePath (lDialog, pFilePath, ".wav");
			InitFilterIndex (lDialog);
			if (lDialog.ShowDialog () == DialogResult.OK)
			{
				pFilePath = lDialog.FileName;
				return true;
			}
			return false;
		}

		static public Boolean OpenPaletteFile (ref String pFilePath)
		{
			OpenFileDialog	lDialog = new OpenFileDialog ();
			StringBuilder	lFilter = new StringBuilder ();

			lFilter.Append (FileExtTypeName (".bmp", "Bitmap Files") + " (*.bmp)|*.bmp");
			lFilter.Append ("|" + FileExtTypeName (".gif", "GIF Files") + " (*.gif)|*.gif");
			lFilter.Append ("|All Files (*.*)|*.*");
			lDialog.Filter = lFilter.ToString ();
			lDialog.CheckFileExists = true;

			InitFilePath (lDialog, pFilePath, ".bmp");
			InitFilterIndex (lDialog);
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
			InitFilePath (pFileDialog, pFilePath, null);
		}

		static public void InitFilePath (FileDialog pFileDialog, String pFilePath, String pDefaultExt)
		{
			if (!String.IsNullOrEmpty (pFilePath) && !pFilePath.Contains ("*"))
			{
				pFileDialog.FileName = pFilePath;
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
			if (!String.IsNullOrEmpty (pDefaultExt))
			{
				if (!String.IsNullOrEmpty (pFilePath))
				{
					try
					{
						pFileDialog.DefaultExt = System.IO.Path.GetExtension (pFilePath);
					}
					catch
					{
					}
				}
				if (String.IsNullOrEmpty (pFileDialog.DefaultExt))
				{
					pFileDialog.DefaultExt = pDefaultExt;
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		static public void InitFilterIndex (FileDialog pFileDialog)
		{
			if (!String.IsNullOrEmpty (pFileDialog.Filter) && !String.IsNullOrEmpty (pFileDialog.DefaultExt))
			{
				Char[]		lDelim = { '|' };
				String[]	lFilters = pFileDialog.Filter.Split (lDelim);
				int			lNdx;

				for (lNdx = 0; lNdx < lFilters.Length; lNdx++)
				{
					if (lFilters[lNdx].Contains (pFileDialog.DefaultExt))
					{
						pFileDialog.FilterIndex = (lNdx / 2) + 1;
						break;
					}
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
