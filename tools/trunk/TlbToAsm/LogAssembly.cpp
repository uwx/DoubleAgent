#include "stdafx.h"
#include "LogAssembly.h"

using namespace System::Reflection;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

LogAssembly::LogAssembly ()
:	mLogLevel (LogNormal|LogNoPrefix),
	mLogSorted (false)
{}

LogAssembly::LogAssembly (UInt32 pLogLevel)
:	mLogLevel (pLogLevel),
	mLogSorted (false)
{}

/////////////////////////////////////////////////////////////////////////////

void LogAssembly::Log (Reflection::Assembly^ pAssembly, bool pLogCode)
{
	LogMessage (mLogLevel, _T(""));
	LogMessage (mLogLevel, _T("=== Assembly ==="));

	try
	{
		tBstrPtr		lTitle = _B ("Assembly");
		array<Type^>^	lTypes;

		if	(pAssembly == nullptr)
		{
			LogMessage (mLogLevel, _T("%s <nullptr>"), lTitle);
		}
		else
		{
			LogMessage (mLogLevel, _T("%s FullName [%s] RuntimeVersion [%s] ReflectionOnly [%d]"), _B(pAssembly->FullName->Split(',')[0]), _B(pAssembly->FullName), _B(pAssembly->ImageRuntimeVersion), (int)pAssembly->ReflectionOnly);
			try
			{
				LogMessage (mLogLevel, _T("%s CodeBase [%s]"), _B(pAssembly->FullName->Split(',')[0]), _B(pAssembly->CodeBase));
			}
			catch AnyExceptionSilent
			{}
			try
			{
				LogMessage (mLogLevel, _T("%s Location [%s]"), _B(pAssembly->FullName->Split(',')[0]), _B(pAssembly->Location));
			}
			catch AnyExceptionSilent
			{}

			if	(pLogCode)
			{
				mLogILBinary = gcnew LogILBinary (pAssembly->GetModules()[0], mLogLevel, 6);
			}
			LogReferences (pAssembly);
			LogAttributes (CustomAttributeData::GetCustomAttributes (pAssembly), 0);

			if	(lTypes = pAssembly->GetTypes())
			{
				Type^	lType;
				int		lNdx = 0;

				LogMessage (mLogLevel, _T("Types [%d]"), lTypes->Length);
					
				if	(mLogSorted)
				{
					Generic::SortedList <String^, Type^>^	lSorted = gcnew Generic::SortedList <String^, Type^>;

					lSorted->Capacity = lTypes->Length;
					for each (lType in lTypes)
					{
						lSorted->Add (lType->FullName, lType);
					}
					lTypes = gcnew array <Type^> (lSorted->Count);
					for each (lType in lSorted->Values)
					{
						lTypes [lNdx++] = lType;
					}
				}
				for	(lNdx = 0; lNdx < lTypes->Length; lNdx++)
				{
					lType = lTypes [lNdx];
					LogType (lType);
				}
			}
		}
	}
	catch AnyExceptionDebug
	{}

	LogMessage (mLogLevel, _T("=== Assembly End ==="));
}

/////////////////////////////////////////////////////////////////////////////

void LogAssembly::LogReferences (Reflection::Assembly^ pAssembly)
{
	try
	{
		array<AssemblyName^>^	lReferences = pAssembly->GetReferencedAssemblies ();

		if	(lReferences)
		{
			AssemblyName^	lReference;
			int				lNdx = 0;

			LogMessage (mLogLevel, _T("References [%d]"), lReferences->Length);

			if	(mLogSorted)
			{
				Generic::SortedList <String^, AssemblyName^>^	lSorted = gcnew Generic::SortedList <String^, AssemblyName^>;

				lSorted->Capacity = lReferences->Length;
				for each (lReference in lReferences)
				{
					lSorted->Add (lReference->ToString(), lReference);
				}			
				lReferences = gcnew array <AssemblyName^> (lSorted->Count);
				for each (lReference in lSorted->Values)
				{
					lReferences [lNdx++] = lReference;
				}
			}
			else
			{
				for	(lNdx = 0; lNdx < lReferences->Length; lNdx++)
				{
					lReference = lReferences [lNdx];
					LogMessage (mLogLevel, _T("    [%s]"), _B(lReference->ToString()));
				}
			}
		}
	}
	catch AnyExceptionDebug
	{}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void LogAssembly::LogType (System::Type^ pType, System::String^ pTitle)
{
	try
	{
		array<Type^>^				lInterfaces;
		array<FieldInfo^>^			lFields;
		array<PropertyInfo^>^		lProperties;
		array<MethodInfo^>^			lMethods;
		array<ConstructorInfo^>^	lConstructors;
		array<EventInfo^>^			lEvents;

		LogType (pType, 0, pTitle);

		try
		{
			TypeLibTypeAttribute^	lTypeLibTypeAttribute;

			lTypeLibTypeAttribute = safe_cast <TypeLibTypeAttribute^> (Attribute::GetCustomAttribute (pType, TypeLibTypeAttribute::typeid));
			if	(lTypeLibTypeAttribute)
			{
				LogMessage (mLogLevel, _T("    TypeLibTypeFlags [%8.8X] [%s]"), (Int32)lTypeLibTypeAttribute->Value, _B(TypeLibTypeFlagsStr(lTypeLibTypeAttribute->Value)));
			}
		}
		catch AnyExceptionSilent
		{}

		try
		{
			if	(
					(lInterfaces = pType->GetInterfaces ())
				&&	(lInterfaces->Length > 0)
				)
			{
				Type^	lInterface;
				int		lNdx = 0;

				LogMessage (mLogLevel, _T("    Interfaces [%d] for [%s]"), lInterfaces->Length, _BT(pType));

				if	(mLogSorted)
				{
					Generic::SortedList <String^, Type^>^	lSorted = gcnew Generic::SortedList <String^, Type^>;

					lSorted->Capacity = lInterfaces->Length;
					for each (lInterface in lInterfaces)
					{
						lSorted->Add (lInterface->FullName, lInterface);
					}
					lInterfaces = gcnew array <Type^> (lSorted->Count);
					for each (lInterface in lSorted->Values)
					{
						lInterfaces [lNdx++] = lInterface;
					}
				}
				for (lNdx = 0; lNdx < lInterfaces->Length; lNdx++)
				{
					lInterface = lInterfaces [lNdx];
					if	(IsInterfaceInherited (pType, lInterface))
					{
						LogType (lInterface, 4, "Inherited");
					}
					else
					if	(IsInterfaceImplemented (pType, lInterface))
					{
						LogType (lInterface, 4, "Implemented");
					}
					else
					{
						LogType (lInterface, 4, "Interface");
					}
				}
			}
		}
		catch AnyExceptionSilent
		{}

		try
		{
			if	(
					(lFields = pType->GetFields (BindingFlags::Instance|BindingFlags::Static|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
				&&	(lFields->Length > 0)
				)
			{
				FieldInfo^	lField;
				int			lNdx = 0;

				LogMessage (mLogLevel, _T("    Fields [%d] for [%s]"), lFields->Length, _BT(pType));

				if	(mLogSorted)
				{
					Generic::SortedList <String^, FieldInfo^>^	lSorted = gcnew Generic::SortedList <String^, FieldInfo^>;

					lSorted->Capacity = lFields->Length;
					for each (lField in lFields)
					{
						lSorted->Add (lField->Name, lField);
					}
					lFields = gcnew array <FieldInfo^> (lSorted->Count);
					for each (lField in lSorted->Values)
					{
						lFields [lNdx++] = lField;
					}
				}
				for	(lNdx = 0; lNdx < lFields->Length; lNdx++)
				{
					lField = lFields [lNdx];
					LogField (lField, lNdx, 4);
				}
			}
		}
		catch AnyExceptionSilent
		{}

		try
		{
			if	(
					(lProperties = pType->GetProperties (BindingFlags::Instance|BindingFlags::Static|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
				&&	(lProperties->Length > 0)
				)
			{
				PropertyInfo^	lProperty;
				int				lNdx = 0;

				LogMessage (mLogLevel, _T("    Properties [%d] for [%s]"), lProperties->Length, _BT(pType));

				if	(mLogSorted)
				{
					Generic::SortedList <String^, PropertyInfo^>^	lSorted = gcnew Generic::SortedList <String^, PropertyInfo^>;

					lSorted->Capacity = lProperties->Length;
					for each (lProperty in lProperties)
					{
						lSorted->Add (lProperty->Name, lProperty);
					}
					lProperties = gcnew array <PropertyInfo^> (lSorted->Count);
					for each (lProperty in lSorted->Values)
					{
						lProperties [lNdx++] = lProperty;
					}
				}
				for	(lNdx = 0; lNdx < lProperties->Length; lNdx++)
				{
					lProperty = lProperties [lNdx];
					LogProperty (lProperty, lNdx, 4);
				}
			}
		}
		catch AnyExceptionSilent
		{}

		try
		{
			if	(
					(lConstructors = pType->GetConstructors (BindingFlags::Instance|BindingFlags::Static|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
				&&	(lConstructors->Length > 0)
				)
			{
				ConstructorInfo^	lConstructor;
				int					lNdx = 0;

				LogMessage (mLogLevel, _T("    Constructors [%d] for [%s]"), lConstructors->Length, _BT(pType));

				if	(mLogSorted)
				{
					Generic::SortedList <String^, ConstructorInfo^>^	lSorted = gcnew Generic::SortedList <String^, ConstructorInfo^>;

					lSorted->Capacity = lConstructors->Length;
					for each (lConstructor in lConstructors)
					{
						lSorted->Add (lConstructor->Name, lConstructor);
					}
					lConstructors = gcnew array <ConstructorInfo^> (lSorted->Count);
					for each (lConstructor in lSorted->Values)
					{
						lConstructors [lNdx++] = lConstructor;
					}
				}
				for	(lNdx = 0; lNdx < lConstructors->Length; lNdx++)
				{
					lConstructor = lConstructors [lNdx];
					LogMethod (lConstructor, nullptr, lNdx, 4);
				}
			}
		}
		catch AnyExceptionSilent
		{}

		try
		{
			if	(
					(lMethods = pType->GetMethods (BindingFlags::Instance|BindingFlags::Static|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
				&&	(lMethods->Length > 0)
				)
			{
				MethodInfo^	lMethod;
				int			lNdx = 0;

				LogMessage (mLogLevel, _T("    Methods [%d] for [%s]"), lMethods->Length, _BT(pType));

				if	(mLogSorted)
				{
					Generic::SortedList <String^, MethodInfo^>^	lSorted = gcnew Generic::SortedList <String^, MethodInfo^>;

					lSorted->Capacity = lMethods->Length;
					for each (lMethod in lMethods)
					{
						lSorted->Add (lMethod->Name, lMethod);
					}
					lMethods = gcnew array <MethodInfo^> (lSorted->Count);
					for each (lMethod in lSorted->Values)
					{
						lMethods [lNdx++] = lMethod;
					}
				}
				for	(lNdx = 0; lNdx < lMethods->Length; lNdx++)
				{
					lMethod = lMethods [lNdx];
					LogMethod (lMethod, lMethod, lNdx, 4);
				}
			}
		}
		catch AnyExceptionSilent
		{}

		try
		{
			if	(
					(lEvents = pType->GetEvents (BindingFlags::Instance|BindingFlags::Static|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
				&&	(lEvents->Length > 0)
				)
			{
				EventInfo^	lEvent;
				int			lNdx = 0;

				LogMessage (mLogLevel, _T("    Events [%d] for [%s]"), lEvents->Length, _BT(pType));

				if	(mLogSorted)
				{
					Generic::SortedList <String^, EventInfo^>^	lSorted = gcnew Generic::SortedList <String^, EventInfo^>;

					lSorted->Capacity = lEvents->Length;
					for each (lEvent in lEvents)
					{
						lSorted->Add (lEvent->Name, lEvent);
					}
					lEvents = gcnew array <EventInfo^> (lSorted->Count);
					for each (lEvent in lSorted->Values)
					{
						lEvents [lNdx++] = lEvent;
					}
				}
				for	(lNdx = 0; lNdx < lEvents->Length; lNdx++)
				{
					lEvent = lEvents [lNdx];
					LogEvent (lEvent, lNdx, 4);
				}
			}
		}
		catch AnyExceptionSilent
		{}

		LogAttributes (CustomAttributeData::GetCustomAttributes (pType), 2);
	}
	catch AnyExceptionDebug
	{}
}

/////////////////////////////////////////////////////////////////////////////

void LogAssembly::LogType (Type^ pType, UInt32 pIndent, String^ pTitle)
{
	try
	{
		tBstrPtr	lTitle;
		tBstrPtr	lIndent = _B(gcnew String(' ', pIndent*2));
		String^		lFuncAttr = gcnew String("");

		if	(String::IsNullOrEmpty (pTitle))
		{
			lTitle = SysAllocString (L"Type");
		}
		else
		{
			lTitle = _B (pTitle);
		}

		if	(
				(pType)
			&&	(pIndent == 0)
			)
		{
			try
			{
				TypeLibFuncAttribute^	lFuncAttribute;

				lFuncAttribute = safe_cast <TypeLibFuncAttribute^> (Attribute::GetCustomAttribute (pType, TypeLibFuncAttribute::typeid));
				if	(lFuncAttribute)
				{
					lFuncAttr = (gcnew String(" Flags [")) + TypeLibFuncFlagsStr(lFuncAttribute->Value) + "]";
				}
			}
			catch AnyExceptionSilent
			{}
		}

		if	(pType == nullptr)
		{
			LogMessage (mLogLevel|LogHighVolume, _T("%s<nullptr> [%s]"), lIndent, lTitle);
		}
		else
		if	(pType->BaseType == nullptr)
		{
			LogMessage (mLogLevel|LogHighVolume, _T("%s%s [%s] [%s] [%s]%s"), lIndent, _B(pType->FullName), lTitle, _B(TypeProps(pType)), _B(TypeAttrsStr(pType->Attributes)), _B(lFuncAttr));
		}
		else
		{
			LogMessage (mLogLevel|LogHighVolume, _T("%s%s [%s] [%s] [%s] Base [%s]%s"), lIndent, _B(pType->FullName), lTitle, _B(TypeProps(pType)), _B(TypeAttrsStr(pType->Attributes)), _B(pType->BaseType->FullName), _B(lFuncAttr));
		}

		try
		{
			if	(pType->HasElementType)
			{
				LogType (pType->GetElementType(), pIndent+2, "Element");
			}
		}
		catch AnyExceptionSilent
	}
	catch AnyExceptionDebug
	{}
}

/////////////////////////////////////////////////////////////////////////////

void LogAssembly::LogField (Reflection::FieldInfo^ pField, System::Int32 pOrder, UInt32 pIndent)
{
	try
	{
		tBstrPtr	lTitle = _B ("Field");
		tBstrPtr	lIndent = _B (gcnew String(' ', pIndent*2));

		if	(pField == nullptr)
		{
			LogMessage (mLogLevel|LogHighVolume, _T("%s(%3.3d) <nullptr> [%s]"), lIndent, pOrder, lTitle);
		}
		else
		{
			try
			{
				Object^	lValue = pField->GetRawConstantValue();
				LogMessage (mLogLevel|LogHighVolume, _T("%s(%3.3d) %s (%s) [%s] [%s] [%s]"), lIndent, pOrder, _B(pField->Name), _B(lValue->ToString()), lTitle, _B(FieldProps(pField)), _B(FieldAttrsStr(pField->Attributes)));
			}
			catch (...)
			{
				LogMessage (mLogLevel|LogHighVolume, _T("%s%3d) %s [%s] [%s] [%s]"), lIndent, pOrder, _B(pField->Name), lTitle, _B(FieldProps(pField)), _B(FieldAttrsStr(pField->Attributes)));
			}
			if	(!pField->FieldType->IsEnum)
			{
				LogType (pField->FieldType, pIndent+2);
			}
		}
	}
	catch AnyExceptionDebug
	{}
}

/////////////////////////////////////////////////////////////////////////////

void LogAssembly::LogProperty (Reflection::PropertyInfo^ pProperty, System::Int32 pOrder, UInt32 pIndent)
{
	try
	{
		tBstrPtr	lTitle = _B ("Property");
		tBstrPtr	lIndent = _B (gcnew String(' ', pIndent*2));

		if	(pProperty == nullptr)
		{
			LogMessage (mLogLevel|LogHighVolume, _T("%s(%3.3d) <nullptr> [%s]"), lIndent, pOrder, lTitle);
		}
		else
		{
			LogMessage (mLogLevel|LogHighVolume, _T("%s(%3.3d) %s [%s] [%s] [%s]"), lIndent, pOrder, _B(pProperty->Name), lTitle, _B(PropertyProps(pProperty)), _B(PropertyAttrsStr(pProperty->Attributes)));
			LogType (pProperty->PropertyType, pIndent+2);
			LogAttributes (CustomAttributeData::GetCustomAttributes (pProperty), pIndent+2);
		}
	}
	catch AnyExceptionDebug
	{}
}

/////////////////////////////////////////////////////////////////////////////

void LogAssembly::LogMethod (System::Reflection::MethodBase^ pMethod, Reflection::MethodInfo^ pMethodInfo, System::Int32 pOrder, UInt32 pIndent)
{
	try
	{
		tBstrPtr	lTitle = _B ("Method");
		tBstrPtr	lIndent = _B (gcnew String(' ', pIndent*2));
		TCHAR		lOrder [20];

		if	(pOrder >= 0)
		{
			_stprintf (lOrder, _T("(%3.3d)"), pOrder);
		}
		else
		{
			_tcscpy (lOrder, _T("     "));
		}

		if	(pMethod == nullptr)
		{
			LogMessage (mLogLevel|LogHighVolume, _T("%s%s <nullptr> [%s]"), lIndent, lOrder, lTitle);
		}
		else
		{
			Boolean	lIsFunction = false;
			String^	lDispId = gcnew String("");
			String^	lFuncAttr = gcnew String("");

			try
			{
				if	(
						(pMethodInfo)
					&&	(pMethodInfo->ReturnType)
					&&	(String::Compare (pMethodInfo->ReturnType->FullName, gcnew String("System.Void"), true) != 0)
					)
				{
					lTitle = _B ("Function");
					lIsFunction = true;
				}
			}
			catch AnyExceptionDebug
			{}

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
					lFuncAttr = (gcnew String(" Flags [")) + TypeLibFuncFlagsStr(lFuncAttribute->Value) + "]";
				}
			}
			catch AnyExceptionDebug
			{}

			LogMessage (mLogLevel|LogHighVolume, _T("%s%s %s [%s] [%s] [%s] [%s] [%s]%s%s"), lIndent, lOrder, _B(pMethod->Name), lTitle, _B(MethodProps(pMethod)), _B(MethodAttrsStr(pMethod->Attributes)), _B(MethodCallType(pMethod->CallingConvention)), _B(MethodImplementation(pMethod->GetMethodImplementationFlags())), _B(lDispId), _B(lFuncAttr));

			if	(lIsFunction)
			{
				LogType (pMethodInfo->ReturnType, pIndent+2, "ReturnType");
			}

			try
			{
				array<ParameterInfo^>^	lParameters;
				int						lParameterNdx;

				if	(
						(lParameters = pMethod->GetParameters ())
					&&	(lParameters->Length > 0)
					)
				{
					for (lParameterNdx = 0; lParameterNdx < lParameters->Length; lParameterNdx++)
					{
						LogParameter (lParameters [lParameterNdx], lParameterNdx, pIndent+2);
					}
				}
			}
			catch AnyExceptionDebug
			{}

			if	(!pMethod->IsAbstract)
			{
				if	(mLogILBinary)
				{
					mLogILBinary->LogMethodBody (pMethod);
				}
				else
				{				
					try
					{
						MethodBody^				lMethodBody;
						array<unsigned char>^	lBody;

						if	(
								(lMethodBody = pMethod->GetMethodBody ())
							&&	(lBody = lMethodBody->GetILAsByteArray ())
							)
						{
							LogMessage (mLogLevel|LogHighVolume, _T("%s    Body [%d]"), lIndent, lBody->Length);
						}
						else
						{
							//LogMessage (mLogLevel|LogHighVolume, _T("%s    No Body"), lIndent);
						}
					}
					catch AnyExceptionDebug
					{}
				}
			}

			LogAttributes (CustomAttributeData::GetCustomAttributes (pMethod), pIndent+2);
		}
	}
	catch AnyExceptionDebug
	{}
}

/////////////////////////////////////////////////////////////////////////////

void LogAssembly::LogEvent (Reflection::EventInfo^ pEvent, System::Int32 pOrder, UInt32 pIndent)
{
	try
	{
		tBstrPtr	lTitle = _B ("Event");
		tBstrPtr	lIndent = _B (gcnew String(' ', pIndent*2));

		if	(pEvent == nullptr)
		{
			LogMessage (mLogLevel|LogHighVolume, _T("%s(%3.3d) <nullptr> [%s]"), lIndent, pOrder, lTitle);
		}
		else
		{
			LogMessage (mLogLevel|LogHighVolume, _T("%s(%3.3d) %s [%s] [%s] [%s]"), lIndent, pOrder, _B(pEvent->Name), lTitle, _B(EventProps(pEvent)), _B(EventAttrsStr(pEvent->Attributes)));

			try
			{
				array<MethodInfo^>^	lMethods;
				MethodInfo^			lMethod;

				if	(
						(lMethod = pEvent->GetAddMethod ())
					&&	(Object::ReferenceEquals (lMethod->DeclaringType, pEvent->DeclaringType))
					)
				{
					LogMethod (lMethod, lMethod, -1, pIndent+2);
				}
				if	(
						(lMethod = pEvent->GetRemoveMethod ())
					&&	(Object::ReferenceEquals (lMethod->DeclaringType, pEvent->DeclaringType))
					)
				{
					LogMethod (lMethod, lMethod, -1, pIndent+2);
				}
				if	(
						(lMethod = pEvent->GetRaiseMethod ())
					&&	(Object::ReferenceEquals (lMethod->DeclaringType, pEvent->DeclaringType))
					)
				{
					LogMethod (lMethod, lMethod, -1, pIndent+2);
				}

				if	(
						(lMethods = pEvent->GetOtherMethods())
					&&	(lMethods->Length > 0)
					)
				{
					for each (lMethod in lMethods)
					{
						if	(Object::ReferenceEquals (lMethod->DeclaringType, pEvent->DeclaringType))
						{
							LogMethod (lMethod, lMethod, -1, pIndent+2);
						}
					}
				}
			}
			catch AnyExceptionDebug

			LogAttributes (CustomAttributeData::GetCustomAttributes (pEvent), pIndent+2);
		}
	}
	catch AnyExceptionDebug
	{}
}

/////////////////////////////////////////////////////////////////////////////

void LogAssembly::LogParameter (Reflection::ParameterInfo^ pParameter, System::Int32 pOrder, UInt32 pIndent)
{
	try
	{
		tBstrPtr		lTitle = _B ("Parameter");
		tBstrPtr		lIndent = _B (gcnew String(' ', pIndent*2));
		array <Type^>^	lModifiers;
		Type^			lModifier;

		if	(pParameter == nullptr)
		{
			LogMessage (mLogLevel|LogHighVolume, _T("%s(%3.3d) <nullptr> [%s]"), lIndent, pOrder, lTitle);
		}
		else
		{
			if	(String::IsNullOrEmpty (pParameter->Name))
			{
				LogMessage (mLogLevel|LogHighVolume, _T("%s(%3.3d) <unnamed> [%s] [%s] [%s]"), lIndent, pOrder, lTitle, _B(ParameterProps(pParameter)), _B(ParameterAttrsStr(pParameter->Attributes)));
			}
			else
			{
				LogMessage (mLogLevel|LogHighVolume, _T("%s(%3.3d) %s [%s] [%s] [%s]"), lIndent, pOrder, _B(pParameter->Name), lTitle, _B(ParameterProps(pParameter)), _B(ParameterAttrsStr(pParameter->Attributes)));
			}
			
			LogType (pParameter->ParameterType, pIndent+2);
			
			if	(lModifiers = pParameter->GetRequiredCustomModifiers ())
			{
				for each (lModifier in lModifiers)
				{
					LogMessage (mLogLevel|LogHighVolume, _T("%s    Required [%s]"), lIndent, _BT(lModifier));
				}
			}
			if	(lModifiers = pParameter->GetOptionalCustomModifiers ())
			{
				for each (lModifier in lModifiers)
				{
					LogMessage (mLogLevel|LogHighVolume, _T("%s    Optional [%s]"), lIndent, _BT(lModifier));
				}
			}
		}
	}
	catch AnyExceptionDebug
	{}
}

/////////////////////////////////////////////////////////////////////////////

void LogAssembly::LogAttributes (System::Collections::Generic::IList <System::Reflection::CustomAttributeData^>^ pAttributes, System::UInt32 pIndent)
{
	try
	{
		tBstrPtr	lIndent = _B (gcnew String(' ', pIndent*2));

		if	(
				(pAttributes)
			&&	(pAttributes->Count > 0)
			)
		{
			CustomAttributeData^			lAttributeData;
			array <CustomAttributeData^>^	lAttributes;
			int								lNdx = 0;

			LogMessage (mLogLevel|LogHighVolume, _T("%sAttributes [%d]"), lIndent, pAttributes->Count);
			
			if	(mLogSorted)
			{
				Generic::SortedList <String^, CustomAttributeData^>^	lSorted = gcnew Generic::SortedList <String^, CustomAttributeData^>;

				lSorted->Capacity = pAttributes->Count;
				for each (lAttributeData in pAttributes)
				{
					lSorted->Add (lAttributeData->ToString(), lAttributeData);
				}
				lAttributes = gcnew array <CustomAttributeData^> (lSorted->Count);
				for each (lAttributeData in lSorted->Values)
				{
					lAttributes [lNdx++] = lAttributeData;
				}
			}
			else
			{
				lAttributes = gcnew array <CustomAttributeData^> (pAttributes->Count);
				for	(lNdx = 0; lNdx < pAttributes->Count; lNdx++)
				{
					lAttributes [lNdx] = pAttributes [lNdx];
				}
			}
			for	(lNdx = 0; lNdx < lAttributes->Length; lNdx++)
			{
				lAttributeData = lAttributes [lNdx];
				try
				{
					int	lArgCountC = 0;
					int	lArgCountN = 0;

					if	(lAttributeData->ConstructorArguments)
					{
						lArgCountC = lAttributeData->ConstructorArguments->Count;
					}
					if	(lAttributeData->NamedArguments)
					{
						lArgCountN = lAttributeData->NamedArguments->Count;
					}

					LogMessage (mLogLevel|LogHighVolume, _T("%s    %s [%d %d]"), lIndent, _B(lAttributeData->ToString()), lArgCountC, lArgCountN);
				}
				catch AnyExceptionDebug
			}
		}
	}
	catch AnyExceptionDebug
	{}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool LogAssembly::IsInterfaceImplemented (Type^ pType, Type^ pInterface)
{
	bool				lRet = false;
	InterfaceMapping^	lMapping;

	try
	{
		lMapping = pType->GetInterfaceMap (pInterface);
	}
	catch AnyExceptionSilent

	if	(lMapping)
	{
		lRet = true;
	}
	return lRet;
}

bool LogAssembly::IsInterfaceInherited (Type^ pType, Type^ pInterface)
{
	bool	lRet = false;

	try
	{
		try
		{
			if	(pType->BaseType->GetInterface (pInterface->FullName))
			{
				lRet = true;
			}
		}
		catch AnyExceptionSilent

		if	(!lRet)
		{
			array<Type^>^	lInterfaces = pType->GetInterfaces ();
			Type^			lInterface;

			for	each (lInterface in lInterfaces)
			{
				if	(!Object::ReferenceEquals (lInterface, pInterface))
				{
					try
					{
						if	(lInterface->GetInterface (pInterface->FullName))
						{
							lRet = true;
						}
					}
					catch AnyExceptionSilent
				}
				if	(lRet)
				{
					break;
				}
			}
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

String^ LogAssembly::TypeProps (Type^ pType)
{
	StringBuilder	lProps;

	try
	{
		if	(pType->IsEnum)
		{
			lProps.Append ("Enum ");
		}
	}
	catch AnyExceptionSilent
	{}
	try
	{
		if	(pType->IsClass)
		{
			lProps.Append ("Class ");
		}
	}
	catch AnyExceptionSilent
	{}
	try
	{
		if	(pType->IsInterface)
		{
			lProps.Append ("Interface ");
		}
	}
	catch AnyExceptionSilent
	{}
	try
	{
		if	(pType->IsAbstract)
		{
			lProps.Append ("Abstract ");
		}
	}
	catch AnyExceptionSilent
	{}
	try
	{
		if	(pType->IsValueType)
		{
			lProps.Append ("Value ");
		}
	}
	catch AnyExceptionSilent
	{}
	try
	{
		if	(pType->IsArray)
		{
			lProps.Append ("Array ");
		}
	}
	catch AnyExceptionSilent
	{}
	try
	{
		if	(pType->IsPrimitive)
		{
			lProps.Append ("Primitive ");
		}
	}
	catch AnyExceptionSilent
	{}
	try
	{
		if	(pType->IsPointer)
		{
			lProps.Append ("Pointer ");
		}
		else
		if	(pType->HasElementType)
		{
			lProps.Append ("Reference ");
		}
	}
	catch AnyExceptionSilent
	{}
	try
	{
		if	(pType->IsCOMObject)
		{
			lProps.Append ("COMObject ");
		}
	}
	catch AnyExceptionSilent
	{}
	try
	{
		if	(
				(pType->IsAutoLayout)
			&&	(!pType->IsValueType)
			&&	(!pType->HasElementType)
			)
		{
			lProps.Append ("AutoLayout ");
		}
	}
	catch AnyExceptionSilent
	{}
	try
	{
		if	(pType->IsExplicitLayout)
		{
			lProps.Append ("ExplicitLayout ");
		}
	}
	catch AnyExceptionSilent
	{}
	try
	{
		if	(
				(pType->IsLayoutSequential)
			&&	(!pType->IsPrimitive)
			)
		{
			lProps.Append ("SequentialLayout ");
		}
	}
	catch AnyExceptionSilent
	{}
	try
	{
		if	(pType->IsAutoClass)
		{
			lProps.Append ("AutoClass ");
		}
	}
	catch AnyExceptionSilent
	{}
	try
	{
		if	(pType->IsAnsiClass)
		{
			lProps.Append ("Ansi ");
		}
	}
	catch AnyExceptionSilent
	{}
	try
	{
		if	(pType->IsUnicodeClass)
		{
			lProps.Append ("Unicode ");
		}
	}
	catch AnyExceptionSilent
	{}
	try
	{
		if	(pType->IsVisible)
		{
			lProps.Append ("Visible ");
		}
	}
	catch AnyExceptionSilent
	{}

	return lProps.ToString()->Trim();
}

String^ LogAssembly::TypeAttrsStr (Reflection::TypeAttributes pTypeAttrs)
{
	StringBuilder	lAttrs;

	//lAttrs.Append (String::Format ("{0:X} ", (int)pTypeAttrs));

	if	(((int)pTypeAttrs & (int)TypeAttributes::ClassSemanticsMask) == (int)TypeAttributes::Class)
	{
		lAttrs.Append ("Class ");
	}
	if	(((int)pTypeAttrs & (int)TypeAttributes::ClassSemanticsMask) == (int)TypeAttributes::Interface)
	{
		lAttrs.Append ("Interface ");
	}

	if	((int)pTypeAttrs & (int)TypeAttributes::Abstract)
	{
		lAttrs.Append ("Abstract ");
	}
	if	((int)pTypeAttrs & (int)TypeAttributes::Sealed)
	{
		lAttrs.Append ("Sealed ");
	}

	if	(((int)pTypeAttrs & (int)TypeAttributes::VisibilityMask) == (int)TypeAttributes::NestedAssembly)
	{
		lAttrs.Append ("NestedAssembly ");
	}
	if	(((int)pTypeAttrs & (int)TypeAttributes::VisibilityMask) == (int)TypeAttributes::NestedFamANDAssem)
	{
		lAttrs.Append ("NestedFamANDAssem ");
	}
	if	(((int)pTypeAttrs & (int)TypeAttributes::VisibilityMask) == (int)TypeAttributes::NestedFamily)
	{
		lAttrs.Append ("NestedFamily ");
	}
	if	(((int)pTypeAttrs & (int)TypeAttributes::VisibilityMask) == (int)TypeAttributes::NestedFamORAssem)
	{
		lAttrs.Append ("NestedFamORAssem ");
	}
	if	(((int)pTypeAttrs & (int)TypeAttributes::VisibilityMask) == (int)TypeAttributes::NestedPrivate)
	{
		lAttrs.Append ("NestedPrivate ");
	}
	if	(((int)pTypeAttrs & (int)TypeAttributes::VisibilityMask) == (int)TypeAttributes::NestedPublic)
	{
		lAttrs.Append ("NestedPublic ");
	}
	if	(((int)pTypeAttrs & (int)TypeAttributes::VisibilityMask) == (int)TypeAttributes::NotPublic)
	{
		lAttrs.Append ("NotPublic ");
	}
	if	(((int)pTypeAttrs & (int)TypeAttributes::VisibilityMask) == (int)TypeAttributes::Public)
	{
		lAttrs.Append ("Public ");
	}

	if	(((int)pTypeAttrs & (int)TypeAttributes::LayoutMask) == (int)TypeAttributes::AutoLayout)
	{
		lAttrs.Append ("AutoLayout ");
	}
	if	(((int)pTypeAttrs & (int)TypeAttributes::LayoutMask) == (int)TypeAttributes::SequentialLayout)
	{
		lAttrs.Append ("SequentialLayout ");
	}
	if	(((int)pTypeAttrs & (int)TypeAttributes::LayoutMask) == (int)TypeAttributes::ExplicitLayout)
	{
		lAttrs.Append ("ExplicitLayout ");
	}

	if	(((int)pTypeAttrs & (int)TypeAttributes::StringFormatMask) == (int)TypeAttributes::AutoClass)
	{
		lAttrs.Append ("AutoClass ");
	}
	if	(((int)pTypeAttrs & (int)TypeAttributes::StringFormatMask) == (int)TypeAttributes::AnsiClass)
	{
		lAttrs.Append ("AnsiClass ");
	}
	if	(((int)pTypeAttrs & (int)TypeAttributes::StringFormatMask) == (int)TypeAttributes::UnicodeClass)
	{
		lAttrs.Append ("UnicodeClass ");
	}

	if	((int)pTypeAttrs & (int)TypeAttributes::Import)
	{
		lAttrs.Append ("Import ");
	}
	if	((int)pTypeAttrs & (int)TypeAttributes::HasSecurity)
	{
		lAttrs.Append ("HasSecurity ");
	}
	if	((int)pTypeAttrs & (int)TypeAttributes::Serializable)
	{
		lAttrs.Append ("Serializable ");
	}
	if	((int)pTypeAttrs & (int)TypeAttributes::BeforeFieldInit)
	{
		lAttrs.Append ("BeforeFieldInit ");
	}

	if	((int)pTypeAttrs & (int)TypeAttributes::SpecialName)
	{
		lAttrs.Append ("SpecialName ");
	}
	if	((int)pTypeAttrs & (int)TypeAttributes::RTSpecialName)
	{
		lAttrs.Append ("RTSpecialName ");
	}

	if	((int)pTypeAttrs & ((int)TypeAttributes::ReservedMask & ~(int)TypeAttributes::HasSecurity))
	{
		lAttrs.Append ("Reserved ");
		lAttrs.Append (String::Format ("{0:X} ", ((int)pTypeAttrs & (int)TypeAttributes::ReservedMask)));
	}

	return lAttrs.ToString()->Trim();
}

/////////////////////////////////////////////////////////////////////////////

String^ LogAssembly::FieldProps (Reflection::FieldInfo^ pField)
{
	StringBuilder	lProps;

	if	(pField->IsPublic)
	{
		lProps.Append ("Public ");
	}
	if	(pField->IsPrivate)
	{
		lProps.Append ("Private ");
	}
	if	(pField->IsAssembly)
	{
		lProps.Append ("Assembly ");
	}
	if	(pField->IsFamily)
	{
		lProps.Append ("Family ");
	}
	if	(pField->IsStatic)
	{
		lProps.Append ("Static ");
	}
	if	(pField->IsInitOnly)
	{
		lProps.Append ("InitOnly ");
	}
	if	(pField->IsLiteral)
	{
		lProps.Append ("Literal ");
	}
	if	(pField->IsSpecialName)
	{
		lProps.Append ("SpecialName ");
	}

	return lProps.ToString()->Trim();
}

String^ LogAssembly::FieldAttrsStr (Reflection::FieldAttributes pFieldAttrs)
{
	StringBuilder	lAttrs;

	//lAttrs.Append (String::Format ("{0:X} ", (int)pFieldAttrs));

	if	(((int)pFieldAttrs & (int)FieldAttributes::FieldAccessMask) == (int)FieldAttributes::PrivateScope)
	{
		lAttrs.Append ("PrivateScope ");
	}
	if	(((int)pFieldAttrs & (int)FieldAttributes::FieldAccessMask) == (int)FieldAttributes::Private)
	{
		lAttrs.Append ("Private ");
	}
	if	(((int)pFieldAttrs & (int)FieldAttributes::FieldAccessMask) == (int)FieldAttributes::Public)
	{
		lAttrs.Append ("Public ");
	}
	if	(((int)pFieldAttrs & (int)FieldAttributes::FieldAccessMask) == (int)FieldAttributes::Assembly)
	{
		lAttrs.Append ("Assembly ");
	}
	if	(((int)pFieldAttrs & (int)FieldAttributes::FieldAccessMask) == (int)FieldAttributes::FamANDAssem)
	{
		lAttrs.Append ("FamANDAssem ");
	}
	if	(((int)pFieldAttrs & (int)FieldAttributes::FieldAccessMask) == (int)FieldAttributes::Family)
	{
		lAttrs.Append ("Family ");
	}
	if	(((int)pFieldAttrs & (int)FieldAttributes::FieldAccessMask) == (int)FieldAttributes::FamORAssem)
	{
		lAttrs.Append ("FamORAssem ");
	}

	if	((int)pFieldAttrs & (int)FieldAttributes::Static)
	{
		lAttrs.Append ("Static ");
	}
	if	((int)pFieldAttrs & (int)FieldAttributes::InitOnly)
	{
		lAttrs.Append ("InitOnly ");
	}
	if	((int)pFieldAttrs & (int)FieldAttributes::Literal)
	{
		lAttrs.Append ("Literal ");
	}
	if	((int)pFieldAttrs & (int)FieldAttributes::HasDefault)
	{
		lAttrs.Append ("HasDefault ");
	}
	if	((int)pFieldAttrs & (int)FieldAttributes::NotSerialized)
	{
		lAttrs.Append ("NotSerialized ");
	}
	if	((int)pFieldAttrs & (int)FieldAttributes::HasFieldMarshal)
	{
		lAttrs.Append ("HasFieldMarshal ");
	}
	if	((int)pFieldAttrs & (int)FieldAttributes::HasFieldRVA)
	{
		lAttrs.Append ("HasFieldRVA ");
	}
	if	((int)pFieldAttrs & (int)FieldAttributes::PinvokeImpl)
	{
		lAttrs.Append ("NotSerialized ");
	}

	if	((int)pFieldAttrs & (int)FieldAttributes::SpecialName)
	{
		lAttrs.Append ("SpecialName ");
	}
	if	((int)pFieldAttrs & (int)FieldAttributes::RTSpecialName)
	{
		lAttrs.Append ("RTSpecialName ");
	}

	if	((int)pFieldAttrs & ((int)FieldAttributes::ReservedMask & ~(int)FieldAttributes::HasDefault) & ~(int)FieldAttributes::RTSpecialName)
	{
		lAttrs.Append ("Reserved ");
		lAttrs.Append (String::Format ("{0:X} ", ((int)pFieldAttrs & (int)FieldAttributes::ReservedMask)));
	}

	return lAttrs.ToString()->Trim();
}

/////////////////////////////////////////////////////////////////////////////

String^ LogAssembly::PropertyProps (Reflection::PropertyInfo^ pProperty)
{
	StringBuilder	lProps;

	if	(pProperty->CanRead)
	{
		lProps.Append ("Read ");
	}
	if	(pProperty->CanWrite)
	{
		lProps.Append ("Write ");
	}
	if	(pProperty->IsSpecialName)
	{
		lProps.Append ("SpecialName ");
	}

	return lProps.ToString()->Trim();
}

String^ LogAssembly::PropertyAttrsStr (Reflection::PropertyAttributes pPropertyAttrs)
{
	StringBuilder	lAttrs;

	//lAttrs.Append (String::Format ("{0:X} ", (int)pPropertyAttrs));

	if	(pPropertyAttrs == Reflection::PropertyAttributes::None)
	{
		lAttrs.Append ("None");
	}
	else
	{
		if	((int)pPropertyAttrs & (int)Reflection::PropertyAttributes::HasDefault)
		{
			lAttrs.Append ("HasDefault ");
		}

		if	((int)pPropertyAttrs & (int)Reflection::PropertyAttributes::SpecialName)
		{
			lAttrs.Append ("SpecialName ");
		}
		if	((int)pPropertyAttrs & (int)Reflection::PropertyAttributes::RTSpecialName)
		{
			lAttrs.Append ("RTSpecialName ");
		}

		if	((int)pPropertyAttrs & (int)Reflection::PropertyAttributes::ReservedMask)
		{
			lAttrs.Append ("Reserved ");
			lAttrs.Append (String::Format ("{0:X} ", ((int)pPropertyAttrs & (int)Reflection::PropertyAttributes::ReservedMask)));
		}
	}
	return lAttrs.ToString()->Trim();
}

/////////////////////////////////////////////////////////////////////////////

String^ LogAssembly::MethodProps (Reflection::MethodBase^ pMethod)
{
	StringBuilder	lProps;

	if	(pMethod->IsPublic)
	{
		lProps.Append ("Public ");
	}
	if	(pMethod->IsPrivate)
	{
		lProps.Append ("Private ");
	}
	if	(pMethod->IsAssembly)
	{
		lProps.Append ("Assembly ");
	}
	if	(pMethod->IsFamily)
	{
		lProps.Append ("Family ");
	}
	if	(pMethod->IsStatic)
	{
		lProps.Append ("Static ");
	}
	if	(pMethod->IsVirtual)
	{
		lProps.Append ("Virtual ");
	}
	if	(pMethod->IsConstructor)
	{
		lProps.Append ("Constructor ");
	}
	if	(pMethod->IsHideBySig)
	{
		lProps.Append ("HideBySig ");
	}
	if	(pMethod->IsFinal)
	{
		lProps.Append ("Final ");
	}
	if	(pMethod->IsSpecialName)
	{
		lProps.Append ("SpecialName ");
	}

	return lProps.ToString()->Trim();
}

String^ LogAssembly::MethodAttrsStr (Reflection::MethodAttributes pMethodAttrs)
{
	StringBuilder	lAttrs;

	//lAttrs.Append (String::Format ("{0:X} ", (int)pMethodAttrs));

	if	(((int)pMethodAttrs & (int)MethodAttributes::MemberAccessMask) == (int)MethodAttributes::Assembly)
	{
		lAttrs.Append ("Assembly ");
	}
	if	(((int)pMethodAttrs & (int)MethodAttributes::MemberAccessMask) == (int)MethodAttributes::FamANDAssem)
	{
		lAttrs.Append ("FamANDAssem ");
	}
	if	(((int)pMethodAttrs & (int)MethodAttributes::MemberAccessMask) == (int)MethodAttributes::Family)
	{
		lAttrs.Append ("Family ");
	}
	if	(((int)pMethodAttrs & (int)MethodAttributes::MemberAccessMask) == (int)MethodAttributes::FamORAssem)
	{
		lAttrs.Append ("FamORAssem ");
	}
	if	(((int)pMethodAttrs & (int)MethodAttributes::MemberAccessMask) == (int)MethodAttributes::Private)
	{
		lAttrs.Append ("Private ");
	}
	if	(((int)pMethodAttrs & (int)MethodAttributes::MemberAccessMask) == (int)MethodAttributes::Public)
	{
		lAttrs.Append ("Public ");
	}

	if	(((int)pMethodAttrs & (int)MethodAttributes::VtableLayoutMask) == (int)MethodAttributes::ReuseSlot)
	{
		lAttrs.Append ("ReuseSlot ");
	}
	if	(((int)pMethodAttrs & (int)MethodAttributes::VtableLayoutMask) == (int)MethodAttributes::NewSlot)
	{
		lAttrs.Append ("NewSlot ");
	}

	if	((int)pMethodAttrs & (int)MethodAttributes::Virtual)
	{
		lAttrs.Append ("Virtual ");
	}
	if	((int)pMethodAttrs & (int)MethodAttributes::Abstract)
	{
		lAttrs.Append ("Abstract ");
	}
	if	((int)pMethodAttrs & (int)MethodAttributes::Final)
	{
		lAttrs.Append ("Final ");
	}
	if	((int)pMethodAttrs & (int)MethodAttributes::CheckAccessOnOverride)
	{
		lAttrs.Append ("CheckAccessOnOverride ");
	}
	if	((int)pMethodAttrs & (int)MethodAttributes::HideBySig)
	{
		lAttrs.Append ("HideBySig ");
	}
	if	((int)pMethodAttrs & (int)MethodAttributes::HasSecurity)
	{
		lAttrs.Append ("HasSecurity ");
	}
	if	((int)pMethodAttrs & (int)MethodAttributes::PinvokeImpl)
	{
		lAttrs.Append ("PinvokeImpl ");
	}

	if	((int)pMethodAttrs & (int)MethodAttributes::SpecialName)
	{
		lAttrs.Append ("SpecialName ");
	}
	if	((int)pMethodAttrs & (int)MethodAttributes::RTSpecialName)
	{
		lAttrs.Append ("RTSpecialName ");
	}

	if	((int)pMethodAttrs & ((int)MethodAttributes::ReservedMask & ~(int)MethodAttributes::RTSpecialName))
	{
		lAttrs.Append ("Reserved ");
		lAttrs.Append (String::Format ("{0:X} ", ((int)pMethodAttrs & (int)MethodAttributes::ReservedMask)));
	}

	return lAttrs.ToString()->Trim();
}

String^ LogAssembly::MethodCallType (Reflection::CallingConventions pCallType)
{
	StringBuilder	lString;

	if	((int)pCallType & (int)CallingConventions::Standard)
	{
		lString.Append ("Standard ");
	}
	if	((int)pCallType & (int)CallingConventions::VarArgs)
	{
		lString.Append ("VarArgs ");
	}
	if	((int)pCallType & (int)CallingConventions::HasThis)
	{
		lString.Append ("HasThis ");
	}
	if	((int)pCallType & (int)CallingConventions::ExplicitThis)
	{
		lString.Append ("ExplicitThis ");
	}

	return lString.ToString()->Trim();
}

String^ LogAssembly::MethodImplementation (Reflection::MethodImplAttributes pAttrs)
{
	StringBuilder	lString;

	if	(((int)pAttrs & (int)MethodImplAttributes::CodeTypeMask) == (int)MethodImplAttributes::IL)
	{
		lString.Append ("IL ");
	}
	if	(((int)pAttrs & (int)MethodImplAttributes::CodeTypeMask) == (int)MethodImplAttributes::OPTIL)
	{
		lString.Append ("OPTIL ");
	}
	if	(((int)pAttrs & (int)MethodImplAttributes::CodeTypeMask) == (int)MethodImplAttributes::Native)
	{
		lString.Append ("Native ");
	}
	if	(((int)pAttrs & (int)MethodImplAttributes::CodeTypeMask) == (int)MethodImplAttributes::Runtime)
	{
		lString.Append ("Runtime ");
	}

	if	(((int)pAttrs & (int)MethodImplAttributes::ManagedMask) == (int)MethodImplAttributes::Managed)
	{
		lString.Append ("Managed ");
	}
	if	(((int)pAttrs & (int)MethodImplAttributes::ManagedMask) == (int)MethodImplAttributes::Unmanaged)
	{
		lString.Append ("Unmanaged ");
	}

	if	((int)pAttrs & (int)MethodImplAttributes::Synchronized)
	{
		lString.Append ("Synchronized ");
	}
	if	((int)pAttrs & (int)MethodImplAttributes::PreserveSig)
	{
		lString.Append ("PreserveSig ");
	}
	if	((int)pAttrs & (int)MethodImplAttributes::InternalCall)
	{
		lString.Append ("InternalCall ");
	}
	if	((int)pAttrs & (int)MethodImplAttributes::ForwardRef)
	{
		lString.Append ("ForwardRef ");
	}
	if	((int)pAttrs & (int)MethodImplAttributes::NoInlining)
	{
		lString.Append ("NoInlining ");
	}
	if	((int)pAttrs & (int)MethodImplAttributes::NoOptimization)
	{
		lString.Append ("NoOptimization ");
	}

	return lString.ToString()->Trim();
}

/////////////////////////////////////////////////////////////////////////////

String^ LogAssembly::EventProps (Reflection::EventInfo^ pEvent)
{
	StringBuilder	lProps;

	if	(pEvent->IsMulticast)
	{
		lProps.Append ("Multicast ");
	}
	if	(pEvent->IsSpecialName)
	{
		lProps.Append ("SpecialName ");
	}

	return lProps.ToString()->Trim();
}

String^ LogAssembly::EventAttrsStr (Reflection::EventAttributes pEventAttrs)
{
	StringBuilder	lAttrs;

	//Attrs.Append (String::Format ("{0:X} ", (int)pEventAttrs));

	if	(pEventAttrs == EventAttributes::None)
	{
		lAttrs.Append ("None");
	}
	else
	{	
		if	((int)pEventAttrs & (int)EventAttributes::SpecialName)
		{
			lAttrs.Append ("SpecialName ");
		}
		if	((int)pEventAttrs & (int)EventAttributes::RTSpecialName)
		{
			lAttrs.Append ("RTSpecialName ");
		}

		if	((int)pEventAttrs & (int)EventAttributes::ReservedMask)
		{
			lAttrs.Append ("Reserved ");
			lAttrs.Append (String::Format ("{0:X} ", ((int)pEventAttrs & (int)EventAttributes::ReservedMask)));
		}
	}
	return lAttrs.ToString()->Trim();
}

/////////////////////////////////////////////////////////////////////////////

String^ LogAssembly::ParameterProps (Reflection::ParameterInfo^ pParameter)
{
	StringBuilder	lProps;

	if	(pParameter->IsIn)
	{
		lProps.Append ("In ");
	}
	if	(pParameter->IsOut)
	{
		lProps.Append ("Out ");
	}
	if	(pParameter->IsRetval)
	{
		lProps.Append ("Retval ");
	}
	if	(pParameter->IsOptional)
	{
		lProps.Append ("Optional ");
	}
	if	(
			(pParameter->DefaultValue)
		&&	(!String::IsNullOrEmpty (pParameter->DefaultValue->ToString()))
		)
	{
		lProps.Append ("Default(");
		lProps.Append (pParameter->DefaultValue->ToString());
		lProps.Append (") (");
		lProps.Append (pParameter->DefaultValue->GetType()->FullName);
		lProps.Append (") ");
	}

	return lProps.ToString()->Trim();
}

String^ LogAssembly::ParameterAttrsStr (Reflection::ParameterAttributes pParameterAttrs)
{
	StringBuilder	lAttrs;

	//lAttrs.Append (String::Format ("{0:X} ", (int)pParameterAttrs));

	if	(pParameterAttrs == ParameterAttributes::None)
	{
		lAttrs.Append ("None");
	}
	else
	{
		if	((int)pParameterAttrs & (int)ParameterAttributes::In)
		{
			lAttrs.Append ("In ");
		}
		if	((int)pParameterAttrs & (int)ParameterAttributes::Out)
		{
			lAttrs.Append ("Out ");
		}
		if	((int)pParameterAttrs & (int)ParameterAttributes::Retval)
		{
			lAttrs.Append ("Retval ");
		}
		if	((int)pParameterAttrs & (int)ParameterAttributes::Optional)
		{
			lAttrs.Append ("Optional ");
		}
		if	((int)pParameterAttrs & (int)ParameterAttributes::Lcid)
		{
			lAttrs.Append ("Lcid ");
		}

		if	((int)pParameterAttrs & (int)ParameterAttributes::HasDefault)
		{
			lAttrs.Append ("HasDefault ");
		}
		if	((int)pParameterAttrs & (int)ParameterAttributes::HasFieldMarshal)
		{
			lAttrs.Append ("HasFieldMarshal ");
		}

		if	((int)pParameterAttrs & ((int)ParameterAttributes::ReservedMask & ~(int)ParameterAttributes::HasDefault))
		{
			lAttrs.Append ("Reserved ");
			lAttrs.Append (String::Format ("{0:X} ", ((int)pParameterAttrs & (int)ParameterAttributes::ReservedMask)));
		}
	}
	return lAttrs.ToString()->Trim();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

String^ LogAssembly::TypeLibTypeFlagsStr (Runtime::InteropServices::TypeLibTypeFlags pFlags)
{
	StringBuilder	lString;

	if	((int)pFlags & (int)TypeLibTypeFlags::FAggregatable)
	{
		lString.Append ("Aggregatable ");
	}
	if	((int)pFlags & (int)TypeLibTypeFlags::FAppObject)
	{
		lString.Append ("AppObject ");
	}
	if	((int)pFlags & (int)TypeLibTypeFlags::FCanCreate)
	{
		lString.Append ("CanCreate ");
	}
	if	((int)pFlags & (int)TypeLibTypeFlags::FControl)
	{
		lString.Append ("Control ");
	}
	if	((int)pFlags & (int)TypeLibTypeFlags::FDispatchable)
	{
		lString.Append ("Dispatchable ");
	}
	if	((int)pFlags & (int)TypeLibTypeFlags::FDual)
	{
		lString.Append ("Dual ");
	}
	if	((int)pFlags & (int)TypeLibTypeFlags::FHidden)
	{
		lString.Append ("Hidden ");
	}
	if	((int)pFlags & (int)TypeLibTypeFlags::FLicensed)
	{
		lString.Append ("Licensed ");
	}
	if	((int)pFlags & (int)TypeLibTypeFlags::FNonExtensible)
	{
		lString.Append ("NonExtensible ");
	}
	if	((int)pFlags & (int)TypeLibTypeFlags::FOleAutomation)
	{
		lString.Append ("OleAutomation ");
	}
	if	((int)pFlags & (int)TypeLibTypeFlags::FPreDeclId)
	{
		lString.Append ("PreDeclId ");
	}
	if	((int)pFlags & (int)TypeLibTypeFlags::FReplaceable)
	{
		lString.Append ("Replaceable ");
	}
	if	((int)pFlags & (int)TypeLibTypeFlags::FRestricted)
	{
		lString.Append ("Restricted ");
	}
	if	((int)pFlags & (int)TypeLibTypeFlags::FReverseBind)
	{
		lString.Append ("ReverseBind ");
	}

	return lString.ToString()->Trim();
}

/////////////////////////////////////////////////////////////////////////////

String^ LogAssembly::TypeLibFuncFlagsStr (Runtime::InteropServices::TypeLibFuncFlags pFlags)
{
	StringBuilder	lString;

	if	((int)pFlags & (int)TypeLibFuncFlags::FBindable)
	{
		lString.Append ("Bindable ");
	}
	if	((int)pFlags & (int)TypeLibFuncFlags::FDefaultBind)
	{
		lString.Append ("DefaultBind ");
	}
	if	((int)pFlags & (int)TypeLibFuncFlags::FDefaultCollelem)
	{
		lString.Append ("DefaultCollelem ");
	}
	if	((int)pFlags & (int)TypeLibFuncFlags::FDisplayBind)
	{
		lString.Append ("DisplayBind ");
	}
	if	((int)pFlags & (int)TypeLibFuncFlags::FHidden)
	{
		lString.Append ("Hidden ");
	}
	if	((int)pFlags & (int)TypeLibFuncFlags::FImmediateBind)
	{
		lString.Append ("ImmediateBind ");
	}
	if	((int)pFlags & (int)TypeLibFuncFlags::FNonBrowsable)
	{
		lString.Append ("NonBrowsable ");
	}
	if	((int)pFlags & (int)TypeLibFuncFlags::FReplaceable)
	{
		lString.Append ("Replaceable ");
	}
	if	((int)pFlags & (int)TypeLibFuncFlags::FRequestEdit)
	{
		lString.Append ("RequestEdit ");
	}
	if	((int)pFlags & (int)TypeLibFuncFlags::FRestricted)
	{
		lString.Append ("Restricted ");
	}
	if	((int)pFlags & (int)TypeLibFuncFlags::FSource)
	{
		lString.Append ("Source ");
	}
	if	((int)pFlags & (int)TypeLibFuncFlags::FUiDefault)
	{
		lString.Append ("UiDefault ");
	}
	if	((int)pFlags & (int)TypeLibFuncFlags::FUsesGetLastError)
	{
		lString.Append ("UsesGetLastError ");
	}

	return lString.ToString()->Trim();
}

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
