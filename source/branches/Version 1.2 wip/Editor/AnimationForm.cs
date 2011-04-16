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
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor
{
	public partial class AnimationForm : UserControl
	{
		private CharacterFile			mCharacterFile = null;
		private FileAnimation			mAnimation = null;
		private int						mReturnItemNone = -1;
		private int						mReturnItemExit = -1;
		private List <ListViewItem>		mStateItems;

		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public AnimationForm ()
		{
			InitializeComponent ();

			mStateItems = new List<ListViewItem> ();
			foreach (ListViewItem lListItem in ListViewStates.Items)
			{
				mStateItems.Add (lListItem);
			}
			ListViewStates.Items.Clear ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Browsable (false)]
		public CharacterFile CharacterFile
		{
			get
			{
				return mCharacterFile;
			}
			set
			{
				mCharacterFile = value;
				this.Animation = null;
			}
		}

		[System.ComponentModel.Browsable (false)]
		public FileAnimation Animation
		{
			get
			{
				return mAnimation;
			}
			set
			{
				mAnimation = value;

				ShowAnimationName ();
				ShowReturnAnimations ();
				ShowReturnAnimation ();
				ShowAnimationStates ();
				ShowAnimationPreview ();
			}
		}

		private Boolean IsEmpty
		{
			get
			{
				return ((mCharacterFile == null) || (mAnimation == null));
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public event AnimationNameChangedEventHandler AnimationNameChanged;
		public event AnimationFrameAddedEventHandler AnimationFrameAdded;
		public event AnimationFrameRemovedEventHandler AnimationFrameRemoved;
		public event AnimationFrameReorderedEventHandler AnimationFrameReordered;
		public event GoToFrameEventHandler GoToFrame;
		public event GoToStateEventHandler GoToState;

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowAnimationName ()
		{
			if (IsEmpty)
			{
				TextBoxName.ResetText ();
				TextBoxName.Enabled = false;
				ToolStripFrames.Enabled = false;
			}
			else
			{
				TextBoxName.Text = mAnimation.Name;
				TextBoxName.Enabled = !MainForm.Singleton.FileIsReadOnly;
				ToolStripFrames.Enabled = !MainForm.Singleton.FileIsReadOnly;
			}
			TextBoxName.Modified = false;
		}

		private void ShowReturnAnimations ()
		{
			ComboBoxReturn.BeginUpdate ();
			ComboBoxReturn.Items.Clear ();
			mReturnItemNone = ComboBoxReturn.Items.Add ("None");
			mReturnItemExit = -1;

			if (IsEmpty)
			{
				ComboBoxReturn.SelectedIndex = -1;
				ComboBoxReturn.Enabled = false;
			}
			else
			{
				String[]	lAnimations = mCharacterFile.GetAnimationNames ();

				mReturnItemExit = ComboBoxReturn.Items.Add ("Use Exit Branching");
				foreach (String lAnimation in lAnimations)
				{
					if (String.Compare (lAnimation, mAnimation.Name, true) != 0)
					{
						ComboBoxReturn.Items.Add (lAnimation);
					}
				}
				ComboBoxReturn.Enabled = !MainForm.Singleton.FileIsReadOnly;
			}

			ComboBoxReturn.EndUpdate ();
		}

		private void ShowReturnAnimation ()
		{
			if (IsEmpty)
			{
				ComboBoxReturn.SelectedIndex = mReturnItemNone;
			}
			else if (mAnimation.ReturnType == 1)
			{
				ComboBoxReturn.SelectedIndex = mReturnItemExit;
			}
			else if ((mAnimation.ReturnType != 0) || (String.IsNullOrEmpty (mAnimation.ReturnName)))
			{
				ComboBoxReturn.SelectedIndex = mReturnItemNone;
			}
			else
			{
				ComboBoxReturn.SelectedIndex = ComboBoxReturn.FindStringExact (mAnimation.ReturnName);
				if (ComboBoxReturn.SelectedIndex < 0)
				{
					ComboBoxReturn.SelectedIndex = mReturnItemNone;
				}
			}
		}

		private void ShowAnimationStates ()
		{
			FileStates	lFileStates = null;

			if (IsEmpty)
			{
				ListViewStates.Enabled = false;
			}
			else
			{
				ListViewStates.Enabled = true;//!MainForm.Singleton.FileIsReadOnly;
				lFileStates = mCharacterFile.States;
			}

			ListViewStates.Items.Clear ();

			foreach (ListViewItem lListItem in mStateItems)
			{
				String[]	lStateAnimations = null;

				if (lFileStates != null)
				{
					if (lFileStates.ContainsKey (lListItem.Text))
					{
						lStateAnimations = lFileStates[lListItem.Text];
					}
					else if (lFileStates.ContainsKey (lListItem.Text.ToUpper ()))
					{
						lStateAnimations = lFileStates[lListItem.Text.ToUpper ()];
					}
				}
				if (
						(lStateAnimations != null)
					&& (
							(Array.IndexOf (lStateAnimations, mAnimation.Name) >= 0)
						|| (Array.IndexOf (lStateAnimations, mAnimation.Name.ToUpper ()) >= 0)
						)
					)
				{
					ListViewStates.Items.Add (lListItem);
				}
			}
		}

		private void ShowAnimationPreview ()
		{
			NumericFrameDuration.Validated -= new System.EventHandler (NumericFrameDuration_Validated);
			if (mCharacterFile == null)
			{
				NumericFrameDuration.ResetText ();
				NumericFrameDuration.Enabled = false;
			}
			else
			{
				NumericFrameDuration.Value = mCharacterFile.NewFrameDuration;
				NumericFrameDuration.Enabled = !MainForm.Singleton.FileIsReadOnly;
			}
			NumericFrameDuration.Validated += new System.EventHandler (NumericFrameDuration_Validated);

			if (IsEmpty)
			{
				ListViewPreview.Clear ();
				ListViewPreview.Enabled = false;
			}
			else
			{
				CursorState	lCursorState = new CursorState (MainForm.Singleton);
				ImageList	lImageList = new ImageList ();
				int			lImageNdx = 0;
				int			lItemNdx = 0;
				int			lListHeight;

				lCursorState.ShowWait ();
				ListViewPreview.BeginUpdate ();
				ListViewPreview.Items.Clear ();

				lImageList.ColorDepth = ColorDepth.Depth32Bit;
				lImageList.ImageSize = new Size (Math.Min (mCharacterFile.Header.ImageSize.Width, 256), Math.Min (mCharacterFile.Header.ImageSize.Height, 256));
				lListHeight = lImageList.ImageSize.Height;

				foreach (FileAnimationFrame lFrame in mAnimation.Frames)
				{
					Bitmap			lImage = null;
					ListViewItem	lListItem;

					try
					{
						lImage = mCharacterFile.GetFrameBitmap (lFrame, true, Color.Transparent);
					}
					catch
					{
					}

					lListItem = new ListViewItem (String.Format ("{0} ({1:D})", Program.TitleFrame (lItemNdx), lFrame.Duration));
					lListItem.Tag = lFrame;
					if (lImage != null)
					{
						lImageList.Images.Add (lImage);
						lListItem.ImageIndex = lImageNdx++;
					}
					ListViewPreview.Items.Add (lListItem);
					//lListHeight = Math.Max (lListHeight, ListViewPreview.GetItemRect (lListItem.Index).Height);
					lItemNdx++;
				}
				ListViewPreview.LargeImageList = lImageList;
				lListHeight += (System.Windows.Forms.SystemInformation.HorizontalScrollBarHeight * 3);
				ListViewPreview.Height = Math.Max (lListHeight, ListViewPreview.MinimumSize.Height);

				ListViewPreview.Enabled = true;
				ListViewPreview.EndUpdate ();
				ShowSelectedFrame ();
				lCursorState.RestoreCursor ();
			}
		}

		private void ShowSelectedFrame ()
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				int	lSelNdx = ListViewPreview.GetSelectedIndex (false);

				ButtonDelete.Enabled = (lSelNdx >= 0);
				ButtonMoveUp.Enabled = (lSelNdx > 0);
				ButtonMoveDown.Enabled = (lSelNdx >= 0) && (lSelNdx < ListViewPreview.Items.Count - 1);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		internal class UpdateAnimation : UndoableUpdate<FileAnimation>
		{
			public UpdateAnimation (CharacterFile pCharacterFile, FileAnimation pAnimation)
				: base (pCharacterFile, pAnimation)
			{
				this.Name = Target.Name;
				this.ReturnType = Target.ReturnType;
				this.ReturnName = Target.ReturnName;
				this.IsRedo = false;
			}

			public String Name
			{
				get;
				set;
			}
			public Byte ReturnType
			{
				get;
				set;
			}
			public String ReturnName
			{
				get;
				set;
			}
			public Boolean IsRedo
			{
				get;
				private set;
			}

			public override String TargetDescription
			{
				get
				{
					return Program.QuotedTitle (Target.Name);
				}
			}
			public override String ActionDescription
			{
				get
				{
					if (Target.Name != this.Name)
					{
						if (this.IsRedo)
						{
							return String.Format (Properties.Resources.UndoAnimationName, String.Format (Properties.Resources.UndoAnimationNames, Program.QuotedTitle (this.Name), String.Empty));
						}
						else
						{
							return String.Format (Properties.Resources.UndoAnimationName, String.Empty);
						}
					}
					return String.Empty;
				}
			}
			public override String ChangeDescription
			{
				get
				{
					if (Target.Name != this.Name)
					{
						if (!this.IsRedo)
						{
							return String.Format (Properties.Resources.UndoAnimationNames, String.Empty, Program.QuotedTitle (this.Name));
						}
					}
					else
					{
						return " " + Properties.Resources.UndoReturnAnimation;
					}
					return String.Empty;
				}
			}

			public override UndoUnit Apply ()
			{
				UpdateAnimation	lApplied = null;

				if (!String.Equals (Target.Name, this.Name))
				{
					lApplied = new UpdateAnimation (this.CharacterFile, this.Target);
					Target.Name = this.Name;
					this.Name = lApplied.Name;
					lApplied.IsRedo = !this.IsRedo;
				}
				else
				{
					if (Target.ReturnType != this.ReturnType)
					{
						Byte	lSwap = Target.ReturnType;
						Target.ReturnType = this.ReturnType;
						this.ReturnType = lSwap;
						lApplied = this;
					}
					if (!String.Equals (Target.ReturnName, this.ReturnName))
					{
						String	lSwap = Target.ReturnName;
						Target.ReturnName = this.ReturnName;
						this.ReturnName = lSwap;
						lApplied = this;
					}
				}
				if (lApplied != null)
				{
					return OnApplied (lApplied);
				}
				return null;
			}
		}

		internal class UpdateNewFrameDuration : UndoableUpdate<CharacterFile>
		{
			public UpdateNewFrameDuration (CharacterFile pCharacterFile, UInt16 pNewFrameDuration)
				: base (pCharacterFile, pCharacterFile)
			{
				this.NewFrameDuration = pNewFrameDuration;
			}

			public UInt16 NewFrameDuration
			{
				get;
				private set;
			}
			public override String TargetDescription
			{
				get
				{
					return Properties.Resources.UndoNewFrameDuration;
				}
			}

			public override UndoUnit Apply ()
			{
				if (CharacterFile.NewFrameDuration != this.NewFrameDuration)
				{
					UInt16	lSwap = Target.NewFrameDuration;
					Target.NewFrameDuration = this.NewFrameDuration;
					this.NewFrameDuration = lSwap;

					return OnApplied (this);
				}
				return null;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal class AddDeleteFrame : UndoableAddDelete<FileAnimationFrame>
		{
			public AddDeleteFrame (CharacterFile pCharacterFile, FileAnimation pAnimation, int pFrameNdx)
				: base (pCharacterFile)
			{
				this.Animation = pAnimation;
				this.FrameNdx = pFrameNdx;
			}

			public AddDeleteFrame (CharacterFile pCharacterFile, FileAnimationFrame pFrame, Boolean pForClipboard)
				: base (pCharacterFile, pFrame, pForClipboard)
			{
				this.Animation = pFrame.Animation;
				this.FrameNdx = pFrame.Container.IndexOf (this.Target);
			}

			public FileAnimation Animation
			{
				get;
				private set;
			}
			public int FrameNdx
			{
				get;
				private set;
			}
			public override String TargetDescription
			{
				get
				{
					return Program.QuotedTitle (Program.TitleFrameAnimation (this.FrameNdx, this.Animation));
				}
			}

			public override UndoUnit Apply ()
			{
				AddDeleteFrame	lApplied = null;

				if (IsDelete)
				{
					if (this.Animation.Frames.Contains (this.Target))
					{
						this.FrameNdx = this.Animation.Frames.IndexOf (this.Target);
						lApplied = new AddDeleteFrame (this.CharacterFile, this.Target, this.ForClipboard);
						if (this.Animation.Frames.Remove (this.Target))
						{
							lApplied.IsDelete = false;
							lApplied.IsRedo = !IsRedo;
						}
						else
						{
							lApplied = null;
						}
					}
				}
				else
				{
					FileAnimationFrame	lFrame = this.Animation.Frames.Insert (this.FrameNdx);

					if (lFrame != null)
					{
						if (this.Target != null)
						{
							this.Target.CopyTo (lFrame);
						}
						this.Target = lFrame;
						this.FrameNdx = this.Animation.Frames.IndexOf (this.Target);
						lApplied = new AddDeleteFrame (this.CharacterFile, this.Target, this.ForClipboard);
						lApplied.IsDelete = true;
						lApplied.IsRedo = !IsRedo;
					}
				}
				if (lApplied != null)
				{
					return OnApplied (lApplied);
				}
				return null;
			}
		}

		internal class ReorderFrame : UndoableUpdate<FileAnimationFrame>
		{
			public ReorderFrame (CharacterFile pCharacterFile, FileAnimation pAnimation, FileAnimationFrame pFrame, int pFrameNdxTo)
				: base (pCharacterFile, pFrame)
			{
				this.Animation = pAnimation;
				this.FrameNdxFrom = pAnimation.Frames.IndexOf (this.Target);
				this.FrameNdxTo = pFrameNdxTo;
			}

			public FileAnimation Animation
			{
				get;
				private set;
			}
			public int FrameNdxFrom
			{
				get;
				private set;
			}
			public int FrameNdxTo
			{
				get;
				private set;
			}
			public override String TargetDescription
			{
				get
				{
					return Program.TitleAnimation (this.Animation);
				}
			}
			public override String ChangeDescription
			{
				get
				{
					return " " + Properties.Resources.UndoFrameOrder;
				}
			}

			public override UndoUnit Apply ()
			{
				int	lSwap = Animation.Frames.IndexOf (Target);
				if (Animation.Frames.Move (Target, this.FrameNdxTo))
				{
					this.FrameNdxFrom = Animation.Frames.IndexOf (Target);
					this.FrameNdxTo = lSwap;

					return OnApplied (this);
				}
				return null;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal Boolean DeleteSelectedFrame (FileAnimationFrame pFrame, Boolean pForClipboard)
		{
			Boolean	lRet = false;

			if ((pFrame != null) && !MainForm.Singleton.FileIsReadOnly)
			{
				AddDeleteFrame	lUpdate = new AddDeleteFrame (mCharacterFile, pFrame, pForClipboard);

				lUpdate.Applied += new UndoUnit.AppliedEventHandler (AddDeleteFrame_Applied);
				lRet = AddDeleteFrame.PutUndo (lUpdate.Apply () as AddDeleteFrame);
			}
			return lRet;
		}

		internal Boolean PasteSelectedFrame (FileAnimation pAnimation, FileAnimationFrame pPasteFrame)
		{
			Boolean	lRet = false;

			if ((pAnimation != null) && (pPasteFrame != null) && !MainForm.Singleton.FileIsReadOnly)
			{
			}
			return lRet;
		}

		internal Boolean PasteSelectedAnimation (FileAnimation pAnimation, FileAnimationFrame pPasteAnimation)
		{
			Boolean	lRet = false;

			return lRet;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void TextBoxName_Validated (object sender, EventArgs e)
		{
			if (TextBoxName.Modified && !IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				if (!String.IsNullOrEmpty (TextBoxName.Text))
				{
					UpdateAnimation	lUpdate = new UpdateAnimation (mCharacterFile, mAnimation);

					lUpdate.Name = TextBoxName.Text;
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoableAnimationUpdate_Applied);
					UpdateAnimation.PutUndo (lUpdate.Apply () as UpdateAnimation);
				}
				ShowAnimationName ();
			}
			TextBoxName.Modified = false;
		}

		private void ComboBoxReturn_SelectionChangeCommitted (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				UpdateAnimation	lUpdate = new UpdateAnimation (mCharacterFile, mAnimation);

				if (ComboBoxReturn.SelectedIndex == mReturnItemNone)
				{
					lUpdate.ReturnType = 2;
					lUpdate.ReturnName = String.Empty;
				}
				else if (ComboBoxReturn.SelectedIndex == mReturnItemExit)
				{
					lUpdate.ReturnType = 1;
					lUpdate.ReturnName = String.Empty;
				}
				else if (ComboBoxReturn.SelectedIndex >= 0)
				{
					lUpdate.ReturnType = 0;
					lUpdate.ReturnName = ComboBoxReturn.SelectedItem.ToString ();
				}

				lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoableAnimationUpdate_Applied);
				UpdateAnimation.PutUndo (lUpdate.Apply () as UpdateAnimation);
			}
		}

		private void UndoableAnimationUpdate_Applied (object sender, EventArgs e)
		{
			if (!IsEmpty)
			{
				try
				{
					UpdateAnimation	lSender = (UpdateAnimation)sender;

					if ((lSender != null) && (lSender.Target == mAnimation))
					{
						if (!String.Equals (lSender.Name, mAnimation.Name))
						{
							ShowAnimationName ();
							if (AnimationNameChanged != null)
							{
								AnimationNameChanged (this, new AnimationEventArgs (mAnimation));
							}
						}
						else
						{
							ShowReturnAnimation ();
						}
					}
				}
				catch
				{
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ListViewPreview_SelectedIndexChanged (object sender, EventArgs e)
		{
			if (!IsEmpty)
			{
				ShowSelectedFrame ();
			}
		}

		private void ListViewPreview_ItemActivate (object sender, EventArgs e)
		{
			if (!IsEmpty && (GoToFrame != null))
			{
				ListViewItem		lItem = ListViewPreview.SelectedItem;
				FileAnimationFrame	lFrame = null;

				if (lItem != null)
				{
					lFrame = lItem.Tag as FileAnimationFrame;
				}
				if (lFrame != null)
				{
					try
					{
						GoToFrame (this, new AnimationFrameEventArgs (mAnimation, lFrame));
					}
					catch
					{
					}
				}
			}
		}

		private void ListViewStates_ItemActivate (object sender, EventArgs e)
		{
			if (!IsEmpty && (GoToState != null))
			{
				ListViewItem	lItem = ListViewStates.SelectedItem;

				if (lItem != null)
				{
					try
					{
						GoToState (this, new StateEventArgs (lItem.Text));
					}
					catch
					{
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void NumericFrameDuration_Validated (object sender, EventArgs e)
		{
			if ((mCharacterFile != null) && !MainForm.Singleton.FileIsReadOnly)
			{
				UpdateNewFrameDuration	lUpdate = new UpdateNewFrameDuration (mCharacterFile, (UInt16)NumericFrameDuration.Value);

				lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoableDurationUpdate_Applied);
				UpdateNewFrameDuration.PutUndo (lUpdate.Apply () as UpdateNewFrameDuration);
			}
		}

		private void UndoableDurationUpdate_Applied (object sender, EventArgs e)
		{
			NumericFrameDuration.Value = mCharacterFile.NewFrameDuration;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonAdd_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				int				lSelNdx = ListViewPreview.GetSelectedIndex (false);
				AddDeleteFrame	lUpdate = new AddDeleteFrame (mCharacterFile, mAnimation, (lSelNdx >= 0) ? lSelNdx + 1 : mAnimation.FrameCount);

				lUpdate.Applied += new UndoUnit.AppliedEventHandler (AddDeleteFrame_Applied);
				AddDeleteFrame.PutUndo (lUpdate.Apply () as AddDeleteFrame);
			}
		}

		private void ButtonDelete_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				int					lSelNdx = ListViewPreview.GetSelectedIndex (false);
				FileAnimationFrame	lFrame = null;

				if (lSelNdx >= 0)
				{
					lFrame = mAnimation.Frames[lSelNdx];
					DeleteSelectedFrame (lFrame, false);
				}
			}
		}

		private void AddDeleteFrame_Applied (object sender, EventArgs e)
		{
			AddDeleteFrame	lUpdate = sender as AddDeleteFrame;

			if (lUpdate != null)
			{
				if (!IsEmpty && (lUpdate.Animation == mAnimation))
				{
					ShowAnimationPreview ();
					ListViewPreview.SelectedIndex = lUpdate.FrameNdx;
				}
				if (lUpdate.IsDelete)
				{
					if (AnimationFrameRemoved != null)
					{
						AnimationFrameRemoved (this, new AnimationFrameEventArgs (lUpdate.Animation, lUpdate.Target));
					}
				}
				else
				{
					if (AnimationFrameAdded != null)
					{
						AnimationFrameAdded (this, new AnimationFrameEventArgs (lUpdate.Animation, lUpdate.Target));
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonMoveUp_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				int					lSelNdx = ListViewPreview.GetSelectedIndex (false);
				FileAnimationFrame	lFrame = null;
				ReorderFrame		lUpdate;

				if (lSelNdx > 0)
				{
					lFrame = mAnimation.Frames[lSelNdx];
				}
				if (lFrame != null)
				{
					lUpdate = new ReorderFrame (mCharacterFile, mAnimation, lFrame, lSelNdx - 1);
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (ReorderFrame_Applied);
					ReorderFrame.PutUndo (lUpdate.Apply () as ReorderFrame);
				}
			}
		}

		private void ButtonMoveDown_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				int					lSelNdx = ListViewPreview.GetSelectedIndex (false);
				FileAnimationFrame	lFrame = null;
				ReorderFrame		lUpdate;

				if ((lSelNdx >= 0) && (lSelNdx < mAnimation.Frames.Count - 1))
				{
					lFrame = mAnimation.Frames[lSelNdx];
				}
				if (lFrame != null)
				{
					lUpdate = new ReorderFrame (mCharacterFile, mAnimation, lFrame, lSelNdx + 1);
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (ReorderFrame_Applied);
					ReorderFrame.PutUndo (lUpdate.Apply () as ReorderFrame);
				}
			}
		}

		private void ReorderFrame_Applied (object sender, EventArgs e)
		{
			ReorderFrame	lUpdate = sender as ReorderFrame;

			if (lUpdate != null)
			{
				if (!IsEmpty && (lUpdate.Animation == mAnimation))
				{
					ShowAnimationPreview ();
					ListViewPreview.SelectedIndex = mAnimation.Frames.IndexOf (lUpdate.Target);
				}
				if (AnimationFrameReordered != null)
				{
					AnimationFrameReordered (this, new AnimationFrameEventArgs (lUpdate.Animation, lUpdate.Target));
				}
			}
		}

		#endregion
	}
}
