using System;
using System.ComponentModel;
using System.Drawing;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using DoubleAgent.Character;
using AgentCharacterEditor.Global;

namespace AgentCharacterEditor.Previews
{
	public partial class BalloonPreview : System.Windows.Controls.Image
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		private CharacterFile mCharacterFile;
		private DoubleAgent.BalloonPreview mBalloonPreview = new DoubleAgent.BalloonPreview ();
		private Timer mAutoPaceTimer = null;
		private Timer mAutoScrollTimer = null;
		private AsyncOperation mAutoPaceAsync = null;
		private AsyncOperation mAutoScrollAsync = null;

		public BalloonPreview ()
		{
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
				mCharacterFile = value;
				if (mCharacterFile == null)
				{
					mBalloonPreview.Style = CharacterStyle.None;
					mBalloonPreview.Balloon = null;
					StopAutoPace ();
					StopAutoScroll ();
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
					mAutoPaceTimer = new Timer (AutoPaceTimerCallback, mAutoPaceAsync = AsyncOperationManager.CreateOperation (this), lAutoPaceTime, lAutoPaceTime);
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
			if (mAutoPaceAsync != null)
			{
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("StopAutoPaceAsync");
#endif
				try
				{
					mAutoPaceAsync.OperationCompleted ();
				}
				catch
				{
				}
				mAutoPaceAsync = null;
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
					mAutoScrollTimer = new Timer (AutoScrollTimerCallback, mAutoScrollAsync = AsyncOperationManager.CreateOperation (this), lAutoScrollTime, lAutoScrollTime);
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
			if (mAutoScrollAsync != null)
			{
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("StopAutoScrollAsync");
#endif
				try
				{
					mAutoScrollAsync.OperationCompleted ();
				}
				catch
				{
				}
				mAutoScrollAsync = null;
			}
			if (lRet && mBalloonPreview.AutoScrollStopped ())
			{
				Refresh ();
			}
			return lRet;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void AutoPaceTimerCallback (Object state)
		{
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("AutoPaceTimerCallback");
#endif
			try
			{
				(state as AsyncOperation).Post (AutoPaceTimerTick, null);
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

		private void AutoPaceTimerTick (Object state)
		{
			Boolean lRefresh = false;
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("AutoPaceTimerTick");
#endif
			if (IsEnabled && IsVisible && mBalloonPreview.OnAutoPace (ref lRefresh))
			{
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("AutoPaceTimerTick [{0}]", lRefresh);
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

		private void AutoScrollTimerCallback (Object state)
		{
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("AutoScrollTimerCallback");
#endif
			try
			{
				(state as AsyncOperation).Post (AutoScrollTimerTick, null);
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

		private void AutoScrollTimerTick (Object state)
		{
			Boolean lRefresh = false;
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("AutoScrollTimerTick");
#endif
			if (IsEnabled && IsVisible && mBalloonPreview.OnAutoScroll (ref lRefresh))
			{
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("AutoScrollTimerTick [{0}]", lRefresh);
#endif
				if (lRefresh)
				{
					Refresh ();
				}
			}
			else
			{
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("AutoScrollTimerTick END");
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
				Drawing lDrawing = mBalloonPreview.MakeDrawing ();
				Source = new DrawingImage (lDrawing);
				RenderTransform = new ScaleTransform (Source.Width / ActualWidth, Source.Height / ActualHeight);
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
