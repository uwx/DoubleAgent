using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;
using System.Xml;
using System.Security.Cryptography;
using System.Security.Cryptography.X509Certificates;

using SandcastleBuilder.Utils;
using SandcastleBuilder.Utils.BuildEngine;

namespace SandcastleBuilder.PlugIns
{
	/// <summary>
	/// This class handles the configurable settings for the PackAndSignMSHC plugin.
	/// </summary>
	public class PackAndSignMSHCSettings
	{
		#region Private data members
		//=====================================================================

		private SandcastleProject m_project;
		private XmlDocument m_configuration;
		private static String m_makeCabConfig = "make-cab";
		private static String m_signToolConfig = "sign-code";
		private static String m_cabFileConfig = "cab-file";
		private static String m_cabPathConfig = "cab-path";
		private static String m_timeStampConfig = "timestamp-url";
		private static String m_certificateConfig = "certificate";

		#endregion

		#region Initialization
		//=====================================================================

		/// <summary>
		/// Use this constructor to get the default settings without reference to any project.
		/// </summary>
		public PackAndSignMSHCSettings ()
		{
			this.Configuration = String.Empty;
		}

		/// <summary>
		/// Use this constructor to get the default settings for a specific project.
		/// </summary>
		/// <param name="project">The project to use.</param>
		public PackAndSignMSHCSettings (SandcastleProject project)
			: this (project, String.Empty)
		{
		}

		/// <summary>
		/// Use this constructor to initialize with a specific configuration, but without reference to any project.
		/// </summary>
		/// <param name="configuration">The initial configuration XML fragment.</param>
		public PackAndSignMSHCSettings (String configuration)
		{
			this.Configuration = configuration;
		}

		/// <summary>
		/// Use this constructor for full initialization and support of project-specific defaults.
		/// </summary>
		/// <param name="project">The project to use.</param>
		/// <param name="configuration">The initial configuration XML fragment.</param>
		public PackAndSignMSHCSettings (SandcastleProject project, String configuration)
		{
			m_project = project;
			this.Configuration = configuration;
		}

		#endregion

		#region Properties
		//=====================================================================

		/// <summary>
		/// The PackAndSignMSHC configuration XML fragment. 
		/// </summary>
		public String Configuration
		{
			get
			{
				if (m_configuration == null)
				{
					Configuration = String.Empty;
				}

				XmlNode v_rootNode = m_configuration.SelectSingleNode ("configuration");
				XmlNode v_makeCabPath;
				XmlNode v_signToolPath;
				XmlNode v_cabFileName;
				XmlNode v_cabPathName;
				XmlNode v_timeStampUrl;
				XmlNode v_certificatePath;

				if (v_rootNode == null)
				{
					m_configuration.AppendChild (v_rootNode = m_configuration.CreateElement ("configuration"));
				}

				v_makeCabPath = v_rootNode.Attributes.GetNamedItem (m_makeCabConfig);
				if (v_makeCabPath == null)
				{
					v_rootNode.Attributes.SetNamedItem (v_makeCabPath = m_configuration.CreateAttribute (m_makeCabConfig));
				}
				v_makeCabPath.Value = MakeCabPath;

				v_signToolPath = v_rootNode.Attributes.GetNamedItem (m_signToolConfig);
				if (v_signToolPath == null)
				{
					v_rootNode.Attributes.SetNamedItem (v_signToolPath = m_configuration.CreateAttribute (m_signToolConfig));
				}
				v_signToolPath.Value = SignToolPath;

				if (String.IsNullOrEmpty (CabFileName) || (CabFileName == DefaultCabFileName))
				{
					v_rootNode.Attributes.RemoveNamedItem (m_cabFileConfig);
				}
				else
				{
					v_cabFileName = v_rootNode.Attributes.GetNamedItem (m_cabFileConfig);
					if (v_cabFileName == null)
					{
						v_rootNode.Attributes.SetNamedItem (v_cabFileName = m_configuration.CreateAttribute (m_cabFileConfig));
					}
					v_cabFileName.Value = CabFileName;
				}

				if (String.IsNullOrEmpty (CabPathName) || (CabPathName == DefaultCabPathName))
				{
					v_rootNode.Attributes.RemoveNamedItem (m_cabPathConfig);
				}
				else
				{
					v_cabPathName = v_rootNode.Attributes.GetNamedItem (m_cabPathConfig);
					if (v_cabPathName == null)
					{
						v_rootNode.Attributes.SetNamedItem (v_cabPathName = m_configuration.CreateAttribute (m_cabPathConfig));
					}
					v_cabPathName.Value = CabPathName;
				}

				v_timeStampUrl = v_rootNode.Attributes.GetNamedItem (m_timeStampConfig);
				if (v_timeStampUrl == null)
				{
					v_rootNode.Attributes.SetNamedItem (v_timeStampUrl = m_configuration.CreateAttribute (m_timeStampConfig));
				}
				v_timeStampUrl.Value = TimeStampUrl;

				v_certificatePath = v_rootNode.Attributes.GetNamedItem (m_certificateConfig);
				if (v_certificatePath == null)
				{
					v_rootNode.Attributes.SetNamedItem (v_certificatePath = m_configuration.CreateAttribute (m_certificateConfig));
				}
				v_certificatePath.Value = CertificateSpec;

				return m_configuration.OuterXml;
			}
			set
			{
				m_configuration = new XmlDocument ();
				if (!String.IsNullOrWhiteSpace (value))
				{
					try
					{
						m_configuration.LoadXml (value);
					}
					catch (Exception exc)
					{
						System.Diagnostics.Debug.Print (exc.Message);
					}
				}

				XmlNode v_rootNode = m_configuration.SelectSingleNode ("configuration");
				XmlNode v_makeCabPath = null;
				XmlNode v_signToolPath = null;
				XmlNode v_cabFileName = null;
				XmlNode v_cabPathName = null;
				XmlNode v_timeStampUrl = null;
				XmlNode v_certificatePath = null;

				if (v_rootNode != null)
				{
					v_makeCabPath = v_rootNode.Attributes.GetNamedItem (m_makeCabConfig);
					v_signToolPath = v_rootNode.Attributes.GetNamedItem (m_signToolConfig);
					v_cabFileName = v_rootNode.Attributes.GetNamedItem (m_cabFileConfig);
					v_cabPathName = v_rootNode.Attributes.GetNamedItem (m_cabPathConfig);
					v_timeStampUrl = v_rootNode.Attributes.GetNamedItem (m_timeStampConfig);
					v_certificatePath = v_rootNode.Attributes.GetNamedItem (m_certificateConfig);
				}
				MakeCabPath = (v_makeCabPath == null) ? String.Empty : v_makeCabPath.Value.Trim ();
				SignToolPath = (v_signToolPath == null) ? String.Empty : v_signToolPath.Value.Trim ();
				CabFileName = (v_cabFileName == null) ? String.Empty : v_cabFileName.Value.Trim ();
				CabPathName = (v_cabPathName == null) ? String.Empty : v_cabPathName.Value.Trim ();
				TimeStampUrl = (v_timeStampUrl == null) ? String.Empty : v_timeStampUrl.Value.Trim ();
				CertificateSpec = (v_certificatePath == null) ? String.Empty : v_certificatePath.Value.Trim ();

				if (String.IsNullOrEmpty (MakeCabPath))
				{
					MakeCabPath = FindMakeCab ();
				}
				if (String.IsNullOrEmpty (SignToolPath))
				{
					SignToolPath = FindSignTool ();
				}
				if (String.IsNullOrEmpty (CabFileName))
				{
					CabFileName = DefaultCabFileName;
				}
				if (String.IsNullOrEmpty (CabPathName))
				{
					CabPathName = DefaultCabPathName;
				}
			}
		}

		//=====================================================================

		/// <summary>The full path of the "makecab.exe" tool.</summary>
		/// <remarks>The path can contain environment variables.</remarks>
		public String MakeCabPath { get; set; }
		/// <summary>The full path of the "signtool.exe" tool.</summary>
		/// <remarks>The path can contain environment variables.</remarks>
		public String SignToolPath { get; set; }
		/// <summary>The file name to be used for the cabinet file.</summary>
		/// <remarks>If this value is blank, the name of the help file will be used with the ".cab" file extension.</remarks>
		public String CabFileName { get; set; }
		/// <summary>The absolute or relative path where the cabinet file is to be created.</summary>
		/// <remarks>If this value is blank, the cabinet file will be placed in the project output folder.</remarks>
		public String CabPathName { get; set; }
		/// <summary>The URL of the timestamping service used to timestamp the digital signature.</summary>
		public String TimeStampUrl { get; set; }
		/// <summary>
		/// The parameters used to specify an Authenticode certificate for the "signtool.exe" tool.
		/// </summary>
		/// <remarks>
		/// <list type="bullet">
		/// <item>
		/// If the certificate is in a certificate store, the value is formatted as follows:
		/// <para>
		/// <c>/s &lt;store name&gt; /n &lt;certificate subject name&gt;</c>
		/// </para>
		/// For now, the store name is always "my" and the certificate issuer parameter is not used.
		/// </item>
		/// <item>
		/// If the certificate is in a file, the value is the full path of the certificate file.
		/// </item>
		/// </list>
		/// </remarks>
		public String CertificateSpec { get; set; }

		//=====================================================================

		/// <summary>
		/// Indicates that the <see cref="P:SandcastleBuilder.PlugIns.PackAndSignMSHCSettings.CertificateSpec"/> is for a certificate in a certificate store.
		/// </summary>
		public bool IsStoreCertificate
		{
			get
			{
				if (!String.IsNullOrWhiteSpace (CertificateSpec))
				{
					try
					{
						String v_prefix = CertificateSpec.Trim ().Split (' ')[0];

						if (IsStoreFlag (v_prefix, StoreStoreFlag))
						{
							return true;
						}
						if (IsStoreFlag (v_prefix, StoreNameFlag))
						{
							return true;
						}
						if (IsStoreFlag (v_prefix, StoreIssuerFlag))
						{
							return true;
						}
						if (IsStoreFlag (v_prefix, StoreTimestamperFlag))
						{
							return true;
						}
					}
					catch { }
				}
				return false;
			}
		}

		/// <summary>
		/// If a store certificate is being used, the name of the certificate store. Otherwise an empty string.
		/// </summary>
		public String StoreCertificateStore
		{
			get
			{
				String v_store = GetStorePart (StoreStoreFlag);
				if (String.IsNullOrWhiteSpace (v_store))
				{
					v_store = DefaultCertificateStore;
				}
				return v_store;
			}
			set
			{
				CertificateSpec = SetStorePart (StoreStoreFlag, value);
			}
		}

		/// <summary>
		/// If a store certificate is being used, certificate's subject name. Otherwise an empty string.
		/// </summary>
		public String StoreCertificateName
		{
			get
			{
				return GetStorePart (StoreNameFlag).Trim ('"');
			}
			set
			{
				CertificateSpec = SetStorePart (StoreNameFlag, value);
			}
		}

		/// <summary>
		/// If a store certificate is being used, certificate's issuer name. Otherwise an empty string.
		/// </summary>
		/// <remarks>
		/// This value is not currently used.
		/// </remarks>
		public String StoreCertificateIssuer
		{
			get
			{
				return (GetStorePart (StoreIssuerFlag)).Trim ('"');
			}
			set
			{
				CertificateSpec = SetStorePart (StoreIssuerFlag, value);
			}
		}

		//=====================================================================

		/// <summary>
		/// Indicates that the <see cref="P:SandcastleBuilder.PlugIns.PackAndSignMSHCSettings.CertificateSpec"/> is for a certificate file.
		/// </summary>
		public bool IsFileCertificate
		{
			get
			{
				if (!String.IsNullOrEmpty (CertificateSpec) && !IsStoreCertificate)
				{
					return true;
				}
				return false;
			}
		}

		/// <summary>
		/// If a file certificate is being used, the full path of the certificate file. Otherwise an empty string.
		/// </summary>
		public String FileCertificatePath
		{
			get
			{
				return IsFileCertificate ? CertificateSpec : String.Empty;
			}
		}

		//=====================================================================

		/// <summary>
		/// The default file name for the cabinet file.
		/// </summary>
		/// <remarks>
		/// The value is an empty string if the class was constructed without a project reference. 
		/// </remarks>
		public String DefaultCabFileName
		{
			get
			{
				return (m_project == null) ? String.Empty : Path.ChangeExtension (m_project.HtmlHelpName, ".cab");
			}
		}

		/// <summary>
		/// The default location for the cabinet file.
		/// </summary>
		/// <remarks>
		/// The value is an empty string if the class was constructed without a project reference. 
		/// </remarks>
		public String DefaultCabPathName
		{
			get
			{
				return (m_project == null) ? String.Empty : m_project.OutputPath;
			}
		}

		/// <summary>
		/// The default certificate store name.
		/// </summary>
		/// <remarks>
		/// At this time, the "my" store is always used.
		/// </remarks>
		public static String DefaultCertificateStore
		{
			get
			{
				return "my";
			}
		}

		//=====================================================================

		/// <summary>
		/// The flag used to specify the certificate store in the "signtool.exe" command line.
		/// </summary>
		public static String StoreStoreFlag
		{
			get { return "/s"; }
		}
		/// <summary>
		/// The flag used to specify the certificate issuer name in the "signtool.exe" command line.
		/// </summary>
		public static String StoreIssuerFlag
		{
			get { return "/i"; }
		}
		/// <summary>
		/// The flag used to specify the certificate subjet name in the "signtool.exe" command line.
		/// </summary>
		public static String StoreNameFlag
		{
			get { return "/n"; }
		}
		/// <summary>
		/// The flag used to specify the timestamper URL in the "signtool.exe" command line.
		/// </summary>
		public static String StoreTimestamperFlag
		{
			get { return "/t"; }
		}

		#endregion

		#region Public Methods
		//=====================================================================

		/// <summary>
		/// Loads an Authenticode code-signing cerificate from the certificate file, if a file path has been provided.
		/// </summary>
		/// <returns>The loaded certificate, if successful.</returns>
		public X509Certificate2 LoadCertificate ()
		{
			String v_errorMessage = String.Empty;
			return LoadCertificate (out v_errorMessage);
		}
		/// <summary>
		/// Loads an Authenticode code-signing cerificate from the certificate file, if a file path has been provided.
		/// </summary>
		/// <param name="errorMessage">An error message string if the load failed.</param>
		/// <returns>The loaded certificate, if successful.</returns>
		public X509Certificate2 LoadCertificate (out String errorMessage)
		{
			X509Certificate2 v_certificate = null;

			errorMessage = String.Empty;

			if (IsFileCertificate)
			{
				v_certificate = LoadFileCertificate (FileCertificatePath, out errorMessage);

				if (v_certificate != null)
				{
					//TODO - add the certificate to "my" store
				}
			}
			else if (IsStoreCertificate)
			{
				v_certificate = LoadStoreCertificate (StoreCertificateStore, StoreCertificateName, StoreCertificateIssuer, out errorMessage);
			}

			return v_certificate;
		}

		//=====================================================================

		/// <summary>
		/// Loads an Authenticode code-signing cerificate from a certificate file.
		/// </summary>
		/// <param name="filePath">The full path of the file to load.</param>
		/// <returns>The loaded certificate, if successful.</returns>
		public static X509Certificate2 LoadFileCertificate (String filePath)
		{
			String v_errorMessage = String.Empty;
			return LoadFileCertificate (filePath, out v_errorMessage);
		}
		/// <summary>
		/// Loads an Authenticode code-signing cerificate from a certificate file.
		/// </summary>
		/// <param name="filePath">The full path of the file to load.</param>
		/// <param name="errorMessage">An error message string if the load failed.</param>
		/// <returns>The loaded certificate, if successful.</returns>
		public static X509Certificate2 LoadFileCertificate (String filePath, out String errorMessage)
		{
			X509Certificate2 v_certificate = null;

			errorMessage = String.Empty;

			try
			{
				RequestPfxPassword v_requestPassword = new RequestPfxPassword (Path.GetFileName (filePath));

				if (v_requestPassword.ShowDialog ().GetValueOrDefault ())
				{
					v_certificate = new X509Certificate2 (filePath, v_requestPassword.SecurePassword);
				}
			}
			catch (CryptographicException exp)
			{
				System.Diagnostics.Debug.Print (exp.Message);
				errorMessage = exp.Message;
			}
			catch (Exception exp)
			{
				System.Diagnostics.Debug.Print (exp.Message);
				errorMessage = exp.Message;
			}

			if ((v_certificate != null) && (!v_certificate.HasPrivateKey))
			{
				errorMessage = String.Format ("The certificate in {0} does not have a private key.", filePath);
				v_certificate = null;
			}
			return v_certificate;
		}

		//=====================================================================

		/// <summary>
		/// Loads an Authenticode code-signing cerificate from a certificate store.
		/// </summary>
		/// <param name="storeName">The certificate store name.</param>
		/// <param name="certificateName">The certificate's subject name (optional).</param>
		/// <param name="certificateIssuer">The certificate's issuer name (optional).</param>
		/// <returns>The loaded certificate, if successful.</returns>
		/// <remarks>Either the <paramref name="certificateName"/> or <paramref name="certificateIssuer"/> must be provided.
		/// If both are provided, then only a certificate that matches both will be loaded.
		/// </remarks>
		public static X509Certificate2 LoadStoreCertificate (String storeName, String certificateName, String certificateIssuer)
		{
			String v_errorMessage = String.Empty;
			return LoadStoreCertificate (storeName, certificateName, certificateIssuer, out v_errorMessage);
		}
		/// <summary>
		/// Loads an Authenticode code-signing cerificate from a certificate store.
		/// </summary>
		/// <param name="storeName">The certificate store name.</param>
		/// <param name="certificateName">The certificate's subject name (optional).</param>
		/// <param name="certificateIssuer">The certificate's issuer name (optional).</param>
		/// <param name="errorMessage">An error message string if the load failed.</param>
		/// <returns>The loaded certificate, if successful.</returns>
		/// <remarks>Either the <paramref name="certificateName"/> or <paramref name="certificateIssuer"/> must be provided.
		/// If both are provided, then only a certificate that matches both will be loaded.
		/// </remarks>
		public static X509Certificate2 LoadStoreCertificate (String storeName, String certificateName, String certificateIssuer, out String errorMessage)
		{
			X509Certificate2 v_certificate = null;

			errorMessage = String.Empty;

			try
			{
				X509Store v_certStore = new X509Store (storeName, StoreLocation.CurrentUser);
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
						errorMessage = String.Format ("Certificate store \"{0}\" is empty.", storeName);
					}

					if ((v_certificates != null) && (v_certificates.Count > 0))
					{
						if (!String.IsNullOrEmpty (certificateName))
						{
							v_certificates = v_certificates.Find (X509FindType.FindBySubjectName, certificateName, true);
						}
						if ((v_certificates != null) && (v_certificates.Count > 0) && !String.IsNullOrEmpty (certificateIssuer))
						{
							v_certificates = v_certificates.Find (X509FindType.FindByIssuerName, certificateIssuer, true);
						}
					}
					else
					{
						errorMessage = String.Format ("Certificate store \"{0}\" does not contain any code signing certificates.", storeName);
					}

					if ((v_certificates != null) && (v_certificates.Count > 0))
					{
						v_certificate = v_certificates[0];
					}
					else
					{
						errorMessage = String.Format ("Certificate store \"{0}\" does not contain the specified certificate.", storeName);
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

			if ((v_certificate != null) && (!v_certificate.HasPrivateKey))
			{
				errorMessage = String.Format ("The certificate for {0} from {1} does not have a private key.", v_certificate.GetNameInfo (X509NameType.SimpleName, false), v_certificate.GetNameInfo (X509NameType.SimpleName, true));
				v_certificate = null;
			}
			return v_certificate;
		}

		#endregion

		#region Helper Methods
		//=====================================================================

		/// <summary>
		/// Uses the environment PATH to search for "makecab.exe".
		/// </summary>
		/// <returns>The full file path, if found. Otherwise an empty string.</returns>
		public static String FindMakeCab ()
		{
			try
			{
				String[] v_paths = Environment.GetEnvironmentVariable ("PATH").Split (';');
				foreach (String v_path in v_paths)
				{
					try
					{
						String v_filePath = Path.Combine (Path.GetFullPath (Environment.ExpandEnvironmentVariables (v_path)), "makecab.exe");

						if (File.Exists (v_filePath))
						{
							return v_filePath;
						}
					}
					catch { }
				}
			}
			catch { }
			return String.Empty;
		}

		/// <summary>
		/// Uses the environment PATH to search for "signtool.exe".
		/// </summary>
		/// <returns>The full file path, if found. Otherwise an empty string.</returns>
		public static String FindSignTool ()
		{
			try
			{
				String[] v_paths = Environment.GetEnvironmentVariable ("PATH").Split (';');
				foreach (String v_path in v_paths)
				{
					try
					{
						String v_filePath = Path.Combine (Path.GetFullPath (Environment.ExpandEnvironmentVariables (v_path)), "signtool.exe");

						if (File.Exists (v_filePath))
						{
							return v_filePath;
						}
					}
					catch { }
				}
			}
			catch { }
			return String.Empty;
		}

		//=====================================================================

		private static bool IsStoreFlag (String flag, String storeFlag)
		{
			if (flag.StartsWith ("-"))
			{
				flag = "/" + flag.Substring (1);
			}
			return (String.Compare (flag, storeFlag, StringComparison.InvariantCultureIgnoreCase) == 0);
		}

		private String GetStorePart (String storeFlag)
		{
			if (!String.IsNullOrEmpty (CertificateSpec))
			{
				try
				{
					Regex v_regEx = new Regex ("(?<flag>(/|-)\\w+)\\s+(?<value>(\".*\")|\\S+)(?(/|-|$))", RegexOptions.CultureInvariant);

					foreach (Match v_match in v_regEx.Matches (CertificateSpec))
					{
						String v_flag = String.Empty;
						String v_value = String.Empty;
						int v_groupNdx;

						for (v_groupNdx = 0; v_groupNdx < v_match.Groups.Count; v_groupNdx++)
						{
							if (v_regEx.GroupNameFromNumber (v_groupNdx) == "flag")
							{
								v_flag = v_match.Groups[v_groupNdx].Value;
							}
							else if (v_regEx.GroupNameFromNumber (v_groupNdx) == "value")
							{
								v_value = v_match.Groups[v_groupNdx].Value;
							}
						}
						if (IsStoreFlag (v_flag, storeFlag))
						{
							return v_value;
						}
					}
				}
				catch { }
			}
			return String.Empty;
		}

		private String SetStorePart (String storeFlag, String value)
		{
			String v_storeStore = IsStoreFlag (storeFlag, StoreStoreFlag) ? value : StoreCertificateStore;
			String v_storeName = IsStoreFlag (storeFlag, StoreNameFlag) ? value : StoreCertificateName;
			String v_storeIssuer = IsStoreFlag (storeFlag, StoreIssuerFlag) ? value : StoreCertificateIssuer;
			List<String> v_storeParts = new List<String> ();

			if (!String.IsNullOrWhiteSpace (v_storeStore))
			{
				v_storeParts.Add (StoreStoreFlag);
				v_storeParts.Add (v_storeStore);
			}
			if (!String.IsNullOrWhiteSpace (v_storeName))
			{
				v_storeParts.Add (StoreNameFlag);
				v_storeParts.Add ("\"" + v_storeName + "\"");
			}
			if (!String.IsNullOrWhiteSpace (v_storeIssuer))
			{
				v_storeParts.Add (StoreIssuerFlag);
				v_storeParts.Add ("\"" + v_storeIssuer + "\"");
			}

			return String.Join (" ", v_storeParts.ToArray ());
		}

		#endregion
	}
}
