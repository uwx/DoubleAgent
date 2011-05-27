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
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using DoubleAgent;
using DoubleAgent.Character;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Updates;

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

		private void ShowAllVoices ()
		{
			if (mVoices == null)
			{
				mVoices = new Sapi4Voices ();
				ComboBoxName.Items.Clear ();

				foreach (Sapi4VoiceInfo lVoiceInfo in mVoices)
				{
					ComboBoxName.Items.Add (new VoiceComboItem (lVoiceInfo));
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void CheckBoxUseTTS_Checked (object sender, RoutedEventArgs e)
		{
			if (!IsPanelFilling && (CharacterFile != null) && !Program.FileIsReadOnly)
			{
				HandleEnabledChanged ();
			}
		}

		private void ComboBoxName_SelectionChanged (object sender, SelectionChangedEventArgs e)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleVoiceChanged ();
			}
		}

		#endregion
	}
}
