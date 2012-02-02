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

namespace SandcastleBuilder.PlugIns.CinSoft
{
	public class TOCFromSitemap : IPlugIn
	{
		#region Private data members
		//=====================================================================

		private ExecutionPointCollection mExecutionPoints;
		private BuildProcess mBuildProcess;

		#endregion

		#region IPlugIn Properties
		//=====================================================================

		public string Name
		{
			get { return "Table of Contents from Sitemap"; }
		}

		public Version Version
		{
			get
			{
				Assembly lAssembly = Assembly.GetExecutingAssembly ();
				FileVersionInfo lFileVersionInfo = FileVersionInfo.GetVersionInfo (lAssembly.Location);
				return new Version (lFileVersionInfo.ProductVersion);
			}
		}

		public string Copyright
		{
			get
			{
				Assembly lAssembly = Assembly.GetExecutingAssembly ();
				AssemblyCopyrightAttribute lCopyrightAttribute = Attribute.GetCustomAttribute (lAssembly, typeof (AssemblyCopyrightAttribute)) as AssemblyCopyrightAttribute;
				return lCopyrightAttribute.Copyright;
			}
		}

		public string Description
		{
			get
			{
				return "This plug-in uses a Sitemap to add bookmark links to the Table of Contents.\r\n" +
				"(Help1 and Help2 formats only)";
			}
		}

		public bool RunsInPartialBuild
		{
			get { return false; }
		}

		public ExecutionPointCollection ExecutionPoints
		{
			get
			{
				if (mExecutionPoints == null)
				{
					mExecutionPoints = new ExecutionPointCollection
                    {
                        new ExecutionPoint(BuildStep.GenerateHelpFormatTableOfContents, ExecutionBehaviors.Before),
                        new ExecutionPoint(BuildStep.CombiningIntermediateTocFiles, ExecutionBehaviors.Before)
                    };
				}
				return mExecutionPoints;
			}
		}

		#endregion

		#region IPlugIn Methods
		//=====================================================================

		public string ConfigurePlugIn (SandcastleProject project, string currentConfig)
		{
			MessageBox.Show ("This plug-in has no configurable settings", "Build Process Plug-In", MessageBoxButton.OK, MessageBoxImage.Information);
			return currentConfig;
		}

		public void Initialize (BuildProcess buildProcess, XPathNavigator configuration)
		{
			mBuildProcess = buildProcess;
		}

		public void Execute (ExecutionContext context)
		{
#if	DEBUG
			Debug.Print ("{0} {1}", context.Behavior.ToString (), context.BuildStep.ToString ());
#endif

			if ((context.BuildStep == BuildStep.GenerateHelpFormatTableOfContents) && (context.Behavior == ExecutionBehaviors.Before))
			{
				UpdateGenerateHelpFormatProject ();
			}
			else if ((context.BuildStep == BuildStep.CombiningIntermediateTocFiles) && (context.Behavior == ExecutionBehaviors.Before))
			{
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
				return mBuildProcess.CurrentProject.HasItems (BuildAction.SiteMap);
			}
		}

		FileItem SiteMapFile
		{
			get
			{
				foreach (FileItem lFileItem in mBuildProcess.CurrentProject.FileItems)
				{
					if (lFileItem.BuildAction == BuildAction.SiteMap)
					{
						return lFileItem;
					}
				}
				return null;
			}
		}

		String SiteMapPath
		{
			get
			{
				foreach (FileItem lFileItem in mBuildProcess.CurrentProject.FileItems)
				{
					if (lFileItem.BuildAction == BuildAction.SiteMap)
					{
						return lFileItem.FullPath;
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
				String lProjectPath = Path.Combine (mBuildProcess.WorkingFolder, "Generate2xTOC.proj");
				XmlDocument lDocument = new XmlDocument ();
				XmlNodeList lNodes;
				XmlAttribute lAttribute;
				String lTransform = Path.Combine (Path.GetDirectoryName (Assembly.GetExecutingAssembly ().Location), "TocToHxsContents_Override.xsl");

#if	DEBUG
				Debug.Print ("  Load      '{0}'", lProjectPath);
#endif
				lDocument.Load (lProjectPath);
				lNodes = lDocument.DocumentElement.SelectNodes ("//*[@Transformations!='']");
				if (lNodes != null)
				{
					foreach (XmlNode lNode in lNodes)
					{
						lAttribute = lNode.Attributes["Transformations"];
						if ((lAttribute != null) && (lAttribute.Value.Contains ("TocToHxSContents")))
						{
#if	DEBUG
							Debug.Print ("  Update    '{0}'", lProjectPath);
							Debug.Print ("    replace '{0}'", lAttribute.Value);
							Debug.Print ("    with    '{0}'", lTransform);
							Debug.Print ("    SiteMap '{0}'", SiteMapPath);
#endif
							mBuildProcess.ReportProgress ("{0}: Update    '{1}'", this.Name, lProjectPath);
							mBuildProcess.ReportProgress ("{0}:   replace '{1}'", this.Name, lAttribute.Value);
							mBuildProcess.ReportProgress ("{0}:   with    '{1}", this.Name, lTransform);
							mBuildProcess.ReportProgress ("{0}:   SiteMap '{1}", this.Name, SiteMapPath);

							lAttribute.Value = lTransform;

							lAttribute = lDocument.CreateAttribute ("Arguments");
							lAttribute.Value = "SiteMap=file://" + SiteMapPath;
							lNode.Attributes.Append (lAttribute);

							lDocument.Save (lProjectPath);
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
				FileItem lSiteMapFile = this.SiteMapFile;
				TocEntryCollection lSiteMapToc;

				if (lSiteMapFile != null)
				{
#if	DEBUG
					Debug.Print ("Load [{0}]", lSiteMapFile.FullPath);
#endif
					mBuildProcess.ReportProgress ("{0}: SitemapToToc '{1}'", this.Name, lSiteMapFile.FullPath);
					lSiteMapToc = new TocEntryCollection (lSiteMapFile);
					lSiteMapToc.Load ();

					TargetTocEntries (lSiteMapToc);
#if	DEBUG_NOT
					LogTocEntries (lSiteMapToc);
#endif
#if	DEBUG_NOT
					Debug.Print ("");
					Debug.Print ("{0}", lSiteMapToc.ToString (HelpFileFormat.MSHelp2));
					Debug.Print ("");
#endif
					try
					{
						String lTocFilePath = Path.Combine (mBuildProcess.WorkingFolder, "_ConceptualTOC_.xml");
						System.IO.StreamWriter lWriter = new System.IO.StreamWriter (lTocFilePath);

#if	DEBUG
						Debug.Print ("Save [{0}]", lTocFilePath);
#endif
						lWriter.WriteLine ("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
						lWriter.WriteLine ("<topics>");

						WriteTocEntries (lSiteMapToc, lWriter);

						lWriter.WriteLine ("</topics>");
						lWriter.Close ();
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

		void TargetTocEntries (TocEntryCollection Entries)
		{
			foreach (TocEntry lEntry in Entries)
			{
				TargetTocEntry (lEntry);
			}
		}

		void TargetTocEntry (TocEntry pEntry)
		{
			if (pEntry != null)
			{
				if ((!String.IsNullOrEmpty (pEntry.SourceFile)) && (String.IsNullOrEmpty (pEntry.DestinationFile)))
				{
					pEntry.DestinationFile = FilePath.AbsoluteToRelativePath (pEntry.SourceFile.BasePath, pEntry.SourceFile.ExpandedPath);
				}
				if (pEntry.Children != null)
				{
					TargetTocEntries (pEntry.Children);
				}
			}
		}

		//
		//	Replacement of TocEntryCollection.SaveToIntermediateTocFile which only works for MAML
		//

		private void WriteTocEntries (TocEntryCollection Entries, StreamWriter Writer)
		{
			foreach (TocEntry lEntry in Entries)
			{
				WriteTocEntry (lEntry, Writer);
			}
		}

		private void WriteTocEntry (TocEntry pEntry, StreamWriter Writer)
		{
			Writer.Write ("<topic");

			if (!String.IsNullOrEmpty (pEntry.Title))
			{
				Writer.Write (" id=\"");
				Writer.Write (pEntry.Title);
				Writer.Write ("\"");
			}

			if (!String.IsNullOrEmpty (pEntry.DestinationFile))
			{
				Writer.Write (" file=\"");
				Writer.Write (pEntry.DestinationFile);
				Writer.Write ("\"");
			}
			else if (!String.IsNullOrEmpty (pEntry.Title))
			{
				Writer.Write (" title=\"");
				Writer.Write (pEntry.Title);
				Writer.Write ("\"");
			}

			if ((pEntry.Children != null) && (pEntry.Children.Count > 0))
			{
				Writer.WriteLine (">");
				WriteTocEntries (pEntry.Children, Writer);
				Writer.WriteLine ("</topic>");
			}
			else
			{
				Writer.WriteLine ("/>");
			}
		}

		#endregion

		#region Debugging Methods
		//=====================================================================

#if DEBUG
		private void LogXmlFile (String FilePath)
		{
			try
			{
				XmlReaderSettings lReaderSettings = new XmlReaderSettings ();
				XmlReader lReader;
				XmlDocument lDocument = new XmlDocument ();

				lReaderSettings.ValidationType = ValidationType.None;
				lReaderSettings.ValidationFlags = System.Xml.Schema.XmlSchemaValidationFlags.None;
				lReaderSettings.ConformanceLevel = ConformanceLevel.Document;
				lReaderSettings.IgnoreComments = true;
				lReaderSettings.IgnoreWhitespace = false;
				lReaderSettings.IgnoreProcessingInstructions = true;
				lReaderSettings.CloseInput = true;

				lReader = XmlReader.Create (FilePath, lReaderSettings);

				try
				{
					Debug.Print ("{0}", FilePath);
					lDocument.Load (lReader);
					Debug.Print ("{0}", lDocument.DocumentElement.OuterXml.Replace ("\n", "\\n").Replace ("\r", "\\r").Replace ("\t", " "));
				}
				catch (Exception exp)
				{
					System.Diagnostics.Debug.Print (exp.Message);
				}

				lReader.Close ();
			}
			catch (Exception exp)
			{
				System.Diagnostics.Debug.Print (exp.Message);
			}
		}


		private void LogTocEntry (TocEntry Entry, String Indent)
		{
			Debug.Print ("{0}Entry '{1}' Source [{2}] Target [{3}] [Include {4} Sort {5} Parse {6} {7} {8} Default {9}]", Indent, Entry.Title, Entry.SourceFile.Path, Entry.DestinationFile, Entry.IncludePage, Entry.SortOrder, Entry.HasProjectTags, Entry.HasLinks, Entry.HasCodeBlocks, Entry.IsDefaultTopic);
			if (Entry.Children != null)
			{
				LogTocEntries (Entry.Children, Indent + "  ");
			}
		}

		private void LogTocEntry (TocEntry Entry)
		{
			LogTocEntry (Entry, "  ");
		}

		private void LogTocEntries (TocEntryCollection Entries, String Indent)
		{
			foreach (TocEntry lEntry in Entries)
			{
				LogTocEntry (lEntry, Indent);
			}
		}

		private void LogTocEntries (TocEntryCollection Entries)
		{
			LogTocEntries (Entries, "  ");
		}
#endif

		#endregion

		#region IDisposable implementation
		//=====================================================================

		~TOCFromSitemap ()
		{
			this.Dispose (false);
		}

		public void Dispose ()
		{
			this.Dispose (true);
			GC.SuppressFinalize (this);
		}

		protected virtual void Dispose (bool disposing)
		{
		}
		#endregion
	}
}
