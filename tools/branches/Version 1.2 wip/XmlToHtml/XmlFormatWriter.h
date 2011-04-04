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
	String^	mOutputPath;
	bool	mEndParagraphs;
	bool	mIndentLines;
	bool	mKeepDocType;
	bool	mKeepComments;
	int		mMaxLineLength;

public:
	bool WriteDocument (System::Xml::XmlDocument^ pXmlDocument);

protected:
	void PutElementDepth (int pElementDepth);
	void PutElementName (String^ pElementName, int pElementDepth);
	bool StartLine ();
	bool EndLine (int pWantLines);
	bool WriteIndent ();
	bool WriteIndent (int pIndent);
	bool WriteText (String^ pText);

	static int NewLinesBefore (String^ pElementName);
	static int NewLinesAfter (String^ pElementName);

protected:
	System::Xml::XmlNodeReader^						mNodeReader;
	System::Xml::XmlTextWriter^						mTextWriter;
	String^											mTextValue;
	System::Collections::Generic::List<String^>^	mElementName;
	int												mNewLinesWritten;
	__int64											mLineStart;
};

/////////////////////////////////////////////////////////////////////////////
} // namespace XmlToHtml
} // namespace DoubleAgent
