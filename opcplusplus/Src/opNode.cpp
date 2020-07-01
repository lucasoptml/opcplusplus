///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: opNode.cpp
/// Date: 11/26/2006
///
/// Description:
///
/// Contains the source code for many of the opNode base class methods.
///****************************************************************

#include "opCPP.h"

//
// opNode
//

// prints out the ast
void opNode::PrintTree(const opString& filename, int depth)
{
	FileNode* file = GetFile();
	opString  s    = file->GetInputName();
	int       line = GetLine();
	
	s += '(';
	
	if (line == -1)
		s += "UNKNOWN) : \t";
	else
		s += opString(line) + ") : \t";
	
	for (int i = 0; i < depth; i++)
		s += ". ";
	
	if (IsTerminal())
	{
		Token t = GetId();

		//NOTE: this prints the value now (make optional?)
		if (t == T_NEWLINE 
		||  t == T_EOF
		||  t == T_CONTINUELINE
		||  t == T_CCOMMENT)
		{
			Log(s + TokenFunctions::ToString(GetId()));
		}
		else
			Log(s + TokenFunctions::ToString(GetId()) + "     '" + GetTreeValue() + "'" );
	}
	else
	{
		// If we're not in -fulltree mode, we want to limit
		// the kinds of nodes we print (mostly auto modifiers).
		if ( !opParameters::Get().PrintFullTree
		&&   (id == G_AUTO_MODIFIERS) )
		{
			return;
		}

		opString value = GetTreeValue();
		
		if ( !value.Size() )
			Log(s + TokenFunctions::ToString(GetId()));
		else
			Log(s + TokenFunctions::ToString(GetId()) + "     '" + value + "'" );

		iterator i   = GetBegin();
		iterator end = GetEnd();

		while (i != end)
		{
			i->PrintTree(filename, depth + 1);

			++i;
		}
	}
}

// expect, but instead of returning the node,
// remove it
void opNode::Erase(Token t)
{
	opNode* errornode = this;

	if(!IsEmpty() && GetPosition() != GetBegin())
	{
		iterator previous = GetPosition();
		--previous;
		errornode = *previous;
	}

	//check for end of block
	if (GetPosition() == GetEnd())
	{
		opError::PrematureError(t,errornode,false);
	}

	Token tok = CurrentNode()->GetId();
	
	//check for end of file
	if(tok == T_EOF)
	{
		opError::PrematureError(t,errornode,true);
	}
	
	//check for match
	if (tok != t)
		opError::ExpectError(t,CurrentNode());

	DeleteCurrentNode();
}

void opNode::ReverseErase(Token t)
{
	vector<Token> Tokens;
	Tokens.push_back(t);
	
	iterator current = GetPosition();

	//if its empty
	if (IsEmpty())
	{
		opError::MissingError(Tokens,this);
	}
	else if (current == GetBegin())
	{
		// error here -- premature beginning of file ??
		// can't walk backwards to check, theres nothing there.
		opError::ReverseExpectError(Tokens,NULL,*current);
	}

	iterator previous = current;
	--previous;

	Token tok = previous->GetId();

	if (tok != t)
	{
		// reverse expect error
		opError::ReverseExpectError(Tokens,*previous,*GetPosition());
	}

	stacked<opNode> node = PopNode(previous);
	node.Delete();
}