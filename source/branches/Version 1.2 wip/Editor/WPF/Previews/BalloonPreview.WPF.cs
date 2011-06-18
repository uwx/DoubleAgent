using System;
using System.ComponentModel;
using System.Drawing;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using DoubleAgent;
using DoubleAgent.Character;
using AgentCharacterEditor.Global;

namespace AgentCharacterEditor.Previews
{
	public partial class BalloonPreview : System.Windows.Controls.Canvas
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		private CharacterFile mCharacterFile;
		private DoubleAgent.BalloonPreview mBalloonPreview = new DoubleAgent.BalloonPreview ();
		private Visual mBalloonVisual = null;
		private AsyncTimer mAutoPaceTimer = null;
		private AsyncTimer mAutoScrollTimer = null;

		public BalloonPreview ()
		{
			ClipToBounds = true;
			IsEnabledChanged += new DependencyPropertyChangedEventHandler (HandleIsEnabledChanged);
			IsVisibleChanged += new DependencyPropertyChangedEventHandler (HandleIsVisibleChanged);
		}

		~BalloonPreview ()
		{
			StopAutoPace ();
			StopAutoScroll ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public CharacterFile CharacterFile
		{
			get
			{
				return mCharacterFile;
			}
			set
			{
				if (mCharacterFile != value)
				{
					StopAutoPace ();
					StopAutoScroll ();
				}

				mCharacterFile = value;

				if (mCharacterFile == null)
				{
					StopAutoPace ();
					StopAutoScroll ();
					mBalloonPreview.Style = CharacterStyle.None;
					mBalloonPreview.Balloon = null;
				}
				else
				{
					mBalloonPreview.Style = mCharacterFile.Header.Style;
					mBalloonPreview.Balloon = ((mCharacterFile.Header.Style & CharacterStyle.Balloon) != CharacterStyle.None) ? mCharacterFile.Balloon : null;

					if (!IsAutoPacing)
					{
						StartAutoPace ();
					}
					if (!IsAutoScrolling)
					{
						StartAutoScroll ();
					}
				}

				Refresh ();
			}
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public DoubleAgent.BalloonPreview Preview
		{
			get
			{
				return mBalloonPreview;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		private bool IsAutoPacing
		{
			get
			{
				return mAutoPaceTimer != null;
			}
		}

		private bool StartAutoPace ()
		{
			bool lRet = false;

			if (IsEnabled && IsVisible)
			{
				Int32 lAutoPaceTime = mBalloonPreview.AutoPaceTime;

				if (lAutoPaceTime > 0)
				{
					StopAutoPace ();
#if DEBUG_NOT
					System.Diagnostics.Debug.Print ("StartAutoPace [{0}]", lAutoPaceTime);
#endif
					mAutoPaceTimer = new AsyncTimer ();
					mAutoPaceTimer.TimerPulse += new AsyncTimer.TimerPulseHandler (AutoPaceTimerPulse);
					mAutoPaceTimer.Start (lAutoPaceTime, lAutoPaceTime);
					lRet = true;
				}
			}
			if (lRet)
			{
				if (mBalloonPreview.AutoPaceStarted ())
				{
					Refresh ();
				}
			}
			else
			{
				StopAutoPace ();
			}
			return lRet;
		}

		private bool StopAutoPace ()
		{
			bool lRet = false;

			if (mAutoPaceTimer != null)
			{
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("StopAutoPaceTimer");
#endif
				try
				{
					mAutoPaceTimer.Dispose ();
				}
				catch
				{
				}
				mAutoPaceTimer = null;
				lRet = true;
			}
			if (lRet && mBalloonPreview.AutoPaceStopped ())
			{
				Refresh ();
			}
			return lRet;
		}

		///////////////////////////////////////////////////////////////////////////////

		private bool IsAutoScrolling
		{
			get
			{
				return mAutoScrollTimer != null;
			}
		}

		private bool StartAutoScroll ()
		{
			bool lRet = false;

			if (IsEnabled && IsVisible)
			{
				Int32 lAutoScrollTime = mBalloonPreview.AutoScrollTime;

				if (lAutoScrollTime > 0)
				{
					StopAutoScroll ();
#if DEBUG_NOT
					System.Diagnostics.Debug.Print ("StartAutoScroll [{0}]", lAutoScrollTime);
#endif
					mAutoScrollTimer = new AsyncTimer ();
					mAutoScrollTimer.TimerPulse += new AsyncTimer.TimerPulseHandler (AutoScrollTimerPulse);
					mAutoScrollTimer.Start (lAutoScrollTime, lAutoScrollTime);
					lRet = true;
				}
			}
			if (lRet)
			{
				if (mBalloonPreview.AutoScrollStarted ())
				{
					Refresh ();
				}
			}
			else
			{
				StopAutoScroll ();
			}
			return lRet;
		}

		private bool StopAutoScroll ()
		{
			bool lRet = false;

			if (mAutoScrollTimer != null)
			{
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("StopAutoScrollTimer");
#endif
				try
				{
					mAutoScrollTimer.Dispose ();
				}
				catch
				{
				}
				mAutoScrollTimer = null;
				lRet = true;
			}
			if (lRet && mBalloonPreview.AutoScrollStopped ())
			{
				Refresh ();
			}
			return lRet;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void AutoPaceTimerPulse (object sender, AsyncTimer.TimerEventArgs e)
		{
			Boolean lRefresh = false;
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("AutoPaceTimerPulse");
#endif
			if (IsEnabled && IsVisible && mBalloonPreview.OnAutoPace (ref lRefresh))
			{
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("AutoPaceTimerPulse [{0}]", lRefresh);
#endif
				if (lRefresh)
				{
					Refresh ();
				}
				if (!IsAutoScrolling)
				{
					StartAutoScroll ();
				}
			}
			else
			{
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("AutoPaceTimerTick END");
#endif
				StopAutoPace ();
			}
		}

		private void AutoScrollTimerPulse (object sender, AsyncTimer.TimerEventArgs e)
		{
			Boolean lRefresh = false;
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("AutoScrollTimerPulse");
#endif
			if (IsEnabled && IsVisible && mBalloonPreview.OnAutoScroll (ref lRefresh))
			{
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("AutoScrollTimerPulse [{0}]", lRefresh);
#endif
				if (lRefresh)
				{
					StopAutoPace (); // Restart AutoPace every time we scroll (so we don't get ahead of ourselves)
					Refresh ();
					StartAutoPace ();
				}
			}
			else
			{
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("AutoScrollTimerPulse END");
#endif
				StopAutoScroll ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected void Refresh ()
		{
#if DEBUG_NOT
            System.Diagnostics.Debug.Print ("Refresh [{0} {1}] [{2}]", ActualWidth, ActualHeight, RenderSize);
#endif
			try
			{
				if (mBalloonVisual != null)
				{
					RemoveVisualChild (mBalloonVisual);
				}
				if ((mBalloonVisual = mBalloonPreview.MakeVisual ()) != null)
				{
					AddVisualChild (mBalloonVisual);
				}
			}
#if DEBUG
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
#else
            catch {}
#endif
		}

		protected override int VisualChildrenCount
		{
			get
			{
				int lRet = base.VisualChildrenCount;
				if (mBalloonVisual != null)
				{
					lRet++;
				}
				return lRet;
			}
		}

		protected override Visual GetVisualChild (int index)
		{
			if (index == base.VisualChildrenCount)
			{
				return mBalloonVisual;
			}
			return base.GetVisualChild (index);
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override System.Windows.Size MeasureOverride (System.Windows.Size constraint)
		{
			base.MeasureOverride (constraint);
			return constraint;
		}

		protected override System.Windows.Size ArrangeOverride (System.Windows.Size arrangeSize)
		{
			base.ArrangeOverride (arrangeSize);
			return arrangeSize;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void HandleIsEnabledChanged (object sender, DependencyPropertyChangedEventArgs e)
		{
			CharacterFile = CharacterFile;
		}

		private void HandleIsVisibleChanged (object sender, DependencyPropertyChangedEventArgs e)
		{
			CharacterFile = CharacterFile;
		}

		#endregion
	}
}
