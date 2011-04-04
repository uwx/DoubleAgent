using System;
using System.Windows.Forms;
using System.ComponentModel;

namespace DoubleAgent
{
	public class ToolStripEx : System.Windows.Forms.ToolStrip
	{
		public ToolStripEx ()
		{
			RenderInline = true;
			CanOverflow = false;
		}

		[System.ComponentModel.Category ("Appearance")]
		public bool RenderInline
		{
			get
			{
				return (Renderer != null) && (typeof (InlineToolStripRenderer).IsAssignableFrom (Renderer.GetType()));
			}
			set
			{
				if (value)
				{
					Renderer = new InlineToolStripRenderer ();
				}
				else
				{
					Renderer = null;
					RenderMode = ToolStripRenderMode.System;
				}
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	class InlineToolStripRenderer : ToolStripSystemRenderer
	{
		protected override void OnRenderToolStripBorder (ToolStripRenderEventArgs e)
		{
		}
	}

}
