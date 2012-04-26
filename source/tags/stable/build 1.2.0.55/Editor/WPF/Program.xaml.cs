﻿using System;
using System.Collections.Generic;
using System.Configuration;
using System.Diagnostics;
using System.Windows;
using System.Windows.Media;
using System.Windows.Interop;
using System.Reflection;
using DoubleAgent;
using DoubleAgent.Theme;

namespace AgentCharacterEditor
{
	public partial class Program : Application
	{
		public Program ()
		{
#if DEBUG
			{
				String lLogName = System.IO.Path.Combine (System.Environment.GetFolderPath (Environment.SpecialFolder.DesktopDirectory), "DoubleACE.Log");
				System.IO.Stream lLogStream = new System.IO.FileStream (lLogName, System.IO.FileMode.Create, System.IO.FileAccess.ReadWrite, System.IO.FileShare.ReadWrite);
				Debug.Listeners.Add (new TextWriterTraceListener (lLogStream));
				Debug.AutoFlush = true;
			}
#endif
			Program.UndoManager = new UndoManager ();

#if DEBUG_NOT
			InitializeComponentDebug ();
			DumpResources (Application.Current.Resources, String.Empty);
#endif
		}

		private void Application_Startup (object sender, StartupEventArgs e)
		{
			CommandLineArgs = e.Args;
		}

		///////////////////////////////////////////////////////////////////////////////
		#region Initialization Debugging
#if DEBUG
		private void InitializeComponentDebug ()
		{
			try
			{
				this.Startup += new System.Windows.StartupEventHandler (this.Application_Startup);
				this.StartupUri = new System.Uri ("MainWindow.xaml", System.UriKind.Relative);
				System.Uri resourceLocater = new System.Uri ("/DoubleACE;component/program.xaml", System.UriKind.Relative);
				System.Windows.Application.LoadComponent (this, resourceLocater);
				_contentLoaded = true;
			}
			catch (Exception pException)
			{
				Debug.Print (pException.Message);
			}
		}

		private static void DumpResources (ResourceDictionary pDict, String pIndent)
		{
			try
			{
				Debug.Print ("{0}Resources [{1}] [{2}]", pIndent, pDict.Keys.Count, pDict.Source);
				foreach (Object lKey in pDict.Keys)
				{
					Debug.Print ("{0}  Key [{1}]", pIndent, lKey);
				}
				Debug.Print ("{0}Merged [{1}]", pIndent, pDict.MergedDictionaries.Count);
				foreach (ResourceDictionary lDict in pDict.MergedDictionaries)
				{
					DumpResources (lDict, pIndent + "  ");
				}
				Debug.Print ("{0}Resources Done [{1}]", pIndent, pDict.Source);
			}
			catch (Exception pException)
			{
				Debug.Print (pException.Message);
			}
		}
#endif
		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		static internal String[] CommandLineArgs
		{
			get;
			private set;
		}

		static internal new MainWindow MainWindow
		{
			get;
			set;
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
			MessageBox.Show (pMessage, Program.AssemblyTitle, MessageBoxButton.OK, MessageBoxImage.Information);
		}

		public static void ShowWarningMessage (String pMessage)
		{
			MessageBox.Show (pMessage, Program.AssemblyTitle, MessageBoxButton.OK, MessageBoxImage.Warning);
		}

		public static void ShowErrorMessage (String pMessage)
		{
			MessageBox.Show (pMessage, Program.AssemblyTitle, MessageBoxButton.OK, MessageBoxImage.Error);
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
