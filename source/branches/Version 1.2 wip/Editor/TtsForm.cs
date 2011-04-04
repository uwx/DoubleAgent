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
			CheckBoxUseTTS.Enabled = (mCharacterFile != null) && (!Program.MainForm.FileIsReadOnly);
			TextBoxTTSModeID.Enabled = (mCharacterFile != null) && (!Program.MainForm.FileIsReadOnly);
			TextBoxVendor.Enabled = (mCharacterFile != null) && (!Program.MainForm.FileIsReadOnly);
			TextBoxLanguage.Enabled = (mCharacterFile != null) && (!Program.MainForm.FileIsReadOnly);
			TextBoxGender.Enabled = (mCharacterFile != null) && (!Program.MainForm.FileIsReadOnly);

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

				TextBoxTTSModeID.Text = mFileTts.ModeId.ToString().ToUpper();
				TextBoxVendor.Text = (lVoiceInfo == null) ? "" : lVoiceInfo.Manufacturer.Replace ("&&", "&");
				TextBoxLanguage.Text = new System.Globalization.CultureInfo (mFileTts.Language).DisplayName;
				TextBoxGender.Text = VoiceComboItem.GenderName (mFileTts.Gender);
			}
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
				VoiceComboItem	lItem = (VoiceComboItem) ComboBoxName.Items [lNdx];
				
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
		#region Event Handlers

		private void CheckBoxUseTTS_CheckedChanged (object sender, EventArgs e)
		{
			if ((mCharacterFile != null) && !Program.MainForm.FileIsReadOnly)
			{
				if (CheckBoxUseTTS.Checked)
				{
					mCharacterFile.Header.Style |= CharacterStyle.CharStyleTts;
				}
				else
				{
					mCharacterFile.Header.Style &= ~CharacterStyle.CharStyleTts;
				}
				ShowTtsProperties ();
			}
		}

		private void ComboBoxName_SelectionChangeCommitted (object sender, EventArgs e)
		{
			if (!IsEmpty  && !Program.MainForm.FileIsReadOnly)
			{
				Sapi4VoiceInfo	lVoiceInfo = VoiceComboInfo (ComboBoxName.SelectedIndex);
				
				if (lVoiceInfo != null)
				{
					mFileTts.Engine = lVoiceInfo.EngineId;
					mFileTts.Mode = lVoiceInfo.ModeId;
					mFileTts.Language = lVoiceInfo.LangId;
					mFileTts.Gender = lVoiceInfo.SpeakerGender;
					ShowTtsProperties ();
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
	}
}
