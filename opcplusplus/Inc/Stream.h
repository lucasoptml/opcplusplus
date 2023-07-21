///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Stream.h
/// Date: 11/26/2006
///
/// Description:
///
/// Wrapper for STL iostream.
/// 
/// 
/// 
///****************************************************************

//special mode objects
class DirectiveOverride;
class IgnoreNewlines;
class LineOverride;
class LineOrigin;
class opSectionStream;
class CommentMode;
class ForceNewlines;

///==========================================
/// opStringStream
///
/// TODO: add opFileStream behavior to opStringStream
///==========================================

class opStringStream
{
public:
	/**** construction / destruction ****/
	
	explicit opStringStream(FileWriteStream &oin) : of(oin)
	{
		IndentLevel    = 0;
		overrideline   = 0;
		overridefile   = NULL;
		lineorigin     = -1;
		gendepth       = 0;
		filedepth      = 0;
		
		//needs to be invalid initially so the first token written redirects.
		lastlinenumber = -1;
		
		//reserve a few k per section.
		o.Reserve(2048);
		
		InputFile = NULL;
		
		ignoreredirects = false;
		ignorenewlines  = false;
		forcenewlines   = false;
	}
	
	/**** utility ****/
	
	void Indent()
	{
		IndentLevel++;
		IndentString += '\t';
	}
	
	void DeIndent()
	{
		IndentLevel--;
		IndentString = IndentString.RLeft(1);
	}
	
	void EndLine();
	
	void NoteLineNumber(FileNode* infile, int line)
	{
		InputFile = infile;
		linenumbers.push_back(line);
	}
	
	void Output()
	{
		of << headingstring.GetString();
		EndLine();
		of << o;
		of << footerstring.GetString();
	}
	
	static void SetLineDirectives(bool newsetting)
	{
		bLineDirectives = newsetting;
	}
	
	void TrimLineEnd()
	{
		linestream = linestream.TrimRight();
	}
	
	/**** operators ****/
	
	inline friend opStringStream& operator << (opStringStream& oin, const endlinestruct& endline)
	{
		//I'm assuming its an endl!
		oin.EndLine();

		return oin;
	}
	
	inline friend opStringStream& operator << (opStringStream& oin, const opString& s)
	{
		oin << s.GetString();

		return oin;
	}

	inline friend opStringStream& operator << (opStringStream& oin, TerminalNode* node);
	
	template<typename T> 
	inline friend opStringStream& operator << (opStringStream& oin, T t)
	{
		oin.linestream += opString(t);

		return oin;
	}
	
	/**** set ****/
	
	void SetHeadingString(const opString& s)
	{
		headingstring = s;
	}
	
	void SetFooterString(const opString& s)
	{
		footerstring = s;
	}
	
	void SetDepths(int infiledepth, int ingendepth)
	{
		gendepth = ingendepth;
		filedepth = infiledepth;
	}

	void PrintComment(const opString& s)
	{
		// handle compact mode and force comments
		if ( opParameters::Get().Compact 
		&&   !ForceComments )
		{
			return;
		}

		(*this) << s;
	}

	static void SetForceComments(bool inbool)
	{
		ForceComments = inbool;
	}

	static bool GetForceComments()
	{
		return ForceComments;
	}

private:
	//line overriding
	void OverrideLine(opNode* node);
	void ResetOverrideLine();
	
	//line origin
	void SetLineOrigin(opNode* node);
	void ResetLineOrigin();
	
	void SetIgnoreRedirects(bool bignore);
	
	FileNode* overridefile;
	int		  overrideline;
	int		  lineorigin;

	int       gendepth;
	int       filedepth;

	bool ignoreredirects;
	bool ignorenewlines;
	
	/**** data ****/

	static bool ForceComments;

	FileWriteStream&	of;
	int					IndentLevel;
	opString			IndentString;
	FileNode*			InputFile;
	static bool			bLineDirectives;

	//we store input inside a linestream, and flush it upon endline.
	opString linestream;
	
	//line number tracking
	int lastlinenumber;

	vector<int> linenumbers;

	//heading string you can set
	opString headingstring;

	//footer string you can set
	opString footerstring;
	
	//pre-pend to reach path to original file
	opString prependpath;

	bool forcenewlines;

	//the full stream
	opString o;

	friend DirectiveOverride;
	friend IgnoreNewlines;
	friend LineOverride;
	friend CommentMode;
	friend LineOrigin;
	friend ForceNewlines;
};

class opSectionStream
{
public:
	opSectionStream(FileWriteStream& stream)
		: heading(stream), body(stream), footer(stream), o(stream)
	{
// 		heading.SetHeadingString(	"\n\t\t// -- sub-heading start --\n");
// 		heading.SetFooterString(	"\n\t\t// -- sub-heading end --\n");
// 		body.SetHeadingString(		"\n\t\t// -- sub-body start --\n");
// 		body.SetFooterString(		"\n\t\t// -- sub-body end --\n");
// 		footer.SetHeadingString(	"\n\t\t// -- sub-footer start --\n");
// 		footer.SetFooterString(		"\n\t\t// -- sub-footer end --\n");
	}

	//heading
	opStringStream heading;
	//body
	opStringStream body;
	//footer
	opStringStream footer;

	//heading string
	opString headingstring;
	//footer string
	opString footerstring;

	//default pipes to body (<<,indent,deindent)
	template<class T> 
	opStringStream& operator<<(T input)
	{
		return body << input;
	}

	void Indent()
	{
		body.Indent();
	}

	void DeIndent()
	{
		body.DeIndent();
	}

	void Output()
	{
		o << headingstring.GetString();
		heading.Output();
		body.Output();
		footer.Output();
		o << footerstring.GetString();
	}

	void TrimLineEnd()
	{
		body.TrimLineEnd();
	}
	
	void SetDepths(int infiledepth, int ingendepth)
	{
		heading.SetDepths(infiledepth,ingendepth);
		body.SetDepths(infiledepth,ingendepth);
		footer.SetDepths(infiledepth,ingendepth);
	}

	void PrintComment(const opString& s)
	{
		body.PrintComment(s);
	}

private:
	FileWriteStream& o;
};

class opFileSectionStream
{
public:
	opFileSectionStream(FileWriteStream& stream) : heading(stream), body(stream), footer(stream), o(stream)
	{
	}

	//heading
	opSectionStream heading;
	
	//body
	opSectionStream body;
	
	//footer
	opSectionStream footer;

	//heading string
	opString headingstring;
	opString footerstring;

	//default pipes to body (<<,indent,deindent)
	template<class T> 
	opStringStream& operator<<(T input)
	{
		return body << input;
	}

	void Indent()
	{
		body.Indent();
	}

	void DeIndent()
	{
		body.DeIndent();
	}

	void Output()
	{
		o << headingstring.GetString();
		heading.Output();
		body.Output();
		footer.Output();
		o << footerstring.GetString();
	}

	void SetDepths(int infiledepth, int ingendepth)
	{
		heading.SetDepths(infiledepth,ingendepth);
		body.SetDepths(infiledepth,ingendepth);
		footer.SetDepths(infiledepth,ingendepth);
	}

	void PrintComment(const opString& s)
	{
		body.PrintComment(s);
	}

private:
	FileWriteStream& o;
};

class opFileStream
{
public:
	opFileStream(FileWriteStream& fheader, FileWriteStream& fsource) 
		: header(fheader), source(fsource), headerstream(fheader), sourcestream(fsource)
	{
		header.headingstring =	"\n// header file start\n";
		header.footerstring =	"\n// header file end\n";
		source.headingstring =	"\n// source file start\n";
		source.footerstring =	"\n// source file end\n";
	}
	
	//header
	opFileSectionStream header;
	//source
	opFileSectionStream source;

	//default pipes to body
	template<class T> 
	opStringStream& operator << (T input)
	{
		return header << input;
	}

	void Output()
	{
		header.Output();
		source.Output();
	}

	void SetDepths(const opString& outputpath);

	void PrintComment(const opString& s)
	{
		header.PrintComment(s);
	}

protected:
	FileWriteStream& headerstream;
	FileWriteStream& sourcestream;
};

class opDialectStream : public opFileStream
{
public:
	opDialectStream(FileWriteStream& fheader, FileWriteStream& fsource) 
		: opFileStream(fheader,fsource)
	{
	}

	void Indent()
	{
		header.heading.Indent();
	}

	void DeIndent()
	{
		header.heading.DeIndent();
	}

	//default pipes to heading
	template<class T> 
	opStringStream& operator << (T input)
	{
		return header.heading << input;
	}

	void PrintComment(const opString& s)
	{
		header.heading.PrintComment(s);
	}
};

class opXmlStream
{
public:
	opXmlStream(ostream& s) : stream(s)
	{
	}

	void Indent()
	{
		indention += "\t";
	}

	void DeIndent()
	{
		if(indention.Length())
			indention = indention.Substring(0,indention.Length()-1);		
	}

	//default pipes to heading
	template<class T> 
	friend opXmlStream& operator << (opXmlStream& stream, const T& input);
	

	void StartTag(const opString& tag)
	{
		stream << '<' << tag << '>';
		Indent();
		EndLine();
	}
	
	void EndTag(const opString& tag)
	{
		DeIndent();
		EndLine();
		stream << "</" << tag << '>';
		EndLine();
	}
	
	void CompactStartTag(const opString& tag)
	{
		stream << '<' << tag << '>';
	}
	
	void CompactEndTag(const opString& tag)
	{
		stream << "</" << tag << '>';
		EndLine();
	}
	
	void EndLine()
	{
		stream << "\n";
		stream << indention;
	}
	
	// escape reserved xml characters
	void EscapeString(opString& s)
	{
		s.Replace("&",  "&amp;");
		s.Replace("<",  "&lt;");
		s.Replace(">",  "&gt;");
		s.Replace("\"", "&quot;");
		s.Replace("'",  "&apos;");
	}
	
	//write without replacement.
	void Write(const opString& s)
	{
		stream << s;
	}
	
private:
	opString indention;
	ostream& stream;
};

template<class T> 
opXmlStream& operator<< <T>(opXmlStream& stream, const T& input)
{
	stream.stream << input;
	return stream;
}

template<>
opXmlStream& operator << <endlinestruct>(opXmlStream& stream, const endlinestruct& input)
{
	stream.EndLine();
	return stream;
}

// override << for opString - do escape replacement
template<>
opXmlStream& operator << <opString>(opXmlStream& stream, const opString& input)
{
	opString in = input;
	
	stream.EscapeString(in);

	stream.stream << in;

	return stream;
}

// override << for string - do escape replacement
template<>
opXmlStream& operator << <string>(opXmlStream& stream, const string& input)
{
	opString in = input;

	stream.EscapeString(in);

	stream.stream << in;

	return stream;
}

class opXmlTag
{
public:
	opXmlTag(opXmlStream& s, const opString& name) : tag(name), stream(s)
	{
		stream.StartTag(tag);
	}
	
	~opXmlTag()
	{
		stream.EndTag(tag);
	}
	
private:
	opString tag;
	opXmlStream& stream;
};

class opXmlTextTag
{
public:
	opXmlTextTag(opXmlStream& s, const opString& name) : tag(name), stream(s)
	{
		stream.CompactStartTag(tag);
	}

	~opXmlTextTag()
	{
		stream.CompactEndTag(tag);
	}

private:
	opString tag;
	opXmlStream& stream;
};


//
// settings override objects
//

class DirectiveOverride
{
public:
	explicit DirectiveOverride(bool newvalue)
	{
		oldvalue = opStringStream::bLineDirectives;
		opStringStream::SetLineDirectives(newvalue);
	}

	~DirectiveOverride()
	{
		opStringStream::SetLineDirectives(oldvalue);
	}
	
private:
	bool oldvalue;
};

class LineOverride
{
public:
	explicit LineOverride(opSectionStream& s, opNode* sourcenode) : stream(s.body)
	{
		stream.OverrideLine(sourcenode);
	}
	
	explicit LineOverride(opStringStream& s, opNode* sourcenode) : stream(s)
	{
		stream.OverrideLine(sourcenode);
	}
	
	~LineOverride()
	{
		stream.ResetOverrideLine();
	}
	
private:
	opStringStream& stream;
};

class LineOrigin
{
public:
	explicit LineOrigin(opSectionStream& s, opNode* sourcenode) : stream(s.body)
	{
		stream.SetLineOrigin(sourcenode);
	}
	
	explicit LineOrigin(opStringStream& s, opNode* sourcenode) : stream(s)
	{
		stream.SetLineOrigin(sourcenode);
	}
	
	~LineOrigin()
	{
		stream.ResetLineOrigin();
	}
	
private:
	opStringStream& stream;
};

class IgnoreNewlines
{
public:
	explicit IgnoreNewlines(opStringStream& s) : stream(s)
	{
		oldvalue = stream.ignorenewlines;
		stream.ignorenewlines = true;
		benabled = true;
	}

	~IgnoreNewlines()
	{
		if(benabled)
			Disable();
	}

	void Disable()
	{
		stream.ignorenewlines = oldvalue;
		benabled = false;
	}

private:
	bool benabled;
	bool oldvalue;
	opStringStream& stream;
};

class CommentMode
{
public:
	explicit CommentMode(opStringStream& s) : stream(s)
	{
		oldvalue = stream.ignoreredirects;
		stream.ignoreredirects = true;
	}

	~CommentMode()
	{
		stream.ignoreredirects = oldvalue;
	}

private:
	bool oldvalue;
	opStringStream& stream;
};

class ForceNewlines
{
public:
	explicit ForceNewlines(opStringStream& s) : stream(s)
	{
		stream.forcenewlines = true;
	}

	~ForceNewlines()
	{
		stream.forcenewlines = false;
	}
	
private:
	opStringStream& stream;
};

// opStringStream << specialization for TerminalNode
// template<>
// inline opStringStream& operator << <TerminalNode*&>(opStringStream &oin, TerminalNode*& node)
// {
// 	Token id = node->GetId();
// 
// 	// handle compact mode and force comments
// 	if ( (id == T_COMMENT || id == T_CCOMMENT) 
// 		&&   opParameters::Get().Compact 
// 		&&   !opStringStream::ForceComments )
// 	{
// 		Log("avoided comment 2");
// 
// 		return oin;
// 	}
// 
// 	if ( id == T_CCOMMENT || id == T_COMMENT )
// 	{
// 		Log("hit comment, compact = " + opString(opParameters::Get().Compact) + ", forcecomments = " + opString(opStringStream::ForceComments) );
// 	}
// 
// 	oin.NoteLineNumber(node->GetFile(), node->GetLine());
// 
// 	node->PrintValue(oin);
// 
// 	return oin;
// }