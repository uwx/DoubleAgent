#include "stdafx.h"
#include "XmlFormatWriter.h"

using namespace System::Collections;
using namespace System::Collections::Specialized;
using namespace System::Xml;

/////////////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG
//#define	_DEBUG_XML_WRITE			LogNormal|LogHighVolume
//#define	_DEBUG_XML_WRITE_EX			LogNormal|LogHighVolume
#endif

/////////////////////////////////////////////////////////////////////////////
namespace DoubleAgent {
namespace XmlToHtml {
/////////////////////////////////////////////////////////////////////////////

XmlFormatWriter::XmlFormatWriter ()
{
	mEndParagraphs = false;
	mIndentLines = false;
	mKeepDocType = false;
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

	System::IO::File::Delete (mOutputPath);

	if	(
			(mNodeReader = gcnew XmlNodeReader (pXmlDocument->DocumentElement))
		&&	(mTextWriter = gcnew XmlTextWriter (mOutputPath, Encoding::UTF8))
		)
	{
		try
		{
			mElementName = gcnew System::Collections::Generic::List<String^>;
			PutElementName (String::Empty, mNodeReader->Depth);
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
			if	(
					(mKeepDocType)
				&&	(pXmlDocument->DocumentType)
				)
			{
				pXmlDocument->DocumentType->WriteTo (mTextWriter);
			}

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
						if	(mNodeReader->XmlSpace != XmlSpace::Preserve)
						{
							EndLine (0);
						}
						WriteText (mTextValue);
						mTextValue = String::Empty;
						mNewLinesWritten = 0;
					}
					if	(mNodeReader->NodeType == XmlNodeType::Text)
					{
#ifdef	_DEBUG_XML_WRITE
						LogMessage (_DEBUG_XML_WRITE, _T("%d [%d %d]%sText *     [%s]"), (int)mNodeReader->XmlSpace, mNodeReader->Depth, mElementName->Count, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), _B(DebugStr(mNodeReader->Value)));
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
						String^	lWhiteSpace = mNodeReader->Value->TrimStart (' ');

						if	(mNodeReader->XmlSpace == XmlSpace::Preserve)
						{
#ifdef	_DEBUG_XML_WRITE_EX
							LogMessage (_DEBUG_XML_WRITE_EX, _T("%d [%d %d]%sWhiteSpace [%s] [%d]"), (int)mNodeReader->XmlSpace, mNodeReader->Depth, mElementName->Count, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), _B(DebugStr(mNodeReader->Value)), (int)mNodeReader->NodeType);
#endif
							mTextWriter->WriteWhitespace (mNodeReader->Value);
							mNewLinesWritten = 0;
						}
						else
						if	(
								(lWhiteSpace->StartsWith ("\n"))
							||	(lWhiteSpace->StartsWith ("\r\n"))
							)
						{
#ifdef	_DEBUG_XML_WRITE_EX
							LogMessage (_DEBUG_XML_WRITE_EX, _T("%d [%d %d]%sNewLine    [%s] [%d]"), (int)mNodeReader->XmlSpace, mNodeReader->Depth, mElementName->Count, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), _B(DebugStr(mNodeReader->Value)), (int)mNodeReader->NodeType);
#endif
							if	(mNewLinesWritten <= 0)
							{
								mNewLinesWritten = -1;
							}
						}
						else
						if	(mNodeReader->NodeType == XmlNodeType::SignificantWhitespace)
						{
#ifdef	_DEBUG_XML_WRITE_EX
							LogMessage (_DEBUG_XML_WRITE_EX, _T("%d [%d %d]%sWhiteSpace [%s] [%d]"), (int)mNodeReader->XmlSpace, mNodeReader->Depth, mElementName->Count, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), _B(DebugStr(mNodeReader->Value)), (int)mNodeReader->NodeType);
#endif
							mTextWriter->WriteWhitespace (mNodeReader->Value);
							mNewLinesWritten = 0;
						}
						else
						{
#ifdef	_DEBUG_XML_WRITE_EX
							LogMessage (_DEBUG_XML_WRITE_EX, _T("%d [%d %d]%sWhiteSpace [%s] [%d]"), (int)mNodeReader->XmlSpace, mNodeReader->Depth, mElementName->Count, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), _B(DebugStr(mNodeReader->Value)), (int)mNodeReader->NodeType);
#endif
							mTextValue += " ";
						}
					}
					else
					if	(
							(mEndParagraphs)
						&&	(mNodeReader->NodeType == XmlNodeType::Element)
						&&	(mNodeReader->IsEmptyElement)
						&&	(mNodeReader->Name == "para")
						)
					{
						// Special case - treats empty para in intellisense as a line break.
#ifdef	_DEBUG_XML_WRITE
						LogMessage (_DEBUG_XML_WRITE, _T("%d [%d %d]%sBr"), (int)mNodeReader->XmlSpace, mNodeReader->Depth, mElementName->Count, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)));
#endif
						mTextWriter->WriteStartElement (mNodeReader->Name);
						mTextWriter->WriteEndElement ();
						PutElementName (mNodeReader->Name, mNodeReader->Depth);
						PutElementDepth (mNodeReader->Depth-1);
					}
					else
					if	(mNodeReader->NodeType == XmlNodeType::Element)
					{
						if	(mNodeReader->XmlSpace != XmlSpace::Preserve)
						{
							EndLine (NewLinesBefore (mNodeReader->Name));
							WriteIndent ();
						}

#ifdef	_DEBUG_XML_WRITE
						if	(mNodeReader->Name == "member")
						{
							LogMessage (_DEBUG_XML_WRITE, _T("%d [%d %d]%sStart      [%s] [%s]"), (int)mNodeReader->XmlSpace, mNodeReader->Depth, mElementName->Count, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), _B(mNodeReader->Name), _B(mNodeReader[0]));
						}
						else
						{
							LogMessage (_DEBUG_XML_WRITE, _T("%d [%d %d]%sStart      [%s]"), (int)mNodeReader->XmlSpace, mNodeReader->Depth, mElementName->Count, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), _B(mNodeReader->Name));
						}
#endif
						mTextWriter->WriteStartElement (mNodeReader->Prefix, mNodeReader->LocalName, mNodeReader->NamespaceURI);
						mTextWriter->WriteAttributes (mNodeReader, false);
						PutElementName (mNodeReader->Name, mNodeReader->Depth);
						mNewLinesWritten = 0;

						if	(mNodeReader->IsEmptyElement)
						{
#ifdef	_DEBUG_XML_WRITE
							LogMessage (_DEBUG_XML_WRITE, _T("%d [%d %d]%sEnd        [%s]"), (int)mNodeReader->XmlSpace, mNodeReader->Depth, mElementName->Count, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), _B(mNodeReader->LocalName));
#endif
							mTextWriter->WriteEndElement ();
							if	(mNodeReader->XmlSpace != XmlSpace::Preserve)
							{
								EndLine (NewLinesAfter (mNodeReader->Name));
							}
						}
						else
						if	(mNodeReader->XmlSpace != XmlSpace::Preserve)
						{
							if	(
									(mIndentLines)
								&&	(mNodeReader->Name == "para")
								)
							{
								EndLine (1);
								WriteIndent (mNodeReader->Depth+1);
								mNewLinesWritten = 0;
							}
						}
					}
					else
					if	(mNodeReader->NodeType == XmlNodeType::EndElement)
					{
						if	(mNodeReader->XmlSpace != XmlSpace::Preserve)
						{
							if	(
									(
										(mEndParagraphs)
									||	(mIndentLines)
									)
								&&	(mNodeReader->Name == "para")
								)
							{
								mNewLinesWritten = 0;
								EndLine (1);
							}
							WriteIndent ();
						}
#ifdef	_DEBUG_XML_WRITE
						LogMessage (_DEBUG_XML_WRITE, _T("%d [%d %d]%sEnd        [%s]"), (int)mNodeReader->XmlSpace, mNodeReader->Depth, mElementName->Count, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), _B(mNodeReader->Name));
#endif
						mTextWriter->WriteFullEndElement ();
						PutElementName (mNodeReader->Name, mNodeReader->Depth);
						PutElementDepth (mNodeReader->Depth);
						mNewLinesWritten = 0;

						if	(mNodeReader->XmlSpace != XmlSpace::Preserve)
						{
							EndLine (NewLinesAfter (mNodeReader->Name));
						}
					}
					else
					if	(mNodeReader->NodeType == XmlNodeType::EntityReference)
					{
						mTextWriter->WriteEntityRef (mNodeReader->Name);
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
						LogMessage (_DEBUG_XML_WRITE, _T("%d [%d %d]%sNode       [%d] [%s] [%s]"), (int)mNodeReader->XmlSpace, mNodeReader->Depth, mElementName->Count, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), (int)mNodeReader->NodeType, _B(mNodeReader->Name), _B(DebugStr(mNodeReader->Value)));
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

void XmlFormatWriter::PutElementDepth (int pElementDepth)
{
	pElementDepth = max (pElementDepth, 0);

	while (mElementName->Count > pElementDepth)
	{
		mElementName->RemoveAt (mElementName->Count-1);
	}
	if	(mElementName->Count < pElementDepth)
	{
		PutElementName (String::Empty, pElementDepth);
	}
}

void XmlFormatWriter::PutElementName (String^ pElementName, int pElementDepth)
{
	pElementDepth = max (pElementDepth, 1);

	while (mElementName->Count >= pElementDepth)
	{
		mElementName->RemoveAt (mElementName->Count-1);
	}
	while	(mElementName->Count < pElementDepth-1)
	{
		mElementName->Add (String::Empty);
	}
	mElementName->Add (pElementName);
}

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
		if	(mNewLinesWritten < 0)
		{
#ifdef	_DEBUG_XML_WRITE_EX
			LogMessage (_DEBUG_XML_WRITE_EX, _T("%d [%d %d]%sEndLine    [ ]"), (int)mNodeReader->XmlSpace, mNodeReader->Depth, mElementName->Count, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)));
#endif
			mTextWriter->WriteWhitespace (" ");
		}
		mNewLinesWritten = 0;
	}
	else
	{
		mNewLinesWritten = max (mNewLinesWritten, 0);
		while (mNewLinesWritten < pWantLines)
		{
			mTextWriter->WriteWhitespace ("\r\n");
			mNewLinesWritten++;
#ifdef	_DEBUG_XML_WRITE_EX
			LogMessage (_DEBUG_XML_WRITE_EX, _T("%d [%d %d]%sEndLine    [\\r\\n] [%d of %d]"), (int)mNodeReader->XmlSpace, mNodeReader->Depth, mElementName->Count, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), mNewLinesWritten, pWantLines);
#endif
			lRet = true;
		}
	}
	if	(lRet)
	{
		StartLine ();
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

#ifdef	_DEBUG_XML_WRITE_EX
		LogMessage (_DEBUG_XML_WRITE_EX, _T("%d [%d %d]%sIndent     [%s] [%d]"), (int)mNodeReader->XmlSpace, mNodeReader->Depth, mElementName->Count, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), _B(DebugStr(lIndent)), pIndent);
#endif
		while (pIndent-- > 0)
		{
			mTextWriter->WriteWhitespace (lIndent);
		}
		mNewLinesWritten = 0;
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

		if	(
				(
					(mNodeReader->Depth < mElementName->Count)
				||	(
						(mNodeReader->Depth == mElementName->Count)
					&&	(mNodeReader->NodeType != XmlNodeType::EndElement)
					)
				)
			&&	(
					(pText->StartsWith ("\r"))
				||	(pText->StartsWith ("\n"))
				||	(pText->StartsWith (" "))
				)
			&&	(!lText->StartsWith (" "))
			)
		{
			lText = String::Concat (" ", lText);
		}
		if	(
				(mNodeReader->Depth == mElementName->Count)
			&&	(mNodeReader->NodeType == XmlNodeType::Element)
			&&	(
					(pText->EndsWith ("\r"))
				||	(pText->EndsWith ("\n"))
				||	(pText->EndsWith ("\t"))
				||	(pText->EndsWith (" "))
				)
			&&	(!lText->EndsWith (" "))
			)
		{
			lText = String::Concat (lText, " ");
		}

#ifdef	_DEBUG_XML_WRITE
		LogMessage (_DEBUG_XML_WRITE, _T("%d [%d %d]%sText       [%s] in [%s] before [%s (%d)]"), (int)mNodeReader->XmlSpace, mNodeReader->Depth, mElementName->Count, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), _B(DebugStr(pText)), _B(mElementName[min(mNodeReader->Depth,mElementName->Count)-1]), _B(mNodeReader->Name), (int)mNodeReader->NodeType);
#endif

		if	(
				(mMaxLineLength > 0)
			&&	(mNodeReader->XmlSpace != XmlSpace::Preserve)
#if	TRUE
			&&	(
					(mNodeReader->Depth != mElementName->Count)
				||	(mNodeReader->NodeType != XmlNodeType::EndElement)
				||	(
						(mNodeReader->Name != "a")
					&&	(mNodeReader->Name != "keyword")
					&&	(mNodeReader->Name != "token")
					&&	(mNodeReader->Name != "typename")
					&&	(mNodeReader->Name != "glossary")
					)
				)
#endif
			)
		{
			int	lLineStart;
			int	lLineEnd;

			mTextWriter->Flush ();
			lLineStart = (int)(mTextWriter->BaseStream->Position - mLineStart);
			lLineEnd = lLineStart + lText->Length;

#ifdef	_DEBUG_XML_WRITE
			LogMessage (_DEBUG_XML_WRITE, _T("%d [%d %d]%s  as       [%s] from [%d] to [%d]"), (int)mNodeReader->XmlSpace, mNodeReader->Depth, mElementName->Count, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), _B(DebugStr(lText)), lLineStart, lLineEnd);
#endif
			if	(lLineEnd >= mMaxLineLength)
			{
				array<String^>^	lWords = lText->Split (String(" ").ToCharArray(), StringSplitOptions::None);
				int				lStartNdx = 0;
				int				lEndNdx;

				for (lEndNdx = 0; lEndNdx < lWords->Length; lEndNdx++)
				{
					if	(lLineStart >= mMaxLineLength)
					{
#ifdef	_DEBUG_XML_WRITE
						LogMessage (_DEBUG_XML_WRITE, _T("%d [%d %d]%s    line   [%s]"), (int)mNodeReader->XmlSpace, mNodeReader->Depth, mElementName->Count, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), _B(String::Join (" ", lWords, lStartNdx, lEndNdx-lStartNdx+1)));
#endif
						if	(lStartNdx > 0)
						{
							EndLine (1);
							StartLine ();
						}
						mTextWriter->WriteString (String::Join (" ", lWords, lStartNdx, lEndNdx-lStartNdx+1));
						mNewLinesWritten = 0;
						lLineStart = mNodeReader->Depth;
						lStartNdx = lEndNdx+1;
					}
					lLineStart += lWords[lEndNdx]->Length+1;
				}
				if	(lEndNdx > lStartNdx)
				{
#ifdef	_DEBUG_XML_WRITE
					LogMessage (_DEBUG_XML_WRITE, _T("%d [%d %d]%s    line   [%s]"), (int)mNodeReader->XmlSpace, mNodeReader->Depth, mElementName->Count, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), _B(String::Join (" ", lWords, lStartNdx, lEndNdx-lStartNdx)));
#endif
					if	(lStartNdx > 0)
					{
						EndLine (1);
						StartLine ();
					}
					mTextWriter->WriteString (String::Join (" ", lWords, lStartNdx, lEndNdx-lStartNdx));
					mNewLinesWritten = 0;
				}
			}
			else
			{
				mTextWriter->WriteString (lText);
			}
		}
		else
		{
#ifdef	_DEBUG_XML_WRITE
			LogMessage (_DEBUG_XML_WRITE, _T("%d [%d %d]%s  as       [%s]"), (int)mNodeReader->XmlSpace, mNodeReader->Depth, mElementName->Count, _B(gcnew String('.',max(mNodeReader->Depth,0)*2)), _B(DebugStr(lText)));
#endif
			mTextWriter->WriteString (lText);
		}

#if	TRUE
		if	(
				(mIndentLines)
			&&	(mNodeReader->XmlSpace != XmlSpace::Preserve)
			&&	(mNodeReader->Depth == mElementName->Count)
			&&	(mNodeReader->NodeType == XmlNodeType::Element)
			&&	(mNodeReader->Name == "see")
			&&	(mElementName[mElementName->Count-1] == "see")
			&&	(mElementName[mElementName->Count-2] == "description")
			&&	(lText->Trim() == ",")
			)
		{
			EndLine (1);
			StartLine ();
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
		)
	{
		return 1;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
} // namespace XmlToHtml
} // namespace DoubleAgent
