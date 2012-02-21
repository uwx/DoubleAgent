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

		private ExecutionPointCollection m_executionPoints;
		private BuildProcess m_buildProcess;

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
				if (m_executionPoints == null)
				{
					m_executionPoints = new ExecutionPointCollection
                    {
                        new ExecutionPoint(BuildStep.TransformReflectionInfo, ExecutionBehaviors.Before),
                        new ExecutionPoint(BuildStep.GenerateIntermediateTableOfContents, ExecutionBehaviors.Before)
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
#if true
			if ((context.BuildStep == BuildStep.TransformReflectionInfo) && (context.Behavior == ExecutionBehaviors.Before))
			{
				m_buildProcess.ReportProgress ("{0} Version {1}\r\n{2}", this.Name, this.Version, this.Copyright);
				try
				{
					String v_reflectionFileName = m_buildProcess.ReflectionInfoFilename;
					XmlDocument v_reflectionDocument = new XmlDocument ();
					XmlNodeList v_namespaceNodes;
					XmlNode v_namespaceData;
					String v_namespaceId;
					String v_namespaceName;

					v_reflectionDocument.Load (v_reflectionFileName);
#if	DEBUG
					v_reflectionDocument.Save (Path.ChangeExtension (v_reflectionFileName, ".before"));
					v_reflectionDocument.Load (v_reflectionFileName);
					m_buildProcess.ReportProgress ("ReflectionInfo [{0}]", v_reflectionFileName);
#endif
					v_namespaceNodes = v_reflectionDocument.SelectNodes ("reflection/apis/api[starts-with(@id,'N:')]");

					foreach (XmlNode v_namespaceNode in v_namespaceNodes)
					{
						v_namespaceId = NodeAttrValue (v_namespaceNode, "id");
#if	DEBUG
						m_buildProcess.ReportProgress ("Namespace [{0}]", v_namespaceId);
#endif
						v_namespaceData = v_namespaceNode.SelectSingleNode ("apidata[@group='namespace' and not(@subgroup)]");
						if (v_namespaceData != null)
						{
							try
							{
								Dictionary<String, XmlNode> v_subgroupElements = new Dictionary<String, XmlNode> ();
								XmlNodeList v_elementNodes;

								v_namespaceName = NodeAttrValue (v_namespaceData, "name");
#if	DEBUG
								m_buildProcess.ReportProgress ("  Apidata [{0}] [{1}]", v_namespaceId, v_namespaceName);
#endif
								v_elementNodes = v_namespaceNode.SelectNodes ("elements/element");

								foreach (XmlNode v_elementNode in v_elementNodes)
								{
									XmlNode v_typeElement;
									XmlNode v_typeData = null;
									String v_subgroup = String.Empty;

									v_typeElement = v_reflectionDocument.SelectSingleNode (String.Format ("reflection/apis/api[@id='{0}']", NodeAttrValue (v_elementNode, "api")));
									if (v_typeElement != null)
									{
										v_typeData = v_typeElement.SelectSingleNode ("apidata");
										if (v_typeData != null)
										{
											v_subgroup = NodeAttrValue (v_typeData, "subgroup");
										}
									}
#if	DEBUG
									m_buildProcess.ReportProgress ("    Element [{0}] [{1}] [{2}]", NodeAttrValue (v_elementNode, "api"), (v_typeElement == null) ? "<not found>" : NodeAttrValue (v_typeElement, "id"), v_subgroup);
#endif
									if (!String.IsNullOrEmpty (v_subgroup))
									{
										if (!v_subgroupElements.ContainsKey (v_subgroup))
										{
											v_subgroupElements.Add (v_subgroup, v_reflectionDocument.CreateElement ("elements"));
										}
										v_subgroupElements[v_subgroup].AppendChild (v_elementNode.CloneNode (true));
									}
								}

#if	DEBUG
								foreach (KeyValuePair<String, XmlNode> v_subgroup in v_subgroupElements)
								{
									m_buildProcess.ReportProgress ("  Subgroup [{0}] Count [{1}]", v_subgroup.Key, v_subgroup.Value.ChildNodes.Count);
								}
#endif
								foreach (KeyValuePair<String, XmlNode> v_subgroup in v_subgroupElements)
								{
									String v_apiId = ApiSubgroupId (v_subgroup.Key);
									XmlElement v_apiNode;
									XmlElement v_apiDataNode;
									XmlElement v_containersNode;
									XmlElement v_containerNode;

									if (String.IsNullOrEmpty (v_apiId))
									{
										continue;
									}

									v_apiNode = v_reflectionDocument.CreateElement ("api");
									v_apiNode.SetAttribute ("id", String.Format ("{0}.{1}", v_apiId, v_namespaceId));

									v_apiDataNode = v_reflectionDocument.CreateElement ("apidata");
									v_apiDataNode.SetAttribute ("name", String.Format ("{0} {1}", v_namespaceData.Attributes.GetNamedItem ("name", String.Empty).Value, v_apiId));
									v_apiDataNode.Attributes.SetNamedItem (v_namespaceData.Attributes.GetNamedItem ("group", String.Empty).CloneNode (true));
									//v_apiDataNode.SetAttribute ("subgroup", v_subgroup.Key);

									v_containersNode = v_reflectionDocument.CreateElement ("containers");
									v_containerNode = v_reflectionDocument.CreateElement ("namespace");
									v_containerNode.SetAttribute ("api", v_namespaceId);
									v_containersNode.AppendChild (v_containerNode);
									//v_topicDataNode.SetAttribute ("group", "api");
									//v_topicDataNode.SetAttribute ("subgroup", v_subgroup.Key);

									//v_fileNameNode = v_reflectionDocument.CreateElement ("file");
									//v_fileNameNode.SetAttribute ("name", String.Format ("{0}.{1}", v_apiId, v_namespaceId).Replace (":", "_").Replace (".", "_"));

									v_apiNode.AppendChild (v_apiDataNode);
									v_apiNode.AppendChild (v_containersNode);
									//v_apiNode.AppendChild (v_fileNameNode);
									v_apiDataNode.AppendChild (v_subgroup.Value);
									v_namespaceNode.ParentNode.InsertAfter (v_apiNode, v_namespaceNode);
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

					v_reflectionDocument.Save (v_reflectionFileName);
#if	DEBUG
					m_buildProcess.ReportProgress ("Saved [{0}]", v_reflectionFileName);
					v_reflectionDocument.Save (Path.ChangeExtension (v_reflectionFileName, ".after"));
#endif
				}
				catch (Exception exp)
				{
#if	DEBUG
					m_buildProcess.ReportWarning (Name, exp.Message);
#endif
					System.Diagnostics.Debug.Print (exp.Message);
				}
			}
#endif
#if false
			if ((context.BuildStep == BuildStep.GenerateIntermediateTableOfContents) && (context.Behavior == ExecutionBehaviors.Before))
			{
				m_buildProcess.ReportProgress ("{0} Version {1}\r\n{2}", this.Name, this.Version, this.Copyright);
				UpdateGenerateIntermediateProject ();
			}
#endif
		}
		#endregion

		#region Helper Methods
		//=====================================================================

		private static String NodeAttrValue (XmlNode node, String attrName)
		{
			if ((node != null) && (node.Attributes != null))
			{
				XmlNode v_attr = node.Attributes.GetNamedItem (attrName);
				if (v_attr != null)
				{
					return v_attr.Value;
				}
			}
			return String.Empty;
		}

		private static String ApiSubgroupId (String subgroupName)
		{
			if (String.Compare (subgroupName, "class", StringComparison.InvariantCultureIgnoreCase) == 0)
			{
				return "Classes";
			}
			else if (String.Compare (subgroupName, "structure", StringComparison.InvariantCultureIgnoreCase) == 0)
			{
				return "Structures";
			}
			else if (String.Compare (subgroupName, "interface", StringComparison.InvariantCultureIgnoreCase) == 0)
			{
				return "Interfaces";
			}
			else if (String.Compare (subgroupName, "enumeration", StringComparison.InvariantCultureIgnoreCase) == 0)
			{
				return "Enumerations";
			}
			else if (String.Compare (subgroupName, "delegate", StringComparison.InvariantCultureIgnoreCase) == 0)
			{
				return "Delegates";
			}
			return String.Empty;
		}

		private void UpdateGenerateIntermediateProject ()
		{
			try
			{
				String v_projectPath = Path.Combine (m_buildProcess.WorkingFolder, "GenerateIntermediateTOC.proj");
				XmlDocument v_document = new XmlDocument ();
				XmlNodeList v_nodes;
				XmlAttribute v_attribute;
				String v_transform = Path.Combine (Path.GetDirectoryName (Assembly.GetExecutingAssembly ().Location), "CreateVSToc_Override.xsl");

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
						if ((v_attribute != null) && (v_attribute.Value.Contains ("CreateVSToc")))
						{
#if	DEBUG
							Debug.Print ("  Update    '{0}'", v_projectPath);
							Debug.Print ("    replace '{0}'", v_attribute.Value);
							Debug.Print ("    with    '{0}'", v_transform);
#endif
							m_buildProcess.ReportProgress ("{0}: Update    '{1}'", this.Name, v_projectPath);
							m_buildProcess.ReportProgress ("{0}:   replace '{1}'", this.Name, v_attribute.Value);
							m_buildProcess.ReportProgress ("{0}:   with    '{1}", this.Name, v_transform);

							v_attribute.Value = v_transform;
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
