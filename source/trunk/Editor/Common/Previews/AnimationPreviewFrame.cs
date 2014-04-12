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
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Animation;
using DoubleAgent.Character;
using AgentCharacterEditor.Global;

namespace AgentCharacterEditor.Previews
{
	public class AnimationPreviewFrame : DiscreteObjectKeyFrame
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public AnimationPreviewFrame (CharacterFile pCharacterFile, FileAnimationFrame pFileFrame, TimeSpan pFrameTime)
			: base (MakeImageSource (pCharacterFile, pFileFrame), KeyTime.FromTimeSpan (pFrameTime))
		{
			FileFrame = pFileFrame;
		}

		public AnimationPreviewFrame (System.Windows.Media.ImageSource pImageSource, FileAnimationFrame pFileFrame, TimeSpan pFrameTime)
			: base (pImageSource, KeyTime.FromTimeSpan (pFrameTime))
		{
			FileFrame = pFileFrame;
		}

		private AnimationPreviewFrame ()
		{
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public FileAnimationFrame FileFrame
		{
			get;
			protected set;
		}
		public TimeSpan BeginTime
		{
			get
			{
				return KeyTime.TimeSpan;
			}
		}
		public TimeSpan EndTime
		{
			get
			{
				return (FileFrame == null) ? KeyTime.TimeSpan : KeyTime.TimeSpan+TimeSpan.FromMilliseconds (FileFrame.Duration*10);
			}
		}
		public System.Windows.Media.ImageSource Image
		{
			get
			{
				return Value as ImageSource;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		static public System.Windows.Media.ImageSource MakeImageSource (CharacterFile pCharacterFile, FileAnimationFrame pFrame)
		{
			if (pFrame != null)
			{
				try
				{
					return FramesListView.GetFrameImage (pCharacterFile, pFrame).MakeImageSource ();
				}
				catch
				{
				}
			}
			return null;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Infrastructure

		protected override System.Windows.Freezable CreateInstanceCore ()
		{
			return new AnimationPreviewFrame ();
		}

		public new AnimationPreviewFrame Clone ()
		{
			AnimationPreviewFrame lClone = CreateInstanceCore () as AnimationPreviewFrame;
			lClone.CloneCore (this);
			return lClone;
		}

		protected override void CloneCore (Freezable sourceFreezable)
		{
			base.CloneCore (sourceFreezable);
			if (sourceFreezable is AnimationPreviewFrame)
			{
				AnimationPreviewFrame lSource = (sourceFreezable as AnimationPreviewFrame);
				this.FileFrame = lSource.FileFrame;
			}
		}

		protected override void CloneCurrentValueCore (System.Windows.Freezable sourceFreezable)
		{
			base.CloneCurrentValueCore (sourceFreezable);
			if (sourceFreezable is AnimationPreviewFrame)
			{
				AnimationPreviewFrame lSource = (sourceFreezable as AnimationPreviewFrame);
				this.FileFrame = lSource.FileFrame;
			}
		}

		//=============================================================================

		protected override void GetAsFrozenCore (System.Windows.Freezable source)
		{
			base.GetAsFrozenCore (source);
			if (source is AnimationPreviewFrame)
			{
				AnimationPreviewFrame lSource = (source as AnimationPreviewFrame);
				this.FileFrame = lSource.FileFrame;
			}
		}

		protected override void GetCurrentValueAsFrozenCore (System.Windows.Freezable source)
		{
			base.GetCurrentValueAsFrozenCore (source);
			if (source is AnimationPreviewFrame)
			{
				AnimationPreviewFrame lSource = (source as AnimationPreviewFrame);
				this.FileFrame = lSource.FileFrame;
			}
		}

		#endregion
	}
}
