///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: StatementInterfaces.h
/// Date: 12/19/2006
///
/// Description: Statement related interface definitions
///
///****************************************************************

namespace interfaces
{

///
/// BasicStatementsBase
///

template<class Parent>
class BasicStatementsBase : public Parent
{
public:
	IMPLEMENTS_INTERFACE(BasicStatementsBase)

	///==========================================
	/// UnknownStatement
	///==========================================

	//is current an unknown statement? transform stuff
	bool UnknownStatement(stacked<opNode>& stuff);

	//list of all unknown statements gathered
	vector<StatementNode*> UnknownStatements;

	///==========================================
	/// OPEnumStatement
	///==========================================

	//is current an openum statement? transform stuff
	bool OPEnumStatement(stacked<opNode>& stuff);

	//list of openum statements gathered
	vector<OPEnumStatementNode*> OPEnumStatements;

	///==========================================
	/// OPObjectStatement
	///==========================================

	//is current an opclass statement? transform stuff
	bool OPObjectStatement(stacked<opNode>& stuff);

	//list of opobject statements gathered
	vector<OPObjectStatementNode*> OPObjectStatements;

	///==========================================
	/// StateStatement
	///==========================================

	//is current an opstate statement? transform stuff
	bool StateStatement(stacked<opNode>& stuff);

	//list of state statements gathered
	vector<StateStatementNode*> StateStatements;

	///==========================================
	/// VisibilityStatement
	///==========================================

	//is current a visibility statement? transform stuff
	bool VisibilityStatement(stacked<opNode>& stuff);

	//we don't gather a list of visibility statements since they're order dependent

	///==========================================
	/// FunctionDefinitionStatement
	///==========================================

	//is current a function definition? transform stuff
	bool FunctionDefinitionStatement(stacked<opNode>& stuff);
	
	//list of function definition statements gathered (not function prototypes!)
	vector<FunctionDefinitionStatementNode*> FunctionDefinitionStatements;

	///==========================================
	/// ConstructorStatement
	///==========================================
	
	bool ConstructorStatement(stacked<opNode>& stuff);
	
	//list of constructor definition statements gathered (not prototypes)
	vector<ConstructorDefinitionStatementNode*> ConstructorDefinitionStatements;
	
	///==========================================
	/// DestructorStatement
	///==========================================
	
	bool DestructorStatement(stacked<opNode>& stuff);
	
	//list of constructor definition statements gathered (not prototypes)
	vector<DestructorDefinitionStatementNode*> DestructorDefinitionStatements;
	
	///==========================================
	/// PreprocessorStatement
	///==========================================
	
	bool ConditionalPreprocessorStatement(stacked<opNode>& stuff);
	
	///==========================================
	/// CPPConstructStatement
	///==========================================
	
	bool CPPConstructStatement(stacked<opNode>& stuff);

	//==========================================
	// FriendStatements
	//==========================================

	bool FriendStatement(stacked<opNode>& stuff);

	//==========================================
	// TypedefStatements
	//==========================================

	bool TypedefStatement(stacked<opNode>& stuff);

	///==========================================
	/// TemplateStatements
	///==========================================

	bool TemplateStatement(stacked<opNode>& stuff);
};

///
/// BasicStatements
///

template<class Parent>
class BasicStatements : public BasicStatementsBase<Parent>
{
public:
	IMPLEMENTS_INTERFACE(BasicStatements)
	REQUIRES_INTERFACE(Functions)
	REQUIRES_INTERFACE(VisibilityLabels)
	REQUIRES_INTERFACE(OPObjects)
	REQUIRES_INTERFACE(OPEnums)

	bool Parse()
	{
		PARSE_START;
		{
			FindBasicStatements();
		}
		PARSE_END;
	}

	bool PostParse();
	
	// Only allow the following statements in opobjects.
	void AllowOnlyBasicStatements();
	
	stacked<opNode> PushUntilStatementEnd()
	{
		return opNode::PushUntilOr<opNode>(T_SEMICOLON,
										   G_OPOBJECT,
										   G_OPENUM,
										   G_FUNCTION_DEFINITION,
										   G_VISIBILITY_LABEL,
										   G_CONSTRUCTOR_DEFINITION,
										   G_DESTRUCTOR_DEFINITION,
										   G_TEMPLATED,
										   G_TYPEDEF,
										   G_FRIEND,
										   G_CLASS,
										   G_STRUCT,
										   G_ENUM,
										   G_UNION,
										   G_POUND_ELIF,
										   G_POUND_ELSE,
										   G_POUND_ENDIF,
										   G_POUND_IF,
										   G_POUND_IFDEF,
										   G_POUND_IFNDEF,
										   false);
	}
	
	// Find all statements except preprocessor ones.
	void FindBasicStatements();
};

///
/// BasicStateStatements
///

template<class Parent>
class BasicStateStatements : public BasicStatementsBase<Parent>
{
public:
	IMPLEMENTS_INTERFACE(BasicStateStatements)
	REQUIRES_INTERFACE(States)
	REQUIRES_INTERFACE(FunctionDefinitions)
	REQUIRES_INTERFACE(Clean)
	
	//TODO: split up the utility function interface
	//		so we don't have all these extra gathering vectors.

	void FindStateStatements();

	void AllowOnlyStateStatements();
};


///
/// FriendStatements
///

// template<class Parent>
// class FriendStatements : public Parent
// {
// public:
// 	IMPLEMENTS_INTERFACE(FriendStatements)
// 
// 	bool Parse()
// 	{
// 		PARSE_START;
// 		{
// 			FindFriendStatements();
// 		}
// 		PARSE_END;
// 	}
// 
// 	bool FindFriendStatements()
// 	{
// 		INSPECT_START(G_FRIEND_STATEMENT);
// 		{
// 			if (PeekStart(G_FRIEND))
// 			{
// 				stacked<FriendStatementNode> newNode = opNode::PushUntilEnd<FriendStatementNode>();
// 				
// 				SetInnerStatement(*newNode);
// 				AppendNode(newNode);
// 			}
// 		}
// 		INSPECT_END;
// 	}
// };

///==========================================
/// UsingStatements
///==========================================

template<class Parent>
class UsingStatements : public Parent
{
public:
	IMPLEMENTS_INTERFACE(UsingStatements)

	bool Parse()
	{
		PARSE_START;
		{
			FindUsingStatements();
		}
		PARSE_END;
	}

	bool FindUsingStatements();
};

///
/// NullStatements
///

template<class Parent>
class NullStatements : public Parent
{
public:
	IMPLEMENTS_INTERFACE(NullStatements)

	bool Parse()
	{
		PARSE_START;
		{
			FindNullStatements();
		}
		PARSE_END;
	}

	bool FindNullStatements();
};

///
/// TypedefStatements
///

// template<class Parent>
// class TypedefStatements : public Parent
// {
// public:
// 	IMPLEMENTS_INTERFACE(TypedefStatements)
// 
// 	bool Parse()
// 	{
// 		PARSE_START;
// 		{
// 			FindTypedefStatements();
// 		}
// 		PARSE_END;
// 	}
// 
// 	bool FindTypedefStatements()
// 	{
// 		INSPECT_START(G_TYPEDEF_STATEMENT);
// 		{
// 			if (PeekStart(G_TYPEDEF))
// 			{
// 				stacked<TypedefStatementNode> newNode = opNode::PushUntilEnd<TypedefStatementNode>();
// 
// 				SetInnerStatement(*newNode);
// 				AppendNode(newNode);
// 			}
// 		}
// 		INSPECT_END;
// 	}
// };

///
/// FuncPointerStatements
///

//NOTE: may need to add initialization for this.
template<class Parent>
class FuncPointerStatements : public Parent
{
public:
	IMPLEMENTS_INTERFACE(FuncPointerStatements)

	bool Parse()
	{
		PARSE_START;
		{
			FindFuncPointerStatements();
		}
		PARSE_END;
	}

	bool FindFuncPointerStatements();
};

///
/// FuncPrototypeStatements
///

template<class Parent>
class FuncPrototypeStatements : public Parent
{
public:
	IMPLEMENTS_INTERFACE(FuncPrototypeStatements)

	bool Parse()
	{
		PARSE_START;
		{
			FindFuncPrototypeStatements();
		}
		PARSE_END;
	}

	bool FindFuncPrototypeStatements();
};

///
/// ConstructorPrototypeStatements
///

template<class Parent>
class ConstructorPrototypeStatements : public Parent
{
public:
	IMPLEMENTS_INTERFACE(ConstructorPrototypeStatements)

	bool FindConstructorPrototypeStatements();
};

///
/// DestructorPrototypeStatements
///

template<class Parent>
class DestructorPrototypeStatements : public Parent
{
public:
	IMPLEMENTS_INTERFACE(DestructorPrototypeStatements)

	bool FindDestructorPrototypeStatements();
};

///
/// TemplateStatements
///

// template<class Parent>
// class TemplateStatements : public Parent
// {
// public:
// 	IMPLEMENTS_INTERFACE(TemplateStatements)
// 
// 	bool Parse()
// 	{
// 		PARSE_START;
// 		{
// 			FindTemplateStatements();
// 		}
// 		PARSE_END;
// 	}
// 	
// 	bool FindTemplateStatements()
// 	{
// 		INSPECT_START(G_TEMPLATE_STATEMENT)
// 		{
// 			if(PeekStart(G_TEMPLATE_DECL))
// 			{
// 				//format: template<...> ...;
// 				stackedcontext<TemplateStatementNode> newNode = opNode::Make<TemplateStatementNode>(G_TEMPLATE_DECL);
// 				stacked<TemplateDeclNode> templatenode = opNode::Expect<TemplateDeclNode>(G_TEMPLATE_DECL);
// 				
// 				stacked<TemplateStatementBodyNode> body = opNode::PushUntilEnd<TemplateStatementBodyNode>();
// 				
// 				newNode->SetTemplate(*templatenode);
// 				newNode->SetBody(*body);
// 				newNode->AppendNode(templatenode);
// 				newNode->AppendNode(body);
// 				
// 				SetInnerStatement(*newNode);
// 				AppendNode(newNode);
// 				
// 				return true;
// 			}
// 		}
// 		INSPECT_END;
// 	}
// };

///
/// DataStatements
///

template<class Parent>
class DataStatements : public Parent
{
public:
	IMPLEMENTS_INTERFACE(DataStatements)

	bool Parse()
	{
		PARSE_START;
		{
			FindDataStatements();
		}
		PARSE_END;
	}

	bool FindDataStatements();

	bool FindDataStatementOnly();
};

template<class Parent>
class DataStatementOnly : public DataStatements<Parent>
{
public:
	IMPLEMENTS_INTERFACE(DataStatementOnly)

	bool Parse();
};

///==========================================
/// ConditionalPreprocessorStatements
///==========================================

template<class Parent>
class ConditionalPreprocessorStatements : public Parent
{
public:
	IMPLEMENTS_INTERFACE(ConditionalPreprocessorStatements)

	bool Parse()
	{
		PARSE_START;
		{
			FindConditionalPreprocessorStatements();
		}
		PARSE_END;
	}

	void FindConditionalPreprocessorStatements()
	{
		FindConditionalPreprocessorStatement<G_POUND_ELIF>();
		FindConditionalPreprocessorStatement<G_POUND_ELSE>();
		FindConditionalPreprocessorStatement<G_POUND_ENDIF>();
		FindConditionalPreprocessorStatement<G_POUND_IF>();
		FindConditionalPreprocessorStatement<G_POUND_IFDEF>();
		FindConditionalPreprocessorStatement<G_POUND_IFNDEF>();
	}

	template<Token hittoken>
	void FindConditionalPreprocessorStatement();
};











} // end namespace interfaces

