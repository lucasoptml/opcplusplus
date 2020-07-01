///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: StatementNodes.h
/// Date: 12/19/2006
///
/// Description: Declares Nodes Related to C++/opC++ statements
///
///****************************************************************

namespace nodes
{

///==========================================
/// 3 Main Statement Classes..
///==========================================

//NOTE: we split this up into StatementBase
//		and also StatementModifierBase
//		(statementmodifierbase extends StatementBase w/ modifier support)
//		reasoning: some statements are only wrapping (the BasicStatements), so they redirect to inner statements
//		but we still want to be able to cast to a statement base class and query them for modifiers.

class StatementBase : public ConditionalSupport< opNode >
{
public:
	DECLARE_NODE(StatementBase,ConditionalSupport< opNode >,T_UNKNOWN);
	
	//ToStatementBase conversion
	virtual StatementBase* ToStatementBase() { return this; }
	virtual class VisibilityStatementNode* ToVisibilityStatementNode() { return NULL; }
	
	VisibilityMode GetVisibilityMode()
	{
		if(HasModifier(T_PUBLIC))
			return vismode_public;
		else if(HasModifier(T_PRIVATE))
			return vismode_private;
		else if(HasModifier(T_PROTECTED))
			return vismode_protected;
		return vismode_none;
	}

	virtual void SetVisibilityMode(VisibilityMode mode)
	{
		UndefinedFunctionError("SetVisibilityMode(VisibilityMode)");
	}
	
	//HasModifier query
	//certain wrapping statements must overload this.
	virtual bool HasModifier(const opString& modifiername) 
	{
		ABSTRACT_FUNCTION;
		return false; 
	}
	virtual bool HasModifier(Token modifiertoken)
	{
		ABSTRACT_FUNCTION;
		return false;
	}

	//????
	virtual opNode* FetchModifier(const opString& name)
	{
		return NULL;
	}

	virtual ValuedModifierNode* GetValuedModifier(const opString& s)
	{
		ABSTRACT_FUNCTION;
		return NULL;
	}

	virtual opNode* GetMemberName()
	{
		UndefinedFunctionError("GetMemberName()");
		return NULL;
	}
	
	virtual void FetchAllModifiers()
	{
		UndefinedFunctionError("FetchAllModifiers()");
	}

	virtual opNode* GetDataType()
	{
		UndefinedFunctionError("GetDataType()");
		return NULL;
	}

	virtual bool ProcessModifiers()
	{
		UndefinedFunctionError("ProcessModifiers()");
		return true;
	}

	//return false on error
	virtual bool ProcessDisallows(DialectCategory* CategorySettings)
	{
		UndefinedFunctionError("ProcessDisallows(DialectCategory* CategorySettings)");
		return true;
	}

	// conditional support - virtualized
	virtual void SetCondition(PreprocessorStatementNode* node)
	{
		Super::SetCondition(node);
	}
};

//extend from this class if
//a statement doesn't have modifiers
class StatementModifierlessBase : public StatementBase
{
public:
	DECLARE_NODE(StatementModifierlessBase, opNode, T_UNKNOWN);
	
	virtual inline bool HasModifier(const opString& modifiername)
	{
		return false;
	}
	
	virtual inline bool HasModifier(Token modifiertoken)
	{
		return false;
	}

	void SetVisibilityMode(VisibilityMode mode) {}

	bool ProcessModifiers() { return true; }

	bool ProcessDisallows(DialectCategory* CategorySettings)
	{
		return true;
	}

	void FetchAllModifiers() {}
};

typedef ModifierSupport< StatementBase > StatementModifierBaseParent;

class StatementModifierBase : public StatementModifierBaseParent
{
public:
	DECLARE_NODE(StatementModifierBase,StatementModifierBaseParent,T_UNKNOWN);
	
	virtual StatementModifierBase* ToStatementModifierBase() { return this; }

	void Init()
	{
		VisibilityModifier	= NULL;
		Category			= NULL;
	}
	
	void SetVisibilityModifier(TerminalNode* node)
	{
		VisibilityModifier = node;
	}
	
	void CheckForDuplicates();
	void CheckSingleVisibility();

	virtual void CheckModifiers()
	{
		modifiers->AllowOnly(T_PUBLIC,T_PRIVATE,T_PROTECTED);
	}

	virtual opNode* GetAutomaticArgument(const opString& argumentname)
	{
		UndefinedFunctionError("opNode* GetAutomaticArgument(const opString&)");
		return NULL;
	}

	void GenerateStatementModifiers();
	
	void SetVisibilityMode(VisibilityMode mode);
		
	//grabs a list of valued argument modifiers
	void BuildArguments(const vector<opString>& valuedmodifiers, vector<ValuedModifierNode*>& arguments);
	void BuildValueModifiers(vector<ValuedModifierNode*>& modifiers);

	//grabs a list of all modifiers
	void BuildModifiers(opList<opNode*>& modifiers);

	virtual bool GetModifierDescription(const opString& name, opString& description)
	{
		return false;
	}

	//call from child clone functions, or else you'll lose modifiers
	void CloneModifiers(StatementModifierBase* node)
	{
		if(modifiers)
		{
			stacked<ModifiersNode> clonemod = modifiers->Clone();
			node->SetModifiers(*clonemod);
			node->AppendNode(clonemod);
		}
		if(automodifiers)
		{
			stacked<AutoModifiersNode> clonemod = automodifiers->Clone();
			node->SetAutoModifiers(*clonemod);
			node->AppendNode(clonemod);
		}
		if(VisibilityModifier)
		{
			stacked<TerminalNode> clonevis = VisibilityModifier->Clone();
			node->SetVisibilityModifier(*clonevis);
			node->AppendNode(clonevis);
		}
	}

	bool ProcessModifiers()
	{
		OPERATIONS_START;
		
		if(GetModifiers())
		{
			CheckModifiers();
			CheckForDuplicates();
			CheckSingleVisibility();
		}

		RegisterAutoModifiers();

		OPERATIONS_END;
	}

	//generates automatic arguments
	virtual void RegisterAutoModifiers()
	{
		UndefinedFunctionError("void RegisterAutoModifiers()");
	}

	virtual opNode* GetVisibility(const opString& name)
	{
		if(VisibilityModifier)
		{
			if(VisibilityModifier->GetValue() == name)
				return VisibilityModifier;
		}

		return NULL;
	}

	void PrintXml( opXmlStream& stream );

	virtual void PrintStatementString(opString& s)
	{
		ABSTRACT_FUNCTION;
	}

protected:

	DialectCategory* GetCategorySettings()
	{
		if(!Category)
			Category = FindParent<OPObjectNode>()->GetCategorySettings();
		return Category;
	}

	//cached category
	DialectCategory* Category;

	//cached visibility
	TerminalNode* VisibilityModifier;

	//
	// Modifier Generator Functions
	//

	opNode* ModifierMemberName(const opString& name);
	opNode* ModifierAllModifiers(const opString& name);
};

///
/// StatementNode
///

class StatementNodeBase : public StatementBase
{
public:
	DECLARE_NODE(StatementNodeBase,StatementBase,T_UNKNOWN);
	
	void Init()
	{
		InnerStatement = NULL;
	}
	
	StatementBase* GetInnerStatement()
	{
		//InnerStatement should never be NULL
		//assert(InnerStatement);

		return InnerStatement;
	}
	
	void SetInnerStatement(StatementBase* statement)
	{
		assert(!InnerStatement);
		InnerStatement = statement;
	}
	
private:
	StatementBase* InnerStatement;
};

//NOTE:	statement node is the wrapping node for figuring out 
//		a ton of different statements

typedef UsingStatements<
		NullStatements<
		FuncPointerStatements<
		FuncPrototypeStatements<
		ConstructorPrototypeStatements<
		DestructorPrototypeStatements<
		DataStatements<
		StatementNodeBase
		> > > > > > > 
		StatementNodeParent;

class StatementNode : public StatementNodeParent
{
public:
	//NOTE: StatementNodeBase is the parent on purpose - if not it will call finds twice, and in the wrong order
	DECLARE_NODE(StatementNode,StatementNodeBase,G_STATEMENT);
	
	void PrintTransformed(opSectionStream& stream);
	void PrintNode(opFileStream& stream);
	
	bool Parse();
	bool PostParse();
	
	virtual bool HasModifier(const opString& modifiername)
	{
		if(GetInnerStatement())
			return GetInnerStatement()->HasModifier(modifiername);
		return false;
	}
	
	virtual bool HasModifier(Token modifiertoken)
	{
		if(GetInnerStatement())
			return GetInnerStatement()->HasModifier(modifiertoken);
		return false;
	}
	
	virtual void SetVisibilityMode(VisibilityMode mode)
	{
		if(GetInnerStatement())
			GetInnerStatement()->SetVisibilityMode(mode);
	}
	
	virtual DataStatementBase* ToDataStatementBase()
	{
		if(GetInnerStatement())
			return GetInnerStatement()->ToDataStatementBase();
		
		return NULL;
	}
	
	virtual FunctionStatementBase* ToFunctionStatementBase()
	{
		if(GetInnerStatement())
			return GetInnerStatement()->ToFunctionStatementBase();
		
		return NULL;
	}
	
	virtual ConstructorStatementBase* ToConstructorStatementBase()
	{ 
		if(GetInnerStatement())
			return GetInnerStatement()->ToConstructorStatementBase();
		
		return NULL; 
	}
	
	virtual DestructorStatementBase* ToDestructorStatementBase()
	{
		if(GetInnerStatement())
			return GetInnerStatement()->ToDestructorStatementBase();
		
		return NULL;
	}
	
	opString ErrorName();
	
	bool ProcessModifiers()
	{
		if (GetInnerStatement())
			return GetInnerStatement()->ProcessModifiers();
		return true;
	}

	void FetchAllModifiers()
	{
		if(GetInnerStatement())
			GetInnerStatement()->FetchAllModifiers();
	}
	
	bool ProcessDisallows(DialectCategory* CategorySettings)
	{
		if (GetInnerStatement())
			return GetInnerStatement()->ProcessDisallows(CategorySettings);

		return true;
	}
};

///
/// DataStatementBase
///

class DataStatementBase : public StatementModifierBase
{
public:
	DECLARE_NODE(DataStatementBase,StatementModifierBase,T_UNKNOWN);

	virtual DataStatementBase* ToDataStatementBase() { return this; }

	virtual opString GetMacroType()
	{
		ABSTRACT_FUNCTION;
		return "";
	}

	virtual void PrintMacroName(opSectionStream& stream)
	{
		ABSTRACT_FUNCTION;
	}

	//print a data mapping macro to the stream
	void PrintDataMapMacro (opSectionStream& stream, const opString& macrotype, 
		const opString& macroid, const opString& mapname);

	void PrintValuedDataMapMacro(opSectionStream& stream, const opString& macrotype, 
		const opString& macroid, const opString& mapname,
		ValuedModifierNode* modifier);

	void PrintModifierMappings (opSectionStream& stream, const opString& macrotype,
		const opString& macroid, const opString& mapname);

	void CheckModifiers();

	bool ProcessDisallows(DialectCategory* CategorySettings);

	void PrintXml(opXmlStream& stream);

	virtual bool GetModifierDescription(const opString& name, opString& description);

protected:

	opString TypeMacroString(opNode* typenode); 
	void NameMacroString(opNode* namenode, opSectionStream& stream);
};

///
/// FunctionStatementBase
///

class FunctionStatementBase : public StatementModifierBase
{
public:
	DECLARE_NODE(FunctionStatementBase,StatementModifierBase,T_UNKNOWN);
	
	virtual FunctionStatementBase* ToFunctionStatementBase() { return this; }
	
	//print a data mapping macro to the stream
	void PrintFunctionMapMacro (opSectionStream& stream, const opString& macrotype, 
								const opString& macroid, const opString& mapname);
	
	void PrintDelegateMapMacro (opSectionStream& stream, const opString& macrotype, 
								const opString& macroid, const opString& mapname);

	void PrintStateFunctionMacro(opSectionStream& stream, const opString& macrotype, 
								 const opString& macroid, const opString& mapname,
								 StateStatementNode* state);
	
	void PrintXml(opXmlStream& stream);

	virtual FunctionNode* GetFunction()
	{
		ABSTRACT_FUNCTION;
		return NULL;
	}
	
	virtual opNode* GetReturnType()
	{
		ABSTRACT_FUNCTION;
		return NULL;
	}
	
	virtual bool IsConstFunction()
	{
		return GetFunction()->IsConst();
	}
	
	virtual bool IsPureFunction()
	{
		return GetFunction()->GetAssignment()?true:false;
	}
	
	virtual opNode* GetMemberName()
	{
		return GetFunction()->GetName();
	}
	
	virtual void PrintMacroName(opSectionStream& stream) { ABSTRACT_FUNCTION; }
	virtual void PrintMacroReturnType(opSectionStream& stream) { ABSTRACT_FUNCTION; }
	virtual void PrintMacroArguments(opSectionStream& stream) { ABSTRACT_FUNCTION; }
	
	virtual void CheckModifiers();
	
	bool ProcessDisallows(DialectCategory* CategorySettings);
	
	void RegisterFunctionModifiers();
	
	void PrintString(opString& s);
	
	//grab a modifier description
	virtual bool GetModifierDescription(const opString& name, opString& description);

	///==========================================
	/// Automatic Modifier Methods
	///==========================================
	
	opNode* ModifierFunctionStatement(const opString& name);
	opNode* ModifierFunctionPure(const opString& name);
	opNode* ModifierFunctionConst(const opString& name);
	opNode* ModifierFunctionArguments(const opString& name);
	opNode* ModifierFunctionReturnType(const opString& name);
	opNode* ModifierFunctionArgumentCount(const opString& name);
	opNode* ModifierFunctionSignature(const opString& name);
	opNode* ModifierFunctionArgumentSignature(const opString& name);
//	opNode* ModifierFunctionPointerSignature(const opString& name);
//	opNode* ModifierFunctionSignatureDefaults(const opString& name);
	opNode* ModifierFunctionArgumentsDefaults(const opString& name);
	opNode* ModifierFunctionOperatorType(const opString& name);
	
private:
	
	//common function
	void PrintMethodMapMacro   (opSectionStream& stream, const opString& macrotype, 
								const opString& macroid, const opString& mapname);
};

///
/// FuncPrototypeStatementNode
///

class FuncPrototypeStatementNode : public FunctionStatementBase
{
public:
	DECLARE_NODE(FuncPrototypeStatementNode,FunctionStatementBase,G_FUNCTION_PROTOTYPE_STATEMENT);

	void Init()
	{
		FunctionPrototype = NULL;
	}

	void SetFunctionPrototype(FunctionPrototypeNode* innode)
	{
		FunctionPrototype = innode;
	}

	FunctionPrototypeNode* GetFunctionPrototype()
	{
		return FunctionPrototype;
	}

	void PrintTransformed(opSectionStream& stream );
	void PrintOriginal(opSectionStream& stream );

	void PrintNode(opFileStream& stream )
	{
		PrintTransformed(stream.header.body);
	}

	virtual void PrintMacroName(opSectionStream& stream);
	virtual void PrintMacroReturnType(opSectionStream& stream);
	virtual void PrintMacroArguments(opSectionStream& stream);

	void PrintStatementString(opString& s);

	FunctionNode* GetFunction()
	{
		return FunctionPrototype->GetFunction();
	}

	opNode* GetReturnType()
	{
		return FunctionPrototype->GetReturn();
	}

	opString ErrorName();

	///==========================================
	/// Automatic Modifier Methods
	///==========================================

	opNode* ModifierFunctionPrototype(const opString& name);

private:

	void RegisterAutoModifiers();

	FunctionPrototypeNode* FunctionPrototype;
};

///
/// FuncDefStatementNode
///

class FunctionDefinitionStatementNode : public UnInlineSupport< FunctionStatementBase >
{
public:
	DECLARE_NODE(FunctionDefinitionStatementNode,FunctionStatementBase,G_FUNCTION_DEFINITION_STATEMENT);

	void Init()
	{
		FunctionDefinition = NULL;
	}

	void SetFunctionDefinition(FunctionDefinitionNode* functiondefinition)
	{
		FunctionDefinition = functiondefinition;
	}
	
	FunctionDefinitionNode* GetFunctionDefinition()
	{
		return FunctionDefinition;
	}
	
	FunctionNode* GetFunction()
	{
		return FunctionDefinition->GetFunction();
	}

	opNode* GetReturnType()
	{
		return FunctionDefinition->GetReturn();
	}

	//printing
	void PrintTransformed(opSectionStream& stream);
	void PrintOriginal(opSectionStream& stream );
	void PrintNode(opFileStream& stream);

	//print parts of the function signature
	void PrintName(opSectionStream& stream);
	void PrintReturnType(opSectionStream& stream);
	void PrintArguments(opSectionStream& stream);

	void PrintStatementString(opString& s);

	opString GetSignatureString();


	opString ErrorName();

	///==========================================
	/// Automatic Modifier Methods
	///==========================================

	opNode* ModifierFunctionDefinition(const opString& name);
	opNode* ModifierFunctionBody(const opString& name);

private:

	void PrintPrototype(opSectionStream& stream);
	void PrintScopedDefinition(opSectionStream& stream);

	void PrintFunction(opFileStream& stream);

	template<bool bInline>
	void PrintFunctionDefinition(opFileStream& stream);

	void RegisterAutoModifiers();

	FunctionDefinitionNode* FunctionDefinition;
};

///
/// VisibilityStatementNode
///

class VisibilityStatementNode : public StatementModifierlessBase
{
public:
	DECLARE_NODE(VisibilityStatementNode,StatementModifierlessBase,G_VISIBILITY_STATEMENT);

	void Init()
	{
		Label = NULL;
	}

	void PrintNode(opFileStream& stream)
	{
		PrintTransformed(stream.header.body);
	}
	
	virtual VisibilityStatementNode* ToVisibilityStatementNode() { return this; }

	void PrintOriginal( opSectionStream& stream )
	{
		PrintOriginalChildren(stream);
	}

	void SetLabel(VisibilityLabelNode* label)
	{
		Label = label;
	}

	VisibilityMode GetVisibilityMode()
	{
		return Label->GetVisibility();
	}

	opString ErrorName() { return ""; }

	bool ProcessModifiers() { return true; }

private:
	VisibilityLabelNode* Label;
};

///
/// PreprocessorStatementNode
///

class PreprocessorStatementNode : public StatementModifierlessBase
{
public:
	DECLARE_NODE(PreprocessorStatementNode,StatementModifierlessBase,G_PREPROCESSOR_STATEMENT);
	
	void Init()
	{
		Preprocessor = NULL;
		Previous = NULL;
	}
	
	bool Parse()
	{
		PARSE_START;
		{

		}
		PARSE_END;
	}

	bool PostParse()
	{
		POSTPARSE_START;
		{
			AllowOnly(G_POUND_ELIF,
				G_POUND_ELSE,
				G_POUND_ENDIF,
				G_POUND_IF,
				G_POUND_IFDEF,
				G_POUND_IFNDEF);
		}
		POSTPARSE_END;
	}
	
	void PrintNode(opFileStream& stream)
	{
		PrintOriginal(stream.header.body);
	}
	
	void PrintOriginal(opSectionStream& stream )
	{
		PrintOriginalChildren(stream);
		stream << endl;
	}
	
	opString ErrorName();
	
	void SetPreprocessor(PreprocessorNode* innode)
	{
		Preprocessor = innode;
	}
	
	PreprocessorNode* GetPreprocessor()
	{
		return Preprocessor;
	}

	PreprocessorStatementNode* ToPreprocessorStatementNode()
	{
		return this;
	}
	
	void SetPrevDirective(PreprocessorStatementNode* node)
	{
		Previous = node;
	}

	PreprocessorStatementNode* GetPrevPreprocessor()
	{
		return Previous;
	}

	//prints the conditions and previous conditions, and returns the number of #if conditions
	int PrintCondition(opSectionStream& stream);
	
private:
	PreprocessorNode* Preprocessor;

	PreprocessorStatementNode* Previous;
};

///
/// StateStatement
///

class StateStatementNode : public StatementModifierBase
{
public:
	DECLARE_NODE(StateStatementNode,StatementModifierBase,G_STATE_STATEMENT);
	
	void Init()
	{
		State = NULL;
	}

	void SetState(StateNode* state)
	{
		State = state;
	}

	StateNode* GetState()
	{
		return State;
	}
	
	void PrintTransformed(opSectionStream& stream);

	void PrintNode(opFileStream& stream);

	//print a state mapping macro to the stream
// 	void PrintStateMapMacro(opSectionStream& stream, const opString& macrotype, 
// 							const opString& macroid, const opString& mapname);
	
//	typedef OPObjectBodyNode::statefunctionpair statefunctionpair;

// 	void GatherStateFunctions(vector<statefunctionpair> & statefunctions);
	
	opString ErrorName();

private:
	StateNode* State;
};

///==========================================
/// Inner Statement Classes..
///==========================================

///
/// NullStatementNode
///

class NullStatementNode : public StatementModifierlessBase
{
public:
	DECLARE_NODE(NullStatementNode,StatementModifierlessBase,G_NULL_STATEMENT);

	void PrintNode(opFileStream& stream )
	{
		
	}

	opString ErrorName() { return ""; }
};

///
/// FriendStatementNode
///

class FriendStatementNode : public StatementModifierlessBase
{
public:
	DECLARE_NODE(FriendStatementNode,StatementModifierlessBase,G_FRIEND_STATEMENT);

	void Init()
	{
		Friend = NULL;
	}

	void SetFriend(FriendNode* innode)
	{
		Friend = innode;
	}

	FriendNode* GetFriend()
	{
		return Friend;
	}

	void PrintNode(opFileStream& stream )
	{
		PrintTransformed(stream.header.body);

		stream << endl;
	}

	bool Parse();

	opString ErrorName();

private:
	FriendNode* Friend;
};

///==========================================
/// UsingStatement
///==========================================

class UsingStatementNode : public StatementModifierlessBase
{
public:
	DECLARE_NODE(UsingStatementNode,StatementModifierlessBase,G_USING_STATEMENT);

	void Init()
	{
		Using = NULL;
	}

	UsingNode* GetUsing()
	{
		return Using;
	}

	void PrintNode(opFileStream& stream )
	{
		PrintTransformed(stream.header.body);
	}

	bool Parse();

	opString ErrorName();

private:
	UsingNode* Using;
};

///
/// TypedefStatementNode
///

class TypedefStatementNode : public StatementModifierBase
{
public:
	DECLARE_NODE(TypedefStatementNode,StatementModifierBase,G_TYPEDEF_STATEMENT);

	void Init()
	{
		Typedef = NULL;
	}
	
	void PrintNode(opFileStream& stream )
	{
		Typedef->PrintTransformed( stream.header.body );

		stream.header.body << endl;
	}

	/*=== set ===*/

	void SetTypedef(TypedefNode* innode)
	{
		Typedef = innode;
	}

	/*=== get ===*/

	TypedefNode* GetTypedef()
	{
		return Typedef;
	}

	/*=== utility ===*/

	opString ErrorName();

	/*=== overrides ===*/

	virtual void RegisterAutoModifiers() {}
	bool ProcessDisallows(DialectCategory* CategorySettings) { return true; }

	void PrintXml( opXmlStream& stream )
	{
		
	}

private:
	TypedefNode* Typedef;
};

///
/// FuncPointerStatementNode
///

class FuncPointerStatementNode : public DataStatementBase
{
public:
	DECLARE_NODE(FuncPointerStatementNode,DataStatementBase,G_FUNCTION_POINTER_STATEMENT);

	void Init()
	{
		FunctionPointer = NULL;
	}
		
	virtual opNode* GetMemberName()
	{
		return FunctionPointer->GetName();
	}

	void SetFunctionPointer(FunctionPointerNode* innode)
	{
		FunctionPointer = innode;
	}

	FunctionPointerNode* GetFunctionPointer()
	{
		return FunctionPointer;
	}

	void PrintTransformed(opSectionStream& stream);
	void PrintOriginal(opSectionStream& stream );
	void PrintString(opString& s);

	void PrintNode(opFileStream& stream )
	{
		PrintTransformed(stream.header.body);
	}

	opString GetMacroType()
	{
		return "FUNCTIONPOINTER";
	}

	void PrintMacroName(opSectionStream& stream)
	{
		return FunctionPointer->GetName()->PrintOriginal(stream);
	}

	opString ErrorName();

	void PrintStatementString(opString& s);

	///==========================================
	/// Automatic Modifier Methods
	///==========================================

	opNode* ModifierDataStatement(const opString& name);
	opNode* ModifierFunctionPointer(const opString& name);

private:
	
	virtual void RegisterAutoModifiers();

	FunctionPointerNode* FunctionPointer;
};

//
// TemplateStatementNode
//

class TemplateStatementNode : public StatementModifierBase
{
public:
	DECLARE_NODE(TemplateStatementNode,StatementModifierBase,G_TEMPLATE_STATEMENT);
	
	void Init()
	{
		Templated = NULL;
	}

	void SetTemplated(TemplatedNode* node)
	{
		Templated = node;
	}

	TemplatedNode* GetTemplated()
	{
		return Templated;
	}

	void PrintNode(opFileStream& stream)
	{
		PrintOriginal( stream.header.body );
	}

	void PrintOriginal(opSectionStream& stream);

	virtual void CheckModifiers()
	{
		modifiers->AllowOnly(T_PUBLIC,T_PRIVATE,T_PROTECTED,G_CPLUSPLUS);
	}

	virtual void RegisterAutoModifiers() {}

	bool ProcessDisallows(DialectCategory* CategorySettings)
	{
		return true;
	}

	opString ErrorName();

	void PrintXml( opXmlStream& stream )
	{

	}

private:
	TemplatedNode* Templated;
};

///
/// OPEnumStatementNode
///

class OPEnumStatementNode : public StatementModifierBase
{
public:
	DECLARE_NODE(OPEnumStatementNode,StatementModifierBase,G_OPENUM_STATEMENT);
	
	void Init()
	{
		Enum = NULL;
	}

	void SetEnum(OPEnumNode* enumnode)
	{
		Enum = enumnode;
	}

	OPEnumNode* GetEnum()
	{
		return Enum;
	}
	
	opNode* GetMemberName()
	{
		return Enum->GetName();
	}
	
	void PrintNode(opFileStream& stream)
	{
		if ( modifiers )
			modifiers->PrintBuiltIn( stream.header.body );

		Enum->PrintNode(stream);
	}

	virtual void PrintStatementString(opString& s)
	{
		Enum->GetIdentifier()->PrintString(s);
		s += ' ';
		Enum->GetName()->PrintString(s);
	}
	
	//no modifiers generated
	virtual void RegisterAutoModifiers() {}

	virtual void CheckModifiers()
	{
		modifiers->AllowOnly(T_PUBLIC,T_PRIVATE,T_PROTECTED,G_CPLUSPLUS);
	}
	
	bool ProcessDisallows(DialectCategory* CategorySettings)
	{
		return true;
	}

	opString ErrorName();

	// conditional support
	void SetCondition(PreprocessorStatementNode* node)
	{
		if(Enum)
			Enum->SetCondition(node);
	}

private:
	OPEnumNode* Enum;
};

///
/// OPObjectStatementNode
///

//TODO: this shouldn't grab modifiers without support really.
class OPObjectStatementNode : public StatementModifierBase
{
public:
	DECLARE_NODE(OPObjectStatementNode,StatementModifierBase,G_OPOBJECT_STATEMENT);

	void Init()
	{
		Object = NULL;
	}

	void SetObject(OPObjectNode* object)
	{
		Object = object;
	}

	OPObjectNode* GetObject()
	{
		return Object;
	}

	void PrintNode(opFileStream& stream)
	{
		if ( modifiers )
			modifiers->PrintBuiltIn( stream.header.body );

		Object->PrintNode(stream);
	}

	//TODO: need to do this probably... for nested stuff
	virtual void RegisterAutoModifiers() {}

	bool ProcessDisallows(DialectCategory* CategorySettings)
	{
		return true;
	}

	opString ErrorName();

	void PrintXml(opXmlStream& stream);

	virtual void SetCondition(PreprocessorStatementNode* node)
	{
		if(Object)
			Object->SetCondition(node);
	}

	virtual void CheckModifiers()
	{
		modifiers->AllowOnly(T_PUBLIC,T_PRIVATE,T_PROTECTED,G_CPLUSPLUS);
	}

private:
	OPObjectNode* Object;
};

///
/// DataBitsNode
///

class DataBitsNode : public opNode
{
public:
	DECLARE_NODE(DataBitsNode,opNode,G_DATA_BITS);

	void Init()
	{
		Number = NULL;
	}

	void SetNumber(TerminalNode* number)
	{
		Number = number;
	}

	TerminalNode* GetNumber()
	{
		return Number;
	}

	opString ErrorName() { return ""; }

private:
	TerminalNode* Number;
};

///
/// DataDeclarationNode
///

class DataDeclarationNode : public opNode
{
public:
	DECLARE_NODE(DataDeclarationNode,opNode,G_DATA_DECLARATION);

	//we'll stick modifiers elsewhere.

	void Init()
	{
		Name	  = NULL;
		Type	  = NULL;
		Bits	  = NULL;
	}

	void SetName(opNode* name)
	{
		Name = name;
	}

	void SetType(opNode* type)
	{
		Type = type;
	}

	void SetBits(DataBitsNode* bits)
	{
		Bits = bits;
	}

	opNode* GetName()
	{
		return Name;
	}

	opNode* GetType()
	{
		return Type;
	}

	DataBitsNode* GetBits()
	{
		return Bits;
	}

	void PrintTransformed(opSectionStream& stream )
	{
		PrintOriginal(stream);
	}

	void PrintOriginal(opSectionStream& stream );
	void PrintString(opString& s);

	//parse the nodes into name,type,bits
	bool Parse();

	void CloneNode(DataDeclarationNode* node)
	{
		stacked<opNode> name = Name->CloneGeneric();
		stacked<opNode> type = Type->CloneGeneric();
		node->SetName(*name);
		node->SetType(*type);
		node->AppendNode(type);
		node->AppendNode(name);

		if(Bits)
		{
			stacked<DataBitsNode> bits = Bits->Clone();
			node->SetBits(*bits);
			node->AppendNode(bits);
		}

	}

	opString ErrorName();

	void ParseTypeName();

	opNode*		   Name;
	opNode*		   Type;
	DataBitsNode*  Bits;
};

///
/// DataInitializationNode
///

class DataInitializationNode : public opNode
{
public:
	DECLARE_NODE(DataInitializationNode,opNode,G_DATA_INITIALIZATION);

	void CloneNode(opNode* newnode)
	{
		CloneChildren(newnode);
	}

	opString ErrorName();

	void PrintOriginal(opSectionStream& stream )
	{
		PrintOriginalChildren(stream);
	}

	void PrintString(opString& s)
	{
		PrintStringChildren(s);
	}
};

///
/// DataStatementNode
///

class DataStatementNode : public DataStatementBase
{
public:
	DECLARE_NODE(DataStatementNode,DataStatementBase,G_DATA_STATEMENT);

	void Init()
	{
		Declaration = NULL;
		Initialization = NULL;
	}

	void SetDeclaration(DataDeclarationNode* node)
	{
		Declaration = node;
	}

	void SetInitialization(DataInitializationNode* node)
	{
		Initialization = node;
	}

	opNode* GetName()
	{
		if(Declaration)
			return Declaration->GetName();
		return NULL;
	}

	opNode* GetType()
	{
		if(Declaration)
			return Declaration->GetType();
		return NULL;
	}

	void SetType(opNode* type)
	{
		Declaration->SetType(type);
	}
	
	DataBitsNode* GetBits()
	{
		if(Declaration)
			return Declaration->GetBits();
		return NULL;
	}

	opNode* GetMemberName()
	{
		//special case for arrays
		if(ArrayNode* node = node_cast<ArrayNode>(GetName()))
		{
			return node->GetName();
		}

		return GetName();
	}

	opNode* GetDataType()
	{
		return GetType();
	}

	void PrintNode(opFileStream& stream )
	{
		PrintTransformed(stream.header.body);
	}

	void PrintOriginal(opSectionStream& stream);
	void PrintTransformed(opSectionStream& stream);
	void PrintString(opString& s);

	void PrintStatementString(opString& s);

	void RegisterAutoModifiers();

	opString ErrorName();

	void CloneNode(DataStatementNode* node)
	{
		CloneModifiers(node);
		
		stacked<DataDeclarationNode> decl = Declaration->Clone();
		node->SetDeclaration(*decl);
		node->AppendNode(decl);

		if(Initialization)
		{
			stacked<DataInitializationNode> init = Initialization->Clone();
			node->SetInitialization(*init);
			node->AppendNode(init);
		}
	}

	// Modifier Generator Functions
	opNode* ModifierDataType(const opString& name);
	opNode* ModifierDataStatement(const opString& name);
	opNode* ModifierDataBits(const opString& name);
	opNode* ModifierDataInitialized(const opString& name);
	opNode* ModifierDataReference(const opString& name);
	opNode* ModifierDataPointer(const opString& name);
	opNode* ModifierDataPointerLevel(const opString& name);
	opNode* ModifierDataBaseType(const opString& name);
	opNode* ModifierDataTemplate(const opString& name);
	opNode* ModifierDataQualified(const opString& name);
	opNode* ModifierDataBasic(const opString& name);
	opNode* ModifierDataFullType(const opString& name);
	opNode* ModifierDataArray(const opString& name);
	opNode* ModifierDataArrayBrackets(const opString& name);
	opNode* ModifierDataArrayCommas(const opString& name);

private:
	DataDeclarationNode* Declaration;
	DataInitializationNode* Initialization;
};

class ConstructorStatementBase : public FunctionStatementBase
{
public:
	DECLARE_NODE(ConstructorStatementBase,FunctionStatementBase,T_UNKNOWN);

	ConstructorStatementBase* ToConstructorStatementBase() { return this; }

	virtual FunctionNode* GetFunction()
	{
		return GetConstructor()->GetFunction();
	}

	virtual opNode* GetReturnType()
	{
		return NULL;
	}

	virtual ConstructorNode* GetConstructor()
	{
		ABSTRACT_FUNCTION;
		return NULL;
	}

	opNode* ModifierFunctionConstructor(const opString& name);

	void RegisterConstructorModifiers();

	void CheckModifiers();
};

///
/// Constructor Definition Statement Node
///

class ConstructorDefinitionStatementNode : public UnInlineSupport< ConstructorStatementBase >
{
public:
	DECLARE_NODE(ConstructorDefinitionStatementNode,ConstructorStatementBase,G_CONSTRUCTOR_DEFINITION_STATEMENT);
	
	void Init()
	{
		ConstructorDefinition = NULL;
	}

	void SetConstructorDefinition(ConstructorDefinitionNode* node)
	{
		ConstructorDefinition = node;
	}
	
	void PrintNode(opFileStream& file);
	void PrintOriginal(opSectionStream& stream);

	virtual ConstructorNode* GetConstructor()
	{
		return ConstructorDefinition->GetConstructor();
	}

	virtual void RegisterAutoModifiers();
	
	opNode* ModifierFunctionBody(const opString& name);

	opString ErrorName();

	void PrintStatementString(opString& s);

private:
	void PrintPrototype(opSectionStream& stream);
	void PrintScopedDefinition(opSectionStream& stream);

private:
	ConstructorDefinitionNode* ConstructorDefinition;
};

///
/// Constructor Prototype Statement Node
///

class ConstructorPrototypeStatementNode : public ConstructorStatementBase
{
public:
	DECLARE_NODE(ConstructorPrototypeStatementNode,ConstructorStatementBase,G_CONSTRUCTOR_PROTOTYPE_STATEMENT);
	
	void Init()
	{
		Constructor = NULL;
	}

	void SetConstructorPrototype(ConstructorPrototypeNode* node)
	{
		Constructor = node;
	}

	void PrintNode(opFileStream& stream);
	void PrintOriginal(opSectionStream& stream);

	void PrintStatementString(opString& s);

	//no modifiers generated
	virtual void RegisterAutoModifiers();

	virtual ConstructorNode* GetConstructor()
	{
		return Constructor->GetConstructor();
	}

	opString ErrorName();

private:
	ConstructorPrototypeNode* Constructor;
};


class DestructorStatementBase : public FunctionStatementBase
{
public:
	DECLARE_NODE(DestructorStatementBase,FunctionStatementBase,T_UNKNOWN);

	DestructorStatementBase* ToDestructorStatementBase() { return this; }

	virtual DestructorNode* GetDestructor()
	{
		ABSTRACT_FUNCTION;
		return NULL;
	}

	virtual FunctionNode* GetFunction()
	{
		return GetDestructor()->GetFunction();
	}

	virtual opNode* GetReturnType()
	{
		return NULL;
	}

	void RegisterDestructorModifiers();

	opNode* ModifierFunctionDestructor(const opString& name);

	void CheckModifiers();
};


///
/// Destructor Definition Statement Node
///

class DestructorDefinitionStatementNode : public UnInlineSupport< DestructorStatementBase >
{
public:
	DECLARE_NODE(DestructorDefinitionStatementNode,DestructorStatementBase,G_DESTRUCTOR_DEFINITION_STATEMENT);

	void Init()
	{
		DestructorDefinition = NULL;
	}

	void SetDestructorDefinition(DestructorDefinitionNode* node)
	{
		DestructorDefinition = node;
	}

	virtual DestructorNode* GetDestructor()
	{
		return DestructorDefinition->GetDestructor();
	}

	void PrintNode(opFileStream& file);
	void PrintOriginal(opSectionStream& stream);

	void PrintStatementString(opString& s);

	virtual void RegisterAutoModifiers();

	opNode* ModifierFunctionBody(const opString& name);

	opString ErrorName();

private:
	void PrintPrototype(opSectionStream& stream);
	void PrintScopedDefinition(opSectionStream& stream);

private:
	DestructorDefinitionNode* DestructorDefinition;
};

///
/// Constructor Prototype Statement Node
///

class DestructorPrototypeStatementNode : public DestructorStatementBase
{
public:
	DECLARE_NODE(DestructorPrototypeStatementNode,DestructorStatementBase,G_CONSTRUCTOR_PROTOTYPE_STATEMENT);

	void Init()
	{
		Destructor = NULL;
	}

	void SetDestructorPrototype(DestructorPrototypeNode* node)
	{
		Destructor = node;
	}

	virtual DestructorNode* GetDestructor()
	{
		return Destructor->GetDestructor();
	}

	void PrintNode(opFileStream& stream);
	void PrintOriginal(opSectionStream& stream);

	void PrintStatementString(opString& s);

	//no modifiers generated
	virtual void RegisterAutoModifiers();

	opString ErrorName();

private:
	DestructorPrototypeNode* Destructor;
};

///==========================================
/// CPPConstructStatementNode
///==========================================

class CPPConstructStatementNode : public StatementModifierBase
{
public:
	DECLARE_NODE(CPPConstructStatementNode,StatementModifierBase,G_CPPCONSTRUCT_STATEMENT);

	void Init()
	{
		Construct = NULL;
	}

	void PrintNode(opFileStream& stream)
	{
		if ( modifiers )
			modifiers->PrintBuiltIn( stream.header.body );

		Construct->PrintOriginal( stream.header.body );
	}

	void SetConstruct(CPPConstructNode* innode)
	{
		Construct = innode;
	}

	CPPConstructNode* GetConstruct()
	{
		return Construct;
	}

	virtual void RegisterAutoModifiers() {}

	bool ProcessDisallows(DialectCategory* CategorySettings)
	{
		return true;
	}

	virtual void CheckModifiers()
	{
		modifiers->AllowOnly(T_PUBLIC,T_PRIVATE,T_PROTECTED,G_CPLUSPLUS);
	}

	void PrintXml( opXmlStream& stream )
	{
		PrintXmlChildren( stream );
	}

	opString ErrorName();

private:
	CPPConstructNode* Construct;
};

} // end namespace nodes

