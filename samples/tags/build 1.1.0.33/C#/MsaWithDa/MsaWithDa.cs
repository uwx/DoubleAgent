using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace MsaWithDa
{
	public partial class MsaWithDa : Form
	{
		const String mMsCharacterName = "Genie";
		const String mMsCharacterFile = "C:\\Windows\\MsAgent\\Chars\\Genie.acs";
		const String mDaCharacterName = "Merlin";
		const String mDaCharacterFile = "C:\\Windows\\MsAgent\\Chars\\Merlin.acs";

		AgentServerObjects.IAgentEx mMsServer;
		AgentServerObjects.IAgentCharacterEx mMsServerChar;
		int mMsServerCharId = 0;
		AgentObjects.IAgentCtlCharacterEx mMsControlChar;

		DoubleAgent.Server.Server mDaServer;
		DoubleAgent.Server.Character mDaServerChar;
		int mDaServerCharId = 0;
		DoubleAgent.Control.Control mDaControl;
		DoubleAgent.Control.Character mDaControlChar;

		public MsaWithDa()
		{
			InitializeComponent();

			mDaControl = new DoubleAgent.Control.Control ();
			mDaControl.Show += new DoubleAgent.Control.ShowEventHandler (mDaControl_Show);
			mDaControl.Hide += new DoubleAgent.Control.HideEventHandler (mDaControl_Hide);

			SetMsControlButtons();
			SetMsServerButtons();
			SetDaControlButtons();
			SetDaServerButtons();
		}

		private void SetMsControlButtons()
		{
			if (mMsControlChar == null)
			{
				mLoadMsControlChar.Enabled = true;
				mUnloadMsControlChar.Enabled = false;
				mShowMsControlChar.Enabled = false;
				mHideMsControlChar.Enabled = false;
			}
			else
			{
				mLoadMsControlChar.Enabled = false;
				mUnloadMsControlChar.Enabled = true;
				mShowMsControlChar.Enabled = !mMsControlChar.Visible;
				mHideMsControlChar.Enabled = mMsControlChar.Visible;
			}
		}

		private void SetMsServerButtons()
		{
			if (mMsServerChar == null)
			{
				mLoadMsServerChar.Enabled = true;
				mUnloadMsServerChar.Enabled = false;
				mShowMsServerChar.Enabled = false;
				mHideMsServerChar.Enabled = false;
			}
			else
			{
				mLoadMsServerChar.Enabled = false;
				mUnloadMsServerChar.Enabled = true;
				mShowMsServerChar.Enabled = true;
				mHideMsServerChar.Enabled = true;
			}
		}

		private void SetDaControlButtons()
		{
			if (mDaControlChar == null)
			{
				mLoadDaControlChar.Enabled = true;
				mUnloadDaControlChar.Enabled = false;
				mShowDaControlChar.Enabled = false;
				mHideDaControlChar.Enabled = false;
			}
			else
			{
				mLoadDaControlChar.Enabled = false;
				mUnloadDaControlChar.Enabled = true;
				mShowDaControlChar.Enabled = !mDaControlChar.Visible;
				mHideDaControlChar.Enabled = mDaControlChar.Visible;
			}
		}

		private void SetDaServerButtons()
		{
			if (mDaServerChar == null)
			{
				mLoadDaServerChar.Enabled = true;
				mUnloadDaServerChar.Enabled = false;
				mShowDaServerChar.Enabled = false;
				mHideDaServerChar.Enabled = false;
			}
			else
			{
				mLoadDaServerChar.Enabled = false;
				mUnloadDaServerChar.Enabled = true;
				mShowDaServerChar.Enabled = true;
				mHideDaServerChar.Enabled = true;
			}
		}

		private void MsControl_ShowEvent(object sender, AxAgentObjects._AgentEvents_ShowEvent e)
		{
			SetMsControlButtons();
		}

		private void MsControl_HideEvent(object sender, AxAgentObjects._AgentEvents_HideEvent e)
		{
			SetMsControlButtons();
		}

        private void mDaControl_Show(string CharacterID, DoubleAgent.Control.VisibilityCauseType Cause)
        {
            SetDaControlButtons();
        }

		private void mDaControl_Hide (string CharacterID, DoubleAgent.Control.VisibilityCauseType Cause)
        {
			SetDaControlButtons();
		}

		private void LoadMsServerChar_Click(object sender, EventArgs e)
		{
			int lReqId;
			object lCharacter;

			if (mMsServer == null)
			{
				mMsServer = (AgentServerObjects.IAgentEx)new AgentServerObjects.AgentServer();
			}

			if (mMsServerChar == null)
			{
				mMsServer.Load(mMsCharacterFile, out mMsServerCharId, out lReqId);
				mMsServer.GetCharacter(mMsServerCharId, out lCharacter);
				mMsServerChar = (AgentServerObjects.IAgentCharacterEx)lCharacter;
				if (mMsServerChar != null)
				{
					mMsServerChar.MoveTo(700, 100, 0, out lReqId);
				}
			}
			SetMsServerButtons();
		}

		private void UnloadMsServerChar_Click(object sender, EventArgs e)
		{
			if ((mMsServer != null) && (mMsServerChar != null))
			{
				mMsServerChar = null;
				mMsServer.Unload(mMsServerCharId);
				mMsServerCharId = 0;
			}
			SetMsServerButtons();
		}

		private void ShowMsServerChar_Click(object sender, EventArgs e)
		{
			int lReqId;

			if (mMsServerChar != null)
			{
				mMsServerChar.Show(0, out lReqId);
				mMsServerChar.Speak(mMsCharacterName + ", Microsoft Agent", "", out lReqId);
			}
			SetMsServerButtons();
		}

		private void HideMsServerChar_Click(object sender, EventArgs e)
		{
			int lReqId;

			if (mMsServerChar != null)
			{
				mMsServerChar.Hide(0, out lReqId);
			}
			SetMsServerButtons();
		}

		private void ShowMsServerOptions_Click(object sender, EventArgs e)
		{
			AgentServerObjects.IAgentPropertySheet lServerProps;

			if (mMsServer == null)
			{
				mMsServer = (AgentServerObjects.IAgentEx)new AgentServerObjects.AgentServer();
			}
			lServerProps = (AgentServerObjects.IAgentPropertySheet)mMsServer;
			lServerProps.SetVisible(1);
		}

		private void ShowMsServerChars_Click(object sender, EventArgs e)
		{
			if (mMsServer == null)
			{
				mMsServer = (AgentServerObjects.IAgentEx)new AgentServerObjects.AgentServer();
			}
			mMsServer.ShowDefaultCharacterProperties(0, 0, 1);
		}

		private void LoadMsControlChar_Click(object sender, EventArgs e)
		{
			if (mMsControlChar == null)
			{
				mMsControl.RaiseRequestErrors = false;
				mMsControl.Characters.Load(mMsCharacterName, mMsCharacterFile);
				mMsControl.RaiseRequestErrors = true;
				mMsControlChar = mMsControl.Characters.Character(mMsCharacterName);
				if (mMsControlChar != null)
				{
					mMsControlChar.MoveTo(700, 100, null);
				}
			}
			SetMsControlButtons();
		}

		private void UnloadMsControlChar_Click(object sender, EventArgs e)
		{
			if (mMsControlChar != null)
			{
				mMsControlChar = null;
				mMsControl.Characters.Unload(mMsCharacterName);
			}
			SetMsControlButtons();
		}

		private void ShowMsControlChar_Click(object sender, EventArgs e)
		{
			if (mMsControlChar != null)
			{
				mMsControlChar.Show(false);
				mMsControlChar.Speak(mMsControlChar.Name + ", Microsoft Agent", null);
			}
		}

		private void HideMsControlChar_Click(object sender, EventArgs e)
		{
			if (mMsControlChar != null)
			{
				mMsControlChar.Hide(false);
			}
		}

		private void ShowMsControlOptions_Click(object sender, EventArgs e)
		{
			mMsControl.PropertySheet.Visible = true;
		}

		private void ShowMsControlChars_Click(object sender, EventArgs e)
		{
			mMsControl.ShowDefaultCharacterProperties();
		}

		private void LoadDaServerChar_Click(object sender, EventArgs e)
		{
			int lReqId;

			if (mDaServer == null)
			{
				mDaServer = new DoubleAgent.Server.Server();
			}

			if (mDaServerChar == null)
			{
				mDaServer.Load(mDaCharacterFile, out mDaServerCharId, out lReqId);
                mDaServerChar = mDaServer.get_Character(mDaServerCharId);
				if (mDaServerChar != null)
				{
					mDaServerChar.MoveTo(700, 300, 0, out lReqId);
				}
			}
			SetDaServerButtons();
		}

		private void UnloadDaServerChar_Click(object sender, EventArgs e)
		{
			if ((mDaServer != null) && (mDaServerChar != null))
			{
				mDaServerChar = null;
				mDaServer.Unload(mDaServerCharId);
				mDaServerCharId = 0;
			}
			SetDaServerButtons();
		}

		private void ShowDaServerChar_Click(object sender, EventArgs e)
		{
			int lReqId;

			if (mDaServerChar != null)
			{
				mDaServerChar.Show(0, out lReqId);
				mDaServerChar.Speak(mDaCharacterName + ", Double Agent", "", out lReqId);
			}
			SetDaServerButtons();
		}

		private void HideDaServerChar_Click(object sender, EventArgs e)
		{
			int lReqId;

			if (mDaServerChar != null)
			{
				mDaServerChar.Hide(0, out lReqId);
			}
			SetDaServerButtons();
		}

		private void ShowDaServerOptions_Click(object sender, EventArgs e)
		{
			DoubleAgent.Server.PropertySheet lServerProps;

			if (mDaServer == null)
			{
				mDaServer = new DoubleAgent.Server.Server();
			}
			lServerProps = mDaServer.PropertySheet;
			lServerProps.SetVisible(1);
		}

		private void ShowDaServerChars_Click(object sender, EventArgs e)
		{
			if (mDaServer == null)
			{
				mDaServer = new DoubleAgent.Server.Server();
			}
			mDaServer.ShowDefaultCharacterProperties(0, 0, 1);
		}

		private void LoadDaControlChar_Click(object sender, EventArgs e)
		{
			if (mDaControlChar == null)
			{
				mDaControl.RaiseRequestErrors = false;
				mDaControl.Characters.Load(mDaCharacterName, mDaCharacterFile);
				mDaControl.RaiseRequestErrors = true;
				mDaControlChar = mDaControl.Characters.Character(mDaCharacterName);
				if (mDaControlChar != null)
				{
					mDaControlChar.MoveTo(700, 300, null);
				}
			}
			SetDaControlButtons();
		}

		private void UnloadDaControlChar_Click(object sender, EventArgs e)
		{
			if (mDaControlChar != null)
			{
				mDaControlChar = null;
				mDaControl.Characters.Unload(mDaCharacterName);
			}
			SetDaControlButtons();
		}

		private void ShowDaControlChar_Click(object sender, EventArgs e)
		{
			if (mDaControlChar != null)
			{
				mDaControlChar.Show(false);
				mDaControlChar.Speak(mDaControlChar.Name + ", Double Agent", null);
			}
		}

		private void HideDaControlChar_Click(object sender, EventArgs e)
		{
			if (mDaControlChar != null)
			{
				mDaControlChar.Hide(false);
			}
		}

		private void ShowDaControlOptions_Click(object sender, EventArgs e)
		{
			mDaControl.PropertySheet.Visible = true;
		}

		private void ShowDaControlChars_Click(object sender, EventArgs e)
		{
			mDaControl.ShowDefaultCharacterProperties(null, null);
		}

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AboutBox lAboutBox = new AboutBox();
            lAboutBox.mDaControl = mDaControl;
            lAboutBox.ShowDialog(this);
        }
	}
}
