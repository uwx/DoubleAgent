using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace AgentCharacterEditor
{
	class FrameSample : System.Windows.Controls.Image
	{
		public FrameSample ()
		{
			InitializeComponent ();
		}

		protected void InitializeComponent ()
		{
			this.Stretch = System.Windows.Media.Stretch.Uniform;
			this.MinWidth = 16;
			this.MinHeight = 16;
			this.MaxWidth = 256;
			this.MaxHeight = 256;
		}

		protected override Size MeasureOverride (Size constraint)
		{
			Size lRet = new Size ();
			if (Source == null)
			{
				lRet.Width = Math.Min (Math.Max (DefaultImageSize.Width, MinWidth), constraint.Width);
				lRet.Height = Math.Min (Math.Max (DefaultImageSize.Height, MinHeight), constraint.Height);
			}
			else
			{
				lRet.Width = Math.Min (Math.Max (Source.Width, MinWidth), constraint.Width);
				lRet.Height = Math.Min (Math.Max (Source.Height, MinHeight), constraint.Height);
			}
#if DEBUG_NOT
			try
			{
				double lSourceWidth = (Source == null) ? 0 : Source.Width;
				double lSourceHeight = (Source == null) ? 0 : Source.Height;
				System.Diagnostics.Debug.Print ("{0} Measure [{1} {2}] return [{3} {4}] source [{5} {6}] min [{7} {8}] max [{9} {10}]", Name, constraint.Width, constraint.Height, lRet.Width, lRet.Height, lSourceWidth, lSourceHeight, MinWidth, MinHeight, MaxWidth, MaxHeight);
			}
			catch
			{
			}
#endif
			return lRet;
		}

		protected override Size ArrangeOverride (Size arrangeSize)
		{
			Size lRet = base.ArrangeOverride (arrangeSize);
#if DEBUG_NOT
			try
			{
				double lSourceWidth = (Source == null) ? 0 : Source.Width;
				double lSourceHeight = (Source == null) ? 0 : Source.Height;
				System.Diagnostics.Debug.Print ("{0} Arrange [{1} {2}] return [{3} {4}] source [{5} {6}] min [{7} {8}] max [{9} {10}]", Name, arrangeSize.Width, arrangeSize.Height, lRet.Width, lRet.Height, lSourceWidth, lSourceHeight, MinWidth, MinHeight, MaxWidth, MaxHeight);
			}
			catch
			{
			}
#endif
			return lRet;
		}

		///////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// The suggested default dimensions of a sample image.
		/// </summary>
		public static System.Drawing.Size DefaultImageSize
		{
			get
			{
				return new System.Drawing.Size (128, 128);
			}
		}

		/// <summary>
		/// The suggested maximum dimensions of a sample image.
		/// </summary>
		public static System.Drawing.Size MaximumImageSize
		{
			get
			{
				return new System.Drawing.Size (256, 256);
			}
		}
	}
}
