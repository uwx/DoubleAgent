#include "stdafx.h"
#include <corhdr.h>
#include "FixupAssemblySpecific.h"

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
	}
#ifdef	_LOG_FIXES
	if	(lNewName)
	{
		LogMessage (_LOG_FIXES, _T("--> Rename     [%s] to [%s]"), _BT(pSourceType), _B(lNewName));
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
	}
#ifdef	_LOG_FIXES
	if	(lNewName)
	{
		LogMessage (_LOG_FIXES, _T("--> Rename     [%s] to [%s]"), _BT(pSourceType), _B(lNewName));
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
	if	(pTypeName->StartsWith ("_DaCtlEvents_"))
	{
		if	(
				(pTypeName->EndsWith ("Event"))
			||	(pTypeName->EndsWith ("EventHandler"))
			||	(pTypeName->EndsWith ("EventProvider"))
			)
		{
			lNewName = pTypeName->Substring (13);
		}
		else
		{
			lNewName = pTypeName->Replace ("_DaCtlEvents_", "Event");
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
	if	(pTypeName->StartsWith ("_DaSvrEvents_"))
	{
		if	(
				(pTypeName->EndsWith ("Event"))
			||	(pTypeName->EndsWith ("EventHandler"))
			||	(pTypeName->EndsWith ("EventProvider"))
			)
		{
			lNewName = pTypeName->Substring (13);
		}
		else
		{
			lNewName = pTypeName->Replace ("_DaSvrEvents_", "Event");
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
		&&	(pTypeName->StartsWith ("_DaCtlEvents_"))
		)
	{
		if	(pTypeName->EndsWith ("_Event"))
		{
			lNewName = "IEvents";
		}
		else
		{
			lNewName = pTypeName->Replace ("_DaCtlEvents_", "IEvents_");
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
		&&	(pTypeName->StartsWith ("_DaSvrEvents_"))
		)
	{
		if	(pTypeName->EndsWith ("_Event"))
		{
			lNewName = "IEvents";
		}
		else
		{
			lNewName = pTypeName->Replace ("_DaSvrEvents_", "IEvents_");
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
		&&	(
				(lSourceType->Name->EndsWith ("SinkHelper"))
			||	(lSourceType->Name->EndsWith ("EventMulticaster"))
			)
		)
	{
		try
		{
			array <Type^>^		lTypeLibTypeArgs = gcnew array <Type^> (1);
			array <Object^>^	lTypeLibTypeFlags = gcnew array <Object^> (1);

#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> Hide       [%s] (internal)"), _BT(lTargetType));
#endif
			lTypeLibTypeArgs [0] = TypeLibTypeFlags::typeid;
			lTypeLibTypeFlags [0] = (TypeLibTypeFlags)((int)TypeLibTypeFlags::FHidden | (int)TypeLibTypeFlags::FRestricted);
			pCustomAttributes->Add (gcnew CustomAttributeBuilder (TypeLibTypeAttribute::typeid->GetConstructor(lTypeLibTypeArgs), lTypeLibTypeFlags));
		}
		catch AnyExceptionSilent
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
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
			LogMessage (_LOG_FIXES, _T("--> Skip       [%s] in [%s] for [%s]"), _BM(pSourceMethod), _BMT(pSourceMethod), _BM(lMethodProperty));
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
			LogMessage (_LOG_FIXES, _T("--> Skip       [%s] in [%s]"), _BM(pSourceProperty), _BMT(pSourceProperty));
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
				||	(pSourceMethod->Name->EndsWith ("HelpComplete"))
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
			LogMessage (_LOG_FIXES, _T("--> Skip       [%s] in [%s] for [%s]"), _BM(pSourceMethod), _BMT(pSourceMethod), _BM(lMethodEvent));
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
			||	(pSourceEvent->Name->EndsWith ("HelpComplete"))
			)
		{
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> Skip       [%s] in [%s]"), _BM(pSourceEvent), _BMT(pSourceEvent));
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
#if	TRUE
	try
	{
		PropertyInfo^		lMethodProperty;
		DispIdAttribute^	lDispIdAttribute = nullptr;

		if	(
				(pSourceMethod->DeclaringType->IsSubclassOf (System::Windows::Forms::AxHost::typeid))
			&&	(pSourceMethod->IsSpecialName)
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
			if	(pMethodName->EndsWith ("CtlAutoSize"))
			{
				pMethodName = pMethodName->Replace ("CtlAutoSize", "AutoSize");
			}
			if	(pMethodName->StartsWith ("get_"))
			{
				pMethodName = pMethodName->Replace ("get_", "get_Ax");
			}
			else
			if	(pMethodName->StartsWith ("set_"))
			{
				pMethodName = pMethodName->Replace ("set_", "set_Ax");
			}
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> Method     [%s] as [%s] in [%s]"), _BM(pSourceMethod), _B(pMethodName), _BMT(pSourceMethod));
#endif
		}
	}
	catch AnyExceptionSilent
#endif
}

void FixupAssemblySpecific::FixPropertyName (PropertyInfo^ pSourceProperty, String^& pPropertyName)
{
#if	TRUE
	try
	{
		DispIdAttribute^	lDispIdAttribute = nullptr;

		if	(
				(pSourceProperty->DeclaringType->IsSubclassOf (System::Windows::Forms::AxHost::typeid))
			&&	(lDispIdAttribute = mCopy->GetDispId (pSourceProperty))
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
			if	(pPropertyName->EndsWith ("CtlAutoSize"))
			{
				pPropertyName = pPropertyName->Replace ("CtlAutoSize", "AutoSize");
			}
			pPropertyName = String::Concat ("Ax", pPropertyName);
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> Property   [%s] as [%s] in [%s]"), _BM(pSourceProperty), _B(pPropertyName), _BMT(pSourceProperty));
#endif
		}
	}
	catch AnyExceptionSilent
#endif
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
		if	(!pEventName->StartsWith ("Agent"))
		{
			pEventName = String::Concat ("Agent", pEventName);
		}
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("--> Event      [%s] as [%s] in [%s]"), _BM(pSourceEvent), _B(pEventName), _BMT(pSourceEvent));
#endif
	}
#if	FALSE
	else
	{
		if	(!pEventName->StartsWith ("On"))
		{
			pEventName = String::Concat ("On", pEventName);
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> Event      [%s] as [%s] in [%s]"), _BM(pSourceEvent), _B(pEventName), _BMT(pSourceEvent));
#endif
		}
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
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
			array <Type^>^		lAttributeArgs = gcnew array <Type^> (1);
			array <Object^>^	lAttributeValues = gcnew array <Object^> (1);

			if	(lDispIdAttribute->Value == 30) //CharacterStyle
			{
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("--> Browse     [false] for [%s.%s]"), _BMT(lSourceProperty), _BM(lSourceProperty));
#endif
				lAttributeValues [0] = (System::Boolean)false;
				lAttributeArgs [0] = System::Boolean::typeid;
				pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::ComponentModel::BrowsableAttribute::typeid->GetConstructor(lAttributeArgs), lAttributeValues));
			}

			if	(
					(lDispIdAttribute->Value == 9) //Connected
				||	(lDispIdAttribute->Value == 30) //CharacterStyle
				)
			{
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("--> Editor     [System::ComponentModel::EditorBrowsableState::Advanced] for [%s.%s]"), _BMT(lSourceProperty), _BM(lSourceProperty));
#endif
				lAttributeValues [0] = System::ComponentModel::EditorBrowsableState::Advanced;
				lAttributeArgs [0] = System::ComponentModel::EditorBrowsableState::typeid;
				pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::ComponentModel::EditorBrowsableAttribute::typeid->GetConstructor(lAttributeArgs), lAttributeValues));
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
					lAttributeValues [0] = gcnew String("AutoSize");
				}
				else
				if	(lDispIdAttribute->Value == DISPID_BACKCOLOR)
				{
					lAttributeValues [0] = gcnew String("BackgroundColor");
				}
				else
				if	(lDispIdAttribute->Value == DISPID_BORDERCOLOR)
				{
					lAttributeValues [0] = gcnew String("BorderColor");
				}
				else
				if	(lDispIdAttribute->Value == DISPID_BORDERSTYLE)
				{
					lAttributeValues [0] = gcnew String("BorderStyle");
				}
				else
				if	(lDispIdAttribute->Value == DISPID_BORDERWIDTH)
				{
					lAttributeValues [0] = gcnew String("BorderWidth");
				}
				else
				if	(lDispIdAttribute->Value == DISPID_BORDERVISIBLE)
				{
					lAttributeValues [0] = gcnew String("BorderVisible");
				}
				else
				if	(lDispIdAttribute->Value == DISPID_MOUSEPOINTER)
				{
					lAttributeValues [0] = gcnew String("CursorStyle");
				}

				lAttributeArgs [0] = System::String::typeid;
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("--> Display    [%s] for [%s.%s]"), _B(lAttributeValues [0]->ToString()), _BMT(lSourceProperty), _BM(lSourceProperty));
#endif
				pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::ComponentModel::DisplayNameAttribute::typeid->GetConstructor(lAttributeArgs), lAttributeValues));
			}
#endif

#if	TRUE
			if	(lSourceProperty->PropertyType->IsImport)
			{
#if	TRUE
				lAttributeArgs [0] = System::Boolean::typeid;
				lAttributeValues [0] = gcnew System::Boolean (false);
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("--> Bindable   [%s] for [%s.%s]"), _B(lAttributeValues [0]->ToString()), _BMT(lSourceProperty), _BM(lSourceProperty));
#endif
				pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::ComponentModel::BindableAttribute::typeid->GetConstructor(lAttributeArgs), lAttributeValues));
#endif

#if	TRUE
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("--> Editor     [System::ComponentModel::EditorBrowsableState::Advanced] for [%s.%s]"), _BMT(lSourceProperty), _BM(lSourceProperty));
#endif
				lAttributeValues [0] = System::ComponentModel::EditorBrowsableState::Advanced;
				lAttributeArgs [0] = System::ComponentModel::EditorBrowsableState::typeid;
				pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::ComponentModel::EditorBrowsableAttribute::typeid->GetConstructor(lAttributeArgs), lAttributeValues));
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
			array <Type^>^		lAttributeArgs = gcnew array <Type^> (1);
			array <Object^>^	lAttributeValues = gcnew array <Object^> (1);

			switch (lDispIdAttribute->Value)
			{
				case DISPID_AUTOSIZE:			lAttributeValues [0] = gcnew String("Layout"); break;
				case DISPID_BACKCOLOR:			lAttributeValues [0] = gcnew String("Appearance"); break;
				case DISPID_BORDERCOLOR:		lAttributeValues [0] = gcnew String("Appearance"); break;
				case DISPID_BORDERSTYLE:		lAttributeValues [0] = gcnew String("Appearance"); break;
				case DISPID_BORDERWIDTH:		lAttributeValues [0] = gcnew String("Appearance"); break;
				case DISPID_BORDERVISIBLE:		lAttributeValues [0] = gcnew String("Appearance"); break;
				case DISPID_MOUSEPOINTER:		lAttributeValues [0] = gcnew String("Appearance"); break;
				case DISPID_MOUSEICON:			lAttributeValues [0] = gcnew String("Appearance"); break;
				case 41: /*AutoConnect*/		lAttributeValues [0] = gcnew String("Behavior"); break;
				case 9: /*Connected*/			lAttributeValues [0] = gcnew String("Behavior"); break;
				case 21: /*RaiseRequestErrors*/	lAttributeValues [0] = gcnew String("Behavior"); break;
			}

			if	(lAttributeValues [0])
			{
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("--> Category   [%s] for [%s] in [%s]"), _B((String^)lAttributeValues[0]), _BM(lSourceProperty), _BMT(lSourceProperty));
#endif
				lAttributeArgs [0] = String::typeid;
				pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::ComponentModel::CategoryAttribute::typeid->GetConstructor(lAttributeArgs), lAttributeValues));
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
		&&	(lSourceEvent->DeclaringType->IsSubclassOf (System::Windows::Forms::AxHost::typeid))
		&&	(lTargetEvent)
		)
	{
		try
		{
			array <Type^>^		lAttributeArgs = gcnew array <Type^> (1);
			array <Object^>^	lAttributeValues = gcnew array <Object^> (1);

			if	(
					(lSourceEvent->Name->StartsWith ("ActivateInput"))
				||	(lSourceEvent->Name->StartsWith ("DeactivateInput"))
				)
			{
				lAttributeValues [0] = gcnew String("Focus");
			}
			else
			if	(
					(lSourceEvent->Name->StartsWith ("Click"))
				||	(lSourceEvent->Name->StartsWith ("DblClick"))
				||	(lSourceEvent->Name->StartsWith ("Show"))
				||	(lSourceEvent->Name->StartsWith ("Hide"))
				||	(lSourceEvent->Name->StartsWith ("RequestStart"))
				||	(lSourceEvent->Name->StartsWith ("RequestComplete"))
				||	(lSourceEvent->Name->StartsWith ("Bookmark"))
				||	(lSourceEvent->Name->StartsWith ("Command"))
				||	(lSourceEvent->Name->StartsWith ("BalloonShow"))
				||	(lSourceEvent->Name->StartsWith ("BalloonHide"))
				||	(lSourceEvent->Name->StartsWith ("ListenStart"))
				||	(lSourceEvent->Name->StartsWith ("ListenComplete"))
				)
			{
				lAttributeValues [0] = gcnew String("Action");
			}
			else
			if	(
					(lSourceEvent->Name->StartsWith ("DragStart"))
				||	(lSourceEvent->Name->StartsWith ("DragComplete"))
				||	(lSourceEvent->Name->StartsWith ("Move"))
				||	(lSourceEvent->Name->StartsWith ("Size"))
				)
			{
				lAttributeValues [0] = gcnew String("Layout");
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
				lAttributeValues [0] = gcnew String("Behavior");
			}

			if	(lAttributeValues [0])
			{
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("--> Category   [%s] for [%s] in [%s]"), _B((String^)lAttributeValues[0]), _BM(lSourceEvent), _BMT(lSourceEvent));
#endif
				lAttributeArgs [0] = String::typeid;
				pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::ComponentModel::CategoryAttribute::typeid->GetConstructor(lAttributeArgs), lAttributeValues));
			}
		}
		catch AnyExceptionSilent
		{}
	}
}

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
