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

namespace SandcastleBuilder.PlugIns.CinSoft
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

		private ExecutionPointCollection mExecutionPoints;
		private BuildProcess mBuildProcess;

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
				Assembly lAssembly = Assembly.GetExecutingAssembly ();
				FileVersionInfo lFileVersionInfo = FileVersionInfo.GetVersionInfo (lAssembly.Location);
				return new Version (lFileVersionInfo.ProductVersion);
			}
		}

		/// <inheritdoc/>
		public string Copyright
		{
			get
			{
				Assembly lAssembly = Assembly.GetExecutingAssembly ();
				AssemblyCopyrightAttribute lCopyrightAttribute = Attribute.GetCustomAttribute (lAssembly, typeof (AssemblyCopyrightAttribute)) as AssemblyCopyrightAttribute;
				return lCopyrightAttribute.Copyright;
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
				if (mExecutionPoints == null)
				{
					mExecutionPoints = new ExecutionPointCollection
                    {
                        new ExecutionPoint(BuildStep.BuildConceptualTopics, ExecutionBehaviors.Before)
                    };
				}
				return mExecutionPoints;
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
			mBuildProcess = buildProcess;
		}

		/// <inheritdoc/>
		public void Execute (ExecutionContext context)
		{
#if	DEBUG
			Debug.Print ("{0} {1}", context.Behavior, context.BuildStep);
#endif

			if ((context.BuildStep == BuildStep.BuildConceptualTopics) && (context.Behavior == ExecutionBehaviors.Before))
			{
				mBuildProcess.ReportProgress ("{0} Version {1}\r\n{2}", this.Name, this.Version, this.Copyright);

				if ((mBuildProcess.ConceptualContent == null) || (mBuildProcess.ConceptualContent.Topics == null) || (mBuildProcess.ConceptualContent.Topics.Count <= 0))
				{
					mBuildProcess.ReportWarning (Name, "No conceptual topics");
				}
				else
				{
					try
					{
						FolderPath lTopicFolder = new FolderPath (Path.Combine (mBuildProcess.WorkingFolder, "ddueXml"), mBuildProcess.CurrentProject);
						XmlDocument lReflectionDocument = new XmlDocument ();
						XPathNavigator lReflectionNavigator;

						lReflectionDocument.Load (mBuildProcess.ReflectionInfoFilename);
						lReflectionNavigator = lReflectionDocument.CreateNavigator ();

						foreach (TopicCollection lCollection in mBuildProcess.ConceptualContent.Topics)
						{
							ScanTopicCollection (lCollection, lTopicFolder, lReflectionNavigator);
						}
					}
					catch (Exception exp)
					{
#if	DEBUG
						mBuildProcess.ReportWarning (Name, exp.Message);
#endif
						System.Diagnostics.Debug.Print (exp.Message);
					}
				}
			}
		}
		#endregion

		#region Helper Methods
		//=====================================================================

		private bool ScanTopicCollection (TopicCollection Collection, FolderPath TopicFolder, XPathNavigator ReflectionInfo)
		{
			bool lChanged = false;

			foreach (Topic lItem in Collection)
			{
				if (ScanTopic (lItem, TopicFolder, ReflectionInfo))
				{
					lChanged = true;
				}
				if (lItem.Subtopics != null)
				{
					if (ScanTopicCollection (lItem.Subtopics, TopicFolder, ReflectionInfo))
					{
						lChanged = true;
					}
				}
			}
			return lChanged;
		}

		private bool ScanTopic (Topic ConceptualTopic, FolderPath TopicFolder, XPathNavigator ReflectionInfo)
		{
			bool lChanged = false;

			if (ConceptualTopic.TopicFile != null)
			{

				try
				{
					FilePath lTopicPath = new FilePath (Path.ChangeExtension (Path.Combine (TopicFolder, ConceptualTopic.Id), ".xml"), mBuildProcess.CurrentProject);
#if DEBUG
					mBuildProcess.ReportProgress ("Topic \"{0}\" File \"{1}\" Process \"{2}\"", ConceptualTopic.Title, ConceptualTopic.TopicFile.Name, lTopicPath);
#endif
					if (lTopicPath.Exists)
					{
						XmlDocument lConceptualDocument = new XmlDocument ();
						XmlNamespaceManager lNamespaceManager;
						XPathNodeIterator lMethodIterator;

						lConceptualDocument.Load (lTopicPath);
						lNamespaceManager = new XmlNamespaceManager (lConceptualDocument.NameTable);
						if (!lNamespaceManager.HasNamespace ("ddue"))
						{
							lNamespaceManager.AddNamespace ("ddue", "http://ddue.schemas.microsoft.com/authoring/2003/5");
						}

						// 
						//	Note - process Methods and Properties to ensure that indexer properties are covered.
						//
						foreach (XmlNode lMethodReference in lConceptualDocument.SelectNodes ("topic//ddue:codeEntityReference[(starts-with(.,'M:') or starts-with(.,'P:')) and not(contains(.,'('))]", lNamespaceManager))
						{
							lMethodIterator = ReflectionInfo.Select (String.Format ("reflection/apis/api[starts-with(@id,'{0}(')]", lMethodReference.InnerText));
							if ((lMethodIterator != null) && lMethodIterator.MoveNext ())
							{
								if (lMethodIterator.Count > 1)
								{
#if	DEBUG
									mBuildProcess.ReportWarning (Name, "Multiple API entries found for \"{0}\" in \"{1}\"", lMethodReference.InnerText, ConceptualTopic.TopicFile.Name);
									do
									{
										mBuildProcess.ReportWarning (Name, "  \"{0}\"", lMethodIterator.Current.GetAttribute ("id", String.Empty));
									}
									while (lMethodIterator.MoveNext ());
#endif
								}
								else
								{
									String lMethodSignature = lMethodIterator.Current.GetAttribute ("id", String.Empty);
#if DEBUG
									mBuildProcess.ReportProgress ("  Replace \"{0}\" with \"{1}\" in \"{2}\"", lMethodReference.InnerText, lMethodSignature, ConceptualTopic.TopicFile.Name);
#endif
									lMethodReference.InnerText = lMethodSignature;
									lChanged = true;
								}
							}
#if	DEBUG
							else if (!lMethodReference.InnerText.StartsWith ("P:"))
							{
								mBuildProcess.ReportWarning (Name, "No API entry found for \"{0}\" in \"{1}\"", lMethodReference.InnerText, ConceptualTopic.TopicFile.Name);
							}
#endif
						}

						if (lChanged)
						{
							lConceptualDocument.Save (lTopicPath);
						}
					}
				}
				catch (Exception exp)
				{
#if	DEBUG
					mBuildProcess.ReportWarning (Name, exp.Message);
#endif
					System.Diagnostics.Debug.Print (exp.Message);
				}
			}
			return lChanged;
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
