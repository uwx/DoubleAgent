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

		private const string s_xhtmlNamespace = "http://www.w3.org/1999/xhtml";

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

			if (String.Compare (m_helpOutput, s_defaultHelpOutput, StringComparison.OrdinalIgnoreCase) != 0)
			{
				LoadBrandingTransform ();
			}
		}

		#endregion

		#region Apply method
		//=====================================================================

		public override void Apply (XmlDocument document, string key)
		{
			ReformatLanguageSpecific (document);
			MakePlainCodeCopies (document);

			if (String.Compare (m_helpOutput, s_defaultHelpOutput, StringComparison.OrdinalIgnoreCase) != 0)
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
				WriteMessage (MessageLevel.Info, String.Format ("  Branding topic {0} ({1}) SelfBranded={2}", key, m_locale, m_selfBranded));

				try
				{
					// The default xhtml namespace is required for the branding transforms to work,
					if (String.IsNullOrEmpty (document.DocumentElement.GetAttribute ("xmlns")))
					{
						document.DocumentElement.SetAttribute ("xmlns", s_xhtmlNamespace);
						document.LoadXml (document.OuterXml);
					}
					SetSelfBranding (document, m_selfBranded);
#if DEBUG_NOT
					String v_tempPrePath = Path.GetFullPath (Path.Combine (m_brandingContent, "..\\..\\Temp\\Pre"));
					if (!Directory.Exists (v_tempPrePath))
					{
						Directory.CreateDirectory (v_tempPrePath);
					}
					v_tempPrePath = Path.Combine (v_tempPrePath, key + ".htm");
					document.Save (v_tempPrePath);
#endif
#if DEBUG_NOT
					String v_tempPostPath = Path.GetFullPath (Path.Combine (m_brandingContent, "''\\..\\Temp\\Post"));
					if (!Directory.Exists (v_tempPostPath))
					{
						Directory.CreateDirectory (v_tempPostPath);
					}
					v_tempPostPath = Path.Combine (v_tempPostPath, key + ".htm");
					using (FileStream v_stream = new FileStream (v_tempPostPath, FileMode.Create, FileAccess.ReadWrite))
#else
					using (Stream v_stream = new MemoryStream ())
#endif
					{
						try
						{
							XPathNavigator v_navigator = document.CreateNavigator ();
							using (XmlWriter v_writer = XmlWriter.Create (v_stream, m_brandingTransform.OutputSettings))
							{
								m_brandingTransform.Transform (v_navigator, m_transformArguments, v_writer);
							}

							XmlReaderSettings v_readerSettings = new XmlReaderSettings ();
							v_readerSettings.ConformanceLevel = ConformanceLevel.Fragment;
							v_stream.Seek (0, SeekOrigin.Begin);
							using (XmlReader v_reader = XmlReader.Create (v_stream, v_readerSettings))
							{
								document.RemoveAll ();
								document.Load (v_reader);
							}

							// The default xhtml namespace needs to be removed for all help targets.
							// For MS Help Viewer, it will be added again if necessary by the SaveComponent.
							RemoveUnusedNamespaces (document);
							document.DocumentElement.RemoveAttribute ("xmlns");
						}
						catch (Exception exp)
						{
							WriteMessage (MessageLevel.Warn, exp.Message);
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

		/// <summary>
		/// Make sure that the document's default namespace is the xmhtml namespace.  If not
		/// the branding transforms will not work.
		/// </summary>
		/// <param name="document">The current document.</param>
		/// <returns></returns>
		private XmlDocument AdjustDocumentNamespace (XmlDocument document)
		{
			XmlNamespaceManager v_namespaceManager = new XmlNamespaceManager (document.NameTable);

			if (String.IsNullOrEmpty (v_namespaceManager.DefaultNamespace))
			{
				try
				{
					Stream v_Stream = new MemoryStream ();
					XmlReaderSettings v_readerSettings;
					XmlParserContext v_parserContext;

					v_namespaceManager = new XmlNamespaceManager (new NameTable ());
					v_namespaceManager.AddNamespace (String.Empty, s_xhtmlNamespace);
					v_readerSettings = new XmlReaderSettings ();
					v_readerSettings.NameTable = v_namespaceManager.NameTable;
					v_readerSettings.ConformanceLevel = ConformanceLevel.Fragment;
					v_parserContext = new XmlParserContext (v_namespaceManager.NameTable, v_namespaceManager, null, XmlSpace.None);

					document.Save (v_Stream);
					v_Stream.Seek (0, SeekOrigin.Begin);

					using (XmlReader v_reader = XmlReader.Create (v_Stream, v_readerSettings, v_parserContext))
					{
						XmlDocument v_document = new XmlDocument ();
						v_document.Load (v_reader);
						return v_document;
					}
				}
				catch (Exception exp)
				{
					WriteMessage (MessageLevel.Warn, exp.Message);
				}
			}
			return document;
		}

		/// <summary>
		/// Mark the document as SelfBranded for the branding transforms.
		/// </summary>
		/// <param name="document">The current document.</param>
		/// <param name="selfBranded">The SelfBranded setting.</param>
		private void SetSelfBranding (XmlDocument document, bool selfBranded)
		{
			try
			{
				XmlNamespaceManager v_namespaceManager;
				XmlNode v_header;

				v_namespaceManager = new XmlNamespaceManager (document.NameTable);
				v_namespaceManager.AddNamespace ("xhtml", s_xhtmlNamespace);

				v_header = document.DocumentElement.SelectSingleNode ("xhtml:head", v_namespaceManager);
				if (v_header != null)
				{
					XmlNode v_branded = v_header.SelectSingleNode ("meta[(@name='SelfBranded') | (@name='Microsoft.Help.SelfBranded')]");
					if (v_branded == null)
					{
						XmlElement v_meta = document.CreateElement ("meta", s_xhtmlNamespace);
						v_meta.SetAttribute ("name", "SelfBranded");
						v_meta.SetAttribute ("content", selfBranded.ToString ().ToLower ());
						v_header.AppendChild (v_meta);
					}
					else
					{
						XmlAttribute v_Attribute = document.CreateAttribute ("meta", String.Empty);
						v_Attribute.Value = selfBranded.ToString ().ToLower ();
						v_branded.Attributes.SetNamedItem (v_Attribute);
					}
				}
			}
			catch (Exception exp)
			{
				WriteMessage (MessageLevel.Warn, exp.Message);
			}
		}

		/// <summary>
		/// Remove extra unused namespace declarations.
		/// </summary>
		/// <param name="document">The current document.</param>
		private void RemoveUnusedNamespaces (XmlDocument document)
		{
			try
			{
				XmlNamespaceManager v_namespaceManager;
				List<XmlNode> v_attribues = new List<XmlNode> ();
#if DEBUG_NOT
				WriteMessage (MessageLevel.Info, "  RemoveUnusedNamespaces");
#endif
				v_namespaceManager = new XmlNamespaceManager (document.NameTable);
				v_namespaceManager.AddNamespace ("xhtml", s_xhtmlNamespace);

				if ((document.DocumentElement != null) && (document.DocumentElement.Attributes != null))
				{
					foreach (XmlNode v_attribute in document.DocumentElement.Attributes)
					{
						if (String.Compare (v_attribute.Prefix, "xmlns", StringComparison.OrdinalIgnoreCase) == 0)
						{
							v_attribues.Add (v_attribute);
							if (!v_namespaceManager.HasNamespace (v_attribute.LocalName))
							{
								v_namespaceManager.AddNamespace (v_attribute.LocalName, v_attribute.Value);
							}
						}
					}
					foreach (XmlNode v_attribute in v_attribues)
					{
#if DEBUG_NOT
						WriteMessage (MessageLevel.Info, String.Format ("    Check namespace [{0}] [{1}]", v_attribute.LocalName, v_attribute.Name));
#endif
						XmlNode v_namespaceUsed = document.SelectSingleNode (String.Format ("(/*//{0}:*) | (/*//*[@{0}:*])", v_attribute.LocalName), v_namespaceManager);
						if (v_namespaceUsed != null)
						{
#if DEBUG_NOT
							WriteMessage (MessageLevel.Info, String.Format ("      Used [{0}] [{1}] [{2}] [{3}]", v_namespaceUsed.Prefix, v_namespaceUsed.LocalName, v_namespaceUsed.Name, v_namespaceUsed.NamespaceURI));
							WriteMessage (MessageLevel.Info, String.Format ("      Used [{0}]", v_namespaceUsed.OuterXml));
#endif
						}
						else
						{
							document.DocumentElement.Attributes.RemoveNamedItem (v_attribute.Name);
						}
					}
				}
			}
			catch (Exception exp)
			{
				WriteMessage (MessageLevel.Warn, exp.Message);
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
					m_transformArguments.AddParam ("content-path", String.Empty, ".\\");

					LoadBrandingConfig (Path.Combine (v_brandingContentBase, "branding.xml"), ref v_brandingTransformName);

					WriteMessage (MessageLevel.Info, String.Format ("Branding Transform \"{0}\" catalogProductFamily={1} catalogProductVersion={2} catalogLocale={3}", Path.Combine (v_brandingContentBase, v_brandingTransformName), m_catalogProductId, m_catalogVersion, m_locale));
					v_brandingTransform.Load (Path.Combine (v_brandingContentBase, v_brandingTransformName), XsltSettings.TrustedXslt, v_resolver);
					m_brandingTransform = v_brandingTransform;
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
