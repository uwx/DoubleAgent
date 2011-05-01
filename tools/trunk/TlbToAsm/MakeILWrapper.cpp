#include "stdafx.h"
#include <corhdr.h>
#include "MakeILWrapper.h"
#include "CopyAssembly.h"

#ifdef	_DEBUG
//#define	_DEBUG_CODE			LogDebugFast
//#define	_DEBUG_TOKENS		LogDebugFast
//#define	_DEBUG_OPCODES		LogDebugFast
//#define	_DEBUG_EXCEPTIONS	LogDebugFast
#endif

/////////////////////////////////////////////////////////////////////////////

using namespace System::Runtime::InteropServices;
using namespace System::Reflection;
using namespace System::Reflection::Emit;
using namespace System::Collections::Generic;

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

MakeILWrapper::MakeILWrapper (Module^ pSourceModule, ModuleBuilder^ pTargetModule, System::Type^ pWrappedType, CopyAssembly^ pCopy)
:	CopyILBinary (pSourceModule, pTargetModule, pCopy),
	mCopy (pCopy),
	mInterfaceType (pWrappedType)
{
#ifdef	_LOG_WRAPPERS_NOT
	LogMessage (_LOG_WRAPPERS, _T("MakeILWrapper [%s]"), _BT(pWrappedType));
#endif
}

bool MakeILWrapper::MakeWrapperMethod (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, MethodInfo^ pWrappedMethod)
{
	bool	lRet = false;

#ifdef	_LOG_WRAPPERS_NOT
	LogMessage (_LOG_WRAPPERS, _T("MakeILWrapper::MakeWrapperMethod [%s.%s] wraps [%s.%s] from [%s.%s]"), _BMT(pTargetMethod), _BM(pTargetMethod), _BMT(pWrappedMethod), _BM(pWrappedMethod), _BMT(pSourceMethod), _BM(pSourceMethod));
#endif
	MakeBasics (pSourceMethod->DeclaringType, pTargetMethod->DeclaringType);

	lRet = GenerateMethod (pSourceMethod, pTargetMethod, pWrappedMethod);

	if	(lRet)
	{
		MakeEnumerator (pSourceMethod, pTargetMethod);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool MakeILWrapper::GenerateMethod (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, MethodInfo^ pWrappedMethod)
{
	bool	lRet = false;

	try
	{
		ILGenerator^			lGenerator = pTargetMethod->GetILGenerator();
		array<ParameterInfo^>^	lParameters = pSourceMethod->GetParameters ();
		Type^					lReturnType = nullptr;
		Type^					lReturnWrapper = nullptr;
		Type^					lReturnInterface = nullptr;
		Type^					lParameterType = nullptr;
		MethodInfo^				lParameterAccessor;

		if	(
				(pTargetMethod->ReturnType)
			&&	(!ReferenceEquals (pTargetMethod->ReturnType, System::Void::typeid))
			)
		{
			LocalBuilder^	lReturnValue;

			lReturnType = pTargetMethod->ReturnType;
			if	(
					(lReturnWrapper = mCopy->GetCoClassWrapper (pSourceMethod->ReturnType))
				&&	(lReturnWrapper = mCopy->GetTargetType (lReturnWrapper, false))
				&&	(String::Compare (lReturnWrapper->FullName, lReturnType->FullName) == 0)
				)
			{
				lReturnInterface = pSourceMethod->ReturnType;
			}

			pTargetMethod->InitLocals = true;
			lReturnValue = lGenerator->DeclareLocal (lReturnType);
			lReturnValue->SetLocalSymInfo ("lRet");
		}

		lGenerator->Emit (OpCodes::Ldarg_0);
		lGenerator->Emit (OpCodes::Ldfld, mInterfaceField);
		if	(lParameters)
		{
			if	(lParameters->Length >= 1)
			{
				lGenerator->Emit (OpCodes::Ldarg_1);
				if	(
						(lParameterType = mCopy->GetCoClassWrapper (lParameters[0]->ParameterType))
					&&	(lParameterType = mCopy->GetTargetType (lParameterType, false))
					&&	(lParameterAccessor = mCopy->GetWrapperAccessor (lParameterType))
					)
				{
					lGenerator->Emit (OpCodes::Callvirt, lParameterAccessor);
				}
			}
			if	(lParameters->Length >= 2)
			{
				lGenerator->Emit (OpCodes::Ldarg_2);
			}
			if	(lParameters->Length >= 3)
			{
				lGenerator->Emit (OpCodes::Ldarg_3);
			}
			if	(lParameters->Length >= 4)
			{
				for (int lNdx = 3; lNdx < lParameters->Length; lNdx++)
				{
					lGenerator->Emit (OpCodes::Ldarg_S, (signed char)(short)(lParameters[lNdx]->Position+1));
				}
			}
		}

		lGenerator->Emit (OpCodes::Callvirt, pWrappedMethod);

		if	(lReturnType)
		{
			if	(
					(lReturnWrapper)
				&&	(lReturnInterface)
				)
			{
				lGenerator->Emit (OpCodes::Newobj, mCopy->GetWrapperConstructor (lReturnWrapper));
			}
			lGenerator->Emit (OpCodes::Stloc_0);
			lGenerator->Emit (OpCodes::Ldloc_0);
		}
		lGenerator->Emit (OpCodes::Ret);
		lRet = true;
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void MakeILWrapper::MakeEnumerator (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod)
{
	try
	{
		TypeBuilder^		lTypeBuilder;
		Type^				lEnumerable;
		Type^				lEnumeratedType;
		Type^				lEnumeratedWrapper;
		Type^				lEnumeratorInterface;
		ConstructorInfo^	lEnumeratorClass;
		MethodInfo^			lWrappedMethod;

		if	(
				(pSourceMethod->Name == "GetEnumerator")
			&&	(lEnumerable = pSourceMethod->DeclaringType->GetInterface ("IEnumerable"))
			&&	(lWrappedMethod = lEnumerable->GetMethod ("GetEnumerator"))
			&&	(lTypeBuilder = safe_cast <TypeBuilder^> (pTargetMethod->DeclaringType))
			)
		{
#ifdef	_LOG_WRAPPERS
			LogMessage (_LOG_WRAPPERS, _T("*==> Method     [%s.%s] overrides [%s.%s]"), _BMT(pTargetMethod), _BM(pTargetMethod), _BMT(lWrappedMethod), _BM(lWrappedMethod));
#endif
			lTypeBuilder->DefineMethodOverride (pTargetMethod, lWrappedMethod);

			if	(
					(lEnumeratedType = CopyAssembly::GetEnumerableType (pSourceMethod->DeclaringType))
				&&	(lEnumeratedType->IsInterface)
				&&	(lEnumeratedWrapper = mCopy->GetCoClassWrapper (lEnumeratedType))
				&&	(lEnumeratedType = mCopy->GetTargetType (lEnumeratedType, false))
				&&	(lEnumeratedWrapper = mCopy->GetTargetType (lEnumeratedWrapper, false))
				)
			{
#ifdef	_LOG_WRAPPERS
				LogMessage (_LOG_WRAPPERS, _T("*==> Type       [%s] enumerates [%s] as [%s]"), _BT(lTypeBuilder), _BT(lEnumeratedType), _BT(lEnumeratedWrapper));
#endif
				if	(
						(lEnumerable = System::Collections::Generic::IEnumerable::typeid)
					&&	(lWrappedMethod = lEnumerable->GetMethod ("GetEnumerator"))
					&&	(lEnumerable = lEnumerable->MakeGenericType (lEnumeratedWrapper))
					&&	(lWrappedMethod = lTypeBuilder->GetMethod (lEnumerable, lWrappedMethod))
					&&	(lEnumeratorInterface = System::Collections::Generic::IEnumerator::typeid->MakeGenericType (lEnumeratedWrapper))
					&&	(lEnumeratorClass = MakeEnumeratorClass (lEnumeratedType, lEnumeratedWrapper))
					)
				{
					MethodAttributes	lMethodAttributes = (MethodAttributes) ((int)MethodAttributes::Public | (int)MethodAttributes::Virtual | (int)MethodAttributes::ReuseSlot | (int)MethodAttributes::HideBySig | (int)MethodAttributes::Final);
					MethodBuilder^		lEnumMethod;
					ILGenerator^		lGenerator;

					if	(
							(lEnumMethod = lTypeBuilder->DefineMethod ("GetEnumerator", lMethodAttributes, lEnumeratorInterface, nullptr))
						&&	(lGenerator = lEnumMethod->GetILGenerator ())
						)
					{
						LocalBuilder^	lReturnValue;

						lEnumMethod->DefineParameter (0, ParameterAttributes::None, nullptr);
						lEnumMethod->InitLocals = true;
						lReturnValue = lGenerator->DeclareLocal (lEnumeratorInterface);
						lReturnValue->SetLocalSymInfo ("lRet");

						lGenerator->Emit (OpCodes::Ldarg_0);
						//lGenerator->Emit (OpCodes::Ldfld, mInterfaceField); // Calling through the wrapped interface causes the IEnumerable version to be called.  Without it, the _NewEnum version in the COM interface is called.
						lGenerator->Emit (OpCodes::Callvirt, System::Collections::IEnumerable::typeid->GetMethod ("GetEnumerator"));
						lGenerator->Emit (OpCodes::Newobj, lEnumeratorClass);
						lGenerator->Emit (OpCodes::Stloc_0);
						lGenerator->Emit (OpCodes::Ldloc_0);
						lGenerator->Emit (OpCodes::Ret);

						lTypeBuilder->DefineMethodOverride (lEnumMethod, lWrappedMethod);
					}
				}
			}
		}
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////

ConstructorInfo^ MakeILWrapper::MakeEnumeratorClass (Type^ pEnumeratedType, System::Type^ pEnumeratedWrapper)
{
	ConstructorInfo^	lEnumeratorConstructor = nullptr;

	try
	{
		TypeBuilder^	lTypeBuilder;
		TypeAttributes	lTypeAttributes = (TypeAttributes) ((int)TypeAttributes::Class | (int)TypeAttributes::AnsiClass | (int)TypeAttributes::BeforeFieldInit | (int)TypeAttributes::NotPublic);
		String^			lTypeName = pEnumeratedWrapper->Namespace + "." + pEnumeratedWrapper->Name + "Enumerator";
		Type^			lEnumInterface;

		if	(
				(lEnumInterface = System::Collections::Generic::IEnumerator::typeid->MakeGenericType (pEnumeratedWrapper))
			&&	(lTypeBuilder = mCopy->mModuleBuilder->DefineType (lTypeName, lTypeAttributes))
			)
		{
			array<Type^>^		lParamTypes = gcnew array<Type^> (1);
			MethodAttributes	lMethodAttributes = (MethodAttributes) ((int)MethodAttributes::Public | (int)MethodAttributes::Virtual | (int)MethodAttributes::ReuseSlot | (int)MethodAttributes::HideBySig | (int)MethodAttributes::Final);
			MethodAttributes	lAccessorAttributes = (MethodAttributes) ((int)lMethodAttributes | (int)MethodAttributes::SpecialName | (int)MethodAttributes::ReuseSlot);
			FieldBuilder^		lEnumField;
			ConstructorBuilder^	lConstructor;
			PropertyInfo^		lSourceProperty;
			PropertyBuilder^	lTargetProperty;
			MethodInfo^			lSourceMethod;
			MethodBuilder^		lTargetMethod;
			ILGenerator^		lGenerator;

#ifdef	_LOG_WRAPPERS
			LogMessage (_LOG_WRAPPERS, _T("*==> Enumerator [%s] for [%s]"), _BT(lTypeBuilder), _BT(pEnumeratedWrapper));
#endif
			lTypeBuilder->AddInterfaceImplementation (lEnumInterface);
			lEnumField = lTypeBuilder->DefineField ("mEnumerator", System::Collections::IEnumerator::typeid, FieldAttributes::Private);

			lParamTypes[0] = System::Collections::IEnumerator::typeid;
			if	(
					(lConstructor = lTypeBuilder->DefineConstructor ((MethodAttributes) ((int)MethodAttributes::Assembly | (int)MethodAttributes::HideBySig), CallingConventions::HasThis, lParamTypes))
				&&	(lGenerator = lConstructor->GetILGenerator ())
				)
			{
				Type^				lBaseType = System::Object::typeid;
				ConstructorInfo^	lBaseTypeConstructor = lBaseType->GetConstructors()[0];

				lConstructor->DefineParameter (1, ParameterAttributes::In, "Enumerator");

				lGenerator->Emit (OpCodes::Ldarg_0);
				lGenerator->Emit (OpCodes::Call, lBaseTypeConstructor);
				lGenerator->Emit (OpCodes::Ldarg_0);
				lGenerator->Emit (OpCodes::Ldarg_1);
				lGenerator->Emit (OpCodes::Stfld, lEnumField);
				//lGenerator->Emit (OpCodes::Ldarg_0);
				//lGenerator->Emit (OpCodes::Ldfld, mInterfaceField);
				//lGenerator->Emit (OpCodes::Callvirt, System::Collections::IEnumerator::typeid->GetMethod ("Reset"));
				lGenerator->Emit (OpCodes::Ret);
			}

			if	(
					(lSourceMethod = System::IDisposable::typeid->GetMethod ("Dispose"))
				&&	(lTargetMethod = lTypeBuilder->DefineMethod ("Dispose", lMethodAttributes, System::Void::typeid, nullptr))
				&&	(lGenerator = lTargetMethod->GetILGenerator ())
				)
			{
				lGenerator->Emit (OpCodes::Ldarg_0);
				lGenerator->Emit (OpCodes::Ldnull);
				lGenerator->Emit (OpCodes::Stfld, lEnumField);
				lGenerator->Emit (OpCodes::Ret);
#ifdef	_LOG_WRAPPERS
				LogMessage (_LOG_WRAPPERS, _T("*==>            [%s.%s] overrides [%s.%s]"), _BMT(lTargetMethod), _BM(lTargetMethod), _BMT(lSourceMethod), _BM(lSourceMethod));
#endif
				lTypeBuilder->DefineMethodOverride (lTargetMethod, lSourceMethod);
			}

			if	(
					(lSourceMethod = System::Collections::IEnumerator::typeid->GetMethod ("MoveNext"))
				&&	(lTargetMethod = lTypeBuilder->DefineMethod ("MoveNext", lMethodAttributes, System::Boolean::typeid, nullptr))
				&&	(lGenerator = lTargetMethod->GetILGenerator ())
				)
			{
				LocalBuilder^	lReturnValue;
				lTargetMethod->DefineParameter (0, ParameterAttributes::None, nullptr);
				lTargetMethod->InitLocals = true;
				lReturnValue = lGenerator->DeclareLocal (System::Boolean::typeid);
				lReturnValue->SetLocalSymInfo ("lRet");

				lGenerator->Emit (OpCodes::Ldarg_0);
				lGenerator->Emit (OpCodes::Ldfld, lEnumField);
				lGenerator->Emit (OpCodes::Callvirt, lSourceMethod);
				lGenerator->Emit (OpCodes::Stloc_0);
				lGenerator->Emit (OpCodes::Ldloc_0);
				lGenerator->Emit (OpCodes::Ret);
#ifdef	_LOG_WRAPPERS
				LogMessage (_LOG_WRAPPERS, _T("*==>            [%s.%s] overrides [%s.%s]"), _BMT(lTargetMethod), _BM(lTargetMethod), _BMT(lSourceMethod), _BM(lSourceMethod));
#endif
				lTypeBuilder->DefineMethodOverride (lTargetMethod, lSourceMethod);
			}

			if	(
					(lSourceMethod = System::Collections::IEnumerator::typeid->GetMethod ("Reset"))
				&&	(lTargetMethod = lTypeBuilder->DefineMethod ("Reset", lMethodAttributes, System::Void::typeid, nullptr))
				&&	(lGenerator = lTargetMethod->GetILGenerator ())
				)
			{
				lGenerator->Emit (OpCodes::Ldarg_0);
				lGenerator->Emit (OpCodes::Ldfld, lEnumField);
				lGenerator->Emit (OpCodes::Callvirt, lSourceMethod);
				lGenerator->Emit (OpCodes::Ret);
#ifdef	_LOG_WRAPPERS
				LogMessage (_LOG_WRAPPERS, _T("*==>            [%s.%s] overrides [%s.%s]"), _BMT(lTargetMethod), _BM(lTargetMethod), _BMT(lSourceMethod), _BM(lSourceMethod));
#endif
				lTypeBuilder->DefineMethodOverride (lTargetMethod, lSourceMethod);
			}

			if	(
					(lSourceProperty = System::Collections::Generic::IEnumerator::typeid->GetProperty ("Current"))
				&&	(lSourceMethod = lSourceProperty->GetGetMethod ())
				&&	(lSourceMethod = lTypeBuilder->GetMethod (lEnumInterface, lSourceMethod))
				&&	(lTargetProperty = lTypeBuilder->DefineProperty ("Current", Reflection::PropertyAttributes::None, pEnumeratedWrapper, nullptr))
				&&	(lTargetMethod = lTypeBuilder->DefineMethod ("get_Current", lAccessorAttributes, pEnumeratedWrapper, nullptr))
				&&	(lGenerator = lTargetMethod->GetILGenerator ())
				)
			{
				LocalBuilder^	lReturnValue;
				LocalBuilder^	lCurrentValue;
				MethodInfo^		lCallMethod = System::Collections::IEnumerator::typeid->GetMethod ("get_Current");
				Emit::Label		lEndLabel;

				lTargetMethod->DefineParameter (0, ParameterAttributes::None, nullptr);
				lTargetMethod->InitLocals = true;
				lReturnValue = lGenerator->DeclareLocal (pEnumeratedWrapper);
				lReturnValue->SetLocalSymInfo ("lRet");
				lCurrentValue = lGenerator->DeclareLocal (pEnumeratedType);
				lCurrentValue->SetLocalSymInfo ("lCurrent");
				lEndLabel = lGenerator->DefineLabel ();

				lGenerator->Emit (OpCodes::Ldnull);
				lGenerator->Emit (OpCodes::Stloc_0);
				lGenerator->Emit (OpCodes::Ldarg_0);
				lGenerator->Emit (OpCodes::Ldfld, lEnumField);
				lGenerator->Emit (OpCodes::Callvirt, lCallMethod);
				lGenerator->Emit (OpCodes::Castclass, pEnumeratedType);
				lGenerator->Emit (OpCodes::Stloc_1);
				lGenerator->Emit (OpCodes::Ldloc_1);
				lGenerator->Emit (OpCodes::Ldnull);
				lGenerator->Emit (OpCodes::Ceq);
				lGenerator->Emit (OpCodes::Brtrue_S, lEndLabel);
				lGenerator->Emit (OpCodes::Ldloc_1);
				lGenerator->Emit (OpCodes::Newobj, mCopy->GetWrapperConstructor (pEnumeratedWrapper));
				lGenerator->Emit (OpCodes::Stloc_0);
				lGenerator->MarkLabel (lEndLabel);
				lGenerator->Emit (OpCodes::Ldloc_0);
				lGenerator->Emit (OpCodes::Ret);
#ifdef	_LOG_WRAPPERS
				LogMessage (_LOG_WRAPPERS, _T("*==>            [%s.%s] overrides [%s.%s]"), _BMT(lTargetMethod), _BM(lTargetMethod), _BMT(lSourceMethod), _BM(lSourceMethod));
#endif
				lTypeBuilder->DefineMethodOverride (lTargetMethod, lSourceMethod);
				lTargetProperty->SetGetMethod (lTargetMethod);
			}

			if	(
					(lSourceProperty = System::Collections::IEnumerator::typeid->GetProperty ("Current"))
				&&	(lSourceMethod = lSourceProperty->GetGetMethod ())
				&&	(lTargetProperty = lTypeBuilder->DefineProperty ("System.Collections.IEnumerator.Current", Reflection::PropertyAttributes::None, System::Object::typeid, nullptr))
				&&	(lTargetMethod = lTypeBuilder->DefineMethod ("System.Collections.IEnumerator.get_Current", lAccessorAttributes, System::Object::typeid, nullptr))
				&&	(lGenerator = lTargetMethod->GetILGenerator ())
				)
			{
				LocalBuilder^	lReturnValue;

				lTargetMethod->DefineParameter (0, ParameterAttributes::None, nullptr);
				lTargetMethod->InitLocals = true;
				lReturnValue = lGenerator->DeclareLocal (System::Object::typeid);
				lReturnValue->SetLocalSymInfo ("lRet");

				lGenerator->Emit (OpCodes::Ldarg_0);
				lGenerator->Emit (OpCodes::Ldfld, lEnumField);
				lGenerator->Emit (OpCodes::Callvirt, lSourceMethod);
				lGenerator->Emit (OpCodes::Stloc_0);
				lGenerator->Emit (OpCodes::Ldloc_0);
				lGenerator->Emit (OpCodes::Ret);
#ifdef	_LOG_WRAPPERS
				LogMessage (_LOG_WRAPPERS, _T("*==>            [%s.%s] overrides [%s.%s]"), _BMT(lTargetMethod), _BM(lTargetMethod), _BMT(lSourceMethod), _BM(lSourceMethod));
#endif
				lTypeBuilder->DefineMethodOverride (lTargetMethod, lSourceMethod);
				lTargetProperty->SetGetMethod (lTargetMethod);
			}

			mCopy->CopiedType (lTypeBuilder, lTypeBuilder);
			lEnumeratorConstructor = lConstructor;
		}
	}
	catch AnyExceptionDebug

	return lEnumeratorConstructor;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void MakeILWrapper::MakeBasics (System::Type^ pSourceType, System::Type^ pTargetType)
{
	if	(!mInterfaceField)
	{
		try
		{
			TypeBuilder^	lTargetType;

			if	(
					(lTargetType = safe_cast <TypeBuilder^> (pTargetType))
				&&	(mInterfaceField = lTargetType->DefineField ("mInterface", mInterfaceType, FieldAttributes::Private))
				)
			{
				GenerateConstructor (pSourceType, lTargetType);
				GenerateAccessor (lTargetType);
			}
		}
		catch AnyException
	}
}

void MakeILWrapper::GenerateConstructor (Type^ pSourceType, TypeBuilder^ pTargetType)
{
	ConstructorInfo^	lCoClass;
	ConstructorBuilder^	lConstructor;
	array<Type^>^		lParamTypes = gcnew array<Type^> (1);
	ILGenerator^		lGenerator;
	Type^				lBaseType = System::Object::typeid;
	ConstructorInfo^	lBaseTypeConstructor = lBaseType->GetConstructors()[0];

	if	(lCoClass = MakeCoClass (pSourceType, pTargetType))
	{
		lConstructor = pTargetType->DefineConstructor ((MethodAttributes) ((int)MethodAttributes::Public | (int)MethodAttributes::HideBySig), CallingConventions::HasThis, nullptr);
	}
	else
	{
		lParamTypes [0] = mInterfaceType;
		lConstructor = pTargetType->DefineConstructor ((MethodAttributes) ((int)MethodAttributes::Public | (int)MethodAttributes::HideBySig), CallingConventions::HasThis, lParamTypes);
		lConstructor->DefineParameter (1, ParameterAttributes::In, "Interface");

		try
		{
			array <Type^>^		lAttrArgTypes = gcnew array <Type^> (1);
			array <Object^>^	lAttrArgValues = gcnew array <Object^> (1);

			lAttrArgTypes [0] = TypeLibFuncFlags::typeid;
			lAttrArgValues [0] = (TypeLibFuncFlags)((int)TypeLibFuncFlags::FHidden | (int)TypeLibFuncFlags::FRestricted);
			lConstructor->SetCustomAttribute (gcnew CustomAttributeBuilder (TypeLibFuncAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));

			lAttrArgTypes [0] = EditorBrowsableState::typeid;
			lAttrArgValues [0] = EditorBrowsableState::Never;
			lConstructor->SetCustomAttribute (gcnew CustomAttributeBuilder (EditorBrowsableAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));
		}
		catch AnyExceptionDebug
	}

	lGenerator = lConstructor->GetILGenerator ();
	lGenerator->Emit (OpCodes::Ldarg_0);
	lGenerator->Emit (OpCodes::Call, lBaseTypeConstructor);
	lGenerator->Emit (OpCodes::Ldarg_0);
	if	(lCoClass)
	{
		lGenerator->Emit (OpCodes::Newobj, lCoClass);
		lGenerator->Emit (OpCodes::Stfld, mInterfaceField);
	}
	else
	{
		lGenerator->Emit (OpCodes::Ldarg_1);
		lGenerator->Emit (OpCodes::Stfld, mInterfaceField);
	}
	lGenerator->Emit (OpCodes::Ret);

	mCopy->PutWrapperConstructor (pTargetType, lConstructor);
}

void MakeILWrapper::GenerateAccessor (System::Reflection::Emit::TypeBuilder^ pTargetType)
{
	PropertyBuilder^	lProperty;
	MethodBuilder^		lMethod;
	LocalBuilder^		lReturnValue;
	array<Type^>^		lParamTypes = gcnew array<Type^> (0);
	ILGenerator^		lGenerator;

	lMethod = pTargetType->DefineMethod ("get_Interface", (MethodAttributes) ((int)MethodAttributes::Public | (int)MethodAttributes::HideBySig | (int)MethodAttributes::SpecialName), mInterfaceType, lParamTypes);
	lMethod->DefineParameter (0, ParameterAttributes::None, nullptr);
	lMethod->InitLocals = true;
	lProperty = pTargetType->DefineProperty ("Interface", Reflection::PropertyAttributes::None, mInterfaceType, lParamTypes),
	lProperty->SetGetMethod (lMethod);

	try
	{
		array <Type^>^		lAttrArgTypes = gcnew array <Type^> (1);
		array <Object^>^	lAttrArgValues = gcnew array <Object^> (1);

		lAttrArgTypes [0] = TypeLibFuncFlags::typeid;
		lAttrArgValues [0] = (TypeLibFuncFlags)((int)TypeLibFuncFlags::FHidden | (int)TypeLibFuncFlags::FRestricted);
		lMethod->SetCustomAttribute (gcnew CustomAttributeBuilder (TypeLibFuncAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));

		lAttrArgTypes [0] = TypeLibVarFlags::typeid;
		lAttrArgValues [0] = (TypeLibVarFlags)((int)TypeLibVarFlags::FHidden | (int)TypeLibVarFlags::FRestricted);
		lProperty->SetCustomAttribute (gcnew CustomAttributeBuilder (TypeLibVarAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));

		lAttrArgTypes [0] = EditorBrowsableState::typeid;
		lAttrArgValues [0] = EditorBrowsableState::Never;
		lMethod->SetCustomAttribute (gcnew CustomAttributeBuilder (EditorBrowsableAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));
		lProperty->SetCustomAttribute (gcnew CustomAttributeBuilder (EditorBrowsableAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));

		lAttrArgTypes [0] = System::Boolean::typeid;
		lAttrArgValues [0] = false;
		lProperty->SetCustomAttribute (gcnew CustomAttributeBuilder (BrowsableAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));
	}
	catch AnyExceptionDebug


	lGenerator = lMethod->GetILGenerator ();
	lReturnValue = lGenerator->DeclareLocal (mInterfaceType);
	lReturnValue->SetLocalSymInfo ("lRet");

	lGenerator->Emit (OpCodes::Ldarg_0);
	lGenerator->Emit (OpCodes::Ldfld, mInterfaceField);
	lGenerator->Emit (OpCodes::Stloc_0);
	lGenerator->Emit (OpCodes::Ldloc_0);
	lGenerator->Emit (OpCodes::Ret);

	mCopy->PutWrapperAccessor (pTargetType, lMethod);
}

/////////////////////////////////////////////////////////////////////////////

ConstructorInfo^ MakeILWrapper::GetWrapperConstructor (Type^ pWrapperType, Type^ pInterfaceType)
{
	ConstructorInfo^	lConstructor = mCopy->GetWrapperConstructor (pWrapperType);

	if	(!lConstructor)
	{
		array<ConstructorInfo^>^	lConstructors;
		array<ParameterInfo^>^		lParameters;

		if	(
				(lConstructors = pWrapperType->GetConstructors (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
			&&	(lConstructors->Length > 0)
			&&	(lParameters = lConstructors[0]->GetParameters ())
			&&	(lParameters->Length > 0)
			&&	(String::Compare (lParameters[0]->ParameterType->FullName, pInterfaceType->FullName) == 0)
			)
		{
			lConstructor = lConstructors[0];
		}
	}

#ifdef	_DEBUG
	if	(!lConstructor)
	{
		LogMessage (LogNormal, _T("*** GetWrapperConstructor [%s]"), _BT(pWrapperType));
	}
#endif
	return lConstructor;
}

MethodInfo^ MakeILWrapper::GetWrapperAccessor (Type^ pWrapperType, Type^ pInterfaceType)
{
	MethodInfo^	lMethod = mCopy->GetWrapperAccessor (pWrapperType);

	if	(!lMethod)
	{
		MethodInfo^	lAccessor;

		if	(
				(lAccessor = pWrapperType->GetMethod ("get_Interface"))
			&&	(lAccessor->ReturnType)
			&&	(String::Compare (lAccessor->ReturnType->FullName, pInterfaceType->FullName) == 0)
			)
		{
			lMethod = lAccessor;
		}
	}
#ifdef	_DEBUG
	if	(!lMethod)
	{
		LogMessage (LogNormal, _T("*** GetWrapperAccessor [%s]"), _BT(pWrapperType));
	}
#endif
	return lMethod;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool MakeILWrapper::NeedsCoClass (Type^ pSourceType)
{
	bool				lRet = false;
	TypeLibTypeFlags	lTypeFlags;
	GuidAttribute^		lGuidAttribute;

	lTypeFlags = mCopy->GetTypeLibTypeFlags (pSourceType);

	if	(
			((int)lTypeFlags & (int)TypeLibTypeFlags::FCanCreate)
		&&	(lGuidAttribute = safe_cast <GuidAttribute^> (Attribute::GetCustomAttribute (pSourceType, GuidAttribute::typeid)))
		)
	{
#ifdef	_LOG_WRAPPERS
		LogMessage (_LOG_WRAPPERS, _T("*==> Class      [%s] Guid [%s]"), _BT(pSourceType), _B(lGuidAttribute->Value->ToString()));
#endif
		lRet = true;
	}
	return lRet;
}

ConstructorInfo^ MakeILWrapper::MakeCoClass (Type^ pSourceType, Type^ pTargetType)
{
	ConstructorInfo^	lRet = nullptr;

	try
	{
		if	(NeedsCoClass (pSourceType))
		{
			TypeBuilder^		lTypeBuilder;
			TypeAttributes		lTypeAttributes = (TypeAttributes) ((int)pSourceType->Attributes & ~(int)TypeAttributes::VisibilityMask | (int)TypeAttributes::NotPublic);
			String^				lTypeName = pTargetType->Namespace + "." + pTargetType->Name + "CoClass";
			ConstructorInfo^	lSourceConstructor;
			ConstructorBuilder^	lTargetConstructor;

			if	(
					(lSourceConstructor = pSourceType->GetConstructor (gcnew array<Type^>(0)))
				&&	(lTypeBuilder = mCopy->mModuleBuilder->DefineType (lTypeName, lTypeAttributes))
				)
			{
				if	(lTargetConstructor = lTypeBuilder->DefineDefaultConstructor (MethodAttributes::Assembly))
				{
					lTargetConstructor->SetImplementationFlags (lSourceConstructor->GetMethodImplementationFlags ());
				}
				mCopy->CopyTypeInterfaces (pSourceType, lTypeBuilder, nullptr);
				mCopy->CopyTypeAttributes (pSourceType, lTypeBuilder, nullptr);
				mCopy->CopiedType (lTypeBuilder, lTypeBuilder);
				lRet = lTargetConstructor;
#ifdef	_LOG_WRAPPERS
				LogMessage (_LOG_WRAPPERS, _T("*==> CoClass    [%s]"), _BT(lTypeBuilder));
#endif
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool MakeILWrapper::PutBodyOpCode (Object^ pData, Emit::OpCode & pOpCode, LPBYTE pOperand, int pOffset, LPBYTE pBinary)
{
	bool			lRet;
	MethodInfo^		lMethod;
	MethodCopyData^	lData = safe_cast <MethodCopyData^> (pData);

	if	(
			(pOpCode.Equals (OpCodes::Ret))
		&&	(lMethod = GetBodyMethod (pData))
		&&	(lMethod->DeclaringType->IsSubclassOf (System::Windows::Forms::AxHost::typeid))
		)
	{
		InterfaceToWrapper (lMethod, lMethod->ReturnParameter, false, lData->mGenerator, pOpCode);
	}

	lRet = CopyILBinary::PutBodyOpCode (pData, pOpCode, pOperand, pOffset, pBinary);

	if	(
			(
				(pOpCode.Equals (OpCodes::Ldarg_1))
			||	(pOpCode.Equals (OpCodes::Ldarg_2))
			||	(pOpCode.Equals (OpCodes::Ldarg_3))
			||	(pOpCode.Equals (OpCodes::Ldarg_S))
			||	(pOpCode.Equals (OpCodes::Ldarg))
			)
		&&	(lMethod = GetBodyMethod (pData))
		)
	{
		if	(
				(lMethod->DeclaringType->Name->EndsWith ("SinkHelper"))
			||	(lMethod->DeclaringType->Name->EndsWith ("Multicaster"))
			)
		{
			if	(pOpCode.Equals (OpCodes::Ldarg_1))
			{
				InterfaceToWrapper (lMethod, 0, true, lData->mGenerator, pOpCode);
			}
			else
			if	(pOpCode.Equals (OpCodes::Ldarg_2))
			{
				InterfaceToWrapper (lMethod, 1, true, lData->mGenerator, pOpCode);
			}
			else
			if	(pOpCode.Equals (OpCodes::Ldarg_3))
			{
				InterfaceToWrapper (lMethod, 2, true, lData->mGenerator, pOpCode);
			}
			else
			if	(pOpCode.Equals (OpCodes::Ldarg_S))
			{
				InterfaceToWrapper (lMethod, (*(signed char*)pOperand)-1, true, lData->mGenerator, pOpCode);
			}
			else
			{
				InterfaceToWrapper (lMethod, (*(short*)pOperand)-1, true, lData->mGenerator, pOpCode);
			}
		}
		else
		if	(lMethod->DeclaringType->IsSubclassOf (System::Windows::Forms::AxHost::typeid))
		{
			if	(pOpCode.Equals (OpCodes::Ldarg_1))
			{
				WrapperToInterface (lMethod, 0, false, lData->mGenerator, pOpCode);
			}
			else
			if	(pOpCode.Equals (OpCodes::Ldarg_2))
			{
				WrapperToInterface (lMethod, 1, false, lData->mGenerator, pOpCode);
			}
			else
			if	(pOpCode.Equals (OpCodes::Ldarg_3))
			{
				WrapperToInterface (lMethod, 2, false, lData->mGenerator, pOpCode);
			}
			else
			if	(pOpCode.Equals (OpCodes::Ldarg_S))
			{
				WrapperToInterface (lMethod, (*(signed char*)pOperand)-1, false, lData->mGenerator, pOpCode);
			}
			else
			{
				WrapperToInterface (lMethod, (*(short*)pOperand)-1, false, lData->mGenerator, pOpCode);
			}
		}
	}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool MakeILWrapper::WrapperToInterface (MethodInfo^ pMethod, int pParameterNdx, bool pIsEvent, Emit::ILGenerator^ pGenerator, Emit::OpCode & pOpCode)
{
	array<ParameterInfo^>^		lParameters;

	if	(
			(pMethod)
		&&	(pParameterNdx >= 0)
		&&	(lParameters = pMethod->GetParameters ())
		&&	(lParameters->Length > pParameterNdx)
		)
	{
		return WrapperToInterface (pMethod, lParameters [pParameterNdx], pIsEvent, pGenerator, pOpCode);
	}
	return false;
}

bool MakeILWrapper::WrapperToInterface (MethodInfo^ pMethod, ParameterInfo^ pParameter, bool pIsEvent, Emit::ILGenerator^ pGenerator, Emit::OpCode & pOpCode)
{
	bool	lRet = false;

	if	(
			(pMethod)
		&&	(pParameter)
		)
	{
		try
		{
			Type^		lWrapperType;
			Type^		lInterfaceType;
			MethodInfo^	lAccessor;

			if	(
					(mTranslator->TranslateType (pParameter->ParameterType, lInterfaceType))
				&&	(mTranslator->TranslateParameter (pParameter, lWrapperType))
				&&	(!ReferenceEquals (lWrapperType, lInterfaceType))
				&&	(lAccessor = GetWrapperAccessor (lWrapperType, lInterfaceType))
				)
			{
				Emit::LocalBuilder^	lLocalWrapper;
				Emit::LocalBuilder^	lLocalInterface;
				Emit::Label			lLabel;

				lLocalWrapper = pGenerator->DeclareLocal (lWrapperType);
				lLocalInterface = pGenerator->DeclareLocal (lInterfaceType);
				lLabel = pGenerator->DefineLabel ();
				pGenerator->Emit (OpCodes::Stloc, lLocalWrapper);
				pGenerator->Emit (OpCodes::Ldnull);
				pGenerator->Emit (OpCodes::Stloc, lLocalInterface);
				pGenerator->Emit (OpCodes::Ldloc, lLocalWrapper);
				pGenerator->Emit (OpCodes::Brfalse, lLabel);
				pGenerator->Emit (OpCodes::Ldloc, lLocalWrapper);
				pGenerator->Emit (OpCodes::Callvirt, lAccessor);
				pGenerator->Emit (OpCodes::Stloc, lLocalInterface);
				pGenerator->MarkLabel (lLabel);
				pGenerator->Emit (OpCodes::Ldloc, lLocalInterface);
#ifdef	_LOG_WRAPPERS
				LogMessage (_LOG_WRAPPERS, _T("*==> Method     [%s.%s] Parameter [%d] [%s] as [%s]"), _BMT(pMethod), _BM(pMethod), pParameter->Position, _BT(lInterfaceType), _BT(lWrapperType));
#endif
				lRet = true;
			}
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool MakeILWrapper::InterfaceToWrapper (MethodInfo^ pMethod, int pParameterNdx, bool pIsEvent, Emit::ILGenerator^ pGenerator, Emit::OpCode & pOpCode)
{
	array<ParameterInfo^>^		lParameters;

	if	(
			(pMethod)
		&&	(pParameterNdx >= 0)
		&&	(lParameters = pMethod->GetParameters ())
		&&	(lParameters->Length > pParameterNdx)
		)
	{
		return InterfaceToWrapper (pMethod, lParameters [pParameterNdx], pIsEvent, pGenerator, pOpCode);
	}
	return false;
}

bool MakeILWrapper::InterfaceToWrapper (MethodInfo^ pMethod, ParameterInfo^ pParameter, bool pIsEvent, Emit::ILGenerator^ pGenerator, Emit::OpCode & pOpCode)
{
	bool	lRet;

	if	(
			(pMethod)
		&&	(pParameter)
		)
	{
		try
		{
			Type^				lInterfaceType;
			Type^				lWrapperType;
			ConstructorInfo^	lConstructor;

#ifdef	_DEBUG_NOT
			try
			{
				Type^	lInterfaceType;
				Type^	lWrapperType1;
				Type^	lWrapperType2;

				if	(mTranslator->TranslateType (pParameter->ParameterType, lInterfaceType))
				{
					if	(lWrapperType1 = mCopy->GetCoClassWrapper (pParameter->ParameterType))
					{
						lWrapperType1 = mCopy->GetTargetType (lWrapperType1, false);
					}
					mTranslator->TranslateParameter (pParameter, lWrapperType2);

					if	(
							(
								(lWrapperType1)
							&&	(!ReferenceEquals (lWrapperType1, lInterfaceType))
							)
						||	(
								(lWrapperType2)
							&&	(!ReferenceEquals (lWrapperType2, lInterfaceType))
							)
						)
					{
						LogMessage (LogDebugFast, _T("InterfaceToWrapper [%s][%u] ForEvent [%u]"), _BT(pParameter->ParameterType), mCopy->IsCoClassWrapper (pParameter->ParameterType), pIsEvent);
						LogMessage (LogDebugFast, _T("  Interface        [%s][%u]"), _BT(lInterfaceType), mCopy->IsCoClassWrapper (lInterfaceType));
						LogMessage (LogDebugFast, _T("  Wrapper1         [%s][%u]"), _BT(lWrapperType1), mCopy->IsCoClassWrapper (lWrapperType1));
						LogMessage (LogDebugFast, _T("  Wrapper2         [%s][%u]"), _BT(lWrapperType2), mCopy->IsCoClassWrapper (lWrapperType2));
					}
				}
			}
			catch AnyExceptionSilent
#endif			

			if	(
					(mTranslator->TranslateType (pParameter->ParameterType, lInterfaceType))
				&&	(lWrapperType = mCopy->GetCoClassWrapper (pParameter->ParameterType))
				&&	(lWrapperType = mCopy->GetTargetType (lWrapperType, false))
				&&	(!ReferenceEquals (lWrapperType, lInterfaceType))
				&&	(lConstructor = GetWrapperConstructor (lWrapperType, lInterfaceType))
				)
			{
				Emit::LocalBuilder^	lLocalInterface;
				Emit::LocalBuilder^	lLocalWrapper;
				Emit::Label			lLabel;

				lLocalInterface = pGenerator->DeclareLocal (lInterfaceType);
				lLocalWrapper = pGenerator->DeclareLocal (lWrapperType);
				lLabel = pGenerator->DefineLabel ();
				pGenerator->Emit (OpCodes::Stloc, lLocalInterface);
				pGenerator->Emit (OpCodes::Ldnull);
				pGenerator->Emit (OpCodes::Stloc, lLocalWrapper);
				pGenerator->Emit (OpCodes::Ldloc, lLocalInterface);
				pGenerator->Emit (OpCodes::Brfalse, lLabel);
				pGenerator->Emit (OpCodes::Ldloc, lLocalInterface);
				pGenerator->Emit (OpCodes::Newobj, lConstructor);
				pGenerator->Emit (OpCodes::Stloc, lLocalWrapper);
				pGenerator->MarkLabel (lLabel);
				pGenerator->Emit (OpCodes::Ldloc, lLocalWrapper);
#ifdef	_LOG_WRAPPERS
				LogMessage (_LOG_WRAPPERS, _T("*==> Method     [%s.%s] Parameter [%d] [%s] as [%s]"), _BMT(pMethod), _BM(pMethod), pParameter->Position, _BT(lInterfaceType), _BT(lWrapperType));
#endif
				lRet = true;
			}
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
