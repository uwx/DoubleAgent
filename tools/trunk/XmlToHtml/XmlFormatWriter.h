#pragma once

namespace DoubleAgent {
namespace XmlToHtml {
/////////////////////////////////////////////////////////////////////////////

ref class XmlFormatWriter
{
public:
	XmlFormatWriter ();
	~XmlFormatWriter ();

public:
	System::String^	mOutputPath;
	bool			mEndParagraphs;
	bool			mIndentLines;
	bool			mKeepDocType;
	bool			mKeepComments;
	int				mMaxLineLength;

public:
	bool WriteDocument (System::Xml::XmlDocument^ pXmlDocument);

protected:
	void PutElementDepth (int pElementDepth);
	void PutElementName (System::String^ pElementName, int pElementDepth);
	bool StartLine ();
	bool EndLine (int pWantLines);
	bool WriteIndent ();
	bool WriteIndent (int pIndent);
	bool WriteText (System::String^ pText);

	static int NewLinesBefore (System::String^ pElementName);
	static int NewLinesAfter (System::String^ pElementName);

protected:
	System::Xml::XmlNodeReader^								mNodeReader;
	System::Xml::XmlTextWriter^								mTextWriter;
	System::String^											mTextValue;
	System::Collections::Generic::List<System::String^>^	mElementName;
	int														mNewLinesWritten;
	__int64													mLineStart;
};

/////////////////////////////////////////////////////////////////////////////
} // namespace XmlToHtml
} // namespace DoubleAgent
