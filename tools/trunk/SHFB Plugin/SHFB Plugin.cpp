#include "stdafx.h"
#include "SHFB Plugin.h"

/////////////////////////////////////////////////////////////////////////////
#pragma managed(push,off)
#ifdef	_DEBUG
#define _LOG_LEVEL_DEBUG	LogNormal
#endif
#define	_LOG_ROOT_PATH		_T("Software\\Cinnamon\\")
#define	_LOG_SECTION_NAME	_T("SHFB")
#define _LOG_DEF_LOGNAME	_LOG_SECTION_NAME _T(".log")
#include "LogAccess.inl"
#include "Log.inl"
#pragma comment(lib, "advapi32.lib")
#pragma managed(pop)
/////////////////////////////////////////////////////////////////////////////

namespace DoubleAgent {
namespace SHFBPlugin {
/////////////////////////////////////////////////////////////////////////////

TocNamespaceGrouping::TocNamespaceGrouping ()
{
	LogStart (true);
#ifdef	_DEBUG
	LogMessage (LogIfActive, _T("TocNamespaceGrouping::TocNamespaceGrouping"));
#endif	
}

TocNamespaceGrouping::~TocNamespaceGrouping ()
{
#ifdef	_DEBUG
	LogMessage (LogIfActive, _T("TocNamespaceGrouping::~TocNamespaceGrouping"));
#endif	
	LogStop (LogIfActive);
}

/////////////////////////////////////////////////////////////////////////////

void TocNamespaceGrouping::Initialize (BuildProcess^ pBuildProcess, XPathNavigator^ pConfiguration)
{
	mBuildProcess = pBuildProcess;
	mBuildProcess->ReportProgress ("{0} Version {1} {2}\r\n", this->Name, this->Version, this->Copyright);

#ifdef	_DEBUG
	LogMessage (LogIfActive, _T("RootContentContainerId [%s]"), _B(mBuildProcess->RootContentContainerId));
	LogMessage (LogIfActive, _T("ApiTocParentId         [%s]"), _B(mBuildProcess->ApiTocParentId));
	LogMessage (LogIfActive, _T("TocParentId            [%s]"), _B(mBuildProcess->CurrentProject->TocParentId));
#endif	
}

void TocNamespaceGrouping::Execute (ExecutionContext^ pContext)
{
#ifdef	_DEBUG
	LogMessage (LogIfActive, _T(""));
	LogMessage (LogIfActive, _T("%s %s"), _B(pContext->Behavior.ToString()), _B(pContext->BuildStep.ToString()));
	LogMessage (LogIfActive, _T(""));
#endif	

	if	(
			(pContext->BuildStep == BuildStep::GenerateIntermediateTableOfContents)
		&&	(pContext->Behavior == ExecutionBehaviors::Before)
		)
	{
		UpdateGenerateIntermediateProject ();
	}
	else
	if	(
			(pContext->BuildStep == BuildStep::GenerateHelpFormatTableOfContents)
		&&	(pContext->Behavior == ExecutionBehaviors::Before)
		)
	{
		UpdateGenerateHelpFormatProject ();
	}
	else
	if	(
			(pContext->BuildStep == BuildStep::CombiningIntermediateTocFiles)
		&&	(pContext->Behavior == ExecutionBehaviors::Before)
		)
	{
		RetargetSiteMapToc ();
	}
	else
	if	(
			(pContext->BuildStep == BuildStep::CombiningIntermediateTocFiles)
		&&	(pContext->Behavior == ExecutionBehaviors::After)
		)
	{
#ifdef	_DEBUG
		LogXmlFile (System::IO::Path::Combine (mBuildProcess->WorkingFolder, "toc.xml"));
#endif	
		ReparentApiTopics ();
	}

#ifdef	_DEBUG
	if	(
			(pContext->BuildStep == BuildStep::CombiningIntermediateTocFiles)
		&&	(pContext->Behavior == ExecutionBehaviors::After)
		)
	{
		LogXmlFile (System::IO::Path::Combine (mBuildProcess->WorkingFolder, "_ConceptualTOC_.xml"));
		LogXmlFile (System::IO::Path::Combine (mBuildProcess->WorkingFolder, "toc.xml"));
	}
#endif	
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void TocNamespaceGrouping::UpdateGenerateIntermediateProject ()
{
	try
	{
		String^			lProjectPath = System::IO::Path::Combine (mBuildProcess->WorkingFolder, "GenerateIntermediateTOC.proj");
		XmlDocument^	lDocument = gcnew XmlDocument();
		XmlNodeList^	lNodes;
		XmlNode^		lNode;
		XmlAttribute^	lAttribute;
		String^			lTransform = gcnew String("C:\\ProgramData\\EWSoftware\\Sandcastle Help File Builder\\Components and Plug-Ins\\CreateVSToc_Override.xsl");
	
#ifdef	_DEBUG
		LogMessage (LogIfActive, _T("  Load      '%s'"), _B(lProjectPath));
#endif		
		lDocument->Load (lProjectPath);
		if (lNodes = lDocument->DocumentElement->SelectNodes ("//*[@Transformations!='']"))
		{
			for each (lNode in lNodes)
			{
				if	(
						(lAttribute = lNode->Attributes["Transformations"])
					&&	(lAttribute->Value->Contains ("CreateVSToc"))
					)
				{
#ifdef	_DEBUG
					LogMessage (LogIfActive, _T("  Update    '%s'"), _B(lProjectPath));
					LogMessage (LogIfActive, _T("    replace '%s'"), _B(lAttribute->Value));
					LogMessage (LogIfActive, _T("    with    '%s'"), _B(lTransform));
#endif	
					mBuildProcess->ReportProgress ("{0}: Update    '{1}'", this->Name, lProjectPath);
					mBuildProcess->ReportProgress ("{0}:   replace '{1}'", this->Name, lAttribute->Value);
					mBuildProcess->ReportProgress ("{0}:   with    '{1}", this->Name, lTransform);

					lAttribute->Value = lTransform;
					lDocument->Save (lProjectPath);
					break;
				}
			}
		}
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////

void TocNamespaceGrouping::UpdateGenerateHelpFormatProject ()
{
	try
	{
		String^			lProjectPath = System::IO::Path::Combine (mBuildProcess->WorkingFolder, "Generate2xTOC.proj");
		XmlDocument^	lDocument = gcnew XmlDocument();
		XmlNodeList^	lNodes;
		XmlNode^		lNode;
		XmlAttribute^	lAttribute;
		String^			lTransform = gcnew String("C:\\ProgramData\\EWSoftware\\Sandcastle Help File Builder\\Components and Plug-Ins\\TocToHxsContents_Override.xsl");
	
#ifdef	_DEBUG
		LogMessage (LogIfActive, _T("  Load      '%s'"), _B(lProjectPath));
#endif		
		lDocument->Load (lProjectPath);
		if (lNodes = lDocument->DocumentElement->SelectNodes ("//*[@Transformations!='']"))
		{
			for each (lNode in lNodes)
			{
				if	(
						(lAttribute = lNode->Attributes["Transformations"])
					&&	(lAttribute->Value->Contains ("TocToHxSContents"))
					)
				{
#ifdef	_DEBUG
					LogMessage (LogIfActive, _T("  Update    '%s'"), _B(lProjectPath));
					LogMessage (LogIfActive, _T("    replace '%s'"), _B(lAttribute->Value));
					LogMessage (LogIfActive, _T("    with    '%s'"), _B(lTransform));
					LogMessage (LogIfActive, _T("    SiteMap '%s'"), _B(SiteMapPath));
#endif	
					mBuildProcess->ReportProgress ("{0}: Update    '{1}'", this->Name, lProjectPath);
					mBuildProcess->ReportProgress ("{0}:   replace '{1}'", this->Name, lAttribute->Value);
					mBuildProcess->ReportProgress ("{0}:   with    '{1}", this->Name, lTransform);
					mBuildProcess->ReportProgress ("{0}:   SiteMap '{1}", this->Name, SiteMapPath);

					lAttribute->Value = lTransform;
					
					lAttribute = lDocument->CreateAttribute ("Arguments");
					lAttribute->Value = "SiteMap=file://" + SiteMapPath;
					lNode->Attributes->Append (lAttribute);

					lDocument->Save (lProjectPath);
					break;
				}
			}
		}
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void TocNamespaceGrouping::RetargetSiteMapToc ()
{
	try
	{
        FileItem^			lSiteMapFile;
        TocEntryCollection^	lSiteMapToc;

        if	(lSiteMapFile = this->SiteMapFile)
        {
#ifdef	_DEBUG            
			LogMessage (LogNormal, _T("Load [%s]"), _B(lSiteMapFile->FullPath));
#endif				
			mBuildProcess->ReportProgress ("{0}: RetargetSiteMapToc '{1}'", this->Name, lSiteMapFile->FullPath);
            lSiteMapToc = gcnew TocEntryCollection(lSiteMapFile);
            lSiteMapToc->Load();

			TargetTocEntries (lSiteMapToc);
#ifdef	_DEBUG_NOT            
			LogTocEntries (lSiteMapToc);
#endif
#ifdef	_DEBUG_NOT
			LogMessage (LogNormal, _T(""));
			LogMessage (LogNormal, _T("%s"), _B(lSiteMap->ToString(HelpFileFormat::MSHelp2)));
			LogMessage (LogNormal, _T(""));
#endif
			//
			//	Replacement of TocEntryCollection::SaveToIntermediateTocFile which only works for MAML 
			//
			try
			{
				String^						lTocFilePath = System::IO::Path::Combine (mBuildProcess->WorkingFolder, "_ConceptualTOC_.xml");
				System::IO::StreamWriter^	lWriter = gcnew System::IO::StreamWriter (lTocFilePath);

#ifdef	_DEBUG            
				LogMessage (LogNormal, _T("Save [%s]"), _B(lTocFilePath));
#endif				
                lWriter->WriteLine("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
                lWriter->WriteLine("<topics>");

                WriteTocEntries (lSiteMapToc, lWriter);

                lWriter->WriteLine("</topics>");
                lWriter->Close ();
			}
			catch AnyExceptionDebug
        }
	}
	catch AnyExceptionDebug
}

void TocNamespaceGrouping::WriteTocEntries (TocEntryCollection^ pEntries, System::IO::StreamWriter^ pWriter)
{
	TocEntry^	lEntry;
	for each (lEntry in pEntries)
	{
		WriteTocEntry (lEntry, pWriter);
	}
}

void TocNamespaceGrouping::WriteTocEntry (TocEntry^ pEntry, System::IO::StreamWriter^ pWriter)
{
    pWriter->Write("<topic");
    
	if	(!String::IsNullOrEmpty (pEntry->Title))
	{
		pWriter->Write(" id=\"");
		pWriter->Write(pEntry->Title);
		pWriter->Write("\"");
	}

    if	(!String::IsNullOrEmpty (pEntry->DestinationFile))
    {
	    pWriter->Write(" file=\"");
	    pWriter->Write(pEntry->DestinationFile);
	    pWriter->Write("\"");
    }
    else
	if	(!String::IsNullOrEmpty (pEntry->Title))
    {
		pWriter->Write(" title=\"");
		pWriter->Write(pEntry->Title);
		pWriter->Write("\"");
    }

	if	(
			(pEntry->Children)
		&&	(pEntry->Children->Count > 0)
		)
	{
	    pWriter->WriteLine(">");
		WriteTocEntries (pEntry->Children, pWriter);
	    pWriter->WriteLine("</topic>");
	}
	else
	{
	    pWriter->WriteLine("/>");
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void TocNamespaceGrouping::ReparentApiTopics ()
{
	try
	{
		String^					lTocFilePath = System::IO::Path::Combine (mBuildProcess->WorkingFolder, "toc.xml");
		XmlDocument^			lDocument = gcnew XmlDocument();
		XPathNavigator^			lNavigator = nullptr;
		XPathNodeIterator^		lNodes = nullptr;
		bool					lChanged = false;

#ifdef	_DEBUG
		LogMessage (LogNormal, _T("ReparentApiTopics %s"), _B(lTocFilePath));
#endif	
		mBuildProcess->ReportProgress ("{0}: ReparentApiTopics '{1}'", this->Name, lTocFilePath);
		lDocument->Load (lTocFilePath);
		
		lNavigator = lDocument->CreateNavigator ();
		if	(
				(lNavigator != nullptr)
			&&	(lNodes = lNavigator->Select ("//topic[@title=@id and @title!='' and not(@file)]"))
			&&	(lNodes->Count > 0)
			)
		{
			array<XPathNavigator^>^	lTargetNodes = gcnew array<XPathNavigator^> (lNodes->Count);
			XPathNavigator^			lTargetNode;
			String^					lTargetId;

#ifdef	_DEBUG
			LogMessage (LogNormal, _T("Targets [%d]"), lNodes->Count);
#endif
			while (lNodes->MoveNext())
			{
				lTargetNodes [lNodes->CurrentPosition-1] = lNodes->Current->Clone();
			}
			for each (lTargetNode in lTargetNodes)
			{
				lTargetId = lTargetNode->GetAttribute ("id", String::Empty);
#ifdef	_DEBUG
				LogMessage (LogNormal, _T("  Target [%s]"), _B(lTargetId));
#endif
				if	(
						(lTargetId->StartsWith ("N:"))
					&&	(lNodes = lNavigator->Select ("//topic[@id='" + lTargetId + "' and not(@title) and @file]"))
					&&	(lNodes->Count == 1)
					&&	(lNodes->MoveNext ())
					)
				{
#ifdef	_DEBUG
					LogMessage (LogNormal, _T("  Source [%s] [%s]"), _B(lNodes->Current->GetAttribute ("id", String::Empty)), _B(lNodes->Current->GetAttribute ("file", String::Empty)));
#endif
					mBuildProcess->ReportProgress ("{0}:   Reparent id='{1}' file='{2}'", this->Name, lTargetId, lNodes->Current->GetAttribute ("file", String::Empty));

					try
					{
						lTargetNode->ReplaceSelf (lNodes->Current);
						lNodes->Current->DeleteSelf ();
						lChanged = true;
					}
					catch AnyExceptionDebug
				}
			}
			
			if	(lChanged)
			{
				lDocument->Save (lTocFilePath);
			}
		}
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool TocNamespaceGrouping::HasSiteMap::get ()
{
	return mBuildProcess->CurrentProject->HasItems (BuildAction::SiteMap);
}

FileItem^ TocNamespaceGrouping::SiteMapFile::get ()
{
	FileItem^	lFileItem;

	for each (lFileItem in mBuildProcess->CurrentProject->FileItems)
	{
		if (lFileItem->BuildAction == BuildAction::SiteMap)
		{
			return lFileItem;
		}
	}
	return nullptr;
}

String^ TocNamespaceGrouping::SiteMapPath::get ()
{
	FileItem^	lFileItem;

	for each (lFileItem in mBuildProcess->CurrentProject->FileItems)
	{
		if (lFileItem->BuildAction == BuildAction::SiteMap)
		{
			return lFileItem->FullPath;
		}
	}
	return String::Empty;
}

/////////////////////////////////////////////////////////////////////////////

void TocNamespaceGrouping::TargetTocEntries (TocEntryCollection^ pEntries)
{
	TocEntry^	lEntry;
	for each (lEntry in pEntries)
	{
		TargetTocEntry (lEntry);
	}
}

void TocNamespaceGrouping::TargetTocEntry (TocEntry^ pEntry)
{
	if	(pEntry)
	{
		if	(
				(!String::IsNullOrEmpty (pEntry->SourceFile))
			&&	(String::IsNullOrEmpty (pEntry->DestinationFile))
			)
		{
			pEntry->DestinationFile = pEntry->SourceFile->AbsoluteToRelativePath (pEntry->SourceFile->BasePath, pEntry->SourceFile->ExpandedPath);
		}
		if	(pEntry->Children)
		{
			TargetTocEntries (pEntry->Children);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void TocNamespaceGrouping::LogXmlFile (String^ pFilePath)
{
	try
	{
		XmlReaderSettings^	lReaderSettings = gcnew XmlReaderSettings;
		XmlReader^			lReader;
		XmlDocument^		lDocument = gcnew XmlDocument();

		lReaderSettings->ValidationType = ValidationType::None;
		lReaderSettings->ValidationFlags = Schema::XmlSchemaValidationFlags::None;
		lReaderSettings->ConformanceLevel = ConformanceLevel::Document;
		lReaderSettings->IgnoreComments = true;
		lReaderSettings->IgnoreWhitespace = false;
		lReaderSettings->IgnoreProcessingInstructions = true;
		lReaderSettings->ProhibitDtd = false;
		lReaderSettings->CloseInput = true;

		lReader = XmlReader::Create (pFilePath, lReaderSettings);
		
		try
		{
			LogMessage (LogNormal, _T("%s"), _B(pFilePath));
			lDocument->Load (lReader);
			LogMessage (LogNormal, _T("%s"), _B(lDocument->DocumentElement->OuterXml));
		}
		catch AnyExceptionDebug
		
		lReader->Close ();
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////

void TocNamespaceGrouping::LogTocEntry (TocEntry^ pEntry, String^ pIndent)
{
	LogMessage (LogNormal, _T("%sEntry '%s' Source [%s] Target [%s] [Include %s Sort %d Parse %s %s %s Default %s]"), _B(pIndent), _B(pEntry->Title), _B(pEntry->SourceFile->Path), _B(pEntry->DestinationFile), _B(pEntry->IncludePage.ToString()), (int)pEntry->SortOrder, _B(pEntry->HasProjectTags.ToString()), _B(pEntry->HasLinks.ToString()), _B(pEntry->HasCodeBlocks.ToString()), _B(pEntry->IsDefaultTopic.ToString()));
	if (pEntry->Children != nullptr)
	{
		LogTocEntries (pEntry->Children, pIndent + "  ");
	}
}

void TocNamespaceGrouping::LogTocEntry (TocEntry^ pEntry)
{
	LogTocEntry (pEntry, "  ");
}

void TocNamespaceGrouping::LogTocEntries (TocEntryCollection^ pEntries, String^ pIndent)
{
	TocEntry^	lEntry;
	for each (lEntry in pEntries)
	{
		LogTocEntry (lEntry, pIndent);
	}
}
 
void TocNamespaceGrouping::LogTocEntries (TocEntryCollection^ pEntries)
{
	LogTocEntries (pEntries, "  ");
}

/////////////////////////////////////////////////////////////////////////////
} // namespace SHFBPlugin
} // namespace DoubleAgent
