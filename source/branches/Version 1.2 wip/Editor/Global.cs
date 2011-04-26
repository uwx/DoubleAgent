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
using System.Windows.Forms;
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor
{
	public static class StringExtensions
	{
		public static String Quoted (this String pString)
		{
			if (!String.IsNullOrEmpty (pString) && (pString[0] == '"') && (pString[pString.Length - 1] == '"'))
			{
				return pString;
			}
			else
			{
				return String.Format ("\"{0}\"", pString);
			}
		}
		static public String NoMenuPrefix (this String pString)
		{
			return pString.Replace ("&", "");
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public static class LangIDExtensions
	{
		public static Boolean PrimaryLanguageEqual (this UInt16 pLangID, FileCharacterName pName)
		{
			if (pName != null)
			{
				return pLangID.PrimaryLanguageEqual (pName.Language);
			}
			return false;
		}
		public static Boolean PrimaryLanguageEqual (this UInt16 pLangID1, UInt16 pLangID2)
		{
			return ((Byte)pLangID1 == (Byte)pLangID2);
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public class Global
	{
		#region String formatting

		static public String TitleCharacterName (FileCharacterName pName)
		{
			if (pName == null)
			{
				return String.Format (Properties.Resources.TitleCharacterName, String.Empty, String.Empty).Trim ();
			}
			else
			{
				return TitleCharacterName (pName.Language, pName.Name);
			}
		}

		static public String TitleCharacterName (UInt16 pLanguage)
		{
			return TitleCharacterName (pLanguage, String.Empty);
		}

		static public String TitleCharacterName (UInt16 pLanguage, String pName)
		{
			if (((Byte)pLanguage != 4) && ((Byte)pLanguage != 22)) // Chinese and Portuguese
			{
				pLanguage = (UInt16)(Byte)pLanguage;
			}
			if (String.IsNullOrEmpty (pName))
			{
				return String.Format (Properties.Resources.TitleCharacterName, (new System.Globalization.CultureInfo (pLanguage)).EnglishName, String.Empty).Trim ();
			}
			else
			{
				return String.Format (Properties.Resources.TitleCharacterName, (new System.Globalization.CultureInfo (pLanguage)).EnglishName, pName.Quoted ());
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		static public String TitleAnimation (FileAnimation pAnimation)
		{
			if (pAnimation == null)
			{
				return String.Format (Properties.Resources.TitleAnimation, String.Empty).Trim ();
			}
			else
			{
				return TitleAnimation (pAnimation.Name);
			}
		}

		static public String TitleAnimation (String pAnimationName)
		{
			return String.Format (Properties.Resources.TitleAnimation, pAnimationName.Quoted ());
		}

		///////////////////////////////////////////////////////////////////////////////

		static public String TitleFrame (FileAnimationFrame pFrame)
		{
			if ((pFrame == null) || (pFrame.Container == null))
			{
				return String.Format (Properties.Resources.TitleFrame, String.Empty).Trim ();
			}
			else
			{
				return TitleFrame (pFrame.Container.IndexOf (pFrame));
			}
		}

		static public String TitleFrame (int pFrameNdx)
		{
			return String.Format (Properties.Resources.TitleFrame, (pFrameNdx + 1).ToString ());
		}

		static public String TitleFrameAnimation (FileAnimationFrame pFrame)
		{
			if ((pFrame == null) || (pFrame.Animation == null))
			{
				return String.Format (Properties.Resources.TitleFrame, String.Empty).Trim ();
			}
			else
			{
				return TitleFrameAnimation (TitleFrame (pFrame), pFrame.Animation.Name);
			}
		}

		static public String TitleFrameAnimation (int pFrameNdx, FileAnimation pAnimation)
		{
			if (pAnimation == null)
			{
				return String.Format (Properties.Resources.TitleFrame, String.Empty).Trim ();
			}
			else
			{
				return TitleFrameAnimation (TitleFrame (pFrameNdx), pAnimation.Name);
			}
		}

		static public String TitleFrameAnimation (int pFrameNdx, String pAnimationName)
		{
			return TitleFrameAnimation (TitleFrame (pFrameNdx), pAnimationName);
		}

		static public String TitleFrameAnimation (String pFrameTitle, String pAnimationName)
		{
			return String.Format (Properties.Resources.TitleFrameAnimation, pFrameTitle, pAnimationName);
		}

		///////////////////////////////////////////////////////////////////////////////

		static public String TitleImage (FileFrameImage pImage)
		{
			if ((pImage == null) || (pImage.Container == null))
			{
				return String.Format (Properties.Resources.TitleImage, String.Empty).Trim ();
			}
			else
			{
				return TitleImage (pImage.Container.IndexOf (pImage));
			}
		}

		static public String TitleImage (int pImageNdx)
		{
			return String.Format (Properties.Resources.TitleImage, (pImageNdx + 1).ToString ());
		}

		static public String TitleImageFrame (FileFrameImage pImage)
		{
			if ((pImage == null) || (pImage.Frame == null) || (pImage.Frame.Animation == null))
			{
				return String.Format (Properties.Resources.TitleImage, String.Empty).Trim ();
			}
			else
			{
				return TitleImageFrame (TitleImage (pImage), TitleFrame (pImage.Frame));
			}
		}

		static public String TitleImageFrame (int pImageNdx, FileAnimationFrame pFrame)
		{
			if ((pFrame == null) || (pFrame.Animation == null))
			{
				return String.Format (Properties.Resources.TitleImage, String.Empty).Trim ();
			}
			else
			{
				return TitleImageFrame (TitleImage (pImageNdx), TitleFrame (pFrame));
			}
		}

		static public String TitleImageFrame (int pImageNdx, int pFrameNdx)
		{
			return TitleImageFrame (TitleImage (pImageNdx), TitleFrame (pFrameNdx));
		}

		static public String TitleImageFrame (String pImageTitle, String pFrameName)
		{
			return String.Format (Properties.Resources.TitleImageFrame, pImageTitle, pFrameName);
		}

		static public String TitleImageFrameAnimation (FileFrameImage pImage)
		{
			if ((pImage == null) || (pImage.Frame == null) || (pImage.Frame.Animation == null))
			{
				return String.Format (Properties.Resources.TitleImage, String.Empty).Trim ();
			}
			else
			{
				return TitleImageFrameAnimation (TitleImageFrame (pImage), pImage.Frame.Animation.Name);
			}
		}

		static public String TitleImageFrameAnimation (int pImageNdx, FileAnimationFrame pFrame)
		{
			if (pFrame == null)
			{
				return String.Format (Properties.Resources.TitleImage, String.Empty).Trim ();
			}
			else
			{
				return TitleImageFrameAnimation (TitleImage (pImageNdx), TitleFrameAnimation (pFrame));
			}
		}

		static public String TitleImageFrameAnimation (int pImageNdx, int pFrameNdx, String pAnimationName)
		{
			return TitleImageFrameAnimation (TitleImage (pImageNdx), TitleFrame (pFrameNdx), pAnimationName);
		}

		static public String TitleImageFrameAnimation (String pImageTitle, String pFrameTitle, String pAnimationName)
		{
			return TitleImageFrameAnimation (pImageTitle, TitleFrameAnimation (pFrameTitle, pAnimationName));
		}

		static public String TitleImageFrameAnimation (String pImageTitle, String pFrameTitle)
		{
			return String.Format (Properties.Resources.TitleImageFrame, pImageTitle, pFrameTitle);
		}

		///////////////////////////////////////////////////////////////////////////////

		static public String OverlayTypeName (MouthOverlay pOverlayType)
		{
			switch (pOverlayType)
			{
				case MouthOverlay.MouthOverlayClosed:
					return Properties.Resources.MouthOverlayClosed;
				case MouthOverlay.MouthOverlayWide1:
					return Properties.Resources.MouthOverlayWide1;
				case MouthOverlay.MouthOverlayWide2:
					return Properties.Resources.MouthOverlayWide2;
				case MouthOverlay.MouthOverlayWide3:
					return Properties.Resources.MouthOverlayWide3;
				case MouthOverlay.MouthOverlayWide4:
					return Properties.Resources.MouthOverlayWide4;
				case MouthOverlay.MouthOverlayMedium:
					return Properties.Resources.MouthOverlayMedium;
				case MouthOverlay.MouthOverlayNarrow:
					return Properties.Resources.MouthOverlayNarrow;
				default:
					return Properties.Resources.MouthOverlay;
			}
		}

		static public String TitleOverlay (FileFrameOverlay pOverlay)
		{
			if (pOverlay == null)
			{
				return String.Format (Properties.Resources.TitleOverlay, String.Empty).Trim ();
			}
			else
			{
				return TitleOverlay (pOverlay.OverlayType);
			}
		}

		static public String TitleOverlay (MouthOverlay pOverlayType)
		{
			return String.Format (Properties.Resources.TitleOverlay, OverlayTypeName (pOverlayType));
		}

		static public String TitleOverlayFrameAnimation (FileFrameOverlay pOverlay)
		{
			if (pOverlay == null)
			{
				return String.Format (Properties.Resources.TitleOverlay, String.Empty).Trim ();
			}
			else
			{
				return TitleOverlayFrameAnimation (pOverlay.Frame, pOverlay.OverlayType);
			}
		}

		static public String TitleOverlayFrameAnimation (FileAnimationFrame pFrame, MouthOverlay pOverlayType)
		{
			if (pFrame == null)
			{
				return String.Format (Properties.Resources.TitleOverlay, String.Empty).Trim ();
			}
			else
			{
				return String.Format (Properties.Resources.TitleOverlayFrame, TitleOverlay (pOverlayType), TitleFrameAnimation (pFrame));
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		static public String TitleState (String pStateName)
		{
			return String.Format (Properties.Resources.TitleState, pStateName.Quoted ());
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public class EditEventArgs : EventArgs
		{
			public EditEventArgs ()
			{
			}

			public EditEventArgs (String pClipboardFormat)
			{
				if (Clipboard.ContainsData (pClipboardFormat))
				{
					PasteObject = Clipboard.GetData (pClipboardFormat);
				}
			}

			public String CopyTitle
			{
				get
				{
					return Properties.Resources.EditCopyThis;
				}
			}
			public String CopyObjectTitle
			{
				get;
				set;
			}

			public String CutTitle
			{
				get
				{
					return Properties.Resources.EditCutThis;
				}
			}
			public String CutObjectTitle
			{
				get;
				set;
			}

			public String DeleteTitle
			{
				get
				{
					return Properties.Resources.EditDeleteThis;
				}
			}
			public String DeleteObjectTitle
			{
				get;
				set;
			}

			public String PasteTitle
			{
				get
				{
					return mPasteTitle;
				}
				set
				{
					mPasteTitle = value;
				}
			}
			public String PasteObjectTitle
			{
				get;
				set;
			}
			public Object PasteObject
			{
				get;
				private set;
			}

			public Boolean IsUsed
			{
				get
				{
					return (mIsUsed) || (CopyObjectTitle != null) || (CutObjectTitle != null) || (DeleteObjectTitle != null) || (PasteObjectTitle != null);
				}
				set
				{
					mIsUsed = value;
				}
			}

			private Boolean mIsUsed = false;
			private String mPasteTitle = Properties.Resources.EditPasteThis;

			///////////////////////////////////////////////////////////////////////////////

			public String PasteAddTitle
			{
				get
				{
					return Properties.Resources.EditPasteAdd;
				}
			}
			public String PasteReplaceTitle
			{
				get
				{
					return Properties.Resources.EditPasteReplace;
				}
			}
			public String PasteTypeTitle (Object pTargetObject)
			{
				return (pTargetObject == null) ? PasteAddTitle : PasteReplaceTitle;
			}
			public String PasteTypeTitle (Object pTargetObject, String pSourceTitle)
			{
				return String.Format (PasteTypeTitle (pTargetObject), pSourceTitle);
			}
			public String PasteTypeTitle (Object pTargetObject, String pTargetTitle, String pSourceTitle)
			{
				if (pTargetObject == null)
				{
					return String.Format (Properties.Resources.EditPasteAs, pSourceTitle, pTargetTitle);
				}
				else
				{
					return String.Format (Properties.Resources.EditPasteOver, pSourceTitle, pTargetTitle);
				}
			}
		}

		public delegate void EditEventHandler (object sender, EditEventArgs e);

		///////////////////////////////////////////////////////////////////////////////

		public class ContextMenuEventArgs : EditEventArgs
		{
			public ContextMenuEventArgs (ContextMenuStrip pContextMenu, Control pActiveControl)
			{
				ContextMenu = pContextMenu;
				ActiveControl = pActiveControl;
			}
			public ContextMenuEventArgs (ContextMenuStrip pContextMenu, Control pActiveControl, String pClipboardFormat)
				: base (pClipboardFormat)
			{
				ContextMenu = pContextMenu;
				ActiveControl = pActiveControl;
			}

			public ContextMenuStrip ContextMenu
			{
				get;
				private set;
			}
			public Control ActiveControl
			{
				get;
				private set;
			}
		}

		public delegate void ContextMenuEventHandler (object sender, ContextMenuEventArgs e);

		///////////////////////////////////////////////////////////////////////////////

		public class AnimationEventArgs : EventArgs
		{
			public AnimationEventArgs (FileAnimation pAnimation)
			{
				Animation = pAnimation;
				AnimationName = pAnimation.Name;
			}
			public AnimationEventArgs (String pAnimationName)
			{
				Animation = null;
				AnimationName = pAnimationName;
			}

			public FileAnimation Animation
			{
				get;
				private set;
			}
			public String AnimationName
			{
				get;
				private set;
			}
		}

		public delegate void GoToAnimationEventHandler (object sender, AnimationEventArgs e);

		///////////////////////////////////////////////////////////////////////////////

		public class AnimationFrameEventArgs : EventArgs
		{
			public AnimationFrameEventArgs (FileAnimation pAnimation, FileAnimationFrame pFrame)
			{
				Animation = pAnimation;
				Frame = pFrame;
			}

			public DoubleAgent.Character.FileAnimation Animation
			{
				get;
				private set;
			}
			public DoubleAgent.Character.FileAnimationFrame Frame
			{
				get;
				private set;
			}
		}

		public delegate void GoToFrameEventHandler (object sender, AnimationFrameEventArgs e);

		///////////////////////////////////////////////////////////////////////////////

		public class StateEventArgs : EventArgs
		{
			public StateEventArgs (String pStateName)
			{
				StateName = pStateName;
			}

			public String StateName
			{
				get;
				private set;
			}
		}

		public delegate void GoToStateEventHandler (object sender, StateEventArgs e);

		#endregion
		///////////////////////////////////////////////////////////////////////////////
	}
}
