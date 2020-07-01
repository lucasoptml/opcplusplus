///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Scanner.cpp
/// Date: 11/26/2006
///
/// Description:
///
/// Contains source code for opScanner class.
///****************************************************************

#include "opCPP.h"

//
// opScanner
//

// adds all scanned tokens into root opNode
void opScanner::AddTokensToRoot(FileNode* root)
{
	opList<opToken>::iterator start = Tokens.Begin();
	opList<opToken>::iterator end   = Tokens.End();

	while (start != end)
	{
		stacked<TerminalNode> newNode = NEWNODE(TerminalNode(*start, root));
		root->AppendNode(newNode);
		++start;
	}
}

// scans the Input into a symbol list
bool opScanner::Scan(FileReadStream& ifs, ScanMode mode, opNode* root)
{
	Tokens.Clear();

	ScanError    = false;
	CurrentLine  = 1;
	ScanComplete = false;
	scanMode     = mode;
	Root         = root;

	inputtype Input;
	ifs.ReadToContainer(Input);

	FixNewlines(Input);

	int it = 0;

	ScanTokens(Input);

	if(opError::HasErrors())
		return false;
	
	// post scanning stuff
	Preprocessor();
	FixStrings();
	Keyword();
	
	if (scanMode != SM_DialectMode)
	{
		BasicType();
		UserDefined();
	}
	
	ContinueLines();
	Decimals();
	WideStrings();
	FixNumbers();

	if (scanMode == SM_NormalMode)
		CPlusPlus();
	
	ScanComplete = true;
	
	return true;
}

// prints tokens - for testing
void opScanner::Print(ostream &o)
{
	opList<opToken>::iterator start = Tokens.Begin();
	opList<opToken>::iterator end   = Tokens.End();

	while (start != end)
	{
		opString output = TokenFunctions::ToString(start->Id)
			+ " (" + opString(start->Line) + "): ";

		o << setw(30) << output;

		if (start->Id == T_NEWLINE)
			o << "\\n";
		else if (start->Id != T_WHITESPACE)
			o << start->Value;

		o << endl;

		++start;
	}
}

// fixes stupid "\r\n" thing
void opScanner::FixNewlines(inputtype& Input)
{
	if (Input.Size() >= 2)
	{
		inputiterator one = Input.Begin();
		inputiterator two = ++(Input.Begin());
		inputiterator end = Input.End();

		while (two != end)
		{
			if (*one == '\r' && *two == '\n')
			{
				//fast fix...
				//set \r to whitespace
				*one = ' ';
			}

			++one;
			++two;
		}
	}
}

// Fix alternative prefixes.
template<Token Type, Token Replacement>
bool opScanner::FixAltPrefix(opList<opToken>::iterator& start, TerminalNode* name)
{
	opList<opToken>::iterator end = Tokens.end();
	opList<opToken>::iterator j   = start;

	++j;

	while ( j != end && (j->Id == T_WHITESPACE || j->Id == T_NEWLINE) )
	{
		++j;
	}

	if ( j != end && j->Id == Type )
	{
		opList<opToken>::iterator k = start;

		++k;
		++j;

		Tokens.Erase(k, j);

		start->Id    = Replacement;
		start->Value = name->GetValue();

		return true;
	}

	return false;
}

// parses the next token
// returns false if done
void opScanner::ScanTokens(const inputtype& Input)
{
	// if we've reached the end of the Input stream,  
	// add an EOF token and return false

	int size    = Input.Size();
	int current = 0;

	//TODO: the input list of chars is really a bad idea
	//		Input should be a vector, and we should not alter it,
	//		instead we should iterate over it (maybe w/ an iterator we pass
	//		around.
	
	while (current != size)
	{
		// scan for the next token 
		// (with the correct precedence)
		if      (current != size && Newline(Input,current));
		else if (current != size && CComment(Input,current));
		else if (current != size && Comment(Input,current));
		else if (current != size && String(Input,current));
		else if (current != size && WhiteSpace(Input,current));
		else if (current != size && Operator(Input,current));
		else if (current != size && Hexadecimals(Input,current));
		else if (current != size && Number(Input,current));
		else if (current != size && GetId(Input,current));
		else if (current != size)
		{
			opToken newToken(T_ANYCHAR, Input[current], CurrentLine);
			
			Tokens.PushBack(newToken);
			++current;
		}
	}
	
	Tokens.PushBack(opToken(T_EOF, "", CurrentLine));
}

// newline - parses a newline
bool opScanner::Newline(const inputtype& Input, int& current)
{
	char c = Input[current];

	if (!IsNewline(c))
		return false;

	opToken newToken(T_NEWLINE, '\n', CurrentLine);

	++CurrentLine;

	Tokens.PushBack(newToken);
	++current;

	return true;
}

// scans for a c-style comment
bool opScanner::CComment(const inputtype& Input, int& current)
{
	int size = Input.Size();
	
	if (current + 1 < size)
	{
		int one = current;
		int two = current + 1;
		
		if (Input[one] == '/' && Input[two] == '*')
		{
			opToken newToken(T_CCOMMENT, "/*", CurrentLine);
			bool    bFoundEnd = false;

			current += 2;

			while (current + 1 < size)
			{
				one = current;
				two = current + 1;

				if (Input[one] == '*' && Input[two] == '/')
				{
					newToken.Value += "*/";

					current += 2;

					bFoundEnd = true;
					break;
				}
				else
				{
					if ( IsNewline(Input[one]) )
						++CurrentLine;

					newToken.Value += Input[one];
					++current;
				}
			}

			// check for unbounded comments
			if (bFoundEnd)
			{
				Tokens.PushBack(newToken);

				return true;
			}
			else
			{
				opError::UnboundedCommentError(Root, newToken.Line);
				ScanError = true;
			}
		}
	}

	return false;
}

// scans for a comment
bool opScanner::Comment(const inputtype& Input, int& current)
{
	int size = Input.Size();
	
	if (current + 1 < size)
	{
		int one = current;
		int two = current + 1;

		if (Input[one] == '/' && Input[two] == '/')
		{
			opToken newToken(T_COMMENT, "//", CurrentLine);

			current += 2;

			while (current < size)
			{
				one = current;

				if (IsNewline(Input[one]))
					break;

				newToken.Value += Input[one];
				++current;
			}

			Tokens.PushBack(newToken);

			return true;
		}
	}

	return false;
}

// scans for strings
bool opScanner::String(const inputtype& Input, int& current)
{
	char start = Input[current];
	int  size  = Input.Size();
	int  count = 0;

	if (start == '\"' || start == '\'')
	{
		opToken newToken((start == '\"') ? T_STRING : T_CHAR, start, CurrentLine);
		char    c, last;

		++count;
		++current;

		c = start;

		while (current < size)
		{
			last = c;
			c    = Input[current];

			if ( IsNewline(c) )
			{
				opError::UnboundedStringError(Root, CurrentLine);
				ScanError = true;
				return false;
			}

			// If we reach an end token, process it.
			if ( c == start )
			{
				// Handle special cases.
				if ( last != '\\' 
				||   ( last == '\\' && count >= 3 && newToken.Value[ count - 2 ] == '\\' ) )
				{
					newToken.Value += c;
					++current;
					Tokens.PushBack(newToken);
					return true;
				}
			}

			newToken.Value += c;
			++current;
			++count;
		}

		opError::UnboundedStringError(Root, CurrentLine);
		ScanError = true;
	}

	return false;
}

// scans for whitespace
bool opScanner::WhiteSpace(const inputtype& Input, int& current)
{
	char c    = Input[current];
	int  size = Input.Size();

	if ( IsWhiteSpace(c) )
	{
		opToken newToken(T_WHITESPACE, c, CurrentLine);
		++current;

		while (current < size)
		{
			c = Input[current];

			if ( !IsWhiteSpace(c) )
				break;

			newToken.Value += c;
			++current;
		}

		Tokens.PushBack(newToken);

		return true;
	}

	return false;
}

// operator - parses an operator
bool opScanner::Operator(const inputtype& Input, int& current)
{
	char c = Input[current];

	// if this character is an operator character,
	// try to parse it as an operator
	if (IsOperatorChar(c))
	{
		opString  op    = c;
		int       start = current + 1;
		int       end	= Input.Size();
		Token     id;
		int       length;
		
		// create a string with all consecutive
		// operator characters
		while (start != end)
		{
			c = Input[start];

			if (!IsOperatorChar(c))
				break;

			op += c;
			++start;
		}

		// find the largest string that is a 
		// valid operator
		while (( id = opTokenMap::GetToken(op) ) == T_UNKNOWN)
			op.PopBack();
	
		length = op.Length();
		
		if (length == 0)
			return false;
		
		opToken newToken(id, op, CurrentLine);
		
		Tokens.PushBack(newToken);
		
		for (int i = 0; i < length; i++)
		{
			++current;
		}
		
		return true;
	}

	return false;
}

// parses a hex number
bool opScanner::Hexadecimals(const inputtype& Input, int& current)
{
	if (current + 2 < Input.Size())
	{
		int one   = current;
		int two   = current + 1;
		int three = current + 2;

		if ( Input[one] == '0'
		&&  (Input[two] == 'x' || Input[two] == 'X')
		&&  IsHexDigit( Input[three] ) )
		{
			int     end = Input.Size();
			opToken newToken(T_HEXADECIMAL, opString("0x") + Input[three], CurrentLine);

			current += 3;

			one = current;

			while (one != end)
			{
				if ( !IsHexDigit(Input[one]) )
					break;

				newToken.Value += Input[one];
				++current;

				one = current;
			}

			Tokens.PushBack(newToken);

			return true;
		}
	}

	return false;
}

// parses a number (an integer)
bool opScanner::Number(const inputtype& Input, int& current)
{
	char c = Input[current];
	int size = Input.Size();
		
	if (IsDigit(c))
	{
		opToken newToken(T_NUMBER, c, CurrentLine);
		++current;

		while (current + 1 < size)
		{
			c = Input[current];

			if (!IsDigit(c))
				break;

			newToken.Value += c;
			++current;
		}

		Tokens.PushBack(newToken);

		return true;
	}

	return false;
}

// id - parses an id
bool opScanner::GetId(const inputtype& Input, int& current)
{
	char c    = Input[current];
	int  size = Input.Size();

	if ( IsAlpha(c) || c == '_' )
	{
		opToken newToken(T_ID, c, CurrentLine);

		++current;

		while (current < size)
		{
			c = Input[current];

			if ( !IsAlphaNum(c) && c != '_' )
				break;

			newToken.Value += c;
			++current;
		}

		Tokens.push_back(newToken);

		return true;
	}

	return false;
}

// checks for a preprocessor directive
void opScanner::Preprocessor()
{
	if (Tokens.HasSize(2))
	{
		opList<opToken>::iterator one = Tokens.Begin();
		opList<opToken>::iterator two = ++Tokens.Begin();
		opList<opToken>::iterator end = Tokens.End();

		while (two != end)
		{
			if (one->Id == T_POUND
			&&  IsPreprocessorKeyword(two->Value))
			{
				one->Value += two->Value;
				one->Id     = opTokenMap::GetToken(two->Value);

				Tokens.Erase(two);
				two = one;
				++two;
				continue;
			}
			
			++one;
			++two;
		}
	}

	if (Tokens.HasSize(3))
	{
		opList<opToken>::iterator one   = Tokens.Begin();
		opList<opToken>::iterator two   = ++Tokens.Begin();
		opList<opToken>::iterator three = ++(++Tokens.Begin());
		opList<opToken>::iterator end   = Tokens.End();

		while (three != end)
		{
			if (one->Id == T_POUND
			&&  two->Id == T_WHITESPACE
			&&  IsPreprocessorKeyword(three->Value))
			{
				one->Value += three->Value;
				one->Id     = opTokenMap::GetToken(three->Value);

				Tokens.Erase(two);
				Tokens.Erase(three);
				two = one;
				++two;
				three = two;
				++three;
				continue;
			}

			++one;
			++two;
			++three;
		}
	}
}

// fixstrings - if you have T_STRING T_STRING,
// merge them into one T_STRING, etc.
void opScanner::FixStrings()
{
	if (!Tokens.HasSize(2))
		return;

	opList<opToken>::iterator i1  = Tokens.Begin();
	opList<opToken>::iterator i2  = ++Tokens.Begin();
	opList<opToken>::iterator i3;
	opList<opToken>::iterator end = Tokens.End();

	while (i2 != end)
	{
		if (i1->Id == T_STRING)
		{
			if (i2->Id == T_STRING)
			{
				i1->Value.TrimQuotes();
				i2->Value.TrimQuotes();
				i1->Value = "\"" + i1->Value + i2->Value + "\"";
				Tokens.Erase(i2);
				i2 = i1;
				++i2;
				continue;
			}
			else if (i2->Id == T_WHITESPACE
			||       i2->Id == T_NEWLINE)
			{
				++i2;

				while (i2 != end)
				{
					if (i2->Id == T_WHITESPACE
					||  i2->Id == T_NEWLINE)
					{
						++i2;
					}
					else if (i2->Id != T_STRING)
					{
						i1 = i2;
						++i2;
						break;
					}
					else
					{
						i1->Value.TrimQuotes();
						i2->Value.TrimQuotes();
						i1->Value = "\"" + i1->Value + i2->Value + "\"";
						i3 = i1;
						++i3;
						++i2;
						Tokens.Erase(i3, i2);
						break;
					}
				}

				if (i2 == end)
					return;
			}
			else
			{
				++i1;
				++i2;
			}
		}
		else
		{
			++i1;
			++i2;
		}
	}
}

// keyword - assumes that the last token parsed
// is an id.  checks if it exists in the symbol 
// map to see if it's a keyword.
void opScanner::Keyword()
{
	opList<opToken>::iterator start = Tokens.Begin();
	opList<opToken>::iterator end   = Tokens.End();
	Token                     id;
	
	while (start != end)
	{
		//NOTE: could be improved probably.
		id = opTokenMap::GetToken(start->Value);
		
		if (id != T_UNKNOWN)
		{
			if ( scanMode != SM_DialectMode && TokenFunctions::IsDialectToken(id))
			{
// 				if(TokenFunctions::IsDialectToken(id))
// 					start->Id = id;
			}
			else if (scanMode != SM_NormalMode && TokenFunctions::IsNormalToken(id))
			{
// 				if(TokenFunctions::IsNormalToken(id))
// 					start->Id = id;
			}
			else// if (scanMode == SM_BasicMode)// && TokenFunctions::IsNormalToken(id))
			{
				start->Id = id;
			}
		}
		
		++start;
	}
}

// basic type - labels all ISO standard/microsoft basic types
// with an id of T_BASIC_TYPE
void opScanner::BasicType()
{
	opList<opToken>::iterator one = Tokens.Begin();
	opList<opToken>::iterator end = Tokens.End();
	opList<opToken>::iterator two;
	opList<opToken>::iterator three;

	while (one != end)
	{
		if (IsBasicType(one->Value))
			one->Id = T_BASIC_TYPE;

		++one;
	}

	if (Tokens.HasSize(3))
	{
		one   = Tokens.Begin();
		two   = ++Tokens.Begin();
		three = ++(++Tokens.Begin());

		while (three != end)
		{
			if (one->Id    == T_BASIC_TYPE
			&&  two->Id    == T_WHITESPACE
			&&  three->Id  == T_BASIC_TYPE
			&&  IsBasicType(one->Value + " " + three->Value) )
			{
				one->Value += opString(" ") + three->Value;

				Tokens.Erase(two);
				Tokens.Erase(three);
				two = one;
				++two;
				three = two;
				++three;
				continue;
			}

			++one;
			++two;
			++three;
		}
	}
}

// if the last token parsed is a user-defined modifier or
// opobject type, update its id
void opScanner::UserDefined()
{	
	opList<opToken>::iterator  start = Tokens.Begin();
	opList<opToken>::iterator  end   = Tokens.End();

	while (start != end)
	{
		if ( start->Id == T_ID )
		{
			opString Value = start->Value;

			// If this is a registered category, change the id.
			if ( DialectTracker::GetCategory(Value) )
				start->Id = T_OPOBJECT;
			// If this is a registered enumeration, change the id.
			else if( DialectTracker::GetEnumeration(Value) )
				start->Id = T_OPENUM;
			else
			{
				CategoryNode*    classcategory  = DialectTracker::GetAltClassPrefix(Value);
				CategoryNode*    structcategory = DialectTracker::GetAltStructPrefix(Value);
				EnumerationNode* enumeration    = DialectTracker::GetAltEnumerationPrefix(Value);

				// Registered class prefix?
				if      ( classcategory  && FixAltPrefix<T_CLASS, T_OPOBJECT>(start, classcategory->GetName()) )
				{
					// do nothing
				}
				// Registered struct prefix?
				else if ( structcategory && FixAltPrefix<T_STRUCT, T_OPOBJECT>(start, structcategory->GetName()) )
				{
					// do nothing
				}
				// Registered enum prefix?
				else if ( enumeration    && FixAltPrefix<T_ENUM, T_OPENUM>(start, enumeration->GetName()) )
				{
					// do nothing
				}
			}
		}
		
		++start;
	}
}

// continue line ("\" followed optionally by whitespace, then by a newline)
void opScanner::ContinueLines()
{
	if (Tokens.HasSize(2))
	{
		opList<opToken>::iterator one = Tokens.Begin();
		opList<opToken>::iterator two = ++Tokens.Begin();
		opList<opToken>::iterator end = Tokens.End();

		while (two != end)
		{
			if (one->Id == T_BACKSLASH
			&&  two->Id == T_NEWLINE)
			{
				one->Id     = T_CONTINUELINE;
				one->Value += two->Value;

				Tokens.Erase(two);
				two = one;
				++two;
				continue;
			}

			++one;
			++two;
		}
	}

	if (Tokens.HasSize(3))
	{
		opList<opToken>::iterator one   = Tokens.Begin();
		opList<opToken>::iterator two   = ++Tokens.Begin();
		opList<opToken>::iterator three = ++(++Tokens.Begin());
		opList<opToken>::iterator end   = Tokens.End();

		int                       i     = 0;

		while (three != end)
		{
			if (one->Id   == T_BACKSLASH
			&&  two->Id   == T_WHITESPACE
			&&  three->Id == T_NEWLINE)
			{
				one->Id     = T_CONTINUELINE;
				one->Value += three->Value;

				Tokens.Erase(two);
				Tokens.Erase(three);

				if(!Tokens.HasSize(3))			
					break;

				two = one;
				++two;
				three = two;
				++three;

				continue;
			}

			++one;
			++two;
			++three;
			++i;
		}
	}
}

// locates decimals
void opScanner::Decimals()
{
	if (Tokens.HasSize(2))
	{
		opList<opToken>::iterator one = Tokens.Begin();
		opList<opToken>::iterator two = ++Tokens.Begin();
		opList<opToken>::iterator end = Tokens.End();

		while (two != end)
		{
			if (one->Id == T_NUMBER
			&&  two->Id == T_DOT)
			{
				one->Id     = T_DECIMAL;
				one->Value += two->Value;

				Tokens.Erase(two);
				two = one;
				++two;

				// case X.X
				if (two != end
				&&  two->Id == T_NUMBER)
				{
					one->Value += two->Value;
					Tokens.Erase(two);
					two = one;
					++two;
				}

				// cases X.f, X.F, X.Xf, X.XF, X.l, X.L, X.Xl, X.XL
				if (two != end
				&&  IsFloatFlagToken(*two))
				{
					one->Value += two->Value;
					Tokens.Erase(two);
					two = one;
					++two;
				}

				continue;
			}

			++one;
			++two;
		}
	}

	if (Tokens.HasSize(2))
	{
		opList<opToken>::iterator one = Tokens.Begin();
		opList<opToken>::iterator two = ++Tokens.Begin();
		opList<opToken>::iterator end = Tokens.End();

		while (two != end)
		{
			if ((one->Id == T_NUMBER || IsBareDecimal(*one))
			&&  IsExpID(*two))
			{
				one->Id     = T_EXPONENTIAL;
				one->Value += two->Value;

				Tokens.Erase(two);
				two = one;
				++two;
				continue;
			}

			++one;
			++two;
		}
	}

	if (Tokens.HasSize(4))
	{
		opList<opToken>::iterator one   = Tokens.Begin();
		opList<opToken>::iterator two   = ++Tokens.Begin();
		opList<opToken>::iterator three = ++(++Tokens.Begin());
		opList<opToken>::iterator four  = ++(++(++Tokens.Begin()));
		opList<opToken>::iterator end   = Tokens.End();

		while (four != end)
		{
			if ((one->Id == T_NUMBER || IsBareDecimal(*one))
			&&  IsExpToken(*two)
			&&  IsSignToken(*three)
			&&  four->Id == T_NUMBER)
			{
				one->Id     = T_EXPONENTIAL;
				one->Value += two->Value + three->Value + four->Value;

				Tokens.Erase(two);
				Tokens.Erase(three);
				Tokens.Erase(four);

				if (!Tokens.HasSize(4))
					break;

				two = one;
				++two;
				three = two;
				++three;
				four = three;
				++four;
				
				continue;
			}

			++one;
			++two;
			++three;
			++four;
		}
	}
}

// checks if the current token is an exp token ("e" or "E" followed by digits)
bool opScanner::IsExpID(const opToken& tok)
{
	if (tok.Id != T_ID)
		return false;

	opString s    = tok.Value;
	int      size = s.Size();

	if (size == 1)
		return false;

	if (!IsExpChar(s.Front()))
		return false;

	for (int i = 1; i < size; i++)
	{
		if (!IsDigit(s[i]))
			return false;
	}

	return true;
}

// parses wide strings (L"string")
void opScanner::WideStrings()
{
	if (Tokens.HasSize(2))
	{
		opList<opToken>::iterator one = Tokens.Begin();
		opList<opToken>::iterator two = ++Tokens.Begin();
		opList<opToken>::iterator end = Tokens.End();

		while (two != end)
		{
			if (one->Id    == T_ID
			&&  one->Value == "L"
			&&  two->Id    == T_STRING)
			{
				one->Id     = T_WIDESTRING;
				one->Value += two->Value;

				Tokens.Erase(two);
				two = one;
				++two;
				continue;
			}

			++one;
			++two;
		}
	}
}

// makes it so if you have T_NUMBER = 3000, T_ID = ul,
// that T_NUMBER = 3000ul, etc.
void opScanner::FixNumbers()
{
	if (Tokens.HasSize(2))
	{
		opList<opToken>::iterator one = Tokens.Begin();
		opList<opToken>::iterator two = ++Tokens.Begin();
		opList<opToken>::iterator end = Tokens.End();

		while (two != end)
		{
			if ((one->Id == T_NUMBER || one->Id == T_HEXADECIMAL)
			&&  IsIntegerSuffix(*two))
			{
				one->Value += two->Value;

				Tokens.Erase(two);
				two = one;
				++two;
				continue;
			}

			++one;
			++two;
		}
	}
}

// Transform T_ID = "c" and T_PLUS_PLUS = "++" into T_CPLUSPLUS = "c++"
void opScanner::CPlusPlus()
{
	if (Tokens.HasSize(2))
	{
		opList<opToken>::iterator one = Tokens.Begin();
		opList<opToken>::iterator two = ++Tokens.Begin();
		opList<opToken>::iterator end = Tokens.End();

		while (two != end)
		{
			if (one->Id == T_ID && one->Value == "c" && two->Id == T_PLUS_PLUS)
			{
				one->Id    = T_CPLUSPLUS;
				one->Value = "c++";

				Tokens.Erase(two);
				two = one;
				++two;
				continue;
			}

			++one;
			++two;
		}
	}
}

