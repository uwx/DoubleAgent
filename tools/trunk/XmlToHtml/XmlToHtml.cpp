#include "stdafx.h"
#include "XmlToHtml.h"
#include "XmlFormatWriter.h"
#include "DaVersion.h"

using namespace System::Collections;
using namespace System::Collections::Specialized;
using namespace System::Xml;
using namespace System::Reflection;
using namespace System::Runtime::InteropServices;
using namespace System::Runtime::InteropServices::ComTypes;
using namespace DoubleAgent;
using namespace DoubleAgent::XmlToHtml;

/////////////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG
//#define	_DEBUG_XML_MEMBERS			LogNormal|LogHighVolume
//#define	_DEBUG_ASSEMBLY_MEMBERS		LogNormal|LogHighVolume
//#define	_DEBUG_MEMBER_FILES			LogNormal
//#define	_DEBUG_INCLUDES				LogNormal
//#define	_DEBUG_ENTITIES				LogNormal|LogHighVolume
#endif

#define	_SYNTAX_TABLE

/////////////////////////////////////////////////////////////////////////////
#pragma managed(push,off)
#ifdef	_DEBUG
#define _LOG_LEVEL_DEBUG	LogNormal
#endif
#define	_LOG_ROOT_PATH		_T("Software\\Cinnamon\\")
#define	_LOG_SECTION_NAME	_T("XmlToHtml")
#define _LOG_DEF_LOGNAME	_LOG_SECTION_NAME _T(".log")
#include "LogAccess.inl"
#include "Log.inl"
#pragma comment(lib, "advapi32.lib")
#pragma managed(pop)
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

int main(array<System::String ^> ^args)
{
	bool								lRestartLog = true;
	DoubleAgent::XmlToHtml::XmlToHtml^	lXmlToHtml;

#ifdef	_DEBUG
	LogStart (true);
	lRestartLog = false;
#endif

	lXmlToHtml = gcnew DoubleAgent::XmlToHtml::XmlToHtml (lRestartLog);
	lXmlToHtml->ProcessCmdLine (Environment::GetCommandLineArgs ());

	LogStop (LogIfActive);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
namespace DoubleAgent {
namespace XmlToHtml {
/////////////////////////////////////////////////////////////////////////////

int XmlToHtml::ProcessCmdLine (array <String^>^ pCmdArgs)
{
	int	lRet = 0;

	try
	{
		int					lCmdArgNdx;
		String^				lTypeLibPath;
		String^				lAssemblyPath;
		String^				lXmlPath;
		ComTypes::ITypeLib^	lTypeLib = nullptr;
		Assembly^			lAssembly = nullptr;
		XmlDocument^		lXmlDocument = nullptr;

		mHtmlPath = nullptr;
		mOutputPath = nullptr;
		mOutputHtml = false;
		mOutputIntellisense = false;
		mOutputSandcastle = false;
		mFormatServer = false;

		mSourceTypes = gcnew Generic::SortedDictionary <String^, System::Type^>;
		mSourceMethods = gcnew Generic::SortedDictionary <String^, System::Reflection::MethodInfo^>;
		mSourceConstructors = gcnew Generic::SortedDictionary <String^, System::Reflection::ConstructorInfo^>;
		mSourceProperties = gcnew Generic::SortedDictionary <String^, System::Reflection::PropertyInfo^>;
		mSourceEvents = gcnew Generic::SortedDictionary <String^, System::Reflection::EventInfo^>;
		mSourceFields = gcnew Generic::SortedDictionary <String^, System::Reflection::FieldInfo^>;

		for	(lCmdArgNdx = 1; lCmdArgNdx < pCmdArgs->Length; lCmdArgNdx++)
		{
			String^	lCmdOpt = CmdOpt (pCmdArgs, lCmdArgNdx);

			if	(String::Compare (lCmdOpt, "Tlb", true) == 0)
			{
				try
				{
					if	(lTypeLibPath = CmdArg (pCmdArgs, lCmdArgNdx+1))
					{
						lCmdArgNdx++;
					}
					else
					{
						lRet = 8;
						break;
					}
					lTypeLib = safe_cast <ComTypes::ITypeLib^> (LoadTypeLibrary (lTypeLibPath));
				}
				catch AnyExceptionDebug
			}
			else
			if	(
					(String::Compare (lCmdOpt, "Asm", true) == 0)
				||	(String::Compare (lCmdOpt, "Assembly", true) == 0)
				)
			{
				try
				{
					if	(lAssembly)
					{
						String^		lRefAssemblyPath;
						Assembly^	lRefAssembly;

						if	(
								(lRefAssemblyPath = CmdArg (pCmdArgs, lCmdArgNdx+1))
							&&	(lRefAssembly = Assembly::LoadFrom (lRefAssemblyPath))
							)
						{
							LoadAssemblyMembers (lRefAssembly);
						}
					}
					else
					{
						if	(lAssemblyPath = CmdArg (pCmdArgs, lCmdArgNdx+1))
						{
							lCmdArgNdx++;
						}
						else
						{
							lRet = 8;
							break;
						}
						lAssembly = Assembly::LoadFrom (lAssemblyPath);
					}
				}
				catch AnyExceptionDebug
			}
			else
			if	(String::Compare (lCmdOpt, "Xml", true) == 0)
			{
				try
				{
					if	(lXmlPath = CmdArg (pCmdArgs, lCmdArgNdx+1))
					{
						lCmdArgNdx++;
					}
					else
					{
						lRet = 8;
						break;
					}
				}
				catch AnyExceptionDebug
			}
			else
			if	(String::Compare (lCmdOpt, "Html", true) == 0)
			{
				try
				{
					if	(mHtmlPath = CmdArg (pCmdArgs, lCmdArgNdx+1))
					{
						lCmdArgNdx++;
					}
					else
					{
						lRet = 8;
						break;
					}
					mHtmlPath = System::IO::Path::GetFullPath (mHtmlPath);
				}
				catch AnyExceptionDebug
			}
			else
			if	(
					(String::Compare (lCmdOpt, "is", true) == 0)
				||	(String::Compare (lCmdOpt, "intellisense", true) == 0)
				)
			{
				try
				{
					if	(mOutputPath = CmdArg (pCmdArgs, lCmdArgNdx+1))
					{
						lCmdArgNdx++;
					}
					else
					{
						lRet = 8;
						break;
					}
					mOutputPath = System::IO::Path::GetFullPath (mOutputPath);
					mOutputIntellisense = true;
					mOutputSandcastle = false;
				}
				catch AnyExceptionDebug
			}
			else
			if	(
					(String::Compare (lCmdOpt, "sc", true) == 0)
				||	(String::Compare (lCmdOpt, "sandcastle", true) == 0)
				)
			{
				try
				{
					if	(mOutputPath = CmdArg (pCmdArgs, lCmdArgNdx+1))
					{
						lCmdArgNdx++;
					}
					else
					{
						lRet = 8;
						break;
					}
					mOutputPath = System::IO::Path::GetFullPath (mOutputPath);
					mOutputIntellisense = false;
					mOutputSandcastle = true;
				}
				catch AnyExceptionDebug
			}
			else
			if	(String::Compare (lCmdOpt, "Out", true) == 0)
			{
				try
				{
					if	(mOutputPath = CmdArg (pCmdArgs, lCmdArgNdx+1))
					{
						lCmdArgNdx++;
					}
					else
					{
						lRet = 8;
						break;
					}
					mOutputPath = System::IO::Path::GetFullPath (mOutputPath);
					mOutputIntellisense = false;
					mOutputSandcastle = false;
				}
				catch AnyExceptionDebug
			}
			else
			if	(
					(String::Compare (lCmdOpt, "server", true) == 0)
				||	(String::Compare (lCmdOpt, "svr", true) == 0)
				)
			{
				mFormatServer = true;
			}

			if	(
					(lAssembly)
				&&	(lXmlPath)
				&&	(!String::IsNullOrEmpty (mHtmlPath))
				&&	(System::IO::Directory::Exists (mHtmlPath))
				&&	(lXmlDocument = LoadXmlFile (lXmlPath, true))
				)
			{
				try
				{
					LogMessage (LogNormal, _T("Convert [%s] to [%s] for [%s]"), _B(lXmlPath), _B(mHtmlPath), _B(lAssemblyPath));
					Console::WriteLine ("Convert {0} to {1} for {2}", lXmlPath, mHtmlPath, lAssemblyPath);

					mOutputHtml = true;
					mTemplatePath = System::IO::Path::Combine (System::IO::Path::GetDirectoryName (lXmlPath), "Templates");
					ResolveEntities (lXmlDocument);
					if	(!LoadXmlIncludes (lXmlDocument, lXmlPath, true))
					{
						lRet = 4;
						break;
					}
					ConvertXmlToHtml (lXmlDocument, lAssembly);
				}
				catch AnyExceptionDebug

				mHtmlPath = nullptr;
				mOutputHtml = false;
			}

			if	(
					(lAssembly)
				&&	(lXmlPath)
				&&	(!String::IsNullOrEmpty (mOutputPath))
				&&	(lXmlDocument = LoadXmlFile (lXmlPath, true))
				)
			{
				try
				{
					LogMessage (LogNormal, _T("Copy [%s] to [%s] for [%s]"), _B(lXmlPath), _B(mOutputPath), _B(lAssemblyPath));
					Console::WriteLine ("Copy {0} to {1} for {2}", lXmlPath, mOutputPath, lAssemblyPath);

					if	(
							(mOutputIntellisense)
						||	(mOutputSandcastle)
						)
					{
						ResolveEntities (lXmlDocument);
						if	(!LoadXmlIncludes (lXmlDocument, lXmlPath, false))
						{
							lRet = 4;
							break;
						}
						ResolveEntities (lXmlDocument);
					}
					if	(!CopyXmlToXml (lXmlDocument, lAssembly))
					{
						LogMessage (LogNormal, _T("Copy [%s] to [%s] failed"), _B(lXmlPath), _B(mOutputPath));
						Console::WriteLine ("Copy {0} to {1} failed", lXmlPath, mOutputPath);
						lRet = 4;
					}
				}
				catch AnyExceptionDebug

				mOutputPath = nullptr;
				mOutputIntellisense = false;
				mOutputSandcastle = false;
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

String^ XmlToHtml::CmdOpt (array <String^>^ pCmdArgs, int pCmdArgNdx)
{
	if	(
			(pCmdArgNdx < pCmdArgs->Length)
		&&	(
				(pCmdArgs[pCmdArgNdx][0] == '-')
			||	(pCmdArgs[pCmdArgNdx][0] == '/')
			)
		)
	{
		return pCmdArgs[pCmdArgNdx]->Substring (1);
	}
	return nullptr;
}

String^ XmlToHtml::CmdArg (array <String^>^ pCmdArgs, int pCmdArgNdx)
{
	if	(
			(pCmdArgNdx < pCmdArgs->Length)
		&&	(pCmdArgs[pCmdArgNdx][0] != '-')
		&&	(pCmdArgs[pCmdArgNdx][0] != '/')
		)
	{
		return pCmdArgs[pCmdArgNdx];
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

Generic::List<XmlNode^>^ XmlToHtml::NodeList (XmlNodeList^ pNodeList)
{
	Generic::List<XmlNode^>^	lNodeList;
	XmlNode^					lNode;

	if	(
			(pNodeList)
		&&	(pNodeList->Count > 0)
		&&	(lNodeList = gcnew Generic::List<XmlNode^>)
		)
	{
		for each (lNode in pNodeList)
		{
			lNodeList->Add (lNode);
		}
	}
	return lNodeList;
}

/////////////////////////////////////////////////////////////////////////////

void XmlToHtml::CopyNodeAttributes (XmlNode^ pSrcNode, XmlNode^ pTrgNode)
{
	if	(
			(pSrcNode)
		&&	(pTrgNode)
		&&	(pSrcNode->Attributes)
		&&	(pTrgNode->Attributes)
		)
	{
		try
		{
			XmlAttribute^	lSrcAttribute;
			XmlAttribute^	lTrgAttribute;

			for each (lSrcAttribute in pSrcNode->Attributes)
			{
				if	(
						(lSrcAttribute->Name == "xmlns")
					&&	(String::IsNullOrEmpty (lSrcAttribute->Value))
					)
				{
					continue;
				}
				if	(lTrgAttribute = pTrgNode->OwnerDocument->CreateAttribute (lSrcAttribute->Name, lSrcAttribute->NamespaceURI))
				{
					lTrgAttribute->Value = lSrcAttribute->Value;
					pTrgNode->Attributes->Append (lTrgAttribute);
				}
			}
		}
		catch AnyExceptionDebug
	}
}

void XmlToHtml::CopyNodeChildren (XmlNode^ pSrcNode, XmlNode^ pTrgNode, XmlSpace pXmlSpace)
{
	if	(
			(pSrcNode)
		&&	(pTrgNode)
		)
	{
		try
		{
			XmlNode^	lSrcNode;
			XmlNode^	lTrgNode;

			if	(
					(pXmlSpace != XmlSpace::Preserve)
				&&	(mOutputHtml)
				&&	(pTrgNode->Name != "code")
				&&	(pTrgNode->Name != "pre")
				)
			{
				XmlAttribute^	lSrcAttribute;

				if	(
						(pSrcNode->Attributes)
					&&	(lSrcAttribute = pSrcNode->Attributes ["xml:space"])
					&&	(lSrcAttribute->Value == "preserve")
					)
				{
					pXmlSpace = XmlSpace::Preserve;
				}
			}

			if	(pSrcNode->NodeType == XmlNodeType::EntityReference)
			{
				XmlDocumentType^	lDocType;
				XmlNamedNodeMap^	lEntities;
				XmlNode^			lEntity = nullptr;

				if	(
						(lDocType = pTrgNode->OwnerDocument->DocumentType)
					&&	(lEntities = lDocType->Entities)
					)
				{
					lEntity = lEntities->GetNamedItem (pSrcNode->Name);
				}
				if	(
						(!lEntity)
					&&	(lDocType = pSrcNode->OwnerDocument->DocumentType)
					&&	(lEntities = lDocType->Entities)
					)
				{
					lEntity = lEntities->GetNamedItem (pSrcNode->Name);
				}

				if	(lEntity)
				{
					try
					{
						if	(lTrgNode = pTrgNode->OwnerDocument->CreateNode (XmlNodeType::DocumentFragment, nullptr, nullptr))
						{
							lTrgNode->InnerXml = lEntity->InnerText;
#ifdef	_DEBUG_ENTITIES
							LogMessage (_DEBUG_ENTITIES, _T("Entity [%s] to [%s] (Xml)"), _B(pSrcNode->Name), _B(lTrgNode->InnerXml));
#endif
							pTrgNode->ParentNode->ReplaceChild (lTrgNode, pTrgNode);
						}
					}
					catch (...)
					{
						try
						{
							if	(lTrgNode = pTrgNode->OwnerDocument->CreateTextNode (lEntity->InnerText))
							{
#ifdef	_DEBUG_ENTITIES
								LogMessage (_DEBUG_ENTITIES, _T("Entity [%s] to [%s] (Text)"), _B(pSrcNode->Name), _B(lTrgNode->InnerText));
#endif
								pTrgNode->ParentNode->ReplaceChild (lTrgNode, pTrgNode);
							}
						}
						catch AnyExceptionSilent
					}
				}
			}
			else
			if	(pSrcNode->FirstChild)
			{
				for (lSrcNode = pSrcNode->FirstChild; lSrcNode; lSrcNode = lSrcNode->NextSibling)
				{
					if	(lTrgNode = pTrgNode->OwnerDocument->CreateNode (lSrcNode->NodeType, lSrcNode->Name, lSrcNode->NamespaceURI))
					{
						pTrgNode->AppendChild (lTrgNode);
						CopyNodeAttributes (lSrcNode, lTrgNode);
						CopyNodeChildren (lSrcNode, lTrgNode, pXmlSpace);
					}
				}
			}
			else
			if	(
					(pSrcNode->NodeType == XmlNodeType::Text)
				||	(pSrcNode->NodeType == XmlNodeType::Whitespace)
				||	(pSrcNode->NodeType == XmlNodeType::SignificantWhitespace)
				)
			{
				if	(
						(pSrcNode->NodeType == XmlNodeType::Text)
					&&	(pSrcNode->InnerText->IndexOf (' ') >= 0)
					&&	(pXmlSpace == XmlSpace::Preserve)
					&&	(mOutputHtml)
					&&	(lTrgNode = pTrgNode->OwnerDocument->CreateNode (XmlNodeType::DocumentFragment, nullptr, nullptr))
					)
				{
					try
					{
						lTrgNode->InnerXml = pSrcNode->OuterXml->Replace (" ", "&nbsp;");
						pTrgNode->ParentNode->ReplaceChild (lTrgNode, pTrgNode);
					}
					catch (...)
					{
						LogMessage (LogNormal, _T("Error? [%s] [%s] in [%s]"), _B(DebugStr(pSrcNode->OuterXml)), _B(DebugStr(pSrcNode->InnerText)), _B(DebugStr(pSrcNode->ParentNode->InnerXml)));
						pTrgNode->InnerText = pSrcNode->InnerText;
					}
				}
				else
				{
					pTrgNode->InnerText = pSrcNode->InnerText;
				}
			}
			else
			{
				pTrgNode->InnerXml = pSrcNode->InnerXml;
			}
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////

XmlNode^ XmlToHtml::CopyNodeOuterXml (XmlNode^ pXmlNode, XmlDocument^ pDocument)
{
	XmlNode^	lRet;

	if	(
			(pXmlNode)
		&&	(pDocument)
		)
	{
		if	(pXmlNode->OwnerDocument->BaseURI == pDocument->BaseURI)
		{
			if	(lRet = pDocument->CreateNode (pXmlNode->NodeType, pXmlNode->Name, pXmlNode->NamespaceURI))
			{
				CopyNodeAttributes (pXmlNode, lRet);
				CopyNodeChildren (pXmlNode, lRet, XmlSpace::Default);
			}
		}
		else
		{
			lRet = pDocument->ImportNode (pXmlNode, true);
		}
	}
	return lRet;
}

XmlNode^ XmlToHtml::CopyNodeInnerXml (XmlNode^ pXmlNode, XmlDocument^ pDocument)
{
	XmlNode^	lRet;

	if	(
			(pXmlNode)
		&&	(lRet = pDocument->CreateNode (XmlNodeType::DocumentFragment, nullptr, nullptr))
		)
	{
		CopyNodeChildren (pXmlNode, lRet, XmlSpace::Default);
	}
	return lRet;
}

XmlNode^ XmlToHtml::ChangeNodeName (XmlNode^ pXmlNode, String^ pNewName)
{
	XmlNode^	lRet;

	if	(
			(pXmlNode)
		&&	(lRet = pXmlNode->OwnerDocument->CreateNode (pXmlNode->NodeType, pNewName, pXmlNode->NamespaceURI))
		)
	{
		CopyNodeChildren (pXmlNode, lRet, XmlSpace::Default);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

XmlNode^ XmlToHtml::MakeNewNode (String^ pInnerXml, XmlDocument^ pDocument)
{
	XmlNode^	lNode;

	if	(lNode = pDocument->CreateNode (XmlNodeType::DocumentFragment, nullptr, nullptr))
	{
		lNode->InnerXml = pInnerXml;
	}
	return lNode;
}

XmlNode^ XmlToHtml::MakeNewNode (String^ pNodeName, String^ pInnerText, XmlDocument^ pDocument)
{
	XmlNode^	lNode;

	if	(lNode = pDocument->CreateNode (XmlNodeType::Element, pNodeName, nullptr))
	{
		lNode->AppendChild (pDocument->CreateTextNode (pInnerText));
	}
	return lNode;
}

/////////////////////////////////////////////////////////////////////////////

XmlAttribute^ XmlToHtml::MakeNewAttribute (String^ pAttributeName, String^ pAttributeText, XmlDocument^ pDocument)
{
	XmlAttribute^	lAttribute;

	if	(lAttribute = pDocument->CreateAttribute (pAttributeName))
	{
		lAttribute->Value = pAttributeText;
	}
	return lAttribute;
}

/////////////////////////////////////////////////////////////////////////////

void XmlToHtml::PutParagraphNode (XmlNode^ pParagraphNode, XmlNode^ pTargetNode)
{
	XmlNode^	lParentNode;

	for	(lParentNode = pTargetNode->ParentNode; lParentNode; lParentNode = lParentNode->ParentNode)
	{
		if	(lParentNode->Name == "p")
		{
			break;
		}
	}
	if	(lParentNode)
	{
		lParentNode->ParentNode->InsertAfter (pParagraphNode, lParentNode);
		pTargetNode->ParentNode->RemoveChild (pTargetNode);
	}
	else
	{
		pTargetNode->ParentNode->ReplaceChild (pParagraphNode, pTargetNode);
	}
}

XmlNode^ XmlToHtml::PreserveNodeSpace (XmlNode^ pXmlNode)
{
	XmlAttribute^	lAttribute;

	if	(lAttribute = pXmlNode->OwnerDocument->CreateAttribute ("xml:space", "http://www.w3.org/XML/1998/namespace"))
	{
		lAttribute->Value = "preserve";
		pXmlNode->Attributes->Append (lAttribute);
	}
	return pXmlNode;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool XmlToHtml::ConvertXmlToHtml (XmlDocument^ pXmlDocument, System::Reflection::Assembly^ pAssembly)
{
	bool				lRet = false;
	XmlNode^			lMembersNode;
	XmlNode^			lMemberNode;
	Type^				lMemberType;
	ConstructorInfo^	lMemberConstructor;
	MethodInfo^			lMemberMethod;
	PropertyInfo^		lMemberProperty;
	EventInfo^			lMemberEvent;
	FieldInfo^			lMemberField;

	LoadAssemblyMembers (pAssembly);

	if	(
			(lMembersNode = pXmlDocument ["doc"])
		&&	(lMembersNode = lMembersNode ["members"])
		)
	{
		LogMessage (LogNormal, _T("XmlMembers [%d]"), lMembersNode->ChildNodes->Count);

		for each (lMemberNode in lMembersNode->ChildNodes)
		{
			String^	lMemberName;
			String^	lMemberFileName;

			if	(String::Compare (lMemberNode->Name, "Member", true) != 0)
			{
				if	(
						(lMemberNode->NodeType != XmlNodeType::Comment)
					&&	(lMemberNode->NodeType != XmlNodeType::Whitespace)
					&&	(lMemberNode->NodeType != XmlNodeType::SignificantWhitespace)
					)
				{
					LogMessage (LogNormal, _T("  Unknown member node [%s]"), _B(lMemberNode->Name));
				}
				continue;
			}
			if	(IsInheritDoc (lMemberNode))
			{
				continue;
			}

			lMemberName = lMemberNode->Attributes["name"]->Value;
#ifdef	_DEBUG_XML_MEMBERS
			LogMessage (_DEBUG_XML_MEMBERS, _T("  Member [%s]"), _B(lMemberName));
#endif

			if	(lMemberType = FindAssemblyType (lMemberName))
			{
				lMemberFileName = PutTypeMember (lMemberName, lMemberNode, lMemberType);
			}
			else
			if	(lMemberMethod = FindAssemblyMethod (lMemberName))
			{
				lMemberFileName = PutMethodMember (lMemberName, lMemberNode, lMemberMethod);
			}
			else
			if	(lMemberConstructor = FindAssemblyConstructor (lMemberName))
			{
				lMemberFileName = PutMethodMember (lMemberName, lMemberNode, lMemberConstructor);
			}
			else
			if	(
					(lMemberProperty = FindAssemblyProperty (lMemberName))
				||	(IsAllPropertiesName (lMemberName))
				)
			{
				lMemberFileName = PutPropertyMember (lMemberName, lMemberNode, lMemberProperty);
			}
			else
			if	(lMemberEvent = FindAssemblyEvent (lMemberName))
			{
				lMemberFileName = PutEventMember (lMemberName, lMemberNode, lMemberEvent);
			}
			else
			if	(
					(lMemberField = FindAssemblyField (lMemberName))
				||	(IsAllFieldsName (lMemberName))
				)
			{
				lMemberFileName = PutFieldMember (lMemberName, lMemberNode, lMemberField);
			}
			else
			{
				LogMessage (LogNormal, _T("  Unknown member [%s]"), _B(lMemberName));
			}

			if	(!String::IsNullOrEmpty (lMemberFileName))
			{
#ifdef	_DEBUG_MEMBER_FILES
				LogMessage (_DEBUG_MEMBER_FILES, _T("  Wrote  [%s]"), _B(lMemberFileName));
#endif
#if	FALSE
				Console::WriteLine ("Wrote {0}", lMemberFileName);
#endif				
			}
		}
		lRet = true;
	}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

Object^ XmlToHtml::LoadTypeLibrary (String^ pTypeLibPath)
{
	HRESULT		lResult;
	ITypeLibPtr	lTypeLib;

	lResult = LoadTypeLib (marshal_as<_bstr_t>(pTypeLibPath), &lTypeLib);
	LogComErrAnon (LogAlways, lResult, _T("Load [%s] Interface [%p]"), _B(pTypeLibPath), lTypeLib.GetInterfacePtr());
	return Marshal::GetObjectForIUnknown ((IntPtr)(LPUNKNOWN)lTypeLib.GetInterfacePtr());
}

XmlDocument^ XmlToHtml::LoadXmlFile (String^ pXmlPath, bool pPreserveWhitespace)
{
	XmlDocument^	lRet;
	XmlDocument^	lXmlLoad;

	try
	{
		if	(lXmlLoad = gcnew XmlDocument)
		{
			lXmlLoad->PreserveWhitespace = pPreserveWhitespace;
			lXmlLoad->Load (pXmlPath);
			lRet = lXmlLoad;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

bool XmlToHtml::LoadXmlIncludes (XmlDocument^ pXmlDocument, String^ pXmlPath, bool pPreserveWhitespace)
{
	bool						lRet = true;
	Generic::List<XmlNode^>^	lIncludeNodes;
	XmlNode^					lIncludeNode;

	if	(!mLoadedIncludes)
	{
		mLoadedIncludes = gcnew Generic::Dictionary<String^, XmlDocument^>;
	}

	while	(lIncludeNodes = NodeList (pXmlDocument->SelectNodes (".//include | .//copy")))
	{
		for each (lIncludeNode in lIncludeNodes)
		{
			try
			{
				XmlAttribute^				lIncludeFile;
				XmlAttribute^				lIncludePath;
				XmlAttribute^				lIncludeReplace;
				String^						lIncludeDocName = nullptr;
				XmlDocument^				lIncludeDoc = nullptr;
				Generic::List<XmlNode^>^	lPathNodes = nullptr;
				XmlNode^					lPathNode;
				XmlNode^					lParentNode;

				lIncludeFile = lIncludeNode->Attributes["file"];
				lIncludePath = lIncludeNode->Attributes["path"];
				lIncludeReplace = lIncludeNode->Attributes["replace"];

				if	(
						(lIncludeFile)
					&&	(!String::IsNullOrEmpty (lIncludeFile->Value))
					)
				{
					if	(System::IO::Path::IsPathRooted (lIncludeFile->Value))
					{
						lIncludeDocName = lIncludeFile->Value;
					}
					else
					{
						lIncludeDocName = System::IO::Path::Combine (System::IO::Path::GetDirectoryName (pXmlPath), lIncludeFile->Value);
					}

					if	(mLoadedIncludes->TryGetValue (lIncludeDocName, lIncludeDoc))
					{
						lIncludeDocName = nullptr;
					}
					else
					if	(lIncludeDoc = LoadXmlFile (lIncludeDocName, pPreserveWhitespace))
					{
						mLoadedIncludes[lIncludeDocName] = lIncludeDoc;
					}
				}
				else
				{
					lIncludeDoc = pXmlDocument;
				}

				if	(
						(lIncludeDoc)
					&&	(lIncludeDocName)
					)
				{
					if	(!LoadXmlIncludes (lIncludeDoc, lIncludeDocName, pPreserveWhitespace))
					{
						lRet = false;
						break;
					}
				}

#ifdef	_DEBUG_INCLUDES
				LogMessage (_DEBUG_INCLUDES, _T("Include [%s]"), _B(lIncludePath->Value));
#endif
				if	(lIncludeDoc)
				{
					try
					{
						lPathNodes = NodeList (lIncludeDoc->SelectNodes (lIncludePath->Value));
					}
					catch (...)
					{
						LogMessage (LogNormal, _T("Invalid IncludePath [%s]"), _B(lIncludePath->Value));
					}
				}

				if	(
						(lIncludeDoc)
					&&	(lPathNodes)
					&&	(lParentNode = lIncludeNode->ParentNode)
					)
				{
					XmlNode^	lIncludedPrev = nullptr;
					XmlNode^	lIncludedNode = nullptr;

#ifdef	_DEBUG_INCLUDES
					LogMessage (_DEBUG_INCLUDES, _T("        [%s] [%d]"), _B(lIncludePath->Value), lPathNodes->Count);
#endif
					for each (lPathNode in lPathNodes)
					{
						lIncludedPrev = lIncludedNode;
						lIncludedNode = nullptr;

						if	(lIncludeNode->Name == "copy")
						{
							XmlNode^	lChildNode;

							lPathNode->InnerXml = lPathNode->InnerXml->Trim(String("\t\r\n").ToCharArray());

							if	(
									(lPathNode->FirstChild)
								&&	(lIncludedNode = pXmlDocument->CreateNode (XmlNodeType::DocumentFragment, nullptr, nullptr))
								)
							{
								for (lChildNode = lPathNode->FirstChild; lChildNode; lChildNode = lChildNode->NextSibling)
								{
									lIncludedNode->AppendChild (pXmlDocument->ImportNode (lChildNode, true));
								}
							}
						}
						else
						{
							lIncludedNode = pXmlDocument->ImportNode (lPathNode, true);
						}

						if	(
								(lIncludedNode)
							&&	(lIncludeReplace)
							)
						{
							array<String^>^	lPairs;
							array<String^>^	lPair;
							int				lNdx;

							if	(lPairs = lIncludeReplace->Value->Replace("\\|","\x01")->Split(String("|").ToCharArray(), StringSplitOptions::RemoveEmptyEntries))
							{
								for	(lNdx = 0; lNdx < lPairs->Length; lNdx++)
								{
									if	(
											(lPair = lPairs[lNdx]->Replace('\x01','|')->Replace("\\=","\x02")->Split('='))
										&&	(lPair->Length == 2)
										&&	(!String::IsNullOrEmpty (lPair[0]))
										)
									{
										XmlAttribute^	lAttribute;

										lPair[0] = lPair[0]->Replace('\x02','=');
										lPair[1] = lPair[1]->Replace('\x02','=');
#ifdef	_DEBUG_INCLUDES
										LogMessage (_DEBUG_INCLUDES, _T("        Replace [%s] [%s]"), _B(lPair[0]), _B(lPair[1]));
#endif
										lIncludedNode->InnerXml = lIncludedNode->InnerXml->Replace (lPair[0], lPair[1]);
										if	(lIncludedNode->Attributes)
										{
											for each (lAttribute in lIncludedNode->Attributes)
											{
												lAttribute->InnerXml = lAttribute->InnerXml->Replace (lPair[0], lPair[1]);
											}
										}
									}
								}
							}
						}

						if	(lIncludedNode)
						{
							if	(lIncludedPrev)
							{
								lParentNode->InsertAfter (lIncludedNode, lIncludedPrev);
							}
							else
							{
								lParentNode->ReplaceChild (lIncludedNode, lIncludeNode);
							}
						}
#ifdef	_DEBUG_INCLUDES_NOT
						LogMessage (_DEBUG_INCLUDES, _T("          [%s]"), _B(DebugStr(lPathNode->InnerXml)));
						LogMessage (_DEBUG_INCLUDES, _T("          [%s]"), _B(DebugStr(lParentNode->OuterXml)));
#endif
					}
				}
				else
				{
#ifdef	_DEBUG
					if	(
							(lIncludeNode)
						&&	(lParentNode = lIncludeNode->ParentNode)
						)
					{
						if	(lIncludePath)
						{
							if	(lIncludeFile)
							{
								LogMessage (LogNormal, _T("Include [%s] [%s] not found"), _B(lIncludeFile->Value), _B(lIncludePath->Value));
							}
							else
							{
								LogMessage (LogNormal, _T("Include [%s] not found"), _B(lIncludePath->Value));
							}
						}
						else
						{
							LogMessage (LogNormal, _T("Invalid Include [%s]"), _B(lIncludeNode->OuterXml));
						}

						lIncludeNode->ParentNode->ReplaceChild (MakeNewNode ("span", lIncludeNode->OuterXml, pXmlDocument), lIncludeNode);
					}
#endif
					Console::WriteLine ("Unknown Include {0}", lIncludePath->Value);
					lRet = false;
					break;
				}
			}
			catch AnyExceptionDebug
		}
		if	(!lRet)
		{
			break;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

XmlDocument^ XmlToHtml::LoadTypeTemplate ()
{
	return LoadTemplate (System::IO::Path::Combine (mTemplatePath, "TypeTemplate.htm"));
}

XmlDocument^ XmlToHtml::LoadMethodTemplate ()
{
	return LoadTemplate (System::IO::Path::Combine (mTemplatePath, "MethodTemplate.htm"));
}

XmlDocument^ XmlToHtml::LoadPropertyTemplate ()
{
	return LoadTemplate (System::IO::Path::Combine (mTemplatePath, "PropertyTemplate.htm"));
}

XmlDocument^ XmlToHtml::LoadFieldTemplate ()
{
	return LoadTemplate (System::IO::Path::Combine (mTemplatePath, "FieldTemplate.htm"));
}

XmlDocument^ XmlToHtml::LoadEventTemplate ()
{
	return LoadTemplate (System::IO::Path::Combine (mTemplatePath, "EventTemplate.htm"));
}

XmlDocument^ XmlToHtml::LoadTemplate (String^ pTemplatePath)
{
	XmlDocument^	lRet = nullptr;

	try
	{
		XmlReaderSettings^	lReaderSettings = gcnew XmlReaderSettings;
		XmlReader^			lReader;
		XmlDocument^		lDocument;

		lReaderSettings->ValidationType = ValidationType::None;
		lReaderSettings->ValidationFlags = Schema::XmlSchemaValidationFlags::None;
		lReaderSettings->ConformanceLevel = ConformanceLevel::Document;
		lReaderSettings->IgnoreComments = true;
		lReaderSettings->IgnoreWhitespace = true;
		lReaderSettings->ProhibitDtd = false;

		lReader = XmlReader::Create (pTemplatePath, lReaderSettings);
		lDocument = gcnew XmlDocument;
		lDocument->Load (lReader);
		lRet = lDocument;
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

static String^ ParametersStr (array<ParameterInfo^>^ pParameters)
{
	StringBuilder^	lRet = gcnew StringBuilder;
	ParameterInfo^	lParameter;

	if	(pParameters)
	{
		for each (lParameter in pParameters)
		{
			if	(lRet->Length > 0)
			{
				lRet->Append (",");
			}
			lRet->Append (String::Format ("{0}.{1}", lParameter->ParameterType->Namespace, lParameter->ParameterType->Name));
			if	(lParameter->ParameterType->IsArray)
			{
				lRet->Append ("[]");
			}
			if	(lParameter->ParameterType->IsByRef)
			{
				lRet->Replace ("&","@");
			}
		}
	}
	return "(" + lRet->ToString () + ")";
}

void XmlToHtml::LoadAssemblyMembers (System::Reflection::Assembly^ pAssembly)
{
	String^						lMemberName;
	array<Type^>^				lTypes;
	Type^						lType;
	array<ConstructorInfo^>^	lConstructors;
	ConstructorInfo^			lConstructor;
	array<MethodInfo^>^			lMethods;
	MethodInfo^					lMethod;
	array<PropertyInfo^>^		lProperties;
	PropertyInfo^				lProperty;
	array<EventInfo^>^			lEvents;
	EventInfo^					lEvent;
	array<FieldInfo^>^			lFields;
	FieldInfo^					lField;
#ifdef	_DEBUG_ASSEMBLY_MEMBERS
	Generic::SortedDictionary <String^, System::Type^>^							lSourceTypes = gcnew Generic::SortedDictionary <String^, System::Type^>;
	Generic::SortedDictionary <String^, System::Reflection::MethodInfo^>^		lSourceMethods;
	Generic::SortedDictionary <String^, System::Reflection::ConstructorInfo^>^	lSourceConstructors;
	Generic::SortedDictionary <String^, System::Reflection::PropertyInfo^>^		lSourceProperties;
	Generic::SortedDictionary <String^, System::Reflection::EventInfo^>^		lSourceEvents;
	Generic::SortedDictionary <String^, System::Reflection::FieldInfo^>^		lSourceFields;

	Generic::KeyValuePair<String^,Type^>^										lSourceType;
	Generic::KeyValuePair<String^,ConstructorInfo^>^							lSourceConstructor;
	Generic::KeyValuePair<String^,MethodInfo^>^									lSourceMethod;
	Generic::KeyValuePair<String^,PropertyInfo^>^								lSourceProperty;
	Generic::KeyValuePair<String^,EventInfo^>^									lSourceEvent;
	Generic::KeyValuePair<String^,FieldInfo^>^									lSourceField;
#endif

	mSourceAssembly = pAssembly;

	LogMessage (LogNormal, _T("LoadAssemblyMembers [%s]"), _B(mSourceAssembly->GetName()->Name));

	if	(lTypes = pAssembly->GetTypes ())
	{
		for each (lType in lTypes)
		{
			if	(
					(!lType->IsClass)
				&&	(!lType->IsEnum)
				)
			{
				continue;
			}
			lMemberName = String::Format ("T:{0}.{1}", lType->Namespace, lType->Name);
#ifdef	_DEBUG_ASSEMBLY_MEMBERS
			lSourceTypes [lMemberName] = lType;
		}
		for each (lSourceType in lSourceTypes)
		{
			mSourceTypes [lSourceType->Key] = lSourceType->Value;
			lSourceMethods = gcnew Generic::SortedDictionary <String^, System::Reflection::MethodInfo^>;
			lSourceConstructors = gcnew Generic::SortedDictionary <String^, System::Reflection::ConstructorInfo^>;
			lSourceProperties = gcnew Generic::SortedDictionary <String^, System::Reflection::PropertyInfo^>;
			lSourceFields = gcnew Generic::SortedDictionary <String^, System::Reflection::FieldInfo^>;
			lSourceEvents = gcnew Generic::SortedDictionary <String^, System::Reflection::EventInfo^>;

			lType = lSourceType->Value;
			LogMessage (_DEBUG_ASSEMBLY_MEMBERS, _T("  Type       %s"), _B(lSourceType->Key));
#else
			mSourceTypes [lMemberName] = lType;
#endif

			if	(lConstructors = lType->GetConstructors (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
			{
				for each (lConstructor in lConstructors)
				{
					lMemberName = String::Format ("M:{0}.{1}.#ctor{2}", lConstructor->DeclaringType->Namespace, lConstructor->DeclaringType->Name, ParametersStr(lConstructor->GetParameters()));
#ifdef	_DEBUG_ASSEMBLY_MEMBERS
					lSourceConstructors [lMemberName] = lConstructor;
				}
				for each (lSourceConstructor in lSourceConstructors)
				{
					mSourceConstructors [lSourceConstructor->Key] = lSourceConstructor->Value;
					LogMessage (_DEBUG_ASSEMBLY_MEMBERS, _T("    Ctor       %s"), _B(lSourceConstructor->Key));
#else
					mSourceConstructors [lMemberName] = lConstructor;
#endif
				}
			}

			if	(lMethods = lType->GetMethods (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
			{
				for each (lMethod in lMethods)
				{
					lMemberName = String::Format ("M:{0}.{1}.{2}{3}", lMethod->DeclaringType->Namespace, lMethod->DeclaringType->Name, lMethod->Name, ParametersStr(lMethod->GetParameters()));
#ifdef	_DEBUG_ASSEMBLY_MEMBERS
					lSourceMethods [lMemberName] = lMethod;
				}
				for each (lSourceMethod in lSourceMethods)
				{
					mSourceMethods [lSourceMethod->Key] = lSourceMethod->Value;
					LogMessage (_DEBUG_ASSEMBLY_MEMBERS, _T("    Method     %s"), _B(lSourceMethod->Key));
#else
					mSourceMethods [lMemberName] = lMethod;
#endif
				}
			}

			if	(lProperties = lType->GetProperties (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
			{
				for each (lProperty in lProperties)
				{
					lMemberName = String::Format ("P:{0}.{1}.{2}", lProperty->DeclaringType->Namespace, lProperty->DeclaringType->Name, lProperty->Name);
#ifdef	_DEBUG_ASSEMBLY_MEMBERS
					lSourceProperties [lMemberName] = lProperty;
				}
				for each (lSourceProperty in lSourceProperties)
				{
					mSourceProperties [lSourceProperty->Key] = lSourceProperty->Value;
					LogMessage (_DEBUG_ASSEMBLY_MEMBERS, _T("    Property   %s"), _B(lSourceProperty->Key));
#else
					mSourceProperties [lMemberName] = lProperty;
#endif
				}
			}

			if	(lEvents = lType->GetEvents (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
			{
				for each (lEvent in lEvents)
				{
					lMemberName = String::Format ("E:{0}.{1}.{2}", lEvent->DeclaringType->Namespace, lEvent->DeclaringType->Name, lEvent->Name);
#ifdef	_DEBUG_ASSEMBLY_MEMBERS
					lSourceEvents [lMemberName] = lEvent;
				}
				for each (lSourceEvent in lSourceEvents)
				{
					mSourceEvents [lSourceEvent->Key] = lSourceEvent->Value;
					LogMessage (_DEBUG_ASSEMBLY_MEMBERS, _T("    Event      %s"), _B(lSourceEvent->Key));
#else
					mSourceEvents [lMemberName] = lEvent;
#endif
				}
			}

			if	(lFields = lType->GetFields (BindingFlags::Instance|BindingFlags::Static|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
			{
				for each (lField in lFields)
				{
					lMemberName = String::Format ("F:{0}.{1}.{2}", lField->DeclaringType->Namespace, lField->DeclaringType->Name, lField->Name);
#ifdef	_DEBUG_ASSEMBLY_MEMBERS
					lSourceFields [lMemberName] = lField;
				}
				for each (lSourceField in lSourceFields)
				{
					mSourceFields [lSourceField->Key] = lSourceField->Value;
					LogMessage (_DEBUG_ASSEMBLY_MEMBERS, _T("    Field      %s"), _B(lSourceField->Key));
#else
					mSourceFields [lMemberName] = lField;
#endif
				}
			}
		}
	}

	LogMessage (LogNormal, _T("Types [%d] Methods [%d] Properties [%d] Events [%d] Fields [%d]"), mSourceTypes->Count, mSourceMethods->Count+mSourceConstructors->Count, mSourceProperties->Count, mSourceEvents->Count, mSourceFields->Count);

#ifdef	_LOG_ASSEMBLY_MEMBERS
	Generic::SortedDictionary<String^,Object^>^	lDictionary = gcnew Generic::SortedDictionary<String^,Object^>^;
#endif
}

/////////////////////////////////////////////////////////////////////////////

Type^ XmlToHtml::FindAssemblyType (String^ pMemberName)
{
	Type^	lRet = nullptr;

	if	(pMemberName->StartsWith ("T:"))
	{
		mSourceTypes->TryGetValue (pMemberName, lRet);
	}
	return lRet;
}

System::Reflection::MethodInfo^ XmlToHtml::FindAssemblyMethod (String^ pMemberName)
{
	MethodInfo^	lRet = nullptr;

	if	(
			(pMemberName->StartsWith ("M:"))
		&&	(pMemberName->IndexOf ("#ctor") < 0)
		&&	(!mSourceMethods->TryGetValue (pMemberName, lRet))
		)
	{
		String^											lMethodName = pMemberName;
		Generic::KeyValuePair<String^, MethodInfo^>^	lMethod;

		if	(lMethodName->IndexOf ('(') > 0)
		{
			lMethodName = lMethodName->Substring (0, lMethodName->IndexOf ('('));
		}
		for each (lMethod in mSourceMethods)
		{
			if	(
					(lMethod->Key->StartsWith (lMethodName))
				&&	(lRet = lMethod->Value)
				)
			{
				break;
			}
		}
	}
	return lRet;
}

System::Reflection::ConstructorInfo^ XmlToHtml::FindAssemblyConstructor (String^ pMemberName)
{
	ConstructorInfo^	lRet = nullptr;

	if	(
			(pMemberName->StartsWith ("M:"))
		&&	(pMemberName->IndexOf ("#ctor") > 0)
		&&	(!mSourceConstructors->TryGetValue (pMemberName, lRet))
		)
	{
		String^												lConstructorName = pMemberName;
		Generic::KeyValuePair<String^, ConstructorInfo^>^	lConstructor;

		if	(lConstructorName->IndexOf ('(') > 0)
		{
			lConstructorName = lConstructorName->Substring (0, lConstructorName->IndexOf ('('));
		}
		for each (lConstructor in mSourceConstructors)
		{
			if	(
					(lConstructor->Key->StartsWith (lConstructorName))
				&&	(lRet = lConstructor->Value)
				)
			{
				break;
			}
		}
	}
	return lRet;
}

System::Reflection::PropertyInfo^ XmlToHtml::FindAssemblyProperty (String^ pMemberName)
{
	PropertyInfo^	lRet = nullptr;

	if	(pMemberName->StartsWith ("P:"))
	{
		mSourceProperties->TryGetValue (pMemberName, lRet);
	}
	return lRet;
}

System::Reflection::FieldInfo^ XmlToHtml::FindAssemblyField (String^ pMemberName)
{
	FieldInfo^	lRet = nullptr;

	if	(pMemberName->StartsWith ("F:"))
	{
		mSourceFields->TryGetValue (pMemberName, lRet);
	}
	return lRet;
}

System::Reflection::EventInfo^ XmlToHtml::FindAssemblyEvent (String^ pMemberName)
{
	EventInfo^	lRet = nullptr;

	if	(pMemberName->StartsWith ("E:"))
	{
		mSourceEvents->TryGetValue (pMemberName, lRet);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

String^ XmlToHtml::PutTypeMember (String^ pMemberName, XmlNode^ pMemberNode, Type^ pAssemblyType)
{
	String^			lRet;
	XmlDocument^	lTemplate;
	XmlElement^		lTargetRoot;

	if	(
			(lTemplate = LoadTypeTemplate())
		&&	(lTargetRoot = lTemplate->DocumentElement)
		)
	{
		PutMemberName (lTargetRoot, pMemberNode, pAssemblyType);
		PutMemberSummary (lTargetRoot, FormatSummary (pMemberNode ["summary"]));
		PutMemberSyntax (lTargetRoot, FormatInnerXml (pMemberNode ["syntax"]), nullptr, nullptr);
		PutMemberRemarks (lTargetRoot, FormatRemarks (pMemberNode ["remarks"]), FormatDetails (pMemberNode->SelectNodes ("./detail | ./details")));
		PutMemberSeeAlso (lTargetRoot, FormatSeeAlso (pMemberNode->SelectNodes ("seealso")));

		lRet = PutMemberFile (TypeFileName (pAssemblyType), lTargetRoot);
	}
	return lRet;
}

String^ XmlToHtml::PutMethodMember (String^ pMemberName, XmlNode^ pMemberNode, MethodInfo^ pAssemblyMethod)
{
	String^			lRet;
	XmlDocument^	lTemplate;
	XmlElement^		lTargetRoot;

	if	(
			(lTemplate = LoadMethodTemplate())
		&&	(lTargetRoot = lTemplate->DocumentElement)
		)
	{
		PutMemberName (lTargetRoot, pMemberNode, pAssemblyMethod);
		PutMemberSummary (lTargetRoot, FormatSummary (pMemberNode ["summary"]));
		PutMemberSyntax (lTargetRoot, FormatSyntax (pMemberNode ["syntax"]), FormatParameters (pMemberNode->SelectNodes ("./param | ./returns | ./value")), FormatExamples (pMemberNode->SelectNodes ("./example")));
		PutMemberRemarks (lTargetRoot, FormatRemarks (pMemberNode ["remarks"]), FormatDetails (pMemberNode->SelectNodes ("./detail | ./details")));
		PutMemberSeeAlso (lTargetRoot, FormatSeeAlso (pMemberNode->SelectNodes ("seealso")));

		lRet = PutMemberFile (MethodFileName (pAssemblyMethod), lTargetRoot);
	}
	return lRet;
}

String^ XmlToHtml::PutMethodMember (String^ pMemberName, XmlNode^ pMemberNode, ConstructorInfo^ pAssemblyConstructor)
{
	return nullptr;
}

String^ XmlToHtml::PutPropertyMember (String^ pMemberName, XmlNode^ pMemberNode, PropertyInfo^ pAssemblyProperty)
{
	String^			lRet;
	XmlDocument^	lTemplate;
	XmlElement^		lTargetRoot;
	String^			lFileName;

	if	(
			(lTemplate = LoadPropertyTemplate())
		&&	(lTargetRoot = lTemplate->DocumentElement)
		)
	{
		if	(pAssemblyProperty)
		{
			PutMemberName (lTargetRoot, pMemberNode, pAssemblyProperty);
			lFileName = PropertyFileName (pAssemblyProperty);
		}
		else
		if	(IsAllPropertiesName (pMemberName))
		{
			PutMemberName (lTargetRoot, AllPropertiesName (pMemberName));
			lFileName = AllPropertiesFileName (pMemberName);
		}
		PutMemberSummary (lTargetRoot, FormatSummary (pMemberNode ["summary"]));
		PutMemberSyntax (lTargetRoot, FormatSyntax (pMemberNode ["syntax"]), FormatParameters (pMemberNode->SelectNodes ("./param | ./returns | ./value")), FormatExamples (pMemberNode->SelectNodes ("./example")));
		PutMemberRemarks (lTargetRoot, FormatRemarks (pMemberNode ["remarks"]), FormatDetails (pMemberNode->SelectNodes ("./detail | ./details")));
		PutMemberSeeAlso (lTargetRoot, FormatSeeAlso (pMemberNode->SelectNodes ("seealso")));

		lRet = PutMemberFile (lFileName, lTargetRoot);
	}
	return lRet;
}

String^ XmlToHtml::PutEventMember (String^ pMemberName, XmlNode^ pMemberNode, EventInfo^ pAssemblyEvent)
{
	String^			lRet;
	XmlDocument^	lTemplate;
	XmlElement^		lTargetRoot;

	if	(
			(lTemplate = LoadEventTemplate())
		&&	(lTargetRoot = lTemplate->DocumentElement)
		)
	{
		PutMemberName (lTargetRoot, pMemberNode, pAssemblyEvent);
		PutMemberSummary (lTargetRoot, FormatSummary (pMemberNode ["summary"]));
		PutMemberSyntax (lTargetRoot, FormatSyntax (pMemberNode ["syntax"]), FormatParameters (pMemberNode->SelectNodes ("./param | ./returns")), FormatExamples (pMemberNode->SelectNodes ("./example")));
		PutMemberRemarks (lTargetRoot, FormatRemarks (pMemberNode ["remarks"]), FormatDetails (pMemberNode->SelectNodes ("./detail | ./details")));
		PutMemberSeeAlso (lTargetRoot, FormatSeeAlso (pMemberNode->SelectNodes ("seealso")));

		lRet = PutMemberFile (EventFileName (pAssemblyEvent), lTargetRoot);
	}
	return lRet;
}

String^ XmlToHtml::PutFieldMember (String^ pMemberName, XmlNode^ pMemberNode, FieldInfo^ pAssemblyField)
{
	String^			lRet;
	XmlDocument^	lTemplate;
	XmlElement^		lTargetRoot;

	if	(
			(lTemplate = LoadFieldTemplate())
		&&	(lTargetRoot = lTemplate->DocumentElement)
		)
	{
		PutMemberName (lTargetRoot, pMemberNode, pAssemblyField);
		PutMemberSummary (lTargetRoot, FormatSummary (pMemberNode ["summary"]));
		PutMemberValues (lTargetRoot, pMemberNode->SelectNodes ("./value"));
		PutMemberRemarks (lTargetRoot, FormatRemarks (pMemberNode ["remarks"]), nullptr);
		PutMemberSeeAlso (lTargetRoot, FormatSeeAlso (pMemberNode->SelectNodes ("seealso")));

		lRet = PutMemberFile (FieldFileName (pAssemblyField), lTargetRoot);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void XmlToHtml::PutMemberName (XmlElement^ pRootElement, XmlNode^ pMemberNode, Type^ pAssemblyType)
{
	PutMemberName (pRootElement, pMemberNode, pAssemblyType->Name, "Object");
}

void XmlToHtml::PutMemberName (XmlElement^ pRootElement, XmlNode^ pMemberNode, MethodInfo^ pAssemblyMethod)
{
	PutMemberName (pRootElement, pMemberNode, /*pAssemblyMethod->DeclaringType->Name + " " + */pAssemblyMethod->Name, "Method");
}

void XmlToHtml::PutMemberName (XmlElement^ pRootElement, XmlNode^ pMemberNode, PropertyInfo^ pAssemblyProperty)
{
	PutMemberName (pRootElement, pMemberNode, /*pAssemblyProperty->DeclaringType->Name + " " + */pAssemblyProperty->Name, "Property");
}

void XmlToHtml::PutMemberName (XmlElement^ pRootElement, XmlNode^ pMemberNode, EventInfo^ pAssemblyEvent)
{
	PutMemberName (pRootElement, pMemberNode, /*pAssemblyEvent->DeclaringType->Name + " " + */pAssemblyEvent->Name, "Event");
}

void XmlToHtml::PutMemberName (XmlElement^ pRootElement, XmlNode^ pMemberNode, FieldInfo^ pAssemblyField)
{
	PutMemberName (pRootElement, pMemberNode, /*pAssemblyField->DeclaringType->Name + " " + */pAssemblyField->Name, "Member");
}

/////////////////////////////////////////////////////////////////////////////

void XmlToHtml::PutMemberName (System::Xml::XmlElement^ pRootElement, XmlNode^ pMemberNode, String^ pMemberName, String^ pSuffix)
{
	XmlAttribute^	lAttribute;

	if	(lAttribute = pMemberNode->Attributes["kind"])
	{
		pSuffix = lAttribute->Value;
	}
	if	(String::IsNullOrEmpty (pSuffix))
	{
		pSuffix = "";
	}
	else
	{
		pSuffix = " " + pSuffix->TrimStart();
	}

	PutMemberName (pRootElement, pMemberName + pSuffix);
}

void XmlToHtml::PutMemberName (XmlElement^ pRootElement, String^ pMemberName)
{
	Generic::List<XmlNode^>^	lNodeList;
	XmlNode^					lNode;

	if	(lNodeList = NodeList (pRootElement->GetElementsByTagName ("member")))
	{
		for each (lNode in lNodeList)
		{
			lNode->ParentNode->ReplaceChild (lNode->OwnerDocument->CreateTextNode (pMemberName), lNode);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

void XmlToHtml::PutMemberSummary (XmlElement^ pRootElement, XmlNode^ pMemberSummary)
{
	Generic::List<XmlNode^>^	lNodeList;
	XmlNode^					lNode;

	if	(lNodeList = NodeList (pRootElement->GetElementsByTagName ("summary")))
	{
		for each (lNode in lNodeList)
		{
			if	(pMemberSummary)
			{
				if	(!String::IsNullOrEmpty (lNode->InnerText))
				{
					lNode->ParentNode->InsertBefore (CopyNodeInnerXml (lNode, lNode->OwnerDocument), lNode);
				}
				lNode->ParentNode->ReplaceChild (CopyNodeInnerXml (pMemberSummary, lNode->OwnerDocument), lNode);
			}
			else
			{
				lNode->ParentNode->RemoveChild (lNode);
			}
		}
	}
}

void XmlToHtml::PutMemberRemarks (XmlElement^ pRootElement, XmlNode^ pMemberRemarks, XmlNode^ pMemberDetails)
{
	Generic::List<XmlNode^>^	lNodeList;
	XmlNode^					lNode;

	if	(lNodeList = NodeList (pRootElement->SelectNodes (".//detail | .//details")))
	{
		for each (lNode in lNodeList)
		{
			if	(pMemberDetails)
			{
				if	(!String::IsNullOrEmpty (lNode->InnerText))
				{
					lNode->ParentNode->InsertBefore (CopyNodeInnerXml (lNode, lNode->OwnerDocument), lNode);
				}
				lNode->ParentNode->ReplaceChild (CopyNodeInnerXml (pMemberDetails, lNode->OwnerDocument), lNode);
			}
			else
			{
				lNode->ParentNode->RemoveChild (lNode);
			}
		}
	}

	if	(lNodeList = NodeList (pRootElement->GetElementsByTagName ("remarks")))
	{
		for each (lNode in lNodeList)
		{
			if	(pMemberRemarks)
			{
				if	(!String::IsNullOrEmpty (lNode->InnerText))
				{
					lNode->ParentNode->InsertBefore (CopyNodeInnerXml (lNode, lNode->OwnerDocument), lNode);
				}
				lNode->ParentNode->ReplaceChild (CopyNodeInnerXml (pMemberRemarks, lNode->OwnerDocument), lNode);
			}
			else
			{
				lNode->ParentNode->RemoveChild (lNode);
			}
		}
	}
}

void XmlToHtml::PutMemberSyntax (XmlElement^ pRootElement, XmlNode^ pMemberSyntax, XmlNode^ pMemberParameters, XmlNode^ pMemberExamples)
{
	Generic::List<XmlNode^>^	lNodeList;
	XmlNode^					lNode;
	XmlNode^					lNewNode;

	if	(pMemberParameters)
	{
		if	(pMemberSyntax)
		{
			pMemberSyntax->AppendChild (CopyNodeOuterXml (pMemberParameters, pMemberSyntax->OwnerDocument));
		}
		else
		{
			pMemberSyntax = pMemberParameters;
		}
	}
	if	(pMemberExamples)
	{
		if	(pMemberSyntax)
		{
			pMemberSyntax->AppendChild (CopyNodeOuterXml (pMemberExamples, pMemberSyntax->OwnerDocument));
		}
		else
		{
			pMemberSyntax = pMemberExamples;
		}
	}

	if	(lNodeList = NodeList (pRootElement->GetElementsByTagName ("syntax")))
	{
		for each (lNode in lNodeList)
		{
			if	(pMemberSyntax)
			{
				if	(!String::IsNullOrEmpty (lNode->InnerText))
				{
					lNode->ParentNode->InsertBefore (CopyNodeInnerXml (lNode, lNode->OwnerDocument), lNode);
				}
				if	(pMemberSyntax->Name == "syntax")
				{
					lNode->ParentNode->ReplaceChild (lNewNode = ChangeNodeName (CopyNodeOuterXml (pMemberSyntax, lNode->OwnerDocument), "div"), lNode);
					lNewNode->Attributes->Append (MakeNewAttribute ("id", "Syntax", lNewNode->OwnerDocument));
				}
				else
				{
					lNode->ParentNode->ReplaceChild (lNewNode = CopyNodeOuterXml (pMemberSyntax, lNode->OwnerDocument), lNode);
				}
			}
			else
			{
				lNode->ParentNode->RemoveChild (lNode);
			}
		}
	}
}

void XmlToHtml::PutMemberValues (XmlElement^ pRootElement, XmlNodeList^ pMemberValues)
{
	Generic::List<XmlNode^>^	lNodeList;
	XmlNode^					lNode;
	XmlNode^					lValue;

	if	(
			(lNodeList = NodeList (pRootElement->GetElementsByTagName ("value")))
		||	(lNodeList = NodeList (pRootElement->GetElementsByTagName ("values")))
		)
	{
		for each (lNode in lNodeList)
		{
			if	(
					(pMemberValues)
				&&	(pMemberValues->Count > 0)
				)
			{
				if	(!String::IsNullOrEmpty (lNode->InnerText))
				{
					lNode->ParentNode->InsertBefore (CopyNodeInnerXml (lNode, lNode->OwnerDocument), lNode);
					lNode->RemoveAll ();
				}
				for each (lValue in pMemberValues)
				{
					lNode->AppendChild (CopyNodeOuterXml (ChangeNodeName (lValue, "p"), lNode->OwnerDocument));
				}
				lNode->ParentNode->ReplaceChild (lValue = ChangeNodeName (lNode, "div"), lNode);
				lValue->Attributes->Append (MakeNewAttribute ("id", "Value", lNode->OwnerDocument));
				lValue->Attributes->Append (MakeNewAttribute ("class", "syntax", lNode->OwnerDocument));
			}
			else
			{
				lNode->ParentNode->RemoveChild (lNode);
			}
		}
	}
}

void XmlToHtml::PutMemberSeeAlso (XmlElement^ pRootElement, XmlNode^ pMemberSeeAlso)
{
	Generic::List<XmlNode^>^	lNodeList;
	XmlNode^					lNode;

	if	(lNodeList = NodeList (pRootElement->GetElementsByTagName ("seealso")))
	{
		for each (lNode in lNodeList)
		{
			if	(pMemberSeeAlso)
			{
				if	(!String::IsNullOrEmpty (lNode->InnerText))
				{
					lNode->ParentNode->InsertBefore (CopyNodeInnerXml (lNode, lNode->OwnerDocument), lNode);
				}
				lNode->ParentNode->ReplaceChild (CopyNodeInnerXml (pMemberSeeAlso, lNode->OwnerDocument), lNode);
			}
			else
			{
				lNode->ParentNode->RemoveChild (lNode);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

XmlNode^ XmlToHtml::FormatSummary (XmlNode^ pXmlNode)
{
	return FormatInnerXml (pXmlNode);
}

XmlNode^ XmlToHtml::FormatDetails (System::Xml::XmlNodeList^ pXmlNodeList)
{
	if	(pXmlNodeList)
	{
		if	(pXmlNodeList->Count > 1)
		{
			XmlNode^	lRet = pXmlNodeList[0]->OwnerDocument->CreateNode (XmlNodeType::DocumentFragment, nullptr, nullptr);
			XmlNode^	lNode;

			for each (lNode in pXmlNodeList)
			{
				lRet->AppendChild (CopyNodeInnerXml (FormatInnerXml (lNode, "example"), lNode->OwnerDocument));
			}
			return lRet;
		}
		else
		{
			return FormatInnerXml (pXmlNodeList[0], "example");
		}
	}
	return nullptr;
}

XmlNode^ XmlToHtml::FormatRemarks (XmlNode^ pXmlNode)
{
	return FormatInnerXml (pXmlNode, "example");
}

/////////////////////////////////////////////////////////////////////////////

XmlNode^ XmlToHtml::FormatInnerXml (XmlNode^ pXmlNode)
{
	return FormatInnerXml (pXmlNode, nullptr);
}

XmlNode^ XmlToHtml::FormatInnerXml (XmlNode^ pXmlNode, String^ pCodeClass)
{
	XmlNode^	lRet;

	if	(
			(pXmlNode)
		&&	(lRet = pXmlNode->Clone ())
		)
	{
		Generic::List<XmlNode^>^	lNodeList;
		XmlNode^					lNode;
		XmlNode^					lNewNode;

		if	(lNodeList = NodeList (lRet->SelectNodes (".//list")))
		{
			for each (lNode in lNodeList)
			{
				lNode->ParentNode->ReplaceChild (FormatList (lNode, pCodeClass), lNode);
			}
		}

		if	(lNodeList = NodeList (lRet->SelectNodes (".//para")))
		{
			for each (lNode in lNodeList)
			{
				if	(lNode->FirstChild == nullptr)
				{
					lNode->ParentNode->ReplaceChild (ChangeNodeName (lNode, "br"), lNode);
				}
				else
				{
					PutParagraphNode (ChangeNodeName (lNode, "p"), lNode);
				}
			}
		}
		if	(lNodeList = NodeList (lRet->SelectNodes (".//note")))
		{
			for each (lNode in lNodeList)
			{
				PutParagraphNode (lNewNode = ChangeNodeName (lNode, "p"), lNode);
				lNewNode->InsertAfter (MakeNewNode ("<span class=\"note\">Note:</span> ", lNewNode->OwnerDocument), nullptr);
				lNewNode->Attributes->Append (MakeNewAttribute ("class", "note", lNewNode->OwnerDocument));
			}
		}

		if	(lNodeList = NodeList (lRet->SelectNodes (".//c | .//strong")))
		{
			for each (lNode in lNodeList)
			{
				lNode->ParentNode->ReplaceChild (ChangeNodeName (lNode, "s"), lNode);
			}
		}
		if	(lNodeList = NodeList (lRet->SelectNodes (".//keyword | .//token | .//typename | .//filename | .//self | .//literal | .//glossary")))
		{
			for each (lNode in lNodeList)
			{
				XmlAttribute^	lClassAttr = lNode->Attributes ["class"];
				String^			lClassName = (lClassAttr) ? String::Concat (lNode->Name, " ", lClassAttr->Value) : (lNode->Name == "self") ? "self keyword" : lNode->Name;

				lNode->ParentNode->ReplaceChild (lNewNode = ChangeNodeName (lNode, "em"), lNode);
				lNewNode->Attributes->Append (MakeNewAttribute ("class", lClassName, lNewNode->OwnerDocument));
			}
		}

		if	(lNodeList = NodeList (lRet->SelectNodes (".//paramref")))
		{
			for each (lNode in lNodeList)
			{
				XmlAttribute^	lParamName;
				XmlNode^		lParamNode;

				if	(
						(lParamName = lNode->Attributes ["name"])
					&&	(lParamNode = MakeNewNode ("em", lParamName->Value, pXmlNode->OwnerDocument))
					)
				{
					lParamNode->Attributes->Append (MakeNewAttribute ("class", "token", lParamNode->OwnerDocument));
					lNode->ParentNode->ReplaceChild (lParamNode, lNode);
				}
			}
		}

		if	(lNodeList = NodeList (lRet->SelectNodes (".//bookmark")))
		{
			for each (lNode in lNodeList)
			{
				XmlAttribute^	lBookmarkName;
				XmlNode^		lBookmarkNode;

				if	(
						(lBookmarkName = lNode->Attributes ["name"])
					&&	(lBookmarkNode = MakeNewNode ("a", " ", pXmlNode->OwnerDocument))
					)
				{
					lBookmarkNode->Attributes->Append (MakeNewAttribute ("name", lBookmarkName->Value, lBookmarkNode->OwnerDocument));
					lNode->ParentNode->ReplaceChild (lBookmarkNode, lNode);
				}
				else
				{
					lNode->ParentNode->RemoveChild (lNode);
				}
			}
		}
		if	(lNodeList = NodeList (lRet->SelectNodes (".//code")))
		{
			for each (lNode in lNodeList)
			{
				lNewNode = ChangeNodeName (lNode, "pre");
				if	(
						(lNode->NextSibling == nullptr)
					&&	(lNode->PreviousSibling == nullptr)
					&&	(lNode->ParentNode->Name == "p")
					)
				{
					lNode->ParentNode->ParentNode->ReplaceChild (lNewNode, lNode->ParentNode);
				}
				else
				{
					lNode->ParentNode->ReplaceChild (lNewNode, lNode);
				}
				if	(pCodeClass)
				{
					lNewNode->Attributes->Append (MakeNewAttribute ("class", pCodeClass, lNewNode->OwnerDocument));
				}
			}
		}
		if	(lNodeList = NodeList (lRet->SelectNodes (".//example")))
		{
			for each (lNode in lNodeList)
			{
				XmlNodeList^	lCodeNodes;
				XmlNode^		lExampleNode;

				if	(
						(lCodeNodes = lNode->SelectNodes (".//code | .//samp | .//pre"))
					&&	(lCodeNodes->Count > 0)
					)
				{
					lNode->ParentNode->ReplaceChild (lExampleNode = ChangeNodeName (lNode, "div"), lNode);
				}
				else
				{
					lNode->ParentNode->ReplaceChild (lExampleNode = ChangeNodeName (lNode, "samp"), lNode);
				}
				lExampleNode->Attributes->Append (MakeNewAttribute ("class", "example", lExampleNode->OwnerDocument));
			}
		}
		if	(lNodeList = NodeList (lRet->SelectNodes (".//see")))
		{
			for each (lNode in lNodeList)
			{
				if	(lNewNode = FormatSee (lNode))
				{
					XmlAttribute^	lClassAttr = lNode->Attributes ["class"];
					String^			lClassName = (lClassAttr) ? String::Concat ("see ", lClassAttr->Value) : "see keyword";

					lNewNode->Attributes->Append (MakeNewAttribute ("class", lClassName, lNewNode->OwnerDocument));
					lNode->ParentNode->ReplaceChild (lNewNode, lNode);
				}
			}
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

XmlNode^ XmlToHtml::FormatSyntax (XmlNode^ pXmlNode)
{
	XmlNode^		lRet;
	XmlNodeList^	lLanguageNodes;
	XmlNodeList^	lCppNodes;
	XmlNodeList^	lCsharpNodes;
	XmlNodeList^	lVbNodes;
	XmlNodeList^	lVb6Nodes;
	XmlNodeList^	lVbscriptNodes;
	XmlNode^		lLanguagesNode;

	if	(pXmlNode)
	{
		if	(
				(lLanguageNodes = pXmlNode->SelectNodes ("./cpp | ./cplusplus | ./csharp | ./vb | ./vbnet | ./vb6 | ./vbscript"))
			&&	(lLanguageNodes->Count > 0)
			)
		{
			lCppNodes = pXmlNode->SelectNodes ("./cpp | ./cplusplus");
			lCsharpNodes = pXmlNode->SelectNodes ("./csharp");
			lVbNodes = pXmlNode->SelectNodes ("./vbnet");
			lVb6Nodes = pXmlNode->SelectNodes ("./vb6");
			lVbscriptNodes = pXmlNode->SelectNodes ("./vbscript");
			if	(
					(!lVbNodes)
				||	(lVbNodes->Count <= 0)
				)
			{
				lVbNodes = pXmlNode->SelectNodes ("./vb");
			}
			if	(
					(!lVb6Nodes)
				||	(lVb6Nodes->Count <= 0)
				)
			{
				lVb6Nodes = pXmlNode->SelectNodes ("./vb");
			}
			if	(
					(!lVbscriptNodes)
				||	(lVbscriptNodes->Count <= 0)
				)
			{
				lVbscriptNodes = pXmlNode->SelectNodes ("./vb");
			}

			if	(
					(lRet = pXmlNode->OwnerDocument->CreateNode (XmlNodeType::Element, "div", nullptr))
#ifdef	_SYNTAX_TABLE
				&&	(lLanguagesNode = pXmlNode->OwnerDocument->CreateNode (XmlNodeType::Element, "table", nullptr))
#else
				&&	(lLanguagesNode = pXmlNode->OwnerDocument->CreateNode (XmlNodeType::Element, "dl", nullptr))
#endif
				)
			{
#ifdef	_SYNTAX_TABLE
				lLanguagesNode->Attributes->Append (MakeNewAttribute ("id", "Languages", lLanguagesNode->OwnerDocument));
				lLanguagesNode->Attributes->Append (MakeNewAttribute ("class", "syntax", lLanguagesNode->OwnerDocument));
#endif
				lRet->AppendChild (lLanguagesNode);

				if	(mFormatServer)
				{
					FormatSyntaxLanguage (lLanguagesNode, "Visual C++", lCppNodes);
					FormatSyntaxLanguage (lLanguagesNode, "Visual C#", lCsharpNodes);
					FormatSyntaxLanguage (lLanguagesNode, "Visual Basic .NET", lVbNodes);
					FormatSyntaxLanguage (lLanguagesNode, "Visual Basic 6", lVb6Nodes);
					FormatSyntaxLanguage (lLanguagesNode, "VBScript", lVbscriptNodes);
				}
				else
				{
					FormatSyntaxLanguage (lLanguagesNode, "Visual Basic .NET", lVbNodes);
					FormatSyntaxLanguage (lLanguagesNode, "Visual Basic 6", lVb6Nodes);
					FormatSyntaxLanguage (lLanguagesNode, "VBScript", lVbscriptNodes);
					FormatSyntaxLanguage (lLanguagesNode, "Visual C#", lCsharpNodes);
					FormatSyntaxLanguage (lLanguagesNode, "Visual C++", lCppNodes);
				}
			}
		}
		else
		if	(pXmlNode->FirstChild)
		{
			lRet = ChangeNodeName (pXmlNode, "div");
		}

		if	(lRet)
		{
			lRet->Attributes->Append (MakeNewAttribute ("id", "Syntax", lRet->OwnerDocument));
			lRet->Attributes->Append (MakeNewAttribute ("class", "syntax", lRet->OwnerDocument));
		}
	}
	return lRet;
}

XmlNode^ XmlToHtml::FormatSyntaxLanguage (XmlNode^ pLanguagesNode, String^ pLanguageName, XmlNodeList^ pSyntaxNodes)
{
	XmlNode^		lLanguageNode = nullptr;
	XmlDocument^	lXmlDocument = pLanguagesNode->OwnerDocument;

	if	(
			(pSyntaxNodes)
		&&	(pSyntaxNodes->Count > 0)
#ifdef	_SYNTAX_TABLE
		&&	(lLanguageNode = lXmlDocument->CreateNode (XmlNodeType::Element, "tr", nullptr))
#else
		&&	(lLanguageNode = lXmlDocument->CreateNode (XmlNodeType::Element, "dt", nullptr))
#endif
		)
	{
#ifdef	_SYNTAX_TABLE
		pLanguagesNode->AppendChild (lLanguageNode);
		pLanguagesNode = lLanguageNode;
		pLanguagesNode->AppendChild (lLanguageNode = lXmlDocument->CreateNode (XmlNodeType::Element, "td", nullptr));
		lLanguageNode->Attributes->Append (MakeNewAttribute ("align", "left", lXmlDocument));
		lLanguageNode->Attributes->Append (MakeNewAttribute ("width", "200ex", lXmlDocument));
		lLanguageNode->Attributes->Append (MakeNewAttribute ("class", "language", lXmlDocument));
		lLanguageNode->AppendChild (MakeNewNode ("p", pLanguageName, lXmlDocument));
#else
		lLanguageNode->InnerText = pLanguageName;
		pLanguagesNode->AppendChild (lLanguageNode);
#endif

		if	(pSyntaxNodes->Count == 1)
		{
#ifdef	_SYNTAX_TABLE
			pLanguagesNode->AppendChild (FormatInnerXml (ChangeNodeName (pSyntaxNodes[0], "td")));
#else
			pLanguagesNode->AppendChild (FormatInnerXml (ChangeNodeName (pSyntaxNodes[0], "dd")));
#endif
		}
		else
		{
			XmlNode^	lSyntaxNode;

#ifdef	_SYNTAX_TABLE
			pLanguagesNode->AppendChild (lLanguageNode = MakeNewNode ("td", nullptr, lXmlDocument));
#else
			pLanguagesNode->AppendChild (lLanguageNode = MakeNewNode ("dd", nullptr, lXmlDocument));
#endif
			for each (lSyntaxNode in pSyntaxNodes)
			{
				lLanguageNode->AppendChild (FormatInnerXml (CopyNodeInnerXml (lSyntaxNode, lXmlDocument)));
			}
		}
	}
	return lLanguageNode;
}

/////////////////////////////////////////////////////////////////////////////

XmlNode^ XmlToHtml::FormatParameters (XmlNodeList^ pXmlNodeList)
{
	XmlNode^					lRet;
	Generic::List<XmlNode^>^	lNodeList;
	XmlNode^					lNode;
	XmlNode^					lRowNode;
	XmlNode^					lColNode;
	XmlNode^					lNameNode;
	XmlAttribute^				lParamName;

	if	(
			(lNodeList = NodeList (pXmlNodeList))
		&&	(lNode = lNodeList [0])
		&&	(lRet = lNode->OwnerDocument->CreateNode (XmlNodeType::Element, "table", nullptr))
		)
	{
		lRet->Attributes->Append (MakeNewAttribute ("id", "Parameters", lRet->OwnerDocument));
		lRet->Attributes->Append (MakeNewAttribute ("class", "syntax", lRet->OwnerDocument));

		lRet->AppendChild (lRowNode = lRet->OwnerDocument->CreateNode (XmlNodeType::Element, "tr", nullptr));
		lRowNode->AppendChild (lColNode = MakeNewNode ("th", "Part", lRet->OwnerDocument));
		lColNode->Attributes->Append (MakeNewAttribute ("align", "left", lRet->OwnerDocument));
		lColNode->Attributes->Append (MakeNewAttribute ("width", "200ex", lRet->OwnerDocument));
		lRowNode->AppendChild (lColNode = MakeNewNode ("th", "Description", lRet->OwnerDocument));
		lColNode->Attributes->Append (MakeNewAttribute ("align", "left", lRet->OwnerDocument));

		for each (lNode in lNodeList)
		{
			lRet->AppendChild (lRowNode = lRet->OwnerDocument->CreateNode (XmlNodeType::Element, "tr", nullptr));
			lParamName = lNode->Attributes["name"];
			if	(lParamName)
			{
				lRowNode->AppendChild (lColNode = lRet->OwnerDocument->CreateNode (XmlNodeType::Element, "td", nullptr));
				lColNode->AppendChild (lNameNode = MakeNewNode ("em", lParamName->Value, lRet->OwnerDocument));
				lNameNode->Attributes->Append (MakeNewAttribute ("class", "token", lRet->OwnerDocument));
			}
			else
			{
				lRowNode->AppendChild (lColNode = lRet->OwnerDocument->CreateNode (XmlNodeType::Element, "td", nullptr));
			}
			lColNode->Attributes->Append (MakeNewAttribute ("width", "200ex", lRet->OwnerDocument));
			lRowNode->AppendChild (lColNode = lRet->OwnerDocument->CreateNode (XmlNodeType::Element, "td", nullptr));
			lColNode->InnerXml = FormatInnerXml (lNode, "syntax")->InnerXml;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

XmlNode^ XmlToHtml::FormatExamples (XmlNodeList^ pXmlNodeList)
{
	XmlNode^					lRet;
	Generic::List<XmlNode^>^	lNodeList;
	XmlNode^					lNode;

	if	(
			(lNodeList = NodeList (pXmlNodeList))
		&&	(lNode = lNodeList [0])
		&&	(lRet = lNode->OwnerDocument->CreateNode (XmlNodeType::DocumentFragment, nullptr, nullptr))
		)
	{
		for each (lNode in lNodeList)
		{
			lRet->AppendChild (FormatInnerXml (lNode, "example"));
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

XmlNode^ XmlToHtml::FormatSeeAlso (XmlNodeList^ pXmlNodeList)
{
	XmlNode^					lRet;
	Generic::List<XmlNode^>^	lNodeList;
	XmlNode^					lNode;
	bool						lFirstNode = true;

	if	(
			(lNodeList = NodeList (pXmlNodeList))
		&&	(lNode = lNodeList [0])
		&&	(lRet = lNode->OwnerDocument->CreateNode (XmlNodeType::DocumentFragment, nullptr, nullptr))
		)
	{
		for each (lNode in lNodeList)
		{
			if	(lNode = FormatSee (lNode))
			{
				if	(lFirstNode)
				{
					lFirstNode = false;
				}
				else
				{
					lRet->AppendChild (lNode->OwnerDocument->CreateNode (XmlNodeType::Element, "br", nullptr));
				}
				lRet->AppendChild (lNode);
			}
		}
	}
	return lRet;
}

XmlNode^ XmlToHtml::FormatSee (XmlNode^ pXmlNode)
{
	XmlNode^	lRet;

	if	(pXmlNode)
	{
		try
		{
			String^				lRefName;
			Type^				lRefType;
			MethodInfo^			lRefMethod;
			PropertyInfo^		lRefProperty;
			EventInfo^			lRefEvent;
			FieldInfo^			lRefField;
			String^				lRefValue;
			XmlNode^			lRefNode;
			XmlAttribute^		lRefAttribute;

			try
			{
				lRefName = pXmlNode->Attributes["cref"]->Value;
			}
			catch (System::Exception^ pException)
			{
				LogMessage (LogNormal, _T("Invalid See [%s]"), _B(pXmlNode->OuterXml));
				throw pException;
			}

			if	(lRefType = FindAssemblyType (lRefName))
			{
				lRefName = lRefType->Name;
				lRefValue = TypeFileName (lRefType);
			}
			else
			if	(lRefMethod = FindAssemblyMethod (lRefName))
			{
				lRefName = lRefMethod->Name;
				lRefValue = MethodFileName (lRefMethod);
			}
			else
			if	(lRefProperty = FindAssemblyProperty (lRefName))
			{
				lRefName = lRefProperty->Name;
				lRefValue = PropertyFileName (lRefProperty);
			}
			else
			if	(IsAllPropertiesName (lRefName))
			{
				lRefValue = AllPropertiesFileName (lRefName);
				lRefName = AllPropertiesName (lRefName);
			}
			else
			if	(lRefEvent = FindAssemblyEvent (lRefName))
			{
				lRefName = lRefEvent->Name;
				lRefValue = EventFileName (lRefEvent);
			}
			else
			if	(lRefField = FindAssemblyField (lRefName))
			{
				lRefName = lRefField->Name;
				lRefValue = FieldFileName (lRefField);
			}
			else
			if	(IsAllFieldsName (lRefName))
			{
				lRefValue = AllFieldsFileName (lRefName);
				lRefName = AllFieldsName (lRefName);
			}
			else
			{
				XmlNode^	lMemberNode;
				String^		lMemberName = "unknown";

				for	(lMemberNode = pXmlNode; lMemberNode; lMemberNode = lMemberNode->ParentNode)
				{
					if	(lMemberNode->Name == "member")
					{
						break;
					}
				}
				if	(
						(lMemberNode)
					&&	(lMemberNode->Attributes["name"])
					)
				{
					lMemberName = lMemberNode->Attributes["name"]->Value;
				}
				LogMessage (LogNormal, _T("  Unknown Reference [%s] in [%s]"), _B(pXmlNode->OuterXml), _B(lMemberName));
				Console::WriteLine ("Unknown Reference {0} in {1}", pXmlNode->OuterXml, lMemberName);
			}

			if	(lRefValue)
			{
				if	(lRefAttribute = pXmlNode->Attributes["text"])
				{
					lRefName = lRefAttribute->Value;
				}
				if	(lRefAttribute = pXmlNode->Attributes["prefix"])
				{
					lRefName = lRefAttribute->Value->TrimEnd() + " " + lRefName;
				}
				if	(lRefAttribute = pXmlNode->Attributes["suffix"])
				{
					lRefName = lRefName + " " + lRefAttribute->Value->TrimStart();
				}
				if	(lRefAttribute = pXmlNode->Attributes["bookmark"])
				{
					lRefValue = lRefValue + "#" + lRefAttribute->Value->Trim();
				}

				lRefNode = pXmlNode->OwnerDocument->CreateNode (XmlNodeType::Element, "a", nullptr);
				lRefNode->Attributes->Append (MakeNewAttribute ("href", lRefValue, pXmlNode->OwnerDocument));
				lRefNode->AppendChild (pXmlNode->OwnerDocument->CreateTextNode (lRefName));

				lRet = lRefNode;
			}
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

XmlNode^ XmlToHtml::FormatList (XmlNode^ pXmlNode, String^ pCodeClass)
{
	XmlNode^		lRet;
	XmlAttribute^	lListType;

	if	(pXmlNode)
	{
		if	(lListType = pXmlNode->Attributes["type"])
		{
			if	(lListType->Value == "bullet")
			{
				lRet = FormatListBullet (pXmlNode);
			}
			else
			if	(lListType->Value == "number")
			{
				lRet = FormatListNumber (pXmlNode);
			}
			else
			if	(lListType->Value == "table")
			{
				lRet = FormatListTable (pXmlNode);
			}
			else
			{
				LogMessage (LogNormal, _T("Unknown list type [%s] [%s]"), _B(lListType->Value), _B(pXmlNode->InnerXml));
			}
		}
		else
		{
			lRet = FormatListDefault (pXmlNode);
		}

		if	(lRet)
		{
			lRet->InnerXml = FormatInnerXml (lRet, pCodeClass)->InnerXml;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

XmlNode^ XmlToHtml::FormatListDefault (XmlNode^ pXmlNode)
{
	XmlNode^		lRet;
	XmlAttribute^	lAttribute;
	XmlNodeList^	lHeaderNodes;
	XmlNodeList^	lItemNodes;
	XmlNode^		lItemNode;
	XmlNode^		lTermNode;
	XmlNode^		lDescriptionNode;

	if	(
			(lHeaderNodes = pXmlNode->SelectNodes ("child::listheader"))
		&&	(lHeaderNodes->Count > 0)
		)
	{
		LogMessage (LogNormal, _T("<listheader> in non-table list [%s]"), _B(pXmlNode->InnerXml));
	}
	if	(
			(lItemNodes = pXmlNode->SelectNodes ("child::item"))
		&&	(lItemNodes->Count > 0)
		)
	{
		lRet = pXmlNode->OwnerDocument->CreateNode (XmlNodeType::Element, "dl", nullptr);

		if	(lAttribute = pXmlNode->Attributes ["class"])
		{
			lRet->Attributes->Append (MakeNewAttribute ("class", lAttribute->Value, lRet->OwnerDocument));
		}

		for each (lItemNode in lItemNodes)
		{
			if	(lTermNode = lItemNode ["term"])
			{
				lRet->AppendChild (ChangeNodeName (lTermNode, "dt"));
			}
			if	(lDescriptionNode = lItemNode ["description"])
			{
				lRet->AppendChild (ChangeNodeName (lDescriptionNode, "dd"));
			}
			if	(
					(!lTermNode)
				&&	(!lDescriptionNode)
				)
			{
				lRet->AppendChild (ChangeNodeName (lItemNode, "dd"));
			}
		}
	}

	return lRet;
}

XmlNode^ XmlToHtml::FormatListBullet (XmlNode^ pXmlNode)
{
	XmlNode^		lRet;
	XmlAttribute^	lAttribute;
	XmlNodeList^	lHeaderNodes;
	XmlNodeList^	lItemNodes;
	XmlNodeList^	lDescriptionNodes;
	XmlNode^		lItemNode;
	XmlNode^		lTermNode;
	XmlNode^		lDescriptionNode;

	if	(
			(lHeaderNodes = pXmlNode->SelectNodes ("child::listheader"))
		&&	(lHeaderNodes->Count > 0)
		)
	{
		LogMessage (LogNormal, _T("<listheader> in non-table list [%s]"), _B(pXmlNode->InnerXml));
	}
	if	(
			(lItemNodes = pXmlNode->SelectNodes ("child::item"))
		&&	(lItemNodes->Count > 0)
		)
	{
		lRet = pXmlNode->OwnerDocument->CreateNode (XmlNodeType::Element, "ul", nullptr);

		if	(lAttribute = pXmlNode->Attributes ["class"])
		{
			lRet->Attributes->Append (MakeNewAttribute ("class", lAttribute->Value, lRet->OwnerDocument));
		}

		for each (lItemNode in lItemNodes)
		{
			if	(lTermNode = lItemNode ["term"])
			{
				if	(
						(lDescriptionNodes = lItemNode->SelectNodes ("child::description"))
					&&	(lDescriptionNodes->Count > 1)
					)
				{
					lItemNode = ChangeNodeName (lTermNode, "li");
					for each (lDescriptionNode in lDescriptionNodes)
					{
						lItemNode->AppendChild (ChangeNodeName (lDescriptionNode, "p"));
					}
					lRet->AppendChild (lItemNode);
				}
				else
				if	(lDescriptionNode = lItemNode ["description"])
				{
					lItemNode = ChangeNodeName (lTermNode, "li");
					lItemNode->AppendChild (ChangeNodeName (lDescriptionNode, "p"));
					lRet->AppendChild (lItemNode);
				}
				else
				{
					lRet->AppendChild (ChangeNodeName (lTermNode, "li"));
				}
			}
			else
			if	(
					(lDescriptionNodes = lItemNode->SelectNodes ("child::description"))
				&&	(lDescriptionNodes->Count > 1)
				)
			{
				lItemNode = nullptr;
				for each (lDescriptionNode in lDescriptionNodes)
				{
					if	(lItemNode)
					{
						lItemNode->AppendChild (ChangeNodeName (lDescriptionNode, "p"));
					}
					else
					{
						lItemNode = ChangeNodeName (lDescriptionNode, "li");
					}
				}
				lRet->AppendChild (lItemNode);
			}
			else
			if	(lDescriptionNode = lItemNode ["description"])
			{
				lRet->AppendChild (ChangeNodeName (lDescriptionNode, "li"));
			}
			else
			{
				lRet->AppendChild (ChangeNodeName (lItemNode, "li"));
			}
		}
	}

	return lRet;
}

XmlNode^ XmlToHtml::FormatListNumber (XmlNode^ pXmlNode)
{
	XmlNode^		lRet;
	XmlAttribute^	lAttribute;
	XmlNodeList^	lHeaderNodes;
	XmlNodeList^	lItemNodes;
	XmlNodeList^	lDescriptionNodes;
	XmlNode^		lItemNode;
	XmlNode^		lTermNode;
	XmlNode^		lDescriptionNode;

	if	(
			(lHeaderNodes = pXmlNode->SelectNodes ("child::listheader"))
		&&	(lHeaderNodes->Count > 0)
		)
	{
		LogMessage (LogNormal, _T("<listheader> in non-table list [%s]"), _B(pXmlNode->InnerXml));
	}
	if	(
			(lItemNodes = pXmlNode->SelectNodes ("child::item"))
		&&	(lItemNodes->Count > 0)
		)
	{
		lRet = pXmlNode->OwnerDocument->CreateNode (XmlNodeType::Element, "ol", nullptr);

		if	(lAttribute = pXmlNode->Attributes ["class"])
		{
			lRet->Attributes->Append (MakeNewAttribute ("class", lAttribute->Value, lRet->OwnerDocument));
		}

		for each (lItemNode in lItemNodes)
		{
			if	(lTermNode = lItemNode ["term"])
			{
				if	(
						(lDescriptionNodes = lItemNode->SelectNodes ("child::description"))
					&&	(lDescriptionNodes->Count > 0)
					)
				{
					lItemNode = ChangeNodeName (lTermNode, "li");
					for each (lDescriptionNode in lDescriptionNodes)
					{
						lItemNode->AppendChild (ChangeNodeName (lDescriptionNode, "p"));
					}
					lRet->AppendChild (lItemNode);
				}
				else
				if	(lDescriptionNode = lItemNode ["description"])
				{
					lItemNode = ChangeNodeName (lTermNode, "li");
					lItemNode->AppendChild (ChangeNodeName (lDescriptionNode, "p"));
					lRet->AppendChild (lItemNode);
				}
				else
				{
					lRet->AppendChild (ChangeNodeName (lTermNode, "li"));
				}
			}
			else
			{
				if	(
						(lDescriptionNodes = lItemNode->SelectNodes ("child::description"))
					&&	(lDescriptionNodes->Count > 1)
					)
				{
					lItemNode = nullptr;
					for each (lDescriptionNode in lDescriptionNodes)
					{
						if	(lItemNode)
						{
							lItemNode->AppendChild (ChangeNodeName (lDescriptionNode, "p"));
						}
						else
						{
							lItemNode = ChangeNodeName (lDescriptionNode, "li");
						}
					}
					lRet->AppendChild (lItemNode);
				}
				if	(lDescriptionNode = lItemNode ["description"])
				{
					lRet->AppendChild (ChangeNodeName (lDescriptionNode, "li"));
				}
				else
				{
					lRet->AppendChild (ChangeNodeName (lItemNode, "li"));
				}
			}
		}
	}

	return lRet;
}

XmlNode^ XmlToHtml::FormatListTable (XmlNode^ pXmlNode)
{
	XmlNode^		lRet;
	XmlAttribute^	lAttribute;
	XmlNodeList^	lHeaderNodes;
	XmlNodeList^	lItemNodes;
	XmlNode^		lItemNode;
	XmlNode^		lTermNode;
	XmlNode^		lDescriptionNode;
	XmlNode^		lRowNode;
	XmlNode^		lColNode;

	lRet = pXmlNode->OwnerDocument->CreateNode (XmlNodeType::Element, "table", nullptr);

	if	(lAttribute = pXmlNode->Attributes ["class"])
	{
		lRet->Attributes->Append (MakeNewAttribute ("class", lAttribute->Value, lRet->OwnerDocument));
	}

	if	(
			(lHeaderNodes = pXmlNode->SelectNodes ("child::listheader"))
		&&	(lHeaderNodes->Count > 0)
		)
	{
		for each (lItemNode in lHeaderNodes)
		{
			lRet->AppendChild (lRowNode = lRet->OwnerDocument->CreateNode (XmlNodeType::Element, "tr", nullptr));

			if	(lTermNode = lItemNode ["term"])
			{
				lRowNode->AppendChild (lColNode = ChangeNodeName (lTermNode, "th"));
				CopyNodeAttributes (lTermNode, lColNode);
			}
			if	(lDescriptionNode = lItemNode ["description"])
			{
				lRowNode->AppendChild (lColNode = ChangeNodeName (lDescriptionNode, "th"));
				CopyNodeAttributes (lDescriptionNode, lColNode);
			}
		}
	}

	if	(
			(lItemNodes = pXmlNode->SelectNodes ("child::item"))
		&&	(lItemNodes->Count > 0)
		)
	{
		for each (lItemNode in lItemNodes)
		{
			lRet->AppendChild (lRowNode = lRet->OwnerDocument->CreateNode (XmlNodeType::Element, "tr", nullptr));

			if	(lTermNode = lItemNode ["term"])
			{
				lRowNode->AppendChild (lColNode = ChangeNodeName (lTermNode, "td"));
				CopyNodeAttributes (lTermNode, lColNode);
			}
			if	(lDescriptionNode = lItemNode ["description"])
			{
				lRowNode->AppendChild (lColNode = ChangeNodeName (lDescriptionNode, "td"));
				CopyNodeAttributes (lDescriptionNode, lColNode);
			}
		}
	}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

String^ XmlToHtml::PutMemberFile (String^ pFileName, XmlElement^ pRootElement)
{
	String^				lRet = nullptr;
	String^				lFileName = System::IO::Path::Combine (mHtmlPath, pFileName);
	XmlWriterSettings^	lWriterSettings;
	XmlWriter^			lWriter;

	System::IO::File::Delete (lFileName);

	if	(lWriterSettings = gcnew XmlWriterSettings ())
	{
		lWriterSettings->Encoding = Encoding::UTF8;
		lWriterSettings->Indent = true;
		lWriterSettings->IndentChars = "\t";
		lWriterSettings->NewLineHandling = Xml::NewLineHandling::Replace;
		lWriterSettings->NewLineOnAttributes = false;
		lWriterSettings->OmitXmlDeclaration = true;
	}
	if	(
			(lWriterSettings)
		&&	(lWriter = XmlWriter::Create (lFileName, lWriterSettings))
		)
	{
		try
		{
			pRootElement->Normalize ();
			pRootElement->OwnerDocument->DocumentType->WriteTo (lWriter);
			pRootElement->WriteTo (lWriter);
			lWriter->Close ();
			lRet = lFileName;
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

String^ XmlToHtml::MemberFileName (String^ pMemberName)
{
	return System::IO::Path::GetExtension (mSourceAssembly->GetName()->Name)->Substring(1) + "_" + pMemberName + ".htm";
}

String^ XmlToHtml::TypeFileName (System::Type^ pAssemblyType)
{
	return "T_" + pAssemblyType->Namespace->Replace('.','_') + "_" + pAssemblyType->Name + ".htm";
}

String^ XmlToHtml::MethodFileName (System::Reflection::MethodInfo^ pAssemblyMethod)
{
	return "M_" + pAssemblyMethod->DeclaringType->Namespace->Replace('.','_') + "_" + pAssemblyMethod->DeclaringType->Name + "_" + pAssemblyMethod->Name + ".htm";
}

String^ XmlToHtml::MethodFileName (System::Reflection::ConstructorInfo^ pAssemblyConstructor)
{
	return "M_" + pAssemblyConstructor->DeclaringType->Namespace->Replace('.','_') + "_" + pAssemblyConstructor->DeclaringType->Name + "_ctor" + ".htm";
}

String^ XmlToHtml::PropertyFileName (System::Reflection::PropertyInfo^ pAssemblyProperty)
{
	return "P_" + pAssemblyProperty->DeclaringType->Namespace->Replace('.','_') + "_" + pAssemblyProperty->DeclaringType->Name + "_" + pAssemblyProperty->Name + ".htm";
}

String^ XmlToHtml::FieldFileName (System::Reflection::FieldInfo^ pAssemblyField)
{
	return "F_" + pAssemblyField->DeclaringType->Namespace->Replace('.','_') + "_" + pAssemblyField->DeclaringType->Name + "_" + pAssemblyField->Name + ".htm";
}

String^ XmlToHtml::EventFileName (System::Reflection::EventInfo^ pAssemblyEvent)
{
	return "E_" + pAssemblyEvent->DeclaringType->Namespace->Replace('.','_') + "_" + pAssemblyEvent->DeclaringType->Name + "_" + pAssemblyEvent->Name + ".htm";
}

/////////////////////////////////////////////////////////////////////////////

bool XmlToHtml::IsAllPropertiesName (String^ pMemberName)
{
	bool			lRet = false;
	array<String^>^	lNameParts;

	if	(
			(pMemberName)
		&&	(pMemberName->StartsWith ("P:"+mSourceAssembly->GetName()->Name))
		&&	(lNameParts = pMemberName->Substring(2)->Split ('.'))
		&&	(lNameParts->Length == 3)
		)
	{
		lRet = true;
	}
	return lRet;
}

String^ XmlToHtml::AllPropertiesName (String^ pMemberName)
{
	array<String^>^	lNameParts = pMemberName->Substring(2)->Split ('.');
	return String::Format ("{0} Object Properties", lNameParts[2]);
}

String^ XmlToHtml::AllPropertiesFileName (String^ pMemberName)
{
	array<String^>^	lNameParts = pMemberName->Substring(2)->Split ('.');
	return MemberFileName (String::Format ("{0}_Properties", lNameParts[2]));
}

/////////////////////////////////////////////////////////////////////////////

bool XmlToHtml::IsAllFieldsName (String^ pMemberName)
{
	bool			lRet = false;
	array<String^>^	lNameParts;

	if	(
			(pMemberName)
		&&	(pMemberName->StartsWith ("F:"))
		&&	(lNameParts = pMemberName->Substring(2)->Split ('.'))
		&&	(lNameParts->Length == 3)
		)
	{
		lRet = true;
	}
	return lRet;
}

String^ XmlToHtml::AllFieldsName (String^ pMemberName)
{
	array<String^>^	lNameParts = pMemberName->Substring(2)->Split ('.');
	return String::Format ("{0} Object Fields", lNameParts[2]);
}

String^ XmlToHtml::AllFieldsFileName (String^ pMemberName)
{
	array<String^>^	lNameParts = pMemberName->Substring(2)->Split ('.');
	return MemberFileName (String::Format ("{0}_Fields", lNameParts[2]));
}

/////////////////////////////////////////////////////////////////////////////

bool XmlToHtml::IsInheritDoc (XmlNode^ pXmlNode)
{
	bool		lRet = false;
	XmlNode^	lChildNode;

	if	(
			(pXmlNode)
		&&	(pXmlNode->ChildNodes)
		)
	{
		lRet = true;
		
		for each (lChildNode in pXmlNode->ChildNodes)
		{
			if	(
					(lChildNode->NodeType == XmlNodeType::Element)
				&&	(lChildNode->Name != "inheritdoc")
				)
			{
				lRet = false;
				break;
			}
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool XmlToHtml::CopyXmlToXml (XmlDocument^ pXmlDocument, System::Reflection::Assembly^ pAssembly)
{
	bool				lRet = false;
	XmlFormatWriter^	lWriter;

	pXmlDocument->Normalize ();
	LoadAssemblyMembers (pAssembly);

	RemoveMemberDetails (pXmlDocument);
	FixListFormats (pXmlDocument);
	RemoveCompositeMembers (pXmlDocument);
	AdjustXmlFormat (pXmlDocument);
	FixMethodSignatures (pXmlDocument);
	FixMemberReferences (pXmlDocument);

	if	(lWriter = gcnew XmlFormatWriter)
	{
		lWriter->mOutputPath = mOutputPath;

		if	(
				(mOutputIntellisense)
			||	(mOutputSandcastle)
			)
		{
			lWriter->mEndParagraphs	= true;
			lWriter->mIndentLines = false;
			lWriter->mKeepDocType = false;
			lWriter->mKeepComments = false;
			lWriter->mMaxLineLength = 0;
		}
		else
		{
			lWriter->mEndParagraphs	= false;
			lWriter->mIndentLines = true;
			lWriter->mKeepDocType = true;
			lWriter->mKeepComments = true;
			lWriter->mMaxLineLength = 100;
		}

		if	(lWriter->WriteDocument (pXmlDocument))
		{
#ifdef	_DEBUG_MEMBER_FILES
			LogMessage (_DEBUG_MEMBER_FILES, _T("Wrote [%s]"), _B(mOutputPath));
#endif
			Console::WriteLine ("Wrote {0}", mOutputPath);
			lRet = true;
		}
		else
		{
#ifdef	_DEBUG_MEMBER_FILES
			LogMessage (_DEBUG_MEMBER_FILES, _T("Write [%s] failed"), _B(mOutputPath));
#endif
			Console::WriteLine ("Write {0} failed", mOutputPath);
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void XmlToHtml::FixMethodSignatures (XmlDocument^ pXmlDocument)
{
	if	(
			(mOutputIntellisense)
		||	(mOutputSandcastle)
		)
	{
		Generic::List<XmlNode^>^	lMemberNodes;
		XmlNode^					lMemberNode;
		String^						lMemberName;

		if	(
				(mSourceMethods)
			&&	(lMemberNodes = NodeList (pXmlDocument->DocumentElement->SelectNodes ("members/member")))
			)
		{
			for each (lMemberNode in lMemberNodes)
			{
				lMemberName = lMemberNode->Attributes["name"]->Value;
#ifdef	_DEBUG_XML_MEMBERS
				LogMessage (_DEBUG_XML_MEMBERS, _T("  Member [%s]"), _B(lMemberName));
#endif
				if	(lMemberName->StartsWith ("M:"))
				{
					Generic::KeyValuePair<String^, MethodInfo^>^	lMethod;

					for each (lMethod in mSourceMethods)
					{
						if	(
								(lMethod->Key->StartsWith (lMemberName))
							&&	(!lMethod->Key->EndsWith ("()"))
							)
						{
							lMemberNode->Attributes["name"]->Value = lMethod->Key;
							break;
						}
					}
				}
				else
				if	(
						(mSourceProperties)
					&&	(lMemberName->StartsWith ("P:"))
					)
				{
					PropertyInfo^			lProperty;
					MethodInfo^				lAccessor;
					array<ParameterInfo^>^	lParameters;

					if	(
							(lProperty = FindAssemblyProperty (lMemberName))
						&&	(lAccessor = lProperty->GetGetMethod ())
						&&	(lParameters = lAccessor->GetParameters ())
						&&	(lParameters->Length > 0)
						)
					{
						Generic::KeyValuePair<String^, MethodInfo^>^	lMethod;
						int												lStringStart;
						
						for each (lMethod in mSourceMethods)
						{
							if	(
									(lMethod->Value->Name == lAccessor->Name)
								&&	(lMethod->Value->DeclaringType->FullName == lAccessor->DeclaringType->FullName)
								&&	((lStringStart = lMethod->Key->IndexOf ('(')) > 0)
								)
							{
								lMemberNode->Attributes["name"]->Value += lMethod->Key->Substring (lStringStart, lMethod->Key->Length-lStringStart);
								break;
							}
						}
					}
				}
			}
		}
	}
}

void XmlToHtml::FixMemberReferences (XmlDocument^ pXmlDocument)
{
	if	(
			(mOutputIntellisense)
		||	(mOutputSandcastle)
		)
	{
		Generic::List<XmlNode^>^	lNodeList;
		XmlNode^					lNode;
		String^						lAssemblyName = mSourceAssembly->GetName()->Name;

		if	(lNodeList = NodeList (pXmlDocument->DocumentElement->SelectNodes (".//see | .//seealso")))
		{
			for each (lNode in lNodeList)
			{
				if	(
						(mOutputSandcastle)
					&&	(mSourceMethods)
					)
				{
					try
					{
						XmlAttribute^	lRefName;

						if	(
								(lRefName = lNode->Attributes ["cref"])
							&&	(lRefName->Value->StartsWith ("M:"))
							)
						{
							Generic::KeyValuePair<String^, MethodInfo^>^	lMethod;

							for each (lMethod in mSourceMethods)
							{
								if	(
										(lMethod->Key->StartsWith (lRefName->Value))
									&&	(!lMethod->Key->EndsWith ("()"))
									)
								{
									lRefName->Value = lMethod->Key;
									break;
								}
							}
						}
					}
					catch AnyExceptionSilent
				}

				if	(
						(mOutputSandcastle)
					&&	(mSourceProperties)
					)
				{
					try
					{
						XmlAttribute^			lRefName;
						PropertyInfo^			lProperty;
						MethodInfo^				lAccessor;
						array<ParameterInfo^>^	lParameters;

						if	(
								(lRefName = lNode->Attributes ["cref"])
							&&	(lRefName->Value->StartsWith ("P:"))
							&&	(lProperty = FindAssemblyProperty (lRefName->Value))
							&&	(lAccessor = lProperty->GetGetMethod ())
							&&	(lParameters = lAccessor->GetParameters ())
							&&	(lParameters->Length > 0)
							)
						{
							Generic::KeyValuePair<String^, MethodInfo^>^	lMethod;
							int												lStringStart;

							for each (lMethod in mSourceMethods)
							{
								if	(
										(lMethod->Value->Name == lAccessor->Name)
									&&	(lMethod->Value->DeclaringType->FullName == lAccessor->DeclaringType->FullName)
									&&	((lStringStart = lMethod->Key->IndexOf ('(')) > 0)
									)
								{
									lRefName->Value += lMethod->Key->Substring (lStringStart, lMethod->Key->Length-lStringStart);
									break;
								}
							}
						}
					}
					catch AnyExceptionSilent
				}

				if	(mOutputIntellisense)
				{
					try
					{
						XmlAttribute^	lRefName;

						if	(
								(lRefName = lNode->Attributes ["cref"])
							&&	(lRefName->Value [1] == ':')
							&&	(lRefName->Value->Substring (2)->StartsWith (lAssemblyName))
							)
						{
							lRefName->Value = lRefName->Value->Substring(0,2) + lRefName->Value->Substring(lAssemblyName->Length+3,lRefName->Value->Length-lAssemblyName->Length-3);
						}
					}
					catch AnyExceptionSilent
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

void XmlToHtml::FixListFormats (System::Xml::XmlDocument^ pXmlDocument)
{
	if	(mOutputIntellisense)
	{
		try
		{
			Generic::List<XmlNode^>^	lParamNodes;
			XmlNode^					lParamNode;
			Generic::List<XmlNode^>^	lListNodes;
			XmlNode^					lListNode;
			Generic::List<XmlNode^>^	lItemNodes;
			XmlNode^					lItemNode;
			XmlNode^					lTermNode;
			XmlNode^					lDescNode;
			String^						lInnerXml;

			if	(lParamNodes = NodeList (pXmlDocument->DocumentElement->SelectNodes (".//param | .//returns | .//value | .//remarks")))
			{
				for each (lParamNode in lParamNodes)
				{
					if	(lListNodes = NodeList (lParamNode->SelectNodes (".//list")))
					{
						for each (lListNode in lListNodes)
						{
							if	(lItemNodes = NodeList (lListNode->SelectNodes ("./item")))
							{
								for each (lItemNode in lItemNodes)
								{
									lTermNode = lItemNode->SelectSingleNode ("term");
									lDescNode = lItemNode->SelectSingleNode ("description");

									if	(
											(lTermNode)
										&&	(lDescNode)
										)
									{
										lInnerXml = lTermNode->InnerXml + " <b>…</b> " + lDescNode->InnerXml;
									}
									else
									if	(lTermNode)
									{
										lInnerXml = lTermNode->InnerXml;
									}
									else
									if	(lDescNode)
									{
										lInnerXml = lDescNode->InnerXml;
									}
									else
									{
										lInnerXml = lItemNode->InnerXml;
									}
									lInnerXml = "<para/>" + lInnerXml;
									lListNode->ReplaceChild (MakeNewNode (lInnerXml, pXmlDocument), lItemNode);
								}
							}

							lListNode->ParentNode->ReplaceChild (CopyNodeInnerXml (lListNode, pXmlDocument), lListNode);
						}
					}
				}
			}
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////

void XmlToHtml::RemoveMemberDetails (System::Xml::XmlDocument^ pXmlDocument)
{
	if	(
			(mOutputIntellisense)
		||	(mOutputSandcastle)
		)
	{
		try
		{
			Generic::List<XmlNode^>^	lDetailsNodes;
			XmlNode^					lDetailsNode;

			if	(
					(mOutputIntellisense)
				?	(lDetailsNodes = NodeList (pXmlDocument->DocumentElement->SelectNodes (".//syntax | .//detail | .//details | .//example | .//seealso")))
				:	(lDetailsNodes = NodeList (pXmlDocument->DocumentElement->SelectNodes (".//syntax | .//detail | .//details")))
				)
			{
				for each (lDetailsNode in lDetailsNodes)
				{
					lDetailsNode->ParentNode->RemoveChild (lDetailsNode);
				}
			}
		}
		catch AnyExceptionDebug
	}
}

void XmlToHtml::RemoveCompositeMembers (System::Xml::XmlDocument^ pXmlDocument)
{
	if	(
			(mOutputIntellisense)
		||	(mOutputSandcastle)
		)
	{
		try
		{
			Generic::List<XmlNode^>^	lMemberNodes;
			XmlNode^					lMemberNode;
			String^						lMemberName;

			if	(lMemberNodes = NodeList (pXmlDocument->DocumentElement->SelectNodes ("members/member")))
			{
				for each (lMemberNode in lMemberNodes)
				{
					lMemberName = lMemberNode->Attributes["name"]->Value;

					if	(
							(IsAllPropertiesName (lMemberName))
						||	(IsAllFieldsName (lMemberName))
						)
					{
						lMemberNode->ParentNode->RemoveChild (lMemberNode);
					}
				}
			}
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////

void XmlToHtml::AdjustXmlFormat (System::Xml::XmlDocument^ pXmlDocument)
{
	if	(
			(mOutputIntellisense)
		||	(mOutputSandcastle)
		)
	{
		try
		{
			Generic::List<XmlNode^>^	lMemberNodes;
			XmlNode^					lMemberNode;
			XmlAttribute^				lMemberName;
			XmlNode^					lSummaryNode;
			XmlNode^					lRemarksNode;
			Generic::List<XmlNode^>^	lParamNodes;
			XmlNode^					lParamNode;
			Generic::List<XmlNode^>^	lSeeNodes;
			XmlNode^					lSeeNode;
			Generic::List<XmlNode^>^	lNodes;
			XmlNode^					lNode;
			XmlAttribute^				lAttribute;

			if	(lMemberNodes = NodeList (pXmlDocument->DocumentElement->SelectNodes ("members/member")))
			{
				for each (lMemberNode in lMemberNodes)
				{
					if	(lMemberName = lMemberNode->Attributes["name"])
					{
						if	(lSummaryNode = lMemberNode->SelectSingleNode ("summary"))
						{
							if	(
									(lMemberName->Value->StartsWith ("M:"))
								||	(lMemberName->Value->StartsWith ("P:"))
								||	(lMemberName->Value->StartsWith ("E:"))
								||	(lMemberName->Value->StartsWith ("F:"))
								)
							{
								if	(lNodes = NodeList (lSummaryNode->SelectNodes ("para")))
								{
									for each (lNode in lNodes)
									{
										if	(lNode->FirstChild != nullptr)
										{
											lSummaryNode->ReplaceChild (CopyNodeInnerXml (lNode, lNode->OwnerDocument), lNode);
										}
									}
								}
							}
							else
							if	(mOutputSandcastle)
							{
								if	(
										(lNodes = NodeList (lSummaryNode->SelectNodes ("para")))
									&&	(lNode = lNodes [0])
									&&	(lNode->FirstChild != nullptr)
									)
								{
									lSummaryNode->ReplaceChild (CopyNodeInnerXml (lNode, lNode->OwnerDocument), lNode);
								}
							}
							
							if	(lNodes = NodeList (lSummaryNode->SelectNodes (".//c")))
							{
								for each (lNode in lNodes)
								{
									lNode->ParentNode->ReplaceChild (ChangeNodeName (lNode, "b"), lNode);
								}
							}
						}

						if	(lRemarksNode = lMemberNode->SelectSingleNode ("remarks"))
						{
							if	(lNodes = NodeList (lRemarksNode->SelectNodes (".//c")))
							{
								for each (lNode in lNodes)
								{
									lNode->ParentNode->ReplaceChild (ChangeNodeName (lNode, "b"), lNode);
								}
							}
						}

						if	(
								(mOutputIntellisense)
							&&	(lParamNodes = NodeList (lMemberNode->SelectNodes ("value")))
							)
						{
							for each (lParamNode in lParamNodes)
							{
								if	(lParamNode->Attributes["name"])
								{
									lParamNode->ParentNode->ReplaceChild (ChangeNodeName (lParamNode, "returns"), lParamNode);
								}
							}
						}

						if	(lParamNodes = NodeList (lMemberNode->SelectNodes ("param | returns | value")))
						{
							for each (lParamNode in lParamNodes)
							{
								if	(lNodes = NodeList (lParamNode->SelectNodes (".//c")))
								{
									for each (lNode in lNodes)
									{
										lNode->ParentNode->ReplaceChild (ChangeNodeName (lNode, "b"), lNode);
									}
								}
							}
						}

						if	(lSeeNodes = NodeList (lMemberNode->SelectNodes (".//see")))
						{
							for each (lSeeNode in lSeeNodes)
							{
								if	(lAttribute = lSeeNode->Attributes["prefix"])
								{
									lSeeNode->ParentNode->InsertBefore (MakeNewNode (lAttribute->Value+" ", lSeeNode->OwnerDocument), lSeeNode);
									lSeeNode->Attributes->Remove (lAttribute);
								}
								if	(lAttribute = lSeeNode->Attributes["suffix"])
								{
									lSeeNode->ParentNode->InsertAfter (MakeNewNode (" "+lAttribute->Value, lSeeNode->OwnerDocument), lSeeNode);
									lSeeNode->Attributes->Remove (lAttribute);
								}
								if	(lAttribute = lSeeNode->Attributes["text"])
								{
									if	(mOutputIntellisense)
									{
										lSeeNode->ParentNode->ReplaceChild (MakeNewNode ("&quot;"+lAttribute->Value+"&quot;", lSeeNode->OwnerDocument), lSeeNode);
									}
									else
									{
										lSeeNode->Attributes->Remove (lAttribute);
									}
								}
								if	(lAttribute = lSeeNode->Attributes["class"])
								{
									lSeeNode->Attributes->Remove (lAttribute);
								}
							}
						}

						if	(lSeeNodes = NodeList (lMemberNode->SelectNodes (".//seealso")))
						{
							for each (lSeeNode in lSeeNodes)
							{
								if	(lAttribute = lSeeNode->Attributes["prefix"])
								{
									lSeeNode->Attributes->Remove (lAttribute);
								}
								if	(lAttribute = lSeeNode->Attributes["suffix"])
								{
									lSeeNode->Attributes->Remove (lAttribute);
								}
								if	(lAttribute = lSeeNode->Attributes["text"])
								{
									lSeeNode->Attributes->Remove (lAttribute);
								}
							}
						}
					}
				}
			}

			if	(lNodes = NodeList (pXmlDocument->DocumentElement->SelectNodes ("//a")))
			{
				for each (lNode in lNodes)
				{
					if	(lAttribute = lNode->Attributes["class"])
					{
						lNode->Attributes->Remove (lAttribute);
					}
				}
			}

			if	(lNodes = NodeList (pXmlDocument->DocumentElement->SelectNodes ("//keyword | //token | //typename | //filename | //self | //literal | //glossary")))
			{
				for each (lNode in lNodes)
				{
					lNode->ParentNode->ReplaceChild (CopyNodeInnerXml (lNode, lNode->OwnerDocument), lNode);
				}
			}
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////

void XmlToHtml::ResolveEntities (XmlDocument^ pXmlDocument)
{
	ResolveEntities (pXmlDocument->DocumentElement);
}

void XmlToHtml::ResolveEntities (XmlNode^ pXmlNode)
{
	Generic::List<XmlNode^>^	lNodeList;
	XmlNode^					lXmlNode;
	XmlAttribute^				lXmlAttribute;

	if	(pXmlNode->NodeType == XmlNodeType::EntityReference)
	{
		XmlDocumentType^	lDocType;
		XmlNamedNodeMap^	lEntities;
		XmlNode^			lEntity = nullptr;

#ifdef	_DEBUG_ENTITIES_NOT
		LogMessage (_DEBUG_ENTITIES, _T("Entity [%s]"), _B(pXmlNode->Name));
#endif
		if	(
				(lDocType = pXmlNode->OwnerDocument->DocumentType)
			&&	(lEntities = lDocType->Entities)
			&&	(lEntity = lEntities->GetNamedItem (pXmlNode->Name))
			)
		{
#ifdef	_DEBUG_ENTITIES_NOT
			LogMessage (_DEBUG_ENTITIES, _T("Entity [%s] to [%s]"), _B(pXmlNode->Name), _B(lEntity->InnerText));
#endif
			try
			{
				if	(lXmlNode = pXmlNode->OwnerDocument->CreateNode (XmlNodeType::DocumentFragment, nullptr, nullptr))
				{
					lXmlNode->InnerXml = lEntity->InnerText;
#ifdef	_DEBUG_ENTITIES
					LogMessage (_DEBUG_ENTITIES, _T("Entity [%s] to [%s] (Xml)"), _B(pXmlNode->Name), _B(lXmlNode->InnerXml));
#endif
					pXmlNode->ParentNode->ReplaceChild (lXmlNode, pXmlNode);
					pXmlNode = lXmlNode;
				}
			}
			catch (...)
			{
				try
				{
					if	(lXmlNode = pXmlNode->OwnerDocument->CreateTextNode (lEntity->InnerText))
					{
#ifdef	_DEBUG_ENTITIES
						LogMessage (_DEBUG_ENTITIES, _T("Entity [%s] to [%s] (Text)"), _B(pXmlNode->Name), _B(lXmlNode->InnerText));
#endif
						pXmlNode->ParentNode->ReplaceChild (lXmlNode, pXmlNode);
						pXmlNode = lXmlNode;
					}
				}
				catch AnyExceptionSilent
			}
		}
	}

	if	(pXmlNode)
	{
		if	(lNodeList = NodeList (pXmlNode->ChildNodes))
		{
			for each (lXmlNode in lNodeList)
			{
				ResolveEntities (lXmlNode);
			}
		}
		if	(pXmlNode->Attributes)
		{
			for each (lXmlAttribute in pXmlNode->Attributes)
			{
				ResolveEntities (lXmlAttribute);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
} // namespace XmlToHtml
} // namespace DoubleAgent
