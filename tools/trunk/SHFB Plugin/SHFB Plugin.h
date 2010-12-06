#pragma once

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;
using namespace System::Reflection;
using namespace System::Windows::Forms;
using namespace System::Xml;
using namespace System::Xml::XPath;

using namespace SandcastleBuilder::Utils;
using namespace SandcastleBuilder::Utils::BuildEngine;
using namespace SandcastleBuilder::Utils::PlugIn;
using namespace SandcastleBuilder::Utils::ConceptualContent;

namespace DoubleAgent {
namespace SHFBPlugin {
/////////////////////////////////////////////////////////////////////////////

public ref class TocNamespaceGrouping : public SandcastleBuilder::Utils::PlugIn::IPlugIn
{
public:
	TocNamespaceGrouping ();
	virtual ~TocNamespaceGrouping ();

#pragma region IPlugIn Implementation
public:
	property String^ Name
	{
		virtual String^ get ()
		{
			return gcnew String("Table Of Contents Type Grouping");
		}
	}
	property String^ Description
	{
		virtual String^ get ()
		{
			return gcnew String("Generates an extra TOC level within namespaces to categorize types");
		}
	}
	property String^ Copyright
	{
		virtual String^ get ()
		{
			AssemblyCopyrightAttribute^ lCopyright = (AssemblyCopyrightAttribute^)Attribute::GetCustomAttribute (Assembly::GetExecutingAssembly (), AssemblyCopyrightAttribute::typeid);
			return lCopyright->Copyright;
		}
	}
	property System::Version^ Version
	{
		virtual System::Version^ get ()
		{
			AssemblyFileVersionAttribute^ lVersion = (AssemblyFileVersionAttribute^)Attribute::GetCustomAttribute (Assembly::GetExecutingAssembly (), AssemblyFileVersionAttribute::typeid);
			return gcnew System::Version (lVersion->Version);
		}
	}
	property Boolean RunsInPartialBuild
	{
		virtual Boolean get ()
		{
			return false;
		}
	}

	property ExecutionPointCollection^ ExecutionPoints
	{
		virtual ExecutionPointCollection^ get ()
		{
			mExecutionPoints = gcnew ExecutionPointCollection ();
			mExecutionPoints->Add (gcnew ExecutionPoint (BuildStep::GenerateIntermediateTableOfContents, ExecutionBehaviors::Before));
			mExecutionPoints->Add (gcnew ExecutionPoint (BuildStep::GenerateHelpFormatTableOfContents, ExecutionBehaviors::Before));
			mExecutionPoints->Add (gcnew ExecutionPoint (BuildStep::CombiningIntermediateTocFiles, ExecutionBehaviors::BeforeAndAfter));
			return mExecutionPoints;
		}
	}
	virtual String^ ConfigurePlugIn (SandcastleProject^ pProject, String^ pCurrentConfig)
	{
		MessageBox::Show ("This plug-in has no configurable settings", "Build Process Plug-In", MessageBoxButtons::OK, MessageBoxIcon::Information);
		return pCurrentConfig;
	}

	virtual void Initialize (BuildProcess^ pBuildProcess, XPathNavigator^ pConfiguration);
	virtual void Execute (ExecutionContext^ pContext);
#pragma endregion

#pragma region Private Methods
private:
	property bool HasSiteMap {bool get ();}
	property FileItem^ SiteMapFile {FileItem^ get ();}
	property String^ SiteMapPath {String^ get ();}

	void UpdateGenerateIntermediateProject ();
	void UpdateGenerateHelpFormatProject ();
	void RetargetSiteMapToc ();
	void ReparentApiTopics ();

	void TargetTocEntries (TocEntryCollection^ pEntries);
	void TargetTocEntry (TocEntry^ pEntry);
	void WriteTocEntries (TocEntryCollection^ pEntries, System::IO::StreamWriter^ pWriter);
	void WriteTocEntry (TocEntry^ pEntry, System::IO::StreamWriter^ pWriter);

	static void LogXmlFile (String^ pFilePath);
	static void LogTocEntries (TocEntryCollection^ pEntries);
	static void LogTocEntries (TocEntryCollection^ pEntries, String^ pIndent);
	static void LogTocEntry (TocEntry^ pEntry);
	static void LogTocEntry (TocEntry^ pEntry, String^ pIndent);
#pragma endregion

#pragma region Private Fields
private:
	ExecutionPointCollection^	mExecutionPoints;
	BuildProcess^				mBuildProcess;
#pragma endregion
};

/////////////////////////////////////////////////////////////////////////////
} // namespace SHFBPlugin
} // namespace DoubleAgent
