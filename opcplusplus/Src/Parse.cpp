///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Parse.cpp
/// Date: 11/26/2006
///
/// Description:
///
/// Contains all the Parse() methods for all grammar nodes.
///****************************************************************

#include "opCPP.h"

//
// ProgramNode
//

bool FileNode::Preprocessor()
{
	INIT_EXCEPTIONS
	{
		//pull out all preprocessor directives
		FindAllPounds();
		
		if (!CheckBlockCorrectness())
			return false;
		
		FindBraces();
		FindParentheses();
		FindBrackets();
		
		ProcessChildNodes();
	}
	CATCH_EXCEPTIONS;

	return true;
}

// checks for bracket, parentheses and brace correctness
// call this before parsing!!
bool FileNode::CheckBlockCorrectness()
{
	opList<opNode*> lefts;
	opString        error = "Encountered '";

	iterator i   = GetBegin();
	iterator end = GetEnd();

	// look for missing "left" or "right"
	while (i != end)
	{
		// if the current token is a left,
		// push it onto the lefts stack
		// check against all lefts
		if (!i->IsTerminal())
		{
			++i;
			continue;
		}

		Token Id = i->GetId();

		//left block known, add to the stack
		if (IsLeftBlock(Id))
			lefts.PushFront(*i);
		//right block, verify it
		else if (IsRightBlock(Id))
		{
			//make sure we have a potential match
			if (lefts.IsEmpty())
			{
				opString value;
				i->PrintString(value);
				
				opError::MessageError(*i, error + value + "' without preceding '"
					+ LeftBlockString(Id) + '\'', false);

				return false;
			}
			//make sure the top left and this new right block match
			else if (!BlockMatch(lefts.Front()->GetId(),Id))
			{
				opString value;
				opString frontvalue;
				lefts.Front()->PrintString(frontvalue);
				i->PrintString(value);

				opError::MessageError(lefts.Front(), error + frontvalue + "' without closing '"
					+ RightBlockString(lefts.Front()->GetId()) 
					+ "', found '" + value + "' instead", false);

				return false;
			}

			lefts.PopFront();
		}

		++i;
	}

	if (lefts.IsEmpty())
		return true;

	opString frontvalue;
	lefts.Front()->PrintString(frontvalue);

	opError::MessageError(lefts.Front(), error + frontvalue + 
		"' without terminating '" + RightBlockString(lefts.Front()->GetId()) + "'", false);

	return false;
}

//
// BraceBlockNode
//

bool BraceBlockNode::Parse()
{
	PARSE_START;
			
	FindBraces();
	FindBrackets();
	FindParentheses();

	PARSE_END;
}

//
// ParenBlockNode
//

bool ParenBlockNode::Parse()
{
	PARSE_START;

	//FindBraces();
	FindBrackets();
	FindParentheses();

	PARSE_END;
}

//
// BracketBlockNode
//

bool BracketBlockNode::Parse()
{
	PARSE_START;

	//FindBraces();
	FindBrackets();
	FindParentheses();

	PARSE_END;
}

//
// AngledBlockNode
//

bool AngledBlockNode::Parse()
{
	PARSE_START;

	FindAngles();
	FindBrackets();
	FindParentheses();
	
	//were this a known template angled bracket, we could check for other things as well.

	PARSE_END;
}

///
/// FunctionPointerDeclNode
///

bool FunctionPointerDeclNode::Parse()
{
	PARSE_START;

	//TODO: handle calling conventions??
	//MS calling conventions:
	//__cdecl
	//__clrcall
	//__stdcall
	//__fastcall
	//__thiscall

	//in GCC attributes are always of format __attribute__((att1, att2, att3(args)))
	//theres also synonyms for each like __noreturn__


	Scope = CheckOr(T_STAR,G_SCOPE_POINTER);
	
	Name = Check(T_ID);

	CheckNone();

	PARSE_END;	
}

//
// FunctionArgumentListNode
//

bool FunctionArgumentListNode::Parse()
{
	PARSE_START;
	
	if(!IsEmpty())
	{
		while(1)
		{
			stacked<FunctionArgumentNode> arg = opNode::PushUntil<FunctionArgumentNode>(T_COMMA,false);

			if(arg->IsEmpty())
				arg->CopyBasics(this);

			if(IsCurrent(T_COMMA))
			{
				Erase(T_COMMA);
				AddArgument(*arg);
				InsertNodeAtCurrent(arg);
			}
			else
			{
				AddArgument(*arg);
				InsertNodeAtCurrent(arg);
				break;
			}
		}
	}

	PARSE_END;
}

//
// FunctionArgumentNode
//

bool FunctionArgumentNode::Parse()
{
	PARSE_START;
	
	stacked<FunctionArgumentDefinitionNode> definition = opNode::PushUntil<FunctionArgumentDefinitionNode>(T_ASSIGN,false);

	Definition = *definition;
	InsertNodeAtCurrent(definition);

	if(IsCurrent(T_ASSIGN))
		Erase(T_ASSIGN);

	stacked<FunctionArgumentDefaultNode> initialization = opNode::PushUntilEnd<FunctionArgumentDefaultNode>();

	if(initialization->IsEmpty())
	{
		initialization.Delete();
	}
	else
	{
		Default = *initialization;		
		InsertNodeAtCurrent(initialization);
	}	
	
	PARSE_END;
}

//
// OPEnumBodyNode
//

bool OPEnumBodyNode::Parse()
{
	PARSE_START;

	while(IsCurrent(T_ID))
	{
		stacked<OPEnumElementNode> newNode = opNode::PushUntil<OPEnumElementNode>(T_COMMA, false);
		newNode->Init();

		if(IsCurrent(T_COMMA))
			Erase(T_COMMA);

		AddElement(*newNode);
		InsertNodeAtCurrent(newNode);
	}

	CheckNone();
	
	PARSE_END;
}

//
// OPEnumElementNode
//

bool OPEnumElementNode::Parse()
{
	PARSE_START;

	TerminalNode* name = Check<TerminalNode>(T_ID);
	Name = name;

	if (IsCurrent(T_ASSIGN))
	{
		Erase(T_ASSIGN);
		
		stacked<OPEnumInitializationNode> init = PushUntilEnd<OPEnumInitializationNode>();

		Initialization = *init;
		AppendNode(init);
	}
	else
		CheckNone();

	PARSE_END;
}

//
// OPInheritanceNode
//

bool OPObjectInheritanceNode::Parse()
{
	PARSE_START;

	//TODO: get the colon out of here?
	if(IsCurrent(T_COLON))
	{
		opNode* colon = Check(T_COLON);
		
		while(1)
		{
			Parent p;
			
			bool bVirtual = false;
			if(IsCurrent(T_VIRTUAL))
			{
				bVirtual = true;
				p.Virtual = Check<TerminalNode>(T_VIRTUAL);
			}
			
			if(IsCurrent(T_PUBLIC) || IsCurrent(T_PROTECTED) || IsCurrent(T_PRIVATE))
			{
				p.Visibility = CheckOr<TerminalNode>(T_PUBLIC,T_PROTECTED,T_PRIVATE);
			}
			
			if(!bVirtual && IsCurrent(T_VIRTUAL))
			{
				p.Virtual = Check<TerminalNode>(T_VIRTUAL);
			}

			p.Type = CheckOr(T_ID,G_SCOPE,G_TEMPLATE_TYPE);

			Parents.PushBack(p);

			//break if no comma
			if(IsCurrent(T_COMMA))
				Check(T_COMMA);
			else
				break;
		}

	}

	CheckNone();			//make sure there's nothing after this, besides whitespace

	PARSE_END;
}

///
/// OPMacroBodyNode
///

bool OPDefineBodyNode::Parse()
{
	PARSE_START;

	DisallowRecursive(T_CONTINUELINE);

	PARSE_END;
}

///
/// ConstructorInitializerNode
///

bool ConstructorInitializerListNode::Parse()
{
	PARSE_START;
	{
		LOOP_START(G_CONSTRUCTOR_INITIALIZER)
		{
			HIT(G_FUNCTION)
			{
				//here we convert a function and the argument list to
				//other types, then reconstruct the type (kinda weird, but it works)
				stacked<FunctionNode> f = opNode::Expect<FunctionNode>(G_FUNCTION);
				stacked<FunctionArgumentListNode> args = f->PopNode<FunctionArgumentListNode>(f->GetArguments());
				stacked<opNode> name = f->PopNode<opNode>(f->GetName());

				stacked<ConstructorInitializerArgumentNode> cargs = opNode::Transform<ConstructorInitializerArgumentNode>(args);
				stacked<ConstructorInitializerNode> constructor = opNode::Transform<ConstructorInitializerNode>(f);

				constructor->SetName(*name);
				constructor->AppendNode(name);
				constructor->SetArgument(*cargs);
				constructor->AppendNode(cargs);

				InsertNodeAtCurrent(constructor);
			}
		}
		LOOP_END;
	}
	PARSE_END;
}

///
/// FriendStatementNode
///

bool FriendStatementNode::Parse()
{
	PARSE_START;
	{
		Friend = Check<FriendNode>(G_FRIEND);
		CheckNone();
	}
	PARSE_END;
}

///
/// UsingStatementNode
///

bool UsingStatementNode::Parse()
{
	PARSE_START;
	{
		Using = Check<UsingNode>(G_USING);
		CheckNone();
	}
	PARSE_END;
}

///
/// DataDeclarationNode
///

bool DataDeclarationNode::Parse()
{
	PARSE_START;
	{
		//with bits
		if(IgnoreUntil<T_COLON>())
		{
			//grab bits - alter context here.
			stackedcontext<DataBitsNode> bits = opNode::Make<DataBitsNode>(T_COLON);
			Erase(T_COLON);
			
			stacked<TerminalNode> number = opNode::Expect<TerminalNode>(T_NUMBER);
			bits->SetNumber(*number);
			bits->AppendNode(number);
			
			Bits = *bits;
			
			CheckNone();
			
			//done with the context now
			bits.Disable();
			
			ParseTypeName();
			
			AppendNode(bits);
		}
		//without bits
		else
		{
			ParseTypeName();
		}
	}
	PARSE_END;
}

void DataDeclarationNode::ParseTypeName()
{
	//determine type, name, modifiers
	SetPosition(GetEnd());
	
	stacked<opNode> name = opNode::ReverseExpectOr(T_ID, G_ARRAY);
	Name = *name;
	
	stacked<opNode> type = opNode::ReverseExpectOr(T_ID,
											   	   G_POINTER, 
												   G_REFERENCE, 
												   G_TEMPLATE_TYPE, 
												   G_SCOPE, 
												   T_BASIC_TYPE,
												   G_FUNDAMENTAL_TYPE,
												   G_POINTER_MEMBER,
												   *name);
	Type = *type;
	
	ResetPosition();
	stacked<ModifiersNode> modifiers = opNode::PushUntilEnd<ModifiersNode>();
	
	DataStatementNode* statement = FindParent<DataStatementNode>();
	statement->SetModifiers(*modifiers);
	statement->PrependNode(modifiers);
	
	AppendNode(type);
	AppendNode(name);	
}

///
/// CriteriaBodyNode
///

bool CriteriaBodyNode::Parse()
{
	PARSE_START;
	{
		//build the operand and operator arrays

		if(IsCurrent(T_E_NOT))
		{
			Check(T_E_NOT);
			bNegate = true;
		}

		while(1)
		{
			opNode* operand = CheckOr(T_ID,
									  G_CRITERIA_VALUE_MODIFIER,
									  G_CRITERIA_GROUP,
									  T_PUBLIC,
									  T_PRIVATE,
									  T_PROTECTED,
									  T_VOLATILE,
									  T_STATIC,
									  T_EXPLICIT,
									  T_CONST,
									  T_VIRTUAL,
									  T_MUTABLE,
									  T_INLINE
									  );
			AddOperand(operand);

			//has an operator?
			if(IsCurrent(T_E_AND)
			|| IsCurrent(T_E_OR))
			{
				//grab operator and continue

				bool bAnd = CurrentNode()->GetId() == T_E_AND;
				CheckOr(T_E_AND,T_E_OR);

				bool bNot = false;
				if(IsCurrent(T_E_NOT))
				{
					Check(T_E_NOT);
					bNot = true;
				}
				
				//set the operand
				if(bAnd)
					AddOperator(bNot?And_Not:And);
				else
					AddOperator(bNot?Or_Not:Or);
			}
			else
			{
				break;
			}
		}
		
		CheckNone();

	}
	PARSE_END;
}