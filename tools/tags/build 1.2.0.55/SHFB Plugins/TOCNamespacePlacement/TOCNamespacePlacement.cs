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

namespace SandcastleBuilder.PlugIns
{
	/// <summary>
	/// 
	/// </summary>
	public class TOCNamespacePlacement : IPlugIn
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
			get { return "Table of Contents Namespace Placement"; }
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
				return "This plug-in uses a SiteMap to place namespace contents in the Table of Contents." +
					"Each namespace can be individually placed anywhere in the TOC.";
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
                        new ExecutionPoint(BuildStep.CombiningIntermediateTocFiles, ExecutionBehaviors.After)
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

			if ((context.BuildStep == BuildStep.CombiningIntermediateTocFiles) && (context.Behavior == ExecutionBehaviors.After))
			{
				m_buildProcess.ReportProgress ("{0} Version {1}\r\n{2}", this.Name, this.Version, this.Copyright);
				ReparentNamespaceTopics ();
			}
		}
		#endregion

		#region Helper Methods
		//=====================================================================

		private void ReparentNamespaceTopics ()
		{
			try
			{
				String v_tocFilePath = Path.Combine (m_buildProcess.WorkingFolder, "toc.xml");
				XmlDocument v_document = new XmlDocument ();
				XPathNavigator v_navigator = null;
				List<XPathNavigator> v_targetNodes = new List<XPathNavigator> ();
				bool v_changed = false;

#if	DEBUG
				Debug.Print ("ReparentNamespaceTopics {0}", v_tocFilePath);
#endif
				m_buildProcess.ReportProgress ("{0}: ReparentNamespaceTopics '{1}'", this.Name, v_tocFilePath);
				v_document.Load (v_tocFilePath);

				v_navigator = v_document.CreateNavigator ();
				if (v_navigator != null)
				{
					XPathNodeIterator v_nodes = v_navigator.Select ("//topic[@title=@id and @title!='' and not(@file)]");

					if ((v_nodes != null) && (v_nodes.Count > 0))
					{
#if	DEBUG
						Debug.Print ("Targets [{0}]", v_nodes.Count);
#endif
						while (v_nodes.MoveNext ())
						{
							v_targetNodes.Add (v_nodes.Current.Clone ());
						}
					}
				}
				foreach (XPathNavigator v_targetNode in v_targetNodes)
				{
					String v_targetId = v_targetNode.GetAttribute ("id", String.Empty);
					XPathNodeIterator v_nodes = null;
#if	DEBUG
					Debug.Print ("  Target [{0}]", v_targetId);
#endif
					if (v_targetId.StartsWith ("N:"))
					{
						v_nodes = v_navigator.Select ("//topic[@id='" + v_targetId + "' and not(@title) and @file]");
					}
					if ((v_nodes != null) && (v_nodes.Count == 1) && v_nodes.MoveNext ())
					{
#if	DEBUG
						Debug.Print ("  Source [{0}] [{1}]", v_nodes.Current.GetAttribute ("id", String.Empty), v_nodes.Current.GetAttribute ("file", String.Empty));
#endif
						m_buildProcess.ReportProgress ("{0}:   Reparent id='{1}' file='{2}'", this.Name, v_targetId, v_nodes.Current.GetAttribute ("file", String.Empty));

						try
						{
							v_targetNode.ReplaceSelf (v_nodes.Current);
							v_nodes.Current.DeleteSelf ();
							v_changed = true;
						}
						catch (Exception exp)
						{
							System.Diagnostics.Debug.Print (exp.Message);
						}
					}
				}

				if (v_changed)
				{
					v_document.Save (v_tocFilePath);
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
		~TOCNamespacePlacement ()
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
