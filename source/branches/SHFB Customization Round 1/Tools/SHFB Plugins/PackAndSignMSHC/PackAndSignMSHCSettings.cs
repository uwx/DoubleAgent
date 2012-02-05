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

namespace SandcastleBuilder.PlugIns.CinSoft
{
	/// <summary>
	/// This class handles the configurable settings for the PackAndSignMSHC plugin.
	/// </summary>
	public class PackAndSignMSHCSettings
	{
		#region Private data members
		//=====================================================================

		private SandcastleProject mProject;
		private XmlDocument mConfiguration;
		private static String mMakeCabConfig = "make-cab";
		private static String mSignToolConfig = "sign-code";
		private static String mCabFileConfig = "cab-file";
		private static String mCabPathConfig = "cab-path";
		private static String mTimeStampConfig = "timestamp-url";
		private static String mCertificateConfig = "certificate";

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
		/// <param name="Project">The project to use.</param>
		public PackAndSignMSHCSettings (SandcastleProject Project)
			: this (Project, String.Empty)
		{
		}

		/// <summary>
		/// Use this constructor to initialize with a specific configuration, but without reference to any project.
		/// </summary>
		/// <param name="Configuration">The initial configuration XML fragment.</param>
		public PackAndSignMSHCSettings (String Configuration)
		{
			this.Configuration = Configuration;
		}

		/// <summary>
		/// Use this constructor for full initialization and support of project-specific defaults.
		/// </summary>
		/// <param name="Project">The project to use.</param>
		/// <param name="Configuration">The initial configuration XML fragment.</param>
		public PackAndSignMSHCSettings (SandcastleProject Project, String Configuration)
		{
			mProject = Project;
			this.Configuration = Configuration;
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
				if (mConfiguration == null)
				{
					Configuration = String.Empty;
				}

				XmlNode lRootNode = mConfiguration.SelectSingleNode ("configuration");
				XmlNode lMakeCabPath;
				XmlNode lSignToolPath;
				XmlNode lCabFileName;
				XmlNode lCabPathName;
				XmlNode lTimeStampUrl;
				XmlNode lCertificatePath;

				if (lRootNode == null)
				{
					mConfiguration.AppendChild (lRootNode = mConfiguration.CreateElement ("configuration"));
				}

				lMakeCabPath = lRootNode.Attributes.GetNamedItem (mMakeCabConfig);
				if (lMakeCabPath == null)
				{
					lRootNode.Attributes.SetNamedItem (lMakeCabPath = mConfiguration.CreateAttribute (mMakeCabConfig));
				}
				lMakeCabPath.Value = MakeCabPath;

				lSignToolPath = lRootNode.Attributes.GetNamedItem (mSignToolConfig);
				if (lSignToolPath == null)
				{
					lRootNode.Attributes.SetNamedItem (lSignToolPath = mConfiguration.CreateAttribute (mSignToolConfig));
				}
				lSignToolPath.Value = SignToolPath;

				if (String.IsNullOrEmpty (CabFileName) || (CabFileName == DefaultCabFileName))
				{
					lRootNode.Attributes.RemoveNamedItem (mCabFileConfig);
				}
				else
				{
					lCabFileName = lRootNode.Attributes.GetNamedItem (mCabFileConfig);
					if (lCabFileName == null)
					{
						lRootNode.Attributes.SetNamedItem (lCabFileName = mConfiguration.CreateAttribute (mCabFileConfig));
					}
					lCabFileName.Value = CabFileName;
				}

				if (String.IsNullOrEmpty (CabPathName) || (CabPathName == DefaultCabPathName))
				{
					lRootNode.Attributes.RemoveNamedItem (mCabPathConfig);
				}
				else
				{
					lCabPathName = lRootNode.Attributes.GetNamedItem (mCabPathConfig);
					if (lCabPathName == null)
					{
						lRootNode.Attributes.SetNamedItem (lCabPathName = mConfiguration.CreateAttribute (mCabPathConfig));
					}
					lCabPathName.Value = CabPathName;
				}

				lTimeStampUrl = lRootNode.Attributes.GetNamedItem (mTimeStampConfig);
				if (lTimeStampUrl == null)
				{
					lRootNode.Attributes.SetNamedItem (lTimeStampUrl = mConfiguration.CreateAttribute (mTimeStampConfig));
				}
				lTimeStampUrl.Value = TimeStampUrl;

				lCertificatePath = lRootNode.Attributes.GetNamedItem (mCertificateConfig);
				if (lCertificatePath == null)
				{
					lRootNode.Attributes.SetNamedItem (lCertificatePath = mConfiguration.CreateAttribute (mCertificateConfig));
				}
				lCertificatePath.Value = CertificateSpec;

				return mConfiguration.OuterXml;
			}
			set
			{
				mConfiguration = new XmlDocument ();
				if (!String.IsNullOrWhiteSpace (value))
				{
					try
					{
						mConfiguration.LoadXml (value);
					}
					catch (Exception exc)
					{
						System.Diagnostics.Debug.Print (exc.Message);
					}
				}

				XmlNode lRootNode = mConfiguration.SelectSingleNode ("configuration");
				XmlNode lMakeCabPath = null;
				XmlNode lSignToolPath = null;
				XmlNode lCabFileName = null;
				XmlNode lCabPathName = null;
				XmlNode lTimeStampUrl = null;
				XmlNode lCertificatePath = null;

				if (lRootNode != null)
				{
					lMakeCabPath = lRootNode.Attributes.GetNamedItem (mMakeCabConfig);
					lSignToolPath = lRootNode.Attributes.GetNamedItem (mSignToolConfig);
					lCabFileName = lRootNode.Attributes.GetNamedItem (mCabFileConfig);
					lCabPathName = lRootNode.Attributes.GetNamedItem (mCabPathConfig);
					lTimeStampUrl = lRootNode.Attributes.GetNamedItem (mTimeStampConfig);
					lCertificatePath = lRootNode.Attributes.GetNamedItem (mCertificateConfig);
				}
				MakeCabPath = (lMakeCabPath == null) ? String.Empty : lMakeCabPath.Value.Trim ();
				SignToolPath = (lSignToolPath == null) ? String.Empty : lSignToolPath.Value.Trim ();
				CabFileName = (lCabFileName == null) ? String.Empty : lCabFileName.Value.Trim ();
				CabPathName = (lCabPathName == null) ? String.Empty : lCabPathName.Value.Trim ();
				TimeStampUrl = (lTimeStampUrl == null) ? String.Empty : lTimeStampUrl.Value.Trim ();
				CertificateSpec = (lCertificatePath == null) ? String.Empty : lCertificatePath.Value.Trim ();

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
		/// Indicates that the <see cref="P:SandcastleBuilder.PlugIns.CinSoft.PackAndSignMSHCSettings.CertificateSpec"/> is for a certificate in a certificate store.
		/// </summary>
		public bool IsStoreCertificate
		{
			get
			{
				if (!String.IsNullOrWhiteSpace (CertificateSpec))
				{
					try
					{
						String lPrefix = CertificateSpec.Trim ().Split (' ')[0];

						if (IsStoreFlag (lPrefix, StoreStoreFlag))
						{
							return true;
						}
						if (IsStoreFlag (lPrefix, StoreNameFlag))
						{
							return true;
						}
						if (IsStoreFlag (lPrefix, StoreIssuerFlag))
						{
							return true;
						}
						if (IsStoreFlag (lPrefix, StoreTimestamperFlag))
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
				String lStore = GetStorePart (StoreStoreFlag);
				if (String.IsNullOrWhiteSpace (lStore))
				{
					lStore = DefaultCertificateStore;
				}
				return lStore;
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
		/// Indicates that the <see cref="P:SandcastleBuilder.PlugIns.CinSoft.PackAndSignMSHCSettings.CertificateSpec"/> is for a certificate file.
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
				return (mProject == null) ? String.Empty : Path.ChangeExtension (mProject.HtmlHelpName, ".cab");
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
				return (mProject == null) ? String.Empty : mProject.OutputPath;
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
			String lErrorMessage = String.Empty;
			return LoadCertificate (out lErrorMessage);
		}
		/// <summary>
		/// Loads an Authenticode code-signing cerificate from the certificate file, if a file path has been provided.
		/// </summary>
		/// <param name="ErrorMessage">An error message string if the load failed.</param>
		/// <returns>The loaded certificate, if successful.</returns>
		public X509Certificate2 LoadCertificate (out String ErrorMessage)
		{
			X509Certificate2 lCertificate = null;

			ErrorMessage = String.Empty;

			if (IsFileCertificate)
			{
				lCertificate = LoadFileCertificate (FileCertificatePath, out ErrorMessage);

				if (lCertificate != null)
				{
					//TODO - add the certificate to "my" store
				}
			}
			else if (IsStoreCertificate)
			{
				lCertificate = LoadStoreCertificate (StoreCertificateStore, StoreCertificateName, StoreCertificateIssuer, out ErrorMessage);
			}

			return lCertificate;
		}

		//=====================================================================

		/// <summary>
		/// Loads an Authenticode code-signing cerificate from a certificate file.
		/// </summary>
		/// <param name="FilePath">The full path of the file to load.</param>
		/// <returns>The loaded certificate, if successful.</returns>
		public static X509Certificate2 LoadFileCertificate (String FilePath)
		{
			String lErrorMessage = String.Empty;
			return LoadFileCertificate (FilePath, out lErrorMessage);
		}
		/// <summary>
		/// Loads an Authenticode code-signing cerificate from a certificate file.
		/// </summary>
		/// <param name="FilePath">The full path of the file to load.</param>
		/// <param name="ErrorMessage">An error message string if the load failed.</param>
		/// <returns>The loaded certificate, if successful.</returns>
		public static X509Certificate2 LoadFileCertificate (String FilePath, out String ErrorMessage)
		{
			X509Certificate2 lCertificate = null;

			ErrorMessage = String.Empty;

			try
			{
				RequestPfxPassword lRequestPassword = new RequestPfxPassword (Path.GetFileName (FilePath));

				if (lRequestPassword.ShowDialog ().GetValueOrDefault ())
				{
					lCertificate = new X509Certificate2 (FilePath, lRequestPassword.SecurePassword);
				}
			}
			catch (CryptographicException exp)
			{
				System.Diagnostics.Debug.Print (exp.Message);
				ErrorMessage = exp.Message;
			}
			catch (Exception exp)
			{
				System.Diagnostics.Debug.Print (exp.Message);
				ErrorMessage = exp.Message;
			}

			if ((lCertificate != null) && (!lCertificate.HasPrivateKey))
			{
				ErrorMessage = String.Format ("The certificate in {0} does not have a private key.", FilePath);
				lCertificate = null;
			}
			return lCertificate;
		}

		//=====================================================================

		/// <summary>
		/// Loads an Authenticode code-signing cerificate from a certificate store.
		/// </summary>
		/// <param name="StoreName">The certificate store name.</param>
		/// <param name="CertificateName">The certificate's subject name (optional).</param>
		/// <param name="CertificateIssuer">The certificate's issuer name (optional).</param>
		/// <returns>The loaded certificate, if successful.</returns>
		/// <remarks>Either the <paramref name="CertificateName"/> or <paramref name="CertificateIssuer"/> must be provided.
		/// If both are provided, then only a certificate that matches both will be loaded.
		/// </remarks>
		public static X509Certificate2 LoadStoreCertificate (String StoreName, String CertificateName, String CertificateIssuer)
		{
			String lErrorMessage = String.Empty;
			return LoadStoreCertificate (StoreName, CertificateName, CertificateIssuer, out lErrorMessage);
		}
		/// <summary>
		/// Loads an Authenticode code-signing cerificate from a certificate store.
		/// </summary>
		/// <param name="StoreName">The certificate store name.</param>
		/// <param name="CertificateName">The certificate's subject name (optional).</param>
		/// <param name="CertificateIssuer">The certificate's issuer name (optional).</param>
		/// <param name="ErrorMessage">An error message string if the load failed.</param>
		/// <returns>The loaded certificate, if successful.</returns>
		/// <remarks>Either the <paramref name="CertificateName"/> or <paramref name="CertificateIssuer"/> must be provided.
		/// If both are provided, then only a certificate that matches both will be loaded.
		/// </remarks>
		public static X509Certificate2 LoadStoreCertificate (String StoreName, String CertificateName, String CertificateIssuer, out String ErrorMessage)
		{
			X509Certificate2 lCertificate = null;

			ErrorMessage = String.Empty;

			try
			{
				X509Store lCertStore = new X509Store (StoreName, StoreLocation.CurrentUser);
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
						ErrorMessage = String.Format ("Certificate store \"{0}\" is empty.", StoreName);
					}

					if ((lCertificates != null) && (lCertificates.Count > 0))
					{
						if (!String.IsNullOrEmpty (CertificateName))
						{
							lCertificates = lCertificates.Find (X509FindType.FindBySubjectName, CertificateName, true);
						}
						if ((lCertificates != null) && (lCertificates.Count > 0) && !String.IsNullOrEmpty (CertificateIssuer))
						{
							lCertificates = lCertificates.Find (X509FindType.FindByIssuerName, CertificateIssuer, true);
						}
					}
					else
					{
						ErrorMessage = String.Format ("Certificate store \"{0}\" does not contain any code signing certificates.", StoreName);
					}

					if ((lCertificates != null) && (lCertificates.Count > 0))
					{
						lCertificate = lCertificates[0];
					}
					else
					{
						ErrorMessage = String.Format ("Certificate store \"{0}\" does not contain the specified certificate.", StoreName);
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

			if ((lCertificate != null) && (!lCertificate.HasPrivateKey))
			{
				ErrorMessage = String.Format ("The certificate for {0} from {1} does not have a private key.", lCertificate.GetNameInfo (X509NameType.SimpleName, false), lCertificate.GetNameInfo (X509NameType.SimpleName, true));
				lCertificate = null;
			}
			return lCertificate;
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
				String[] lPaths = Environment.GetEnvironmentVariable ("PATH").Split (';');
				foreach (String lPath in lPaths)
				{
					try
					{
						String lFilePath = Path.Combine (Path.GetFullPath (Environment.ExpandEnvironmentVariables (lPath)), "makecab.exe");

						if (File.Exists (lFilePath))
						{
							return lFilePath;
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
				String[] lPaths = Environment.GetEnvironmentVariable ("PATH").Split (';');
				foreach (String lPath in lPaths)
				{
					try
					{
						String lFilePath = Path.Combine (Path.GetFullPath (Environment.ExpandEnvironmentVariables (lPath)), "signtool.exe");

						if (File.Exists (lFilePath))
						{
							return lFilePath;
						}
					}
					catch { }
				}
			}
			catch { }
			return String.Empty;
		}

		//=====================================================================

		private static bool IsStoreFlag (String Flag, String StoreFlag)
		{
			if (Flag.StartsWith ("-"))
			{
				Flag = "/" + Flag.Substring (1);
			}
			return (String.Compare (Flag, StoreFlag, StringComparison.InvariantCultureIgnoreCase) == 0);
		}

		private String GetStorePart (String StoreFlag)
		{
			if (!String.IsNullOrEmpty (CertificateSpec))
			{
				try
				{
					Regex lRegEx = new Regex ("(?<flag>(/|-)\\w+)\\s+(?<value>(\".*\")|\\S+)(?(/|-|$))", RegexOptions.CultureInvariant);

					foreach (Match lMatch in lRegEx.Matches (CertificateSpec))
					{
						String lFlag = String.Empty;
						String lValue = String.Empty;
						int lGroupNdx;

						for (lGroupNdx = 0; lGroupNdx < lMatch.Groups.Count; lGroupNdx++)
						{
							if (lRegEx.GroupNameFromNumber (lGroupNdx) == "flag")
							{
								lFlag = lMatch.Groups[lGroupNdx].Value;
							}
							else
								if (lRegEx.GroupNameFromNumber (lGroupNdx) == "value")
								{
									lValue = lMatch.Groups[lGroupNdx].Value;
								}
						}
						if (IsStoreFlag (lFlag, StoreFlag))
						{
							return lValue;
						}
					}
				}
				catch { }
			}
			return String.Empty;
		}

		private String SetStorePart (String StoreFlag, String Value)
		{
			String lStoreStore = IsStoreFlag (StoreFlag, StoreStoreFlag) ? Value : StoreCertificateStore;
			String lStoreName = IsStoreFlag (StoreFlag, StoreNameFlag) ? Value : StoreCertificateName;
			String lStoreIssuer = IsStoreFlag (StoreFlag, StoreIssuerFlag) ? Value : StoreCertificateIssuer;
			List<String> lStoreParts = new List<String> ();

			if (!String.IsNullOrWhiteSpace (lStoreStore))
			{
				lStoreParts.Add (StoreStoreFlag);
				lStoreParts.Add (lStoreStore);
			}
			if (!String.IsNullOrWhiteSpace (lStoreName))
			{
				lStoreParts.Add (StoreNameFlag);
				lStoreParts.Add ("\"" + lStoreName + "\"");
			}
			if (!String.IsNullOrWhiteSpace (lStoreIssuer))
			{
				lStoreParts.Add (StoreIssuerFlag);
				lStoreParts.Add ("\"" + lStoreIssuer + "\"");
			}

			return String.Join (" ", lStoreParts.ToArray ());
		}

		#endregion
	}
}
