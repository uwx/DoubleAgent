using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DoubleAgent.Character;

namespace AgentCharacterEditor
{
	public partial class AnimationForm : UserControl
	{
		private CharacterFile			mCharacterFile = null;
		private FileAnimation			mAnimation = null;
		private String					mAnimationName = null;
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

		public CharacterFile CharacterFile
		{
			get
			{
				return mCharacterFile;
			}
			set
			{
				mCharacterFile = value;
				mAnimation = null;
				mAnimationName = null;
			}
		}

		public FileAnimation FileAnimation
		{
			get
			{
				return mAnimation;
			}
			set
			{
				mAnimation = value;
				mAnimationName = null;
			}
		}

		public String AnimationName
		{
			get
			{
				return mAnimationName;
			}
			set
			{
				mAnimationName = value;
				ShowAnimationName ();
				ShowReturnAnimations ();
				ShowReturnAnimation ();
				ShowAnimationStates ();
				ShowAnimationPreview ();
			}
		}

		private bool IsEmpty
		{
			get
			{
				return ((mCharacterFile == null) || (mAnimation == null) || String.IsNullOrEmpty (mAnimationName));
			}
		}

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
				TextBoxName.Text = mAnimationName;
				TextBoxName.Enabled = !Program.MainForm.FileIsReadOnly;
				ToolStripFrames.Enabled = !Program.MainForm.FileIsReadOnly;
			}
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
					if (!lAnimation.Equals (mAnimationName, StringComparison.InvariantCultureIgnoreCase))
					{
						ComboBoxReturn.Items.Add (lAnimation);
					}
				}
				ComboBoxReturn.Enabled = !Program.MainForm.FileIsReadOnly;
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
				ListViewStates.Enabled = true;//!Program.MainForm.FileIsReadOnly;
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
							(Array.IndexOf (lStateAnimations, mAnimationName) >= 0)
						|| (Array.IndexOf (lStateAnimations, mAnimationName.ToUpper ()) >= 0)
						)
					)
				{
					ListViewStates.Items.Add (lListItem);
				}
			}
		}

		private void ShowAnimationPreview ()
		{
			if (mCharacterFile == null)
			{
				NumericFrameDuration.ResetText ();
				NumericFrameDuration.Enabled = false;
			}
			else
			{
				NumericFrameDuration.Value = mCharacterFile.NewFrameDuration;
				NumericFrameDuration.Enabled = !Program.MainForm.FileIsReadOnly;
			}

			if (IsEmpty)
			{
				ListViewPreview.Clear ();
				ListViewPreview.Enabled = false;
			}
			else
			{
				Cursor		lSaveCursor;
				ImageList	lImageList = new ImageList ();
				int			lImageNdx = 0;
				int			lItemNdx = 0;
				int			lListHeight;

				lSaveCursor = Program.MainForm.Cursor;
				Program.MainForm.Cursor = System.Windows.Forms.Cursors.WaitCursor;
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

					lListItem = new ListViewItem ("Frame " + (lItemNdx + 1).ToString () + " (" + lFrame.Duration.ToString () + ")");
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
				Program.MainForm.Cursor = lSaveCursor;
			}
		}

		private void ShowSelectedFrame ()
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				int	lSelNdx = ListViewPreview.GetSelectedIndex (false);

				ButtonRemove.Enabled = (lSelNdx >= 0);
				ButtonMoveUp.Enabled = (lSelNdx > 0);
				ButtonMoveDown.Enabled = (lSelNdx >= 0) && (lSelNdx < ListViewPreview.Items.Count - 1);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Delegates

		public class AnimationEventArgs : EventArgs
		{
			internal AnimationEventArgs (FileAnimation pAnimation)
			{
				mAnimation = pAnimation;
			}

			public DoubleAgent.Character.FileAnimation Animation
			{
				get
				{
					return mAnimation;
				}
			}

			private FileAnimation	mAnimation;
		}

		public class AnimationFrameEventArgs : EventArgs
		{
			internal AnimationFrameEventArgs (FileAnimation pAnimation, FileAnimationFrame pFrame)
			{
				mAnimation = pAnimation;
				mFrame = pFrame;
			}

			public DoubleAgent.Character.FileAnimation Animation
			{
				get
				{
					return mAnimation;
				}
			}

			public DoubleAgent.Character.FileAnimationFrame Frame
			{
				get
				{
					return mFrame;
				}
			}

			private FileAnimation		mAnimation;
			private FileAnimationFrame	mFrame;
		}

		public delegate void AnimationNameChangedEvent (object sender, AnimationEventArgs e);
		public delegate void AnimationFrameAddedEvent (object sender, AnimationFrameEventArgs e);
		public delegate void AnimationFrameRemovedEvent (object sender, AnimationFrameEventArgs e);
		public delegate void AnimationFrameReorderedEvent (object sender, AnimationFrameEventArgs e);

		public event AnimationNameChangedEvent AnimationNameChanged;
		public event AnimationFrameAddedEvent AnimationFrameAdded;
		public event AnimationFrameRemovedEvent AnimationFrameRemoved;
		public event AnimationFrameReorderedEvent AnimationFrameReordered;

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void TextBoxName_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				if (!String.IsNullOrEmpty (TextBoxName.Text))
				{
					mAnimation.Name = TextBoxName.Text;
					mAnimationName = mAnimation.Name;
					if (AnimationNameChanged != null)
					{
						AnimationNameChanged (this, new AnimationEventArgs (mAnimation));
					}
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
				ShowAnimationName ();
			}
		}

		private void ComboBoxReturn_SelectionChangeCommitted (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				if (ComboBoxReturn.SelectedIndex == mReturnItemNone)
				{
					mAnimation.ReturnType = 2;
					mAnimation.ReturnName = String.Empty;
				}
				else if (ComboBoxReturn.SelectedIndex == mReturnItemExit)
				{
					mAnimation.ReturnType = 1;
					mAnimation.ReturnName = String.Empty;
				}
				else if (ComboBoxReturn.SelectedIndex >= 0)
				{
					mAnimation.ReturnType = 0;
					mAnimation.ReturnName = ComboBoxReturn.SelectedItem.ToString ();
				}

				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				ShowReturnAnimation ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ListViewPreview_SelectedIndexChanged (object sender, EventArgs e)
		{
			ShowSelectedFrame ();
		}

		private void NumericFrameDuration_Validated (object sender, EventArgs e)
		{
			if ((mCharacterFile != null) && !Program.MainForm.FileIsReadOnly)
			{
				mCharacterFile.NewFrameDuration = (UInt16)NumericFrameDuration.Value;
				NumericFrameDuration.Value = mCharacterFile.NewFrameDuration;
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonAdd_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				int					lSelNdx = ListViewPreview.GetSelectedIndex (false);
				FileAnimationFrame	lFrame = mAnimation.Frames.Insert ((lSelNdx >= 0) ? lSelNdx + 1 : -1);

				if (lFrame != null)
				{
					ShowAnimationPreview ();
					ListViewPreview.SelectedIndex =  mAnimation.Frames.IndexOf (lFrame);
					if (AnimationFrameAdded != null)
					{
						AnimationFrameAdded (this, new AnimationFrameEventArgs (mAnimation, lFrame));
					}
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		private void ButtonRemove_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				int					lSelNdx = ListViewPreview.GetSelectedIndex (false);
				FileAnimationFrame	lFrame = null;

				if (lSelNdx >= 0)
				{
					lFrame = mAnimation.Frames[lSelNdx];
				}
				if ((lFrame != null) && mAnimation.Frames.Remove (lFrame))
				{
					ShowAnimationPreview ();
					ListViewPreview.SelectedIndex = lSelNdx;
					if (AnimationFrameRemoved != null)
					{
						AnimationFrameRemoved (this, new AnimationFrameEventArgs (mAnimation, lFrame));
					}
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		private void ButtonMoveUp_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				int					lSelNdx = ListViewPreview.GetSelectedIndex (false);
				FileAnimationFrame	lFrame = null;

				if (lSelNdx > 0)
				{
					lFrame = mAnimation.Frames[lSelNdx];
				}
				if ((lFrame != null) && mAnimation.Frames.Move (lFrame, lSelNdx - 1))
				{
					ShowAnimationPreview ();
					ListViewPreview.SelectedIndex = mAnimation.Frames.IndexOf (lFrame);
					if (AnimationFrameReordered != null)
					{
						AnimationFrameReordered (this, new AnimationFrameEventArgs (mAnimation, lFrame));
					}
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		private void ButtonMoveDown_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				int					lSelNdx = ListViewPreview.GetSelectedIndex (false);
				FileAnimationFrame	lFrame = null;

				if ((lSelNdx >= 0) && (lSelNdx < mAnimation.Frames.Count - 1))
				{
					lFrame = mAnimation.Frames[lSelNdx];
				}
				if ((lFrame != null) && mAnimation.Frames.Move (lFrame, lSelNdx + 1))
				{
					ShowAnimationPreview ();
					ListViewPreview.SelectedIndex = mAnimation.Frames.IndexOf (lFrame);
					if (AnimationFrameReordered != null)
					{
						AnimationFrameReordered (this, new AnimationFrameEventArgs (mAnimation, lFrame));
					}
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
	}
}
