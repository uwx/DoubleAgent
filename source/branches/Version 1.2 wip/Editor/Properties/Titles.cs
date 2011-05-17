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
using DoubleAgent.Character;
using AgentCharacterEditor.Global;

namespace AgentCharacterEditor.Properties
{
	public static class Titles
{
		static public String CharacterName (FileCharacterName pName)
		{
			if (pName == null)
			{
				return String.Format (Resources.TitleCharacterName, String.Empty, String.Empty).Trim ();
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
				return String.Format (Resources.TitleCharacterName, (new System.Globalization.CultureInfo (pLanguage)).EnglishName, String.Empty).Trim ();
			}
			else
			{
				return String.Format (Resources.TitleCharacterName, (new System.Globalization.CultureInfo (pLanguage)).EnglishName, pName.Quoted ());
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		static public String Animation (FileAnimation pAnimation)
		{
			if (pAnimation == null)
			{
				return String.Format (Resources.TitleAnimation, String.Empty).Trim ();
			}
			else
			{
				return Animation (pAnimation.Name);
			}
		}

		static public String Animation (String pAnimationName)
		{
			return String.Format (Resources.TitleAnimation, pAnimationName.Quoted ());
		}

		///////////////////////////////////////////////////////////////////////////////

		static public String Frame (FileAnimationFrame pFrame)
		{
			if ((pFrame == null) || (pFrame.Container == null))
			{
				return String.Format (Resources.TitleFrame, String.Empty).Trim ();
			}
			else
			{
				return Frame (pFrame.Container.IndexOf (pFrame));
			}
		}

		static public String Frame (int pFrameNdx)
		{
			return String.Format (Resources.TitleFrame, (pFrameNdx + 1).ToString ());
		}

		static public String FrameAnimation (FileAnimationFrame pFrame)
		{
			if ((pFrame == null) || (pFrame.Animation == null))
			{
				return String.Format (Resources.TitleFrame, String.Empty).Trim ();
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
				return String.Format (Resources.TitleFrame, String.Empty).Trim ();
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
			return String.Format (Resources.TitleFrameAnimation, pFrameTitle, pAnimationName);
		}

		///////////////////////////////////////////////////////////////////////////////

		static public String Image (FileFrameImage pImage)
		{
			if ((pImage == null) || (pImage.Container == null))
			{
				return String.Format (Resources.TitleImage, String.Empty).Trim ();
			}
			else
			{
				return Image (pImage.Container.IndexOf (pImage));
			}
		}

		static public String Image (int pImageNdx)
		{
			return String.Format (Resources.TitleImage, (pImageNdx + 1).ToString ());
		}

		static public String ImageFrame (FileFrameImage pImage)
		{
			if ((pImage == null) || (pImage.Frame == null) || (pImage.Frame.Animation == null))
			{
				return String.Format (Resources.TitleImage, String.Empty).Trim ();
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
				return String.Format (Resources.TitleImage, String.Empty).Trim ();
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
			return String.Format (Resources.TitleImageFrame, pImageTitle, pFrameName);
		}

		static public String ImageFrameAnimation (FileFrameImage pImage)
		{
			if ((pImage == null) || (pImage.Frame == null) || (pImage.Frame.Animation == null))
			{
				return String.Format (Resources.TitleImage, String.Empty).Trim ();
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
				return String.Format (Resources.TitleImage, String.Empty).Trim ();
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
			return String.Format (Resources.TitleImageFrame, pImageTitle, pFrameTitle);
		}

		///////////////////////////////////////////////////////////////////////////////

		static public String OverlayTypeName (MouthOverlay pOverlayType)
		{
			switch (pOverlayType)
			{
				case MouthOverlay.MouthOverlayClosed:
					return Resources.MouthOverlayClosed;
				case MouthOverlay.MouthOverlayWide1:
					return Resources.MouthOverlayWide1;
				case MouthOverlay.MouthOverlayWide2:
					return Resources.MouthOverlayWide2;
				case MouthOverlay.MouthOverlayWide3:
					return Resources.MouthOverlayWide3;
				case MouthOverlay.MouthOverlayWide4:
					return Resources.MouthOverlayWide4;
				case MouthOverlay.MouthOverlayMedium:
					return Resources.MouthOverlayMedium;
				case MouthOverlay.MouthOverlayNarrow:
					return Resources.MouthOverlayNarrow;
				default:
					return Resources.MouthOverlay;
			}
		}

		static public String Overlay (FileFrameOverlay pOverlay)
		{
			if (pOverlay == null)
			{
				return String.Format (Resources.TitleOverlay, String.Empty).Trim ();
			}
			else
			{
				return Overlay (pOverlay.OverlayType);
			}
		}

		static public String Overlay (MouthOverlay pOverlayType)
		{
			return String.Format (Resources.TitleOverlay, OverlayTypeName (pOverlayType));
		}

		static public String OverlayFrameAnimation (FileFrameOverlay pOverlay)
		{
			if (pOverlay == null)
			{
				return String.Format (Resources.TitleOverlay, String.Empty).Trim ();
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
				return String.Format (Resources.TitleOverlay, String.Empty).Trim ();
			}
			else
			{
				return String.Format (Resources.TitleOverlayFrame, Overlay (pOverlayType), FrameAnimation (pFrame));
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		static public String State (FileState pState)
		{
			if (pState == null)
			{
				return String.Format (Resources.TitleState, String.Empty).Trim ();
			}
			else
			{
				return State (pState.StateName);
			}
		}

		static public String State (String pStateName)
		{
			return String.Format (Resources.TitleState, pStateName.Quoted ());
		}
	}
}
