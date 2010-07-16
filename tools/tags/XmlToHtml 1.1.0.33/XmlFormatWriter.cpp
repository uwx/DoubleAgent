#include "stdafx.h"
#include "XmlFormatWriter.h"

using namespace System::Collections;
using namespace System::Collections::Specialized;
using namespace System::Xml;

/////////////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG
//#define	_DEBUG_XML_WRITE			LogNormal|LogHighVolume
#endif

/////////////////////////////////////////////////////////////////////////////
namespace DoubleAgent {
namespace XmlToHtml {
/////////////////////////////////////////////////////////////////////////////

XmlFormatWriter::XmlFormatWriter ()
{
	mEndParagraphs = false;
	mIndentLines = false;
	mKeepComments = false;
	mMaxLineLength = 0;
}

XmlFormatWriter::~XmlFormatWriter ()
{
}

/////////////////////////////////////////////////////////////////////////////

bool XmlFormatWriter::WriteDocument (XmlDocument^ pXmlDocument)
{
	bool	lRet = false;

	//pXmlDocument->PreserveWhitespace = false;
	//pXmlDocument->Normalize ();

	System::IO::File::Delete (mOutputPath);

	if	(
			(mNodeReader = gcnew XmlNodeReader (pXmlDocument->DocumentElement))
		&&	(mTextWriter = gcnew XmlTextWriter (mOutputPath, Encoding::UTF8))
		)
	{
		try
		{
			mNewLinesWritten = 0;
			mLineStart = 0;
			mTextValue = String::Empty;

			mTextWriter->Namespaces = true;
			if	(mIndentLines)
			{
				mTextWriter->Formatting	= Formatting::Indented;
				mTextWriter->Indentation = 1;
				mTextWriter->IndentChar = '\t';
			}
			else
			{
				mTextWriter->Formatting	= Formatting::None;
				mTextWriter->Indentation = 0;
			}

#ifndef	_DEBUG_XML_WRITE
			mTextWriter->WriteStartDocument ();
#endif
			while	(mNodeReader->Read ())
			{
				if	(
						(mNodeReader->NodeType == XmlNodeType::Text)
					&&	(mNodeReader->XmlSpace != XmlSpace::Preserve)
					)
				{
					mTextValue = mTextValue + mNodeReader->Value;
				}
				else
				{
					if	(!String::IsNullOrEmpty (mTextValue))
					{
#ifdef	_DEBUG_XML_WRITE
						LogMessage (_DEBUG_XML_WRITE, _T("%d%sText  [%s]"), mNodeReader->Depth, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), _B(mTextValue->Replace("\t","\\t")->Replace("\n","\\n")->Replace("\r","\\r")));
#endif
						WriteText (mTextValue);
						mTextValue = String::Empty;
						mNewLinesWritten = 0;
					}
					if	(mNodeReader->NodeType == XmlNodeType::Text)
					{
#ifdef	_DEBUG_XML_WRITE
						LogMessage (_DEBUG_XML_WRITE, _T("%d%sText* [%s]"), mNodeReader->Depth, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), _B(mNodeReader->Value->Replace("\t","\\t")->Replace("\n","\\n")->Replace("\r","\\r")));
#endif
						mTextWriter->WriteString (mNodeReader->Value);
						mNewLinesWritten = 0;
					}
					else
					if	(
							(mNodeReader->NodeType == XmlNodeType::Whitespace)
						||	(mNodeReader->NodeType == XmlNodeType::SignificantWhitespace)
						)
					{
						if	(
								(mNodeReader->Value == "\n")
							||	(mNodeReader->Value == "\r\n")
							)
						{
							if	(mNewLinesWritten > 0)
							{
								mNewLinesWritten--;
							}
							else
							{
								mTextWriter->WriteWhitespace (mNodeReader->Value);
								mNewLinesWritten++;
							}
							StartLine ();
						}
						else
						{
							mTextWriter->WriteWhitespace (mNodeReader->Value);
							mNewLinesWritten = 0;
						}
					}
					else
					if	(
							(mNodeReader->NodeType == XmlNodeType::Element)
						&&	(mNodeReader->IsEmptyElement)
						&&	(mNodeReader->Name == "para")
						)
					{
						// Special case - treats empty para in intellisense as a line break.
						mTextWriter->WriteStartElement (mNodeReader->Name);
						mTextWriter->WriteEndElement ();
					}
					else
					if	(mNodeReader->NodeType == XmlNodeType::Element)
					{
						if	(mNodeReader->XmlSpace != XmlSpace::Preserve)
						{
							EndLine (NewLinesBefore (mNodeReader->Name));
							EndNode ();
							WriteIndent ();
						}
						else
						if	(mNodeReader->Name == "para")
						{
							EndLine (NewLinesBefore (mNodeReader->Name));
						}

#ifdef	_DEBUG_XML_WRITE
						if	(mNodeReader->Name == "member")
						{
							LogMessage (_DEBUG_XML_WRITE, _T("%d%sStart [%s] [%s]"), mNodeReader->Depth, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), _B(mNodeReader->Name), _B(mNodeReader[0]));
						}
						else
						{
							LogMessage (_DEBUG_XML_WRITE, _T("%d%sStart [%s]"), mNodeReader->Depth, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), _B(mNodeReader->Name));
						}
#endif
						mTextWriter->WriteStartElement (mNodeReader->Prefix, mNodeReader->LocalName, mNodeReader->NamespaceURI);
						mTextWriter->WriteAttributes (mNodeReader, false);
						mNewLinesWritten = 0;

						if	(mNodeReader->IsEmptyElement)
						{
#ifdef	_DEBUG_XML_WRITE
							LogMessage (_DEBUG_XML_WRITE, _T("%d%sEnd   [%s]"), mNodeReader->Depth, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), _B(mNodeReader->LocalName));
#endif
							mTextWriter->WriteEndElement ();

							if	(mNodeReader->XmlSpace != XmlSpace::Preserve)
							{
								EndLine (NewLinesAfter (mNodeReader->Name));
							}
						}
						else
						{
							EndNode ();
						}
					}
					else
					if	(mNodeReader->NodeType == XmlNodeType::EndElement)
					{
						if	(
								(mEndParagraphs)
							&&	(mNodeReader->XmlSpace != XmlSpace::Preserve)
							&&	(mNodeReader->Name == "para")
							)
						{
							mNewLinesWritten = 0;
							EndLine (1);
						}
						if	(mNodeReader->XmlSpace != XmlSpace::Preserve)
						{
							EndNode ();
							WriteIndent (mNodeReader->Depth);
						}
#ifdef	_DEBUG_XML_WRITE
						LogMessage (_DEBUG_XML_WRITE, _T("%d%sEnd   [%s]"), mNodeReader->Depth, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), _B(mNodeReader->Name));
#endif
						mTextWriter->WriteFullEndElement ();
						mNewLinesWritten = 0;

						if	(mNodeReader->XmlSpace != XmlSpace::Preserve)
						{
							EndLine (NewLinesAfter (mNodeReader->Name));
						}
						else
						if	(mNodeReader->Name == "para")
						{
							EndLine (NewLinesAfter (mNodeReader->Name));
						}
					}
					else
					if	(mNodeReader->NodeType == XmlNodeType::Comment)
					{
						if	(mKeepComments)
						{
							EndLine (1);
							WriteIndent ();
							mTextWriter->WriteComment (mNodeReader->Value);
							mNewLinesWritten = 0;
							EndLine (1);
						}
					}
					else
					{
#ifdef	_DEBUG_XML_WRITE
						LogMessage (_DEBUG_XML_WRITE, _T("%d%sNode  [%d] [%s] [%s]"), mNodeReader->Depth, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), (int)mNodeReader->NodeType, _B(mNodeReader->Name), _B(mNodeReader->Value->Replace("\t","\\t")->Replace("\n","\\n")->Replace("\r","\\r")));
#endif
						mTextWriter->WriteNode (mNodeReader, true);
					}
				}
			}

#ifndef	_DEBUG_XML_WRITE
			mTextWriter->WriteEndDocument ();
#endif
			mTextWriter->Close ();
			lRet = true;
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool XmlFormatWriter::StartLine ()
{
	bool	lRet = false;
	
	if	(mMaxLineLength > 0)
	{
		mTextWriter->Flush ();
		mLineStart = mTextWriter->BaseStream->Position;
		lRet = true;
	}
	return lRet;
}

bool XmlFormatWriter::EndLine (int pWantLines)
{
	bool	lRet = false;
	
	if	(pWantLines <= 0)
	{
		mNewLinesWritten = 0;
	}
	else
	{
		while (mNewLinesWritten < pWantLines)
		{
			mTextWriter->WriteWhitespace ("\r\n");
			mNewLinesWritten++;
			lRet = true;
		}
	}
	if	(lRet)
	{
		StartLine ();
	}
	return lRet;
}

bool XmlFormatWriter::EndNode ()
{
	bool	lRet = false;

	if	(
			(mNewLinesWritten <= 0)
		&&	(mMaxLineLength > 0)
		&&	(mNodeReader->XmlSpace != XmlSpace::Preserve)
		)
	{
		mTextWriter->Flush ();
		if	((long)(mTextWriter->BaseStream->Position - mLineStart) >= mMaxLineLength)
		{
			lRet = EndLine (1);
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool XmlFormatWriter::WriteIndent ()
{
	return WriteIndent (mNodeReader->Depth);
}

bool XmlFormatWriter::WriteIndent (int pIndent)
{
	bool	lRet = false;
	
	if	(
			(pIndent > 0)
		&&	(mNewLinesWritten > 0)
		&&	(mTextWriter->Indentation > 0)
		)
	{
		String^	lIndent = gcnew String (mTextWriter->IndentChar, mTextWriter->Indentation);
		
		while (pIndent-- > 0)
		{
			mTextWriter->WriteWhitespace (lIndent);
		}
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool XmlFormatWriter::WriteText (String^ pText)
{
	bool	lRet = false;

	if	(!String::IsNullOrEmpty (pText))
	{
		String^	lText = pText;

		while (lText->IndexOf ('\r') >= 0)
		{
			lText = lText->Replace ('\r', '\n');
		}
		while (lText->IndexOf ("\n\t") >= 0)
		{
			lText = lText->Replace ("\n\t", "\n");
		}
		while (lText->IndexOf ("\n\n") >= 0)
		{
			lText = lText->Replace ("\n\n", "\n");
		}
		while (lText->IndexOf ("\n ") >= 0)
		{
			lText = lText->Replace ("\n ", "\n");
		}
		while (lText->IndexOf (" \n") >= 0)
		{
			lText = lText->Replace (" \n", "\n");
		}
		lText = lText->Trim (String("\t\r\n").ToCharArray());
		while (lText->IndexOf ('\n') > 0)
		{
			lText = lText->Replace ('\n', ' ');
		}
		mTextWriter->WriteString (lText);
#if	FALSE	
		StringCollection^	lSplit = gcnew StringCollection;
		array<String^>^		lJoin;
		String^				lText;
		array<Char>^		lWhitespaceChars = String(" \t\r\n").ToCharArray();
		
		lSplit->AddRange (pText->Split (lWhitespaceChars));
		for (int lNdx = lSplit->Count-1; lNdx >= 0; lNdx--)
		{
			if	(String::IsNullOrEmpty (lSplit [lNdx]))
			{
				lSplit->RemoveAt (lNdx);
			}
		}
		if	(pText->StartsWith (" "))
		{
			lSplit->Insert (0, "");
		}
		if	(pText->LastIndexOfAny (lWhitespaceChars) == pText->Length-1)
		{
			lSplit->Add ("");
		}
		lJoin = gcnew array<String^> (lSplit->Count);
		lSplit->CopyTo (lJoin, 0);

		if	(
				(lJoin->Length > 0)		
			&&	(mMaxLineLength > 0)
			&&	(mNodeReader->XmlSpace != XmlSpace::Preserve)
			)
		{
			int	lIndent = mNodeReader->Depth;
			int	lLineLength;
			int	lStartNdx = 0;
			int	lEndNdx;

			mTextWriter->Flush ();
			lLineLength = (long) (mTextWriter->BaseStream->Position - mLineStart);
			
			for	(lEndNdx = 0; lEndNdx < lJoin->Length; lEndNdx++)
			{
				if	(
						(lEndNdx > lStartNdx)
					&&	(lLineLength >= mMaxLineLength)
					)
				{
					if	(WriteIndent (lIndent))
					{
						lLineLength = mNodeReader->Depth;
					}
					lText = String::Join (" ", lJoin, lStartNdx, lEndNdx-lStartNdx);
					mTextWriter->WriteString (lText->Trim());
					mTextWriter->WriteWhitespace ("\r\n");
					lStartNdx = lEndNdx;
					lLineLength = 0;
					mNewLinesWritten = 1;
					StartLine ();
				}
				lLineLength += lJoin [lEndNdx]->Length+1;
			}
			if	(lEndNdx > lStartNdx)
			{
				WriteIndent (lIndent);
				lText = String::Join (" ", lJoin, lStartNdx, lEndNdx-lStartNdx);
				mTextWriter->WriteString (lText);
				mNewLinesWritten = 0;
			}
		}
		else
		{
			lText = String::Join (" ", lJoin);
			mTextWriter->WriteString (lText);
		}
#endif		
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

int XmlFormatWriter::NewLinesBefore (String^ pElementName)
{
	if	(pElementName == "member")
	{
		return 2;
	}
	else
	if	(
			(pElementName == "doc")
		||	(pElementName == "assembly")
		||	(pElementName == "members")
		||	(pElementName == "summary")
		||	(pElementName == "syntax")
		||	(pElementName == "remarks")
		||	(pElementName == "param")
		||	(pElementName == "returns")
		||	(pElementName == "value")
		||	(pElementName == "seealso")
		||	(pElementName == "para")
		||	(pElementName == "note")
		||	(pElementName == "list")
		||	(pElementName == "item")
		||	(pElementName == "term")
		||	(pElementName == "description")
		)
	{
		return 1;
	}
	return 0;
}

int XmlFormatWriter::NewLinesAfter (String^ pElementName)
{
	if	(pElementName == "member")
	{
		return 2;
	}
	else
	if	(
			(pElementName == "doc")
		||	(pElementName == "assembly")
		||	(pElementName == "members")
		||	(pElementName == "summary")
		||	(pElementName == "syntax")
		||	(pElementName == "remarks")
		||	(pElementName == "param")
		||	(pElementName == "returns")
		||	(pElementName == "value")
		||	(pElementName == "seealso")
		||	(pElementName == "para")
		||	(pElementName == "note")
		||	(pElementName == "list")
		||	(pElementName == "item")
		||	(pElementName == "term")
		||	(pElementName == "description")
		||	(pElementName == "bookmark")
		)
	{
		return 1;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
} // namespace XmlToHtml
} // namespace DoubleAgent
