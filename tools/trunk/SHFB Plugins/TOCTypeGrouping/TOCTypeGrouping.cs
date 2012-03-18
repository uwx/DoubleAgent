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
                        new ExecutionPoint(BuildStep.TransformReflectionInfo, ExecutionBehaviors.After),
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
				AddReflectionTypeGrouping ();
				UpdateTransformManifestProject ();
			}
			else if ((context.BuildStep == BuildStep.TransformReflectionInfo) && (context.Behavior == ExecutionBehaviors.After))
			{
				m_buildProcess.ReportProgress ("{0} Version {1}\r\n{2}", this.Name, this.Version, this.Copyright);
				AddTypeGroupTitles ();
			}
#endif
#if true
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

		private void AddReflectionTypeGrouping ()
		{
			try
			{
				String v_reflectionFileName = m_buildProcess.ReflectionInfoFilename;
				XmlDocument v_reflectionDocument = new XmlDocument ();
				XmlNodeList v_namespaceNodes;
				XmlNode v_namespaceData;
				String v_namespaceId;
				String v_namespaceName;

				v_reflectionDocument.Load (v_reflectionFileName);
#if	DEBUG_NOT
				v_reflectionDocument.Save (Path.ChangeExtension (v_reflectionFileName, ".before"));
				v_reflectionDocument.Load (v_reflectionFileName);
				m_buildProcess.ReportProgress ("ReflectionInfo [{0}]", v_reflectionFileName);
#endif
				v_namespaceNodes = v_reflectionDocument.SelectNodes ("reflection/apis/api[starts-with(@id,'N:')]");

				foreach (XmlNode v_namespaceNode in v_namespaceNodes)
				{
					v_namespaceId = NodeAttrValue (v_namespaceNode, "id");
#if	DEBUG
					m_buildProcess.ReportProgress ("  Namespace [{0}]", v_namespaceId);
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
							m_buildProcess.ReportProgress ("    Apidata [{0}] [{1}]", v_namespaceId, v_namespaceName);
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
								m_buildProcess.ReportProgress ("      Element [{0}] [{1}] [{2}]", NodeAttrValue (v_elementNode, "api"), (v_typeElement == null) ? "<not found>" : NodeAttrValue (v_typeElement, "id"), v_subgroup);
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
								m_buildProcess.ReportProgress ("    Subgroup [{0}] Count [{1}]", v_subgroup.Key, v_subgroup.Value.ChildNodes.Count);
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
								v_apiDataNode.Attributes.SetNamedItem (v_namespaceData.Attributes.GetNamedItem ("name", String.Empty).CloneNode (true));
								v_apiDataNode.Attributes.SetNamedItem (v_namespaceData.Attributes.GetNamedItem ("group", String.Empty).CloneNode (true));
								v_apiDataNode.SetAttribute ("subgroup", String.Format ("{0}Types", v_subgroup.Key));

								v_containersNode = v_reflectionDocument.CreateElement ("containers");
								v_containerNode = v_reflectionDocument.CreateElement ("namespace");
								v_containerNode.SetAttribute ("api", v_namespaceId);
								v_containersNode.AppendChild (v_containerNode);

								v_apiNode.AppendChild (v_apiDataNode);
								v_apiNode.AppendChild (v_containersNode);
								v_apiNode.AppendChild (v_subgroup.Value);
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
#if	DEBUG_NOT
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

		private void AddTypeGroupTitles ()
		{
			try
			{
				String v_referenceContentFileName = Path.Combine (Path.Combine (m_buildProcess.PresentationStyleFolder, "content"), "reference_content.xml");
				String v_sharedContentFileName = Path.Combine (m_buildProcess.WorkingFolder, "SharedBuilderContent.xml");
				XmlDocument v_referenceContentDocument = new XmlDocument ();
				XmlDocument v_sharedContentDocument = new XmlDocument ();
				String[] v_groups = { "class", "structure", "interface", "delegate", "enumeration" };
				XmlElement v_titleNode;
				XmlNode v_includeNode;

				v_referenceContentDocument.Load (v_referenceContentFileName);
				v_sharedContentDocument.Load (v_sharedContentFileName);

				foreach (String v_group in v_groups)
				{
					v_includeNode = v_referenceContentDocument.SelectSingleNode (String.Format ("//item[@id='{0}TypesFilterLabel']", v_group));
					if (v_includeNode != null)
					{
						v_titleNode = v_sharedContentDocument.CreateElement ("item");
						v_titleNode.SetAttribute ("id", String.Format ("{0}TypesTopicTitle", v_group));
						v_titleNode.InnerText = "{0} " + v_includeNode.InnerText;
						v_sharedContentDocument.DocumentElement.AppendChild (v_titleNode);

						v_titleNode = v_sharedContentDocument.CreateElement ("item");
						v_titleNode.SetAttribute ("id", String.Format ("{0}TypesIndexEntry", v_group));
						v_titleNode.InnerText = "{0} " + v_includeNode.InnerText.ToLower ();
						v_sharedContentDocument.DocumentElement.AppendChild (v_titleNode);
					}
				}

				v_sharedContentDocument.Save (v_sharedContentFileName);
			}
			catch (Exception exp)
			{
#if	DEBUG
				m_buildProcess.ReportWarning (Name, exp.Message);
#endif
				System.Diagnostics.Debug.Print (exp.Message);
			}
		}

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

		//=====================================================================

		private void UpdateTransformManifestProject ()
		{
			UpdateProjectTransform ("TransformManifest.proj", "ApplyVSDocModel.xsl", "ApplyVSDocModel_Override.xsl");
			UpdateProjectTransform ("TransformManifest.proj", "ApplyPrototypeDocModel.xsl", "ApplyPrototypeDocModel_Override.xsl");
		}

		private void UpdateGenerateIntermediateProject ()
		{
			UpdateProjectTransform ("GenerateIntermediateTOC.proj", "CreateVSToc.xsl", "CreateVSToc_Override.xsl");
		}

		private void UpdateProjectTransform (String projectName, String sourceTransform, String targetTransform)
		{
			try
			{
				String v_projectPath = Path.Combine (m_buildProcess.WorkingFolder, projectName);
				String v_sourceTransform = sourceTransform.ToLower ();
				String v_targetTransform = Path.Combine (Path.GetDirectoryName (Assembly.GetExecutingAssembly ().Location), targetTransform);
				XmlDocument v_document = new XmlDocument ();
				XmlNodeList v_nodes;
				XmlAttribute v_attribute;
				bool v_projectChanged = false;
				bool v_transformCopied = false;

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
						if ((v_attribute != null) && (v_attribute.Value.ToLower ().Contains (v_sourceTransform)))
						{
							String[] v_sourceValues = v_attribute.Value.Split (';');
							List<String> v_targetValues = new List<String> ();
							String v_targetValue;

							foreach (String v_sourceValue in v_sourceValues)
							{
								if (v_sourceValue.ToLower ().Contains (v_sourceTransform))
								{
									v_targetValues.Add (v_targetTransform);
								}
								else
								{
									v_targetValues.Add (v_sourceValue);
								}
							}
							v_targetValue = String.Join (";", v_targetValues.ToArray ());
#if	DEBUG
							Debug.Print ("  Update    '{0}'", v_projectPath);
							Debug.Print ("    replace '{0}'", v_attribute.Value);
							Debug.Print ("    with    '{0}'", v_targetValue);
#endif
							m_buildProcess.ReportProgress ("{0}: Update    '{1}'", this.Name, v_projectPath);
							m_buildProcess.ReportProgress ("{0}:   replace '{1}'", this.Name, v_attribute.Value);
							m_buildProcess.ReportProgress ("{0}:   with    '{1}'", this.Name, v_targetValue);

							if (!v_transformCopied)
							{
								CopySourceTransform (v_attribute, sourceTransform);
								v_transformCopied = true;
							}
							v_attribute.Value = v_targetValue;
							v_projectChanged = true;
						}
					}
				}
				if (v_projectChanged)
				{
					v_document.Save (v_projectPath);
				}
			}
			catch (Exception exp)
			{
				System.Diagnostics.Debug.Print (exp.Message);
			}
		}

		private void CopySourceTransform (XmlAttribute transformAttr, String sourceTransform)
		{
			try
			{
				String[] v_sourceValues = transformAttr.Value.Split (';');
				String v_sourcePath = transformAttr.Value;
				String v_targetPath = Path.Combine (Path.GetDirectoryName (Assembly.GetExecutingAssembly ().Location), sourceTransform);

				if (v_sourceValues.Length > 1)
				{
					String v_sourceTransform = sourceTransform.ToLower ();
					foreach (String v_sourceValue in v_sourceValues)
					{
						if (v_sourceValue.ToLower ().Contains (v_sourceTransform))
						{
							v_sourcePath = v_sourceValue;
							break;
						}
					}
				}

				if (!Path.IsPathRooted (v_sourcePath))
				{
					if (transformAttr.OwnerElement.Attributes["SandcastlePath"] != null)
					{
						v_sourcePath = Path.Combine (m_buildProcess.SandcastleFolder, v_sourcePath);
					}
				}

				try
				{
					if (File.Exists (v_sourcePath))
					{
						File.Delete (v_targetPath);
						File.Copy (v_sourcePath, v_targetPath);
						m_buildProcess.ReportProgress ("{0}:   copied  '{1}'", this.Name, v_sourcePath);
						m_buildProcess.ReportProgress ("{0}:   to      '{1}'", this.Name, v_targetPath);
					}
					else
					{
						m_buildProcess.ReportProgress ("{0}:   copy    '{1}'", this.Name, v_sourcePath);
						m_buildProcess.ReportProgress ("{0}:   to      '{1}'", this.Name, v_targetPath);
						m_buildProcess.ReportWarning (this.Name, "'{0}' not found", v_sourcePath);
					}
				}
				catch (Exception exp)
				{
					m_buildProcess.ReportProgress ("{0}:   copy    '{1}'", this.Name, v_sourcePath);
					m_buildProcess.ReportProgress ("{0}:   to      '{1}'", this.Name, v_targetPath);
					m_buildProcess.ReportWarning (this.Name, "copy failed '{0}'", exp.Message);
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
