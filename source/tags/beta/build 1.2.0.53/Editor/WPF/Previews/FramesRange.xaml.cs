using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using AgentCharacterEditor.Global;

namespace AgentCharacterEditor.Previews
{
	public partial class FramesRange : System.Windows.Controls.ContentControl, INotifyPropertyChanged
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		Dictionary<int, Point> mTicksMap = new Dictionary<int, Point> ();
		Boolean mIsUpdating = false;

		public FramesRange ()
		{
			InitializeComponent ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public int SelectionStart
		{
			get
			{
				int lSelection = 0;

				if (mTicksMap != null)
				{
					Double lTick = SliderStart.Value;
					foreach (KeyValuePair<int, Point> lTicks in mTicksMap)
					{
						if (lTicks.Value.X <= lTick)
						{
							lSelection = lTicks.Key;
						}
						else
						{
							break;
						}
					}
				}
				return lSelection;
			}
			set
			{
				if (value != SelectionStart)
				{
					ShowSelectionRange (value, SelectionEnd);
				}
			}
		}

		public int SelectionEnd
		{
			get
			{
				int lSelection = -1;

				if (ListView != null)
				{
					lSelection = ListView.Items.Count - 1;
				}
				if (mTicksMap != null)
				{
					Double lTick = SliderEnd.Value;
					foreach (KeyValuePair<int, Point> lTicks in mTicksMap)
					{
						if (lTicks.Value.X <= lTick)
						{
							lSelection = lTicks.Key;
						}
						else
						{
							break;
						}
					}
				}
				return lSelection;
			}
			set
			{
				if (value != SelectionEnd)
				{
					ShowSelectionRange (SelectionStart, value);
				}
			}
		}

		public FramesListView ListView
		{
			get;
			set;
		}

		//=============================================================================

		public String SelectionStartText
		{
			get
			{
				return String.Format ("Start the preview at frame {0}", SelectionStart+1);
			}
		}

		public String SelectionEndText
		{
			get
			{
				return String.Format ("End the preview at frame {0}", SelectionEnd+1);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public event PropertyChangedEventHandler PropertyChanged;

		public void SelectionStartChanged ()
		{
			if (PropertyChanged != null)
			{
				try
				{
					PropertyChanged (this, new PropertyChangedEventArgs ("SelectionStart"));
					PropertyChanged (this, new PropertyChangedEventArgs ("SelectionStartText"));
				}
				catch
				{
				}
			}
		}

		public void SelectionEndChanged ()
		{
			if (PropertyChanged != null)
			{
				try
				{
					PropertyChanged (this, new PropertyChangedEventArgs ("SelectionEnd"));
					PropertyChanged (this, new PropertyChangedEventArgs ("SelectionEndText"));
				}
				catch
				{
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		public Boolean RecalcLayout ()
		{
			try
			{
				int lSelectionStart = SelectionStart;
				int lSelectionEnd = SelectionEnd;

				SliderStart.Minimum = 0;
				SliderStart.Maximum = 0;
				SliderStart.Ticks.Clear ();
				SliderEnd.Minimum = 0;
				SliderEnd.Maximum = 0;
				SliderEnd.Ticks.Clear ();

				mTicksMap.Clear ();

				if ((ListView != null) && (ListView.Items.Count > 1))
				{
					Thickness lMargin = new Thickness (ListView.Margin.Left, Margin.Top, ListView.Margin.Right, Margin.Bottom);
					Thickness lPadding = new Thickness (ListView.Padding.Left, 0, ListView.Padding.Right, 0);
					FramesListItem lEndItem;
					Point lTickPos = new Point ();
					int lItemNdx = 0;

					lEndItem = ListView.Items[0] as FramesListItem;
					lPadding.Left += lEndItem.Margin.Left + lEndItem.ActualWidth / 2.0;
					lEndItem = ListView.Items[ListView.Items.Count - 1] as FramesListItem;
					lPadding.Right += lEndItem.Margin.Right + lEndItem.ActualWidth / 2.0;
					Margin = lMargin;
					SliderGrid.Margin = lPadding;

#if DEBUG_NOT
					System.Diagnostics.Debug.Print ("Items {0}", ListView.Items.Count);
#endif
					foreach (FramesListItem lListItem in ListView.Items)
					{
						if (lItemNdx > 0)
						{
							lTickPos.X += lListItem.Margin.Left;
							lTickPos.X += lListItem.ActualWidth / 2.0;
						}
#if DEBUG_NOT
						System.Diagnostics.Debug.Print ("  Tick {0} {1}", lItemNdx, lTickPos.X);
#endif
						mTicksMap[lItemNdx++] = lTickPos;
						SliderStart.Ticks.Add (lTickPos.X);
						SliderEnd.Ticks.Add (lTickPos.X);
						lTickPos.X += lListItem.ActualWidth / 2.0;
						lTickPos.X += lListItem.Margin.Right;
					}

					ShowSelectionRange (lSelectionStart, lSelectionEnd);
					return true;
				}
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
			return false;
		}

		private void ShowSelectionRange (int pSelectionStart, int pSelectionEnd)
		{
			if (!mIsUpdating && (mTicksMap.Count > 1))
			{
				int lSelectionStart = SelectionStart;
				int lSelectionEnd = SelectionEnd;

				mIsUpdating = true;

				try
				{
					pSelectionStart = Math.Min (Math.Max (pSelectionStart, 0), mTicksMap.Count - 2);
					pSelectionEnd = Math.Min (Math.Max (pSelectionEnd, SelectionStart + 1), mTicksMap.Count - 1);
#if DEBUG_NOT
					System.Diagnostics.Debug.Print ("ShowSelectionRange {0}-{1}", pSelectionStart, pSelectionEnd);
#endif
					SliderStart.Minimum = mTicksMap[0].X;
					SliderStart.Maximum = mTicksMap[pSelectionEnd].X;
					SliderStart.Value = mTicksMap[pSelectionStart].X;

					SliderEnd.Minimum = mTicksMap[pSelectionStart].X;
					SliderEnd.Maximum = mTicksMap[mTicksMap.Count - 1].X;
					SliderEnd.Value = mTicksMap[pSelectionEnd].X;

					SliderGrid.ColumnDefinitions[0].Width = new GridLength (SliderStart.Value);
					SliderGrid.ColumnDefinitions[1].Width = new GridLength (SliderEnd.Value - SliderStart.Value);
					SliderGrid.ColumnDefinitions[2].Width = new GridLength (SliderEnd.Maximum - SliderEnd.Value);
					SliderGrid.UpdateLayout ();
#if DEBUG_NOT
					System.Diagnostics.Debug.Print ("  Start {0} Minimum {1} Maximum {2} Width {3}", SliderStart.Value, SliderStart.Minimum, SliderStart.Maximum, SliderStart.ActualWidth);
					System.Diagnostics.Debug.Print ("  End   {0} Minimum {1} Maximum {2} Width {3}", SliderEnd.Value, SliderEnd.Minimum, SliderEnd.Maximum, SliderEnd.ActualWidth);
					System.Diagnostics.Debug.Print ("  Grid  {0} [{1} {2} {3}]", SliderGrid.ActualWidth, SliderGrid.ColumnDefinitions[0].ActualWidth, SliderGrid.ColumnDefinitions[1].ActualWidth, SliderGrid.ColumnDefinitions[2].ActualWidth);
#endif
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}

				if (SelectionStart != lSelectionStart)
				{
					SelectionStartChanged ();
				}
				if (SelectionEnd != lSelectionEnd)
				{
					SelectionEndChanged ();
				}

				mIsUpdating = false;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void SliderStart_ValueChanged (object sender, RoutedPropertyChangedEventArgs<double> e)
		{
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("  Start Changed {0} to {1} Selection {2}-{3}", e.OldValue, e.NewValue, SelectionStart, SelectionEnd);
#endif
			ShowSelectionRange (SelectionStart, SelectionEnd);
			SelectionStartChanged ();
		}

		private void SliderEnd_ValueChanged (object sender, RoutedPropertyChangedEventArgs<double> e)
		{
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("  End   Changed {0} to {1} Selection {2}-{3}", e.OldValue, e.NewValue, SelectionStart, SelectionEnd);
#endif
			ShowSelectionRange (SelectionStart, SelectionEnd);
			SelectionEndChanged ();
		}

		private void SliderGrid_RequestBringIntoView (object sender, RequestBringIntoViewEventArgs e)
		{
			e.Handled = true;
		}

		#endregion
	}
}
