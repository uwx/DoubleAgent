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
#if DEBUG
//#define DEBUG_LAYOUT
//#define DEBUG_BRANCHING
#endif
/////////////////////////////////////////////////////////////////////////////
using System;
using System.Collections;
using System.Collections.Generic;
using DoubleAgent.Character;
using AgentCharacterEditor;
using AgentCharacterEditor.Global;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor.Previews
{
	public partial class FramesPreview
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		private void InitializeCommon ()
		{
			ShowBranching = true;
			ShowExitBranching = true;
			if (Program.MainWindow != null)
			{
				RecalcLayout (ImageScaleType.Medium);
			}
		}

		#endregion
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

		//=============================================================================

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
		public System.Drawing.Size ImageSize
		{
			get;
			protected set;
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public System.Drawing.Size MinImageSize
		{
			get
			{
				return new System.Drawing.Size (16, 16);
			}
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public System.Drawing.Size MaxImageSize
		{
			get
			{
				return new System.Drawing.Size (256, 256);
			}
		}

		//=============================================================================

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

			Frames.ShowAnimationFrames (CharacterFile, Animation);
			BuildBranchingGraphs ();
			RecalcLayout (ImageScale);
		}

		//=============================================================================

		private class BranchingItem
		{
			public int SrcFrameNdx = -1;
			public int DstFrameNdx = -1;
			public int SrcOffset = 0;
			public int DstOffset = 0;
			public int LabelOffset = 0;
			public int Height = 1;
			public Boolean IsValid;

			public BranchingItem (int pSrcFrameNdx, int pDstFrameNdx, Boolean pIsValid)
			{
				SrcFrameNdx = pSrcFrameNdx;
				DstFrameNdx = pDstFrameNdx;
				IsValid = pIsValid;
			}

			public int MinFrameNdx
			{
				get
				{
					return IsValid ? Math.Min (SrcFrameNdx, DstFrameNdx) : SrcFrameNdx;
				}
			}
			public int MaxFrameNdx
			{
				get
				{
					return IsValid ? Math.Max (SrcFrameNdx, DstFrameNdx) : SrcFrameNdx;
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

			public virtual String Label
			{
				get
				{
					return IsValid ? "exit" : String.Format ("exit ({0})", DstFrameNdx + 1);
				}
			}

			public Boolean Overlaps (BranchingItem pItem)
			{
				if (IsValid && pItem.IsValid)
				{
					if ((MaxFrameNdx <= pItem.MinFrameNdx) || (MinFrameNdx >= pItem.MaxFrameNdx))
					{
						return false;
					}
					return true;
				}
				return false;
			}
			public Boolean Contains (BranchingItem pItem)
			{
				if (IsValid && pItem.IsValid)
				{
					if ((MinFrameNdx <= pItem.MinFrameNdx) && (MaxFrameNdx >= pItem.MaxFrameNdx))
					{
						return true;
					}
				}
				return false;
			}
		}
		private class BranchingItemP : BranchingItem
		{
			public int Probability;

			public BranchingItemP (int pSrcFrameNdx, int pDstFrameNdx, int pProbability, Boolean pIsValid)
				: base (pSrcFrameNdx, pDstFrameNdx, pIsValid)
			{
				Probability = pProbability;
			}

			public override String Label
			{
				get
				{
					return IsValid ? String.Format ("{0}%", Probability) : String.Format ("{0}% ({1})", Probability, DstFrameNdx + 1);
				}
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
		private System.Drawing.Size mBranchingOffset = new System.Drawing.Size ();

		//=============================================================================

		public void BuildBranchingGraphs (int pDefaultBranchingHeight)
		{
			System.Drawing.Point lMaxBranchingOffset = new System.Drawing.Point (0, 0);
			System.Drawing.Point lMaxExitBranchingOffset = new System.Drawing.Point (0, 0);

			mBranchingOffset.Width = 4;
			mBranchingOffset.Height = pDefaultBranchingHeight;

			if (BuildBranchingGraphs (ref lMaxBranchingOffset, ref lMaxExitBranchingOffset))
			{
#if DEBUG_BRANCHING
				System.Diagnostics.Debug.Print ("Branching [{0} {1}] Exit [{2} {3}]", lMaxBranchingOffset.X, lMaxBranchingOffset.Y, lMaxExitBranchingOffset.X, lMaxExitBranchingOffset.Y);
#endif
				lMaxBranchingOffset.X = Math.Max (lMaxBranchingOffset.X, lMaxExitBranchingOffset.X);
				if (lMaxBranchingOffset.X > 0)
				{
					mBranchingOffset.Width = Math.Min (mBranchingOffset.Width, ImageSize.Width / lMaxBranchingOffset.X);
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

		private Boolean BuildBranchingGraphs (ref System.Drawing.Point pMaxBranchingOffset, ref System.Drawing.Point pMaxExitBranchingOffset)
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
					foreach (BranchingItem lBranching in mBranching)
					{
						if (lBranching.IsValid)
						{
							pMaxBranchingOffset.Y = Math.Max (pMaxBranchingOffset.Y, lBranching.Height);
						}
					}
					foreach (BranchingItem lBranching in mBranching)
					{
						if (!lBranching.IsValid)
						{
							lBranching.Height = pMaxBranchingOffset.Y + 3;
						}
					}
					foreach (BranchingItem lBranching in mBranching)
					{
						pMaxBranchingOffset.Y = Math.Max (pMaxBranchingOffset.Y, lBranching.Height);
					}

					ArrangeBranchingX (mBranching);
					foreach (BranchingItem lBranching in mBranching)
					{
						pMaxBranchingOffset.X = Math.Max (pMaxBranchingOffset.X, Math.Abs (lBranching.SrcOffset));
						pMaxBranchingOffset.X = Math.Max (pMaxBranchingOffset.X, Math.Abs (lBranching.DstOffset));
					}
				}
				if (ShowExitBranching)
				{
					ArrangeBranchingY (mExitBranching);
					foreach (BranchingItem lBranching in mExitBranching)
					{
						if (lBranching.IsValid)
						{
							pMaxExitBranchingOffset.Y = Math.Max (pMaxExitBranchingOffset.Y, lBranching.Height);
						}
					}
					foreach (BranchingItem lBranching in mExitBranching)
					{
						if (!lBranching.IsValid)
						{
							lBranching.Height = pMaxExitBranchingOffset.Y + 3;
						}
					}
					foreach (BranchingItem lBranching in mExitBranching)
					{
						pMaxExitBranchingOffset.Y = Math.Max (pMaxExitBranchingOffset.Y, lBranching.Height);
					}

					ArrangeBranchingX (mExitBranching);
					foreach (BranchingItem lBranching in mExitBranching)
					{
						pMaxExitBranchingOffset.X = Math.Max (pMaxExitBranchingOffset.X, Math.Abs (lBranching.SrcOffset));
						pMaxExitBranchingOffset.X = Math.Max (pMaxExitBranchingOffset.X, Math.Abs (lBranching.DstOffset));
					}
				}
			}
#if DEBUG_BRANCHING
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
			System.Diagnostics.Debug.Unindent ();
#endif
			return (mBranching.Length > 0) || (mExitBranching.Length > 0);
		}

		//=============================================================================

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
							lBranching.Add (new BranchingItemP (lFrameNdx, lFrameBranch.mFrameNdx, lFrameBranch.mProbability, (lFrameBranch.mFrameNdx < Animation.FrameCount)));
						}
					}
					if (ShowExitBranching && (lFrame.ExitFrame >= 0))
					{
						lExitBranching.Add (new BranchingItem (lFrameNdx, lFrame.ExitFrame, (lFrame.ExitFrame < Animation.FrameCount)));
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
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
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
					if (lBranching.IsValid && (lBranching.MaxFrameNdx == lFrameNdx))
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
						if ((lMinBranching != lMidBranching) && (lMinBranching.Height >= lMidBranching.Height))
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
						if ((lMaxBranching != lMidBranching) && (lMaxBranching.Height >= lMidBranching.Height))
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
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
			System.Diagnostics.Debug.Unindent ();
#endif
		}

		//=============================================================================

#if DEBUG
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
		#region Sizing Adjustments

		public System.Drawing.Size GetImageScaleSize (ImageScaleType pImageScale)
		{
			System.Drawing.Size lImageSize = (CharacterFile != null) ? CharacterFile.Header.ImageSize : new System.Drawing.Size (128, 128);
			System.Drawing.SizeF lSmallSize = new System.Drawing.SizeF (lImageSize.Width, lImageSize.Height);
			System.Drawing.SizeF lMediumSize = new System.Drawing.SizeF (lImageSize.Width, lImageSize.Height);
			System.Drawing.SizeF lLargeSize = new System.Drawing.SizeF (lImageSize.Width, lImageSize.Height);

			lSmallSize.Width /= 4.0f;
			lSmallSize.Height /= 4.0f;
			lMediumSize.Width /= 2.0f;
			lMediumSize.Height /= 2.0f;

			while (System.Drawing.Size.Round (lSmallSize).EitherLT (MinImageSize))
			{
				lSmallSize.Width *= 2.0f;
				lSmallSize.Height *= 2.0f;
				lMediumSize.Width *= 2.0f;
				lMediumSize.Height *= 2.0f;
				lLargeSize.Width *= 2.0f;
				lLargeSize.Height *= 2.0f;
			}

			while (System.Drawing.Size.Round (lLargeSize).EitherGT (MaxImageSize))
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
				return System.Drawing.Size.Round (lSmallSize);
			}
			else if (pImageScale == ImageScaleType.Large)
			{
				return System.Drawing.Size.Round (lLargeSize);
			}
			else
			{
				return System.Drawing.Size.Round (lMediumSize);
			}
		}

		#endregion
	}
}
