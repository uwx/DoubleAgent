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
	class PackAndSignMSHCSettings
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

		public PackAndSignMSHCSettings ()
		{
			this.Configuration = String.Empty;
		}

		public PackAndSignMSHCSettings (SandcastleProject Project)
			: this (Project, String.Empty)
		{
		}

		public PackAndSignMSHCSettings (String Configuration)
		{
			this.Configuration = Configuration;
		}

		public PackAndSignMSHCSettings (SandcastleProject Project, String Configuration)
		{
			mProject = Project;
			this.Configuration = Configuration;
		}

		#endregion

		#region Properties
		//=====================================================================

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
				lCertificatePath.Value = CertificatePath;

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
				CertificatePath = (lCertificatePath == null) ? String.Empty : lCertificatePath.Value.Trim ();

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

		public String MakeCabPath { get; set; }
		public String SignToolPath { get; set; }
		public String CabFileName { get; set; }
		public String CabPathName { get; set; }
		public String TimeStampUrl { get; set; }
		public String CertificatePath { get; set; }

		//=====================================================================

		public bool IsStoreCertificate
		{
			get
			{
				if (!String.IsNullOrWhiteSpace (CertificatePath))
				{
					try
					{
						String lPrefix = CertificatePath.Trim ().Split (' ')[0];

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
				CertificatePath = SetStorePart (StoreStoreFlag, value);
			}
		}

		public String StoreCertificateName
		{
			get
			{
				return GetStorePart (StoreNameFlag).Trim ('"');
			}
			set
			{
				CertificatePath = SetStorePart (StoreNameFlag, value);
			}
		}

		public String StoreCertificateIssuer
		{
			get
			{
				return (GetStorePart (StoreIssuerFlag)).Trim ('"');
			}
			set
			{
				CertificatePath = SetStorePart (StoreIssuerFlag, value);
			}
		}

		//=====================================================================

		public bool IsFileCertificate
		{
			get
			{
				if (!String.IsNullOrEmpty (CertificatePath) && !IsStoreCertificate)
				{
					return true;
				}
				return false;
			}
		}

		public String FileCertificatePath
		{
			get
			{
				return IsFileCertificate ? CertificatePath : String.Empty;
			}
		}

		//=====================================================================

		public String DefaultCabFileName
		{
			get
			{
				return (mProject == null) ? String.Empty : Path.ChangeExtension (mProject.HtmlHelpName, ".cab");
			}
		}

		public String DefaultCabPathName
		{
			get
			{
				return (mProject == null) ? String.Empty : mProject.OutputPath;
			}
		}

		public static String DefaultCertificateStore
		{
			get
			{
				return "my";
			}
		}

		//=====================================================================

		public static String StoreStoreFlag
		{
			get { return "/s"; }
		}
		public static String StoreIssuerFlag
		{
			get { return "/i"; }
		}
		public static String StoreNameFlag
		{
			get { return "/n"; }
		}
		public static String StoreTimestamperFlag
		{
			get { return "/t"; }
		}

		#endregion

		#region Public Methods
		//=====================================================================

		public X509Certificate2 LoadCertificate ()
		{
			String lErrorMessage = String.Empty;
			return LoadCertificate (out lErrorMessage);
		}
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

		public static X509Certificate2 LoadFileCertificate (String FilePath)
		{
			String lErrorMessage = String.Empty;
			return LoadFileCertificate (FilePath, out lErrorMessage);
		}
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

		public static X509Certificate2 LoadStoreCertificate (String StoreName, String CertificateName, String CertificateIssuer)
		{
			String lErrorMessage = String.Empty;
			return LoadStoreCertificate (StoreName, CertificateName, CertificateIssuer, out lErrorMessage);
		}
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

		static String FindMakeCab ()
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

		static String FindSignTool ()
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

		private static bool IsStoreFlag (String pFlag, String pStoreFlag)
		{
			if (pFlag.StartsWith ("-"))
			{
				pFlag = "/" + pFlag.Substring (1);
			}
			return (String.Compare (pFlag, pStoreFlag, StringComparison.InvariantCultureIgnoreCase) == 0);
		}

		private String GetStorePart (String pStoreFlag)
		{
			if (!String.IsNullOrEmpty (CertificatePath))
			{
				try
				{
					Regex lRegEx = new Regex ("(?<flag>(/|-)\\w+)\\s+(?<value>(\".*\")|\\S+)(?(/|-|$))", RegexOptions.CultureInvariant);

					foreach (String lGroup in lRegEx.GetGroupNames ())
					{
						System.Diagnostics.Debug.Print ("  Group [{0}]", lGroup);
					}

					foreach (Match lMatch in lRegEx.Matches (CertificatePath))
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
						if (IsStoreFlag (lFlag, pStoreFlag))
						{
							return lValue;
						}
					}
				}
				catch { }
			}
			return String.Empty;
		}
		private String SetStorePart (String pStoreFlag, String pValue)
		{
			String lStoreStore = IsStoreFlag (pStoreFlag, StoreStoreFlag) ? pValue : StoreCertificateStore;
			String lStoreName = IsStoreFlag (pStoreFlag, StoreNameFlag) ? pValue : StoreCertificateName;
			String lStoreIssuer = IsStoreFlag (pStoreFlag, StoreIssuerFlag) ? pValue : StoreCertificateIssuer;
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
