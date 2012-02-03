using System;
using System.Collections.Generic;
using System.IO;
using System.Security.Cryptography;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Windows;
using SandcastleBuilder.Utils;

namespace SandcastleBuilder.PlugIns.CinSoft
{
	public partial class PackAndSignMHSCConfig : OwnedWPFWindow
	{
		#region Private data members
		//=====================================================================

		private SandcastleProject mProject;
		private PackAndSignMSHCSettings mSettings;
		private X509Certificate2 mCertificate = null;

		#endregion

		#region Initialization
		//=====================================================================

		public PackAndSignMHSCConfig (SandcastleProject pProject)
		{
			mProject = pProject;
			mSettings = new PackAndSignMSHCSettings (mProject);
			InitializeComponent ();
		}

		#endregion

		#region Properties
		//=====================================================================

		public String Configuration
		{
			get
			{
				return mSettings.Configuration;
			}
			set
			{
				mSettings = new PackAndSignMSHCSettings (mProject, value);
			}
		}

		#endregion

		#region Helper Methods
		//=====================================================================

		String BrowseForExeFile (String CurrentFileName)
		{
			return BrowseForExeFile (CurrentFileName, String.Empty);
		}
		String BrowseForExeFile (String CurrentFileName, String Title)
		{
			Microsoft.Win32.OpenFileDialog lDialog = new Microsoft.Win32.OpenFileDialog ();
			StringBuilder lFilter = new StringBuilder ();

			lFilter.Append (FileExtTypeName (".exe", "Applications") + " (*.exe)|*.exe");
			lFilter.Append ("|All Files (*.*)|*.*");

			lDialog.Filter = lFilter.ToString ();
			lDialog.CheckFileExists = true;
			if (!String.IsNullOrEmpty (CurrentFileName))
			{
				lDialog.FileName = Path.GetFileName (CurrentFileName);
				lDialog.InitialDirectory = Path.GetDirectoryName (CurrentFileName);
			}
			if (!String.IsNullOrWhiteSpace (Title))
			{
				lDialog.Title = Title;
			}
			if (lDialog.ShowDialog (this).GetValueOrDefault ())
			{
				return lDialog.FileName;
			}
			return CurrentFileName;
		}

		String BrowseForFolder (String CurrentPath)
		{
			return BrowseForFolder (CurrentPath, String.Empty);
		}
		String BrowseForFolder (String CurrentPath, String Description)
		{
			return BrowseForFolder (CurrentPath, mSettings.DefaultCabPathName, Description);
		}
		String BrowseForFolder (String CurrentPath, String DefaultPath, String Description)
		{
			System.Windows.Forms.FolderBrowserDialog lDialog = new System.Windows.Forms.FolderBrowserDialog ();
			FolderPath lPath = new FolderPath (mProject);
			String lPathRoot = Path.GetPathRoot (CurrentPath).TrimStart ('\\', '/');

			if (String.IsNullOrEmpty (CurrentPath))
			{
				lPath.Path = FolderPath.GetFullPath (DefaultPath);
			}
			else if (Path.IsPathRooted (CurrentPath))
			{
				lPath.Path = FolderPath.GetFullPath (CurrentPath);
			}
			else
			{
				lPath.Path = FolderPath.RelativeToAbsolutePath (DefaultPath, CurrentPath);
			}

			if (!String.IsNullOrEmpty (lPath))
			{
				lDialog.SelectedPath = lPath.Path.TrimEnd ('\\', '/');
			}
			lDialog.ShowNewFolderButton = true;
			lDialog.Description = Description;

			if (lDialog.ShowDialog () == System.Windows.Forms.DialogResult.OK)
			{
				lPath.Path = FolderPath.TerminatePath (lDialog.SelectedPath);
				lPath.IsFixedPath = true;

				if (Path.IsPathRooted (CurrentPath) && String.IsNullOrEmpty (lPathRoot))
				{
					lPath.PersistablePath = "\\" + FolderPath.AbsoluteToRelativePath (Path.GetPathRoot (lPath), lPath);
				}
				else if (!Path.IsPathRooted (CurrentPath) && !String.IsNullOrEmpty (DefaultPath))
				{
					lPath.PersistablePath = FolderPath.AbsoluteToRelativePath (DefaultPath, lPath);
				}

				return lPath.PersistablePath;
			}
			return CurrentPath;
		}

		//=============================================================================

		static public String FileExtTypeName (String pFileExt)
		{
			String lFileType = pFileExt;

			if (lFileType.StartsWith ("."))
			{
				lFileType = lFileType.Substring (1);
			}
			lFileType = lFileType.ToUpper () + " File";

			return FileExtTypeName (pFileExt, lFileType);
		}

		static public String FileExtTypeName (String pFileExt, String pDefault)
		{
			String lTypeName = pDefault;

			try
			{
				Microsoft.Win32.RegistryKey lFileExtKey = Microsoft.Win32.Registry.ClassesRoot.OpenSubKey (pFileExt, false);
				Microsoft.Win32.RegistryKey lProgIdKey = null;

				if (lFileExtKey != null)
				{
					lProgIdKey = Microsoft.Win32.Registry.ClassesRoot.OpenSubKey (lFileExtKey.GetValue (String.Empty).ToString ());
				}
				if (lProgIdKey != null)
				{
					lTypeName = lProgIdKey.GetValue (String.Empty).ToString ();
				}
				if (String.IsNullOrEmpty (lTypeName))
				{
					lTypeName = pDefault;
				}
			}
			catch { }

			return lTypeName;
		}

		#endregion

		#region Certificate
		//=====================================================================

		private void ShowCertificateDetails ()
		{
			if (mCertificate == null)
			{
				CertificateIssuedBy.Text = "(none)";
				CertificateIssuedTo.Text = "(none)";
				CertificatePurpose.Text = "(none)";
				CertificateExpiration.Text = "(none)";
			}
			else
			{
				List<String> lExtensions = new List<String> ();

				CertificateIssuedBy.Text = mCertificate.GetNameInfo (X509NameType.SimpleName, true);
				CertificateIssuedTo.Text = mCertificate.GetNameInfo (X509NameType.SimpleName, false);
				CertificateExpiration.Text = mCertificate.NotAfter.ToString ();

				foreach (X509Extension lExtension in mCertificate.Extensions)
				{
					X509KeyUsageExtension lKeyUsage = lExtension as X509KeyUsageExtension;
					X509EnhancedKeyUsageExtension lEnhancedKeyUsage = lExtension as X509EnhancedKeyUsageExtension;

					if (lKeyUsage != null)
					{
						lExtensions.Add (lKeyUsage.KeyUsages.ToString ());
					}
					if (lEnhancedKeyUsage != null)
					{
						foreach (Oid lOid in lEnhancedKeyUsage.EnhancedKeyUsages)
						{
							lExtensions.Add (lOid.FriendlyName);
						}
					}

				}
				CertificatePurpose.Text = String.Join (", ", lExtensions.ToArray ());
			}
		}

		private void DisplayCertificate ()
		{
			if (mCertificate != null)
			{
				try
				{
					System.Windows.Interop.WindowInteropHelper lInteropHelper = new System.Windows.Interop.WindowInteropHelper (this);

					X509Certificate2UI.DisplayCertificate (mCertificate, lInteropHelper.Handle);
				}
				catch (Exception exp)
				{
					System.Diagnostics.Debug.Print (exp.Message);
				}
			}
		}

		//=====================================================================

		private X509Certificate2 OpenStoreCertificate ()
		{
			String lErrorMessage = String.Empty;
			return OpenStoreCertificate (out lErrorMessage);
		}
		private X509Certificate2 OpenStoreCertificate (out String ErrorMessage)
		{
			ErrorMessage = String.Empty;

			try
			{
				System.Windows.Interop.WindowInteropHelper lInteropHelper = new System.Windows.Interop.WindowInteropHelper (this);
				String lCertificateStore = mSettings.StoreCertificateStore;
				X509Store lCertStore = new X509Store (lCertificateStore, StoreLocation.CurrentUser);
				X509Certificate2Collection lCertificates;

				lCertStore.Open (OpenFlags.ReadOnly | OpenFlags.OpenExistingOnly);
				try
				{
					lCertificates = lCertStore.Certificates as X509Certificate2Collection;
					if ((lCertificates != null) && (lCertificates.Count > 0))
					{
						lCertificates = lCertificates.Find (X509FindType.FindByKeyUsage, X509KeyUsageFlags.DigitalSignature, true);
					}
					else
					{
						ErrorMessage = String.Format ("Certificate store \"{0}\" is empty.", lCertificateStore);
					}

					if ((lCertificates != null) && (lCertificates.Count > 0))
					{
						lCertificates = X509Certificate2UI.SelectFromCollection (lCertificates, "Select a Certificate", "Select a certificate to sign the Cabinet file.", X509SelectionFlag.SingleSelection, lInteropHelper.Handle);
					}
					else
					{
						ErrorMessage = String.Format ("Certificate store \"{0}\" does not contain any code signing certificates.", lCertificateStore);
					}

					if ((lCertificates != null) && (lCertificates.Count > 0))
					{
						mCertificate = lCertificates[0]; 
						mSettings.StoreCertificateStore = lCertStore.Name;
						mSettings.StoreCertificateName = mCertificate.GetNameInfo (X509NameType.SimpleName, false);
					}
				}
				catch (Exception exp)
				{
					System.Diagnostics.Debug.Print (exp.Message);
					ErrorMessage = exp.Message;
				}
				lCertStore.Close ();
			}
			catch (Exception exp)
			{
				System.Diagnostics.Debug.Print (exp.Message);
				ErrorMessage = exp.Message;
			}
			return mCertificate;
		}

		//=====================================================================

		String BrowseForCertificateFile (String CurrentFileName)
		{
			return BrowseForCertificateFile (CurrentFileName, String.Empty);
		}
		String BrowseForCertificateFile (String CurrentFileName, String Title)
		{
			Microsoft.Win32.OpenFileDialog lDialog = new Microsoft.Win32.OpenFileDialog ();
			StringBuilder lFilter = new StringBuilder ();

			lFilter.Append (FileExtTypeName (".pfx", "Personal Information Exchange") + " (*.pfx)|*.pfx");
			lFilter.Append ("|All Files (*.*)|*.*");

			lDialog.Filter = lFilter.ToString ();
			lDialog.CheckFileExists = true;
			if (!String.IsNullOrEmpty (CurrentFileName))
			{
				lDialog.FileName = Path.GetFileName (CurrentFileName);
				lDialog.InitialDirectory = Path.GetDirectoryName (CurrentFileName);
			}
			if (!String.IsNullOrWhiteSpace (Title))
			{
				lDialog.Title = Title;
			}
			if (lDialog.ShowDialog (this).GetValueOrDefault ())
			{
				return lDialog.FileName;
			}
			return String.Empty;
		}

		#endregion

		#region Event Handlers
		//=====================================================================

		private void OnLoaded (object sender, RoutedEventArgs e)
		{
			MakeCabPath.Text = mSettings.MakeCabPath;
			CabFileName.Text = mSettings.CabFileName;
			CabFileNameDefault.IsEnabled = (mSettings.CabFileName != mSettings.DefaultCabFileName);
			CabPathName.Text = mSettings.CabPathName;
			CabPathName.IsEnabled = (mSettings.CabPathName != mSettings.DefaultCabPathName);
			CabPathOutput.IsChecked = (mSettings.CabPathName == mSettings.DefaultCabPathName);
			CabPathThis.IsChecked = (mSettings.CabPathName != mSettings.DefaultCabPathName);
			CabPathBrowse.IsEnabled = (mSettings.CabPathName != mSettings.DefaultCabPathName);

			SignToolPath.Text = mSettings.SignToolPath;
			TimestampServer.Text = mSettings.TimeStampUrl;
			ShowCertificate.IsEnabled = !String.IsNullOrEmpty (mSettings.CertificatePath);

			mCertificate = mSettings.LoadCertificate ();
			ShowCertificateDetails ();
		}

		private void OnOK (object sender, RoutedEventArgs e)
		{
			mSettings.MakeCabPath = MakeCabPath.Text;
			mSettings.CabFileName = CabFileName.Text;
			mSettings.CabPathName = (CabPathOutput.IsChecked.GetValueOrDefault ()) ? String.Empty : CabPathName.Text;

			mSettings.SignToolPath = SignToolPath.Text;
			mSettings.TimeStampUrl = TimestampServer.Text;

			DialogResult = true;
			Close ();
		}

		private void OnCancel (object sender, RoutedEventArgs e)
		{
			DialogResult = false;
			Close ();
		}

		//=====================================================================

		private void OnBrowseForMakeCab (object sender, RoutedEventArgs e)
		{
			mSettings.MakeCabPath = BrowseForExeFile (MakeCabPath.Text, "Where is makecab.exe?");
			MakeCabPath.Text = mSettings.MakeCabPath;
		}

		private void OnBrowseForSignTool (object sender, RoutedEventArgs e)
		{
			mSettings.SignToolPath = BrowseForExeFile (SignToolPath.Text, "Where is signtool.exe?");
			SignToolPath.Text = mSettings.SignToolPath;
		}

		private void OnBrowseForCabPath (object sender, RoutedEventArgs e)
		{
			mSettings.CabPathName = BrowseForFolder (CabPathName.Text, "Select the folder where the Cabinet file will be created.");
			CabPathName.Text = mSettings.CabPathName;
		}

		private void OnCabPathOutput (object sender, RoutedEventArgs e)
		{
			CabPathName.Text = mSettings.CabPathName = mSettings.DefaultCabPathName;
			CabPathName.IsEnabled = false;
			CabPathBrowse.IsEnabled = false;
			CabPathOutput.IsChecked = true;
			CabPathThis.IsChecked = false;
		}

		private void OnCabPathThis (object sender, RoutedEventArgs e)
		{
			CabPathName.IsEnabled = true;
			CabPathBrowse.IsEnabled = true;
			CabPathOutput.IsChecked = false;
			CabPathThis.IsChecked = true;
		}

		private void OnGetStoreCertificate (object sender, RoutedEventArgs e)
		{
			String lErrorMessage = String.Empty;

			mCertificate = OpenStoreCertificate (out lErrorMessage);
			ShowCertificateDetails ();

			if (!String.IsNullOrEmpty (lErrorMessage))
			{
				MessageBox.Show (this, lErrorMessage, "Error", MessageBoxButton.OK, MessageBoxImage.Error);
			}
		}

		private void OnGetFileCertificate (object sender, RoutedEventArgs e)
		{
			String lCertificatePath = BrowseForCertificateFile (mSettings.FileCertificatePath);

			if (!String.IsNullOrEmpty (lCertificatePath))
			{
				String lErrorMessage = String.Empty;
				X509Certificate2 lCertificate = PackAndSignMSHCSettings.LoadFileCertificate (lCertificatePath, out lErrorMessage);

				if (lCertificate != null)
				{
					mSettings.CertificatePath = lCertificatePath;
					mCertificate = lCertificate;
					ShowCertificateDetails ();
				}
				else if (!String.IsNullOrEmpty (lErrorMessage))
				{
					MessageBox.Show (this, lErrorMessage, "Error", MessageBoxButton.OK, MessageBoxImage.Error);
				}
			}
		}

		private void OnShowCertificate (object sender, RoutedEventArgs e)
		{
			DisplayCertificate ();
		}

		#endregion
	}
}
