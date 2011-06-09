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
using System.Collections;
using System.Collections.Generic;
using DoubleAgent.Character;
using AgentCharacterEditor;
using AgentCharacterEditor.Global;
using AppResources = AgentCharacterEditor.Resources;

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

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		public void ShowAnimationFrames (CharacterFile pCharacterFile, FileAnimation pAnimation)
		{
			CharacterFile = pCharacterFile;
			Animation = pAnimation;
			ShowAnimationFrames ();
		}

		///////////////////////////////////////////////////////////////////////////////

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
						System.Drawing.Pen lPen = new System.Drawing.Pen (System.Drawing.Color.Pink, 5.0f);

						lGraphics.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.None;
						lGraphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;

						lGraphics.DrawRectangle (lPen, 3.0f, 3.0f, (float)lImage.Width - 7.0f, (float)lImage.Height - 7.0f);
						lGraphics.DrawLine (lPen, 3.0f, 3.0f, (float)lImage.Width - 4.0f, (float)lImage.Height - 4.0f);
						lGraphics.DrawLine (lPen, 3.0f, (float)lImage.Height - 4.0f, (float)lImage.Width - 4.0f, 3.0f);
						lGraphics.Dispose ();
					}
				}
				catch
				{
				}
			}
			return lImage;
		}

		#endregion
	}
}
