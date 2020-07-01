///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Tokens.h
/// Date: 11/26/2006
///
/// Description:
///
/// This file contains all tokens used when the input source code is scanned.
/// These tokens include all C++ tokens as well as our extension keywords.
/// There are also several helper methods / data structures present in this file.
///****************************************************************

#undef T_FALSE
#undef T_TRUE

// tokens
enum Token 
{
	Tokens_MIN,

	///==========================================
	/// Terminal Tokens
	///==========================================

	TERMINALS_START,

	///==========================================
	/// Miscellaneous Tokens
	///==========================================

	MISC_TOKENS_START,

	T_EOF,
	T_WHITESPACE,
	T_ID,
	T_CCOMMENT,
	T_COMMENT,
	T_ANYCHAR,
	T_NEWLINE,
	T_CONTINUELINE,
	T_HEXADECIMAL,
	T_NUMBER,
	T_DECIMAL,
	T_EXPONENTIAL,
	T_STRING,
	T_CHAR,
	T_WIDESTRING,
	T_BASIC_TYPE,
	T_MODIFIER,

	MISC_TOKENS_END,

	///==========================================
	/// Keywords
	///==========================================

	KEYWORDS_START,

	SPECIAL_KEYWORDS_START,

	T_SPECIAL_LEFT_BRACE,
	T_SPECIAL_RIGHT_BRACE,
	T_SPECIAL_LEFT_PAREN,
	T_SPECIAL_RIGHT_PAREN,
	T_SPECIAL_LEFT_BRACKET,
	T_SPECIAL_RIGHT_BRACKET,

	SPECIAL_KEYWORDS_END,

	///==========================================
	/// C++ Keywords
	///==========================================

	CPP_KEYWORDS_START,

	T_POUND_DEFINE,
	T_POUND_ELIF,
	T_POUND_ELSE,
	T_POUND_ENDIF,
	T_POUND_ERROR,
	T_POUND_IF,
	T_POUND_IFDEF,
	T_POUND_IFNDEF,
	T_POUND_IMPORT,
	T_POUND_INCLUDE,
	T_POUND_LINE,
	T_POUND_PRAGMA,
	T_POUND_UNDEF,
	T_POUND_USING,
	T_POUND_WARNING,
	T_AUTO,
	T_UNSIGNED,
	T_BREAK,
	T_CONTINUE,
	T_ELSE,
	T_FOR,
	T_SIGNED,
	T_SWITCH,
	T_CASE,
	T_GOTO,
	T_SIZEOF,
	T_TYPEDEF,
	T_DO,
	T_EXTERN,
	T_IF,
	T_RETURN,
	T_WHILE,
	T_ASM,
	T_DYNAMIC_CAST,
	T_REINTERPRET_CAST,
	T_TRY,
	T_EXPLICIT,
	T_NEW,
	T_STATIC_CAST,
	T_TYPEID,
	T_CATCH,
	T_FALSE,
	T_OPERATOR,
	T_TEMPLATE,
	T_TYPENAME,
	T_FRIEND,
	T_THIS,
	T_USING,
	T_CONST_CAST,
	T_THROW,
	T_DELETE,
	T_TRUE,
	T_E_BITAND,
	T_E_COMPL,
	T_E_NOT_EQ,
	T_E_OR_EQ,
	T_E_XOR_EQ,
	T_E_AND_EQ,
	T_E_BITOR,
	T_E_XOR,
	T_OPOBJECT,
	T_OPENUM,
	T_STATE,
	T_CLASS,
	T_INLINE,
	T_STATIC,
	T_UNION,
	T_ENUM,
	T_CONST,
	T_VOID,
	T_STRUCT,
	T_VIRTUAL,
	T_MUTABLE,
	T_VOLATILE,
	T_PRIVATE,
	T_PROTECTED,
	T_PUBLIC,

	CPP_KEYWORDS_END,

	///==========================================
	/// opCPP Keywords
	///==========================================

	OPCPP_KEYWORDS_START,

	T_NAMESPACE,
	T_OPDEFINE,
	T_OPMACRO,
	T_EXPAND,
	T_OPINCLUDE,
	T_UNINLINE,
	T_SPACER,

	OPCPP_KEYWORDS_END,

	///==========================================
	/// opCPP Normal Mode Only Keywords
	///==========================================

	OPCPP_NORMAL_KEYWORDS_START,
	
	T_DEFAULT,
	T_REGISTER,
	
	OPCPP_NORMAL_KEYWORDS_END,

	///==========================================
	/// opCPP Dialect Keywords
	///==========================================

	OPCPP_DIALECT_KEYWORDS_START,

	T_CATEGORY,
	T_DATAMAP,
	T_FUNCTIONMAP,
	T_FUNCTIONMODIFIER,
	T_DATAMODIFIER,
	T_LOCATION,
	T_NOTE,
	T_ENUMERATION,
	T_ENUMMAP,
	T_DISALLOW,
	T_CODE,
	T_EXTENSION,
	T_EXTEND_POINT,
	T_EXTENSION_POINT,
	T_FILE_DECLARATION,
	T_IS,
	T_CPLUSPLUS,
	T_E_AND,	
	T_E_NOT,
	T_E_OR,

	OPCPP_DIALECT_KEYWORDS_END,

	KEYWORDS_END,

	///==========================================
	/// Operators
	///==========================================

	OPERATORS_START,

	T_PLUS,
	T_MINUS,
	T_STAR,
	T_DIVIDE,
	T_MODULUS,
	T_ASSIGN,
	T_PLUS_EQUALS,
	T_MINUS_EQUALS,
	T_STAR_EQUALS,
	T_DIVIDE_EQUALS,
	T_BITWISE_AND_EQUALS,
	T_BITWISE_EXCLUSIVE_OR_EQUALS,
	T_BITWISE_INCLUSIVE_OR_EQUALS,
	T_LEFT_SHIFT_EQUALS,
	T_RIGHT_SHIFT_EQUALS,
	T_MODULUS_EQUALS,
	T_PLUS_PLUS,
	T_MINUS_MINUS,
	T_AND,
	T_OR,
	T_BITWISE_EXCLUSIVE_OR,
	T_BITWISE_INCLUSIVE_OR,
	T_LESS_THAN,
	T_GREATER_THAN,
	T_LESS_THAN_OR_EQUAL_TO,
	T_GREATER_THAN_OR_EQUAL_TO,
	T_EQUALS,
	T_NOT_EQUALS,
	T_NEGATION,
	T_LEFT_SHIFT,
	T_RIGHT_SHIFT,
	T_LEFT_PAREN,
	T_RIGHT_PAREN,
	T_LEFT_BRACKET,
	T_RIGHT_BRACKET,
	T_LEFT_BRACE,
	T_RIGHT_BRACE,
	T_AMPERSAND,
	T_DOT,
	T_ARROW,
	T_DOT_STAR,
	T_ARROW_STAR,
	T_COMMA,
	T_QUESTION_MARK,
	T_COLON,
	T_SEMICOLON,
	T_SCOPE_RESOLUTION,
	T_BITWISE_COMPLEMENT,
	T_POUND,
	T_BACKSLASH,
	T_CONCATENATION,
	T_DOUBLE_ACCENT,
	T_ACCENT,

	OPERATORS_END,

	TERMINALS_END,

	///==========================================
	/// Grammars
	///==========================================

	GRAMMARS_START,

	///==========================================
	/// opCPP Dialect Grammars
	///==========================================

	OPCPP_DIALECT_GRAMMARS_START,

	G_DIALECTFILE,
	G_CATEGORY,
	G_CATEGORY_BODY,
	G_CATEGORY_LOCATION,
	G_CATEGORY_LOCATION_BODY,
	G_CATEGORY_DATAMAP,
	G_CATEGORY_FUNCTIONMAP,
	G_CATEGORY_MAP_BODY,
	G_IS,
	G_IS_BODY,
	G_CRITERIA_ARGUMENT,
	G_CRITERIA_GROUP,
	G_CRITERIA_VALUE_MODIFIER,
	G_DATAMODIFIER,
	G_FUNCTIONMODIFIER,
	G_MODIFIER_ARGUMENT,
	G_NOTE,
	G_NOTE_BODY,
	G_NOTE_DEFINITION,
	G_NOTE_DEFINITION_ARGUMENT_LIST,
	G_NOTE_DEFINITION_ARGUMENT,
	G_ENUMERATION,
	G_ENUMERATION_BODY,
	G_ENUMERATION_LOCATION,
	G_ENUMERATION_LOCATION_BODY,
	G_ENUMERATION_MAP,
	G_DIALECT_MODIFIERS,
	G_DISALLOW,
	G_DISALLOW_BODY,
	G_CODE,
	G_CODE_BODY,
	G_EXTENSION,
	G_EXTEND_POINT,
	G_EXTENSION_POINT,
	G_FILE_DECLARATION,
	G_FILE_DECLARATION_BODY,
	G_FILE_DECLARATION_LOCATION,
	G_DIALECT_NAMESPACE,

	//==========================================
	// opCPP Dialect Modifiers
	//==========================================

	OPCPP_DIALECT_MODIFIERS_START,

	G_ERROR,
	G_DESCRIPTION,
	G_HIDDEN,
	G_BEFORE,
	G_AFTER,
	G_VERBATIM,
	G_CLASS_PREFIX,
	G_STRUCT_PREFIX,
	G_ENUM_PREFIX,
	G_OVERRIDE,

	OPCPP_DIALECT_MODIFIERS_END,

	///==========================================
	/// opCPP Dialect Statement Grammars
	///==========================================

	OPCPP_DIALECT_STATEMENT_GRAMMARS_START,

	G_DATAMODIFIER_STATEMENT,
	G_FUNCTIONMODIFIER_STATEMENT,
	G_CATEGORY_LOCATION_STATEMENT,
	G_NOTE_STATEMENT,
	G_CATEGORY_DATAMAP_STATEMENT,
	G_CATEGORY_FUNCTIONMAP_STATEMENT,
	G_ENUMERATIONMAP_STATEMENT,
	G_ENUMERATION_LOCATION_STATEMENT,
	G_IS_STATEMENT,
	G_DISALLOW_STATEMENT,
	G_FILE_DECLARATION_LOCATION_STATEMENT,
	G_DIALECT_OPINCLUDE_STATEMENT,
	G_CATEGORY_STATEMENT,
	G_ENUMERATION_STATEMENT,
	G_DIALECT_OPDEFINE_STATEMENT,
	G_NOTE_DEFINITION_STATEMENT,
	G_FILE_DECLARATION_STATEMENT,
	G_CODE_STATEMENT,
	G_DIALECT_NAMESPACE_STATEMENT,
	G_EXTENSION_STATEMENT,
	G_EXTEND_POINT_STATEMENT,
	G_DIALECT_OPMACRO_STATEMENT,

	OPCPP_DIALECT_STATEMENT_GRAMMARS_END,

	OPCPP_DIALECT_GRAMMARS_END,

	///==========================================
	/// opCPP Grammars
	///==========================================
	OPCPP_GRAMMARS_START,

	G_CPPFILE,
	G_OPFILE,
	G_FUNDAMENTAL_TYPE,
	G_NAMESPACE,
	G_NAMESPACEBLOCK,
	G_NAMESPACEDECL,
	G_USING_NAMESPACE_KEYWORD,
	G_USING,
	G_NAMESPACE_ALIAS,
	G_BRACE_BLOCK,
	G_PAREN_BLOCK,
	G_BRACKET_BLOCK,
	G_ANGLED_BLOCK,
	G_POINTER,
	G_POINTER_MEMBER,
	G_FUNCTION,
	G_FUNCTION_DEFINITION,
	G_FUNCTION_ARGUMENTLIST,
	G_FUNCTION_ARGUMENT,
	G_FUNCTION_ARGUMENTDEFINITION,
	G_FUNCTION_ARGUMENTDEFAULT,
	G_FUNCTION_PROTOTYPE,
	G_CONSTRUCTOR,
	G_CONSTRUCTOR_DEFINITION,
	G_CONSTRUCTOR_PROTOTYPE,
	G_CONSTRUCTOR_INITIALIZER_LIST,
	G_CONSTRUCTOR_INITIALIZER,
	G_CONSTRUCTOR_INITIALIZER_ARGUMENT,
	G_DESTRUCTOR,
	G_DESTRUCTOR_DEFINITION,
	G_DESTRUCTOR_PROTOTYPE,
	G_OPENUM,
	G_OPENUM_BODY,
	G_OPENUM_ELEMENT,
	G_OPENUM_INITIALIZATION,
	G_OPOBJECT,
	G_OPOBJECT_INHERITANCE,
	G_OPOBJECT_BODY,
	G_STATE,
	G_STATEBLOCK,
	G_TEMPLATE_TYPE,
	G_TEMPLATE_TYPE_BODY,
	G_TEMPLATE_DECL,
	G_ARRAY,
	G_TYPE_ARRAY,
	G_REFERENCE,
	G_VISIBILITY_LABEL,
	G_SCOPE,
	G_SCOPE_POINTER,
	G_TYPENAME,
	G_TYPEDEF,
	G_FUNCTION_POINTER,
	G_FUNCTION_POINTER_DECL,
	G_OPERATOR,
	G_OPDEFINE,
	G_OPDEFINE_BODY,
	G_POUND_DEFINE,
	G_POUND_ELIF,
	G_POUND_ELSE,
	G_POUND_ENDIF,
	G_POUND_ERROR,
	G_POUND_IF,
	G_POUND_IFDEF,
	G_POUND_IFNDEF,
	G_POUND_IMPORT,
	G_POUND_INCLUDE,
	G_POUND_LINE,
	G_POUND_PRAGMA,
	G_POUND_UNDEF,
	G_POUND_USING,
	G_POUND_WARNING,
	G_OPINCLUDE,
	G_MODIFIERS,
	G_AUTO_MODIFIERS,
	G_EXPAND_CALL,
	G_OPMACRO,
	G_OPMACRO_ARGUMENT,
	G_OPMACRO_ARGUMENT_LIST,
	G_OPMACRO_BODY,
	G_EXPAND_CALL_ARGUMENT,
	G_EXPAND_CALL_ARGUMENT_LIST,
	G_CONCATENATION_ARGUMENT_OPERATOR,
	G_CONCATENATION_OPERATOR,
	G_SINGLE_QUOTE_OPERATOR,
	G_DOUBLE_QUOTE_OPERATOR,
	G_VALUED_MODIFIER,
	G_VALUED_MODIFIER_ARGUMENT,
	G_EXPANDABLE_ARGUMENT,
	G_FRIEND,
	G_DATA_DECLARATION,
	G_DATA_INITIALIZATION,
	G_DATA_BITS,
	G_CPLUSPLUS,
	G_CPLUSPLUS_BODY,
	G_CPPCONSTRUCT_INHERITANCE,
	G_CLASS,
	G_STRUCT,
	G_ENUM,
	G_UNION,
	G_TEMPLATED,

	OPCPP_GRAMMARS_END,

	STATEMENT_GRAMMARS_START,

	G_STATEMENT,
	G_FUNCTION_DEFINITION_STATEMENT,
	G_VISIBILITY_STATEMENT,
	G_PREPROCESSOR_STATEMENT,
	G_STATE_STATEMENT,
	G_NULL_STATEMENT,
	G_FRIEND_STATEMENT,
	G_USING_STATEMENT,
	G_TYPEDEF_STATEMENT,
	G_FUNCTION_POINTER_STATEMENT,
	G_FUNCTION_PROTOTYPE_STATEMENT,
	G_OPENUM_STATEMENT,
	G_OPOBJECT_STATEMENT,
	G_DATA_STATEMENT,
	G_TEMPLATE_STATEMENT,
	G_CONSTRUCTOR_DEFINITION_STATEMENT,
	G_CONSTRUCTOR_PROTOTYPE_STATEMENT,
	G_DESTRUCTOR_DEFINITION_STATEMENT,
	G_DESTRUCTOR_PROTOTYPE_STATEMENT,
	G_CPPCONSTRUCT_STATEMENT,
	
	STATEMENT_GRAMMARS_END,

	GRAMMARS_END,

	///==========================================
	/// Special Tokens
	///==========================================

	Tokens_MAX,

	// abstract/undefined tokens
	T_OPNODE,	
	T_UNKNOWN
};

/**** enumstring to #enumstring map ****/

class opTokenNames
{
public:
	// construction / destruction
	opTokenNames() {}
	~opTokenNames() {}

	static void SetTokenName(Token t, const opString& s)
	{
		TokenNames[t] = s;
	}

	static const opString& GetTokenName(Token t)
	{
		return TokenNames[t];
	}

private:
	static opString TokenNames[Tokens_MAX];
};

/**** identification methods ****/

class TokenFunctions
{
public:

	/*=== public static methods ===*/

	static bool IsWithinRange(Token val, Token start, Token end)
	{
		return val > start && val < end;
	}

	static bool IsMiscToken(Token t)
	{
		return IsWithinRange(t,MISC_TOKENS_START,MISC_TOKENS_END);
	}

	static bool IsOperator(Token t)
	{
		return IsWithinRange(t,OPERATORS_START,OPERATORS_END);
	}

	static bool IsSpecialKeyword(Token t)
	{
		return IsWithinRange(t,SPECIAL_KEYWORDS_START,SPECIAL_KEYWORDS_END);
	}

	static bool IsCPPKeyword(Token t)
	{
		return IsWithinRange(t,CPP_KEYWORDS_START,CPP_KEYWORDS_END);
	}

	static bool IsOPCPPKeyword(Token t)
	{
		return IsWithinRange(t,OPCPP_KEYWORDS_START,OPCPP_KEYWORDS_END);
	}

	static bool IsOPCPPDialectKeyword(Token t)
	{
		return IsWithinRange(t,OPCPP_DIALECT_KEYWORDS_START,OPCPP_DIALECT_KEYWORDS_END);
	}

	static bool IsOPCPPDialectModifier(Token t)
	{
		return IsWithinRange(t,OPCPP_DIALECT_MODIFIERS_START,OPCPP_DIALECT_MODIFIERS_END);
	}

	static bool IsNormalKeyword(Token t)
	{
		return IsWithinRange(t,OPCPP_NORMAL_KEYWORDS_START,OPCPP_NORMAL_KEYWORDS_END);
	}

	static bool IsKeyword(Token t)
	{
		return IsWithinRange(t,KEYWORDS_START,KEYWORDS_END);
	}

	static bool IsOPCPPDialectGrammar(Token t)
	{
		return IsWithinRange(t,OPCPP_DIALECT_GRAMMARS_START,OPCPP_DIALECT_GRAMMARS_END);
	}

	static bool IsStatement(Token t)
	{
		return IsWithinRange(t, STATEMENT_GRAMMARS_START,STATEMENT_GRAMMARS_END)
			|| IsWithinRange(t, OPCPP_DIALECT_STATEMENT_GRAMMARS_START, OPCPP_DIALECT_STATEMENT_GRAMMARS_END);
	}

	static bool IsGrammar(Token t)
	{
		return IsWithinRange(t,GRAMMARS_START,GRAMMARS_END);
	}

	static bool IsTerminal(Token t)
	{
		return IsWithinRange(t,TERMINALS_START,TERMINALS_END);
	}

	static bool IsNormalToken(Token t)
	{
		return IsNormalKeyword(t) || IsCPPKeyword(t);
	}

	static bool IsDialectToken(Token t)
	{
		return IsOPCPPDialectKeyword(t);
	}

	static bool IsModifier(Token t)
	{
		//return IsWithinRange(t, OPCPP_MODIFIERS_START, OPCPP_MODIFIERS_END)
		//TODO: speed up
		return (t == T_INLINE 
			||  t == T_VOLATILE
			||  t == T_MUTABLE
			||  t == T_CONST
			||  t == T_STATIC
			||  t == T_PUBLIC
			||  t == T_PROTECTED
			||  t == T_PRIVATE
			||	t == T_UNINLINE
			||  t == T_MODIFIER
			||  t == G_VALUED_MODIFIER
			||  t == T_EXPLICIT
			);
	}

	static bool IsStartEndToken(Token t)
	{
		//sloowww
		return (t == MISC_TOKENS_START ||
			t == MISC_TOKENS_END   ||
			t == SPECIAL_KEYWORDS_START ||
			t == SPECIAL_KEYWORDS_END ||
			t == CPP_KEYWORDS_START ||
			t == CPP_KEYWORDS_END ||
			t == OPERATORS_START ||
			t == OPERATORS_END ||
			t == OPCPP_KEYWORDS_START ||
			t == OPCPP_KEYWORDS_END ||
			t == OPCPP_NORMAL_KEYWORDS_START ||
			t == OPCPP_NORMAL_KEYWORDS_END ||
			t == OPCPP_DIALECT_KEYWORDS_START ||
			t == OPCPP_DIALECT_KEYWORDS_END ||
			t == OPCPP_DIALECT_GRAMMARS_START ||
			t == OPCPP_DIALECT_GRAMMARS_END ||
			t == GRAMMARS_START ||
			t == GRAMMARS_END ||
			t == Tokens_MAX ||
			t == Tokens_MIN ||
			t == STATEMENT_GRAMMARS_START ||
			t == STATEMENT_GRAMMARS_END ||
			t == OPCPP_DIALECT_STATEMENT_GRAMMARS_START ||
			t == OPCPP_DIALECT_STATEMENT_GRAMMARS_END ||
			t == KEYWORDS_START ||
			t == KEYWORDS_END ||
			t == TERMINALS_START ||
			t == TERMINALS_END ||
			t == OPCPP_GRAMMARS_START ||
			t == OPCPP_GRAMMARS_END ||
			t == OPCPP_DIALECT_MODIFIERS_START ||
			t == OPCPP_DIALECT_MODIFIERS_END
			);
	}

	static opString ToString(Token t)
	{
		return opTokenNames::GetTokenName(t);
	}

	static bool IsComment(Token t)
	{
		return t == T_COMMENT || t == T_CCOMMENT;
	}

	static bool IsWhitespace(Token t)
	{
		return t == T_WHITESPACE || t == T_NEWLINE;
	}
};

/**** maps ****/

class opTokenMap
{
public:
	// construction / destruction
	opTokenMap() {}

	~opTokenMap() {}

	/**** utility ****/

	static opString GetString(Token t)
	{
		return TokenToString[t];
	}

	static Token GetToken(const opString& s)
	{
		Token t       = T_UNKNOWN;
		bool  bResult = StringToToken.Find(s,t);
	
		return t;
	}

	static void AddPair(Token t, const opString& s)
	{
		TokenToString[t] = s;
		StringToToken.Insert(s, t);
	}

	/*=== These methods are for registering dialect modifiers only. ===*/

	static void AddDialectModifierPair(Token t, const opString& s)
	{
		TokenToDialectModifier[t] = s;
	}

	static opString GetDialectModifierString(Token t)
	{
		return TokenToDialectModifier[t];
	}

private:
	static opString					    TokenToString[Tokens_MAX];
	static opHashTable<opString, Token> StringToToken;
	static opHashTable<Token, opString> TokenToDialectModifier;
};

/**** opToken class ****/

// Initialization function for tokens.
void InitTokens();

// struct for an opToken
struct opToken
{
	// construction / destruction
	opToken()
		: Id(Tokens_MAX), Line(0)
	{
	}

	opToken(Token _id, const opString &_value, int _line)
		: Id(_id), Value(_value), Line(_line)
	{
	}

	opToken(const opToken &s)
		: Id(s.Id), Value(s.Value), Line(s.Line) {}

	Token    Id;
	opString Value;
	int      Line;
};

//
// Compile time token mappings
//

namespace tokenmapping
{
	//template compile-time mapping functions

	//TokenNodeType (get the node type from the token id)
	template<Token TokenType>
	struct TokenNodeType
	{
		//valid types contain typedef NodeType type;
	};

	//NodeToken (get the token id from the type)
	template<class NodeType>
	struct NodeTypeToken
	{
		//static const Token id = T_UNKNOWN;
	};

	template<>
	struct NodeTypeToken<TerminalNode>
	{
		static const Token id = T_UNKNOWN;
	};

	//TokenString - friendliest name, may be the node name, i.e. "TerminalNode"
	template<Token TokenType>
	const char* TokenString()
	{
		return "";//unknown
	}

	//Actual stringized version of Token, i.e. "T_WHITESPACE"
	template<Token TokenType>
	const char* TokenIdString()
	{
		return "";//unknown
	}

	//IsMapped
	template<Token TokenType>
	struct IsMapped
	{
		enum { value = false };
	};

	//IsGrammar
	template<Token TokenType>
	struct IsGrammar
	{
		enum { value = false };
	};

	template<class NodeType>
	struct NodeTypeIsGrammar
	{
		enum { value = false };
	};

	//IsKeyword
	template<Token TokenType>
	struct IsKeyword
	{
		enum { value = false };
	};

	//IsName
	template<Token TokenType>
	struct IsName
	{
		enum { value = false };
	};

	//IsOperator
	template<Token TokenType>
	struct IsOperator
	{
		enum { value = false };
	};

	//RegisterMappings
	template<Token TokenType>
	void RegisterMapping()
	{

	}

	template<Token TokenType>
	void RegisterOperator()
	{
		opTokenMap::AddPair(TokenType,TokenString<TokenType>());
	}

	template<Token TokenType>
	void RegisterKeyword()
	{
		opTokenMap::AddPair(TokenType,TokenString<TokenType>());
	}

	/*=== dialect modifier registration ===*/

	template<Token TokenType>
	struct IsDialectModifier
	{
		enum { value = false };
	};

	// Given something like G_OVERRIDE, returns "override".
	template<Token TokenType>
	const char* DialectModifierString()
	{
		return "";
	}

	template<Token TokenType>
	void RegisterDialectModifier()
	{
		opTokenMap::AddDialectModifierPair(TokenType, DialectModifierString<TokenType>());
	}
}



