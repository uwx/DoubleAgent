using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Windows;
using System.Xml;
using System.Xml.XPath;

using SandcastleBuilder.Utils;
using SandcastleBuilder.Utils.BuildEngine;
using SandcastleBuilder.Utils.PlugIn;

namespace SandcastleBuilder.PlugIns
{
	/// <summary>
	/// This plugin adds an extra level of hierachy to the TOC of each namespace to group types within the namespace.
	/// The groupings are the same as those used in the generated "Members" topic - Classes, Interfaces, Delegates, Enumerations.
	/// </summary>
	/// <remarks>
	/// For compatibility with the MS Help Viewer, it generates a topic (like the "Members" topic) for each added TOC entry.
	/// </remarks>
	public class TOCTypeGrouping : IPlugIn
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
			get { return "Table of Contents Type Grouping"; }
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
				return "This plug-in categorizes types by 'Class', 'Interface', 'Delegate', 'Enumeration', etc." +
					"It adds an extra level of category entries for each namespace in the Table of Contents.";
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
                        new ExecutionPoint(BuildStep.TransformReflectionInfo, ExecutionBehaviors.After),
                        new ExecutionPoint(BuildStep.GenerateIntermediateTableOfContents, ExecutionBehaviors.Before)
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
#if false
			if ((context.BuildStep == BuildStep.TransformReflectionInfo) && (context.Behavior == ExecutionBehaviors.After))
			{
				mBuildProcess.ReportProgress ("{0} Version {1}\r\n{2}", this.Name, this.Version, this.Copyright);
				try
				{
					String lReflectionFileName = Path.ChangeExtension(mBuildProcess.ReflectionInfoFilename, ".xml");
					XmlDocument lReflectionDocument = new XmlDocument ();
					XmlNodeList lNamespaceNodes;
					XmlNode lNamespaceData;
					String lNamespaceId;
					String lNamespaceName;

					mBuildProcess.ReportProgress ("ReflectionInfo [{0}]", lReflectionFileName);
					lReflectionDocument.Load (lReflectionFileName);
					lNamespaceNodes = lReflectionDocument.SelectNodes ("reflection/apis/api[starts-with(@id,'N:')]");

					foreach (XmlNode lNamespaceNode in lNamespaceNodes)
					{
						lNamespaceId = NodeAttrValue (lNamespaceNode, "id");
						mBuildProcess.ReportProgress ("Namespace [{0}]", lNamespaceId);

						lNamespaceData = lNamespaceNode.SelectSingleNode ("apidata[@group='namespace' and not(@subgroup)]");
						if (lNamespaceData != null)
						{
							try
							{
								Dictionary<String, XmlNode> lSubgroupElements = new Dictionary<String, XmlNode> ();
								XmlNodeList lElementNodes;

								lNamespaceName = NodeAttrValue (lNamespaceData, "name");
								mBuildProcess.ReportProgress ("  Apidata [{0}] [{1}]", lNamespaceId, lNamespaceName);
								lElementNodes = lNamespaceNode.SelectNodes ("elements/element");

								foreach (XmlNode lElementNode in lElementNodes)
								{
									XmlNode lTypeElement;
									XmlNode lTypeData = null;
									String lSubgroup = String.Empty;

									lTypeElement = lReflectionDocument.SelectSingleNode (String.Format ("reflection/apis/api[@id='{0}']", NodeAttrValue (lElementNode, "api")));
									if (lTypeElement != null)
									{
										lTypeData = lTypeElement.SelectSingleNode ("apidata");
										if (lTypeData != null)
										{
											lSubgroup = NodeAttrValue (lTypeData, "subgroup");
										}
									}
									mBuildProcess.ReportProgress ("    Element [{0}] [{1}] [{2}]", NodeAttrValue (lElementNode, "api"), (lTypeElement == null) ? "<not found>" : NodeAttrValue (lTypeElement, "id"), lSubgroup);

									if (!String.IsNullOrEmpty (lSubgroup))
									{
										if (!lSubgroupElements.ContainsKey (lSubgroup))
										{
											lSubgroupElements.Add (lSubgroup, lReflectionDocument.CreateElement ("elements"));
										}
										lSubgroupElements[lSubgroup].AppendChild (lElementNode.CloneNode (true));
									}
								}

								foreach (KeyValuePair<String, XmlNode> lSubgroup in lSubgroupElements)
								{
									mBuildProcess.ReportProgress ("  Subgroup [{0}] Count [{1}]", lSubgroup.Key, lSubgroup.Value.ChildNodes.Count);
								}
								foreach (KeyValuePair<String, XmlNode> lSubgroup in lSubgroupElements)
								{
									String lApiId = ApiSubgroupId (lSubgroup.Key);
									XmlElement lApiNode;
									XmlElement lApiDataNode;
									XmlElement lTopicDataNode;
									XmlElement lFileNameNode;
									XmlAttribute lAttribute;

									if (String.IsNullOrEmpty (lApiId))
									{
										continue;
									}

									lApiNode = lReflectionDocument.CreateElement ("api");
									lAttribute = lReflectionDocument.CreateAttribute ("id");
									lAttribute.Value = String.Format ("{0}.{1}", lApiId, lNamespaceId);
									lApiNode.Attributes.SetNamedItem (lAttribute);

									lApiDataNode = lReflectionDocument.CreateElement ("apidata");
									lApiDataNode.Attributes.SetNamedItem (lNamespaceData.Attributes.GetNamedItem ("name", String.Empty).CloneNode (true));
									lApiDataNode.Attributes.SetNamedItem (lNamespaceData.Attributes.GetNamedItem ("group", String.Empty).CloneNode (true));

									lAttribute = lReflectionDocument.CreateAttribute ("subgroup");
									//lAttribute.Value = "type";
									//lApiDataNode.Attributes.SetNamedItem (lAttribute);

									//lAttribute = lReflectionDocument.CreateAttribute ("subsubgroup");
									lAttribute.Value = lSubgroup.Key;
									lApiDataNode.Attributes.SetNamedItem (lAttribute);

									lTopicDataNode = lReflectionDocument.CreateElement ("topicdata");
									lTopicDataNode.Attributes.SetNamedItem (lNamespaceData.Attributes.GetNamedItem ("name", String.Empty).CloneNode (true));

									lAttribute = lReflectionDocument.CreateAttribute ("group");
									lAttribute.Value = "list";
									lTopicDataNode.Attributes.SetNamedItem (lAttribute);

									lAttribute = lReflectionDocument.CreateAttribute ("subgroup");
									lAttribute.Value = lSubgroup.Key;
									lTopicDataNode.Attributes.SetNamedItem (lAttribute);

									lFileNameNode = lReflectionDocument.CreateElement ("file");
									lAttribute = lReflectionDocument.CreateAttribute ("name");
									lAttribute.Value = String.Format ("{0}.{1}", lApiId, lNamespaceId).Replace (":", "_").Replace (".", "_");
									lFileNameNode.Attributes.SetNamedItem (lAttribute);

									lApiNode.AppendChild (lTopicDataNode);
									lApiNode.AppendChild (lApiDataNode);
									lApiNode.AppendChild (lFileNameNode);
									lApiDataNode.AppendChild (lSubgroup.Value);
									lNamespaceNode.ParentNode.InsertAfter (lApiNode, lNamespaceNode);
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

					lReflectionDocument.Save (lReflectionFileName);
				}
				catch (Exception exp)
				{
#if	DEBUG
					mBuildProcess.ReportWarning (Name, exp.Message);
#endif
					System.Diagnostics.Debug.Print (exp.Message);
				}
			}
#endif
#if true
			if ((context.BuildStep == BuildStep.GenerateIntermediateTableOfContents) && (context.Behavior == ExecutionBehaviors.Before))
			{
				mBuildProcess.ReportProgress ("{0} Version {1}\r\n{2}", this.Name, this.Version, this.Copyright);
				UpdateGenerateIntermediateProject ();
			}
#endif
		}
		#endregion

		#region Helper Methods
		//=====================================================================

		private static String NodeAttrValue (XmlNode Node, String AttrName)
		{
			if ((Node != null) && (Node.Attributes != null))
			{
				XmlNode lAttr = Node.Attributes.GetNamedItem (AttrName);
				if (lAttr != null)
				{
					return lAttr.Value;
				}
			}
			return String.Empty;
		}

		private static String ApiSubgroupId (String SubgroupName)
		{
			if (String.Compare (SubgroupName, "class", StringComparison.InvariantCultureIgnoreCase) == 0)
			{
				return "Classes";
			}
			else if (String.Compare (SubgroupName, "structure", StringComparison.InvariantCultureIgnoreCase) == 0)
			{
				return "Structures";
			}
			else if (String.Compare (SubgroupName, "interface", StringComparison.InvariantCultureIgnoreCase) == 0)
			{
				return "Interfaces";
			}
			else if (String.Compare (SubgroupName, "enumeration", StringComparison.InvariantCultureIgnoreCase) == 0)
			{
				return "Enumerations";
			}
			else if (String.Compare (SubgroupName, "delegate", StringComparison.InvariantCultureIgnoreCase) == 0)
			{
				return "Delegates";
			}
			return String.Empty;
		}

		private void UpdateGenerateIntermediateProject ()
		{
			try
			{
				String lProjectPath = Path.Combine (mBuildProcess.WorkingFolder, "GenerateIntermediateTOC.proj");
				XmlDocument lDocument = new XmlDocument ();
				XmlNodeList lNodes;
				XmlAttribute lAttribute;
				String lTransform = Path.Combine (Path.GetDirectoryName (Assembly.GetExecutingAssembly ().Location), "CreateVSToc_Override.xsl");

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
						if ((lAttribute != null) && (lAttribute.Value.Contains ("CreateVSToc")))
						{
#if	DEBUG
							Debug.Print ("  Update    '{0}'", lProjectPath);
							Debug.Print ("    replace '{0}'", lAttribute.Value);
							Debug.Print ("    with    '{0}'", lTransform);
#endif
							mBuildProcess.ReportProgress ("{0}: Update    '{1}'", this.Name, lProjectPath);
							mBuildProcess.ReportProgress ("{0}:   replace '{1}'", this.Name, lAttribute.Value);
							mBuildProcess.ReportProgress ("{0}:   with    '{1}", this.Name, lTransform);

							lAttribute.Value = lTransform;
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

		#endregion

		#region IDisposable implementation
		//=====================================================================

		/// <inheritdoc/>
		~TOCTypeGrouping ()
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
