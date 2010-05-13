#include "StdAfx.h"
#include "AssemblyDlg.h"
#include "LogAssembly.h"

using namespace System::Windows::Forms;
using namespace System::Reflection;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

ref class NodesByText : public System::Collections::IComparer
{
public:
	virtual Int32 Compare (Object^ x, Object^ y)
	{
		return (String::Compare (safe_cast <TreeNode^> (x)->Text, safe_cast <TreeNode^> (y)->Text, true));
	}
};

ref class NodesByName : public System::Collections::IComparer
{
public:
	virtual Int32 Compare (Object^ x, Object^ y)
	{
		return (String::Compare (safe_cast <TreeNode^> (x)->Name, safe_cast <TreeNode^> (y)->Name, true));
	}
};

/////////////////////////////////////////////////////////////////////////////

void AssemblyDlg::ShowAssemblyTree (Assembly^ pAssembly)
{
	try
	{
		TreeNode^	lRootNode;
		String^		lName = pAssembly->FullName->Split(',')[0];

		AssemblyTree->BeginUpdate ();
		AssemblyTree->Sorted = true;
		AssemblyTree->TreeViewNodeSorter = gcnew NodesByName;

		lRootNode = AssemblyTree->Nodes->Add ("Assembly", lName, AssemblyImage, AssemblyImage);
		lRootNode->Nodes->Add ("0_Version", pAssembly->ImageRuntimeVersion, AssemblyImage, AssemblyImage);
		try
		{
			lRootNode->Nodes->Add ("1_Location", pAssembly->Location, AssemblyImage, AssemblyImage);
		}
		catch AnyExceptionSilent
		{}
		try
		{
			lRootNode->Nodes->Add ("2_CodeBase", pAssembly->CodeBase, AssemblyImage, AssemblyImage);
		}
		catch AnyExceptionSilent
		{}

		ShowReferences (lRootNode, pAssembly);
		ShowAttributes (lRootNode, CustomAttributeData::GetCustomAttributes (pAssembly));
		ShowTypes (lRootNode, pAssembly);

		lRootNode->Expand ();
		AssemblyTree->EndUpdate ();
	}
	catch AnyExceptionDebug
	{}
}

/////////////////////////////////////////////////////////////////////////////

void AssemblyDlg::ShowReferences (TreeNode^ pParentNode, System::Reflection::Assembly^ pAssembly)
{
	try
	{
		TreeNode^				lRootNode;
		array<AssemblyName^>^	lReferences = pAssembly->GetReferencedAssemblies ();
		AssemblyName^			lReference;

		if	(
				(lReferences)
			&&	(lReferences->Length > 0)
			)
		{
			lRootNode = pParentNode->Nodes->Add ("References", "References", FolderClosedImage, FolderClosedImage);

			for each (lReference in lReferences)
			{
				lRootNode->Nodes->Add (lReference->ToString(), lReference->ToString(), AssemblyImage, AssemblyImage);
			}
			lRootNode->Expand ();
		}
	}
	catch AnyExceptionDebug
	{}
}

/////////////////////////////////////////////////////////////////////////////

void AssemblyDlg::ShowTypes (TreeNode^ pParentNode, Assembly^ pAssembly)
{
	try
	{
		TreeNode^				lClassesRoot;
		TreeNode^				lInterfacesRoot;
		TreeNode^				lEnumsRoot;
		TreeNode^				lOtherRoot;
		TreeNode^				lTypeRoot;
		array<Type^>^			lTypes;
		Type^					lType;

		if	(lTypes = pAssembly->GetTypes())
		{
			for each (lType in lTypes)
			{
				if	(lType->IsClass)
				{
					if	(lClassesRoot == nullptr)
					{
						lClassesRoot = pParentNode->Nodes->Add ("5_Class", "Class", FolderClosedImage, FolderClosedImage);
					}
					lTypeRoot = lClassesRoot;
				}
				else
				if	(lType->IsInterface)
				{
					if	(lInterfacesRoot == nullptr)
					{
						lInterfacesRoot = pParentNode->Nodes->Add ("6_Interface", "Interface", FolderClosedImage, FolderClosedImage);
					}
					lTypeRoot = lInterfacesRoot;
				}
				else
				if	(lType->IsEnum)
				{
					if	(lEnumsRoot == nullptr)
					{
						lEnumsRoot = pParentNode->Nodes->Add ("7_Enum", "Enum", FolderClosedImage, FolderClosedImage);
					}
					lTypeRoot = lEnumsRoot;
				}
				else
				{
					if	(lOtherRoot == nullptr)
					{
						lOtherRoot = pParentNode->Nodes->Add ("8_Other", "Other", FolderClosedImage, FolderClosedImage);
					}
					lTypeRoot = lOtherRoot;
				}

				ShowType (lTypeRoot, lType);
				lTypeRoot->Expand ();
			}
		}
	}
	catch AnyExceptionDebug
	{}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

TreeNode^ AssemblyDlg::ShowType (TreeNode^ pParentNode, System::Type^ pType, System::Boolean pExpand, System::String^ pNodeName)
{
	TreeNode^	lTypeNode = nullptr;

	try
	{
		int				lTypeImage = OtherImage;
		StringBuilder^	lTypeName = gcnew StringBuilder;
		TreeNode^		lNode;

		if	(pType->IsClass)
		{
			lTypeImage = ClassImage;
		}
		else
		if	(pType->IsInterface)
		{
			lTypeImage = InterfaceImage;
		}
		else
		if	(pType->IsEnum)
		{
			lTypeImage = EnumImage;
		}

		lTypeName->Append (pType->FullName);
		if	(pType->IsPublic)
		{
			lTypeName->Append (" (Public)");
		}
		if	(pType->IsSealed)
		{
			lTypeName->Append (" (Sealed)");
		}
		if	(pType->IsImport)
		{
			lTypeName->Append (" (Import)");
		}
		if	(pType->IsCOMObject)
		{
			lTypeName->Append (" (COMObject)");
		}

		lTypeNode = pParentNode->Nodes->Add ((pNodeName == nullptr) ? pType->FullName : pNodeName, lTypeName->ToString(), lTypeImage, lTypeImage);

		if	(pExpand)
		{
			array<Type^>^			lInterfaces;
			Type^					lInterface;
			array<FieldInfo^>^		lFields;
			FieldInfo^				lField;
			array<PropertyInfo^>^	lProperties;
			PropertyInfo^			lProperty;
			array<MethodInfo^>^		lMethods;
			MethodInfo^				lMethod;
			array<EventInfo^>^		lEvents;
			EventInfo^				lEvent;

			if	(pType->BaseType)
			{
				ShowType (lTypeNode, pType->BaseType, false, "__Base");
			}

			if	(
					(lInterfaces = pType->GetInterfaces ())
				&&	(lInterfaces->Length > 0)
				)
			{
				lNode = lTypeNode->Nodes->Add ("0_Interfaces", "Interfaces", FolderClosedImage, FolderClosedImage);
				for each (lInterface in lInterfaces)
				{
					if	(!LogAssembly::IsInterfaceInherited (pType, lInterface))
					{
						ShowType (lNode, lInterface, false);
					}
				}
				if	(lNode->GetNodeCount(false) > 0)
				{
					lNode->Expand ();
				}
				else
				{
					lTypeNode->Nodes->Remove (lNode);
				}
			}

			if	(
					(lFields = pType->GetFields (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
				&&	(lFields->Length > 0)
				)
			{
				lNode = lTypeNode->Nodes->Add ("1_Fields", "Fields", FolderClosedImage, FolderClosedImage);
				for each (lField in lFields)
				{
					ShowField (lNode, lField);
				}
				lNode->Expand ();
			}

			if	(
					(lProperties = pType->GetProperties (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
				&&	(lProperties->Length > 0)
				)
			{
				lNode = lTypeNode->Nodes->Add ("2_Properties", "Properties", FolderClosedImage, FolderClosedImage);
				for each (lProperty in lProperties)
				{
					if	(Object::ReferenceEquals (lProperty->DeclaringType, pType))
					{
						ShowProperty (lNode, lProperty);
					}
				}
				if	(lNode->GetNodeCount(false) > 0)
				{
					lNode->Expand ();
				}
				else
				{
					lTypeNode->Nodes->Remove (lNode);
				}
			}

			if	(
					(lMethods = pType->GetMethods (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
				&&	(lMethods->Length > 0)
				)
			{
				lNode = lTypeNode->Nodes->Add ("3_Methods", "Methods", FolderClosedImage, FolderClosedImage);
				for each (lMethod in lMethods)
				{
					if	(Object::ReferenceEquals (lMethod->DeclaringType, pType))
					{
						ShowMethod (lNode, lMethod);
					}
				}
				if	(lNode->GetNodeCount(false) > 0)
				{
					lNode->Expand ();
				}
				else
				{
					lTypeNode->Nodes->Remove (lNode);
				}
			}

			if	(
					(lEvents = pType->GetEvents (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
				&&	(lEvents->Length > 0)
				)
			{
				lNode = lTypeNode->Nodes->Add ("4_Events", "Events", FolderClosedImage, FolderClosedImage);
				for each (lEvent in lEvents)
				{
					if	(Object::ReferenceEquals (lEvent->DeclaringType, pType))
					{
						ShowEvent (lNode, lEvent);
					}
				}
				if	(lNode->GetNodeCount(false) > 0)
				{
					lNode->Expand ();
				}
				else
				{
					lTypeNode->Nodes->Remove (lNode);
				}
			}

			ShowAttributes (lTypeNode, CustomAttributeData::GetCustomAttributes (pType));

			//lTypeNode->Expand ();
		}
	}
	catch AnyExceptionDebug
	{}

	return lTypeNode;
}

/////////////////////////////////////////////////////////////////////////////

TreeNode^ AssemblyDlg::ShowField (TreeNode^ pParentNode, System::Reflection::FieldInfo^ pField)
{
	TreeNode^	lFieldNode = nullptr;

	try
	{
		if	(pField)
		{
			lFieldNode = pParentNode->Nodes->Add (pField->Name, String::Format ("{0} [{1}]", pField->Name, LogAssembly::FieldProps(pField)), OtherImage, OtherImage);
			ShowType (lFieldNode, pField->FieldType, false);
		}
	}
	catch AnyExceptionDebug
	{}

	return lFieldNode;
}

/////////////////////////////////////////////////////////////////////////////

TreeNode^ AssemblyDlg::ShowProperty (TreeNode^ pParentNode, System::Reflection::PropertyInfo^ pProperty)
{
	TreeNode^	lPropertyNode = nullptr;

	try
	{
		if	(pProperty)
		{
			lPropertyNode = pParentNode->Nodes->Add (pProperty->Name, String::Format ("{0} [{1}]", pProperty->Name, LogAssembly::PropertyProps(pProperty)), OtherImage, OtherImage);
			ShowType (lPropertyNode, pProperty->PropertyType, false);
			ShowAttributes (lPropertyNode, CustomAttributeData::GetCustomAttributes (pProperty));
		}
	}
	catch AnyExceptionDebug
	{}

	return lPropertyNode;
}

/////////////////////////////////////////////////////////////////////////////

TreeNode^ AssemblyDlg::ShowMethod (TreeNode^ pParentNode, System::Reflection::MethodInfo^ pMethod)
{
	TreeNode^	lMethodNode = nullptr;

	try
	{
		if	(pMethod)
		{
			String^					lDispId = gcnew String("");
			String^					lFuncAttr = gcnew String("");

			try
			{
				DispIdAttribute^		lDispIdAttribute;
				TypeLibFuncAttribute^	lFuncAttribute;

				lDispIdAttribute = safe_cast <DispIdAttribute^> (Attribute::GetCustomAttribute (pMethod, DispIdAttribute::typeid));
				if	(lDispIdAttribute)
				{
					lDispId = lDispId->Format (" DispId [{0:D} 0x{0:X}]", (Int32)lDispIdAttribute->Value);
				}
				lFuncAttribute = safe_cast <TypeLibFuncAttribute^> (Attribute::GetCustomAttribute (pMethod, TypeLibFuncAttribute::typeid));
				if	(lFuncAttribute)
				{
					lFuncAttr = (gcnew String(" Flags [")) + LogAssembly::TypeLibFuncFlagsStr(lFuncAttribute->Value) + "]";
				}
			}
			catch AnyExceptionDebug
			{}

#if	FALSE
			lMethodNode = pParentNode->Nodes->Add (pMethod->Name, String::Format ("{0} [{1}] [{2}]{3}{4}", pMethod->Name, LogAssembly::MethodProps(pMethod), LogAssembly::MethodCallType(pMethod->CallingConvention), lDispId, lFuncAttr), OtherImage, OtherImage);
#else
			lMethodNode = pParentNode->Nodes->Add (pMethod->Name, String::Format ("{0} [{1}]{2}", pMethod->Name, LogAssembly::MethodProps(pMethod), lFuncAttr), MethodImage, MethodImage);
			if	(!String::IsNullOrEmpty (lDispId))
			{
				lMethodNode->Nodes->Add ("DispId", lDispId->Trim(), OtherImage, OtherImage);
			}
#endif
			try
			{
				if	(
						(pMethod->ReturnType)
					&&	(String::Compare (pMethod->ReturnType->FullName, gcnew String("System.Void"), true) != 0)
					)
				{
					ShowType (lMethodNode, pMethod->ReturnType, false);
				}
			}
			catch AnyExceptionDebug
			{}

			try
			{
				array<ParameterInfo^>^	lParameters;
				ParameterInfo^			lParameter;

				if	(
						(lParameters = pMethod->GetParameters ())
					&&	(lParameters->Length > 0)
					)
				{
					for each (lParameter in lParameters)
					{
						ShowParameter (lMethodNode, lParameter);
					}
				}
			}
			catch AnyExceptionDebug
			{}

			lMethodNode->Nodes->Add ("Call", String::Format ("[{0}]", LogAssembly::MethodCallType(pMethod->CallingConvention)), OtherImage, OtherImage);
			ShowAttributes (lMethodNode, CustomAttributeData::GetCustomAttributes (pMethod));
		}
	}
	catch AnyExceptionDebug
	{}

	return lMethodNode;
}

/////////////////////////////////////////////////////////////////////////////

TreeNode^ AssemblyDlg::ShowEvent (TreeNode^ pParentNode, System::Reflection::EventInfo^ pEvent)
{
	TreeNode^	lEventNode = nullptr;

	try
	{
		if	(pEvent)
		{
			lEventNode = pParentNode->Nodes->Add (pEvent->Name, String::Format ("{0} [{1}]", pEvent->Name, LogAssembly::EventProps(pEvent)), EventImage, EventImage);

			try
			{
				array<MethodInfo^>^		lMethods;
				MethodInfo^				lMethod;
				TreeNode^				lNode;

				if	(
						(lMethod = pEvent->GetAddMethod ())
					&&	(Object::ReferenceEquals (lMethod->DeclaringType, pEvent->DeclaringType))
					)
				{
					ShowMethod (lNode = lEventNode->Nodes->Add ("0_Add", "Add", MethodImage, MethodImage), lMethod);
					lNode->Expand ();
				}
				if	(
						(lMethod = pEvent->GetRemoveMethod ())
					&&	(Object::ReferenceEquals (lMethod->DeclaringType, pEvent->DeclaringType))
					)
				{
					ShowMethod (lNode = lEventNode->Nodes->Add ("1_Remove", "Remove", MethodImage, MethodImage), lMethod);
					lNode->Expand ();
				}
				if	(
						(lMethod = pEvent->GetRaiseMethod ())
					&&	(Object::ReferenceEquals (lMethod->DeclaringType, pEvent->DeclaringType))
					)
				{
					ShowMethod (lNode = lEventNode->Nodes->Add ("2_Raise", "Raise", MethodImage, MethodImage), lMethod);
					lNode->Expand ();
				}

				if	(
						(lMethods = pEvent->GetOtherMethods())
					&&	(lMethods->Length > 0)
					)
				{
					lNode = lEventNode->Nodes->Add ("3_Other", "Other", FolderClosedImage, FolderClosedImage);
					for each (lMethod in lMethods)
					{
						if	(Object::ReferenceEquals (lMethod->DeclaringType, pEvent->DeclaringType))
						{
							ShowMethod (lNode, lMethod);
						}
					}
					if	(lNode->GetNodeCount(false) > 0)
					{
						lNode->Expand ();
					}
					else
					{
						lEventNode->Nodes->Remove (lNode);
					}
				}
			}
			catch AnyExceptionDebug
			{}

			ShowAttributes (lEventNode, CustomAttributeData::GetCustomAttributes (pEvent));
		}
	}
	catch AnyExceptionDebug
	{}

	return lEventNode;
}

/////////////////////////////////////////////////////////////////////////////

TreeNode^ AssemblyDlg::ShowParameter (TreeNode^ pParentNode, System::Reflection::ParameterInfo^ pParameter)
{
	TreeNode^	lParameterNode = nullptr;

	try
	{
		if	(pParameter)
		{
			lParameterNode = pParentNode->Nodes->Add (pParameter->Name, String::Format ("{0} [{1}]", pParameter->Name, LogAssembly::ParameterProps(pParameter)), OtherImage, OtherImage);
			ShowType (lParameterNode, pParameter->ParameterType, false);

			try
			{
				array<Type^>^	lTypes;
				Type^			lType;
				TreeNode^		lModifiersNode = nullptr;

				if	(
						(lTypes = pParameter->GetRequiredCustomModifiers ())
					&&	(lTypes->Length > 0)
					)
				{
					lModifiersNode = lParameterNode->Nodes->Add ("Modifiers", "Modifiers", FolderClosedImage, FolderClosedImage);
					for each (lType in lTypes)
					{
						ShowType (lModifiersNode, lType, false);
					}
				}

				if	(
						(lTypes = pParameter->GetOptionalCustomModifiers ())
					&&	(lTypes->Length > 0)
					)
				{
					if	(lModifiersNode == nullptr)
					{
						lModifiersNode = lParameterNode->Nodes->Add ("Modifiers", "Modifiers", FolderClosedImage, FolderClosedImage);
					}
					for each (lType in lTypes)
					{
						ShowType (lModifiersNode, lType, false);
					}
				}
			}
			catch AnyExceptionDebug
			{}
		}
	}
	catch AnyExceptionDebug
	{}

	return lParameterNode;
}

/////////////////////////////////////////////////////////////////////////////

void AssemblyDlg::ShowAttributes (TreeNode^ pParentNode, System::Collections::Generic::IList <System::Reflection::CustomAttributeData^>^ pAttributes)
{
	try
	{
		TreeNode^				lRootNode;
		CustomAttributeData^	lAttributeData;

		if	(
				(pAttributes)
			&&	(pAttributes->Count > 0)
			)
		{
			lRootNode = pParentNode->Nodes->Add ("Attributes", "Attributes", FolderClosedImage, FolderClosedImage);
			for each (lAttributeData in pAttributes)
			{
				lRootNode->Nodes->Add ("", lAttributeData->ToString(), OtherImage, OtherImage);
			}
			//lRootNode->Expand ();
		}
	}
	catch AnyExceptionDebug
	{}
}

/////////////////////////////////////////////////////////////////////////////
};
};