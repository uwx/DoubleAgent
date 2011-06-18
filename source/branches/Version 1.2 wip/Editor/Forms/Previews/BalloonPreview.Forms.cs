using System;
using System.Windows.Forms;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Previews
{
	public partial class BalloonPreview : UserControl
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		private CharacterFile mCharacterFile;
		private DoubleAgent.BalloonPreview mBalloonPreview = new DoubleAgent.BalloonPreview ();
		private Timer mAutoPaceTimer = null;
		private Timer mAutoScrollTimer = null;

		public BalloonPreview ()
		{
			InitializeComponent ();
			DoubleBuffered = true;
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

			if (Enabled && Visible)
			{
				Int32 lAutoPaceTime = mBalloonPreview.AutoPaceTime;

				if (lAutoPaceTime > 0)
				{
					mAutoPaceTimer = new Timer ();
					mAutoPaceTimer.Tick += new EventHandler (AutoPaceTimer_Tick);
					mAutoPaceTimer.Interval = (int)lAutoPaceTime;
					mAutoPaceTimer.Start ();
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
				mAutoPaceTimer.Dispose ();
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

			if (Enabled && Visible)
			{
				Int32 lAutoScrollTime = mBalloonPreview.AutoScrollTime;

				if (lAutoScrollTime > 0)
				{
					mAutoScrollTimer = new Timer ();
					mAutoScrollTimer.Tick += new EventHandler (AutoScrollTimer_Tick);
					mAutoScrollTimer.Interval = (int)lAutoScrollTime;
					mAutoScrollTimer.Start ();
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
				mAutoScrollTimer.Dispose ();
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

		void AutoPaceTimer_Tick (object sender, EventArgs e)
		{
			Boolean lRefresh = false;

			if (Enabled && Visible && mBalloonPreview.OnAutoPace (ref lRefresh))
			{
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
				StopAutoPace ();
			}
		}

		void AutoScrollTimer_Tick (object sender, EventArgs e)
		{
			Boolean lRefresh = false;

			if (Enabled && Visible && mBalloonPreview.OnAutoScroll (ref lRefresh))
			{
				if (lRefresh)
				{
					Refresh ();
				}
			}
			else
			{
				StopAutoScroll ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override void OnPaint (PaintEventArgs e)
		{
			// Pause AutoPace while drawing
			if (mAutoPaceTimer != null)
			{
				mAutoPaceTimer.Stop ();
			}

			e.Graphics.Clear (BackColor);
			mBalloonPreview.Draw (e.Graphics);

			if (mAutoPaceTimer != null)
			{
				mAutoPaceTimer.Start ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override void OnEnabledChanged (EventArgs e)
		{
			base.OnEnabledChanged (e);
			CharacterFile = CharacterFile;
		}

		protected override void OnVisibleChanged (EventArgs e)
		{
			base.OnVisibleChanged (e);
			CharacterFile = CharacterFile;
		}

		#endregion
	}
}
