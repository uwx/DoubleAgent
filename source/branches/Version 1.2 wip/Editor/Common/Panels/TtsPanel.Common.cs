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
using DoubleAgent;
using DoubleAgent.Character;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Updates;

namespace AgentCharacterEditor.Panels
{
	public partial class TtsPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		private Sapi4Voices mVoices = null;

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public override ResolvePart FilePart
		{
			get
			{
				return base.FilePart;
			}
			set
			{
				base.FilePart = value;
				ShowTtsProperties ();
			}
		}

		protected FileTts FileTts
		{
			get
			{
				return (CharacterFile == null) ? null : CharacterFile.Tts;
			}
		}

		public override Boolean IsPanelEmpty
		{
			get
			{
				return ((FileTts == null) || ((CharacterFile.Header.Style & CharacterStyle.Tts) == CharacterStyle.None));
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		class VoiceComboItem
		{
			public Sapi4VoiceInfo VoiceInfo;

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
			Boolean lWasFilling = PushIsPanelFilling (true);

			CheckBoxUseTTS.IsEnabled = (CharacterFile != null) && !Program.FileIsReadOnly;
			GroupBoxTTS.IsEnabled = !IsPanelEmpty;
			TextBoxTTSModeID.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly;
			TextBoxVendor.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly;
			TextBoxLanguage.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly;
			TextBoxGender.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly;

			LabelName.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly; 
			LabelGender.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly; 
			LabelLanguage.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly; 
			LabelVendor.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly; 
			LabelModeID.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly; 

			if (IsPanelEmpty)
			{
				CheckBoxUseTTS.IsChecked = false;
				ComboBoxName.SelectedIndex = -1;
				ComboBoxName.IsEnabled = false;
				TextBoxTTSModeID.Clear ();
				TextBoxVendor.Clear ();
				TextBoxLanguage.Clear ();
				TextBoxGender.Clear ();
			}
			else
			{
				Sapi4VoiceInfo lVoiceInfo;

				CheckBoxUseTTS.IsChecked = true;
				ShowAllVoices ();
				lVoiceInfo = VoiceComboInfo (FileTts.Mode);
				ComboBoxName.SelectedIndex = VoiceComboNdx (FileTts.Mode);
				ComboBoxName.IsEnabled = !Program.FileIsReadOnly;

				TextBoxTTSModeID.Text = FileTts.ModeId.ToString ().ToUpper ();
				TextBoxVendor.Text = (lVoiceInfo == null) ? "" : lVoiceInfo.Manufacturer.Replace ("&&", "&");
				TextBoxLanguage.Text = new System.Globalization.CultureInfo (FileTts.Language).DisplayName;
				TextBoxGender.Text = VoiceComboItem.GenderName (FileTts.Gender);
			}

			PopIsPanelFilling (lWasFilling);
		}

		///////////////////////////////////////////////////////////////////////////////

		private int VoiceComboNdx (Guid pModeId)
		{
			int lNdx;

			for (lNdx = 0; lNdx < ComboBoxName.Items.Count; lNdx++)
			{
				VoiceComboItem lItem = (VoiceComboItem)ComboBoxName.Items[lNdx];

				if (lItem.VoiceInfo.ModeId.Equals (pModeId))
				{
					return lNdx;
				}
			}
			return -1;
		}

		private Sapi4VoiceInfo VoiceComboInfo (Guid pModeId)
		{
			int lNdx;

			for (lNdx = 0; lNdx < ComboBoxName.Items.Count; lNdx++)
			{
				VoiceComboItem lItem = (VoiceComboItem)ComboBoxName.Items[lNdx];

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
				VoiceComboItem lItem = (VoiceComboItem)ComboBoxName.Items[pComboNdx];
				return lItem.VoiceInfo;
			}
			return null;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		internal UndoableUpdate PasteTts (FileTts pPasteTts)
		{
			if ((CharacterFile != null) && (pPasteTts != null) && !Program.FileIsReadOnly)
			{
				System.Media.SystemSounds.Beep.Play ();
			}
			return null;
		}

		///////////////////////////////////////////////////////////////////////////////

		protected void HandleEnabledChanged ()
		{
			UpdateCharacterTts lUpdate = new UpdateCharacterTts (null);

			if (CheckBoxUseTTS.IsChecked.Value)
			{
				lUpdate.CharacterStyle |= CharacterStyle.Tts;
			}
			else
			{
				lUpdate.CharacterStyle &= ~CharacterStyle.Tts;
			}
			UpdateCharacterTts.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterTts, this);
		}

		protected void HandleVoiceChanged ()
		{
			Sapi4VoiceInfo lVoiceInfo = VoiceComboInfo (ComboBoxName.SelectedIndex);

			if (lVoiceInfo != null)
			{
				UpdateCharacterTts lUpdate = new UpdateCharacterTts (lVoiceInfo);

				UpdateCharacterTts.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterTts, this);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override void UpdateApplied (Object pUpdate)
		{
			UpdateCharacterTts lUpdate = pUpdate as UpdateCharacterTts;

			if ((lUpdate != null) && (lUpdate.CharacterFile == CharacterFile))
			{
				ShowTtsProperties ();
			}
		}

		#endregion
	}
}