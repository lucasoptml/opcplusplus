///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Mappings.h
/// Date: 03/23/2007
///
/// Description:
///
/// Header for token mappings (tokens -> grammars, keywords, etc.)
///****************************************************************

//mapping macros
#define TOKENMAPPED(tokenid) template<> struct IsMapped<tokenid> { enum { value = true}; };

#define TOKENSTRING(tokenid,tokenstring) template<> const char* TokenString<tokenid>() { return tokenstring; }

#define TOKENNAME(tokenid) template<> struct IsName<tokenid> { enum { value = true}; };

#define TOKENGRAMMAR(tokenid,nodetype)									\
template<> struct IsGrammar<tokenid> { enum { value = true}; };			\
template<> struct NodeTypeIsGrammar<nodetype> { enum { value = true}; };

#define TOKENNODETYPE(tokenid,nodetype) template<> struct TokenNodeType<tokenid> { typedef nodetype type; };
										
#define TOKENNODEID(tokenid,nodetype) template<> struct NodeTypeToken<nodetype> { static const Token id = tokenid; };

#define TOKENIDSTRING(tokenid,tokenidstring) template<> const char* TokenIdString<tokenid>() { return tokenidstring; }

#define TOKENKEYWORD(tokenid)						\
template<> struct IsKeyword<tokenid> { enum { value = true }; }; \
template<> void RegisterMapping<tokenid>()			\
{													\
	RegisterKeyword<tokenid>();						\
}

#define TOKENOPERATOR(tokenid)							\
template<> struct IsOperator<tokenid> { enum { value = true}; };	\
template<> void RegisterMapping<tokenid>()				\
{														\
	RegisterOperator<tokenid>();						\
}

#define DIALECTMODIFIERSTRING(tokenid, modifierstring)	\
	template<> const char* DialectModifierString<tokenid>() { return modifierstring; } \
	template<> struct IsDialectModifier<tokenid> { enum { value = true }; };

//Add a name that is not a keyword (has no real associated string mapping)
#define Add_Name(tokenid)				\
namespace tokenmapping					\
{										\
	TOKENMAPPED(tokenid);				\
	TOKENIDSTRING(tokenid,#tokenid);	\
	TOKENSTRING(tokenid,#tokenid);		\
	TOKENNAME(tokenid);					\
	TOKENNODETYPE(tokenid,TerminalNode);\
}

//Add a keyword, which has a mapping to a string value
#define Add_Keyword(tokenid,tokenname)	\
namespace tokenmapping					\
{										\
	TOKENMAPPED(tokenid);				\
	TOKENIDSTRING(tokenid,#tokenid);	\
	TOKENSTRING(tokenid,tokenname);		\
	TOKENKEYWORD(tokenid);				\
	TOKENNODETYPE(tokenid,TerminalNode);\
}

//Add an operator, which as a mapping to an operator string
#define Add_Operator(tokenid,tokenname)	\
namespace tokenmapping					\
{										\
	TOKENMAPPED(tokenid);				\
	TOKENIDSTRING(tokenid,#tokenid);	\
	TOKENSTRING(tokenid,tokenname);		\
	TOKENOPERATOR(tokenid);				\
	TOKENNODETYPE(tokenid,TerminalNode);\
}

//Add a grammar, which has a mapping to a nodetype
#define Add_Grammar(tokenid,nodetype)	\
namespace tokenmapping					\
{										\
	TOKENMAPPED(tokenid);				\
	TOKENIDSTRING(tokenid,#tokenid);	\
	TOKENSTRING(tokenid,#nodetype);		\
	TOKENGRAMMAR(tokenid,nodetype);		\
	TOKENNODETYPE(tokenid,nodetype);	\
	TOKENNODEID(tokenid,nodetype);		\
}
//#nodetype
//Add a grammar, which has a mapping to a nodetype
#define Add_Dialect_Modifier_Grammar(tokenid,nodetype,modifierstring)	\
namespace tokenmapping													\
{																		\
	TOKENMAPPED(tokenid);												\
	TOKENIDSTRING(tokenid,#tokenid);									\
	TOKENSTRING(tokenid,#nodetype);										\
	TOKENGRAMMAR(tokenid,nodetype);										\
	TOKENNODETYPE(tokenid,nodetype);									\
	TOKENNODEID(tokenid,nodetype);										\
	DIALECTMODIFIERSTRING(tokenid, modifierstring)						\
}

///==========================================
/// Non-Keyword Tokens
///==========================================

Add_Name(T_EOF)
Add_Name(T_WHITESPACE)
Add_Name(T_ID)
Add_Name(T_CCOMMENT)
Add_Name(T_COMMENT)
Add_Name(T_ANYCHAR)
Add_Name(T_NEWLINE)
Add_Name(T_CONTINUELINE)
Add_Name(T_HEXADECIMAL)
Add_Name(T_NUMBER)
Add_Name(T_DECIMAL)
Add_Name(T_EXPONENTIAL)
Add_Name(T_STRING)
Add_Name(T_CHAR)
Add_Name(T_WIDESTRING)
Add_Name(T_BASIC_TYPE)
Add_Name(T_MODIFIER)

///==========================================
/// Special Keywords
///==========================================

Add_Keyword(T_SPECIAL_LEFT_BRACE,"+{}");
Add_Keyword(T_SPECIAL_RIGHT_BRACE,"-{}");
Add_Keyword(T_SPECIAL_LEFT_PAREN,"+()");
Add_Keyword(T_SPECIAL_RIGHT_PAREN,"-()");
Add_Keyword(T_SPECIAL_LEFT_BRACKET,"+[]");
Add_Keyword(T_SPECIAL_RIGHT_BRACKET,"-[]");

///==========================================
/// C++ Keywords
///==========================================

Add_Keyword(T_AUTO, "auto")
Add_Keyword(T_CONST, "const")
Add_Keyword(T_VOID, "void")
Add_Keyword(T_STRUCT, "struct")
Add_Keyword(T_UNSIGNED, "unsigned")
Add_Keyword(T_BREAK, "break")
Add_Keyword(T_CONTINUE, "continue")
Add_Keyword(T_ELSE, "else")
Add_Keyword(T_FOR, "for")
Add_Keyword(T_SIGNED, "signed")
Add_Keyword(T_SWITCH, "switch")
Add_Keyword(T_CASE, "case")
Add_Keyword(T_DEFAULT, "default")
Add_Keyword(T_ENUM, "enum")
Add_Keyword(T_GOTO, "goto")
Add_Keyword(T_REGISTER, "register")
Add_Keyword(T_SIZEOF, "sizeof")
Add_Keyword(T_TYPEDEF, "typedef")
Add_Keyword(T_VOLATILE, "volatile")
Add_Keyword(T_DO, "do")
Add_Keyword(T_EXTERN, "extern")
Add_Keyword(T_IF, "if")
Add_Keyword(T_RETURN, "return")
Add_Keyword(T_STATIC, "static")
Add_Keyword(T_UNION, "union")
Add_Keyword(T_WHILE, "while")
Add_Keyword(T_ASM, "asm")
Add_Keyword(T_DYNAMIC_CAST, "dynamic_cast")
Add_Keyword(T_REINTERPRET_CAST, "reinterpret_cast")
Add_Keyword(T_TRY, "try")
Add_Keyword(T_EXPLICIT, "explicit")
Add_Keyword(T_NEW, "new")
Add_Keyword(T_STATIC_CAST, "static_cast")
Add_Keyword(T_TYPEID, "typeid")
Add_Keyword(T_CATCH, "catch")
Add_Keyword(T_FALSE, "false")
Add_Keyword(T_OPERATOR, "operator")
Add_Keyword(T_TEMPLATE, "template")
Add_Keyword(T_TYPENAME, "typename")
Add_Keyword(T_CLASS, "class")
Add_Keyword(T_FRIEND, "friend")
Add_Keyword(T_PRIVATE, "private")
Add_Keyword(T_THIS, "this")
Add_Keyword(T_USING, "using")
Add_Keyword(T_CONST_CAST, "const_cast")
Add_Keyword(T_INLINE, "inline")
Add_Keyword(T_PUBLIC, "public")
Add_Keyword(T_THROW, "throw")
Add_Keyword(T_VIRTUAL, "virtual")
Add_Keyword(T_DELETE, "delete")
Add_Keyword(T_MUTABLE, "mutable")
Add_Keyword(T_PROTECTED, "protected")
Add_Keyword(T_TRUE, "true")
Add_Keyword(T_E_BITAND, "bitand")
Add_Keyword(T_E_COMPL, "compl")
Add_Keyword(T_E_NOT_EQ, "not_eq")
Add_Keyword(T_E_OR_EQ, "or_eq")
Add_Keyword(T_E_XOR_EQ, "xor_eq")
Add_Keyword(T_E_AND_EQ, "and_eq")
Add_Keyword(T_E_BITOR, "bitor")
Add_Keyword(T_E_XOR, "xor")
Add_Keyword(T_POUND_DEFINE,"#define")
Add_Keyword(T_POUND_ELIF,"#elif")
Add_Keyword(T_POUND_ELSE,"#else")
Add_Keyword(T_POUND_ENDIF,"#endif")
Add_Keyword(T_POUND_ERROR,"#error")
Add_Keyword(T_POUND_IF,"#if")
Add_Keyword(T_POUND_IFDEF,"#ifdef")
Add_Keyword(T_POUND_IFNDEF,"#ifndef")
Add_Keyword(T_POUND_IMPORT,"#import")
Add_Keyword(T_POUND_INCLUDE,"#include")
Add_Keyword(T_POUND_LINE,"#line")
Add_Keyword(T_POUND_PRAGMA,"#pragma")
Add_Keyword(T_POUND_UNDEF,"#undef")
Add_Keyword(T_POUND_USING,"#using")
Add_Keyword(T_POUND_WARNING,"#warning")

///==========================================
/// Operators
///==========================================

Add_Operator(T_PLUS, "+")
Add_Operator(T_MINUS, "-")
Add_Operator(T_STAR, "*")
Add_Operator(T_DIVIDE, "/")
Add_Operator(T_MODULUS, "%")
Add_Operator(T_ASSIGN, "=")
Add_Operator(T_PLUS_EQUALS, "+=")
Add_Operator(T_MINUS_EQUALS, "-=")
Add_Operator(T_STAR_EQUALS, "*=")
Add_Operator(T_DIVIDE_EQUALS, "/=")
Add_Operator(T_BITWISE_AND_EQUALS, "&=")
Add_Operator(T_BITWISE_EXCLUSIVE_OR_EQUALS, "^=")
Add_Operator(T_BITWISE_INCLUSIVE_OR_EQUALS, "|=")
Add_Operator(T_LEFT_SHIFT_EQUALS, "<<=")
Add_Operator(T_RIGHT_SHIFT_EQUALS, ">>=")
Add_Operator(T_MODULUS_EQUALS, "%=")
Add_Operator(T_PLUS_PLUS, "++")
Add_Operator(T_MINUS_MINUS, "--")
Add_Operator(T_AND, "&&")
Add_Operator(T_OR, "||")
Add_Operator(T_BITWISE_EXCLUSIVE_OR, "^")
Add_Operator(T_BITWISE_INCLUSIVE_OR, "|")
Add_Operator(T_LESS_THAN, "<")
Add_Operator(T_GREATER_THAN, ">")
Add_Operator(T_LESS_THAN_OR_EQUAL_TO, "<=")
Add_Operator(T_GREATER_THAN_OR_EQUAL_TO, ">=")
Add_Operator(T_EQUALS, "==")
Add_Operator(T_NOT_EQUALS, "!=")
Add_Operator(T_NEGATION, "!")
Add_Operator(T_LEFT_SHIFT, "<<")
Add_Operator(T_RIGHT_SHIFT, ">>")
Add_Operator(T_LEFT_PAREN, "(")
Add_Operator(T_RIGHT_PAREN, ")")
Add_Operator(T_LEFT_BRACKET, "[")
Add_Operator(T_RIGHT_BRACKET, "]")
Add_Operator(T_LEFT_BRACE, "{")
Add_Operator(T_RIGHT_BRACE, "}")
Add_Operator(T_AMPERSAND, "&")
Add_Operator(T_DOT, ".")
Add_Operator(T_ARROW, "->")
Add_Operator(T_DOT_STAR, ".*")
Add_Operator(T_ARROW_STAR, "->*")
Add_Operator(T_COMMA, ",")
Add_Operator(T_QUESTION_MARK, "?")
Add_Operator(T_COLON, ":")
Add_Operator(T_SEMICOLON, ";")
Add_Operator(T_SCOPE_RESOLUTION, "::")
Add_Operator(T_BITWISE_COMPLEMENT, "~")
Add_Operator(T_POUND,"#")
Add_Operator(T_BACKSLASH, "\\")
Add_Operator(T_CONCATENATION,"##")
Add_Operator(T_ACCENT,"`");
Add_Operator(T_DOUBLE_ACCENT,"``");

///==========================================
/// opCPP Keywords
///==========================================

Add_Name(T_OPOBJECT)
Add_Name(T_OPENUM)
Add_Keyword(T_STATE, "state")
Add_Keyword(T_NAMESPACE, "namespace")
Add_Keyword(T_OPDEFINE, "opdefine")
Add_Keyword(T_OPMACRO, "opmacro")
Add_Keyword(T_EXPAND, "expand")
Add_Keyword(T_OPINCLUDE, "opinclude")
Add_Keyword(T_UNINLINE, "uninline")
Add_Keyword(T_SPACER, "@")

///==========================================
/// opCPP Dialect Keywords
///==========================================

Add_Keyword(T_CATEGORY, "category")
Add_Keyword(T_DATAMAP, "datamap")
Add_Keyword(T_FUNCTIONMAP, "functionmap")
Add_Keyword(T_FUNCTIONMODIFIER, "functionmodifier")
Add_Keyword(T_DATAMODIFIER, "datamodifier")
Add_Keyword(T_LOCATION, "location")
Add_Keyword(T_NOTE, "note")
Add_Keyword(T_ENUMERATION, "enumeration")
Add_Keyword(T_ENUMMAP, "enummap")
Add_Keyword(T_IS, "is")
Add_Keyword(T_E_AND, "and")
Add_Keyword(T_E_NOT, "not")
Add_Keyword(T_E_OR, "or")
Add_Keyword(T_CPLUSPLUS, "c++")
Add_Keyword(T_DISALLOW, "disallow")
Add_Keyword(T_CODE, "code")
Add_Keyword(T_EXTENSION, "extension")
Add_Keyword(T_EXTEND_POINT, "extendpoint")
Add_Keyword(T_EXTENSION_POINT, "extensionpoint")
Add_Keyword(T_FILE_DECLARATION, "file")

///==========================================
/// opCPP Dialect Grammars
///==========================================

Add_Grammar(G_DIALECTFILE,DialectFileNode)
Add_Grammar(G_CATEGORY,CategoryNode)
Add_Grammar(G_CATEGORY_BODY,CategoryBodyNode)
Add_Grammar(G_DATAMODIFIER,DataModifierNode)
Add_Grammar(G_MODIFIER_ARGUMENT,ModifierArgumentNode)
Add_Grammar(G_FUNCTIONMODIFIER,FunctionModifierNode)
Add_Grammar(G_CATEGORY_DATAMAP,CategoryDatamapNode)
Add_Grammar(G_CATEGORY_FUNCTIONMAP,CategoryFunctionmapNode)
Add_Grammar(G_CATEGORY_MAP_BODY,CategoryMapBodyNode)
Add_Grammar(G_IS,IsNode)
Add_Grammar(G_IS_BODY, IsBodyNode)
Add_Grammar(G_CRITERIA_ARGUMENT, CriteriaArgumentNode)
Add_Grammar(G_CRITERIA_VALUE_MODIFIER, CriteriaValueModifierNode)
Add_Grammar(G_CRITERIA_GROUP, CriteriaGroupNode)
Add_Grammar(G_CATEGORY_LOCATION,CategoryLocationNode)
Add_Grammar(G_CATEGORY_LOCATION_BODY,CategoryLocationBodyNode)
Add_Grammar(G_NOTE, NoteNode)
Add_Grammar(G_NOTE_BODY, NoteBodyNode)
Add_Grammar(G_NOTE_DEFINITION, NoteDefinitionNode)
Add_Grammar(G_NOTE_DEFINITION_ARGUMENT_LIST, NoteArgumentListNode)
Add_Grammar(G_NOTE_DEFINITION_ARGUMENT, NoteArgumentNode)
Add_Grammar(G_ENUMERATION, EnumerationNode)
Add_Grammar(G_ENUMERATION_BODY, EnumerationBodyNode)
Add_Grammar(G_ENUMERATION_LOCATION, EnumerationLocationNode)
Add_Grammar(G_ENUMERATION_LOCATION_BODY, EnumerationLocationBodyNode)
Add_Grammar(G_ENUMERATION_MAP, EnumerationMapNode)
Add_Grammar(G_DIALECT_MODIFIERS, DialectModifiersNode)
Add_Grammar(G_DISALLOW, DisallowNode)
Add_Grammar(G_DISALLOW_BODY, DisallowBodyNode)
Add_Grammar(G_CODE, CodeNode)
Add_Grammar(G_CODE_BODY, CodeBodyNode)
Add_Grammar(G_EXTENSION, ExtensionNode)
Add_Grammar(G_EXTEND_POINT, ExtendPointNode)
Add_Grammar(G_EXTENSION_POINT, ExtensionPointNode)
Add_Grammar(G_FILE_DECLARATION, FileDeclarationNode)
Add_Grammar(G_FILE_DECLARATION_BODY, FileDeclarationBodyNode)
Add_Grammar(G_FILE_DECLARATION_LOCATION, FileDeclarationLocationNode)
Add_Grammar(G_DIALECT_NAMESPACE, DialectNamespaceNode)

// dialect modifiers
Add_Dialect_Modifier_Grammar(G_ERROR, ErrorNode, "error")
Add_Dialect_Modifier_Grammar(G_DESCRIPTION, DescriptionNode, "description")
Add_Dialect_Modifier_Grammar(G_HIDDEN, HiddenNode, "hidden")
Add_Dialect_Modifier_Grammar(G_BEFORE, BeforeNode, "before")
Add_Dialect_Modifier_Grammar(G_AFTER, AfterNode, "after")
Add_Dialect_Modifier_Grammar(G_VERBATIM, VerbatimNode, "verbatim")
Add_Dialect_Modifier_Grammar(G_CLASS_PREFIX, ClassPrefixNode, "classprefix")
Add_Dialect_Modifier_Grammar(G_STRUCT_PREFIX, StructPrefixNode, "structprefix")
Add_Dialect_Modifier_Grammar(G_ENUM_PREFIX, EnumPrefixNode, "enumprefix")
Add_Dialect_Modifier_Grammar(G_OVERRIDE, OverrideNode, "override")

///==========================================
/// opCPP Dialect Statement Grammars
///==========================================

Add_Grammar(G_DATAMODIFIER_STATEMENT, DataModifierStatementNode)
Add_Grammar(G_FUNCTIONMODIFIER_STATEMENT, FunctionModifierStatementNode)
Add_Grammar(G_CATEGORY_LOCATION_STATEMENT, CategoryLocationStatementNode)
Add_Grammar(G_NOTE_STATEMENT, NoteStatementNode)
Add_Grammar(G_CATEGORY_DATAMAP_STATEMENT, CategoryDatamapStatementNode)
Add_Grammar(G_CATEGORY_FUNCTIONMAP_STATEMENT, CategoryFunctionmapStatementNode)
Add_Grammar(G_IS_STATEMENT, IsStatementNode)
Add_Grammar(G_DISALLOW_STATEMENT, DisallowStatementNode)
Add_Grammar(G_ENUMERATIONMAP_STATEMENT, EnumerationMapStatementNode)
Add_Grammar(G_ENUMERATION_LOCATION_STATEMENT, EnumerationLocationStatementNode)
Add_Grammar(G_FILE_DECLARATION_LOCATION_STATEMENT, FileDeclarationLocationStatementNode)
Add_Grammar(G_DIALECT_OPINCLUDE_STATEMENT, DialectOPIncludeStatementNode)
Add_Grammar(G_CATEGORY_STATEMENT, CategoryStatementNode)
Add_Grammar(G_ENUMERATION_STATEMENT, EnumerationStatementNode)
Add_Grammar(G_DIALECT_OPDEFINE_STATEMENT, DialectOPDefineStatementNode)
Add_Grammar(G_NOTE_DEFINITION_STATEMENT, NoteDefinitionStatementNode)
Add_Grammar(G_FILE_DECLARATION_STATEMENT, FileDeclarationStatementNode)
Add_Grammar(G_CODE_STATEMENT, CodeStatementNode)
Add_Grammar(G_DIALECT_NAMESPACE_STATEMENT, DialectNamespaceStatementNode)
Add_Grammar(G_EXTENSION_STATEMENT, ExtensionStatementNode)
Add_Grammar(G_EXTEND_POINT_STATEMENT, ExtendPointStatementNode)
Add_Grammar(G_DIALECT_OPMACRO_STATEMENT, DialectOPMacroStatementNode)

///==========================================
/// Grammars
///==========================================

Add_Grammar(G_CPPFILE, CppFileNode)
Add_Grammar(G_OPFILE, OPFileNode)
Add_Grammar(G_FUNDAMENTAL_TYPE, FundamentalTypeNode)
Add_Grammar(G_NAMESPACE, NamespaceNode)
Add_Grammar(G_NAMESPACEBLOCK, NamespaceBlockNode)
Add_Grammar(G_NAMESPACEDECL, NamespaceDeclNode)
Add_Grammar(G_USING_NAMESPACE_KEYWORD, UsingNamespaceKeywordNode)
Add_Grammar(G_USING, UsingNode)
Add_Grammar(G_NAMESPACE_ALIAS, NamespaceAliasNode)
Add_Grammar(G_BRACE_BLOCK, BraceBlockNode)
Add_Grammar(G_PAREN_BLOCK, ParenBlockNode)
Add_Grammar(G_BRACKET_BLOCK, BracketBlockNode)
Add_Grammar(G_ANGLED_BLOCK, AngledBlockNode)
Add_Grammar(G_POINTER, PointerNode)
Add_Grammar(G_POINTER_MEMBER, PointerMemberNode)
Add_Grammar(G_FUNCTION, FunctionNode)
Add_Grammar(G_FUNCTION_DEFINITION, FunctionDefinitionNode)
Add_Grammar(G_FUNCTION_PROTOTYPE, FunctionPrototypeNode)
Add_Grammar(G_FUNCTION_ARGUMENTLIST, FunctionArgumentListNode)
Add_Grammar(G_FUNCTION_ARGUMENT, FunctionArgumentNode)
Add_Grammar(G_FUNCTION_ARGUMENTDEFINITION, FunctionArgumentDefinitionNode)
Add_Grammar(G_FUNCTION_ARGUMENTDEFAULT, FunctionArgumentDefaultNode)
Add_Grammar(G_CONSTRUCTOR, ConstructorNode)
Add_Grammar(G_CONSTRUCTOR_DEFINITION, ConstructorDefinitionNode)
Add_Grammar(G_CONSTRUCTOR_PROTOTYPE, ConstructorPrototypeNode)
Add_Grammar(G_CONSTRUCTOR_INITIALIZER_LIST, ConstructorInitializerListNode)
Add_Grammar(G_CONSTRUCTOR_INITIALIZER_ARGUMENT, ConstructorInitializerArgumentNode)
Add_Grammar(G_CONSTRUCTOR_INITIALIZER, ConstructorInitializerNode)
Add_Grammar(G_DESTRUCTOR, DestructorNode)
Add_Grammar(G_DESTRUCTOR_PROTOTYPE, DestructorPrototypeNode)
Add_Grammar(G_DESTRUCTOR_DEFINITION, DestructorDefinitionNode)
Add_Grammar(G_OPENUM, OPEnumNode)
Add_Grammar(G_OPENUM_BODY, OPEnumBodyNode)
Add_Grammar(G_OPENUM_ELEMENT, OPEnumElementNode)
Add_Grammar(G_OPENUM_INITIALIZATION, OPEnumInitializationNode)
Add_Grammar(G_OPOBJECT, OPObjectNode)
Add_Grammar(G_OPOBJECT_INHERITANCE, OPObjectInheritanceNode)
Add_Grammar(G_OPOBJECT_BODY, OPObjectBodyNode)
Add_Grammar(G_STATE, StateNode)
Add_Grammar(G_STATEBLOCK, StateBlockNode)
Add_Grammar(G_TEMPLATE_TYPE,TemplateTypeNode)
Add_Grammar(G_TEMPLATE_TYPE_BODY, TemplateTypeBodyNode)
Add_Grammar(G_TEMPLATE_DECL,TemplateDeclNode)
Add_Grammar(G_ARRAY,ArrayNode)
Add_Grammar(G_TYPE_ARRAY,TypeArrayNode)
Add_Grammar(G_REFERENCE,ReferenceNode)
Add_Grammar(G_VISIBILITY_LABEL,VisibilityLabelNode)
Add_Grammar(G_SCOPE,ScopeNode)
Add_Grammar(G_SCOPE_POINTER,ScopePointerNode)
Add_Grammar(G_TYPENAME, TypenameNode)
Add_Grammar(G_TYPEDEF, TypedefNode)
Add_Grammar(G_FUNCTION_POINTER, FunctionPointerNode)
Add_Grammar(G_FUNCTION_POINTER_DECL, FunctionPointerDeclNode)
Add_Grammar(G_OPERATOR, OperatorNode)
Add_Grammar(G_OPDEFINE, OPDefineNode)
Add_Grammar(G_OPDEFINE_BODY, OPDefineBodyNode)
Add_Grammar(G_POUND_DEFINE, PoundDefineNode)
Add_Grammar(G_POUND_ELIF, PoundElifNode)
Add_Grammar(G_POUND_ELSE, PoundElseNode)
Add_Grammar(G_POUND_ENDIF, PoundEndifNode)
Add_Grammar(G_POUND_ERROR, PoundErrorNode)
Add_Grammar(G_POUND_IF, PoundIfNode)
Add_Grammar(G_POUND_IFDEF, PoundIfdefNode)
Add_Grammar(G_POUND_IFNDEF, PoundIfndefNode)
Add_Grammar(G_POUND_IMPORT, PoundImportNode)
Add_Grammar(G_POUND_INCLUDE, PoundIncludeNode)
Add_Grammar(G_POUND_LINE, PoundLineNode)
Add_Grammar(G_POUND_PRAGMA, PoundPragmaNode)
Add_Grammar(G_POUND_UNDEF, PoundUndefNode)
Add_Grammar(G_POUND_USING, PoundUsingNode)
Add_Grammar(G_POUND_WARNING, PoundWarningNode)
Add_Grammar(G_OPINCLUDE, OPIncludeNode)
Add_Grammar(G_MODIFIERS, ModifiersNode)
Add_Grammar(G_AUTO_MODIFIERS, AutoModifiersNode)
Add_Grammar(G_EXPAND_CALL, ExpandCallNode)
Add_Grammar(G_OPMACRO, OPMacroNode)
Add_Grammar(G_OPMACRO_ARGUMENT, OPMacroArgumentNode)
Add_Grammar(G_OPMACRO_ARGUMENT_LIST, OPMacroArgumentListNode)
Add_Grammar(G_OPMACRO_BODY, OPMacroBodyNode)
Add_Grammar(G_EXPAND_CALL_ARGUMENT, ExpandCallArgumentNode)
Add_Grammar(G_EXPAND_CALL_ARGUMENT_LIST, ExpandCallArgumentListNode)
Add_Grammar(G_CONCATENATION_ARGUMENT_OPERATOR, ConcatenationArgumentOperatorNode)
Add_Grammar(G_CONCATENATION_OPERATOR, ConcatenationOperatorNode)
Add_Grammar(G_SINGLE_QUOTE_OPERATOR, SingleQuoteOperatorNode)
Add_Grammar(G_DOUBLE_QUOTE_OPERATOR, DoubleQuoteOperatorNode)
Add_Grammar(G_EXPANDABLE_ARGUMENT, ExpandableArgumentNode)
Add_Grammar(G_FRIEND, FriendNode)
Add_Grammar(G_DATA_DECLARATION, DataDeclarationNode)
Add_Grammar(G_DATA_INITIALIZATION, DataInitializationNode)
Add_Grammar(G_DATA_BITS, DataBitsNode)
Add_Grammar(G_CPLUSPLUS, CPlusPlusNode)
Add_Grammar(G_CPLUSPLUS_BODY, CPlusPlusBodyNode)
Add_Grammar(G_CPPCONSTRUCT_INHERITANCE, CPPConstructInheritanceNode)
Add_Grammar(G_CLASS, ClassNode)
Add_Grammar(G_STRUCT, StructNode)
Add_Grammar(G_ENUM, EnumNode)
Add_Grammar(G_UNION, UnionNode)
Add_Grammar(G_TEMPLATED, TemplatedNode)

//valued data modifier
Add_Grammar(G_VALUED_MODIFIER,ValuedModifierNode)
Add_Grammar(G_VALUED_MODIFIER_ARGUMENT,ValuedModifierArgumentNode)

// statement grammars
Add_Grammar(G_STATEMENT,StatementNode)
Add_Grammar(G_FUNCTION_DEFINITION_STATEMENT,FunctionDefinitionStatementNode)
Add_Grammar(G_VISIBILITY_STATEMENT,VisibilityStatementNode)
Add_Grammar(G_PREPROCESSOR_STATEMENT, PreprocessorStatementNode)
Add_Grammar(G_STATE_STATEMENT,StateStatementNode)
Add_Grammar(G_NULL_STATEMENT,NullStatementNode)
Add_Grammar(G_FRIEND_STATEMENT,FriendStatementNode)
Add_Grammar(G_USING_STATEMENT, UsingStatementNode)
Add_Grammar(G_TYPEDEF_STATEMENT,TypedefStatementNode)
Add_Grammar(G_FUNCTION_POINTER_STATEMENT,FuncPointerStatementNode)
Add_Grammar(G_FUNCTION_PROTOTYPE_STATEMENT,FuncPrototypeStatementNode)
Add_Grammar(G_OPENUM_STATEMENT,OPEnumStatementNode)
Add_Grammar(G_OPOBJECT_STATEMENT, OPObjectStatementNode)
Add_Grammar(G_DATA_STATEMENT,DataStatementNode)
Add_Grammar(G_TEMPLATE_STATEMENT,TemplateStatementNode)
Add_Grammar(G_CONSTRUCTOR_DEFINITION_STATEMENT,ConstructorDefinitionStatementNode)
Add_Grammar(G_CONSTRUCTOR_PROTOTYPE_STATEMENT,ConstructorPrototypeStatementNode)
Add_Grammar(G_DESTRUCTOR_DEFINITION_STATEMENT,DestructorDefinitionStatementNode)
Add_Grammar(G_DESTRUCTOR_PROTOTYPE_STATEMENT,DestructorPrototypeStatementNode)
Add_Grammar(G_CPPCONSTRUCT_STATEMENT, CPPConstructStatementNode)
