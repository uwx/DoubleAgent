/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent Server.

    The Double Agent Server is free software:
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser Public License for more details.

    You should have received a copy of the GNU Lesser Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include <comdef.h>
#include <comutil.h>
#include "DaControlOdl.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef	_DACTLPTR_ALLOWNULL
#define	_DACTLPTR_CHKNULL	true
#else
#define	_DACTLPTR_CHKNULL	operator bool()
#endif

/////////////////////////////////////////////////////////////////////////////

#define	_DACTLPTR_DECLARE(i) class i##Ptr : public _COM_SMARTPTR<_COM_SMARTPTR_LEVEL2<i##, &__uuidof(i##)> >

#define	_DACTLPTR_CONSTRUCT(i) \
	i##Ptr () {}; \
	i##Ptr (const i##Ptr& p) : _com_ptr_t<ThisIIID> (p) {} \
	i##Ptr (i##* p) : _com_ptr_t<ThisIIID> (p) {} \
	i##Ptr (const _variant_t& varSrc) : _com_ptr_t<ThisIIID> (varSrc) {} \
	template<typename _OtherIID> i##Ptr (const _com_ptr_t<_OtherIID>& p) : _com_ptr_t<ThisIIID> (p) {} \
	template<typename _InterfaceType> i##Ptr (_InterfaceType* p) : _com_ptr_t<ThisIIID> (p) {}

#define	_DACTLPTR_ASSIGN(i) \
	i##Ptr& operator= (const i##Ptr& p) {_com_ptr_t<ThisIIID>::operator= (p); return *this;} \
	i##Ptr& operator= (i##* p) {_com_ptr_t<ThisIIID>::operator= (p); return *this;} \
	i##Ptr& operator= (const _variant_t& varSrc) {_com_ptr_t<ThisIIID>::operator= (varSrc); return *this;} \
	template<typename _OtherIID> i##Ptr& operator= (const _com_ptr_t<_OtherIID>& p) {_com_ptr_t<ThisIIID>::operator= (p); return *this;} \
	template<typename _InterfaceType> i##Ptr& operator= (_InterfaceType* p) {_com_ptr_t<ThisIIID>::operator= (p); return *this;}

__if_exists(IFontDisp)
{
	_COM_SMARTPTR_TYPEDEF(IFontDisp, __uuidof(IFontDisp));
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaCtlBalloon2Ptr)
{
	_DACTLPTR_DECLARE(IDaCtlBalloon2)
	{
	public:
		_DACTLPTR_CONSTRUCT(IDaCtlBalloon2)
		_DACTLPTR_ASSIGN(IDaCtlBalloon2)

		__declspec(property(get=_get_Enabled))															bool					Enabled;
		__declspec(property(get=_get_Style, put=_put_Style))											long					Style;
		__declspec(property(get=_get_Visible, put=_put_Visible))										bool					Visible;
		__declspec(property(get=_get_NumberOfLines, put=_put_NumberOfLines))							long					NumberOfLines;
		__declspec(property(get=_get_CharsPerLine, put=_put_CharsPerLine))								long					CharsPerLine;
		__declspec(property(get=_get_TextColor, put=_put_TextColor))									OLE_COLOR				TextColor;
		__declspec(property(get=_get_BackColor, put=_put_BackColor))									OLE_COLOR				BackColor;
		__declspec(property(get=_get_BorderColor, put=_put_BorderColor))								OLE_COLOR				BorderColor;
		__declspec(property(get=_get_FontName, put=_put_FontName))										_bstr_t					FontName;
		__declspec(property(get=_get_FontSize, put=_put_FontSize))										long					FontSize;
		__declspec(property(get=_get_FontBold, put=_put_FontBold))										bool					FontBold;
		__declspec(property(get=_get_FontItalic, put=_put_FontItalic))									bool					FontItalic;
		__declspec(property(get=_get_FontStrikethru, put=_put_FontStrikethru))							bool					FontStrikethru;
		__declspec(property(get=_get_FontUnderline, put=_put_FontUnderline))							bool					FontUnderline;
		__declspec(property(get=_get_FontCharSet, put=_put_FontCharSet))								short					FontCharSet;
		__declspec(property(get=_get_SizeToText, put=_put_SizeToText))									bool					SizeToText;
		__declspec(property(get=_get_AutoHide, put=_put_AutoHide))										bool					AutoHide;
		__declspec(property(get=_get_AutoPace, put=_put_AutoPace))										bool					AutoPace;
		__declspec(property(get=_get_NoAppend, put=_put_NoAppend))										bool					NoAppend;
		__declspec(property(get=_get_ShowPartialLines, put=_put_ShowPartialLines))						bool					ShowPartialLines;

		bool _get_Enabled () {VARIANT_BOOL lEnabled=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Enabled(&lEnabled); return lEnabled?true:false;}
		long _get_Style () {long lStyle=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Style(&lStyle); return lStyle;}
		HRESULT _put_Style (long Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_Style(Value); else return E_POINTER;}
		bool _get_Visible () {VARIANT_BOOL lVisible=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Visible(&lVisible); return lVisible?true:false;}
		HRESULT _put_Visible (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_Visible(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		long _get_NumberOfLines () {long lNumberOfLines=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_NumberOfLines(&lNumberOfLines); return lNumberOfLines;}
		HRESULT _put_NumberOfLines (long Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_NumberOfLines(Value); else return E_POINTER;}
		long _get_CharsPerLine () {long lCharsPerLine=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_CharsPerLine(&lCharsPerLine); return lCharsPerLine;}
		HRESULT _put_CharsPerLine (long Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_CharsPerLine(Value); else return E_POINTER;}
		OLE_COLOR _get_TextColor () {OLE_COLOR lTextColor=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_TextColor(&lTextColor); return lTextColor;}
		HRESULT _put_TextColor (OLE_COLOR Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_TextColor(Value); else return E_POINTER;}
		OLE_COLOR _get_BackColor () {OLE_COLOR lBackColor=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_BackColor(&lBackColor); return lBackColor;}
		HRESULT _put_BackColor (OLE_COLOR Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_BackColor(Value); else return E_POINTER;}
		OLE_COLOR _get_BorderColor () {OLE_COLOR lBorderColor=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_BorderColor(&lBorderColor); return lBorderColor;}
		HRESULT _put_BorderColor (OLE_COLOR Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_BorderColor(Value); else return E_POINTER;}
		_bstr_t _get_FontName () {BSTR lFontName=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_FontName(&lFontName); return _bstr_t(lFontName, false);}
		HRESULT _put_FontName (const _bstr_t& Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_FontName(Value); else return E_POINTER;}
		long _get_FontSize () {long lFontSize=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_FontSize(&lFontSize); return lFontSize;}
		HRESULT _put_FontSize (long Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_FontSize(Value); else return E_POINTER;}
		bool _get_FontBold () {VARIANT_BOOL lFontBold=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_FontBold(&lFontBold); return lFontBold?true:false;}
		HRESULT _put_FontBold (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_FontBold(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_FontItalic () {VARIANT_BOOL lFontItalic=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_FontItalic(&lFontItalic); return lFontItalic?true:false;}
		HRESULT _put_FontItalic (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_FontItalic(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_FontStrikethru () {VARIANT_BOOL lFontStrikethru=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_FontStrikethru(&lFontStrikethru); return lFontStrikethru?true:false;}
		HRESULT _put_FontStrikethru (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_FontStrikethru(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_FontUnderline () {VARIANT_BOOL lFontUnderline=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_FontUnderline(&lFontUnderline); return lFontUnderline?true:false;}
		HRESULT _put_FontUnderline (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_FontUnderline(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		short _get_FontCharSet () {short lFontCharSet=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_FontCharSet(&lFontCharSet); return lFontCharSet;}
		HRESULT _put_FontCharSet (short Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_FontCharSet(Value); else return E_POINTER;}
		bool _get_SizeToText () {VARIANT_BOOL lSizeToText=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_SizeToText(&lSizeToText); return lSizeToText?true:false;}
		HRESULT _put_SizeToText (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_SizeToText(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_AutoHide () {VARIANT_BOOL lAutoHide=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_AutoHide(&lAutoHide); return lAutoHide?true:false;}
		HRESULT _put_AutoHide (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_AutoHide(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_AutoPace () {VARIANT_BOOL lAutoPace=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_AutoPace(&lAutoPace); return lAutoPace?true:false;}
		HRESULT _put_AutoPace (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_AutoPace(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_NoAppend () {VARIANT_BOOL lNoAppend=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_NoAppend(&lNoAppend); return lNoAppend?true:false;}
		HRESULT _put_NoAppend (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_NoAppend(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_ShowPartialLines () {VARIANT_BOOL lShowPartialLines=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_ShowPartialLines(&lShowPartialLines); return lShowPartialLines?true:false;}
		HRESULT _put_ShowPartialLines (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_ShowPartialLines(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaCtlCommand2Ptr)
{
	_DACTLPTR_DECLARE(IDaCtlCommand2)
	{
	public:
		_DACTLPTR_CONSTRUCT(IDaCtlCommand2)
		_DACTLPTR_ASSIGN(IDaCtlCommand2)

		__declspec(property(get=_get_Caption, put=_put_Caption))										_bstr_t					Caption;
		__declspec(property(get=_get_Enabled, put=_put_Enabled))										bool					Enabled;
		__declspec(property(get=_get_Visible, put=_put_Visible))										bool					Visible;
		__declspec(property(get=_get_Name))																_bstr_t					Name;
		__declspec(property(get=_get_VoiceCaption, put=_put_VoiceCaption))								_bstr_t					VoiceCaption;
		__declspec(property(get=_get_VoiceGrammar, put=_put_VoiceGrammar))								_bstr_t					VoiceGrammar;
		__declspec(property(get=_get_ConfidenceThreshold, put=_put_ConfidenceThreshold))				long					ConfidenceThreshold;
		__declspec(property(get=_get_ConfidenceText, put=_put_ConfidenceText))							_bstr_t					ConfidenceText;

		_bstr_t _get_Caption () {BSTR lCaption=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Caption(&lCaption); return _bstr_t(lCaption, false);}
		HRESULT _put_Caption (const _bstr_t& Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_Caption(Value); else return E_POINTER;}
		bool _get_Enabled () {VARIANT_BOOL lEnabled=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Enabled(&lEnabled); return lEnabled?true:false;}
		HRESULT _put_Enabled (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_Enabled(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_Visible () {VARIANT_BOOL lVisible=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Visible(&lVisible); return lVisible?true:false;}
		HRESULT _put_Visible (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_Visible(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		_bstr_t _get_Name () {BSTR lName=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Name(&lName); return _bstr_t(lName, false);}
		_bstr_t _get_VoiceCaption () {BSTR lVoiceCaption=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_VoiceCaption(&lVoiceCaption); return _bstr_t(lVoiceCaption, false);}
		HRESULT _put_VoiceCaption (const _bstr_t& Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_VoiceCaption(Value); else return E_POINTER;}
		_bstr_t _get_VoiceGrammar () {BSTR lVoiceGrammar=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_VoiceGrammar(&lVoiceGrammar); return _bstr_t(lVoiceGrammar, false);}
		HRESULT _put_VoiceGrammar (const _bstr_t& Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_VoiceGrammar(Value); else return E_POINTER;}
		long _get_ConfidenceThreshold () {long lConfidenceThreshold=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_ConfidenceThreshold(&lConfidenceThreshold); return lConfidenceThreshold;}
		HRESULT _put_ConfidenceThreshold (long Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_ConfidenceThreshold(Value); else return E_POINTER;}
		_bstr_t _get_ConfidenceText () {BSTR lConfidenceText=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_ConfidenceText(&lConfidenceText); return _bstr_t(lConfidenceText, false);}
		HRESULT _put_ConfidenceText (const _bstr_t& Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_ConfidenceText(Value); else return E_POINTER;}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaCtlCommands2Ptr)
{
	_DACTLPTR_DECLARE(IDaCtlCommands2)
	{
	public:
		_DACTLPTR_CONSTRUCT(IDaCtlCommands2)
		_DACTLPTR_ASSIGN(IDaCtlCommands2)

		__declspec(property(get=_get_Count))															long					Count;
		__declspec(property(get=_get_Item))																IDaCtlCommand2Ptr		Item[];
		__declspec(property(get=_get_Index))															IDaCtlCommand2Ptr		Index[];
		__declspec(property(get=_get_Caption, put=_put_Caption))										_bstr_t					Caption;
		__declspec(property(get=_get_VoiceGrammar, put=_put_VoiceGrammar))								_bstr_t					VoiceGrammar;
		__declspec(property(get=_get_Visible, put=_put_Visible))										bool					Visible;
		__declspec(property(get=_get_DefaultCommand, put=_put_DefaultCommand))							_bstr_t					DefaultCommand;
		__declspec(property(get=_get_FontName, put=_put_FontName))										_bstr_t					FontName;
		__declspec(property(get=_get_FontSize, put=_put_FontSize))										long					FontSize;
		__declspec(property(get=_get_VoiceCaption, put=_put_VoiceCaption))								_bstr_t					VoiceCaption;
		__declspec(property(get=_get_GlobalVoiceCommandsEnabled, put=_put_GlobalVoiceCommandsEnabled))	bool					GlobalVoiceCommandsEnabled;

		long _get_Count () {long lCount=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Count(&lCount); return lCount;}
		IDaCtlCommand2Ptr _get_Item (BSTR Name) {IDaCtlCommand2Ptr lCommand; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Item(Name, &lCommand); return lCommand;}
		IDaCtlCommand2Ptr _get_Index (long Index) {IDaCtlCommand2Ptr lIndex; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Index(Index, &lIndex); return lIndex;}
		_bstr_t _get_Caption () {BSTR lCaption=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Caption(&lCaption); return _bstr_t(lCaption, false);}
		HRESULT _put_Caption (const _bstr_t& Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_Caption(Value); else return E_POINTER;}
		_bstr_t _get_VoiceGrammar () {BSTR lVoiceGrammar=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_VoiceGrammar(&lVoiceGrammar); return _bstr_t(lVoiceGrammar, false);}
		HRESULT _put_VoiceGrammar (const _bstr_t& Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_VoiceGrammar(Value); else return E_POINTER;}
		bool _get_Visible () {VARIANT_BOOL lVisible=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Visible(&lVisible); return lVisible?true:false;}
		HRESULT _put_Visible (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_Visible(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		_bstr_t _get_DefaultCommand () {BSTR lDefaultCommand=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_DefaultCommand(&lDefaultCommand); return _bstr_t(lDefaultCommand, false);}
		HRESULT _put_DefaultCommand (BSTR Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_DefaultCommand(Value); else return E_POINTER;}
		_bstr_t _get_FontName () {BSTR lFontName=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_FontName(&lFontName); return _bstr_t(lFontName, false);}
		HRESULT _put_FontName (const _bstr_t& Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_FontName(Value); else return E_POINTER;}
		long _get_FontSize () {long lFontSize=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_FontSize(&lFontSize); return lFontSize;}
		HRESULT _put_FontSize (long Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_FontSize(Value); else return E_POINTER;}
		_bstr_t _get_VoiceCaption () {BSTR lVoiceCaption=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_VoiceCaption(&lVoiceCaption); return _bstr_t(lVoiceCaption, false);}
		HRESULT _put_VoiceCaption (const _bstr_t& Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_VoiceCaption(Value); else return E_POINTER;}
		bool _get_GlobalVoiceCommandsEnabled () {VARIANT_BOOL lGlobalVoiceCommandsEnabled=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_GlobalVoiceCommandsEnabled(&lGlobalVoiceCommandsEnabled); return lGlobalVoiceCommandsEnabled?true:false;}
		HRESULT _put_GlobalVoiceCommandsEnabled (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_GlobalVoiceCommandsEnabled(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaCtlUserInputPtr)
{
	_DACTLPTR_DECLARE(IDaCtlUserInput)
	{
	public:
		_DACTLPTR_CONSTRUCT(IDaCtlUserInput)
		_DACTLPTR_ASSIGN(IDaCtlUserInput)

		__declspec(property(get=_get_Count))									short					Count;
		__declspec(property(get=_get_CharacterID))								_bstr_t					CharacterID;
		__declspec(property(get=_get_Name))										_bstr_t					Name;
		__declspec(property(get=_get_Alt1Name))									_bstr_t					Alt1Name;
		__declspec(property(get=_get_Alt2Name))									_bstr_t					Alt2Name;
		__declspec(property(get=_get_Confidence))								long					Confidence;
		__declspec(property(get=_get_Alt1Confidence))							long					Alt1Confidence;
		__declspec(property(get=_get_Alt2Confidence))							long					Alt2Confidence;
		__declspec(property(get=_get_Voice))									_bstr_t					Voice;
		__declspec(property(get=_get_Alt1Voice))								_bstr_t					Alt1Voice;
		__declspec(property(get=_get_Alt2Voice))								_bstr_t					Alt2Voice;

		short _get_Count () {short lCount=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Count(&lCount); return lCount;}
		_bstr_t _get_CharacterID () {BSTR lCharacterID=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_CharacterID(&lCharacterID); return _bstr_t(lCharacterID, false);}
		_bstr_t _get_Name () {BSTR lName=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Name(&lName); return _bstr_t(lName, false);}
		_bstr_t _get_Alt1Name () {BSTR lName=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Alt1Name(&lName); return _bstr_t(lName, false);}
		_bstr_t _get_Alt2Name () {BSTR lName=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Alt2Name(&lName); return _bstr_t(lName, false);}
		long _get_Confidence () {long lConfidence=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Confidence(&lConfidence); return lConfidence;}
		long _get_Alt1Confidence () {long lConfidence=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Alt1Confidence(&lConfidence); return lConfidence;}
		long _get_Alt2Confidence () {long lConfidence=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Alt2Confidence(&lConfidence); return lConfidence;}
		_bstr_t _get_Voice () {BSTR lVoice=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Voice(&lVoice); return _bstr_t(lVoice, false);}
		_bstr_t _get_Alt1Voice () {BSTR lVoice=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Alt1Voice(&lVoice); return _bstr_t(lVoice, false);}
		_bstr_t _get_Alt2Voice () {BSTR lVoice=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Alt2Voice(&lVoice); return _bstr_t(lVoice, false);}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaCtlCommandsWindowPtr)
{
	_DACTLPTR_DECLARE(IDaCtlCommandsWindow)
	{
	public:
		_DACTLPTR_CONSTRUCT(IDaCtlCommandsWindow)
		_DACTLPTR_ASSIGN(IDaCtlCommandsWindow)

		__declspec(property(get=_get_Visible, put=_put_Visible))				bool					Visible;
		__declspec(property(get=_get_Left))										short					Left;
		__declspec(property(get=_get_Top))										short					Top;
		__declspec(property(get=_get_Width))									short					Width;
		__declspec(property(get=_get_Height))									short					Height;

		bool _get_Visible () {VARIANT_BOOL lVisible=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Visible(&lVisible); return lVisible?true:false;}
		HRESULT _put_Visible (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_Visible(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		short _get_Left () {short lLeft=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Left(&lLeft); return lLeft;}
		short _get_Top () {short lTop=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Top(&lTop); return lTop;}
		short _get_Width () {short lWidth=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Width(&lWidth); return lWidth;}
		short _get_Height () {short lHeight=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Height(&lHeight); return lHeight;}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaCtlPropertySheet2Ptr)
{
	_DACTLPTR_DECLARE(IDaCtlPropertySheet2)
	{
	public:
		_DACTLPTR_CONSTRUCT(IDaCtlPropertySheet2)
		_DACTLPTR_ASSIGN(IDaCtlPropertySheet2)

		__declspec(property(get=_get_Visible, put=_put_Visible))				bool					Visible;
		__declspec(property(get=_get_Page, put=_put_Page))						_bstr_t					Page;
		__declspec(property(get=_get_Left))										short					Left;
		__declspec(property(get=_get_Top))										short					Top;
		__declspec(property(get=_get_Width))									short					Width;
		__declspec(property(get=_get_Height))									short					Height;

		bool _get_Visible () {VARIANT_BOOL lVisible=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Visible(&lVisible); return lVisible?true:false;}
		HRESULT _put_Visible (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_Visible(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		_bstr_t _get_Page () {BSTR lPage=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Page(&lPage); return _bstr_t(lPage, false);}
		HRESULT _put_Page (const _bstr_t& Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_Page(Value); else return E_POINTER;}
		short _get_Left () {short lLeft=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Left(&lLeft); return lLeft;}
		short _get_Top () {short lTop=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Top(&lTop); return lTop;}
		short _get_Width () {short lWidth=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Width(&lWidth); return lWidth;}
		short _get_Height () {short lHeight=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Height(&lHeight); return lHeight;}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaCtlSettingsPtr)
{
	_DACTLPTR_DECLARE(IDaCtlSettings)
	{
	public:
		_DACTLPTR_CONSTRUCT(IDaCtlSettings)
		_DACTLPTR_ASSIGN(IDaCtlSettings)

		__declspec(property(get=_get_SoundEffectsEnabled))						bool					SoundEffectsEnabled;
		__declspec(property(get=_get_BalloonEnabled))							bool					BalloonEnabled;
		__declspec(property(get=_get_TTSEnabled))								bool					TTSEnabled;
		__declspec(property(get=_get_TTSSpeed))									short					TTSSpeed;
		__declspec(property(get=_get_SREnabled))								bool					SREnabled;
		__declspec(property(get=_get_SRHotKey))									_bstr_t					SRHotKey;
		__declspec(property(get=_get_SRHotKeyTime))								short					SRHotKeyTime;
		__declspec(property(get=_get_SRListeningTip))							bool					SRListeningTip;
		__declspec(property(get=_get_SRListeningPrompt))						bool					SRListeningPrompt;
		__declspec(property(get=_get_AudioStatus))								AudioStatusType			AudioStatus;

		bool _get_SoundEffectsEnabled () {VARIANT_BOOL lSoundEffectsEnabled=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_SoundEffectsEnabled(&lSoundEffectsEnabled); return lSoundEffectsEnabled?true:false;}
		bool _get_BalloonEnabled () {VARIANT_BOOL lBalloonEnabled=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_BalloonEnabled(&lBalloonEnabled); return lBalloonEnabled?true:false;}
		bool _get_TTSEnabled () {VARIANT_BOOL lTTSEnabled=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_TTSEnabled(&lTTSEnabled); return lTTSEnabled?true:false;}
		short _get_TTSSpeed () {short lTTSSpeed=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_TTSSpeed(&lTTSSpeed); return lTTSSpeed;}
		bool _get_SREnabled () {VARIANT_BOOL lSREnabled=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_SREnabled(&lSREnabled); return lSREnabled?true:false;}
		_bstr_t _get_SRHotKey () {BSTR lSRHotKey=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_SRHotKey(&lSRHotKey); return _bstr_t(lSRHotKey, false);}
		short _get_SRHotKeyTime () {short lSRHotKeyTime=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_SRHotKeyTime(&lSRHotKeyTime); return lSRHotKeyTime;}
		bool _get_SRListeningTip () {VARIANT_BOOL lSRListeningTip=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_SRListeningTip(&lSRListeningTip); return lSRListeningTip?true:false;}
		bool _get_SRListeningPrompt () {VARIANT_BOOL lSRListeningPrompt=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_SRListeningPrompt(&lSRListeningPrompt); return lSRListeningPrompt?true:false;}
		AudioStatusType _get_AudioStatus () {AudioStatusType lAudioStatus=AudioStatus_Error; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_AudioStatus(&lAudioStatus); return lAudioStatus;}

		__if_exists(IFontDisp)
		{
			__declspec(property(get=_get_BalloonFont))							IFontDispPtr			BalloonFont;
			IFontDispPtr _get_BalloonFont () {IFontDispPtr lBalloonFont; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_BalloonFont(&lBalloonFont); return lBalloonFont;}
		}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaCtlCharacterFilesPtr)
{
	_DACTLPTR_DECLARE(IDaCtlCharacterFiles)
	{
	public:
		_DACTLPTR_CONSTRUCT(IDaCtlCharacterFiles)
		_DACTLPTR_ASSIGN(IDaCtlCharacterFiles)

		__declspec(property(get=_get_FilePaths))														SAFEARRAY*				FilePaths;
		__declspec(property(get=_get_SearchPath, put=_put_SearchPath))									_bstr_t					SearchPath;
		__declspec(property(get=_get_DefaultSearchPath))												_bstr_t					DefaultSearchPath;
		__declspec(property(get=_get_DoubleAgentFiles, put=_put_DoubleAgentFiles))						bool					DoubleAgentFiles;
		__declspec(property(get=_get_MsAgentFiles, put=_put_MsAgentFiles))								bool					MsAgentFiles;
		__declspec(property(get=_get_MsOfficeFiles, put=_put_MsOfficeFiles))							bool					MsOfficeFiles;
		__declspec(property(get=_get_CompliantCharacters, put=_put_CompliantCharacters))				bool					CompliantCharacters;
		__declspec(property(get=_get_NonCompliantCharacters, put=_put_NonCompliantCharacters))			bool					NonCompliantCharacters;
		__declspec(property(get=_get_SpeakingCharacters, put=_put_SpeakingCharacters))					bool					SpeakingCharacters;
		__declspec(property(get=_get_NonSpeakingCharacters, put=_put_NonSpeakingCharacters))			bool					NonSpeakingCharacters;
		__declspec(property(get=_get_VerifyVersion, put=_put_VerifyVersion))							bool					VerifyVersion;
		__declspec(property(get=_get_DefaultFilePath))													_bstr_t					DefaultFilePath;
		__declspec(property(get=_get_DefaultFileName))													_bstr_t					DefaultFileName;

		SAFEARRAY* _get_FilePaths () {SAFEARRAY* lFilePaths=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_FilePaths(&lFilePaths); return lFilePaths;}
		_bstr_t _get_SearchPath () {BSTR lSearchPath=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_SearchPath(&lSearchPath); return _bstr_t(lSearchPath, false);}
		HRESULT _put_SearchPath (const _bstr_t& Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_SearchPath(Value); else return E_POINTER;}
		_bstr_t _get_DefaultSearchPath () {BSTR lDefaultSearchPath=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_DefaultSearchPath(&lDefaultSearchPath); return _bstr_t(lDefaultSearchPath, false);}
		bool _get_DoubleAgentFiles () {VARIANT_BOOL lDoubleAgentFiles=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_DoubleAgentFiles(&lDoubleAgentFiles); return lDoubleAgentFiles?true:false;}
		HRESULT _put_DoubleAgentFiles (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_DoubleAgentFiles(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_MsAgentFiles () {VARIANT_BOOL lMsAgentFiles=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_MsAgentFiles(&lMsAgentFiles); return lMsAgentFiles?true:false;}
		HRESULT _put_MsAgentFiles (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_MsAgentFiles(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_MsOfficeFiles () {VARIANT_BOOL lMsOfficeFiles=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_MsOfficeFiles(&lMsOfficeFiles); return lMsOfficeFiles?true:false;}
		HRESULT _put_MsOfficeFiles (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_MsOfficeFiles(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_SpeakingCharacters () {VARIANT_BOOL lSpeakingCharacters=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_SpeakingCharacters(&lSpeakingCharacters); return lSpeakingCharacters?true:false;}
		HRESULT _put_CompliantCharacters (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_CompliantCharacters(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_NonCompliantCharacters () {VARIANT_BOOL lNonCompliantCharacters=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_NonCompliantCharacters(&lNonCompliantCharacters); return lNonCompliantCharacters?true:false;}
		HRESULT _put_SpeakingCharacters (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_SpeakingCharacters(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_NonSpeakingCharacters () {VARIANT_BOOL lNonSpeakingCharacters=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_NonSpeakingCharacters(&lNonSpeakingCharacters); return lNonSpeakingCharacters?true:false;}
		HRESULT _put_NonSpeakingCharacters (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_NonSpeakingCharacters(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_VerifyVersion () {VARIANT_BOOL lVerifyVersion=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_VerifyVersion(&lVerifyVersion); return lVerifyVersion?true:false;}
		HRESULT _put_VerifyVersion (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_VerifyVersion(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		_bstr_t _get_DefaultFilePath () {BSTR lDefaultFilePath=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_DefaultFilePath(&lDefaultFilePath); return _bstr_t(lDefaultFilePath, false);}
		_bstr_t _get_DefaultFileName () {BSTR lDefaultFileName=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_DefaultFileName(&lDefaultFileName); return _bstr_t(lDefaultFileName, false);}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaCtlFormattedTextPtr)
{
	_DACTLPTR_DECLARE(IDaCtlFormattedText)
	{
	public:
		_DACTLPTR_CONSTRUCT(IDaCtlFormattedText)
		_DACTLPTR_ASSIGN(IDaCtlFormattedText)

		__declspec(property(get=_get_RawText, put=_put_RawText))				_bstr_t					RawText;
		__declspec(property(get=_get_ParsedText))								_bstr_t					ParsedText;
		__declspec(property(get=_get_SpeechText))								_bstr_t					SpeechText;
		__declspec(property(get=_get_WordCount))								long					WordCount;
		__declspec(property(get=_get_WordIndex))								long					WordIndex;
		__declspec(property(get=_get_DisplayText))								_bstr_t					DisplayText;
		__declspec(property(get=_get_DisplayWord))								_bstr_t					DisplayWord[];

		_bstr_t _get_RawText () {BSTR lRawText=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_RawText(&lRawText); return _bstr_t(lRawText, false);}
		HRESULT _put_RawText (const _bstr_t& Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_RawText(Value); else return E_POINTER;}
		_bstr_t _get_ParsedText () {BSTR lParsedText=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_ParsedText(&lParsedText); return _bstr_t(lParsedText, false);}
		_bstr_t _get_SpeechText () {BSTR lSpeechText=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_SpeechText(&lSpeechText); return _bstr_t(lSpeechText, false);}
		long _get_WordCount () {long lWordCount=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_WordCount(&lWordCount); return lWordCount;}
		long _get_WordIndex () {long lWordIndex=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_WordIndex(&lWordIndex); return lWordIndex;}
		_bstr_t _get_DisplayText () {BSTR lDisplayText=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_DisplayText(&lDisplayText); return _bstr_t(lDisplayText, false);}
		_bstr_t _get_DisplayWord (long WordNdx) {BSTR lDisplayWord=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_DisplayWord(WordNdx, &lDisplayWord); return _bstr_t(lDisplayWord, false);}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaCtlTTSEnginePtr)
{
	_DACTLPTR_DECLARE(IDaCtlTTSEngine)
	{
	public:
		_DACTLPTR_CONSTRUCT(IDaCtlTTSEngine)
		_DACTLPTR_ASSIGN(IDaCtlTTSEngine)

		__declspec(property(get=_get_TTSModeID))								_bstr_t					TTSModeID;
		__declspec(property(get=_get_DisplayName))								_bstr_t					DisplayName;
		__declspec(property(get=_get_Manufacturer))								_bstr_t					Manufacturer;
		__declspec(property(get=_get_Gender))									SpeechGenderType		Gender;
		__declspec(property(get=_get_LanguageID))								long					LanguageID;
		__declspec(property(get=_get_LanguageName))								_bstr_t					LanguageName[];

		_bstr_t _get_TTSModeID () {BSTR lTTSModeID=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_TTSModeID(&lTTSModeID); return _bstr_t(lTTSModeID, false);}
		_bstr_t _get_DisplayName () {BSTR lDisplayName=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_DisplayName(&lDisplayName); return _bstr_t(lDisplayName, false);}
		_bstr_t _get_Manufacturer () {BSTR lManufacturer=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Manufacturer(&lManufacturer); return _bstr_t(lManufacturer, false);}
		SpeechGenderType _get_Gender () {SpeechGenderType lGender=SpeechGender_Neutral; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Gender(&lGender); return lGender;}
		long _get_LanguageID () {long lLanguageID=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_LanguageID(&lLanguageID); return lLanguageID;}
		_bstr_t _get_LanguageName (bool EnglishName=true) {BSTR lLanguageName=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_LanguageName(EnglishName?VARIANT_TRUE:VARIANT_FALSE, &lLanguageName); return _bstr_t(lLanguageName, false);}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaCtlTTSEnginesPtr)
{
	_DACTLPTR_DECLARE(IDaCtlTTSEngines)
	{
	public:
		_DACTLPTR_CONSTRUCT(IDaCtlTTSEngines)
		_DACTLPTR_ASSIGN(IDaCtlTTSEngines)

		__declspec(property(get=_get_Count))									long					Count;
		__declspec(property(get=_get_Item))										IDaCtlTTSEnginePtr		Item[];

		long _get_Count () {long lCount=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Count(&lCount); return lCount;}
		IDaCtlTTSEnginePtr _get_Item (VARIANT Index) {IDaCtlTTSEnginePtr lItem; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Item(Index, &lItem); return lItem;}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaCtlSREnginePtr)
{
	_DACTLPTR_DECLARE(IDaCtlSREngine)
	{
	public:
		_DACTLPTR_CONSTRUCT(IDaCtlSREngine)
		_DACTLPTR_ASSIGN(IDaCtlSREngine)

		__declspec(property(get=_get_SRModeID))									_bstr_t					SRModeID;
		__declspec(property(get=_get_DisplayName))								_bstr_t					DisplayName;
		__declspec(property(get=_get_Manufacturer))								_bstr_t					Manufacturer;
		__declspec(property(get=_get_LanguageID))								long					LanguageID;
		__declspec(property(get=_get_LanguageName))								_bstr_t					LanguageName[];
		__declspec(property(get=_get_LanguageIDs))								SAFEARRAY*				LanguageIDs;
		__declspec(property(get=_get_LanguageNames))							SAFEARRAY*				LanguageNames[];

		_bstr_t _get_SRModeID () {BSTR lSRModeID=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_SRModeID(&lSRModeID); return _bstr_t(lSRModeID, false);}
		_bstr_t _get_DisplayName () {BSTR lDisplayName=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_DisplayName(&lDisplayName); return _bstr_t(lDisplayName, false);}
		_bstr_t _get_Manufacturer () {BSTR lManufacturer=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Manufacturer(&lManufacturer); return _bstr_t(lManufacturer, false);}
		long _get_LanguageID () {long lLanguageID=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_LanguageID(&lLanguageID); return lLanguageID;}
		_bstr_t _get_LanguageName (bool EnglishName=true) {BSTR lLanguageName=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_LanguageName(EnglishName?VARIANT_TRUE:VARIANT_FALSE, &lLanguageName); return _bstr_t(lLanguageName, false);}
		SAFEARRAY* _get_LanguageIDs () {SAFEARRAY* lLanguageIDs=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_LanguageIDs(&lLanguageIDs); return lLanguageIDs;}
		SAFEARRAY* _get_LanguageNames (bool EnglishName=true) {SAFEARRAY* lLanguageNames=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_LanguageNames(EnglishName?VARIANT_TRUE:VARIANT_FALSE, &lLanguageNames); return lLanguageNames;}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaCtlSREnginesPtr)
{
	_DACTLPTR_DECLARE(IDaCtlSREngines)
	{
	public:
		_DACTLPTR_CONSTRUCT(IDaCtlSREngines)
		_DACTLPTR_ASSIGN(IDaCtlSREngines)

		__declspec(property(get=_get_Count))									long					Count;
		__declspec(property(get=_get_Item))										IDaCtlSREnginePtr		Item[];

		long _get_Count () {long lCount=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Count(&lCount); return lCount;}
		IDaCtlSREnginePtr _get_Item (VARIANT Index) {IDaCtlSREnginePtr lItem; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Item(Index, &lItem); return lItem;}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaCtlCharacter2Ptr)
{
	_DACTLPTR_DECLARE(IDaCtlCharacter2)
	{
	public:
		_DACTLPTR_CONSTRUCT(IDaCtlCharacter2)
		_DACTLPTR_ASSIGN(IDaCtlCharacter2)

		__declspec(property(get=_get_Balloon))															IDaCtlBalloon2Ptr		Balloon;
		__declspec(property(get=_get_Commands))															IDaCtlCommands2Ptr		Commands;
		__declspec(property(get=_get_Style, put=_put_Style))											long					Style;
		__declspec(property(get=_get_IdleEnabled, put=_put_IdleEnabled))								bool					IdleEnabled;
		__declspec(property(get=_get_SoundEffectsEnabled, put=_put_SoundEffectsEnabled))				bool					SoundEffectsEnabled;
		__declspec(property(get=_get_Smoothed))															bool					Smoothed;
		__declspec(property(get=_get_SmoothEdges))														bool					SmoothEdges;
		__declspec(property(get=_get_HasIcon))															bool					HasIcon;
		__declspec(property(get=_get_IconShown, put=_put_IconShown))									bool					IconShown;
		__declspec(property(get=_get_IconVisible))														bool					IconVisible;
		__declspec(property(get=_get_IconIdentity, put=_put_IconIdentity))								_bstr_t					IconIdentity;
		__declspec(property(get=_get_IconTip, put=_put_IconTip))										_bstr_t					IconTip;
		__declspec(property(get=_get_TTSEngine))														IDaCtlTTSEnginePtr		TTSEngine[];
		__declspec(property(get=_get_SREngine))															IDaCtlSREnginePtr		SREngine[];
		__declspec(property(get=_get_CharacterID))														_bstr_t					CharacterID;
		__declspec(property(get=_get_UniqueID))															_bstr_t					UniqueID;
		__declspec(property(get=_get_Name, put=_put_Name))												_bstr_t					Name;
		__declspec(property(get=_get_Description, put=_put_Description))								_bstr_t					Description;
		__declspec(property(get=_get_ExtraData))														_bstr_t					ExtraData;
		__declspec(property(get=_get_FileName))															_bstr_t					FileName;
		__declspec(property(get=_get_FilePath))															_bstr_t					FilePath;
		__declspec(property(get=_get_LanguageID, put=_put_LanguageID))									long					LanguageID;
		__declspec(property(get=_get_TTSModeID, put=_put_TTSModeID))									_bstr_t					TTSModeID;
		__declspec(property(get=_get_SRModeID, put=_put_SRModeID))										_bstr_t					SRModeID;
		__declspec(property(get=_get_Left, put=_put_Left))												short					Left;
		__declspec(property(get=_get_Top, put=_put_Top))												short					Top;
		__declspec(property(get=_get_Width, put=_put_Width))											short					Width;
		__declspec(property(get=_get_Height, put=_put_Height))											short					Height;
		__declspec(property(get=_get_OriginalWidth))													short					OriginalWidth;
		__declspec(property(get=_get_OriginalHeight))													short					OriginalHeight;
		__declspec(property(get=_get_Visible))															bool					Visible;
		__declspec(property(get=_get_ActiveState, put=_put_ActiveState))								ActiveStateType			ActiveState;
		__declspec(property(get=_get_IdleState))														bool					IdleState;
		__declspec(property(get=_get_HasOtherClients))													bool					HasOtherClients;
		__declspec(property(get=_get_MoveCause))														MoveCauseType			MoveCause;
		__declspec(property(get=_get_VisibilityCause))													VisibilityCauseType		VisibilityCause;
		__declspec(property(get=_get_ListeningStatus))													ListeningStatusType		ListeningStatus;
		__declspec(property(get=_get_Animations))														SAFEARRAY*				Animations;
		__declspec(property(get=_get_States))															SAFEARRAY*				States;
		__declspec(property(get=_get_Connected))														bool					Connected;
		__declspec(property(get=_get_SuspendPause, put=_put_SuspendPause))								bool					SuspendPause;
		__declspec(property(get=_get_SuspendStop, put=_put_SuspendStop))								bool					SuspendStop;
		__declspec(property(get=_get_SuspendHide, put=_put_SuspendHide))								bool					SuspendHide;

		IDaCtlBalloon2Ptr _get_Balloon () {IDaCtlBalloon2Ptr lBalloon; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Balloon(&lBalloon); return lBalloon;}
		IDaCtlCommands2Ptr _get_Commands () {IDaCtlCommands2Ptr lCommands; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Commands(&lCommands); return lCommands;}
		long _get_Style () {long lStyle=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Style(&lStyle); return lStyle;}
		HRESULT _put_Style (long Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_Style(Value); else return E_POINTER;}
		bool _get_IdleEnabled () {VARIANT_BOOL lIdleEnabled=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_IdleEnabled(&lIdleEnabled); return lIdleEnabled?true:false;}
		HRESULT _put_IdleEnabled (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_IdleEnabled(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_SoundEffectsEnabled () {VARIANT_BOOL lSoundEffectsEnabled=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_SoundEffectsEnabled(&lSoundEffectsEnabled); return lSoundEffectsEnabled?true:false;}
		HRESULT _put_SoundEffectsEnabled (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_SoundEffectsEnabled(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_Smoothed () {VARIANT_BOOL lSmoothed=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Smoothed(&lSmoothed); return lSmoothed?true:false;}
		bool _get_SmoothEdges () {VARIANT_BOOL lSmoothEdges=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_SmoothEdges(&lSmoothEdges); return lSmoothEdges?true:false;}
		bool _get_HasIcon () {VARIANT_BOOL lHasIcon=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_HasIcon(&lHasIcon); return lHasIcon?true:false;}
		bool _get_IconShown () {VARIANT_BOOL lIconShown=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_IconShown(&lIconShown); return lIconShown?true:false;}
		HRESULT _put_IconShown (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_IconShown(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_IconVisible () {VARIANT_BOOL lIconVisible=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_IconVisible(&lIconVisible); return lIconVisible?true:false;}
		_bstr_t _get_IconIdentity () {BSTR lIconIdentity=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_IconIdentity(&lIconIdentity); return _bstr_t(lIconIdentity, false);}
		HRESULT _put_IconIdentity (const _bstr_t& Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_IconIdentity(Value); else return E_POINTER;}
		_bstr_t _get_IconTip () {BSTR lIconTip=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_IconTip(&lIconTip); return _bstr_t(lIconTip, false);}
		HRESULT _put_IconTip (const _bstr_t& Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_IconTip(Value); else return E_POINTER;}
		IDaCtlTTSEnginePtr _get_TTSEngine (VARIANT GetDefault) {IDaCtlTTSEnginePtr lTTSEngine; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_TTSEngine(GetDefault, &lTTSEngine); return lTTSEngine;}
		IDaCtlSREnginePtr _get_SREngine (VARIANT GetDefault) {IDaCtlSREnginePtr lSREngine; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_SREngine(GetDefault, &lSREngine); return lSREngine;}
		_bstr_t _get_CharacterID () {BSTR lCharacterID=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_CharacterID(&lCharacterID); return _bstr_t(lCharacterID, false);}
		_bstr_t _get_UniqueID () {BSTR lUniqueID=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_UniqueID(&lUniqueID); return _bstr_t(lUniqueID, false);}
		_bstr_t _get_Name () {BSTR lName=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Name(&lName); return _bstr_t(lName, false);}
		HRESULT _put_Name (const _bstr_t& Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_Name(Value); else return E_POINTER;}
		_bstr_t _get_Description () {BSTR lDescription=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Description(&lDescription); return _bstr_t(lDescription, false);}
		HRESULT _put_Description (const _bstr_t& Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_Description(Value); else return E_POINTER;}
		_bstr_t _get_ExtraData () {BSTR lExtraData=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_ExtraData(&lExtraData); return _bstr_t(lExtraData, false);}
		_bstr_t _get_FileName () {BSTR lFileName=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_FileName(&lFileName); return _bstr_t(lFileName, false);}
		_bstr_t _get_FilePath () {BSTR lFilePath=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_FilePath(&lFilePath); return _bstr_t(lFilePath, false);}
		long _get_LanguageID () {long lLanguageID=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_LanguageID(&lLanguageID); return lLanguageID;}
		HRESULT _put_LanguageID (long Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_LanguageID(Value); else return E_POINTER;}
		_bstr_t _get_TTSModeID () {BSTR lTTSModeID=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_TTSModeID(&lTTSModeID); return _bstr_t(lTTSModeID, false);}
		HRESULT _put_TTSModeID (const _bstr_t& Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_TTSModeID(Value); else return E_POINTER;}
		_bstr_t _get_SRModeID () {BSTR lSRModeID=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_SRModeID(&lSRModeID); return _bstr_t(lSRModeID, false);}
		HRESULT _put_SRModeID (const _bstr_t& Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_SRModeID(Value); else return E_POINTER;}
		short _get_Left () {short lLeft=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Left(&lLeft); return lLeft;}
		HRESULT _put_Left (short Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_Left(Value); else return E_POINTER;}
		short _get_Top () {short lTop=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Top(&lTop); return lTop;}
		HRESULT _put_Top (short Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_Top(Value); else return E_POINTER;}
		short _get_Width () {short lWidth=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Width(&lWidth); return lWidth;}
		HRESULT _put_Width (short Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_Width(Value); else return E_POINTER;}
		short _get_Height () {short lHeight=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Height(&lHeight); return lHeight;}
		HRESULT _put_Height (short Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_Height(Value); else return E_POINTER;}
		short _get_OriginalWidth () {short lOriginalWidth=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_OriginalWidth(&lOriginalWidth); return lOriginalWidth;}
		short _get_OriginalHeight () {short lOriginalHeight=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_OriginalHeight(&lOriginalHeight); return lOriginalHeight;}
		bool _get_Visible () {VARIANT_BOOL lVisible=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Visible(&lVisible); return lVisible?true:false;}
		ActiveStateType _get_ActiveState () {ActiveStateType lActiveState=ActiveState_Inactive; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_ActiveState(&lActiveState); return lActiveState;}
		HRESULT _put_ActiveState (ActiveStateType Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_ActiveState(Value); else return E_POINTER;}
		bool _get_IdleState () {VARIANT_BOOL lIdleState=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_IdleState(&lIdleState); return lIdleState?true:false;}
		bool _get_HasOtherClients () {VARIANT_BOOL lHasOtherClients=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_HasOtherClients(&lHasOtherClients); return lHasOtherClients?true:false;}
		MoveCauseType _get_MoveCause () {MoveCauseType lMoveCause=MoveCause_NeverMoved; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_MoveCause(&lMoveCause); return lMoveCause;}
		VisibilityCauseType _get_VisibilityCause () {VisibilityCauseType lVisibilityCause=VisibilityCause_NeverShown; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_VisibilityCause(&lVisibilityCause); return lVisibilityCause;}
		ListeningStatusType _get_ListeningStatus () {ListeningStatusType lListeningStatus=ListeningStatus_Error; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_ListeningStatus(&lListeningStatus); return lListeningStatus;}
		SAFEARRAY* _get_Animations () {SAFEARRAY* lAnimations=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Animations(&lAnimations); return lAnimations;}
		SAFEARRAY* _get_States () {SAFEARRAY* lStates=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_States(&lStates); return lStates;}
		bool _get_Connected () {VARIANT_BOOL lConnected=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Connected(&lConnected); return lConnected?true:false;}
		bool _get_SuspendPause () {VARIANT_BOOL lSuspendPause=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_SuspendPause(&lSuspendPause); return lSuspendPause?true:false;}
		HRESULT _put_SuspendPause (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_SuspendPause(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_SuspendStop () {VARIANT_BOOL lSuspendStop=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_SuspendStop(&lSuspendStop); return lSuspendStop?true:false;}
		HRESULT _put_SuspendStop (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_SuspendStop(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_SuspendHide () {VARIANT_BOOL lSuspendHide=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_SuspendHide(&lSuspendHide); return lSuspendHide?true:false;}
		HRESULT _put_SuspendHide (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_SuspendHide(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaCtlCharacters2Ptr)
{
	_DACTLPTR_DECLARE(IDaCtlCharacters2)
	{
	public:
		_DACTLPTR_CONSTRUCT(IDaCtlCharacters2)
		_DACTLPTR_ASSIGN(IDaCtlCharacters2)

		__declspec(property(get=_get_Count))									long					Count;
		__declspec(property(get=_get_Item))										IDaCtlCharacter2Ptr		Item[];
		__declspec(property(get=_get_Index))									IDaCtlCharacter2Ptr		Index[];

		long _get_Count () {long lCount=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Count(&lCount); return lCount;}
		IDaCtlCharacter2Ptr _get_Item (BSTR CharacterID) {IDaCtlCharacter2Ptr lCommand; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Item(CharacterID, &lCommand); return lCommand;}
		IDaCtlCharacter2Ptr _get_Index (long Index) {IDaCtlCharacter2Ptr lIndex; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Index(Index, &lIndex); return lIndex;}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaCtlRequestPtr)
{
	_DACTLPTR_DECLARE(IDaCtlRequest)
	{
	public:
		_DACTLPTR_CONSTRUCT(IDaCtlRequest)
		_DACTLPTR_ASSIGN(IDaCtlRequest)

		__declspec(property(get=_get_ID))										long					ID;
		__declspec(property(get=_get_Status))									long					Status;
		__declspec(property(get=_get_Description))								_bstr_t					Description;
		__declspec(property(get=_get_Number))									long					Number;

		long _get_ID () {long lID=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_ID(&lID); return lID;}
		long _get_Status () {long lStatus=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Status(&lStatus); return lStatus;}
		_bstr_t _get_Description () {BSTR lDescription=NULL; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Description(&lDescription); return _bstr_t(lDescription, false);}
		long _get_Number () {long lNumber=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Number(&lNumber); return lNumber;}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaControl2Ptr)
{
	_DACTLPTR_DECLARE(IDaControl2)
	{
	public:
		_DACTLPTR_CONSTRUCT(IDaControl2)
		_DACTLPTR_ASSIGN(IDaControl2)

		__declspec(property(get=_get_RaiseRequestErrors, put=_put_RaiseRequestErrors))						bool						RaiseRequestErrors;
		__declspec(property(get=_get_Connected, put=_put_Connected))										bool						Connected;
		__declspec(property(get=_get_AutoConnect, put=_put_AutoConnect))									unsigned short				AutoConnect;
		__declspec(property(get=_get_RaiseRequestErrors, put=_put_RaiseRequestErrors))						bool						RaiseRequestErrors;
		__declspec(property(get=_get_CharacterStyle, put=_put_CharacterStyle))								long						CharacterStyle;
		__declspec(property(get=_get_ControlCharacter, put=_put_ControlCharacter))							IDaCtlCharacter2Ptr			ControlCharacter;
		__declspec(property(get=_get_Characters))															IDaCtlCharacters2Ptr		Characters;
		__declspec(property(get=_get_CharacterFiles))														IDaCtlCharacterFilesPtr		CharacterFiles;
		__declspec(property(get=_get_TTSEngines))															IDaCtlTTSEnginesPtr			TTSEngines;
		__declspec(property(get=_get_SREngines))															IDaCtlSREnginesPtr			SREngines;
		__declspec(property(get=_get_PropertySheet))														IDaCtlPropertySheet2Ptr		PropertySheet;
		__declspec(property(get=_get_CommandsWindow))														IDaCtlCommandsWindowPtr		CommandsWindow;
		__declspec(property(get=_get_Settings))																IDaCtlSettingsPtr			Settings;

		bool _get_RaiseRequestErrors () {VARIANT_BOOL lRaiseRequestErrors=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_RaiseRequestErrors(&lRaiseRequestErrors); return lRaiseRequestErrors?true:false;}
		HRESULT _put_RaiseRequestErrors (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_RaiseRequestErrors(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_Connected () {VARIANT_BOOL lConnected=VARIANT_FALSE; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Connected(&lConnected); return lConnected?true:false;}
		HRESULT _put_Connected (bool Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_Connected(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		unsigned short _get_AutoConnect () {short lAutoConnect=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_AutoConnect(&lAutoConnect); return (unsigned short)lAutoConnect;}
		HRESULT _put_AutoConnect (unsigned short Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_AutoConnect((short)Value); else return E_POINTER;}
		long _get_CharacterStyle () {long lCharacterStyle=0; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_CharacterStyle(&lCharacterStyle); return lCharacterStyle;}
		HRESULT _put_CharacterStyle (long Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_CharacterStyle(Value); else return E_POINTER;}
		IDaCtlCharacter2Ptr _get_ControlCharacter () {IDaCtlCharacter2Ptr lCharacter; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_ControlCharacter(&lCharacter); return lCharacter;}
		HRESULT _put_ControlCharacter (IDaCtlCharacter2 * Value) {if (_DACTLPTR_CHKNULL) return GetInterfacePtr()->put_ControlCharacter(Value); else return E_POINTER;}
		IDaCtlCharacters2Ptr _get_Characters () {IDaCtlCharacters2Ptr lCharacters; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Characters(&lCharacters); return lCharacters;}
		IDaCtlCharacterFilesPtr _get_CharacterFiles () {IDaCtlCharacterFilesPtr lCharacterFiles; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_CharacterFiles(&lCharacterFiles); return lCharacterFiles;}
		IDaCtlTTSEnginesPtr _get_TTSEngines () {IDaCtlTTSEnginesPtr lTTSEngines; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_TTSEngines(&lTTSEngines); return lTTSEngines;}
		IDaCtlSREnginesPtr _get_SREngines () {IDaCtlSREnginesPtr lSREngines; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_SREngines(&lSREngines); return lSREngines;}
		IDaCtlPropertySheet2Ptr _get_PropertySheet () {IDaCtlPropertySheet2Ptr lPropertySheet; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_PropertySheet(&lPropertySheet); return lPropertySheet;}
		IDaCtlCommandsWindowPtr _get_CommandsWindow () {IDaCtlCommandsWindowPtr lCommandsWindow; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_CommandsWindow(&lCommandsWindow); return lCommandsWindow;}
		IDaCtlSettingsPtr _get_Settings () {IDaCtlSettingsPtr lSettings; if (_DACTLPTR_CHKNULL) GetInterfacePtr()->get_Settings(&lSettings); return lSettings;}
	};
}

/////////////////////////////////////////////////////////////////////////////

#undef	_DACTLPTR_DECLARE
#undef	_DACTLPTR_CONSTRUCT
#undef	_DACTLPTR_ASSIGN
#undef	_DACTLPTR_CHKNULL

/////////////////////////////////////////////////////////////////////////////
