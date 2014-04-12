/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
using DoubleAgent.Character;
using AgentCharacterEditor;
using AgentCharacterEditor.Global;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor.Properties
{
	public static class Titles
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Character

		static public String CharacterName (FileCharacterName pName)
		{
			if (pName == null)
			{
				return String.Format (AppResources.Resources.TitleCharacterName, String.Empty, String.Empty).Trim ();
			}
			else
			{
				return CharacterName (pName.Language, pName.Name);
			}
		}

		static public String CharacterName (UInt16 pLanguage)
		{
			return CharacterName (pLanguage, String.Empty);
		}

		static public String CharacterName (UInt16 pLanguage, String pName)
		{
			if (((Byte)pLanguage != 4) && ((Byte)pLanguage != 22)) // Chinese and Portuguese
			{
				pLanguage = (UInt16)(Byte)pLanguage;
			}
			if (String.IsNullOrEmpty (pName))
			{
				return String.Format (AppResources.Resources.TitleCharacterName, (new System.Globalization.CultureInfo (pLanguage)).EnglishName, String.Empty).Trim ();
			}
			else
			{
				return String.Format (AppResources.Resources.TitleCharacterName, (new System.Globalization.CultureInfo (pLanguage)).EnglishName, pName.Quoted ());
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Animation

		static public String Animation (FileAnimation pAnimation)
		{
			if (pAnimation == null)
			{
				return String.Format (AppResources.Resources.TitleAnimation, String.Empty).Trim ();
			}
			else
			{
				return Animation (pAnimation.Name);
			}
		}

		static public String Animation (String pAnimationName)
		{
			return String.Format (AppResources.Resources.TitleAnimation, pAnimationName.Quoted ());
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Frame

		static public String Frame (FileAnimationFrame pFrame)
		{
			if ((pFrame == null) || (pFrame.Container == null))
			{
				return String.Format (AppResources.Resources.TitleFrame, String.Empty).Trim ();
			}
			else
			{
				return Frame (pFrame.Container.IndexOf (pFrame));
			}
		}

		static public String Frame (int pFrameNdx)
		{
			return String.Format (AppResources.Resources.TitleFrame, (pFrameNdx + 1).ToString ());
		}

		static public String FrameAnimation (FileAnimationFrame pFrame)
		{
			if ((pFrame == null) || (pFrame.Animation == null))
			{
				return String.Format (AppResources.Resources.TitleFrame, String.Empty).Trim ();
			}
			else
			{
				return FrameAnimation (Frame (pFrame), pFrame.Animation.Name);
			}
		}

		static public String FrameAnimation (int pFrameNdx, FileAnimation pAnimation)
		{
			if (pAnimation == null)
			{
				return String.Format (AppResources.Resources.TitleFrame, String.Empty).Trim ();
			}
			else
			{
				return FrameAnimation (Frame (pFrameNdx), pAnimation.Name);
			}
		}

		static public String FrameAnimation (int pFrameNdx, String pAnimationName)
		{
			return FrameAnimation (Frame (pFrameNdx), pAnimationName);
		}

		static public String FrameAnimation (String pFrameTitle, String pAnimationName)
		{
			return String.Format (AppResources.Resources.TitleFrameAnimation, pFrameTitle, pAnimationName);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Image

		static public String Image (FileFrameImage pImage)
		{
			if ((pImage == null) || (pImage.Container == null))
			{
				return String.Format (AppResources.Resources.TitleImage, String.Empty).Trim ();
			}
			else
			{
				return Image (pImage.Container.IndexOf (pImage));
			}
		}

		static public String Image (int pImageNdx)
		{
			return String.Format (AppResources.Resources.TitleImage, (pImageNdx + 1).ToString ());
		}

		static public String ImageFrame (FileFrameImage pImage)
		{
			if ((pImage == null) || (pImage.Frame == null) || (pImage.Frame.Animation == null))
			{
				return String.Format (AppResources.Resources.TitleImage, String.Empty).Trim ();
			}
			else
			{
				return ImageFrame (Image (pImage), Frame (pImage.Frame));
			}
		}

		static public String ImageFrame (int pImageNdx, FileAnimationFrame pFrame)
		{
			if ((pFrame == null) || (pFrame.Animation == null))
			{
				return String.Format (AppResources.Resources.TitleImage, String.Empty).Trim ();
			}
			else
			{
				return ImageFrame (Image (pImageNdx), Frame (pFrame));
			}
		}

		static public String ImageFrame (int pImageNdx, int pFrameNdx)
		{
			return ImageFrame (Image (pImageNdx), Frame (pFrameNdx));
		}

		static public String ImageFrame (String pImageTitle, String pFrameName)
		{
			return String.Format (AppResources.Resources.TitleImageFrame, pImageTitle, pFrameName);
		}

		static public String ImageFrameAnimation (FileFrameImage pImage)
		{
			if ((pImage == null) || (pImage.Frame == null) || (pImage.Frame.Animation == null))
			{
				return String.Format (AppResources.Resources.TitleImage, String.Empty).Trim ();
			}
			else
			{
				return ImageFrameAnimation (ImageFrame (pImage), pImage.Frame.Animation.Name);
			}
		}

		static public String ImageFrameAnimation (int pImageNdx, FileAnimationFrame pFrame)
		{
			if (pFrame == null)
			{
				return String.Format (AppResources.Resources.TitleImage, String.Empty).Trim ();
			}
			else
			{
				return ImageFrameAnimation (Image (pImageNdx), FrameAnimation (pFrame));
			}
		}

		static public String ImageFrameAnimation (int pImageNdx, int pFrameNdx, String pAnimationName)
		{
			return ImageFrameAnimation (Image (pImageNdx), Frame (pFrameNdx), pAnimationName);
		}

		static public String ImageFrameAnimation (String pImageTitle, String pFrameTitle, String pAnimationName)
		{
			return ImageFrameAnimation (pImageTitle, FrameAnimation (pFrameTitle, pAnimationName));
		}

		static public String ImageFrameAnimation (String pImageTitle, String pFrameTitle)
		{
			return String.Format (AppResources.Resources.TitleImageFrame, pImageTitle, pFrameTitle);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Overlay

		static public String OverlayTypeName (MouthOverlay pOverlayType)
		{
			switch (pOverlayType)
			{
				case MouthOverlay.Closed:
					return AppResources.Resources.MouthOverlayClosed;
				case MouthOverlay.Wide1:
					return AppResources.Resources.MouthOverlayWide1;
				case MouthOverlay.Wide2:
					return AppResources.Resources.MouthOverlayWide2;
				case MouthOverlay.Wide3:
					return AppResources.Resources.MouthOverlayWide3;
				case MouthOverlay.Wide4:
					return AppResources.Resources.MouthOverlayWide4;
				case MouthOverlay.Medium:
					return AppResources.Resources.MouthOverlayMedium;
				case MouthOverlay.Narrow:
					return AppResources.Resources.MouthOverlayNarrow;
				default:
					return AppResources.Resources.MouthOverlay;
			}
		}

		static public String OverlayTypeName (FileFrameOverlay pOverlay)
		{
			return (pOverlay == null) ? String.Empty : OverlayTypeName (pOverlay.OverlayType);
		}

		static public String Overlay (FileFrameOverlay pOverlay)
		{
			if (pOverlay == null)
			{
				return AppResources.Resources.TitleOverlay;
			}
			else
			{
				return Overlay (pOverlay.OverlayType);
			}
		}

		static public String Overlay (MouthOverlay pOverlayType)
		{
			return String.Format (AppResources.Resources.TitleOverlayType, OverlayTypeName (pOverlayType));
		}

		static public String OverlayFrameAnimation (FileFrameOverlay pOverlay)
		{
			if (pOverlay == null)
			{
				return String.Format (AppResources.Resources.TitleOverlayType, String.Empty).Trim ();
			}
			else
			{
				return OverlayFrameAnimation (pOverlay.Frame, pOverlay.OverlayType);
			}
		}

		static public String OverlayFrameAnimation (FileAnimationFrame pFrame, MouthOverlay pOverlayType)
		{
			if (pFrame == null)
			{
				return String.Format (AppResources.Resources.TitleOverlayType, String.Empty).Trim ();
			}
			else
			{
				return String.Format (AppResources.Resources.TitleOverlayFrame, Overlay (pOverlayType), FrameAnimation (pFrame));
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region State

		static public String State (FileState pState)
		{
			if (pState == null)
			{
				return String.Format (AppResources.Resources.TitleState, String.Empty).Trim ();
			}
			else
			{
				return State (pState.StateName);
			}
		}

		static public String State (String pStateName)
		{
			return String.Format (AppResources.Resources.TitleState, pStateName.Quoted ());
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Edit Menu

		static public String PasteAddTitle
		{
			get
			{
				return AppResources.Resources.EditPasteAdd;
			}
		}
		static public String PasteReplaceTitle
		{
			get
			{
				return AppResources.Resources.EditPasteReplace;
			}
		}

		static public String PasteTypeTitle (Object pTargetObject)
		{
			return (pTargetObject == null) ? PasteAddTitle : PasteReplaceTitle;
		}
		static public String PasteTypeTitle (Object pTargetObject, String pSourceTitle)
		{
			return String.Format (PasteTypeTitle (pTargetObject), pSourceTitle);
		}
		static public String PasteTypeTitle (Object pTargetObject, String pTargetTitle, String pSourceTitle)
		{
			if (pTargetObject == null)
			{
				return String.Format (AppResources.Resources.EditPasteAs, pSourceTitle, pTargetTitle);
			}
			else
			{
				return String.Format (AppResources.Resources.EditPasteOver, pSourceTitle, pTargetTitle);
			}
		}

		#endregion
	}
}
