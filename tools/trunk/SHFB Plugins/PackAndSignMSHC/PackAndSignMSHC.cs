using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Xml.XPath;

using SandcastleBuilder.Utils;
using SandcastleBuilder.Utils.BuildEngine;
using SandcastleBuilder.Utils.PlugIn;

namespace SandcastleBuilder.PlugIns
{
	/// <summary>
	/// This plugin packs an MS Help Viewer (Help3) MSHC file into a .CAB file and signs it
	/// with an Authenticode digital signature.
	/// <para>
	/// The resulting .CAB file will be recognized by "Manage Help Settings" as signed content.
	/// The user will be presented with the publisher's name and asked to accept the signature,
	/// and the content can be installed WITHOUT administrative privileges.
	/// </para>
	/// </summary>
	/// <remarks>
	/// To work properly, the makecab.exe and signtool.exe tools must be available. They can
	/// be downloaded from the Microsoft Windows SDK site.
	/// This plugin also requires a valid Authenticode code-signing certificate. For testing purposes,
	/// a test certificate in the user's "my" certificate store can be used.  Refer to the
	/// Visual Studio documentation for information on creating test certificates.
	/// <para>
	/// When the code-signing certificate is selected from a certificate store, at present
	/// the user's personal "my" store is used. If required, this code can be adapted to use the 
	/// machine store, or a store other than "my" (whatever signtool will accept).
	/// </para><para>
	/// If the code-signing certificate is retrieved from a file, you will have to enter
	/// the file's password every time it's opened. This may not be appropriate for unattended
	/// builds so you should consider installing the certificate in your "my" certificate store.
	/// </para>
	/// </remarks>
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

		/// <inheritdoc/>
		public string Name
		{
			get { return "Pack and Sign MSHC files"; }
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
				return "This plug-in will pack MSHC files into a CAB file and code-sign the CAB file." +
					"The purpose is to create a deployment package that the Help Viewer will trust.";
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
                        new ExecutionPoint(BuildStep.CompilingHelpFile, ExecutionBehaviors.After)
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
			PackAndSignMHSCConfig lConfigDialog = new PackAndSignMHSCConfig (project);
			lConfigDialog.Configuration = currentConfig;
			if (lConfigDialog.ShowDialog ().GetValueOrDefault ())
			{
				return lConfigDialog.Configuration;
			}
			return currentConfig;
		}

		/// <inheritdoc/>
		public void Initialize (BuildProcess buildProcess, XPathNavigator configuration)
		{
			mBuildProcess = buildProcess;
			mConfiguration = configuration.OuterXml;
		}

		/// <inheritdoc/>
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
							lSignToolArgs = String.Format ("sign {0} {1} \"{2}\"", lSettings.CertificateSpec, lSignToolStamp, lTargetPath);
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

		/// <summary>
		/// Verifies that all required settings are present and valid.
		/// </summary>
		/// <param name="Settings">The configuration settings for this plugin.</param>
		/// <param name="DdfPath">A path to the makecab directives template that must accompany this assembly.</param>
		/// <returns>True if the settings are all valid.</returns>
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

		/// <summary>
		/// Updates the &lt;helpfilename&gt;.msha and HelpContentSetup.msha files to install the Cabinet file
		/// instead of the MSHC file.
		/// </summary>
		private void FixMSHAFiles ()
		{
			FixMSHAFile (Path.Combine (mBuildProcess.OutputFolder, "HelpContentSetup.msha"));
			FixMSHAFile (Path.Combine (mBuildProcess.OutputFolder, mBuildProcess.CurrentProject.HtmlHelpName + ".msha"));
			FixMSHAFile (Path.Combine (mBuildProcess.WorkingFolder, mBuildProcess.CurrentProject.HtmlHelpName + ".msha"));
		}

		/// <summary>
		/// Updates an MSHA file to install the Cabinet file instead of the MSHC file.
		/// </summary>
		/// <param name="FilePath">The full path of the MSHA file to update.</param>
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

		/// <inheritdoc/>
		~PackAndSignMSHC ()
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
