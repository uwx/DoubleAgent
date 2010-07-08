#include "stdafx.h"
#include "XmlToHtml.h"
#include "DaVersion.h"

using namespace System::Collections;
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
//#define	_DEBUG_XML_WRITE			LogNormal|LogHighVolume
#endif

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
				}
				catch AnyExceptionDebug
			}
		}

		if	(
				(lAssembly)
			&&	(lXmlPath)
			&&	(lXmlDocument = LoadXmlFile (lXmlPath))
			&&	(!String::IsNullOrEmpty (mOutputPath))
			)
		{
			LogMessage (LogNormal, _T("Copy [%s] to [%s] for [%s]"), _B(lXmlPath), _B(mOutputPath), _B(lAssemblyPath));
			Console::WriteLine ("Copy {0} to {1} for {2}", lXmlPath, mOutputPath, lAssemblyPath);

			LoadXmlIncludes (lXmlDocument,lXmlPath);
			CopyXmlToXml (lXmlDocument, lAssembly);
		}

		if	(
				(lAssembly)
			&&	(lXmlPath)
			&&	(lXmlDocument = LoadXmlFile (lXmlPath))
			&&	(!String::IsNullOrEmpty (mHtmlPath))
			&&	(System::IO::Directory::Exists (mHtmlPath))
			)
		{
			LogMessage (LogNormal, _T("Convert [%s] to [%s] for [%s]"), _B(lXmlPath), _B(mHtmlPath), _B(lAssemblyPath));
			Console::WriteLine ("Convert {0} to {1} for {2}", lXmlPath, mHtmlPath, lAssemblyPath);

			mTemplatePath = System::IO::Path::Combine (System::IO::Path::GetDirectoryName (lXmlPath), "Templates");
			LoadXmlIncludes (lXmlDocument,lXmlPath);
			ConvertXmlToHtml (lXmlDocument, lAssembly);
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

static inline Generic::List<XmlNode^>^ NodeList (XmlNodeList^ pNodeList)
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

static inline XmlNode^ CopyNode (XmlNode^ pXmlNode, XmlDocument^ pDocument)
{
	XmlNode^		lRet;
	XmlAttribute^	lSrcAttribute;
	XmlAttribute^	lTrgAttribute;

	if	(
			(pXmlNode)
		&&	(lRet = pDocument->CreateNode (pXmlNode->NodeType, pXmlNode->Name, pXmlNode->NamespaceURI))
		)
	{
		lRet->InnerXml = pXmlNode->InnerXml;
		if	(pXmlNode->Attributes)
		{
			for each (lSrcAttribute in pXmlNode->Attributes)
			{
				if	(lTrgAttribute = pDocument->CreateAttribute (lSrcAttribute->Prefix, lSrcAttribute->Name, lSrcAttribute->NamespaceURI))
				{
					lTrgAttribute->Value = lSrcAttribute->Value;
					lRet->Attributes->Append (lTrgAttribute);
				}
			}
		}
	}
	return lRet;
}

static inline XmlNode^ CopyNodeInnerXml (XmlNode^ pXmlNode, XmlDocument^ pDocument)
{
	XmlNode^	lRet;

	if	(
			(pXmlNode)
		&&	(lRet = pDocument->CreateNode (XmlNodeType::DocumentFragment, nullptr, nullptr))
		)
	{
		lRet->InnerXml = pXmlNode->InnerXml;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

static inline XmlNode^ ChangeNodeName (XmlNode^ pXmlNode, String^ pNewName)
{
	XmlNode^	lRet;

	if	(
			(pXmlNode)
		&&	(lRet = pXmlNode->OwnerDocument->CreateNode (pXmlNode->NodeType, pNewName, pXmlNode->NamespaceURI))
		)
	{
		lRet->InnerXml = pXmlNode->InnerXml;
	}
	return lRet;
}

static inline XmlNode^ PreserveNodeSpace (XmlNode^ pXmlNode)
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

static inline XmlNode^ MakeNewNode (String^ pInnerXml, XmlDocument^ pDocument)
{
	XmlNode^	lNode;

	if	(lNode = pDocument->CreateNode (XmlNodeType::DocumentFragment, nullptr, nullptr))
	{
		lNode->InnerXml = pInnerXml;
	}
	return lNode;
}

static inline XmlNode^ MakeNewNode (String^ pNodeName, String^ pInnerText, XmlDocument^ pDocument)
{
	XmlNode^	lNode;

	if	(lNode = pDocument->CreateNode (XmlNodeType::Element, pNodeName, nullptr))
	{
		lNode->AppendChild (pDocument->CreateTextNode (pInnerText));
	}
	return lNode;
}

/////////////////////////////////////////////////////////////////////////////

static inline XmlAttribute^ MakeNewAttribute (String^ pAttributeName, String^ pAttributeText, XmlDocument^ pDocument)
{
	XmlAttribute^	lAttribute;

	if	(lAttribute = pDocument->CreateAttribute (pAttributeName))
	{
		lAttribute->Value = pAttributeText;
	}
	return lAttribute;
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
				if	(lMemberNode->NodeType != XmlNodeType::Comment)
				{
					LogMessage (LogNormal, _T("  Unknown member node [%s]"), _B(lMemberNode->Name));
				}
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
				Console::WriteLine ("Wrote {0}", lMemberFileName);
			}
		}
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

XmlDocument^ XmlToHtml::LoadXmlFile (String^ pXmlPath)
{
	XmlDocument^	lRet;
	XmlDocument^	lXmlLoad;

	try
	{
		if	(lXmlLoad = gcnew XmlDocument)
		{
			lXmlLoad->Load (pXmlPath);
			lRet = lXmlLoad;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

void XmlToHtml::LoadXmlIncludes (XmlDocument^ pXmlDocument, String^ pXmlPath)
{
	Generic::List<XmlNode^>^	lIncludeNodes;
	XmlNode^					lIncludeNode;

	if	(lIncludeNodes = NodeList (pXmlDocument->SelectNodes ("descendant::include")))
	{
		for each (lIncludeNode in lIncludeNodes)
		{
			try
			{
				XmlAttribute^				lIncludeFile;
				XmlAttribute^				lIncludePath;
				XmlDocument^				lIncludeDoc;
				Generic::List<XmlNode^>^	lPathNodes;
				XmlNode^					lPathNode;

				try
				{
					lIncludeFile = lIncludeNode->Attributes["file"];
				}
				catch AnyExceptionSilent
				try
				{
					lIncludePath = lIncludeNode->Attributes["path"];
				}
				catch AnyExceptionSilent

				if	(
						(lIncludeFile)
					&&	(!String::IsNullOrEmpty (lIncludeFile->Value))
					)
				{
					if	(System::IO::Path::IsPathRooted (lIncludeFile->Value))
					{
						lIncludeDoc = LoadXmlFile (lIncludeFile->Value);
					}
					else
					{
						lIncludeDoc = LoadXmlFile (System::IO::Path::Combine (System::IO::Path::GetDirectoryName (pXmlPath), lIncludeFile->Value));
					}
				}
				else
				{
					lIncludeDoc = pXmlDocument;
				}

#ifdef	_DEBUG_INCLUDES
				LogMessage (_DEBUG_INCLUDES, _T("Include [%s]"), _B(lIncludePath->Value));
#endif
				if	(
						(lIncludeDoc)
					&&	(lPathNodes = NodeList (lIncludeDoc->SelectNodes (lIncludePath->Value)))
					)
				{
					XmlNode^	lParentNode = lIncludeNode->ParentNode;
					XmlNode^	lIncludedNode = nullptr;
#ifdef	_DEBUG_INCLUDES
					LogMessage (_DEBUG_INCLUDES, _T("        [%s] [%d]"), _B(lIncludePath->Value), lPathNodes->Count);
#endif
					for each (lPathNode in lPathNodes)
					{
						lPathNode->InnerXml = lPathNode->InnerXml->Trim(String("\t\r\n").ToCharArray());

						if	(lIncludedNode)
						{
							lIncludeNode = lIncludedNode;
							lParentNode->InsertAfter (lIncludedNode = CopyNodeInnerXml (lPathNode, lParentNode->OwnerDocument), lIncludeNode);
						}
						else
						{
							lParentNode->ReplaceChild (lIncludedNode = CopyNodeInnerXml (lPathNode, lParentNode->OwnerDocument), lIncludeNode);
						}
#ifdef	_DEBUG_INCLUDES
						LogMessage (_DEBUG_INCLUDES, _T("          [%s]"), _B(lPathNode->InnerXml->Replace("\t","\\t")/*->Replace("\n","\\n")->Replace("\r","\\r")*/));
						LogMessage (_DEBUG_INCLUDES, _T("          [%s]"), _B(lParentNode->OuterXml->Replace("\t","\\t")/*->Replace("\n","\\n")->Replace("\r","\\r")*/));
#endif
					}
				}
			}
			catch AnyExceptionDebug
		}
	}
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
	mSourceTypes = gcnew Generic::SortedDictionary <String^, System::Type^>;
	mSourceMethods = gcnew Generic::SortedDictionary <String^, System::Reflection::MethodInfo^>;
	mSourceConstructors = gcnew Generic::SortedDictionary <String^, System::Reflection::ConstructorInfo^>;
	mSourceProperties = gcnew Generic::SortedDictionary <String^, System::Reflection::PropertyInfo^>;
	mSourceEvents = gcnew Generic::SortedDictionary <String^, System::Reflection::EventInfo^>;
	mSourceFields = gcnew Generic::SortedDictionary <String^, System::Reflection::FieldInfo^>;

	LogMessage (LogNormal, _T("LoadAssemblyMembers [%s]"), _B(mSourceAssembly->GetName()->Name));

	if	(lTypes = pAssembly->GetTypes ())
	{
		for each (lType in lTypes)
		{
			if	(!lType->IsClass)
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

			if	(lFields = lType->GetFields (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
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
		PutMemberRemarks (lTargetRoot, FormatRemarks (pMemberNode ["remarks"]), FormatDetails (pMemberNode->SelectSingleNode ("child::*[self::detail or self::details]")));
		PutMemberSeeAlso (lTargetRoot, FormatSeeAlso (pMemberNode->SelectNodes ("child::seealso")));

		lRet = PutMemberFile (TypeFileName (pAssemblyType), lTargetRoot);
	}
	return lRet;
}

String^ XmlToHtml::PutMethodMember (String^ pMemberName, XmlNode^ pMemberNode, MethodInfo^ pAssemblyMethod)
{
	String^						lRet;
	XmlDocument^	lTemplate;
	XmlElement^					lTargetRoot;

	if	(
			(lTemplate = LoadMethodTemplate())
		&&	(lTargetRoot = lTemplate->DocumentElement)
		)
	{
		PutMemberName (lTargetRoot, pMemberNode, pAssemblyMethod);
		PutMemberSummary (lTargetRoot, FormatSummary (pMemberNode ["summary"]));
		PutMemberSyntax (lTargetRoot, FormatSyntax (pMemberNode ["syntax"]), FormatParameters (pMemberNode->SelectNodes ("child::*[self::param or self::returns]")), FormatExamples (pMemberNode->SelectNodes ("child::example")));
		PutMemberRemarks (lTargetRoot, FormatRemarks (pMemberNode ["remarks"]), FormatDetails (pMemberNode->SelectSingleNode ("child::*[self::detail or self::details]")));
		PutMemberSeeAlso (lTargetRoot, FormatSeeAlso (pMemberNode->SelectNodes ("child::seealso")));

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
		PutMemberSyntax (lTargetRoot, FormatSyntax (pMemberNode ["syntax"]), FormatParameters (pMemberNode->SelectNodes ("child::*[self::param or self::returns]")), FormatExamples (pMemberNode->SelectNodes ("child::example")));
		PutMemberRemarks (lTargetRoot, FormatRemarks (pMemberNode ["remarks"]), FormatDetails (pMemberNode->SelectSingleNode ("child::*[self::detail or self::details]")));
		PutMemberSeeAlso (lTargetRoot, FormatSeeAlso (pMemberNode->SelectNodes ("child::seealso")));

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
		PutMemberSyntax (lTargetRoot, FormatSyntax (pMemberNode ["syntax"]), FormatParameters (pMemberNode->SelectNodes ("child::*[self::param or self::returns]")), FormatExamples (pMemberNode->SelectNodes ("child::example")));
		PutMemberRemarks (lTargetRoot, FormatRemarks (pMemberNode ["remarks"]), FormatDetails (pMemberNode->SelectSingleNode ("child::*[self::detail or self::details]")));
		PutMemberSeeAlso (lTargetRoot, FormatSeeAlso (pMemberNode->SelectNodes ("child::seealso")));

		lRet = PutMemberFile (EventFileName (pAssemblyEvent), lTargetRoot);
	}
	return lRet;
}

String^ XmlToHtml::PutFieldMember (String^ pMemberName, XmlNode^ pMemberNode, FieldInfo^ pAssemblyField)
{
	return nullptr;
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

	if	(lNodeList = NodeList (pRootElement->SelectNodes ("descendant::*[self::detail or self::details]")))
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

	if	(pMemberParameters)
	{
		if	(pMemberSyntax)
		{
			pMemberSyntax->AppendChild (CopyNode (pMemberParameters, pMemberSyntax->OwnerDocument));
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
			pMemberSyntax->AppendChild (CopyNode (pMemberExamples, pMemberSyntax->OwnerDocument));
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
					lNode->ParentNode->ReplaceChild (CopyNodeInnerXml (pMemberSyntax, lNode->OwnerDocument), lNode);
				}
				else
				{
					lNode->ParentNode->ReplaceChild (CopyNode (pMemberSyntax, lNode->OwnerDocument), lNode);
				}
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

XmlNode^ XmlToHtml::FormatSummary (XmlNode^ pXmlNode)
{
	return FormatInnerXml (pXmlNode);
}

XmlNode^ XmlToHtml::FormatDetails (XmlNode^ pXmlNode)
{
	return FormatInnerXml (pXmlNode, "example");
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

		if	(lNodeList = NodeList (lRet->SelectNodes ("descendant::para")))
		{
			for each (lNode in lNodeList)
			{
				lNode->ParentNode->ReplaceChild (ChangeNodeName (lNode, "p"), lNode);
			}
		}
		if	(lNodeList = NodeList (lRet->SelectNodes ("descendant::note")))
		{
			for each (lNode in lNodeList)
			{
				XmlNode^	lNoteNode;

				lNode->ParentNode->ReplaceChild (lNoteNode = ChangeNodeName (lNode, "p"), lNode);
				lNoteNode->InsertAfter (MakeNewNode ("<b>Note:</b>&nbsp;", lNoteNode->OwnerDocument), nullptr);
				//lNoteNode->Attributes->Append (MakeNewAttribute ("class", "note", lNoteNode->OwnerDocument));
			}
		}
		if	(lNodeList = NodeList (lRet->SelectNodes ("descendant::code")))
		{
			for each (lNode in lNodeList)
			{
				XmlNode^	lCodeNode;

				lNode->ParentNode->ReplaceChild (lCodeNode = ChangeNodeName (lNode, "pre"), lNode);
				if	(pCodeClass)
				{
					lCodeNode->Attributes->Append (MakeNewAttribute ("class", pCodeClass, lCodeNode->OwnerDocument));
				}
			}
		}
		if	(lNodeList = NodeList (lRet->SelectNodes ("descendant::c")))
		{
			for each (lNode in lNodeList)
			{
				lNode->ParentNode->ReplaceChild (ChangeNodeName (lNode, "s"), lNode);
			}
		}
		if	(lNodeList = NodeList (lRet->SelectNodes ("descendant::list")))
		{
			for each (lNode in lNodeList)
			{
				lNode->ParentNode->ReplaceChild (FormatList (lNode, pCodeClass), lNode);
			}
		}
		if	(lNodeList = NodeList (lRet->SelectNodes ("descendant::example")))
		{
			for each (lNode in lNodeList)
			{
				XmlNodeList^	lCodeNodes;
				XmlNode^		lExampleNode;

				if	(
						(lCodeNodes = lNode->SelectNodes ("descendant::*[self::code or self::pre]"))
					&&	(lCodeNodes->Count > 0)
					)
				{
					lNode->ParentNode->ReplaceChild (lExampleNode = ChangeNodeName (lNode, "div"), lNode);
				}
				else
				{
					lNode->ParentNode->ReplaceChild (lExampleNode = ChangeNodeName (lNode, "code"), lNode);
				}
				lExampleNode->Attributes->Append (MakeNewAttribute ("class", "example", lExampleNode->OwnerDocument));
			}
		}
		if	(lNodeList = NodeList (lRet->SelectNodes ("descendant::see")))
		{
			for each (lNode in lNodeList)
			{
				XmlNode^	lRefNode;

				if	(lRefNode = FormatSee (lNode))
				{
					lNode->ParentNode->ReplaceChild (lRefNode, lNode);
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
	XmlNode^		lCppNode;
	XmlNode^		lCsharpNode;
	XmlNode^		lVbNode;
	XmlNode^		lVb6Node;
	XmlNode^		lVbscriptNode;
	XmlNode^		lLanguagesNode;
	XmlNode^		lLanguageNode;

	if	(pXmlNode)
	{
		if	(
				(lLanguageNodes = pXmlNode->SelectNodes ("child::*[self::cpp or self::cplusplus or self::csharp or self::vb or self::vbnet or self::vb6 or self::vbscript]"))
			&&	(lLanguageNodes->Count > 0)
			)
		{
			lCppNode = pXmlNode->SelectSingleNode ("child::*[self::cpp or self::cplusplus]");
			lCsharpNode = pXmlNode->SelectSingleNode ("child::csharp");
			lVbNode = pXmlNode->SelectSingleNode ("child::*[self::vbnet or self::vb]");
			lVb6Node = pXmlNode->SelectSingleNode ("child::*[self::vb6 or self::vb]");
			lVbscriptNode = pXmlNode->SelectSingleNode ("child::*[self::vbscript or self::vb]");

			if	(
					(lRet = pXmlNode->OwnerDocument->CreateNode (XmlNodeType::Element, "div", nullptr))
				&&	(lLanguagesNode = pXmlNode->OwnerDocument->CreateNode (XmlNodeType::Element, "dl", nullptr))
				)
			{
				lRet->AppendChild (lLanguagesNode);

				if	(
						(lVbNode)
					&&	(lLanguageNode = pXmlNode->OwnerDocument->CreateNode (XmlNodeType::Element, "dt", nullptr))
					)
				{
					lLanguageNode->InnerText = "Visual Basic .NET";
					lLanguagesNode->AppendChild (lLanguageNode);
					lLanguagesNode->AppendChild (ChangeNodeName (lVbNode, "dd"));
				}
				if	(
						(lVb6Node)
					&&	(lLanguageNode = pXmlNode->OwnerDocument->CreateNode (XmlNodeType::Element, "dt", nullptr))
					)
				{
					lLanguageNode->InnerText = "Visual Basic 6";
					lLanguagesNode->AppendChild (lLanguageNode);
					lLanguagesNode->AppendChild (ChangeNodeName (lVb6Node, "dd"));
				}
				if	(
						(lVbscriptNode)
					&&	(lLanguageNode = pXmlNode->OwnerDocument->CreateNode (XmlNodeType::Element, "dt", nullptr))
					)
				{
					lLanguageNode->InnerText = "VBScript";
					lLanguagesNode->AppendChild (lLanguageNode);
					lLanguagesNode->AppendChild (ChangeNodeName (lVbscriptNode, "dd"));
				}
				if	(
						(lCsharpNode)
					&&	(lLanguageNode = pXmlNode->OwnerDocument->CreateNode (XmlNodeType::Element, "dt", nullptr))
					)
				{
					lLanguageNode->InnerText = "Visual C#";
					lLanguagesNode->AppendChild (lLanguageNode);
					lLanguagesNode->AppendChild (ChangeNodeName (lCsharpNode, "dd"));
				}
				if	(
						(lCppNode)
					&&	(lLanguageNode = pXmlNode->OwnerDocument->CreateNode (XmlNodeType::Element, "dt", nullptr))
					)
				{
					lLanguageNode->InnerText = "Visual C++";
					lLanguagesNode->AppendChild (lLanguageNode);
					lLanguagesNode->AppendChild (ChangeNodeName (lCppNode, "dd"));
				}
			}
		}
		else
		{
			lRet = ChangeNodeName (pXmlNode, "div");
		}

		if	(lRet)
		{
			lRet = FormatInnerXml (lRet);
			lRet->Attributes->Append (MakeNewAttribute ("class", "syntax", lRet->OwnerDocument));
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

XmlNode^ XmlToHtml::FormatParameters (XmlNodeList^ pXmlNodeList)
{
	XmlNode^					lRet;
	Generic::List<XmlNode^>^	lNodeList;
	XmlNode^					lNode;
	XmlNode^					lRowNode;
	XmlNode^					lColNode;
	XmlAttribute^				lParamName;

	if	(
			(lNodeList = NodeList (pXmlNodeList))
		&&	(lNode = lNodeList [0])
		&&	(lRet = lNode->OwnerDocument->CreateNode (XmlNodeType::Element, "table", nullptr))
		)
	{
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
				lColNode->AppendChild (MakeNewNode ("i", lParamName->Value, lRet->OwnerDocument));
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
				LogMessage (LogNormal, _T("  Unknown Reference [%s]"), _B(pXmlNode->OuterXml));
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

				lRefNode = pXmlNode->OwnerDocument->CreateNode (XmlNodeType::Element, "a", nullptr);
				lRefNode->Attributes->Append (MakeNewAttribute ("href", lRefValue, pXmlNode->OwnerDocument));
				//lRefNode->AppendChild (MakeNewNode ("b", lRefName, pXmlNode->OwnerDocument));
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
	XmlNodeList^	lHeaderNodes;
	XmlNodeList^	lItemNodes;
	XmlNode^		lItemNode;
	XmlNode^		lTermNode;
	XmlNode^		lDescriptionNode;
	XmlNode^		lRowNode;

	lRet = pXmlNode->OwnerDocument->CreateNode (XmlNodeType::Element, "table", nullptr);

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
				lRowNode->AppendChild (ChangeNodeName (lTermNode, "th"));
			}
			if	(lDescriptionNode = lItemNode ["description"])
			{
				lRowNode->AppendChild (ChangeNodeName (lDescriptionNode, "th"));
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
				lRowNode->AppendChild (ChangeNodeName (lTermNode, "td"));
			}
			if	(lDescriptionNode = lItemNode ["description"])
			{
				lRowNode->AppendChild (ChangeNodeName (lDescriptionNode, "td"));
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
	return System::IO::Path::GetExtension (pAssemblyType->Namespace)->Substring(1) + "_" + pAssemblyType->Name + ".htm";
}

String^ XmlToHtml::MethodFileName (System::Reflection::MethodInfo^ pAssemblyMethod)
{
	return System::IO::Path::GetExtension (pAssemblyMethod->DeclaringType->Namespace)->Substring(1) + "_" + pAssemblyMethod->DeclaringType->Name + "_" + pAssemblyMethod->Name + ".htm";
}

String^ XmlToHtml::MethodFileName (System::Reflection::ConstructorInfo^ pAssemblyConstructor)
{
	return System::IO::Path::GetExtension (pAssemblyConstructor->DeclaringType->Namespace)->Substring(1) + "_" + pAssemblyConstructor->DeclaringType->Name + "_ctor" + ".htm";
}

String^ XmlToHtml::PropertyFileName (System::Reflection::PropertyInfo^ pAssemblyProperty)
{
	return System::IO::Path::GetExtension (pAssemblyProperty->DeclaringType->Namespace)->Substring(1) + "_" + pAssemblyProperty->DeclaringType->Name + "_" + pAssemblyProperty->Name + ".htm";
}

String^ XmlToHtml::FieldFileName (System::Reflection::FieldInfo^ pAssemblyField)
{
	return System::IO::Path::GetExtension (pAssemblyField->DeclaringType->Namespace)->Substring(1) + "_" + pAssemblyField->DeclaringType->Name + "_" + pAssemblyField->Name + ".htm";
}

String^ XmlToHtml::EventFileName (System::Reflection::EventInfo^ pAssemblyEvent)
{
	return System::IO::Path::GetExtension (pAssemblyEvent->DeclaringType->Namespace)->Substring(1) + "_Event_" + pAssemblyEvent->Name + ".htm";
}

/////////////////////////////////////////////////////////////////////////////

bool XmlToHtml::IsAllPropertiesName (String^ pMemberName)
{
	bool			lRet = false;
	array<String^>^	lNameParts;

	if	(
			(pMemberName)
		&&	(pMemberName->StartsWith ("P:"+mSourceAssembly->GetName()->Name))
		&&	(lNameParts = pMemberName->Substring(2)->Split (String(".").ToCharArray()))
		&&	(lNameParts->Length == 3)
		)
	{
		lRet = true;
	}
	return lRet;
}

String^ XmlToHtml::AllPropertiesName (String^ pMemberName)
{
	array<String^>^	lNameParts = pMemberName->Substring(2)->Split (String(".").ToCharArray());
	return String::Format ("{0} Object Properties", lNameParts[2]);
}

String^ XmlToHtml::AllPropertiesFileName (String^ pMemberName)
{
	array<String^>^	lNameParts = pMemberName->Substring(2)->Split (String(".").ToCharArray());
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
		&&	(lNameParts = pMemberName->Substring(2)->Split (String(".").ToCharArray()))
		&&	(lNameParts->Length == 3)
		)
	{
		lRet = true;
	}
	return lRet;
}

String^ XmlToHtml::AllFieldsName (String^ pMemberName)
{
	array<String^>^	lNameParts = pMemberName->Substring(2)->Split (String(".").ToCharArray());
	return String::Format ("{0} Object Fields", lNameParts[2]);
}

String^ XmlToHtml::AllFieldsFileName (String^ pMemberName)
{
	array<String^>^	lNameParts = pMemberName->Substring(2)->Split (String(".").ToCharArray());
	return MemberFileName (String::Format ("{0}_Fields", lNameParts[2]));
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

static int NewLinesBefore (String^ pElementName)
{
	if	(pElementName == "member")
	{
		return 2;
	}
	else
	if	(
			(pElementName == "doc")
		||	(pElementName == "assembly")
		||	(pElementName == "members")
		||	(pElementName == "summary")
		||	(pElementName == "syntax")
		||	(pElementName == "remarks")
		||	(pElementName == "seealso")
		||	(pElementName == "para")
		||	(pElementName == "note")
		)
	{
		return 1;
	}
	return 0;
}

static int NewLinesAfter (String^ pElementName)
{
	if	(pElementName == "member")
	{
		return 2;
	}
	else
	if	(
			(pElementName == "doc")
		||	(pElementName == "assembly")
		||	(pElementName == "members")
		||	(pElementName == "summary")
		||	(pElementName == "syntax")
		||	(pElementName == "remarks")
		||	(pElementName == "seealso")
		||	(pElementName == "para")
		||	(pElementName == "note")
		)
	{
		return 1;
	}
	return 0;
}

static int WriteNewLines (XmlWriter^ pWriter, int pNewLines, int pWantLines)
{
	while (pNewLines < pWantLines)
	{
		pWriter->WriteWhitespace ("\r\n");
		pNewLines++;
	}
	return pNewLines;
}

/////////////////////////////////////////////////////////////////////////////

bool XmlToHtml::CopyXmlToXml (XmlDocument^ pXmlDocument, System::Reflection::Assembly^ pAssembly)
{
	bool				lRet = false;
	XmlNodeReader^		lNodeReader;
	XmlWriterSettings^	lWriterSettings;
	XmlWriter^			lWriter;
	String^				lTextValue = String::Empty;
	int					lNewLines = 0;

	LoadAssemblyMembers (pAssembly);
	RemoveMemberDetails (pXmlDocument);
	FixMethodSignatures (pXmlDocument);
	FixMemberReferences (pXmlDocument);

	pXmlDocument->PreserveWhitespace = false;
	pXmlDocument->Normalize ();

	System::IO::File::Delete (mOutputPath);

	if	(lWriterSettings = gcnew XmlWriterSettings ())
	{
		lWriterSettings->Encoding = Encoding::UTF8;
		lWriterSettings->Indent = false;
		lWriterSettings->IndentChars = String::Empty;
		lWriterSettings->NewLineHandling = Xml::NewLineHandling::None;
		lWriterSettings->NewLineOnAttributes = false;
#ifdef	_DEBUG_XML_WRITE
		lWriterSettings->ConformanceLevel = Xml::ConformanceLevel::Fragment;
#endif
	}
	if	(
			(lWriterSettings)
		&&	(lNodeReader = gcnew XmlNodeReader (pXmlDocument->DocumentElement))
		&&	(lWriter = XmlTextWriter::Create (mOutputPath, lWriterSettings))
		)
	{
		try
		{
#ifdef	_DEBUG_XML_WRITE
			int	lIndent = 0;
#else
			lWriter->WriteStartDocument ();
#endif
			while	(lNodeReader->Read ())
			{
				if	(
						(lNodeReader->NodeType == XmlNodeType::Text)
					&&	(lNodeReader->XmlSpace != XmlSpace::Preserve)
					)
				{
					lTextValue = lTextValue + lNodeReader->Value;
				}
				else
				{
					if	(!String::IsNullOrEmpty (lTextValue))
					{
#ifdef	_DEBUG_XML_WRITE_NOT
						LogMessage (_DEBUG_XML_WRITE, _T("%d%sText  [%s]"), lIndent, _B(gcnew String('.',max(lIndent,0)*2)), _B(lTextValue));
#endif
						lWriter->WriteString (FixTextWhitespace (lTextValue));
						lTextValue = String::Empty;
						lNewLines = 0;
					}
					if	(lNodeReader->NodeType == XmlNodeType::Text)
					{
#ifdef	_DEBUG_XML_WRITE_NOT
						LogMessage (_DEBUG_XML_WRITE, _T("%d%sText* [%s]"), lIndent, _B(gcnew String('.',max(lIndent,0)*2)), _B(lNodeReader->Value));
#endif
						lWriter->WriteString (lNodeReader->Value);
						lNewLines = 0;
					}
					else
					if	(
							(lNodeReader->NodeType == XmlNodeType::Whitespace)
						||	(lNodeReader->NodeType == XmlNodeType::SignificantWhitespace)
						)
					{
						lWriter->WriteWhitespace (lNodeReader->Value);
						lNewLines = 0;
					}
					else
					if	(lNodeReader->NodeType == XmlNodeType::Element)
					{
						WriteNewLines (lWriter, lNewLines, NewLinesBefore (lNodeReader->Name));
						lNewLines = 0;

#ifdef	_DEBUG_XML_WRITE
						if	(lNodeReader->Name == "member")
						{
							LogMessage (_DEBUG_XML_WRITE, _T("%d%sStart [%s] [%s]"), lIndent, _B(gcnew String('.',max(lIndent,0)*2)), _B(lNodeReader->Name), _B(lNodeReader[0]));
						}
						else
						{
							LogMessage (_DEBUG_XML_WRITE, _T("%d%sStart [%s]"), lIndent, _B(gcnew String('.',max(lIndent,0)*2)), _B(lNodeReader->Name));
						}
						lIndent++;
#endif
						lWriter->WriteStartElement (lNodeReader->Prefix, lNodeReader->LocalName, lNodeReader->NamespaceURI);
						lWriter->WriteAttributes (lNodeReader, false);
						if	(lNodeReader->IsEmptyElement)
						{
#ifdef	_DEBUG_XML_WRITE
							lIndent--;
							LogMessage (_DEBUG_XML_WRITE, _T("%d%sEnd   [%s]"), lIndent, _B(gcnew String('.',max(lIndent,0)*2)), _B(lNodeReader->LocalName));
#endif
							lWriter->WriteEndElement ();
							lNewLines = WriteNewLines (lWriter, 0, NewLinesAfter (lNodeReader->Name));
						}
					}
					else
					if	(lNodeReader->NodeType == XmlNodeType::EndElement)
					{
						if	(lNodeReader->Name == "para")
						{
							lNewLines = WriteNewLines (lWriter, 0, 1); // Only works for C++
						}
#ifdef	_DEBUG_XML_WRITE
						lIndent--;
						LogMessage (_DEBUG_XML_WRITE, _T("%d%sEnd   [%s]"), lIndent, _B(gcnew String('.',max(lIndent,0)*2)), _B(lNodeReader->Name));
#endif
						lWriter->WriteFullEndElement ();
						lNewLines = WriteNewLines (lWriter, 0, NewLinesAfter (lNodeReader->Name));
					}
					else
					if	(lNodeReader->NodeType != XmlNodeType::Comment)
					{
#ifdef	_DEBUG_XML_WRITE
						LogMessage (_DEBUG_XML_WRITE, _T("%d%sNode  [%d] [%s] [%s]"), lIndent, _B(gcnew String('.',max(lIndent,0)*2)), (int)lNodeReader->NodeType, _B(lNodeReader->Name), _B(lNodeReader->Value->Replace("\t","\\t")->Replace("\n","\\n")->Replace("\r","\\r")));
#endif
						lWriter->WriteNode (lNodeReader, true);
					}
				}
			}

#ifndef	_DEBUG_XML_WRITE
			lWriter->WriteEndDocument ();
#endif
			lWriter->Close ();
#ifdef	_DEBUG_MEMBER_FILES
			LogMessage (_DEBUG_MEMBER_FILES, _T("Wrote [%s]"), _B(mOutputPath));
#endif
			Console::WriteLine ("Wrote {0}", mOutputPath);
			lRet = true;
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

String^ XmlToHtml::FixTextWhitespace (String^ pText)
{
	String^	lText = pText;

	while (lText->IndexOf ('\r') >= 0)
	{
		lText = lText->Replace ('\r', '\n');
	}
	while (lText->IndexOf ("\n\t") >= 0)
	{
		lText = lText->Replace ("\n\t", "\n");
	}
	while (lText->IndexOf ("\n\n") >= 0)
	{
		lText = lText->Replace ("\n\n", "\n");
	}
	while (lText->IndexOf ("\n ") >= 0)
	{
		lText = lText->Replace ("\n ", "\n");
	}
	while (lText->IndexOf (" \n") >= 0)
	{
		lText = lText->Replace (" \n", "\n");
	}
	lText = lText->Trim (String("\t\r\n").ToCharArray());
	while (lText->IndexOf ('\n') > 0)
	{
		lText = lText->Replace ('\n', ' ');
	}

#ifdef	_DEBUG_NOT
	String^	lSrc = pText->Replace("\t","\\t")->Replace("\r","\\r")->Replace("\n","\\n");
	String^	lTrg = lText->Replace("\t","\\t")->Replace("\r","\\r")->Replace("\n","\\n");
	LogMessage (LogDebug, _T("Text [%s]"), _B(lSrc));
	LogMessage (LogDebug, _T("     [%s]"), _B(lTrg));
#endif
	return lText;
}

/////////////////////////////////////////////////////////////////////////////

void XmlToHtml::FixMethodSignatures (XmlDocument^ pXmlDocument)
{
	XmlNode^	lMembersNode;
	XmlNode^	lMemberNode;

	if	(
			(lMembersNode = pXmlDocument ["doc"])
		&&	(lMembersNode = lMembersNode ["members"])
		)
	{
		LogMessage (LogNormal, _T("XmlMembers [%d]"), lMembersNode->ChildNodes->Count);

		for each (lMemberNode in lMembersNode->ChildNodes)
		{
			String^	lMemberName;

			if	(String::Compare (lMemberNode->Name, "Member", true) != 0)
			{
				if	(lMemberNode->NodeType != XmlNodeType::Comment)
				{
					LogMessage (LogNormal, _T("  Unknown member node [%d] [%s]"), (int)lMemberNode->NodeType, _B(lMemberNode->Name));
				}
				continue;
			}

			lMemberName = lMemberNode->Attributes["name"]->Value;
#ifdef	_DEBUG_XML_MEMBERS
			LogMessage (_DEBUG_XML_MEMBERS, _T("  Member [%s]"), _B(lMemberName));
#endif
			if	(
					(lMemberName->StartsWith ("M:"))
				&&	(mSourceMethods)
				)
			{
				Generic::KeyValuePair<String^, MethodInfo^>^	lMethod;

				for each (lMethod in mSourceMethods)
				{
					if	(lMethod->Key->StartsWith (lMemberName))
					{
						lMemberNode->Attributes["name"]->Value = lMethod->Key;
						break;
					}
				}
			}
		}
	}
}

void XmlToHtml::FixMemberReferences (XmlDocument^ pXmlDocument)
{
	Generic::List<XmlNode^>^	lNodeList;
	XmlNode^					lNode;
	String^						lAssemblyName = mSourceAssembly->GetName()->Name;

	if	(lNodeList = NodeList (pXmlDocument->DocumentElement->SelectNodes ("descendant::*[self::see or self::seealso]")))
	{
		for each (lNode in lNodeList)
		{
			if	(mSourceMethods)
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
							if	(lMethod->Key->StartsWith (lRefName->Value))
							{
								lRefName->Value = lMethod->Key;
								break;
							}
						}
					}
				}
				catch AnyExceptionSilent
			}

			//try
			//{
			//	XmlAttribute^	lRefName;

			//	if	(
			//			(lRefName = lNode->Attributes ["cref"])
			//		&&	(lRefName->Value [1] == ':')
			//		&&	(lRefName->Value->Substring (2)->StartsWith (lAssemblyName))
			//		)
			//	{
			//		lRefName->Value = lRefName->Value->Substring(0,2) + lRefName->Value->Substring(lAssemblyName->Length+3,lRefName->Value->Length-lAssemblyName->Length-3);
			//	}
			//}
			//catch AnyExceptionSilent
		}
	}
}

void XmlToHtml::RemoveMemberDetails (System::Xml::XmlDocument^ pXmlDocument)
{
	try
	{
		Generic::List<XmlNode^>^	lDetailsNodes;
		XmlNode^					lDetailsNode;

		if	(lDetailsNodes = NodeList (pXmlDocument->DocumentElement->SelectNodes ("descendant::*[self::syntax or self::detail or self::details or self::example]")))
		{
			for each (lDetailsNode in lDetailsNodes)
			{
				lDetailsNode->ParentNode->RemoveChild (lDetailsNode);
			}
		}
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////
} // namespace XmlToHtml
} // namespace DoubleAgent
