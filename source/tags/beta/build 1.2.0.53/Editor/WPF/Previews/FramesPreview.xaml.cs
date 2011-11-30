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
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Media;
using AgentCharacterEditor.Global;

namespace AgentCharacterEditor.Previews
{
	public partial class FramesPreview : ScrollViewer
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public FramesPreview ()
		{
			InitializeComponent ();
			Range.ListView = Frames;
			InitializeCommon ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		public void BuildBranchingGraphs ()
		{
			BuildBranchingGraphs ((int)this.FontSize);
		}

		public void RecalcLayout ()
		{
			RecalcLayout (ImageScale);
		}

		public void RecalcLayout (ImageScaleType pImageScale)
		{
			Thickness lFramesMargin = new Thickness (0, 4, 0, 4);

			ImageScale = pImageScale;
			ImageSize = GetImageScaleSize (pImageScale);
			Frames.ImageSize = ImageSize.ToWPF ();
			Frames.UpdateLayout ();

			try
			{
				if (mBranchingVisual != null)
				{
					RemoveVisualChild (mBranchingVisual);
				}
				if ((mBranchingVisual = DrawBranching ()) != null)
				{
					mBranchingVisual.Offset = new Vector (mBranchingVisual.Offset.X, mBranchingVisual.Offset.Y + lFramesMargin.Top);
					AddVisualChild (mBranchingVisual);
					lFramesMargin.Top += mBranchingVisual.ContentBounds.Height;
				}
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}

			try
			{
				if (mExitBranchingVisual != null)
				{
					RemoveVisualChild (mExitBranchingVisual);
				}
				if ((mExitBranchingVisual = DrawExitBranching ()) != null)
				{
					mExitBranchingVisual.Offset = new Vector (mExitBranchingVisual.Offset.X, mExitBranchingVisual.Offset.Y + lFramesMargin.Top + Frames.ActualHeight);
					AddVisualChild (mExitBranchingVisual);
					lFramesMargin.Bottom += mExitBranchingVisual.ContentBounds.Height;
				}
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}

			Frames.Margin = lFramesMargin;

#if DEBUG
			if (Range.RecalcLayout ())
			{
				Range.Visibility = Visibility.Visible;
			}
			else
#endif
			{
				Range.Visibility = Visibility.Collapsed;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Drawing

		private DrawingVisual DrawBranching ()
		{
			DrawingVisual lVisual = null;

			if (ShowBranching && (mBranching.Length > 0))
			{
				Dictionary<BranchingItem, Point> lLineCenter = new Dictionary<BranchingItem, Point> ();

				using (DrawingStuff lDrawingStuff = new DrawingStuff (Frames, lVisual = new DrawingVisual ()))
				{

					foreach (BranchingItemP lBranching in mBranching)
					{
						lLineCenter[lBranching] = DrawBranchingLine (-1, lBranching, lDrawingStuff);
					}
					foreach (BranchingItemP lBranching in mBranching)
					{
						DrawLineLabel (lBranching.Label, lLineCenter[lBranching], lBranching, lDrawingStuff);
					}
				}

				lVisual.Offset = new Vector (-HorizontalOffset, -lVisual.ContentBounds.Top);
			}

			return lVisual;
		}

		private DrawingVisual DrawExitBranching ()
		{
			DrawingVisual lVisual = null;

			if (ShowExitBranching && (mExitBranching.Length > 0))
			{
				Dictionary<BranchingItem, Point> lLineCenter = new Dictionary<BranchingItem, Point> ();

				using (DrawingStuff lDrawingStuff = new DrawingStuff (Frames, lVisual = new DrawingVisual ()))
				{

					foreach (BranchingItem lBranching in mExitBranching)
					{
						lLineCenter[lBranching] = DrawBranchingLine (1, lBranching, lDrawingStuff);
					}
					foreach (BranchingItem lBranching in mExitBranching)
					{
						DrawLineLabel (lBranching.Label, lLineCenter[lBranching], lBranching, lDrawingStuff);
					}
				}

				lVisual.Offset = new Vector (-HorizontalOffset, -lVisual.ContentBounds.Top);
			}

			return lVisual;
		}

		//=============================================================================

		private class DrawingStuff : IDisposable
		{
			public DrawingVisual DrawingVisual;
			public DrawingContext DrawingContext;
			public Brush BackgroundBrush;
			public Brush ForegroundBrush;
			public Brush ErrorBrush;
			public Pen BackgroundPen;
			public Pen ForegroundPen;
			public Pen ErrorPen;
			public Size ArrowSize = new Size (2, 4);

			public DrawingStuff (FramesListView pFrames, DrawingVisual pVisual)
			{
				DrawingVisual = pVisual;
				DrawingContext = pVisual.RenderOpen ();

				BackgroundBrush = pFrames.Background;
				ForegroundBrush = pFrames.Foreground;
				ErrorBrush = Brushes.Red;
				BackgroundPen = new Pen (BackgroundBrush, 5.0);
				ForegroundPen = new Pen (ForegroundBrush, 0.5);
				ForegroundPen.StartLineCap = PenLineCap.Flat;
				ForegroundPen.EndLineCap = PenLineCap.Flat;
				ErrorPen = new Pen (ErrorBrush, 0.5);
				ErrorPen.StartLineCap = PenLineCap.Flat;
				ErrorPen.EndLineCap = PenLineCap.Flat;
			}
			~DrawingStuff ()
			{
				Dispose ();
			}
			public void Dispose ()
			{
				DrawingContext.Close ();
				GC.SuppressFinalize (this);
			}
		}

		//=============================================================================

		private Point DrawBranchingLine (Double pOffsetY, BranchingItem pBranching, DrawingStuff pDrawingStuff)
		{
			Point lCenterPoint = new Point ();
			//TODO - invalid branching targets
			try
			{
				FramesListItem lSrcItem = Frames.Items[pBranching.SrcFrameNdx] as FramesListItem;
				PathGeometry lLineGeometry = new PathGeometry ();
				PolyLineSegment lLineSegment = new PolyLineSegment ();
				PathFigure lLineFigure = new PathFigure ();
				PolyLineSegment lArrowSegment = new PolyLineSegment ();
				PathFigure lArrowFigure = new PathFigure ();

				if (pBranching.IsValid)
				{
					FramesListItem lDstItem = Frames.Items[pBranching.DstFrameNdx] as FramesListItem;

					lLineFigure.StartPoint = new Point ((lSrcItem.ActualWidth / 2.0) + (Double)(pBranching.SrcOffset * mBranchingOffset.Width), 0);
					lLineSegment.Points.Add (new Point ((lDstItem.ActualWidth / 2.0) + (Double)(pBranching.DstOffset * mBranchingOffset.Width), 0));
					lLineFigure.StartPoint = lSrcItem.TranslatePoint (lLineFigure.StartPoint, this);
					lLineSegment.Points[0] = lDstItem.TranslatePoint (lLineSegment.Points[0], this);
					lLineFigure.StartPoint = new Point (lLineFigure.StartPoint.X + HorizontalOffset, lLineFigure.StartPoint.Y + VerticalOffset);
					lLineSegment.Points[0] = new Point (lLineSegment.Points[0].X + HorizontalOffset, lLineSegment.Points[0].Y + VerticalOffset);
					lLineSegment.Points.Insert (0, new Point (lLineFigure.StartPoint.X, lLineFigure.StartPoint.Y + (pBranching.Height * mBranchingOffset.Height * pOffsetY)));
					lLineSegment.Points.Insert (1, new Point (lLineSegment.Points[1].X, lLineSegment.Points[0].Y));

					lCenterPoint.X = (lLineSegment.Points[0].X + lLineSegment.Points[1].X) / 2.0;
					lCenterPoint.Y = lLineSegment.Points[0].Y;
				}
				else
				{
					lLineFigure.StartPoint = new Point ((lSrcItem.ActualWidth / 2.0) + (Double)(pBranching.SrcOffset * mBranchingOffset.Width), 0);
					lLineFigure.StartPoint = lSrcItem.TranslatePoint (lLineFigure.StartPoint, this);
					lLineFigure.StartPoint = new Point (lLineFigure.StartPoint.X + HorizontalOffset, lLineFigure.StartPoint.Y + VerticalOffset);
					lLineSegment.Points.Add (new Point (lLineFigure.StartPoint.X, lLineFigure.StartPoint.Y + (pBranching.Height * mBranchingOffset.Height * pOffsetY)));

					lCenterPoint.X = lLineSegment.Points[0].X;
					lCenterPoint.Y = lLineSegment.Points[0].Y - (mBranchingOffset.Height * pOffsetY);
				}

				lLineFigure.IsClosed = false;
				lLineFigure.IsFilled = false;
				lLineFigure.Segments.Add (lLineSegment);
				lLineGeometry.Figures.Add (lLineFigure);

				lArrowFigure.IsClosed = true;
				lArrowFigure.IsFilled = true;
				if (pBranching.IsValid)
				{
					lArrowFigure.StartPoint = lLineSegment.Points[lLineSegment.Points.Count - 1];
					lArrowSegment.Points.Add (new Point (lArrowFigure.StartPoint.X - pDrawingStuff.ArrowSize.Width, lArrowFigure.StartPoint.Y + (pDrawingStuff.ArrowSize.Height * pOffsetY)));
					lArrowSegment.Points.Add (new Point (lArrowFigure.StartPoint.X + pDrawingStuff.ArrowSize.Width, lArrowFigure.StartPoint.Y + (pDrawingStuff.ArrowSize.Height * pOffsetY)));
				}
				else
				{
					lArrowFigure.StartPoint = lLineSegment.Points[lLineSegment.Points.Count - 1];
					lArrowSegment.Points.Add (new Point (lArrowFigure.StartPoint.X - pDrawingStuff.ArrowSize.Width, lArrowFigure.StartPoint.Y - (pDrawingStuff.ArrowSize.Height * pOffsetY)));
					lArrowSegment.Points.Add (new Point (lArrowFigure.StartPoint.X + pDrawingStuff.ArrowSize.Width, lArrowFigure.StartPoint.Y - (pDrawingStuff.ArrowSize.Height * pOffsetY)));
				}
				lArrowSegment.IsStroked = false;
				lArrowFigure.Segments.Add (lArrowSegment);
				lLineGeometry.Figures.Add (lArrowFigure);

				pDrawingStuff.DrawingContext.DrawGeometry (pBranching.IsValid ? pDrawingStuff.ForegroundBrush : pDrawingStuff.ErrorBrush, pBranching.IsValid ? pDrawingStuff.ForegroundPen : pDrawingStuff.ErrorPen, lLineGeometry);
			}
			catch
			{
			}
			return lCenterPoint;
		}

		private void DrawLineLabel (String pLabel, Point pCenter, BranchingItem pBranching, DrawingStuff pDrawingStuff)
		{
			try
			{
				Point lCenter = pCenter;
				FormattedText lText = new FormattedText (pLabel, System.Globalization.CultureInfo.CurrentUICulture, Frames.FlowDirection, new Typeface (Frames.FontFamily, Frames.FontStyle, Frames.FontWeight, Frames.FontStretch), Frames.FontSize, pDrawingStuff.ForegroundBrush);

				lText.TextAlignment = TextAlignment.Center;
				lText.Trimming = TextTrimming.None;

				if (pBranching.LabelOffset < 0)
				{
					lCenter.X -= (lText.Width / 2.0) - ((Double)pBranching.LabelOffset * (Double)mBranchingOffset.Width) + pDrawingStuff.BackgroundPen.Thickness;
				}
				else if (pBranching.LabelOffset > 0)
				{
					lCenter.X += (lText.Width / 2.0) + ((Double)pBranching.LabelOffset * (Double)mBranchingOffset.Width) + pDrawingStuff.BackgroundPen.Thickness;
				}
				lCenter.Y -= lText.Height / 2.0;

				pDrawingStuff.DrawingContext.DrawGeometry (pDrawingStuff.BackgroundBrush, pDrawingStuff.BackgroundPen, lText.BuildGeometry (lCenter));
				pDrawingStuff.DrawingContext.DrawText (lText, lCenter);
			}
			catch
			{
			}
		}

		//=============================================================================

		private System.Windows.Media.DrawingVisual mBranchingVisual = null;
		private System.Windows.Media.DrawingVisual mExitBranchingVisual = null;

		protected override int VisualChildrenCount
		{
			get
			{
				int lRet = base.VisualChildrenCount;
				if (mBranchingVisual != null)
				{
					lRet++;
				}
				if (mExitBranchingVisual != null)
				{
					lRet++;
				}
				return lRet;
			}
		}

		protected override Visual GetVisualChild (int index)
		{
			if (index >= base.VisualChildrenCount)
			{
				if ((mBranchingVisual != null) && (index-- == base.VisualChildrenCount))
				{
					return mBranchingVisual;
				}
				else if ((mExitBranchingVisual != null) && (index-- == base.VisualChildrenCount))
				{
					return mExitBranchingVisual;
				}
				else
				{
					return null;
				}
			}
			return base.GetVisualChild (index);
		}

		protected override void OnScrollChanged (ScrollChangedEventArgs e)
		{
			if (mBranchingVisual != null)
			{
				mBranchingVisual.Offset = new Vector (-e.HorizontalOffset, mBranchingVisual.Offset.Y);
			}
			if (mExitBranchingVisual != null)
			{
				mExitBranchingVisual.Offset = new Vector (-e.HorizontalOffset, mExitBranchingVisual.Offset.Y);
			}
			base.OnScrollChanged (e);
		}

		#endregion
	}
}
