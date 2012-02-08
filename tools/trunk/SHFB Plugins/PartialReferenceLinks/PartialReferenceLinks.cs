using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Windows;
using System.Xml;
using System.Xml.XPath;

using SandcastleBuilder.Utils;
using SandcastleBuilder.Utils.BuildEngine;
using SandcastleBuilder.Utils.PlugIn;
using SandcastleBuilder.Utils.ConceptualContent;

namespace SandcastleBuilder.PlugIns
{
	/// <summary>
	/// This plugin scans MAML Conceptual content for incomplete API references and completes them.
	/// </summary>
	/// <remarks>
	/// Using the MAML &lt;codeEntityReferenct&gt; tag for a method reference requires the complete method
	/// signature.  This plugin allows methods to be referenced by name only. It uses the generated
	/// reflection information to update incomplete &lt;codeEntityReferenct&gt; method references with the
	/// full method signature.
	/// <para>
	/// Note that parameter references are also processed so that <i>indexer</i> parameters can be treated the same way.
	/// </para>
	/// </remarks>
	public class PartialReferenceLinks : IPlugIn
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
			get { return "Resolve Partial Reference Links"; }
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
				return "This plug-in scans the conceptual topics for partial reference links and completes them." +
					"This allows you to reference methods by name only and use this plug-in to complete their signatures.\n" +
					"This is essentially the same as applying 'autoUpgrade' to all method references, but it works for" +
					"methods with no overloads.";
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
                        new ExecutionPoint(BuildStep.BuildConceptualTopics, ExecutionBehaviors.Before)
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
			Debug.Print ("{0} {1}", context.Behavior, context.BuildStep);
#endif

			if ((context.BuildStep == BuildStep.BuildConceptualTopics) && (context.Behavior == ExecutionBehaviors.Before))
			{
				m_buildProcess.ReportProgress ("{0} Version {1}\r\n{2}", this.Name, this.Version, this.Copyright);

				if ((m_buildProcess.ConceptualContent == null) || (m_buildProcess.ConceptualContent.Topics == null) || (m_buildProcess.ConceptualContent.Topics.Count <= 0))
				{
					m_buildProcess.ReportWarning (Name, "No conceptual topics");
				}
				else
				{
					try
					{
						FolderPath v_topicFolder = new FolderPath (Path.Combine (m_buildProcess.WorkingFolder, "ddueXml"), m_buildProcess.CurrentProject);
						XmlDocument v_reflectionDocument = new XmlDocument ();
						XPathNavigator v_reflectionNavigator;

						v_reflectionDocument.Load (m_buildProcess.ReflectionInfoFilename);
						v_reflectionNavigator = v_reflectionDocument.CreateNavigator ();

						foreach (TopicCollection v_collection in m_buildProcess.ConceptualContent.Topics)
						{
							ScanTopicCollection (v_collection, v_topicFolder, v_reflectionNavigator);
						}
					}
					catch (Exception exp)
					{
#if	DEBUG
						m_buildProcess.ReportWarning (Name, exp.Message);
#endif
						System.Diagnostics.Debug.Print (exp.Message);
					}
				}
			}
		}
		#endregion

		#region Helper Methods
		//=====================================================================

		private bool ScanTopicCollection (TopicCollection collection, FolderPath topicFolder, XPathNavigator reflectionInfo)
		{
			bool v_changed = false;

			foreach (Topic v_iItem in collection)
			{
				if (ScanTopic (v_iItem, topicFolder, reflectionInfo))
				{
					v_changed = true;
				}
				if (v_iItem.Subtopics != null)
				{
					if (ScanTopicCollection (v_iItem.Subtopics, topicFolder, reflectionInfo))
					{
						v_changed = true;
					}
				}
			}
			return v_changed;
		}

		private bool ScanTopic (Topic conceptualTopic, FolderPath topicFolder, XPathNavigator reflectionInfo)
		{
			bool v_changed = false;

			if (conceptualTopic.TopicFile != null)
			{

				try
				{
					FilePath v_topicPath = new FilePath (Path.ChangeExtension (Path.Combine (topicFolder, conceptualTopic.Id), ".xml"), m_buildProcess.CurrentProject);
#if DEBUG
					m_buildProcess.ReportProgress ("Topic \"{0}\" File \"{1}\" Process \"{2}\"", conceptualTopic.Title, conceptualTopic.TopicFile.Name, v_topicPath);
#endif
					if (v_topicPath.Exists)
					{
						XmlDocument v_conceptualDocument = new XmlDocument ();
						XmlNamespaceManager v_namespaceManager;
						XPathNodeIterator v_methodIterator;

						v_conceptualDocument.Load (v_topicPath);
						v_namespaceManager = new XmlNamespaceManager (v_conceptualDocument.NameTable);
						if (!v_namespaceManager.HasNamespace ("ddue"))
						{
							v_namespaceManager.AddNamespace ("ddue", "http://ddue.schemas.microsoft.com/authoring/2003/5");
						}

						// 
						//	Note - process Methods and Properties to ensure that indexer properties are covered.
						//
						foreach (XmlNode v_methodReference in v_conceptualDocument.SelectNodes ("topic//ddue:codeEntityReference[(starts-with(.,'M:') or starts-with(.,'P:')) and not(contains(.,'('))]", v_namespaceManager))
						{
							v_methodIterator = reflectionInfo.Select (String.Format ("reflection/apis/api[starts-with(@id,'{0}(')]", v_methodReference.InnerText));
							if ((v_methodIterator != null) && v_methodIterator.MoveNext ())
							{
								if (v_methodIterator.Count > 1)
								{
#if	DEBUG
									m_buildProcess.ReportWarning (Name, "Multiple API entries found for \"{0}\" in \"{1}\"", v_methodReference.InnerText, conceptualTopic.TopicFile.Name);
									do
									{
										m_buildProcess.ReportWarning (Name, "  \"{0}\"", v_methodIterator.Current.GetAttribute ("id", String.Empty));
									}
									while (v_methodIterator.MoveNext ());
#endif
								}
								else
								{
									String v_methodSignature = v_methodIterator.Current.GetAttribute ("id", String.Empty);
#if DEBUG
									m_buildProcess.ReportProgress ("  Replace \"{0}\" with \"{1}\" in \"{2}\"", v_methodReference.InnerText, v_methodSignature, conceptualTopic.TopicFile.Name);
#endif
									v_methodReference.InnerText = v_methodSignature;
									v_changed = true;
								}
							}
#if	DEBUG
							else if (!v_methodReference.InnerText.StartsWith ("P:"))
							{
								m_buildProcess.ReportWarning (Name, "No API entry found for \"{0}\" in \"{1}\"", v_methodReference.InnerText, conceptualTopic.TopicFile.Name);
							}
#endif
						}

						if (v_changed)
						{
							v_conceptualDocument.Save (v_topicPath);
						}
					}
				}
				catch (Exception exp)
				{
#if	DEBUG
					m_buildProcess.ReportWarning (Name, exp.Message);
#endif
					System.Diagnostics.Debug.Print (exp.Message);
				}
			}
			return v_changed;
		}

		#endregion

		#region IDisposable implementation
		//=====================================================================

		/// <inheritdoc/>
		~PartialReferenceLinks ()
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
