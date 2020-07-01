///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: ExpandNodes.h
/// Date: 04/19/2007
///
/// Description:
///
/// Expansion Related Nodes
///****************************************************************

namespace nodes
{

///==========================================
/// ExpandCallNode
///==========================================

typedef Trim< ExpandCalls< opNode > > ExpandCallArgumentNodeParent;

class ExpandCallArgumentNode : public ExpandCallArgumentNodeParent
{
public:
	DECLARE_NODE(ExpandCallArgumentNode,ExpandCallArgumentNodeParent,G_EXPAND_CALL_ARGUMENT);

	void CloneNode(opNode* newnode)
	{
		CloneChildren(newnode);
	}

	//this identifies and causes expansion of expand grammars
	bool PreParse();

	opString ErrorName() { return ""; }
};

typedef ListUtility< opNode > ExpandCallArgumentListNodeParent;

class ExpandCallArgumentListNode : public ExpandCallArgumentListNodeParent
{
public:
	DECLARE_NODE(ExpandCallArgumentListNode,ExpandCallArgumentListNodeParent,G_EXPAND_CALL_ARGUMENT_LIST);

	bool PreParse();

	const vector<ExpandCallArgumentNode*>& GetArguments()
	{
		return Arguments;
	}

	opString ErrorName();

private:
	vector<ExpandCallArgumentNode*> Arguments;
};

class ExpandCallNode : public opNode
{
public:
	DECLARE_NODE(ExpandCallNode,opNode,G_EXPAND_CALL);

	void Init()
	{		
		Arguments = NULL;
		Name = NULL;
	}

	void SetName(opNode* inname)
	{
		Name = inname;
	}

	void SetArguments(ExpandCallArgumentListNode* inargs)
	{
		Arguments = inargs;
	}

	opNode* GetName()
	{
		return Name;
	}

	ExpandCallArgumentListNode* GetArguments()
	{
		return Arguments;
	}

	opString ErrorName();

// 	bool PreProcess(opSymbolTracker& tracker);
// 	bool PreParse(opSymbolTracker& tracker);

	//get the opmacro signature we want to match
	opString GetSignature();


protected:
	bool Expand(opSymbolTracker& tracker, opNode::iterator expandit, opNode* parent);

	friend opMacroExpander;

private:
	opNode*			            Name;
	ExpandCallArgumentListNode* Arguments;
	static int                  ExpansionDepth;
};

///
/// ExpandableNode
///

//expandable node should have the following abilities
//must be able to recursively replace token matches w/ replacement nodes

class ExpandableNode : public opNode
{
public:
	DECLARE_NODE(ExpandableNode,opNode,T_UNKNOWN);
	
	/**** operations for expandable nodes ****/
	void ReplaceNodes(const opString& matchname, iterator start, iterator end);
	void ReplaceNodes(const opString& matchname, opNode* node);
	
	void CallExpands();
	void CallOperators();
	
	void PrintOriginal(opSectionStream& stream)
	{
		PrintOriginalUnspacedChildren(stream);
	}

	opString ErrorName() { return ""; }
	
private:
	/**** recursive functions ****/
	void DoReplacement(opNode* currentnode, const opString& matchname, iterator start, iterator end);
	void DoReplacement(opNode* currentnode, const opString& matchname, opNode* replacement);
	
	//TODO: add expand (find and call)
	//		add operator calls
};

//this class is used to wrap expandable arguments, it is collapsed after substitution
class ExpandableArgumentNode : public opNode
{
public:
	DECLARE_NODE(ExpandableArgumentNode,opNode,G_EXPANDABLE_ARGUMENT);
	
	void PrintOriginal(opSectionStream& stream)
	{
		PrintOriginalUnspacedChildren(stream);
	}
	
	void SetValue(const opString& invalue)
	{
		value = invalue;
	}
	
	opString GetTreeValue()
	{
		return value;
	}
	
	void CloneNode(ExpandableArgumentNode* node)
	{
		node->SetValue(value);
		CloneChildren(node);
	}

	opString ErrorName() { return ""; }

private:
	opString value;
};

///==========================================
/// ConcatenationOperatorNode, ConcatenationArgumentOperatorNode
///==========================================

class ConcatenationArgumentOperatorNode : public opNode
{
public:
	DECLARE_NODE(ConcatenationArgumentOperatorNode,opNode,G_CONCATENATION_ARGUMENT_OPERATOR);

	void Init()
	{
		Argument = NULL;
	}

	void SetArgument(opNode* innode)
	{
		Argument = innode;
	}

	opNode* GetArgument()
	{
		return Argument;
	}

	void PrintOriginal(opSectionStream& stream)
	{
		PrintOriginalUnspacedChildren(stream);
	}

	void PrintString(opString& s)
	{
		PrintStringUnspacedChildren(s);
	}

	opString ErrorName()
	{
		opString name;

		PrintStringChildren(name);

		return name;
	}

	void CloneNode(ConcatenationArgumentOperatorNode* node)
	{
		CloneChildren(node);
	}

private:
	opNode* Argument;
};

class ConcatenationOperatorNode : public opNode
{
public:
	DECLARE_NODE(ConcatenationOperatorNode,opNode,G_CONCATENATION_OPERATOR);

	void Init()
	{
		Left  = NULL;
		Right = NULL;
	}

	void SetLeft(ConcatenationArgumentOperatorNode* innode)
	{
		Left = innode;
	}

	void SetRight(ConcatenationArgumentOperatorNode* innode)
	{
		Right = innode;
	}

	ConcatenationArgumentOperatorNode* GetLeft()
	{
		return Left;
	}

	ConcatenationArgumentOperatorNode* GetRight()
	{
		return Right;
	}

	opString ErrorName();
	
	void CloneNode(ConcatenationOperatorNode* node)
	{
		stacked<ConcatenationArgumentOperatorNode> left = Left->Clone();
		stacked<ConcatenationArgumentOperatorNode> right = Right->Clone();

		node->SetLeft(*left);
		node->SetRight(*right);

		node->AppendNode(left);
		node->AppendNode(right);
	}

	void PrintNode(opFileStream& stream)
	{
		PrintOriginal(stream.header.body);
	}

	void PrintOriginal(opSectionStream& stream );
	void PrintString(opString& s);

private:
	ConcatenationArgumentOperatorNode* Left;
	ConcatenationArgumentOperatorNode* Right;
};

///==========================================
/// SingleQuoteOperatorNode
///==========================================

class SingleQuoteOperatorNode : public opNode
{
public:
	DECLARE_NODE(SingleQuoteOperatorNode,opNode,G_SINGLE_QUOTE_OPERATOR);

	opString ErrorName();

	void CloneNode(SingleQuoteOperatorNode* node)
	{
		CloneChildren(node);
	}

	void PrintOriginal(opSectionStream& stream );

	void PrintString(opString& s)
	{
		s += "'";
		PrintStringUnspacedChildren(s);
		s += "'";
	}
};

///==========================================
/// DoubleQuoteOperatorNode
///==========================================

class DoubleQuoteOperatorNode : public opNode
{
public:
	DECLARE_NODE(DoubleQuoteOperatorNode,opNode,G_DOUBLE_QUOTE_OPERATOR);

	void PrintOriginal(opSectionStream& stream);

	void CloneNode(DoubleQuoteOperatorNode* node)
	{
		CloneChildren(node);
	}

	opString ErrorName();

	void PrintString(opString& s)
	{
		s += '"';
		PrintStringUnspacedChildren(s);
		s += '"';
	}
};

} // end of namespace nodes
