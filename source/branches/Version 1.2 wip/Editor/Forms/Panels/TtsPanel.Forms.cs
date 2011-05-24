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
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Updates;
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Panels
{
	public partial class TtsPanel : AgentCharacterEditor.Panels.FilePartPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public TtsPanel ()
		{
			InitializeComponent ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		protected override bool PushIsPanelShowing (bool pIsPanelShowing)
		{
			if (pIsPanelShowing && !IsPanelShowing)
			{
				SuspendLayout ();
			}
			return base.PushIsPanelShowing (pIsPanelShowing);
		}

		protected override void PopIsPanelShowing (bool pWasPanelShowing)
		{
			if (!pWasPanelShowing && IsPanelShowing)
			{
				ResumeLayout (true);
			}
			base.PopIsPanelShowing (pWasPanelShowing);
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

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void CheckBoxUseTTS_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsPanelShowing && (CharacterFile != null) && !Program.FileIsReadOnly)
			{
				HandleEnabledChanged ();
			}
		}

		private void ComboBoxName_SelectionChangeCommitted (object sender, EventArgs e)
		{
			if (!IsPanelShowing && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleVoiceChanged ();
			}
		}

		#endregion
	}
}
