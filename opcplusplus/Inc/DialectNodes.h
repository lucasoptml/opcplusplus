///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: DialectNodes.h
/// Date: 03/23/2007
///
/// Description:
///
/// Dialect nodes header.
///****************************************************************

namespace dialect_nodes
{

///==========================================
/// Dialect Node Base
///==========================================

class DialectNodeBase : public opNode
{
public:
	DECLARE_NODE(DialectNodeBase,opNode,T_UNKNOWN);

	virtual CriteriaNodeBase* ToCriteriaNodeBase() { return NULL; }
};

///==========================================
/// DialectFileNode
///==========================================

// defines dialect context
typedef context::Dialect< ExpandCalls< OPMacros< FileNode > > > DialectFileNodeParent;

class DialectFileNode : public DialectFileNodeParent
{
public:
	/**** construction/destruction ****/
	
	DECLARE_NODE(DialectFileNode,DialectFileNodeParent,G_DIALECTFILE);

	void SetFiles(const opString& headerfile, const opString& sourcefile)
	{
		HeaderFile = headerfile;
		SourceFile = sourcefile;
	}

	opString ErrorName() { return ""; }

	void PrintDialectNode(opDialectStream& stream);

	void PrintXml(opXmlStream& stream);

	void PrintHighlighting(opSectionStream& stream);

	bool PostOperations();

private:

	//output filenames
	opString SourceFile;
	opString HeaderFile;
};

///==========================================
/// CategoryNode
///==========================================

class CategoryNode : public DialectNodeBase
{
public:
	DECLARE_NODE(CategoryNode,opNode,G_CATEGORY);
	
	void Init()
	{
		Name = NULL;
		Body = NULL;

		// alt prefix stuff
		ClassPrefix  = NULL;
		StructPrefix = NULL;
	}
	
	void SetName(TerminalNode* innode)
	{
		Name = innode;
	}

	TerminalNode* GetName()
	{
		return Name;
	}	

	void SetBody(CategoryBodyNode* innode)
	{
		Body = innode;
	}
	
	CategoryBodyNode* GetBody()
	{
		return Body;
	}

	void SetClassPrefix(ClassPrefixNode* innode)
	{
		ClassPrefix = innode;
	}

	ClassPrefixNode* GetClassPrefix()
	{
		return ClassPrefix;
	}

	void SetStructPrefix(StructPrefixNode* innode)
	{
		StructPrefix = innode;
	}

	StructPrefixNode* GetStructPrefix()
	{
		return StructPrefix;
	}
	
	void PrintDialectNode(opDialectStream& stream);
	void PrintXml(opXmlStream& stream);
	
	opString ErrorName();

	void PrintString(opString& s)
	{
		s += "category ";
		
		Name->PrintString(s);

		s += " { ... }";
	}
	
private:
	TerminalNode*     Name;
	CategoryBodyNode* Body;

	// alt prefix stuff
	ClassPrefixNode*  ClassPrefix;
	StructPrefixNode* StructPrefix;
};

typedef context::Category< opNode > CategoryBodyNodeParent; 

class CategoryBodyNode : public CategoryBodyNodeParent
{
public:
	DECLARE_NODE(CategoryBodyNode,CategoryBodyNodeParent,G_CATEGORY_BODY);

	void PrintDialectNode(opDialectStream& stream)
	{
		PrintNodeChildren(stream);
	}

	opString ErrorName();

	void Register(DialectCategory* categoryinfo);
};

///==========================================
/// CategoryLocationNode
///==========================================

class LocationNodeBase : public DialectNodeBase
{
public:
	DECLARE_NODE(LocationNodeBase,DialectNodeBase,T_UNKNOWN);
	
	void Init()
	{
		Name = NULL;
	}
	
	void SetName(opNode* innode)
	{
		Name = innode;
	}
	
	opNode* GetName()
	{
		return Name;
	}
	
	opString ErrorName();
	void PrintXml(opXmlStream& stream);

protected:
	opNode* Name;
};

class CategoryLocationNode : public LocationNodeBase
{
public:
	DECLARE_NODE(CategoryLocationNode,LocationNodeBase,G_CATEGORY_LOCATION);

	void Init()
	{
		Body = NULL;
	}
	
	void SetBody(CategoryLocationBodyNode* inbody)
	{
		Body = inbody;
	}

	void PrintDialectNode(opDialectStream& stream);

	void Register(DialectCategory* categoryinfo);

	void PrintString(opString& s)
	{
		s += "location ";

		Name->PrintString(s);

		s += " { ... }";
	}

private:

	CategoryLocationBodyNode* Body;
};

///==========================================
/// CategoryLocationBodyNode
///==========================================

typedef context::CategoryLocation< opNode > CategoryLocationBodyNodeParent;

class CategoryLocationBodyNode : public CategoryLocationBodyNodeParent
{
public:
	DECLARE_NODE(CategoryLocationBodyNode,CategoryLocationBodyNodeParent,G_CATEGORY_LOCATION_BODY);

	void PrintDialectNode(opDialectStream& stream)
	{
		PrintNodeChildren(stream);
	}

	void Register(DialectLocation* locationinfo);

	opString ErrorName();
};

class MapNodeBase : public DialectNodeBase
{
public:
	DECLARE_NODE(MapNodeBase, DialectNodeBase, T_UNKNOWN);

	void Init()
	{
		Name  = NULL;
		Order = NULL;
	}

	void SetName(TerminalNode* innode)
	{
		Name = innode;
	}

	TerminalNode* GetName()
	{
		return Name;
	}

	void SetOrder(OrderModifierNodeBase* order)
	{
		Order = order;
	}

	OrderModifierNodeBase* GetOrder()
	{
		return Order;
	}

protected:
	TerminalNode*          Name;
	OrderModifierNodeBase* Order;
};

///==========================================
/// CategoryMapNode
///==========================================

class CategoryMapNode : public MapNodeBase
{
public:
	DECLARE_NODE(CategoryMapNode,MapNodeBase,T_UNKNOWN);
	
	void Init()
	{
		Body = NULL;
	}
	
	void SetBody(CategoryMapBodyNode* innode)
	{
		Body = innode;
	}
	
	typedef DialectMap::MapMode MapMode;
	typedef DialectMap          MapModeEnum;
	
	MapMode GetMode();
	
	void PrintDialectNode(opDialectStream& stream);
	void Register(DialectLocation* locationinfo);
	
protected:
	CategoryMapBodyNode*   Body;
	DialectMap*            MapInfo;
};

///==========================================
/// CategoryDatamapNode
///==========================================

class CategoryDatamapNode : public CategoryMapNode
{
public:
	DECLARE_NODE(CategoryDatamapNode,CategoryMapNode,G_CATEGORY_DATAMAP);

	void PrintXml(opXmlStream& stream);
	opString ErrorName();

	void PrintString(opString& s)
	{
		s += "datamap ";

		Name->PrintString(s);

		if (Body)
			s += " { ... }";
	}
};

///==========================================
/// CategoryFunctionmapNode
///==========================================

class CategoryFunctionmapNode : public CategoryMapNode
{
public:
	DECLARE_NODE(CategoryFunctionmapNode,CategoryMapNode,G_CATEGORY_FUNCTIONMAP);

	void PrintXml(opXmlStream& stream);
	opString ErrorName();

	void PrintString(opString& s)
	{
		s += "functionmap ";

		Name->PrintString(s);

		if (Body)
			s += " { ... }";
	}
};

typedef context::CategoryMap< opNode > CategoryExpressionMapNodeParent;

class CategoryExpressionMapNode : public CategoryExpressionMapNodeParent
{
public:
	DECLARE_NODE(CategoryExpressionMapNode,CategoryExpressionMapNodeParent,T_UNKNOWN);

	void Register(DialectExpressionMap* mapinfo);

	void PrintDialectNode(opDialectStream& stream )
	{
		PrintNodeChildren(stream);
	}

	void PrintXml(opXmlStream& stream);

private:
	DialectExpressionMap* ExpressionMap;
};

///==========================================
/// CategoryMapBodyNode
///==========================================


class CategoryMapBodyNode : public CategoryExpressionMapNode
{
public:
	DECLARE_NODE(CategoryMapBodyNode,CategoryExpressionMapNode,G_CATEGORY_MAP_BODY);

	opString ErrorName();
};


///==========================================
/// DisallowBodyNode
///==========================================

class DisallowBodyNode : public CategoryExpressionMapNode
{
public:
	DECLARE_NODE(DisallowBodyNode,CategoryExpressionMapNode,G_DISALLOW_BODY);

	opString ErrorName();
};

///==========================================
/// Disallow Node
///==========================================

class DisallowNode : public DialectNodeBase
{
public:
	DECLARE_NODE(DisallowNode,DialectNodeBase,G_DISALLOW);

	void Init()
	{
		Name = NULL;
		Body = NULL;
	}

	void SetName(TerminalNode* name)
	{
		Name = name;
	}

	TerminalNode* GetName()
	{
		return Name;
	}

	void SetBody(DisallowBodyNode* body)
	{
		Body = body;
	}

	void PrintDialectNode(opDialectStream& stream)
	{}

	opString ErrorName();

	void PrintXml(opXmlStream& stream);

	void Register(DialectTypeBase* typeinfo);

	TerminalNode* ErrorString();

	void PrintString(opString& s)
	{
		s += "disallow ";

		Name->PrintString(s);

		if (Body)
			s += " { ... }";
	}

private:
	TerminalNode*	  Name;
	DisallowBodyNode* Body;

	DialectDisallow*  DisallowInfo;
};


///==========================================
/// ModifierArgumentNode
///==========================================

class ModifierArgumentNode : public opNode
{
public:
	DECLARE_NODE(ModifierArgumentNode,opNode,G_MODIFIER_ARGUMENT);

	void Init()
	{
		Condition = NULL;
	}

	void SetCondition(TerminalNode* node)
	{
		Condition = node;
	}

	TerminalNode* GetCondition()
	{
		return Condition;
	}

	void PrintString(opString& s);

	bool Parse();

	opString ErrorName();

private:
	TerminalNode* Condition;
};

///==========================================
/// ModifierNodeBase
///==========================================

class ModifierNodeBase : public DialectNodeBase
{
public:
	DECLARE_NODE(ModifierNodeBase,DialectNodeBase,T_UNKNOWN);

	void Init()
	{
		Name = NULL;
		Arguments = NULL;
	}

	void SetName(TerminalNode* innode)
	{
		Name = innode;
	}

	void SetArgument(ModifierArgumentNode* innode)
	{
		Arguments = innode;
	}

	ModifierArgumentNode* GetArguments()
	{
		return Arguments;
	}

	TerminalNode* GetName()
	{
		return Name;
	}

	//get the regex error string, if available
	TerminalNode* ErrorString();

	void Validate();

	//fill in a description, if available
	bool GetDescription(opString& description);

protected:
	TerminalNode* Name;
	ModifierArgumentNode* Arguments;
};


///==========================================
/// DataModifierNode
///==========================================

class DataModifierNode : public ModifierNodeBase
{
public:
	DECLARE_NODE(DataModifierNode,ModifierNodeBase,G_DATAMODIFIER);
	
	void PrintDialectNode(opDialectStream& stream);
	
	void PrintString(opString& s);

	void Register(DialectCategory* categorynode);
	void PrintXml(opXmlStream& stream);

	opString ErrorName();
};

///==========================================
/// FunctionModifierNode
///==========================================

class FunctionModifierNode : public ModifierNodeBase
{
public:
	DECLARE_NODE(FunctionModifierNode,ModifierNodeBase,G_FUNCTIONMODIFIER);

	void PrintDialectNode(opDialectStream& stream);

	void PrintString(opString& s);
	
	void Register(DialectCategory* categorynode);
	void PrintXml(opXmlStream& stream);

	opString ErrorName();
};

///==========================================
/// CriteriaNodeBase
///==========================================

typedef Clean<DialectNodeBase> CriteriaNodeBaseParent;

class CriteriaNodeBase : public CriteriaNodeBaseParent
{
public:
	
	virtual CriteriaNodeBase* ToCriteriaNodeBase()
	{
		return this;
	}

	virtual bool Evaluate(ModifierSupportBase* statement) = NULL;
	virtual void GetKnownModifiers(opSet<opString>& modifiers) = NULL;
	
	virtual bool Validate(DialectExpressionMap* map) = NULL;
};

class CriteriaArgumentNode : public opNode
{
public:
	DECLARE_NODE(CriteriaArgumentNode,opNode,G_CRITERIA_ARGUMENT);

	void Init()
	{
		Argument = NULL;
	}

	/*=== get ===*/

	TerminalNode* GetArgument()
	{
		return Argument;
	}

	/*=== utility ===*/

	bool Parse();

	opString ErrorName();

	void PrintString(opString& s);

private:
	TerminalNode* Argument;
};


///==========================================
/// IsNode
///==========================================

class IsNode : public CriteriaNodeBase
{
public:
	DECLARE_NODE(IsNode,CriteriaNodeBase,G_IS);

	void Init()
	{
		Body = NULL;
	}

	void SetBody(IsBodyNode* innode)
	{
		Body = innode;
	}

	opString ErrorName();

	bool Evaluate(ModifierSupportBase* statement);

	void GetKnownModifiers(opSet<opString>& modifiers);

	void PrintString(opString& s)
	{
		s += "is ";
		PrintStringChildren(s);
	}

	void PrintDialectNode(opDialectStream& stream )
	{}

	virtual bool Validate(DialectExpressionMap* map);

private:
	IsBodyNode* Body;
};



//a base class?
//need to share between operands and groupings?
class CriteriaOperandNode : public opNode
{
public:
	DECLARE_NODE(CriteriaOperandNode, opNode, T_UNKNOWN);


};

class CriteriaValueModifierNode : public CriteriaOperandNode
{
public:
	DECLARE_NODE(CriteriaValueModifierNode,CriteriaOperandNode,G_CRITERIA_VALUE_MODIFIER);

	void Init()
	{
		Name = NULL;
		Argument = NULL;
	}

	void SetName(TerminalNode* node)
	{
		Name = node;
	}

	void SetArgument(CriteriaArgumentNode* node)
	{
		Argument = node;
	}

	CriteriaArgumentNode* GetArgument()
	{
		return Argument;
	}

	TerminalNode* GetName()
	{
		return Name;
	}

	void PrintString(opString& s);

	opString ErrorName();

private:
	TerminalNode*		  Name;
	CriteriaArgumentNode* Argument;
};


typedef CriteriaGroups< 
		CriteriaValueModifiers< 
		Clean<
		CriteriaOperandNode
		> > >
		CriteriaBodyNodeParent;

class CriteriaBodyNode : public CriteriaBodyNodeParent
{
public:
	DECLARE_NODE(CriteriaBodyNode, CriteriaBodyNodeParent, T_UNKNOWN);
	
	void Init()
	{
		bNegate = false;
	}

	enum Operator
	{
		And,
		Or,
		And_Not,
		Or_Not,
	};
	
	void AddOperand(opNode* operand)
	{
		Operands.PushBack(operand);
	}
	
	void AddOperator(Operator op)
	{
		Operators.PushBack(op);
	}
	
	bool Parse();
	
	//builds known argument requirements
	void BuildKnown(opSet<opString>& knownnames, bool bNegative = false);
	void BuildKnownOperand(opNode* operand, opSet<opString>& knownnames, bool bNegative);
	
	bool Evaluate(ModifierSupportBase* statement);
	bool EvaluateOperand(opNode* operand, ModifierSupportBase* statement);
	
	opString ErrorName();
	
	bool Validate(DialectExpressionMap* map);
	bool ValidateOperand(opNode* operand, DialectExpressionMap* map);
	
protected:
	opArray<opNode*>  Operands;
	opArray<Operator> Operators;
	
	//negate the first operand?
	bool			  bNegate;
};


class CriteriaGroupNode : public CriteriaBodyNode
{
public: 
	DECLARE_NODE(CriteriaGroupNode, CriteriaBodyNode, G_CRITERIA_GROUP);
	
	opString ErrorName();
};


///==========================================
/// IsBodyNode
///==========================================

class IsBodyNode : public CriteriaBodyNode
{
public:
	DECLARE_NODE(IsBodyNode, CriteriaBodyNode, G_IS_BODY);

	void PrintString(opString& s)
	{
		PrintStringChildren(s);
	}
};

///==========================================
/// EverythingNode
///==========================================

//may not be needed anymore.
// class EverythingNode : public CriteriaNodeBase
// {
// public:
// 	DECLARE_NODE(EverythingNode,CriteriaNodeBase,G_EVERYTHING);
// 
// 	bool Evaluate(ModifierSupportBase* statement) { return true; }
// 
// 	void GetKnownModifiers(opSet<opString>& modifiers) {}
// 
// 	opString ErrorName() { return ""; }
// 
// 	void PrintDialectNode(opDialectStream& stream )
// 	{}
// 
// };

///==========================================
/// NoteNode
///==========================================

class NoteBodyNode : public CategoryExpressionMapNode
{
public:
	DECLARE_NODE(NoteBodyNode,CategoryExpressionMapNode,G_NOTE_BODY);

	opString ErrorName();
};

class NoteNode : public DialectNodeBase
{
public:
	DECLARE_NODE(NoteNode,DialectNodeBase,G_NOTE);

	void Init()
	{
		Name  = NULL;
		Body  = NULL;
		Order = NULL;
	}

	void SetName(TerminalNode* innode)
	{
		Name = innode;
	}

	TerminalNode* GetName()
	{
		return Name;
	}

	void SetBody(NoteBodyNode* node)
	{
		Body = node;
	}

	NoteBodyNode* GetBody()
	{
		return Body;
	}

	void SetOrder(OrderModifierNodeBase* order)
	{
		Order = order;
	}

	OrderModifierNodeBase* GetOrder()
	{
		return Order;
	}

	void PrintDialectNode(opDialectStream& stream);
	void PrintXml(opXmlStream& stream);

	void Register(DialectLocationBase* locationinfo);

	opString ErrorName();

	void PrintString(opString& s);

private:
	TerminalNode*          Name;
	NoteBodyNode*          Body;
	DialectNote*           NoteInfo;
	OrderModifierNodeBase* Order;
};

///==========================================
/// Note Definition Node
///==========================================

class NoteArgumentNode : public opNode
{
public:
	DECLARE_NODE(NoteArgumentNode,opNode,G_NOTE_DEFINITION_ARGUMENT);
	
	bool Parse();
	
	void SetName(TerminalNode* inname)
	{
		Name = inname;
	}
	
	TerminalNode* GetName()
	{
		return Name;
	}

	void PrintString(opString& s);
	
	opString ErrorName() { return ""; }

private:
	TerminalNode* Name;
};

typedef Clean< 
		ListUtility< 
		opNode > > 
		NoteArgumentListNodeParent;

class NoteArgumentListNode : public NoteArgumentListNodeParent
{
public:
	DECLARE_NODE(NoteArgumentListNode,NoteArgumentListNodeParent,G_NOTE_DEFINITION_ARGUMENT_LIST);
	
	bool Parse();
	
	void PrintString(opString& s);

	opString ErrorName();

private:
	vector<NoteArgumentNode*> Arguments;
};

class NoteDefinitionNode : public DialectNodeBase
{
public:
	DECLARE_NODE(NoteDefinitionNode,opNode,G_NOTE_DEFINITION);
	
	void Init()
	{
		Path      = NULL;
		Body      = NULL;
		Arguments = NULL;
		Override  = NULL;
		bVerbatim = false;
	}

	void SetPath(ScopeNode* inpath)
	{
		Path = inpath;
	}
	
	void SetArguments(NoteArgumentListNode* args)
	{
		Arguments = args;
	}
	
	void SetBody(BraceBlockNode* body)
	{
		Body = body;
	}

	BraceBlockNode* GetBody()
	{
		return Body;
	}
	
	ScopeNode* GetPath()
	{
		return Path;
	}

	NoteArgumentListNode* GetArguments()
	{
		return Arguments;
	}

	void SetOverride(OverrideNode* innode)
	{
		Override = innode;
	}

	OverrideNode* GetOverride()
	{
		return Override;
	}

	void SetVerbatim(bool inbool)
	{
		bVerbatim = inbool;
	}

	bool GetVerbatim()
	{
		return bVerbatim;
	}
	
	void PrintDialectNode(opDialectStream& stream) 
	{

	}
	
	bool GetPathType(opString& name);
	bool GetPathLocation(opString& name);
	bool GetPathTag(opString& name);
	bool GetPathSubTag(opString& name);
	
	bool HasPathTag();
	bool HasPathSubTag();
	
	void GetArguments(vector<opString>& arguments);
	
	opString ErrorName();

	bool IsBodyEmpty();

	int  NumPathItems();
	bool GetPathItem(int index, opString& name);
	bool HasPathItem(int index);

	void PrintString(opString& s)
	{
		s += "note ";

		Path->PrintString(s);

		s += "( ... ) { ... }";
	}

	bool PostParse();

private:
	ScopeNode*            Path;
	NoteArgumentListNode* Arguments;
	BraceBlockNode*       Body;
	OverrideNode*         Override;
	bool                  bVerbatim;
};

///==========================================
/// EnumerationNode
///==========================================

class EnumerationNode : public DialectNodeBase
{
public:
	DECLARE_NODE(EnumerationNode,opNode,G_ENUMERATION);
	
	void Init()
	{
		Name = NULL;
		Body = NULL;

		// alt prefix stuff
		EnumPrefix = NULL;
	}

	void SetName(TerminalNode* innode)
	{
		Name = innode;
	}

	TerminalNode* GetName()
	{
		return Name;
	}	

	void SetBody(EnumerationBodyNode* innode)
	{
		Body = innode;
	}
	
	EnumerationBodyNode* GetBody()
	{
		return Body;
	}

	void SetEnumPrefix(EnumPrefixNode* innode)
	{
		EnumPrefix = innode;
	}

	EnumPrefixNode* GetEnumPrefix()
	{
		return EnumPrefix;
	}
	
	void PrintDialectNode(opDialectStream& stream);
	void PrintXml(opXmlStream& stream);
	
	opString ErrorName();

	void PrintString(opString& s)
	{
		s += "enumeration ";

		Name->PrintString(s);

		s += " { ... }";
	}

private:
	TerminalNode*        Name;
	EnumerationBodyNode* Body;

	// alt prefix stuff
	EnumPrefixNode* EnumPrefix;
};

///==========================================
/// EnumerationBodyNode
///==========================================

typedef context::Enumeration< opNode > EnumerationBodyNodeParent; 

class EnumerationBodyNode : public EnumerationBodyNodeParent
{
public:
	DECLARE_NODE(EnumerationBodyNode,EnumerationBodyNodeParent,G_ENUMERATION_BODY);

	void Register(DialectEnumeration* enuminfo);

	opString ErrorName();
};

///==========================================
/// EnumerationLocationNode
///==========================================

class EnumerationLocationNode : public LocationNodeBase
{
public:
	DECLARE_NODE(EnumerationLocationNode,LocationNodeBase,G_ENUMERATION_LOCATION);

	void Init()
	{
		Body = NULL;
	}

	void SetBody(EnumerationLocationBodyNode* inbody)
	{
		Body = inbody;
	}

	void Register(DialectEnumeration* enuminfo);

	opString ErrorName();

private:
	EnumerationLocationBodyNode* Body;
};

///==========================================
/// EnumerationLocationBodyNode
///==========================================

typedef context::EnumerationLocation< opNode > EnumerationLocationBodyNodeParent;

class EnumerationLocationBodyNode : public EnumerationLocationBodyNodeParent
{
public:
	DECLARE_NODE(EnumerationLocationBodyNode,EnumerationLocationBodyNodeParent,G_ENUMERATION_LOCATION_BODY);

	void Register(DialectEnumLocation* locationinfo);

	opString ErrorName();
};

///==========================================
/// EnumerationMapNode
///==========================================

class EnumerationMapNode : public MapNodeBase
{
public:
	DECLARE_NODE(EnumerationMapNode,MapNodeBase,G_ENUMERATION_MAP);
	
	opString ErrorName();

	void PrintString(opString& s)
	{
		s += "enummap ";
		s += MapInfo->GetName();
	}
	
	void Register(DialectEnumLocation* locationinfo);
	void PrintXml(opXmlStream& stream);
	
private:
	DialectEnumMap* MapInfo;
};

///==========================================
/// CodeNode
///==========================================

typedef OPIncludes<opNode> CodeBodyNodeParent;

class CodeBodyNode : public CodeBodyNodeParent
{
public:
	DECLARE_NODE(CodeBodyNode,CodeBodyNodeParent,G_CODE_BODY);

	void PrintOriginal(opSectionStream& stream)
	{
		PrintOriginalUnspacedChildren(stream);
	}

	opString ErrorName();
};

class CodeNode : public DialectNodeBase
{
public:
	DECLARE_NODE(CodeNode,DialectNodeBase,G_CODE);

	void Init()
	{
		Name      = NULL;
		Body      = NULL;
		bVerbatim = false;
	}

	opString ErrorName();

	//validate location name
	bool PostParse();

	void PrintDialectNode(opDialectStream& stream);

	void SetName(TerminalNode* name)
	{
		Name = name;
	}

	TerminalNode* GetName()
	{
		return Name;
	}

	void SetBody(CodeBodyNode* node)
	{
		Body = node;
	}

	CodeBodyNode* GetBody()
	{
		return Body;
	}

	void PrintString(opString& s)
	{
		s += "code ";

		Name->PrintString(s);

		s += " { ... }";
	}

	void SetVerbatim(bool inbool)
	{
		bVerbatim = inbool;
	}

	bool GetVerbatim()
	{
		return bVerbatim;
	}

private:
	TerminalNode* Name;
	CodeBodyNode* Body;
	bool          bVerbatim;
};

//==========================================
// ExtendPointNode
//==========================================

class ExtendPointNode : public DialectNodeBase
{
public:
	DECLARE_NODE(ExtendPointNode,DialectNodeBase,G_EXTEND_POINT)

	void Init()
	{
		Name = NULL;
		Body = NULL;
	}

	void SetName(TerminalNode* innode)
	{
		Name = innode;
	}

	TerminalNode* GetName()
	{
		return Name;
	}

	void SetBody(BraceBlockNode* innode)
	{
		Body = innode;
	}

	BraceBlockNode* GetBody()
	{
		return Body;
	}

	opString ErrorName();

	void PrintOriginal(opSectionStream& stream)
	{
		Body->PrintOriginalChildren(stream);
	}

	void PrintDialectNode(opDialectStream& stream)
	{

	}

	void PrintString(opString& s)
	{
		s += "extendpoint ";
		
		Name->PrintString(s);

		s += " { ... }";
	}

private:

	TerminalNode*   Name;
	BraceBlockNode* Body;
};

//==========================================
// ExtensionNode
//==========================================

class ExtensionNode : public DialectNodeBase
{
public:
	DECLARE_NODE(ExtensionNode,DialectNodeBase,G_EXTENSION)

	void Init()
	{
		Name = NULL;
	}

	void SetName(TerminalNode* innode)
	{
		Name = innode;
	}

	TerminalNode* GetName()
	{
		return Name;
	}

	opString ErrorName();

	void PrintDialectNode(opDialectStream& stream)
	{}

	void AddExtendPoint(ExtendPointNode* node)
	{
		ExtendPoints.PushBack(node);
	}

	void PrintExtendPoints(opSectionStream& stream)
	{
		size_t size = ExtendPoints.size();

		for (size_t i = 0; i < size; i++)
			ExtendPoints[i]->PrintOriginal(stream);
	}

	void PrintString(opString& s)
	{
		s += "extension ";

		Name->PrintString(s);
	}

private:

	TerminalNode*             Name;
	opArray<ExtendPointNode*> ExtendPoints;
};

//==========================================
// ExtensionPointNode
//==========================================

class ExtensionPointNode : public DialectNodeBase
{
public:
	DECLARE_NODE(ExtensionPointNode,DialectNodeBase,G_EXTENSION_POINT)

	void Init()
	{
		Name = NULL;
	}

	void SetName(TerminalNode* innode)
	{
		Name = innode;
	}

	TerminalNode* GetName()
	{
		return Name;
	}

	opString ErrorName();

	void PrintDialectNode(opDialectStream& stream);

	void CloneNode(ExtensionPointNode* node)
	{
		stacked<TerminalNode> clone = Name->Clone();
		node->SetName(*clone);
		node->AppendNode(clone);
	}

	void PrintOriginal(opSectionStream& stream);

private:

	TerminalNode* Name;
};

//==========================================
// FileDeclarationLocationNode
//==========================================

class FileDeclarationLocationNode : public DialectNodeBase
{
public:
	DECLARE_NODE(FileDeclarationLocationNode,DialectNodeBase,G_FILE_DECLARATION_LOCATION)

	void Init()
	{
		Name  = NULL;
		Order = NULL;
	}

	void SetName(TerminalNode* innode)
	{
		Name = innode;
	}

	TerminalNode* GetName()
	{
		return Name;
	}

	void SetOrder(OrderModifierNodeBase* order)
	{
		Order = order;
	}

	OrderModifierNodeBase* GetOrder()
	{
		return Order;
	}

	opString ErrorName();

	void PrintString(opString& s)
	{
		s += "location ";
		Name->PrintString(s);
	}

private:

	TerminalNode*          Name;
	OrderModifierNodeBase* Order;
};

//==========================================
// FileDeclarationNode
//==========================================

typedef context::FileDeclaration< DialectNodeBase > FileDeclarationBodyNodeParent;

class FileDeclarationBodyNode : public FileDeclarationBodyNodeParent
{
public:
	DECLARE_NODE(FileDeclarationBodyNode,FileDeclarationBodyNodeParent,G_FILE_DECLARATION_BODY)

	opString ErrorName()
	{
		return "";
	}
};

class FileDeclarationNode : public DialectNodeBase
{
public:
	DECLARE_NODE(FileDeclarationNode,DialectNodeBase,G_FILE_DECLARATION)

	void Init()
	{
		Name = NULL;
		Body = NULL;
	}

	void SetName(TerminalNode* innode)
	{
		Name = innode;
	}

	TerminalNode* GetName()
	{
		return Name;
	}

	void SetBody(FileDeclarationBodyNode* innode)
	{
		Body = innode;
	}

	FileDeclarationBodyNode* GetBody()
	{
		return Body;
	}

	opString ErrorName();

	void PrintString(opString& s)
	{
		s += "file ";
		
		Name->PrintString(s);

		s += " { ... }";
	}

private:

	TerminalNode*            Name;
	FileDeclarationBodyNode* Body;
};

//==========================================
// DialectNamespaceNode
//==========================================

class DialectNamespaceNode : public DialectNodeBase
{
public:
	DECLARE_NODE(DialectNamespaceNode,DialectNodeBase,G_DIALECT_NAMESPACE)

	void Init()
	{
		Name = NULL;
		Body = NULL;
	}

	void SetName(TerminalNode* innode)
	{
		Name = innode;
	}

	TerminalNode* GetName()
	{
		return Name;
	}

	void SetBody(BraceBlockNode* innode)
	{
		Body = innode;
	}

	BraceBlockNode* GetBody()
	{
		return Body;
	}

	void PrintDialectNode(opDialectStream& stream);
	void PrintOriginal(opSectionStream& stream);

	opString ErrorName();

	void PrintString(opString& s)
	{
		s += "namespace ";

		Name->PrintString(s);

		s += " { ... }";
	}

private:

	TerminalNode*   Name;
	BraceBlockNode* Body;
};

} // end namespace dialect_nodes