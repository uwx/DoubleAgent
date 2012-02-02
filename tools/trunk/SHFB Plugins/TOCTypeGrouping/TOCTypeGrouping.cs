using System;
using System.Diagnostics;
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
	public class SitemapToTOC : IPlugIn
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
			get { return "Table of Contents Type Grouping"; }
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
				return "This plug-in categorizes types by 'Class', 'Interface', 'Delegate', 'Enumeration', etc." +
					"It adds an extra level of category entries for each namespace in the Table of Contents.";
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
                        new ExecutionPoint(BuildStep.GenerateIntermediateTableOfContents, ExecutionBehaviors.Before)
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

			if ((context.BuildStep == BuildStep.GenerateIntermediateTableOfContents) && (context.Behavior == ExecutionBehaviors.Before))
			{
				UpdateGenerateIntermediateProject ();
			}
		}
		#endregion

		#region Helper Methods
		//=====================================================================

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

		~SitemapToTOC ()
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
