using System;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Windows;
using System.Xml;
using System.Xml.XPath;

using SandcastleBuilder.Utils;
using SandcastleBuilder.Utils.BuildEngine;
using SandcastleBuilder.Utils.ConceptualContent;
using SandcastleBuilder.Utils.PlugIn;

namespace SandcastleBuilder.PlugIns
{
/// <summary>
/// 
/// </summary>
	public class TOCFromSitemap : IPlugIn
	{
		#region Private data members
		//=====================================================================

		private ExecutionPointCollection m_executionPoints;
		private BuildProcess m_buildProcess;

		#endregion

		#region IPlugIn Properties
		//=====================================================================

		/// <inheritdoc/>
		public string Name
		{
			get { return "Table of Contents from Sitemap"; }
		}

		/// <inheritdoc/>
		public Version Version
		{
			get
			{
				Assembly v_assembly = Assembly.GetExecutingAssembly ();
				FileVersionInfo v_fileVersionInfo = FileVersionInfo.GetVersionInfo (v_assembly.Location);
				return new Version (v_fileVersionInfo.ProductVersion);
			}
		}

		/// <inheritdoc/>
		public string Copyright
		{
			get
			{
				Assembly v_assembly = Assembly.GetExecutingAssembly ();
				AssemblyCopyrightAttribute v_copyrightAttribute = Attribute.GetCustomAttribute (v_assembly, typeof (AssemblyCopyrightAttribute)) as AssemblyCopyrightAttribute;
				return v_copyrightAttribute.Copyright;
			}
		}

		/// <inheritdoc/>
		public string Description
		{
			get
			{
				return "This plug-in uses a Sitemap to add bookmark links to the Table of Contents.\r\n" +
				"(Help1 and Help2 formats only)";
			}
		}

		/// <inheritdoc/>
		public bool RunsInPartialBuild
		{
			get { return false; }
		}

		/// <inheritdoc/>
		public ExecutionPointCollection ExecutionPoints
		{
			get
			{
				if (m_executionPoints == null)
				{
					m_executionPoints = new ExecutionPointCollection
                    {
                        new ExecutionPoint(BuildStep.GenerateHelpFormatTableOfContents, ExecutionBehaviors.Before),
                        new ExecutionPoint(BuildStep.CombiningIntermediateTocFiles, ExecutionBehaviors.Before)
                    };
				}
				return m_executionPoints;
			}
		}

		#endregion

		#region IPlugIn Methods
		//=====================================================================

		/// <inheritdoc/>
		public string ConfigurePlugIn (SandcastleProject project, string currentConfig)
		{
			MessageBox.Show ("This plug-in has no configurable settings", "Build Process Plug-In", MessageBoxButton.OK, MessageBoxImage.Information);
			return currentConfig;
		}

		/// <inheritdoc/>
		public void Initialize (BuildProcess buildProcess, XPathNavigator configuration)
		{
			m_buildProcess = buildProcess;
		}

		/// <inheritdoc/>
		public void Execute (ExecutionContext context)
		{
#if	DEBUG
			Debug.Print ("{0} {1}", context.Behavior.ToString (), context.BuildStep.ToString ());
#endif

			if ((context.BuildStep == BuildStep.GenerateHelpFormatTableOfContents) && (context.Behavior == ExecutionBehaviors.Before))
			{
				m_buildProcess.ReportProgress ("{0} Version {1}\r\n{2}", this.Name, this.Version, this.Copyright);
				UpdateGenerateHelpFormatProject ();
			}
			else if ((context.BuildStep == BuildStep.CombiningIntermediateTocFiles) && (context.Behavior == ExecutionBehaviors.Before))
			{
				m_buildProcess.ReportProgress ("{0} Version {1}\r\n{2}", this.Name, this.Version, this.Copyright);
#if	DEBUG_NOT
				LogXmlFile (Path.Combine (mBuildProcess.WorkingFolder, "toc.xml"));
#endif
				SitemapToToc ();
			}

#if	DEBUG_NOT
			if 	(context.BuildStep == BuildStep.CombiningIntermediateTocFiles)&& (context.Behavior == ExecutionBehaviors.Before))
			{
				LogXmlFile (Path.Combine (mBuildProcess.WorkingFolder, "_ConceptualTOC_.xml"));
				LogXmlFile (Path.Combine (mBuildProcess.WorkingFolder, "toc.xml"));
			}
#endif
		}
		#endregion

		#region Private Properties
		//=====================================================================

		bool HasSiteMap
		{
			get
			{
				return m_buildProcess.CurrentProject.HasItems (BuildAction.SiteMap);
			}
		}

		FileItem SiteMapFile
		{
			get
			{
				foreach (FileItem v_fileItem in m_buildProcess.CurrentProject.FileItems)
				{
					if (v_fileItem.BuildAction == BuildAction.SiteMap)
					{
						return v_fileItem;
					}
				}
				return null;
			}
		}

		String SiteMapPath
		{
			get
			{
				foreach (FileItem v_fileItem in m_buildProcess.CurrentProject.FileItems)
				{
					if (v_fileItem.BuildAction == BuildAction.SiteMap)
					{
						return v_fileItem.FullPath;
					}
				}
				return String.Empty;
			}
		}

		#endregion

		#region Helper Methods
		//=====================================================================

		private void UpdateGenerateHelpFormatProject ()
		{
			try
			{
				String v_projectPath = Path.Combine (m_buildProcess.WorkingFolder, "Generate2xTOC.proj");
				XmlDocument v_document = new XmlDocument ();
				XmlNodeList v_nodes;
				XmlAttribute v_attribute;
				String v_transform = Path.Combine (Path.GetDirectoryName (Assembly.GetExecutingAssembly ().Location), "TocToHxsContents_Override.xsl");

#if	DEBUG
				Debug.Print ("  Load      '{0}'", v_projectPath);
#endif
				v_document.Load (v_projectPath);
				v_nodes = v_document.DocumentElement.SelectNodes ("//*[@Transformations!='']");
				if (v_nodes != null)
				{
					foreach (XmlNode v_node in v_nodes)
					{
						v_attribute = v_node.Attributes["Transformations"];
						if ((v_attribute != null) && (v_attribute.Value.Contains ("TocToHxSContents")))
						{
#if	DEBUG
							Debug.Print ("  Update    '{0}'", v_projectPath);
							Debug.Print ("    replace '{0}'", v_attribute.Value);
							Debug.Print ("    with    '{0}'", v_transform);
							Debug.Print ("    SiteMap '{0}'", SiteMapPath);
#endif
							m_buildProcess.ReportProgress ("{0}: Update    '{1}'", this.Name, v_projectPath);
							m_buildProcess.ReportProgress ("{0}:   replace '{1}'", this.Name, v_attribute.Value);
							m_buildProcess.ReportProgress ("{0}:   with    '{1}", this.Name, v_transform);
							m_buildProcess.ReportProgress ("{0}:   SiteMap '{1}", this.Name, SiteMapPath);

							v_attribute.Value = v_transform;

							v_attribute = v_document.CreateAttribute ("Arguments");
							v_attribute.Value = "SiteMap=file://" + SiteMapPath;
							v_node.Attributes.Append (v_attribute);

							v_document.Save (v_projectPath);
							break;
						}
					}
				}
			}
			catch (Exception exp)
			{
				System.Diagnostics.Debug.Print (exp.Message);
			}
		}

		//=====================================================================

		private void SitemapToToc ()
		{
			try
			{
				FileItem v_siteMapFile = this.SiteMapFile;
				TocEntryCollection v_siteMapToc;

				if (v_siteMapFile != null)
				{
#if	DEBUG
					Debug.Print ("Load [{0}]", v_siteMapFile.FullPath);
#endif
					m_buildProcess.ReportProgress ("{0}: SitemapToToc '{1}'", this.Name, v_siteMapFile.FullPath);
					v_siteMapToc = new TocEntryCollection (v_siteMapFile);
					v_siteMapToc.Load ();

					TargetTocEntries (v_siteMapToc);
#if	DEBUG_NOT
					LogTocEntries (v_siteMapToc);
#endif
#if	DEBUG_NOT
					Debug.Print ("");
					Debug.Print ("{0}", v_siteMapToc.ToString (HelpFileFormat.MSHelp2));
					Debug.Print ("");
#endif
					try
					{
						String v_tocFilePath = Path.Combine (m_buildProcess.WorkingFolder, "_ConceptualTOC_.xml");
						System.IO.StreamWriter v_writer = new System.IO.StreamWriter (v_tocFilePath);

#if	DEBUG
						Debug.Print ("Save [{0}]", v_tocFilePath);
#endif
						v_writer.WriteLine ("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
						v_writer.WriteLine ("<topics>");

						WriteTocEntries (v_siteMapToc, v_writer);

						v_writer.WriteLine ("</topics>");
						v_writer.Close ();
					}
					catch (Exception exp)
					{
						System.Diagnostics.Debug.Print (exp.Message);
					}
				}
			}
			catch (Exception exp)
			{
				System.Diagnostics.Debug.Print (exp.Message);
			}
		}

		void TargetTocEntries (TocEntryCollection entries)
		{
			foreach (TocEntry v_entry in entries)
			{
				TargetTocEntry (v_entry);
			}
		}

		void TargetTocEntry (TocEntry entry)
		{
			if (entry != null)
			{
				if ((!String.IsNullOrEmpty (entry.SourceFile)) && (String.IsNullOrEmpty (entry.DestinationFile)))
				{
					entry.DestinationFile = FilePath.AbsoluteToRelativePath (entry.SourceFile.BasePath, entry.SourceFile.ExpandedPath);
				}
				if (entry.Children != null)
				{
					TargetTocEntries (entry.Children);
				}
			}
		}

		//
		//	Replacement of TocEntryCollection.SaveToIntermediateTocFile which only works for MAML
		//

		private void WriteTocEntries (TocEntryCollection entries, StreamWriter writer)
		{
			foreach (TocEntry v_entry in entries)
			{
				WriteTocEntry (v_entry, writer);
			}
		}

		private void WriteTocEntry (TocEntry entry, StreamWriter writer)
		{
			writer.Write ("<topic");

			if (!String.IsNullOrEmpty (entry.Title))
			{
				writer.Write (" id=\"");
				writer.Write (entry.Title);
				writer.Write ("\"");
			}

			if (!String.IsNullOrEmpty (entry.DestinationFile))
			{
				writer.Write (" file=\"");
				writer.Write (entry.DestinationFile);
				writer.Write ("\"");
			}
			else if (!String.IsNullOrEmpty (entry.Title))
			{
				writer.Write (" title=\"");
				writer.Write (entry.Title);
				writer.Write ("\"");
			}

			if ((entry.Children != null) && (entry.Children.Count > 0))
			{
				writer.WriteLine (">");
				WriteTocEntries (entry.Children, writer);
				writer.WriteLine ("</topic>");
			}
			else
			{
				writer.WriteLine ("/>");
			}
		}

		#endregion

		#region Debugging Methods
		//=====================================================================

#if DEBUG
		private void LogXmlFile (String filePath)
		{
			try
			{
				XmlReaderSettings v_readerSettings = new XmlReaderSettings ();
				XmlReader v_reader;
				XmlDocument v_document = new XmlDocument ();

				v_readerSettings.ValidationType = ValidationType.None;
				v_readerSettings.ValidationFlags = System.Xml.Schema.XmlSchemaValidationFlags.None;
				v_readerSettings.ConformanceLevel = ConformanceLevel.Document;
				v_readerSettings.IgnoreComments = true;
				v_readerSettings.IgnoreWhitespace = false;
				v_readerSettings.IgnoreProcessingInstructions = true;
				v_readerSettings.CloseInput = true;

				v_reader = XmlReader.Create (filePath, v_readerSettings);

				try
				{
					Debug.Print ("{0}", filePath);
					v_document.Load (v_reader);
					Debug.Print ("{0}", v_document.DocumentElement.OuterXml.Replace ("\n", "\\n").Replace ("\r", "\\r").Replace ("\t", " "));
				}
				catch (Exception exp)
				{
					System.Diagnostics.Debug.Print (exp.Message);
				}

				v_reader.Close ();
			}
			catch (Exception exp)
			{
				System.Diagnostics.Debug.Print (exp.Message);
			}
		}


		private void LogTocEntry (TocEntry entry, String indent)
		{
			Debug.Print ("{0}Entry '{1}' Source [{2}] Target [{3}] [Include {4} Sort {5} Parse {6} {7} {8} Default {9}]", indent, entry.Title, entry.SourceFile.Path, entry.DestinationFile, entry.IncludePage, entry.SortOrder, entry.HasProjectTags, entry.HasLinks, entry.HasCodeBlocks, entry.IsDefaultTopic);
			if (entry.Children != null)
			{
				LogTocEntries (entry.Children, indent + "  ");
			}
		}

		private void LogTocEntry (TocEntry entry)
		{
			LogTocEntry (entry, "  ");
		}

		private void LogTocEntries (TocEntryCollection entries, String indent)
		{
			foreach (TocEntry lEntry in entries)
			{
				LogTocEntry (lEntry, indent);
			}
		}

		private void LogTocEntries (TocEntryCollection entries)
		{
			LogTocEntries (entries, "  ");
		}
#endif

		#endregion

		#region IDisposable implementation
		//=====================================================================

		/// <inheritdoc/>
		~TOCFromSitemap ()
		{
			this.Dispose (false);
		}

		/// <inheritdoc/>
		public void Dispose ()
		{
			this.Dispose (true);
			GC.SuppressFinalize (this);
		}

		/// <inheritdoc/>
		protected virtual void Dispose (bool disposing)
		{
		}
		#endregion
	}
}
