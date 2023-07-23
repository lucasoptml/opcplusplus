#include "StatementInterfaces.h"

#pragma once

//
// BasicStatementsBase
//

//is current an unknown statement? transform stuff
template<class Parent>
inline bool BasicStatementsBase<Parent>::UnknownStatement(stacked<opNode>& stuff)
{
	if (opNode::IsCurrent(T_SEMICOLON))
	{
		stacked<StatementNode> statement = opNode::Transform<StatementNode>(stuff);

		if (statement->IsEmpty())
			statement->CopyBasics(opNode::CurrentNode());

		opNode::Erase(T_SEMICOLON);

		UnknownStatements.push_back(*statement);
		opNode::InsertNodeAtCurrent(statement);
		return true;
	}
	return false;
}

//
// BasicStatements
// 

//is current an openum statement? transform stuff
template<class Parent>
inline bool interfaces::BasicStatementsBase<Parent>::OPEnumStatement(stacked<opNode>& stuff)
{
	if (opNode::IsCurrent(G_OPENUM))
	{
		stackedcontext<OPEnumStatementNode> statement = opNode::Make<OPEnumStatementNode>(G_OPENUM);

		stacked<OPEnumNode> openum = opNode::Expect<OPEnumNode>(G_OPENUM);

		if (!stuff->IsEmpty())
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
		opNode::InsertNodeAtCurrent(statement);
		return true;
	}
	return false;
}


//is current an opclass statement? transform stuff
template<class Parent>
inline bool interfaces::BasicStatementsBase<Parent>::OPObjectStatement(stacked<opNode>& stuff)
{
	if (opNode::IsCurrent(G_OPOBJECT))
	{
		stackedcontext<OPObjectStatementNode> statement = opNode::Make<OPObjectStatementNode>(G_OPOBJECT);

		stacked<OPObjectNode> object = opNode::Expect<OPObjectNode>(G_OPOBJECT);

		if (!stuff->IsEmpty())
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
		opNode::InsertNodeAtCurrent(statement);
		return true;
	}
	return false;
}


//is current an opstate statement? transform stuff
template<class Parent>
inline bool interfaces::BasicStatementsBase<Parent>::StateStatement(stacked<opNode>& stuff)
{
	if (opNode::IsCurrent(G_STATE))
	{
		stackedcontext<StateStatementNode> statement = opNode::Make<StateStatementNode>(G_STATE);

		stacked<StateNode> state = opNode::Expect<StateNode>(G_STATE);

		if (!stuff->IsEmpty())
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
		opNode::InsertNodeAtCurrent(statement);
		return true;
	}
	return false;
}

//is current a visibility statement? transform stuff
template<class Parent>
inline bool interfaces::BasicStatementsBase<Parent>::VisibilityStatement(stacked<opNode>& stuff)
{
	if (opNode::IsCurrent(G_VISIBILITY_LABEL))
	{
		//ok, we need to turn this into a visibility statement
		//we actually need to error if we had something in this... it should be caught by AllowOnly!
		//looks...ok (would rather collapse stuff here)

		stackedcontext<VisibilityStatementNode> visstatement = opNode::Make<VisibilityStatementNode>(G_VISIBILITY_LABEL);

		stacked<VisibilityLabelNode> vislabel = opNode::Expect<VisibilityLabelNode>(G_VISIBILITY_LABEL);

		visstatement->SetLabel(*vislabel);
		visstatement->AppendNode(vislabel);

		opNode::CollapseNodeAtCurrent(stuff);

		opNode::InsertNodeAtCurrent(visstatement);
		return true;
	}
	return false;
}

//is current a function definition? transform stuff
template<class Parent>
inline bool interfaces::BasicStatementsBase<Parent>::FunctionDefinitionStatement(stacked<opNode>& stuff)
{
	if (opNode::IsCurrent(G_FUNCTION_DEFINITION))
	{
		//ok, we need to turn this into a function statement
		//how?
		stackedcontext<FunctionDefinitionStatementNode> statement = opNode::Make<FunctionDefinitionStatementNode>(G_FUNCTION_DEFINITION);

		stacked<FunctionDefinitionNode> functiondefinition = opNode::Expect<FunctionDefinitionNode>(G_FUNCTION_DEFINITION);

		if (!stuff->IsEmpty())
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
		opNode::InsertNodeAtCurrent(statement);
		return true;
	}
	return false;
}

template<class Parent>
inline bool interfaces::BasicStatementsBase<Parent>::ConstructorStatement(stacked<opNode>& stuff)
{
	if (opNode::IsCurrent(G_CONSTRUCTOR_DEFINITION))
	{
		//ok, we need to turn this into a function statement
		//how?
		stackedcontext<ConstructorDefinitionStatementNode>
			statement = opNode::Make<ConstructorDefinitionStatementNode>(G_CONSTRUCTOR_DEFINITION);

		stacked<ConstructorDefinitionNode>
			constructordefinition = opNode::Expect<ConstructorDefinitionNode>(G_CONSTRUCTOR_DEFINITION);

		if (!stuff->IsEmpty())
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
		opNode::InsertNodeAtCurrent(statement);
		return true;
	}
	return false;
}

template<class Parent>
inline bool interfaces::BasicStatementsBase<Parent>::DestructorStatement(stacked<opNode>& stuff)
{
	if (opNode::IsCurrent(G_DESTRUCTOR_DEFINITION))
	{
		//ok, we need to turn this into a function statement
		//how?
		stackedcontext<DestructorDefinitionStatementNode>
			statement = opNode::Make<DestructorDefinitionStatementNode>(G_DESTRUCTOR_DEFINITION);

		stacked<DestructorDefinitionNode>
			destructordefinition = opNode::Expect<DestructorDefinitionNode>(G_DESTRUCTOR_DEFINITION);

		if (!stuff->IsEmpty())
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
		opNode::InsertNodeAtCurrent(statement);
		return true;
	}
	return false;
}

template<class Parent>
inline bool interfaces::BasicStatementsBase<Parent>::ConditionalPreprocessorStatement(stacked<opNode>& stuff)
{
	if (opNode::IsCurrent(G_POUND_ELIF)
		|| opNode::IsCurrent(G_POUND_ELSE)
		|| opNode::IsCurrent(G_POUND_ENDIF)
		|| opNode::IsCurrent(G_POUND_IF)
		|| opNode::IsCurrent(G_POUND_IFDEF)
		|| opNode::IsCurrent(G_POUND_IFNDEF))
	{
		Token                                     id = opNode::CurrentNode()->GetId();
		stackedcontext<PreprocessorStatementNode> statement = opNode::Make<PreprocessorStatementNode>(id);
		stacked<PreprocessorNode>                 preprocessor = opNode::Expect<PreprocessorNode>(id);

		statement->SetPreprocessor(*preprocessor);
		statement->AppendNode(preprocessor);

		if (!stuff->IsEmpty())
			statement->CollapseNode(stuff, statement->GetBegin());
		else
			stuff.Delete();

		opNode::InsertNodeAtCurrent(statement);

		return true;
	}

	return false;
}

template<class Parent>
inline bool interfaces::BasicStatementsBase<Parent>::CPPConstructStatement(stacked<opNode>& stuff)
{
	if (opNode::IsCurrent(G_CLASS)
		|| opNode::IsCurrent(G_STRUCT)
		|| opNode::IsCurrent(G_ENUM)
		|| opNode::IsCurrent(G_UNION))
	{
		Token                                     id = opNode::CurrentNode()->GetId();
		stackedcontext<CPPConstructStatementNode> statement = opNode::Make<CPPConstructStatementNode>(id);
		stacked<CPPConstructNode>                 construct = opNode::Expect<CPPConstructNode>(id);

		if (!stuff->IsEmpty())
		{
			stacked<ModifiersNode> modifiers = opNode::Transform<ModifiersNode>(stuff);
			statement->SetModifiers(*modifiers);
			statement->AppendNode(modifiers);
		}
		else
			stuff.Delete();

		statement->SetConstruct(*construct);
		statement->AppendNode(construct);

		opNode::InsertNodeAtCurrent(statement);

		return true;
	}

	return false;
}

template<class Parent>
inline bool interfaces::BasicStatementsBase<Parent>::FriendStatement(stacked<opNode>& stuff)
{
	if (opNode::IsCurrent(G_FRIEND))
	{
		stacked<FriendStatementNode> statement = opNode::Make<FriendStatementNode>(G_FRIEND);
		stacked<FriendNode>          thefriend = opNode::Expect<FriendNode>(G_FRIEND);

		statement->SetFriend(*thefriend);
		statement->AppendNode(thefriend);

		if (!stuff->IsEmpty())
			statement->CollapseNode(stuff, statement->GetBegin());
		else
			stuff.Delete();

		opNode::InsertNodeAtCurrent(statement);

		return true;
	}

	return false;
}

template<class Parent>
inline bool interfaces::BasicStatementsBase<Parent>::TypedefStatement(stacked<opNode>& stuff)
{
	if (opNode::IsCurrent(G_TYPEDEF))
	{
		stackedcontext<TypedefStatementNode> statement = opNode::Make<TypedefStatementNode>(G_TYPEDEF);
		stacked<TypedefNode>                 thetypedef = opNode::Expect<TypedefNode>(G_TYPEDEF);

		if (!stuff->IsEmpty())
		{
			stacked<ModifiersNode> modifiers = opNode::Transform<ModifiersNode>(stuff);
			statement->SetModifiers(*modifiers);
			statement->AppendNode(modifiers);
		}
		else
			stuff.Delete();

		statement->SetTypedef(*thetypedef);
		statement->AppendNode(thetypedef);

		opNode::InsertNodeAtCurrent(statement);

		return true;
	}

	return false;
}

template<class Parent>
inline bool interfaces::BasicStatementsBase<Parent>::TemplateStatement(stacked<opNode>& stuff)
{
	if (opNode::IsCurrent(G_TEMPLATED))
	{
		stackedcontext<TemplateStatementNode> statement = opNode::Make<TemplateStatementNode>(G_TEMPLATED);
		stacked<TemplatedNode>                templated = opNode::Expect<TemplatedNode>(G_TEMPLATED);

		if (!stuff->IsEmpty())
		{
			stacked<ModifiersNode> modifiers = opNode::Transform<ModifiersNode>(stuff);

			statement->SetModifiers(*modifiers);
			statement->AppendNode(modifiers);
		}
		else
			stuff.Delete();

		statement->SetTemplated(*templated);
		statement->AppendNode(templated);

		opNode::InsertNodeAtCurrent(statement);

		return true;
	}

	return false;
}

//
// BasicStatements
//

template<class Parent>
inline bool BasicStatements<Parent>::Parse()
{
	PARSE_START;
	{
		FindBasicStatements();
	}
	PARSE_END;
}

template<class Parent>
inline bool BasicStatements<Parent>::PostParse()
{
	POSTPARSE_START;
	{
		AllowOnlyBasicStatements();
	}
	POSTPARSE_END;
}

// Only allow the following statements in opobjects.
template<class Parent>
inline void BasicStatements<Parent>::AllowOnlyBasicStatements()
{
	opNode::AllowOnly(G_STATEMENT,
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

// Find all statements except preprocessor ones.
template<class Parent>
inline void BasicStatements<Parent>::FindBasicStatements()
{
	bool bFinished = false;

	while (!bFinished)
	{
		//if ( TemplateStatement() )
		//	continue;

		stacked<opNode> stuff = PushUntilStatementEnd();

		bFinished = !(*stuff);

		if (bFinished)
			break;

		if (this->FunctionDefinitionStatement(stuff));
		else if (this->VisibilityStatement(stuff));
		else if (this->OPObjectStatement(stuff));
		else if (this->OPEnumStatement(stuff));
		else if (this->ConstructorStatement(stuff));
		else if (this->DestructorStatement(stuff));
		else if (this->TemplateStatement(stuff));
		else if (this->TypedefStatement(stuff));
		else if (this->FriendStatement(stuff));
		else if (this->CPPConstructStatement(stuff));
		else if (this->ConditionalPreprocessorStatement(stuff));
		else if (this->UnknownStatement(stuff));
		else
		{
			opNode::CollapseNodeAtCurrent(stuff);

			//I think this function should never error.
			bFinished = true;
		}
	}
}


//
// BasicStateStatements
//

template<class Parent>
void interfaces::BasicStateStatements<Parent>::FindStateStatements()
{
	//for now we only look for function definitions
	//later we can add a new statement type (G_STATEMENT_FUNCTIONSONLY)
	bool bfinished = false;
	while (!bfinished)
	{
		stacked<opNode> stuff = opNode::PushUntilOr<opNode>(G_FUNCTION_DEFINITION,
			G_STATE,
			T_SEMICOLON,
			false);

		bfinished = !(*stuff);
		if (bfinished)
			break;

		//handle null statements...
		if (opNode::IsCurrent(T_SEMICOLON))
		{
			opNode::Erase(T_SEMICOLON);

			opNode::CollapseNodeAtCurrent(stuff);
		}
		else if (this->FunctionDefinitionStatement(stuff));
		else if (this->StateStatement(stuff));
		else
		{
			opNode::CollapseNodeAtCurrent(stuff);

			//I think this function should never error.
			bfinished = true;
		}
	}
}

template<class Parent>
inline void BasicStateStatements<Parent>::AllowOnlyStateStatements()
{
	opNode::AllowOnly(G_FUNCTION_DEFINITION_STATEMENT,
		G_STATE_STATEMENT);
}

//
// UsingStatements 
//

template<class Parent>
inline bool interfaces::UsingStatements<Parent>::Parse()
{
	PARSE_START;
	{
		FindUsingStatements();
	}
	PARSE_END;
}

template<class Parent>
inline bool UsingStatements<Parent>::FindUsingStatements()
{
	INSPECT_START(G_USING_STATEMENT);
	{
		if (opNode::PeekStart(G_USING))
		{
			stacked<UsingStatementNode> newNode = opNode::PushUntilEnd<UsingStatementNode>();

			this->SetInnerStatement(*newNode);
			opNode::AppendNode(newNode);
		}
	}
	INSPECT_END;
}

//
// NullStatements
//


template<class Parent>
inline bool interfaces::NullStatements<Parent>::Parse()
{
	PARSE_START;
	{
		FindNullStatements();
	}
	PARSE_END;
}

template<class Parent>
inline bool NullStatements<Parent>::FindNullStatements()
{
	INSPECT_START(G_NULL_STATEMENT);
	{
		if (opNode::IsEmpty())
		{
			stacked<NullStatementNode> newNode = NEWNODE(NullStatementNode());

			newNode->CopyBasics(this);

			this->SetInnerStatement(*newNode);
			opNode::AppendNode(newNode);

			return true;
		}
	}
	INSPECT_END;
}

//
// FunctionPointerStatements
//


template<class Parent>
inline bool interfaces::FuncPointerStatements<Parent>::Parse()
{
	PARSE_START;
	{
		FindFuncPointerStatements();
	}
	PARSE_END;
}

template<class Parent>
inline bool FuncPointerStatements<Parent>::FindFuncPointerStatements()
{
	INSPECT_START(G_FUNCTION_POINTER_STATEMENT);
	{
		if (opNode::PeekEnd(G_FUNCTION_POINTER))
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

			this->SetInnerStatement(*newNode);
			opNode::AppendNode(newNode);

			return true;
		}
	}
	INSPECT_END;
}

//
// Function Prototype Statements
//

template<class Parent>
inline bool interfaces::FuncPrototypeStatements<Parent>::Parse()
{
	PARSE_START;
	{
		FindFuncPrototypeStatements();
	}
	PARSE_END;
}

template<class Parent>
inline bool FuncPrototypeStatements<Parent>::FindFuncPrototypeStatements()
{
	INSPECT_START(G_FUNCTION_PROTOTYPE_STATEMENT);
	{
		if (opNode::PeekEnd(G_FUNCTION_PROTOTYPE))
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

			this->SetInnerStatement(*newNode);
			opNode::AppendNode(newNode);

			return true;
		}
	}
	INSPECT_END;
}

//
// ConstructorPrototypeStatements
//

template<class Parent>
inline bool ConstructorPrototypeStatements<Parent>::FindConstructorPrototypeStatements()
{
	INSPECT_START(G_CONSTRUCTOR_PROTOTYPE_STATEMENT);
	{
		if (opNode::PeekEnd(G_CONSTRUCTOR_PROTOTYPE))
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

			this->SetInnerStatement(*newNode);
			opNode::AppendNode(newNode);

			return true;
		}
	}
	INSPECT_END;
}

//
// DestructorPrototypeStatements
//

template<class Parent>
inline bool DestructorPrototypeStatements<Parent>::FindDestructorPrototypeStatements()
{
	INSPECT_START(G_DESTRUCTOR_PROTOTYPE_STATEMENT);
	{
		if (opNode::PeekEnd(G_DESTRUCTOR_PROTOTYPE))
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

			this->SetInnerStatement(*newNode);
			opNode::AppendNode(newNode);

			return true;
		}
	}
	INSPECT_END;
}

//
// DataStatementOnly
//

template<NodeType Parent>
inline bool DataStatementOnly<Parent>::Parse()
{
	PARSE_START;
	{
		if (!this->FindDataStatementOnly())
		{
			opError::MessageError(this, "Unnamed data declarations not allowed.");
			//GenericError("unnamed data declarations not allowed");
		}
	}
	PARSE_END;
}

//
// ConditionalPreprocessorStatements
//


template<class Parent>
template<Token hittoken>
inline void ConditionalPreprocessorStatements<Parent>::FindConditionalPreprocessorStatement()
{
	LOOP_START(G_PREPROCESSOR_STATEMENT);
	{
		HIT(hittoken)
		{
			stackedcontext<PreprocessorStatementNode> newNode = opNode::Make<PreprocessorStatementNode>(hittoken);

			stacked<PreprocessorNode> preprocessor = opNode::Expect<PreprocessorNode>(hittoken);

			newNode->SetPreprocessor(*preprocessor);
			newNode->AppendNode(preprocessor);

			opNode::InsertNodeAtCurrent(newNode);
		}
	}
	LOOP_END;
}

//
// ConditionalPreprocessorStatements
//

template<class Parent>
inline bool ConditionalPreprocessorStatements<Parent>::Parse()
{
	PARSE_START;
	{
		FindConditionalPreprocessorStatements();
	}
	PARSE_END;
}