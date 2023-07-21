///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: opError.cpp
/// Date: 11/26/2006
///
/// Description:
///
/// Contains source code for error logging, including initialization of errors, etc.
///****************************************************************

#include "opCPP.h"

///
/// opLog
///

ostream* opLog::o       = &cout;
opString opLog::lastLog = "first_log";

///
/// opError
///

opList<ErrorInfo> opError::Errors;
opString		  opError::ParseErrors[Tokens_MAX + 1];
Token             opError::ContextToken       = Tokens_MAX;
opNode*			  opError::ContextNode        = NULL;
int               opError::MaxErrorNameLength = 40;

// This method initializes all the parse errors.
void opError::InitParseErrors()
{
	// Initialize default parse errors.
	for (int i = 0; i < Tokens_MAX; i++)
	{
		Token t = (Token) i;

		if (TokenFunctions::IsTerminal(t))
			ParseErrors[t] = opTokenMap::GetString(t);
		else
			ParseErrors[t] = TokenFunctions::ToString(t) + " parse error uninitialized ... please report to www.opcpp.com";
	}

	///==========================================
	/// Miscellaneous Tokens
	///==========================================

	ParseErrors[T_EOF] = "End Of File";
	ParseErrors[T_WHITESPACE] = "Whitespace";
	ParseErrors[T_ID] = "Identifier";
	ParseErrors[T_CCOMMENT] = "C Style Comment";
	ParseErrors[T_COMMENT] = "C++ Style Comment";
	ParseErrors[T_ANYCHAR] = "Any Character";
	ParseErrors[T_HEXADECIMAL] = "Hexadecimal Number";
	ParseErrors[T_NEWLINE] = "Newline";
	ParseErrors[T_CONTINUELINE] = "Macro Continuation Character";
	ParseErrors[T_NUMBER] = "Number";
	ParseErrors[T_DECIMAL] = "Decimal";
	ParseErrors[T_EXPONENTIAL] = "Exponential";
	ParseErrors[T_STRING] = "String";
	ParseErrors[T_CHAR] = "Character";
	ParseErrors[T_WIDESTRING] = "Wide String";
	ParseErrors[T_BASIC_TYPE] = "Basic Type";
	ParseErrors[T_MODIFIER] = "User-Defined Modifier";
	ParseErrors[Tokens_MAX] = "Unknown error occurred. :(";
	ParseErrors[T_OPOBJECT] = "opC++ Category";
	ParseErrors[T_OPENUM] = "opC++ Enumeration";

	///==========================================
	/// Grammars
	///==========================================

	/*=== code grammars ===*/

	// special characters: « »

	ParseErrors[G_OPFILE] = "opC++ File";//ErrorNamed
	ParseErrors[G_CPPFILE] = "C++ File";//ErrorNamed
	ParseErrors[G_FUNDAMENTAL_TYPE] = "Fundamental-Type";//ErrorNamed
	ParseErrors[G_NAMESPACE] = "namespace Name { ... }";//ErrorNamed
	ParseErrors[G_NAMESPACEBLOCK] = "namespace Name {" LCHEV " ... " RCHEV "}";//ErrorNamed
	ParseErrors[G_NAMESPACEDECL] = "namespace Name { ... }";//ErrorNamed
	ParseErrors[G_NAMESPACE_ALIAS] = "namespace Alias = Name;";
	ParseErrors[G_BRACE_BLOCK] = "{ ... }";//ok
	ParseErrors[G_PAREN_BLOCK] = "( ... )";//ok
	ParseErrors[G_BRACKET_BLOCK] = "[ ... ]";//ok
	ParseErrors[G_ANGLED_BLOCK] = "< ... >";//ok
	ParseErrors[G_POINTER] = "Pointer-Type";//ErrorNamed
	ParseErrors[G_POINTER_MEMBER] = "Type Scope::*";
	ParseErrors[G_OPENUM] = "openum Name { ... };";//ErrorNamed
	ParseErrors[G_OPENUM_BODY] = "openum Name {" LCHEV " ... " RCHEV "};";//ErrorNamed
	ParseErrors[G_OPENUM_ELEMENT] = "EnumName [= ... ],";//ErrorNamed
	ParseErrors[G_OPENUM_INITIALIZATION] = "EnumName = " LCHEV " Value " RCHEV ",";//ErrorNamed
	ParseErrors[G_OPOBJECT_INHERITANCE] = ": Visibility Name";//ErrorNamed
	ParseErrors[G_STATE] = "state Name { ... }";//ErrorNamed;
	ParseErrors[G_STATEBLOCK] = "state Name {" LCHEV " ... " RCHEV "}";//ErrorNamed;
	ParseErrors[G_TEMPLATE_TYPE] = "TemplateName< ... >";//ErrorNamed
	ParseErrors[G_TEMPLATE_TYPE_BODY] = "< ... >";//ErrorNamed
	ParseErrors[G_TEMPLATE_DECL] = "template< ... >";//ErrorNamed
	ParseErrors[G_FUNCTION] = "FunctionName( ... )";//ErrorNamed
	ParseErrors[G_FUNCTION_DEFINITION] = "ReturnType FunctionName( ... ) { ... }";//ErrorNamed
	ParseErrors[G_FUNCTION_PROTOTYPE] = "ReturnType FunctionName( ... )";//ErrorNamed
	ParseErrors[G_FUNCTION_ARGUMENTLIST] = "( ... )";//ErrorNamed
	ParseErrors[G_FUNCTION_ARGUMENT] = "Name [= Default]";//ErrorNamed
	ParseErrors[G_FUNCTION_ARGUMENTDEFINITION] = "Name [= Default]";//ok
	ParseErrors[G_FUNCTION_ARGUMENTDEFAULT] = "Name = Default";//ok
	ParseErrors[G_CONSTRUCTOR] = "Constructor( ... )";//ErrorNamed
	ParseErrors[G_CONSTRUCTOR_PROTOTYPE] = "Constructor( ... )";//ErrorNamed
	ParseErrors[G_CONSTRUCTOR_DEFINITION] = "Constructor( ... ) { ... }";//ErrorNamed
	ParseErrors[G_CONSTRUCTOR_INITIALIZER_LIST] = "Constructor( ... ) : " LCHEV " ... " RCHEV;//ErrorNamed
	ParseErrors[G_CONSTRUCTOR_INITIALIZER] = "Initializer( ... )";//ErrorNamed
	ParseErrors[G_CONSTRUCTOR_INITIALIZER_ARGUMENT] = "Initializer(" LCHEV " ... " RCHEV ")";//ErrorNamed
	ParseErrors[G_DESTRUCTOR] = "~Destructor( ... )";
	ParseErrors[G_DESTRUCTOR_PROTOTYPE] = "~Destructor( ... )";
	ParseErrors[G_DESTRUCTOR_DEFINITION] = "~Destructor( ... ) { ... }";
	ParseErrors[G_ARRAY] = "Name[ ... ]";//ErrorNamed
	ParseErrors[G_TYPE_ARRAY] = "Type[ ... ]";//ErrorNamed
	ParseErrors[G_VISIBILITY_LABEL] = "public: or private: or protected:";//ErrorNamed
	ParseErrors[G_SCOPE] = "Scope::Specifier";//ErrorNamed
	ParseErrors[G_SCOPE_POINTER] = "Scope::*";//ErrorNamed
	ParseErrors[G_REFERENCE] = "Reference-Type";//ErrorNamed
	ParseErrors[G_TYPENAME] = "typename Type";//ErrorNamed
	ParseErrors[G_FUNCTION_POINTER] = "ReturnType ( *Name )( ... )";//ErrorNamed
	ParseErrors[G_FUNCTION_POINTER_DECL] = "ReturnType (" LCHEV " *Name " RCHEV ")( ... )";//ErrorNamed
	ParseErrors[G_OPERATOR] = "operator OperatorName";//ErrorNamed
	ParseErrors[G_OPDEFINE] = "opdefine Name [optional ( ... )] { ... }";//ErrorNamed
	ParseErrors[G_OPDEFINE_BODY] = "opdefine Name [optional ( ... )] {" LCHEV " ... " RCHEV "}";//ErrorNamed
	ParseErrors[G_OPMACRO] = "opmacro Name [optional ( ... )] { ... }";//ErrorNamed
	ParseErrors[G_OPMACRO_BODY] = "opmacro Name [optional ( ... )] {" LCHEV " ... " RCHEV "}";//ErrorNamed
	ParseErrors[G_OPMACRO_ARGUMENT] = "[optional 'argument'] Name";//ok
	ParseErrors[G_OPMACRO_ARGUMENT_LIST] = "opmacro Name (" LCHEV " ... " RCHEV ")";//ErrorNamed
	ParseErrors[G_POUND_DEFINE] = "Standard C++ #define Definition";//ErrorNamed
	ParseErrors[G_POUND_ELIF] = "Standard C++ #elif Definition";//ErrorNamed
	ParseErrors[G_POUND_ELSE] = "Standard C++ #else Definition";//ErrorNamed
	ParseErrors[G_POUND_ENDIF] = "Standard C++ #endif Definition";//ErrorNamed
	ParseErrors[G_POUND_ERROR] = "Standard C++ #error Definition";//ErrorNamed
	ParseErrors[G_POUND_IF] = "Standard C++ #if Definition";//ErrorNamed
	ParseErrors[G_POUND_IFDEF] = "Standard C++ #ifdef Definition";//ErrorNamed
	ParseErrors[G_POUND_IFNDEF] = "Standard C++ #ifndef Definition";//ErrorNamed
	ParseErrors[G_POUND_IMPORT] = "Standard C++ #import Definition";//ErrorNamed
	ParseErrors[G_POUND_INCLUDE] = "Standard C++ #include Definition";//ErrorNamed
	ParseErrors[G_POUND_LINE] = "Standard C++ #line Definition";//ErrorNamed
	ParseErrors[G_POUND_PRAGMA] = "Standard C++ #pragma Definition";//ErrorNamed
	ParseErrors[G_POUND_UNDEF] = "Standard C++ #undef Definition";//ErrorNamed
	ParseErrors[G_POUND_USING] = "Standard C++ #using Definition";//ErrorNamed
	ParseErrors[G_POUND_WARNING] = "Standard C++ #warning Definition";//ErrorNamed
	ParseErrors[G_OPINCLUDE] = "opinclude \"Filename\"";//ErrorNamed
	ParseErrors[G_OPOBJECT] = "CategoryType Name [ : Visibility Inheritance ] { ... }";//ErrorNamed
	ParseErrors[G_OPOBJECT_INHERITANCE] = ": Visibility Inheritance";//ErrorNamed
	ParseErrors[G_OPOBJECT_BODY] = "CategoryType Name {" LCHEV " ... " RCHEV "}";//ErrorNamed
	ParseErrors[G_EXPAND_CALL] = "expand Name( ... )";//ErrorNamed
	ParseErrors[G_EXPAND_CALL_ARGUMENT_LIST] = "expand Name(" LCHEV " ... " RCHEV ")";//ErrorNamed
	ParseErrors[G_EXPAND_CALL_ARGUMENT] = ParseErrors[G_EXPAND_CALL_ARGUMENT_LIST];//ok
	ParseErrors[G_CONCATENATION_OPERATOR] = "Identifier@Identifier";
	ParseErrors[G_CONCATENATION_ARGUMENT_OPERATOR] = "Identifier";
	ParseErrors[G_SINGLE_QUOTE_OPERATOR] = "`quoted character`";
	ParseErrors[G_DOUBLE_QUOTE_OPERATOR] = "``quoted string``";
	ParseErrors[G_VALUED_MODIFIER] = "Modifier( ... )";//ErrorNamed
	ParseErrors[G_VALUED_MODIFIER_ARGUMENT] = "Modifier(" LCHEV " ... " RCHEV ")";//ErrorNamed
	ParseErrors[G_FRIEND] = "friend [optional class/struct] Name or friend FunctionPrototype or friend FunctionDefinition";//ErrorNamed
	ParseErrors[G_MODIFIERS] = "Modifiers";						//TODO: what does this do? ever get called?
	ParseErrors[G_AUTO_MODIFIERS] = "Modifiers";				//TODO: what does this do? ever get called?
	ParseErrors[G_TYPEDEF] = "typedef [optional const/volatile] Type Name";//ErrorNamed
	ParseErrors[G_DATA_DECLARATION] = "Modifiers Type Name";
	ParseErrors[G_DATA_INITIALIZATION] = "Modifiers Type Name = ...";
	ParseErrors[G_DATA_BITS] = " : " LCHEV " Bits " RCHEV;
	ParseErrors[G_CPLUSPLUS] = "c++ { ... }";
	ParseErrors[G_CPLUSPLUS_BODY] = ParseErrors[G_CPLUSPLUS];
	ParseErrors[G_USING_NAMESPACE_KEYWORD] = "using namespace";
	ParseErrors[G_USING] = "using Scope";
	ParseErrors[G_CPPCONSTRUCT_INHERITANCE] = ": Visibility Inheritance";
	ParseErrors[G_CLASS] = "class Name [ : Visibility Inheritance ] { ... };";
	ParseErrors[G_STRUCT] = "struct Name [ : Visibility Inheritance ] { ... };";
	ParseErrors[G_ENUM] = "enum Name { ... };";
	ParseErrors[G_UNION] = "union Name { ... };";
	ParseErrors[G_TEMPLATED] = "template<...> Construct";

	/*=== code statement grammars ===*/
	
	ParseErrors[G_STATEMENT] = ParseErrors[G_OPOBJECT_BODY];//ErrorNamed
	ParseErrors[G_FUNCTION_DEFINITION_STATEMENT] = "Modifiers ReturnType FunctionName( ... ) { ... }";//ErrorNamed
	ParseErrors[G_VISIBILITY_STATEMENT] = ParseErrors[G_VISIBILITY_LABEL];//ok?
	ParseErrors[G_PREPROCESSOR_STATEMENT] = "C++ preprocessor statement";
	ParseErrors[G_NULL_STATEMENT] = ";";//ok?
	ParseErrors[G_FRIEND_STATEMENT] = "friend [optional class/struct] Name;";//ErrorNamed
	ParseErrors[G_TYPEDEF_STATEMENT] = "typedef [optional const/volatile] Type Name;";//ErrorNamed
	ParseErrors[G_FUNCTION_POINTER_STATEMENT] = "Modifiers ReturnType (*Name)( ... );";//ErrorNamed
	ParseErrors[G_STATE_STATEMENT] = "state Name { ... }";//ErrorNamed
	ParseErrors[G_FUNCTION_PROTOTYPE_STATEMENT] = "Modifiers ReturnType FunctionName( ... );";//ErrorNamed
	ParseErrors[G_DATA_STATEMENT] = "Modifiers Type Name;";//ErrorNamed
	ParseErrors[G_TEMPLATE_STATEMENT] = "template< ... > ...;";//ErrorNamed
	ParseErrors[G_CONSTRUCTOR_DEFINITION_STATEMENT] = "Modifiers Constructor( ... ) { ... }";//ErrorNamed
	ParseErrors[G_CONSTRUCTOR_PROTOTYPE_STATEMENT] = "Modifiers Constructor( ... );";//ErrorNamed
	ParseErrors[G_DESTRUCTOR_DEFINITION_STATEMENT] = "Modifiers ~Destructor( ... ) { ... }";//ErrorNamed
	ParseErrors[G_DESTRUCTOR_PROTOTYPE_STATEMENT] = "Modifiers ~Destructor( ... );";
	ParseErrors[G_OPENUM_STATEMENT] = ParseErrors[G_OPENUM];//ErrorNamed
	ParseErrors[G_OPOBJECT_STATEMENT] =	ParseErrors[G_OPOBJECT];//ErrorNamed
	ParseErrors[G_USING_STATEMENT] = "using Scope;";//ErrorNamed
	
	/*=== dialect grammars ===*/
	
	ParseErrors[G_DIALECTFILE] = "opC++ Dialect File";//ok
	ParseErrors[G_CATEGORY] = "category Name { ... }";//ErrorNamed
	ParseErrors[G_CATEGORY_BODY] = "category Name {" LCHEV " ... " RCHEV "}";//ErrorNamed
	ParseErrors[G_CATEGORY_DATAMAP] = "datamap Name { ... }";//ErrorNamed
	ParseErrors[G_CATEGORY_FUNCTIONMAP] = "functionmap Name { ... }";//ErrorNamed
	ParseErrors[G_DATAMODIFIER] = "datamodifier Name or datamodifier Name()";//ErrorNamed
	ParseErrors[G_FUNCTIONMODIFIER] = "functionmodifier Name or functionmodifier Name()";//ErrorNamed
	ParseErrors[G_IS] = "is ModifierExpression";//ErrorNamed
	ParseErrors[G_IS_BODY] = "is " LCHEV " ModifierExpression " RCHEV;  
	ParseErrors[G_CRITERIA_ARGUMENT] = "\"Criteria Regex Pattern\"";//ErrorNamed
	ParseErrors[G_CATEGORY_LOCATION] = "location Name { ... }";//ErrorNamed
	ParseErrors[G_CATEGORY_LOCATION_BODY] = "location Name {" LCHEV " ... " RCHEV "}";//ErrorNamed
	ParseErrors[G_NOTE] = "note Name";//ErrorNamed
	ParseErrors[G_NOTE_BODY] = "note Name {" LCHEV " ... " RCHEV "}";
	ParseErrors[G_NOTE_DEFINITION] = "note specifier::tag::subtag... ( ... ) { ... }";//ErrorNamed
	ParseErrors[G_NOTE_DEFINITION_ARGUMENT_LIST] = "note specifier::tag::subtag... (" LCHEV " ... " RCHEV ")";//ErrorNamed
	ParseErrors[G_NOTE_DEFINITION_ARGUMENT] = "note argument";//ok
	ParseErrors[G_EXPANDABLE_ARGUMENT] = "derived note argument";//ok
	ParseErrors[G_DIALECT_MODIFIERS] = "Modifiers";//ok
	ParseErrors[G_MODIFIER_ARGUMENT] = "ModifierName(" LCHEV " \"optional regex expression\" " RCHEV ")";//ErrorNamed
	ParseErrors[G_CATEGORY_MAP_BODY] = "map Name {" LCHEV " ... " RCHEV "}";//ErrorNamed
	ParseErrors[G_ENUMERATION] = "enumeration Name { ... }";//ErrorNamed
	ParseErrors[G_ENUMERATION_BODY] = "enumeration Name {" LCHEV " ... " RCHEV "}";//ErrorNamed
	ParseErrors[G_ENUMERATION_LOCATION] = "location Name { ... }";//ErrorNamed
	ParseErrors[G_ENUMERATION_LOCATION_BODY] = "location Name {" LCHEV " ... " RCHEV "}";//ErrorNamed
	ParseErrors[G_ENUMERATION_MAP] = "enummap Name";//ErrorNamed	
	ParseErrors[G_DISALLOW] = "disallow Name { ... }";
	ParseErrors[G_DISALLOW_BODY] = "disallow Name {" LCHEV " ... " RCHEV "}";
	ParseErrors[G_CRITERIA_GROUP] = "( ... criteria ... )";
	ParseErrors[G_CRITERIA_VALUE_MODIFIER] = "Modifier(\"optional regex pattern\")";
	ParseErrors[G_CODE] = "code Location { ... }";
	ParseErrors[G_CODE_BODY] = "code Location {" LCHEV " ... " RCHEV "}";
	ParseErrors[G_EXTENSION] = "extension Name";
	ParseErrors[G_EXTEND_POINT] = "extendpoint Name { ... }";
	ParseErrors[G_EXTENSION_POINT] = "extensionpoint Name";
	ParseErrors[G_FILE_DECLARATION] = "file Name { ... }";
	ParseErrors[G_FILE_DECLARATION_BODY] = "file Name {" LCHEV " ... " RCHEV "}";
	ParseErrors[G_FILE_DECLARATION_LOCATION] = "location Name";
	ParseErrors[G_DIALECT_NAMESPACE] = "namespace Name { ... }";
	ParseErrors[G_ERROR] = "error( ... )";//ErrorNamed
	ParseErrors[G_DESCRIPTION] = "description( ... )";//ErrorNamed
	ParseErrors[G_HIDDEN] = "hidden";//ErrorNamed
	ParseErrors[G_BEFORE] = "before( ... )";//ErrorNamed
	ParseErrors[G_AFTER] = "after( ... )";//ErrorNamed
	ParseErrors[G_VERBATIM] = "verbatim";//ErrorNamed
	ParseErrors[G_CLASS_PREFIX] = "classprefix( ... )";//ErrorNamed
	ParseErrors[G_STRUCT_PREFIX] = "structprefix( ... )";//ErrorNamed
	ParseErrors[G_ENUM_PREFIX] = "enumprefix( ... )";//ErrorNamed
	ParseErrors[G_OVERRIDE] = "override";//ErrorNamed

	/*=== dialect statement grammars ===*/
	
	//TODO: all these route to DialectStatementBase, may want specific errors
	ParseErrors[G_CATEGORY_DATAMAP_STATEMENT] = "datamap Name { ... }";
	ParseErrors[G_CATEGORY_FUNCTIONMAP_STATEMENT] = "functionmap Name { ... }";
	ParseErrors[G_NOTE_STATEMENT] = "note Name;";
	ParseErrors[G_DATAMODIFIER_STATEMENT] = "datamodifier Name; or datamodifier Name();";
	ParseErrors[G_FUNCTIONMODIFIER_STATEMENT] = "functionmodifier Name; or functionmodifier Name();";
	ParseErrors[G_CATEGORY_LOCATION_STATEMENT] = ParseErrors[G_CATEGORY_LOCATION];
	ParseErrors[G_IS_STATEMENT] = "is ModifierExpression;";
	ParseErrors[G_DISALLOW_STATEMENT] = ParseErrors[G_DISALLOW];
	ParseErrors[G_ENUMERATIONMAP_STATEMENT] = ParseErrors[G_ENUMERATION_MAP];
	ParseErrors[G_ENUMERATION_LOCATION_STATEMENT] = ParseErrors[G_ENUMERATION_LOCATION];
	ParseErrors[G_CPPCONSTRUCT_STATEMENT] = "C++ class, struct or enum";
	ParseErrors[G_FILE_DECLARATION_LOCATION_STATEMENT] = ParseErrors[G_FILE_DECLARATION_LOCATION];
	ParseErrors[G_DIALECT_NAMESPACE_STATEMENT] = ParseErrors[G_DIALECT_NAMESPACE];
	ParseErrors[G_DIALECT_OPINCLUDE_STATEMENT] = ParseErrors[G_OPINCLUDE];
	ParseErrors[G_CATEGORY_STATEMENT] = ParseErrors[G_CATEGORY];
	ParseErrors[G_ENUMERATION_STATEMENT] = ParseErrors[G_ENUMERATION];
	ParseErrors[G_DIALECT_OPDEFINE_STATEMENT] = ParseErrors[G_OPDEFINE];
	ParseErrors[G_NOTE_DEFINITION_STATEMENT] = ParseErrors[G_NOTE_DEFINITION];
	ParseErrors[G_FILE_DECLARATION_STATEMENT] = ParseErrors[G_FILE_DECLARATION];
	ParseErrors[G_CODE_STATEMENT] = ParseErrors[G_CODE];
	ParseErrors[G_EXTENSION_STATEMENT] = ParseErrors[G_EXTENSION];
	ParseErrors[G_EXTEND_POINT_STATEMENT] = ParseErrors[G_EXTEND_POINT];
	ParseErrors[G_DIALECT_OPMACRO_STATEMENT] = ParseErrors[G_OPMACRO];
	
	// Put parse errors above this line in the appropriate spot.
	
	// Print out missing errors.
	PrintMissingErrors();
}

const int ErrorPosition = __LINE__ - 6;

// This method prints any uninitialized errors.
void opError::PrintMissingErrors()
{
	// Only print missing errors in dev mode.
	if ( !opParameters::Get().DeveloperMode )
		return;

	// Do we have any missing error strings?
	bool bMissing = false;

	for (int i = 0; i < Tokens_MAX; i++)
	{
		Token t = (Token) i;

		if (!TokenFunctions::IsTerminal(t)
		&&  !TokenFunctions::IsStartEndToken(t)
		&&  !ParseErrors[t].Length())
		{
			bMissing = true;	
			break;
		}
	}

	if(!bMissing)
		return;
	else
	{
		Log("Uninitialized Errors");
		Log("--------------------");
	}

	// Initialize missing error codes and
	// print them out in debug mode.
	for (int i = 0; i < Tokens_MAX; i++)
	{
		Token t = (Token) i;

		if (!TokenFunctions::IsStartEndToken(t) 
		&&  !ParseErrors[t].Length())
		{
			ParseErrors[t] = TokenFunctions::ToString(t) + " parse error uninitialized ... please report to www.opcpp.com";

			Log(opString(__FILE__) + "(" + opString(ErrorPosition) + ") : " + TokenFunctions::ToString(t));
		}
	}

	Log("");
}

// Prints the errors sorted.
void opError::Print()
{
	//TODO: improve error printing/handling
	if(!Errors.IsEmpty())
	{
		Errors.Sort(opError::SortFunctor);

		opList<ErrorInfo>::iterator i   = Errors.Begin();
		opList<ErrorInfo>::iterator end = Errors.End();

		Log(" ");

		bool bfixedsys = opParameters::Get().FixedSys;

		while (i != end)
		{
			opString errorstring = i->Error.GetString();

			if(bfixedsys)
			{
				errorstring.Replace(LCHEV,(char)0xAE);
				errorstring.Replace(RCHEV,(char)0xAF);
			}

			Log(errorstring);
			Log(" ");

			++i;
		}

		opDriver::AddErrors(opError::NumErrors());
	}
}

// Clears the internal list of errors.
void opError::Clear()
{
	Errors.Clear();
}

// Truncates error names and adds '...' if necessary.
void opError::TruncateErrorName(opString& name)
{
	if (name.Length() > MaxErrorNameLength)
	{
		name  = name.Substring(0, MaxErrorNameLength);

		if (name[MaxErrorNameLength - 1] != ' ')
			name += " ";

		name += "...";
	}
}

// This method adds an error to the internal list.
void opError::Add(FileNode* file, int line, const opString& error)
{
	ErrorInfo newError(file, line, "");

	//NOTE: using absolute paths for errors now.
	path filepath = file->GetInputName().GetString();
	filepath = initial_path() / filepath;
	filepath.normalize();

	opString nativefile = filepath.string();

	newError.Error = nativefile + "(" + line + ") : error :\n" + error;

	Errors.push_back(newError);
}

// This functor sorts errors first by filename, then by line number.
bool opError::SortFunctor(const ErrorInfo& e1, const ErrorInfo& e2)
{
	if (e1.File == e2.File)
		return e1.Line < e2.Line;

	return e1.File->GetInputName() < e2.File->GetInputName();
}

opString opError::NodeString(opNode* node)
{
	//if its a terminal node we want the value
	if (TerminalNode* terminal = node_cast<TerminalNode>(node))
		return terminal->GetValue();

	//if we want the 
	opString errorname = node->ErrorName();
	
	if(errorname.Length())
	{
		//do some cleanup
		errorname.Remove('\n');
		errorname.Replace('\t',' ');
		return errorname;
	}

	return ParseErrors[node->GetId()];
}

opString opError::ContextString()
{
	if (!ContextNode)
		return ParseErrors[ContextToken];

	if (ContextToken != ContextNode->GetId())
		return ParseErrors[ContextToken];
	else
	{
		opString errorname = ContextNode->ErrorName();
		
		if(errorname.Length())
		{
			//do some cleanup
			errorname.Remove('\n');
			errorname.Replace('\t',' ');

			return errorname;
		}
		
		return ParseErrors[ContextToken];
	}
}

///==========================================
/// opError Error Functions
///==========================================

/*=== exception errors ===*/

// "<Exception> Exception Caught! : Please report to www.opcpp.com"
void opError::ExceptionError(const opString &Exception)
{
	Log(Exception + " Exception Caught! : Please report to www.opcpp.com");
	Log(opParameters::Get().GetCommandLineString());
	Log(" ");
}

/*=== scanner errors ===*/

// Found unbounded C-style comment.
void opError::UnboundedCommentError(opNode* root, int line)
{
	opString error = "\tFound unbounded C-style comment.";
	
	Add(root->GetFile(), line, error);
}

// Found unbounded string.
void opError::UnboundedStringError(opNode* root, int line)
{
	opString error = "\tFound unbounded string.";

	Add(root->GetFile(), line, error);
}

/*=== compiler errors ===*/

// "Name <string message> already defined.  See previous definition."
void opError::RedefinitionError(const opString& name, opNode* olddef, opNode* newdef)
{
	opString error = "\t'";

	error += name;
	error += "' already defined.";
	error += "\n\t";
	error += olddef->GetFile()->GetAbsoluteFileName() + '(' + olddef->GetLine() + ") : ";
	error += "see original definition";

	Add(newdef->GetFile(), newdef->GetLine(), error);
}

// "Member <string message> cannot be same name as enclosing category."
void opError::MemberNameSameAsCategoryNameError(const opString& name, opNode* category, opNode* def)
{
	opString error = "\tMember '";

	error += name;
	error += "' cannot be same name as enclosing category.";
	error += "\n\t";
	error += category->GetFile()->GetInputName() + '(' + category->GetLine() + ") : ";
	error += "see category definition";

	Add(def->GetFile(), def->GetLine(), error);
}

// "Expected <string message> got <Value> in <Context>"
void opError::ExpectError(const opString& expected, opNode* got)
{
	opString error = "\tGot          '";

	error += NodeString(got);
	error += "'\n";
	error += "\tIn context   '";
	error += ContextString();
	error += "'\n";
	error += "\tExpected     '";
	error += expected;
	error += "'";

	Add(got->GetFile(), got->GetLine(), error);
	opException::ThrowException();
}

// "Expected <Grammar> got <Value> in <Context>"
void opError::ExpectError(Token expected, opNode* got)
{
	vector<Token> v;

	v.push_back(expected);

	ExpectError(v, got);
}

// "Expected <Multiple> got '<Value>' in context <Context>"
void opError::ExpectError(const vector<Token>& expected, opNode* got)
{
	int      size   = (int) expected.size();
	opString error  = "";
	opString spacer = "\t          or '";

	assert(size > 0);

	error += "\tGot          '";
	error += NodeString(got);
	error += "'\n";

	error += "\tIn context   '";
	error += ContextString();
	error += "'\n";

	error += "\tExpected     '";
	error += ParseErrors[expected[0]];
	error += "'";

	if (size > 1)
		error += "\n";

	for (int i = 1; i < size; i++)
	{
		error += spacer;
		error += ParseErrors[expected[i]];
		error += "'";

		if (i < size - 1)
			error += "\n";
	}

	Add(got->GetFile(), got->GetLine(), error);
	opException::ThrowException();
}

// Expected <type(s)>, before <got> in context <context>, instead got <token>.
void opError::ReverseExpectError(const vector<Token>& expected, opNode* got, opNode* before)
{
	int      size   = (int) expected.size();
	opString spacer = "\t          or '"; 
	opString error;
	
	assert(size > 0);

	error += "\tGot          '";

	if (got)
	{
		error += NodeString(got);
		error += "'\n";
	}
	else
		error += "nothing'\n";

	if (before)
	{
		error += "\tBefore       '";
		error += NodeString(before);
		error += "'\n";
	}

	error += "\tIn context   '";
	error += ContextString();
	error += "'\n";

	error += "\tExpected     '";
	error += ParseErrors[expected[0]];
	error += "'";
	
	if (size > 1)
		error += "\n";

	for (int i = 1; i < size; i++)
	{
		error += spacer;
		error += ParseErrors[expected[i]];
		error += "'";

		if (i < size - 1)
			error += "\n";
	}

	int       line;
	FileNode* file;

	if (got)
	{
		line = got->GetLine();
		file = got->GetFile();
	}
	else
	{
		line = before->GetLine();
		file = before->GetFile();
	}

	Add(file, line, error);
	opException::ThrowException();
}

void opError::MissingError(const vector<Token>& expected, opNode* where)
{
	int      size  = (int) expected.size();
	opString error;
	opString spacer;

	assert(size > 0);

	error  += "\tMissing     '";
	spacer += "\t         or '";
	error  += ParseErrors[expected[0]];
	error  += "'\n";

	for (int i = 1; i < size; i++)
	{
		error += spacer;
		error += ParseErrors[expected[i]];
		error += "'\n";
	}

	error += "\tIn context   '";
	error += ContextString();
	error += "'";

	Add(where->GetFile(), where->GetLine(), error);
	opException::ThrowException();
}

// "<Value> not allowed in <Context>, Removal may solve"
void opError::CheckNoneError(opNode* unexpected)
{
	opString error = "\t'";
	
	error += NodeString(unexpected) + "' not allowed in context '"
		     + ContextString() + "', remove?";

	Add(unexpected->GetFile(), unexpected->GetLine(), error);
	opException::ThrowException();
}

// "<Value> not allowed in <Context>"
void opError::DisallowError(opNode* disallowed)
{
	opString error = "\t'";

	error += NodeString(disallowed) + "' not allowed in context '"
		     + ContextString() + "'";

	Add(disallowed->GetFile(), disallowed->GetLine(), error);
	opException::ThrowException();
}

// Reached premature end of <location>, Expected <list of grammars> in context <Context>
void opError::PrematureError(Token expected, opNode* beginning, bool bEOF)
{
	vector<Token> v;

	v.push_back(expected);

	PrematureError(v, beginning, bEOF);
}

// Reached premature end of <location>, Expected <list of grammars> in context <Context>
void opError::PrematureError(const vector<Token>& expected, opNode* beginning, bool bEOF)
{
	int      size   = (int) expected.size();
	opString error  = "\tReached      'premature end of ";
	opString spacer = "\t          or '";
	
	assert(size > 0);
	
	if (bEOF)
		error += "file'";
	else
		error += "block'";
	
	error += "\n";

	error += "\tIn context   '";
	error += ContextString();
	error += "'\n";

	error  += "\tExpected     '";
	error += ParseErrors[expected[0]];
	error += "'";

	if (size > 1)
		error += "\n";

	for (int i = 1; i < size; i++)
	{
		error += spacer;
		error += ParseErrors[expected[i]];
		error += "'";

		if (i < size - 1)
			error += "\n";
	}	

	Add(beginning->GetFile(), beginning->GetLine(), error);
	opException::ThrowException();
}

// Reached premature end of <location>.
void opError::PrematureError(opNode* beginning, bool bEOF)
{
	opString error = "\tReached premature end of ";

	if (bEOF)
		error += "file";
	else
		error += "block";

	error += ".";

	Add(beginning->GetFile(), beginning->GetLine(), error);
	opException::ThrowException();
}

// Mutual exclusion error.
void opError::MutualExclusionError(opNode* node1, opNode* node2)
{
	opString error = "\tConstructs '";

	node1->PrintString(error);

	error += "' and '";

	node2->PrintString(error);

	error += "' cannot be used together!";

	Add(node1->GetFile(), node1->GetLine(), error);
	opException::ThrowException();
}

///==========================================
/// Dialect Errors
///==========================================

// builds a list of data modifiers for error output
void opError::GetDataModifierList(const opString& spacer, opString& modifierlist, opNode* innernode)
{
	OPObjectNode*    object   = innernode->FindParent<OPObjectNode>();
	DialectCategory* category = object->GetCategorySettings();

	opArray<opString> modifiers;
	category->GetDataModifiers(modifiers);

	modifiers.Sort();

	int num = modifiers.Size();

	for (int i = 0; i < num; i++)
	{
		modifierlist += modifiers[i];

		if( i + 1 < num )
		{
			modifierlist += ",\n";
			modifierlist += spacer;
		}
	}
}

// builds a list of function modifiers for error output
void opError::GetFunctionModifierList(const opString& spacer, opString& modifierlist, opNode* innernode)
{
	OPObjectNode* object = innernode->FindParent<OPObjectNode>();
	DialectCategory* category = object->GetCategorySettings();

	opArray<opString> modifiers;
	category->GetFunctionModifiers(modifiers);

	modifiers.Sort();

	int num = modifiers.Size();

	for (int i = 0; i < num; i++)
	{
		modifierlist += modifiers[i];

		if( i + 1 < num )
		{
			modifierlist += ",\n";
			modifierlist += spacer;
		}
	}
}

// builds a list of values for error output
void opError::GetList(const opString& spacer, opString& output, opSet<opString>& list)
{
	opSet<opString>::iterator it = list.begin();
	opSet<opString>::iterator end = list.end();
	
	while(it != end)
	{
		output += *it;
		
		++it;	
		
		if( it != end )
		{
			output += ",\n";
			output += spacer;
		}
	}
}

// Invalid modifier <Modifier> used, available modifiers: <Valid Modifier List>
void opError::InvalidDataModifierError(opNode* badmodifier)
{
	opString value;
	opString error = "\tInvalid modifier '";
	opString spacer;

	badmodifier->PrintString(value);
	
	error  += value + "' used.\n";
	error  += "\tAvailable modifiers: "; 
	spacer  = "\t                     ";
	
	//get and print valid modifier list
	GetDataModifierList(spacer, error, badmodifier);
	
	Add(badmodifier->GetFile(),badmodifier->GetLine(),error);
	opException::ThrowException();
}

void opError::InvalidFunctionModifierError(opNode* badmodifier)
{
	opString value;
	opString error = "\tInvalid modifier '";
	opString spacer;

	badmodifier->PrintString(value);
	
	error  += value + "' used.\n";
	error  += "\tAvailable modifiers: "; 
	spacer  = "\t                     "; 

	//get and print valid modifier list
	GetFunctionModifierList(spacer, error, badmodifier);

	Add(badmodifier->GetFile(),badmodifier->GetLine(),error);
	opException::ThrowException();
}

void opError::ExpandError(opNode* node, const opString& signature, const opSymbolTracker& tracker)
{
	//TODO: improve this error, it should print out similar signatures!
	//      "... did you mean ...?"
	MessageError(node,opString("opmacro ") + signature + " not found, check name and arguments.");		
}

// Invalid <modifier name> value <value>: failed regex "regex"
// Invalid <modifier name> value <value>: <custom error message>
// TODO: need to store more information in the tracker to do this.
void opError::InvalidValueError(ValuedModifierNode* modifier, ModifierNodeBase* definition)
{
	opString error = "\tInvalid ";
	opString value;

	modifier->GetArguments()->PrintString(value);
	
	error += modifier->GetModifier()->GetValue() + "() value '" + value + "' : ";
	
	//1. if we have a custom error message, use it
	//2. if not use the regex message

	TerminalNode* customerror = definition->ErrorString();
	if(!customerror)
	{
		error += "failed pattern ";
		error += definition->GetArguments()->GetCondition()->GetValue();
	}
	else
	{
		error += "failed rule ";
		error += customerror->GetValue();
	}

	Add(modifier->GetFile(),modifier->GetLine(),error);
	opException::ThrowException();
}

void opError::NoteRedeclaredError(DialectNote* oldnote, NoteNode* newnote)
{
	//syntax: note redeclared
	//		  needs to print the line number for...
	//		  1. the error location
	//		  2. the original location
	//		  these require the nodes to be accessible
	
	NoteNode* notenode = oldnote->GetNoteNode();
	
	opString error;
	error += "\tRedefined note '";
	newnote->GetName()->PrintString(error);
	error += "'";
	
	if(notenode)
	{
		error += "\n\t";
		error += notenode->GetFile()->GetAbsoluteFileName() + '(' + notenode->GetLine() + ") : ";
		error += "see original definition";
	}
	
	Add(newnote->GetFile(),newnote->GetLine(),error);
	opException::ThrowException();
}

void opError::MessageError(opNode* node, const opString& message, bool bThrowException)
{
	opString error = "\t";

	error += message;

	Add(node->GetFile(), node->GetLine(), error);

	if (bThrowException)
		opException::ThrowException();
}

void opError::MapRedeclaredError(DialectMap* oldmap, CategoryMapNode* newmap)
{
	//syntax: map redeclared
	//		  needs to print the line number for...
	//		  1. the error location
	//		  2. the original location
	//		  these require the nodes to be accessible

	CategoryMapNode* mapnode = oldmap->GetMapNode();

	opString error;
	error += "\tRedefined map '";
	mapnode->GetName()->PrintString(error);
	error += "'";

	if(mapnode)
	{
		error += "\n\t";
		error += mapnode->GetFile()->GetAbsoluteFileName() + '(' + mapnode->GetLine() + ") : ";
		error += "see original definition";
	}

	Add(newmap->GetFile(),newmap->GetLine(),error);
	opException::ThrowException();
	
}

void opError::NameCollisionError(const opString& name, opNode* oldnode, opNode* newnode)
{
	//syntax: originally declared as a... redeclared as a...
	//		  needs to print the line number for...
	//		  1. the error location
	//		  2. the original location
	//		  these require the nodes to be accessible


	opString error = "\t'" + name + "' redefined as different type!";

	error += "\n\t";
	error += oldnode->GetFile()->GetAbsoluteFileName() + '(' + oldnode->GetLine() + ") : ";
	error += "see original definition";

	Add(newnode->GetFile(),newnode->GetLine(),error);
	opException::ThrowException();
}

void opError::DisallowError(DisallowNode* disallow, opNode* location)
{
	//syntax: failed rule 'Name' in context 'blah' : message

	opString error = "\tFailed rule '";

	error += disallow->GetName()->GetValue();
	error += "'";

	TerminalNode* customerror = disallow->ErrorString();

	if (customerror)
	{
		error += " : ";
		error += customerror->GetValue();
	}
	
	error += "\n";
	error += "\tIn context  '";
	error += ContextString();
	error += "'";

	Add(location->GetFile(),location->GetLine(),error);
	opException::ThrowException();
}

// does not throw an exception
void opError::OperandError(opNode* node, DialectExpressionMap* map, const opString& operandname)
{
	opString spacer;
	opString error  = "\tInvalid modifier '";
	
	error += operandname + "' used in criteria expression.\n";
	error += "\tAvailable modifiers: "; 
	spacer = "\t                     ";
	
	//now grab all the modifiers from the map
	opArray<opString> modifiers;
	map->GetModifiers(modifiers);
	
	opSet<opString> modifierset;
	modifierset.Insert(modifiers.begin(),modifiers.end());
	
	//get and print valid modifier list
	GetList(spacer, error, modifierset);
	
	Add(node->GetFile(),node->GetLine(),error);
}

// invalid extendpoint error
void opError::InvalidExtensionError(opNode* node, const opString& name, const opString& type)
{
	opString error = "\tNo extension is registered for the " + type + " '" + name + "'.\n";
	opString spacer;

	typedef opMap<opString, ExtensionNode*> extensiontable;

	const extensiontable&          extensions = DialectTracker::GetExtensions();
	extensiontable::const_iterator i          = extensions.begin();
	extensiontable::const_iterator end        = extensions.end();
	opSet<opString>                extensionlist;

	while (i != end)
	{
		ExtensionNode* node = i->second;

		extensionlist.Insert( node->GetName()->GetValue() );

		++i;
	}

	error  += "\tAvailable extentions: ";
	spacer += "\t                      ";

	GetList(spacer, error, extensionlist);

	Add(node->GetFile(),node->GetLine(),error);
	opException::ThrowException();
}

// note definition error
void opError::NoteDefinitionError(NoteDefinitionNode* oldnode, NoteDefinitionNode* newnode)
{
	//syntax: note redefined
	//		  needs to print the line number for...
	//		  1. the error location
	//		  2. the original location
	//		  these require the nodes to be accessible

	opString error;
	error += "\tRedefined note '";
	oldnode->GetPath()->PrintString(error);
	error += "' without specifying 'override' modifier!";

	error += "\n\t";
	error += oldnode->GetFile()->GetAbsoluteFileName() + '(' + oldnode->GetLine() + ") : ";
	error += "see original definition";

	Add(newnode->GetFile(),newnode->GetLine(),error);
	opException::ThrowException();
}

// alt prefix redefinition error
void opError::AltPrefixRedefinitionError(const opString& type, opNode* oldprefix, opNode* newprefix)
{
	opString error = "\tOnly one alternate prefix is allowed per ";
	
	error += type;
	error += "!\n\t";

	error += oldprefix->GetFile()->GetAbsoluteFileName() + '(' + oldprefix->GetLine() + ") : ";
	error += "see original definition";

	Add(newprefix->GetFile(), newprefix->GetLine(), error);
	opException::ThrowException();
}