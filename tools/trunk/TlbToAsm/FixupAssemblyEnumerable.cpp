#include "stdafx.h"
#include "FixupAssemblySpecific.h"

using namespace System::Runtime::CompilerServices;

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

generic <class T> ref class _EnumeratorClass : public System::Collections::Generic::IEnumerator<T>
{
private:
	System::Collections::IEnumerator^	mInnerEnumerator;
public:
	_EnumeratorClass (System::Collections::IEnumerator^ pEnumerator)
	:	mInnerEnumerator (pEnumerator)
	{
#ifdef	_DEBUG
		System::Diagnostics::Debug::Print ("{0}.{1}()", GetType()->Namespace, GetType()->Name);
#endif
	}
	virtual ~_EnumeratorClass () {}

	virtual void Reset ()
	{
		mInnerEnumerator->Reset ();
	}
	virtual bool MoveNext ()
	{
		return mInnerEnumerator->MoveNext ();
	}

	virtual System::Object^ get_Current_NonGeneric () = System::Collections::IEnumerator::Current::get
	{
		return mInnerEnumerator->Current;
	}

	virtual T get_Current_Generic () = System::Collections::Generic::IEnumerator<T>::Current::get
	{
		return (T) (mInnerEnumerator->Current);
	}
};

/////////////////////////////////////////////////////////////////////////////

generic <class T> ref class _EnumerableClass abstract : public System::Collections::IEnumerable
{
public:
//	property System::Collections::Generic::IEnumerator<T>^ List
//	{
//		System::Collections::Generic::IEnumerator<T>^ get ()
//		{
////#ifdef	_DEBUG
////			System::Diagnostics::Debug::Print ("{0}.{1}.get_List", GetType()->Namespace, GetType()->Name);
////#endif
////			return gcnew _EnumeratorClass<T> (GetEnumerator());
//			return nullptr;
//		}
//	}

	void get_List ()
	{
	}
	
	virtual System::Collections::IEnumerator^ GetEnumerator () abstract;
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

Type^ FixupAssemblySpecific::AddInternalType (Type^ pType)
{
	return AddInternalType (pType, nullptr);
}

Type^ FixupAssemblySpecific::AddInternalType (Type^ pType, Type^ pGenericArgument)
{
	Type^	lRet = nullptr;

	if	(mInternalCopy)
	{
		try
		{
			TypeBuilder^	lTypeBuilder = nullptr;
			String^			lTypeName = String::Concat (mInternalCopy->mModuleName, ".", pType->Name);

#ifdef	_LOG_FIXES_NOT
			LogMessage (_LOG_FIXES, _T("--> Add?       [%s]"), _B(lTypeName));
#endif
			try
			{
				lTypeBuilder = mInternalCopy->mModuleBuilder->DefineType (lTypeName, pType->Attributes);
			}
			catch AnyExceptionSilent

			if	(lTypeBuilder)
			{
				mInternalCopy->CopyType (pType, lTypeBuilder);
				mInternalCopy->CopiedType (pType, lTypeBuilder);
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("--> Add        [%s] from [%s]"), _BT(lTypeBuilder), _BT(pType));
#endif
				if	(!pGenericArgument)
				{
					lRet = lTypeBuilder;
				}
			}
#ifdef	_LOG_FIXES_NOT
			else
			{
				LogMessage (_LOG_FIXES, _T("--> Add~       [%s] from [%s]"), _B(lTypeName), _BT(pType));
			}
#endif
			
			if	(
					(!lTypeBuilder)
				&&	(pGenericArgument)
				)
			{
				try
				{
					lTypeBuilder = safe_cast <TypeBuilder^> (mInternalCopy->GetTargetType (pType, false));
				}
				catch AnyExceptionSilent
			}
			
			if	(
					(lTypeBuilder)
				&&	(pGenericArgument)
				)
			{
				if	(pType->Name->LastIndexOf('`') > 0)
				{
					lTypeName = String::Concat (mInternalCopy->mModuleName, ".", pType->Name->Substring (0, pType->Name->LastIndexOf('`')), "_", pGenericArgument->Name);
				}
				else
				{
					lTypeName = String::Concat (mInternalCopy->mModuleName, ".", pType->Name, "_", pGenericArgument->Name);
				}

#ifdef	_LOG_FIXES_NOT
				LogMessage (_LOG_FIXES, _T("--> Add?       [%s] from [%s]"), _B(lTypeName), _BT(pType));
#endif
				try
				{
					if	(
							(pType = lTypeBuilder->MakeGenericType (pGenericArgument))
						&&	(lTypeBuilder = mInternalCopy->mModuleBuilder->DefineType (lTypeName, pType->Attributes))
						)
					{
						lTypeBuilder->SetParent (pType);
						mInternalCopy->CopiedType (pType, lTypeBuilder);
#ifdef	_LOG_FIXES
						LogMessage (_LOG_FIXES, _T("--> Add        [%s] from [%s]"), _BT(lTypeBuilder), _BT(pType));
#endif
						lRet = lTypeBuilder;
					}
#ifdef	_LOG_FIXES_NOT
					else
					{
						LogMessage (_LOG_FIXES, _T("--> Add~       [%s] from [%s]"), _B(lTypeName), _BT(pType));
					}
#endif
				}
				catch AnyExceptionDebug
			}
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#if	FALSE
//
//	Change IEnumerable inheritance to specific IEnumerable<T> implementation
//
Type^ FixupAssemblySpecific::MakeEnumerableBase (Type^ pSourceType)
{
	Type^	lBaseType = nullptr;
	if	(
			(pSourceType)
		&&	(pSourceType->IsClass)
		&&	(mInternalCopy)
		)
	{
		try
		{
			array<Type^>^	lSourceInterfaces;
			Type^			lSourceInterface;
			Type^			lEnumeratorType = nullptr;

			if	(lSourceInterfaces = pSourceType->GetInterfaces ())
			{
				for each (lSourceInterface in lSourceInterfaces)
				{
					if	(
							(ReferenceEquals (lSourceInterface, System::Collections::IEnumerable::typeid))
						&&	(mCopy->IsInterfaceInherited (pSourceType, lSourceInterface))
						)
					{
						break;
					}
					lSourceInterface = nullptr;
				}
			}
			if	(lSourceInterface)
			{
				array<MethodInfo^>^		lSourceMethods;
				int						lMethodNdx;
				DispIdAttribute^		lDispIdAttribute;

				if	(lSourceMethods = pSourceType->GetMethods (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
				{
					for	(lMethodNdx = lSourceMethods->Length-1; lMethodNdx >= 0; lMethodNdx--)
					{
						if	(
								(lDispIdAttribute = mCopy->GetDispId (lSourceMethods [lMethodNdx]))
							&&	(lDispIdAttribute->Value == DISPID_NEWENUM)
							)
						{
							break;
						}
					}
				}
				if	(lMethodNdx >= 0)
				{
					for	(lMethodNdx = lSourceMethods->Length-1; lMethodNdx >= 0; lMethodNdx--)
					{
						if	(
								(lDispIdAttribute = mCopy->GetDispId (lSourceMethods [lMethodNdx]))
							&&	(lDispIdAttribute->Value == DISPID_VALUE)
							&&	(lSourceMethods [lMethodNdx]->ReturnType)
							)
						{
							lEnumeratorType = lSourceMethods [lMethodNdx]->ReturnType;
#ifdef	_LOG_FIXES
							LogMessage (_LOG_FIXES, _T("--> Enum       [%s] in [%s]"), _BT(lEnumeratorType), _BT(pSourceType));
#endif
						}
					}
				}
			}

/*
				String^		lTypeName;
				lTypeName = lType->FullName;
				lTypeName = lTypeName->Substring (0, lTypeName->LastIndexOf (_T('.')));
				lTypeName = String::Concat (lTypeName, "._EnumerableClass`1[[", lEnumeratorType->FullName, "]]");
				LogMessage (LogDebug, _T("------ Enum [%s]"), _B(lTypeName));
*/
/*
			if	(lEnumeratorType)
			{
				array<Type^>^	lGenericArguments = gcnew array<Type^> (1);

				lGenericArguments[0] = mCopy->GetTargetArgumentType (lEnumeratorType);
				lBaseType = System::Collections::Generic::IEnumerator::typeid->MakeGenericType (lGenericArguments);
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("--> EnumBase   [%s] for [%s]"), _BT(lBaseType), _BT(pSourceType));
#endif
			}
*/
			if	(
					(lEnumeratorType)
				&&	(!mTargetEnumerable)
				)
			{
				String^			lTypeName ("_EnumerableClass");
				Type^			lSourceType;
				array<String^>^	lGenericParameters = gcnew array<String^> (1);

				lTypeName = String::Concat (mCopy->mModuleName, ".", lTypeName);

				if	(
						(lSourceType = _EnumerableClass::typeid)
					&&	(mTargetEnumerable = mInternalCopy->mModuleBuilder->DefineType (lTypeName, lSourceType->Attributes))
					)
				{
					lGenericParameters[0] = "T";
					mTargetEnumerable->SetParent (System::Object::typeid);
					mTargetEnumerable->DefineGenericParameters (lGenericParameters);

					mInternalCopy->CopyType (lSourceType, mTargetEnumerable);
					mInternalCopy->mAddedTypes->Add (mTargetEnumerable);
				}
			}

			if	(
					(lEnumeratorType)
				&&	(mTargetEnumerable)
				)
			{
				array<Type^>^	lGenericArguments = gcnew array<Type^> (1);

				lGenericArguments[0] = mCopy->GetTargetArgumentType (lEnumeratorType);
				lBaseType = mTargetEnumerable->MakeGenericType (lGenericArguments);
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("--> EnumBase   [%s] for [%s]"), _BT(lBaseType), _BT(pSourceType));
#endif
//				lBaseType = nullptr;
			}
		}
		catch AnyExceptionDebug
	}
	return lBaseType;
}
#endif

/////////////////////////////////////////////////////////////////////////////
//
//	If we assigned a base type then skip the inheritance of IEnumerable
//
#if	FALSE
bool FixupAssemblySpecific::FixEnumerableInterface (Type^ pSourceType, Type^ pSourceInterface, Type^& pTargetInterface)
{
	bool	lRet = false;
	if	(
			(mInternalCopy)
		&&	(pSourceType->IsInterface)
		&&	(pSourceType->IsImport)
		&&	(ReferenceEquals (pSourceInterface, System::Collections::IEnumerable::typeid))
		)
	{
		Type^	lEnumeratorType = nullptr;

		try
		{
			array<MethodInfo^>^		lSourceMethods;
			int						lMethodNdx;
			DispIdAttribute^		lDispIdAttribute;

			if	(lSourceMethods = pSourceType->GetMethods (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
			{
				for	(lMethodNdx = lSourceMethods->Length-1; lMethodNdx >= 0; lMethodNdx--)
				{
					if	(
							(lDispIdAttribute = mCopy->GetDispId (lSourceMethods [lMethodNdx]))
						&&	(lDispIdAttribute->Value == DISPID_NEWENUM)
						)
					{
						break;
					}
				}
			}
			if	(lMethodNdx >= 0)
			{
				for	(lMethodNdx = lSourceMethods->Length-1; lMethodNdx >= 0; lMethodNdx--)
				{
					if	(
							(lDispIdAttribute = mCopy->GetDispId (lSourceMethods [lMethodNdx]))
						&&	(lDispIdAttribute->Value == DISPID_VALUE)
						&&	(lSourceMethods [lMethodNdx]->ReturnType)
						&&	(!ReferenceEquals (lSourceMethods [lMethodNdx]->ReturnType, System::String::typeid))
						)
					{
						lEnumeratorType = lSourceMethods [lMethodNdx]->ReturnType;
#ifdef	_LOG_FIXES
						LogMessage (_LOG_FIXES, _T("--> Enum       [%s] in [%s]"), _BT(lEnumeratorType), _BT(pSourceType));
#endif
					}
				}
			}
		}
		catch AnyExceptionSilent

		if	(
				(lEnumeratorType)
			&&	(lEnumeratorType = mCopy->GetTargetArgumentType (lEnumeratorType))
			)
		{
			pTargetInterface = System::Collections::Generic::IEnumerable::typeid->MakeGenericType (lEnumeratorType);
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> Interface  [%s] to [%s] for [%s]"), _BT(pSourceInterface), _BT(pTargetInterface), _BT(pSourceType));
#endif
			if	(!mEnumerableTypes)
			{
				mEnumerableTypes = gcnew Dictionary <Type^, Type^>;
			}
			mEnumerableTypes [pSourceType] = lEnumeratorType;
		}
	}
	return lRet;
}
#endif	

/////////////////////////////////////////////////////////////////////////////

#if	FALSE	
bool FixupAssemblySpecific::FixEnumerableMethod (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes)
{
	bool				lRet = false;
	DispIdAttribute^	lDispIdAttribute;
	MethodInfo^			lMethodOverride;
	Type^				lEnumeratorType;

	if	(
			(mInternalCopy)
		&&	(pSourceMethod)
		&&	(pSourceMethod->DeclaringType->IsClass)
		&&	(pSourceMethod->ReturnType)
		&&	(ReferenceEquals (pSourceMethod->ReturnType, System::Collections::IEnumerator::typeid))
		&&	(lDispIdAttribute = mCopy->GetDispId (pSourceMethod))
		&&	(lDispIdAttribute->Value == DISPID_NEWENUM)
		&&	(lMethodOverride = mCopy->IsMethodOverride (pSourceMethod))
		)
	{
		if	(
				(mEnumerableTypes)
			&&	(
					(
						(mEnumerableTypes->ContainsKey (lMethodOverride->DeclaringType))
					&&	(lEnumeratorType = mEnumerableTypes [lMethodOverride->DeclaringType])
					)
				||	(
						(lMethodOverride = mCopy->IsMethodOverride (pSourceMethod, 2))
					&&	(mEnumerableTypes->ContainsKey (lMethodOverride->DeclaringType))
					&&	(lEnumeratorType = mEnumerableTypes [lMethodOverride->DeclaringType])
					)
				)
			)
		{
			if	(!mEnumeratorMethods)
			{
				mEnumeratorMethods = gcnew Dictionary <MethodInfo^, Type^>;
			}
			mEnumeratorMethods [pSourceMethod] = lEnumeratorType;
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> Enumerate  [%s.%s] as [%s]"), _BMT(pSourceMethod), _BM(pSourceMethod), _BT(pSourceMethod->ReturnType));
#endif
			lRet = true;
		}
	}
	return lRet;
}
#endif	

/////////////////////////////////////////////////////////////////////////////

void FixupAssemblySpecific::PreCopyTypes ()
{
return;
	if	(mInternalCopy)
	{
		try
		{
			KeyValuePair <Type^, Type^>^	lCopiedType;
			Dictionary <Type^, Type^>^		lCopiedTypes = gcnew Dictionary <Type^, Type^>;
			Type^							lEnumeratorType;

			mEnumerableTypes = gcnew Dictionary <Type^, Type^>;
			mEnumeratorTypes = gcnew Dictionary <Type^, Type^>;

			for each (lCopiedType in mCopy->mCopiedTypes)
			{
				lCopiedTypes->Add (lCopiedType->Key, lCopiedType->Value);
			}
			for each (lCopiedType in lCopiedTypes)
			{
				if	(lEnumeratorType = GetEnumeratorType (lCopiedType->Key))
				{
					mEnumerableTypes [lCopiedType->Key] = lEnumeratorType;
				}
			}
		
			if	(mEnumerableTypes->Count > 0)
			{	
				TypeBuilder^					lEnumeratorClass = nullptr;
				KeyValuePair <Type^, Type^>^	lEnumerableType;

				try
				{
					Type^	lSourceType = _EnumeratorClass::typeid;
					String^	lTypeName = String::Concat (mInternalCopy->mModuleName, ".", lSourceType->Name);

					try
					{
						lEnumeratorClass = mInternalCopy->mModuleBuilder->DefineType (lTypeName, lSourceType->Attributes);
					}
					catch AnyExceptionSilent

					if	(lEnumeratorClass)
					{
						mInternalCopy->CopyType (lSourceType, lEnumeratorClass);
						mInternalCopy->CopiedType (lSourceType, lEnumeratorClass);
#ifdef	_LOG_FIXES
						LogMessage (_LOG_FIXES, _T("--> Add        [%s] from [%s]"), _BT(lEnumeratorClass), _BT(lSourceType));
#endif
					}
				}
				catch AnyExceptionDebug
				
				if	(lEnumeratorClass)
				{
					for each (lEnumerableType in mEnumerableTypes)
					{
#ifdef	_LOG_FIXES_NOT
						LogMessage (_LOG_FIXES, _T("--> Make       [%s] for [%s]"), _BT(lEnumeratorClass), _BT(lEnumerableType->Value));
#endif
						mEnumeratorTypes [lEnumerableType->Key] = lEnumeratorClass->MakeGenericType (lEnumerableType->Value);
#ifdef	_LOG_FIXES
						LogMessage (_LOG_FIXES, _T("--> Make       [%s]"), _BT(mEnumeratorTypes [lEnumerableType->Key]));
#endif
					}
					
					mInternalCopy->CopyMethodBodies ();
					mInternalCopy->CreateTypes ();
					mInternalCopy->mCopiedMethods->Clear ();
					mInternalCopy->mCopiedConstructors->Clear ();
				}
			}
		}
		catch AnyExceptionDebug
	}
}

Type^ FixupAssemblySpecific::GetEnumeratorType (Type^ pSourceType)
{
	Type^	lEnumeratorType = nullptr;

	if	(
			(pSourceType->IsClass)
		&&	(pSourceType->IsImport)
		)
	{
		try
		{
			array<Type^>^			lSourceInterfaces;
			Type^					lSourceInterface;
			array<MethodInfo^>^		lSourceMethods;
			int						lMethodNdx;
			DispIdAttribute^		lDispIdAttribute;

			if	(lSourceInterfaces = pSourceType->GetInterfaces ())
			{
				for each (lSourceInterface in lSourceInterfaces)
				{
					if	(
							(ReferenceEquals (lSourceInterface, System::Collections::IEnumerable::typeid))
						&&	(mCopy->IsInterfaceInherited (pSourceType, lSourceInterface))
						)
					{
						break;
					}
					lSourceInterface = nullptr;
				}
			}

			if	(
					(lSourceInterface)
				&&	(lSourceMethods = pSourceType->GetMethods (BindingFlags::Instance|BindingFlags::Public|BindingFlags::NonPublic))
				)
			{
				for	(lMethodNdx = lSourceMethods->Length-1; lMethodNdx >= 0; lMethodNdx--)
				{
					if	(
							(lDispIdAttribute = mCopy->GetDispId (lSourceMethods [lMethodNdx]))
						&&	(lDispIdAttribute->Value == DISPID_NEWENUM)
						)
					{
						break;
					}
				}

				if	(lMethodNdx >= 0)
				{
					for	(lMethodNdx = lSourceMethods->Length-1; lMethodNdx >= 0; lMethodNdx--)
					{
						if	(
								(lDispIdAttribute = mCopy->GetDispId (lSourceMethods [lMethodNdx]))
							&&	(lDispIdAttribute->Value == DISPID_VALUE)
							&&	(lSourceMethods [lMethodNdx]->ReturnType)
							&&	(!ReferenceEquals (lSourceMethods [lMethodNdx]->ReturnType, System::String::typeid))
							)
						{
							lEnumeratorType = lSourceMethods [lMethodNdx]->ReturnType;
#ifdef	_LOG_FIXES
							LogMessage (_LOG_FIXES, _T("--> Enum       [%s] in [%s]"), _BT(lEnumeratorType), _BT(pSourceType));
#endif
						}
					}
				}
			}

			if	(
					(lEnumeratorType)
				&&	(lEnumeratorType = mCopy->GetTargetArgumentType (lEnumeratorType, false))
				)
			{
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("--> Enum       [%s] in [%s]"), _BT(lEnumeratorType), _BT(pSourceType));
#endif
			}
		}
		catch AnyExceptionSilent
	}
	return lEnumeratorType;
}

/////////////////////////////////////////////////////////////////////////////

bool FixupAssemblySpecific::FixEnumerableTarget (Type^ pSourceType, TypeBuilder^ pTargetType)
{
return false;
	bool	lRet = false;

	if	(
			(mEnumerableTypes)
		&&	(mEnumerableTypes->Count > 0)
		&&	(mEnumeratorTypes)
		&&	(mEnumeratorTypes->Count > 0)
		)
	{
		try
		{
			Type^	lEnumeratorType;
			Type^	lEnumeratorClass;
			
			if	(
					(mEnumerableTypes->TryGetValue (pSourceType, lEnumeratorType))
				&&	(mEnumeratorTypes->TryGetValue (pSourceType, lEnumeratorClass))
				&&	(false)
				)
			{
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("--> ????       [%s] [%s]"), _BT(lEnumeratorType), _BT(lEnumeratorClass));
#endif
				MethodInfo^			lSourceMethod;
				MethodInfo^			lTargetMethod;
				ConstructorInfo^ 	lSourceConstructor;
				ConstructorInfo^	lTargetConstructor;
				PropertyInfo^		lSourceProperty = nullptr;
				MethodInfo^			lSourceAccessor;
				MethodBuilder^		lTargetAccessor;
				Type^				lReturnType;

				lSourceConstructor = _EnumeratorClass::typeid->GetConstructors () [0];
				mCopy->TranslateConstructor (lSourceConstructor, lTargetConstructor);
//				lTargetConstructor = lEnumeratorClass->GetConstructors () [0];
				mInternalCopy->mTranslateConstructors [lSourceConstructor] = lTargetConstructor;
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("*** ctor       [%s.%s] [%8.8X] to [%s.%s]"), _BMT(lSourceConstructor), _BM(lSourceConstructor), lSourceConstructor->MetadataToken, _BMT(lTargetConstructor), _BM(lTargetConstructor));
#endif					
				lSourceMethod = pSourceType->GetMethod ("GetEnumerator");
				mCopy->TranslateMethod (lSourceMethod, lTargetMethod);
				lSourceMethod = _EnumerableClass::typeid->GetMethod ("GetEnumerator");
				mInternalCopy->mTranslateMethods [lSourceMethod] = lTargetMethod;
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("*** GetEnum    [%s.%s] [%8.8X] to [%s.%s]"), _BMT(lSourceMethod), _BM(lSourceMethod), lSourceMethod->MetadataToken, _BMT(lTargetMethod), _BM(lTargetMethod));
#endif					
//				lSourceProperty = _EnumerableClass::typeid->GetProperty ("List");
//				lSourceAccessor = lSourceProperty->GetGetMethod ();
//				lReturnType = System::Collections::Generic::IEnumerator::typeid;
//				lReturnType = lReturnType->MakeGenericType (lEnumeratorType);
				lSourceAccessor = _EnumerableClass::typeid->GetMethod ("get_List");
				lTargetAccessor = pTargetType->DefineMethod (lSourceAccessor->Name, (MethodAttributes)((int)lSourceAccessor->Attributes&~(int)MethodAttributes::SpecialName), lSourceAccessor->CallingConvention, lReturnType, gcnew array<Type^>(0));
//				lTargetAccessor->SetImplementationFlags (lSourceAccessor->GetMethodImplementationFlags ());
				lTargetAccessor->SetImplementationFlags ((MethodImplAttributes)((int)MethodImplAttributes::IL|(int)MethodImplAttributes::Managed|(int)MethodImplAttributes::InternalCall));
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("*** Method     [%s.%s] [%8.8X] to [%s.%s] Return [%s]"), _BMT(lSourceAccessor), _BM(lSourceAccessor), lSourceAccessor->MetadataToken, _BMT(lTargetAccessor), _BM(lTargetAccessor), _BT(lReturnType));
#endif			
				mInternalCopy->mTranslateTypes [_EnumeratorClass::typeid] = pTargetType;
				mInternalCopy->mTranslateTypes [System::Collections::Generic::IEnumerator::typeid] = lReturnType;

				//lTargetAccessor->CreateMethodBody (nullptr, 0);				
				mInternalCopy->CopyMethodBody (lSourceAccessor, lTargetAccessor);
			}
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

#if	FALSE
bool FixupAssemblySpecific::FixEnumerableTarget (Type^ pSourceType, TypeBuilder^ pTargetType)
{
	bool	lRet = false;

	if	(
			(mInternalCopy)
		&&	(pSourceType->IsClass)
		&&	(pSourceType->IsImport)
		)
	{
		Type^	lEnumeratorType = nullptr;

		try
		{
			array<Type^>^			lSourceInterfaces;
			Type^					lSourceInterface;
			array<MethodInfo^>^		lSourceMethods;
			int						lMethodNdx;
			DispIdAttribute^		lDispIdAttribute;

			if	(lSourceInterfaces = pSourceType->GetInterfaces ())
			{
				for each (lSourceInterface in lSourceInterfaces)
				{
					if	(
							(ReferenceEquals (lSourceInterface, System::Collections::IEnumerable::typeid))
						&&	(mCopy->IsInterfaceInherited (pSourceType, lSourceInterface))
						)
					{
						break;
					}
					lSourceInterface = nullptr;
				}
			}

			if	(
					(lSourceInterface)
				&&	(lSourceMethods = pSourceType->GetMethods (BindingFlags::Instance|BindingFlags::Public|BindingFlags::NonPublic))
				)
			{
				for	(lMethodNdx = lSourceMethods->Length-1; lMethodNdx >= 0; lMethodNdx--)
				{
					if	(
							(lDispIdAttribute = mCopy->GetDispId (lSourceMethods [lMethodNdx]))
						&&	(lDispIdAttribute->Value == DISPID_NEWENUM)
						)
					{
						break;
					}
				}

				if	(lMethodNdx >= 0)
				{
					for	(lMethodNdx = lSourceMethods->Length-1; lMethodNdx >= 0; lMethodNdx--)
					{
						if	(
								(lDispIdAttribute = mCopy->GetDispId (lSourceMethods [lMethodNdx]))
							&&	(lDispIdAttribute->Value == DISPID_VALUE)
							&&	(lSourceMethods [lMethodNdx]->ReturnType)
							&&	(!ReferenceEquals (lSourceMethods [lMethodNdx]->ReturnType, System::String::typeid))
							)
						{
							lEnumeratorType = lSourceMethods [lMethodNdx]->ReturnType;
#ifdef	_LOG_FIXES
							LogMessage (_LOG_FIXES, _T("--> Enum       [%s] in [%s]"), _BT(lEnumeratorType), _BT(pSourceType));
#endif
						}
					}
				}
			}
		}
		catch AnyExceptionSilent

		if	(
				(lEnumeratorType)
			&&	(lEnumeratorType = mCopy->GetTargetArgumentType (lEnumeratorType, false))
			)
		{
			try
			{
				Type^				lSourceType = _EnumerableClass::typeid;
				PropertyInfo^		lSourceProperty;
				MethodInfo^			lSourceMethod;
				MethodBuilder^		lTargetMethod;
				Type^				lReturnType;
				Type^				lEnumeratorClass;

				if	(!mInternalCopy->TranslateType (_EnumeratorClass::typeid, lEnumeratorClass))
				{
					AddInternalType (_EnumeratorClass::typeid);
					/**/mInternalCopy->CopyMethodBodies ();
				}
				
				if	(lEnumeratorClass = mInternalCopy->GetTargetType (_EnumeratorClass::typeid, false))
				{
					MethodInfo^			lSourceMethod;
					MethodInfo^			lTargetMethod;
					ConstructorInfo^ 	lSourceConstructor;
					ConstructorInfo^	lTargetConstructor;

					lEnumeratorClass = lEnumeratorClass->MakeGenericType (lEnumeratorType);
					mInternalCopy->mTranslateTypes [_EnumeratorClass::typeid] = lEnumeratorClass;
#ifdef	_LOG_FIXES
					LogMessage (_LOG_FIXES, _T("*** Type       [%s] [%8.8X] to [%s]"), _BT(_EnumeratorClass::typeid), _EnumeratorClass::typeid->MetadataToken, _BT(lEnumeratorClass));
#endif					
					lSourceConstructor = _EnumeratorClass::typeid->GetConstructors () [0];
					if	(mCopy->TranslateConstructor (lSourceConstructor, lTargetConstructor))
					{
						lTargetConstructor = lEnumeratorClass->GetConstructor (mCopy->GetParameterTypes (lTargetConstructor, false));
					}
					mInternalCopy->mTranslateConstructors [lSourceConstructor] = lTargetConstructor;
#ifdef	_LOG_FIXES
					LogMessage (_LOG_FIXES, _T("*** ctor       [%s.%s] [%8.8X] to [%s.%s]"), _BMT(lSourceConstructor), _BM(lSourceConstructor), lSourceConstructor->MetadataToken, _BMT(lTargetConstructor), _BM(lTargetConstructor));
#endif					
					lSourceMethod = pSourceType->GetMethod ("GetEnumerator");
					mCopy->TranslateMethod (lSourceMethod, lTargetMethod);
					lSourceMethod = lSourceType->GetMethod ("GetEnumerator");
					mInternalCopy->mTranslateMethods [lSourceMethod] = lTargetMethod;
#ifdef	_LOG_FIXES
					LogMessage (_LOG_FIXES, _T("*** GetEnum    [%s.%s] [%8.8X] to [%s.%s]"), _BMT(lSourceMethod), _BM(lSourceMethod), lSourceMethod->MetadataToken, _BMT(lTargetMethod), _BM(lTargetMethod));
#endif					
				}

				lSourceProperty = lSourceType->GetProperty ("List");
				lSourceMethod = lSourceProperty->GetGetMethod ();
				lReturnType = System::Collections::Generic::IEnumerator::typeid;
				lReturnType = lReturnType->MakeGenericType (lEnumeratorType);
				lTargetMethod = pTargetType->DefineMethod (lSourceMethod->Name, (MethodAttributes)((int)lSourceMethod->Attributes&~(int)MethodAttributes::SpecialName), lSourceMethod->CallingConvention, lReturnType, gcnew array<Type^>(0));
				lTargetMethod->SetImplementationFlags (lSourceMethod->GetMethodImplementationFlags ());
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("*** Method     [%s.%s] [%8.8X] to [%s.%s] Return [%s]"), _BMT(lSourceMethod), _BM(lSourceMethod), lSourceMethod->MetadataToken, _BMT(lTargetMethod), _BM(lTargetMethod), _BT(lReturnType));
#endif			
				mInternalCopy->mTranslateTypes [lSourceType] = pTargetType;
				mInternalCopy->mTranslateTypes [System::Collections::Generic::IEnumerator::typeid] = lReturnType;
				
				mInternalCopy->CopyMethodBody (lSourceMethod, lTargetMethod);

			}
			catch AnyExceptionDebug
		}
	}
#if	FALSE
	if	(
			(mInternalCopy)
		&&	(pSourceType)
		&&	(mEnumeratorMethods)
		)
	{
		MethodInfo^	lSourceMethod = nullptr;
		Type^		lEnumeratorType = nullptr;

		try
		{
			KeyValuePair <MethodInfo^, Type^>^	lEnumeratorMethod;

			for each (lEnumeratorMethod in mEnumeratorMethods)
			{
				if	(ReferenceEquals (lEnumeratorMethod->Key->DeclaringType, pSourceType))
				{
					lSourceMethod = lEnumeratorMethod->Key;
					lEnumeratorType = lEnumeratorMethod->Value;
					break;
				}
			}
		}
		catch AnyExceptionSilent

		if	(
				(lSourceMethod)
			&&	(lEnumeratorType)
			)
		{
			try
			{
				MethodInfo^			lEnumeratorMethod;
				MethodBuilder^		lMethodBuilder;
				String^				lMethodName = lSourceMethod->Name;
				MethodAttributes	lMethodAttrs = lSourceMethod->Attributes;
				Type^				lReturnType;

#ifdef	_LOG_FIXES_NOT
				LogMessage (_LOG_FIXES, _T("    EnumMethod [%s.%s] {%s]"), _BMT(lSourceMethod), _BM(lSourceMethod), _BT(lEnumeratorType));
#endif
				try
				{
					array<MethodInfo^>^	lEnumerableMethods;
					Type^				lEnumerableType;

					lEnumerableType = System::Collections::Generic::IEnumerable::typeid->MakeGenericType (lEnumeratorType);

					if	(lEnumerableMethods = System::Collections::Generic::IEnumerable::typeid->GetMethods (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
					{
						for each (lEnumeratorMethod in lEnumerableMethods)
						{
							if	(String::Compare (lEnumeratorMethod->Name, lSourceMethod->Name) == 0)
							{
								break;
							}
							lEnumeratorMethod = nullptr;
						}
					}
					if	(
							(lEnumeratorMethod)
						&&	(lEnumeratorMethod = pTargetType->GetMethod (lEnumerableType, lEnumeratorMethod))
						)
					{
#ifdef	_LOG_FIXES_NOT
						LogMessage (_LOG_FIXES, _T("    EnumMethod [%s.%s]"), _BMT(lEnumeratorMethod), _BM(lEnumeratorMethod));
#endif
					}
				}
				catch AnyExceptionDebug

				if	(
						(lEnumeratorMethod)
					&&	(lMethodBuilder = pTargetType->DefineMethod (lMethodName+"T", lMethodAttrs, lSourceMethod->CallingConvention))
					)
				{
					lReturnType = System::Collections::Generic::IEnumerator::typeid->MakeGenericType (lEnumeratorType);
					mCopy->CopyMethod (lSourceMethod, lMethodBuilder, lReturnType);
//					mCopy->CopyMethodAttributes (lSourceMethod, lMethodBuilder);
					pTargetType->DefineMethodOverride (lMethodBuilder, lEnumeratorMethod);
#ifdef	_LOG_FIXES
					LogMessage (_LOG_FIXES, _T("--> Enumerate  [%s.%s] as [%s]"), _BMT(lMethodBuilder), _BM(lMethodBuilder), _BT(lMethodBuilder->ReturnType));
#endif
					lRet = true;
				}
			}
			catch AnyExceptionDebug
		}
	}
#endif	
	return lRet;
}
#endif

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
