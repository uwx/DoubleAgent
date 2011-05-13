using System;
using System.Windows.Media.Animation;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Previews
{
	public class AnimationPreviewFrame : DiscreteObjectKeyFrame
	{
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

		public FileAnimationFrame FileFrame
		{
			get;
			protected set;
		}

		///////////////////////////////////////////////////////////////////////////////

		static public System.Windows.Media.ImageSource MakeImageSource (CharacterFile pCharacterFile, FileAnimationFrame pFrame)
		{
			if (pFrame != null)
			{
				try
				{
					return (MakeBitmapSource (FramesListView.GetFrameImage (pCharacterFile, pFrame)));
				}
				catch
				{
				}
			}
			return null;
		}

		static private System.Windows.Media.ImageSource MakeBitmapSource (System.Drawing.Bitmap pBitmap)
		{
			System.Windows.Media.ImageSource lImageSource = null;

			if (pBitmap != null)
			{
				try
				{
					System.Drawing.Imaging.BitmapData lBitmapData;
					lBitmapData = pBitmap.LockBits (new System.Drawing.Rectangle (0, 0, pBitmap.Width, pBitmap.Height), System.Drawing.Imaging.ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
					try
					{
						lImageSource = System.Windows.Media.Imaging.BitmapSource.Create (lBitmapData.Width, lBitmapData.Height, 96.0, 96.0, System.Windows.Media.PixelFormats.Bgra32, null, lBitmapData.Scan0, lBitmapData.Height * lBitmapData.Stride, lBitmapData.Stride);
					}
					catch
					{
					}
					pBitmap.UnlockBits (lBitmapData);
				}
				catch
				{
				}
			}
			return lImageSource;
		}
	}
}
