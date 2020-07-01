///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: DialectModifierNodes.h
/// Date: 10/22/2007
///
/// Description:
///
/// File for dialect modifier classes.
///****************************************************************

namespace dialect_nodes
{

///
/// DialectModifiersNode - contains all dialect modifiers
///

typedef DialectModifiers<ModifiersBase> DialectModifiersNodeParent; 

class DialectModifiersNode : public DialectModifiersNodeParent
{
public:
	DECLARE_NODE(DialectModifiersNode,DialectModifiersNodeParent,G_DIALECT_MODIFIERS);

	opString ErrorName() { return ""; }

	void PrintNode(opFileStream& stream)
	{}

	void PrintDialectNode(opDialectStream& stream)
	{}

	void PrintString(opString& s)
	{
		PrintStringChildren(s);
	}

	void CheckForDuplicates();

 	bool Parse()
 	{
 		PARSE_START;
 		{
 			CheckForDuplicates();
 		}
 		PARSE_END;
 	}
};

///
/// DialectModifierNode
///

class DialectModifierNode : public opNode
{
public:
	DECLARE_NODE(DialectModifierNode,opNode,T_UNKNOWN);

	void PrintString(opString& s)
	{
		s += ModifierName;
	}

	opString ErrorName() 
	{ 
		return ModifierName;
	}

protected:
	opString ModifierName;
};

///
/// ValuedDialectModifierNode
///

class ValuedDialectModiferNode : public DialectModifierNode
{
public:
	DECLARE_NODE(ValuedDialectModiferNode,DialectModifierNode,T_UNKNOWN);

	void Init()
	{
		Value = NULL;
	}

	void SetValue(ParenBlockNode* innode)
	{
		Value = innode;
	}

	ParenBlockNode* GetValue()
	{
		return Value;
	}

	void PrintString(opString& s)
	{
		s += ModifierName;

		Value->PrintString(s);
	}

	opString ErrorName() 
	{ 
		opString name = ModifierName;

		if (Value)
			Value->PrintString(name);
		else
			name += "( ... )";

		return name;
	}

protected:
	ParenBlockNode* Value;
};

///
/// ErrorNode
///

class ErrorNode : public ValuedDialectModiferNode
{
public:
	DECLARE_NODE(ErrorNode,ValuedDialectModiferNode,G_ERROR);

	bool Parse()
	{
		PARSE_START;
		{
			Value->Check(T_STRING);
			Value->CheckNone();
		}
		PARSE_END;
	}

	TerminalNode* GetError()
	{
		return node_cast<TerminalNode>( Value->FirstChild() );
	}

	void Init()
	{
		ModifierName = StaticModifierName();
	}

	static opString StaticModifierName()
	{
		return "error";
	}
};

///
/// DescriptionNode
///

class DescriptionNode : public ValuedDialectModiferNode
{
public:
	DECLARE_NODE(DescriptionNode,ValuedDialectModiferNode,G_DESCRIPTION);

	bool Parse()
	{
		PARSE_START;
		{
			Value->Check(T_STRING);
			Value->CheckNone();
		}
		PARSE_END;
	}

	TerminalNode* GetDescription()
	{
		return node_cast<TerminalNode>( Value->FirstChild() );
	}

	void Init()
	{
		ModifierName = StaticModifierName();
	}

	static opString StaticModifierName()
	{
		return "description";
	}
};

///
/// HiddenNode
///

class HiddenNode : public DialectModifierNode
{
public:
	DECLARE_NODE(HiddenNode,DialectModifierNode,G_HIDDEN);

	void Init()
	{
		ModifierName = StaticModifierName();
	}

	static opString StaticModifierName()
	{
		return "hidden";
	}
};

//==========================================
// OrderModifierNodeBase
//
// Base class for BeforeNode and AfterNode.
//==========================================

class OrderModifierNodeBase : public ValuedDialectModiferNode
{
public:
	DECLARE_NODE(OrderModifierNodeBase,ValuedDialectModiferNode,T_UNKNOWN);

	void Init()
	{
		Location = NULL;
	}

	bool Parse()
	{
		PARSE_START;
		{
			Location = Value->Check<TerminalNode>(T_ID);
			Value->CheckNone();
		}
		PARSE_END;
	}

	TerminalNode* GetLocation()
	{
		return Location;
	}

private:

	TerminalNode* Location;
};

///
/// BeforeNode
///

class BeforeNode : public OrderModifierNodeBase
{
public:
	DECLARE_NODE(BeforeNode,OrderModifierNodeBase,G_BEFORE);

	void Init()
	{
		ModifierName = StaticModifierName();
	}

	static opString StaticModifierName()
	{
		return "before";
	}
};

//==========================================
// AfterNode
//==========================================

class AfterNode : public OrderModifierNodeBase
{
public:
	DECLARE_NODE(AfterNode,OrderModifierNodeBase,G_AFTER);

	void Init()
	{
		ModifierName = StaticModifierName();
	}

	static opString StaticModifierName()
	{
		return "after";
	}
};

///
/// VerbatimNode
///

class VerbatimNode : public DialectModifierNode
{
public:
	DECLARE_NODE(VerbatimNode,DialectModifierNode,G_VERBATIM);

	void Init()
	{
		ModifierName = StaticModifierName();
	}

	static opString StaticModifierName()
	{
		return "verbatim";
	}
};

//============================================
// PrefixNodeBase
//
// This is the base class for ClassPrefixNode,
// StructPrefixNode, and EnumPrefixNode.
//============================================

class PrefixNodeBase : public ValuedDialectModiferNode
{
public:
	DECLARE_NODE(PrefixNodeBase,ValuedDialectModiferNode,T_UNKNOWN);

	void Init()
	{
		Prefix = NULL;
	}

	bool Parse()
	{
		PARSE_START;
		{
			Prefix = Value->Check<TerminalNode>(T_ID);
			Value->CheckNone();
		}
		PARSE_END;
	}

	TerminalNode* GetPrefix()
	{
		return Prefix;
	}

private:

	TerminalNode* Prefix;
};

//
// ClassPrefixNode
//

class ClassPrefixNode : public PrefixNodeBase
{
public:
	DECLARE_NODE(ClassPrefixNode,PrefixNodeBase,G_CLASS_PREFIX);

	void Init()
	{
		ModifierName = StaticModifierName();
	}

	static opString StaticModifierName()
	{
		return "classprefix";
	}
};

//
// StructPrefixNode
//

class StructPrefixNode : public PrefixNodeBase
{
public:
	DECLARE_NODE(StructPrefixNode,PrefixNodeBase,G_STRUCT_PREFIX);

	void Init()
	{
		ModifierName = StaticModifierName();
	}

	static opString StaticModifierName()
	{
		return "structprefix";
	}
};

//
// EnumPrefixNode
//

class EnumPrefixNode : public PrefixNodeBase
{
public:
	DECLARE_NODE(EnumPrefixNode,PrefixNodeBase,G_ENUM_PREFIX);

	void Init()
	{
		ModifierName = StaticModifierName();
	}

	static opString StaticModifierName()
	{
		return "enumprefix";
	}
};

///
/// OverrideNode
///

class OverrideNode : public DialectModifierNode
{
public:
	DECLARE_NODE(OverrideNode,DialectModifierNode,G_OVERRIDE);

	void Init()
	{
		ModifierName = StaticModifierName();
	}

	static opString StaticModifierName()
	{
		return "override";
	}
};


} // end namespace dialect_nodes