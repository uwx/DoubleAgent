#pragma once

namespace DoubleAgent {
/////////////////////////////////////////////////////////////////////////////

ref class LogAssembly
{
public:
	LogAssembly () : mLogLevel (LogNormal|LogNoPrefix) {}
	LogAssembly (UInt32 pLogLevel) : mLogLevel (pLogLevel) {}
	~LogAssembly () {}

	void Log (System::Reflection::Assembly^ pAssembly);
	void LogReferences (System::Reflection::Assembly^ pAssembly);
	void LogType (System::Type^ pType, System::String^ pTitle);
	void LogType (System::Type^ pType) {LogType (pType, nullptr);}
	void LogType (System::Type^ pType, System::UInt32 pIndent, System::String^ pTitle);
	void LogType (System::Type^ pType, System::UInt32 pIndent) {LogType (pType, pIndent, nullptr);}
	void LogField (System::Reflection::FieldInfo^ pField, System::UInt32 pIndent);
	void LogProperty (System::Reflection::PropertyInfo^ pProperty, System::UInt32 pIndent);
	void LogMethod (System::Reflection::MethodBase^ pMethod, System::Reflection::MethodInfo^ pMethodInfo, System::UInt32 pIndent);
	void LogEvent (System::Reflection::EventInfo^ pEvent, System::UInt32 pIndent);
	void LogParameter (System::Reflection::ParameterInfo^ pParameter, System::UInt32 pIndent);
	void LogAttributes (System::Collections::Generic::IList <System::Reflection::CustomAttributeData^>^ pAttributes, System::UInt32 pIndent);

	static bool IsInterfaceImplemented (Type^ pType, Type^ pInterface);
	static bool IsInterfaceInherited (Type^ pType, Type^ pInterface);

	static System::String^ TypeProps (System::Type^ pType);
	static System::String^ TypeAttrsStr (System::Reflection::TypeAttributes pTypeAttrs);
	static System::String^ FieldProps (System::Reflection::FieldInfo^ pField);
	static System::String^ PropertyProps (System::Reflection::PropertyInfo^ pProperty);
	static System::String^ PropertyAttrsStr (System::Reflection::PropertyAttributes pPropertyAttrs);
	static System::String^ MethodProps (System::Reflection::MethodInfo^ pMethod);
	static System::String^ MethodAttrsStr (System::Reflection::MethodAttributes pMethodAttrs);
	static System::String^ MethodCallType (System::Reflection::CallingConventions pCallType);
	static System::String^ MethodImplementation (System::Reflection::MethodImplAttributes pAttrs);
	static System::String^ EventProps (System::Reflection::EventInfo^ pEvent);
	static System::String^ EventAttrsStr (System::Reflection::EventAttributes pEventAttrs);
	static System::String^ ParameterProps (System::Reflection::ParameterInfo^ pParameter);
	static System::String^ TypeLibTypeFlagsStr (System::Runtime::InteropServices::TypeLibTypeFlags pFlags);
	static System::String^ TypeLibFuncFlagsStr (System::Runtime::InteropServices::TypeLibFuncFlags pFlags);

public:
	System::UInt32	mLogLevel;
};

/////////////////////////////////////////////////////////////////////////////
};