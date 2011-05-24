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
using AgentCharacterEditor.Updates;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Panels
{
	public partial class BalloonPanel : AgentCharacterEditor.Panels.FilePartPanel
	{
		public BalloonPanel ()
		{
			InitializeComponent ();
		}

		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowBalloonStyle ()
		{
			Boolean lWasShowing = PushIsPanelShowing (true);

			if (IsPanelEmpty)
			{
				TextBoxCharsPerLine.Clear ();
				TextBoxNumLines.Clear ();
				RadioButtonSizeToText.IsChecked = false;
				RadioButtonNumLines.IsChecked = false;
				CheckBoxAutoHide.IsChecked = false;
				CheckBoxAutoPace.IsChecked = false;
			}
			else
			{
				TextBoxCharsPerLine.Text = FileBalloon.PerLine.ToString ();
				TextBoxNumLines.Text = FileBalloon.Lines.ToString ();

				if ((CharacterFile.Header.Style & CharacterStyle.SizeToText) != CharacterStyle.None)
				{
					RadioButtonSizeToText.IsChecked = true;
					RadioButtonNumLines.IsChecked = false;
					TextBoxNumLines.IsEnabled = false;
				}
				else
				{
					RadioButtonSizeToText.IsChecked = false;
					RadioButtonNumLines.IsChecked = true;
					TextBoxNumLines.IsEnabled = !Program.FileIsReadOnly;
				}

				CheckBoxAutoHide.IsChecked = ((CharacterFile.Header.Style & CharacterStyle.NoAutoHide) == CharacterStyle.None);
				CheckBoxAutoPace.IsChecked = ((CharacterFile.Header.Style & CharacterStyle.NoAutoPace) == CharacterStyle.None);
			}

			LabelCharsPerLine.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly;
			TextBoxCharsPerLine.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly;
			RadioButtonSizeToText.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly;
			RadioButtonNumLines.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly;
			CheckBoxAutoHide.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly;
			CheckBoxAutoPace.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly;

			TextBoxCharsPerLine.IsModified = false;
			TextBoxNumLines.IsModified = false;

			PopIsPanelShowing (lWasShowing);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowBalloonFont ()
		{
			Boolean lWasShowing = PushIsPanelShowing (true);

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
				if (FileBalloon.Font == null)
				{
					LabelBalloonFontSample.Content = null;
					LabelBalloonFontSample.FontFamily = FontFamily;
					LabelBalloonFontSample.FontSize = FontSize;
					LabelBalloonFontSample.FontStyle = FontStyle;
					LabelBalloonFontSample.FontWeight = FontWeight;
				}
				else
				{
					System.Drawing.Font lFont = FileBalloon.Font;

					TextBalloonFontSample.Text = FormatFontName (lFont);
					try
					{
						LabelBalloonFontSample.FontFamily = (new FontFamilyConverter ()).ConvertFrom (GetFontFamilyName (lFont)) as FontFamily;
					}
					catch
					{
					}
					try
					{
						LabelBalloonFontSample.FontSize = lFont.SizeInPoints;
						LabelBalloonFontSample.FontWeight = lFont.Bold ? FontWeights.Bold : FontWeights.Normal;
						LabelBalloonFontSample.FontStyle = ((lFont.Style & System.Drawing.FontStyle.Italic) != 0) ? FontStyles.Italic : FontStyles.Normal;
					}
					catch
					{
					}
				}
				ButtonBalloonFont.IsEnabled = !Program.FileIsReadOnly;
			}

			PopIsPanelShowing (lWasShowing);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowBalloonColors ()
		{
			Boolean lWasShowing = PushIsPanelShowing (true);

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
				LabelBalloonForegroundSample.Background = new SolidColorBrush (Color.FromArgb (FileBalloon.FgColor.A, FileBalloon.FgColor.R, FileBalloon.FgColor.G, FileBalloon.FgColor.B));
				LabelBalloonBackgroundSample.Background = new SolidColorBrush (Color.FromArgb (FileBalloon.BkColor.A, FileBalloon.BkColor.R, FileBalloon.BkColor.G, FileBalloon.BkColor.B));
				LabelBalloonBorderSample.Background = new SolidColorBrush (Color.FromArgb (FileBalloon.BrColor.A, FileBalloon.BrColor.R, FileBalloon.BrColor.G, FileBalloon.BrColor.B));
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

			PopIsPanelShowing (lWasShowing);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowBalloonPreview ()
		{
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void CheckBoxWordBalloon_Checked (object sender, RoutedEventArgs e)
		{
			if (!IsPanelShowing && (CharacterFile != null) && !Program.FileIsReadOnly)
			{
				HandleEnabledChanged ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxCharsPerLine_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (!IsPanelShowing && TextBoxCharsPerLine.IsModified && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

				try
				{
					lUpdate.PerLine = UInt16.Parse (TextBoxCharsPerLine.Text);
				}
				catch
				{
				}
				PushIsPanelShowing (true);
				TextBoxCharsPerLine.Text = lUpdate.PerLine.ToString ();
				PopIsPanelShowing (false);

				UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterBalloon, this);
			}
			TextBoxCharsPerLine.IsModified = false;
		}

		private void TextBoxNumLines_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (!IsPanelShowing && TextBoxNumLines.IsModified && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

				try
				{
					lUpdate.Lines = UInt16.Parse (TextBoxNumLines.Text);
				}
				catch
				{
				}
				PushIsPanelShowing (true);
				TextBoxNumLines.Text = lUpdate.Lines.ToString ();
				PopIsPanelShowing (false);

				UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterBalloon, this);
			}
			TextBoxNumLines.IsModified = false;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void RadioButtonNumLines_Checked (object sender, RoutedEventArgs e)
		{
			if (!IsPanelShowing && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleSizeToTextChanged ();
			}
		}

		private void RadioButtonSizeToText_Checked (object sender, RoutedEventArgs e)
		{
			if (!IsPanelShowing && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleSizeToTextChanged ();
			}
		}

		private void CheckBoxAutoHide_Checked (object sender, RoutedEventArgs e)
		{
			if (!IsPanelShowing && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleAutoHideChanged ();
			}
		}

		private void CheckBoxAutoPace_Checked (object sender, RoutedEventArgs e)
		{
			if (!IsPanelShowing && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleAutoPaceChanged ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonBalloonFont_Click (object sender, RoutedEventArgs e)
		{
			if (!IsPanelShowing && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateFont ();
			}
		}

		private void ButtonBalloonForeground_Click (object sender, RoutedEventArgs e)
		{
			if (!IsPanelShowing && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateForeground ();
			}
		}

		private void ButtonBalloonBackground_Click (object sender, RoutedEventArgs e)
		{
			if (!IsPanelShowing && (!IsPanelEmpty) && (!Program.FileIsReadOnly))
			{
				UpdateBackground ();
			}
		}

		private void ButtonBalloonBorder_Click (object sender, RoutedEventArgs e)
		{
			if (!IsPanelShowing && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateBorder ();
			}
		}

		#endregion
	}
}
