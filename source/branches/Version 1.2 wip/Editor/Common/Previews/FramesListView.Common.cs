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
using System.Threading;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Previews
{
	public partial class FramesListView
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public CharacterFile CharacterFile
		{
			get;
			set;
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public FileAnimation Animation
		{
			get;
			set;
		}

		//=============================================================================

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		internal FrameImageWorker ImageWorker
		{
			get
			{
				if (mImageWorker == null)
				{
					mImageWorker = new FrameImageWorker (CharacterFile);
					mImageWorker.ProgressChanged += new ProgressChangedEventHandler (ImageWorker_ProgressChanged);
				}
				return mImageWorker;
			}
		}
		private FrameImageWorker mImageWorker = null;

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		public void ShowAnimationFrames (CharacterFile pCharacterFile, FileAnimation pAnimation)
		{
			if (mImageWorker != null)
			{
				mImageWorker.Reset (pCharacterFile);
			}
			CharacterFile = pCharacterFile;
			Animation = pAnimation;
#if DEBUG_NOT
			if (Animation != null)
			{
				System.Diagnostics.Debug.Print ("ShowAnimationFrames [{0}]", Animation.Name);
			}
#endif
			ShowAnimationFrames ();
#if DEBUG_NOT
			if (Animation != null)
			{
				System.Diagnostics.Debug.Print ("ShowAnimationFrames [{0}] Done", Animation.Name);
			}
#endif
		}

		//=============================================================================

		public System.Drawing.Bitmap GetFrameImage (FileAnimationFrame pFrame)
		{
			System.Drawing.Bitmap lImage = null;

			if (pFrame != null)
			{
				if (HasFrameImage (pFrame))
				{
					lImage = GetFrameImageAsync (pFrame, false);
				}
				if (lImage == null)
				{
					lImage = GetFrameImage (CharacterFile, pFrame);
				}
			}
			return lImage;
		}

		public System.Drawing.Bitmap GetFrameImageAsync (FileAnimationFrame pFrame)
		{
			return GetFrameImageAsync (pFrame, true);
		}
		public System.Drawing.Bitmap GetFrameImageAsync (FileAnimationFrame pFrame, Boolean pAllowPlaceholder)
		{
			System.Drawing.Bitmap lImage = null;

			try
			{
				lImage = ImageWorker.GetFrameImage (pFrame, pAllowPlaceholder);
			}
			catch
			{
			}
			return lImage;
		}

		private void ImageWorker_ProgressChanged (object sender, ProgressChangedEventArgs e)
		{
			try
			{
				FrameImageChanged (e.UserState as FileAnimationFrame);
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		static public Boolean HasFrameImage (FileAnimationFrame pFrame)
		{
			return (pFrame != null) && (pFrame.ImageCount > 0);
		}

		static public System.Drawing.Bitmap GetFrameImage (CharacterFile pCharacterFile, FileAnimationFrame pFrame)
		{
			System.Drawing.Bitmap lImage = null;

			if ((pCharacterFile != null) && (pFrame != null))
			{
				try
				{
					if (pFrame.ImageCount > 0)
					{
						lImage = pCharacterFile.GetFrameBitmap (pFrame, true, System.Drawing.Color.Transparent);
					}
					else
					{
						lImage = new System.Drawing.Bitmap (pCharacterFile.Header.ImageSize.Width, pCharacterFile.Header.ImageSize.Height);
						System.Drawing.Graphics lGraphics = System.Drawing.Graphics.FromImage (lImage);
						System.Drawing.Pen lPen = new System.Drawing.Pen (System.Drawing.Color.Pink, Math.Max (pCharacterFile.Header.ImageSize.Width, pCharacterFile.Header.ImageSize.Height) / 30.0f);

						lGraphics.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.None;
						lGraphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;

						lGraphics.DrawRectangle (lPen, 3.0f, 3.0f, (float)lImage.Width - 7.0f, (float)lImage.Height - 7.0f);
						lGraphics.DrawLine (lPen, 3.0f, 3.0f, (float)lImage.Width - 4.0f, (float)lImage.Height - 4.0f);
						lGraphics.DrawLine (lPen, 3.0f, (float)lImage.Height - 4.0f, (float)lImage.Width - 4.0f, 3.0f);
						lGraphics.Dispose ();
					}
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
			}
			return lImage;
		}

		#endregion
	}

	///////////////////////////////////////////////////////////////////////////////

	internal partial class FrameImageWorker : BackgroundWorker
	{
		private Queue<FileAnimationFrame> mFrames = null;
		private Dictionary<FileAnimationFrame, System.Drawing.Bitmap> mImages = null;
		private int mWaitForWork = 0;
		private Object mLock = new Object ();

		public FrameImageWorker (CharacterFile pCharacterFile)
		{
			WorkerReportsProgress = true;
			WorkerSupportsCancellation = true;
			Reset (pCharacterFile);
		}

		public CharacterFile CharacterFile
		{
			get;
			protected set;
		}

		public int FrameCount
		{
			get
			{
				int lRet = 0;
				lock (mLock)
				{
					try
					{
						lRet = mFrames.Count;
					}
					catch
					{
					}
				}
				return lRet;
			}
		}

		//=============================================================================

		public void Reset (CharacterFile pCharacterFile)
		{
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("ResetAsyncImages");
#endif
			lock (mLock)
			{
				try
				{
					mFrames = new Queue<FileAnimationFrame> ();
					mImages = new Dictionary<FileAnimationFrame, System.Drawing.Bitmap> ();
					CharacterFile = pCharacterFile;
				}
				catch
				{
				}
			}
		}

		//=============================================================================

		public System.Drawing.Bitmap GetFrameImage (FileAnimationFrame pFrame)
		{
			return GetFrameImage (pFrame, false);
		}
		public System.Drawing.Bitmap GetFrameImage (FileAnimationFrame pFrame, Boolean pAllowPlaceholder)
		{
			System.Drawing.Bitmap lImage = null;

			lock (mLock)
			{
				try
				{
					if ((pFrame != null) && mImages.ContainsKey (pFrame))
					{
						lImage = mImages[pFrame];
					}
				}
				catch
				{
				}
			}

			if (lImage == null)
			{
				MakeFrameImage (pFrame);
			}

			if ((lImage == null) && pAllowPlaceholder && (CharacterFile != null))
			{
				try
				{
					lImage = new System.Drawing.Bitmap (CharacterFile.Header.ImageSize.Width, CharacterFile.Header.ImageSize.Height);
					System.Drawing.Graphics lGraphics = System.Drawing.Graphics.FromImage (lImage);
					System.Drawing.Pen lPen = new System.Drawing.Pen (System.Drawing.Color.SkyBlue, Math.Max (CharacterFile.Header.ImageSize.Width, CharacterFile.Header.ImageSize.Height) / 30.0f);

					lGraphics.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.None;
					lGraphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;

					lGraphics.DrawRectangle (lPen, 3.0f, 3.0f, (float)lImage.Width - 7.0f, (float)lImage.Height - 7.0f);
					lGraphics.DrawLine (lPen, 3.0f, 3.0f, (float)lImage.Width - 4.0f, (float)lImage.Height - 4.0f);
					lGraphics.DrawLine (lPen, 3.0f, (float)lImage.Height - 4.0f, (float)lImage.Width - 4.0f, 3.0f);
					lGraphics.Dispose ();
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
			}
			return lImage;
		}

		public Boolean MakeFrameImage (FileAnimationFrame pFrame)
		{
			return MakeFrameImage (pFrame, false);
		}
		public Boolean MakeFrameImage (FileAnimationFrame pFrame, Boolean pRefresh)
		{
			Boolean lRet = false;

			if (pFrame != null)
			{
				lock (mLock)
				{
					try
					{
						if (pRefresh && (mImages.ContainsKey (pFrame)))
						{
							mImages.Remove (pFrame);
						}
						if (pRefresh || !mFrames.Contains (pFrame))
						{
							mFrames.Enqueue (pFrame);
						}
					}
					catch
					{
					}
				}

				if (FrameCount > 0)
				{
#if DEBUG
					Boolean lThreadEnding = false;
					if (IsBusy && WaitForWork <= 0)
					{
						System.Diagnostics.Debug.Print ("!!! Worker thread ending [{0}]", pFrame);
						lThreadEnding = true;
					}
#endif

					WaitForWork = 100;
#if DEBUG_NOT
					System.Diagnostics.Debug.Print ("MakeAsyncImages [{0}] [{1}]", FrameCount, WaitForWork);
#endif
					try
					{
						RunWorkerAsync ();
					}
#if DEBUG
					catch (Exception pException)
					{
						if (lThreadEnding)
						{
							System.Diagnostics.Debug.Print (pException.Message);
						}
					}
					//#if DEBUG_NOT
					//catch (Exception pException)
					//{
					//    System.Diagnostics.Debug.Print (pException.Message);
					//}
#else
					catch
					{
					}
#endif
				}
			}
			return lRet;
		}

		///////////////////////////////////////////////////////////////////////////////

		private int WaitForWork
		{
			get
			{
				int lRet = 0;
				lock (mLock)
				{
					lRet = mWaitForWork;
				}
				return lRet;
			}
			set
			{
				lock (mLock)
				{
					mWaitForWork = value;
				}
			}
		}

		protected override void OnDoWork (DoWorkEventArgs e)
		{
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("  StartAsyncImages [{0}] [{1}]", FrameCount, WaitForWork);
#endif
			while (WaitForWork-- > 0 && !e.Cancel)
			{
				FileAnimationFrame lFrame = null;
				System.Drawing.Bitmap lBitmap = null;
				int lProgress = 0;

				lock (mLock)
				{
					try
					{
						lFrame = (mFrames.Count > 0) ? mFrames.Dequeue () : null;
						lProgress = mFrames.Count;
					}
					catch
					{
					}
				}

				if (CancellationPending)
				{
					e.Cancel = true;
					break;
				}
				else if (lFrame == null)
				{
					Thread.Sleep (10);
					continue;
				}
				else
				{
					try
					{
						lBitmap = FramesListView.GetFrameImage (CharacterFile, lFrame);
					}
					catch (Exception pException)
					{
						System.Diagnostics.Debug.Print (pException.Message);
					}
				}

				if (CancellationPending)
				{
					e.Cancel = true;
					break;
				}
				else if (lBitmap != null)
				{
					lock (mLock)
					{
						try
						{
							mImages[lFrame] = lBitmap;
#if DEBUG_NOT
							System.Diagnostics.Debug.Print ("    PutAsyncImage [{0}] [{1}]", lProgress, lFrame);
#endif
						}
						catch
						{
						}
					}
					try
					{
						ReportProgress (lProgress, lFrame);
					}
					catch (Exception pException)
					{
						System.Diagnostics.Debug.Print (pException.Message);
					}
				}
			}
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("  EndAsyncImages [{0}] [{1}] [{2}]", FrameCount, WaitForWork, e.Cancel);
#endif
		}
	}
}
