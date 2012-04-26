using System;
using System.Collections.Generic;
using System.IO;
using System.Security.Cryptography;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Windows;
using SandcastleBuilder.Utils;

namespace SandcastleBuilder.PlugIns
{
	/// <summary>
	/// This is the configuration dialog for the PackAndSignMHSC plugin.
	/// </summary>
	public partial class PackAndSignMHSCConfig : OwnedWPFWindow
	{
		#region Private data members
		//=====================================================================

		private SandcastleProject m_project;
		private PackAndSignMSHCSettings m_settings;
		private X509Certificate2 m_certificate = null;

		#endregion

		#region Initialization
		//=====================================================================

		public PackAndSignMHSCConfig (SandcastleProject project)
		{
			m_project = project;
			m_settings = new PackAndSignMSHCSettings (m_project);
			InitializeComponent ();
		}

		#endregion

		#region Properties
		//=====================================================================

		/// <summary>
		/// The configuration XML fragment.
		/// </summary>
		public String Configuration
		{
			get
			{
				return m_settings.Configuration;
			}
			set
			{
				m_settings = new PackAndSignMSHCSettings (m_project, value);
			}
		}

		#endregion

		#region Helper Methods
		//=====================================================================

		private String BrowseForExeFile (String currentFileName)
		{
			return BrowseForExeFile (currentFileName, String.Empty);
		}
		private String BrowseForExeFile (String currentFileName, String title)
		{
			Microsoft.Win32.OpenFileDialog v_dialog = new Microsoft.Win32.OpenFileDialog ();
			StringBuilder v_filter = new StringBuilder ();

			v_filter.Append (FileExtTypeName (".exe", "Applications") + " (*.exe)|*.exe");
			v_filter.Append ("|All Files (*.*)|*.*");

			v_dialog.Filter = v_filter.ToString ();
			v_dialog.CheckFileExists = true;
			if (!String.IsNullOrEmpty (currentFileName))
			{
				v_dialog.FileName = Path.GetFileName (currentFileName);
				v_dialog.InitialDirectory = Path.GetDirectoryName (currentFileName);
			}
			if (!String.IsNullOrWhiteSpace (title))
			{
				v_dialog.Title = title;
			}
			if (v_dialog.ShowDialog (this).GetValueOrDefault ())
			{
				return v_dialog.FileName;
			}
			return currentFileName;
		}

		private String BrowseForFolder (String currentPath)
		{
			return BrowseForFolder (currentPath, String.Empty);
		}
		private String BrowseForFolder (String currentPath, String description)
		{
			return BrowseForFolder (currentPath, m_settings.DefaultCabPathName, description);
		}
		private String BrowseForFolder (String currentPath, String defaultPath, String description)
		{
			System.Windows.Forms.FolderBrowserDialog v_dialog = new System.Windows.Forms.FolderBrowserDialog ();
			FolderPath v_path = new FolderPath (m_project);
			String v_pathRoot = Path.GetPathRoot (currentPath).TrimStart ('\\', '/');

			if (String.IsNullOrEmpty (currentPath))
			{
				v_path.Path = FolderPath.GetFullPath (defaultPath);
			}
			else if (Path.IsPathRooted (currentPath))
			{
				v_path.Path = FolderPath.GetFullPath (currentPath);
			}
			else
			{
				v_path.Path = FolderPath.RelativeToAbsolutePath (defaultPath, currentPath);
			}

			if (!String.IsNullOrEmpty (v_path))
			{
				v_dialog.SelectedPath = v_path.Path.TrimEnd ('\\', '/');
			}
			v_dialog.ShowNewFolderButton = true;
			v_dialog.Description = description;

			if (v_dialog.ShowDialog () == System.Windows.Forms.DialogResult.OK)
			{
				v_path.Path = FolderPath.TerminatePath (v_dialog.SelectedPath);
				v_path.IsFixedPath = true;

				if (Path.IsPathRooted (currentPath) && String.IsNullOrEmpty (v_pathRoot))
				{
					v_path.PersistablePath = "\\" + FolderPath.AbsoluteToRelativePath (Path.GetPathRoot (v_path), v_path);
				}
				else if (!Path.IsPathRooted (currentPath) && !String.IsNullOrEmpty (defaultPath))
				{
					v_path.PersistablePath = FolderPath.AbsoluteToRelativePath (defaultPath, v_path);
				}

				return v_path.PersistablePath;
			}
			return currentPath;
		}

		//=============================================================================

		private static String FileExtTypeName (String fileExt)
		{
			String v_fileType = fileExt;

			if (v_fileType.StartsWith ("."))
			{
				v_fileType = v_fileType.Substring (1);
			}
			v_fileType = v_fileType.ToUpper () + " File";

			return FileExtTypeName (fileExt, v_fileType);
		}

		private static String FileExtTypeName (String fileExt, String defaultValue)
		{
			String v_typeName = defaultValue;

			try
			{
				Microsoft.Win32.RegistryKey v_fileExtKey = Microsoft.Win32.Registry.ClassesRoot.OpenSubKey (fileExt, false);
				Microsoft.Win32.RegistryKey v_progIdKey = null;

				if (v_fileExtKey != null)
				{
					v_progIdKey = Microsoft.Win32.Registry.ClassesRoot.OpenSubKey (v_fileExtKey.GetValue (String.Empty).ToString ());
				}
				if (v_progIdKey != null)
				{
					v_typeName = v_progIdKey.GetValue (String.Empty).ToString ();
				}
				if (String.IsNullOrEmpty (v_typeName))
				{
					v_typeName = defaultValue;
				}
			}
			catch { }

			return v_typeName;
		}

		#endregion

		#region Certificate
		//=====================================================================

		private void ShowCertificateDetails ()
		{
			if (m_certificate == null)
			{
				CertificateIssuedBy.Text = "(none)";
				CertificateIssuedTo.Text = "(none)";
				CertificatePurpose.Text = "(none)";
				CertificateExpiration.Text = "(none)";
			}
			else
			{
				List<String> v_extensions = new List<String> ();

				CertificateIssuedBy.Text = m_certificate.GetNameInfo (X509NameType.SimpleName, true);
				CertificateIssuedTo.Text = m_certificate.GetNameInfo (X509NameType.SimpleName, false);
				CertificateExpiration.Text = m_certificate.NotAfter.ToString ();

				foreach (X509Extension v_extension in m_certificate.Extensions)
				{
					X509KeyUsageExtension v_keyUsage = v_extension as X509KeyUsageExtension;
					X509EnhancedKeyUsageExtension v_enhancedKeyUsage = v_extension as X509EnhancedKeyUsageExtension;

					if (v_keyUsage != null)
					{
						v_extensions.Add (v_keyUsage.KeyUsages.ToString ());
					}
					if (v_enhancedKeyUsage != null)
					{
						foreach (Oid v_oid in v_enhancedKeyUsage.EnhancedKeyUsages)
						{
							v_extensions.Add (v_oid.FriendlyName);
						}
					}

				}
				CertificatePurpose.Text = String.Join (", ", v_extensions.ToArray ());
			}
		}

		private void DisplayCertificate ()
		{
			if (m_certificate != null)
			{
				try
				{
					System.Windows.Interop.WindowInteropHelper lInteropHelper = new System.Windows.Interop.WindowInteropHelper (this);

					X509Certificate2UI.DisplayCertificate (m_certificate, lInteropHelper.Handle);
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
			String v_errorMessage = String.Empty;
			return OpenStoreCertificate (out v_errorMessage);
		}
		private X509Certificate2 OpenStoreCertificate (out String errorMessage)
		{
			errorMessage = String.Empty;

			try
			{
				System.Windows.Interop.WindowInteropHelper v_interopHelper = new System.Windows.Interop.WindowInteropHelper (this);
				String v_certificateStore = m_settings.StoreCertificateStore;
				X509Store v_certStore = new X509Store (v_certificateStore, StoreLocation.CurrentUser);
				X509Certificate2Collection v_certificates;

				v_certStore.Open (OpenFlags.ReadOnly | OpenFlags.OpenExistingOnly);
				try
				{
					v_certificates = v_certStore.Certificates as X509Certificate2Collection;
					if ((v_certificates != null) && (v_certificates.Count > 0))
					{
						v_certificates = v_certificates.Find (X509FindType.FindByKeyUsage, X509KeyUsageFlags.DigitalSignature, true);
					}
					else
					{
						errorMessage = String.Format ("Certificate store \"{0}\" is empty.", v_certificateStore);
					}

					if ((v_certificates != null) && (v_certificates.Count > 0))
					{
						v_certificates = X509Certificate2UI.SelectFromCollection (v_certificates, "Select a Certificate", "Select a certificate to sign the Cabinet file.", X509SelectionFlag.SingleSelection, v_interopHelper.Handle);
					}
					else
					{
						errorMessage = String.Format ("Certificate store \"{0}\" does not contain any code signing certificates.", v_certificateStore);
					}

					if ((v_certificates != null) && (v_certificates.Count > 0))
					{
						m_certificate = v_certificates[0]; 
						m_settings.StoreCertificateStore = v_certStore.Name;
						m_settings.StoreCertificateName = m_certificate.GetNameInfo (X509NameType.SimpleName, false);
					}
				}
				catch (Exception exp)
				{
					System.Diagnostics.Debug.Print (exp.Message);
					errorMessage = exp.Message;
				}
				v_certStore.Close ();
			}
			catch (Exception exp)
			{
				System.Diagnostics.Debug.Print (exp.Message);
				errorMessage = exp.Message;
			}
			return m_certificate;
		}

		//=====================================================================

		private String BrowseForCertificateFile (String currentFileName)
		{
			return BrowseForCertificateFile (currentFileName, String.Empty);
		}
		private String BrowseForCertificateFile (String currentFileName, String title)
		{
			Microsoft.Win32.OpenFileDialog v_dialog = new Microsoft.Win32.OpenFileDialog ();
			StringBuilder v_filter = new StringBuilder ();

			v_filter.Append (FileExtTypeName (".pfx", "Personal Information Exchange") + " (*.pfx)|*.pfx");
			v_filter.Append ("|All Files (*.*)|*.*");

			v_dialog.Filter = v_filter.ToString ();
			v_dialog.CheckFileExists = true;
			if (!String.IsNullOrEmpty (currentFileName))
			{
				v_dialog.FileName = Path.GetFileName (currentFileName);
				v_dialog.InitialDirectory = Path.GetDirectoryName (currentFileName);
			}
			if (!String.IsNullOrWhiteSpace (title))
			{
				v_dialog.Title = title;
			}
			if (v_dialog.ShowDialog (this).GetValueOrDefault ())
			{
				return v_dialog.FileName;
			}
			return String.Empty;
		}

		#endregion

		#region Event Handlers
		//=====================================================================

		private void OnLoaded (object sender, RoutedEventArgs e)
		{
			MakeCabPath.Text = m_settings.MakeCabPath;
			CabFileName.Text = m_settings.CabFileName;
			CabFileNameDefault.IsEnabled = (m_settings.CabFileName != m_settings.DefaultCabFileName);
			CabPathName.Text = m_settings.CabPathName;
			CabPathName.IsEnabled = (m_settings.CabPathName != m_settings.DefaultCabPathName);
			CabPathOutput.IsChecked = (m_settings.CabPathName == m_settings.DefaultCabPathName);
			CabPathThis.IsChecked = (m_settings.CabPathName != m_settings.DefaultCabPathName);
			CabPathBrowse.IsEnabled = (m_settings.CabPathName != m_settings.DefaultCabPathName);

			SignToolPath.Text = m_settings.SignToolPath;
			TimestampServer.Text = m_settings.TimeStampUrl;
			ShowCertificate.IsEnabled = !String.IsNullOrEmpty (m_settings.CertificateSpec);

			m_certificate = m_settings.LoadCertificate ();
			ShowCertificateDetails ();
		}

		private void OnOK (object sender, RoutedEventArgs e)
		{
			m_settings.MakeCabPath = MakeCabPath.Text;
			m_settings.CabFileName = CabFileName.Text;
			m_settings.CabPathName = (CabPathOutput.IsChecked.GetValueOrDefault ()) ? String.Empty : CabPathName.Text;

			m_settings.SignToolPath = SignToolPath.Text;
			m_settings.TimeStampUrl = TimestampServer.Text;

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
			m_settings.MakeCabPath = BrowseForExeFile (MakeCabPath.Text, "Where is makecab.exe?");
			MakeCabPath.Text = m_settings.MakeCabPath;
		}

		private void OnBrowseForSignTool (object sender, RoutedEventArgs e)
		{
			m_settings.SignToolPath = BrowseForExeFile (SignToolPath.Text, "Where is signtool.exe?");
			SignToolPath.Text = m_settings.SignToolPath;
		}

		private void OnBrowseForCabPath (object sender, RoutedEventArgs e)
		{
			m_settings.CabPathName = BrowseForFolder (CabPathName.Text, "Select the folder where the Cabinet file will be created.");
			CabPathName.Text = m_settings.CabPathName;
		}

		private void OnCabPathOutput (object sender, RoutedEventArgs e)
		{
			CabPathName.Text = m_settings.CabPathName = m_settings.DefaultCabPathName;
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
			String v_errorMessage = String.Empty;

			m_certificate = OpenStoreCertificate (out v_errorMessage);
			ShowCertificateDetails ();

			if (!String.IsNullOrEmpty (v_errorMessage))
			{
				MessageBox.Show (this, v_errorMessage, "Error", MessageBoxButton.OK, MessageBoxImage.Error);
			}
		}

		private void OnGetFileCertificate (object sender, RoutedEventArgs e)
		{
			String v_certificatePath = BrowseForCertificateFile (m_settings.FileCertificatePath);

			if (!String.IsNullOrEmpty (v_certificatePath))
			{
				String v_errorMessage = String.Empty;
				X509Certificate2 v_certificate = PackAndSignMSHCSettings.LoadFileCertificate (v_certificatePath, out v_errorMessage);

				if (v_certificate != null)
				{
					m_settings.CertificateSpec = v_certificatePath;
					m_certificate = v_certificate;
					ShowCertificateDetails ();
				}
				else if (!String.IsNullOrEmpty (v_errorMessage))
				{
					MessageBox.Show (this, v_errorMessage, "Error", MessageBoxButton.OK, MessageBoxImage.Error);
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
