#pragma once

namespace DoubleAgent {
namespace XmlToHtml {
/////////////////////////////////////////////////////////////////////////////

///<summary>
///Sample class summary
///</summary>
ref class XmlToHtml
{
public:
	///<summary>
	///Sample constructor summary
	///</summary>
	XmlToHtml (bool pRestartLog) : mRestartLog (pRestartLog) {}
	~XmlToHtml () {}

	int ProcessCmdLine (array <String^>^ pCmdArgs);
	bool ConvertXmlToHtml (System::Xml::XmlDocument^ pXmlDocument, System::Reflection::Assembly^ pAssembly);
	bool CopyXmlToXml (System::Xml::XmlDocument^ pXmlDocument, System::Reflection::Assembly^ pAssembly);

protected:
	///<summary>
	///Sample method summary
	///</summary>
	static String^ CmdOpt (array <String^>^ pCmdArgs, int pCmdArgNdx);
	static String^ CmdArg (array <String^>^ pCmdArgs, int pCmdArgNdx);

	System::Object^ LoadTypeLibrary (System::String^ pTypeLibPath);
	System::Xml::XmlDocument^ LoadXmlFile (System::String^ pXmlPath);
	void LoadXmlIncludes (System::Xml::XmlDocument^ pXmlDocument, String^ pXmlPath);
	System::Xml::XmlDocument^ LoadTypeTemplate ();
	System::Xml::XmlDocument^ LoadMethodTemplate ();
	System::Xml::XmlDocument^ LoadPropertyTemplate ();
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
	void PutMemberSeeAlso (System::Xml::XmlElement^ pRootElement, System::Xml::XmlNode^ pMemberSeeAlso);

	System::Xml::XmlNode^ FormatSummary (System::Xml::XmlNode^ pXmlNode);
	System::Xml::XmlNode^ FormatDetails (System::Xml::XmlNode^ pXmlNode);
	System::Xml::XmlNode^ FormatRemarks (System::Xml::XmlNode^ pXmlNode);
	System::Xml::XmlNode^ FormatInnerXml (System::Xml::XmlNode^ pXmlNode);
	System::Xml::XmlNode^ FormatInnerXml (System::Xml::XmlNode^ pXmlNode, String^ pCodeClass);
	System::Xml::XmlNode^ FormatSyntax (System::Xml::XmlNode^ pXmlNode);
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

	void FixMethodSignatures (System::Xml::XmlDocument^ pXmlDocument);
	void FixMemberReferences (System::Xml::XmlDocument^ pXmlDocument);
	void RemoveMemberDetails (System::Xml::XmlDocument^ pXmlDocument);
	String^ FixTextWhitespace (String^ pText);

public:
	///<summary>
	///Sample field summary
	///</summary>
	bool	mRestartLog;
	String^	mOutputPath;
	String^	mHtmlPath;
	String^	mTemplatePath;

protected:
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
