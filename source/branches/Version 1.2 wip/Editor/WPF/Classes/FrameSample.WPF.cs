using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using AgentCharacterEditor.Global;

namespace AgentCharacterEditor
{
	class FrameSample : System.Windows.Controls.Image
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public FrameSample ()
		{
			InitializeComponent ();
		}

		protected void InitializeComponent ()
		{
			this.Stretch = System.Windows.Media.Stretch.Uniform;
			this.MinWidth = 16;
			this.MinHeight = 16;
			this.MaxWidth = MaximumImageSize.Width;
			this.MaxHeight = MaximumImageSize.Height;
			this.SnapsToDevicePixels = false;
			this.UseLayoutRounding = false;
			this.ClipToBounds = true;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

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

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Layout

		protected override Size MeasureOverride (Size constraint)
		{
			Size lRet;
			Size lSize;
			Point lMin = new Point (MinWidth, MinHeight);
			Point lMax = new Point (constraint.Width, constraint.Height);

			try
			{
				GeneralTransform lTransform = LayoutTransform.Inverse;
				lMin = lTransform.Transform (lMin);
				lMax = lTransform.Transform (lMax);
			}
			catch
			{
			}

			if (Source == null)
			{
				lSize = DefaultImageSize.ToWPF ().ScaleToScreenResolution ();
			}
			else
			{
				lSize = new Size (Source.Width, Source.Height);
			}

			lRet = new Size (Math.Min (Math.Max (lSize.Width, lMin.X), lMax.X), Math.Min (Math.Max (lSize.Height, lMin.Y), lMax.Y));
			lRet = lRet.PreserveAspectRatio (lSize);
#if DEBUG_NOT
			try
			{
				double lSourceWidth = (Source == null) ? 0 : Source.Width;
				double lSourceHeight = (Source == null) ? 0 : Source.Height;
				System.Diagnostics.Debug.Print ("{0} Measure [{1} {2}] Return [{3} {4}] Source [{5} {6}] Min [{7} {8}] max [{9} {10}] Transform [{11}]", Name, constraint.Width, constraint.Height, lRet.Width, lRet.Height, lSourceWidth, lSourceHeight, MinWidth, MinHeight, MaxWidth, MaxHeight, LayoutTransform.ToString ());
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

		#endregion
	}
}
