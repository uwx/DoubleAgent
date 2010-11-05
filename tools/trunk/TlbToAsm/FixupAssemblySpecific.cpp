#include "stdafx.h"
#include <corhdr.h>
#include "FixupAssemblySpecific.h"

#define	AX_PROPERTY_PREFIX	"Ctl"
#define	AX_EVENT_PREFIX		"Ctl"

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////
//
//	Rename types:
//	-	Assign types to appropriate namespaces
//	-	Remove "Class" suffixes on classes
//	-	Add "I" prefixes on interfaces
//	-	Remove redundant prefixes
//
String^ FixupAssemblySpecific::RenameClass (Type^ pSourceType, String^ pTypeName)
{
	String^	lNewName = nullptr;

	if	(
			(pSourceType->IsClass)
		&&	(!pSourceType->IsAbstract)
		)
	{
		lNewName = RenameControlClass (pSourceType, pTypeName);

		if	(!lNewName)
		{
			lNewName = RenameServerClass (pSourceType, pTypeName);
		}

		if	(
				(!lNewName)
			&&	(pTypeName->StartsWith ("Da"))
			&&	(pTypeName->EndsWith ("Class"))
			)
		{
			lNewName = pTypeName->Substring (2, pTypeName->Length-7);
		}
		if	(
				(!lNewName)
			&&	(pTypeName->EndsWith ("Class"))
			)
		{
			lNewName = pTypeName->Substring (0, pTypeName->Length-5);
		}
	}
#ifdef	_LOG_FIXES
	if	(lNewName)
	{
		LogMessage (_LOG_FIXES, _T("---> Rename     [%s] to [%s]"), _BT(pSourceType), _B(lNewName));
	}
#endif
	return lNewName;
}

String^ FixupAssemblySpecific::RenameInterface (Type^ pSourceType, String^ pTypeName)
{
	String^	lNewName = nullptr;

	if	(pSourceType->IsInterface)
	{
		lNewName = RenameControlInterface (pSourceType, pTypeName);

		if	(!lNewName)
		{
			lNewName = RenameServerInterface (pSourceType, pTypeName);
		}

		if	(!lNewName)
		{
			lNewName = RenameNativeInterface (pSourceType, pTypeName);
		}

		if	(
				(!lNewName)
			&&	(pTypeName->StartsWith ("Da"))
			)
		{
			lNewName = String::Concat ("I", pTypeName->Substring (2));
		}
		if	(!lNewName)
		{
			lNewName = String::Concat ("I", pTypeName);
		}
	}
#ifdef	_LOG_FIXES
	if	(lNewName)
	{
		LogMessage (_LOG_FIXES, _T("---> Rename     [%s] to [%s]"), _BT(pSourceType), _B(lNewName));
	}
#endif
	return lNewName;
}

/////////////////////////////////////////////////////////////////////////////

String^ FixupAssemblySpecific::RenameControlClass (Type^ pSourceType, String^ pTypeName)
{
	String^	lNewName = nullptr;

	if	(
			(pTypeName->StartsWith ("DaCtl"))
		&&	(pTypeName->EndsWith ("Class"))
		)
	{
		lNewName = pTypeName->Substring (5, pTypeName->Length-10);
	}
	else
	if	(pTypeName->StartsWith ("AxDa"))
	{
		if	(pTypeName->EndsWith ("EventMulticaster"))
		{
			lNewName = gcnew String ("EventMulticaster");
		}
		else
		{
			lNewName = String::Concat ("Ax", pTypeName->Substring (4));
		}
	}
	else
	if	(
			(pTypeName->StartsWith ("_DaCtlEvents_"))
		||	(pTypeName->StartsWith ("_DaCtlEvents2_"))
		)
	{
		if	(pTypeName->EndsWith ("Event"))
		{
			if	(mCopy->mAssemblyBuilder->FullName->Contains ("AxControl"))
			{
				lNewName = pTypeName->Replace ("_DaCtlEvents2_", AX_EVENT_PREFIX)->Replace ("_DaCtlEvents_", AX_EVENT_PREFIX);
			}
		}
		else
		if	(pTypeName->EndsWith ("EventHandler"))
		{
			if	(mCopy->mAssemblyBuilder->FullName->Contains ("AxControl"))
			{
				lNewName = pTypeName->Replace ("_DaCtlEvents2_", AX_EVENT_PREFIX)->Replace ("_DaCtlEvents_", AX_EVENT_PREFIX);
			}
			else
			{
				lNewName = pTypeName->Replace ("_DaCtlEvents2_", "")->Replace ("_DaCtlEvents_", "");
			}
		}
		else
		if	(
				(pTypeName->EndsWith ("EventProvider"))
			||	(pTypeName->EndsWith ("SinkHelper"))
			)
		{
			lNewName = pTypeName->Replace ("_DaCtlEvents2_", "")->Replace ("_DaCtlEvents_", "");
		}
		else
		{
			lNewName = pTypeName->Replace ("_DaCtlEvents2_", "Event")->Replace ("_DaCtlEvents_", "Event");
		}
	}
	return lNewName;
}

String^ FixupAssemblySpecific::RenameServerClass (Type^ pSourceType, String^ pTypeName)
{
	String^	lNewName = nullptr;

	if	(
			(pTypeName->StartsWith ("DaSvr"))
		&&	(pTypeName->EndsWith ("Class"))
		)
	{
		lNewName = pTypeName->Substring (5, pTypeName->Length-10);
	}
	else
	if	(pTypeName->StartsWith ("_DaSvrEvents2_"))
	{
		if	(
				(pTypeName->EndsWith ("Event"))
			||	(pTypeName->EndsWith ("EventHandler"))
			||	(pTypeName->EndsWith ("EventProvider"))
			)
		{
			lNewName = pTypeName->Replace ("_DaSvrEvents2_", "");
		}
		else
		{
			lNewName = pTypeName->Replace ("_DaSvrEvents2_", "Event");
		}
	}
	return lNewName;
}

/////////////////////////////////////////////////////////////////////////////

String^ FixupAssemblySpecific::RenameControlInterface (Type^ pSourceType, String^ pTypeName)
{
	String^	lNewName = nullptr;

	if	(pTypeName->StartsWith ("DaCtl"))
	{
		lNewName = String::Concat ("I", pTypeName->Substring (5));
	}
	else
	if	(
			(!pSourceType->IsImport)
		&&	(	
				(pTypeName->StartsWith ("_DaCtlEvents_"))
			||	(pTypeName->StartsWith ("_DaCtlEvents2_"))
			)
		)
	{
		if	(pTypeName->EndsWith ("_Event"))
		{
			if	(pTypeName->StartsWith ("_DaCtlEvents2_"))
			{
				lNewName = "IEvents2";
			}
			else
			{
				lNewName = "IEvents";
			}
		}
		else
		{
			lNewName = pTypeName->Replace ("_DaCtlEvents2_", "IEvents2_")->Replace ("_DaCtlEvents_", "IEvents_");
		}
	}
	return lNewName;
}

String^ FixupAssemblySpecific::RenameServerInterface (Type^ pSourceType, String^ pTypeName)
{
	String^	lNewName = nullptr;

	if	(pTypeName->StartsWith ("DaSvr"))
	{
		lNewName = String::Concat ("I", pTypeName->Substring (5));
	}
	else
	if	(
			(!pSourceType->IsImport)
		&&	(pTypeName->StartsWith ("_DaSvrEvents2_"))
		)
	{
		if	(pTypeName->EndsWith ("_Event"))
		{
			lNewName = "IEvents";
		}
		else
		{
			lNewName = pTypeName->Replace ("_DaSvrEvents2_", "IEvents_");
		}
	}
	return lNewName;
}

/////////////////////////////////////////////////////////////////////////////

String^ FixupAssemblySpecific::RenameNativeInterface (Type^ pSourceType, String^ pTypeName)
{
	String^	lNewName = nullptr;
#if	TRUE
	if	(
			(pSourceType->IsInterface)
		&&	(pSourceType->IsImport)
		)
	{
		if	(
				(pTypeName->StartsWith ("IDaSvr"))
			||	(pTypeName->StartsWith ("_DaSvr"))
			||	(pTypeName->StartsWith ("IDaCtl"))
			||	(pTypeName->StartsWith ("_DaCtl"))
			||	(pTypeName->StartsWith ("IDa"))
			)
		{
			lNewName = String::Concat ("Native.", pTypeName);
		}
	}
#endif
	return lNewName;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
//
//	Hide classes that are only used internally
//
void FixupAssemblySpecific::HideInternalClass (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes)
{
#if	TRUE
	Type^			lSourceType = nullptr;
	TypeBuilder^	lTargetType = nullptr;

	try
	{
		lSourceType = safe_cast <Type^> (pSource);
		lTargetType = safe_cast <TypeBuilder^> (pTarget);
	}
	catch AnyExceptionSilent

	if	(
			(lSourceType)
		&&	(lTargetType)
		&&	(lSourceType->IsClass)
		&&	(!lSourceType->IsAbstract)
		&&	(!lSourceType->IsImport)
		&&	(
				(lSourceType->Name->EndsWith ("SinkHelper"))
			||	(lSourceType->Name->EndsWith ("EventMulticaster"))
			||	(lSourceType->Name->Contains ("HelpComplete"))
			)
		)
	{
		try
		{
			array <Type^>^		lAttrArgTypes = gcnew array <Type^> (1);
			array <Object^>^	lAttrArgValues = gcnew array <Object^> (1);

#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("---> Hide       [%s] (internal)"), _BT(lTargetType));
#endif
			lAttrArgTypes [0] = TypeLibTypeFlags::typeid;
			lAttrArgValues [0] = (TypeLibTypeFlags)((int)TypeLibTypeFlags::FHidden | (int)TypeLibTypeFlags::FRestricted);
			pCustomAttributes->Add (gcnew CustomAttributeBuilder (TypeLibTypeAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));
		}
		catch AnyExceptionSilent
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
//
//	For now - skip the methods that the ActiveX interop imported created
//	to handle default parameters.
//
bool FixupAssemblySpecific::SkipGeneratedMethod (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes)
{
	bool	lRet = false;

#if	TRUE
	try
	{
		array<MethodInfo^>^		lMethods;
		MethodInfo^				lMethod;
		array<ParameterInfo^>^	lParameters;
		array<ParameterInfo^>^	lOtherParameters;

		if	(
				(lMethods = pSourceMethod->DeclaringType->GetMethods (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
			&&	(lParameters = pSourceMethod->GetParameters ())
			)
		{
			for each (lMethod in lMethods)
			{
				if	(
						(String::Compare (lMethod->Name, pSourceMethod->Name) == 0)
					&&	(lOtherParameters = lMethod->GetParameters ())
					&&	(lOtherParameters->Length > lParameters->Length)
					)
				{
#ifdef	_LOG_FIXES
					LogMessage (_LOG_FIXES, _T("---> Skip       [%s.%s] for [%s]"), _BMT(pSourceMethod), _B(mCopy->GetMethodSignature (pSourceMethod)), _B(mCopy->GetMethodSignature (lMethod)));
#endif
					lRet = true;
					break;
				}
			}
		}
	}
	catch AnyExceptionSilent
#endif

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
//
//	Skip ActiveX properties that don't work with Windows Forms
//	Skip obsolete Agent Control properties
//TODO - Replace MouseIcon property with Cursor property
//
bool FixupAssemblySpecific::SkipObsoleteProperty (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes)
{
	bool	lRet = false;

#if	TRUE
	try
	{
		PropertyInfo^		lMethodProperty;
		DispIdAttribute^	lDispIdAttribute = nullptr;

		if	(
				(pSourceMethod->DeclaringType->IsClass)
			&&	(pSourceMethod->IsSpecialName)
			&&	(
					(pSourceMethod->Name->StartsWith ("get_"))
				||	(pSourceMethod->Name->StartsWith ("set_"))
				)
			&&	(lMethodProperty = pSourceMethod->DeclaringType->GetProperty (pSourceMethod->Name->Substring(4), BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
			&&	(lDispIdAttribute = mCopy->GetDispId (lMethodProperty))
			&&	(
					(pSourceMethod->Name->EndsWith ("Suspended"))
				||	(lDispIdAttribute->Value == DISPID_MOUSEICON)
				)
			)
		{
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("---> Skip       [%s.%s] for [%s]"), _BMT(pSourceMethod), _BM(pSourceMethod), _BM(lMethodProperty));
#endif
			lRet = true;
		}
	}
	catch AnyExceptionSilent
#endif
	return lRet;
}

bool FixupAssemblySpecific::SkipObsoleteProperty (PropertyInfo^ pSourceProperty, Reflection::PropertyAttributes & pPropertyAttributes)
{
	bool	lRet = false;

#if	TRUE
	try
	{
		DispIdAttribute^	lDispIdAttribute = nullptr;

		if	(
				(pSourceProperty->DeclaringType->IsClass)
			&&	(lDispIdAttribute = mCopy->GetDispId (pSourceProperty))
			&&	(
					(pSourceProperty->Name->EndsWith ("Suspended"))
				||	(lDispIdAttribute->Value == DISPID_MOUSEICON)
				)
			)
		{
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("---> Skip       [%s.%s]"), _BMT(pSourceProperty), _BM(pSourceProperty));
#endif
			lRet = true;
		}
	}
	catch AnyExceptionSilent
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
//
//	Skip deprecated events
//
bool FixupAssemblySpecific::SkipObsoleteEvent (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes)
{
	bool	lRet = false;
#if	TRUE
	try
	{
		EventInfo^	lMethodEvent;

		if	(
				(
					(pSourceMethod->Name->EndsWith ("Restart"))
				||	(pSourceMethod->Name->EndsWith ("Shutdown"))
				||	(
						(pSourceMethod->DeclaringType->IsSubclassOf (System::Windows::Forms::AxHost::typeid))
					&&	(pSourceMethod->Name->EndsWith ("HelpComplete"))
					)
				)
			&&	(
					(
						(pSourceMethod->Name->StartsWith ("add_"))
					&&	(lMethodEvent = pSourceMethod->DeclaringType->GetEvent (pSourceMethod->Name->Substring(4), BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
					)
				||	(
						(pSourceMethod->Name->StartsWith ("remove_"))
					&&	(lMethodEvent = pSourceMethod->DeclaringType->GetEvent (pSourceMethod->Name->Substring(7), BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
					)
				)
			)
		{
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("---> Skip       [%s.%s] for [%s]"), _BMT(pSourceMethod), _BM(pSourceMethod), _BM(lMethodEvent));
#endif
			lRet = true;
		}
	}
	catch AnyExceptionSilent
#endif
	return lRet;
}

bool FixupAssemblySpecific::SkipObsoleteEvent (EventInfo^ pSourceEvent, EventAttributes & pEventAttributes)
{
	bool	lRet = false;
#if	TRUE
	try
	{
		if	(
				(pSourceEvent->Name->EndsWith ("Restart"))
			||	(pSourceEvent->Name->EndsWith ("Shutdown"))
			||	(
					(pSourceEvent->DeclaringType->IsSubclassOf (System::Windows::Forms::AxHost::typeid))
				&&	(pSourceEvent->Name->EndsWith ("HelpComplete"))
				)
			)
		{
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("---> Skip       [%s.%s]"), _BMT(pSourceEvent), _BM(pSourceEvent));
#endif
			lRet = true;
		}
	}
	catch AnyExceptionSilent
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
//
//	Rename ActiveX properties to differentiate them from Windows Forms properties
//	(and rename CtlAutoSize back to AutoSize while we're at it)
//
void FixupAssemblySpecific::FixMethodName (MethodBase^ pSourceMethod, String^& pMethodName)
{
	try
	{
		PropertyInfo^		lMethodProperty;
		DispIdAttribute^	lDispIdAttribute = nullptr;

		if	(pSourceMethod->DeclaringType->IsSubclassOf (System::Windows::Forms::AxHost::typeid))
		{
			if	(
					(pSourceMethod->IsSpecialName)
				&&	(
						(pMethodName->StartsWith ("get_"))
					||	(pMethodName->StartsWith ("set_"))
					)
				&&	(lMethodProperty = pSourceMethod->DeclaringType->GetProperty (pSourceMethod->Name->Substring(4), BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
				&&	(lDispIdAttribute = mCopy->GetDispId (lMethodProperty))
				&&	(
						(lDispIdAttribute->Value == DISPID_AUTOSIZE)
					||	(lDispIdAttribute->Value == DISPID_BACKCOLOR)
					||	(lDispIdAttribute->Value == DISPID_BORDERCOLOR)
					||	(lDispIdAttribute->Value == DISPID_BORDERSTYLE)
					||	(lDispIdAttribute->Value == DISPID_BORDERWIDTH)
					||	(lDispIdAttribute->Value == DISPID_BORDERVISIBLE)
					||	(lDispIdAttribute->Value == DISPID_MOUSEPOINTER)
					||	(lDispIdAttribute->Value == DISPID_MOUSEICON)
					)
				)
			{
#if	TRUE
				if	(pMethodName->EndsWith ("CtlAutoSize"))
				{
					pMethodName = pMethodName->Replace ("CtlAutoSize", "AutoSize");
				}
				if	(pMethodName->StartsWith ("get_"))
				{
					pMethodName = pMethodName->Replace ("get_", "get_"AX_PROPERTY_PREFIX);
				}
				else
				if	(pMethodName->StartsWith ("set_"))
				{
					pMethodName = pMethodName->Replace ("set_", "set_"AX_PROPERTY_PREFIX);
				}
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("---> Method     [%s.%s] as [%s]"), _BMT(pSourceMethod), _BM(pSourceMethod), _B(pMethodName));
#endif
#endif
			}
			else
			if	(pMethodName->StartsWith ("RaiseOn"))
			{
				pMethodName = pMethodName->Replace ("RaiseOn", "Raise"AX_EVENT_PREFIX);
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("---> Method     [%s.%s] as [%s]"), _BMT(pSourceMethod), _BM(pSourceMethod), _B(pMethodName));
#endif
			}
		}
	}
	catch AnyExceptionSilent
}
//
//	Rename ActiveX properties to highlight them and avoid naming collisions
//
void FixupAssemblySpecific::FixPropertyName (PropertyInfo^ pSourceProperty, String^& pPropertyName)
{
	try
	{
		DispIdAttribute^	lDispIdAttribute = nullptr;

		if	(pSourceProperty->DeclaringType->IsSubclassOf (System::Windows::Forms::AxHost::typeid))
		{
			if	(
					(lDispIdAttribute = mCopy->GetDispId (pSourceProperty))
				&&	(
						(lDispIdAttribute->Value == DISPID_AUTOSIZE)
					||	(lDispIdAttribute->Value == DISPID_BACKCOLOR)
					||	(lDispIdAttribute->Value == DISPID_BORDERCOLOR)
					||	(lDispIdAttribute->Value == DISPID_BORDERSTYLE)
					||	(lDispIdAttribute->Value == DISPID_BORDERWIDTH)
					||	(lDispIdAttribute->Value == DISPID_BORDERVISIBLE)
					||	(lDispIdAttribute->Value == DISPID_MOUSEPOINTER)
					||	(lDispIdAttribute->Value == DISPID_MOUSEICON)
					)
				)
			{
#if	TRUE
				if	(pPropertyName->EndsWith ("CtlAutoSize"))
				{
					pPropertyName = pPropertyName->Replace ("CtlAutoSize", "AutoSize");
				}
				pPropertyName = String::Concat (AX_PROPERTY_PREFIX, pPropertyName);
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("---> Property   [%s.%s] as [%s]"), _BMT(pSourceProperty), _BM(pSourceProperty), _B(pPropertyName));
#endif
#endif
			}
		}
	}
	catch AnyExceptionSilent
}
//
//	Apply consistent naming conventions to event names
//
void FixupAssemblySpecific::FixEventName (EventInfo^ pSourceEvent, String^& pEventName)
{
	if	(pSourceEvent->DeclaringType->IsSubclassOf (System::Windows::Forms::AxHost::typeid))
	{
		if	(pEventName->EndsWith ("Event"))
		{
			pEventName = pEventName->Substring (0, pEventName->Length-5);
		}
		if	(!pEventName->StartsWith (AX_EVENT_PREFIX))
		{
			pEventName = String::Concat (AX_EVENT_PREFIX, pEventName);
		}
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("---> Event      [%s.%s] as [%s]"), _BMT(pSourceEvent), _BM(pSourceEvent), _B(pEventName));
#endif
	}
}
//
//	Reflect event name changes in multicast delegate fields
//
void FixupAssemblySpecific::FixEventFieldName (FieldInfo^ pSourceField, String^& pFieldName)
{
	if	(pSourceField->DeclaringType->IsSubclassOf (System::Windows::Forms::AxHost::typeid))
	{
		if	(pSourceField->FieldType->Name->EndsWith ("EventHandler"))
		{
			pFieldName = String::Concat (AX_PROPERTY_PREFIX, pFieldName);
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("---> Field      [%s.%s] as [%s]"), _BMT(pSourceField), _BM(pSourceField), _B(pFieldName));
#endif
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void FixupAssemblySpecific::SetActiveXControlAttributes (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes)
{
	Type^			lSourceType = nullptr;
	TypeBuilder^	lTargetType = nullptr;

	try
	{
		lSourceType = safe_cast <Type^> (pSource);
		lTargetType = safe_cast <TypeBuilder^> (pTarget);
	}
	catch AnyExceptionSilent

	if	(
			(lSourceType)
		&&	(lTargetType)
		&&	(lSourceType->IsClass)
		&&	(lSourceType->IsSubclassOf (System::Windows::Forms::AxHost::typeid))
		)
	{
		try
		{
			array <Type^>^		lAttrArgTypes = gcnew array <Type^> (1);
			array <Object^>^	lAttrArgValues = gcnew array <Object^> (1);

#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("---> ToolBox    [%s]"), _BT(lSourceType));
#endif
			lAttrArgTypes [0] = System::Boolean::typeid;
			lAttrArgValues [0] = true;
			pCustomAttributes->Add (gcnew CustomAttributeBuilder (ToolboxItemAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));
		}
		catch AnyExceptionSilent
		{}
	}
}

/////////////////////////////////////////////////////////////////////////////

void FixupAssemblySpecific::SetActiveXMethodVisibility (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes)
{
	MethodInfo^			lSourceMethod = nullptr;
	MethodBuilder^		lTargetMethod = nullptr;
	DispIdAttribute^	lDispIdAttribute = nullptr;

	try
	{
		lSourceMethod = safe_cast <MethodInfo^> (pSource);
		lTargetMethod = safe_cast <MethodBuilder^> (pTarget);
		lDispIdAttribute = mCopy->GetDispId (lSourceMethod);

		if	(
				(!lDispIdAttribute)
			&&	(lSourceMethod->DeclaringType->IsSubclassOf (System::Windows::Forms::AxHost::typeid))
			&&	(lSourceMethod->IsSpecialName)
			&&	(
					(lSourceMethod->Name->StartsWith ("get_"))
				||	(lSourceMethod->Name->StartsWith ("set_"))
				)
			)
		{
			PropertyInfo^	lMethodProperty;

			if	(lMethodProperty = lSourceMethod->DeclaringType->GetProperty (lSourceMethod->Name->Substring(4), BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
			{
				lDispIdAttribute = mCopy->GetDispId (lMethodProperty);
			}
		}
	}
	catch AnyExceptionSilent

	if	(
			(lSourceMethod)
		&&	(lSourceMethod->DeclaringType->IsSubclassOf (System::Windows::Forms::AxHost::typeid))
		&&	(lTargetMethod)
		&&	(lDispIdAttribute)
		)
	{
		try
		{
			array <Type^>^		lAttrArgTypes = gcnew array <Type^> (1);
			array <Object^>^	lAttrArgValues = gcnew array <Object^> (1);

			if	(
					(lDispIdAttribute->Value == 4) //AudioOutput
				||	(lDispIdAttribute->Value == 5) //SpeechInput
				)
			{
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("---> Hide       [%s.%s]"), _BMT(lSourceMethod), _BM(lSourceMethod));
#endif
				lAttrArgTypes [0] = TypeLibFuncFlags::typeid;
				lAttrArgValues [0] = (TypeLibFuncFlags)((int)TypeLibFuncFlags::FHidden | (int)TypeLibFuncFlags::FRestricted);
				pCustomAttributes->Add (gcnew CustomAttributeBuilder (TypeLibFuncAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));
			}
		}
		catch AnyExceptionSilent
		{}
	}
}

/////////////////////////////////////////////////////////////////////////////

void FixupAssemblySpecific::SetActiveXPropertyVisibility (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes)
{
	PropertyInfo^		lSourceProperty = nullptr;
	PropertyBuilder^	lTargetProperty = nullptr;
	DispIdAttribute^	lDispIdAttribute = nullptr;

	try
	{
		lSourceProperty = safe_cast <PropertyInfo^> (pSource);
		lTargetProperty = safe_cast <PropertyBuilder^> (pTarget);
		lDispIdAttribute = mCopy->GetDispId (lSourceProperty);
	}
	catch AnyExceptionSilent

	if	(
			(lSourceProperty)
		&&	(lSourceProperty->DeclaringType->IsSubclassOf (System::Windows::Forms::AxHost::typeid))
		&&	(lTargetProperty)
		&&	(lDispIdAttribute)
		)
	{
		try
		{
			array <Type^>^		lAttrArgTypes = gcnew array <Type^> (1);
			array <Object^>^	lAttrArgValues = gcnew array <Object^> (1);

			if	(lDispIdAttribute->Value == 30) //CharacterStyle
			{
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("---> Browse     [false] for [%s.%s]"), _BMT(lSourceProperty), _BM(lSourceProperty));
#endif
				lAttrArgTypes [0] = System::Boolean::typeid;
				lAttrArgValues [0] = (System::Boolean)false;
				pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::ComponentModel::BrowsableAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));
			}

			if	(
					(lDispIdAttribute->Value == 9) //Connected
				||	(lDispIdAttribute->Value == 30) //CharacterStyle
				)
			{
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("---> Editor     [System::ComponentModel::EditorBrowsableState::Advanced] for [%s.%s]"), _BMT(lSourceProperty), _BM(lSourceProperty));
#endif
				lAttrArgTypes [0] = System::ComponentModel::EditorBrowsableState::typeid;
				lAttrArgValues [0] = System::ComponentModel::EditorBrowsableState::Advanced;
				pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::ComponentModel::EditorBrowsableAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));
			}

			if	(
					(lDispIdAttribute->Value == 4) //AudioOutput
				||	(lDispIdAttribute->Value == 5) //SpeechInput
				)
			{
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("---> Hide       [%s.%s]"), _BMT(lSourceProperty), _BM(lSourceProperty));
#endif
				lAttrArgTypes [0] = TypeLibVarFlags::typeid;
				lAttrArgValues [0] = (TypeLibVarFlags)((int)TypeLibVarFlags::FHidden | (int)TypeLibVarFlags::FRestricted);
				pCustomAttributes->Add (gcnew CustomAttributeBuilder (TypeLibVarAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));
			}

#if	TRUE
			if	(
					(lDispIdAttribute->Value == DISPID_AUTOSIZE)
				||	(lDispIdAttribute->Value == DISPID_BACKCOLOR)
				||	(lDispIdAttribute->Value == DISPID_BORDERCOLOR)
				||	(lDispIdAttribute->Value == DISPID_BORDERSTYLE)
				||	(lDispIdAttribute->Value == DISPID_BORDERWIDTH)
				||	(lDispIdAttribute->Value == DISPID_BORDERVISIBLE)
				||	(lDispIdAttribute->Value == DISPID_MOUSEPOINTER)
				)
			{
				if	(lDispIdAttribute->Value == DISPID_AUTOSIZE)
				{
					lAttrArgValues [0] = gcnew String("AutoSize");
				}
				else
				if	(lDispIdAttribute->Value == DISPID_BACKCOLOR)
				{
					lAttrArgValues [0] = gcnew String("BackgroundColor");
				}
				else
				if	(lDispIdAttribute->Value == DISPID_BORDERCOLOR)
				{
					lAttrArgValues [0] = gcnew String("BorderColor");
				}
				else
				if	(lDispIdAttribute->Value == DISPID_BORDERSTYLE)
				{
					lAttrArgValues [0] = gcnew String("BorderStyle");
				}
				else
				if	(lDispIdAttribute->Value == DISPID_BORDERWIDTH)
				{
					lAttrArgValues [0] = gcnew String("BorderWidth");
				}
				else
				if	(lDispIdAttribute->Value == DISPID_BORDERVISIBLE)
				{
					lAttrArgValues [0] = gcnew String("BorderVisible");
				}
				else
				if	(lDispIdAttribute->Value == DISPID_MOUSEPOINTER)
				{
					lAttrArgValues [0] = gcnew String("CursorStyle");
				}

				lAttrArgTypes [0] = System::String::typeid;
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("---> Display    [%s] for [%s.%s]"), _B(lAttrArgValues [0]->ToString()), _BMT(lSourceProperty), _BM(lSourceProperty));
#endif
				pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::ComponentModel::DisplayNameAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));
			}
#endif

#if	TRUE
			if	(lSourceProperty->PropertyType->IsImport)
			{
#if	TRUE
				lAttrArgTypes [0] = System::Boolean::typeid;
				lAttrArgValues [0] = gcnew System::Boolean (false);
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("---> Bindable   [%s] for [%s.%s]"), _B(lAttrArgValues [0]->ToString()), _BMT(lSourceProperty), _BM(lSourceProperty));
#endif
				pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::ComponentModel::BindableAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));
#endif

#if	TRUE
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("---> Editor     [System::ComponentModel::EditorBrowsableState::Advanced] for [%s.%s]"), _BMT(lSourceProperty), _BM(lSourceProperty));
#endif
				lAttrArgTypes [0] = System::ComponentModel::EditorBrowsableState::typeid;
				lAttrArgValues [0] = System::ComponentModel::EditorBrowsableState::Advanced;
				pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::ComponentModel::EditorBrowsableAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));
#endif
			}
#endif
		}
		catch AnyExceptionSilent
		{}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
//	Set the designer category for ActiveX properties
//
void FixupAssemblySpecific::SetActiveXPropertyCategory (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes)
{
	PropertyInfo^		lSourceProperty = nullptr;
	PropertyBuilder^	lTargetProperty = nullptr;
	DispIdAttribute^	lDispIdAttribute = nullptr;

	try
	{
		lSourceProperty = safe_cast <PropertyInfo^> (pSource);
		lTargetProperty = safe_cast <PropertyBuilder^> (pTarget);
		lDispIdAttribute = safe_cast <DispIdAttribute^> (Attribute::GetCustomAttribute (lSourceProperty, DispIdAttribute::typeid));
	}
	catch AnyExceptionSilent

	if	(
			(lSourceProperty)
		&&	(lSourceProperty->DeclaringType->IsSubclassOf (System::Windows::Forms::AxHost::typeid))
		&&	(lTargetProperty)
		&&	(lDispIdAttribute)
		)
	{
		try
		{
			array <Type^>^		lAttrArgTypes = gcnew array <Type^> (1);
			array <Object^>^	lAttrArgValues = gcnew array <Object^> (1);

			switch (lDispIdAttribute->Value)
			{
				case DISPID_AUTOSIZE:			lAttrArgValues [0] = gcnew String("Layout"); break;
				case DISPID_BACKCOLOR:			lAttrArgValues [0] = gcnew String("Appearance"); break;
				case DISPID_BORDERCOLOR:		lAttrArgValues [0] = gcnew String("Appearance"); break;
				case DISPID_BORDERSTYLE:		lAttrArgValues [0] = gcnew String("Appearance"); break;
				case DISPID_BORDERWIDTH:		lAttrArgValues [0] = gcnew String("Appearance"); break;
				case DISPID_BORDERVISIBLE:		lAttrArgValues [0] = gcnew String("Appearance"); break;
				case DISPID_MOUSEPOINTER:		lAttrArgValues [0] = gcnew String("Appearance"); break;
				case DISPID_MOUSEICON:			lAttrArgValues [0] = gcnew String("Appearance"); break;
				case 41: /*AutoConnect*/		lAttrArgValues [0] = gcnew String("Behavior"); break;
				case 9: /*Connected*/			lAttrArgValues [0] = gcnew String("Behavior"); break;
				case 21: /*RaiseRequestErrors*/	lAttrArgValues [0] = gcnew String("Behavior"); break;
			}

			if	(lAttrArgValues [0])
			{
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("---> Category   [%s] for [%s.%s]"), _B((String^)lAttrArgValues[0]), _BMT(lSourceProperty), _BM(lSourceProperty));
#endif
				lAttrArgTypes [0] = String::typeid;
				pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::ComponentModel::CategoryAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));
			}
		}
		catch AnyExceptionSilent
		{}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
//	Set the designer category for ActiveX events
//
void FixupAssemblySpecific::SetActiveXEventCategory (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes)
{
	EventInfo^			lSourceEvent = nullptr;
	EventBuilder^		lTargetEvent = nullptr;

	try
	{
		lSourceEvent = safe_cast <EventInfo^> (pSource);
		lTargetEvent = safe_cast <EventBuilder^> (pTarget);
	}
	catch AnyExceptionSilent

	if	(
			(lSourceEvent)
		&&	(lTargetEvent)
		)
	{
		try
		{
			array <Type^>^		lAttrArgTypes = gcnew array <Type^> (1);
			array <Object^>^	lAttrArgValues = gcnew array <Object^> (1);

			if	(lSourceEvent->DeclaringType->IsSubclassOf (System::Windows::Forms::AxHost::typeid))
			{
				if	(
						(lSourceEvent->Name->StartsWith ("ActivateInput"))
					||	(lSourceEvent->Name->StartsWith ("DeactivateInput"))
					)
				{
					lAttrArgValues [0] = gcnew String("Focus");
				}
				else
				if	(
						(lSourceEvent->Name->StartsWith ("Click"))
					||	(lSourceEvent->Name->StartsWith ("DblClick"))
					||	(lSourceEvent->Name->StartsWith ("Show"))
					||	(lSourceEvent->Name->StartsWith ("Hide"))
					||	(lSourceEvent->Name->StartsWith ("RequestStart"))
					||	(lSourceEvent->Name->StartsWith ("RequestComplete"))
					||	(lSourceEvent->Name->StartsWith ("Command"))
					||	(lSourceEvent->Name->StartsWith ("BalloonShow"))
					||	(lSourceEvent->Name->StartsWith ("BalloonHide"))
					)
				{
					lAttrArgValues [0] = gcnew String("Action");
				}
				else
				if	(
						(lSourceEvent->Name->StartsWith ("DragStart"))
					||	(lSourceEvent->Name->StartsWith ("DragComplete"))
					||	(lSourceEvent->Name->StartsWith ("Move"))
					||	(lSourceEvent->Name->StartsWith ("Size"))
					)
				{
					lAttrArgValues [0] = gcnew String("Layout");
				}
				else
				if	(
						(lSourceEvent->Name->StartsWith ("IdleStart"))
					||	(lSourceEvent->Name->StartsWith ("IdleComplete"))
					||	(lSourceEvent->Name->StartsWith ("DefaultCharacterChange"))
					||	(lSourceEvent->Name->StartsWith ("AgentPropertyChange"))
					||	(lSourceEvent->Name->StartsWith ("ActiveClientChange"))
					)
				{
					lAttrArgValues [0] = gcnew String("Behavior");
				}
				else
				if	(
						(lSourceEvent->Name->StartsWith ("Bookmark"))
					||	(lSourceEvent->Name->StartsWith ("ListenStart"))
					||	(lSourceEvent->Name->StartsWith ("ListenComplete"))
					||	(lSourceEvent->Name->StartsWith ("Voice"))
					)
				{
					lAttrArgValues [0] = gcnew String("Speech");
				}

				if	(lAttrArgValues [0])
				{
#ifdef	_LOG_FIXES
					LogMessage (_LOG_FIXES, _T("---> Category   [%s] for [%s.%s]"), _B((String^)lAttrArgValues[0]), _BMT(lSourceEvent), _BM(lSourceEvent));
#endif
					lAttrArgTypes [0] = String::typeid;
					pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::ComponentModel::CategoryAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));
				}
			}
			else
			if	(
					(lSourceEvent->DeclaringType->IsClass)
				&&	(lSourceEvent->Name->EndsWith ("HelpComplete"))
				)
			{
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("---> Hide       [%s.%s"), _BMT(lSourceEvent), _BM(lSourceEvent));
#endif
				lAttrArgTypes [0] = TypeLibFuncFlags::typeid;
				lAttrArgValues [0] = (TypeLibFuncFlags)((int)TypeLibFuncFlags::FHidden | (int)TypeLibFuncFlags::FRestricted);
				pCustomAttributes->Add (gcnew CustomAttributeBuilder (TypeLibFuncAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));
			}
		}
		catch AnyExceptionSilent
		{}
	}
}

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
