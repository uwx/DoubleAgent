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
using System.Text;
using System.Drawing;
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor
{
	namespace Updates
	{
		internal class UpdateCharacterHeader : UndoableUpdate<CharacterFile>
		{
			public UpdateCharacterHeader (CharacterFile pCharacterFile)
				: base (pCharacterFile, pCharacterFile)
			{
				ImageSize = Target.Header.ImageSize;
				Guid = Target.Header.Guid;
				IconFilePath = Target.IconFilePath;
				PaletteFilePath = Target.PaletteFilePath;
				PaletteTransparency = Target.Header.Transparency;
			}

			public Size ImageSize
			{
				get;
				set;
			}
			public Guid Guid
			{
				get;
				set;
			}
			public String IconFilePath
			{
				get;
				set;
			}
			public String PaletteFilePath
			{
				get;
				set;
			}
			public Byte PaletteTransparency
			{
				get;
				set;
			}
			public UInt16 NewFrameDuration
			{
				get;
				set;
			}
			public override String TargetDescription
			{
				get
				{
					return ImageSizeChanged ? Global.TitleAnimation ((FileAnimation)null) + " " + Properties.Resources.UndoAnimationSize : GuidChanged ? Properties.Resources.UndoGuid : IconChanged ? Properties.Resources.UndoIcon : PaletteChanged ? Properties.Resources.UndoPalette : PaletteTransparencyChanged ? Properties.Resources.UndoPaletteTransparency : NewFrameDurationChanged ? Properties.Resources.UndoNewFrameDuration : String.Empty;
				}
			}

			public override UndoUnit Apply ()
			{
				UpdateCharacterHeader	lApplied = null;

				if (ImageSizeChanged)
				{
					Size lSwap = Target.Header.ImageSize;
					Target.Header.ImageSize = ImageSize;
					ImageSize = lSwap;
					lApplied = this;
				}
				if (GuidChanged)
				{
					Guid lSwap = Target.Header.Guid;
					Target.Header.Guid = Guid;
					Guid = lSwap;
					lApplied = this;
				}
				if (IconChanged)
				{
					String	lSwap = Target.IconFilePath;
					Target.IconFilePath = IconFilePath;
					IconFilePath = lSwap;
					lApplied = this;
				}
				if (PaletteChanged)
				{
					String	lSwap = Target.PaletteFilePath;
					Target.PaletteFilePath = PaletteFilePath;
					PaletteFilePath = lSwap;
					lApplied = this;
				}
				if (PaletteTransparencyChanged)
				{
					Byte lSwap = Target.Header.Transparency;
					Target.Header.Transparency = PaletteTransparency;
					PaletteTransparency = lSwap;
					lApplied = this;
				}
				if (NewFrameDurationChanged)
				{
					UInt16 lSwap = Target.NewFrameDuration;
					Target.NewFrameDuration = NewFrameDuration;
					NewFrameDuration = lSwap;
					lApplied = this;
				}
				if (lApplied != null)
				{
					return OnApplied (lApplied);
				}
				return null;
			}

			public Boolean ImageSizeChanged
			{
				get
				{
					return (Target.Header.ImageSize != ImageSize);
				}
			}
			public Boolean GuidChanged
			{
				get
				{
					return (Target.Header.Guid != Guid);
				}
			}
			public Boolean IconChanged
			{
				get
				{
					return !String.Equals (Target.IconFilePath, IconFilePath);
				}
			}
			public Boolean PaletteChanged
			{
				get
				{
					return !String.Equals (Target.PaletteFilePath, PaletteFilePath);
				}
			}
			public Boolean PaletteTransparencyChanged
			{
				get
				{
					return (Target.Header.Transparency != PaletteTransparency);
				}
			}
			public Boolean NewFrameDurationChanged
			{
				get
				{
					return (Target.NewFrameDuration != NewFrameDuration);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal class UpdateCharacterBalloon : UndoableUpdate<CharacterFile>
		{
			public UpdateCharacterBalloon (CharacterFile pCharacterFile)
				: base (pCharacterFile, pCharacterFile)
			{
				CharacterStyle = pCharacterFile.Header.Style & mStyleMask;
				if (pCharacterFile.Balloon != null)
				{
					UndoObject = new FileBalloon ();
					pCharacterFile.Balloon.CopyTo (UndoObject);
				}
			}

			public CharacterStyle CharacterStyle
			{
				get;
				set;
			}
			public UInt16 Lines
			{
				get
				{
					return (UndoObject != null) ? UndoObject.Lines : (UInt16)0;
				}
				set
				{
					if (UndoObject != null)
						UndoObject.Lines = value;
				}
			}
			public UInt16 PerLine
			{
				get
				{
					return (UndoObject != null) ? UndoObject.PerLine : (UInt16)0;
				}
				set
				{
					if (UndoObject != null)
						UndoObject.PerLine = value;
				}
			}
			public Color BkColor
			{
				get
				{
					return (UndoObject != null) ? UndoObject.BkColor : Color.Empty;
				}
				set
				{
					if (UndoObject != null)
						UndoObject.BkColor = value;
				}
			}
			public Color FgColor
			{
				get
				{
					return (UndoObject != null) ? UndoObject.FgColor : Color.Empty;
				}
				set
				{
					if (UndoObject != null)
						UndoObject.FgColor = value;
				}
			}
			public Color BrColor
			{
				get
				{
					return (UndoObject != null) ? UndoObject.BrColor : Color.Empty;
				}
				set
				{
					if (UndoObject != null)
						UndoObject.BrColor = value;
				}
			}
			public Font Font
			{
				get
				{
					return (UndoObject != null) ? UndoObject.Font : null;
				}
				set
				{
					if (UndoObject != null)
						UndoObject.Font = value;
				}
			}
			private FileBalloon UndoObject
			{
				get;
				set;
			}

			public override String TargetDescription
			{
				get
				{
					return Properties.Resources.TitleBalloon;
				}
			}
			public override String ChangeDescription
			{
				get
				{
					return StyleChanged ? Properties.Resources.UndoBalloonStyle : SizeChanged ? Properties.Resources.UndoBalloonSize : ColorChanged ? Properties.Resources.UndoBalloonColor : FontChanged ? Properties.Resources.UndoBalloonFont : String.Empty;
				}
			}

			public override UndoUnit Apply ()
			{
				UndoUnit	lApplied = null;

				if (StyleChanged)
				{
					CharacterStyle	lSwap = CharacterFile.Header.Style & mStyleMask;
					CharacterFile.Header.Style = (CharacterFile.Header.Style & ~mStyleMask) | (CharacterStyle & mStyleMask);
					CharacterStyle = lSwap;
					lApplied = this;
				}
				if ((CharacterFile.Balloon != null) && (UndoObject != null) && !UndoObject.Equals (CharacterFile.Balloon))
				{
					FileBalloon	lSwap = new FileBalloon ();
					CharacterFile.Balloon.CopyTo (lSwap);
					UndoObject.CopyTo (CharacterFile.Balloon);
					lSwap.CopyTo (UndoObject);
					lApplied = this;
				}
				if (lApplied != null)
				{
					return OnApplied (lApplied);
				}
				return null;
			}

			private const CharacterStyle	mStyleMask = (CharacterStyle.CharStyleBalloon | CharacterStyle.CharStyleSizeToText | CharacterStyle.CharStyleNoAutoHide | CharacterStyle.CharStyleNoAutoPace);

			public Boolean StyleChanged
			{
				get
				{
					return (CharacterFile.Header.Style & mStyleMask) != (CharacterStyle & mStyleMask);
				}
			}

			public Boolean SizeChanged
			{
				get
				{
					return (CharacterFile.Balloon != null) && ((CharacterFile.Balloon.Lines != Lines) || (CharacterFile.Balloon.PerLine != PerLine));
				}
			}

			public Boolean ColorChanged
			{
				get
				{
					return (CharacterFile.Balloon != null) && ((CharacterFile.Balloon.BkColor != BkColor) || (CharacterFile.Balloon.FgColor != FgColor) || (CharacterFile.Balloon.BrColor != BrColor));
				}
			}

			public Boolean FontChanged
			{
				get
				{
					try
					{
						return (CharacterFile.Balloon != null) && !CharacterFile.Balloon.Font.Equals (Font);
					}
					catch
					{
					}
					return false;
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal class UpdateCharacterTts : UndoableUpdate<CharacterFile>
		{
			public UpdateCharacterTts (CharacterFile pCharacterFile, Sapi4VoiceInfo pVoiceInfo)
				: base (pCharacterFile, pCharacterFile)
			{
				CharacterStyle = pCharacterFile.Header.Style & CharacterStyle.CharStyleTts;
				if (pVoiceInfo != null)
				{
					VoiceInfo = new Sapi4VoiceInfo ();
					VoiceInfo.EngineId = pVoiceInfo.EngineId;
					VoiceInfo.ModeId = pVoiceInfo.ModeId;
					VoiceInfo.LangId = pVoiceInfo.LangId;
					VoiceInfo.SpeakerGender = pVoiceInfo.SpeakerGender;
				}
			}

			public CharacterStyle CharacterStyle
			{
				get;
				set;
			}
			public Sapi4VoiceInfo VoiceInfo
			{
				get;
				private set;
			}

			public override String TargetDescription
			{
				get
				{
					return Properties.Resources.TitleTts;
				}
			}

			public override UndoUnit Apply ()
			{
				UndoUnit	lApplied = null;

				if ((CharacterFile.Header.Style & CharacterStyle.CharStyleTts) != (CharacterStyle & CharacterStyle.CharStyleTts))
				{
					CharacterStyle	lSwap = CharacterFile.Header.Style & CharacterStyle.CharStyleTts;
					CharacterFile.Header.Style = (CharacterFile.Header.Style & ~CharacterStyle.CharStyleTts) | (CharacterStyle & CharacterStyle.CharStyleTts);
					CharacterStyle = lSwap;
					lApplied = this;
				}
				if ((CharacterFile.Tts != null) && (VoiceInfo != null))
				{
					Sapi4VoiceInfo	lVoiceInfo = new Sapi4VoiceInfo ();

					lVoiceInfo.EngineId = CharacterFile.Tts.Engine;
					lVoiceInfo.ModeId = CharacterFile.Tts.Mode;
					lVoiceInfo.LangId = CharacterFile.Tts.Language;
					lVoiceInfo.SpeakerGender = CharacterFile.Tts.Gender;

					if (!lVoiceInfo.Equals (VoiceInfo))
					{
						CharacterFile.Tts.Engine = VoiceInfo.EngineId;
						CharacterFile.Tts.Mode = VoiceInfo.ModeId;
						CharacterFile.Tts.Language = VoiceInfo.LangId;
						CharacterFile.Tts.Gender = VoiceInfo.SpeakerGender;
						VoiceInfo = lVoiceInfo;
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

		///////////////////////////////////////////////////////////////////////////////

		internal class AddDeleteCharacterName : UndoableAddDelete<FileCharacterName>
		{
			public AddDeleteCharacterName (CharacterFile pCharacterFile, FileCharacterName pName, Boolean pRemove, Boolean pForClipboard)
				: base (pCharacterFile, pName, pForClipboard)
			{
				IsDelete = pRemove;
			}

			public override String TargetDescription
			{
				get
				{
					return Global.TitleCharacterName (Target);
				}
			}

			public override UndoUnit Apply ()
			{
				AddDeleteCharacterName	lApplied = null;

				if (IsDelete)
				{
					if (CharacterFile.Names.Remove (Target))
					{
						lApplied = new AddDeleteCharacterName (CharacterFile, Target, false, ForClipboard);
						lApplied.IsRedo = !IsRedo;
					}
				}
				else
				{
					FileCharacterName	lName = CharacterFile.Names.Add (Target.Language, Target.Name);

					if (lName != null)
					{
						Target.CopyTo (lName);
						lApplied = new AddDeleteCharacterName (CharacterFile, lName, true, ForClipboard);
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

		internal class UpdateCharacterName : UndoableUpdate<CharacterFile>
		{
			public UpdateCharacterName (CharacterFile pCharacterFile, UInt16 pLanguage, String pName, String pDesc1, String pDesc2)
				: base (pCharacterFile, pCharacterFile)
			{
				UndoObject = new FileCharacterName (pLanguage, pName);
				UndoObject.Desc1 = pDesc1;
				UndoObject.Desc2 = pDesc2;
			}

			public UpdateCharacterName (CharacterFile pCharacterFile, FileCharacterName pName, Boolean pForClipboard)
				: base (pCharacterFile, pCharacterFile, pForClipboard)
			{
				UndoObject = new FileCharacterName (pName.Language, pName);
			}

			public UInt16 Language
			{
				get
				{
					return UndoObject.Language;
				}
			}
			public String Name
			{
				get
				{
					return UndoObject.Name;
				}
			}
			public String Desc1
			{
				get
				{
					return UndoObject.Desc1;
				}
			}
			public String Desc2
			{
				get
				{
					return UndoObject.Desc2;
				}
			}
			private FileCharacterName UndoObject
			{
				get;
				set;
			}


			public override String TargetDescription
			{
				get
				{
					return Global.TitleCharacterName (Language);
				}
			}
			public override String ActionDescription
			{
				get
				{
					return ForClipboard ? base.ActionDescription : Properties.Resources.UndoActionChange;
				}
			}

			public override UndoUnit Apply ()
			{
				UpdateCharacterName	lApplied = null;
				FileCharacterName	lName;
				FileCharacterName	lSwap;

				lName = CharacterFile.FindName (Language);
				if (!Global.PrimaryLanguageEqual (lName, Language))
				{
					lName = null;
				}
				if (lName != null)
				{
#if DEBUG_NOT
					System.Diagnostics.Debug.Print ("Applying {0}", lName.ToString ());
#endif
					if (!UndoObject.Equals (lName))
					{
						lSwap = new FileCharacterName (lName.Language, lName);
						UndoObject.CopyTo (lName);
						lSwap.CopyTo (UndoObject);
						lApplied = this;
					}
#if DEBUG_NOT
				if (lApplied != null)
				{
					System.Diagnostics.Debug.Print ("Applyed  {0}", lName.ToString ());
				}
#endif
				}
				if (lApplied != null)
				{
					return OnApplied (lApplied);
				}
				return null;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal class AddDeleteStateAnimation : UndoableUpdate<CharacterFile>
		{
			public AddDeleteStateAnimation (CharacterFile pCharacterFile, String pStateName, String pAnimationName, Boolean pIsDelete)
				: base (pCharacterFile, pCharacterFile)
			{
				StateName = pStateName;
				AnimationName = pAnimationName;
				IsDelete = pIsDelete;
			}

			public String StateName
			{
				get;
				private set;
			}
			public String AnimationName
			{
				get;
				private set;
			}
			public Boolean IsDelete
			{
				get;
				private set;
			}

			public override String TargetDescription
			{
				get
				{
					return Global.QuotedTitle (StateName);
				}
			}
			public override String ChangeDescription
			{
				get
				{
					return String.Format (Properties.Resources.UndoStateAnimation, Global.QuotedTitle (AnimationName));
				}
			}

			public override UndoUnit Apply ()
			{
				if (IsDelete)
				{
					if (CharacterFile.States.RemoveStateAnimation (StateName, AnimationName))
					{
						return OnApplied (new AddDeleteStateAnimation (CharacterFile, StateName, AnimationName, false));
					}
				}
				else
				{
					if (CharacterFile.States.AddStateAnimation (StateName, AnimationName))
					{
						return OnApplied (new AddDeleteStateAnimation (CharacterFile, StateName, AnimationName, true));
					}
				}
				return null;
			}
		}

		internal class UpdateAllStateAnimations : UndoableUpdate<CharacterFile>
		{
			public UpdateAllStateAnimations (CharacterFile pCharacterFile, String pStateName, String[] pAnimationNames)
				: base (pCharacterFile, pCharacterFile)
			{
				StateName = pStateName;
				AnimationNames = pAnimationNames;
			}

			public String StateName
			{
				get;
				private set;
			}
			public String[] AnimationNames
			{
				get;
				private set;
			}
			public override String TargetDescription
			{
				get
				{
					return Global.TitleState (StateName);
				}
			}

			public override UndoUnit Apply ()
			{
				UpdateAllStateAnimations	lApplied = null;
				String[]					lAnimationNames = null;

				if (CharacterFile.States.ContainsKey (StateName))
				{
					lAnimationNames = CharacterFile.States[StateName].Clone () as String[];
				}
				if (lAnimationNames != null)
				{
					foreach (String lAnimationName in lAnimationNames)
					{
						if ((AnimationNames == null) || !Array.Exists (AnimationNames, lAnimationName.Equals))
						{
							if (CharacterFile.States.RemoveStateAnimation (StateName, lAnimationName))
							{
								lApplied = this;
							}
						}
					}
				}
				if (AnimationNames != null)
				{
					foreach (String lAnimationName in AnimationNames)
					{
						if ((lAnimationNames == null) || !Array.Exists (lAnimationNames, lAnimationName.Equals))
						{
							if (CharacterFile.States.AddStateAnimation (StateName, lAnimationName))
							{
								lApplied = this;
							}
						}
					}
				}
				if (lApplied != null)
				{
					lApplied.AnimationNames = lAnimationNames;
					return OnApplied (lApplied);
				}
				return null;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal class AddDeleteAnimation : UndoableAddDelete<FileAnimation>
		{
			public AddDeleteAnimation (CharacterFile pCharacterFile, String pAnimationName, Boolean pForClipboard)
				: base (pCharacterFile, null, pForClipboard)
			{
				AnimationName = pAnimationName;
			}

			public AddDeleteAnimation (CharacterFile pCharacterFile, FileAnimation pAnimation, Boolean pForClipboard)
				: base (pCharacterFile, pAnimation, pForClipboard)
			{
				AnimationName = pAnimation.Name;
			}

			public String AnimationName
			{
				get;
				private set;
			}
			public override String TargetDescription
			{
				get
				{
					return Global.TitleAnimation (AnimationName);
				}
			}

			public override UndoUnit Apply ()
			{
				AddDeleteAnimation	lApplied = null;
				FileAnimation		lAnimation = null;

				if (IsDelete)
				{
					if (CharacterFile.Gestures.Contains (AnimationName))
					{
						lAnimation = CharacterFile.Gestures[AnimationName];

						if (CharacterFile.Gestures.Remove (AnimationName))
						{
							lApplied = new AddDeleteAnimation (CharacterFile, lAnimation, ForClipboard);
							lApplied.IsDelete = false;
							lApplied.IsRedo = !IsRedo;
						}
					}
				}
				else
				{
					lAnimation = CharacterFile.Gestures.Add (AnimationName);

					if (lAnimation != null)
					{
						if (Target != null)
						{
							Target.CopyTo (lAnimation);
						}
						lApplied = new AddDeleteAnimation (CharacterFile, lAnimation, ForClipboard);
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

		internal class UpdateAnimation : UndoableUpdate<FileAnimation>
		{
			public UpdateAnimation (CharacterFile pCharacterFile, FileAnimation pAnimation, Boolean pForClipboard)
				: base (pCharacterFile, pAnimation, pForClipboard)
			{
				UndoObject = new FileAnimation ();
				UndoObject.Name = Target.Name;
				Target.CopyTo (UndoObject);
				IsRedo = false;
			}

			public String Name
			{
				get
				{
					return UndoObject.Name;
				}
				set
				{
					UndoObject.Name = value;
				}
			}
			public Byte ReturnType
			{
				get
				{
					return UndoObject.ReturnType;
				}
				set
				{
					UndoObject.ReturnType = value;
				}
			}
			public String ReturnName
			{
				get
				{
					return UndoObject.ReturnName;
				}
				set
				{
					UndoObject.ReturnName = value;
				}
			}
			public Boolean IsRedo
			{
				get;
				private set;
			}
			private FileAnimation UndoObject
			{
				get;
				set;
			}

			public override String TargetDescription
			{
				get
				{
					return Global.QuotedTitle (Target.Name);
				}
			}
			public override String ActionDescription
			{
				get
				{
					if (NameChanged)
					{
						if (IsRedo)
						{
							return String.Format (Properties.Resources.UndoAnimationName, String.Format (Properties.Resources.UndoAnimationNames, Global.QuotedTitle (Name), String.Empty));
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
					if (NameChanged)
					{
						if (!IsRedo)
						{
							return String.Format (Properties.Resources.UndoAnimationNames, String.Empty, Global.QuotedTitle (Name));
						}
					}
					else
					{
						return Properties.Resources.UndoReturnAnimation;
					}
					return String.Empty;
				}
			}

			public override UndoUnit Apply ()
			{
				UpdateAnimation	lApplied = null;

				if (NameChanged)
				{
					lApplied = new UpdateAnimation (CharacterFile, Target, ForClipboard);
					Target.Name = Name;
					Name = lApplied.Name;
					lApplied.IsRedo = !IsRedo;
				}
				else
				{
					if (ReturnTypeChanged || ReturnNameChanged)
					{
						FileAnimation	lSwap = new FileAnimation ();
						Target.CopyTo (lSwap);
						UndoObject.CopyTo (Target);
						lSwap.CopyTo (UndoObject);
						lApplied = this;
					}
				}
				if (lApplied != null)
				{
					return OnApplied (lApplied);
				}
				return null;
			}

			public Boolean NameChanged
			{
				get
				{
					return !String.Equals (Target.Name, Name);
				}
			}
			public Boolean ReturnTypeChanged
			{
				get
				{
					return (Target.ReturnType != ReturnType);
				}
			}
			public Boolean ReturnNameChanged
			{
				get
				{
					return !String.Equals (Target.ReturnName, ReturnName);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal class AddDeleteAnimationFrame : UndoableAddDelete<FileAnimationFrame>
		{
			public AddDeleteAnimationFrame (CharacterFile pCharacterFile, FileAnimation pAnimation, int pFrameNdx, Boolean pForClipboard)
				: base (pCharacterFile, null, pForClipboard)
			{
				Animation = pAnimation;
				FrameNdx = pFrameNdx;
			}

			public AddDeleteAnimationFrame (CharacterFile pCharacterFile, FileAnimationFrame pFrame, Boolean pForClipboard)
				: base (pCharacterFile, pFrame, pForClipboard)
			{
				Animation = pFrame.Animation;
				FrameNdx = pFrame.Container.IndexOf (Target);
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
					return Global.QuotedTitle (Global.TitleFrameAnimation (FrameNdx, Animation));
				}
			}

			public override UndoUnit Apply ()
			{
				AddDeleteAnimationFrame	lApplied = null;

				if (IsDelete)
				{
					if (Animation.Frames.Contains (Target))
					{
						FrameNdx = Animation.Frames.IndexOf (Target);
						lApplied = new AddDeleteAnimationFrame (CharacterFile, Target, ForClipboard);
						if (Animation.Frames.Remove (Target))
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
					FileAnimationFrame	lFrame = Animation.Frames.Insert (FrameNdx);

					if (lFrame != null)
					{
						if (Target != null)
						{
							Target.CopyTo (lFrame);
						}
						Target = lFrame;
						FrameNdx = Animation.Frames.IndexOf (Target);
						lApplied = new AddDeleteAnimationFrame (CharacterFile, Target, ForClipboard);
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

		internal class ReorderAnimationFrame : UndoableUpdate<FileAnimationFrame>
		{
			public ReorderAnimationFrame (CharacterFile pCharacterFile, FileAnimation pAnimation, FileAnimationFrame pFrame, int pFrameNdxTo)
				: base (pCharacterFile, pFrame)
			{
				Animation = pAnimation;
				FrameNdxFrom = pAnimation.Frames.IndexOf (Target);
				FrameNdxTo = pFrameNdxTo;
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
					return Global.TitleAnimation (Animation);
				}
			}
			public override String ChangeDescription
			{
				get
				{
					return Properties.Resources.UndoFrameOrder;
				}
			}

			public override UndoUnit Apply ()
			{
				int	lSwap = Animation.Frames.IndexOf (Target);
				if (Animation.Frames.Move (Target, FrameNdxTo))
				{
					FrameNdxFrom = Animation.Frames.IndexOf (Target);
					FrameNdxTo = lSwap;

					return OnApplied (this);
				}
				return null;
			}
		}

		internal class UpdateAnimationFrame : UndoableUpdate<FileAnimationFrame>
		{
			public UpdateAnimationFrame (CharacterFile pCharacterFile, FileAnimationFrame pFrame, Boolean pForClipboard)
				: base (pCharacterFile, pFrame, pForClipboard)
			{
				UndoObject = new FileAnimationFrame ();
				SoundFilePath = pFrame.SoundFilePath;
				pFrame.CopyTo (UndoObject);
			}

			public FileAnimation Animation
			{
				get
				{
					return Target.Animation;
				}
			}
			public UInt16 Duration
			{
				get
				{
					return UndoObject.Duration;
				}
				set
				{
					UndoObject.Duration = value;
				}
			}
			public Int16 ExitFrame
			{
				get
				{
					return UndoObject.ExitFrame;
				}
				set
				{
					UndoObject.ExitFrame = value;
				}
			}
			public FileFrameBranch[] Branching
			{
				get
				{
					return UndoObject.Branching;
				}
				set
				{
					UndoObject.Branching = value;
				}
			}
			public String SoundFilePath
			{
				get;
				set;
			}
			private FileAnimationFrame UndoObject
			{
				get;
				set;
			}


			public override String TargetDescription
			{
				get
				{
					return Global.QuotedTitle (Global.TitleFrameAnimation (Target));
				}
			}
			public override String ChangeDescription
			{
				get
				{
					if (ForClipboard)
					{
						return base.ChangeDescription;
					}
					else
					{
						return DurationChanged ? Properties.Resources.UndoFrameDuration : ExitFrameChanged ? Properties.Resources.UndoFrameExit : SoundChanged ? Properties.Resources.UndoFrameSoundPath : BranchingChanged ? Properties.Resources.UndoFrameBranching : String.Empty;
					}
				}
			}

			public override UndoUnit Apply ()
			{
				UndoUnit	lApplied = null;

				if (DurationChanged || ExitFrameChanged || BranchingChanged)
				{
					FileAnimationFrame	lSwap = new FileAnimationFrame ();
					Target.CopyTo (lSwap);
					UndoObject.CopyTo (Target);
					lSwap.CopyTo (UndoObject);
					lApplied = this;
				}
				if (SoundChanged)
				{
					String	lSwap = Target.SoundFilePath;
					Target.SoundFilePath = SoundFilePath;
					SoundFilePath = lSwap;
					lApplied = this;
				}
				if (lApplied != null)
				{
					return OnApplied (lApplied);
				}
				return null;
			}

			public Boolean DurationChanged
			{
				get
				{
					return (Target.Duration != Duration);
				}
			}
			public Boolean ExitFrameChanged
			{
				get
				{
					return (Target.ExitFrame != ExitFrame);
				}
			}
			public Boolean BranchingChanged
			{
				get
				{
					return !BranchingEqual (Target.Branching, Branching);
				}
			}
			public Boolean SoundChanged
			{
				get
				{
					return !String.Equals (Target.SoundFilePath, SoundFilePath);
				}
			}

			public static Boolean BranchingEqual (FileFrameBranch[] pSource, FileFrameBranch[] pTarget)
			{
				if ((pSource == null) != (pTarget == null))
				{
					return false;
				}
				if ((pSource != null) && (pTarget != null))
				{
					if (pSource.Length != pTarget.Length)
					{
						return false;
					}
					for (int lNdx = 0; lNdx < pSource.Length; lNdx++)
					{
						if ((pSource[lNdx].mFrameNdx != pTarget[lNdx].mFrameNdx) || (pSource[lNdx].mProbability != pTarget[lNdx].mProbability))
						{
							return false;
						}
					}
				}
				return true;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal class AddDeleteFrameImage : UndoableAddDelete<FileFrameImage>
		{
			public AddDeleteFrameImage (CharacterFile pCharacterFile, FileAnimationFrame pFrame, String pImageFilePath, Boolean pForClipboard)
				: base (pCharacterFile, pForClipboard)
			{
				Frame = pFrame;
				ImageFilePath = pImageFilePath;
			}

			public AddDeleteFrameImage (CharacterFile pCharacterFile, FileFrameImage pImage, Boolean pForClipboard)
				: base (pCharacterFile, pImage, pForClipboard)
			{
				Frame = pImage.Frame;
				ImageFilePath = Target.ImageFilePath;
				ImageNdx = pImage.Container.IndexOf (Target);
			}

			public FileAnimation Animation
			{
				get
				{
					return Frame.Animation;
				}
			}
			public FileAnimationFrame Frame
			{
				get;
				set;
			}
			public String ImageFilePath
			{
				get;
				private set;
			}
			public int ImageNdx
			{
				get;
				private set;
			}
			public override String TargetDescription
			{
				get
				{
					return Global.QuotedTitle (Global.TitleImageFrameAnimation (ImageNdx, Frame));
				}
			}

			public override UndoUnit Apply ()
			{
				AddDeleteFrameImage	lApplied = null;

				if (IsDelete)
				{
					if (Frame.Images.Contains (Target))
					{
						ImageNdx = Frame.Images.IndexOf (Target);
						lApplied = new AddDeleteFrameImage (CharacterFile, Target, ForClipboard);
						if (Frame.Images.Remove (Target))
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
					FileFrameImage	lFrameImage = Frame.Images.Add (ImageFilePath);

					if (lFrameImage != null)
					{
						if (Target != null)
						{
							Target.CopyTo (lFrameImage);
							Frame.Images.Move (lFrameImage, ImageNdx);
						}
						Target = lFrameImage;
						ImageNdx = Frame.Images.IndexOf (Target);
						lApplied = new AddDeleteFrameImage (CharacterFile, Target, ForClipboard);
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

		internal class ReorderFrameImage : UndoableUpdate<FileFrameImage>
		{
			public ReorderFrameImage (CharacterFile pCharacterFile, FileFrameImage pImage, int pImageNdxTo)
				: base (pCharacterFile, pImage)
			{
				ImageNdxFrom = Frame.Images.IndexOf (Target);
				ImageNdxTo = pImageNdxTo;
			}

			public FileAnimation Animation
			{
				get
				{
					return Frame.Animation;
				}
			}
			public FileAnimationFrame Frame
			{
				get
				{
					return Target.Frame;
				}
			}
			public int ImageNdxFrom
			{
				get;
				private set;
			}
			public int ImageNdxTo
			{
				get;
				private set;
			}

			public override String TargetDescription
			{
				get
				{
					return Global.QuotedTitle (Global.TitleFrameAnimation (Frame));
				}
			}
			public override String ChangeDescription
			{
				get
				{
					return Properties.Resources.UndoImageOrder;
				}
			}

			public override UndoUnit Apply ()
			{
				int	lSwap = Frame.Images.IndexOf (Target);
				if (Frame.Images.Move (Target, ImageNdxTo))
				{
					ImageNdxFrom = Frame.Images.IndexOf (Target);
					ImageNdxTo = lSwap;

					return OnApplied (this);
				}
				return null;
			}
		}

		internal class UpdateFrameImage : UndoableUpdate<FileFrameImage>
		{
			public UpdateFrameImage (CharacterFile pCharacterFile, FileFrameImage pImage, Boolean pForClipboard)
				: base (pCharacterFile, pImage, pForClipboard)
			{
				UndoObject = new FileFrameImage ();
				Target.CopyTo (UndoObject);
				ImageFilePath = Target.ImageFilePath;
			}

			public FileAnimation Animation
			{
				get
				{
					return Frame.Animation;
				}
			}
			public FileAnimationFrame Frame
			{
				get
				{
					return Target.Frame;
				}
			}
			public Point Offset
			{
				get
				{
					return UndoObject.Offset;
				}
				set
				{
					UndoObject.Offset = value;
				}
			}
			public String ImageFilePath
			{
				get;
				set;
			}
			private FileFrameImage UndoObject
			{
				get;
				set;
			}

			public override String TargetDescription
			{
				get
				{
					return Global.QuotedTitle (Global.TitleImageFrameAnimation (Target));
				}
			}
			public override String ChangeDescription
			{
				get
				{
					if (ForClipboard)
					{
						return base.ChangeDescription;
					}
					else
					{
						return (Target.Offset != Offset) ? Properties.Resources.UndoImageOffset : (!String.Equals (Target.ImageFilePath, ImageFilePath)) ? Properties.Resources.UndoImageFilePath : String.Empty;
					}
				}
			}

			public override UndoUnit Apply ()
			{
				UndoUnit	lApplied = null;

				if (Target.Offset != Offset)
				{
					FileFrameImage	lSwap = new FileFrameImage ();
					Target.CopyTo (lSwap);
					UndoObject.CopyTo (Target);
					lSwap.CopyTo (UndoObject);
					lApplied = this;
				}
				if (!String.Equals (Target.ImageFilePath, ImageFilePath))
				{
					String	lSwap = Target.ImageFilePath;
					Target.ImageFilePath = ImageFilePath;
					ImageFilePath = lSwap;
					lApplied = this;
				}
				if (lApplied != null)
				{
					return OnApplied (lApplied);
				}
				return null;
			}
#if DEBUG
			public override String DebugString ()
			{
				return String.Format ("UndoOffset {2:D},{3:D} TargetOffset {0:D},{1:D}", Offset.X, Offset.Y, Target.Offset.X, Target.Offset.Y);
			}
#endif
		}

		///////////////////////////////////////////////////////////////////////////////

		internal class AddDeleteFrameOverlay : UndoableAddDelete<FileFrameOverlay>
		{
			public AddDeleteFrameOverlay (CharacterFile pCharacterFile, FileFrameOverlay pOverlay, Boolean pForClipboard)
				: base (pCharacterFile, pOverlay, pForClipboard)
			{
				Frame = pOverlay.Frame;
				OverlayType = Target.OverlayType;
				OverlayImagePath = Target.ImageFilePath;
			}
			public AddDeleteFrameOverlay (CharacterFile pCharacterFile, FileAnimationFrame pFrame, MouthOverlay pOverlayType, String pOverlayImagePath, Boolean pForClipboard)
				: base (pCharacterFile, pForClipboard)
			{
				Frame = pFrame;
				OverlayType = pOverlayType;
				OverlayImagePath = pOverlayImagePath;
			}

			public FileAnimationFrame Frame
			{
				get;
				private set;
			}
			public MouthOverlay OverlayType
			{
				get;
				private set;
			}
			public String OverlayImagePath
			{
				get;
				private set;
			}

			public override String TargetDescription
			{
				get
				{
					return Global.QuotedTitle (Global.TitleOverlayFrameAnimation (Frame, OverlayType));
				}
			}

			public override UndoUnit Apply ()
			{
				AddDeleteFrameOverlay	lApplied = null;

				if (IsDelete)
				{
					if (Frame.Overlays.Contains (Target))
					{
						lApplied = new AddDeleteFrameOverlay (CharacterFile, Target, ForClipboard);
						if (Frame.Overlays.Remove (Target))
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
					FileFrameOverlay	lFrameOverlay = Frame.Overlays.Add (OverlayType, OverlayImagePath);

					if (lFrameOverlay != null)
					{
						if (Target != null)
						{
							Target.CopyTo (lFrameOverlay);
						}
						Target = lFrameOverlay;
						lApplied = new AddDeleteFrameOverlay (CharacterFile, Target, ForClipboard);
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

		internal class UpdateFrameOverlay : UndoableUpdate<FileFrameOverlay>
		{
			public UpdateFrameOverlay (CharacterFile pCharacterFile, FileFrameOverlay pOverlay, Boolean pForClipboard)
				: base (pCharacterFile, pOverlay, pForClipboard)
			{
				UndoObject = new FileFrameOverlay ();
				Target.CopyTo (UndoObject);
				OverlayImagePath = Target.ImageFilePath;
			}

			public FileAnimationFrame Frame
			{
				get
				{
					return Target.Frame;
				}
			}
			public Boolean ReplaceFlag
			{
				get
				{
					return UndoObject.ReplaceFlag;
				}
				set
				{
					UndoObject.ReplaceFlag = value;
				}
			}
			public Point Offset
			{
				get
				{
					return UndoObject.Offset;
				}
				set
				{
					UndoObject.Offset = value;
				}
			}
			public String OverlayImagePath
			{
				get;
				set;
			}
			private FileFrameOverlay UndoObject
			{
				get;
				set;
			}

			public override String TargetDescription
			{
				get
				{
					return Global.QuotedTitle (Global.TitleOverlayFrameAnimation (Target));
				}
			}
			public override String ChangeDescription
			{
				get
				{
					if (ForClipboard)
					{
						return base.ChangeDescription;
					}
					else
					{
						return (Target.ReplaceFlag != ReplaceFlag) ? Properties.Resources.UndoOverlayReplaceFlag : (Target.Offset != Offset) ? Properties.Resources.UndoOverlayOffset : (!String.Equals (Target.ImageFilePath, OverlayImagePath)) ? Properties.Resources.UndoOverlayImagePath : String.Empty;
					}
				}
			}

			public override UndoUnit Apply ()
			{
				UndoUnit	lApplied = null;

				if ((Target.ReplaceFlag != ReplaceFlag) || (Target.Offset != Offset))
				{
					FileFrameOverlay	lSwap = new FileFrameOverlay ();
					Target.CopyTo (lSwap);
					UndoObject.CopyTo (Target);
					lSwap.CopyTo (UndoObject);
					lApplied = this;
				}
				if (!String.Equals (Target.ImageFilePath, OverlayImagePath))
				{
					String	lSwap = Target.ImageFilePath;
					Target.ImageFilePath = OverlayImagePath;
					OverlayImagePath = lSwap;
					lApplied = this;
				}
				if (lApplied != null)
				{
					return OnApplied (lApplied);
				}
				return null;
			}
		}
	}
}

