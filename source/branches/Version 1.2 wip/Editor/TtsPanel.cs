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
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DoubleAgent;
using DoubleAgent.Character;
using AgentCharacterEditor.Updates;

namespace AgentCharacterEditor
{
	public partial class TtsPanel : UserControl
	{
		private CharacterFile	mCharacterFile = null;
		private Sapi4Voices		mVoices = null;

		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public TtsPanel ()
		{
			InitializeComponent ();
			CausesValidation = Visible;

			if (Program.MainForm != null)
			{
				Program.MainForm.LoadConfig += new EventHandler (MainForm_LoadConfig);
				Program.MainForm.SaveConfig += new EventHandler (MainForm_SaveConfig);
			}
		}

		void MainForm_LoadConfig (object sender, EventArgs e)
		{
			Properties.Settings	lSettings = Properties.Settings.Default;
		}

		void MainForm_SaveConfig (object sender, EventArgs e)
		{
			Properties.Settings	lSettings = Properties.Settings.Default;
		}

		private void TtsForm_VisibleChanged (object sender, EventArgs e)
		{
			CausesValidation = Visible;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Browsable (false)]
		public CharacterFile CharacterFile
		{
			get
			{
				return mCharacterFile;
			}
			set
			{
				mCharacterFile = value;
				ShowTtsProperties ();
			}
		}

		[System.ComponentModel.Browsable (false)]
		public FileTts FileTts
		{
			get
			{
				return (mCharacterFile == null) ? null : mCharacterFile.Tts;
			}
		}

		private Boolean IsEmpty
		{
			get
			{
				return ((FileTts == null) || ((mCharacterFile.Header.Style & CharacterStyle.CharStyleTts) == 0));
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		class VoiceComboItem
		{
			public Sapi4VoiceInfo	VoiceInfo;

			public VoiceComboItem (Sapi4VoiceInfo pVoiceInfo)
			{
				VoiceInfo = pVoiceInfo;
			}

			public override String ToString ()
			{
				if (String.IsNullOrEmpty (VoiceInfo.Manufacturer))
				{
					return String.Format ("{0} - {1} {2}", VoiceInfo.VoiceName, GenderName (VoiceInfo.SpeakerGender), new System.Globalization.CultureInfo (VoiceInfo.LangId).EnglishName);
				}
				else
				{
					return String.Format ("{0} - {1} {2} - {3}", VoiceInfo.VoiceName, GenderName (VoiceInfo.SpeakerGender), new System.Globalization.CultureInfo (VoiceInfo.LangId).EnglishName, VoiceInfo.Manufacturer.Replace ("&&", "&"));
				}
			}

			static public String GenderName (int pGender)
			{
				return (pGender == 1) ? "Female" : (pGender == 2) ? "Male" : "";
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowTtsProperties ()
		{
			CheckBoxUseTTS.Enabled = (mCharacterFile != null) && !Program.FileIsReadOnly;
			TextBoxTTSModeID.Enabled = !IsEmpty && !Program.FileIsReadOnly;
			TextBoxVendor.Enabled = !IsEmpty && !Program.FileIsReadOnly;
			TextBoxLanguage.Enabled = !IsEmpty && !Program.FileIsReadOnly;
			TextBoxGender.Enabled = !IsEmpty && !Program.FileIsReadOnly;

			CausesValidation = false;
			SuspendLayout ();
			if (IsEmpty)
			{
				CheckBoxUseTTS.Checked = false;
				ComboBoxName.SelectedIndex = -1;
				ComboBoxName.Enabled = false;
				TextBoxTTSModeID.ResetText ();
				TextBoxVendor.ResetText ();
				TextBoxLanguage.ResetText ();
				TextBoxGender.ResetText ();
			}
			else
			{
				Sapi4VoiceInfo	lVoiceInfo;

				CheckBoxUseTTS.Checked = true;
				ShowAllVoices ();
				lVoiceInfo = VoiceComboInfo (FileTts.Mode);
				ComboBoxName.SelectedIndex = VoiceComboNdx (FileTts.Mode);
				ComboBoxName.Enabled = !Program.FileIsReadOnly;

				TextBoxTTSModeID.Text = FileTts.ModeId.ToString ().ToUpper ();
				TextBoxVendor.Text = (lVoiceInfo == null) ? "" : lVoiceInfo.Manufacturer.Replace ("&&", "&");
				TextBoxLanguage.Text = new System.Globalization.CultureInfo (FileTts.Language).DisplayName;
				TextBoxGender.Text = VoiceComboItem.GenderName (FileTts.Gender);
			}
			ResumeLayout (true);
			CausesValidation = Visible;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowAllVoices ()
		{
			if (mVoices == null)
			{
				mVoices = new Sapi4Voices ();
				ComboBoxName.BeginUpdate ();
				ComboBoxName.Items.Clear ();

				foreach (Sapi4VoiceInfo lVoiceInfo in mVoices)
				{
					ComboBoxName.Items.Add (new VoiceComboItem (lVoiceInfo));
				}

				ComboBoxName.EndUpdate ();
			}
		}

		private int VoiceComboNdx (Guid pModeId)
		{
			int	lNdx;

			for (lNdx = 0; lNdx < ComboBoxName.Items.Count; lNdx++)
			{
				VoiceComboItem	lItem = (VoiceComboItem)ComboBoxName.Items[lNdx];

				if (lItem.VoiceInfo.ModeId.Equals (pModeId))
				{
					return lNdx;
				}
			}
			return -1;
		}

		private Sapi4VoiceInfo VoiceComboInfo (Guid pModeId)
		{
			int	lNdx;

			for (lNdx = 0; lNdx < ComboBoxName.Items.Count; lNdx++)
			{
				VoiceComboItem	lItem = (VoiceComboItem)ComboBoxName.Items[lNdx];

				if (lItem.VoiceInfo.ModeId.Equals (pModeId))
				{
					return lItem.VoiceInfo;
				}
			}
			return null;
		}

		private Sapi4VoiceInfo VoiceComboInfo (int pComboNdx)
		{
			if ((pComboNdx >= 0) && (pComboNdx < ComboBoxName.Items.Count))
			{
				VoiceComboItem	lItem = (VoiceComboItem)ComboBoxName.Items[pComboNdx];
				return lItem.VoiceInfo;
			}
			return null;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		internal UndoableUpdate PasteTts (FileTts pPasteTts)
		{
			if ((mCharacterFile != null) && (pPasteTts != null) && !Program.FileIsReadOnly)
			{
				System.Media.SystemSounds.Beep.Play ();
			}
			return null;
		}

		private void OnUpdateApplied (object sender, EventArgs e)
		{
			UpdateCharacterTts	lUpdate = sender as UpdateCharacterTts;

			if ((lUpdate != null) && (lUpdate.CharacterFile == mCharacterFile))
			{
				ShowTtsProperties ();
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void CheckBoxUseTTS_CheckedChanged (object sender, EventArgs e)
		{
			if (CausesValidation && (mCharacterFile != null) && !Program.FileIsReadOnly)
			{
				UpdateCharacterTts	lUpdate = new UpdateCharacterTts (null);

				if (CheckBoxUseTTS.Checked)
				{
					lUpdate.CharacterStyle |= CharacterStyle.CharStyleTts;
				}
				else
				{
					lUpdate.CharacterStyle &= ~CharacterStyle.CharStyleTts;
				}
				UpdateCharacterTts.PutUndo (lUpdate.Apply (OnUpdateApplied) as UpdateCharacterTts, this);
			}
		}

		private void ComboBoxName_SelectionChangeCommitted (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				Sapi4VoiceInfo	lVoiceInfo = VoiceComboInfo (ComboBoxName.SelectedIndex);

				if (lVoiceInfo != null)
				{
					UpdateCharacterTts	lUpdate = new UpdateCharacterTts (lVoiceInfo);

					UpdateCharacterTts.PutUndo (lUpdate.Apply (OnUpdateApplied) as UpdateCharacterTts, this);
				}
			}
		}

		#endregion
	}
}
