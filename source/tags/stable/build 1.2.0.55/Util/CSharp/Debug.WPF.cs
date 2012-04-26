/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2012 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is a utility used by Double Agent but not specific to
	Double Agent.  However, it is included as part of the Double Agent
	source code under the following conditions:

    This is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Media.Animation;

namespace DoubleAgent
{
#if DEBUG
	///////////////////////////////////////////////////////////////////////////////

	public static partial class DebuggingExtensions
	{
		public static void DebugPrint (this System.Windows.UIElement pUIElement)
		{
			DebugPrint (pUIElement, String.Empty);
		}

		public static void DebugPrint (this System.Windows.UIElement pUIElement, String pIndent)
		{
			DebugPrint (pUIElement, String.Empty, String.Empty);
		}

		public static void DebugPrint (this System.Windows.UIElement pUIElement, String pTitle, String pIndent)
		{
			try
			{
				String lTitle = String.IsNullOrEmpty (pTitle) ? String.Empty : pTitle + " ";

				if (pUIElement == null)
				{
					System.Diagnostics.Debug.Print ("{0}{1}<null>", pIndent, lTitle);
				}
				else
				{
					if (pUIElement is FrameworkElement)
					{
						FrameworkElement lFrameworkElement = pUIElement as FrameworkElement;
						System.Diagnostics.Debug.Print ("{0}{1}{2} [{3}] [{4}] Size [{5} {6}] Margin [{7} {8} {9} {10}] DevPxl/LayRnd [{11} {12}]", pIndent, lTitle, pUIElement.GetType ().Name, lFrameworkElement.Name, pUIElement.Visibility, Math.Round (lFrameworkElement.ActualWidth), Math.Round (lFrameworkElement.ActualHeight), lFrameworkElement.Margin.Left, lFrameworkElement.Margin.Top, lFrameworkElement.Margin.Right, lFrameworkElement.Margin.Bottom, lFrameworkElement.SnapsToDevicePixels, lFrameworkElement.UseLayoutRounding);
					}
					else
					{
						System.Diagnostics.Debug.Print ("{0}{1}{2} [{3}]", pIndent, lTitle, pUIElement.GetType ().Name, pUIElement.Visibility);
					}

					if ((Grid.GetRow (pUIElement) != 0) || (Grid.GetColumn (pUIElement) != 0) || (Grid.GetRowSpan (pUIElement) != 1) || (Grid.GetColumnSpan (pUIElement) != 1))
					{
						System.Diagnostics.Debug.Print ("{0}  Row [{1}] Span [{2}] Col [{3}] Span [{4}]", pIndent, Grid.GetRow (pUIElement), Grid.GetRowSpan (pUIElement), Grid.GetColumn (pUIElement), Grid.GetColumnSpan (pUIElement));
					}
				}

				if (pUIElement is Grid)
				{
					Grid lGrid = pUIElement as Grid;
					if (lGrid.RowDefinitions != null)
					{
						System.Diagnostics.Debug.Print ("{0}  {1} Rows", pIndent, lGrid.RowDefinitions.Count);
						foreach (RowDefinition lRow in lGrid.RowDefinitions)
						{
							System.Diagnostics.Debug.Print ("{0}      Row [{1}] Actual [{2}]", pIndent, lRow.Height, lRow.ActualHeight);
						}
					}
					if (lGrid.ColumnDefinitions != null)
					{
						System.Diagnostics.Debug.Print ("{0}  {1} Cols", pIndent, lGrid.ColumnDefinitions.Count);
						foreach (ColumnDefinition lCol in lGrid.ColumnDefinitions)
						{
							System.Diagnostics.Debug.Print ("{0}      Col [{1}] Actual [{2}]", pIndent, lCol.Width, lCol.ActualWidth);
						}
					}
				}

				if (pUIElement is Label)
				{
					System.Diagnostics.Debug.Print ("{0}  Label [{1}]", pIndent, (pUIElement as Label).Content);
				}
				else if (pUIElement is TextBox)
				{
					System.Diagnostics.Debug.Print ("{0}  Text [{1}]", pIndent, (pUIElement as TextBox).Text);
				}
				else if (pUIElement is ToggleButton)
				{
					System.Diagnostics.Debug.Print ("{0}  Checked [{1}] Content [{2}]", pIndent, (pUIElement as ToggleButton).IsChecked, (pUIElement as ToggleButton).Content);
				}
				else if (pUIElement is ContentControl)
				{
					DebugPrint ((pUIElement as ContentControl).Content as UIElement, "Content", pIndent + "  ");
				}
				else if (pUIElement is Panel)
				{
					UIElementCollection lChildren = (pUIElement as Panel).Children;
					String lIndent = pIndent + "  ";

					foreach (UIElement lChild in lChildren)
					{
						DebugPrint (lChild, "Child", lIndent);
					}
				}
			}
			catch
			{
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		public static void DebugPrint (this System.Windows.Media.Animation.Timeline pTimeline)
		{
			DebugPrint (pTimeline, String.Empty);
		}

		public static void DebugPrint (this System.Windows.Media.Animation.Timeline pTimeline, String pIndent)
		{
			DebugPrint (pTimeline, String.Empty, String.Empty);
		}

		public static void DebugPrint (this System.Windows.Media.Animation.Timeline pTimeline, String pTitle, String pIndent)
		{
			try
			{
				String lTitle = String.IsNullOrEmpty (pTitle) ? String.Empty : pTitle + " ";

				if (pTimeline == null)
				{
					System.Diagnostics.Debug.Print ("{0}{1}<null>", pIndent, lTitle);
				}
				else
				{
					System.Diagnostics.Debug.Print ("{0}{1}{2} Start [{3}] Duration [{4} ({5} {6})] Fill [{7}] Repeat [{8}] Reverse [{9}] Type [{10}]", pIndent, lTitle, pTimeline.Name, pTimeline.BeginTime, pTimeline.Duration, pTimeline.AccelerationRatio, pTimeline.DecelerationRatio, pTimeline.FillBehavior, pTimeline.RepeatBehavior, pTimeline.AutoReverse, pTimeline.GetType().Name);
				}

				if (pTimeline is TimelineGroup)
				{
					TimelineGroup lGroup = pTimeline as TimelineGroup;
					System.Diagnostics.Debug.Print ("{0}{1}  {2} Children", pIndent, lTitle, lGroup.Children.Count);
					foreach (Timeline lTimeline in lGroup.Children)
					{
						lTimeline.DebugPrint ("Child", pIndent + "  ");
					}
				}
			}
			catch
			{
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		public static void DebugPrint (this System.Windows.Media.Animation.Clock pClock)
		{
			DebugPrint (pClock, String.Empty);
		}

		public static void DebugPrint (this System.Windows.Media.Animation.Clock pClock, String pIndent)
		{
			DebugPrint (pClock, String.Empty, String.Empty);
		}

		public static void DebugPrint (this System.Windows.Media.Animation.Clock pClock, String pTitle, String pIndent)
		{
			try
			{
				String lTitle = String.IsNullOrEmpty (pTitle) ? String.Empty : pTitle + " ";

				if (pClock == null)
				{
					System.Diagnostics.Debug.Print ("{0}{1}<null>", pIndent, lTitle);
				}
				else
				{
					System.Diagnostics.Debug.Print ("{0}{1}{2} Time [{3}] Progress [({4}) {5}]", pIndent, lTitle, pClock.CurrentState, pClock.CurrentTime, pClock.CurrentIteration, pClock.CurrentProgress);
				}

				if (pClock is ClockGroup)
				{
					ClockGroup lGroup = pClock as ClockGroup;

					System.Diagnostics.Debug.Print ("{0}{1}{2} Children", pIndent, lTitle, lGroup.Children.Count);
					foreach (Clock lClock in lGroup.Children)
					{
						lClock.DebugPrint ("Child", pIndent + "  ");
					}
				}
			}
			catch
			{
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////
#endif
}
