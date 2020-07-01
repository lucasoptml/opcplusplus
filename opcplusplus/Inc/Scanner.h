///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Scanner.h
/// Date: 11/26/2006
///
/// Description:
///
/// opScanner class declaration.
///****************************************************************

namespace scanner
{

class opScanner
{
public:
	/**** construction / destruction ****/
	
	opScanner() : ScanComplete(false) {}
	virtual ~opScanner() {}
	
	/**** enumerations ****/
	
	enum ScanMode
	{
		SM_NormalMode,
		SM_DialectMode,
		SM_BasicMode,
	};
	
	/**** utility ****/
	
	bool Scan(FileReadStream& ifs, ScanMode mode, opNode* root);
	void Print(ostream &o);
	void AddTokensToRoot(FileNode* infile);
	
private:
	/**** private utility ****/
	
	typedef opArray<char>		inputtype; 
	typedef inputtype::iterator	inputiterator;

	void FixNewlines(inputtype& Input);
	template<Token Type, Token Replacement>
	bool FixAltPrefix(opList<opToken>::iterator& start, TerminalNode* name);
	
	// scanning
	void ScanTokens		(const inputtype& Input);
	bool Newline		(const inputtype& Input, int& index);
	bool CComment		(const inputtype& Input, int& index);
	bool Comment		(const inputtype& Input, int& index);
	bool WhiteSpace		(const inputtype& Input, int& index);
	bool String			(const inputtype& Input, int& index);
	bool Operator		(const inputtype& Input, int& index);
	bool Hexadecimals	(const inputtype& Input, int& index);
	bool Number			(const inputtype& Input, int& index);
	bool GetId			(const inputtype& Input, int& index);

	// post scanning
	void Preprocessor();
	void FixStrings();
	void Keyword();
	void BasicType();
	void UserDefined();
	void ContinueLines();
	void Decimals();
	void WideStrings();
	void FixNumbers();
	void CPlusPlus();

	/**** private helper utility ****/

	// returns true if the token is a float/long specifier (f, F, l, L)
	bool IsFloatFlagToken(const opToken& tok)
	{
		return tok.Id == T_ID && (tok.Value.Size() == 1) && IsFloatChar(tok.Value.Front());
	}

	// returns true if the token is a decimal and it doesn't have a float specifier
	bool IsBareDecimal(const opToken& tok)
	{
		return tok.Id == T_DECIMAL && !IsFloatChar(tok.Value.Back());
	}

	bool IsExpID(const opToken& tok);

	// returns true if the token is a T_ID and is either "e" or "E"
	bool IsExpToken(const opToken& tok)
	{
		return tok.Id == T_ID && (tok.Value.Size() == 1) && IsExpChar(tok.Value.Front());
	}

	// returns true if the token is either + or -
	bool IsSignToken(const opToken& tok)
	{
		return tok.Id == T_PLUS || tok.Id == T_MINUS;
	}
	
private:
	//opDeque<char>    Input;
	opList<opToken>  Tokens;
	bool             ScanError;
	int              CurrentLine;
	bool             ScanComplete;
	ScanMode         scanMode;
	opNode*		     Root;
	
public:
	/**** static utility ****/
	
	static bool IsNewline(char c)
	{
		return c == '\n' || c == '\r';
	}
	
	static bool IsExpChar(char c)
	{
		return c == 'e' || c == 'E';
	}
	
	static bool IsFloatChar(char c)
	{
		return c == 'f' || c == 'F' || c == 'l' || c == 'L';
	}
	
	static bool IsWhiteSpace(char c)
	{
		return (c == ' ' || c == '\t');
	}
	
	static bool IsDigit(char c)
	{
		int i = c;

		return (i > 47 && i < 58);
	}

	static bool IsAlpha(char c)
	{
		int i = c;

		return (i > 96 && i < 123) || (i > 64 && i < 91);
	}

	static bool IsAlphaNum(char c)
	{
		return IsAlpha(c) || IsDigit(c);
	}

	static bool IsOperatorChar(char c)
	{
		return (c == '+' ||
				c == '-' ||
				c == '*' ||
				c == '/' ||
				c == '<' ||
				c == '>' ||
				c == '^' ||
				c == '=' ||
				c == '?' ||
				c == '(' ||
				c == ')' ||
				c == '[' ||
				c == ']' ||
				c == '{' ||
				c == '}' ||
				c == '&' ||
				c == '.' ||
				c == ',' ||
				c == ':' ||
				c == ';' ||
				c == '%' ||
				c == '!' ||
				c == '|' ||
				c == '~' ||
				c == '#' ||
				c == '\\' ||
				c == '`');
	}

	static bool IsPreprocessorKeyword(const opString &s)
	{
		if (s == "define" 
		||	s == "elif" 
		||	s == "else" 
		||	s == "endif" 
		||	s == "error" 
		||	s == "if" 
		||	s == "ifdef" 
		||	s == "ifndef" 
		||	s == "import" 
		||	s == "include" 
		||	s == "line" 
		||	s == "pragma" 
		||	s == "undef" 
		||	s == "using" 
		||	s == "warning")
		{
			return true;
		}

		return false;
	}

	static bool IsStandardBasicType(const opString& s)
	{
		if (s == "char"
		||  s == "bool"
		||  s == "short"
		||  s == "long"
		||  s == "float"
		||  s == "double"
		||  s == "wchar_t"
		||  s == "int"
		||  s == "void"
		||  s == "long long"
		||  s == "long double")
		{
			return true;
		}

		return false;
	}

	static bool IsMicrosoftBasicType(const opString& s)
	{
		if (s == "__wchar_t"
		||  s == "__int8"
		||  s == "__int16"
		||  s == "__int32"
		||  s == "__int64")
		{
			return true;
		}

		return false;
	}

	static bool IsBasicType(const opString& s)
	{
		return IsStandardBasicType(s) || IsMicrosoftBasicType(s);
	}

	static bool IsSignable(const opString& s)
	{
		return (s == "char"
		||      s == "short"
		||      s == "long"
		||   	s == "int"
		||      s == "__int8"
		||      s == "__int16"
		||      s == "__int32"
		||      s == "__int64");
	}

	static bool IsHexDigit(char c)
	{
		int i = c;

		return (i > 47 && i < 58)
			|| (i > 96 && i < 103)
			|| (i > 64 && i < 71);
	}

	static bool IsIntegerSuffix(const opToken& t)
	{
		opString s = t.Value;

		return (s == "u"
			||  s == "l"
			||  s == "U"
			||  s == "L"
			||  s == "ul"
			||  s == "lu"
			||  s == "UL"
			||  s == "LU"
			||  s == "uL"
			||  s == "Ul"
			||  s == "lU"
			||  s == "Lu"
			||  s == "LL"
			||  s == "ll"
			||  s == "ull"
			||  s == "uLL"
			||  s == "Ull"
			||  s == "ULL"
			||  s == "i8"
			||  s == "I8"
			||  s == "ui8"
			||  s == "uI8"
			||  s == "Ui8"
			||  s == "UI8"
			||  s == "i16"
			||  s == "I16"
			||  s == "ui16"
			||  s == "uI16"
			||  s == "Ui16"
			||  s == "UI16"
			||  s == "i32"
			||  s == "I32"
			||  s == "ui32"
			||  s == "uI32"
			||  s == "Ui32"
			||  s == "UI32"
			||  s == "i64"
			||  s == "I64"
			||  s == "ui64"
			||  s == "uI64"
			||  s == "Ui64"
			||  s == "UI64");
	}
};

/**** inline block methods ****/

inline bool BlockMatch(Token left, Token right)
{
	if (left == T_LEFT_BRACE && right != T_RIGHT_BRACE)
		return false;
	else 
		if (left == T_LEFT_PAREN && right != T_RIGHT_PAREN)
			return false;
		else 
			if (left == T_LEFT_BRACKET && right != T_RIGHT_BRACKET)
				return false;

	return true;
}

inline opString RightBlockString(Token left)
{
	if (left == T_LEFT_BRACE)
		return opTokenMap::GetString(T_RIGHT_BRACE);
	if (left == T_LEFT_PAREN)
		return opTokenMap::GetString(T_RIGHT_PAREN);
	if (left == T_LEFT_BRACKET)
		return opTokenMap::GetString(T_RIGHT_BRACKET);

	return "";
}

inline opString LeftBlockString(Token right)
{
	if (right == T_RIGHT_BRACE)
		return opTokenMap::GetString(T_LEFT_BRACE);
	if (right == T_RIGHT_PAREN)
		return opTokenMap::GetString(T_LEFT_PAREN);
	if (right == T_RIGHT_BRACKET)
		return opTokenMap::GetString(T_LEFT_BRACKET);

	return "";
}

//NOTE: would be nice to just assign left/right block enum ranges instead.
inline bool IsLeftBlock(Token t)
{
	if (t == T_LEFT_BRACE
		||	t == T_LEFT_BRACKET
		||	t == T_LEFT_PAREN)
		return true;

	return false;
}

inline bool IsRightBlock(Token t)
{
	if(t == T_RIGHT_BRACE
		|| t == T_RIGHT_BRACKET
		|| t == T_RIGHT_PAREN)
		return true;

	return false;
}

inline bool IsBlock(Token t)
{
	if(IsLeftBlock(t) || IsRightBlock(t))
		return true;

	return false;
}

} // end namespace scanner
