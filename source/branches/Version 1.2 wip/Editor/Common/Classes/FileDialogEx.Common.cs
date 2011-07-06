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
using System.Drawing;
using System.Drawing.Imaging;
using System.Text;
using AgentCharacterEditor.Properties;

namespace AgentCharacterEditor.Global
{
	public partial class OpenFileDialogEx
	{
		static public Boolean OpenCharacterFile (ref String pFilePath)
		{
			return OpenExistingFile (ref pFilePath, CharacterFileFilter, CharacterFileDefaultExt);
		}

		static public Boolean SaveCharacterFile (ref String pFilePath)
		{
			return OpenSaveFile (ref pFilePath, CharacterFileFilter, CharacterFileDefaultExt);
		}

		//=============================================================================

		static public Boolean OpenImageFile (ref String pFilePath)
		{
			return OpenExistingFile (ref pFilePath, ImageFileFilter, ImageFileDefaultExt);
		}

		static public Boolean OpenIconFile (ref String pFilePath)
		{
			return OpenExistingFile (ref pFilePath, IconFileFilter, IconFileDefaultExt);
		}

		static public Boolean OpenSoundFile (ref String pFilePath)
		{
			return OpenExistingFile (ref pFilePath, SoundFileFilter, SoundFileDefaultExt);
		}

		static public Boolean OpenPaletteFile (ref String pFilePath)
		{
			if (OpenExistingFile (ref pFilePath, PaletteFileFilter, PaletteFileDefaultExt))
			{
				Bitmap lBitmap = null;
				ColorPalette lPalette = null;

				try
				{
					lBitmap = new Bitmap (pFilePath);
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
					ShowPaletteError (pFilePath);
				}
				return true;
			}
			return false;
		}

		//=============================================================================

		static public String CharacterFileFilter
		{
			get
			{
				StringBuilder lFilter = new StringBuilder ();

				lFilter.Append (FileExtTypeName (".acd", "Character Definition Files") + " (*.acd)|*.acd");
				lFilter.Append ("|" + FileExtTypeName (".acs", "Character Files") + " (*.acs)|*.acs");
				lFilter.Append ("|" + FileExtTypeName (".acf", "HTTP Character Files") + " (*.acf)|*.acf");
				lFilter.Append ("|All Files (*.*)|*.*");
				return lFilter.ToString ();
			}
		}
		static public String CharacterFileDefaultExt
		{
			get
			{
				return ".acd";
			}
		}

		static public String IconFileFilter
		{
			get
			{
				StringBuilder lFilter = new StringBuilder ();

				lFilter.Append (FileExtTypeName (".ico", "Icon Files") + " (*.ico)|*.ico");
				lFilter.Append ("|All Files (*.*)|*.*");
				return lFilter.ToString ();
			}
		}
		static public String IconFileDefaultExt
		{
			get
			{
				return ".ico";
			}
		}

		static public String PaletteFileFilter
		{
			get
			{
				StringBuilder lFilter = new StringBuilder ();

				lFilter.Append (FileExtTypeName (".bmp", "Bitmap Files") + " (*.bmp)|*.bmp");
				lFilter.Append ("|" + FileExtTypeName (".gif", "GIF Files") + " (*.gif)|*.gif");
				lFilter.Append ("|All Files (*.*)|*.*");
				return lFilter.ToString ();
			}
		}
		static public String PaletteFileDefaultExt
		{
			get
			{
				return ".bmp";
			}
		}

		static public String ImageFileFilter
		{
			get
			{
				StringBuilder lFilter = new StringBuilder ();

				lFilter.Append (FileExtTypeName (".bmp", "Bitmap Files") + " (*.bmp)|*.bmp");
				lFilter.Append ("|" + FileExtTypeName (".gif", "GIF Files") + " (*.gif)|*.gif");
				lFilter.Append ("|" + FileExtTypeName (".jpg", "Jpeg Files") + " (*.jpg,*.jpeg)|*.jpg;*.jpeg");
				lFilter.Append ("|" + FileExtTypeName (".png", "PNG Files") + " (*.png)|*.png");
				lFilter.Append ("|All Image Files (*.bmp,*.gif,*.jpg,*.jpeg,*.png)|*.bmp;*.gif;*.jpg;*.jpeg;*.png");
				lFilter.Append ("|All Files (*.*)|*.*");
				return lFilter.ToString ();
			}
		}
		static public String ImageFileDefaultExt
		{
			get
			{
				return ".bmp";
			}
		}

		static public String SoundFileFilter
		{
			get
			{
				StringBuilder lFilter = new StringBuilder ();

				lFilter.Append (FileExtTypeName (".wav", "Sound Files") + " (*.wav)|*.wav");
				lFilter.Append ("|All Files (*.*)|*.*");
				return lFilter.ToString ();
			}
		}
		static public String SoundFileDefaultExt
		{
			get
			{
				return ".wav";
			}
		}

		//=============================================================================

		static protected void GetPathDefaults (String pFilePath, ref String pInitialDirectory, ref String pFileName, ref String pDefaultExt)
		{
			pInitialDirectory = String.Empty;
			pFileName = String.Empty;

			if (!String.IsNullOrEmpty (pFilePath))
			{
				if (pFilePath.Contains ("*"))
				{
					try
					{
						pInitialDirectory = System.IO.Path.GetFullPath (System.IO.Path.GetDirectoryName (pFilePath));
					}
					catch
					{
					}
				}
				else
				{
					pFileName = pFilePath;
					try
					{
						pFileName = System.IO.Path.GetFullPath (pFilePath);
						pInitialDirectory = System.IO.Path.GetDirectoryName (pFileName);
						if (pFileName.StartsWith (pInitialDirectory))
						{
							pFileName = System.IO.Path.GetFileName (pFileName);
						}
					}
					catch
					{
					}
				}

				if (System.IO.Path.HasExtension (pFilePath))
				{
					pDefaultExt = System.IO.Path.GetExtension (pFilePath);
				}
			}
			if (!String.IsNullOrEmpty (pDefaultExt) && !String.IsNullOrEmpty (pFilePath))
			{
				try
				{
					pDefaultExt = System.IO.Path.GetExtension (pFilePath);
				}
				catch
				{
				}
			}
		}

		//=============================================================================

		static public String FileExtTypeName (String pFileExt)
		{
			String lFileType = pFileExt;

			if (lFileType.StartsWith ("."))
			{
				lFileType = lFileType.Substring (1);
			}
			lFileType = lFileType.ToUpper () + " File";

			return FileExtTypeName (pFileExt, lFileType);
		}

		static public String FileExtTypeName (String pFileExt, String pDefault)
		{
			String lTypeName = pDefault;

			try
			{
				Microsoft.Win32.RegistryKey lFileExtKey = Microsoft.Win32.Registry.ClassesRoot.OpenSubKey (pFileExt, false);
				Microsoft.Win32.RegistryKey lProgIdKey = null;

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
