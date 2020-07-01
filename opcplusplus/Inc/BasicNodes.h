/*
	| Author: Kevin Depue / Lucas Ellis (2006)
	| Desc:   Basic node classes (e.g., Pointer, 
	|         Reference, TemplateType, etc.).
*/

namespace nodes
{

//forward declarations
class ScopeNode;

//visibility modes (used by statements)
enum VisibilityMode
{
	vismode_none,
	vismode_public,
	vismode_private,
	vismode_protected,
	vismode_default,
};

///==========================================
/// Terminal nodes
///==========================================

class TerminalNode : public opNode
{
public:
	DECLARE_NODE(TerminalNode, opNode, T_UNKNOWN);

	void CloneNode(TerminalNode* newnode) 
	{
		newnode->value = value;
	}

	// construction / destruction
	// validated construction from scanner
	TerminalNode(const opToken &t, FileNode* infile) 
	{ 
		SetId(t.Id);
		SetLine(t.Line);
		SetFile(infile);

		value = t.Value;

		//terminals whose printed value differs from their recognized value
		if(t.Id == T_SPACER)
			value = "";
		else if(t.Id == T_SPECIAL_LEFT_BRACE)
			value = "{";
		else if(t.Id == T_SPECIAL_RIGHT_BRACE)
			value = "}";
		else if(t.Id == T_SPECIAL_LEFT_PAREN)
			value = "(";
		else if(t.Id == T_SPECIAL_RIGHT_PAREN)
			value = ")";
		else if(t.Id == T_SPECIAL_LEFT_BRACKET)
			value = "[";
		else if(t.Id == T_SPECIAL_RIGHT_BRACKET)
			value = "]";
		else if(t.Id == T_ACCENT)
			value = "'";
		else if(t.Id == T_DOUBLE_ACCENT)
			value = "\"";

	}
	
	// manual construction - always verify usage
	TerminalNode(const opString& invalue, Token intoken, int line, FileNode* infile)
	{
		value = invalue;
		SetId(intoken);
		SetLine(line);
		SetFile(infile);

		if(intoken == T_SPACER)
			value = "";
	}
	
	void PrintValue(opStringStream& s)
	{
		Token id = GetId();
		
		if (id == T_NEWLINE)
			s << endl;
		else
			s << this;
	}

	void PrintValue(opFileStream& s)
	{
		PrintValue(s.header.body);
	}

	void PrintValue(opSectionStream& s)
	{
		PrintValue(s.body);
	}

	//PrintNode : FileStream version (multi-file)
	void PrintNode(opFileStream& s)
	{
		PrintValue(s.header.body);
	}

	//PrintNode : FileSection version (single-file)
	void PrintDialectNode(opDialectStream& s)
	{
		PrintValue(s.header.heading);
	}

	void PrintOriginal(opSectionStream& s)
	{
		PrintValue(s);
	}

	void PrintTransformed(opSectionStream& s)
	{
		PrintValue(s);
	}
	
	void PrintString(opString& s)
	{
		s += value;
	}

	static void MacroPrintEndl(opSectionStream& s, int& charnum)
	{
		const int desiredchar = 60;
		int numspaces = max(desiredchar - charnum,0);

		opString spaces;
		spaces.Reserve(numspaces);
		for(int i = 0; i < numspaces; i++)
			spaces += ' ';

		s << spaces << " \\" << endl;
		charnum = 0;	
	}

	// printing for macros (opdefines)
	void MacroPrint(opSectionStream& s, int& charnum)
	{
		if (GetId() == T_NEWLINE)
		{
			MacroPrintEndl(s,charnum);
		}
		else if (GetId() == T_WHITESPACE)
		{
			const int tabnum = 4;
			
			int valuesize = value.Size();
			for(int i = 0; i < valuesize; i++)
			{
				if(value[i] == '\t')
					charnum += tabnum;
				else
					charnum += 1;
			}
			
			PrintValue(s);
		}
		else if (GetId() != T_COMMENT && GetId() != T_CCOMMENT)
		{
			charnum += value.Length();
			PrintValue(s);
		}
	}

	const opString& GetValue() const
	{ 
		return value; 
	}

	opString ErrorName()  { return value; }
	bool	 IsTerminal() { return true; }
	bool     IsGrammar()  { return false; }

	opString GetTreeValue() { return GetValue(); }

protected:
	opString value;
};

///==========================================
/// Namespace nodes
///==========================================

class NamespaceBlockNode;
class NamespaceDeclNode;

class NamespaceNode : public opNode
{
public:
	DECLARE_NODE(NamespaceNode,opNode,G_NAMESPACE);

	void Init()
	{
		//Decl = NULL;
		Name = NULL;
		Body = NULL;
	}

	void PrintNode( opFileStream& stream );

	void PrintXml(opXmlStream& stream);

	void PrintTop(opFileStream& stream, int& numnamespaces);

	void SetName(TerminalNode* node)
	{
		Name = node;
	}

	void SetBody(NamespaceBlockNode* inbody)
	{
		Body = inbody;
	}

	TerminalNode* GetName()
	{
		return Name;
	}

	opString ErrorName();

	/**** queries ****/

	void BuildClassScope(opString& outscope)
	{
		if(GetParent())
			GetParent()->BuildClassScope(outscope);

		//only want to separate if there's already a class noted
		if(outscope.Size())
			outscope += "::";

		outscope += Name->GetValue();
	}

private:

	TerminalNode*		Name;
	NamespaceBlockNode* Body;
};

class NamespaceAliasNode : public opNode
{
public:
	DECLARE_NODE(NamespaceAliasNode,opNode,G_NAMESPACE_ALIAS);

	void Init()
	{
		Name = NULL;
		//Value = NULL;
	}

	void PrintNode( opFileStream& stream );
	void PrintTop(opFileStream& stream, int& numnamespaces);

	void SetName(TerminalNode* node)
	{
		Name = node;
	}
	
	void AddScope(TerminalNode* value)
	{
		Scopes.PushBack(value);
	}

	TerminalNode* GetName()
	{
		return Name;
	}

	opString ErrorName();

private:
	TerminalNode*		   Name;
	opArray<TerminalNode*> Scopes;
};

typedef context::Global< opNode > NamespaceBlockNodeParent;

class NamespaceBlockNode : public NamespaceBlockNodeParent
{
public:
	DECLARE_NODE(NamespaceBlockNode,NamespaceBlockNodeParent,G_NAMESPACEBLOCK);

	void PrintNode(opFileStream& stream)
	{
		PrintNodeChildren(stream);
	}

	void PrintDialectNode(opDialectStream& stream)
	{
		PrintNodeChildren(stream);
	}

	opString ErrorName();
};

//==========================================
// UsingNamespaceKeywordNode
//==========================================

class UsingNamespaceKeywordNode : public opNode
{
public:
	DECLARE_NODE(UsingNamespaceKeywordNode,opNode,G_USING_NAMESPACE_KEYWORD);

	void PrintOriginal( opSectionStream& stream )
	{
		stream << "using namespace ";
	}

	void PrintNode(opFileStream& stream )
	{
		PrintOriginal(stream.header.body);
	}

	opString ErrorName()
	{
		return "";
	}
};

///==========================================
/// UsingNode
///==========================================

typedef Clean< Scopes< opNode > > UsingNodeParent;

class UsingNode : public UsingNodeParent
{
public:
	DECLARE_NODE(UsingNode,UsingNodeParent,G_USING);

	void Init()
	{
		Scope = NULL;
	}

	void PrintNode(opFileStream& stream )
	{
		PrintOriginal(stream.header.body);
	}

	void PrintTransformed( opSectionStream& stream );	
	void PrintOriginal(opSectionStream& stream );
	void PrintString(opString& s);

	opString ErrorName();

	void SetScope(ScopeNode* inscope)
	{
		Scope = inscope;
	}

	ScopeNode* GetScope()
	{
		return Scope;
	}

private:
	ScopeNode* Scope;
};

/*=============================================*
* block nodes (e.g., (), [], {}, <>, etc.)    *
*=============================================*/

typedef Blocks< ExpandCalls< opNode > > BlockNodeBaseParent;

//blocknode base
class BlockNodeBase : public BlockNodeBaseParent
{
public:
	DECLARE_NODE(BlockNodeBase,BlockNodeBaseParent,T_UNKNOWN);

	opString ErrorName() { return ""; }

	//TODO: make a template function for printing :P
};

// {} block
class BraceBlockNode : public BlockNodeBase
{
public:
	DECLARE_NODE(BraceBlockNode,BlockNodeBase,G_BRACE_BLOCK);

	bool Parse();

	void PrintNode(opFileStream& stream)
	{
		PrintOriginal(stream.header.body);
	}

	void PrintDialectNode(opDialectStream& stream)
	{
		PrintOriginal(stream.header.heading);
	}

	void PrintOriginal(opSectionStream& stream );
	void PrintTransformed( opSectionStream& stream );
	void PrintString(opString& s);

	void PrintOriginalTrimmed(opSectionStream& stream);

	//makes this function public
	void PrintOriginalChildren(opSectionStream& stream )
	{
		Super::PrintOriginalUnspacedChildren(stream);
	}

	//makes this function public
	void PrintOriginalUnspacedChildren(opSectionStream& stream )
	{
		Super::PrintOriginalUnspacedChildren(stream);
	}

	void MacroPrint(opSectionStream& stream, int& charnum);

	void CloneNode(opNode* newnode) 
	{
		CloneChildren(newnode);
	}
};

// () block
class ParenBlockNode : public BlockNodeBase
{
public:
	DECLARE_NODE(ParenBlockNode,BlockNodeBase,G_PAREN_BLOCK);

	bool Parse();
	
	void PrintNode(opFileStream& stream)
	{
		PrintOriginal(stream.header.body);
	}

	void PrintDialectNode(opDialectStream& stream)
	{
		PrintOriginal(stream.header.heading);
	}

	void PrintOriginal(opSectionStream& stream );
	void PrintTransformed( opSectionStream& stream );
	void PrintString(opString& s);

	void MacroPrint(opSectionStream& stream, int& charnum);

	void CloneNode(opNode* newnode) 
	{
		CloneChildren(newnode);
	}
};

// [] block
class BracketBlockNode : public BlockNodeBase
{
public:
	DECLARE_NODE(BracketBlockNode,BlockNodeBase,G_BRACKET_BLOCK);

	bool Parse();

	void PrintNode(opFileStream& stream)
	{
		PrintOriginal(stream.header.body);
	}

	void PrintDialectNode(opDialectStream& stream)
	{
		PrintOriginal(stream.header.heading);
	}

	void PrintOriginal(opSectionStream& stream );
	void PrintTransformed( opSectionStream& stream );
	void PrintString(opString& s);

	void MacroPrint(opSectionStream& stream, int& charnum);

	void CloneNode(opNode* newnode) 
	{
		CloneChildren(newnode);
	}
};

class AngledBlockNode : public BlockNodeBase
{
public:
	DECLARE_NODE(AngledBlockNode,BlockNodeBase,G_ANGLED_BLOCK);

	bool Parse();

	void PrintNode(opFileStream& stream)
	{
		PrintOriginal(stream.header.body);
	}

	void PrintDialectNode(opDialectStream& stream)
	{
		PrintOriginal(stream.header.heading);
	}

	void PrintOriginal(opSectionStream& stream );
	void PrintTransformed( opSectionStream& stream );
	void PrintString(opString& s);

	void CloneNode(opNode* newnode) 
	{
		CloneChildren(newnode);
	}
};

///==========================================
/// Pointer nodes
///==========================================

class PointerNode : public opNode
{
public:
	DECLARE_NODE(PointerNode,opNode,G_POINTER);

	void Init()
	{
		Type = NULL;
	}

	enum StarType
	{
		Plain,
		Const,
		Volatile,
		ConstVolatile,
	};

	void PrintOriginal(opSectionStream& stream );
	void PrintTransformed( opSectionStream& stream );
	void PrintString(opString& s);

	void SetType(opNode* intype)
	{
		Type = intype;
	}

	opNode* GetType()
	{
		return Type;
	}

	void AddStar(StarType type)
	{
		Stars.PushBack(type);
	}

	StarType GetStarType(int index)
	{
		return Stars[index];
	}

	int GetNumStars()
	{
		return Stars.Size();
	}

	void CloneNode(PointerNode* node)
	{
		stacked<opNode> clonetype = Type->CloneGeneric();
		node->SetType(*clonetype);
		node->AppendNode(clonetype);
		
		int num = GetNumStars();
		for(int i = 0; i < num; i++)
			node->AddStar(Stars[i]);
	}

	opString ErrorName();

private:
	opNode* Type;
	opArray<StarType> Stars;
};

///==========================================
/// Operator nodes
///==========================================

typedef context::Operator< opNode > OperatorNodeParent;
class OperatorNode : public OperatorNodeParent
{
public:
	DECLARE_NODE(OperatorNode,OperatorNodeParent,G_OPERATOR);
	
	void Init()
	{
		OperatorType = NULL;
	}

	void PrintOriginal( opSectionStream& stream );
	void PrintString(opString& s);
	
	void CloneNode(OperatorNode* newnode)
	{
		newnode->bCastOperator = bCastOperator;
		
		stacked<opNode> clonetype = OperatorType->CloneGeneric();
		
		newnode->OperatorType = *clonetype;
		newnode->AppendNode(clonetype);
	}

	opString ErrorName();
};

///==========================================
/// Function nodes
///==========================================


class FunctionArgumentDefinitionBase : public opNode
{
public:
	DECLARE_NODE(FunctionArgumentDefinitionBase,opNode,T_UNKNOWN);

	void Init()
	{
		InnerStatement = NULL;
	}

	class DataStatementNode* GetInnerStatement()
	{
		return InnerStatement;
	}

	void SetInnerStatement(DataStatementNode* statement)
	{
		assert(!InnerStatement);//set once
		InnerStatement = statement;
	}

private:
	DataStatementNode* InnerStatement;
};

typedef DataStatementOnly<FunctionArgumentDefinitionBase> FunctionArgumentDefinitionNodeParent;

class FunctionArgumentDefinitionNode : public FunctionArgumentDefinitionNodeParent
{
public:
	DECLARE_NODE(FunctionArgumentDefinitionNode,FunctionArgumentDefinitionNodeParent,G_FUNCTION_ARGUMENTDEFINITION);

	void PrintOriginal(opSectionStream& stream )
	{
		PrintOriginalChildren(stream);
	}

	void PrintString(opString& s)
	{
		PrintStringChildren(s);
	}

	bool Parse()
	{
		PARSE_START;
		{
			if(!GetInnerStatement())
			{
				//TODO: this message sucks.
				opError::MessageError(this,"Invalid argument provided to function.");
			}
		}
		PARSE_END;
	}

	void CloneNode(opNode* newnode)
	{
		CloneChildren(newnode);
	}

	opString ErrorName() { return ""; }
};

class FunctionArgumentDefaultNode : public opNode
{
public:
	DECLARE_NODE(FunctionArgumentDefaultNode,opNode,G_FUNCTION_ARGUMENTDEFAULT);
	
	void PrintOriginal(opSectionStream& stream )
	{
		PrintOriginalChildren(stream);
	}
	
	void PrintString(opString& s)
	{
		PrintStringChildren(s);
	}

	void CloneNode(opNode* newnode)
	{
		CloneChildren(newnode);
	}

	opString ErrorName() { return ""; }
};

class FunctionArgumentNode : public opNode
{
public:
	DECLARE_NODE(FunctionArgumentNode,opNode,G_FUNCTION_ARGUMENT);

	void Init()
	{
		Definition = NULL;
		Default = NULL;
	}

	void PrintOriginal(opSectionStream& stream);
	void PrintString(opString& s);

	bool Parse();

	FunctionArgumentDefinitionNode* GetDefinition()
	{
		return Definition;
	}

	FunctionArgumentDefaultNode* GetDefault()
	{
		return Default;
	}

	void SetDefinition(FunctionArgumentDefinitionNode* node)
	{
		Definition = node;
	}

	void SetDefault(FunctionArgumentDefaultNode* node)
	{
		Default = node;
	}

	void CloneNode(FunctionArgumentNode* node)
	{
		stacked<FunctionArgumentDefinitionNode> clonedef = Definition->Clone();
		node->SetDefinition(*clonedef);
		node->AppendNode(clonedef);

		if(Default)
		{
			stacked<FunctionArgumentDefaultNode> clonedefault = Default->Clone();
			node->SetDefault(*clonedefault);
			node->AppendNode(clonedefault);
		}
	}

	opString ErrorName();

private:
	FunctionArgumentDefinitionNode* Definition;
	FunctionArgumentDefaultNode* Default;
};

class FunctionArgumentListNode : public context::Argument<opNode>
{
public:
	DECLARE_NODE(FunctionArgumentListNode,context::Argument<opNode>,G_FUNCTION_ARGUMENTLIST);
	
	bool Parse();

	//print the original argument list (cleaned)
	void PrintOriginal(opSectionStream& stream );
	
	void PrintString(opString& s);

	//print the default-less argument list
	void PrintDefaultless(opSectionStream& stream);

	//print the name only argument list
	void PrintNameOnly(opSectionStream& stream);

	//print the signature only argument list
	void PrintSignature(opSectionStream& stream);
	void PrintSignature(opString& s);

	void AddArgument(FunctionArgumentNode* node)
	{
		Arguments.push_back(node);
	}

	int GetNumArguments()
	{
		return (int)Arguments.size();
	}

	FunctionArgumentNode* GetArgument(int index)
	{
		return Arguments[index];
	}

	void CloneNode(FunctionArgumentListNode* node)
	{
		int numargs = (int) Arguments.size();
		for(int i = 0; i < numargs; i++)
		{
			stacked<FunctionArgumentNode> clone = Arguments[i]->Clone();
			node->AddArgument(*clone);
			node->AppendNode(clone);
		}
	}

	opString ErrorName();

private:
	vector<FunctionArgumentNode*> Arguments;
};

class FunctionNode : public opNode
{
public:
	DECLARE_NODE(FunctionNode,opNode,G_FUNCTION);

	void PrintTransformed( opSectionStream& stream );
	void PrintOriginal(opSectionStream& stream );
	void PrintString(opString& s);
	void PrintDefaultless(opSectionStream& stream );

	void Init()
	{
		Name = NULL;
		Arguments = NULL;
		bConst = false;
		Assignment = NULL;
	}

	opNode* GetName()
	{
		return Name;
	}

	FunctionArgumentListNode* GetArguments()
	{
		return Arguments;
	}

	TerminalNode* GetAssignment()
	{
		return Assignment;
	}
	
	bool IsConst()
	{
		return bConst;
	}
	
	void SetName(opNode* inName)
	{
		Name = inName;
	}

	void SetArguments(FunctionArgumentListNode* inArguments)
	{
		Arguments = inArguments;
	}

	void SetConst(bool bconst)
	{
		bConst = bconst;
	}
	
	void SetAssignment(TerminalNode* node)
	{
		Assignment = node;
	}

	opString ErrorName();

private:
	opNode* Name;
	FunctionArgumentListNode* Arguments;
	
	//c++ trailing function modifiers
	bool bConst;
	TerminalNode* Assignment;
};

class FunctionPrototypeNode : public opNode
{
public:
	DECLARE_NODE(FunctionPrototypeNode,opNode,G_FUNCTION_PROTOTYPE);

	void Init()
	{
		Return = NULL;
		Function = NULL;
	}

	void PrintTransformed( opSectionStream& stream );
	void PrintOriginal(opSectionStream& stream );

	opNode* GetReturn()
	{
		return Return;
	}

	FunctionNode* GetFunction()
	{
		return Function;
	}

	void SetReturn(opNode* inReturn)
	{
		Return = inReturn;
	}

	void SetFunction(FunctionNode* inFunction)
	{
		Function = inFunction;
	}

	opString ErrorName();

	void PrintString(opString& s);

private:
	opNode* Return;
	FunctionNode* Function;
};

class FunctionDefinitionNode : public opNode
{
public:
	DECLARE_NODE(FunctionDefinitionNode,opNode,G_FUNCTION_DEFINITION);

	void Init()
	{
		Return = NULL;
		Function = NULL;
		Body = NULL;
	}

	void PrintTransformed( opSectionStream& stream );
	void PrintOriginal( opSectionStream& stream );

	opNode* GetReturn()
	{
		return Return;
	}

	FunctionNode* GetFunction()
	{
		return Function;
	}

	BraceBlockNode* GetBody()
	{
		return Body;
	}

	void SetReturn(opNode* inReturn)
	{
		Return = inReturn;
	}

	void SetFunction(FunctionNode* inFunction)
	{
		Function = inFunction;
	}

	void SetBody(BraceBlockNode* inBody)
	{
		Body = inBody;
	}

	opString ErrorName();

	void PrintString(opString& s)
	{
		Return->PrintString(s);

		s += " ";

		Function->PrintString(s);

		s += " { ... }";
	}

private:
	opNode* Return;
	FunctionNode* Function;
	BraceBlockNode* Body;
};

///==========================================
/// DestructorNode
///==========================================

class DestructorNode : public opNode
{
public:
	DECLARE_NODE(DestructorNode,opNode,G_DESTRUCTOR);

	void Init()
	{
		function = NULL;
	}

	opNode* GetName()
	{
		return function->GetName();
	}

	void SetFunction(FunctionNode* node)
	{
		function = node;
	}

	FunctionNode* GetFunction()
	{
		return function;
	}

	void PrintOriginal(opSectionStream& stream);
	void PrintString(opString& s);

	void PrintDefaultless(opSectionStream& stream)
	{
		function->PrintDefaultless(stream);
	}

	opString ErrorName();

private:
	FunctionNode* function;
};

///==========================================
/// Destructor Prototype Node
///==========================================

class DestructorPrototypeNode : public opNode
{
public:
	DECLARE_NODE(DestructorPrototypeNode, opNode, G_DESTRUCTOR_PROTOTYPE);
	
	void Init()
	{
		Destructor = NULL;
	}
	
	void SetDestructor(DestructorNode* node)
	{
		Destructor = node;
	}
	
	DestructorNode* GetDestructor()
	{
		return Destructor;
	}
	
	void PrintOriginal(opSectionStream& stream);
	opString ErrorName();
	
	void PrintString(opString& s);
	
private:
	DestructorNode* Destructor;
};


///==========================================
/// Destructor Definition Node
///==========================================

class DestructorDefinitionNode : public opNode
{
public:
	DECLARE_NODE(DestructorDefinitionNode, opNode, G_DESTRUCTOR_DEFINITION);

	void Init()
	{
		Destructor = NULL;
		Body = NULL;
	}

	void SetDestructor( DestructorNode* node )
	{
		Destructor = node;
	}

	void SetBody(BraceBlockNode* body)
	{
		Body = body;
	}

	BraceBlockNode* GetBody()
	{
		return Body;
	}

	DestructorNode* GetDestructor()
	{
		return Destructor;
	}

	void PrintOriginal(opSectionStream& stream);


	opString ErrorName();

private:
	BraceBlockNode* Body;
	DestructorNode* Destructor;
};

///==========================================
/// Constructor Node
///==========================================

class ConstructorNode : public opNode
{
public:
	DECLARE_NODE(ConstructorNode,opNode,G_CONSTRUCTOR);
	
	void Init()
	{
		function = NULL;
	}

	void SetFunction(FunctionNode* node)
	{
		function = node;
	}

	FunctionNode* GetFunction()
	{
		return function;
	}
	
	opNode* GetName()
	{
		return function->GetName();
	}

	void PrintOriginal(opSectionStream& stream);
		void PrintString(opString& s);
	opString ErrorName();
	
	void PrintDefaultless(opSectionStream& stream)
	{
		function->PrintDefaultless(stream);
	}

private:
	FunctionNode* function;
};

///==========================================
/// Constructor Prototype Node
///==========================================

class ConstructorPrototypeNode : public opNode
{
public:
	DECLARE_NODE(ConstructorPrototypeNode, opNode, G_CONSTRUCTOR_PROTOTYPE);
	
	void Init()
	{
		Constructor = NULL;
	}

	void SetConstructor(ConstructorNode* node)
	{
		Constructor = node;
	}

	ConstructorNode* GetConstructor()
	{
		return Constructor;
	}
	
	void PrintOriginal(opSectionStream& stream);

	opString ErrorName();

	void PrintString(opString& s)
	{
		return Constructor->PrintString(s);
	}

private:
	ConstructorNode* Constructor;
};

///==========================================
/// Constructor Initializer List Node
///==========================================

//this is the optional initializer list on constructors
class ConstructorInitializerListNode : public opNode
{
public:
	DECLARE_NODE(ConstructorInitializerListNode, opNode, G_CONSTRUCTOR_INITIALIZER_LIST);

	void PrintOriginal(opSectionStream& stream)
	{
		PrintOriginalChildren(stream);
	}

	//this converts functions to initializer nodes
	bool Parse();

	opString ErrorName();
};

///==========================================
/// Constructor Initializer Node
///==========================================

class ConstructorInitializerArgumentNode : public opNode
{
public:
	DECLARE_NODE(ConstructorInitializerArgumentNode,opNode,G_CONSTRUCTOR_INITIALIZER_ARGUMENT);

	void PrintOriginal(opSectionStream& stream)
	{
		PrintOriginalChildren(stream);
	}

	opString ErrorName();
};

class ConstructorInitializerNode : public opNode
{
public:
	DECLARE_NODE(ConstructorInitializerNode,opNode,G_CONSTRUCTOR_INITIALIZER);

	void Init()
	{
		Name = NULL;
		Argument = NULL;
	}

	void PrintOriginal(opSectionStream& stream)
	{
		Name->PrintOriginal(stream);
		stream << '(';
		Argument->PrintOriginal(stream);
		stream << ')';
	}

	void SetName(opNode* name)
	{
		Name = name;
	}

	opNode* GetName()
	{
		return Name;
	}

	void SetArgument(ConstructorInitializerArgumentNode* arg)
	{
		Argument = arg;
	}

	opString ErrorName();

private:
	ConstructorInitializerArgumentNode* Argument;
	opNode* Name;
};


///==========================================
/// Constructor Definition Node
///==========================================

class ConstructorDefinitionNode : public opNode
{
public:
	DECLARE_NODE(ConstructorDefinitionNode, opNode, G_CONSTRUCTOR_DEFINITION);

	void Init()
	{
		Constructor  = NULL;
		Initializers = NULL;
		Body		 = NULL;
	}

	void SetConstructor( ConstructorNode* node )
	{
		Constructor = node;
	}

	void SetBody(BraceBlockNode* body)
	{
		Body = body;
	}

	BraceBlockNode* GetBody()
	{
		return Body;
	}

	void SetInitializers(ConstructorInitializerListNode* init)
	{
		Initializers = init;
	}

	ConstructorInitializerListNode* GetInitializers()
	{
		return Initializers;
	}

	ConstructorNode* GetConstructor()
	{
		return Constructor;
	}

	void PrintOriginal(opSectionStream& stream);

	opString ErrorName();

private:
	BraceBlockNode*					Body;
	ConstructorInitializerListNode* Initializers;
	ConstructorNode*				Constructor;
};

//==========================================
// TemplateTypeNode
//==========================================

typedef context::TemplateType< AngledBlockNode > TemplateTypeBodyNodeParent;

class TemplateTypeBodyNode : public TemplateTypeBodyNodeParent
{
public:
	DECLARE_NODE(TemplateTypeBodyNode,TemplateTypeBodyNodeParent,G_TEMPLATE_TYPE_BODY);

	void Clone(TemplateTypeBodyNode* node)
	{
		CloneChildren( node );
	}

	void PrintNode(opFileStream& stream)
	{
		PrintOriginal(stream.header.body);
	}

	void PrintDialectNode(opDialectStream& stream)
	{
		PrintOriginal(stream.header.heading);
	}

	void PrintTransformed( opSectionStream& stream )
	{
		Super::PrintTransformed( stream );
	}

	void PrintOriginal( opSectionStream& stream )
	{
		Super::PrintOriginal( stream );
	}

	void PrintString(opString& s)
	{
		Super::PrintString( s );
	}

	opString ErrorName()
	{
		return "";
	}
};

class TemplateTypeNode : public opNode
{
public:
	DECLARE_NODE(TemplateTypeNode,opNode,G_TEMPLATE_TYPE);

	void Init()
	{
		Name   = NULL;
		Braces = NULL;		
	}

	void SetName(TerminalNode* innode)
	{
		Name = innode;
	}

	TerminalNode* GetName()
	{
		return Name;
	}

	void SetBody(TemplateTypeBodyNode* innode)
	{
		Braces = innode;
	}

	TemplateTypeBodyNode* GetBody()
	{
		return Braces;
	}

	void PrintTransformed( opSectionStream& stream );
	void PrintOriginal( opSectionStream& stream );
	void PrintString(opString& s);

	void CloneNode(TemplateTypeNode* node)
	{
		stacked<TerminalNode>         clonedname   = Name->Clone();
		stacked<TemplateTypeBodyNode> clonedbraces = Braces->Clone();

		node->SetName(*clonedname);
		node->AppendNode(clonedname);

		node->SetBody(*clonedbraces);
		node->AppendNode(clonedbraces);
	}

	opString ErrorName();

private:
	TerminalNode*         Name;
	TemplateTypeBodyNode* Braces;
};

class TemplateDeclNode : public opNode
{
public:
	DECLARE_NODE(TemplateDeclNode,opNode,G_TEMPLATE_DECL);

	void Init()
	{
		braces = NULL;
	}

	void SetBraces(AngledBlockNode* inbraces)
	{
		braces = inbraces;
	}
	
	void PrintTransformed( opSectionStream& stream );

	void PrintOriginal(opSectionStream& stream)
	{
		PrintTransformed(stream);
	}

	void PrintString(opString& s)
	{
		s += "template";
		//braces->PrintStringUnspacedChildren(s);
		braces->PrintString(s);
	}

	opString ErrorName();

private:
	AngledBlockNode* braces;
};

///==========================================
/// Array node
///==========================================

class ArrayNode : public opNode
{
public:
	DECLARE_NODE(ArrayNode,opNode,G_ARRAY);

	void Init()
	{
		Name = NULL;
	}

	void PrintTransformed( opSectionStream& stream );
	void PrintOriginal(opSectionStream& stream );

	void PrintString(opString& s);

	void SetName(TerminalNode* n)
	{
		Name = n;
	}

	TerminalNode* GetName()
	{
		return Name;
	}

	void AddBracket(BracketBlockNode* b)
	{
		Brackets.push_back(b);
	}

	int GetNumBrackets()
	{
		return (int)Brackets.size();
	}

	BracketBlockNode* GetBracket(int index)
	{
		return Brackets[index];
	}

	opString ErrorName();

	void CloneNode(ArrayNode* node)
	{
		stacked<TerminalNode> name = Name->Clone();
		node->SetName(*name);
		node->AppendNode(name);

		int num = (int)Brackets.size();
		for(int i = 0; i < num; i++)
		{
			stacked<BracketBlockNode> bracket = Brackets[i]->Clone();
			node->AddBracket(*bracket);
			node->AppendNode(bracket);
		}
	}

private:
	TerminalNode* Name;
	vector<BracketBlockNode*> Brackets;
};

///==========================================
/// Type Array node
///==========================================

class TypeArrayNode : public opNode
{
public:
	DECLARE_NODE(TypeArrayNode,opNode,G_TYPE_ARRAY);

	void Init()
	{
		Type = NULL;
	}

	void PrintTransformed( opSectionStream& stream );
	void PrintOriginal(opSectionStream& stream );

	void SetType(opNode* t)
	{
		Type = t;
	}

	opNode* GetType()
	{
		return Type;
	}

	void AddBracket(BracketBlockNode* b)
	{
		Brackets.push_back(b);
	}

	int GetNumBrackets()
	{
		return (int)Brackets.size();
	}

	BracketBlockNode* GetBracket(int index)
	{
		return Brackets[index];
	}

	opString ErrorName();

private:
	opNode* Type;
	vector<BracketBlockNode*> Brackets;
};


///==========================================
/// Reference nodes
///==========================================

class ReferenceNode : public opNode
{
public:
	DECLARE_NODE(ReferenceNode,opNode,G_REFERENCE);

	void Init()
	{
		Type = NULL;
	}

	void SetType(opNode* type)
	{
		Type = type;
	}

	opNode* GetType()
	{
		return Type;
	}

	void PrintTransformed( opSectionStream& stream );
	void PrintOriginal( opSectionStream& stream );
	void PrintString(opString& s);

	void CloneNode(ReferenceNode* node)
	{
		stacked<opNode> clonetype = Type->CloneGeneric();
		node->SetType(*clonetype);
		node->AppendNode(clonetype);
	}

	opString ErrorName();

private:
	opNode* Type;
};

///==========================================
/// Visibility nodes
///==========================================

class VisibilityLabelNode : public opNode
{
public:
	DECLARE_NODE(VisibilityLabelNode,opNode,G_VISIBILITY_LABEL);

	void Init()
	{
		Label = NULL;
	}

	void SetLabel(TerminalNode* label)
	{
		Label = label;
		
		if(Label->GetId() == T_PUBLIC)
			Visibility = vismode_public;
		else if(Label->GetId() == T_PRIVATE)
			Visibility = vismode_private;
		else if(Label->GetId() == T_PROTECTED)
			Visibility = vismode_protected;
		else
			Visibility = vismode_none;
	}

	void PrintTransformed( opSectionStream& stream );
	void PrintOriginal( opSectionStream& stream );

	VisibilityMode GetVisibility()
	{
		return Visibility;
	}

	opString ErrorName();

private:
	VisibilityMode Visibility;
	TerminalNode* Label;
};

///==========================================
/// Scope nodes
///==========================================

class ScopeNode : public opNode
{
public:
	DECLARE_NODE(ScopeNode,opNode,G_SCOPE);

	void Init()
	{
		bGlobal = false;
	}

	// adds a scope to Scope vector
	//
	// example: hello::there <==> Scope {hello,there}
	void AddScope(opNode* inscope)
	{
		Scope.push_back(inscope);
	}

	// put a new scope on the front of the existing scope
	void PrependScope(opNode* inscope)
	{
		Scope.push_front(inscope);
	}

	void PrintNode(opFileStream& stream )
	{
		PrintOriginal(stream.header.body);
	}

	void PrintTransformed( opSectionStream& stream );
	void PrintOriginal(opSectionStream & stream);
	void PrintString(opString& s);

	void PrintDialectNode(opDialectStream& stream )
	{
		PrintOriginal(stream.header.heading);
	}

	opNode* GetScopeNode(int index)
	{
		return Scope[index];
	}

	int GetScopeNum()
	{
		return (int) Scope.size();
	}

	void SetGlobal(bool newglobal)
	{
		bGlobal = newglobal;
	}

	bool IsGlobal()
	{
		return bGlobal;
	}

	void CloneNode(ScopeNode* node)
	{
		int scopenum = GetScopeNum();
		for(int i = 0; i < scopenum; i++)
		{
			stacked<opNode> clonescope = Scope[i]->CloneGeneric();
			node->AddScope(*clonescope);
			node->AppendNode(clonescope);
		}

		node->SetGlobal(bGlobal);
	}

	opString ErrorName();

private:

	bool             bGlobal;
	opDeque<opNode*> Scope;
};


///==========================================
/// ScopePointer nodes
///==========================================

class ScopePointerNode : public opNode
{
public:
	DECLARE_NODE(ScopePointerNode,opNode,G_SCOPE_POINTER);

	void Init()
	{
		bGlobal = false;
		Stars = 0;
	}

	// adds a scope to Scope vector
	//
	// example: hello::there <==> Scope {hello,there}
	void AddScope(opNode* inscope)
	{
		Scope.push_back(inscope);
	}

	void PrintNode(opFileStream& stream )
	{
		PrintOriginal(stream.header.body);
	}

	void PrintTransformed( opSectionStream& stream );
	void PrintOriginal(opSectionStream & stream);
	void PrintString(opString& s);

	opNode* GetScopeNode(int index)
	{
		return Scope[index];
	}

	int GetScopeNum()
	{
		return (int) Scope.size();
	}

	void SetGlobal(bool newglobal)
	{
		bGlobal = newglobal;
	}

	bool IsGlobal()
	{
		return bGlobal;
	}

	void SetNumStars(int stars)
	{
		Stars = stars;
	}

	void CloneNode(ScopePointerNode* node)
	{
		int scopenum = GetScopeNum();
		for(int i = 0; i < scopenum; i++)
		{
			stacked<opNode> clonescope = Scope[i]->CloneGeneric();
			node->SetGlobal(bGlobal);
			node->SetNumStars(Stars);
			node->AddScope(*clonescope);
			node->AppendNode(clonescope);
		}
	}

	opString ErrorName();

private:

	bool bGlobal;
	int Stars;
	opArray<opNode*> Scope;
};


///==========================================
/// Pointer Member Node
///==========================================

class PointerMemberNode : public opNode
{
public:
	DECLARE_NODE(PointerMemberNode,opNode,G_POINTER_MEMBER);

	void Init()
	{
		Type = NULL;
		Scope = NULL;
	}

	void SetType(opNode* node)
	{
		Type = node;
	}

	void SetScope(ScopePointerNode* node)
	{
		Scope = node;
	}

	void CloneNode(PointerMemberNode* node)
	{
		stacked<opNode> type = Type->CloneGeneric();
		stacked<ScopePointerNode> scope = Scope->Clone();
		node->SetType(*type);
		node->SetScope(*scope);
		node->AppendNode(type);
		node->AppendNode(scope);
	}

	void PrintOriginal(opSectionStream& stream );
	void PrintString(opString& s);

	opString ErrorName();

private:
	opNode*			  Type;
	ScopePointerNode* Scope;
};


///==========================================
/// Typename nodes
///==========================================

class TypenameNode : public opNode
{
public:
	DECLARE_NODE(TypenameNode,opNode,G_TYPENAME);

	void Init()
	{
		Name = NULL;
	}

	void SetName(opNode* inname)
	{
		Name = inname;
	}

	void PrintTransformed( opSectionStream& stream );

	opString ErrorName();

private:
	opNode* Name;
};

///==========================================
/// Typedef nodes
///==========================================

class TypedefNode : public opNode
{
public:
	DECLARE_NODE(TypedefNode,opNode,G_TYPEDEF);

	void Init()
	{
		Modifier = NULL;
		Type     = NULL;
		Name     = NULL;
	}

	/*=== set ===*/

	void SetModifier(TerminalNode* innode)
	{
		Modifier = innode;
	}

	void SetType(opNode* innode)
	{
		Type = innode;
	}

	void SetName(TerminalNode* innode)
	{
		Name = innode;
	}

	/*=== get ===*/

	TerminalNode* GetModifier()
	{
		return Modifier;
	}

	opNode* GetType()
	{
		return Type;
	}

	TerminalNode* GetName()
	{
		return Name;
	}

	opString ErrorName();

	void PrintTransformed(opSectionStream& stream);

private:
	TerminalNode* Modifier;
	opNode*       Type;
	TerminalNode* Name;
};

///==========================================
/// Function pointer nodes
///==========================================

typedef Clean< Scopes< opNode > > FunctionPointerDeclNodeParent;

//TODO: member function support
//TODO: weird modifier support (__cdecl etc)
class FunctionPointerDeclNode : public FunctionPointerDeclNodeParent
{
public:
	DECLARE_NODE(FunctionPointerDeclNode,FunctionPointerDeclNodeParent,G_FUNCTION_POINTER_DECL);

	void Init()
	{
		Scope = NULL;
		Name = NULL;
	}

	void PrintTransformed( opSectionStream& stream );
	void PrintOriginal(opSectionStream& stream );
	void PrintString(opString& s);

	opNode* GetName()
	{
		return Name;
	}

	opNode* GetScope()
	{
		return Scope;
	}

	void SetName(opNode* node)
	{
		Name = node;
	}

	void SetScope(opNode* node)
	{
		Scope = node;
	}

	bool Parse();

	opString ErrorName();

	void CloneNode(FunctionPointerDeclNode* node)
	{
		stacked<opNode> scope = Scope->CloneGeneric();
		stacked<opNode> name = Name->CloneGeneric();
		node->SetScope(*scope);
		node->SetName(*name);
		node->AppendNode(scope);
		node->AppendNode(name);
	}

private:
	opNode* Scope;
	opNode* Name;
};

class FunctionPointerNode : public opNode
{
public:
	DECLARE_NODE(FunctionPointerNode,opNode,G_FUNCTION_POINTER);

	void Init()
	{
		Return = NULL;
		Decl = NULL;
		Arguments = NULL;
	}

	void PrintTransformed( opSectionStream& stream );
	void PrintOriginal(opSectionStream& stream );

	void SetReturn(opNode* ret)
	{
		Return = ret;
	}

	opNode* GetReturn()
	{
		return Return;
	}

	void SetDecl(FunctionPointerDeclNode* decl)
	{
		Decl = decl;
	}

	FunctionPointerDeclNode* GetDecl()
	{
		return Decl;
	}

	void SetArguments(opNode* args)
	{
		Arguments = args;
	}

	opNode* GetArguments()
	{
		return Arguments;
	}

	opNode* GetName()
	{
		return Decl->GetName();
	}

	opString ErrorName();
	
	void CloneNode(FunctionPointerNode* node)
	{
		stacked<opNode> ret = Return->CloneGeneric();
		stacked<FunctionPointerDeclNode> decl = Decl->Clone();
		stacked<opNode> args = Arguments->CloneGeneric();
		node->SetReturn(*ret);
		node->SetDecl(*decl);
		node->SetArguments(*args);
		node->AppendNode(ret);
		node->AppendNode(decl);
		node->AppendNode(args);
	}

private:
	opNode* Return;
	FunctionPointerDeclNode* Decl;
	opNode* Arguments;
};

///==========================================
/// Preprocessor nodes
///==========================================

class PreprocessorNode : public opNode
{
public:
	DECLARE_NODE(PreprocessorNode,opNode,T_UNKNOWN);

	void PrintTransformed( opSectionStream& stream );
	void PrintOriginal(opSectionStream& stream );
	void PrintString(opString& s);

	void SetDirectiveName(const opString& indirname)
	{
		DirectiveName = indirname;
	}

	void PrintNode(opFileStream& stream)
	{
		PrintOriginal(stream.header.body);
	}

	void PrintDialectNode(opDialectStream& stream)
	{
		PrintOriginal(stream.header.heading);
	}

	opString ErrorName();

protected:
	opString DirectiveName;
};

class PoundDefineNode : public PreprocessorNode
{
public:
	DECLARE_NODE(PoundDefineNode, PreprocessorNode, G_POUND_DEFINE);
};

class PoundElifNode : public PreprocessorNode
{
public:
	DECLARE_NODE(PoundElifNode, PreprocessorNode, G_POUND_ELIF);
};

class PoundElseNode : public PreprocessorNode
{
public:
	DECLARE_NODE(PoundElseNode, PreprocessorNode, G_POUND_ELSE);
};

class PoundEndifNode : public PreprocessorNode
{
public:
	DECLARE_NODE(PoundEndifNode, PreprocessorNode, G_POUND_ENDIF);
};

class PoundErrorNode : public PreprocessorNode
{
public:
	DECLARE_NODE(PoundErrorNode, PreprocessorNode, G_POUND_ERROR);
};

class PoundIfNode : public PreprocessorNode
{
public:
	DECLARE_NODE(PoundIfNode, PreprocessorNode, G_POUND_IF);
};

class PoundIfdefNode : public PreprocessorNode
{
public:
	DECLARE_NODE(PoundIfdefNode, PreprocessorNode, G_POUND_IFDEF);
};

class PoundIfndefNode : public PreprocessorNode
{
public:
	DECLARE_NODE(PoundIfndefNode, PreprocessorNode, G_POUND_IFNDEF);
};

class PoundImportNode : public PreprocessorNode
{
public:
	DECLARE_NODE(PoundImportNode, PreprocessorNode, G_POUND_IMPORT);
};

class PoundIncludeNode : public PreprocessorNode
{
public:
	DECLARE_NODE(PoundIncludeNode, PreprocessorNode, G_POUND_INCLUDE);
};

class PoundLineNode : public PreprocessorNode
{
public:
	DECLARE_NODE(PoundLineNode, PreprocessorNode, G_POUND_LINE);
};

class PoundPragmaNode : public PreprocessorNode
{
public:
	DECLARE_NODE(PoundPragmaNode, PreprocessorNode, G_POUND_PRAGMA);
};

class PoundUndefNode : public PreprocessorNode
{
public:
	DECLARE_NODE(PoundUndefNode, PreprocessorNode, G_POUND_UNDEF);
};

class PoundUsingNode : public PreprocessorNode
{
public:
	DECLARE_NODE(PoundUsingNode, PreprocessorNode, G_POUND_USING);
};

class PoundWarningNode : public PreprocessorNode
{
public:
	DECLARE_NODE(PoundWarningNode, PreprocessorNode, G_POUND_WARNING);
};

///==========================================
/// FundamentalTypeNode
///==========================================

class FundamentalTypeNode : public opNode
{
public:
	DECLARE_NODE(FundamentalTypeNode, opNode, G_FUNDAMENTAL_TYPE);

	void Init()
	{
		Type = NULL;
		Signed = false;
	}

	void SetType(TerminalNode* innode)
	{
		Type = innode;
	}

	void SetIsSigned(bool bSigned)
	{
		Signed = bSigned;
	}

	void PrintTransformed(opSectionStream& stream)
	{
		PrintOriginal(stream);
	}
	
	void PrintOriginal(opSectionStream& stream)
	{
		if(Signed)
			stream << "signed ";
		else
			stream << "unsigned ";
		
		Type->PrintOriginal(stream);
	}
	
	void PrintString(opString& s)
	{
		if(Signed)
			s += "signed ";
		else
			s += "unsigned ";

		Type->PrintString(s);
	}
	
	void PrintNode(opFileStream& stream )
	{
		PrintOriginal(stream.header.body);
	}

	void CloneNode(FundamentalTypeNode* node)
	{
		stacked<TerminalNode> clonetype = Type->Clone();
		node->SetType(*clonetype);
		node->AppendNode(clonetype);
		node->SetIsSigned(Signed);
	}

	opString ErrorName();

private:
	TerminalNode* Type;
	bool          Signed;
};

///==========================================
/// FriendNode
///==========================================

class FriendNode : public opNode
{
public:
	DECLARE_NODE(FriendNode, opNode, G_FRIEND);

	enum ForwardDeclType
	{
		Class,
		Struct,
		None
	};

	void Init()
	{
		Friend        = NULL;
		ForwardDecl   = NULL;
		bHasSemicolon = false;
	}

	void SetFriend(opNode* innode)
	{
		Friend = innode;
	}

	opNode* GetFriend()
	{
		return Friend;
	}

	void SetSemicolon(bool inbool)
	{
		bHasSemicolon = inbool;
	}

	bool HasSemicolon() const
	{
		return bHasSemicolon;
	}

	void SetForwardDecl(opNode* inforward)
	{
		ForwardDecl = inforward;
	}

	ForwardDeclType GetForwardDeclType();

	void PrintTransformed(opSectionStream& stream)
	{
		PrintOriginal(stream);
	}

	void PrintOriginal(opSectionStream& stream);

	opString ErrorName();

private:

	opNode* ForwardDecl;
	opNode* Friend;
	bool    bHasSemicolon;
};

///==========================================
/// CPPConstructNode
///==========================================

class CPPConstructNode : public opNode
{
public:
	DECLARE_NODE(CPPConstructNode, opNode, T_UNKNOWN);

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

	void PrintOriginal(opSectionStream& stream)
	{
		stream << Type;

		if (Name)
		{
			stream << ' ';
			Name->PrintOriginal(stream);
		}

		stream << endl;

		Body->PrintOriginal(stream);

		stream << ';' << endl;
	}

	opString ErrorName();

protected:
	opString        Type;
	TerminalNode*   Name;
	BraceBlockNode* Body;
};

//==========================================
// CPPConstructObjectNode
//==========================================

class CPPConstructInheritanceNode : public opNode
{
public:
	DECLARE_NODE(CPPConstructInheritanceNode, opNode, G_CPPCONSTRUCT_INHERITANCE);

	opString ErrorName()
	{
		return "";
	}

	void PrintString(opString& s)
	{
		PrintStringChildren(s);
	}

	void PrintOriginal(opSectionStream& stream)
	{
		PrintOriginalChildren(stream);
	}
};

class CPPConstructObjectNode : public CPPConstructNode
{
public:
	DECLARE_NODE(CPPConstructObjectNode, CPPConstructNode, T_UNKNOWN);

	void Init()
	{
		Inheritance = NULL;
	}

	void SetInheritance(CPPConstructInheritanceNode* innode)
	{
		Inheritance = innode;
	}

	CPPConstructInheritanceNode* GetInheritance()
	{
		return Inheritance;
	}

	void PrintOriginal(opSectionStream& stream)
	{
		stream << Type;

		if (Name)
		{
			stream << ' ';
			Name->PrintOriginal(stream);

			if (Inheritance)
			{
				stream << ' ';
				Inheritance->PrintOriginal(stream);
			}
		}

		stream << endl;

		Body->PrintOriginal(stream);

		stream << ';' << endl;
	}

	opString ErrorName();

private:

	CPPConstructInheritanceNode* Inheritance;
};

///==========================================
/// EnumNode
///==========================================

class EnumNode : public CPPConstructNode
{
public:
	DECLARE_NODE(EnumNode, CPPConstructNode, G_ENUM);

	void Init()
	{
		Type = "enum";
	}

	opString ErrorName()
	{
		return Super::ErrorName();
	}
};

///==========================================
/// UnionNode
///==========================================

class UnionNode : public CPPConstructNode
{
public:
	DECLARE_NODE(UnionNode, CPPConstructNode, G_UNION);

	void Init()
	{
		Type = "union";
	}

	opString ErrorName()
	{
		return Super::ErrorName();
	}
};

///==========================================
/// ClassNode
///==========================================

class ClassNode : public CPPConstructObjectNode
{
public:
	DECLARE_NODE(ClassNode, CPPConstructObjectNode, G_CLASS);

	void Init()
	{
		Type = "class";
	}

	opString ErrorName()
	{
		return Super::ErrorName();
	}
};

///==========================================
/// StructNode
///==========================================

class StructNode : public CPPConstructObjectNode
{
public:
	DECLARE_NODE(StructNode, CPPConstructObjectNode, G_STRUCT);

	void Init()
	{
		Type = "struct";
	}

	opString ErrorName()
	{
		return Super::ErrorName();
	}
};

//==========================================
// TemplatedNode
//==========================================

class TemplatedNode : public opNode
{
public:
	DECLARE_NODE(TemplatedNode, opNode, G_TEMPLATED);

	void Init()
	{
		TheTemplate = NULL;
		Templated   = NULL;
	}

	void SetTheTemplate(TemplateDeclNode* node)
	{
		TheTemplate = node;
	}

	TemplateDeclNode* GetTheTemplate()
	{
		return TheTemplate;
	}

	void SetTemplated(opNode* node)
	{
		Templated = node;
	}

	opNode* GetTemplated()
	{
		return Templated;
	}

	opString ErrorName();

	void PrintOriginal( opSectionStream& stream )
	{
		PrintOriginalChildren( stream );
	}

private:

	TemplateDeclNode* TheTemplate;
	opNode*           Templated;
};

} // end of namespace nodes

