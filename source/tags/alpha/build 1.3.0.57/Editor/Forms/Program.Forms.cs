/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
using System.Collections.Generic;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Imaging;
using System.Reflection;
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor
{
	static public partial class Program
	{
		[STAThread]
		static void Main (String[] args)
		{
			Application.EnableVisualStyles ();
			Application.SetCompatibleTextRenderingDefault (false);

#if DEBUG
			{
				String lLogName = System.IO.Path.Combine (System.Environment.GetFolderPath (Environment.SpecialFolder.DesktopDirectory), "DoubleACE.Log");
				System.IO.Stream lLogStream = new System.IO.FileStream (lLogName, System.IO.FileMode.Create, System.IO.FileAccess.ReadWrite, System.IO.FileShare.ReadWrite);
				System.Diagnostics.Debug.Listeners.Add (new System.Diagnostics.TextWriterTraceListener (lLogStream));
				System.Diagnostics.Debug.AutoFlush = true;
			}
#endif
			Program.UndoManager = new UndoManager ();
			Program.MainWindow = new MainWindow (args);
			Program.MainWindow.Initialize ();

			Application.Run (Program.MainWindow);

#if DEBUG
			System.Diagnostics.Debug.Flush ();
#endif
		}

		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		static internal MainWindow MainWindow
		{
			get;
			private set;
		}

		static internal UndoManager UndoManager
		{
			get;
			private set;
		}

		static internal Boolean FileIsReadOnly
		{
			get
			{
				return ((MainWindow.CharacterFile == null) || (MainWindow.CharacterFile.IsReadOnly));
			}
		}

		static internal Boolean FileIsDirty
		{
			get
			{
				return ((MainWindow.CharacterFile != null) && (MainWindow.CharacterFile.IsDirty));
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Message Boxes

		public static void ShowInfoMessage (String pMessage)
		{
			MessageBox.Show (pMessage, Program.AssemblyTitle, MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

		public static void ShowWarningMessage (String pMessage)
		{
			MessageBox.Show (pMessage, Program.AssemblyTitle, MessageBoxButtons.OK, MessageBoxIcon.Warning);
		}

		public static void ShowErrorMessage (String pMessage)
		{
			MessageBox.Show (pMessage, Program.AssemblyTitle, MessageBoxButtons.OK, MessageBoxIcon.Error);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Assembly Attribute Accessors

		static public String AssemblyTitle
		{
			get
			{
				object[] attributes = Assembly.GetExecutingAssembly ().GetCustomAttributes (typeof (AssemblyTitleAttribute), false);
				if (attributes.Length > 0)
				{
					AssemblyTitleAttribute titleAttribute = (AssemblyTitleAttribute)attributes[0];
					if (titleAttribute.Title != "")
					{
						return titleAttribute.Title;
					}
				}
				return System.IO.Path.GetFileNameWithoutExtension (Assembly.GetExecutingAssembly ().CodeBase);
			}
		}

		static public String AssemblyVersion
		{
			get
			{
				return Assembly.GetExecutingAssembly ().GetName ().Version.ToString ();
			}
		}

		static public String AssemblyDescription
		{
			get
			{
				object[] attributes = Assembly.GetExecutingAssembly ().GetCustomAttributes (typeof (AssemblyDescriptionAttribute), false);
				if (attributes.Length == 0)
				{
					return "";
				}
				return ((AssemblyDescriptionAttribute)attributes[0]).Description;
			}
		}

		static public String AssemblyProduct
		{
			get
			{
				object[] attributes = Assembly.GetExecutingAssembly ().GetCustomAttributes (typeof (AssemblyProductAttribute), false);
				if (attributes.Length == 0)
				{
					return "";
				}
				return ((AssemblyProductAttribute)attributes[0]).Product;
			}
		}

		static public String AssemblyCopyright
		{
			get
			{
				object[] attributes = Assembly.GetExecutingAssembly ().GetCustomAttributes (typeof (AssemblyCopyrightAttribute), false);
				if (attributes.Length == 0)
				{
					return "";
				}
				return ((AssemblyCopyrightAttribute)attributes[0]).Copyright;
			}
		}

		static public String AssemblyCompany
		{
			get
			{
				object[] attributes = Assembly.GetExecutingAssembly ().GetCustomAttributes (typeof (AssemblyCompanyAttribute), false);
				if (attributes.Length == 0)
				{
					return "";
				}
				return ((AssemblyCompanyAttribute)attributes[0]).Company;
			}
		}
		#endregion
	}
}
