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
			Boolean lWasFilling = PushIsPanelFilling (true);

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

#if DEBUG_NOT
					System.Diagnostics.Debug.Print ("Font [{0}] Size [{1}] [{2} {3}] [{4} {5}]", GetFontFamilyName (lFont), lFont.SizeInPoints, lFont.Size, lFont.Unit, lFont.GetHeight(), lFont.GetHeight(96.0f));
#endif

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
						LabelBalloonFontSample.FontSize = lFont.GetHeight (96.0f);
						LabelBalloonFontSample.FontWeight = lFont.Bold ? FontWeights.Bold : FontWeights.Normal;
						LabelBalloonFontSample.FontStyle = ((lFont.Style & System.Drawing.FontStyle.Italic) != 0) ? FontStyles.Italic : FontStyles.Normal;
					}
					catch
					{
					}
				}
				ButtonBalloonFont.IsEnabled = !Program.FileIsReadOnly;
			}

			PopIsPanelFilling (lWasFilling);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowBalloonColors ()
		{
			Boolean lWasFilling = PushIsPanelFilling (true);

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

			PopIsPanelFilling (lWasFilling);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowBalloonPreview ()
		{
			Boolean lWasFilling = PushIsPanelFilling (true);

			if (IsPanelEmpty)
			{
				GroupBoxBalloonPreview.IsEnabled = false;
				WordBalloonPreview.CharacterFile = null;
			}
			else
			{
				GroupBoxBalloonPreview.IsEnabled = true;
				WordBalloonPreview.CharacterFile = CharacterFile;
			}
			WordBalloonPreview.InvalidateArrange ();
			WordBalloonPreview.InvalidateVisual ();

			PopIsPanelFilling (lWasFilling);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void CheckBoxWordBalloon_CheckChanged (object sender, RoutedEventArgs e)
		{
			if (!IsPanelFilling && (CharacterFile != null) && !Program.FileIsReadOnly)
			{
				HandleEnabledChanged ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void NumericCharsPerLine_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (!IsPanelFilling && NumericCharsPerLine.IsModified && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleCharsPerLineChanged ();
			}
			NumericCharsPerLine.IsModified = false;
		}

		private void NumericNumLines_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (!IsPanelFilling && NumericNumLines.IsModified && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleNumLinesChanged ();
			}
			NumericNumLines.IsModified = false;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void RadioButtonNumLines_Checked (object sender, RoutedEventArgs e)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleSizeToTextChanged ();
			}
		}

		private void RadioButtonSizeToText_Checked (object sender, RoutedEventArgs e)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleSizeToTextChanged ();
			}
		}

		private void CheckBoxAutoHide_CheckChanged (object sender, RoutedEventArgs e)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleAutoHideChanged ();
			}
		}

		private void CheckBoxAutoPace_CheckChanged (object sender, RoutedEventArgs e)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleAutoPaceChanged ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonBalloonFont_Click (object sender, RoutedEventArgs e)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateFont ();
			}
		}

		private void ButtonBalloonForeground_Click (object sender, RoutedEventArgs e)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateForeground ();
			}
		}

		private void ButtonBalloonBackground_Click (object sender, RoutedEventArgs e)
		{
			if (!IsPanelFilling && (!IsPanelEmpty) && (!Program.FileIsReadOnly))
			{
				UpdateBackground ();
			}
		}

		private void ButtonBalloonBorder_Click (object sender, RoutedEventArgs e)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateBorder ();
			}
		}

		#endregion
	}
}
