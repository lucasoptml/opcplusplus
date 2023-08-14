///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: opNodeInlines.h
/// Date: 11/26/2006
///
/// Description:
///
/// This file contains the implementation for all inline methods in the
/// opNode base class.
///****************************************************************

#pragma once

#include "Timer.h"
#include "Paths.h"

///
/// opNodeBase
///

// copy line number from another node
inline void opNodeBase::CopyLineNum(opNode* n)
{
	line = n->line;
}

inline void opNodeBase::CopyFile(opNode* n)
{
	file = n->file;
}

// destructor
inline opNodeBase::~opNodeBase()
{
	iterator start = GetBegin();
	iterator end   = GetEnd();

	while (start != end)
	{
		delete *start;

		++start;
	}

	parent = NULL;
}

template<class N>
inline void opNodeBase::AppendNode(stacked<N>& newchild)
{
	AppendNode(*newchild);
	newchild.Connected();
}

template<class N>
inline void opNodeBase::PrependNode(stacked<N>& newchild)
{
	PrependNode(*newchild);
	newchild.Connected();
}

// appends a node to end of children list
inline void opNodeBase::AppendNode(opNode* newChild)
{
	newChild->parent = (opNode*) this;
	children.PushBack(newChild);
}

// appends a node to end of children list
inline void opNodeBase::PrependNode(opNode* newChild)
{
	newChild->parent = (opNode*) this;
	children.PushFront(newChild);
}

template<class N> 
inline void opNodeBase::InsertNode(stacked<N>& newChild, iterator index)
{
	InsertNode(*newChild, index);
	newChild.Connected();
}

// inserts a node at index 
inline void opNodeBase::InsertNode(opNode* newChild, iterator index)
{
	newChild->parent = (opNode*) this;
	children.Insert(index, newChild);
}

template<class N>
inline void opNodeBase::InsertNodeAtCurrent(stacked<N>& newChild)
{
	InsertNodeAtCurrent(*newChild);
	newChild.Connected();
}

// inserts a node at the current position
inline void opNodeBase::InsertNodeAtCurrent(opNode* newChild)
{
	InsertNode(newChild, pos);
}

// Removes a node from the children deque at "index"
inline void opNodeBase::RemoveNode(iterator index)
{
	assert(index != GetEnd());

	if(GetPosition() == index)
		IncrementPosition();

#ifdef _DEBUG
	index->parent = NULL;
#endif	
	
	children.Erase(index);
}

// Removes node from children deque at "pos"
inline void opNodeBase::RemoveCurrentNode()
{
	RemoveNode(pos);
}

// Removes first node
inline void opNodeBase::RemoveFirstNode()
{
	RemoveNode(GetBegin());
}

// Removes last node
inline void opNodeBase::RemoveLastNode()
{
	RemoveNode(GetLast());
}

// deletes a node from the children deque at "index"
inline void opNodeBase::DeleteNode(iterator index)
{
	assert(index != GetEnd());

	//if we're deleting the position, our position will
	//get invalidated, so we need to move it first
	if(index == GetPosition())
		IncrementPosition();

	delete *index;

	children.Erase(index);
}

// deletes the specified node, if found
inline void opNodeBase::DeleteNode(opNode* node)
{
	iterator i = FindNodeIndex(node);

	if (i != GetEnd())
		DeleteNode(i);
}

// deletes node from children deque at "pos"
inline void opNodeBase::DeleteCurrentNode()
{
	DeleteNode(pos);
}

// deletes first node
inline void opNodeBase::DeleteFirstNode()
{
	DeleteNode(GetBegin());
}

// deletes last node
inline void opNodeBase::DeleteLastNode()
{
	DeleteNode(GetLast());
}

//pops a known node into a stacked object
template<class N>
inline stacked<N> opNodeBase::PopNode(N* node)
{
	iterator index = FindNodeIndex(node);

	//this should be safe.
	N* popped = (N*) *index;
	RemoveNode(index);

	return stacked<N>(popped);
}

// pops node into a stacked object
inline stacked<opNode> opNodeBase::PopNode(iterator index)
{
	assert(index != GetEnd());

	opNode* popped = *index;
	RemoveNode(index);

	return stacked<opNode>(popped);
}

inline stacked<opNode> opNodeBase::PopCurrentNode()
{
	return PopNode(pos);
}

inline stacked<opNode> opNodeBase::PopFirstNode()
{
	return PopNode(GetBegin());
}

inline stacked<opNode> opNodeBase::PopLastNode()
{
	return PopNode(GetLast());
}

// move nodes
inline void opNodeBase::MoveNodes(iterator index, opNode* node, iterator start, iterator end)
{
	//switch parent
	iterator it = start;
	while(it != end)
	{
		it->parent = (opNode*)this;
		++it;
	}
	
	children.Splice(index, node->children, start, end);
}

// general collapse node method, deletes the node
template<class N> 
inline void opNodeBase::CollapseNode(stacked<N>& collapse, iterator index)
{
	iterator it = collapse->children.begin();
	iterator end = collapse->children.end();

	while(it != end)
	{
		it->parent = (opNode*)this;
		++it;
	}

	children.Splice(index, collapse->children);
	collapse.Delete();
}

// drops all the nodes contents at the current position
template<class N>
inline void opNodeBase::CollapseNodeAtCurrent(stacked<N>& collapse)
{
	CollapseNode(collapse, pos);
}

// returns current node
inline opNode* opNodeBase::CurrentNode()
{
	assert(pos != GetEnd());

	return *pos;
}

// returns current node, or null
// do not call in empty nodes!
inline opNode* opNodeBase::PreviousNode()
{
	iterator lastpos = pos;

	if (lastpos == GetBegin())
		return NULL;

	return *(--lastpos);
}

// return first child
inline opNode* opNodeBase::FirstChild()
{
	iterator first = GetBegin();

	assert(first != GetEnd());

	return *first;
}

// return last child
inline opNode* opNodeBase::LastChild()
{
	iterator last = children.Last();

	assert(last != GetEnd());

	return *last;
}

// returns index of input node
inline opNode::iterator opNodeBase::FindNodeIndex(opNode* innode)
{
	return children.Find(innode);
}

///
/// opNode
///

/**** process ****/

// process
inline bool opNode::Process() 
{
	//was parsing successful?
	//quit processing this node and its children if not.
	if (!Parse())
		return false;
	
	return ProcessChildNodes();
}

// process children
inline bool opNode::ProcessChildNodes()
{
	iterator i		= GetBegin();
	iterator end	= GetEnd();
	bool     result = true;
	
	while (i != end)
	{
		result = i->Process() ? result : false;
		
		++i;
	}
	
	return result;
}

// returns true if the current node is a whitespace node
inline bool opNode::IsWhitespace()
{
	Token tok = GetId();

	if(tok == T_WHITESPACE || tok == T_NEWLINE)
		return true;

	return false;
}

// returns true if this node is a comment
inline bool opNode::IsComment()
{
	Token tok = GetId();

	if(tok == T_CCOMMENT || tok == T_COMMENT)
		return true;

	return false;
}

// eats whitespace until a non-whitespace character is reached
inline void opNode::EatWhitespaceAndComments()
{
	iterator end = GetEnd();

	// ignore whitespace
	while (GetPosition() != end)
	{
		if (!CurrentNode()->IsWhitespace() && !CurrentNode()->IsComment())
			break;

		DeleteCurrentNode();
	}
}

// returns true if this node is an overridable operator
// found list of overloadable operators here:
// http://en.wikipedia.org/wiki/Operators_in_C_and_C++
inline bool opNode::IsOverloadableOperator()
{
	Token id = GetId();

	if (id == T_PLUS      ||
		id == T_PLUS_PLUS ||
		id == T_PLUS_EQUALS ||
		id == T_MINUS ||
		id == T_MINUS_MINUS ||
		id == T_MINUS_EQUALS ||
		id == T_STAR ||
		id == T_STAR_EQUALS ||
		id == T_DIVIDE ||
		id == T_DIVIDE_EQUALS ||
		id == T_MODULUS || 
		id == T_MODULUS_EQUALS ||
		id == T_LESS_THAN ||
		id == T_LESS_THAN_OR_EQUAL_TO ||
		id == T_GREATER_THAN ||
		id == T_GREATER_THAN_OR_EQUAL_TO ||
		id == T_NOT_EQUALS ||
		id == T_EQUALS ||
		id == T_NEGATION ||
		id == T_AND ||
		id == T_OR ||
		id == T_LEFT_SHIFT ||
		id == T_LEFT_SHIFT_EQUALS ||
		id == T_RIGHT_SHIFT ||
		id == T_RIGHT_SHIFT_EQUALS ||
		id == T_BITWISE_COMPLEMENT ||
		id == T_AMPERSAND ||
		id == T_BITWISE_AND_EQUALS ||
		id == T_BITWISE_INCLUSIVE_OR ||
		id == T_BITWISE_INCLUSIVE_OR_EQUALS ||
		id == T_BITWISE_EXCLUSIVE_OR ||
		id == T_BITWISE_EXCLUSIVE_OR_EQUALS ||
		id == T_ASSIGN ||
		id == T_ARROW ||
		id == T_ARROW_STAR ||
		id == T_COMMA ||
		id == T_NEW ||
		id == T_DELETE)
	{
		return true;
	}

	if (id == G_PAREN_BLOCK && IsEmpty())
		return true;

	if (id == G_BRACKET_BLOCK && IsEmpty())
		return true;

	// TODO: check for cast operator!

	return false;
}

// returns true if the current opnode is a modifier
inline bool opNode::IsModifier()
{
	return TokenFunctions::IsModifier(GetId());
}

//returns true if the node is inside a node w/ the given token
inline bool opNode::IsInside(Token t)
{
	if(!GetParent())
		return false;
	if(GetParent()->GetId() == t)
		return true;
	return GetParent()->IsInside(t);
}

struct TokenListEnd
{
	static inline bool Matches(Token compare)
	{
		return false;
	}
};

template<Token token, typename nexttoken = TokenListEnd>
struct TokenListNode
{
	static inline bool Matches(Token compare)
	{
		if(compare == token)
			return true;
		return nexttoken::Matches(compare);
	}
};

//syntax


/**** private matching functions / utility functions ****/

template<typename TokenList>
inline bool opNode::IgnoreUntilBase()
{
	iterator end = GetEnd();

	while (GetPosition() != end)
	{
		Token tok = CurrentNode()->GetId();

		if (TokenList::Matches(tok))
			return true;

		IncrementPosition();
	}

	return false;
}

template<Token t1>
inline bool opNode::IgnoreUntil()
{
	return IgnoreUntilBase< 
		TokenListNode<t1
		>
	>
	();
}

template<Token t1, Token t2>
inline bool opNode::IgnoreUntil()
{
	return IgnoreUntilBase< 
		TokenListNode<t1,
			TokenListNode<t2> 
		>
	>
	();
}

template<Token t1, Token t2, Token t3>
inline bool opNode::IgnoreUntil()
{
	return IgnoreUntilBase< 
		TokenListNode<t1,
		TokenListNode<t2,
		TokenListNode<t3
		> > >
	>
	();
}

template<Token t1, Token t2, Token t3, Token t4>
inline bool opNode::IgnoreUntil()
{
	return IgnoreUntilBase< 
		TokenListNode<t1,
		TokenListNode<t2,
		TokenListNode<t3,
		TokenListNode<t4
		> > > >
	>
	();
}

template<Token t1, Token t2, Token t3, Token t4, Token t5>
inline bool opNode::IgnoreUntil()
{
	return IgnoreUntilBase< 
		TokenListNode<t1,
		TokenListNode<t2,
		TokenListNode<t3,
		TokenListNode<t4,
		TokenListNode<t5
		> > > > >
	>
	();
}

template<Token t1, Token t2, Token t3, Token t4, Token t5, Token t6>
inline bool opNode::IgnoreUntil()
{
	return IgnoreUntilBase< 
		TokenListNode<t1,
		TokenListNode<t2,
		TokenListNode<t3,
		TokenListNode<t4,
		TokenListNode<t5,
		TokenListNode<t6
		> > > > > >
	>
	();
}

template<Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7>
inline bool opNode::IgnoreUntil()
{
	return IgnoreUntilBase< 
		TokenListNode<t1,
		TokenListNode<t2,
		TokenListNode<t3,
		TokenListNode<t4,
		TokenListNode<t5,
		TokenListNode<t6,
		TokenListNode<t7
		> > > > > > >
	>
	();
}

template<Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8>
inline bool opNode::IgnoreUntil()
{
	return IgnoreUntilBase< 
		TokenListNode<t1,
		TokenListNode<t2,
		TokenListNode<t3,
		TokenListNode<t4,
		TokenListNode<t5,
		TokenListNode<t6,
		TokenListNode<t7,
		TokenListNode<t8
		> > > > > > > >
	>
	();
}

template<Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9>
inline bool opNode::IgnoreUntil()
{
	return IgnoreUntilBase< 
		TokenListNode<t1,
		TokenListNode<t2,
		TokenListNode<t3,
		TokenListNode<t4,
		TokenListNode<t5,
		TokenListNode<t6,
		TokenListNode<t7,
		TokenListNode<t8,
		TokenListNode<t9
		> > > > > > > > >
	>
	();
}

template<Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10>
inline bool opNode::IgnoreUntil()
{
	return IgnoreUntilBase< 
		TokenListNode<t1,
		TokenListNode<t2,
		TokenListNode<t3,
		TokenListNode<t4,
		TokenListNode<t5,
		TokenListNode<t6,
		TokenListNode<t7,
		TokenListNode<t8,
		TokenListNode<t9,
		TokenListNode<t10
		> > > > > > > > > >
	>
	();
}

template<Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11>
inline bool opNode::IgnoreUntil()
{
	return IgnoreUntilBase< 
		TokenListNode<t1,
		TokenListNode<t2,
		TokenListNode<t3,
		TokenListNode<t4,
		TokenListNode<t5,
		TokenListNode<t6,
		TokenListNode<t7,
		TokenListNode<t8,
		TokenListNode<t9,
		TokenListNode<t10,
		TokenListNode<t11
		> > > > > > > > > > >
	>
	();
}

template<Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12>
inline bool opNode::IgnoreUntil()
{
	return IgnoreUntilBase< 
		TokenListNode<t1,
		TokenListNode<t2,
		TokenListNode<t3,
		TokenListNode<t4,
		TokenListNode<t5,
		TokenListNode<t6,
		TokenListNode<t7,
		TokenListNode<t8,
		TokenListNode<t9,
		TokenListNode<t10,
		TokenListNode<t11,
		TokenListNode<t12
		> > > > > > > > > > > >
	>
	();
}

template<Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13>
inline bool opNode::IgnoreUntil()
{
	return IgnoreUntilBase< 
		TokenListNode<t1,
		TokenListNode<t2,
		TokenListNode<t3,
		TokenListNode<t4,
		TokenListNode<t5,
		TokenListNode<t6,
		TokenListNode<t7,
		TokenListNode<t8,
		TokenListNode<t9,
		TokenListNode<t10,
		TokenListNode<t11,
		TokenListNode<t12,
		TokenListNode<t13
		> > > > > > > > > > > > >
	>
	();
}

template<Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14>
inline bool opNode::IgnoreUntil()
{
	return IgnoreUntilBase< 
		TokenListNode<t1,
		TokenListNode<t2,
		TokenListNode<t3,
		TokenListNode<t4,
		TokenListNode<t5,
		TokenListNode<t6,
		TokenListNode<t7,
		TokenListNode<t8,
		TokenListNode<t9,
		TokenListNode<t10,
		TokenListNode<t11,
		TokenListNode<t12,
		TokenListNode<t13,
		TokenListNode<t14
		> > > > > > > > > > > > > >
	>
	();
}

template<Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15>
inline bool opNode::IgnoreUntil()
{
	return IgnoreUntilBase< 
		TokenListNode<t1,
		TokenListNode<t2,
		TokenListNode<t3,
		TokenListNode<t4,
		TokenListNode<t5,
		TokenListNode<t6,
		TokenListNode<t7,
		TokenListNode<t8,
		TokenListNode<t9,
		TokenListNode<t10,
		TokenListNode<t11,
		TokenListNode<t12,
		TokenListNode<t13,
		TokenListNode<t14,
		TokenListNode<t15
		> > > > > > > > > > > > > > >
	>
	();
}

// ignore until


/*
inline bool opNode::IgnoreUntil(Token t)
{
	vector<Token> tokens;

	tokens.push_back(t);

	return IgnoreUntilBase(tokens);
}


// ignore until or - 2 tokens
inline bool opNode::IgnoreUntilOr(Token t1, Token t2)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);

	return IgnoreUntilBase(tokens);
}

// ignore until or - 3 tokens
inline bool opNode::IgnoreUntilOr(Token t1, Token t2, Token t3)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);

	return IgnoreUntilBase(tokens);
}

// ignore until or - 4 tokens
inline bool opNode::IgnoreUntilOr(Token t1, Token t2, Token t3, Token t4)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);

	return IgnoreUntilBase(tokens);
}

// ignore until or - 5 tokens
inline bool opNode::IgnoreUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);

	return IgnoreUntilBase(tokens);
}

// ignore until or - 6 tokens
inline bool opNode::IgnoreUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);

	return IgnoreUntilBase(tokens);
}

// ignore until or - 7 tokens
inline bool opNode::IgnoreUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);

	return IgnoreUntilBase(tokens);
}

// ignore until or - 8 tokens
inline bool opNode::IgnoreUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);

	return IgnoreUntilBase(tokens);
}

// ignore until or - 9 tokens
inline bool opNode::IgnoreUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);

	return IgnoreUntilBase(tokens);
}

// ignore until or - 10 tokens
inline bool opNode::IgnoreUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);

	return IgnoreUntilBase(tokens);
}
*/

// disallows token t in current context. recursive if specified.
inline void opNode::DisallowBase(Token t, bool bRecursive, opNode* node)
{
	if(!node)
		node = this;

	ResetPosition();

	iterator i   = GetBegin();
	iterator end = GetEnd();

	if (bRecursive)
	{
		while (i != end)
		{
			Token tok = i->GetId();

			if (i->IsTerminal())
			{
				if (t == tok)
				{
					//opError::SetCurrentContext(t);
					//TODO: may need further improvement (friendly name)
					AlterContext newcontext(node->GetId(),node);
					opError::DisallowError(*i);
				}
			}
			else
				i->DisallowBase(t, bRecursive, *i);

			++i;
		}
	}
	else
	{
		while (i != end)
		{
			Token tok = i->GetId();

			if (t == tok)
			{
				AlterContext newcontext(node->GetId(),node);
				opError::DisallowError(*i);
			}

			++i;
		}
	}

	ResetPosition();
}

// PushUntil - pushes until iterator end
template<class T> 
stacked<T> opNode::PushUntil(iterator end)
{
	stacked<T> newNode = NEWNODE(T());
	iterator   start   = GetPosition();

	SetPosition(end);

	newNode->CopyBasics(*start);

	newNode->MoveNodes(newNode->GetBegin(), this, start, end);

	return newNode;
}

// Adds nodes to the addto node until the specified
// token t is reached.  If there is an error it throws
// an exception
inline opNode* opNode::PushUntilBase(opNode* addto, const vector<Token> &tokens, bool bErrorOnMissing)
{
	Token tok = T_UNKNOWN;
	vector<Token>::const_iterator t_start = tokens.begin();
	vector<Token>::const_iterator t_end   = tokens.end();
	iterator                      end     = GetEnd();

	opNode* first = NULL; 
	if(GetPosition() != end)
	{
		first = CurrentNode();
		//copy the line from the first token?
		addto->CopyBasics(first);	
	}
	else
		addto->CopyBasics(this);

	while (GetPosition() != end)
	{
		tok = CurrentNode()->GetId();

		if (find(t_start,t_end,tok) == t_end)
		{
			addto->AppendNode(*PopCurrentNode());
		}
		else
		{
			return addto;
		}
	}

	if (bErrorOnMissing)
	{
		//NOTE: this error is a bit weird sometimes
		opNode* previous = NULL;
		if(!IsEmpty())
			previous = PreviousNode();
		
		//delete addto;
		//if this errors it's going to throw an exception
		//or else it will return a valid type
		opError::PrematureError(tokens, first?first:previous?previous:this, tok == T_EOF);
		return NULL;
	}

	return addto;
}

// pushuntil for 1 token
inline void opNode::PushUntilAdd(opNode* addto, Token t, bool bErrorOnMissing)
{
	vector<Token> tokens;
	tokens.push_back(t);

	PushUntilBase(addto,tokens,bErrorOnMissing);
}

// pushuntilor - nongeneration version - 2 tokens
inline void opNode::PushUntilOrAdd(opNode* addto, Token t1, Token t2, bool bErrorOnMissing)
{
	vector<Token> tokens;
	tokens.push_back(t1);
	tokens.push_back(t2);

	PushUntilBase(addto,tokens,bErrorOnMissing);
}

// pushuntilor - nongeneration version - 3 tokens
inline void opNode::PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, bool bErrorOnMissing)
{
	vector<Token> tokens;
	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);

	PushUntilBase(addto,tokens,bErrorOnMissing);
}

// pushuntilor - nongeneration version - 4 tokens
inline void opNode::PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, bool bErrorOnMissing)
{
	vector<Token> tokens;
	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);

	PushUntilBase(addto,tokens,bErrorOnMissing);
}

// pushuntilor - nongeneration version - 5 tokens
inline void opNode::PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, Token t5, bool bErrorOnMissing)
{
	vector<Token> tokens;
	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);

	PushUntilBase(addto,tokens,bErrorOnMissing);
}

// pushuntilor - nongeneration version - 6 tokens
inline void opNode::PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, bool bErrorOnMissing)
{
	vector<Token> tokens;
	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);

	PushUntilBase(addto,tokens,bErrorOnMissing);
}

// pushuntilor - nongeneration version - 7 tokens
inline void opNode::PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, bool bErrorOnMissing)
{
	vector<Token> tokens;
	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);

	PushUntilBase(addto,tokens,bErrorOnMissing);
}

// pushuntilor - nongeneration version - 8 tokens
inline void opNode::PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, bool bErrorOnMissing)
{
	vector<Token> tokens;
	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);

	PushUntilBase(addto,tokens,bErrorOnMissing);
}

// pushuntilor - nongeneration version - 9 tokens
inline void opNode::PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, bool bErrorOnMissing)
{
	vector<Token> tokens;
	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);

	PushUntilBase(addto,tokens,bErrorOnMissing);
}

// pushuntilor - nongeneration version - 10 tokens
inline void opNode::PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, bool bErrorOnMissing)
{
	vector<Token> tokens;
	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);

	PushUntilBase(addto,tokens,bErrorOnMissing);
}

inline void opNode::PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, bool bErrorOnMissing)
{
	vector<Token> tokens;
	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);

	PushUntilBase(addto,tokens,bErrorOnMissing);
}

inline void opNode::PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, bool bErrorOnMissing)
{
	vector<Token> tokens;
	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);

	PushUntilBase(addto,tokens,bErrorOnMissing);
}

inline void opNode::PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, bool bErrorOnMissing)
{
	vector<Token> tokens;
	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);
	tokens.push_back(t13);

	PushUntilBase(addto,tokens,bErrorOnMissing);
}

inline void opNode::PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, bool bErrorOnMissing)
{
	vector<Token> tokens;
	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);
	tokens.push_back(t13);
	tokens.push_back(t14);

	PushUntilBase(addto,tokens,bErrorOnMissing);
}

inline void opNode::PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15, bool bErrorOnMissing)
{
	vector<Token> tokens;
	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);
	tokens.push_back(t13);
	tokens.push_back(t14);
	tokens.push_back(t15);

	PushUntilBase(addto,tokens,bErrorOnMissing);
}

// pushuntil for 1 token - generation version
template<class T> 
inline stacked<T> opNode::PushUntil(Token t, bool bErrorOnMissing)
{
	stacked<T>    newNode = NEWNODE(T());
	vector<Token> tokens;

	tokens.push_back(t);

	PushUntilBase(*newNode, tokens, bErrorOnMissing);

	return newNode;
}

// pushuntilor - generation version - 2 tokens
template<class T> 
inline stacked<T> opNode::PushUntilOr(Token t1, Token t2, bool bErrorOnMissing)
{
	stacked<T>    newNode = NEWNODE(T());
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);

	PushUntilBase(*newNode, tokens, bErrorOnMissing);

	return newNode;
}

// pushuntilor - generation version - 3 tokens
template<class T> 
inline stacked<T> opNode::PushUntilOr(Token t1, Token t2, Token t3, bool bErrorOnMissing)
{
	stacked<T>    newNode = NEWNODE(T());
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);

	PushUntilBase(*newNode, tokens, bErrorOnMissing);

	return newNode;
}

// pushuntilor - generation version - 4 tokens
template<class T> 
inline stacked<T> opNode::PushUntilOr(Token t1, Token t2, Token t3, Token t4, bool bErrorOnMissing)
{
	stacked<T>    newNode = NEWNODE(T());
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);

	PushUntilBase(*newNode, tokens, bErrorOnMissing);

	return newNode;
}

// pushuntilor - generation version - 5 tokens
template<class T> 
inline stacked<T> opNode::PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, bool bErrorOnMissing)
{
	stacked<T>    newNode = NEWNODE(T());
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);

	PushUntilBase(*newNode, tokens, bErrorOnMissing);

	return newNode;
}

// pushuntilor - generation version - 6 tokens
template<class T> 
inline stacked<T> opNode::PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, bool bErrorOnMissing)
{
	stacked<T>    newNode = NEWNODE(T());
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);

	PushUntilBase(*newNode, tokens, bErrorOnMissing);

	return newNode;
}

// pushuntilor - generation version - 7 tokens
template<class T> 
inline stacked<T> opNode::PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, bool bErrorOnMissing)
{
	stacked<T>    newNode = NEWNODE(T());
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);

	PushUntilBase(*newNode, tokens, bErrorOnMissing);

	return newNode;
}

// pushuntilor - generation version - 8 tokens
template<class T> 
inline stacked<T> opNode::PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, bool bErrorOnMissing)
{
	stacked<T>    newNode = NEWNODE(T());
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);

	PushUntilBase(*newNode, tokens, bErrorOnMissing);

	return newNode;
}

// pushuntilor - generation version - 9 tokens
template<class T> 
inline stacked<T> opNode::PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, bool bErrorOnMissing)
{
	stacked<T>    newNode = NEWNODE(T());
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);

	PushUntilBase(*newNode, tokens, bErrorOnMissing);

	return newNode;
}

// pushuntilor - generation version - 10 tokens
template<class T> 
inline stacked<T> opNode::PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, bool bErrorOnMissing)
{
	stacked<T>    newNode = NEWNODE(T());
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);

	PushUntilBase(*newNode, tokens, bErrorOnMissing);

	return newNode;
}

template<class T> 
inline stacked<T> opNode::PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, bool bErrorOnMissing)
{
	stacked<T>    newNode = NEWNODE(T());
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);

	PushUntilBase(*newNode, tokens, bErrorOnMissing);

	return newNode;
}

template<class T> 
inline stacked<T> opNode::PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, bool bErrorOnMissing)
{
	stacked<T>    newNode = NEWNODE(T());
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);

	PushUntilBase(*newNode, tokens, bErrorOnMissing);

	return newNode;
}

template<class T> 
inline stacked<T> opNode::PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, bool bErrorOnMissing)
{
	stacked<T>    newNode = NEWNODE(T());
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);
	tokens.push_back(t13);

	PushUntilBase(*newNode, tokens, bErrorOnMissing);

	return newNode;
}

template<class T> 
inline stacked<T> opNode::PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, bool bErrorOnMissing)
{
	stacked<T>    newNode = NEWNODE(T());
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);
	tokens.push_back(t13);
	tokens.push_back(t14);

	PushUntilBase(*newNode, tokens, bErrorOnMissing);

	return newNode;
}

template<class T> 
inline stacked<T> opNode::PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15, bool bErrorOnMissing)
{
	stacked<T>    newNode = NEWNODE(T());
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);
	tokens.push_back(t13);
	tokens.push_back(t14);
	tokens.push_back(t15);

	PushUntilBase(*newNode, tokens, bErrorOnMissing);

	return newNode;
}

template<class T> 
inline stacked<T> opNode::PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15, Token t16, bool bErrorOnMissing)
{
	stacked<T>    newNode = NEWNODE(T());
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);
	tokens.push_back(t13);
	tokens.push_back(t14);
	tokens.push_back(t15);
	tokens.push_back(t16);

	PushUntilBase(*newNode, tokens, bErrorOnMissing);

	return newNode;
}

template<class T> 
inline stacked<T> opNode::PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15, Token t16, Token t17, bool bErrorOnMissing)
{
	stacked<T>    newNode = NEWNODE(T());
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);
	tokens.push_back(t13);
	tokens.push_back(t14);
	tokens.push_back(t15);
	tokens.push_back(t16);
	tokens.push_back(t17);

	PushUntilBase(*newNode, tokens, bErrorOnMissing);

	return newNode;
}

template<class T> 
inline stacked<T> opNode::PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15, Token t16, Token t17, Token t18, bool bErrorOnMissing)
{
	stacked<T>    newNode = NEWNODE(T());
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);
	tokens.push_back(t13);
	tokens.push_back(t14);
	tokens.push_back(t15);
	tokens.push_back(t16);
	tokens.push_back(t17);
	tokens.push_back(t18);

	PushUntilBase(*newNode, tokens, bErrorOnMissing);

	return newNode;
}

template<class T> 
inline stacked<T> opNode::PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15, Token t16, Token t17, Token t18, Token t19, bool bErrorOnMissing)
{
	stacked<T>    newNode = NEWNODE(T());
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);
	tokens.push_back(t13);
	tokens.push_back(t14);
	tokens.push_back(t15);
	tokens.push_back(t16);
	tokens.push_back(t17);
	tokens.push_back(t18);
	tokens.push_back(t19);

	PushUntilBase(*newNode, tokens, bErrorOnMissing);

	return newNode;
}

template<class T> 
inline stacked<T> opNode::PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15, Token t16, Token t17, Token t18, Token t19, Token t20, bool bErrorOnMissing)
{
	stacked<T>    newNode = NEWNODE(T());
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);
	tokens.push_back(t13);
	tokens.push_back(t14);
	tokens.push_back(t15);
	tokens.push_back(t16);
	tokens.push_back(t17);
	tokens.push_back(t18);
	tokens.push_back(t19);
	tokens.push_back(t20);

	PushUntilBase(*newNode, tokens, bErrorOnMissing);

	return newNode;
}

// push until end base
template<class T> 
inline stacked<T> opNode::PushUntilEndBase()
{
	stacked<T> newNode = NEWNODE(T());

	iterator i   = GetPosition();
	iterator end = GetEnd();

	if (i != end)
	{
		newNode->CopyBasics(*i);

		SetPosition(end);

		newNode->MoveNodes(newNode->GetBegin(), this, i, end);
	}
	else
		newNode->CopyBasics(this);

	return newNode;
}

// push until end
template<class T> 
inline stacked<T> opNode::PushUntilEnd()
{
	return PushUntilEndBase<T>();
}

// push until end - specialized for opnode
inline stacked<opNode> opNode::PushUntilEnd()
{
	return PushUntilEndBase<opNode>();
}

// takes the children of one node and makes them
// children of a new node of type T
template<class T,class N>
inline stacked<T> opNode::Transform(stacked<N>& node)
{
	stacked<T> newNode = NEWNODE(T());

	newNode->SetParent(node->GetParent());
	newNode->CopyBasics(*node);
	newNode->CollapseNode(node, newNode->GetBegin());

	return newNode;
}

// expect base
// expect token t..throw an error if not found
template<class T, bool bRemoveNode>
inline stacked<T> opNode::ExpectBase(const vector<Token>& Tokens, bool bReversed, opNode* after)
{
	if (!bReversed)
	{
		iterator end = GetEnd();
		Token    tok;

		if (GetPosition() != end)
			tok = CurrentNode()->GetId();
		else
		{
			opNode* previous = PreviousNode();
			opError::PrematureError(Tokens, previous?previous:this, false);
		}

		if (tok == T_EOF)
			opError::PrematureError(Tokens, CurrentNode(), true);
		else if (find(Tokens.begin(), Tokens.end(), tok) == Tokens.end())
			opError::ExpectError(Tokens,CurrentNode());

		T* newNode = (T*) CurrentNode();

		if (bRemoveNode)
			RemoveCurrentNode();
		else
			IncrementPosition();

		return stacked<T>(newNode);
	}
	else
	{
		iterator current = GetPosition();
		Token    tok;
		iterator end = GetEnd();

		//if its empty
		if (IsEmpty())
		{
			// reached premature start of file (it can get here, rarely)
			opError::MissingError(Tokens,this);
		}
		else if (current == GetBegin())
		{
			// error here -- premature beginning of file ??
			// can't walk backwards to check, theres nothing there.
			opError::ReverseExpectError(Tokens,NULL,*current);
		}

		//TODO: fixme! this doesnt work???
		iterator previous = current;
		--previous;

		tok = previous->GetId();

		// If the token we're reverse expect-or-ing is not in the
		// list of valid tokens, throw an error.
		if (find(Tokens.begin(), Tokens.end(), tok) == Tokens.end())
		{
			opNode* node;

			if (GetPosition() == GetEnd())
				node = NULL;
			else
				node = *GetPosition();
			
			// reverse expect error
			opError::ReverseExpectError(Tokens,*previous, after ? after : node);
		}

		T* newNode = (T*) *previous;

		if (bRemoveNode)
		{
			DecrementPosition();//should be safe now.		
			RemoveCurrentNode();
		}
		else
		{
			current = current;//??
		}

		return stacked<T>(newNode);
	}
}

// expect or - 2 tokens
template<class T>
inline stacked<T> opNode::ExpectOr(Token t1, Token t2)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);

	return ExpectBase<T,true>(tokens);

}

// expect or - 3 tokens
template<class T>
inline stacked<T> opNode::ExpectOr(Token t1, Token t2, Token t3)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);

	return ExpectBase<T,true>(tokens);

}

// expect or - 4 tokens
template<class T>
inline stacked<T> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);

	return ExpectBase<T,true>(tokens);

}

// expect or - 5 tokens
template<class T>
inline stacked<T> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);

	return ExpectBase<T,true>(tokens);

}

// expect or - 6 tokens
template<class T>
inline stacked<T> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);

	return ExpectBase<T,true>(tokens);

}

// expect or - 7 tokens
template<class T>
inline stacked<T> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);

	return ExpectBase<T,true>(tokens);

}

// expect or - 8 tokens
template<class T>
inline stacked<T> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);

	return ExpectBase<T,true>(tokens);

}

// expect or - 9 tokens
template<class T>
inline stacked<T> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);

	return ExpectBase<T,true>(tokens);

}

// expect or - 10 tokens
template<class T>
inline stacked<T> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10)
{
	vector<Token> tokens;
	
	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	
	return ExpectBase<T,true>(tokens);
}

template<class T> 
inline stacked<T> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);

	return ExpectBase<T,true>(tokens);
}

template<class T> 
inline stacked<T> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);

	return ExpectBase<T,true>(tokens);
}

template<class T> 
inline stacked<T> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);
	tokens.push_back(t13);

	return ExpectBase<T,true>(tokens);
}

template<class T> 
inline stacked<T> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);
	tokens.push_back(t13);
	tokens.push_back(t14);

	return ExpectBase<T,true>(tokens);
}

template<class T> 
inline stacked<T> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);
	tokens.push_back(t13);
	tokens.push_back(t14);
	tokens.push_back(t15);

	return ExpectBase<T,true>(tokens);
}

// expect or - non-template 2 tokens
inline stacked<opNode> opNode::ExpectOr(Token t1, Token t2)
{
	return opNode::ExpectOr<opNode>(t1,t2);
}

// expect or - non-template 3 tokens
inline stacked<opNode> opNode::ExpectOr(Token t1, Token t2, Token t3)
{
	return opNode::ExpectOr<opNode>(t1,t2,t3);
}

// expect or - non-template 4 tokens
inline stacked<opNode> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4)
{
	return opNode::ExpectOr<opNode>(t1,t2,t3,t4);
}

// expect or - non-template 5 tokens
inline stacked<opNode> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5)
{
	return opNode::ExpectOr<opNode>(t1,t2,t3,t4,t5);
}

// expect or - non-template 6 tokens
inline stacked<opNode> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6)
{
	return opNode::ExpectOr<opNode>(t1,t2,t3,t4,t5,t6);
}

// expect or - non-template 7 tokens
inline stacked<opNode> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7)
{
	return opNode::ExpectOr<opNode>(t1,t2,t3,t4,t5,t6,t7);
}

// expect or - non-template 8 tokens
inline stacked<opNode> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8)
{
	return opNode::ExpectOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8);
}

// expect or - non-template 9 tokens
inline stacked<opNode> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9)
{
	return opNode::ExpectOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8,t9);
}

// expect or - non-template 10 tokens
inline stacked<opNode> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10)
{
	return opNode::ExpectOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10);
}

inline stacked<opNode> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11)
{
	return opNode::ExpectOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11);
}

inline stacked<opNode> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12)
{
	return opNode::ExpectOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12);
}

inline stacked<opNode> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13)
{
	return opNode::ExpectOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13);
}

inline stacked<opNode> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14)
{
	return opNode::ExpectOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14);
}

inline stacked<opNode> opNode::ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15)
{
	return opNode::ExpectOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15);
}

// reverse expect or - 2 tokens
template<class T>
inline stacked<T> opNode::ReverseExpectOr(Token t1, Token t2, opNode* after)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);

	return ExpectBase<T,true>(tokens,true,after);

}

// reverse expect or - 3 tokens
template<class T>
inline stacked<T> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, opNode* after)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);

	return ExpectBase<T,true>(tokens,true,after);

}

// reverse expect or - 4 tokens
template<class T>
inline stacked<T> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, opNode* after)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);

	return ExpectBase<T,true>(tokens,true,after);

}

// reverse expect or - 5 tokens
template<class T>
inline stacked<T> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, opNode* after)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);

	return ExpectBase<T,true>(tokens,true,after);

}

// reverse expect or - 6 tokens
template<class T>
inline stacked<T> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, opNode* after)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);

	return ExpectBase<T,true>(tokens,true,after);

}

// reverse expect or - 7 tokens
template<class T>
inline stacked<T> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, opNode* after)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);

	return ExpectBase<T,true>(tokens,true,after);

}

// reverse expect or - 8 tokens
template<class T>
inline stacked<T> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, opNode* after)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);

	return ExpectBase<T,true>(tokens,true,after);

}

// reverse expect or - 9 tokens
template<class T>
inline stacked<T> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, opNode* after)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);

	return ExpectBase<T,true>(tokens,true,after);

}

// reverse expect or - 10 tokens
template<class T>
inline stacked<T> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, opNode* after)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);

	return ExpectBase<T,true>(tokens,true,after);
}

// reverse expect or - 11 tokens
template<class T> 
inline stacked<T> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, opNode* after)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);

	return ExpectBase<T,true>(tokens,true,after);
}

// reverse expect or - 12 tokens
template<class T> 
inline stacked<T> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, opNode* after)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);

	return ExpectBase<T,true>(tokens,true,after);
}

// reverse expect or - 13 tokens
template<class T> 
inline stacked<T> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, opNode* after)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);
	tokens.push_back(t13);

	return ExpectBase<T,true>(tokens,true,after);
}

// reverse expect or - 14 tokens
template<class T> 
inline stacked<T> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, opNode* after)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);
	tokens.push_back(t13);
	tokens.push_back(t14);

	return ExpectBase<T,true>(tokens,true,after);
}

// reverse expect or - 15 tokens
template<class T> 
inline stacked<T> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15, opNode* after)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);
	tokens.push_back(t13);
	tokens.push_back(t14);
	tokens.push_back(t15);

	return ExpectBase<T,true>(tokens,true,after);
}

// reverse expect or - non-template 2 tokens
inline stacked<opNode> opNode::ReverseExpectOr(Token t1, Token t2, opNode* after)
{
	return opNode::ReverseExpectOr<opNode>(t1,t2,after);
}

// reverse expect or - non-template 3 tokens
inline stacked<opNode> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, opNode* after)
{
	return opNode::ReverseExpectOr<opNode>(t1,t2,t3,after);
}

// reverse expect or - non-template 4 tokens
inline stacked<opNode> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, opNode* after)
{
	return opNode::ReverseExpectOr<opNode>(t1,t2,t3,t4,after);
}

// reverse expect or - non-template 5 tokens
inline stacked<opNode> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, opNode* after)
{
	return opNode::ReverseExpectOr<opNode>(t1,t2,t3,t4,t5,after);
}

// reverse expect or - non-template 6 tokens
inline stacked<opNode> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, opNode* after)
{
	return opNode::ReverseExpectOr<opNode>(t1,t2,t3,t4,t5,t6,after);
}

// reverse expect or - non-template 7 tokens
inline stacked<opNode> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, opNode* after)
{
	return opNode::ReverseExpectOr<opNode>(t1,t2,t3,t4,t5,t6,t7,after);
}

// reverse expect or - non-template 8 tokens
inline stacked<opNode> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, opNode* after)
{
	return opNode::ReverseExpectOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8,after);
}

// reverse expect or - non-template 9 tokens
inline stacked<opNode> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, opNode* after)
{
	return opNode::ReverseExpectOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8,t9,after);
}

// reverse expect or - non-template 10 tokens
inline stacked<opNode> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, opNode* after)
{
	return opNode::ReverseExpectOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,after);
}

// reverse expect or - non-template 11 tokens
inline stacked<opNode> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, opNode* after)
{
	return opNode::ReverseExpectOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,after);
}

// reverse expect or - non-template 12 tokens
inline stacked<opNode> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, opNode* after)
{
	return opNode::ReverseExpectOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,after);
}

// reverse expect or - non-template 13 tokens
inline stacked<opNode> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, opNode* after)
{
	return opNode::ReverseExpectOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,after);
}

// reverse expect or - non-template 14 tokens
inline stacked<opNode> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, opNode* after)
{
	return opNode::ReverseExpectOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,after);
}

// reverse expect or - non-template 15 tokens
inline stacked<opNode> opNode::ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15, opNode* after)
{
	return opNode::ReverseExpectOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,after);
}

// special Expect for overloadable operators
//TODO: this needs to be redone!
inline opNode* opNode::CheckOverloadableOperator()
{
	if (GetPosition() == GetEnd() || CurrentNode()->GetId() == T_EOF)
	{
		opError::PrematureError(this, CurrentNode()->GetId() == T_EOF);
	}
	else if (!CurrentNode()->IsOverloadableOperator())
		opError::ExpectError("Overloadable Operator", this);

	opNode* newNode = CurrentNode();

	IncrementPosition();

	return newNode;
}

// check or - 2 tokens
template<class T>
inline T* opNode::CheckOr(Token t1, Token t2)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);

	return *ExpectBase<T,false>(tokens);

}

// check or - 3 tokens
template<class T>
inline T* opNode::CheckOr(Token t1, Token t2, Token t3)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);

	return *ExpectBase<T,false>(tokens);

}

// check or - 4 tokens
template<class T>
inline T* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);

	return *ExpectBase<T,false>(tokens);

}

// check or - 5 tokens
template<class T>
inline T* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);

	return *ExpectBase<T,false>(tokens);

}

// check or - 6 tokens
template<class T>
inline T* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	
	return *ExpectBase<T,false>(tokens);

}

// check or - 7 tokens
template<class T>
inline T* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);

	return *ExpectBase<T,false>(tokens);

}

// check or - 8 tokens
template<class T>
inline T* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);

	return *ExpectBase<T,false>(tokens);

}

// check or - 9 tokens
template<class T>
inline T* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);

	return *ExpectBase<T,false>(tokens);

}

// check or - 10 tokens
template<class T>
inline T* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);

	return *ExpectBase<T,false>(tokens);
}

template<class T> 
inline T* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);

	return *ExpectBase<T,false>(tokens);
}

template<class T> 
inline T* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);

	return *ExpectBase<T,false>(tokens);
}

template<class T> 
inline T* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);
	tokens.push_back(t13);

	return *ExpectBase<T,false>(tokens);
}

template<class T> 
inline T* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);
	tokens.push_back(t13);
	tokens.push_back(t14);

	return *ExpectBase<T,false>(tokens);
}

template<class T> 
inline T* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15)
{
	vector<Token> tokens;

	tokens.push_back(t1);
	tokens.push_back(t2);
	tokens.push_back(t3);
	tokens.push_back(t4);
	tokens.push_back(t5);
	tokens.push_back(t6);
	tokens.push_back(t7);
	tokens.push_back(t8);
	tokens.push_back(t9);
	tokens.push_back(t10);
	tokens.push_back(t11);
	tokens.push_back(t12);
	tokens.push_back(t13);
	tokens.push_back(t14);
	tokens.push_back(t15);

	return *ExpectBase<T,false>(tokens);
}

// check or - non-template 2 tokens
inline opNode* opNode::CheckOr(Token t1, Token t2)
{
	return CheckOr<opNode>(t1,t2);
}

// check or - non-template 3 tokens
inline opNode* opNode::CheckOr(Token t1, Token t2, Token t3)
{
	return CheckOr<opNode>(t1,t2,t3);
}

// check or - non-template 4 tokens
inline opNode* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4)
{
	return CheckOr<opNode>(t1,t2,t3,t4);
}

// check or - non-template 5 tokens
inline opNode* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5)
{
	return CheckOr<opNode>(t1,t2,t3,t4,t5);
}

// check or - non-template 6 tokens
inline opNode* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6)
{
	return CheckOr<opNode>(t1,t2,t3,t4,t5,t6);
}

// check or - non-template 7 tokens
inline opNode* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7)
{
	return CheckOr<opNode>(t1,t2,t3,t4,t5,t6,t7);
}

// check or - non-template 8 tokens
inline opNode* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8)
{
	return CheckOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8);
}

// check or - non-template 9 tokens
inline opNode* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9)
{
	return CheckOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8,t9);
}

// check or - non-template 10 tokens
inline opNode* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10)
{
	return CheckOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10);
}

inline opNode* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11)
{
	return CheckOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11);
}

inline opNode* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12)
{
	return CheckOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12);
}

inline opNode* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13)
{
	return CheckOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13);
}

inline opNode* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14)
{
	return CheckOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14);
}

inline opNode* opNode::CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15)
{
	return CheckOr<opNode>(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15);
}

// checks to make sure there are no more non-whitespace tokens
// if there are, it throws an exception
inline void opNode::CheckNone()
{
	iterator end = GetEnd();
	Token    tok;

	while (GetPosition() != end)
	{
		tok = CurrentNode()->GetId();

		if (tok != T_WHITESPACE && tok != T_NEWLINE)
			opError::CheckNoneError(CurrentNode());

		DeleteCurrentNode();
	}
}

// make
template<class T>
inline stacked<T> opNode::Make(Token t)
{
	if (GetPosition() == GetEnd())
	{
		opNode* previous = PreviousNode();
		opError::PrematureError(t,previous?previous:this, false);
	}

	Token tok = CurrentNode()->GetId();
	
	if(tok == T_EOF)
		opError::PrematureError(t,CurrentNode(), true);
	
	if (tok != t)
		opError::ExpectError(t,CurrentNode());

	stacked<T> newNode = NEWNODE(T());

	newNode->CopyBasics(CurrentNode());

	return newNode;
} 	

// peek function - allows you to peek at the next token
inline bool opNode::Peek(Token t)
{
	iterator i = GetPosition();

	++i;

	if (i == GetEnd())
		return false;

	return i->GetId() == t;
}

// peek function - allows you to peak at the next token
// NOTE: Ignores whitespace!
inline bool opNode::PeekUncleaned(Token t)
{
	iterator i   = GetPosition();
	iterator end = GetEnd();

	++i;

	while (i != end)
	{
		opNode* current = *i;

		if ( !current->IsWhitespace()
		&&   !current->IsComment() )
		{
			break;
		}

		++i;
	}

	if (i == GetEnd())
		return false;

	return i->GetId() == t;
}

// peek start
inline bool opNode::PeekStart(Token t)
{
	iterator i = GetBegin();

	if (i == GetEnd())
		return false;

	return i->GetId() == t;
}

// peek end
inline bool opNode::PeekEnd(Token t)
{
	iterator i = GetLast();

	if (i == GetEnd())
		return false;

	return i->GetId() == t;
}

// allow only - base method
inline void opNode::AllowOnlyBase(const vector<Token>& Tokens)
{
	ResetPosition();

	iterator end = GetEnd();

	while (GetPosition() != end)
	{
		Token t = GetPosition()->GetId();

		if (!TokenFunctions::IsWhitespace(t) && !TokenFunctions::IsComment(t) && t != T_EOF)
		{
			vector<Token>::const_iterator iter = 
				find(Tokens.begin(), Tokens.end(), t);

			if (iter == Tokens.end())
			{
				//opError::SetCurrentContext(t);
				opError::DisallowError(*GetPosition());
			}
		}

		IncrementPosition();
	}

	ResetPosition();
}

// allow only - 1 token
inline void opNode::AllowOnly(Token t)
{
	vector<Token> Tokens;

	Tokens.push_back(t);

	AllowOnlyBase(Tokens);
}

// allow only - 2 tokens
inline void opNode::AllowOnly(Token t1, Token t2)
{
	vector<Token> Tokens;

	Tokens.push_back(t1);
	Tokens.push_back(t2);

	AllowOnlyBase(Tokens);
}

// allow only - 3 tokens
inline void opNode::AllowOnly(Token t1, Token t2, Token t3)
{
	vector<Token> Tokens;

	Tokens.push_back(t1);
	Tokens.push_back(t2);
	Tokens.push_back(t3);

	AllowOnlyBase(Tokens);
}

// allow only - 4 tokens
inline void opNode::AllowOnly(Token t1, Token t2, Token t3, Token t4)
{
	vector<Token> Tokens;

	Tokens.push_back(t1);
	Tokens.push_back(t2);
	Tokens.push_back(t3);
	Tokens.push_back(t4);

	AllowOnlyBase(Tokens);
}

// allow only - 5 tokens
inline void opNode::AllowOnly(Token t1, Token t2, Token t3, Token t4, Token t5)
{
	vector<Token> Tokens;

	Tokens.push_back(t1);
	Tokens.push_back(t2);
	Tokens.push_back(t3);
	Tokens.push_back(t4);
	Tokens.push_back(t5);

	AllowOnlyBase(Tokens);
}

// allow only - 6 tokens
inline void opNode::AllowOnly(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6)
{
	vector<Token> Tokens;

	Tokens.push_back(t1);
	Tokens.push_back(t2);
	Tokens.push_back(t3);
	Tokens.push_back(t4);
	Tokens.push_back(t5);
	Tokens.push_back(t6);

	AllowOnlyBase(Tokens);
}

// allow only - 7 tokens
inline void opNode::AllowOnly(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7)
{
	vector<Token> Tokens;

	Tokens.push_back(t1);
	Tokens.push_back(t2);
	Tokens.push_back(t3);
	Tokens.push_back(t4);
	Tokens.push_back(t5);
	Tokens.push_back(t6);
	Tokens.push_back(t7);

	AllowOnlyBase(Tokens);
}

// allow only - 8 tokens
inline void opNode::AllowOnly(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8)
{
	vector<Token> Tokens;

	Tokens.push_back(t1);
	Tokens.push_back(t2);
	Tokens.push_back(t3);
	Tokens.push_back(t4);
	Tokens.push_back(t5);
	Tokens.push_back(t6);
	Tokens.push_back(t7);
	Tokens.push_back(t8);

	AllowOnlyBase(Tokens);
}

// allow only - 9 tokens
inline void opNode::AllowOnly(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9)
{
	vector<Token> Tokens;

	Tokens.push_back(t1);
	Tokens.push_back(t2);
	Tokens.push_back(t3);
	Tokens.push_back(t4);
	Tokens.push_back(t5);
	Tokens.push_back(t6);
	Tokens.push_back(t7);
	Tokens.push_back(t8);
	Tokens.push_back(t9);

	AllowOnlyBase(Tokens);
}

// allow only - 10 tokens
inline void opNode::AllowOnly(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10)
{
	vector<Token> Tokens;

	Tokens.push_back(t1);
	Tokens.push_back(t2);
	Tokens.push_back(t3);
	Tokens.push_back(t4);
	Tokens.push_back(t5);
	Tokens.push_back(t6);
	Tokens.push_back(t7);
	Tokens.push_back(t8);
	Tokens.push_back(t9);
	Tokens.push_back(t10);

	AllowOnlyBase(Tokens);
}

// allow only - 11 tokens
inline void opNode::AllowOnly(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, 
							  Token t7, Token t8, Token t9, Token t10, Token t11)
{
	vector<Token> Tokens;

	Tokens.push_back(t1);
	Tokens.push_back(t2);
	Tokens.push_back(t3);
	Tokens.push_back(t4);
	Tokens.push_back(t5);
	Tokens.push_back(t6);
	Tokens.push_back(t7);
	Tokens.push_back(t8);
	Tokens.push_back(t9);
	Tokens.push_back(t10);
	Tokens.push_back(t11);

	AllowOnlyBase(Tokens);
}

// allow only - 12 tokens
inline void opNode::AllowOnly(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, 
							  Token t7, Token t8, Token t9, Token t10, Token t11, Token t12)
{
	vector<Token> Tokens;

	Tokens.push_back(t1);
	Tokens.push_back(t2);
	Tokens.push_back(t3);
	Tokens.push_back(t4);
	Tokens.push_back(t5);
	Tokens.push_back(t6);
	Tokens.push_back(t7);
	Tokens.push_back(t8);
	Tokens.push_back(t9);
	Tokens.push_back(t10);
	Tokens.push_back(t11);
	Tokens.push_back(t12);

	AllowOnlyBase(Tokens);
}

inline void opNode::AllowOnly(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13)
{
	vector<Token> Tokens;

	Tokens.push_back(t1);
	Tokens.push_back(t2);
	Tokens.push_back(t3);
	Tokens.push_back(t4);
	Tokens.push_back(t5);
	Tokens.push_back(t6);
	Tokens.push_back(t7);
	Tokens.push_back(t8);
	Tokens.push_back(t9);
	Tokens.push_back(t10);
	Tokens.push_back(t11);
	Tokens.push_back(t12);
	Tokens.push_back(t13);

	AllowOnlyBase(Tokens);
}

inline void opNode::AllowOnly(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14)
{
	vector<Token> Tokens;

	Tokens.push_back(t1);
	Tokens.push_back(t2);
	Tokens.push_back(t3);
	Tokens.push_back(t4);
	Tokens.push_back(t5);
	Tokens.push_back(t6);
	Tokens.push_back(t7);
	Tokens.push_back(t8);
	Tokens.push_back(t9);
	Tokens.push_back(t10);
	Tokens.push_back(t11);
	Tokens.push_back(t12);
	Tokens.push_back(t13);
	Tokens.push_back(t14);

	AllowOnlyBase(Tokens);
}

inline void opNode::AllowOnly(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15)
{
	vector<Token> Tokens;

	Tokens.push_back(t1);
	Tokens.push_back(t2);
	Tokens.push_back(t3);
	Tokens.push_back(t4);
	Tokens.push_back(t5);
	Tokens.push_back(t6);
	Tokens.push_back(t7);
	Tokens.push_back(t8);
	Tokens.push_back(t9);
	Tokens.push_back(t10);
	Tokens.push_back(t11);
	Tokens.push_back(t12);
	Tokens.push_back(t13);
	Tokens.push_back(t14);
	Tokens.push_back(t15);

	AllowOnlyBase(Tokens);
}

// Only one of the two types is allowed.
inline void opNode::DisallowBoth(Token t1, Token t2)
{
	opNode*  node1;
	opNode*  node2;
	opNode*  firstfound;
	iterator i   = GetBegin();
	iterator end = GetEnd();

	node1 = node2 = firstfound = NULL;

	while (i != end)
	{
		Token t = i->GetId();

		if (t == t1)
		{
			node1 = *i;

			if ( !firstfound )
				firstfound = node1;
		}
		else if (t == t2)
		{
			node2 = *i;

			if ( !firstfound )
				firstfound = node2;
		}

		// check for mutual exclusion..
		if (node1 && node2)
		{
			if (firstfound == node1)
				opError::MutualExclusionError(node1, node2);
			else
				opError::MutualExclusionError(node2, node1);
		}

		++i;
	}

	ResetPosition();
}

template<class T>
inline T* opNode::FindParent()
{
	opNode* parent = GetParent();
	while(parent)
	{
		if(T* node = fast_node_cast<T>(parent))
			return node;
		
		parent = parent->GetParent();
	}
	
	return NULL;
}

template<class T>
inline T* opNode::FindNode()
{
	iterator i = GetBegin();
	iterator end = GetEnd();

	while(i != end)
	{
		if(T* node = fast_node_cast<T>(*i))
			return node;

		++i;
	}

	return NULL;
}

///
/// FileNode
///

template<class T>
inline T* FileNode::Load(const opString& file, opScanner::ScanMode scanmode, bool bIncluded)
{
	//std::ifstream  ifs;
	opScanner scanner;
	T*        rootNode = NULL;
	
	//try to get the node from the loaded file table if possible
	//NOTE: disabled caching for now
	rootNode = NULL;//(T*)GetLoadedFile(file);
	
	if(rootNode)
		return rootNode;
	
	double totaltimestart = opTimer::GetTimeSeconds();
	
	// try to open the file
	//ifs.open(file);
	FileReadStream ifs(file);
	
	rootNode = *NEWNODE(T());
	
	//fix up the inputname
	path filepath = file.GetString();
	//FIXME: filepath.normalize();
	rootNode->InputName = to_relative_path(filepath).string();
	
	path inputpath = rootNode->InputName.GetString();
	rootNode->bAbsolutePath = inputpath.is_absolute();
	
	//TODO: probably not correct - this is really ugly too.
	if(!rootNode->bAbsolutePath)
	{
		path abspath = std::filesystem::current_path() / rootNode->InputName.GetString();
		//FIXME: abspath.normalize();

		rootNode->AbsoluteFileName = abspath.string();
	}
	else
		rootNode->AbsoluteFileName = rootNode->InputName;

	//add it to the loaded file table
	if(!bIncluded)
		FileTable.push_back(rootNode);
	
	if (!ifs.IsOpen())
	{
		return rootNode;
	}
	
	// now scan with timing
	double scantimestart = opTimer::GetTimeSeconds();
	bool bScanError = !scanner.Scan(ifs, scanmode, rootNode);
	double scantimeend = opTimer::GetTimeSeconds();
	
	rootNode->scanMs = (scantimeend - scantimestart)*1000.0;
	
	//scanner error? end this.
	if(bScanError)
	{
		return rootNode;
	}
	
	scanner.AddTokensToRoot(rootNode);
	
	double parsetimestart = opTimer::GetTimeSeconds();
	
	// preprocessor
	if ( !rootNode->Preprocessor() )
		return rootNode;
	
	// pre process
	if ( !rootNode->PreProcess() )	
		return rootNode;
	
	if ( !bIncluded )
	{
		// pre operations
		if ( !rootNode->PreOperations() )
			return rootNode;
		
		// process
		if ( !rootNode->Process() )
			return rootNode;

		// operations
		if ( !rootNode->Operations() )
			return rootNode;
		
		// post process
		if ( !rootNode->PostProcess() )
			return rootNode;

		// post operations
		if ( !rootNode->PostOperations() )
			return rootNode;
	}
	
	double parsetimeend = opTimer::GetTimeSeconds();
	rootNode->parseMs = (parsetimeend - parsetimestart)*1000.0;

	double totaltimeend = opTimer::GetTimeSeconds();
	rootNode->totalMs = (totaltimeend - totaltimestart)*1000.0;

	//success
	return rootNode;
}

//
// Queries
//

inline void opNode::BuildClassScope(opString& outscope)
{ 
	if(GetParent())
		GetParent()->BuildClassScope(outscope);
}

inline opString opNode::GetClassScope()
{
	opString scope;
	if(GetParent())
		GetParent()->BuildClassScope(scope);
	return scope;
}

//
// Error
//

inline AlterContext::AlterContext(opNode* node)
{
	opError::GetCurrentContext(oldtoken,oldnode);
	opError::SetCurrentContext(node->GetId(),node);
	benabled = true;
}