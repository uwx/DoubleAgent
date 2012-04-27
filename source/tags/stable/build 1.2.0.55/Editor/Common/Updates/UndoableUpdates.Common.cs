/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
using System.Drawing;
using System.Text;
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Properties;
using DoubleAgent;
using DoubleAgent.Character;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor.Updates
{
	///////////////////////////////////////////////////////////////////////////////
	#region Character

	internal class UpdateCharacterHeader : UndoableUpdate<FileHeader>
	{
		public UpdateCharacterHeader ()
		{
			ImageSize = Target.ImageSize;
			Guid = Target.Guid;
			IconFilePath = CharacterFile.IconFilePath;
			PaletteFilePath = CharacterFile.PaletteFilePath;
			PaletteTransparency = Target.Transparency;
			NewFrameDuration = CharacterFile.NewFrameDuration;
		}

		public override FileHeader Target
		{
			get
			{
				return CharacterFile.Header;
			}
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
				return ImageSizeChanged ? Titles.Animation ((FileAnimation)null) + " " + AppResources.Resources.UndoAnimationSize : GuidChanged ? AppResources.Resources.UndoGuid : IconChanged ? AppResources.Resources.UndoIcon : PaletteChanged ? AppResources.Resources.UndoPalette : PaletteTransparencyChanged ? AppResources.Resources.UndoPaletteTransparency : NewFrameDurationChanged ? AppResources.Resources.UndoNewFrameDuration : String.Empty;
			}
		}

		//=============================================================================

		public override UndoUnit Apply ()
		{
			UpdateCharacterHeader lApplied = null;
#if DEBUG
			System.Diagnostics.Debug.Print ("Apply {0}", DebugString);
#endif
			if (ImageSizeChanged)
			{
				Size lSwap = Target.ImageSize;
				Target.ImageSize = ImageSize;
				ImageSize = lSwap;
				lApplied = this;
			}
			if (GuidChanged)
			{
				Guid lSwap = Target.Guid;
				Target.Guid = Guid;
				Guid = lSwap;
				lApplied = this;
			}
			if (IconChanged)
			{
				String lSwap = CharacterFile.IconFilePath;
				CharacterFile.IconFilePath = IconFilePath;
				IconFilePath = lSwap;
				lApplied = this;
			}
			if (PaletteChanged)
			{
				String lSwap = CharacterFile.PaletteFilePath;
				CharacterFile.PaletteFilePath = PaletteFilePath;
				PaletteFilePath = lSwap;
				lApplied = this;
			}
			if (PaletteTransparencyChanged)
			{
				Byte lSwap = Target.Transparency;
				Target.Transparency = PaletteTransparency;
				PaletteTransparency = lSwap;
				lApplied = this;
			}
			if (NewFrameDurationChanged)
			{
				UInt16 lSwap = CharacterFile.NewFrameDuration;
				CharacterFile.NewFrameDuration = NewFrameDuration;
				NewFrameDuration = lSwap;
				lApplied = this;
			}
			if (lApplied != null)
			{
				return OnApplied (lApplied);
			}
			return null;
		}

		//=============================================================================

		public Boolean ImageSizeChanged
		{
			get
			{
				return (Target.ImageSize != ImageSize);
			}
		}
		public Boolean GuidChanged
		{
			get
			{
				return (Target.Guid != Guid);
			}
		}
		public Boolean IconChanged
		{
			get
			{
				return !String.Equals (CharacterFile.IconFilePath, IconFilePath);
			}
		}
		public Boolean PaletteChanged
		{
			get
			{
				return !String.Equals (CharacterFile.PaletteFilePath, PaletteFilePath);
			}
		}
		public Boolean PaletteTransparencyChanged
		{
			get
			{
				return (Target.Transparency != PaletteTransparency);
			}
		}
		public Boolean NewFrameDurationChanged
		{
			get
			{
				return (CharacterFile.NewFrameDuration != NewFrameDuration);
			}
		}

#if DEBUG
		public override string DebugString
		{
			get
			{
				return String.Format ("From [{0:D},{1:D}] [{2}] [{3}] [{4} {5}] [{6}]", Target.ImageSize.Width, Target.ImageSize.Height, Target.Guid.ToString ().ToUpper (), CharacterFile.IconFilePath, CharacterFile.PaletteFilePath, Target.Transparency.ToString (), CharacterFile.NewFrameDuration.ToString ())
				 + String.Format (" to [{0:D},{1:D}] [{2}] [{3}] [{4} {5}] [{6}]", ImageSize.Width, ImageSize.Height, Guid.ToString ().ToUpper (), IconFilePath, PaletteFilePath, PaletteTransparency.ToString (), NewFrameDuration.ToString ());
			}
		}
#endif
	}

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	internal class AddDeleteCharacterName : UndoableAddDelete<FileCharacterName>
	{
		public AddDeleteCharacterName (FileCharacterName pName, Boolean pIsDelete, Boolean pForClipboard)
			: base (new ResolveName (pName), pForClipboard)
		{
			IsDelete = pIsDelete;
		}

		public UInt16 Language
		{
			get
			{
				return (RawTarget as ResolveName).Language;
			}
		}
		public override String TargetDescription
		{
			get
			{
				return Titles.CharacterName (Target);
			}
		}

		//=============================================================================

		public override UndoUnit Apply ()
		{
			AddDeleteCharacterName lApplied = null;
			FileCharacterName lTarget = Target;
#if DEBUG
			System.Diagnostics.Debug.Print ("Apply {0}", DebugString);
#endif
			if (IsDelete)
			{
				if (CharacterFile.Names.Remove (lTarget))
				{
					lApplied = new AddDeleteCharacterName (lTarget, false, IsForClipboard);
					lApplied.IsRedo = !IsRedo;
				}
			}
			else
			{
				FileCharacterName lName = CharacterFile.Names.Add (Target.Language, Target.Name);

				if (lName != null)
				{
					lTarget.CopyTo (lName);
					lApplied = new AddDeleteCharacterName (lName, true, IsForClipboard);
					lApplied.IsRedo = !IsRedo;
				}
			}
			if (lApplied != null)
			{
				return OnApplied (lApplied);
			}
			return null;
		}

		//=============================================================================

#if DEBUG
		public override string DebugString
		{
			get
			{
				return String.Format ("{0}({1}) [{2:D} {2:X4}]", ActionDescription, RedoString, Language);
			}
		}
#endif
	}

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	internal class UpdateCharacterName : UndoableUpdate<FileCharacterName>
	{
		public UpdateCharacterName (UInt16 pLanguage, String pName, String pDesc1, String pDesc2)
			: base (new ResolveName (pLanguage))
		{
			UndoObject = new FileCharacterName (pLanguage, pName);
			UndoObject.Desc1 = pDesc1;
			UndoObject.Desc2 = pDesc2;
		}

		public UpdateCharacterName (FileCharacterName pName, Boolean pForClipboard)
			: base (new ResolveName (pName), pForClipboard)
		{
			UndoObject = new FileCharacterName (pName.Language, pName);
			pName.CopyTo (UndoObject);
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
				return Titles.CharacterName (Language);
			}
		}
		public override String ActionDescription
		{
			get
			{
				return IsForClipboard ? base.ActionDescription : AppResources.Resources.UndoActionChange;
			}
		}

		public override UndoUnit Apply ()
		{
			UpdateCharacterName lApplied = null;
			FileCharacterName lTarget = Target;
			FileCharacterName lSwap;
#if DEBUG
			System.Diagnostics.Debug.Print ("Apply {0}", DebugString);
#endif
			if ((lTarget != null) && !UndoObject.Equals (lTarget))
			{
				lSwap = new FileCharacterName (lTarget.Language, lTarget);
				UndoObject.CopyTo (lTarget);
				lSwap.CopyTo (UndoObject);
				lApplied = this;
			}
			if (lApplied != null)
			{
				return OnApplied (lApplied);
			}
			return null;
		}
#if DEBUG
		public override string DebugString
		{
			get
			{
				FileCharacterName lTarget = Target;
				return ((lTarget != null) ? String.Format ("From [{0:D} {0:X4}] [{1}] [{2}] [{3}]", lTarget.Language, lTarget.Name, lTarget.Desc1, lTarget.Desc2) : "From <empty>")
				+ String.Format (" to [{0:D} {0:X4}] [{1}] [{2}] [{3}]", Language, Name, Desc1, Desc2);
			}
		}
#endif
	}

	#endregion
	///////////////////////////////////////////////////////////////////////////////
	#region Balloon

	internal class UpdateCharacterBalloon : UndoableUpdate<FileBalloon>
	{
		public UpdateCharacterBalloon ()
		{
			CharacterStyle = CharacterFile.Header.Style & mStyleMask;
			if (Target != null)
			{
				UndoObject = new FileBalloon ();
				Target.CopyTo (UndoObject);
			}
		}

		public override FileBalloon Target
		{
			get
			{
				return CharacterFile.Balloon;
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
				{
					UndoObject.Lines = Math.Min (Math.Max (value, FileBalloon.MinLines), FileBalloon.MaxLines);
				}
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
				{
					UndoObject.PerLine = Math.Min (Math.Max (value, FileBalloon.MinPerLine), FileBalloon.MaxPerLine);
				}
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
				{
					UndoObject.BkColor = value;
				}
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
				{
					UndoObject.FgColor = value;
				}
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
				{
					UndoObject.BrColor = value;
				}
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
				{
					UndoObject.Font = value;
				}
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
				return AppResources.Resources.TitleBalloon;
			}
		}
		public override String ChangeDescription
		{
			get
			{
				return StyleChanged ? AppResources.Resources.UndoBalloonStyle : SizeChanged ? AppResources.Resources.UndoBalloonSize : ColorChanged ? AppResources.Resources.UndoBalloonColor : FontChanged ? AppResources.Resources.UndoBalloonFont : String.Empty;
			}
		}

		public override UndoUnit Apply ()
		{
			UndoUnit lApplied = null;
#if DEBUG
			System.Diagnostics.Debug.Print ("Apply {0}", DebugString);
#endif
			if (StyleChanged)
			{
				CharacterStyle lSwap = CharacterFile.Header.Style & mStyleMask;
				CharacterFile.Header.Style = (CharacterFile.Header.Style & ~mStyleMask) | (CharacterStyle & mStyleMask);
				CharacterStyle = lSwap;
				lApplied = this;
			}
			if ((Target != null) && (UndoObject != null) && !UndoObject.Equals (Target))
			{
				FileBalloon lSwap = new FileBalloon ();
				Target.CopyTo (lSwap);
				UndoObject.CopyTo (Target);
				lSwap.CopyTo (UndoObject);
				lApplied = this;
			}
			if (lApplied != null)
			{
				return OnApplied (lApplied);
			}
			return null;
		}

		private const CharacterStyle mStyleMask = (CharacterStyle.Balloon | CharacterStyle.SizeToText | CharacterStyle.NoAutoHide | CharacterStyle.NoAutoPace);

		public Boolean EnabledChanged
		{
			get
			{
				return (CharacterFile.Header.Style & CharacterStyle.Balloon) != (CharacterStyle & CharacterStyle.Balloon);
			}
		}

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
				return (Target != null) && ((Target.Lines != Lines) || (Target.PerLine != PerLine));
			}
		}

		public Boolean ColorChanged
		{
			get
			{
				return (Target != null) && ((Target.BkColor != BkColor) || (Target.FgColor != FgColor) || (Target.BrColor != BrColor));
			}
		}

		public Boolean FontChanged
		{
			get
			{
				try
				{
					if (Target != null)
					{
						if ((Target.Font == null) != (Font == null))
						{
							return true;
						}
						else if ((Target.Font != null) && (Font != null))
						{
							return !Target.Font.Equals (Font);
						}
					}
				}
				catch
				{
				}
				return false;
			}
		}

#if DEBUG
		public override string DebugString
		{
			get
			{
				return String.Format ("From [{0}]", FileHeader.StyleString (CharacterFile.Header.Style))
				+ ((Target != null) ? String.Format (" [{0} {1}] [{2:X8} {3:X8} {4:X8}] {5}", Target.Lines.ToString (), Target.PerLine.ToString (), Target.FgColor.ToArgb (), Target.BkColor.ToArgb (), Target.BrColor.ToArgb (), (Target.Font != null) ? Target.Font.ToString () : "[]") : String.Empty)
				+ String.Format (" to [{0}] [{1} {2}] [{3:X8} {4:X8} {5:X8}] {6}", FileHeader.StyleString (CharacterStyle), Lines.ToString (), PerLine.ToString (), FgColor.ToArgb (), BkColor.ToArgb (), BrColor.ToArgb (), (Font != null) ? Font.ToString () : "[]");
			}
		}
#endif
	}

	#endregion
	///////////////////////////////////////////////////////////////////////////////
	#region TTS

	internal class UpdateCharacterTts : UndoableUpdate<FileTts>
	{
		public UpdateCharacterTts (Sapi4VoiceInfo pVoiceInfo)
		{
			CharacterStyle = CharacterFile.Header.Style & CharacterStyle.Tts;
			if (pVoiceInfo != null)
			{
				VoiceInfo = new Sapi4VoiceInfo ();
				VoiceInfo.EngineId = pVoiceInfo.EngineId;
				VoiceInfo.ModeId = pVoiceInfo.ModeId;
				VoiceInfo.LangId = pVoiceInfo.LangId;
				VoiceInfo.SpeakerGender = pVoiceInfo.SpeakerGender;
			}
		}

		public override FileTts Target
		{
			get
			{
				return CharacterFile.Tts;
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
				return AppResources.Resources.TitleTts;
			}
		}

		public override UndoUnit Apply ()
		{
			UndoUnit lApplied = null;
#if DEBUG
			System.Diagnostics.Debug.Print ("Apply {0}", DebugString);
#endif
			if (EnabledChanged)
			{
				CharacterStyle lSwap = CharacterFile.Header.Style & CharacterStyle.Tts;
				CharacterFile.Header.Style = (CharacterFile.Header.Style & ~CharacterStyle.Tts) | (CharacterStyle & CharacterStyle.Tts);
				CharacterStyle = lSwap;
				Target = CharacterFile.Tts;
				lApplied = this;
			}
			if ((Target != null) && (VoiceInfo != null))
			{
				Sapi4VoiceInfo lVoiceInfo = new Sapi4VoiceInfo ();

				lVoiceInfo.EngineId = Target.Engine;
				lVoiceInfo.ModeId = Target.Mode;
				lVoiceInfo.LangId = Target.Language;
				lVoiceInfo.SpeakerGender = Target.Gender;

				if (!lVoiceInfo.Equals (VoiceInfo))
				{
					Target.Engine = VoiceInfo.EngineId;
					Target.Mode = VoiceInfo.ModeId;
					Target.Language = VoiceInfo.LangId;
					Target.Gender = VoiceInfo.SpeakerGender;
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

		public Boolean EnabledChanged
		{
			get
			{
				return (CharacterFile.Header.Style & CharacterStyle.Tts) != (CharacterStyle & CharacterStyle.Tts);
			}
		}
#if DEBUG
		public override string DebugString
		{
			get
			{
				return String.Format ("From [{0}]", FileHeader.StyleString (CharacterFile.Header.Style))
				+ ((Target != null) ? String.Format (" [{0}] [{1}] [{2:D} {2:X4}] [{3}]", Target.Engine.ToString ().ToUpper (), Target.Mode.ToString ().ToUpper (), Target.Language, Target.Gender.ToString ()) : String.Empty)
				+ String.Format (" to [{0}]", FileHeader.StyleString (CharacterStyle))
				+ ((VoiceInfo != null) ? String.Format (" [{0}] [{1}] [{2:D} {2:X4}] [{3}]", VoiceInfo.EngineId.ToString ().ToUpper (), VoiceInfo.ModeId.ToString ().ToUpper (), VoiceInfo.LangId, VoiceInfo.SpeakerGender.ToString ()) : String.Empty);
			}
		}
#endif
	}

	#endregion
	///////////////////////////////////////////////////////////////////////////////
	#region State

	internal class AddDeleteStateAnimation : UndoableAddDelete<FileState>
	{
		public AddDeleteStateAnimation (String pStateName, String pAnimationName, Boolean pIsDelete)
			: base (new ResolveState (pStateName))
		{
			AnimationName = pAnimationName;
			IsDelete = pIsDelete;
		}

		public String StateName
		{
			get
			{
				return (Target == null) ? (RawTarget as ResolveState).StateName : Target.StateName;
			}
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
				return Target.StateName.Quoted ();
			}
		}
		public override String ChangeDescription
		{
			get
			{
				return String.Format (AppResources.Resources.UndoStateAnimation, AnimationName.Quoted ());
			}
		}

		public override UndoUnit Apply ()
		{
			UndoableUpdate lApplied = null;
#if DEBUG
			System.Diagnostics.Debug.Print ("Apply {0}", DebugString);
#endif
			if (IsDelete)
			{
				if (CharacterFile.States.RemoveStateAnimation (StateName, AnimationName))
				{
					IsDelete = false;
					IsRedo = !IsRedo;
					lApplied = this;
				}
			}
			else
			{
				if (CharacterFile.States.AddStateAnimation (StateName, AnimationName))
				{
					IsDelete = true;
					IsRedo = !IsRedo;
					lApplied = this;
				}
			}
			if (lApplied != null)
			{
				return OnApplied (lApplied);
			}
			return null;
		}

#if DEBUG
		public override string DebugString
		{
			get
			{
				return String.Format ("{0}({1}) [{2}] [{3}]", ActionDescription, RedoString, Target, AnimationName);
			}
		}
#endif
	}

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	internal class UpdateAllStateAnimations : UndoableUpdate<FileState>
	{
		public UpdateAllStateAnimations (String pStateName, String[] pAnimationNames)
			: base (new ResolveState (pStateName))
		{
			AnimationNames = pAnimationNames;
		}

		public String StateName
		{
			get
			{
				return (Target == null) ? (RawTarget as ResolveState).StateName : Target.StateName;
			}
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
				return Titles.State (Target);
			}
		}

		public override UndoUnit Apply ()
		{
			UpdateAllStateAnimations lApplied = null;
			FileState lState = Target;
			String[] lAnimationNames = null;

#if DEBUG
			System.Diagnostics.Debug.Print ("Apply {0}", DebugString);
#endif
			if (lState != null)
			{
				lAnimationNames = lState.AnimationNames.Clone () as String[];
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

#if DEBUG
		public override string DebugString
		{
			get
			{
				String[] lAnimationNames = null;

				if (CharacterFile.States.Contains (Target))
				{
					lAnimationNames = Target.AnimationNames;
				}
				return String.Format ("[{0}] from [{1}] to [{2}]", StateName, (lAnimationNames == null) ? "<empty>" : String.Join (" ", lAnimationNames), (AnimationNames == null) ? "<empty>" : String.Join (" ", AnimationNames));
			}
		}
#endif
	}

	#endregion
	///////////////////////////////////////////////////////////////////////////////
	#region Animation

	internal class AddDeleteAnimation : UndoableAddDelete<FileAnimation>
	{
		public AddDeleteAnimation (String pAnimationName, Boolean pForClipboard)
			: base (new ResolveAnimation (pAnimationName), pForClipboard)
		{
			IsDelete = false;
		}

		public AddDeleteAnimation (FileAnimation pAnimation, Boolean pForClipboard)
			: base (new ResolveAnimation (pAnimation), pForClipboard)
		{
			IsDelete = true;
		}

		public String AnimationName
		{
			get
			{
				return (RawTarget as ResolveAnimation).AnimationName;
			}
		}
		public override String TargetDescription
		{
			get
			{
				return Titles.Animation (AnimationName);
			}
		}

		public override UndoUnit Apply ()
		{
			AddDeleteAnimation lApplied = null;
			FileAnimation lTarget = Target;
#if DEBUG
			System.Diagnostics.Debug.Print ("Apply {0}", DebugString);
#endif
			if (IsDelete)
			{
				if (CharacterFile.Gestures.Remove (AnimationName))
				{
					lApplied = new AddDeleteAnimation (lTarget, IsForClipboard);
					lApplied.IsDelete = false;
					lApplied.IsRedo = !IsRedo;
				}
			}
			else
			{
				FileAnimation lAnimation = CharacterFile.Gestures.Add (AnimationName);

				if (lAnimation != null)
				{
					if (lTarget != null)
					{
						lTarget.CopyTo (lAnimation);
					}
					lApplied = new AddDeleteAnimation (lAnimation, IsForClipboard);
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

#if DEBUG
		public override string DebugString
		{
			get
			{
				return String.Format ("{0}({1}) [{2}]", ActionDescription, RedoString, AnimationName);
			}
		}
#endif
	}

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	internal class UpdateAnimation : UndoableUpdate<FileAnimation>
	{
		public UpdateAnimation (FileAnimation pAnimation, Boolean pForClipboard)
			: base (new ResolveAnimation (pAnimation), pForClipboard)
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
				return Target.Name.Quoted ();
			}
		}
		public override String ActionDescription
		{
			get
			{
				if (IsForClipboard)
				{
					return base.ActionDescription;
				}
				else if (NameChanged)
				{
					if (IsRedo)
					{
						return String.Format (AppResources.Resources.UndoAnimationName, String.Format (AppResources.Resources.UndoAnimationNames, Name.Quoted (), String.Empty));
					}
					else
					{
						return String.Format (AppResources.Resources.UndoAnimationName, String.Empty);
					}
				}
				return String.Empty;
			}
		}
		public override String ChangeDescription
		{
			get
			{
				if (IsForClipboard)
				{
					return base.ChangeDescription;
				}
				else if (NameChanged)
				{
					if (!IsRedo)
					{
						return String.Format (AppResources.Resources.UndoAnimationNames, String.Empty, Name.Quoted ());
					}
				}
				else
				{
					return AppResources.Resources.UndoReturnAnimation;
				}
				return String.Empty;
			}
		}

		public override UndoUnit Apply ()
		{
			UpdateAnimation lApplied = null;
#if DEBUG
			System.Diagnostics.Debug.Print ("Apply {0}", DebugString);
#endif
			if (NameChanged)
			{
				lApplied = new UpdateAnimation (Target, IsForClipboard);
				Target.Name = Name;
				Name = lApplied.Name;
				lApplied.IsRedo = !IsRedo;
			}
			else
			{
				if (ReturnTypeChanged || ReturnNameChanged)
				{
					FileAnimation lSwap = new FileAnimation ();
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

#if DEBUG
		public override string DebugString
		{
			get
			{
				return String.Format ("{0} [{1}]", ActionDescription, Target.Name);
			}
		}
#endif
	}

	#endregion
	///////////////////////////////////////////////////////////////////////////////
	#region Frame

	internal class AddDeleteAnimationFrame : UndoableAddDelete<FileAnimationFrame>
	{
		public AddDeleteAnimationFrame (FileAnimation pAnimation, int pFrameNdx, Boolean pForClipboard)
			: base (new ResolveAnimationFrame (pAnimation, pFrameNdx), pForClipboard)
		{
			IsDelete = false;
		}

		public AddDeleteAnimationFrame (FileAnimationFrame pFrame, Boolean pForClipboard)
			: base (new ResolveAnimationFrame (pFrame), pForClipboard)
		{
			IsDelete = true;
		}

		public FileAnimation Animation
		{
			get
			{
				return (RawTarget as ResolveAnimationFrame).Animation.Target;
			}

		}
		public String AnimationName
		{
			get
			{
				return (RawTarget as ResolveAnimationFrame).Animation.AnimationName;
			}
		}
		public int FrameNdx
		{
			get
			{
				return (RawTarget as ResolveAnimationFrame).FrameNdx;
			}
		}
		public override String TargetDescription
		{
			get
			{
				return Titles.FrameAnimation (FrameNdx, AnimationName).Quoted ();
			}
		}

		//=============================================================================

		public void ShiftBranchingTargets (Boolean pShiftPrev, Boolean pShiftNext)
		{
			if ((pShiftPrev || pShiftNext) && (Animation != null))
			{
				Dictionary<FileAnimationFrame, UpdateAnimationFrame> lUpdates = new Dictionary<FileAnimationFrame, UpdateAnimationFrame> ();

				if (BranchingUpdates != null)
				{
					foreach (UpdateAnimationFrame lUpdate in BranchingUpdates)
					{
						lUpdates[lUpdate.Target] = lUpdate;
					}
				}

				if (pShiftPrev)
				{
					UpdateAnimationFrame.ShiftFrameBranchingTargets (Animation.Frames, Math.Min (FrameNdx + 1, Animation.Frames.Count - 1), -1, ref lUpdates);
				}
				else if (pShiftNext)
				{
					UpdateAnimationFrame.ShiftFrameBranchingTargets (Animation.Frames, FrameNdx, 1, ref lUpdates);
				}
				if (lUpdates.Count > 0)
				{
					BranchingUpdates = new UpdateAnimationFrame[lUpdates.Count];
					lUpdates.Values.CopyTo (BranchingUpdates, 0);
				}
			}
		}

		public String MoveBranchingSources (Boolean pMovePrev, Boolean pMoveNext)
		{
			String lError = null;

			if ((pMovePrev || pMoveNext) && (Animation != null))
			{
				Dictionary<FileAnimationFrame, UpdateAnimationFrame> lUpdates = new Dictionary<FileAnimationFrame, UpdateAnimationFrame> ();

				if (BranchingUpdates != null)
				{
					foreach (UpdateAnimationFrame lUpdate in BranchingUpdates)
					{
						lUpdates[lUpdate.Target] = lUpdate;
					}
				}

				if (pMovePrev && (FrameNdx > 0))
				{
					lError = UpdateAnimationFrame.MergeFrameBranching (Target, Animation.Frames[FrameNdx - 1], ref lUpdates);
				}
				else if (pMoveNext && (FrameNdx < Animation.Frames.Count - 1))
				{
					lError = UpdateAnimationFrame.MergeFrameBranching (Target, Animation.Frames[FrameNdx + 1], ref lUpdates);
				}
				if ((lError == null) && (lUpdates.Count > 0))
				{
					BranchingUpdates = new UpdateAnimationFrame[lUpdates.Count];
					lUpdates.Values.CopyTo (BranchingUpdates, 0);
				}
			}
			return lError;
		}

		private UpdateAnimationFrame[] ApplyBranchingUpdates (UpdateAnimationFrame[] pBranchingUpdates)
		{
			UpdateAnimationFrame[] lAppliedUpdates = null;

			if (pBranchingUpdates != null)
			{
				lAppliedUpdates = new UpdateAnimationFrame[pBranchingUpdates.Length];

				for (int lNdx = 0; lNdx < pBranchingUpdates.Length; lNdx++)
				{
					if (pBranchingUpdates[lNdx] != null)
					{
						lAppliedUpdates[lNdx] = pBranchingUpdates[lNdx].Apply () as UpdateAnimationFrame;
					}
				}
			}
			return lAppliedUpdates;
		}

		private UpdateAnimationFrame[] BranchingUpdates
		{
			get;
			set;
		}

		//=============================================================================

		public override UndoUnit Apply ()
		{
			AddDeleteAnimationFrame lApplied = null;
			UpdateAnimationFrame[] lAppliedBranchingUpdates = null;
			FileAnimation lAnimation = Animation;
			FileAnimationFrame lTarget = Target;
			int lFrameNdx = FrameNdx;
#if DEBUG
			System.Diagnostics.Debug.Print ("Apply {0}", DebugString);
#endif
			if (IsDelete)
			{
				lAppliedBranchingUpdates = ApplyBranchingUpdates (BranchingUpdates);
				lApplied = new AddDeleteAnimationFrame (lTarget, IsForClipboard);
				if (Animation.Frames.Remove (lTarget))
				{
					(lApplied.RawTarget as ResolveAnimationFrame).TargetContained = false;
					lApplied.IsDelete = false;
					lApplied.IsRedo = !IsRedo;
					lApplied.BranchingUpdates = lAppliedBranchingUpdates;
				}
				else
				{
					ApplyBranchingUpdates (lAppliedBranchingUpdates);
					lApplied = null;
				}
			}
			else
			{
				FileAnimationFrame lFrame = lAnimation.Frames.Insert (lFrameNdx);

				if (lFrame != null)
				{
					if ((lTarget != null) && (IsRedo || IsForClipboard))
					{
						lTarget.CopyTo (lFrame);
					}
					lApplied = new AddDeleteAnimationFrame (lFrame, IsForClipboard);
					lApplied.IsDelete = true;
					lApplied.IsRedo = !IsRedo;
					lApplied.BranchingUpdates = ApplyBranchingUpdates (BranchingUpdates);
				}
			}
			if (lApplied != null)
			{
#if DEBUG
				System.Diagnostics.Debug.Print ("Applied {0}", lApplied.DebugString);
#endif
				return OnApplied (lApplied);
			}
			return null;
		}

		//=============================================================================

#if DEBUG
		public override string DebugString
		{
			get
			{
				return String.Format ("{0}({1}) [{2} ({3})]", ActionDescription, RedoString, AnimationName, FrameNdx);
			}
		}
#endif
	}

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	internal class ReorderAnimationFrame : UndoableUpdate<FileAnimationFrame>
	{
		public ReorderAnimationFrame (FileAnimationFrame pFrame, int pFrameNdxTo)
			: base (new ResolveAnimationFrame (pFrame))
		{
			FrameNdxFrom = pFrame.Container.IndexOf (Target);
			FrameNdxTo = pFrameNdxTo;
		}

		public FileAnimation Animation
		{
			get
			{
				return (RawTarget as ResolveAnimationFrame).Animation.Target;
			}
		}
		public String AnimationName
		{
			get
			{
				return (RawTarget as ResolveAnimationFrame).Animation.AnimationName;
			}
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
				return Titles.Animation (AnimationName);
			}
		}
		public override String ChangeDescription
		{
			get
			{
				return AppResources.Resources.UndoFrameOrder;
			}
		}

		//=============================================================================

		public void SwapBranching (Boolean pSwapSources, Boolean pSwapTargets)
		{
			if (pSwapSources || pSwapTargets)
			{
				Dictionary<FileAnimationFrame, UpdateAnimationFrame> lUpdates = new Dictionary<FileAnimationFrame, UpdateAnimationFrame> ();
				FileAnimationFrame lTarget = Target;
				FileAnimationFrame lSwap = (FrameNdxTo < lTarget.Container.Count) ? Target.Container[FrameNdxTo] : null;

				if (pSwapSources)
				{
					UpdateAnimationFrame.SwapFrameBranchingSources (lTarget, lSwap, ref lUpdates);
				}
				if (pSwapTargets)
				{
					UpdateAnimationFrame.SwapFrameBranchingTargets (lTarget, lSwap, ref lUpdates);
				}
				if (lUpdates.Count > 0)
				{
					BranchingUpdates = new UpdateAnimationFrame[lUpdates.Count];
					lUpdates.Values.CopyTo (BranchingUpdates, 0);
				}
			}
		}

		private void ApplyBranchingUpdates ()
		{
			if (BranchingUpdates != null)
			{
				for (int lNdx = 0; lNdx < BranchingUpdates.Length; lNdx++)
				{
					if (BranchingUpdates[lNdx] != null)
					{
						BranchingUpdates[lNdx] = BranchingUpdates[lNdx].Apply () as UpdateAnimationFrame;
					}
				}
			}
		}

		private UpdateAnimationFrame[] BranchingUpdates
		{
			get;
			set;
		}

		//=============================================================================

		public override UndoUnit Apply ()
		{
			FileAnimationFrame lTarget = Target;
			int lSwap = lTarget.Container.IndexOf (lTarget);
#if DEBUG
			System.Diagnostics.Debug.Print ("Apply   {0}", DebugString);
#endif
			if (lTarget.Container.Move (lTarget, FrameNdxTo))
			{
				FrameNdxFrom = lTarget.Container.IndexOf (lTarget);
				FrameNdxTo = lSwap;
#if DEBUG
				System.Diagnostics.Debug.Print ("Applied {0}", DebugString);
#endif
				ApplyBranchingUpdates ();
				return OnApplied (this);
			}
			return null;
		}

		//=============================================================================

#if DEBUG
		public override string DebugString
		{
			get
			{
				return String.Format ("{0} [{1}] [{2:D} to {3:D}]", ChangeDescription, AnimationName, FrameNdxFrom, FrameNdxTo);
			}
		}
#endif
	}

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	internal class UpdateAnimationFrame : UndoableUpdate<FileAnimationFrame>
	{
		public UpdateAnimationFrame (FileAnimationFrame pFrame, Boolean pForClipboard)
			: base (new ResolveAnimationFrame (pFrame), pForClipboard)
		{
			UndoObject = new FileAnimationFrame ();
			pFrame.CopyTo (UndoObject);
		}

		public FileAnimation Animation
		{
			get
			{
				return (RawTarget as ResolveAnimationFrame).Animation.Target;
			}
		}
		public String AnimationName
		{
			get
			{
				return (RawTarget as ResolveAnimationFrame).Animation.AnimationName;
			}
		}
		public int FrameNdx
		{
			get
			{
				return (RawTarget as ResolveAnimationFrame).FrameNdx;
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
			get
			{
				return UndoObject.SoundFilePath;
			}
			set
			{
				UndoObject.SoundFilePath = value;
			}
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
				return Titles.FrameAnimation (Target).Quoted ();
			}
		}
		public override String ChangeDescription
		{
			get
			{
				if (IsForClipboard)
				{
					return base.ChangeDescription;
				}
				else
				{
					return DurationChanged ? AppResources.Resources.UndoFrameDuration : ExitFrameChanged ? AppResources.Resources.UndoFrameExit : SoundChanged ? AppResources.Resources.UndoFrameSoundPath : BranchingChanged ? AppResources.Resources.UndoFrameBranching : String.Empty;
				}
			}
		}

		//=============================================================================

		public override UndoUnit Apply ()
		{
			UndoUnit lApplied = null;
#if DEBUG
			System.Diagnostics.Debug.Print ("Apply   {0}", DebugString);
#endif
			if (DurationChanged || ExitFrameChanged || BranchingChanged || SoundChanged)
			{
				FileAnimationFrame lSwap = new FileAnimationFrame ();
				Target.CopyTo (lSwap);
				UndoObject.CopyTo (Target);
				lSwap.CopyTo (UndoObject);
				lApplied = this;
			}
			if (lApplied != null)
			{
#if DEBUG
				System.Diagnostics.Debug.Print ("Applied {0}", DebugString);
#endif
				return OnApplied (lApplied);
			}
			return null;
		}

		//=============================================================================

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
				return !Target.IsBranchingEqual (Branching);
			}
		}
		public Boolean SoundChanged
		{
			get
			{
				return !String.Equals (Target.SoundFilePath, SoundFilePath);
			}
		}

#if DEBUG
		public override string DebugString
		{
			get
			{
				return String.Format ("{0} ({1:D}) From [{2:D}] [{3:D} {4}] [{5:D} ({6})]", AnimationName, FrameNdx, Target.Duration, Target.ExitFrame, BranchingString (Target.Branching), Target.SoundNdx, Target.SoundFilePath)
				+ String.Format (" to [{0:D}] [{1:D} {2}] [{3:D} ({4})]", Duration, ExitFrame, BranchingString (Branching), Target.SoundNdx, SoundFilePath);
			}
		}

		public static String BranchingString (FileFrameBranch[] pBranching)
		{
			StringBuilder lString = new StringBuilder ();

			if (pBranching != null)
			{
				foreach (FileFrameBranch lBranching in pBranching)
				{
					lString.Append (String.Format ("({0:D} {1:D}) ", lBranching.mProbability, lBranching.mFrameNdx));
				}
			}
			return lString.ToString ().Trim ();
		}
#endif

		///////////////////////////////////////////////////////////////////////////////

		public static Boolean SwapFrameBranchingSources (FileAnimationFrame pFrame1, FileAnimationFrame pFrame2, ref Dictionary<FileAnimationFrame, UpdateAnimationFrame> pUpdates)
		{
			if ((pFrame1 != null) && (pFrame2 != null) && (pFrame1.HasBranching () || pFrame2.HasBranching ()))
			{
				UpdateAnimationFrame lUpdate;

				lUpdate = pUpdates.ContainsKey (pFrame1) ? pUpdates[pFrame1] : new UpdateAnimationFrame (pFrame1, false);
				pUpdates[pFrame1] = lUpdate;
				lUpdate.ExitFrame = pFrame2.ExitFrame;
				lUpdate.Branching = pFrame2.Branching;

				lUpdate = pUpdates.ContainsKey (pFrame2) ? pUpdates[pFrame2] : new UpdateAnimationFrame (pFrame2, false);
				pUpdates[pFrame2] = lUpdate;
				lUpdate.ExitFrame = pFrame1.ExitFrame;
				lUpdate.Branching = pFrame1.Branching;

				return true;
			}
			return false;
		}

		public static Boolean SwapFrameBranchingTargets (FileAnimationFrame pFrame1, FileAnimationFrame pFrame2, ref Dictionary<FileAnimationFrame, UpdateAnimationFrame> pUpdates)
		{
			Boolean lRet = false;

			if ((pFrame1 != null) && (pFrame2 != null) && (pFrame1.Container != null) && (pFrame2.Container == pFrame1.Container))
			{
				UpdateAnimationFrame lUpdate;
				int lFrameNdx1 = pFrame1.Container.IndexOf (pFrame1);
				int lFrameNdx2 = pFrame1.Container.IndexOf (pFrame2);

				foreach (FileAnimationFrame lFrame in pFrame1.Container)
				{
					if (lFrame.HasBranching (lFrameNdx1) || lFrame.HasBranching (lFrameNdx2))
					{
						lUpdate = pUpdates.ContainsKey (lFrame) ? pUpdates[lFrame] : new UpdateAnimationFrame (lFrame, false);
						pUpdates[lFrame] = lUpdate;
#if DEBUG_NOT
						System.Diagnostics.Debug.Print ("    Put {0}", lUpdate.DebugString);
#endif

						if (lFrame.Branching != null)
						{
							FileFrameBranch[] lBranching = lFrame.CopyBranching ();

							for (int lNdx = 0; lNdx < lFrame.Branching.Length; lNdx++)
							{
								if (lFrame.Branching[lNdx].mFrameNdx == lFrameNdx1)
								{
									lBranching[lNdx].mFrameNdx = (Int16)lFrameNdx2;
									lRet = true;
								}
								else if (lFrame.Branching[lNdx].mFrameNdx == lFrameNdx2)
								{
									lBranching[lNdx].mFrameNdx = (Int16)lFrameNdx1;
									lRet = true;
								}
							}
							if (lRet)
							{
								lUpdate.Branching = lBranching;
							}
						}

						if (lFrame.ExitFrame == lFrameNdx1)
						{
							lUpdate.ExitFrame = (Int16)lFrameNdx2;
							lRet = true;
						}
						else if (lFrame.ExitFrame == lFrameNdx2)
						{
							lUpdate.ExitFrame = (Int16)lFrameNdx1;
							lRet = true;
						}
#if DEBUG_NOT
						System.Diagnostics.Debug.Print ("    Put {0}", lUpdate.DebugString);
#endif
					}
				}
			}
			return lRet;
		}

		//=============================================================================

		public static Boolean ShiftFrameBranchingTargets (FileFrames pFrames, int pStartIndex, int pShift, ref Dictionary<FileAnimationFrame, UpdateAnimationFrame> pUpdates)
		{
			Boolean lRet = false;

			if (pFrames != null)
			{
				foreach (FileAnimationFrame lFrame in pFrames)
				{
					if (ShiftFrameBranchingTargets (lFrame, pStartIndex, pShift, ref pUpdates))
					{
						lRet = true;
					}
				}
			}
			return lRet;
		}

		public static Boolean ShiftFrameBranchingTargets (FileAnimationFrame pFrame, int pStartIndex, int pShift, ref Dictionary<FileAnimationFrame, UpdateAnimationFrame> pUpdates)
		{
			Boolean lRet = false;

			if ((pFrame != null) && (pShift != 0))
			{
				UpdateAnimationFrame lUpdate = pUpdates.ContainsKey (pFrame) ? pUpdates[pFrame] : null;
				FileFrameBranch[] lBranching = null;

				if ((lUpdate != null) && (lUpdate.Branching != null))
				{
					lBranching = new FileFrameBranch[lUpdate.Branching.Length];
				}
				else if (pFrame.Branching != null)
				{
					foreach (FileFrameBranch lBranch in pFrame.Branching)
					{
						if (lBranch.mFrameNdx >= pStartIndex)
						{
							lUpdate = pUpdates.ContainsKey (pFrame) ? pUpdates[pFrame] : new UpdateAnimationFrame (pFrame, false);
							lBranching = new FileFrameBranch[lUpdate.Branching.Length];
							break;
						}
					}
				}
				if ((lUpdate == null) && (pFrame.ExitFrame >= pStartIndex))
				{
					lUpdate = pUpdates.ContainsKey (pFrame) ? pUpdates[pFrame] : new UpdateAnimationFrame (pFrame, false);
				}

				if (lUpdate != null)
				{
					pUpdates[pFrame] = lUpdate;
#if DEBUG_NOT
					System.Diagnostics.Debug.Print ("    Put {0}", lUpdate.DebugString);
#endif
					if (lBranching != null)
					{
						for (int lNdx = 0; lNdx < lUpdate.Branching.Length; lNdx++)
						{
							lBranching[lNdx] = lUpdate.Branching[lNdx];

							if (lUpdate.Branching[lNdx].mFrameNdx >= pStartIndex)
							{
								lBranching[lNdx].mFrameNdx += (Int16)pShift;
								lRet = true;
							}
						}
						if (lRet)
						{
							lUpdate.Branching = lBranching;
						}
					}

					if (lUpdate.ExitFrame >= pStartIndex)
					{
						lUpdate.ExitFrame += (Int16)pShift;
						lRet = true;
					}
#if DEBUG_NOT
					System.Diagnostics.Debug.Print ("    Put {0}", lUpdate.DebugString);
#endif
				}
			}
			return lRet;
		}

		//=============================================================================

		public static String MergeFrameBranching (FileAnimationFrame pSource, FileAnimationFrame pTarget, ref Dictionary<FileAnimationFrame, UpdateAnimationFrame> pUpdates)
		{
			String lError = null;

			if ((pSource != null) && (pTarget != null))
			{
				UpdateAnimationFrame lUpdate = pUpdates.ContainsKey (pTarget) ? pUpdates[pTarget] : null;

				if ((pSource.ExitFrame >= 0) && (pTarget.ExitFrame < 0))
				{
					lUpdate = pUpdates.ContainsKey (pTarget) ? pUpdates[pTarget] : new UpdateAnimationFrame (pTarget, false);
				}
				else if ((pSource.Branching != null) && ((pTarget.Branching == null) || !pTarget.IsBranchingEqual (pSource.Branching)))
				{
					lUpdate = pUpdates.ContainsKey (pTarget) ? pUpdates[pTarget] : new UpdateAnimationFrame (pTarget, false);
				}

				if (lUpdate != null)
				{
					pUpdates[pTarget] = lUpdate;
#if DEBUG//_NOT
					System.Diagnostics.Debug.Print ("    Put {0}", lUpdate.DebugString);
#endif
					if (pSource.ExitFrame >= 0)
					{
						if (lUpdate.ExitFrame < 0)
						{
							lUpdate.ExitFrame = pSource.ExitFrame;
						}
						else if (lUpdate.ExitFrame != pSource.ExitFrame)
						{
							lError = "Exit frame conflict";
						}
					}
					if ((lError == null) && (pSource.Branching != null))
					{
						if (lUpdate.Branching == null)
						{
							lUpdate.Branching = pSource.CopyBranching ();
						}
						else
						{
							FileFrameBranch[] lBranching = new FileFrameBranch[pSource.Branching.Length + lUpdate.Branching.Length];
							int lRemainder = 100;
							int lSourceNdx;
							int lTargetNdx;
							int lUpdateNdx = 0;

							for (lSourceNdx = 0; lSourceNdx < pSource.Branching.Length; lSourceNdx++)
							{
								if (pSource.Branching[lSourceNdx].mFrameNdx >= 0)
								{
									lRemainder -= pSource.Branching[lSourceNdx].mProbability;
									lBranching[lUpdateNdx++] = pSource.Branching[lSourceNdx];
								}
							}
							for (lTargetNdx = 0; lTargetNdx < lUpdate.Branching.Length; lTargetNdx++)
							{
								if (lUpdate.Branching[lTargetNdx].mFrameNdx >= 0)
								{
									for (lSourceNdx = 0; lSourceNdx < lBranching.Length; lSourceNdx++)
									{
										if (lBranching[lSourceNdx].mFrameNdx == lUpdate.Branching[lTargetNdx].mFrameNdx)
										{
											lRemainder -= lUpdate.Branching[lTargetNdx].mProbability;
											lBranching[lSourceNdx].mProbability += lUpdate.Branching[lTargetNdx].mProbability;
											break;
										}
										else if (lSourceNdx == lUpdateNdx)
										{
											lRemainder -= lUpdate.Branching[lTargetNdx].mProbability;
											lBranching[lUpdateNdx++] = lUpdate.Branching[lTargetNdx];
											break;
										}
									}
								}
							}

							if (lUpdateNdx > 3)
							{
								lError = "Too many branches";
							}
							else if (lRemainder < 0)
							{
								lError = "Total probability > 100";
							}
							else
							{
								Array.Resize (ref lBranching, lUpdateNdx);
								lUpdate.Branching = lBranching;
							}
						}
					}
#if DEBUG//_NOT
					if (lError == null)
					{
						System.Diagnostics.Debug.Print ("    Put {0}", lUpdate.DebugString);
					}
					else
					{
						System.Diagnostics.Debug.Print ("    Error {0}", lError);
					}
#endif
				}
			}
			return lError;
		}
	}

	#endregion
	///////////////////////////////////////////////////////////////////////////////
	#region FrameImage

	internal class AddDeleteFrameImage : UndoableAddDelete<FileFrameImage>
	{
		public AddDeleteFrameImage (FileAnimationFrame pFrame, String pImageFilePath, Boolean pForClipboard)
			: base (new ResolveFrameImage (pFrame, pFrame.ImageCount), pForClipboard)
		{
			IsDelete = false;
			ImageFilePath = pImageFilePath;
		}

		public AddDeleteFrameImage (FileFrameImage pImage, Boolean pForClipboard)
			: base (new ResolveFrameImage (pImage), pForClipboard)
		{
			IsDelete = true;
			ImageFilePath = pImage.ImageFilePath;
		}

		public FileAnimation Animation
		{
			get
			{
				return (RawTarget as ResolveFrameImage).Frame.Animation.Target;
			}
		}
		public String AnimationName
		{
			get
			{
				return (RawTarget as ResolveFrameImage).Frame.Animation.AnimationName;
			}
		}
		public FileAnimationFrame Frame
		{
			get
			{
				return (RawTarget as ResolveFrameImage).Frame.Target;
			}
		}
		public int FrameNdx
		{
			get
			{
				return (RawTarget as ResolveFrameImage).Frame.FrameNdx;
			}
		}
		public int ImageNdx
		{
			get
			{
				return (RawTarget as ResolveFrameImage).ImageNdx;
			}
		}

		public String ImageFilePath
		{
			get;
			private set;
		}
		public override String TargetDescription
		{
			get
			{
				return Titles.ImageFrameAnimation (ImageNdx, Frame).Quoted ();
			}
		}

		public override UndoUnit Apply ()
		{
			AddDeleteFrameImage lApplied = null;
			FileAnimationFrame lFrame = Frame;
			FileFrameImage lTarget = Target;
#if DEBUG
			System.Diagnostics.Debug.Print ("Apply {0}", DebugString);
#endif
			if (IsDelete)
			{
				lApplied = new AddDeleteFrameImage (lTarget, IsForClipboard);
				if (lFrame.Images.Remove (lTarget))
				{
					(lApplied.RawTarget as ResolveFrameImage).TargetContained = false;
					lApplied.IsDelete = false;
					lApplied.IsRedo = !IsRedo;
				}
				else
				{
					lApplied = null;
				}
			}
			else
			{
				FileFrameImage lFrameImage = lFrame.Images.Add (ImageFilePath);

				if (lFrameImage != null)
				{
					lFrame.Images.Move (lFrameImage, ImageNdx);
					if (lTarget != null)
					{
						lTarget.CopyTo (lFrameImage);
					}
					lApplied = new AddDeleteFrameImage (lFrameImage, IsForClipboard);
					lApplied.IsDelete = true;
					lApplied.IsRedo = !IsRedo;
				}
			}
			if (lApplied != null)
			{
#if DEBUG
				System.Diagnostics.Debug.Print ("Apply {0}", lApplied.DebugString);
#endif
				return OnApplied (lApplied);
			}
			return null;
		}

#if DEBUG
		public override string DebugString
		{
			get
			{
				return String.Format ("{0}({1}) [{2} ({3}) ({4})]", ActionDescription, RedoString, AnimationName, FrameNdx, ImageNdx);
			}
		}
#endif
	}

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	internal class ReorderFrameImage : UndoableUpdate<FileFrameImage>
	{
		public ReorderFrameImage (FileFrameImage pImage, int pImageNdxTo)
			: base (new ResolveFrameImage (pImage))
		{
			ImageNdxFrom = Frame.Images.IndexOf (Target);
			ImageNdxTo = pImageNdxTo;
		}

		public FileAnimation Animation
		{
			get
			{
				return (RawTarget as ResolveFrameImage).Frame.Animation.Target;
			}
		}
		public String AnimationName
		{
			get
			{
				return (RawTarget as ResolveFrameImage).Frame.Animation.AnimationName;
			}
		}
		public FileAnimationFrame Frame
		{
			get
			{
				return (RawTarget as ResolveFrameImage).Frame.Target;
			}
		}
		public int FrameNdx
		{
			get
			{
				return (RawTarget as ResolveFrameImage).Frame.FrameNdx;
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
				return Titles.FrameAnimation (Frame).Quoted ();
			}
		}
		public override String ChangeDescription
		{
			get
			{
				return AppResources.Resources.UndoImageOrder;
			}
		}

		public override UndoUnit Apply ()
		{
			FileFrameImage lTarget = Target;
			int lSwap = lTarget.Container.IndexOf (lTarget);
#if DEBUG
			System.Diagnostics.Debug.Print ("Apply {0}", DebugString);
#endif
			if (lTarget.Container.Move (lTarget, ImageNdxTo))
			{
				ImageNdxFrom = lTarget.Container.IndexOf (lTarget);
				ImageNdxTo = lSwap;
#if DEBUG
				System.Diagnostics.Debug.Print ("Applied {0}", DebugString);
#endif
				return OnApplied (this);
			}
			return null;
		}

#if DEBUG
		public override string DebugString
		{
			get
			{
				return String.Format ("{0} [{1} ({2:D})] [{3:D} to {4:D}]", ChangeDescription, AnimationName, FrameNdx, ImageNdxFrom, ImageNdxTo);
			}
		}
#endif
	}

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	internal class UpdateFrameImage : UndoableUpdate<FileFrameImage>
	{
		public UpdateFrameImage (FileFrameImage pImage, Boolean pForClipboard)
			: base (new ResolveFrameImage (pImage), pForClipboard)
		{
			UndoObject = new FileFrameImage ();
			pImage.CopyTo (UndoObject);
		}

		public FileAnimation Animation
		{
			get
			{
				return (RawTarget as ResolveFrameImage).Frame.Animation.Target;
			}
		}
		public String AnimationName
		{
			get
			{
				return (RawTarget as ResolveFrameImage).Frame.Animation.AnimationName;
			}
		}
		public FileAnimationFrame Frame
		{
			get
			{
				return (RawTarget as ResolveFrameImage).Frame.Target;
			}
		}
		public int FrameNdx
		{
			get
			{
				return (RawTarget as ResolveFrameImage).Frame.FrameNdx;
			}
		}
		public int ImageNdx
		{
			get
			{
				return (RawTarget as ResolveFrameImage).ImageNdx;
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
			get
			{
				return UndoObject.ImageFilePath;
			}
			set
			{
				UndoObject.ImageFilePath = value;
			}
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
				return Titles.ImageFrameAnimation (Target).Quoted ();
			}
		}
		public override String ChangeDescription
		{
			get
			{
				if (IsForClipboard)
				{
					return base.ChangeDescription;
				}
				else
				{
					FileFrameImage lTarget = Target;
					return (lTarget.Offset != Offset) ? AppResources.Resources.UndoImageOffset : !String.Equals (lTarget.ImageFilePath, ImageFilePath) ? AppResources.Resources.UndoImageFilePath : String.Empty;
				}
			}
		}

		public override UndoUnit Apply ()
		{
			UndoUnit lApplied = null;
			FileFrameImage lTarget = Target;
#if DEBUG
			System.Diagnostics.Debug.Print ("Apply {0}", DebugString);
#endif
			if (!lTarget.Equals (UndoObject))
			{
				FileFrameImage lSwap = new FileFrameImage ();
				lTarget.CopyTo (lSwap);
				UndoObject.CopyTo (lTarget);
				UndoObject = lSwap;
				lApplied = this;
			}
			if (lApplied != null)
			{
				return OnApplied (lApplied);
			}
			return null;
		}
#if DEBUG
		public override String DebugString
		{
			get
			{
				return String.Format ("[{0} ({1:D}) ({2:D})] From [{3:D},{4:D}] [{5} ({6:D})] to [{7:D},{8:D}] [{9}]", AnimationName, FrameNdx, ImageNdx, Target.Offset.X, Target.Offset.Y, Target.ImageFilePath, Target.ImageNdx, Offset.X, Offset.Y, ImageFilePath);
			}
		}
#endif
	}

	#endregion
	///////////////////////////////////////////////////////////////////////////////
	#region FrameOverlay

	internal class AddDeleteFrameOverlay : UndoableAddDelete<FileFrameOverlay>
	{
		public AddDeleteFrameOverlay (FileAnimationFrame pFrame, MouthOverlay pOverlayType, String pOverlayImagePath, Boolean pForClipboard)
			: base (new ResolveFrameOverlay (pFrame, pOverlayType), pForClipboard)
		{
			IsDelete = false;
			OverlayImagePath = pOverlayImagePath;
		}
		public AddDeleteFrameOverlay (FileFrameOverlay pOverlay, Boolean pForClipboard)
			: base (new ResolveFrameOverlay (pOverlay), pForClipboard)
		{
			IsDelete = true;
			OverlayImagePath = Target.ImageFilePath;
		}

		public FileAnimation Animation
		{
			get
			{
				return (RawTarget as ResolveFrameOverlay).Frame.Animation.Target;
			}
		}
		public String AnimationName
		{
			get
			{
				return (RawTarget as ResolveFrameOverlay).Frame.Animation.AnimationName;
			}
		}
		public FileAnimationFrame Frame
		{
			get
			{
				return (RawTarget as ResolveFrameOverlay).Frame.Target;
			}
		}
		public int FrameNdx
		{
			get
			{
				return (RawTarget as ResolveFrameOverlay).Frame.FrameNdx;
			}
		}
		public MouthOverlay OverlayType
		{
			get
			{
				return (RawTarget as ResolveFrameOverlay).OverlayType;
			}
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
				return Titles.OverlayFrameAnimation (Frame, OverlayType).Quoted ();
			}
		}

		public override UndoUnit Apply ()
		{
			AddDeleteFrameOverlay lApplied = null;
			FileAnimationFrame lFrame = Frame;
			FileFrameOverlay lTarget = Target;
#if DEBUG
			System.Diagnostics.Debug.Print ("Apply {0}", DebugString);
#endif
			if (IsDelete)
			{
				if (lFrame.Overlays.Contains (lTarget))
				{
					lApplied = new AddDeleteFrameOverlay (lTarget, IsForClipboard);
					if (lFrame.Overlays.Remove (lTarget))
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
				FileFrameOverlay lFrameOverlay = lFrame.Overlays.Add (OverlayType, OverlayImagePath);

				if (lFrameOverlay != null)
				{
					if (lTarget != null)
					{
						lTarget.CopyTo (lFrameOverlay);
					}
					lApplied = new AddDeleteFrameOverlay (lFrameOverlay, IsForClipboard);
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

#if DEBUG
		public override string DebugString
		{
			get
			{
				return String.Format ("{0}({1}) [{2} ({3}) ({4})]", ActionDescription, RedoString, AnimationName, FrameNdx, OverlayType);
			}
		}
#endif
	}

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	internal class UpdateFrameOverlay : UndoableUpdate<FileFrameOverlay>
	{
		public UpdateFrameOverlay (FileFrameOverlay pOverlay, Boolean pForClipboard)
			: base (new ResolveFrameOverlay (pOverlay), pForClipboard)
		{
			UndoObject = new FileFrameOverlay ();
			pOverlay.CopyTo (UndoObject);
		}

		public FileAnimation Animation
		{
			get
			{
				return (RawTarget as ResolveFrameOverlay).Frame.Animation.Target;
			}
		}
		public String AnimationName
		{
			get
			{
				return (RawTarget as ResolveFrameOverlay).Frame.Animation.AnimationName;
			}
		}
		public FileAnimationFrame Frame
		{
			get
			{
				return (RawTarget as ResolveFrameOverlay).Frame.Target;
			}
		}
		public int FrameNdx
		{
			get
			{
				return (RawTarget as ResolveFrameOverlay).Frame.FrameNdx;
			}
		}
		public MouthOverlay OverlayType
		{
			get
			{
				return (RawTarget as ResolveFrameOverlay).OverlayType;
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
			get
			{
				return UndoObject.ImageFilePath;
			}
			set
			{
				UndoObject.ImageFilePath = value;
			}
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
				return Titles.OverlayFrameAnimation (Target).Quoted ();
			}
		}
		public override String ChangeDescription
		{
			get
			{
				if (IsForClipboard)
				{
					return base.ChangeDescription;
				}
				else
				{
					FileFrameOverlay lTarget = Target;
					return (lTarget.ReplaceFlag != ReplaceFlag) ? AppResources.Resources.UndoOverlayReplaceFlag : (lTarget.Offset != Offset) ? AppResources.Resources.UndoOverlayOffset : (!String.Equals (lTarget.ImageFilePath, OverlayImagePath)) ? AppResources.Resources.UndoOverlayImagePath : String.Empty;
				}
			}
		}

		public override UndoUnit Apply ()
		{
			UndoUnit lApplied = null;
			FileFrameOverlay lTarget = Target;
#if DEBUG
			System.Diagnostics.Debug.Print ("Apply {0}", DebugString);
#endif
			if (!lTarget.Equals (UndoObject))
			{
				FileFrameOverlay lSwap = new FileFrameOverlay ();
				lTarget.CopyTo (lSwap);
				UndoObject.CopyTo (lTarget);
				UndoObject = lSwap;
				lApplied = this;
			}
			if (lApplied != null)
			{
				return OnApplied (lApplied);
			}
			return null;
		}

#if DEBUG
		public override String DebugString
		{
			get
			{
				return String.Format ("[{0} ({1:D}) ({2:D})] From [{3:D},{4:D}] [{5}] [{6} ({7:D})] to [{8:D},{9:D}] [{10}] [{11}]", AnimationName, FrameNdx, OverlayType, Target.Offset.X, Target.Offset.Y, Target.ReplaceFlag, Target.ImageFilePath, Target.ImageNdx, Offset.X, Offset.Y, ReplaceFlag, OverlayImagePath);
			}
		}
#endif
	}

	#endregion
}

