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
using AgentCharacterEditor.Properties;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor.Global
{
	public partial class OpenFileDialogEx
	{
		static public Boolean OpenExistingFile (ref String pFilePath, String pFilter, String pDefaultExt)
		{
			System.Windows.Forms.OpenFileDialog lDialog = new System.Windows.Forms.OpenFileDialog ();

			lDialog.Filter = pFilter;
			lDialog.CheckFileExists = true;
			InitFilePath (lDialog, pFilePath, pDefaultExt);
			InitFilterIndex (lDialog);

			if (lDialog.ShowDialog () == DialogResult.OK)
			{
				pFilePath = lDialog.FileName;
				return true;
			}
			return false;
		}

		static public Boolean OpenSaveFile (ref String pFilePath, String pFilter, String pDefaultExt)
		{
			System.Windows.Forms.SaveFileDialog lDialog = new System.Windows.Forms.SaveFileDialog ();

			lDialog.Filter = pFilter;
			lDialog.CheckPathExists = true;
			lDialog.OverwritePrompt = true;
			InitFilePath (lDialog, pFilePath, pDefaultExt);
			InitFilterIndex (lDialog);

			if (lDialog.ShowDialog () == DialogResult.OK)
			{
				pFilePath = lDialog.FileName;
				return true;
			}
			return false;
		}

		//=============================================================================

		static public void InitFilePath (System.Windows.Forms.FileDialog pFileDialog, String pFilePath)
		{
			InitFilePath (pFileDialog, pFilePath, null);
		}

		static public void InitFilePath (System.Windows.Forms.FileDialog pFileDialog, String pFilePath, String pDefaultExt)
		{
			String lInitialDirectory = String.Empty;
			String lFileName = String.Empty;

			GetPathDefaults (pFilePath, ref lInitialDirectory, ref lFileName, ref pDefaultExt);

			pFileDialog.InitialDirectory = lInitialDirectory;
			pFileDialog.FileName = lFileName;
			pFileDialog.DefaultExt = pDefaultExt;
		}

		static public void InitFilterIndex (System.Windows.Forms.FileDialog pFileDialog)
		{
			if (!String.IsNullOrEmpty (pFileDialog.Filter) && !String.IsNullOrEmpty (pFileDialog.DefaultExt))
			{
				Char[] lDelim = { '|' };
				String[] lFilters = pFileDialog.Filter.Split (lDelim);
				int lNdx;

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

		//=============================================================================

		static private void ShowPaletteError (String pFilePath)
		{
			MessageBox.Show (String.Format (AppResources.Resources.MsgNoFilePalette, pFilePath), Program.AssemblyTitle, MessageBoxButtons.OK, MessageBoxIcon.Warning);
		}
	}
}
