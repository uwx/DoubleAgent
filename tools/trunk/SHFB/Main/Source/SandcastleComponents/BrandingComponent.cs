//=============================================================================
// System  : Sandcastle Help File Builder Components
// File    : BrandingComponent.cs
// Author  : Don Fehr (Don.Fehr@videotron.ca)
// Updated : 
// Compiler: Microsoft Visual C#
//
// This file manages the MSDN and/or custom branding of MSHC packages.
// It also allows the branding to be applied as part of the build process
// so that Help1 and Web targets are supported.
//
// This code is published under the Microsoft Public License (Ms-PL).  A copy
// of the license should be distributed with the code.  It can also be found
// at the project website: http://SHFB.CodePlex.com.   This notice, the
// author's name, and all copyright notices must remain intact in all
// applications, documentation, and source files.
//
// Version     Date     Who  Comments
// ============================================================================
// unknown  02/06/2012  DBF  Created the code
//=============================================================================

using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Xml;
using System.Xml.XPath;
using System.Xml.Xsl;

using Microsoft.Ddue.Tools;
using SandcastleBuilder.MicrosoftHelpViewer;

namespace SandcastleBuilder.Components
{
	class BrandingComponent : BuildComponent
	{
		#region Private data members
		//=====================================================================

		private const string s_configSelfBranded = "self-branded";
		private const string s_configLocale = "locale";
		private const string s_configBrandingContent = "branding-content";
		private const string s_configBrandingPackage = "branding-package";
		private const string s_configBasePackage = "base-package";
		private const string s_configHelpOutput = "help-output";
		private const string s_configCatalogProductId = "catalog-prodict-id";
		private const string s_configCatalogVersion = "catalog-version";
		private const string s_configVendorName = "vendor-name";

		private const string s_defaultLocale = "en-us";
		private const string s_defaultBrandingPackage = "dev10";
		private const string s_defaultHelpOutput = "MSHelpViewer";
		private const string s_defaultCatalogProductId = "VS";
		private const string s_defaultCatalogVersion = "100";
		private const string s_packageExtension = ".mshc";

		private bool m_selfBranded = true;
		private string m_locale = s_defaultLocale;
		private string m_brandingContent = String.Empty;
		private string m_brandingPackage = s_defaultBrandingPackage;
		private string m_basePackage = String.Empty;
		private string m_helpOutput = s_defaultHelpOutput;
		private string m_catalogProductId = s_defaultCatalogProductId;
		private string m_catalogVersion = s_defaultCatalogVersion;
		private string m_vendorName = String.Empty;
		private HelpLibraryManager m_helpLibraryManager = new HelpLibraryManager ();
		private XslCompiledTransform m_brandingTransform = null;
		private XsltArgumentList m_transformArguments = null;

		#endregion

		#region Constructor
		//=====================================================================

		public BrandingComponent (BuildAssembler assembler, XPathNavigator configuration)
			: base (assembler, configuration)
		{
			XPathNavigator v_configData = configuration.SelectSingleNode ("data");

			if (v_configData != null)
			{
				String v_configValue;

				m_brandingContent = v_configData.GetAttribute (s_configBrandingContent, String.Empty);

				if (!String.IsNullOrEmpty (v_configValue = v_configData.GetAttribute (s_configLocale, String.Empty)))
				{
					m_locale = v_configValue;
				}
				if (!String.IsNullOrEmpty (v_configValue = v_configData.GetAttribute (s_configSelfBranded, String.Empty)))
				{
					m_selfBranded = bool.Parse (v_configValue);
				}
				if (!String.IsNullOrEmpty (v_configValue = v_configData.GetAttribute (s_configBrandingPackage, String.Empty)))
				{
					m_brandingPackage = v_configValue;
				}
				if (!String.IsNullOrEmpty (v_configValue = v_configData.GetAttribute (s_configBasePackage, String.Empty)))
				{
					m_basePackage = v_configValue;
				}
				else
				{
					v_configValue = m_helpLibraryManager.HelpLibraryViewerPath;
					if (String.IsNullOrEmpty (v_configValue))
					{
						WriteMessage (MessageLevel.Error, "The MS Help Viewer installation could not be found. A base branding package must be specified.");
					}
					else
					{
						m_basePackage = Path.Combine (v_configValue, s_defaultBrandingPackage + s_packageExtension);
					}
				}
				if (!String.IsNullOrEmpty (v_configValue = v_configData.GetAttribute (s_configHelpOutput, String.Empty)))
				{
					m_helpOutput = v_configValue;
				}
				if (!String.IsNullOrEmpty (v_configValue = v_configData.GetAttribute (s_configCatalogProductId, String.Empty)))
				{
					m_catalogProductId = v_configValue;
				}
				if (!String.IsNullOrEmpty (v_configValue = v_configData.GetAttribute (s_configCatalogVersion, String.Empty)))
				{
					m_catalogVersion = v_configValue;
				}
				m_vendorName = v_configData.GetAttribute (s_configVendorName, String.Empty);
			}

			if (String.Compare (m_helpOutput, "MSHelpViewer", StringComparison.OrdinalIgnoreCase) != 0)
			{
				LoadBrandingTransform ();
			}
		}

		#endregion

		#region Apply method
		//=====================================================================

		public override void Apply (XmlDocument document, string key)
		{
#if DEBUG_NOT
			try
			{
				WriteMessage (MessageLevel.Info, String.Format ("--- HelpOutput [{0}] SelfBranded [{1}] Package [{2}] Locale [{3}] ---", m_helpOutput, m_selfBranded, m_brandingPackage, m_locale));
			}
			catch { }
#endif
			ReformatLanguageSpecific (document);
			MakePlainCodeCopies (document);

			if (String.Compare (m_helpOutput, "MSHelpViewer", StringComparison.OrdinalIgnoreCase) != 0)
			{
				ApplyBranding (document, key);
			}
		}

		#endregion

		#region Branding
		//=====================================================================

		private void ApplyBranding (XmlDocument document, string key)
		{
			if (m_brandingTransform != null)
			{
#if DEBUG
				try
				{
					WriteMessage (MessageLevel.Info, String.Format ("  Branding [{0}] [{1}]", key, m_locale));
				}
				catch { }
#endif
				try
				{
					XmlReader v_reader = new XmlNodeReader (document);
					//XmlReader v_reader = new XhtmlNodeReader (document, this);
					//Stream lStream = new MemoryStream();

					//XmlNamespaceManager v_Manager = new XmlNamespaceManager (v_Reader.NameTable);
					//WriteMessage (MessageLevel.Info, String.Format ("  DefaultNamespace [{0}]", v_Manager.DefaultNamespace));
					//foreach (KeyValuePair<String, String> lNamespace in v_Manager.GetNamespacesInScope (XmlNamespaceScope.All))
					//{
					//    WriteMessage (MessageLevel.Info, String.Format ("  Namespace [{0}] [{1}]", lNamespace.Key, lNamespace.Value));
					//}
					//v_Manager.AddNamespace ("xhtml", "http://www.w3.org/1999/xhtml");
					//WriteMessage (MessageLevel.Info, String.Format ("  DefaultNamespace [{0}]", v_Manager.DefaultNamespace));
					//foreach (KeyValuePair<String, String> lNamespace in v_Manager.GetNamespacesInScope (XmlNamespaceScope.All))
					//{
					//    WriteMessage (MessageLevel.Info, String.Format ("  Namespace [{0}] [{1}]", lNamespace.Key, lNamespace.Value));
					//}

					XmlNode v_header = document.DocumentElement.SelectSingleNode ("head");
					XmlElement v_meta = document.CreateElement ("meta");
					v_meta.SetAttribute ("name", "SelfBranded");
					v_meta.SetAttribute ("content", "true");
					v_header.AppendChild (v_meta);

					String v_tempPath = Path.GetFullPath (Path.Combine (m_brandingContent, "..\\Temp"));
					if (!Directory.Exists (v_tempPath))
					{
						Directory.CreateDirectory (v_tempPath);
					}
					v_tempPath = Path.Combine (v_tempPath, key + ".htm");

					using (FileStream v_stream = new FileStream (v_tempPath, FileMode.Create, FileAccess.ReadWrite))
					{
						using (XmlWriter v_writer = XmlWriter.Create (v_stream, m_brandingTransform.OutputSettings))
						{
							m_brandingTransform.Transform (v_reader, m_transformArguments, v_writer);
						}
					}
				}
				catch (XsltException exp)
				{
					WriteMessage (MessageLevel.Warn, String.Format ("{0} at {1} {2} {3}", exp.Message, exp.SourceUri, exp.LineNumber, exp.LinePosition));
				}
				catch (Exception exp)
				{
					WriteMessage (MessageLevel.Error, exp.Message);
				}
			}
		}

		private void LoadBrandingTransform ()
		{
			try
			{
				String v_brandingContentBase = null;

				if (!String.IsNullOrEmpty (m_brandingContent))
				{
					v_brandingContentBase = Path.GetFullPath (Environment.ExpandEnvironmentVariables (m_brandingContent));
				}
				if (!String.IsNullOrEmpty (v_brandingContentBase))
				{
					String v_brandingTransformName = String.Format ("branding-{0}.xslt", m_locale);
					XslCompiledTransform v_brandingTransform = new XslCompiledTransform ();
					XmlResolver v_resolver = new XmlUrlResolver ();

					m_transformArguments = new XsltArgumentList ();
					m_transformArguments.XsltMessageEncountered += new XsltMessageEncounteredEventHandler (OnTransformMessageEncountered);
					m_transformArguments.AddParam ("catalogProductFamily", String.Empty, m_catalogProductId);
					m_transformArguments.AddParam ("catalogProductVersion", String.Empty, m_catalogVersion);
					m_transformArguments.AddParam ("catalogLocale", String.Empty, m_locale);
					LoadBrandingConfig (Path.Combine (v_brandingContentBase, "branding.xml"), ref v_brandingTransformName);

					WriteMessage (MessageLevel.Info, String.Format ("Branding transform \"{0}\" catalogProductFamily={1} catalogProductVersion={2} catalogLocale={3}", Path.Combine (v_brandingContentBase, v_brandingTransformName), m_catalogProductId, m_catalogVersion, m_locale));
					v_brandingTransform.Load (Path.Combine (v_brandingContentBase, v_brandingTransformName), XsltSettings.TrustedXslt, v_resolver);
					m_brandingTransform = v_brandingTransform;
				}
			}
			catch (XsltCompileException exp)
			{
				WriteMessage (MessageLevel.Warn, String.Format ("{0} at {1} {2} {3}", exp.Message, exp.SourceUri, exp.LineNumber, exp.LinePosition));
			}
			catch (Exception exp)
			{
				WriteMessage (MessageLevel.Error, exp.Message);
			}
		}

		private void LoadBrandingConfig (String configPath, ref String transformName)
		{
			try
			{
				XmlDocument v_brandingConfig = new XmlDocument ();
				XmlNamespaceManager v_namespaceManager;
				XmlNode v_argumentsNode;
				XmlNode v_transformNode;

				v_brandingConfig.Load (configPath);
				v_namespaceManager = new XmlNamespaceManager (v_brandingConfig.NameTable);
				v_namespaceManager.AddNamespace ("branding", "urn:FH-Branding");

				v_argumentsNode = v_brandingConfig.DocumentElement.SelectSingleNode ("branding:common-parameters", v_namespaceManager);
				v_transformNode = v_brandingConfig.DocumentElement.SelectSingleNode (String.Format ("branding:transform-parameters[@xml:lang='{0}']", m_locale), v_namespaceManager);

				if (v_argumentsNode != null)
				{
					foreach (XmlNode v_argumentNode in v_argumentsNode.ChildNodes)
					{
						if (String.Compare (v_argumentNode.LocalName, "parameter", StringComparison.OrdinalIgnoreCase) == 0)
						{
							XmlNode v_argumentName = v_argumentNode.Attributes.GetNamedItem ("name");
							XmlNode v_argumentValue = v_argumentNode.Attributes.GetNamedItem ("value");

							if ((v_argumentName != null) && (v_argumentNode != null))
							{
								m_transformArguments.AddParam (v_argumentName.Value, String.Empty, v_argumentValue.Value);
							}
						}
					}
				}
				if (v_transformNode != null)
				{
					v_transformNode = v_transformNode.Attributes.GetNamedItem ("transform");
				}
				if (v_transformNode != null)
				{
					transformName = v_transformNode.Value;
				}
			}
			catch { }
		}

		void OnTransformMessageEncountered (object sender, XsltMessageEncounteredEventArgs e)
		{
			WriteMessage (MessageLevel.Info, e.Message);
		}

		private class XhtmlNodeReader : XmlNodeReader
		{
			private Microsoft.Ddue.Tools.BuildComponent m_buildComponent;
			private XmlNamespaceManager m_namespaceManager;

			public XhtmlNodeReader (XmlDocument document, Microsoft.Ddue.Tools.BuildComponent BuildComponent)
				: base (document)
			{
				m_buildComponent = BuildComponent;
				m_namespaceManager = new XmlNamespaceManager (NameTable);

				m_namespaceManager.AddNamespace ("", "http://www.w3.org/1999/xhtml");
				m_namespaceManager.AddNamespace ("xhtml", "http://www.w3.org/1999/xhtml");
			}

			public override string LookupNamespace (string prefix)
			{
				m_buildComponent.BuildAssembler.MessageHandler (m_buildComponent.GetType (), MessageLevel.Info, String.Format ("LookupNamespace ==> {0}", prefix));
				return base.LookupNamespace (prefix);
			}

			public override string LocalName
			{
				get
				{
					return base.LocalName;
				}
			}

			public override String Name
			{
				get
				{
					if (!String.IsNullOrEmpty (base.Prefix))
					{
						m_buildComponent.BuildAssembler.MessageHandler (m_buildComponent.GetType (), MessageLevel.Info, String.Format ("Name ==> {0}", base.Name));
					}
					else
					{
						if (base.Name == "head")
						{
							m_buildComponent.BuildAssembler.MessageHandler (m_buildComponent.GetType (), MessageLevel.Info, String.Format ("Name ==> {0}:head", Prefix));
						}
						if (base.Name == "body")
						{
							m_buildComponent.BuildAssembler.MessageHandler (m_buildComponent.GetType (), MessageLevel.Info, String.Format ("Name ==> {0}:body", Prefix));
						}
					}
					return base.Name;
				}
			}
			public override String Prefix
			{
				get
				{
					if (!String.IsNullOrEmpty (base.Prefix))
					{
						m_buildComponent.BuildAssembler.MessageHandler (m_buildComponent.GetType (), MessageLevel.Info, String.Format ("Prefix ==> {0}", base.Prefix));
					}
					else
					{
						if (base.NodeType == XmlNodeType.Element)
						{
							return "xhtml";
						}
						if (base.NodeType == XmlNodeType.EndElement)
						{
							return "xhtml";
						}
					}
					return base.Prefix;
				}
			}
		}

		#endregion

		#region LanguageSpecific reformatting
		//=====================================================================

		/// <summary>
		/// Reformats all LanguageSpecific spans to the format used by the
		/// MS Help Viewer.
		/// </summary>
		private void ReformatLanguageSpecific (XmlDocument document)
		{
			int v_uniqueIdSequence = 0;
			XmlNodeList v_nodeList = document.SelectNodes ("//span[@class='languageSpecificText']");

			foreach (XmlNode v_node in v_nodeList)
			{
				XmlNodeList v_partList = v_node.SelectNodes ("span[@class]");
				String v_partText = String.Empty;

				if ((v_partList.Count > 0) && (v_partList.Count == v_node.ChildNodes.Count))
				{
#if true
					//
					//	Option 1 - implement LST as it appears in the final page
					//
					String v_uniqueId = String.Format (CultureInfo.InvariantCulture, "IDLST{0:D6}", ++v_uniqueIdSequence);
					XmlElement v_spanElement;
					XmlElement v_scriptElement;

					foreach (XmlNode v_partNode in v_partList)
					{
						if (!String.IsNullOrEmpty (v_partText))
						{
							v_partText += "|";
						}
						v_partText += String.Format (CultureInfo.InvariantCulture, "{0}={1}", v_partNode.Attributes.GetNamedItem ("class").Value, v_partNode.InnerText.Trim ('\''));
					}

					v_spanElement = document.CreateElement ("span");
					v_spanElement.SetAttribute ("id", v_uniqueId);
					v_scriptElement = document.CreateElement ("script");
					v_scriptElement.SetAttribute ("type", "text/javascript");
					v_scriptElement.InnerText = String.Format (CultureInfo.InvariantCulture, "addToLanSpecTextIdSet(\"{0}?{1}\");", v_uniqueId, v_partText);

					v_node.ParentNode.InsertAfter (v_scriptElement, v_node);
					v_node.ParentNode.ReplaceChild (v_spanElement, v_node);
#else
					//
					//	Option 2 - implement LST as it appears in the raw page
					//
					XmlElement v_lstElement;

					v_lstElement = document.CreateElement ("mtps:LanguageSpecificText");

					foreach (XmlNode v_partNode in v_partList)
					{
						v_lstElement.SetAttribute (String.Format (CultureInfo.InvariantCulture, "devLang{0}", v_partNode.Attributes.GetNamedItem ("class").Value), v_partNode.InnerText);
					}
					v_node.ParentNode.ReplaceChild (v_lstElement, v_node);
#endif
				}
			}
		}

		#endregion

		#region Code snippet finalization
		//=====================================================================

		/// <summary>
		/// Each code snippet in an MSHC document has two formats - colorized and
		/// plain.
		/// The colorization happens in different places for Conceptual and
		/// Sandcastle Reference topics, so we use this opportunity to make a
		/// plain copy of each colorized code snippet.
		/// The MS Help Viewer uses the plain version in its Copy and Print scripts.
		/// </summary>
		private void MakePlainCodeCopies (XmlDocument document)
		{
			XmlNodeList v_codeNodes = document.SelectNodes ("//div[@class='OH_CodeSnippetContainerCode']");

			foreach (XmlNode v_ColoredCodeDiv in v_codeNodes)
			{
				XmlNode v_codeNodeId = v_ColoredCodeDiv.Attributes.GetNamedItem ("id");

				if ((v_codeNodeId != null) && (v_codeNodeId.Value.Contains ("_code_Div")))
				{
					XmlNode v_plainCodeDiv = v_ColoredCodeDiv.ParentNode.SelectSingleNode (String.Format (CultureInfo.InvariantCulture, "div[@id='{0}']", v_codeNodeId.Value.Replace ("_code_Div", "_code_Plain_Div")));

					if (v_plainCodeDiv != null)
					{
						XmlNode v_coloredCode = v_ColoredCodeDiv.SelectSingleNode ("descendant::pre");
						XmlNode v_plainCode = v_plainCodeDiv.SelectSingleNode ("descendant::pre");

						if ((v_coloredCode != null) && (v_plainCode != null))
						{
							v_plainCode.InnerXml = String.Empty;
							v_plainCode.AppendChild (document.CreateTextNode (v_coloredCode.InnerText));
						}
					}
				}
			}
		}

		#endregion
	}
}
