using System;
using System.Windows.Forms;
using System.Collections;
using System.ComponentModel;
using System.Text;
using System.Diagnostics;

namespace CtlAndSvr
{
	public partial class MainForm : Form
	{
		private DoubleAgent.Control.Character	mCharacter;

		public MainForm ()
		{
			InitializeComponent ();
			ShowConnected ();

			AxControlBinding.DataSource = TestDaControl;

			TestDaControl.CharacterFiles.MsOfficeFiles = false;
			TestDaControl.CharacterFiles.VerifyVersion = true;
			CharacterFiles.DataSource = TestDaControl.CharacterFiles.FilePaths;
			CharacterFiles.SelectedItem = TestDaControl.CharacterFiles.DefaultFilePath;

			TtsEngines.DataSource = null;
			TtsEngines.DisplayMember = "DisplayName";
			TtsEngines.DataSource = TestDaControl.TTSEngines;

			SrEngines.DataSource = null;
			SrEngines.DisplayMember = "DisplayName";
			SrEngines.DataSource = TestDaControl.SREngines;
			
			SelectCharacter (false);
			ShowCharacterState ();
		}

		private void ShowConnected ()
		{
			ConnectedCheck.Checked = TestDaControl.Connected;
			ContainedCheck.Checked = (TestDaControl.ControlCharacter != null);
			ContainedCheck.Enabled = !TestDaControl.Connected;
		}

		private void SelectCharacter (bool pVisible)
		{
			DoubleAgent.Control.Characters	lCharacters = TestDaControl.Characters;
			DoubleAgent.Control.Character	lPrevCharacter = null;
			String							lFilePath = CharacterFiles.SelectedValue.ToString ();

			if ((lCharacters.Count > 0)
			&& ((lPrevCharacter = lCharacters.get_Index (0)) != null))
			{
				Debug.Print ("Visible [{0}] Connected [{1}] [{2}]", lPrevCharacter.Visible.ToString(), lPrevCharacter.Connected.ToString(), TestDaControl.Connected.ToString());

				if ((lPrevCharacter.FilePath != lFilePath)
				|| (lPrevCharacter.Connected != TestDaControl.Connected))
				{
					foreach (DoubleAgent.Control.Character lLoadedCharacter in lCharacters)
					{
						lCharacters.Unload (lLoadedCharacter.CharacterID);
					}
				}
			}

			if (lCharacters.Count <= 0)
			{
				mCharacter = null;
			}

			if ((mCharacter == null)
			&& (!String.IsNullOrEmpty (lFilePath)))
			{
				lCharacters.Load ("MyCharacter", lFilePath);
				if (lCharacters.Count > 0)
				{
					mCharacter = lCharacters.get_Index (0);
				}
			}

			if (mCharacter != null)
			{
				if ((ContainedCheck.Checked)
				&& (!TestDaControl.Connected))
				{
					TestDaControl.ControlCharacter = mCharacter;
				}
				else
				{
					TestDaControl.ControlCharacter = null;
				}

				if (pVisible)
				{
					mCharacter.Show (false);
				}

				TtsEngines.DataSource = mCharacter.FindTTSEngines (0);
				TtsEngines.ValueMember = "TTSModeID";
				TtsEngines.SelectedValue = mCharacter.TTSModeID;
				TtsEngines.SelectedItem = mCharacter.TTSModeID;

				SrEngines.DataSource = mCharacter.FindSREngines (0);
				SrEngines.ValueMember = "SRModeID";
				SrEngines.SelectedValue = mCharacter.SRModeID;
				SrEngines.SelectedItem = mCharacter.SRModeID;
			}
			else
			{
				TtsEngines.DataSource = TestDaControl.TTSEngines;
				TtsEngines.SelectedItem = null;
				SrEngines.DataSource = TestDaControl.SREngines;
				SrEngines.SelectedItem = null;
			}
		}
		
		private void ShowCharacterState ()
		{
			if (mCharacter == null)
			{
				ShowButton.Enabled = false;
				HideButton.Enabled = false;
				SpeakButton.Enabled = false;
				SpeechText.Enabled = false;
				ListenCheck.Enabled = false;
				ListenCheck.Checked = false; 
			}
			else
			{
				ShowButton.Enabled = !mCharacter.Visible;
				HideButton.Enabled = mCharacter.Visible;
				SpeakButton.Enabled = mCharacter.Visible;
				SpeechText.Enabled = mCharacter.Visible;
				ListenCheck.Enabled = mCharacter.Visible;
				ListenCheck.Checked = (TestDaControl.Settings.AudioStatus == DoubleAgent.Control.AudioStatusType.CharacterListening); 
			}
		}

		private bool IsCharacterVisible
		{
			get
			{
				if (mCharacter != null)
				{
					return mCharacter.Visible;
				}
				return false;
			}
		}

		private void CharacterFiles_SelectionChangeCommitted (object sender, EventArgs e)
		{
			SelectCharacter (IsCharacterVisible);
			ShowCharacterState ();
		}

		private void ConnectedCheck_CheckedChanged (object sender, EventArgs e)
		{
			bool lVisible = IsCharacterVisible;
			TestDaControl.Connected = ConnectedCheck.Checked;
			SelectCharacter (lVisible);
			ShowCharacterState ();
			ShowConnected ();
		}

		private void ContainedCheck_CheckedChanged (object sender, EventArgs e)
		{
			SelectCharacter (IsCharacterVisible);
			ShowCharacterState ();
			ShowConnected ();
		}

		private void ShowButton_Click (object sender, EventArgs e)
		{
			if (mCharacter != null)
			{
				mCharacter.Show (false);
			}
		}

		private void HideButton_Click (object sender, EventArgs e)
		{
			if (mCharacter != null)
			{
				mCharacter.Hide (false);
			}
		}

		private void SpeakButton_Click (object sender, EventArgs e)
		{
			if (mCharacter != null)
			{
				mCharacter.Speak (SpeechText.Text, null);
			}
		}

		private void ListenCheck_CheckedChanged (object sender, EventArgs e)
		{
			if (mCharacter != null)
			{
				mCharacter.Listen (ListenCheck.Checked);
			}
		}

		private void TestDaControl_AgentShow (object Sender, DoubleAgent.AxControl.ShowEvent e)
		{
			ShowCharacterState ();
		}

		private void TestDaControl_AgentHide (object Sender, DoubleAgent.AxControl.HideEvent e)
		{
			ShowCharacterState ();
		}

		private void TestDaControl_AgentListenStart (object Sender, DoubleAgent.AxControl.ListenStartEvent e)
		{
			ShowCharacterState ();
		}

		private void TestDaControl_AgentListenComplete (object Sender, DoubleAgent.AxControl.ListenCompleteEvent e)
		{
			ShowCharacterState ();
		}
	}
}
