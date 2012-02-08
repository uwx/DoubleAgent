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
		private XslCompiledTransform m_BrandingTransform = null;
		private XsltArgumentList m_TransformArguments = null;

		#endregion

		#region Constructor
		//=====================================================================

		public BrandingComponent (BuildAssembler assembler, XPathNavigator configuration)
			: base (assembler, configuration)
		{
			XPathNavigator lConfigData = configuration.SelectSingleNode ("data");

			if (lConfigData != null)
			{
				String lConfigValue;

				m_brandingContent = lConfigData.GetAttribute (s_configBrandingContent, String.Empty);

				if (!String.IsNullOrEmpty (lConfigValue = lConfigData.GetAttribute (s_configLocale, String.Empty)))
				{
					m_locale = lConfigValue;
				}
				if (!String.IsNullOrEmpty (lConfigValue = lConfigData.GetAttribute (s_configSelfBranded, String.Empty)))
				{
					m_selfBranded = bool.Parse (lConfigValue);
				}
				if (!String.IsNullOrEmpty (lConfigValue = lConfigData.GetAttribute (s_configBrandingPackage, String.Empty)))
				{
					m_brandingPackage = lConfigValue;
				}
				if (!String.IsNullOrEmpty (lConfigValue = lConfigData.GetAttribute (s_configBasePackage, String.Empty)))
				{
					m_basePackage = lConfigValue;
				}
				else
				{
					lConfigValue = m_helpLibraryManager.HelpLibraryViewerPath;
					if (String.IsNullOrEmpty (lConfigValue))
					{
						WriteMessage (MessageLevel.Error, "The MS Help Viewer installation could not be found. A base branding package must be specified.");
					}
					else
					{
						m_basePackage = Path.Combine (lConfigValue, s_defaultBrandingPackage + s_packageExtension);
					}
				}
				if (!String.IsNullOrEmpty (lConfigValue = lConfigData.GetAttribute (s_configHelpOutput, String.Empty)))
				{
					m_helpOutput = lConfigValue;
				}
				if (!String.IsNullOrEmpty (lConfigValue = lConfigData.GetAttribute (s_configCatalogProductId, String.Empty)))
				{
					m_catalogProductId = lConfigValue;
				}
				if (!String.IsNullOrEmpty (lConfigValue = lConfigData.GetAttribute (s_configCatalogVersion, String.Empty)))
				{
					m_catalogVersion = lConfigValue;
				}
				m_vendorName = lConfigData.GetAttribute (s_configVendorName, String.Empty);
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
			if (m_BrandingTransform != null)
			{
#if DEBUG
				try
				{
					WriteMessage (MessageLevel.Info, String.Format ("  Branding [{0}] [{1}]", key, m_locale));
				}
				catch { }
#endif
			}
		}

		private void LoadBrandingTransform ()
		{
			try
			{
				String lBrandingContentBase = null;

				if (!String.IsNullOrEmpty (m_brandingContent))
				{
					lBrandingContentBase = Path.GetFullPath (Environment.ExpandEnvironmentVariables (m_brandingContent));
				}
				if (!String.IsNullOrEmpty (lBrandingContentBase))
				{
					String lBrandingTransformName = String.Format ("branding-{0}.xslt", m_locale);
					XslCompiledTransform lBrandingTransform = new XslCompiledTransform ();
					XsltSettings lTransformSettings = new XsltSettings (true, true);
					XmlResolver lResolver = new XmlUrlResolver ();

					m_TransformArguments = new XsltArgumentList ();
					m_TransformArguments.XsltMessageEncountered += new XsltMessageEncounteredEventHandler (OnTransformMessageEncountered);
					m_TransformArguments.AddParam ("catalogProductFamily", String.Empty, m_catalogProductId);
					m_TransformArguments.AddParam ("catalogProductVersion", String.Empty, m_catalogVersion);
					m_TransformArguments.AddParam ("catalogLocale", String.Empty, m_locale);
					LoadBrandingConfig (Path.Combine (lBrandingContentBase, "branding.xml"), ref lBrandingTransformName);

					WriteMessage (MessageLevel.Info, String.Format ("Branding transform \"{0}\" catalogProductFamily={1} catalogProductVersion={2} catalogLocale={3}", Path.Combine (lBrandingContentBase, lBrandingTransformName), m_catalogProductId, m_catalogVersion, m_locale));
					lBrandingTransform.Load (Path.Combine (lBrandingContentBase, lBrandingTransformName), lTransformSettings, lResolver);
					m_BrandingTransform = lBrandingTransform;
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

		private void LoadBrandingConfig (String pConfigPath, ref String pTransformName)
		{
			try
			{
				XmlDocument lBrandingConfig = new XmlDocument ();
				XmlNamespaceManager lNamespaceManager;
				XmlNode lArgumentsNode;
				XmlNode lTransformNode;

				lBrandingConfig.Load (pConfigPath);
				lNamespaceManager = new XmlNamespaceManager (lBrandingConfig.NameTable);
				lNamespaceManager.AddNamespace ("branding", "urn:FH-Branding");

				lArgumentsNode = lBrandingConfig.DocumentElement.SelectSingleNode ("branding:common-parameters", lNamespaceManager);
				lTransformNode = lBrandingConfig.DocumentElement.SelectSingleNode (String.Format ("branding:transform-parameters[@xml:lang='{0}']", m_locale), lNamespaceManager);

				if (lArgumentsNode != null)
				{
					foreach (XmlNode lArgumentNode in lArgumentsNode.ChildNodes)
					{
						if (String.Compare (lArgumentNode.LocalName, "parameter", StringComparison.OrdinalIgnoreCase) == 0)
						{
							XmlNode lArgumentName = lArgumentNode.Attributes.GetNamedItem ("name");
							XmlNode lArgumentValue = lArgumentNode.Attributes.GetNamedItem ("value");

							if ((lArgumentName != null) && (lArgumentNode != null))
							{
								m_TransformArguments.AddParam (lArgumentName.Value, String.Empty, lArgumentValue.Value);
							}
						}
					}
				}
				if (lTransformNode != null)
				{
					lTransformNode = lTransformNode.Attributes.GetNamedItem ("transform");
				}
				if (lTransformNode != null)
				{
					pTransformName = lTransformNode.Value;
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
			int uniqueIdSequence = 0;
			XmlNodeList nodeList = document.SelectNodes ("//span[@class='languageSpecificText']");

			foreach (XmlNode node in nodeList)
			{
				XmlNodeList partList = node.SelectNodes ("span[@class]");
				String partText = String.Empty;

				if ((partList.Count > 0) && (partList.Count == node.ChildNodes.Count))
				{
#if true
					//
					//	Option 1 - implement LST as it appears in the final page
					//
					String uniqueId = String.Format (CultureInfo.InvariantCulture, "IDLST{0:D6}", ++uniqueIdSequence);
					XmlElement spanElement;
					XmlElement scriptElement;

					foreach (XmlNode partNode in partList)
					{
						if (!String.IsNullOrEmpty (partText))
						{
							partText += "|";
						}
						partText += String.Format (CultureInfo.InvariantCulture, "{0}={1}", partNode.Attributes.GetNamedItem ("class").Value, partNode.InnerText.Trim ('\''));
					}

					spanElement = document.CreateElement ("span");
					spanElement.SetAttribute ("id", uniqueId);
					scriptElement = document.CreateElement ("script");
					scriptElement.SetAttribute ("type", "text/javascript");
					scriptElement.InnerText = String.Format (CultureInfo.InvariantCulture, "addToLanSpecTextIdSet(\"{0}?{1}\");", uniqueId, partText);

					node.ParentNode.InsertAfter (scriptElement, node);
					node.ParentNode.ReplaceChild (spanElement, node);
#else
					//
					//	Option 2 - implement LST as it appears in the raw page
					//
					XmlElement lstElement;

					lstElement = _document.CreateElement ("mtps:LanguageSpecificText");

					foreach (XmlNode partNode in partList)
					{
						lstElement.SetAttribute (String.Format (CultureInfo.InvariantCulture, "devLang{0}", partNode.Attributes.GetNamedItem ("class").Value), partNode.InnerText);
					}
					node.ParentNode.ReplaceChild (lstElement, node);
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
			XmlNodeList codeNodes = document.SelectNodes ("//div[@class='OH_CodeSnippetContainerCode']");

			foreach (XmlNode coloredCodeDiv in codeNodes)
			{
				XmlNode codeNodeId = coloredCodeDiv.Attributes.GetNamedItem ("id");

				if ((codeNodeId != null) && (codeNodeId.Value.Contains ("_code_Div")))
				{
					XmlNode plainCodeDiv = coloredCodeDiv.ParentNode.SelectSingleNode (String.Format (CultureInfo.InvariantCulture, "div[@id='{0}']", codeNodeId.Value.Replace ("_code_Div", "_code_Plain_Div")));

					if (plainCodeDiv != null)
					{
						XmlNode coloredCode = coloredCodeDiv.SelectSingleNode ("descendant::pre");
						XmlNode plainCode = plainCodeDiv.SelectSingleNode ("descendant::pre");

						if ((coloredCode != null) && (plainCode != null))
						{
							plainCode.InnerXml = String.Empty;
							plainCode.AppendChild (document.CreateTextNode (coloredCode.InnerText));
						}
					}
				}
			}
		}

		#endregion
	}
}
