using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Xml.XPath;

using SandcastleBuilder.Utils;
using SandcastleBuilder.Utils.BuildEngine;
using SandcastleBuilder.Utils.PlugIn;

namespace SandcastleBuilder.PlugIns.CinSoft
{
	public class PackAndSignMSHC : IPlugIn
	{
		#region Private data members
		//=====================================================================

		private ExecutionPointCollection mExecutionPoints;
		private BuildProcess mBuildProcess;
		private String mConfiguration;

		#endregion

		#region IPlugIn Properties
		//=====================================================================

		public string Name
		{
			get { return "Pack and Sign MSHC files"; }
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
				return "This plug-in will pack MSHC files into a CAB file and code-sign the CAB file." +
					"The purpose is to create a deployment package that the Help Viewer will trust.";
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
                        new ExecutionPoint(BuildStep.CompilingHelpFile, ExecutionBehaviors.After)
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
			PackAndSignMHSCConfig lConfigDialog = new PackAndSignMHSCConfig (project);
			lConfigDialog.Configuration = currentConfig;
			if (lConfigDialog.ShowDialog ().GetValueOrDefault ())
			{
				return lConfigDialog.Configuration;
			}
			return currentConfig;
		}

		public void Initialize (BuildProcess buildProcess, XPathNavigator configuration)
		{
			mBuildProcess = buildProcess;
			mConfiguration = configuration.OuterXml;
		}

		public void Execute (ExecutionContext context)
		{
			if (mBuildProcess.CurrentFormat == HelpFileFormat.MSHelpViewer)
			{
				PackAndSignMSHCSettings lSettings = new PackAndSignMSHCSettings (mBuildProcess.CurrentProject, mConfiguration);
				String lDdfPath = null;

				mBuildProcess.ReportProgress ("{0} Version {1}\r\n{2}", this.Name, this.Version, this.Copyright);

				if (CanExecute (lSettings, out lDdfPath))
				{
					FilePath lMakeCabPath = new FilePath (lSettings.MakeCabPath, mBuildProcess.CurrentProject);
					FilePath lSignToolPath = new FilePath (lSettings.SignToolPath, mBuildProcess.CurrentProject);
					String lSourcePath = Path.Combine (mBuildProcess.OutputFolder, Path.ChangeExtension (mBuildProcess.CurrentProject.HtmlHelpName, ".mshc"));
					String lTargetPath = Path.Combine (lSettings.CabPathName, lSettings.CabFileName);
					String lMakeCabArgs;
					String lSignToolArgs = null;
					String lSignToolStamp = String.Empty;

					lMakeCabArgs = String.Format ("/V1 /F \"{0}\" /D SourcePath='{1}' /D TargetDir='{2}' /D TargetFile='{3}'", lDdfPath, lSourcePath, lSettings.CabPathName, lSettings.CabFileName);
					mBuildProcess.RunProcess (lMakeCabPath, lMakeCabArgs);

					if (File.Exists (lTargetPath))
					{
						mBuildProcess.HelpViewerFiles.Add (lTargetPath);

						if (!String.IsNullOrEmpty (lSettings.TimeStampUrl))
						{
							lSignToolStamp = String.Format ("{0} {1} ", PackAndSignMSHCSettings.StoreTimestamperFlag, lSettings.TimeStampUrl);
						}
						if (lSettings.IsFileCertificate)
						{
							lSignToolArgs = String.Format ("sign /f \"{0}\" {1} \"{2}\"", lSettings.FileCertificatePath, lSignToolStamp, lTargetPath);
						}
						else if (lSettings.IsStoreCertificate)
						{
							lSignToolArgs = String.Format ("sign {0} {1} \"{2}\"", lSettings.CertificatePath, lSignToolStamp, lTargetPath);
						}
						else
						{
							mBuildProcess.ReportWarning (Name, "No code-signing certificate selected");
						}
						if (!String.IsNullOrEmpty (lSignToolArgs))
						{
							mBuildProcess.RunProcess (lSignToolPath, lSignToolArgs);
							FixMSHAFiles ();
						}
					}
				}
			}
		}
		#endregion

		#region Helper Methods
		//=====================================================================

		private bool CanExecute (PackAndSignMSHCSettings Settings, out String DdfPath)
		{
			DdfPath = Path.ChangeExtension (Assembly.GetExecutingAssembly ().Location, ".ddf");
			FilePath lMakeCabPath = new FilePath (Settings.MakeCabPath,mBuildProcess.CurrentProject);
			FilePath lSignToolPath = new FilePath (Settings.SignToolPath, mBuildProcess.CurrentProject);

			if (mBuildProcess.IsPartialBuild)
			{
				mBuildProcess.ReportWarning (Name, "Skipped for partial build");
			}
			else if (!File.Exists (DdfPath))
			{
				mBuildProcess.ReportWarning (Name, "Missing directives file \"{0}\"", DdfPath);
			}
			else if (String.IsNullOrEmpty (Settings.CabFileName))
			{
				mBuildProcess.ReportWarning (Name, "Blank Cabinet file name");
			}
			else if (String.IsNullOrEmpty (Settings.CabPathName))
			{
				mBuildProcess.ReportWarning (Name, "Blank Cabinet output path");
			}
			else if (String.IsNullOrEmpty (Settings.MakeCabPath))
			{
				mBuildProcess.ReportWarning (Name, "No path specified for makecab.exe");
			}
			else if (String.IsNullOrEmpty (Settings.SignToolPath))
			{
				mBuildProcess.ReportWarning (Name, "No path specified for signtool.exe");
			}
			else if (!lMakeCabPath.Exists)
			{
				mBuildProcess.ReportWarning (Name, "Invalid path for makecab.exe - \"{0}\"", Settings.MakeCabPath);
			}
			else if (!lSignToolPath.Exists)
			{
				mBuildProcess.ReportWarning (Name, "Invalid path for signtool.exe - \"{0}\"", Settings.SignToolPath);
			}
			else if (String.IsNullOrEmpty (Settings.TimeStampUrl))
			{
				mBuildProcess.ReportWarning (Name, "No timestamer specified - the Cabinet file signature may not be trusted");
				return true;
			}
			else
			{
				return true;
			}
			return false;
		}

		private void FixMSHAFiles ()
		{
			FixMSHAFile (Path.Combine (mBuildProcess.OutputFolder, "HelpContentSetup.msha"));
			FixMSHAFile (Path.Combine (mBuildProcess.OutputFolder, mBuildProcess.CurrentProject.HtmlHelpName + ".msha"));
			FixMSHAFile (Path.Combine (mBuildProcess.WorkingFolder, mBuildProcess.CurrentProject.HtmlHelpName + ".msha"));
		}

		private void FixMSHAFile (String FilePath)
		{
			try
			{
				String [] lInputLines = File.ReadAllLines (FilePath);
				List<String> lOutputLines = new List<String> ();

				foreach (String lLine in lInputLines)
				{
					lOutputLines.Add (lLine.Replace (".mshc", ".cab").Replace (".MSHC", ".CAB"));
				}
				File.WriteAllLines (FilePath, lOutputLines.ToArray());
			}
			catch (Exception exp)
			{
				System.Diagnostics.Debug.Print (exp.Message);
			}
		}
		#endregion

		#region IDisposable implementation
		//=====================================================================

		~PackAndSignMSHC ()
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
