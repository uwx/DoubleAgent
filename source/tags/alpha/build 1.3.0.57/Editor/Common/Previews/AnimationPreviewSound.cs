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
using System.IO;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Previews
{
	public class AnimationPreviewSound : MediaTimeline
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		private System.IO.FileStream mTempFile = null;
#if DEBUG
		static private int mNextInstanceNum = 1;
		private int mInstanceNum = mNextInstanceNum++;
#endif

		public AnimationPreviewSound (CharacterFile pCharacterFile, FileAnimationFrame pFileFrame, TimeSpan? pBeginTime)
			: base (pBeginTime)
		{
			FileFrame = pFileFrame;
			Player = new System.Windows.Media.MediaPlayer ();

			Name = String.Format ("Sound{0}_{1}", pFileFrame.SoundNdx, pFileFrame.Container.IndexOf (pFileFrame));
			FillBehavior = FillBehavior.Stop;
			SetSourcePath (pCharacterFile, pFileFrame);
#if DEBUG
			System.Diagnostics.Debug.Print ("  Frame [{0}] Sound [{1}] at [{2}] Source [{3}]", pFileFrame.Container.IndexOf (pFileFrame), Name, BeginTime, Source);
#endif

			CurrentStateInvalidated += new EventHandler (OnCurrentStateInvalidated);
#if DEBUG_NOT
			Completed += new EventHandler (OnCompleted);
			CurrentGlobalSpeedInvalidated += new EventHandler (OnCurrentGlobalSpeedInvalidated);
#endif
#if DEBUG_NOT
			CurrentTimeInvalidated += new EventHandler (OnCurrentTimeInvalidated);
			RemoveRequested += new EventHandler (OnRemoveRequested);
#endif
#if DEBUG_NOT
			Player.BufferingStarted += new EventHandler (Player_BufferingStarted);
			Player.BufferingEnded += new EventHandler (Player_BufferingEnded);
			Player.Changed += new EventHandler (Player_Changed);
			Player.MediaOpened += new EventHandler (Player_MediaOpened);
			Player.MediaEnded += new EventHandler (Player_MediaEnded);
			Player.MediaFailed += new EventHandler<ExceptionEventArgs> (Player_MediaFailed);
#endif
		}

		private AnimationPreviewSound ()
		{
			//
			//	No event trapping on frozen clones - apparently frozen clones not used.
			//
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public FileAnimationFrame FileFrame
		{
			get;
			protected set;
		}
		public System.Windows.Media.MediaPlayer Player
		{
			get;
			protected set;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		private Boolean SetSourcePath (CharacterFile pCharacterFile, FileAnimationFrame pFileFrame)
		{
			String lSoundFilePath = pCharacterFile.GetSoundFilePath (pFileFrame.SoundNdx);

			if (String.IsNullOrEmpty (lSoundFilePath))
			{
				try
				{
					Byte[] lSound = pCharacterFile.GetSound (pFileFrame.SoundNdx);

					if ((lSound != null) && lSound.Length > 0)
					{
						mTempFile = new FileStream (Path.GetTempFileName(), FileMode.Create, FileAccess.ReadWrite, FileShare.None, 8, FileOptions.DeleteOnClose);
						mTempFile = new FileStream (Path.ChangeExtension (mTempFile.Name, ".wav"), FileMode.Create, FileAccess.ReadWrite, (FileShare.ReadWrite | FileShare.Delete));
						mTempFile.Write (lSound, 0, lSound.Length);
						mTempFile.Flush ();
						mTempFile = new FileStream (mTempFile.Name, FileMode.Open, FileAccess.Read, (FileShare.ReadWrite | FileShare.Delete), 8, FileOptions.DeleteOnClose);
						lSoundFilePath = mTempFile.Name;
					}
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
			}
			if (!String.IsNullOrEmpty (lSoundFilePath))
			{
				SetValue (SourceProperty, SoundFileUri (lSoundFilePath));
				return true;
			}
			return false;
		}

		static private System.Uri SoundFileUri (String pSoundFilePath)
		{
			UriBuilder lUriBuilder = new UriBuilder (Uri.UriSchemeFile, String.Empty);
			lUriBuilder.Path = pSoundFilePath;
			return lUriBuilder.Uri;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Infrastructure

		//
		//	Note - mTempFile is not copied to clones
		//

		protected override System.Windows.Freezable CreateInstanceCore ()
		{
			return new AnimationPreviewSound ();
		}

		public new AnimationPreviewSound Clone ()
		{
			AnimationPreviewSound lClone = CreateInstanceCore () as AnimationPreviewSound;
			lClone.CloneCore (this);
			return lClone;
		}

		protected override void CloneCore (Freezable sourceFreezable)
		{
			base.CloneCore (sourceFreezable);
			if (sourceFreezable is AnimationPreviewSound)
			{
				AnimationPreviewSound lSource = (sourceFreezable as AnimationPreviewSound);
				this.FileFrame = lSource.FileFrame;
				this.Player = lSource.Player;
			}
		}

		protected override void CloneCurrentValueCore (System.Windows.Freezable sourceFreezable)
		{
			base.CloneCurrentValueCore (sourceFreezable);
			if (sourceFreezable is AnimationPreviewSound)
			{
				AnimationPreviewSound lSource = (sourceFreezable as AnimationPreviewSound);
				this.FileFrame = lSource.FileFrame;
				this.Player = lSource.Player;
			}
		}

		protected override void GetAsFrozenCore (System.Windows.Freezable source)
		{
			base.GetAsFrozenCore (source);
			if (source is AnimationPreviewSound)
			{
				AnimationPreviewSound lSource = (source as AnimationPreviewSound);
				this.FileFrame = lSource.FileFrame;
				this.Player = lSource.Player;
			}
		}

		protected override void GetCurrentValueAsFrozenCore (System.Windows.Freezable source)
		{
			base.GetCurrentValueAsFrozenCore (source);
			if (source is AnimationPreviewSound)
			{
				AnimationPreviewSound lSource = (source as AnimationPreviewSound);
				this.FileFrame = lSource.FileFrame;
				this.Player = lSource.Player;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void OnCurrentStateInvalidated (object sender, EventArgs e)
		{
			MediaClock lClock = sender as MediaClock;
#if DEBUG_NOT
			try
			{
				System.Diagnostics.Debug.Print ("[{0}] {1}(S{2}) State [{3}] Frozen [{4}] Duration [{5}] Source [{6}]", lClock.Parent.CurrentTime, Name, mInstanceNum, lClock.CurrentState, IsFrozen, GetNaturalDuration (lClock), Source);
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
#endif
			try
			{
				if (lClock.CurrentState == ClockState.Active)
				{
					Player.Clock = lClock;
				}
				else if (lClock.CurrentState == ClockState.Stopped)
				{
					Player.Clock = null;
				}
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
		}

		void OnCurrentGlobalSpeedInvalidated (object sender, EventArgs e)
		{
#if DEBUG
			try
			{
				MediaClock lClock = sender as MediaClock;
				if (lClock.CurrentGlobalSpeed.HasValue)
				{
					System.Diagnostics.Debug.Print ("[{0}] {1}(S{2}) Speed [{3}]", lClock.Parent.CurrentTime, Name, mInstanceNum, lClock.CurrentGlobalSpeed);
				}
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
#endif
		}

		private void OnCurrentTimeInvalidated (object sender, EventArgs e)
		{
#if DEBUG
			try
			{
				MediaClock lClock = sender as MediaClock;
				if (lClock.CurrentTime.HasValue)
				{
					System.Diagnostics.Debug.Print ("[{0}] {1}(S{2}) Tick Duration [{3}]", lClock.Parent.CurrentTime, Name, mInstanceNum, GetNaturalDuration (lClock));
				}
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
#endif
		}

		private void OnCompleted (object sender, EventArgs e)
		{
#if DEBUG
			try
			{
				MediaClock lClock = sender as MediaClock;
				System.Diagnostics.Debug.Print ("[{0}] {1}(S{2}) Completed [{3}]", lClock.Parent.CurrentTime, Name, mInstanceNum, lClock.CurrentState);
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
#endif
		}

		private void OnRemoveRequested (object sender, EventArgs e)
		{
#if DEBUG
			System.Diagnostics.Debug.Print ("{0}(S{1}) Remove", Name, mInstanceNum);
#endif
		}

		//=============================================================================

#if DEBUG
		void Player_MediaOpened (object sender, EventArgs e)
		{
			try
			{
				TimeSpan? lTime = ((Player != null) && (Player.Clock != null)) ? Player.Clock.CurrentTime : null;
				System.Diagnostics.Debug.Print ("[{0}] {1}(S{2}) MediaOpened", lTime, Name, mInstanceNum);
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
		}

		void Player_MediaEnded (object sender, EventArgs e)
		{
			try
			{
				TimeSpan? lTime = ((Player != null) && (Player.Clock != null)) ? Player.Clock.CurrentTime : null;
				System.Diagnostics.Debug.Print ("[{0}] {1}(S{2}) MediaEnded", lTime, Name, mInstanceNum);
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
		}

		void Player_MediaFailed (object sender, ExceptionEventArgs e)
		{
			try
			{
				TimeSpan? lTime = ((Player != null) && (Player.Clock != null)) ? Player.Clock.CurrentTime : null;
				System.Diagnostics.Debug.Print ("{1}(S{2}) MediaFailed [{2}]", lTime, Name, mInstanceNum, e.ErrorException);
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
		}

		void Player_Changed (object sender, EventArgs e)
		{
			try
			{
				TimeSpan? lTime = ((Player != null) && (Player.Clock != null)) ? Player.Clock.CurrentTime : null;
				System.Diagnostics.Debug.Print ("{1}(S{2}) PlayerChanged", lTime, Name, mInstanceNum);
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
		}

		void Player_BufferingEnded (object sender, EventArgs e)
		{
			try
			{
				TimeSpan? lTime = ((Player != null) && (Player.Clock != null)) ? Player.Clock.CurrentTime : null;
				System.Diagnostics.Debug.Print ("{1}(S{2}) BufferingEnded", lTime, Name, mInstanceNum);
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
		}

		void Player_BufferingStarted (object sender, EventArgs e)
		{
			try
			{
				TimeSpan? lTime = ((Player != null) && (Player.Clock != null)) ? Player.Clock.CurrentTime : null;
				System.Diagnostics.Debug.Print ("{1}(S{2}) BufferingStarted", lTime, Name, mInstanceNum);
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
		}
#endif

		#endregion
	}
}
