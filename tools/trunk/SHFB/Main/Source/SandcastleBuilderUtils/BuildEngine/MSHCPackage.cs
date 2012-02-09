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

		String m_packagePath;
		Package m_package = null;
		XmlDocument m_defaultContentTypes = null;
		ProjectCollection m_projectCollection = null;
		Dictionary<String, String> m_globalProperties = new Dictionary<String, String> ();

		#endregion

		#region Constructors
		//=====================================================================

		public MSHCPackage (String PackagePath, System.IO.FileMode OpenMode, System.IO.FileAccess OpenAccess, System.IO.FileShare OpenShare)
		{
			this.PackagePath = PackagePath;
			m_package = Package.Open (this.PackagePath, OpenMode, OpenAccess, OpenShare);
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
			if (m_package != null)
			{
				try
				{
					m_package.Close ();
				}
#if DEBUG
				catch (Exception exp)
				{
					Debug.Print (exp.Message);
				}
#else
				catch {}
#endif
			}
		}

		#endregion

		#region Public Properties
		//=====================================================================

		bool IsOpen
		{
			get { return (m_package != null); }
		}

		public String PackagePath
		{
			get
			{
				return m_packagePath;
			}
			protected set
			{
				m_packagePath = value;
				if (!String.IsNullOrEmpty (m_packagePath))
				{
					if (!Path.HasExtension (m_packagePath) || (String.Compare (Path.GetExtension (m_packagePath), ".mshc", StringComparison.OrdinalIgnoreCase) != 0))
					{
						m_packagePath += ".mshc";
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
			get { return m_globalProperties; }
		}

		#endregion

		#region Public Methods
		#region Contents Queries
		//=====================================================================

		public String[] GetPartNames ()
		{
			List<String> v_partNames = new List<String> ();

			if (IsOpen)
			{
				foreach (PackagePart v_part in m_package.GetParts ())
				{
					v_partNames.Add (partName (v_part));
				}
			}
			return v_partNames.ToArray ();
		}

		public bool PartExists (String partName)
		{
			if (IsOpen)
			{
				if (m_package.PartExists (PartUri (partName)))
				{
					return true;
				}
			}
			return false;
		}

		#endregion

		#region Copy Single Part
		//=====================================================================

		public bool GetPart (String partName, String targetPath)
		{
			return GetPart (partName, targetPath, false);
		}
		public bool GetPart (String partName, String targetPath, bool replaceTarget)
		{
			bool v_ret = false;

			if (PartExists (partName))
			{
				if (replaceTarget || !File.Exists (targetPath))
				{
					String v_targetFolder = Path.GetDirectoryName (targetPath);
					System.IO.FileStream v_stream;

					if (!Directory.Exists (v_targetFolder))
					{
						Directory.CreateDirectory (v_targetFolder);
					}
					v_stream = System.IO.File.OpenWrite (targetPath);

					if (v_stream != null)
					{
						v_ret = GetPart (partName, v_stream);
						v_stream.Close ();
					}
				}
			}
			return v_ret;
		}
		public bool GetPart (String partName, System.IO.Stream targetStream)
		{
			bool v_ret = false;

			if (PartExists (partName))
			{
				PackagePart v_part = m_package.GetPart (PartUri (partName));
				System.IO.Stream v_partStream = null;

				if (v_part != null)
				{
					v_partStream = v_part.GetStream (FileMode.Open, FileAccess.Read);
				}
				if (v_partStream != null)
				{
					v_partStream.CopyTo (targetStream);
					v_partStream.Close ();
					v_ret = true;
				}
			}
			return v_ret;
		}

		//=====================================================================

		public bool PutPart (String partName, String sourcePath)
		{
			return PutPart (partName, sourcePath, false);
		}
		public bool PutPart (String partName, String sourcePath, bool replaceTarget)
		{
			return PutPart (partName, sourcePath, replaceTarget, DefaultPartType (sourcePath));
		}
		public bool PutPart (String partName, String sourcePath, bool replaceTarget, String targetType)
		{
			bool v_ret = false;

			if (IsOpen && !PartExists (partName) || replaceTarget)
			{
				System.IO.FileStream v_stream = System.IO.File.OpenRead (sourcePath);

				if (v_stream != null)
				{
					v_ret = PutPart (partName, v_stream, replaceTarget, targetType);
					v_stream.Close ();
				}
			}
			return v_ret;
		}
		public bool PutPart (String partName, System.IO.Stream sourceStream)
		{
			return PutPart (partName, sourceStream, false);
		}
		public bool PutPart (String partName, System.IO.Stream sourceStream, bool replaceTarget)
		{
			return PutPart (partName, sourceStream, replaceTarget, String.Empty);
		}
		public bool PutPart (String partName, System.IO.Stream sourceStream, bool replaceTarget, String targetType)
		{
			bool v_ret = false;

			if (IsOpen)
			{
				PackagePart v_part;
				System.IO.Stream v_partStream = null;

				if (PartExists (partName))
				{
					if (replaceTarget)
					{
						m_package.DeletePart (PartUri (partName));
					}
					else
					{
						return false;
					}
				}

				v_part = m_package.CreatePart (PartUri (partName), targetType, CompressionOption.Maximum);
				if (v_part != null)
				{
					v_partStream = v_part.GetStream (FileMode.Create, FileAccess.ReadWrite);
				}
				if (v_partStream != null)
				{
					sourceStream.CopyTo (v_partStream);
					v_partStream.Close ();
					v_ret = true;
				}
			}
			return v_ret;
		}

		#endregion

		#region Copy All Parts
		//=====================================================================

		public uint GetAllParts (String targetFolder)
		{
			return GetAllParts (targetFolder, false);
		}
		public uint GetAllParts (String targetFolder, bool replaceTargets)
		{
			uint v_partCount = 0;

			if (IsOpen)
			{
				Uri v_targetFolder = new Uri (targetFolder);
				Uri v_targetUri;
				String v_targetPath;

				if (!v_targetFolder.ToString ().EndsWith ("/"))
				{
					v_targetFolder = new Uri (v_targetFolder.ToString () + "/");
				}

				foreach (PackagePart v_part in m_package.GetParts ())
				{
					v_targetUri = new Uri (v_targetFolder, partName (v_part));
					v_targetPath = Path.GetDirectoryName (v_targetUri.AbsolutePath);

					if (!Directory.Exists (v_targetPath))
					{
						Directory.CreateDirectory (v_targetPath);
					}
					if (GetPart (partName (v_part), v_targetUri.AbsolutePath, replaceTargets))
					{
						v_partCount++;
					}
				}
			}
			return v_partCount;
		}

		public uint PutAllParts (String sourceFolder)
		{
			return PutAllParts (sourceFolder, false);
		}
		public uint PutAllParts (String sourceFolder, bool replaceTargets)
		{
			return PutAllParts (sourceFolder, true, replaceTargets);
		}
		public uint PutAllParts (String sourceFolder, bool recursive, bool replaceTargets)
		{
			return PutAllParts (sourceFolder, recursive, replaceTargets, null);
		}
		private uint PutAllParts (String sourceFolder, bool recursive, bool replaceTargets, String targetPath)
		{
			uint v_partCount = 0;
			DirectoryInfo v_sourceFolder = new DirectoryInfo (sourceFolder);

			foreach (FileInfo v_file in v_sourceFolder.GetFiles ())
			{
				Uri v_partUri = PartUri (String.IsNullOrEmpty (targetPath) ? v_file.Name : Path.Combine (targetPath, v_file.Name));

				if (PutPart (partName (v_partUri), v_file.FullName, replaceTargets))
				{
					v_partCount++;
				}
			}

			if (recursive)
			{
				foreach (DirectoryInfo v_folder in v_sourceFolder.GetDirectories ())
				{
					String v_targetPath = v_folder.Name;
					if (!String.IsNullOrEmpty (targetPath))
					{
						v_targetPath = Path.Combine (targetPath, v_targetPath);
					}
					v_partCount += PutAllParts (v_folder.FullName, recursive, replaceTargets, v_targetPath);
				}
			}
			return v_partCount;
		}

		#endregion

		#region Copy Parts by manifest
		//=====================================================================

		public uint GetTheseParts (String targetFolder, String manifestPath)
		{
			return GetTheseParts (targetFolder, GetManifestProject (manifestPath));
		}
		public uint GetTheseParts (String targetFolder, String manifestPath, bool replaceTargets)
		{
			return GetTheseParts (targetFolder, GetManifestProject (manifestPath), replaceTargets);
		}
		public uint GetTheseParts (String targetFolder, Microsoft.Build.Evaluation.Project manifest)
		{
			return GetTheseParts (targetFolder, manifest, false);
		}
		public uint GetTheseParts (String targetFolder, Microsoft.Build.Evaluation.Project manifest, bool replaceTargets)
		{
			uint v_partCount = 0;

			if (IsOpen && (manifest != null))
			{
				try
				{
					foreach (ProjectItem v_ManifestItem in manifest.AllEvaluatedItems)
					{
						bool v_copyItem = true;
						ProjectMetadata v_metaData;

						if ((v_metaData = v_ManifestItem.GetMetadata ("CopyToOutputDirectory")) != null)
						{
							v_copyItem = bool.Parse (v_metaData.EvaluatedValue);
						}
						if (v_copyItem && GetPart (v_ManifestItem.EvaluatedInclude, Path.Combine (targetFolder, v_ManifestItem.EvaluatedInclude), replaceTargets))
						{
							v_partCount++;
						}
					}
				}
#if DEBUG
				catch (Exception exp)
				{
					Debug.Print (exp.Message);
				}
#else
				catch {}
#endif
			}
			return v_partCount;
		}

		//=====================================================================

		public uint PutTheseParts (String manifestPath)
		{
			return PutTheseParts (GetManifestProject (manifestPath));
		}
		public uint PutTheseParts (String manifestPath, bool replaceTargets)
		{
			return PutTheseParts (GetManifestProject (manifestPath), replaceTargets);
		}
		public uint PutTheseParts (Microsoft.Build.Evaluation.Project manifest)
		{
			return PutTheseParts (manifest, false);
		}
		public uint PutTheseParts (Microsoft.Build.Evaluation.Project manifest, bool replaceTargets)
		{
			uint v_partCount = 0;

			if (IsOpen && (manifest != null))
			{
				try
				{
					foreach (ProjectItem lManifestItem in manifest.AllEvaluatedItems)
					{
						bool v_copyItem = true;
						String v_itemType = null;
						ProjectMetadata v_metaData;

						if ((v_metaData = lManifestItem.GetMetadata ("SubType")) != null)
						{
							v_itemType = v_metaData.EvaluatedValue;
						}
						if ((v_metaData = lManifestItem.GetMetadata ("CopyToOutputDirectory")) != null)
						{
							v_copyItem = bool.Parse (v_metaData.EvaluatedValue);
						}
						if (v_copyItem && PutPart (lManifestItem.EvaluatedInclude, Path.Combine (manifest.DirectoryPath, lManifestItem.EvaluatedInclude), replaceTargets, v_itemType))
						{
							v_partCount++;
						}
					}
				}
#if DEBUG
				catch (Exception exp)
				{
					Debug.Print (exp.Message);
				}
#else
				catch {}
#endif
			}
			return v_partCount;
		}

		//=====================================================================

		public static uint CopyTheseParts (String targetFolder, String manifestPath)
		{
			return CopyTheseParts (targetFolder, manifestPath, null);
		}
		public static uint CopyTheseParts (String targetFolder, String manifestPath, Dictionary<String, String> manifestProperties)
		{
			return CopyTheseParts (targetFolder, manifestPath, manifestProperties, false);
		}
		public static uint CopyTheseParts (String targetFolder, String manifestPath, bool replaceTargets)
		{
			return CopyTheseParts (targetFolder, manifestPath, null, replaceTargets);
		}
		public static uint CopyTheseParts (String targetFolder, String manifestPath, Dictionary<String, String> manifestProperties, bool replaceTargets)
		{
			ProjectCollection v_projectCollection = new ProjectCollection (manifestProperties);
			return CopyTheseParts (targetFolder, new Project (manifestPath, null, null, v_projectCollection), replaceTargets);
		}
		public static uint CopyTheseParts (String targetFolder, Microsoft.Build.Evaluation.Project manifest)
		{
			return CopyTheseParts (targetFolder, manifest, false);
		}
		public static uint CopyTheseParts (String targetFolder, Microsoft.Build.Evaluation.Project manifest, bool replaceTargets)
		{
			uint v_partCount = 0;
			if (manifest != null)
			{
				try
				{
					String v_targetFolder = Path.GetFullPath (targetFolder.Replace ('/', '\\').TrimEnd ('\\'));

					foreach (ProjectItem lManifestItem in manifest.AllEvaluatedItems)
					{
						bool v_copyItem = true;
						ProjectMetadata v_metaData;
						String v_sourcePath;
						String v_targetPath;

						if ((v_metaData = lManifestItem.GetMetadata ("CopyToOutputDirectory")) != null)
						{
							v_copyItem = bool.Parse (v_metaData.EvaluatedValue);
						}
						if (v_copyItem)
						{
							v_sourcePath = Path.Combine (manifest.DirectoryPath, lManifestItem.EvaluatedInclude);
							v_targetPath = Path.Combine (v_targetFolder, lManifestItem.EvaluatedInclude);

							if (replaceTargets || !File.Exists (v_targetPath))
							{
								if (!Directory.Exists (Path.GetDirectoryName (v_targetPath)))
								{
									Directory.CreateDirectory (Path.GetDirectoryName (v_targetPath));
								}
								File.Copy (v_sourcePath, v_targetPath, replaceTargets);
								File.SetAttributes (v_sourcePath, FileAttributes.Normal);
								v_partCount++;
							}
						}
					}
				}
#if DEBUG
				catch (Exception exp)
				{
					Debug.Print (exp.Message);
				}
#else
				catch {}
#endif
			}
			return v_partCount;
		}

		//=====================================================================

		private Project GetManifestProject (String manifestPath)
		{
			if (m_projectCollection == null)
			{
				m_projectCollection = new ProjectCollection (ManifestProperties);
			}
			return new Project (manifestPath, null, null, m_projectCollection);
		}

		#endregion
		#endregion

		#region Helper Methods
		//=====================================================================

		static private Uri PartUri (String partName)
		{
			String v_partName = partName.Replace ('\\', '/');
			if (!v_partName.StartsWith ("/"))
			{
				v_partName = "/" + v_partName;
			}
			return new Uri (v_partName, UriKind.Relative);
		}
		static private String partName (PackagePart part)
		{
			if (part != null)
			{
				return partName (part.Uri);
			}
			return String.Empty;
		}
		static private String partName (Uri partUri)
		{
			if (partUri != null)
			{
				return partUri.ToString ().TrimStart ('\\', '/');
			}
			return String.Empty;
		}

		public String DefaultPartType (String partName)
		{
			String v_partType = String.Empty;

			try
			{
				String v_nameExt = Path.GetExtension (partName);
				XmlNamespaceManager v_namespaceManager;
				XmlNode v_typeNode;
				XmlNode v_typeAttribute;

				if (m_defaultContentTypes == null)
				{
					m_defaultContentTypes = new XmlDocument ();
					m_defaultContentTypes.LoadXml (
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
				v_namespaceManager = new XmlNamespaceManager (m_defaultContentTypes.NameTable);
				v_namespaceManager.AddNamespace ("openxml", "http://schemas.openxmlformats.org/package/2006/content-types");

				v_typeNode = m_defaultContentTypes.DocumentElement.SelectSingleNode (String.Format ("openxml:Default[@Extension=substring-after('{0}','.')]", v_nameExt), v_namespaceManager);
				if (v_typeNode != null)
				{
					v_typeAttribute = v_typeNode.Attributes.GetNamedItem ("ContentType");
					if (v_typeAttribute != null)
					{
						v_partType = v_typeAttribute.Value;
					}
				}
			}
#if DEBUG
			catch (Exception exp)
			{
				Debug.Print (exp.Message);
			}
#else
			catch {}
#endif
			return v_partType;
		}

		#endregion
	}
}
