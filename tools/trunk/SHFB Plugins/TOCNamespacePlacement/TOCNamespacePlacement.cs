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

namespace SandcastleBuilder.PlugIns.CinSoft
{
	public class TOCNamespacePlacement : IPlugIn
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
			get { return "Table of Contents Namespace Placement"; }
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
				return "This plug-in uses a SiteMap to place namespace contents in the Table of Contents." +
					"Each namespace can be individually placed anywhere in the TOC.";
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
                        new ExecutionPoint(BuildStep.CombiningIntermediateTocFiles, ExecutionBehaviors.After)
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
			Debug.Print ("{0} {1}", context.Behavior, context.BuildStep);
#endif

			if ((context.BuildStep == BuildStep.CombiningIntermediateTocFiles) && (context.Behavior == ExecutionBehaviors.After))
			{
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
				String lTocFilePath = Path.Combine (mBuildProcess.WorkingFolder, "toc.xml");
				XmlDocument lDocument = new XmlDocument ();
				XPathNavigator lNavigator = null;
				List<XPathNavigator> lTargetNodes = new List<XPathNavigator> ();
				bool lChanged = false;

#if	DEBUG
				Debug.Print ("ReparentNamespaceTopics {0}", lTocFilePath);
#endif
				mBuildProcess.ReportProgress ("{0}: ReparentNamespaceTopics '{1}'", this.Name, lTocFilePath);
				lDocument.Load (lTocFilePath);

				lNavigator = lDocument.CreateNavigator ();
				if (lNavigator != null)
				{
					XPathNodeIterator lNodes = lNavigator.Select ("//topic[@title=@id and @title!='' and not(@file)]");

					if ((lNodes != null) && (lNodes.Count > 0))
					{
#if	DEBUG
						Debug.Print ("Targets [{0}]", lNodes.Count);
#endif
						while (lNodes.MoveNext ())
						{
							lTargetNodes.Add (lNodes.Current.Clone ());
						}
					}
				}
				foreach (XPathNavigator lTargetNode in lTargetNodes)
				{
					String lTargetId = lTargetNode.GetAttribute ("id", String.Empty);
					XPathNodeIterator lNodes = null;
#if	DEBUG
					Debug.Print ("  Target [{0}]", lTargetId);
#endif
					if (lTargetId.StartsWith ("N:"))
					{
						lNodes = lNavigator.Select ("//topic[@id='" + lTargetId + "' and not(@title) and @file]");
					}
					if ((lNodes != null) && (lNodes.Count == 1) && lNodes.MoveNext ())
					{
#if	DEBUG
						Debug.Print ("  Source [{0}] [{1}]", lNodes.Current.GetAttribute ("id", String.Empty), lNodes.Current.GetAttribute ("file", String.Empty));
#endif
						mBuildProcess.ReportProgress ("{0}:   Reparent id='{1}' file='{2}'", this.Name, lTargetId, lNodes.Current.GetAttribute ("file", String.Empty));

						try
						{
							lTargetNode.ReplaceSelf (lNodes.Current);
							lNodes.Current.DeleteSelf ();
							lChanged = true;
						}
						catch (Exception exp)
						{
							System.Diagnostics.Debug.Print (exp.Message);
						}
					}
				}

				if (lChanged)
				{
					lDocument.Save (lTocFilePath);
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

		~TOCNamespacePlacement ()
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
