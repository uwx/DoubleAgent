#if DEBUG
//#define DEBUG_LAYOUT
//#define DEBUG_BRANCHING
#endif

using System;
using System.Collections;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using DoubleAgent.Character;

namespace AgentCharacterEditor
{
	public partial class FramesPreview : UserControl
	{
		public FramesPreview ()
		{
			InitializeComponent ();

			ShowBranching = true;
			ShowExitBranching = true;
			if (Program.MainForm != null)
			{
				RecalcLayout (ImageScaleType.Medium);
			}
		}

		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public CharacterFile CharacterFile
		{
			get;
			protected set;
		}
		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public FileAnimation Animation
		{
			get;
			protected set;
		}

		///////////////////////////////////////////////////////////////////////////////

		public enum ImageScaleType
		{
			Small,
			Medium,
			Large
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public ImageScaleType ImageScale
		{
			get;
			protected set;
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Size ImageSize
		{
			get;
			protected set;
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Size MinImageSize
		{
			get
			{
				return new Size (16, 16);
			}
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Size MaxImageSize
		{
			get
			{
				return new Size (256, 256);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		public Boolean ShowBranching
		{
			get;
			set;
		}

		public Boolean ShowExitBranching
		{
			get;
			set;
		}

		///////////////////////////////////////////////////////////////////////////////

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		public void Clear ()
		{
			CharacterFile = null;
			Animation = null;
			Frames.CharacterFile = null;
			Frames.Animation = null;
			Frames.Clear ();
		}

		public void ShowAnimationFrames (CharacterFile pCharacterFile, FileAnimation pAnimation)
		{
			CharacterFile = pCharacterFile;
			Animation = pAnimation;
			Frames.CharacterFile = CharacterFile;
			Frames.Animation = Animation;

			Frames.BeginUpdate ();
			Frames.Items.Clear ();

			foreach (FileAnimationFrame lFrame in pAnimation.Frames)
			{
				ListViewItem lListItem;

				lListItem = new ListViewItem (String.Format ("{0} ({1:D})", Global.TitleFrame (lFrame), lFrame.Duration));
				lListItem.Tag = null;
				Frames.Items.Add (lListItem);
				Frames.GetItemImage (lListItem); // Pre-cache all images now - all images drawn immediately anyway.
			}

			Frames.ArrangeIcons (Frames.Alignment);
			Frames.EndUpdate ();

			ShowBranchingGraphs ();
			RecalcLayout (ImageScale);
		}

		///////////////////////////////////////////////////////////////////////////////

		private class BranchingItem
		{
			public int SrcFrameNdx;
			public int DstFrameNdx;
			public int SrcOffset = 0;
			public int DstOffset = 0;
			public int LabelOffset = 0;
			public int Height = 1;

			public BranchingItem (int pSrcFrameNdx, int pDstFrameNdx)
			{
				SrcFrameNdx = pSrcFrameNdx;
				DstFrameNdx = pDstFrameNdx;
			}

			public int MinFrameNdx
			{
				get
				{
					return Math.Min (SrcFrameNdx, DstFrameNdx);
				}
			}
			public int MaxFrameNdx
			{
				get
				{
					return Math.Max (SrcFrameNdx, DstFrameNdx);
				}
			}

			public int MinOffset
			{
				get
				{
					return (MinFrameNdx == SrcFrameNdx) ? SrcOffset : DstOffset;
				}
				set
				{
					if (MinFrameNdx == SrcFrameNdx)
					{
						SrcOffset = value;
					}
					else
					{
						DstOffset = value;
					}

				}
			}
			public int MaxOffset
			{
				get
				{
					return (MaxFrameNdx == SrcFrameNdx) ? SrcOffset : DstOffset;
				}
				set
				{
					if (MaxFrameNdx == SrcFrameNdx)
					{
						SrcOffset = value;
					}
					else
					{
						DstOffset = value;
					}

				}
			}

			public Boolean Overlaps (BranchingItem pItem)
			{
				if ((MaxFrameNdx <= pItem.MinFrameNdx) || (MinFrameNdx >= pItem.MaxFrameNdx))
				{
					return false;
				}
				return true;
			}
			public Boolean Contains (BranchingItem pItem)
			{
				if ((MinFrameNdx <= pItem.MinFrameNdx) && (MaxFrameNdx >= pItem.MaxFrameNdx))
				{
					return true;
				}
				return false;
			}
		}
		private class BranchingItemP : BranchingItem
		{
			public int Probability;

			public BranchingItemP (int pSrcFrameNdx, int pDstFrameNdx, int pProbability)
				: base (pSrcFrameNdx, pDstFrameNdx)
			{
				Probability = pProbability;
			}
		}

		private class BranchingByMinNdx : IComparer
		{
			int IComparer.Compare (Object pObj1, Object pObj2)
			{
				return (pObj2 as BranchingItem).MinFrameNdx - (pObj1 as BranchingItem).MinFrameNdx;
			}
		}
		private class BranchingByHeight : IComparer
		{
			int IComparer.Compare (Object pObj1, Object pObj2)
			{
				return (pObj2 as BranchingItem).Height - (pObj1 as BranchingItem).Height;
			}
		}

		private BranchingItemP[] mBranching = new BranchingItemP[0];
		private BranchingItem[] mExitBranching = new BranchingItem[0];
		private int mBranchingHeight = 0;
		private int mExitBranchingHeight = 0;
		private Size mBranchingOffset = new Size ();

		///////////////////////////////////////////////////////////////////////////////

		public void ShowBranchingGraphs ()
		{
			Point lMaxBranchingOffset = new Point (0, 0);
			Point lMaxExitBranchingOffset = new Point (0, 0);

			mBranchingOffset.Width = 4;
			mBranchingOffset.Height = Font.Height;

			if (ShowBranchingGraphs (ref lMaxBranchingOffset, ref lMaxExitBranchingOffset))
			{
#if DEBUG_BRANCHING
				System.Diagnostics.Debug.Print ("Branching [{0} {1}] Exit [{2} {3}]", lMaxBranchingOffset.X, lMaxBranchingOffset.Y, lMaxExitBranchingOffset.X, lMaxExitBranchingOffset.Y);
#endif
				lMaxBranchingOffset.X = Math.Max (lMaxBranchingOffset.X, lMaxExitBranchingOffset.X);
				if (lMaxBranchingOffset.X > 0)
				{
					mBranchingOffset.Width = Math.Min (mBranchingOffset.Width, (ImageSize.Width * 2) / (lMaxBranchingOffset.X * 3));
				}

				if (lMaxBranchingOffset.Y > 0)
				{
					lMaxBranchingOffset.Y++;
				}
				if (lMaxExitBranchingOffset.Y > 0)
				{
					lMaxExitBranchingOffset.Y++;
				}
#if DEBUG_BRANCHING
				System.Diagnostics.Debug.Print ("Branching Offset [{0} {1}]", mBranchingOffset.Width, mBranchingOffset.Height);
#endif
			}

			mBranchingHeight = (ShowBranching) ? lMaxBranchingOffset.Y * mBranchingOffset.Height : 0;
			mExitBranchingHeight = (ShowExitBranching) ? lMaxExitBranchingOffset.Y * mBranchingOffset.Height : 0;
		}

		private Boolean ShowBranchingGraphs (ref Point pMaxBranchingOffset, ref Point pMaxExitBranchingOffset)
		{
#if DEBUG_BRANCHING
			System.Diagnostics.Debug.Print ("Show Branching");
			System.Diagnostics.Debug.Indent ();
			try
			{
#endif
				pMaxBranchingOffset.Y = 0;
				pMaxExitBranchingOffset.Y = 0;

				if (InitBranching ())
				{
#if DEBUG_BRANCHING
					foreach (BranchingItemP lBranching in mBranching)
					{
						System.Diagnostics.Debug.Print ("Branch [{0} to {1}]", lBranching.SrcFrameNdx + 1, lBranching.DstFrameNdx + 1);
					}
					foreach (BranchingItem lBranching in mExitBranching)
					{
						System.Diagnostics.Debug.Print ("Exit   [{0} to {1}]", lBranching.SrcFrameNdx + 1, lBranching.DstFrameNdx + 1);
					}
#endif
					if (ShowBranching)
					{
						ArrangeBranchingY (mBranching);
						ArrangeBranchingX (mBranching);

						foreach (BranchingItem lBranching in mBranching)
						{
							pMaxBranchingOffset.X = Math.Max (pMaxBranchingOffset.X, Math.Abs (lBranching.SrcOffset));
							pMaxBranchingOffset.X = Math.Max (pMaxBranchingOffset.X, Math.Abs (lBranching.DstOffset));
							pMaxBranchingOffset.Y = Math.Max (pMaxBranchingOffset.Y, lBranching.Height);
						}
					}
					if (ShowExitBranching)
					{
						ArrangeBranchingY (mExitBranching);
						ArrangeBranchingX (mExitBranching);

						foreach (BranchingItem lBranching in mExitBranching)
						{
							pMaxExitBranchingOffset.X = Math.Max (pMaxExitBranchingOffset.X, Math.Abs (lBranching.SrcOffset));
							pMaxExitBranchingOffset.X = Math.Max (pMaxExitBranchingOffset.X, Math.Abs (lBranching.DstOffset));
							pMaxExitBranchingOffset.Y = Math.Max (pMaxExitBranchingOffset.Y, lBranching.Height);
						}
					}
				}
#if DEBUG_BRANCHING
			}
			catch (Exception e)
			{
				System.Diagnostics.Debug.Print (e.Message);
			}
			System.Diagnostics.Debug.Unindent ();
#endif
			return (mBranching.Length > 0) || (mExitBranching.Length > 0);
		}

		private Boolean InitBranching ()
		{
			System.Collections.Generic.List<BranchingItemP> lBranching = new System.Collections.Generic.List<BranchingItemP> ();
			System.Collections.Generic.List<BranchingItem> lExitBranching = new System.Collections.Generic.List<BranchingItem> ();

			if (ShowBranching || ShowExitBranching)
			{
				int lFrameNdx;
				FileAnimationFrame lFrame;

				for (lFrameNdx = 0; lFrameNdx < Animation.FrameCount; lFrameNdx++)
				{
					lFrame = Animation.Frames[lFrameNdx];

					if (ShowBranching && (lFrame.Branching != null))
					{
						foreach (FileFrameBranch lFrameBranch in lFrame.Branching)
						{
							lBranching.Add (new BranchingItemP (lFrameNdx, lFrameBranch.mFrameNdx, lFrameBranch.mProbability));
						}
					}
					if (ShowExitBranching && (lFrame.ExitFrame >= 0))
					{
						lExitBranching.Add (new BranchingItem (lFrameNdx, lFrame.ExitFrame));
					}
				}
			}

			mBranching = lBranching.ToArray ();
			mExitBranching = lExitBranching.ToArray ();

			//Array.Sort (mBranching, new BranchingByMinNdx());
			//Array.Sort (mExitBranching, new BranchingByMinNdx());

			return (mBranching.Length > 0) || (mExitBranching.Length > 0);
		}

		private void ArrangeBranchingY (System.Collections.IEnumerable pBranching)
		{
			Dictionary<KeyValuePair<BranchingItem, BranchingItem>, Boolean> lOverlaps = new Dictionary<KeyValuePair<BranchingItem, BranchingItem>, Boolean> ();
			HashSet<BranchingItem> lUsedKeys = new HashSet<BranchingItem> ();

#if DEBUG_BRANCHING
			foreach (BranchingItem lBranching in pBranching)
			{
				System.Diagnostics.Debug.Print ("Arrange{0}Y", ((lBranching is BranchingItemP) ? "Branching" : "Exit Branching"));
				break;
			}

			System.Diagnostics.Debug.Indent ();
			try
			{
#endif
				foreach (BranchingItem lBranching in pBranching)
				{
					foreach (BranchingItem lBranching2 in pBranching)
					{
						if (!Object.ReferenceEquals (lBranching2, lBranching) && lBranching2.Contains (lBranching))
						{
							KeyValuePair<BranchingItem, BranchingItem> lOverlap = new KeyValuePair<BranchingItem, BranchingItem> (lBranching, lBranching2);
							if (lOverlaps.ContainsKey (lOverlap))
							{
								continue;
							}
							lOverlap = new KeyValuePair<BranchingItem, BranchingItem> (lBranching2, lBranching);
							lOverlaps[lOverlap] = true;
#if DEBUG_BRANCHING
							System.Diagnostics.Debug.Print ("{0} contains {1}", DebugName (lBranching2), DebugName (lBranching));
#endif
						}
					}
				}

				foreach (BranchingItem lBranching in pBranching)
				{
					foreach (BranchingItem lBranching2 in pBranching)
					{
						if (!Object.ReferenceEquals (lBranching2, lBranching) && lBranching2.Overlaps (lBranching))
						{
							KeyValuePair<BranchingItem, BranchingItem> lOverlap = new KeyValuePair<BranchingItem, BranchingItem> (lBranching, lBranching2);
							if (lOverlaps.ContainsKey (lOverlap))
							{
								continue;
							}
							lOverlap = new KeyValuePair<BranchingItem, BranchingItem> (lBranching2, lBranching);
							if (lOverlaps.ContainsKey (lOverlap))
							{
								continue;
							}
							lOverlaps[lOverlap] = false;
#if DEBUG_BRANCHING
							System.Diagnostics.Debug.Print ("{0} overlaps {1}", DebugName (lBranching2), DebugName (lBranching));
#endif
						}
					}
				}

				foreach (KeyValuePair<BranchingItem, BranchingItem> lOverlap in lOverlaps.Keys)
				{
					lOverlap.Key.Height = GetOverlapHeight (lOverlap.Key, lOverlaps, lUsedKeys);
#if DEBUG_BRANCHING
					System.Diagnostics.Debug.Print ("{0} Height {1}", DebugName (lOverlap.Key), lOverlap.Key.Height);
#endif
				}

#if DEBUG_BRANCHING
			}
			catch (Exception e)
			{
				System.Diagnostics.Debug.Print (e.Message);
			}
			System.Diagnostics.Debug.Unindent ();
#endif
		}

		private int GetOverlapHeight (BranchingItem pKeyItem, Dictionary<KeyValuePair<BranchingItem, BranchingItem>, Boolean> pOverlaps, HashSet<BranchingItem> pUsedKeys)
		{
			if (!pUsedKeys.Contains (pKeyItem))
			{
				pUsedKeys.Add (pKeyItem);

				foreach (KeyValuePair<BranchingItem, BranchingItem> lOverlap in pOverlaps.Keys)
				{
					if (Object.ReferenceEquals (lOverlap.Key, pKeyItem))
					{
						pKeyItem.Height = Math.Max (pKeyItem.Height, GetOverlapHeight (lOverlap.Value, pOverlaps, pUsedKeys) + 1);
					}
				}
			}
			return pKeyItem.Height;
		}

		private void ArrangeBranchingX (System.Collections.IEnumerable pBranching)
		{
			BranchingByHeight lComparer = new BranchingByHeight ();
			ArrayList[] lMinBranches = new ArrayList[Animation.FrameCount];
			ArrayList[] lMaxBranches = new ArrayList[Animation.FrameCount];
			ArrayList[] lMidBranches = new ArrayList[Animation.FrameCount];
			int lFrameNdx;

#if DEBUG_BRANCHING
			foreach (BranchingItem lBranching in pBranching)
			{
				System.Diagnostics.Debug.Print ("Arrange{0}X", ((lBranching is BranchingItemP) ? "Branching" : "Exit Branching"));
				break;
			}
			System.Diagnostics.Debug.Indent ();
			try
			{
#endif

				for (lFrameNdx = 0; lFrameNdx < Animation.FrameCount; lFrameNdx++)
				{
					lMinBranches[lFrameNdx] = new ArrayList ();
					lMaxBranches[lFrameNdx] = new ArrayList ();
					lMidBranches[lFrameNdx] = new ArrayList ();

					foreach (BranchingItem lBranching in pBranching)
					{
						if (lBranching.MinFrameNdx == lFrameNdx)
						{
							lMinBranches[lFrameNdx].Add (lBranching);
						}
						if (lBranching.MaxFrameNdx == lFrameNdx)
						{
							lMaxBranches[lFrameNdx].Add (lBranching);
						}
						if ((((lBranching.MaxFrameNdx - lBranching.MinFrameNdx) % 2) == 0) && (((lBranching.MinFrameNdx + lBranching.MaxFrameNdx) / 2) == lFrameNdx))
						{
							lMidBranches[lFrameNdx].Add (lBranching);
						}
					}

					if (lMinBranches[lFrameNdx].Count > 0)
					{
						int lOffset = 1 + (lMaxBranches[lFrameNdx].Count - lMinBranches[lFrameNdx].Count);

						lMinBranches[lFrameNdx].Sort (lComparer);

						foreach (BranchingItem lBranching in lMinBranches[lFrameNdx])
						{
							lBranching.MinOffset = lOffset;
#if DEBUG_BRANCHING
							System.Diagnostics.Debug.Print ("{0} MinOffset [{1}]", DebugName (lBranching), lBranching.MinOffset);
#endif
							lOffset += 2;
						}
					}

					if (lMaxBranches[lFrameNdx].Count > 0)
					{
						int lOffset = -1 - (lMinBranches[lFrameNdx].Count - lMaxBranches[lFrameNdx].Count);

						lMaxBranches[lFrameNdx].Sort (lComparer);

						foreach (BranchingItem lBranching in lMaxBranches[lFrameNdx])
						{
							lBranching.MaxOffset = lOffset;
#if DEBUG_BRANCHING
							System.Diagnostics.Debug.Print ("{0} MaxOffset [{1}]", DebugName (lBranching), lBranching.MaxOffset);
#endif
							lOffset -= 2;
						}
					}
				}

				for (lFrameNdx = 0; lFrameNdx < Animation.FrameCount; lFrameNdx++)
				{
					foreach (BranchingItem lMidBranching in lMidBranches[lFrameNdx])
					{
						foreach (BranchingItem lMinBranching in lMinBranches[lFrameNdx])
						{
							if (lMinBranching.Height >= lMidBranching.Height)
							{
								lMidBranching.LabelOffset = Math.Min (lMidBranching.LabelOffset, lMinBranching.MinOffset - 1);
								if (lMidBranching.LabelOffset == 0)
								{
									lMidBranching.LabelOffset--;
								}
							}
						}
						foreach (BranchingItem lMaxBranching in lMaxBranches[lFrameNdx])
						{
							if (lMaxBranching.Height >= lMidBranching.Height)
							{
								lMidBranching.LabelOffset = Math.Max (lMidBranching.LabelOffset, lMaxBranching.MaxOffset + 1);
								if (lMidBranching.LabelOffset == 0)
								{
									lMidBranching.LabelOffset++;
								}
							}
						}
#if DEBUG_BRANCHING
						System.Diagnostics.Debug.Print ("{0} LabelOffset [{1}]", DebugName (lMidBranching), lMidBranching.LabelOffset);
#endif
					}
				}
#if DEBUG_BRANCHING
			}
			catch (Exception e)
			{
				System.Diagnostics.Debug.Print (e.Message);
			}
			System.Diagnostics.Debug.Unindent ();
#endif
		}

		///////////////////////////////////////////////////////////////////////////////

#if DEBUG_BRANCHING
		static private String DebugName (BranchingItem pBranching)
		{
			if (pBranching is BranchingItemP)
			{
				return String.Format ("[{0} to {1} ({2:D3}%)]", pBranching.SrcFrameNdx + 1, pBranching.DstFrameNdx + 1, (pBranching as BranchingItemP).Probability);
			}
			else
			{
				return String.Format ("[{0} to {1} (exit)]", pBranching.SrcFrameNdx + 1, pBranching.DstFrameNdx + 1);
			}
		}
#endif

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
						DrawLineLabel (String.Format ("{0}%", lBranching.Probability), lLineCenter[lBranching], lBranching, lStringStuff);
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
						DrawLineLabel ("exit", lLineCenter[lBranching], lBranching, lStringStuff);
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

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

		private PointF DrawBranchingLine (int pAnchorY, int pOffsetX, BranchingItem pBranching, LineDrawStuff pLineStuff)
		{
			Rectangle lItemRect;
			PointF[] lLinePoints = new PointF[4];

			try
			{
				lItemRect = Frames.GetItemRect (pBranching.SrcFrameNdx);
				lLinePoints[0].X = (float)Frames.Left + (float)lItemRect.Left + ((float)lItemRect.Width / 2.0f) + (float)(pBranching.SrcOffset * mBranchingOffset.Width);
				lLinePoints[0].Y = (float)pAnchorY;
				lItemRect = Frames.GetItemRect (pBranching.DstFrameNdx);
				lLinePoints[3].X = (float)Frames.Left + (float)lItemRect.Left + ((float)lItemRect.Width / 2.0f) + (float)(pBranching.DstOffset * mBranchingOffset.Width);
				lLinePoints[3].Y = (float)pAnchorY;

				lLinePoints[1].X = lLinePoints[0].X;
				lLinePoints[1].Y = lLinePoints[0].Y + (pBranching.Height * mBranchingOffset.Height * pOffsetX);
				lLinePoints[2].X = lLinePoints[3].X;
				lLinePoints[2].Y = lLinePoints[1].Y;

				pLineStuff.Graphics.DrawLines (pLineStuff.Pen, lLinePoints);
			}
			catch
			{
			}
			return new PointF ((lLinePoints[1].X + lLinePoints[2].X) / 2.0f, lLinePoints[1].Y);
		}

		private void DrawLineLabel (String pLabel, PointF pCenter, BranchingItem pBranching, StringDrawStuff pStringStuff)
		{
			PointF lCenter = pCenter;
			RectangleF lRect;

			try
			{
				lRect = new RectangleF (new PointF (), pStringStuff.Graphics.MeasureString (pLabel, pStringStuff.Font, pCenter, pStringStuff.Format));
				lRect.Offset (lRect.Size.Width / -2.0f, lRect.Size.Height / -2.0f);
				if (pBranching.LabelOffset < 0)
				{
					lCenter.X -= lRect.Size.Width + ((float)pBranching.LabelOffset * (float)mBranchingOffset.Width);
				}
				else if (pBranching.LabelOffset > 0)
				{
					lCenter.X += lRect.Size.Width + ((float)pBranching.LabelOffset * (float)mBranchingOffset.Width);
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

		public Size GetImageScaleSize (ImageScaleType pImageScale)
		{
			Size lImageSize = (CharacterFile != null) ? CharacterFile.Header.ImageSize : new Size (128, 128);
			SizeF lSmallSize = new SizeF (lImageSize.Width, lImageSize.Height);
			SizeF lMediumSize = new SizeF (lImageSize.Width, lImageSize.Height);
			SizeF lLargeSize = new SizeF (lImageSize.Width, lImageSize.Height);

			lSmallSize.Width /= 4.0f;
			lSmallSize.Height /= 4.0f;
			lMediumSize.Width /= 2.0f;
			lMediumSize.Height /= 2.0f;

			while (Size.Round (lSmallSize).EitherLT (MinImageSize))
			{
				lSmallSize.Width *= 2.0f;
				lSmallSize.Height *= 2.0f;
				lMediumSize.Width *= 2.0f;
				lMediumSize.Height *= 2.0f;
				lLargeSize.Width *= 2.0f;
				lLargeSize.Height *= 2.0f;
			}

			while (Size.Round (lLargeSize).EitherGT (MaxImageSize))
			{
				lSmallSize.Width /= 2.0f;
				lSmallSize.Height /= 2.0f;
				lMediumSize.Width /= 2.0f;
				lMediumSize.Height /= 2.0f;
				lLargeSize.Width /= 2.0f;
				lLargeSize.Height /= 2.0f;
			}

			if (pImageScale == ImageScaleType.Small)
			{
				return Size.Round (lSmallSize);
			}
			else if (pImageScale == ImageScaleType.Large)
			{
				return Size.Round (lLargeSize);
			}
			else
			{
				return Size.Round (lMediumSize);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		public void RecalcLayout (ImageScaleType pImageScale)
		{
			ImageList lImageList = new ImageList ();

			ImageScale = pImageScale;
			ImageSize = GetImageScaleSize (pImageScale);

			lImageList.ColorDepth = ColorDepth.Depth32Bit;
			lImageList.ImageSize = ImageSize;
			Frames.LargeImageList = lImageList;

#if DEBUG_LAYOUT
			Size lCharImageSize = (CharacterFile != null) ? CharacterFile.Header.ImageSize : new Size (128, 128);
			System.Diagnostics.Debug.Print ("RecalcLayout {0} ImageSize {1} Character {2}", ImageScale, ImageSize, lCharImageSize);
			System.Diagnostics.Debug.Print ("   Small {0} Medium {1} Large {2}", GetImageScaleSize (ImageScaleType.Small), GetImageScaleSize (ImageScaleType.Medium), GetImageScaleSize (ImageScaleType.Large));
#endif
			RecalcLayout ();
		}

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
