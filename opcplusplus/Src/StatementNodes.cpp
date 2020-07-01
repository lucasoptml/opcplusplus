///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: StatementNodes.cpp
/// Date: 12/19/2006
///
/// Description:
///
/// Statement node source code.
///****************************************************************

#include "opCPP.h"

///
/// StatementBase
///

// check for duplicate modifiers
void StatementModifierBase::CheckForDuplicates()
{
	iterator i   = modifiers->GetBegin();
	iterator j;
	iterator end = modifiers->GetEnd();

	while (i != end)
	{
		j = i;
		++j;

		while (j != end)
		{
			Token itok = i->GetId();
			Token jtok = j->GetId();

			if (itok == jtok 
			&&  itok != G_CPLUSPLUS) // allow duplicate c++ modifiers!
			{
				// If we have two valued modifiers, make sure they're distinct.
				if ( itok == G_VALUED_MODIFIER )
				{				
					ValuedModifierNode* imod = node_cast<ValuedModifierNode>( *i );
					ValuedModifierNode* jmod = node_cast<ValuedModifierNode>( *j );

					if ( imod->GetModifier()->GetValue() == jmod->GetModifier()->GetValue() )
					{
						opError::MessageError(jmod, "Duplicate valued modifier '" + jmod->GetModifier()->GetValue() + "( ... )' found!");
					}
				}
				// If we have two regular modifiers, make sure they're distinct.
				else if ( itok == T_MODIFIER )
				{
					TerminalNode* imod = node_cast<TerminalNode>( *i );
					TerminalNode* jmod = node_cast<TerminalNode>( *j );

					if ( imod->GetValue() == jmod->GetValue() )
					{
						opError::MessageError(jmod, "Duplicate modifier '" + jmod->GetValue() + "' found!");
					}
				}
				else
					opError::MessageError(*j, "Duplicate modifier '" + j->GetStringValue() + "' found!");
			}

			++j;
		}

		++i;
	}
}

// make sure there is only zero or one visibility label
void StatementModifierBase::CheckSingleVisibility()
{
	iterator i   = modifiers->GetBegin();
	iterator end = modifiers->GetEnd();
	int      num = 0;
	
	while (i != end)
	{
		Token tok = i->GetId();
		
		if (tok == T_PUBLIC || tok == T_PRIVATE || tok == T_PROTECTED)
		{
			++num;
		}
		
		if(num > 1)
		{
			opError::MessageError(*i,"Ambiguous visibility modifiers (public,private,protected): only one allowed per statement!");
		}

		++i;
	}
}

void StatementModifierBase::BuildArguments(const vector<opString>& valuedmodifiers, 
										   vector<ValuedModifierNode*>& arguments)
{
	int valnum = (int)valuedmodifiers.size();
	for(int i = 0; i < valnum; i++)
	{
		ValuedModifierNode* node = NULL;

		if(modifiers)
			node = modifiers->GetValuedModifier(valuedmodifiers[i]);
		if(automodifiers && !node)
			node = automodifiers->GetValuedModifier(valuedmodifiers[i]);

		if(node)
			arguments.push_back(node);
	}
}

void StatementModifierBase::BuildValueModifiers(vector<ValuedModifierNode*>& modifierlist)
{
	if(modifiers)
		modifiers->BuildValueModifiers(modifierlist);
	if(automodifiers)
		automodifiers->BuildValueModifiers(modifierlist);
}

void StatementModifierBase::BuildModifiers(opList<opNode*>& modifierlist)
{
	if(modifiers)
	{
		iterator begin	= modifiers->GetBegin();
		iterator end	= modifiers->GetEnd();
		modifierlist.Insert(modifierlist.end(),begin,end);
	}
	if(automodifiers)
	{
		iterator begin	= automodifiers->GetBegin();
		iterator end	= automodifiers->GetEnd();
		modifierlist.Insert(modifierlist.end(),begin,end);
	}
}

///
/// StatementNode
///

bool StatementNode::Parse()
{
	PARSE_START;

	// TODO - rank by most common!
	if (FindNullStatements()
	||  FindUsingStatements()
	//||  FindTypedefStatements()
	||  FindFuncPointerStatements()
	||  FindFuncPrototypeStatements()
	||	FindConstructorPrototypeStatements()
	||  FindDestructorPrototypeStatements()
	||  FindDataStatements())
	{
		// EMPTY BODY
	}
	
	PARSE_END;
}

bool StatementNode::PostParse()
{
	POSTPARSE_START;
	{
		AllowOnly(G_USING_STATEMENT,
				  G_FRIEND_STATEMENT,
				  G_NULL_STATEMENT,
				  G_DATA_STATEMENT,
				  G_FUNCTION_POINTER_STATEMENT,
				  G_FUNCTION_PROTOTYPE_STATEMENT,
				  G_CONSTRUCTOR_PROTOTYPE_STATEMENT,
				  G_DESTRUCTOR_PROTOTYPE_STATEMENT,
				  G_CPLUSPLUS);
	}
	POSTPARSE_END;
}

///
/// FunctionStatementBase
///

void FunctionStatementBase::CheckModifiers()
{
	modifiers->AllowOnly(G_VALUED_MODIFIER,
						 T_MODIFIER,
						 T_PUBLIC,
						 T_PRIVATE,
						 T_PROTECTED,
						 T_INLINE,
						 T_UNINLINE,
						 T_STATIC,
						 T_VIRTUAL,
						 T_CONST,
						 G_CPLUSPLUS,
						 T_ID);

	modifiers->CheckFunctionModifiers();
}


///
/// DataStatementBase
///

void DataStatementBase::CheckModifiers()
{
	AlterContext(this);
	
	modifiers->AllowOnly(G_VALUED_MODIFIER,
						 T_MODIFIER,
						 T_PUBLIC,
						 T_PRIVATE,
						 T_PROTECTED,
						 T_STATIC,
						 T_CONST,
						 T_VOLATILE,
						 T_MUTABLE,
						 G_CPLUSPLUS,
						 T_ID);

	modifiers->CheckDataModifiers();
}

///
/// ConstructorStatementBase
///

void ConstructorStatementBase::CheckModifiers()
{	
	
	modifiers->AllowOnly(G_VALUED_MODIFIER,
						 T_MODIFIER,
						 T_PUBLIC,
						 T_PRIVATE,
						 T_PROTECTED,
						 T_INLINE,
						 T_UNINLINE,
						 T_EXPLICIT,
						 G_CPLUSPLUS);

	modifiers->CheckFunctionModifiers();
}

///
/// DestructorStatementBase
///

void DestructorStatementBase::CheckModifiers()
{
	AlterContext(this);
	
	modifiers->AllowOnly(G_VALUED_MODIFIER,
						 T_MODIFIER,
						 T_PUBLIC,
						 T_PRIVATE,
						 T_PROTECTED,
						 T_INLINE,
						 T_UNINLINE,
						 T_VIRTUAL,
						 G_CPLUSPLUS);

	modifiers->CheckFunctionModifiers();
}