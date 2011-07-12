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
using System.Windows;
using System.Windows.Media;
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Updates;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Panels
{
	public partial class BalloonPanel : AgentCharacterEditor.Panels.FilePartPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public BalloonPanel ()
		{
			InitializeComponent ();

			NumericNumLines.Minimum = (decimal)FileBalloon.MinLines;
			NumericNumLines.Maximum = (decimal)FileBalloon.MaxLines;
			NumericCharsPerLine.Minimum = (decimal)FileBalloon.MinPerLine;
			NumericCharsPerLine.Maximum = (decimal)FileBalloon.MaxPerLine;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowBalloonFont ()
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				if (IsPanelEmpty)
				{
					LabelBalloonFontSample.Content = null;
					LabelBalloonFontSample.FontFamily = FontFamily;
					LabelBalloonFontSample.FontSize = FontSize;
					LabelBalloonFontSample.FontStyle = FontStyle;
					LabelBalloonFontSample.FontWeight = FontWeight;
					ButtonBalloonFont.IsEnabled = false;
				}
				else
				{
					LabelBalloonFontSample.Content = TextBalloonFontSample;

					if (FileBalloon.Font == null)
					{
						TextBalloonFontSample.Text = String.Empty;
						TextBalloonFontSample.FontFamily = FontFamily;
						TextBalloonFontSample.FontSize = FontSize;
						TextBalloonFontSample.FontStyle = FontStyle;
						TextBalloonFontSample.FontWeight = FontWeight;
						TextBalloonFontSample.TextDecorations = null;
						TextBalloonFontSample.TextEffects = null;
					}
					else
					{
						System.Drawing.Font lFont = FileBalloon.Font;
						DoubleAgent.FontProperties lFontProperties = new DoubleAgent.FontProperties (lFont);
#if DEBUG_NOT
						System.Diagnostics.Debug.Print ("Font [{0}] Size [{1}] [{2} {3}] [{4} {5}]", GetFontFamilyName (lFont), lFont.SizeInPoints, lFont.Size, lFont.Unit, lFont.GetHeight (), lFont.GetHeight (96.0f));
#endif
						TextBalloonFontSample.Text = FormatFontName (lFont);
						TextBalloonFontSample.FontFamily = lFontProperties.Typeface.FontFamily;
						TextBalloonFontSample.FontSize = lFontProperties.FontRenderingEmSize;
						TextBalloonFontSample.FontStyle = lFontProperties.Typeface.Style;
						TextBalloonFontSample.FontWeight = lFontProperties.Typeface.Weight;
						TextBalloonFontSample.FontStretch = lFontProperties.Typeface.Stretch;
						TextBalloonFontSample.TextDecorations = lFontProperties.TextDecorations;
						TextBalloonFontSample.TextEffects = lFontProperties.TextEffects;
#if DEBUG_NOT
						System.Diagnostics.Debug.Print ("Font [{0}] [{1}] [{2}] [{3}] [{4}]", TextBalloonFontSample.Text, TextBalloonFontSample.FontFamily, TextBalloonFontSample.FontSize, TextBalloonFontSample.FontStyle, TextBalloonFontSample.FontWeight);
#endif
					}
					ButtonBalloonFont.IsEnabled = !Program.FileIsReadOnly;
				}
				LabelBalloonFontSample.InvalidateArrange();
			}
		}

		//=============================================================================

		private void ShowBalloonColors ()
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				if (IsPanelEmpty)
				{
					LabelBalloonForegroundSample.Background = Background;
					LabelBalloonBackgroundSample.Background = Background;
					LabelBalloonBorderSample.Background = Background;
					LabelBalloonFontSample.Foreground = Foreground;
					LabelBalloonFontSample.Background = Background;
					LabelBalloonFontSample.BorderThickness = new Thickness (0.0);
				}
				else
				{
					LabelBalloonForegroundSample.Background = FileBalloon.FgColor.ToWPFBrush ();
					LabelBalloonBackgroundSample.Background = FileBalloon.BkColor.ToWPFBrush ();
					LabelBalloonBorderSample.Background = FileBalloon.BrColor.ToWPFBrush ();
					LabelBalloonFontSample.Foreground = LabelBalloonForegroundSample.Background;
					LabelBalloonFontSample.Background = LabelBalloonBackgroundSample.Background;
					LabelBalloonFontSample.BorderBrush = LabelBalloonBorderSample.Background;
					LabelBalloonFontSample.BorderThickness = new Thickness (1.0);
				}

				ButtonBalloonForeground.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly;
				ButtonBalloonBackground.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly;
				ButtonBalloonBorder.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly;
				LabelBalloonForegroundSample.IsEnabled = !IsPanelEmpty;
				LabelBalloonBackgroundSample.IsEnabled = !IsPanelEmpty;
				LabelBalloonBorderSample.IsEnabled = !IsPanelEmpty;
			}
		}

		//=============================================================================

		private void ShowBalloonPreview ()
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				if (IsPanelEmpty)
				{
					GroupBoxBalloonPreview.IsEnabled = false;
					WordBalloonPreview.CharacterFile = null;
					WordBalloonPreview.Visibility = Visibility.Collapsed;
				}
				else
				{
					GroupBoxBalloonPreview.IsEnabled = true;
					WordBalloonPreview.CharacterFile = CharacterFile;
					WordBalloonPreview.Visibility = Visibility.Visible;
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void CheckBoxWordBalloon_CheckChanged (object sender, RoutedEventArgs e)
		{
			HandleEnabledChanged ();
		}

		//=============================================================================

		private void NumericCharsPerLine_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (NumericCharsPerLine.IsModified)
			{
				HandleCharsPerLineChanged ();
			}
			NumericCharsPerLine.IsModified = false;
		}

		private void NumericNumLines_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (NumericNumLines.IsModified)
			{
				HandleNumLinesChanged ();
			}
			NumericNumLines.IsModified = false;
		}

		//=============================================================================

		private void RadioButtonNumLines_Checked (object sender, RoutedEventArgs e)
		{
			HandleSizeToTextChanged ();
		}

		private void RadioButtonSizeToText_Checked (object sender, RoutedEventArgs e)
		{
			HandleSizeToTextChanged ();
		}

		private void CheckBoxAutoHide_CheckChanged (object sender, RoutedEventArgs e)
		{
			HandleAutoHideChanged ();
		}

		private void CheckBoxAutoPace_CheckChanged (object sender, RoutedEventArgs e)
		{
			HandleAutoPaceChanged ();
		}

		//=============================================================================

		private void ButtonBalloonFont_Click (object sender, RoutedEventArgs e)
		{
			HandleUpdateFont ();
		}

		private void ButtonBalloonForeground_Click (object sender, RoutedEventArgs e)
		{
			HandleUpdateForeground ();
		}

		private void ButtonBalloonBackground_Click (object sender, RoutedEventArgs e)
		{
			HandleUpdateBackground ();
		}

		private void ButtonBalloonBorder_Click (object sender, RoutedEventArgs e)
		{
			HandleUpdateBorder ();
		}

		#endregion
	}
}
