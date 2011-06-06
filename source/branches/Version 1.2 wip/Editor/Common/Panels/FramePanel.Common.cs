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
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Updates;
using AgentCharacterEditor.Properties;
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Panels
{
	public partial class FramePanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public override ResolvePart FilePart
		{
			get
			{
				return base.FilePart;
			}
			set
			{
				base.FilePart = value;
				if (FilePart is ResolveAnimationFrame)
				{
					(FilePart as ResolveAnimationFrame).Scope = ResolveAnimationFrame.ScopeType.ScopeFrame;
				}
				Frame = (FilePart is ResolveAnimationFrame) ? (FilePart as ResolveAnimationFrame).Target : null;

				ShowFrameName ();
				ShowFrameDuration ();
				ShowFrameSample ();
				ShowFrameImages ();
				ShowFrameSound ();
			}
		}

		protected FileAnimationFrame Frame
		{
			get;
			set;
		}

		protected FileAnimation Animation
		{
			get
			{
				return (Frame == null) ? null : Frame.Animation;
			}
		}

		protected String FrameTitle
		{
			get
			{
				return Titles.Frame (Frame);
			}
		}

		public override Boolean IsPanelEmpty
		{
			get
			{
				return base.IsPanelEmpty || (Frame == null);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowFrameName ()
		{
			Boolean lWasFilling = PushIsPanelFilling (true);

			if (IsPanelEmpty)
			{
				TextBoxFrameName.Clear ();
				TextBoxFrameName.IsEnabled = false;
			}
			else
			{
				TextBoxFrameName.Text = Titles.FrameAnimation (Frame);
				TextBoxFrameName.IsEnabled = true;
			}

			PopIsPanelFilling (lWasFilling);
		}

		private void ShowFrameDuration ()
		{
			if (IsPanelEmpty)
			{
				NumericDuration.Clear ();
				NumericDuration.IsEnabled = false;
			}
			else
			{
				NumericDuration.Value = Frame.Duration;
				NumericDuration.IsEnabled = !Program.FileIsReadOnly;
			}
			NumericDuration.IsModified = false;
		}

		#endregion
	}
}
