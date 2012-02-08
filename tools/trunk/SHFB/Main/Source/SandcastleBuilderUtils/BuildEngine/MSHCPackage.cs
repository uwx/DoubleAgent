using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Packaging;
using System.Xml;
using Microsoft.Build.Evaluation;

namespace SandcastleBuilder.Utils.BuildEngine
{
	class MSHCPackage : IDisposable
	{
		#region Private data members
		//=====================================================================

		String m_PackagePath;
		Package m_Package = null;
		XmlDocument m_DefaultContentTypes = null;
		ProjectCollection m_ProjectCollection = null;
		Dictionary<String, String> m_GlobalProperties = new Dictionary<String, String> ();

		#endregion

		#region Constructors
		//=====================================================================

		public MSHCPackage (String PackagePath, System.IO.FileMode OpenMode, System.IO.FileAccess OpenAccess, System.IO.FileShare OpenShare)
		{
			this.PackagePath = PackagePath;
			m_Package = Package.Open (this.PackagePath, OpenMode, OpenAccess, OpenShare);
		}
		public MSHCPackage (String PackagePath, System.IO.FileMode OpenMode, System.IO.FileAccess OpenAccess)
			: this (PackagePath, OpenMode, OpenAccess, (OpenAccess == FileAccess.Read) ? FileShare.Read : FileShare.None)
		{ }
		public MSHCPackage (String PackagePath, System.IO.FileMode OpenMode)
			: this (PackagePath, OpenMode, (OpenMode == FileMode.Open) ? FileAccess.Read : FileAccess.ReadWrite)
		{ }
		public MSHCPackage (String PackagePath)
			: this (PackagePath, FileMode.Open)
		{ }

		public MSHCPackage (String PackageName, String PackagePath, System.IO.FileMode OpenMode, System.IO.FileAccess OpenAccess)
			: this (Path.Combine (PackagePath, PackageName), OpenMode, OpenAccess)
		{ }
		public MSHCPackage (String PackageName, String PackagePath, System.IO.FileMode OpenMode)
			: this (Path.Combine (PackagePath, PackageName), OpenMode)
		{ }
		public MSHCPackage (String PackageName, String PackagePath)
			: this (Path.Combine (PackagePath, PackageName))
		{ }

		#endregion

		#region IDisposable implementation
		//=====================================================================

		/// <inheritDoc/>
		~MSHCPackage ()
		{
			this.Dispose (false);
		}

		/// <inheritDoc/>
		public void Dispose ()
		{
			this.Dispose (true);
			GC.SuppressFinalize (this);
		}

		/// <inheritDoc/>
		protected virtual void Dispose (bool disposing)
		{
			if (m_Package != null)
			{
				try
				{
					m_Package.Close ();
				}
				catch (Exception exp)
				{
#if DEBUG
					Debug.Print (exp.Message);
#endif
				}
			}
		}

		#endregion

		#region Public Properties
		//=====================================================================

		bool IsOpen
		{
			get { return (m_Package != null); }
		}

		public String PackagePath
		{
			get
			{
				return m_PackagePath;
			}
			protected set
			{
				m_PackagePath = value;
				if (!String.IsNullOrEmpty (m_PackagePath))
				{
					if (!Path.HasExtension (m_PackagePath) || (String.Compare (Path.GetExtension (m_PackagePath), ".mshc", StringComparison.OrdinalIgnoreCase) != 0))
					{
						m_PackagePath += ".mshc";
					}
				}
			}
		}

		public String PackageName
		{
			get
			{
				if (!String.IsNullOrEmpty (PackagePath))
				{
					return Path.GetFileNameWithoutExtension (PackagePath);
				}
				return String.Empty;
			}
		}

		Dictionary<String, String> ManifestProperties
		{
			get { return m_GlobalProperties; }
		}

		#endregion

		#region Public Methods
		#region Contents Queries
		//=====================================================================

		public String[] GetPartNames ()
		{
			List<String> lPartNames = new List<String> ();

			if (IsOpen)
			{
				foreach (PackagePart lPart in m_Package.GetParts ())
				{
					lPartNames.Add (PartName (lPart));
				}
			}
			return lPartNames.ToArray ();
		}

		public bool PartExists (String PartName)
		{
			if (IsOpen)
			{
				if (m_Package.PartExists (PartUri (PartName)))
				{
					return true;
				}
			}
			return false;
		}

		#endregion

		#region Copy Single Part
		//=====================================================================

		public bool GetPart (String PartName, String TargetPath)
		{
			return GetPart (PartName, TargetPath, false);
		}
		public bool GetPart (String PartName, String TargetPath, bool ReplaceTarget)
		{
			bool lRet = false;

			if (PartExists (PartName))
			{
				if (ReplaceTarget || !File.Exists (TargetPath))
				{
					String lTargetFolder = Path.GetDirectoryName (TargetPath);
					System.IO.FileStream lStream;

					if (!Directory.Exists (lTargetFolder))
					{
						Directory.CreateDirectory (lTargetFolder);
					}
					lStream = System.IO.File.OpenWrite (TargetPath);

					if (lStream != null)
					{
						lRet = GetPart (PartName, lStream);
						lStream.Close ();
					}
				}
			}
			return lRet;
		}
		public bool GetPart (String PartName, System.IO.Stream TargetStream)
		{
			bool lRet = false;

			if (PartExists (PartName))
			{
				PackagePart lPart = m_Package.GetPart (PartUri (PartName));
				System.IO.Stream lPartStream = null;

				if (lPart != null)
				{
					lPartStream = lPart.GetStream (FileMode.Open, FileAccess.Read);
				}
				if (lPartStream != null)
				{
					lPartStream.CopyTo (TargetStream);
					lPartStream.Close ();
					lRet = true;
				}
			}
			return lRet;
		}

		//=====================================================================

		public bool PutPart (String PartName, String SourcePath)
		{
			return PutPart (PartName, SourcePath, false);
		}
		public bool PutPart (String PartName, String SourcePath, bool ReplaceTarget)
		{
			return PutPart (PartName, SourcePath, ReplaceTarget, DefaultPartType (SourcePath));
		}
		public bool PutPart (String PartName, String SourcePath, bool ReplaceTarget, String TargetType)
		{
			bool lRet = false;

			if (IsOpen && !PartExists (PartName) || ReplaceTarget)
			{
				System.IO.FileStream lStream = System.IO.File.OpenRead (SourcePath);

				if (lStream != null)
				{
					lRet = PutPart (PartName, lStream, ReplaceTarget, TargetType);
					lStream.Close ();
				}
			}
			return lRet;
		}
		public bool PutPart (String PartName, System.IO.Stream SourceStream)
		{
			return PutPart (PartName, SourceStream, false);
		}
		public bool PutPart (String PartName, System.IO.Stream SourceStream, bool ReplaceTarget)
		{
			return PutPart (PartName, SourceStream, ReplaceTarget, String.Empty);
		}
		public bool PutPart (String PartName, System.IO.Stream SourceStream, bool ReplaceTarget, String TargetType)
		{
			bool lRet = false;

			if (IsOpen)
			{
				PackagePart lPart;
				System.IO.Stream lPartStream = null;

				if (PartExists (PartName))
				{
					if (ReplaceTarget)
					{
						m_Package.DeletePart (PartUri (PartName));
					}
					else
					{
						return false;
					}
				}

				lPart = m_Package.CreatePart (PartUri (PartName), TargetType, CompressionOption.Maximum);
				if (lPart != null)
				{
					lPartStream = lPart.GetStream (FileMode.Create, FileAccess.ReadWrite);
				}
				if (lPartStream != null)
				{
					SourceStream.CopyTo (lPartStream);
					lPartStream.Close ();
					lRet = true;
				}
			}
			return lRet;
		}

		#endregion

		#region Copy All Parts
		//=====================================================================

		public uint GetAllParts (String TargetFolder)
		{
			return GetAllParts (TargetFolder, false);
		}
		public uint GetAllParts (String TargetFolder, bool ReplaceTargets)
		{
			uint lPartCount = 0;

			if (IsOpen)
			{
				Uri lTargetFolder = new Uri (TargetFolder);
				Uri lTargetUri;
				String lTargetPath;

				if (!lTargetFolder.ToString ().EndsWith ("/"))
				{
					lTargetFolder = new Uri (lTargetFolder.ToString () + "/");
				}

				foreach (PackagePart lPart in m_Package.GetParts ())
				{
					lTargetUri = new Uri (lTargetFolder, PartName (lPart));
					lTargetPath = Path.GetDirectoryName (lTargetUri.AbsolutePath);

					if (!Directory.Exists (lTargetPath))
					{
						Directory.CreateDirectory (lTargetPath);
					}
					if (GetPart (PartName (lPart), lTargetUri.AbsolutePath, ReplaceTargets))
					{
						lPartCount++;
					}
				}
			}
			return lPartCount;
		}

		public uint PutAllParts (String SourceFolder)
		{
			return PutAllParts (SourceFolder, false);
		}
		public uint PutAllParts (String SourceFolder, bool ReplaceTargets)
		{
			return PutAllParts (SourceFolder, true, ReplaceTargets);
		}
		public uint PutAllParts (String SourceFolder, bool Recursive, bool ReplaceTargets)
		{
			return PutAllParts (SourceFolder, Recursive, ReplaceTargets, null);
		}
		private uint PutAllParts (String SourceFolder, bool Recursive, bool ReplaceTargets, String TargetPath)
		{
			uint lPartCount = 0;
			DirectoryInfo lSourceFolder = new DirectoryInfo (SourceFolder);

			foreach (FileInfo lFile in lSourceFolder.GetFiles ())
			{
				Uri lPartUri = PartUri (String.IsNullOrEmpty (TargetPath) ? lFile.Name : Path.Combine (TargetPath, lFile.Name));

				if (PutPart (PartName (lPartUri), lFile.FullName, ReplaceTargets))
				{
					lPartCount++;
				}
			}

			if (Recursive)
			{
				foreach (DirectoryInfo lFolder in lSourceFolder.GetDirectories ())
				{
					String lTargetPath = lFolder.Name;
					if (!String.IsNullOrEmpty (TargetPath))
					{
						lTargetPath = Path.Combine (TargetPath, lTargetPath);
					}
					lPartCount += PutAllParts (lFolder.FullName, Recursive, ReplaceTargets, lTargetPath);
				}
			}
			return lPartCount;
		}

		#endregion

		#region Copy Parts by Manifest
		//=====================================================================

		public uint GetTheseParts (String TargetFolder, String ManifestPath)
		{
			return GetTheseParts (TargetFolder, GetManifestProject (ManifestPath));
		}
		public uint GetTheseParts (String TargetFolder, String ManifestPath, bool ReplaceTargets)
		{
			return GetTheseParts (TargetFolder, GetManifestProject (ManifestPath), ReplaceTargets);
		}
		public uint GetTheseParts (String TargetFolder, Microsoft.Build.Evaluation.Project Manifest)
		{
			return GetTheseParts (TargetFolder, Manifest, false);
		}
		public uint GetTheseParts (String TargetFolder, Microsoft.Build.Evaluation.Project Manifest, bool ReplaceTargets)
		{
			uint lPartCount = 0;

			if (IsOpen && (Manifest != null))
			{
				try
				{
					foreach (ProjectItem lManifestItem in Manifest.AllEvaluatedItems)
					{
						bool lCopyItem = true;
						ProjectMetadata lMetaData;

						if ((lMetaData = lManifestItem.GetMetadata ("CopyToOutputDirectory")) != null)
						{
							lCopyItem = bool.Parse (lMetaData.EvaluatedValue);
						}
						if (lCopyItem && GetPart (lManifestItem.EvaluatedInclude, Path.Combine (TargetFolder, lManifestItem.EvaluatedInclude), ReplaceTargets))
						{
							lPartCount++;
						}
					}
				}
				catch (Exception exp)
				{
#if DEBUG
					Debug.Print (exp.Message);
#endif
				}
			}
			return lPartCount;
		}

		public uint PutTheseParts (String SourceFolder, String ManifestPath)
		{
			return PutTheseParts (SourceFolder, GetManifestProject (ManifestPath));
		}
		public uint PutTheseParts (String SourceFolder, String ManifestPath, bool ReplaceTargets)
		{
			return PutTheseParts (SourceFolder, GetManifestProject (ManifestPath), ReplaceTargets);
		}
		public uint PutTheseParts (String SourceFolder, Microsoft.Build.Evaluation.Project Manifest)
		{
			return PutTheseParts (SourceFolder, Manifest, false);
		}
		public uint PutTheseParts (String SourceFolder, Microsoft.Build.Evaluation.Project Manifest, bool ReplaceTargets)
		{
			uint lPartCount = 0;

			if (IsOpen && (Manifest != null))
			{
				try
				{
					foreach (ProjectItem lManifestItem in Manifest.AllEvaluatedItems)
					{
						bool lCopyItem = true;
						String lItemType = null;
						ProjectMetadata lMetaData;

						if ((lMetaData = lManifestItem.GetMetadata ("SubType")) != null)
						{
							lItemType = lMetaData.EvaluatedValue;
						}
						if ((lMetaData = lManifestItem.GetMetadata ("CopyToOutputDirectory")) != null)
						{
							lCopyItem = bool.Parse (lMetaData.EvaluatedValue);
						}
						if (lCopyItem && PutPart (lManifestItem.EvaluatedInclude, Path.Combine (Manifest.DirectoryPath, lManifestItem.EvaluatedInclude), ReplaceTargets, lItemType))
						{
							lPartCount++;
						}
					}
				}
				catch (Exception exp)
				{
#if DEBUG
					Debug.Print (exp.Message);
#endif
				}
			}
			return lPartCount;
		}

		private Project GetManifestProject (String ManifestPath)
		{
			if (m_ProjectCollection == null)
			{
				m_ProjectCollection = new ProjectCollection (ManifestProperties);
			}
			return new Project (ManifestPath, null, null, m_ProjectCollection);
		}

		#endregion
		#endregion

		#region Helper Methods
		//=====================================================================

		static private Uri PartUri (String PartName)
		{
			String lPartName = PartName.Replace ('\\', '/');
			if (!lPartName.StartsWith ("/"))
			{
				lPartName = "/" + lPartName;
			}
			return new Uri (lPartName, UriKind.Relative);
		}
		static private String PartName (PackagePart Part)
		{
			if (Part != null)
			{
				return PartName (Part.Uri);
			}
			return String.Empty;
		}
		static private String PartName (Uri PartUri)
		{
			if (PartUri != null)
			{
				return PartUri.ToString ().TrimStart ('\\', '/');
			}
			return String.Empty;
		}

		public String DefaultPartType (String PartName)
		{
			String lPartType = String.Empty;

			try
			{
				String lNameExt = Path.GetExtension (PartName);
				XmlNamespaceManager lNamespaceManager;
				XmlNode lTypeNode;
				XmlNode lTypeAttribute;

				if (m_DefaultContentTypes == null)
				{
					m_DefaultContentTypes = new XmlDocument ();
					m_DefaultContentTypes.LoadXml (
						"<?xml version='1.0' encoding='utf-8'?>" +
						"<Types xmlns='http://schemas.openxmlformats.org/package/2006/content-types'>" +
							"<Default Extension='css' ContentType='text/richtext' />" +
							"<Default Extension='js' ContentType='text/richtext' />" +
							"<Default Extension='gif' ContentType='image/gif' />" +
							"<Default Extension='ico' ContentType='image/gif' />" +
							"<Default Extension='png' ContentType='image/png' />" +
							"<Default Extension='xml' ContentType='text/xml' />" +
							"<Default Extension='xslt' ContentType='text/richtext' />" +
							"<Default Extension='htm' ContentType='text/html' />" +
						"</Types>");
				}
				lNamespaceManager = new XmlNamespaceManager (m_DefaultContentTypes.NameTable);
				lNamespaceManager.AddNamespace ("openxml", "http://schemas.openxmlformats.org/package/2006/content-types");

				lTypeNode = m_DefaultContentTypes.DocumentElement.SelectSingleNode (String.Format ("openxml:Default[@Extension=substring-after('{0}','.')]", lNameExt), lNamespaceManager);
				if (lTypeNode != null)
				{
					lTypeAttribute = lTypeNode.Attributes.GetNamedItem ("ContentType");
					if (lTypeAttribute != null)
					{
						lPartType = lTypeAttribute.Value;
					}
				}
			}
			catch (Exception exp)
			{
#if DEBUG
				Debug.Print (exp.Message);
#endif
			}
			return lPartType;
		}

		#endregion
	}
}
