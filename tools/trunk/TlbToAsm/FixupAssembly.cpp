#include "stdafx.h"
#include <corhdr.h>
#include "FixupAssembly.h"

#define	_LOG_FIXES	LogNormal

namespace DoubleAgent {
/////////////////////////////////////////////////////////////////////////////

bool FixupAssembly::FixupType (Type^ pSourceType, TypeAttributes & pTypeAttributes)
{
	bool	lRet = CopyAssembly::FixupType (pSourceType, pTypeAttributes);

//
//	Skip NonCreatable classes.  The import is documented to do this, but it doesn't.
//
#if	TRUE
	if	(
			(pSourceType->IsClass)
		&&	(pSourceType->IsCOMObject)
		&&	(((int)GetTypeLibTypeFlags (pSourceType) & (int)TypeLibTypeFlags::FCanCreate) == 0)
		)
	{
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("--> Skip NonCreatable Class [%s]"), _B(pSourceType->FullName));
#endif
		lRet = true;
	}
#else
	if	(
			(pSourceType->IsClass)
		&&	(pSourceType->IsCOMObject)
		&&	(((int)GetTypeLibTypeFlags (pSourceType) & (int)TypeLibTypeFlags::FCanCreate) == 0)
		)
	{
		pTypeAttributes = (TypeAttributes) ((int)pTypeAttributes & ~(int)TypeAttributes::VisibilityMask | (int)TypeAttributes::NotPublic);
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("--> Class [%s] from [%s] to [%s]"), _B(pSourceType->FullName), _B(TypeAttrsStr(pSourceType->Attributes)), _B(TypeAttrsStr(pTypeAttributes)));
#endif
	}
#endif

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool FixupAssembly::FixupMethod (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes)
{
	bool					lRet = CopyAssembly::FixupMethod (pSourceMethod, pMethodAttributes);
	TypeLibFuncAttribute^	lFuncAttribute = nullptr;

	if	(pSourceMethod->DeclaringType->IsClass)
	{
		try
		{
			String^	lNamePrefix = nullptr;
			try
			{
				lNamePrefix = pSourceMethod->Name->Substring (0, pSourceMethod->Name->IndexOf ('_'));
			}
			catch AnyExceptionSilent

//
//	Hide the generated overrideable methods. We don't want to allow per-method overrides.
//
			if	(
					(lNamePrefix)
				&&	(pSourceMethod->DeclaringType->Name->IndexOf (lNamePrefix->Substring (1)) == 0)
				)
			{
				pMethodAttributes = (MethodAttributes) ((int)pMethodAttributes & ~(int)MethodAttributes::MemberAccessMask | (int)MethodAttributes::Private);
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("--> Method [%s] in [%s] from [%s] to [%s]"), _B(pSourceMethod->Name), _B(pSourceMethod->DeclaringType->FullName), _B(MethodAttrsStr(pSourceMethod->Attributes)), _B(MethodAttrsStr(pMethodAttributes)));
#endif
			}
		}
		catch AnyExceptionSilent
	}

	try
	{
		lFuncAttribute = safe_cast <TypeLibFuncAttribute^> (Attribute::GetCustomAttribute (pSourceMethod, TypeLibFuncAttribute(0).GetType()));
	}
	catch AnyExceptionSilent

//
//	Hide NonBrowsable methods (mostly deprecated methods) or make them Protected (applies to _NewEnum)
//
	if	(
			(lFuncAttribute)
		&&	((int)lFuncAttribute->Value & (int)TypeLibFuncFlags::FRestricted)
		&&	((int)lFuncAttribute->Value & (int)TypeLibFuncFlags::FNonBrowsable)
		)
	{
		if	((int)lFuncAttribute->Value & (int)TypeLibFuncFlags::FHidden)
		{
			pMethodAttributes = (MethodAttributes) ((int)pMethodAttributes & ~(int)MethodAttributes::MemberAccessMask | (int)MethodAttributes::Private);
		}
		else
		{
			pMethodAttributes = (MethodAttributes) ((int)pMethodAttributes & ~(int)MethodAttributes::MemberAccessMask | (int)MethodAttributes::Family);
		}
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("--> Method [%s] in [%s] from [%s] to [%s]"), _B(pSourceMethod->Name), _B(pSourceMethod->DeclaringType->FullName), _B(MethodAttrsStr(pSourceMethod->Attributes)), _B(MethodAttrsStr(pMethodAttributes)));
#endif
	}

//
//	Seal accessor method overrides on classes.
//
	if	(
			(!lRet)
		&&	(pSourceMethod->IsSpecialName)
		&&	(pSourceMethod->DeclaringType->IsClass)
		&&	(pSourceMethod->DeclaringType->IsCOMObject)
		)
	{
		pMethodAttributes = (MethodAttributes) ((int)pMethodAttributes | (int)MethodAttributes::Final);
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("--> Method [%s] in [%s] from [%s] to [%s]"), _B(pSourceMethod->Name), _B(pSourceMethod->DeclaringType->FullName), _B(MethodAttrsStr(pSourceMethod->Attributes)), _B(MethodAttrsStr(pMethodAttributes)));
#endif
	}

#if	FALSE
//
//	Protect hidden methods?
//
	if	(
			(!lRet)
		&&	(lFuncAttribute)
		&&	((int)lFuncAttribute->Value & (int)TypeLibFuncFlags::FHidden)
		)
	{
		pMethodAttributes = (MethodAttributes) ((int)pMethodAttributes & ~(int)MethodAttributes::MemberAccessMask | (int)MethodAttributes::Assembly);
	}
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool FixupAssembly::FixupCustomAttribute (Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues)
{
	bool	lRet = CopyAssembly::FixupCustomAttribute (pTarget, pAttribute, pAttributeValues);

	try
	{
		TypeBuilder^	lTypeBuilder;
		String^			lAttributeString;

		try
		{
			lTypeBuilder = safe_cast <TypeBuilder^> (pTarget);
			lAttributeString = pAttribute->ToString();
		}
		catch AnyExceptionSilent

		if	(
				(lTypeBuilder)
			&&	(!lTypeBuilder->IsClass)
			&&	(lAttributeString->IndexOf (INTEROP_COCLASS_TYPE) >= 0)
			&&	(pAttributeValues->Length == 1)
			&&	(String::Compare (pAttributeValues[0]->GetType()->ToString(), "System.RuntimeType", true) == 0)
			)
		{
//
//	Just remove this attribute to keep interfaces from showing up as classes in the object browser.
//	We want them to show up as interfaces, and the CoClassAttribute causes interfaces to show up as classes in VB.
//		
#if	FALSE		
			Type^	lValueType;

			if	(
					(lValueType = GetTargetType (pAttributeValues[0]->ToString(), false))
				&&	(lValueType->IsClass)
				&&	(lValueType->IsCOMObject)
				&&	(((int)GetTypeLibTypeFlags (lValueType) & (int)TypeLibTypeFlags::FCanCreate) == 0)
				)
#endif			
			{
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("--> Skip [%s] in [%s] for [%s]"), _B(lAttributeString), _B(lTypeBuilder->FullName), _B(pAttributeValues[0]->ToString()));
#endif
				lRet = true;
			}
#if	FALSE				
			else
			{
				Type^	lValueType;

				if	(lValueType = GetTargetType (pAttributeValues[0]->ToString(), true))
				{
#ifdef	_LOG_FIXES
					LogMessage (_LOG_FIXES, _T("--> Change [%s] to [%s] in [%s] for [%s]"), _B(pAttributeValues[0]->ToString()), _B(lValueType->FullName), _B(lAttributeString), _B(lTypeBuilder->FullName));
#endif
					pAttributeValues[0] = lValueType;
				}
			}
#endif				
		}

		if	(
				(!lRet)
			&&	(lTypeBuilder)
			&&	(
					(lAttributeString->IndexOf (INTEROP_COMSOURCEINTERFACES_TYPE) >= 0)
				||	(lAttributeString->IndexOf (INTEROP_COMEVENTINTERFACE_TYPE) >= 0)
				)
			)
		{
			int		lValueNdx;
			Type^	lValueType;

//
//	Translate references to copied types.
//		
			for	(lValueNdx = 0; lValueNdx < pAttributeValues->Length; lValueNdx++)
			{
				if	(String::Compare (pAttributeValues[lValueNdx]->GetType()->ToString(), "System.RuntimeType", true) == 0)
				{
					lValueType = GetTargetType (pAttributeValues[lValueNdx]->ToString(), true);
					if	(lValueType)
					{
#ifdef	_LOG_FIXES
						LogMessage (_LOG_FIXES, _T("--> Change [%s] to [%s] in [%s] for [%s]"), _B(pAttributeValues[lValueNdx]->ToString()), _B(lValueType->FullName), _B(lAttributeString), _B(lTypeBuilder->FullName));
#endif
						pAttributeValues[lValueNdx] = lValueType;
					}
				}
				else
				if	(String::Compare (pAttributeValues[lValueNdx]->GetType()->ToString(), "System.String", true) == 0)
				{
					lValueType = GetTargetType (pAttributeValues[lValueNdx]->ToString(), false);
					if	(lValueType)
					{
#ifdef	_LOG_FIXES
						LogMessage (_LOG_FIXES, _T("--> Change [%s] to [%s] in [%s] for [%s]"), _B(pAttributeValues[lValueNdx]->ToString()), _B(lValueType->FullName), _B(lAttributeString), _B(lTypeBuilder->FullName));
#endif
						pAttributeValues[lValueNdx] = lValueType->FullName;
					}
				}
			}
		}
	}
	catch AnyExceptionDebug

#if	FALSE
//
//	This logic was to unhide the GetEnumerator method, but if it's only marked as Restricted and not Hidden in the ODL, that's enough.
//		
	try
	{
		MethodBuilder^	lMethodBuilder;
		String^			lAttributeString;

		try
		{
			lMethodBuilder = safe_cast <MethodBuilder^> (pTarget);
			lAttributeString = pAttribute->ToString();
		}
		catch AnyExceptionSilent

		if	(
				(lMethodBuilder)
			&&	(lAttributeString->IndexOf (INTEROP_TYPELIBFUNC_TYPE) >= 0)
			&&	(pAttribute->ConstructorArguments->Count > 0)
			&&	(String::Compare (lMethodBuilder->Name, "GetEnumerator", true) == 0)
			)
		{
#ifdef	_LOG_FIXES
			int	lTypeLibFuncFlags = (Int16) pAttribute->ConstructorArguments [0].Value;
			LogMessage (_LOG_FIXES, _T("--> Skip [%s] for [%s]"), _B(TypeLibFuncFlagsStr((TypeLibFuncFlags)lTypeLibFuncFlags)), _B(lMethodBuilder->Name));
#endif
			lRet = true;
		}
	}
	catch AnyExceptionDebug
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void FixupAssembly::FixupCustomAttributes (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes)
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
		&&	(!lSourceType->IsClass)
		)
	{
//
//	Hide deprecated CoClass interfaces
//	
		try
		{
			CoClassAttribute^	lCoClassAttribute;
			array <Type^>^		lTypeLibTypeArgs = gcnew array <Type^> (1);
			array <Object^>^	lTypeLibTypeFlags = gcnew array <Object^> (1);

			lCoClassAttribute = safe_cast <CoClassAttribute^> (Attribute::GetCustomAttribute (lSourceType, CoClassAttribute(nullptr).GetType()));
			if	(lCoClassAttribute)
			{
				if	((int)GetTypeLibTypeFlags (lCoClassAttribute->CoClass) & (int)TypeLibTypeFlags::FRestricted)
				{
#ifdef	_LOG_FIXES
					LogMessage (_LOG_FIXES, _T("--> Hide [%s] for Restricted [%s]"), _B(lTargetType->FullName), _B(lCoClassAttribute->CoClass->FullName));
#endif
					lTypeLibTypeArgs [0] = TypeLibTypeFlags::typeid;
					lTypeLibTypeFlags [0] = TypeLibTypeFlags::FHidden;
					pCustomAttributes->Add (gcnew CustomAttributeBuilder (TypeLibTypeAttribute::typeid->GetConstructor(lTypeLibTypeArgs), lTypeLibTypeFlags));
				}
			}
		}
		catch AnyExceptionSilent
		{}
	}
}

/////////////////////////////////////////////////////////////////////////////

bool FixupAssembly::FixupEnum (Type^ pSourceType, TypeAttributes & pTypeAttributes)
{
	return CopyAssembly::FixupEnum (pSourceType, pTypeAttributes);
}

bool FixupAssembly::FixupProperty (PropertyInfo^ pSourceProperty, Reflection::PropertyAttributes & pPropertyAttributes)
{
	return CopyAssembly::FixupProperty (pSourceProperty, pPropertyAttributes);
}

bool FixupAssembly::FixupEvent (EventInfo^ pSourceEvent, EventAttributes & pEventAttributes)
{
	return CopyAssembly::FixupEvent (pSourceEvent, pEventAttributes);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

TypeLibTypeFlags FixupAssembly::GetTypeLibTypeFlags (Type^ pType)
{
	TypeLibTypeFlags	lRet = (TypeLibTypeFlags)0;

	try
	{
		TypeLibTypeAttribute^	lTypeLibTypeAttribute;

		lTypeLibTypeAttribute = safe_cast <TypeLibTypeAttribute^> (Attribute::GetCustomAttribute (pType, TypeLibTypeAttribute(0).GetType()));
		if	(lTypeLibTypeAttribute)
		{
			lRet = lTypeLibTypeAttribute->Value;
		}
	}
	catch AnyExceptionSilent
	{}

	return lRet;
}

TypeLibTypeFlags FixupAssembly::GetTypeLibTypeFlags (String^ pTypeName)
{
	TypeLibTypeFlags			lRet = (TypeLibTypeFlags)0;
	KeyValuePair<Type^, Type^>^	lPair;

	for each (lPair in mCopiedTypes)
	{
		if	(String::Compare (lPair->Key->FullName, pTypeName, true) == 0)
		{
			lRet = GetTypeLibTypeFlags (lPair->Key);
			break;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
};