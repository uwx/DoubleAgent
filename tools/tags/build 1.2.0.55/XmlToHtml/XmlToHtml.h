#pragma once

using namespace System;

namespace DoubleAgent {
namespace XmlToHtml {
/////////////////////////////////////////////////////////////////////////////

ref class XmlToHtml
{
public:
	XmlToHtml (bool pRestartLog) : mRestartLog (pRestartLog) {}
	~XmlToHtml () {}

	int ProcessCmdLine (array <String^>^ pCmdArgs);
	bool ConvertXmlToHtml (System::Xml::XmlDocument^ pXmlDocument, System::Reflection::Assembly^ pAssembly);
	bool CopyXmlToXml (System::Xml::XmlDocument^ pXmlDocument, System::Reflection::Assembly^ pAssembly);

protected:
	static String^ CmdOpt (array <String^>^ pCmdArgs, int pCmdArgNdx);
	static String^ CmdArg (array <String^>^ pCmdArgs, int pCmdArgNdx);

	System::Object^ LoadTypeLibrary (System::String^ pTypeLibPath);
	System::Xml::XmlDocument^ LoadXmlFile (System::String^ pXmlPath, bool pPreserveWhitespace);
	bool LoadXmlIncludes (System::Xml::XmlDocument^ pXmlDocument, String^ pXmlPath, bool pPreserveWhitespace);
	System::Xml::XmlDocument^ LoadTypeTemplate ();
	System::Xml::XmlDocument^ LoadMethodTemplate ();
	System::Xml::XmlDocument^ LoadPropertyTemplate ();
	System::Xml::XmlDocument^ LoadFieldTemplate ();
	System::Xml::XmlDocument^ LoadEventTemplate ();
	System::Xml::XmlDocument^ LoadTemplate (String^ pTemplatePath);
	void LoadAssemblyMembers (System::Reflection::Assembly^ pAssembly);

	System::Type^ FindAssemblyType (String^ pMemberName);
	System::Reflection::MethodInfo^ FindAssemblyMethod (String^ pMemberName);
	System::Reflection::ConstructorInfo^ FindAssemblyConstructor (String^ pMemberName);
	System::Reflection::PropertyInfo^ FindAssemblyProperty (String^ pMemberName);
	System::Reflection::EventInfo^ FindAssemblyEvent (String^ pMemberName);
	System::Reflection::FieldInfo^ FindAssemblyField (String^ pMemberName);

	String^ PutTypeMember (String^ pMemberName, System::Xml::XmlNode^ pMemberNode, System::Type^ pAssemblyType);
	String^ PutMethodMember (String^ pMemberName, System::Xml::XmlNode^ pMemberNode, System::Reflection::MethodInfo^ pAssemblyMethod);
	String^ PutMethodMember (String^ pMemberName, System::Xml::XmlNode^ pMemberNode, System::Reflection::ConstructorInfo^ pAssemblyConstructor);
	String^ PutPropertyMember (String^ pMemberName, System::Xml::XmlNode^ pMemberNode, System::Reflection::PropertyInfo^ pAssemblyProperty);
	String^ PutEventMember (String^ pMemberName, System::Xml::XmlNode^ pMemberNode, System::Reflection::EventInfo^ pAssemblyEvent);
	String^ PutFieldMember (String^ pMemberName, System::Xml::XmlNode^ pMemberNode, System::Reflection::FieldInfo^ pAssemblyField);

	void PutMemberName (System::Xml::XmlElement^ pRootElement, System::Xml::XmlNode^ pMemberNode, System::Type^ pAssemblyType);
	void PutMemberName (System::Xml::XmlElement^ pRootElement, System::Xml::XmlNode^ pMemberNode, System::Reflection::MethodInfo^ pAssemblyMethod);
	void PutMemberName (System::Xml::XmlElement^ pRootElement, System::Xml::XmlNode^ pMemberNode, System::Reflection::PropertyInfo^ pAssemblyProperty);
	void PutMemberName (System::Xml::XmlElement^ pRootElement, System::Xml::XmlNode^ pMemberNode, System::Reflection::FieldInfo^ pAssemblyField);
	void PutMemberName (System::Xml::XmlElement^ pRootElement, System::Xml::XmlNode^ pMemberNode, System::Reflection::EventInfo^ pAssemblyEvent);
	void PutMemberName (System::Xml::XmlElement^ pRootElement, System::Xml::XmlNode^ pMemberNode, String^ pMemberName, String^ pSuffix);
	void PutMemberName (System::Xml::XmlElement^ pRootElement, String^ pMemberName);

	void PutMemberSummary (System::Xml::XmlElement^ pRootElement, System::Xml::XmlNode^ pMemberSummary);
	void PutMemberRemarks (System::Xml::XmlElement^ pRootElement, System::Xml::XmlNode^ pMemberRemarks, System::Xml::XmlNode^ pMemberDetails);
	void PutMemberSyntax (System::Xml::XmlElement^ pRootElement, System::Xml::XmlNode^ pMemberSyntax, System::Xml::XmlNode^ pMemberParameters, System::Xml::XmlNode^ pMemberExamples);
	void PutMemberValues (System::Xml::XmlElement^ pRootElement, System::Xml::XmlNodeList^ pMemberValues);
	void PutMemberSeeAlso (System::Xml::XmlElement^ pRootElement, System::Xml::XmlNode^ pMemberSeeAlso);

	System::Xml::XmlNode^ FormatSummary (System::Xml::XmlNode^ pXmlNode);
	System::Xml::XmlNode^ FormatDetails (System::Xml::XmlNodeList^ pXmlNodeList);
	System::Xml::XmlNode^ FormatRemarks (System::Xml::XmlNode^ pXmlNode);
	System::Xml::XmlNode^ FormatInnerXml (System::Xml::XmlNode^ pXmlNode);
	System::Xml::XmlNode^ FormatInnerXml (System::Xml::XmlNode^ pXmlNode, String^ pCodeClass);
	System::Xml::XmlNode^ FormatSyntax (System::Xml::XmlNode^ pXmlNode);
	System::Xml::XmlNode^ FormatSyntaxLanguage (System::Xml::XmlNode^ pLanguagesNode, System::String^ pLanguageName, System::Xml::XmlNodeList^ pSyntaxNodes);
	System::Xml::XmlNode^ FormatParameters (System::Xml::XmlNodeList^ pXmlNodeList);
	System::Xml::XmlNode^ FormatExamples (System::Xml::XmlNodeList^ pXmlNodeList);
	System::Xml::XmlNode^ FormatSeeAlso (System::Xml::XmlNodeList^ pXmlNodeList);
	System::Xml::XmlNode^ FormatSee (System::Xml::XmlNode^ pXmlNode);
	System::Xml::XmlNode^ FormatList (System::Xml::XmlNode^ pXmlNode, String^ pCodeClass);
	System::Xml::XmlNode^ FormatListDefault (System::Xml::XmlNode^ pXmlNode);
	System::Xml::XmlNode^ FormatListBullet (System::Xml::XmlNode^ pXmlNode);
	System::Xml::XmlNode^ FormatListNumber (System::Xml::XmlNode^ pXmlNode);
	System::Xml::XmlNode^ FormatListTable (System::Xml::XmlNode^ pXmlNode);

	String^ PutMemberFile (String^ pFileName, System::Xml::XmlElement^ pRootElement);
	String^ MemberFileName (String^ pMemberName);
	String^ TypeFileName (System::Type^ pAssemblyType);
	String^ MethodFileName (System::Reflection::MethodInfo^ pAssemblyMethod);
	String^ MethodFileName (System::Reflection::ConstructorInfo^ pAssemblyConstructor);
	String^ PropertyFileName (System::Reflection::PropertyInfo^ pAssemblyProperty);
	String^ EventFileName (System::Reflection::EventInfo^ pAssemblyEvent);
	String^ FieldFileName (System::Reflection::FieldInfo^ pAssemblyField);

	bool IsAllPropertiesName (String^ pMemberName);
	String^ AllPropertiesName (String^ pMemberName);
	String^ AllPropertiesFileName (String^ pMemberName);

	bool IsAllFieldsName (String^ pMemberName);
	String^ AllFieldsName (String^ pMemberName);
	String^ AllFieldsFileName (String^ pMemberName);

	bool IsConceptualLink (String^ pLink);
	bool IsInheritDoc (System::Xml::XmlNode^ pXmlNode);

	void FixMethodSignatures (System::Xml::XmlDocument^ pXmlDocument);
	void FixMemberReferences (System::Xml::XmlDocument^ pXmlDocument);
	void FixListFormats (System::Xml::XmlDocument^ pXmlDocument);
	void MoveMemberDetails (System::Xml::XmlDocument^ pXmlDocument);
	void RemoveMemberDetails (System::Xml::XmlDocument^ pXmlDocument);
	void RemoveCompositeMembers (System::Xml::XmlDocument^ pXmlDocument);
	void AdjustXmlFormat (System::Xml::XmlDocument^ pXmlDocument);
	void ResolveEntities (System::Xml::XmlDocument^ pXmlDocument);
	void ResolveEntities (System::Xml::XmlNode^ pXmlNode);

protected:
	System::Collections::Generic::List<System::Xml::XmlNode^>^ NodeList (System::Xml::XmlNodeList^ pNodeList);
	void CopyNodeAttributes (System::Xml::XmlNode^ pSrcNode, System::Xml::XmlNode^ pTrgNode);
	void CopyNodeChildren (System::Xml::XmlNode^ pSrcNode, System::Xml::XmlNode^ pTrgNode, System::Xml::XmlSpace pXmlSpace);
	System::Xml::XmlNode^ CopyNodeOuterXml (System::Xml::XmlNode^ pXmlNode, System::Xml::XmlDocument^ pDocument);
	System::Xml::XmlNode^ CopyNodeInnerXml (System::Xml::XmlNode^ pXmlNode, System::Xml::XmlDocument^ pDocument);

	System::Xml::XmlNode^ ChangeNodeName (System::Xml::XmlNode^ pXmlNode, String^ pNewName);
	System::Xml::XmlNode^ MakeNewNode (String^ pInnerXml, System::Xml::XmlDocument^ pDocument);
	System::Xml::XmlNode^ MakeNewNode (String^ pNodeName, String^ pInnerText, System::Xml::XmlDocument^ pDocument);
	System::Xml::XmlAttribute^ MakeNewAttribute (String^ pAttributeName, String^ pAttributeText, System::Xml::XmlDocument^ pDocument);

	void PutParagraphNode (System::Xml::XmlNode^ pParagraphNode, System::Xml::XmlNode^ pTargetNode);
	System::Xml::XmlNode^ PreserveNodeSpace (System::Xml::XmlNode^ pXmlNode);

public:
	bool	mRestartLog;
	String^	mOutputPath;
	String^	mHtmlPath;
	String^	mTemplatePath;
	bool	mFormatServer;

protected:
	bool																							mOutputHtml;
	bool																							mOutputIntellisense;
	bool																							mOutputSandcastle;
	System::Collections::Generic::Dictionary<String^, System::Xml::XmlDocument^>^					mLoadedIncludes;
	System::Reflection::Assembly^																	mSourceAssembly;
	System::Collections::Generic::SortedDictionary <String^, System::Type^>^						mSourceTypes;
	System::Collections::Generic::SortedDictionary <String^, System::Reflection::MethodInfo^>^		mSourceMethods;
	System::Collections::Generic::SortedDictionary <String^, System::Reflection::ConstructorInfo^>^	mSourceConstructors;
	System::Collections::Generic::SortedDictionary <String^, System::Reflection::PropertyInfo^>^	mSourceProperties;
	System::Collections::Generic::SortedDictionary <String^, System::Reflection::FieldInfo^>^		mSourceFields;
	System::Collections::Generic::SortedDictionary <String^, System::Reflection::EventInfo^>^		mSourceEvents;
};

/////////////////////////////////////////////////////////////////////////////
} // namespace XmlToHtml
} // namespace DoubleAgent
