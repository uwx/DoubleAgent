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
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace AgentCharacterEditor.Previews
{
	public partial class AnimationPreviewWPF : UserControl
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public AnimationPreviewWPF ()
		{
			InitializeComponent ();
			this.Image = new System.Windows.Media.ImageDrawing ();
			this.Image.Changed += new EventHandler (OnImageChanged);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public System.Windows.Media.ImageDrawing Image
		{
			get;
			protected set;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		public void StopImageAnimation ()
		{
			if (this.Image != null)
			{
				try
				{
					this.Image.ApplyAnimationClock (System.Windows.Media.ImageDrawing.ImageSourceProperty, null);
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override void OnRender (DrawingContext drawingContext)
		{
			base.OnRender (drawingContext);
			if (this.Image != null)
			{
				drawingContext.DrawImage (this.Image.ImageSource, this.Image.Rect);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void OnImageChanged (object sender, EventArgs e)
		{
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("OnImageChanged");
#endif
			InvalidateVisual ();
		}

		#endregion
	}
}
