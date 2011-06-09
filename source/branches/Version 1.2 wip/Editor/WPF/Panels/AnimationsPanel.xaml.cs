﻿/////////////////////////////////////////////////////////////////////////////
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
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Navigation;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Panels
{
	public partial class AnimationsPanel : AgentCharacterEditor.Panels.FilePartPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public AnimationsPanel ()
		{
			InitializeComponent ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Navigation

		public new class PanelContext : FilePartPanel.PanelContext
		{
			public PanelContext (AnimationsPanel pPanel)
				: base (pPanel)
			{
				SelectedAnimation = pPanel.ListViewAnimations.SelectedIndex;
			}

			public void RestoreContext (AnimationsPanel pPanel)
			{
				base.RestoreContext (pPanel);
				pPanel.ListViewAnimations.SelectedIndex = SelectedAnimation;
			}

			public int SelectedAnimation
			{
				get;
				protected set;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowAnimationNames (String[] pAnimationNames)
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				ListViewAnimations.SetVerticalScrollBarVisibility (ScrollBarVisibility.Disabled);

				if ((pAnimationNames == null) || (pAnimationNames.Length <= 0))
				{
					ListViewAnimations.Items.Clear ();
				}
				else
				{
					ListViewItemCommon lItem;
					int lNdx;

					ListViewAnimations.UpdateItemCount (pAnimationNames.Length);

					for (lNdx = 0; lNdx < pAnimationNames.Length; lNdx++)
					{
						lItem = ListViewAnimations.Items[lNdx] as ListViewItemCommon;
						lItem.HorizontalAlignment = System.Windows.HorizontalAlignment.Left;
						lItem.Content = pAnimationNames[lNdx];
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private ListViewItemCommon SelectAnimation (String pAnimationName)
		{
			foreach (ListViewItemCommon lItem in ListViewAnimations.Items)
			{
				if (String.Compare (lItem.Content as String, pAnimationName, false) == 0)
				{
					lItem.IsSelected = true;
					return lItem;
				}
			}
			return null;
		}

		private FileAnimation GetSelectedAnimation (Boolean pIncludeFocus)
		{
			return GetSelectedAnimation (ListViewAnimations.SelectedItem as ListViewItemCommon);
		}
		private FileAnimation GetSelectedAnimation (ListViewItemCommon pItem)
		{
			FileAnimation lAnimation = null;

			if ((pItem != null) && (CharacterFile.Gestures.Contains (pItem.Content as String)))
			{
				lAnimation = CharacterFile.Gestures[pItem.Content as String];
			}
			return lAnimation;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowPaletteImage (System.Drawing.Bitmap pPaletteBitmap)
		{
			ImagePalette.Source = pPaletteBitmap.MakeImageSource ();
		}

		private void ShowPaletteTransparency (int pTransparencyNdx, System.Drawing.Color pTransparencyColor)
		{
			if (pTransparencyColor.Equals (System.Drawing.Color.Empty))
			{
				LabelTransparencySample.Background = Background;
			}
			else
			{
				LabelTransparencySample.Background = pTransparencyColor.ToWPFBrush ();
				LabelTransparencySample.Foreground = GetContrastingColor (pTransparencyColor).ToWPFBrush ();
			}
			if (pTransparencyNdx < 0)
			{
				LabelTransparencySample.Content = String.Empty;
				LabelTransparency.IsEnabled = false;
			}
			else
			{
				LabelTransparencySample.Content = pTransparencyNdx.ToString ();
				LabelTransparency.IsEnabled = !Program.FileIsReadOnly;
			}
		}

		private void ShowSelectedTransparency (int pTransparencyNdx, System.Drawing.Color pTransparencyColor)
		{
			if (pTransparencyNdx < 0)
			{
				LabelColorSample.Visibility = Visibility.Hidden;
				LabelTransparencyClick.Visibility = Program.FileIsReadOnly ? Visibility.Collapsed : Visibility.Hidden;
			}
			else
			{
				LabelColorSample.Background = pTransparencyColor.ToWPFBrush ();
				LabelColorSample.Foreground = GetContrastingColor (pTransparencyColor).ToWPFBrush ();
				LabelColorSample.Content = pTransparencyNdx.ToString ();
				LabelColorSample.Visibility = Visibility.Visible;
				LabelTransparencyClick.Visibility = Program.FileIsReadOnly ? Visibility.Collapsed : Visibility.Visible;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private int PaletteMouseColorNdx (System.Windows.Point pMousePos)
		{
			System.Windows.Point lColorPos = new System.Windows.Point ();

			lColorPos.X = pMousePos.X * 16 / ImagePalette.ActualWidth;
			lColorPos.Y = pMousePos.Y * 16 / ImagePalette.ActualHeight;

			return (Math.Min (Math.Max ((int)Math.Floor (lColorPos.Y), 0), 16) * 16) + Math.Min (Math.Max ((int)Math.Floor (lColorPos.X), 0), 16);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void TextBoxNewName_TextChanged (object sender, TextChangedEventArgs e)
		{
			ShowAddState ();
		}

		private void ListViewAnimations_SelectionChanged (object sender, SelectionChangedEventArgs e)
		{
			ShowDeleteState ();
		}

		private void ListViewAnimations_MouseDoubleClick (object sender, MouseButtonEventArgs e)
		{
			if (!IsPanelEmpty && (Navigate != null))
			{
				FileAnimation lAnimation = GetSelectedAnimation (false);

				if (lAnimation != null)
				{
					try
					{
						Navigate (this, new AgentCharacterEditor.Navigation.NavigationEventArgs (new ResolveAnimation (lAnimation)));
					}
					catch
					{
					}
				}
			}
		}

		private void ListViewAnimations_ContextMenuOpening (object sender, ContextMenuEventArgs e)
		{
			if (IsPanelEmpty)
			{
				e.Handled = true;
			}
			else
			{
				MenuItemAdd.IsEnabled = CanAddAnimation;
				MenuItemAdd.SetTitle (AddAnimationTitle);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonAdd_Click (object sender, RoutedEventArgs e)
		{
			HandleAddAnimation ();
		}

		private void ButtonDelete_Click (object sender, RoutedEventArgs e)
		{
			HandleDeleteAnimation ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void NumericFrameWidth_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (NumericFrameWidth.IsModified)
			{
				HandleUpdateFrameWidth ();
			}
			NumericFrameWidth.IsModified = false;
		}

		private void NumericFrameHeight_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (NumericFrameHeight.IsModified)
			{
				HandleUpdateFrameHeight ();
			}
			NumericFrameHeight.IsModified = false;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxPaletteFile_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (TextBoxPaletteFile.IsModified)
			{
				HandleUpdatePaletteFile ();
			}
			TextBoxPaletteFile.IsModified = false;
		}

		private void ButtonPaletteImport_Click (object sender, RoutedEventArgs e)
		{
			HandleImportPalette ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ImagePalette_MouseMove (object sender, MouseEventArgs e)
		{
			ShowSelectedTransparency (PaletteMouseColorNdx (e.GetPosition (ImagePalette)));
		}

		private void ImagePalette_MouseLeave (object sender, MouseEventArgs e)
		{
			ShowSelectedTransparency (-1, System.Drawing.Color.Empty);
		}

		private void ImagePalette_MouseDown (object sender, MouseButtonEventArgs e)
		{
			HandleUpdatePaletteTransparency (PaletteMouseColorNdx (e.GetPosition (ImagePalette)));
		}

		#endregion

		private void ListViewAnimations_MouseUp (object sender, MouseButtonEventArgs e)
		{
			System.Diagnostics.Debug.Print ("ListViewAnimations_MouseUp");
		}
	}
}
