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

namespace AgentCharacterEditor.Navigation
{
	public abstract class ResolvePart
	{
		public CharacterFile CharacterFile
		{
			get
			{
				return Program.MainForm.CharacterFile;
			}
		}
		public abstract Object Part
		{
			get;
		}
	}

	internal abstract class ResolvePart<T> : ResolvePart where T : class
	{
		public override Object Part
		{
			get
			{
				return Target;
			}
		}
		public abstract T Target
		{
			get;
		}
		public Boolean TargetContained
		{
			get;
			protected set;
		}
		protected T CachedTarget
		{
			get;
			set;
		}
		protected Boolean TargetCached
		{
			get
			{
				return (CachedTarget != null);
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	internal class ResolveCharacter : ResolvePart<CharacterFile>
	{
		public override CharacterFile Target
		{
			get
			{
				try
				{
					return CharacterFile;
				}
				catch
				{
				}
				return null;
			}
		}

		public enum ScopeType
		{
			ScopeCharacter,
			ScopeAnimations,
			ScopeStates
		}
		public ScopeType Scope
		{
			get;
			set;
		}
	}

	internal class ResolveHeader : ResolvePart<FileHeader>
	{
		public override FileHeader Target
		{
			get
			{
				try
				{
					return (CharacterFile != null) ? CharacterFile.Header : null;
				}
				catch
				{
				}
				return null;
			}
		}
	}

	internal class ResolveBalloon : ResolvePart<FileBalloon>
	{
		public override FileBalloon Target
		{
			get
			{
				try
				{
					return (CharacterFile != null) && ((CharacterFile.Header.Style & CharacterStyle.Balloon) != CharacterStyle.None) ? CharacterFile.Balloon : null;
				}
				catch
				{
				}
				return null;
			}
		}
	}

	internal class ResolveTts : ResolvePart<FileTts>
	{
		public override FileTts Target
		{
			get
			{
				try
				{
					return (CharacterFile != null) && ((CharacterFile.Header.Style & CharacterStyle.Tts) != CharacterStyle.None) ? CharacterFile.Tts : null;
				}
				catch
				{
				}
				return null;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	internal class ResolveState : ResolvePart<FileState>
	{
		public ResolveState (String pStateName)
		{
			StateName = pStateName;
			TargetContained = true;
			TargetContained = (Target != null);
		}

		public String StateName
		{
			get;
			protected set;
		}
		public override FileState Target
		{
			get
			{
				try
				{
					if ((CharacterFile != null) && (CharacterFile.States != null) && (!TargetCached || (TargetContained && !CharacterFile.States.Contains (CachedTarget))))
					{
						CachedTarget = null;
						if (!String.IsNullOrEmpty (StateName))
						{
							if (CharacterFile.States.Contains (StateName))
							{
								CachedTarget = CharacterFile.States[StateName];
							}
							else if (CharacterFile.States.Contains (StateName.ToUpper ()))
							{
								CachedTarget = CharacterFile.States[StateName.ToUpper ()];
							}
						}
					}
				}
				catch
				{
				}
#if DEBUG_NOT
				if (!TargetCached)
				{
					System.Diagnostics.Debug.Print ("State [{0}] not found", StateName);
				}
#endif
				return CachedTarget;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	internal class ResolveName : ResolvePart<FileCharacterName>
	{
		public ResolveName (UInt16 pLanguage)
		{
			Language = pLanguage;
			TargetContained = true;
			TargetContained = (Target != null);
		}
		public ResolveName (FileCharacterName pName)
		{
			Language = pName.Language;
			CachedTarget = pName;
			TargetContained = (pName.Owner == CharacterFile) && CharacterFile.Names.Contains (pName);
		}

		public UInt16 Language
		{
			get;
			protected set;
		}
		public override FileCharacterName Target
		{
			get
			{
				try
				{
					if (!TargetCached || (TargetContained && (CharacterFile != null) && !CharacterFile.Names.Contains (CachedTarget)))
					{
						CachedTarget = CharacterFile.FindName (Language);
						if (!Language.PrimaryLanguageEqual (CachedTarget))
						{
							CachedTarget = null;
						}
					}
				}
				catch
				{
				}
#if DEBUG_NOT
				if (!TargetCached)
				{
					System.Diagnostics.Debug.Print ("Name [{0:D} {0:X4}] not found", Language);
				}
#endif
				return CachedTarget;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	internal class ResolveAnimation : ResolvePart<FileAnimation>
	{
		public ResolveAnimation (String pAnimationName)
		{
			AnimationName = pAnimationName;
			TargetContained = true;
			TargetContained = (Target != null);
		}
		public ResolveAnimation (FileAnimation pAnimation)
		{
			AnimationName = pAnimation.Name;
			CachedTarget = pAnimation;
			TargetContained = (pAnimation.Owner == CharacterFile) && CharacterFile.Gestures.Contains (pAnimation);
		}

		public String AnimationName
		{
			get;
			protected set;
		}
		public override FileAnimation Target
		{
			get
			{
				try
				{
					if (!TargetCached || (TargetContained && (CharacterFile != null) && !CharacterFile.Gestures.Contains (CachedTarget)))
					{
						CachedTarget = CharacterFile.Gestures[AnimationName];
					}
				}
				catch
				{
				}
#if DEBUG_NOT
				if (!TargetCached)
				{
					System.Diagnostics.Debug.Print ("Animation [{0}] not found", AnimationName);
				}
#endif
				return CachedTarget;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	internal class ResolveAnimationFrame : ResolvePart<FileAnimationFrame>
	{
		public ResolveAnimationFrame (FileAnimation pAnimation, int pFrameNdx)
		{
			Animation = new ResolveAnimation (pAnimation);
			FrameNdx = pFrameNdx;
			TargetContained = true;
			TargetContained = (Target != null);
		}
		public ResolveAnimationFrame (FileAnimationFrame pFrame)
		{
			Animation = new ResolveAnimation (pFrame.Animation);
			FrameNdx = (pFrame.Container != null) ? pFrame.Container.IndexOf (pFrame) : -1;
			CachedTarget = pFrame;
			TargetContained = (Animation.TargetContained) && (pFrame.Container != null) && pFrame.Container.Contains (pFrame);
		}

		public enum ScopeType
		{
			ScopeFrame,
			ScopeOverlays,
			ScopeBranching
		}
		public ScopeType Scope
		{
			get;
			set;
		}

		public ResolveAnimation Animation
		{
			get;
			set;
		}
		public int FrameNdx
		{
			get;
			protected set;
		}
		public override FileAnimationFrame Target
		{
			get
			{
				try
				{
					if (!TargetCached || (TargetContained && !Animation.Target.Frames.Contains (CachedTarget)))
					{
						CachedTarget = Animation.Target.Frames[FrameNdx];
					}
				}
				catch
				{
				}
#if DEBUG_NOT
				if (!TargetCached)
				{
					System.Diagnostics.Debug.Print ("AnimationFrame [{0} ({1:D})] not found", Animation.AnimationName, FrameNdx);
				}
#endif
				return CachedTarget;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	internal class ResolveFrameImage : ResolvePart<FileFrameImage>
	{
		public ResolveFrameImage (FileAnimationFrame pFrame, int pImageNdx)
		{
			Frame = new ResolveAnimationFrame (pFrame);
			ImageNdx = pImageNdx;
			TargetContained = true;
			TargetContained = (Target != null);
		}
		public ResolveFrameImage (FileFrameImage pImage)
		{
			Frame = new ResolveAnimationFrame (pImage.Container.Frame);
			ImageNdx = (pImage.Container != null) ? pImage.Container.IndexOf (pImage) : -1;
			CachedTarget = pImage;
			TargetContained = (Frame.TargetContained) && (pImage.Container != null) && pImage.Container.Contains (pImage);
		}

		public ResolveAnimationFrame Frame
		{
			get;
			set;
		}
		public int ImageNdx
		{
			get;
			protected set;
		}
		public override FileFrameImage Target
		{
			get
			{
				try
				{
					if (!TargetCached || (TargetContained && !Frame.Target.Images.Contains (CachedTarget)))
					{
						CachedTarget = Frame.Target.Images[ImageNdx];
					}
				}
				catch
				{
				}
#if DEBUG_NOT
				if (!TargetCached)
				{
					System.Diagnostics.Debug.Print ("FrameImage [{0} ({1:D}) ({2:D})] not found", Frame.Animation.AnimationName, Frame.FrameNdx, ImageNdx);
				}
#endif
				return CachedTarget;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	internal class ResolveFrameOverlay : ResolvePart<FileFrameOverlay>
	{
		public ResolveFrameOverlay (FileAnimationFrame pFrame, MouthOverlay pOverlayType)
		{
			Frame = new ResolveAnimationFrame (pFrame);
			OverlayType = pOverlayType;
			TargetContained = true;
			TargetContained = (Target != null);
		}
		public ResolveFrameOverlay (FileFrameOverlay pOverlay)
		{
			Frame = new ResolveAnimationFrame (pOverlay.Container.Frame);
			OverlayType = pOverlay.OverlayType;
			CachedTarget = pOverlay;
			TargetContained = (Frame.TargetContained) && (pOverlay.Container != null) && pOverlay.Container.Contains (pOverlay);
		}

		public ResolveAnimationFrame Frame
		{
			get;
			set;
		}
		public MouthOverlay OverlayType
		{
			get;
			protected set;
		}
		public override FileFrameOverlay Target
		{
			get
			{
				try
				{
					if (!TargetCached || (TargetContained && !Frame.Target.Overlays.Contains (CachedTarget)))
					{
						CachedTarget = Frame.Target.Overlays[OverlayType];
					}
				}
				catch
				{
				}
#if DEBUG_NOT
				if (!TargetCached)
				{
					System.Diagnostics.Debug.Print ("FrameOverlay [{0} ({1:D}) ({2:D})] not found", Frame.Animation.AnimationName, Frame.FrameNdx, (int)OverlayType);
				}
#endif
				return CachedTarget;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public class NavigationEventArgs : EventArgs
	{
		public NavigationEventArgs (ResolvePart pPart)
		{
			Part = pPart;
		}
		public ResolvePart Part
		{
			get;
			private set;
		}
	}

	public delegate void NavigationEventHandler (object sender, NavigationEventArgs e);
}

