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

namespace AgentCharacterEditor
{
	public partial class TtsForm : UserControl
	{
		private CharacterFile	mCharacterFile = null;
		private FileTts			mFileTts = null;
		private Sapi4Voices		mVoices = null;

		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public TtsForm ()
		{
			mCharacterFile = null;
			InitializeComponent ();
		}

		public void LoadConfig ()
		{
			Properties.Settings	lSettings = Properties.Settings.Default;
		}

		public void SaveConfig ()
		{
			Properties.Settings	lSettings = Properties.Settings.Default;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public CharacterFile CharacterFile
		{
			get
			{
				return mCharacterFile;
			}
			set
			{
				mCharacterFile = value;
				if (mCharacterFile != null)
				{
					mFileTts = mCharacterFile.Tts;
				}
				else
				{
					mFileTts = null;
				}
				ShowTtsProperties ();
			}
		}

		private bool IsEmpty
		{
			get
			{
				return ((mCharacterFile == null) || (mFileTts == null) || ((mCharacterFile.Header.Style & CharacterStyle.CharStyleTts) == 0));
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
			CheckBoxUseTTS.Enabled = (mCharacterFile != null) && !Program.MainForm.FileIsReadOnly;
			TextBoxTTSModeID.Enabled = !IsEmpty && !Program.MainForm.FileIsReadOnly;
			TextBoxVendor.Enabled = !IsEmpty && !Program.MainForm.FileIsReadOnly;
			TextBoxLanguage.Enabled = !IsEmpty && !Program.MainForm.FileIsReadOnly;
			TextBoxGender.Enabled = !IsEmpty && !Program.MainForm.FileIsReadOnly;

			CheckBoxUseTTS.CheckedChanged -= new System.EventHandler (CheckBoxUseTTS_CheckedChanged);
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
				lVoiceInfo = VoiceComboInfo (mFileTts.Mode);
				ComboBoxName.SelectedIndex = VoiceComboNdx (mFileTts.Mode);
				ComboBoxName.Enabled = !Program.MainForm.FileIsReadOnly;

				TextBoxTTSModeID.Text = mFileTts.ModeId.ToString ().ToUpper ();
				TextBoxVendor.Text = (lVoiceInfo == null) ? "" : lVoiceInfo.Manufacturer.Replace ("&&", "&");
				TextBoxLanguage.Text = new System.Globalization.CultureInfo (mFileTts.Language).DisplayName;
				TextBoxGender.Text = VoiceComboItem.GenderName (mFileTts.Gender);
			}
			ResumeLayout (true);
			CheckBoxUseTTS.CheckedChanged += new System.EventHandler (CheckBoxUseTTS_CheckedChanged);
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

		internal class UpdateTts : UndoableUpdate
		{
			public UpdateTts (CharacterFile pCharacterFile, Sapi4VoiceInfo pVoiceInfo)
				: base (pCharacterFile)
			{
				this.CharacterStyle = pCharacterFile.Header.Style & CharacterStyle.CharStyleTts;
				if (pVoiceInfo != null)
				{
					this.VoiceInfo = new Sapi4VoiceInfo ();
					this.VoiceInfo.EngineId = pVoiceInfo.EngineId;
					this.VoiceInfo.ModeId = pVoiceInfo.ModeId;
					this.VoiceInfo.LangId = pVoiceInfo.LangId;
					this.VoiceInfo.SpeakerGender = pVoiceInfo.SpeakerGender;
				}
			}

			public CharacterStyle CharacterStyle
			{
				get;
				set;
			}
			public Sapi4VoiceInfo VoiceInfo
			{
				get;
				private set;
			}

			public override UndoUnit Apply ()
			{
				UndoUnit	lApplied = null;

				if ((CharacterFile.Header.Style & CharacterStyle.CharStyleTts) != (this.CharacterStyle & CharacterStyle.CharStyleTts))
				{
					CharacterStyle	lSwap = CharacterFile.Header.Style & CharacterStyle.CharStyleTts;
					CharacterFile.Header.Style = (CharacterFile.Header.Style & ~CharacterStyle.CharStyleTts) | (this.CharacterStyle & CharacterStyle.CharStyleTts);
					this.CharacterStyle = lSwap;
					lApplied = this;
				}
				if ((CharacterFile.Tts != null) && (this.VoiceInfo != null))
				{
					Sapi4VoiceInfo	lVoiceInfo = new Sapi4VoiceInfo ();

					lVoiceInfo.EngineId = CharacterFile.Tts.Engine;
					lVoiceInfo.ModeId = CharacterFile.Tts.Mode;
					lVoiceInfo.LangId = CharacterFile.Tts.Language;
					lVoiceInfo.SpeakerGender = CharacterFile.Tts.Gender;

					if (!lVoiceInfo.Equals (this.VoiceInfo))
					{
						CharacterFile.Tts.Engine = this.VoiceInfo.EngineId;
						CharacterFile.Tts.Mode = this.VoiceInfo.ModeId;
						CharacterFile.Tts.Language = this.VoiceInfo.LangId;
						CharacterFile.Tts.Gender = this.VoiceInfo.SpeakerGender;
						this.VoiceInfo = lVoiceInfo;
						lApplied = this;
					}
				}

				if (lApplied != null)
				{
					return OnApplied (lApplied);
				}
				return null;
			}

			public override string ToString ()
			{
				return "Text-to-Speech";
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void CheckBoxUseTTS_CheckedChanged (object sender, EventArgs e)
		{
			if ((mCharacterFile != null) && !Program.MainForm.FileIsReadOnly)
			{
				UpdateTts	lUpdate = new UpdateTts (mCharacterFile, null);

				if (CheckBoxUseTTS.Checked)
				{
					lUpdate.CharacterStyle |= CharacterStyle.CharStyleTts;
				}
				else
				{
					lUpdate.CharacterStyle &= ~CharacterStyle.CharStyleTts;
				}
				lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableAction_Applied);
				UndoableUpdate.PutUndo (lUpdate.Apply () as UpdateTts);
			}
		}

		private void ComboBoxName_SelectionChangeCommitted (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				Sapi4VoiceInfo	lVoiceInfo = VoiceComboInfo (ComboBoxName.SelectedIndex);

				if (lVoiceInfo != null)
				{
					UpdateTts	lUpdate = new UpdateTts (mCharacterFile, lVoiceInfo);

					lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableAction_Applied);
					UndoableUpdate.PutUndo (lUpdate.Apply () as UpdateTts);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void UndoableAction_Applied (object sender, EventArgs e)
		{
			ShowTtsProperties ();
		}

		#endregion
	}
}
