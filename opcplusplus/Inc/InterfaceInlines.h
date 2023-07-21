///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: InterfaceInlines.h
/// Date: 03/13/2007
///
/// Description:
///
/// Interface Inlines
///****************************************************************

///
/// DataStatements
///

template<class Parent>
inline bool DataStatements<Parent>::FindDataStatementOnly()
{
	if (!HasNumChildren(2))
		return false;

	stackedcontext<DataStatementNode> newNode = NEWNODE(DataStatementNode());
	newNode->CopyBasics(FirstChild());
	
	stacked<DataDeclarationNode> declaration = opNode::PushUntil<DataDeclarationNode>(T_ASSIGN,false);

	newNode->SetDeclaration(*declaration);
	newNode->AppendNode(declaration);

	if(IsCurrent(T_ASSIGN))
	{
		Erase(T_ASSIGN);
		stacked<DataInitializationNode> initialization = opNode::PushUntilEnd<DataInitializationNode>();
		
		newNode->SetInitialization(*initialization);
		newNode->AppendNode(initialization);
	}
	
	SetInnerStatement(*newNode);
	AppendNode(newNode);
	
	return true;
}

//
// Trim
//


template<class Parent>
bool Trim<Parent>::PreParse()
{
	PREPARSE_START;
	{
		this.DoTrim();
	}
	PREPARSE_END;
}

template<class Parent>
bool Trim<Parent>::Parse()
{
	PARSE_START;
	{
		this.DoTrim();
	}
	PARSE_END;
}

template<class Parent>
void Trim<Parent>::DoTrim()
{
	iterator i = this.GetBegin();
	iterator end = this.GetEnd();

	// Trim the front.
	while (i != end
		&& i->IsWhitespace())
	{
		iterator old = i;
		++i;
		this.DeleteNode(old);
	}

	if (this.IsEmpty())
		return;

	// Trim the end.
	i = this.GetBegin();
	end = --this.GetEnd();

	while (end != i
		&& end->IsWhitespace())
	{
		iterator old = end;
		--end;
		this.DeleteNode(old);
	}
}

