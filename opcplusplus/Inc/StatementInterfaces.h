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
	bool UnknownStatement(stacked<opNode>& stuff)
	{
		if(IsCurrent(T_SEMICOLON))
		{
			stacked<StatementNode> statement = opNode::Transform<StatementNode>(stuff);
			
			if(statement->IsEmpty())
				statement->CopyBasics(CurrentNode());
			
			Erase(T_SEMICOLON);
			
			UnknownStatements.push_back(*statement);
			InsertNodeAtCurrent(statement);
			return true;
		}
		return false;
	}

	//list of all unknown statements gathered
	vector<StatementNode*> UnknownStatements;

	///==========================================
	/// OPEnumStatement
	///==========================================

	//is current an openum statement? transform stuff
	bool OPEnumStatement(stacked<opNode>& stuff)
	{
		if(IsCurrent(G_OPENUM))
		{
			stackedcontext<OPEnumStatementNode> statement = opNode::Make<OPEnumStatementNode>(G_OPENUM);

			stacked<OPEnumNode> openum = opNode::Expect<OPEnumNode>(G_OPENUM);

			if(!stuff->IsEmpty())
			{
				stacked<ModifiersNode> modifiers = opNode::Transform<ModifiersNode>(stuff);
				statement->SetModifiers(*modifiers);
				statement->AppendNode(modifiers);
			}
			else
				stuff.Delete();

			statement->SetEnum(*openum);
			statement->AppendNode(openum);

			OPEnumStatements.push_back(*statement);
			InsertNodeAtCurrent(statement);
			return true;
		}
		return false;
	}

	//list of openum statements gathered
	vector<OPEnumStatementNode*> OPEnumStatements;

	///==========================================
	/// OPObjectStatement
	///==========================================

	//is current an opclass statement? transform stuff
	bool OPObjectStatement(stacked<opNode>& stuff)
	{
		if(IsCurrent(G_OPOBJECT))
		{
			stackedcontext<OPObjectStatementNode> statement = opNode::Make<OPObjectStatementNode>(G_OPOBJECT);

			stacked<OPObjectNode> object = opNode::Expect<OPObjectNode>(G_OPOBJECT);

			if(!stuff->IsEmpty())
			{
				stacked<ModifiersNode> modifiers = opNode::Transform<ModifiersNode>(stuff);
				statement->SetModifiers(*modifiers);
				statement->AppendNode(modifiers);
			}
			else
				stuff.Delete();

			statement->SetObject(*object);
			statement->AppendNode(object);

			OPObjectStatements.push_back(*statement);
			InsertNodeAtCurrent(statement);
			return true;
		}
		return false;
	}

	//list of opobject statements gathered
	vector<OPObjectStatementNode*> OPObjectStatements;

	///==========================================
	/// StateStatement
	///==========================================

	//is current an opstate statement? transform stuff
	bool StateStatement(stacked<opNode>& stuff)
	{
		if(IsCurrent(G_STATE))
		{
			stackedcontext<StateStatementNode> statement = opNode::Make<StateStatementNode>(G_STATE);

			stacked<StateNode> state = opNode::Expect<StateNode>(G_STATE);

			if(!stuff->IsEmpty())
			{
				stacked<ModifiersNode> modifiers = opNode::Transform<ModifiersNode>(stuff);
				statement->SetModifiers(*modifiers);
				statement->AppendNode(modifiers);
			}
			else
				stuff.Delete();					

			statement->SetState(*state);
			statement->AppendNode(state);

			StateStatements.push_back(*statement);
			InsertNodeAtCurrent(statement);
			return true;
		}
		return false;
	}

	//list of state statements gathered
	vector<StateStatementNode*> StateStatements;

	///==========================================
	/// VisibilityStatement
	///==========================================

	//is current a visibility statement? transform stuff
	bool VisibilityStatement(stacked<opNode>& stuff)
	{
		if(IsCurrent(G_VISIBILITY_LABEL))
		{
			//ok, we need to turn this into a visibility statement
			//we actually need to error if we had something in this... it should be caught by AllowOnly!
			//looks...ok (would rather collapse stuff here)

			stackedcontext<VisibilityStatementNode> visstatement = opNode::Make<VisibilityStatementNode>(G_VISIBILITY_LABEL);

			stacked<VisibilityLabelNode> vislabel = opNode::Expect<VisibilityLabelNode>(G_VISIBILITY_LABEL);

			visstatement->SetLabel(*vislabel);
			visstatement->AppendNode(vislabel);

			CollapseNodeAtCurrent(stuff);

			InsertNodeAtCurrent(visstatement);
			return true;
		}
		return false;
	}

	//we don't gather a list of visibility statements since they're order dependent

	///==========================================
	/// FunctionDefinitionStatement
	///==========================================

	//is current a function definition? transform stuff
	bool FunctionDefinitionStatement(stacked<opNode>& stuff)
	{
		if(IsCurrent(G_FUNCTION_DEFINITION))
		{
			//ok, we need to turn this into a function statement
			//how?
			stackedcontext<FunctionDefinitionStatementNode> statement = opNode::Make<FunctionDefinitionStatementNode>(G_FUNCTION_DEFINITION);
			
			stacked<FunctionDefinitionNode> functiondefinition = opNode::Expect<FunctionDefinitionNode>(G_FUNCTION_DEFINITION);
			
			if(!stuff->IsEmpty())
			{
				stacked<ModifiersNode> modifiers = opNode::Transform<ModifiersNode>(stuff);
				statement->SetModifiers(*modifiers);
				statement->AppendNode(modifiers);
			}
			else
				stuff.Delete();
			
			//need to do something w/ this
			statement->SetFunctionDefinition(*functiondefinition);
			statement->AppendNode(functiondefinition);
			
			FunctionDefinitionStatements.push_back(*statement);
			InsertNodeAtCurrent(statement);
			return true;
		}
		return false;
	}
	
	//list of function definition statements gathered (not function prototypes!)
	vector<FunctionDefinitionStatementNode*> FunctionDefinitionStatements;

	///==========================================
	/// ConstructorStatement
	///==========================================
	
	bool ConstructorStatement(stacked<opNode>& stuff)
	{
		if(IsCurrent(G_CONSTRUCTOR_DEFINITION))
		{
			//ok, we need to turn this into a function statement
			//how?
			stackedcontext<ConstructorDefinitionStatementNode> 
			statement = opNode::Make<ConstructorDefinitionStatementNode>(G_CONSTRUCTOR_DEFINITION);
			
			stacked<ConstructorDefinitionNode>
			constructordefinition = opNode::Expect<ConstructorDefinitionNode>(G_CONSTRUCTOR_DEFINITION);
			
			if(!stuff->IsEmpty())
			{
				stacked<ModifiersNode> modifiers = opNode::Transform<ModifiersNode>(stuff);
				statement->SetModifiers(*modifiers);
				statement->AppendNode(modifiers);
			}
			else
				stuff.Delete();
			
			//need to do something w/ this
			statement->SetConstructorDefinition(*constructordefinition);
			statement->AppendNode(constructordefinition);
			
			ConstructorDefinitionStatements.push_back(*statement);
			InsertNodeAtCurrent(statement);
			return true;
		}
		return false;
	}
	
	//list of constructor definition statements gathered (not prototypes)
	vector<ConstructorDefinitionStatementNode*> ConstructorDefinitionStatements;
	
	///==========================================
	/// DestructorStatement
	///==========================================
	
	bool DestructorStatement(stacked<opNode>& stuff)
	{
		if(IsCurrent(G_DESTRUCTOR_DEFINITION))
		{
			//ok, we need to turn this into a function statement
			//how?
			stackedcontext<DestructorDefinitionStatementNode> 
				statement = opNode::Make<DestructorDefinitionStatementNode>(G_DESTRUCTOR_DEFINITION);

			stacked<DestructorDefinitionNode>
				destructordefinition = opNode::Expect<DestructorDefinitionNode>(G_DESTRUCTOR_DEFINITION);

			if(!stuff->IsEmpty())
			{
				stacked<ModifiersNode> modifiers = opNode::Transform<ModifiersNode>(stuff);
				statement->SetModifiers(*modifiers);
				statement->AppendNode(modifiers);
			}
			else
				stuff.Delete();
			
			//need to do something w/ this
			statement->SetDestructorDefinition(*destructordefinition);
			statement->AppendNode(destructordefinition);
			
			DestructorDefinitionStatements.push_back(*statement);
			InsertNodeAtCurrent(statement);
			return true;
		}
		return false;
	}
	
	//list of constructor definition statements gathered (not prototypes)
	vector<DestructorDefinitionStatementNode*> DestructorDefinitionStatements;
	
	///==========================================
	/// PreprocessorStatement
	///==========================================
	
	bool ConditionalPreprocessorStatement(stacked<opNode>& stuff)
	{
		if (IsCurrent(G_POUND_ELIF)
		||  IsCurrent(G_POUND_ELSE)
		||  IsCurrent(G_POUND_ENDIF)
		||  IsCurrent(G_POUND_IF)
		||  IsCurrent(G_POUND_IFDEF)
		||  IsCurrent(G_POUND_IFNDEF))
		{
			Token                                     id           = CurrentNode()->GetId();
			stackedcontext<PreprocessorStatementNode> statement    = opNode::Make<PreprocessorStatementNode>(id);
			stacked<PreprocessorNode>                 preprocessor = opNode::Expect<PreprocessorNode>(id);

			statement->SetPreprocessor(*preprocessor);
			statement->AppendNode(preprocessor);

			if (!stuff->IsEmpty())
				statement->CollapseNode(stuff, statement->GetBegin());
			else
				stuff.Delete();

			InsertNodeAtCurrent(statement);

			return true;
		}

		return false;
	}
	
	///==========================================
	/// CPPConstructStatement
	///==========================================
	
	bool CPPConstructStatement(stacked<opNode>& stuff)
	{
		if (IsCurrent(G_CLASS)
		||  IsCurrent(G_STRUCT)
		||  IsCurrent(G_ENUM)
		||  IsCurrent(G_UNION))
		{
			Token                                     id        = CurrentNode()->GetId();
			stackedcontext<CPPConstructStatementNode> statement = opNode::Make<CPPConstructStatementNode>(id);
			stacked<CPPConstructNode>                 construct = opNode::Expect<CPPConstructNode>(id);

			if ( !stuff->IsEmpty() )
			{
				stacked<ModifiersNode> modifiers = opNode::Transform<ModifiersNode>( stuff );
				statement->SetModifiers( *modifiers );
				statement->AppendNode( modifiers );
			}
			else
				stuff.Delete();
			
			statement->SetConstruct(*construct);
			statement->AppendNode(construct);

			InsertNodeAtCurrent(statement);

			return true;
		}

		return false;
	}

	//==========================================
	// FriendStatements
	//==========================================

	bool FriendStatement(stacked<opNode>& stuff)
	{
		if ( IsCurrent( G_FRIEND ) )
		{
			stacked<FriendStatementNode> statement = opNode::Make<FriendStatementNode>( G_FRIEND );
			stacked<FriendNode>          thefriend = opNode::Expect<FriendNode>( G_FRIEND );

			statement->SetFriend( *thefriend );
			statement->AppendNode( thefriend );

			if (!stuff->IsEmpty())
				statement->CollapseNode(stuff, statement->GetBegin());
			else
				stuff.Delete();

			InsertNodeAtCurrent(statement);

			return true;
		}

		return false;
	}

	//==========================================
	// TypedefStatements
	//==========================================

	bool TypedefStatement(stacked<opNode>& stuff)
	{
		if ( IsCurrent( G_TYPEDEF ) )
		{
			stackedcontext<TypedefStatementNode> statement  = opNode::Make<TypedefStatementNode>( G_TYPEDEF );
			stacked<TypedefNode>                 thetypedef = opNode::Expect<TypedefNode>( G_TYPEDEF );

			if ( !stuff->IsEmpty() )
			{
				stacked<ModifiersNode> modifiers = opNode::Transform<ModifiersNode>( stuff );
				statement->SetModifiers( *modifiers );
				statement->AppendNode( modifiers );
			}
			else
				stuff.Delete();

			statement->SetTypedef( *thetypedef );
			statement->AppendNode( thetypedef );

			InsertNodeAtCurrent( statement );

			return true;
		}

		return false;
	}

	///==========================================
	/// TemplateStatements
	///==========================================

	bool TemplateStatement(stacked<opNode>& stuff)
	{
		if ( IsCurrent(G_TEMPLATED) )
		{
			stackedcontext<TemplateStatementNode> statement = opNode::Make<TemplateStatementNode>(G_TEMPLATED);
			stacked<TemplatedNode>                templated = opNode::Expect<TemplatedNode>(G_TEMPLATED);

			if ( !stuff->IsEmpty() )
			{
				stacked<ModifiersNode> modifiers = opNode::Transform<ModifiersNode>( stuff );

				statement->SetModifiers( *modifiers );
				statement->AppendNode( modifiers );
			}
			else
				stuff.Delete();

			statement->SetTemplated( *templated );
			statement->AppendNode( templated );

			InsertNodeAtCurrent(statement);

			return true;
		}

		return false;
	}
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

	bool PostParse()
	{
		POSTPARSE_START;
		{
			AllowOnlyBasicStatements();
		}
		POSTPARSE_END;
	}
	
	// Only allow the following statements in opobjects.
	void AllowOnlyBasicStatements()
	{
		AllowOnly(G_STATEMENT,
				  G_OPENUM_STATEMENT,
				  G_OPOBJECT_STATEMENT,
				  G_FUNCTION_DEFINITION_STATEMENT,
				  G_VISIBILITY_STATEMENT,
				  G_CONSTRUCTOR_DEFINITION_STATEMENT,
				  G_DESTRUCTOR_DEFINITION_STATEMENT,
				  G_TYPEDEF_STATEMENT,
				  G_FRIEND_STATEMENT,
				  G_PREPROCESSOR_STATEMENT,
				  G_CPPCONSTRUCT_STATEMENT,
				  G_CPLUSPLUS,
				  G_TEMPLATE_STATEMENT);
	}
	
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
	void FindBasicStatements()
	{
		bool bFinished = false;

		while(!bFinished)
		{
			//if ( TemplateStatement() )
			//	continue;

			stacked<opNode> stuff = PushUntilStatementEnd();
			
			bFinished = !(*stuff);
			
			if (bFinished)
				break;
			
			if		(FunctionDefinitionStatement(stuff));
			else if	(VisibilityStatement(stuff));
			else if (OPObjectStatement(stuff));
			else if	(OPEnumStatement(stuff));
			else if (ConstructorStatement(stuff));
			else if (DestructorStatement(stuff));
			else if (TemplateStatement(stuff));
			else if (TypedefStatement(stuff));
			else if (FriendStatement(stuff));
			else if (CPPConstructStatement(stuff));
			else if (ConditionalPreprocessorStatement(stuff));
			else if	(UnknownStatement(stuff));
			else
			{
				CollapseNodeAtCurrent(stuff);
				
				//I think this function should never error.
				bFinished = true;
			}
		}
	}
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

	void FindStateStatements()
	{
		//for now we only look for function definitions
		//later we can add a new statement type (G_STATEMENT_FUNCTIONSONLY)
		bool bfinished = false;
		while(!bfinished)
		{
			stacked<opNode> stuff = opNode::PushUntilOr<opNode>(G_FUNCTION_DEFINITION,
														G_STATE,
														T_SEMICOLON,
														false);
			
			bfinished = !(*stuff);
			if(bfinished)
				break;
			
			//handle null statements...
			if(IsCurrent(T_SEMICOLON))
			{
				Erase(T_SEMICOLON);

				CollapseNodeAtCurrent(stuff);
			}
			else if	(FunctionDefinitionStatement(stuff));
			else if (StateStatement(stuff));
			else
			{
				CollapseNodeAtCurrent(stuff);

				//I think this function should never error.
				bfinished = true;
			}
		}
	}

	void AllowOnlyStateStatements()
	{
		AllowOnly(G_FUNCTION_DEFINITION_STATEMENT,
				  G_STATE_STATEMENT);
	}
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

	bool FindUsingStatements()
	{
		INSPECT_START(G_USING_STATEMENT);
		{
			if (PeekStart(G_USING))
			{
				stacked<UsingStatementNode> newNode = opNode::PushUntilEnd<UsingStatementNode>();

				SetInnerStatement(*newNode);
				AppendNode(newNode);
			}
		}
		INSPECT_END;
	}
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

	bool FindNullStatements()
	{
		INSPECT_START(G_NULL_STATEMENT);
		{
			if (IsEmpty())
			{
				stacked<NullStatementNode> newNode = NEWNODE(NullStatementNode());

				newNode->CopyBasics(this);

				SetInnerStatement(*newNode);
				AppendNode(newNode);

				return true;
			}
		}
		INSPECT_END;
	}
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

	bool FindFuncPointerStatements()
	{
		INSPECT_START(G_FUNCTION_POINTER_STATEMENT);
		{
			if (PeekEnd(G_FUNCTION_POINTER))
			{
				stackedcontext<FuncPointerStatementNode> newNode = NEWNODE(FuncPointerStatementNode());
				newNode->CopyBasics(this);
				
				stacked<ModifiersNode> modifiers = opNode::PushUntil<ModifiersNode>(G_FUNCTION_POINTER);
				stacked<FunctionPointerNode> fpn = opNode::Expect<FunctionPointerNode>(G_FUNCTION_POINTER);
				
				if (!modifiers->IsEmpty())
				{
					modifiers->CopyBasics(*fpn);
					newNode->SetModifiers(*modifiers);
					newNode->AppendNode(modifiers);
				}
				else
					modifiers.Delete();
				
				newNode->SetFunctionPointer(*fpn);
				newNode->AppendNode(fpn);
				
				SetInnerStatement(*newNode);
				AppendNode(newNode);
				
				return true;
			}
		}
		INSPECT_END;
	}
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

	bool FindFuncPrototypeStatements()
	{
		INSPECT_START(G_FUNCTION_PROTOTYPE_STATEMENT);
		{
			if (PeekEnd(G_FUNCTION_PROTOTYPE))
			{
				stackedcontext<FuncPrototypeStatementNode> newNode = NEWNODE(FuncPrototypeStatementNode());
				newNode->CopyBasics(this);

				stacked<ModifiersNode> modifiers = opNode::PushUntil<ModifiersNode>(G_FUNCTION_PROTOTYPE);
				stacked<FunctionPrototypeNode> fpn = opNode::Expect<FunctionPrototypeNode>(G_FUNCTION_PROTOTYPE);

				if (!modifiers->IsEmpty())
				{
					modifiers->CopyBasics(*fpn);
					newNode->SetModifiers(*modifiers);
					newNode->AppendNode(modifiers);
				}
				else
					modifiers.Delete();

				newNode->SetFunctionPrototype(*fpn);
				newNode->AppendNode(fpn);

				SetInnerStatement(*newNode);
				AppendNode(newNode);

				return true;
			}
		}
		INSPECT_END;
	}
};

///
/// ConstructorPrototypeStatements
///

template<class Parent>
class ConstructorPrototypeStatements : public Parent
{
public:
	IMPLEMENTS_INTERFACE(ConstructorPrototypeStatements)

	bool FindConstructorPrototypeStatements()
	{
		INSPECT_START(G_CONSTRUCTOR_PROTOTYPE_STATEMENT);
		{
			if (PeekEnd(G_CONSTRUCTOR_PROTOTYPE))
			{
				stackedcontext<ConstructorPrototypeStatementNode> newNode = NEWNODE(ConstructorPrototypeStatementNode());
				
				newNode->CopyBasics(this);
				
				stacked<ModifiersNode> modifiers = opNode::PushUntil<ModifiersNode>(G_CONSTRUCTOR_PROTOTYPE);
				stacked<ConstructorPrototypeNode> cpn = opNode::Expect<ConstructorPrototypeNode>(G_CONSTRUCTOR_PROTOTYPE);
				
				if (!modifiers->IsEmpty())
				{
					modifiers->CopyBasics(*cpn);
					newNode->SetModifiers(*modifiers);
					newNode->AppendNode(modifiers);
				}
				else
					modifiers.Delete();
				
				newNode->SetConstructorPrototype(*cpn);
				newNode->AppendNode(cpn);
				
				SetInnerStatement(*newNode);
				AppendNode(newNode);
				
				return true;
			}
		}
		INSPECT_END;
	}
};

///
/// DestructorPrototypeStatements
///

template<class Parent>
class DestructorPrototypeStatements : public Parent
{
public:
	IMPLEMENTS_INTERFACE(DestructorPrototypeStatements)

		bool FindDestructorPrototypeStatements()
	{
		INSPECT_START(G_DESTRUCTOR_PROTOTYPE_STATEMENT);
		{
			if (PeekEnd(G_DESTRUCTOR_PROTOTYPE))
			{
				stackedcontext<DestructorPrototypeStatementNode> newNode = NEWNODE(DestructorPrototypeStatementNode());

				newNode->CopyBasics(this);

				stacked<ModifiersNode> modifiers = opNode::PushUntil<ModifiersNode>(G_DESTRUCTOR_PROTOTYPE);
				stacked<DestructorPrototypeNode> cpn = opNode::Expect<DestructorPrototypeNode>(G_DESTRUCTOR_PROTOTYPE);

				if (!modifiers->IsEmpty())
				{
					modifiers->CopyBasics(*cpn);
					newNode->SetModifiers(*modifiers);
					newNode->AppendNode(modifiers);
				}
				else
					modifiers.Delete();

				newNode->SetDestructorPrototype(*cpn);
				newNode->AppendNode(cpn);

				SetInnerStatement(*newNode);
				AppendNode(newNode);

				return true;
			}
		}
		INSPECT_END;
	}
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

	bool FindDataStatements()
	{
		INSPECT_START(G_DATA_STATEMENT);
		{
			FindDataStatementOnly();
		}
		INSPECT_END;
	}

	bool FindDataStatementOnly();
};

template<class Parent>
class DataStatementOnly : public DataStatements<Parent>
{
public:
	IMPLEMENTS_INTERFACE(DataStatementOnly)

	bool Parse()
	{
		PARSE_START;
		{
			if(!FindDataStatementOnly())
			{
				opError::MessageError(this,"Unnamed data declarations not allowed.");
				//GenericError("unnamed data declarations not allowed");
			}
		}
		PARSE_END;
	}
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
	void FindConditionalPreprocessorStatement()
	{
		LOOP_START(G_PREPROCESSOR_STATEMENT);
		{
			HIT(hittoken)
			{
				stackedcontext<PreprocessorStatementNode> newNode = opNode::Make<PreprocessorStatementNode>(hittoken);

				stacked<PreprocessorNode> preprocessor = opNode::Expect<PreprocessorNode>(hittoken);

				newNode->SetPreprocessor(*preprocessor);
				newNode->AppendNode(preprocessor);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}
};

} // end namespace interfaces

