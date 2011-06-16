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
using System.Windows.Controls.Primitives;
using System.Windows.Input;

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
			return new System.Windows.Size (pSize.Width, pSize.Height);
		}
		static public System.Windows.Size ToWPF (this System.Drawing.SizeF pSize)
		{
			return new System.Windows.Size (pSize.Width, pSize.Height);
		}
		static public System.Drawing.SizeF FromWPF (this System.Windows.Size pSize)
		{
			return new System.Drawing.SizeF ((Single)pSize.Width, (Single)pSize.Height);
		}

		static public System.Windows.Rect ToWPF (this System.Drawing.Rectangle pRect)
		{
			return new System.Windows.Rect (pRect.X, pRect.Y, pRect.Width, pRect.Height);
		}
		static public System.Windows.Rect ToWPF (this System.Drawing.RectangleF pRect)
		{
			return new System.Windows.Rect (pRect.X, pRect.Y, pRect.Width, pRect.Height);
		}
		static public System.Drawing.RectangleF FromWPF (this System.Windows.Rect pRect)
		{
			return new System.Drawing.RectangleF ((Single)pRect.X, (Single)pRect.Y, (Single)pRect.Width, (Single)pRect.Height);
		}

		static public System.Windows.Media.Color ToWPF (this System.Drawing.Color pColor)
		{
			return System.Windows.Media.Color.FromArgb (pColor.A, pColor.R, pColor.G, pColor.B);
		}
		static public System.Windows.Media.Brush ToWPFBrush (this System.Drawing.Color pColor)
		{
			return new System.Windows.Media.SolidColorBrush (pColor.ToWPF ());
		}
		static public System.Drawing.Color FromWPF (this System.Windows.Media.Color pColor)
		{
			return System.Drawing.Color.FromArgb (pColor.A, pColor.R, pColor.G, pColor.B);
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public static partial class ImageExtensions
	{
		static public System.Windows.Size ScaleToScreenResolution (this System.Windows.Size pSize)
		{
			try
			{
				Size lResolution = ImageExtensions.GuessScreenResolution ().ToWPF ();
				return new Size (pSize.Width * 96.0 / lResolution.Width, pSize.Height * 96.0 / lResolution.Height);
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
			return pSize;
		}

		static public System.Windows.Size TransformToScreenResolution (this System.Windows.Size pSize, System.Windows.Media.GeneralTransform pTransform)
		{
			try
			{
				Size lResolution = ImageExtensions.GuessScreenResolution ().ToWPF ();
				Point lTransformed = pTransform.Transform (new Point (pSize.Width * 96.0 / lResolution.Width, pSize.Height * 96.0 / lResolution.Height));
				return new Size (lTransformed.X, lTransformed.Y);
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
			return pSize;
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public static partial class ListViewExtensions
	{
		public static void BeginUpdate (this ListView pListView)
		{
		}

		public static void EndUpdate (this ListView pListView)
		{
			pListView.InvalidateArrange ();
		}

		public static void Clear (this ListView pListView)
		{
			pListView.Items.Clear ();
		}

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

		///////////////////////////////////////////////////////////////////////////////

		public static ListViewItem GetSelectedItem (this ListView pListView, Boolean pIncludeFocus)
		{
			return pListView.SelectedItem as ListViewItem;
		}

		public static int GetSelectedIndex (this ListView pListView, Boolean pIncludeFocus)
		{
			return pListView.SelectedIndex;
		}

		///////////////////////////////////////////////////////////////////////////////

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
			ScrollViewer lScrollHost = null;
			if (pListView.Template != null)
			{
				lScrollHost = pListView.Template.FindName ("ScrollHost", pListView) as ScrollViewer;
			}
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
			ScrollViewer lScrollHost = null;
			if (pListView.Template != null)
			{
				lScrollHost = pListView.Template.FindName ("ScrollHost", pListView) as ScrollViewer;
			}
			if (lScrollHost != null)
			{
				lScrollHost.HorizontalScrollBarVisibility = pScrollBarVisibility;
				return true;
			}
			return false;
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public static partial class TreeViewExtensions
	{
		public static void SetSelectedItem (this TreeView pTreeView, TreeViewItem pTreeViewItem)
		{
			SetSelectedItem (pTreeView, pTreeViewItem, true);
		}

		public static void SetSelectedItem (this TreeView pTreeView, TreeViewItem pTreeViewItem, Boolean pEnsureVisible)
		{
			if (pTreeViewItem != pTreeView.SelectedItem)
			{
				TreeViewItem lSelectedItem = pTreeView.SelectedItem as TreeViewItem;
				TreeViewItem lParentItem;
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("OldSelectedItem [{0}]", lSelectedItem);
				System.Diagnostics.Debug.Print ("OldFocusedItem  [{0}]", FocusManager.GetFocusedElement (FocusManager.GetFocusScope (pTreeView)));
				System.Diagnostics.Debug.Print ("SetSelectedItem [{0}]", pTreeViewItem);
#endif

				if (pTreeViewItem != null)
				{
					for (lParentItem = pTreeViewItem.Parent as TreeViewItem; lParentItem != null; lParentItem = lParentItem.Parent as TreeViewItem)
					{
						lParentItem.IsExpanded = true;
					}
					if (pEnsureVisible)
					{
						pTreeViewItem.BringIntoView ();
					}
					pTreeViewItem.IsSelected = true;
				}
				if (lSelectedItem != null)
				{
					lSelectedItem.IsSelected = false;
				}
#if DEBUG_NOT

				System.Diagnostics.Debug.Print ("NewSelectedItem [{0}]", lSelectedItem);
				System.Diagnostics.Debug.Print ("NewFocusedItem  [{0}]", FocusManager.GetFocusedElement (FocusManager.GetFocusScope (pTreeView)));
#endif
				if (pTreeView.IsKeyboardFocusWithin && (pTreeView.SelectedItem != null))
				{
					(pTreeView.SelectedItem as TreeViewItem).Focus ();
				}
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("NewFocusedItem  [{0}]", FocusManager.GetFocusedElement (FocusManager.GetFocusScope (pTreeView)));
#endif
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public static partial class ComboBoxExtensions
	{
		public static void BeginUpdate (this ComboBox pComboBox)
		{
		}

		public static void EndUpdate (this ComboBox pComboBox)
		{
			pComboBox.InvalidateArrange ();
		}

		public static int FindStringExact (this ComboBox pComboBox, String pString)
		{
			foreach (Object lItem in pComboBox.Items)
			{
				if ((lItem is String) && ((lItem as String) == pString))
				{
					return pComboBox.Items.IndexOf (lItem);
				}
				else if ((lItem is ComboBoxItem) && ((lItem as ComboBoxItem).Content.ToString () == pString))
				{
					return pComboBox.Items.IndexOf (lItem);
				}
			}
			return -1;
		}
	}

	///////////////////////////////////////////////////////////////////////////////
#if DEBUG
	///////////////////////////////////////////////////////////////////////////////

	public static partial class DebuggingExtensions
	{
		public static void DebugPrint (this UIElement pRootElement)
		{
			DebugPrint (pRootElement, String.Empty);
		}

		public static void DebugPrint (this UIElement pRootElement, String pIndent)
		{
			DebugPrint (pRootElement, String.Empty, String.Empty);
		}

		public static void DebugPrint (this UIElement pRootElement, String pTitle, String pIndent)
		{
			String lTitle = String.IsNullOrEmpty (pTitle) ? String.Empty : pTitle + " ";

			if (pRootElement == null)
			{
				System.Diagnostics.Debug.Print ("{0}{1}<null>", pIndent, lTitle);
			}
			else
			{
				if (pRootElement is FrameworkElement)
				{
					System.Diagnostics.Debug.Print ("{0}{1}{2} [{3}] [{4}] Size [{5} {6}] Margin [{7} {8} {9} {10}]", pIndent, lTitle, pRootElement.GetType ().Name, (pRootElement as FrameworkElement).Name, pRootElement.Visibility, Math.Round ((pRootElement as FrameworkElement).ActualWidth), Math.Round ((pRootElement as FrameworkElement).ActualHeight), (pRootElement as FrameworkElement).Margin.Left, (pRootElement as FrameworkElement).Margin.Top, (pRootElement as FrameworkElement).Margin.Right, (pRootElement as FrameworkElement).Margin.Bottom);
				}
				else
				{
					System.Diagnostics.Debug.Print ("{0}{1}{2} [{3}]", pIndent, lTitle, pRootElement.GetType ().Name, pRootElement.Visibility);
				}

				if (pRootElement is Label)
				{
					System.Diagnostics.Debug.Print ("{0}  Label [{1}]", pIndent, (pRootElement as Label).Content);
				}
				else if (pRootElement is TextBox)
				{
					System.Diagnostics.Debug.Print ("{0}  Text [{1}]", pIndent, (pRootElement as TextBox).Text);
				}
				else if (pRootElement is ToggleButton)
				{
					System.Diagnostics.Debug.Print ("{0}  Checked [{1}] Content [{2}]", pIndent, (pRootElement as ToggleButton).IsChecked, (pRootElement as ToggleButton).Content);
				}
				else if (pRootElement is ContentControl)
				{
					DebugPrint ((pRootElement as ContentControl).Content as UIElement, "Content", pIndent + "  ");
				}
				else if (pRootElement is Panel)
				{
					UIElementCollection lChildren = (pRootElement as Panel).Children;
					String lIndent = pIndent + "  ";

					foreach (UIElement lChild in lChildren)
					{
						DebugPrint (lChild, "Child", lIndent);
					}
				}
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////
#endif
	///////////////////////////////////////////////////////////////////////////////
}
