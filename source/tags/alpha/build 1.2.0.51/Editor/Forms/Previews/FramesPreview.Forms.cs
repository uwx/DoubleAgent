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
#if DEBUG
//#define DEBUG_LAYOUT
#endif
/////////////////////////////////////////////////////////////////////////////
using System;
using System.Collections;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using DoubleAgent.Character;
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Properties;

namespace AgentCharacterEditor.Previews
{
	public partial class FramesPreview : UserControl
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public FramesPreview ()
		{
			InitializeComponent ();
			InitializeCommon ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		public void BuildBranchingGraphs ()
		{
			BuildBranchingGraphs (Font.Height);
		}

		public void RecalcLayout (ImageScaleType pImageScale)
		{
			ImageScale = pImageScale;
			ImageSize = GetImageScaleSize (pImageScale);
			Frames.ImageSize = ImageSize;
#if DEBUG_LAYOUT
			Size lCharImageSize = (CharacterFile != null) ? CharacterFile.Header.ImageSize : new Size (128, 128);
			System.Diagnostics.Debug.Print ("RecalcLayout {0} ImageSize {1} Character {2}", ImageScale, ImageSize, lCharImageSize);
			System.Diagnostics.Debug.Print ("   Small {0} Medium {1} Large {2}", GetImageScaleSize (ImageScaleType.Small), GetImageScaleSize (ImageScaleType.Medium), GetImageScaleSize (ImageScaleType.Large));
#endif
			RecalcLayout ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Drawing

		protected override void OnPaint (PaintEventArgs e)
		{
			base.OnPaint (e);

			if ((ShowBranching && (mBranching.Length > 0)) || (ShowExitBranching && (mExitBranching.Length > 0)))
			{
				LineDrawStuff lLineStuff = new LineDrawStuff (this, e.Graphics);
				StringDrawStuff lStringStuff = new StringDrawStuff (this, e.Graphics);

				if (ShowBranching && (mBranching.Length > 0))
				{
					Dictionary<BranchingItem, PointF> lLineCenter = new Dictionary<BranchingItem, PointF> ();

					foreach (BranchingItemP lBranching in mBranching)
					{
						lLineCenter[lBranching] = DrawBranchingLine (Frames.Top, -1, lBranching, lLineStuff);
					}
					foreach (BranchingItemP lBranching in mBranching)
					{
						DrawLineLabel (lBranching.Label, lLineCenter[lBranching], lBranching, lStringStuff);
					}
				}

				if (ShowExitBranching && (mExitBranching.Length > 0))
				{
					Dictionary<BranchingItem, PointF> lLineCenter = new Dictionary<BranchingItem, PointF> ();

					foreach (BranchingItem lBranching in mExitBranching)
					{
						lLineCenter[lBranching] = DrawBranchingLine (Frames.Bottom, 1, lBranching, lLineStuff);
					}
					foreach (BranchingItem lBranching in mExitBranching)
					{
						DrawLineLabel (lBranching.Label, lLineCenter[lBranching], lBranching, lStringStuff);
					}
				}
			}
		}

		//=============================================================================

		private class StringDrawStuff
		{
			public Graphics Graphics;
			public Font Font;
			public Brush ForeBrush;
			public Brush BackBrush;
			public StringFormat Format;

			public StringDrawStuff (Control pOwner, Graphics pGraphics)
			{
				Graphics = pGraphics;
				Font = pOwner.Font;
				ForeBrush = new SolidBrush (pOwner.ForeColor);
				BackBrush = new SolidBrush (pOwner.BackColor);

				Format = new StringFormat (StringFormatFlags.NoWrap);
				Format.Alignment = StringAlignment.Center;
				Format.LineAlignment = StringAlignment.Center;
			}
		}

		private class LineDrawStuff
		{
			public Graphics Graphics;
			public Pen Pen;

			public LineDrawStuff (Control pOwner, Graphics pGraphics)
			{
				Graphics = pGraphics;
				Pen = new Pen (pOwner.ForeColor, 2.0f);

				Pen.ScaleTransform (0.5f, 0.5f);
				Pen.EndCap = System.Drawing.Drawing2D.LineCap.ArrowAnchor;
			}
		}

		private PointF DrawBranchingLine (int pAnchorY, int pOffsetY, BranchingItem pBranching, LineDrawStuff pLineStuff)
		{
			PointF lLineCenter = new PointF ();
			//TODO - invalid branching targets
			try
			{
				Rectangle lItemRect;
				PointF[] lLinePoints = new PointF[4];

				lItemRect = Frames.GetItemRect (pBranching.SrcFrameNdx);
				lLinePoints[0].X = (Single)Frames.Left + (Single)lItemRect.Left + ((Single)lItemRect.Width / 2.0f) + (Single)(pBranching.SrcOffset * mBranchingOffset.Width);
				lLinePoints[0].Y = (Single)pAnchorY;
				lItemRect = Frames.GetItemRect (pBranching.DstFrameNdx);
				lLinePoints[3].X = (Single)Frames.Left + (Single)lItemRect.Left + ((Single)lItemRect.Width / 2.0f) + (Single)(pBranching.DstOffset * mBranchingOffset.Width);
				lLinePoints[3].Y = (Single)pAnchorY;

				lLinePoints[1].X = lLinePoints[0].X;
				lLinePoints[1].Y = lLinePoints[0].Y + (pBranching.Height * mBranchingOffset.Height * pOffsetY);
				lLinePoints[2].X = lLinePoints[3].X;
				lLinePoints[2].Y = lLinePoints[1].Y;

				lLineCenter.X = (lLinePoints[1].X + lLinePoints[2].X) / 2.0f;
				lLineCenter.Y = lLinePoints[1].Y;

				pLineStuff.Graphics.DrawLines (pLineStuff.Pen, lLinePoints);
			}
			catch
			{
			}
			return lLineCenter;
		}

		private void DrawLineLabel (String pLabel, PointF pCenter, BranchingItem pBranching, StringDrawStuff pStringStuff)
		{
			try
			{
				PointF lCenter = pCenter;
				RectangleF lRect;

				lRect = new RectangleF (new PointF (), pStringStuff.Graphics.MeasureString (pLabel, pStringStuff.Font, pCenter, pStringStuff.Format));
				lRect.Offset (lRect.Size.Width / -2.0f, lRect.Size.Height / -2.0f);
				if (pBranching.LabelOffset < 0)
				{
					lCenter.X -= (lRect.Size.Width / 2.0f) - ((Single)pBranching.LabelOffset * (Single)mBranchingOffset.Width);
				}
				else if (pBranching.LabelOffset > 0)
				{
					lCenter.X += (lRect.Size.Width / 2.0f) + ((Single)pBranching.LabelOffset * (Single)mBranchingOffset.Width);
				}
				lRect.Offset (lCenter);

				pStringStuff.Graphics.FillRectangle (pStringStuff.BackBrush, lRect);
				pStringStuff.Graphics.DrawString (pLabel, pStringStuff.Font, pStringStuff.ForeBrush, lCenter, pStringStuff.Format);
			}
			catch
			{
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Sizing Adjustments

		public void RecalcLayout ()
		{
			Size lListSize = ImageSize;

			foreach (ListViewItem lListItem in Frames.Items)
			{
				Rectangle lItemRect = Frames.GetItemRect (lListItem.Index);

				lListSize.Height = Math.Max (lListSize.Height, lItemRect.Height);
				lListSize.Width = Math.Max (lListSize.Width, lItemRect.Right);
			}

#if DEBUG_LAYOUT
			System.Diagnostics.Debug.Print ("RecalcLayout List {0}", lListSize);
#endif
			Invalidate ();
			SuspendLayout ();
			Frames.Size = lListSize;
			ResumeLayout ();
			PerformLayout ();
#if DEBUG_LAYOUT
			DebugLayout ("RecalcLayout");
#endif
		}

		protected override void OnLayout (LayoutEventArgs e)
		{
			Size lClientSize = new Size ();

#if DEBUG_LAYOUT
			DebugLayout ("  OnLayout");
#endif
			VerticalScroll.Value = 0;
			Frames.Location = new Point (-HorizontalScroll.Value, Math.Max ((ShowBranching) ? mBranchingHeight : 0, 4));

			lClientSize.Width = ClientSize.Width;
			lClientSize.Height = Frames.Height + ((ShowBranching) ? mBranchingHeight : 0) + Math.Max (ShowExitBranching ? mExitBranchingHeight : 0, 4);
			lClientSize.Height = Math.Max (lClientSize.Height, MinimumSize.Height);

			if (ClientSize.Height != lClientSize.Height)
			{
				try
				{
#if DEBUG_LAYOUT
					System.Diagnostics.Debug.Print ("  SetClient {0}", lClientSize);
#endif
					Height += lClientSize.Height - ClientSize.Height;
#if DEBUG_LAYOUT
					System.Diagnostics.Debug.Print ("  NewClient {0}", ClientSize);
#endif
				}
				catch
				{
				}
			}

			base.OnLayout (e);

#if DEBUG_LAYOUT
			DebugLayout ("  OnLayout(base)");
#endif
			//
			//	Second round for cases when there is just a bit of horizonal scroll so the vertical scroll bar shows up
			//
			if (ClientSize.Height != lClientSize.Height)
			{
				try
				{
#if DEBUG_LAYOUT
					lClientSize.Width = ClientSize.Width;
					System.Diagnostics.Debug.Print ("  SetClient {0}", lClientSize);
#endif
					Height += lClientSize.Height - ClientSize.Height;
#if DEBUG_LAYOUT
					System.Diagnostics.Debug.Print ("  NewClient {0}", ClientSize);
#endif
					base.OnLayout (e);
				}
				catch
				{
				}
#if DEBUG_LAYOUT
				DebugLayout ("  OnLayout(done)");
#endif
			}
		}

		private void FramesPreview_VisibleChanged (object sender, EventArgs e)
		{
#if DEBUG_LAYOUT
			if (Visible)
			{
				DebugLayout ("  VisibleChanged");
			}
#endif
		}

#if DEBUG_LAYOUT
		private void DebugLayout (String pTitle)
		{
			String lSpacing = new String (' ', 16);
			String lTitle = (pTitle + lSpacing).Substring (0, lSpacing.Length);

			System.Diagnostics.Debug.Print ("{0} List   {1} Margin {2} {3}", lTitle, new Rectangle (Frames.Location, Frames.Size), mBranchingHeight * mBranchingOffset.Height, mExitBranchingHeight * mBranchingOffset.Height);
			System.Diagnostics.Debug.Print ("{0} Client {1} Scroll {2} {3}", lSpacing, ClientSize, HorizontalScroll.Visible, VerticalScroll.Visible);
			System.Diagnostics.Debug.Print ("{0} Frame  {1} Win {2}", lSpacing, (Size - ClientSize), Size);
		}
#endif
		#endregion
	}
}
