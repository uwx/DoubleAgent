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
	/// The resulting .CAB file will be recognized by "Manage Help settings" as signed content.
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

		private ExecutionPointCollection m_executionPoints;
		private BuildProcess m_buildProcess;
		private String m_configuration;

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
				if (m_executionPoints == null)
				{
					m_executionPoints = new ExecutionPointCollection
                    {
                        new ExecutionPoint(BuildStep.CompilingHelpFile, ExecutionBehaviors.After)
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
			PackAndSignMHSCConfig v_configDialog = new PackAndSignMHSCConfig (project);
			v_configDialog.Configuration = currentConfig;
			if (v_configDialog.ShowDialog ().GetValueOrDefault ())
			{
				return v_configDialog.Configuration;
			}
			return currentConfig;
		}

		/// <inheritdoc/>
		public void Initialize (BuildProcess buildProcess, XPathNavigator configuration)
		{
			m_buildProcess = buildProcess;
			m_configuration = configuration.OuterXml;
		}

		/// <inheritdoc/>
		public void Execute (ExecutionContext context)
		{
			if (m_buildProcess.CurrentFormat == HelpFileFormat.MSHelpViewer)
			{
				PackAndSignMSHCSettings v_settings = new PackAndSignMSHCSettings (m_buildProcess.CurrentProject, m_configuration);
				String v_ddfPath = null;

				m_buildProcess.ReportProgress ("{0} Version {1}\r\n{2}", this.Name, this.Version, this.Copyright);

				if (CanExecute (v_settings, out v_ddfPath))
				{
					FilePath v_makeCabPath = new FilePath (v_settings.MakeCabPath, m_buildProcess.CurrentProject);
					FilePath v_signToolPath = new FilePath (v_settings.SignToolPath, m_buildProcess.CurrentProject);
					String v_sourcePath = Path.Combine (m_buildProcess.OutputFolder, Path.ChangeExtension (m_buildProcess.CurrentProject.HtmlHelpName, ".mshc"));
					String v_targetPath = Path.Combine (v_settings.CabPathName, v_settings.CabFileName);
					String v_makeCabArgs;
					String v_signToolArgs = null;
					String v_signToolStamp = String.Empty;

					v_makeCabArgs = String.Format ("/V1 /F \"{0}\" /D SourcePath='{1}' /D TargetDir='{2}' /D TargetFile='{3}'", v_ddfPath, v_sourcePath, v_settings.CabPathName, v_settings.CabFileName);
					m_buildProcess.RunProcess (v_makeCabPath, v_makeCabArgs);

					if (File.Exists (v_targetPath))
					{
						m_buildProcess.HelpViewerFiles.Add (v_targetPath);

						if (!String.IsNullOrEmpty (v_settings.TimeStampUrl))
						{
							v_signToolStamp = String.Format ("{0} {1} ", PackAndSignMSHCSettings.StoreTimestamperFlag, v_settings.TimeStampUrl);
						}
						if (v_settings.IsFileCertificate)
						{
							v_signToolArgs = String.Format ("sign /f \"{0}\" {1} \"{2}\"", v_settings.FileCertificatePath, v_signToolStamp, v_targetPath);
						}
						else if (v_settings.IsStoreCertificate)
						{
							v_signToolArgs = String.Format ("sign {0} {1} \"{2}\"", v_settings.CertificateSpec, v_signToolStamp, v_targetPath);
						}
						else
						{
							m_buildProcess.ReportWarning (Name, "No code-signing certificate selected");
						}
						if (!String.IsNullOrEmpty (v_signToolArgs))
						{
							m_buildProcess.RunProcess (v_signToolPath, v_signToolArgs);
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
		/// <param name="settings">The configuration settings for this plugin.</param>
		/// <param name="ddfPath">A path to the makecab directives template that must accompany this assembly.</param>
		/// <returns>True if the settings are all valid.</returns>
		private bool CanExecute (PackAndSignMSHCSettings settings, out String ddfPath)
		{
			FilePath v_makeCabPath = new FilePath (settings.MakeCabPath,m_buildProcess.CurrentProject);
			FilePath v_signToolPath = new FilePath (settings.SignToolPath, m_buildProcess.CurrentProject);

			ddfPath = Path.ChangeExtension (Assembly.GetExecutingAssembly ().Location, ".ddf");

			if (m_buildProcess.IsPartialBuild)
			{
				m_buildProcess.ReportWarning (Name, "Skipped for partial build");
			}
			else if (!File.Exists (ddfPath))
			{
				m_buildProcess.ReportWarning (Name, "Missing directives file \"{0}\"", ddfPath);
			}
			else if (String.IsNullOrEmpty (settings.CabFileName))
			{
				m_buildProcess.ReportWarning (Name, "Blank Cabinet file name");
			}
			else if (String.IsNullOrEmpty (settings.CabPathName))
			{
				m_buildProcess.ReportWarning (Name, "Blank Cabinet output path");
			}
			else if (String.IsNullOrEmpty (settings.MakeCabPath))
			{
				m_buildProcess.ReportWarning (Name, "No path specified for makecab.exe");
			}
			else if (String.IsNullOrEmpty (settings.SignToolPath))
			{
				m_buildProcess.ReportWarning (Name, "No path specified for signtool.exe");
			}
			else if (!v_makeCabPath.Exists)
			{
				m_buildProcess.ReportWarning (Name, "Invalid path for makecab.exe - \"{0}\"", settings.MakeCabPath);
			}
			else if (!v_signToolPath.Exists)
			{
				m_buildProcess.ReportWarning (Name, "Invalid path for signtool.exe - \"{0}\"", settings.SignToolPath);
			}
			else if (String.IsNullOrEmpty (settings.TimeStampUrl))
			{
				m_buildProcess.ReportWarning (Name, "No timestamer specified - the Cabinet file signature may not be trusted");
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
			FixMSHAFile (Path.Combine (m_buildProcess.OutputFolder, "HelpContentSetup.msha"));
			FixMSHAFile (Path.Combine (m_buildProcess.OutputFolder, m_buildProcess.CurrentProject.HtmlHelpName + ".msha"));
			FixMSHAFile (Path.Combine (m_buildProcess.WorkingFolder, m_buildProcess.CurrentProject.HtmlHelpName + ".msha"));
		}

		/// <summary>
		/// Updates an MSHA file to install the Cabinet file instead of the MSHC file.
		/// </summary>
		/// <param name="filePath">The full path of the MSHA file to update.</param>
		private void FixMSHAFile (String filePath)
		{
			try
			{
				String [] v_inputLines = File.ReadAllLines (filePath);
				List<String> v_outputLines = new List<String> ();

				foreach (String lLine in v_inputLines)
				{
					v_outputLines.Add (lLine.Replace (".mshc", ".cab").Replace (".MSHC", ".CAB"));
				}
				File.WriteAllLines (filePath, v_outputLines.ToArray());
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
