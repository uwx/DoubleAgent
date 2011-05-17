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
