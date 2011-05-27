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
using System.Drawing;
using System.Windows.Forms;
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Properties;
using AgentCharacterEditor.Updates;
using DoubleAgent.Character;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor.Panels
{
	public partial class AnimationsPanel : FilePartPanel
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
				FocusedAnimation = pPanel.ListViewAnimations.FocusedIndex;
			}

			public void RestoreContext (AnimationsPanel pPanel)
			{
				base.RestoreContext (pPanel);
				pPanel.ListViewAnimations.SelectedIndex = SelectedAnimation;
				pPanel.ListViewAnimations.FocusedIndex = FocusedAnimation;
			}

			public int SelectedAnimation
			{
				get;
				protected set;
			}
			public int FocusedAnimation
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
			Boolean lWasFilling = PushIsPanelFilling (true);

			ListViewAnimations.BeginUpdate ();
			if ((pAnimationNames == null) || (pAnimationNames.Length <= 0))
			{
				ListViewAnimations.Items.Clear ();
			}
			else
			{
				int lNdx;

				ListViewAnimations.UpdateItemCount (pAnimationNames.Length);

				for (lNdx = 0; lNdx < pAnimationNames.Length; lNdx++)
				{
					ListViewAnimations.Items[lNdx].Text = pAnimationNames[lNdx];
				}
			}
			ListViewAnimations.EndUpdate ();
			ListViewAnimations.ArrangeIcons ();

			PopIsPanelFilling (lWasFilling);
		}

		private void ShowAddState (Object pAddUiObject, Boolean pAddEnabled, String pAddText)
		{
			if (pAddUiObject is ToolStripItem)
			{
				(pAddUiObject as ToolStripItem).Enabled = pAddEnabled;
				(pAddUiObject as ToolStripItem).Text = pAddText.NoMenuPrefix();
			}
		}

		private void ShowDeleteState (Object pDeleteUiObject, Boolean pDeleteEnabled, String pDeleteText)
		{
			if (pDeleteUiObject is ToolStripItem)
			{
				(pDeleteUiObject as ToolStripItem).Enabled = pDeleteEnabled;
				(pDeleteUiObject as ToolStripItem).Text = pDeleteText.NoMenuPrefix();
			}
		}

		/////////////////////////////////////////////////////////////////////////////

		protected override void OnEditMenu (object sender, Global.ContextMenuEventArgs e)
		{
			base.OnEditMenu (sender, e);

			if (!IsPanelEmpty && !Program.FileIsReadOnly && IsControlEditTarget (ListViewAnimations, e))
			{
				ToolStripMenuItem lMenuItem;

				e.ContextMenu.Items.Insert (0, new ToolStripSeparator ());
				e.ContextMenu.Items.Insert (0, lMenuItem = new ToolStripMenuItem (ButtonAdd.Text, ButtonAdd.Image, ButtonAdd_Click));
				lMenuItem.Enabled = ButtonAdd.Enabled;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private ListViewItemCommon SelectAnimation (String pAnimationName)
		{
			foreach (ListViewItemCommon lItem in ListViewAnimations.Items)
			{
				if (String.Compare (lItem.Text, pAnimationName, false) == 0)
				{
					lItem.Selected = true;
					return lItem;
				}
			}
			return null;
		}

		private FileAnimation GetSelectedAnimation (Boolean pIncludeFocus)
		{
			return GetSelectedAnimation (ListViewAnimations.GetSelectedItem (pIncludeFocus) as ListViewItemCommon);
		}
		private FileAnimation GetSelectedAnimation (ListViewItemCommon pItem)
		{
			FileAnimation lAnimation = null;

			if ((pItem != null) && (CharacterFile.Gestures.Contains (pItem.Text)))
			{
				lAnimation = CharacterFile.Gestures[pItem.Text];
			}
			return lAnimation;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowPaletteImage (System.Drawing.Bitmap pPaletteBitmap)
		{
			PictureBoxPalette.Image = pPaletteBitmap;
		}

		private void ShowPaletteTransparency (int pTransparencyNdx, System.Drawing.Color pTransparencyColor)
		{
			if (pTransparencyColor.Equals (System.Drawing.Color.Empty))
			{
				LabelTransparencySample.BackColor = BackColor;
			}
			else
			{
				LabelTransparencySample.BackColor = pTransparencyColor;
				LabelTransparencySample.ForeColor = (LabelTransparencySample.BackColor.GetBrightness () > 0.5) ? Color.Black : Color.White;
				LabelTransparency.Enabled = true;
			}
			if (pTransparencyNdx < 0)
			{
				LabelTransparencySample.ResetText ();
			}
			else
			{
				LabelTransparencySample.Text = pTransparencyNdx.ToString ();
				LabelTransparency.Enabled = false;
			}
		}

		private void ShowSelectedTransparency (int pTransparencyNdx, System.Drawing.Color pTransparencyColor)
		{
			if (pTransparencyNdx < 0)
			{
				LabelColorSample.Visible = false;
				LabelTransparencyClick.Visible = false;
			}
			else
			{
				LabelColorSample.BackColor = pTransparencyColor;
				LabelColorSample.ForeColor = (LabelColorSample.BackColor.GetBrightness () > 0.5) ? Color.Black : Color.White;
				LabelColorSample.Text = pTransparencyNdx.ToString ();
				LabelColorSample.Visible = true;
				LabelTransparencyClick.Visible = !Program.FileIsReadOnly;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private int PaletteMouseColorNdx (System.Drawing.Point pMousePos)
		{
			System.Drawing.Point lColorPos = new System.Drawing.Point ();

			lColorPos.X = pMousePos.X * 16 / PictureBoxPalette.Width;
			lColorPos.Y = pMousePos.Y * 16 / PictureBoxPalette.Height;

			return (Math.Min (Math.Max (lColorPos.Y, 0), 16) * 16) + Math.Min (Math.Max (lColorPos.X, 0), 16);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void TextBoxNewName_TextChanged (object sender, EventArgs e)
		{
			ShowAddState ();
		}

		private void ListViewAnimations_SelectedIndexChanged (object sender, EventArgs e)
		{
			ShowDeleteState ();
		}

		private void ListViewAnimations_ItemActivate (object sender, EventArgs e)
		{
			if (!IsPanelEmpty && (Navigate != null))
			{
				FileAnimation lAnimation = GetSelectedAnimation (false);

				if (lAnimation != null)
				{
					try
					{
						Navigate (this, new NavigationEventArgs (new ResolveAnimation (lAnimation)));
					}
					catch
					{
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonAdd_Click (object sender, EventArgs e)
		{
			if (!IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleAddAnimation ();
			}
		}

		private void ButtonDelete_Click (object sender, EventArgs e)
		{
			if (!IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleDeleteAnimation ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void NumericFrameWidth_Validated (object sender, EventArgs e)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleUpdateFrameWidth ();
			}
		}

		private void NumericFrameHeight_Validated (object sender, EventArgs e)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleUpdateFrameHeight ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxPaletteFile_Validated (object sender, EventArgs e)
		{
			if (!IsPanelFilling && TextBoxPaletteFile.IsModified && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleUpdatePaletteFile ();
			}
		}

		private void ButtonPaletteImport_Click (object sender, EventArgs e)
		{
			if (!IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleImportPalette ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void PictureBoxPalette_MouseMove (object sender, MouseEventArgs e)
		{
			ShowSelectedTransparency (PaletteMouseColorNdx (e.Location));
		}

		private void PictureBoxPalette_MouseLeave (object sender, EventArgs e)
		{
			ShowSelectedTransparency (-1, System.Drawing.Color.Empty);
		}

		private void PictureBoxPalette_MouseClick (object sender, MouseEventArgs e)
		{
			if (!IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleUpdatePaletteTransparency (PaletteMouseColorNdx (e.Location));
			}
		}

		#endregion
	}
}
