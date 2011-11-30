/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#include "DaServerOdl.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef	_DASVRPTR_ALLOWNULL
#define	_DASVRPTR_CHKNULL	true
#else
#define	_DASVRPTR_CHKNULL	operator bool()
#endif

/////////////////////////////////////////////////////////////////////////////

#define	_DASVRPTR_DECLARE(i) class i##Ptr : public _COM_SMARTPTR<_COM_SMARTPTR_LEVEL2<i##, &__uuidof(i##)> >

#define	_DASVRPTR_CONSTRUCT(i) \
	i##Ptr () {}; \
	i##Ptr (const i##Ptr& p) : _com_ptr_t<ThisIIID> (p) {} \
	i##Ptr (i##* p) : _com_ptr_t<ThisIIID> (p) {} \
	i##Ptr (const _variant_t& varSrc) : _com_ptr_t<ThisIIID> (varSrc) {} \
	template<typename _OtherIID> i##Ptr (const _com_ptr_t<_OtherIID>& p) : _com_ptr_t<ThisIIID> (p) {} \
	template<typename _InterfaceType> i##Ptr (_InterfaceType* p) : _com_ptr_t<ThisIIID> (p) {}

#define	_DASVRPTR_ASSIGN(i) \
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

__if_not_exists(IDaSvrBalloon2Ptr)
{
	_DASVRPTR_DECLARE(IDaSvrBalloon2)
	{
	public:
		_DASVRPTR_CONSTRUCT(IDaSvrBalloon2)
		_DASVRPTR_ASSIGN(IDaSvrBalloon2)

		__declspec(property(get=_get_Enabled))									bool					Enabled;
		__declspec(property(get=_get_Style, put=_put_Style))					long					Style;
		__declspec(property(get=_get_Visible, put=_put_Visible))				bool					Visible;
		__declspec(property(get=_get_NumberOfLines, put=_put_NumberOfLines))	long					NumberOfLines;
		__declspec(property(get=_get_CharsPerLine, put=_put_CharsPerLine))		long					CharsPerLine;
		__declspec(property(get=_get_TextColor, put=_put_TextColor))			OLE_COLOR				TextColor;
		__declspec(property(get=_get_BackColor, put=_put_BackColor))			OLE_COLOR				BackColor;
		__declspec(property(get=_get_BorderColor, put=_put_BorderColor))		OLE_COLOR				BorderColor;
		__declspec(property(get=_get_FontName, put=_put_FontName))				_bstr_t					FontName;
		__declspec(property(get=_get_FontSize, put=_put_FontSize))				long					FontSize;
		__declspec(property(get=_get_FontBold, put=_put_FontBold))				bool					FontBold;
		__declspec(property(get=_get_FontItalic, put=_put_FontItalic))			bool					FontItalic;
		__declspec(property(get=_get_FontStrikethru, put=_put_FontStrikethru))	bool					FontStrikethru;
		__declspec(property(get=_get_FontUnderline, put=_put_FontUnderline))	bool					FontUnderline;
		__declspec(property(get=_get_FontCharSet, put=_put_FontCharSet))		short					FontCharSet;

		bool _get_Enabled () {VARIANT_BOOL lEnabled=VARIANT_FALSE; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Enabled(&lEnabled); return lEnabled?true:false;}
		long _get_Style () {long lStyle=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Style(&lStyle); return lStyle;}
		HRESULT _put_Style (long Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_Style(Value); else return E_POINTER;}
		bool _get_Visible () {VARIANT_BOOL lVisible=VARIANT_FALSE; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Visible(&lVisible); return lVisible?true:false;}
		HRESULT _put_Visible (bool Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_Visible(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		long _get_NumberOfLines () {long lNumberOfLines=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_NumberOfLines(&lNumberOfLines); return lNumberOfLines;}
		HRESULT _put_NumberOfLines (long Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_NumberOfLines(Value); else return E_POINTER;}
		long _get_CharsPerLine () {long lCharsPerLine=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_CharsPerLine(&lCharsPerLine); return lCharsPerLine;}
		HRESULT _put_CharsPerLine (long Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_CharsPerLine(Value); else return E_POINTER;}
		OLE_COLOR _get_TextColor () {OLE_COLOR lTextColor=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_TextColor(&lTextColor); return lTextColor;}
		HRESULT _put_TextColor (OLE_COLOR Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_TextColor(Value); else return E_POINTER;}
		OLE_COLOR _get_BackColor () {OLE_COLOR lBackColor=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_BackColor(&lBackColor); return lBackColor;}
		HRESULT _put_BackColor (OLE_COLOR Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_BackColor(Value); else return E_POINTER;}
		OLE_COLOR _get_BorderColor () {OLE_COLOR lBorderColor=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_BorderColor(&lBorderColor); return lBorderColor;}
		HRESULT _put_BorderColor (OLE_COLOR Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_BorderColor(Value); else return E_POINTER;}
		_bstr_t _get_FontName () {BSTR lFontName=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_FontName(&lFontName); return _bstr_t(lFontName, false);}
		HRESULT _put_FontName (const _bstr_t& Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_FontName(Value); else return E_POINTER;}
		long _get_FontSize () {long lFontSize=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_FontSize(&lFontSize); return lFontSize;}
		HRESULT _put_FontSize (long Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_FontSize(Value); else return E_POINTER;}
		bool _get_FontBold () {VARIANT_BOOL lFontBold=VARIANT_FALSE; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_FontBold(&lFontBold); return lFontBold?true:false;}
		HRESULT _put_FontBold (bool Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_FontBold(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_FontItalic () {VARIANT_BOOL lFontItalic=VARIANT_FALSE; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_FontItalic(&lFontItalic); return lFontItalic?true:false;}
		HRESULT _put_FontItalic (bool Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_FontItalic(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_FontStrikethru () {VARIANT_BOOL lFontStrikethru=VARIANT_FALSE; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_FontStrikethru(&lFontStrikethru); return lFontStrikethru?true:false;}
		HRESULT _put_FontStrikethru (bool Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_FontStrikethru(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_FontUnderline () {VARIANT_BOOL lFontUnderline=VARIANT_FALSE; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_FontUnderline(&lFontUnderline); return lFontUnderline?true:false;}
		HRESULT _put_FontUnderline (bool Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_FontUnderline(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		short _get_FontCharSet () {short lFontCharSet=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_FontCharSet(&lFontCharSet); return lFontCharSet;}
		HRESULT _put_FontCharSet (short Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_FontCharSet(Value); else return E_POINTER;}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaSvrCommand2Ptr)
{
	_DASVRPTR_DECLARE(IDaSvrCommand2)
	{
	public:
		_DASVRPTR_CONSTRUCT(IDaSvrCommand2)
		_DASVRPTR_ASSIGN(IDaSvrCommand2)

		__declspec(property(get=_get_Caption, put=_put_Caption))										_bstr_t					Caption;
		__declspec(property(get=_get_Enabled, put=_put_Enabled))										bool					Enabled;
		__declspec(property(get=_get_Visible, put=_put_Visible))										bool					Visible;
		__declspec(property(get=_get_CommandID))														long					CommandID;
		__declspec(property(get=_get_VoiceCaption, put=_put_VoiceCaption))								_bstr_t					VoiceCaption;
		__declspec(property(get=_get_VoiceGrammar, put=_put_VoiceGrammar))								_bstr_t					VoiceGrammar;
		__declspec(property(get=_get_ConfidenceThreshold, put=_put_ConfidenceThreshold))				long					ConfidenceThreshold;
		__declspec(property(get=_get_ConfidenceText, put=_put_ConfidenceText))							_bstr_t					ConfidenceText;

		_bstr_t _get_Caption () {BSTR lCaption=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Caption(&lCaption); return _bstr_t(lCaption, false);}
		HRESULT _put_Caption (const _bstr_t& Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_Caption(Value); else return E_POINTER;}
		bool _get_Enabled () {VARIANT_BOOL lEnabled=VARIANT_FALSE; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Enabled(&lEnabled); return lEnabled?true:false;}
		HRESULT _put_Enabled (bool Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_Enabled(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		bool _get_Visible () {VARIANT_BOOL lVisible=VARIANT_FALSE; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Visible(&lVisible); return lVisible?true:false;}
		HRESULT _put_Visible (bool Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_Visible(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		long _get_CommandID () {long lCommandID=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_CommandID(&lCommandID); return lCommandID;}
		_bstr_t _get_VoiceCaption () {BSTR lVoiceCaption=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_VoiceCaption(&lVoiceCaption); return _bstr_t(lVoiceCaption, false);}
		HRESULT _put_VoiceCaption (const _bstr_t& Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_VoiceCaption(Value); else return E_POINTER;}
		_bstr_t _get_VoiceGrammar () {BSTR lVoiceGrammar=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_VoiceGrammar(&lVoiceGrammar); return _bstr_t(lVoiceGrammar, false);}
		HRESULT _put_VoiceGrammar (const _bstr_t& Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_VoiceGrammar(Value); else return E_POINTER;}
		long _get_ConfidenceThreshold () {long lConfidenceThreshold=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_ConfidenceThreshold(&lConfidenceThreshold); return lConfidenceThreshold;}
		HRESULT _put_ConfidenceThreshold (long Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_ConfidenceThreshold(Value); else return E_POINTER;}
		_bstr_t _get_ConfidenceText () {BSTR lConfidenceText=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_ConfidenceText(&lConfidenceText); return _bstr_t(lConfidenceText, false);}
		HRESULT _put_ConfidenceText (const _bstr_t& Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_ConfidenceText(Value); else return E_POINTER;}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaSvrCommands2Ptr)
{
	_DASVRPTR_DECLARE(IDaSvrCommands2)
	{
	public:
		_DASVRPTR_CONSTRUCT(IDaSvrCommands2)
		_DASVRPTR_ASSIGN(IDaSvrCommands2)

		__declspec(property(get=_get_Count))															long					Count;
		__declspec(property(get=_get_Item))																IDaSvrCommand2Ptr		Item[];
		__declspec(property(get=_get_Command))															IDaSvrCommand2Ptr		Command[];
		__declspec(property(get=_get_Caption, put=_put_Caption))										_bstr_t					Caption;
		__declspec(property(get=_get_VoiceGrammar, put=_put_VoiceGrammar))								_bstr_t					VoiceGrammar;
		__declspec(property(get=_get_Visible, put=_put_Visible))										bool					Visible;
		__declspec(property(get=_get_DefaultCommand, put=_put_DefaultCommand))							long					DefaultCommand;
		__declspec(property(get=_get_FontName, put=_put_FontName))										_bstr_t					FontName;
		__declspec(property(get=_get_FontSize, put=_put_FontSize))										long					FontSize;
		__declspec(property(get=_get_VoiceCaption, put=_put_VoiceCaption))								_bstr_t					VoiceCaption;
		__declspec(property(get=_get_GlobalVoiceCommandsEnabled, put=_put_GlobalVoiceCommandsEnabled))	bool					GlobalVoiceCommandsEnabled;

		long _get_Count () {long lCount=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Count(&lCount); return lCount;}
		IDaSvrCommand2Ptr _get_Item (long Index) {IDaSvrCommand2Ptr lItem; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Item(Index, &lItem); return lItem;}
		IDaSvrCommand2Ptr _get_Command (long CommandID) {IDaSvrCommand2Ptr lCommand; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Command(CommandID, &lCommand); return lCommand;}
		_bstr_t _get_Caption () {BSTR lCaption=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Caption(&lCaption); return _bstr_t(lCaption, false);}
		HRESULT _put_Caption (const _bstr_t& Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_Caption(Value); else return E_POINTER;}
		_bstr_t _get_VoiceGrammar () {BSTR lVoiceGrammar=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_VoiceGrammar(&lVoiceGrammar); return _bstr_t(lVoiceGrammar, false);}
		HRESULT _put_VoiceGrammar (const _bstr_t& Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_VoiceGrammar(Value); else return E_POINTER;}
		bool _get_Visible () {VARIANT_BOOL lVisible=VARIANT_FALSE; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Visible(&lVisible); return lVisible?true:false;}
		HRESULT _put_Visible (bool Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_Visible(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		long _get_DefaultCommand () {long lDefaultCommand=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_DefaultCommand(&lDefaultCommand); return lDefaultCommand;}
		HRESULT _put_DefaultCommand (long Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_DefaultCommand(Value); else return E_POINTER;}
		_bstr_t _get_FontName () {BSTR lFontName=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_FontName(&lFontName); return _bstr_t(lFontName, false);}
		HRESULT _put_FontName (const _bstr_t& Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_FontName(Value); else return E_POINTER;}
		long _get_FontSize () {long lFontSize=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_FontSize(&lFontSize); return lFontSize;}
		HRESULT _put_FontSize (long Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_FontSize(Value); else return E_POINTER;}
		_bstr_t _get_VoiceCaption () {BSTR lVoiceCaption=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_VoiceCaption(&lVoiceCaption); return _bstr_t(lVoiceCaption, false);}
		HRESULT _put_VoiceCaption (const _bstr_t& Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_VoiceCaption(Value); else return E_POINTER;}
		bool _get_GlobalVoiceCommandsEnabled () {VARIANT_BOOL lGlobalVoiceCommandsEnabled=VARIANT_FALSE; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_GlobalVoiceCommandsEnabled(&lGlobalVoiceCommandsEnabled); return lGlobalVoiceCommandsEnabled?true:false;}
		HRESULT _put_GlobalVoiceCommandsEnabled (bool Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_GlobalVoiceCommandsEnabled(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaSvrUserInput2Ptr)
{
	_DASVRPTR_DECLARE(IDaSvrUserInput2)
	{
	public:
		_DASVRPTR_CONSTRUCT(IDaSvrUserInput2)
		_DASVRPTR_ASSIGN(IDaSvrUserInput2)

		__declspec(property(get=_get_Count))									long					Count;
		__declspec(property(get=_get_ItemCommandID))							long					ItemCommandID[];
		__declspec(property(get=_get_ItemConfidence))							long					ItemConfidence[];
		__declspec(property(get=_get_ItemText))									_bstr_t					ItemText[];

		long _get_Count () {long lCount=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Count(&lCount); return lCount;}
		long _get_ItemCommandID (long ItemIndex) {long lItemCommandID=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_ItemCommandID(ItemIndex, &lItemCommandID); return lItemCommandID;}
		long _get_ItemConfidence (long ItemIndex) {long lItemConfidence=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_ItemConfidence(ItemIndex, &lItemConfidence); return lItemConfidence;}
		_bstr_t _get_ItemText (long ItemIndex) {BSTR lItemText=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_ItemText(ItemIndex, &lItemText); return _bstr_t(lItemText, false);}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaSvrCommandsWindow2Ptr)
{
	_DASVRPTR_DECLARE(IDaSvrCommandsWindow2)
	{
	public:
		_DASVRPTR_CONSTRUCT(IDaSvrCommandsWindow2)
		_DASVRPTR_ASSIGN(IDaSvrCommandsWindow2)

		__declspec(property(get=_get_Visible, put=_put_Visible))				bool					Visible;
		__declspec(property(get=_get_Left))										short					Left;
		__declspec(property(get=_get_Top))										short					Top;
		__declspec(property(get=_get_Width))									short					Width;
		__declspec(property(get=_get_Height))									short					Height;

		bool _get_Visible () {VARIANT_BOOL lVisible=VARIANT_FALSE; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Visible(&lVisible); return lVisible?true:false;}
		HRESULT _put_Visible (bool Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_Visible(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		short _get_Left () {short lLeft=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Left(&lLeft); return lLeft;}
		short _get_Top () {short lTop=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Top(&lTop); return lTop;}
		short _get_Width () {short lWidth=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Width(&lWidth); return lWidth;}
		short _get_Height () {short lHeight=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Height(&lHeight); return lHeight;}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaSvrPropertySheet2Ptr)
{
	_DASVRPTR_DECLARE(IDaSvrPropertySheet2)
	{
	public:
		_DASVRPTR_CONSTRUCT(IDaSvrPropertySheet2)
		_DASVRPTR_ASSIGN(IDaSvrPropertySheet2)

		__declspec(property(get=_get_Visible, put=_put_Visible))				bool					Visible;
		__declspec(property(get=_get_Page, put=_put_Page))						_bstr_t					Page;
		__declspec(property(get=_get_Left))										short					Left;
		__declspec(property(get=_get_Top))										short					Top;
		__declspec(property(get=_get_Width))									short					Width;
		__declspec(property(get=_get_Height))									short					Height;

		bool _get_Visible () {VARIANT_BOOL lVisible=VARIANT_FALSE; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Visible(&lVisible); return lVisible?true:false;}
		HRESULT _put_Visible (bool Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_Visible(Value?VARIANT_TRUE:VARIANT_FALSE); else return E_POINTER;}
		_bstr_t _get_Page () {BSTR lPage=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Page(&lPage); return _bstr_t(lPage, false);}
		HRESULT _put_Page (const _bstr_t& Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_Page(Value); else return E_POINTER;}
		short _get_Left () {short lLeft=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Left(&lLeft); return lLeft;}
		short _get_Top () {short lTop=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Top(&lTop); return lTop;}
		short _get_Width () {short lWidth=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Width(&lWidth); return lWidth;}
		short _get_Height () {short lHeight=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Height(&lHeight); return lHeight;}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaSvrSettingsPtr)
{
	_DASVRPTR_DECLARE(IDaSvrSettings)
	{
	public:
		_DASVRPTR_CONSTRUCT(IDaSvrSettings)
		_DASVRPTR_ASSIGN(IDaSvrSettings)

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

		bool _get_SoundEffectsEnabled () {VARIANT_BOOL lSoundEffectsEnabled=VARIANT_FALSE; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_SoundEffectsEnabled(&lSoundEffectsEnabled); return lSoundEffectsEnabled?true:false;}
		bool _get_BalloonEnabled () {VARIANT_BOOL lBalloonEnabled=VARIANT_FALSE; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_BalloonEnabled(&lBalloonEnabled); return lBalloonEnabled?true:false;}
		bool _get_TTSEnabled () {VARIANT_BOOL lTTSEnabled=VARIANT_FALSE; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_TTSEnabled(&lTTSEnabled); return lTTSEnabled?true:false;}
		short _get_TTSSpeed () {short lTTSSpeed=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_TTSSpeed(&lTTSSpeed); return lTTSSpeed;}
		bool _get_SREnabled () {VARIANT_BOOL lSREnabled=VARIANT_FALSE; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_SREnabled(&lSREnabled); return lSREnabled?true:false;}
		_bstr_t _get_SRHotKey () {BSTR lSRHotKey=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_SRHotKey(&lSRHotKey); return _bstr_t(lSRHotKey, false);}
		short _get_SRHotKeyTime () {short lSRHotKeyTime=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_SRHotKeyTime(&lSRHotKeyTime); return lSRHotKeyTime;}
		bool _get_SRListeningTip () {VARIANT_BOOL lSRListeningTip=VARIANT_FALSE; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_SRListeningTip(&lSRListeningTip); return lSRListeningTip?true:false;}
		bool _get_SRListeningPrompt () {VARIANT_BOOL lSRListeningPrompt=VARIANT_FALSE; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_SRListeningPrompt(&lSRListeningPrompt); return lSRListeningPrompt?true:false;}
		AudioStatusType _get_AudioStatus () {AudioStatusType lAudioStatus=AudioStatus_Error; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_AudioStatus(&lAudioStatus); return lAudioStatus;}

		__if_exists(IFontDisp)
		{
			__declspec(property(get=_get_BalloonFont))							IFontDispPtr			BalloonFont;
			IFontDispPtr _get_BalloonFont () {IFontDispPtr lBalloonFont; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_BalloonFont(&lBalloonFont); return lBalloonFont;}
		}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaSvrCharacterFilesPtr)
{
	_DASVRPTR_DECLARE(IDaSvrCharacterFiles)
	{
	public:
		_DASVRPTR_CONSTRUCT(IDaSvrCharacterFiles)
		_DASVRPTR_ASSIGN(IDaSvrCharacterFiles)

		__declspec(property(get=_get_FilePaths))								SAFEARRAY*				FilePaths;
		__declspec(property(get=_get_SearchPath, put=_put_SearchPath))			_bstr_t					SearchPath;
		__declspec(property(get=_get_DefaultSearchPath))						_bstr_t					DefaultSearchPath;
		__declspec(property(get=_get_Filter, put=_put_Filter))					long					Filter;
		__declspec(property(get=_get_DefaultFilePath))							_bstr_t					DefaultFilePath;
		__declspec(property(get=_get_DefaultFileName))							_bstr_t					DefaultFileName;

		SAFEARRAY* _get_FilePaths () {SAFEARRAY* lFilePaths=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_FilePaths(&lFilePaths); return lFilePaths;}
		_bstr_t _get_SearchPath () {BSTR lSearchPath=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_SearchPath(&lSearchPath); return _bstr_t(lSearchPath, false);}
		HRESULT _put_SearchPath (const _bstr_t& Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_SearchPath(Value); else return E_POINTER;}
		_bstr_t _get_DefaultSearchPath () {BSTR lDefaultSearchPath=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_DefaultSearchPath(&lDefaultSearchPath); return _bstr_t(lDefaultSearchPath, false);}
		long _get_Filter () {long lFilter=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Filter(&lFilter); return lFilter;}
		HRESULT _put_Filter (long Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_Filter(Value); else return E_POINTER;}
		_bstr_t _get_DefaultFilePath () {BSTR lDefaultFilePath=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_DefaultFilePath(&lDefaultFilePath); return _bstr_t(lDefaultFilePath, false);}
		_bstr_t _get_DefaultFileName () {BSTR lDefaultFileName=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_DefaultFileName(&lDefaultFileName); return _bstr_t(lDefaultFileName, false);}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaSvrFormattedTextPtr)
{
	_DASVRPTR_DECLARE(IDaSvrFormattedText)
	{
	public:
		_DASVRPTR_CONSTRUCT(IDaSvrFormattedText)
		_DASVRPTR_ASSIGN(IDaSvrFormattedText)

		__declspec(property(get=_get_RawText, put=_put_RawText))				_bstr_t					RawText;
		__declspec(property(get=_get_ParsedText))								_bstr_t					ParsedText;
		__declspec(property(get=_get_SpeechText))								_bstr_t					SpeechText;
		__declspec(property(get=_get_WordCount))								long					WordCount;
		__declspec(property(get=_get_WordIndex))								long					WordIndex;
		__declspec(property(get=_get_DisplayText))								_bstr_t					DisplayText;
		__declspec(property(get=_get_DisplayWord))								_bstr_t					DisplayWord[];

		_bstr_t _get_RawText () {BSTR lRawText=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_RawText(&lRawText); return _bstr_t(lRawText, false);}
		HRESULT _put_RawText (const _bstr_t& Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_RawText(Value); else return E_POINTER;}
		_bstr_t _get_ParsedText () {BSTR lParsedText=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_ParsedText(&lParsedText); return _bstr_t(lParsedText, false);}
		_bstr_t _get_SpeechText () {BSTR lSpeechText=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_SpeechText(&lSpeechText); return _bstr_t(lSpeechText, false);}
		long _get_WordCount () {long lWordCount=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_WordCount(&lWordCount); return lWordCount;}
		long _get_WordIndex () {long lWordIndex=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_WordIndex(&lWordIndex); return lWordIndex;}
		_bstr_t _get_DisplayText () {BSTR lDisplayText=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_DisplayText(&lDisplayText); return _bstr_t(lDisplayText, false);}
		_bstr_t _get_DisplayWord (long WordNdx) {BSTR lDisplayWord=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_DisplayWord(WordNdx, &lDisplayWord); return _bstr_t(lDisplayWord, false);}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaSvrTTSEnginePtr)
{
	_DASVRPTR_DECLARE(IDaSvrTTSEngine)
	{
	public:
		_DASVRPTR_CONSTRUCT(IDaSvrTTSEngine)
		_DASVRPTR_ASSIGN(IDaSvrTTSEngine)

		__declspec(property(get=_get_TTSModeID))								_bstr_t					TTSModeID;
		__declspec(property(get=_get_DisplayName))								_bstr_t					DisplayName;
		__declspec(property(get=_get_Manufacturer))								_bstr_t					Manufacturer;
		__declspec(property(get=_get_Gender))									SpeechGenderType		Gender;
		__declspec(property(get=_get_LanguageID))								long					LanguageID;
		__declspec(property(get=_get_LanguageName))								_bstr_t					LanguageName[];

		_bstr_t _get_TTSModeID () {BSTR lTTSModeID=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_TTSModeID(&lTTSModeID); return _bstr_t(lTTSModeID, false);}
		_bstr_t _get_DisplayName () {BSTR lDisplayName=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_DisplayName(&lDisplayName); return _bstr_t(lDisplayName, false);}
		_bstr_t _get_Manufacturer () {BSTR lManufacturer=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Manufacturer(&lManufacturer); return _bstr_t(lManufacturer, false);}
		SpeechGenderType _get_Gender () {SpeechGenderType lGender=SpeechGender_Neutral; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Gender(&lGender); return lGender;}
		long _get_LanguageID () {long lLanguageID=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_LanguageID(&lLanguageID); return lLanguageID;}
		_bstr_t _get_LanguageName (bool EnglishName=true) {BSTR lLanguageName=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_LanguageName(EnglishName?VARIANT_TRUE:VARIANT_FALSE, &lLanguageName); return _bstr_t(lLanguageName, false);}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaSvrTTSEnginesPtr)
{
	_DASVRPTR_DECLARE(IDaSvrTTSEngines)
	{
	public:
		_DASVRPTR_CONSTRUCT(IDaSvrTTSEngines)
		_DASVRPTR_ASSIGN(IDaSvrTTSEngines)

		__declspec(property(get=_get_Count))									long					Count;
		__declspec(property(get=_get_Item))										IDaSvrTTSEnginePtr		Item[];

		long _get_Count () {long lCount=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Count(&lCount); return lCount;}
		IDaSvrTTSEnginePtr _get_Item (long Index) {IDaSvrTTSEnginePtr lItem; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Item(Index, &lItem); return lItem;}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaSvrSREnginePtr)
{
	_DASVRPTR_DECLARE(IDaSvrSREngine)
	{
	public:
		_DASVRPTR_CONSTRUCT(IDaSvrSREngine)
		_DASVRPTR_ASSIGN(IDaSvrSREngine)

		__declspec(property(get=_get_SRModeID))									_bstr_t					SRModeID;
		__declspec(property(get=_get_DisplayName))								_bstr_t					DisplayName;
		__declspec(property(get=_get_Manufacturer))								_bstr_t					Manufacturer;
		__declspec(property(get=_get_LanguageID))								long					LanguageID;
		__declspec(property(get=_get_LanguageName))								_bstr_t					LanguageName[];
		__declspec(property(get=_get_LanguageIDs))								SAFEARRAY*				LanguageIDs;
		__declspec(property(get=_get_LanguageNames))							SAFEARRAY*				LanguageNames[];

		_bstr_t _get_SRModeID () {BSTR lSRModeID=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_SRModeID(&lSRModeID); return _bstr_t(lSRModeID, false);}
		_bstr_t _get_DisplayName () {BSTR lDisplayName=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_DisplayName(&lDisplayName); return _bstr_t(lDisplayName, false);}
		_bstr_t _get_Manufacturer () {BSTR lManufacturer=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Manufacturer(&lManufacturer); return _bstr_t(lManufacturer, false);}
		long _get_LanguageID () {long lLanguageID=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_LanguageID(&lLanguageID); return lLanguageID;}
		_bstr_t _get_LanguageName (bool EnglishName=true) {BSTR lLanguageName=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_LanguageName(EnglishName?VARIANT_TRUE:VARIANT_FALSE, &lLanguageName); return _bstr_t(lLanguageName, false);}
		SAFEARRAY* _get_LanguageIDs () {SAFEARRAY* lLanguageIDs=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_LanguageIDs(&lLanguageIDs); return lLanguageIDs;}
		SAFEARRAY* _get_LanguageNames (bool EnglishName=true) {SAFEARRAY* lLanguageNames=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_LanguageNames(EnglishName?VARIANT_TRUE:VARIANT_FALSE, &lLanguageNames); return lLanguageNames;}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaSvrSREnginesPtr)
{
	_DASVRPTR_DECLARE(IDaSvrSREngines)
	{
	public:
		_DASVRPTR_CONSTRUCT(IDaSvrSREngines)
		_DASVRPTR_ASSIGN(IDaSvrSREngines)

		__declspec(property(get=_get_Count))									long					Count;
		__declspec(property(get=_get_Item))										IDaSvrSREnginePtr		Item[];

		long _get_Count () {long lCount=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Count(&lCount); return lCount;}
		IDaSvrSREnginePtr _get_Item (long Index) {IDaSvrSREnginePtr lItem; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Item(Index, &lItem); return lItem;}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaSvrCharacter2Ptr)
{
	_DASVRPTR_DECLARE(IDaSvrCharacter2)
	{
	public:
		_DASVRPTR_CONSTRUCT(IDaSvrCharacter2)
		_DASVRPTR_ASSIGN(IDaSvrCharacter2)

		__declspec(property(get=_get_Balloon))									IDaSvrBalloon2Ptr		Balloon;
		__declspec(property(get=_get_Commands))									IDaSvrCommands2Ptr		Commands;
		__declspec(property(get=_get_Style, put=_put_Style))					long					Style;
		__declspec(property(get=_get_HasIcon))									bool					HasIcon;
		__declspec(property(get=_get_IconVisible))								bool					IconVisible;
		__declspec(property(get=_get_IconIdentity, put=_put_IconIdentity))		_bstr_t					IconIdentity;
		__declspec(property(get=_get_IconTip, put=_put_IconTip))				_bstr_t					IconTip;
		__declspec(property(get=_get_TTSEngine))								IDaSvrTTSEnginePtr		TTSEngine[];
		__declspec(property(get=_get_SREngine))									IDaSvrSREnginePtr		SREngine[];
		__declspec(property(get=_get_CharacterID))								long					CharacterID;
		__declspec(property(get=_get_UniqueID))									_bstr_t					UniqueID;
		__declspec(property(get=_get_Name, put=_put_Name))						_bstr_t					Name;
		__declspec(property(get=_get_Description, put=_put_Description))		_bstr_t					Description;
		__declspec(property(get=_get_ExtraData))								_bstr_t					ExtraData;
		__declspec(property(get=_get_FileName))									_bstr_t					FileName;
		__declspec(property(get=_get_FilePath))									_bstr_t					FilePath;
		__declspec(property(get=_get_LanguageID, put=_put_LanguageID))			long					LanguageID;
		__declspec(property(get=_get_TTSModeID, put=_put_TTSModeID))			_bstr_t					TTSModeID;
		__declspec(property(get=_get_SRModeID, put=_put_SRModeID))				_bstr_t					SRModeID;
		__declspec(property(get=_get_Left, put=_put_Left))						short					Left;
		__declspec(property(get=_get_Top, put=_put_Top))						short					Top;
		__declspec(property(get=_get_Width, put=_put_Width))					short					Width;
		__declspec(property(get=_get_Height, put=_put_Height))					short					Height;
		__declspec(property(get=_get_OriginalWidth))							short					OriginalWidth;
		__declspec(property(get=_get_OriginalHeight))							short					OriginalHeight;
		__declspec(property(get=_get_Visible))									bool					Visible;
		__declspec(property(get=_get_ActiveState, put=_put_ActiveState))		ActiveStateType			ActiveState;
		__declspec(property(get=_get_IdleState))								bool					IdleState;
		__declspec(property(get=_get_OtherClientCount))							long					OtherClientCount;
		__declspec(property(get=_get_MoveCause))								MoveCauseType			MoveCause;
		__declspec(property(get=_get_VisibilityCause))							VisibilityCauseType		VisibilityCause;
		__declspec(property(get=_get_ListeningStatus))							ListeningStatusType		ListeningStatus;
		__declspec(property(get=_get_Animations))								SAFEARRAY*				Animations;
		__declspec(property(get=_get_States))									SAFEARRAY*				States;

		IDaSvrBalloon2Ptr _get_Balloon () {IDaSvrBalloon2Ptr lBalloon; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Balloon(&lBalloon); return lBalloon;}
		IDaSvrCommands2Ptr _get_Commands () {IDaSvrCommands2Ptr lCommands; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Commands(&lCommands); return lCommands;}
		long _get_Style () {long lStyle=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Style(&lStyle); return lStyle;}
		HRESULT _put_Style (long Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_Style(Value); else return E_POINTER;}
		bool _get_HasIcon () {VARIANT_BOOL lHasIcon=VARIANT_FALSE; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_HasIcon(&lHasIcon); return lHasIcon?true:false;}
		bool _get_IconVisible () {VARIANT_BOOL lIconVisible=VARIANT_FALSE; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_IconVisible(&lIconVisible); return lIconVisible?true:false;}
		_bstr_t _get_IconIdentity () {BSTR lIconIdentity=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_IconIdentity(&lIconIdentity); return _bstr_t(lIconIdentity, false);}
		HRESULT _put_IconIdentity (const _bstr_t& Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_IconIdentity(Value); else return E_POINTER;}
		_bstr_t _get_IconTip () {BSTR lIconTip=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_IconTip(&lIconTip); return _bstr_t(lIconTip, false);}
		HRESULT _put_IconTip (const _bstr_t& Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_IconTip(Value); else return E_POINTER;}
		IDaSvrTTSEnginePtr _get_TTSEngine (bool GetDefault=true) {IDaSvrTTSEnginePtr lTTSEngine; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_TTSEngine(GetDefault?VARIANT_TRUE:VARIANT_FALSE, &lTTSEngine); return lTTSEngine;}
		IDaSvrSREnginePtr _get_SREngine (bool GetDefault=true) {IDaSvrSREnginePtr lSREngine; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_SREngine(GetDefault?VARIANT_TRUE:VARIANT_FALSE, &lSREngine); return lSREngine;}
		long _get_CharacterID () {long lCharacterID=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_CharacterID(&lCharacterID); return lCharacterID;}
		_bstr_t _get_UniqueID () {BSTR lUniqueID=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_UniqueID(&lUniqueID); return _bstr_t(lUniqueID, false);}
		_bstr_t _get_Name () {BSTR lName=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Name(&lName); return _bstr_t(lName, false);}
		HRESULT _put_Name (const _bstr_t& Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_Name(Value); else return E_POINTER;}
		_bstr_t _get_Description () {BSTR lDescription=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Description(&lDescription); return _bstr_t(lDescription, false);}
		HRESULT _put_Description (const _bstr_t& Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_Description(Value); else return E_POINTER;}
		_bstr_t _get_ExtraData () {BSTR lExtraData=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_ExtraData(&lExtraData); return _bstr_t(lExtraData, false);}
		_bstr_t _get_FileName () {BSTR lFileName=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_FileName(&lFileName); return _bstr_t(lFileName, false);}
		_bstr_t _get_FilePath () {BSTR lFilePath=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_FilePath(&lFilePath); return _bstr_t(lFilePath, false);}
		long _get_LanguageID () {long lLanguageID=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_LanguageID(&lLanguageID); return lLanguageID;}
		HRESULT _put_LanguageID (long Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_LanguageID(Value); else return E_POINTER;}
		_bstr_t _get_TTSModeID () {BSTR lTTSModeID=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_TTSModeID(&lTTSModeID); return _bstr_t(lTTSModeID, false);}
		HRESULT _put_TTSModeID (const _bstr_t& Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_TTSModeID(Value); else return E_POINTER;}
		_bstr_t _get_SRModeID () {BSTR lSRModeID=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_SRModeID(&lSRModeID); return _bstr_t(lSRModeID, false);}
		HRESULT _put_SRModeID (const _bstr_t& Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_SRModeID(Value); else return E_POINTER;}
		short _get_Left () {short lLeft=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Left(&lLeft); return lLeft;}
		HRESULT _put_Left (short Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_Left(Value); else return E_POINTER;}
		short _get_Top () {short lTop=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Top(&lTop); return lTop;}
		HRESULT _put_Top (short Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_Top(Value); else return E_POINTER;}
		short _get_Width () {short lWidth=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Width(&lWidth); return lWidth;}
		HRESULT _put_Width (short Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_Width(Value); else return E_POINTER;}
		short _get_Height () {short lHeight=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Height(&lHeight); return lHeight;}
		HRESULT _put_Height (short Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_Height(Value); else return E_POINTER;}
		short _get_OriginalWidth () {short lOriginalWidth=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_OriginalWidth(&lOriginalWidth); return lOriginalWidth;}
		short _get_OriginalHeight () {short lOriginalHeight=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_OriginalHeight(&lOriginalHeight); return lOriginalHeight;}
		bool _get_Visible () {VARIANT_BOOL lVisible=VARIANT_FALSE; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Visible(&lVisible); return lVisible?true:false;}
		ActiveStateType _get_ActiveState () {ActiveStateType lActiveState=ActiveState_Inactive; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_ActiveState(&lActiveState); return lActiveState;}
		HRESULT _put_ActiveState (ActiveStateType Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_ActiveState(Value); else return E_POINTER;}
		bool _get_IdleState () {VARIANT_BOOL lIdleState=VARIANT_FALSE; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_IdleState(&lIdleState); return lIdleState?true:false;}
		long _get_OtherClientCount () {long lOtherClientCount=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_OtherClientCount(&lOtherClientCount); return lOtherClientCount;}
		MoveCauseType _get_MoveCause () {MoveCauseType lMoveCause=MoveCause_NeverMoved; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_MoveCause(&lMoveCause); return lMoveCause;}
		VisibilityCauseType _get_VisibilityCause () {VisibilityCauseType lVisibilityCause=VisibilityCause_NeverShown; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_VisibilityCause(&lVisibilityCause); return lVisibilityCause;}
		ListeningStatusType _get_ListeningStatus () {ListeningStatusType lListeningStatus=ListeningStatus_Error; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_ListeningStatus(&lListeningStatus); return lListeningStatus;}
		SAFEARRAY* _get_Animations () {SAFEARRAY* lAnimations=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Animations(&lAnimations); return lAnimations;}
		SAFEARRAY* _get_States () {SAFEARRAY* lStates=NULL; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_States(&lStates); return lStates;}
	};
}

/////////////////////////////////////////////////////////////////////////////

__if_not_exists(IDaServer2Ptr)
{
	_DASVRPTR_DECLARE(IDaServer2)
	{
	public:
		_DASVRPTR_CONSTRUCT(IDaServer2)
		_DASVRPTR_ASSIGN(IDaServer2)

		__declspec(property(get=_get_CharacterStyle, put=_put_CharacterStyle))	long						CharacterStyle;
		__declspec(property(get=_get_Character))								IDaSvrCharacter2Ptr			Character[];
		__declspec(property(get=_get_CharacterFiles))							IDaSvrCharacterFilesPtr		CharacterFiles;
		__declspec(property(get=_get_TTSEngines))								IDaSvrTTSEnginesPtr			TTSEngines;
		__declspec(property(get=_get_SREngines))								IDaSvrSREnginesPtr			SREngines;
		__declspec(property(get=_get_PropertySheet))							IDaSvrPropertySheet2Ptr		PropertySheet;
		__declspec(property(get=_get_CommandsWindow))							IDaSvrCommandsWindow2Ptr	CommandsWindow;
		__declspec(property(get=_get_Settings))									IDaSvrSettingsPtr			Settings;

		long _get_CharacterStyle () {long lCharacterStyle=0; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_CharacterStyle(&lCharacterStyle); return lCharacterStyle;}
		HRESULT _put_CharacterStyle (long Value) {if (_DASVRPTR_CHKNULL) return GetInterfacePtr()->put_CharacterStyle(Value); else return E_POINTER;}
		IDaSvrCharacter2Ptr _get_Character (long CharacterID) {IDaSvrCharacter2Ptr lCharacter; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Character(CharacterID, &lCharacter); return lCharacter;}
		IDaSvrCharacterFilesPtr _get_CharacterFiles () {IDaSvrCharacterFilesPtr lCharacterFiles; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_CharacterFiles(&lCharacterFiles); return lCharacterFiles;}
		IDaSvrTTSEnginesPtr _get_TTSEngines () {IDaSvrTTSEnginesPtr lTTSEngines; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_TTSEngines(&lTTSEngines); return lTTSEngines;}
		IDaSvrSREnginesPtr _get_SREngines () {IDaSvrSREnginesPtr lSREngines; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_SREngines(&lSREngines); return lSREngines;}
		IDaSvrPropertySheet2Ptr _get_PropertySheet () {IDaSvrPropertySheet2Ptr lPropertySheet; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_PropertySheet(&lPropertySheet); return lPropertySheet;}
		IDaSvrCommandsWindow2Ptr _get_CommandsWindow () {IDaSvrCommandsWindow2Ptr lCommandsWindow; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_CommandsWindow(&lCommandsWindow); return lCommandsWindow;}
		IDaSvrSettingsPtr _get_Settings () {IDaSvrSettingsPtr lSettings; if (_DASVRPTR_CHKNULL) GetInterfacePtr()->get_Settings(&lSettings); return lSettings;}
	};
}
/////////////////////////////////////////////////////////////////////////////

#undef	_DASVRPTR_DECLARE
#undef	_DASVRPTR_CONSTRUCT
#undef	_DASVRPTR_ASSIGN
#undef	_DASVRPTR_CHKNULL

/////////////////////////////////////////////////////////////////////////////
