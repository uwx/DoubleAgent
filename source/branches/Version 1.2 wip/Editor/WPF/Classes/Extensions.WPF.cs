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
using System.Windows.Controls;
using System.Windows.Input;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor.Global
{
	public static partial class StringExtensions
	{
		/// <summary>
		/// Updates the mnemonic prefix indicator ('&amp;') in a string.
		/// </summary>
		/// <param name="pString">The initial string value.</param>
		/// <returns>The string value with any mnemonic prefix updated for the current application build.</returns>
		static public String FixMenuPrefix (this String pString)
		{
			return String.IsNullOrEmpty (pString) ? String.Empty : pString.Replace ("&", "_");
		}

		/// <summary>
		/// Strips the mnemonic prefix indicator ('&amp;') from a string.
		/// </summary>
		/// <param name="pString">The initial string value.</param>
		/// <returns>The string value with any mnemonic prefix removed.</returns>
		static public String NoMenuPrefix (this String pString)
		{
			return String.IsNullOrEmpty (pString) ? String.Empty : pString.Replace ("&", "");
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public static class DrawingExtensions
	{
		static public System.Windows.Point ToWPF (this System.Drawing.Point pPoint)
		{
			return new System.Windows.Point (pPoint.X, pPoint.Y);
		}
		static public System.Windows.Point ToWPF (this System.Drawing.PointF pPoint)
		{
			return new System.Windows.Point (pPoint.X, pPoint.Y);
		}
		static public System.Drawing.PointF FromWPF (this System.Windows.Point pPoint)
		{
			return new System.Drawing.PointF ((Single)pPoint.X, (Single)pPoint.Y);
		}

		static public System.Windows.Size ToWPF (this System.Drawing.Size pSize)
		{
			return new System.Windows.Size (pSize.Width, pSize.Width);
		}
		static public System.Windows.Size ToWPF (this System.Drawing.SizeF pSize)
		{
			return new System.Windows.Size (pSize.Width, pSize.Width);
		}
		static public System.Drawing.SizeF FromWPF (this System.Windows.Size pSize)
		{
			return new System.Drawing.SizeF ((Single)pSize.Width, (Single)pSize.Width);
		}

		static public System.Windows.Rect ToWPF (this System.Drawing.Rectangle pRect)
		{
			return new System.Windows.Rect (pRect.X, pRect.Y, pRect.Width, pRect.Width);
		}
		static public System.Windows.Rect ToWPF (this System.Drawing.RectangleF pRect)
		{
			return new System.Windows.Rect (pRect.X, pRect.Y, pRect.Width, pRect.Width);
		}
		static public System.Drawing.RectangleF FromWPF (this System.Windows.Rect pRect)
		{
			return new System.Drawing.RectangleF ((Single)pRect.X, (Single)pRect.Y, (Single)pRect.Width, (Single)pRect.Width);
		}

		static public System.Windows.Media.Color ToWPF (this System.Drawing.Color pColor)
		{
			return System.Windows.Media.Color.FromArgb (pColor.A, pColor.R, pColor.G, pColor.B);
		}
		static public System.Windows.Media.Brush ToWPFBrush (this System.Drawing.Color pColor)
		{
			return new System.Windows.Media.SolidColorBrush (pColor.ToWPF());
		}
		static public System.Drawing.Color FromWPF (this System.Windows.Media.Color pColor)
		{
			return System.Drawing.Color.FromArgb (pColor.A, pColor.R, pColor.G, pColor.B);
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public static class EditEventsExtensions
	{
		static public void PutCutTitle (this CanExecuteRoutedEventArgs pEventArgs)
		{
			pEventArgs.PutCutTitle (String.Empty);
		}

		static public void PutCutTitle (this CanExecuteRoutedEventArgs pEventArgs, String pObjectTitle)
		{
			if (pEventArgs.Parameter is Button)
			{
				(pEventArgs.Parameter as Button).ToolTip = String.Format (AppResources.Resources.EditCutThis.NoMenuPrefix (), pObjectTitle);
			}
			if (pEventArgs.Parameter is MenuItem)
			{
				(pEventArgs.Parameter as MenuItem).Header = String.Format (AppResources.Resources.EditCutThis.FixMenuPrefix (), pObjectTitle);
			}
			if (!String.IsNullOrEmpty (pObjectTitle))
			{
				pEventArgs.CanExecute = true;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		static public void PutCopyTitle (this CanExecuteRoutedEventArgs pEventArgs)
		{
			pEventArgs.PutCopyTitle (String.Empty);
		}

		static public void PutCopyTitle (this CanExecuteRoutedEventArgs pEventArgs, String pObjectTitle)
		{
			if (pEventArgs.Parameter is Button)
			{
				(pEventArgs.Parameter as Button).ToolTip = String.Format (AppResources.Resources.EditCopyThis.NoMenuPrefix (), pObjectTitle);
			}
			if (pEventArgs.Parameter is MenuItem)
			{
				(pEventArgs.Parameter as MenuItem).Header = String.Format (AppResources.Resources.EditCopyThis.FixMenuPrefix (), pObjectTitle);
			}
			if (!String.IsNullOrEmpty (pObjectTitle))
			{
				pEventArgs.CanExecute = true;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		static public void PutDeleteTitle (this CanExecuteRoutedEventArgs pEventArgs)
		{
			pEventArgs.PutDeleteTitle (String.Empty);
		}

		static public void PutDeleteTitle (this CanExecuteRoutedEventArgs pEventArgs, String pObjectTitle)
		{
			if (pEventArgs.Parameter is Button)
			{
				(pEventArgs.Parameter as Button).ToolTip = String.Format (AppResources.Resources.EditDeleteThis.NoMenuPrefix (), pObjectTitle);
			}
			if (pEventArgs.Parameter is MenuItem)
			{
				(pEventArgs.Parameter as MenuItem).Header = String.Format (AppResources.Resources.EditDeleteThis.FixMenuPrefix (), pObjectTitle);
			}
			if (!String.IsNullOrEmpty (pObjectTitle))
			{
				pEventArgs.CanExecute = true;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		static public void PutPasteTitle (this CanExecuteRoutedEventArgs pEventArgs)
		{
			pEventArgs.PutPasteTitle (String.Empty);
		}

		static public void PutPasteTitle (this CanExecuteRoutedEventArgs pEventArgs, String pObjectTitle)
		{
			if (pEventArgs.Parameter is Button)
			{
				(pEventArgs.Parameter as Button).ToolTip = String.Format (AppResources.Resources.EditPasteThis.NoMenuPrefix (), pObjectTitle);
			}
			if (pEventArgs.Parameter is MenuItem)
			{
				(pEventArgs.Parameter as MenuItem).Header = String.Format (AppResources.Resources.EditPasteThis.FixMenuPrefix (), pObjectTitle);
			}
			if (!String.IsNullOrEmpty (pObjectTitle))
			{
				pEventArgs.CanExecute = true;
			}
		}

		static public void PutPasteTitle (this CanExecuteRoutedEventArgs pEventArgs, String pPasteTypeTitle, String pObjectTitle)
		{
			if (pEventArgs.Parameter is Button)
			{
				(pEventArgs.Parameter as Button).ToolTip = String.Format (pPasteTypeTitle.NoMenuPrefix (), pObjectTitle);
			}
			if (pEventArgs.Parameter is MenuItem)
			{
				(pEventArgs.Parameter as MenuItem).Header = String.Format (pPasteTypeTitle.FixMenuPrefix (), pObjectTitle);
			}
			if (!String.IsNullOrEmpty (pObjectTitle))
			{
				pEventArgs.CanExecute = true;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		static public void PutUndoTitle (this CanExecuteRoutedEventArgs pEventArgs)
		{
			pEventArgs.PutUndoTitle (String.Empty);
		}

		static public void PutUndoTitle (this CanExecuteRoutedEventArgs pEventArgs, String pObjectTitle)
		{
			if (pEventArgs.Parameter is Button)
			{
				if (String.IsNullOrEmpty (pObjectTitle))
				{
					(pEventArgs.Parameter as Button).ToolTip = AppResources.Resources.EditUndo.NoMenuPrefix ();
				}
				else
				{
					(pEventArgs.Parameter as Button).ToolTip = String.Format (AppResources.Resources.EditUndoThis.NoMenuPrefix (), pObjectTitle);
				}
			}
			if (pEventArgs.Parameter is MenuItem)
			{
				if (String.IsNullOrEmpty (pObjectTitle))
				{
					(pEventArgs.Parameter as MenuItem).Header = AppResources.Resources.EditUndo.FixMenuPrefix ();
				}
				else
				{
					(pEventArgs.Parameter as MenuItem).Header = String.Format (AppResources.Resources.EditUndoThis.FixMenuPrefix (), pObjectTitle);
				}
			}
			if (!String.IsNullOrEmpty (pObjectTitle))
			{
				pEventArgs.CanExecute = true;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		static public void PutRedoTitle (this CanExecuteRoutedEventArgs pEventArgs)
		{
			pEventArgs.PutRedoTitle (String.Empty);
		}

		static public void PutRedoTitle (this CanExecuteRoutedEventArgs pEventArgs, String pObjectTitle)
		{
			if (pEventArgs.Parameter is Button)
			{
				if (String.IsNullOrEmpty (pObjectTitle))
				{
					(pEventArgs.Parameter as Button).ToolTip = AppResources.Resources.EditRedo.NoMenuPrefix ();
				}
				else
				{
					(pEventArgs.Parameter as Button).ToolTip = String.Format (AppResources.Resources.EditRedoThis.NoMenuPrefix (), pObjectTitle);
				}
			}
			if (pEventArgs.Parameter is MenuItem)
			{
				if (String.IsNullOrEmpty (pObjectTitle))
				{
					(pEventArgs.Parameter as MenuItem).Header = AppResources.Resources.EditRedo.FixMenuPrefix ();
				}
				else
				{
					(pEventArgs.Parameter as MenuItem).Header = String.Format (AppResources.Resources.EditRedoThis.FixMenuPrefix (), pObjectTitle);
				}
			}
			if (!String.IsNullOrEmpty (pObjectTitle))
			{
				pEventArgs.CanExecute = true;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public static partial class ListViewExtensions
	{
		/// <summary>
		/// Updates the <see cref="System.Windows.Controls.ListView.Items"/> collection to contain a specific number of items.
		/// </summary>
		/// <param name="pItemCount">The number of items required.</param>
		/// <returns>True if items were added or removed.</returns>
		public static Boolean UpdateItemCount (this ListView pListView, int pItemCount)
		{
			Boolean lRet = false;

			while (pListView.Items.Count > pItemCount)
			{
				pListView.Items.RemoveAt (pListView.Items.Count - 1);
				lRet = true;
			}
			while (pListView.Items.Count < pItemCount)
			{
				pListView.Items.Add (new ListViewItemCommon (""));
				lRet = true;
			}
			return lRet;
		}

		public static ScrollBarVisibility GetVerticalScrollBarVisibility (this ListView pListView)
		{
			ScrollViewer lScrollHost = pListView.Template.FindName ("ScrollHost", pListView) as ScrollViewer;
			if (lScrollHost != null)
			{
				return lScrollHost.VerticalScrollBarVisibility;
			}
			return ScrollBarVisibility.Auto;
		}

		public static Boolean SetVerticalScrollBarVisibility (this ListView pListView, ScrollBarVisibility pScrollBarVisibility)
		{
			ScrollViewer lScrollHost = pListView.Template.FindName ("ScrollHost", pListView) as ScrollViewer;
			if (lScrollHost != null)
			{
				lScrollHost.VerticalScrollBarVisibility = pScrollBarVisibility;
				return true;
			}
			return false;
		}

		public static ScrollBarVisibility GetHorizontalScrollBarVisibility (this ListView pListView)
		{
			ScrollViewer lScrollHost = pListView.Template.FindName ("ScrollHost", pListView) as ScrollViewer;
			if (lScrollHost != null)
			{
				return lScrollHost.HorizontalScrollBarVisibility;
			}
			return ScrollBarVisibility.Auto;
		}

		public static Boolean SetHorizontalScrollBarVisibility (this ListView pListView, ScrollBarVisibility pScrollBarVisibility)
		{
			ScrollViewer lScrollHost = pListView.Template.FindName ("ScrollHost", pListView) as ScrollViewer;
			if (lScrollHost != null)
			{
				lScrollHost.HorizontalScrollBarVisibility = pScrollBarVisibility;
				return true;
			}
			return false;
		}
	}
}
