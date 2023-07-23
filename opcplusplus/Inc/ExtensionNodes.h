///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: ExtensionNodes.h
/// Date: 11/26/2006
///
/// Description:
///
/// This file contains our extended node classes.  Each node class 
/// represents an extended grammar in the opC++ syntax.
///****************************************************************

#include "ModifierSupport.h"
#include "DialectNodes.h"

namespace nodes
{

///==========================================
/// OPTypeBase
///==========================================

// shared printing and modifier support (between opObjectnode and opEnumnode)
class OPTypeBase : public ConditionalSupport< ModifierSupport< opNode > >
{
public:
	OPTypeBase()
	{
		Init();
	}
	
	void Init()
	{
		NamespaceStart	= NULL;
		NamespaceEnd	= NULL;
		ScopeArgument	= NULL;

		AltNamespaceStart = NULL;
		AltNamespaceEnd   = NULL;
		AltScopeArgument  = NULL;
	}

	enum NoteType
	{
		Note,
		Start,
		End,
	};

	//printing utility functions
	void PrintNote(DialectNote& notenode, opSectionStream& stream, NoteType type = Note, int map_count = 0);
	void PrintNote(DialectLocationBase& location, DialectNote& noteitem, opSectionStream& stream);

	//argument fetch functions
	virtual opNode* GetContextArgument(const opString& argumentname);
	opNode* GetModifierArgument(const opString& argumentname, vector<ValuedModifierNode*>& modifiervalues);
	
	opNode* GetStartArgument(const opString& argumentname, stackedgroup& tempmodifiers);
	opNode* GetMappingArgument(const opString& argumentname, int map_index, stackedgroup& tempmodifiers);
	opNode* GetEndArgument(const opString& argumentname, int map_count, stackedgroup& tempmodifiers);

	virtual DialectTypeBase* GetTypeSettings() = NULL;

	virtual void RegisterAutoModifiers();

	// modifier generator functions
	opNode* ModifierScope(const opString& name);
	opNode* ModifierAltScope(const opString& name);
	opNode* ModifierNamespaceStart(const opString& name);
	opNode* ModifierNamespaceEnd(const opString& name);
	opNode* ModifierAltNamespaceStart(const opString& name);
	opNode* ModifierAltNamespaceEnd(const opString& name);
	opNode* ModifierInnerType(const opString& name);

private:
	//cached arguments
	opNode* ScopeArgument;
	opNode* AltScopeArgument;
	opNode* NamespaceStart;
	opNode* AltNamespaceStart;
	opNode* NamespaceEnd;
	opNode* AltNamespaceEnd;
};

///==========================================
/// OPEnum nodes
///==========================================

class OPEnumBodyNode;
class OPEnumElementNode;

class OPEnumNode : public OPTypeBase
{
public:
	DECLARE_NODE(OPEnumNode,opNode,G_OPENUM);

	void Init()
	{
		Name = NULL;
		Body = NULL;
		EnumSettings = NULL;
		Identifier = NULL;
	}

	void PrintNode( opFileStream& stream );

	void SetName(TerminalNode* inname)
	{
		Name = inname;
	}

	void SetBody(OPEnumBodyNode* inbody)
	{
		Body = inbody;
	}

	void SetIdentifier(TerminalNode* identifier)
	{
		Identifier = identifier;
	}

	TerminalNode* GetIdentifier()
	{
		return Identifier;
	}

	TerminalNode* GetName()
	{
		return Name;
	}

	OPEnumBodyNode* GetBody()
	{
		return Body;
	}

	opString ErrorName();

	DialectTypeBase* GetTypeSettings()
	{
		return GetEnumSettings();
	}

	DialectEnumeration* GetEnumSettings();

	//generate modifiers
	bool PostParse();

private:

	//note printing
	void PrintLocationNotes(DialectEnumLocation* settings, opSectionStream& stream);

	void PrintEnumMap(opSectionStream& stream,
		DialectEnumeration& enumeration, 
		DialectEnumLocation& location, 
		DialectEnumMap& mapinfo);

	void PrintEnumMapStart(opSectionStream& stream,
		DialectEnumeration& enumeration, 
		DialectEnumLocation& location, 
		DialectEnumMap& mapinfo);

	void PrintEnumMapping(opSectionStream& stream,
		DialectEnumeration& enumeration, 
		DialectEnumLocation& location,
		DialectEnumMap& mapinfo,
		OPEnumElementNode* element,
		int map_index);

	void PrintEnumMapEnd(opSectionStream& stream,
		DialectEnumeration& enumeration, 
		DialectEnumLocation& location, 
		DialectEnumMap& mapinfo,
		int map_count);	


	//argument fetch
	opNode* GetContextArgument(const opString& argumentname);
	opNode* GetElementArgument(const opString& argumentname, OPEnumElementNode* element);

private:
	TerminalNode*   Identifier;
	TerminalNode*   Name;
	OPEnumBodyNode* Body;

	DialectEnumeration* EnumSettings;
};

typedef Clean< opNode > OPEnumBodyNodeParent;

class OPEnumBodyNode : public OPEnumBodyNodeParent
{
public:
	DECLARE_NODE(OPEnumBodyNode,OPEnumBodyNodeParent,G_OPENUM_BODY);

	bool Parse();

	void AddElement(OPEnumElementNode* elt)
	{
		Elements.push_back(elt);
	}

	int GetNumElements()
	{
		return (int)Elements.size();
	}
	
	OPEnumElementNode* GetLastElement()
	{
		return Elements.back();
	}

	opString ErrorName();

private:
	vector<OPEnumElementNode*> Elements;
};

class OPEnumInitializationNode : public opNode
{
public:
	DECLARE_NODE(OPEnumInitializationNode,opNode,G_OPENUM_INITIALIZATION);
	
	void PrintOriginal( opSectionStream& stream )
	{
		PrintOriginalChildren(stream);
	}

	void PrintString(opString& s)
	{
		PrintStringChildren(s);
	}

	void CloneNode(OPEnumInitializationNode* node)
	{
		CloneChildren(node);
	}

	opString ErrorName() { return ""; }
};

typedef Clean< opNode > OPEnumElementNodeParent;

class OPEnumElementNode : public OPEnumElementNodeParent 
{
public:
	DECLARE_NODE(OPEnumElementNode,OPEnumElementNodeParent,G_OPENUM_ELEMENT);

	void Init()
	{
		Name = NULL;
		Initialization = NULL;
	}

	void PrintTransformed( opSectionStream& stream );
	void PrintOriginal( opSectionStream& stream );
	bool Parse();

	TerminalNode* GetName()
	{
		return Name;
	}

	OPEnumInitializationNode* GetInitialization()
	{
		return Initialization;
	}

	opString ErrorName();

private:
	TerminalNode*			  Name;
	OPEnumInitializationNode* Initialization;
};

///==========================================
/// OPObject nodes
///==========================================

class OPObjectInheritanceNode;
class OPObjectBodyNode;

typedef OPTypeBase OPObjectNodeParent;

class OPObjectNode : public OPObjectNodeParent
{
public:
	DECLARE_NODE(OPObjectNode,OPObjectNodeParent,G_OPOBJECT);
	
	void Init()
	{
		MiddleModifiers  = NULL;
		Inheritance		 = NULL;
		CategorySettings = NULL;
		Name			 = NULL;
		ParentArgument	 = NULL;
		ClassArgument	 = NULL;
		AltClassArgument = NULL;
	}
	
	TerminalNode* GetName()
	{
		return Name;
	}	
	
	void SetName(TerminalNode* t)
	{
		Name = t;
	}

	CPlusPlusNode* GetMiddleModifiers()
	{
		return MiddleModifiers;
	}

	void SetMiddleModifiers(CPlusPlusNode* innode)
	{
		MiddleModifiers = innode;
	}
	
	OPObjectInheritanceNode* GetInheritance()
	{
		return Inheritance;
	}
	
	void SetInheritance(OPObjectInheritanceNode* i)
	{
		Inheritance = i;
	}	

	void SetBody(OPObjectBodyNode* body)
	{
		Body = body;
	}

	OPObjectBodyNode* GetBody()
	{
		return Body;
	}

	void SetCategory(opNode* category);
	
	void SetCategoryName(const opString& s)
	{
		CategoryName = s;
	}
	
	const opString& GetCategoryName() 
	{
		return CategoryName;
	}

	void PrintNode( opFileStream& stream );

	void PrintXml(opXmlStream& stream );

	void BuildClassScope(opString& outscope)
	{
		if(GetParent())
			GetParent()->BuildClassScope(outscope);
		
		//only want to separate if there's already a class noted
		if(outscope.Size())
			outscope += "::";

		outscope += GetClassName();
	}
	
	//string building functions
	opString GetClassName()
	{
		return Name->GetValue();
	}

	opString GetScope()
	{
		opString scope = GetClassScope();
		scope += "::";
		return scope;
	}

	opString ErrorName();

	opString GetScopedClassName()
	{
		opString scope = GetClassScope();
		if(scope.Size())
			return scope + "::" + GetClassName();
		return GetClassName();
	}

	void PrintClassName(opSectionStream& stream)
	{
		Name->PrintOriginal(stream);
	}

	void PrintScopedClassName(opSectionStream& stream)
	{
		//NOTE: we have to print via the name node so we get debugging 
		opString classscope = GetClassScope();
		if(classscope.Size())
		{	
			stream << classscope;
			stream << "::";
		}

		PrintClassName(stream);
	}
	
	void PrintInheritance(opSectionStream& stream);
	
	//always returns something (parent or category_baseclass)
	opString GetParentName();
	
	DialectTypeBase* GetTypeSettings()
	{
		return GetCategorySettings();
	}

	DialectCategory* GetCategorySettings();

	///==========================================
	/// Queries
	///==========================================
	
	bool HasPureFunctions();
	bool HasConstructor();
	bool HasDestructor();
	bool HasSimpleConstructor();
	bool HasComplexConstructor();
	
	///==========================================
	/// Automatic Modifier Methods
	///==========================================

	/*=== class (instance) ===*/

	opNode* ModifierClassName(const opString& name);
	opNode* ModifierAltClassName(const opString& name);
	opNode* ModifierParentName(const opString& name);

	/*=== category ===*/

	opNode* ModifierCategoryStatement(const opString& name);
	opNode* ModifierCategoryAbstract(const opString& name);
	opNode* ModifierCategoryBaseclass(const opString& name);
	opNode* ModifierCategorySingleInheritance(const opString& name);
	opNode* ModifierCategoryMultipleInheritance(const opString& name);
	opNode* ModifierCategoryHasConstructor(const opString& name);
	opNode* ModifierCategoryHasDestructor(const opString& name);
	opNode* ModifierCategoryHasSimpleConstructor(const opString& name);
	opNode* ModifierCategoryHasComplexConstructor(const opString& name);
//	opNode* ModifierCategoryName(const opString& name);

	/**** printing utility functions ****/

	void PrintVisibility(VisibilityMode mode, opSectionStream& stream);
	
	/**** printing ****/
public:
	
	
	void PrintDataMapNote ( opSectionStream& stream, const opString& macrotype, const opString& macroid,
		const opString& mapname, const opString& note );
	
private:
	
	void PrintLocationNotes(DialectLocation* settings, opSectionStream& stream);
	
	/**** print maps ****/
	
	void PrintMap(DialectLocation& location, DialectMap& mapitem, opSectionStream& stream);
	
	/**** print data notes ****/
	void PrintDataMap(opSectionStream& stream,
		DialectCategory& category, 
		DialectLocation& location, 
		DialectMap& mapinfo);

	void PrintDataMapStart(opSectionStream& stream,
		DialectCategory& category, 
		DialectLocation& location, 
		DialectMap& mapinfo);

	void PrintDataMapping(opSectionStream& stream,
		DialectCategory& category,
		DialectLocation& location,
		DialectMap& mapinfo,
		DataStatementBase* statement,
		int map_index);

	void PrintDataMapEnd(opSectionStream& stream,
		DialectCategory& category, 
		DialectLocation& location, 
		DialectMap& mapinfo,
		int map_count);	

 	/**** print function notes ****/
 	void PrintFunctionMap(opSectionStream& stream,
 						  DialectCategory& category, 
 						  DialectLocation& location, 
 						  DialectMap& mapinfo);
 	
 	void PrintFunctionMapStart(opSectionStream& stream,
 							   DialectCategory& category, 
 							   DialectLocation& location, 
 							   DialectMap& mapinfo);

	void PrintFunctionMapping(opSectionStream& stream,
							  DialectCategory& category,
							  DialectLocation& location,
							  DialectMap& mapinfo,
							  FunctionStatementBase* statement,
							  int map_index);

 	void PrintFunctionMapEnd(opSectionStream& stream,
 							 DialectCategory& category, 
 							 DialectLocation& location, 
 							 DialectMap& mapinfo,
							 int map_count);

	/**** argument fetch functions ****/
	opNode* GetContextArgument(const opString& argumentname);
	opNode* GetStatementArgument(const opString& argumentname, StatementBase* statement, stackedgroup& tempmodifiers);
	
	friend class FunctionDefinitionStatementNode;

	//cached node
	opNode* ParentArgument;
	opNode* ClassArgument;
	opNode* AltClassArgument;

	/**** note printing utility functions ****/
	
	//inject notes
// 	void PrintInjectNoteStart(opSectionStream& stream);
// 	void PrintInjectNoteEnd(opSectionStream& stream);

	//location query
	bool IsInsideClass()
	{
		return IsInside(G_OPOBJECT);
	}

	/**** post parse ****/
	bool PostParse();

	void ProcessVisibility();
	void ProcessStatements();
	bool ProcessDisallows();
	bool ProcessObjectDisallows();

	void RegisterAutoModifiers();

	DialectCategory* CategorySettings;

	opString CategoryName;

	//vector<opString> InjectNotes;
	
	TerminalNode*            Name;
	CPlusPlusNode*           MiddleModifiers;
	OPObjectInheritanceNode* Inheritance;
	OPObjectBodyNode*        Body;
};

typedef context::Inheritance< opNode > OPInheritanceNodeParent;

class OPObjectInheritanceNode : public OPInheritanceNodeParent
{
public:
	DECLARE_NODE(OPObjectInheritanceNode,OPInheritanceNodeParent,G_OPOBJECT_INHERITANCE);

	bool Parse();

	void Init()
	{
		Object = NULL;
	}

	struct Parent
	{
		Parent()
		{
			Virtual = NULL;
			Visibility = NULL;
			Type = NULL;
		}
		
		TerminalNode* Virtual;
		TerminalNode* Visibility;
		opNode*		  Type;
	};

	void SetObject(OPObjectNode* node)
	{
		Object = node;
	}

	const opArray<Parent>& GetParents()
	{
		return Parents;
	}

	virtual void PrintTransformed( opSectionStream& stream );

	opString ErrorName();

	void PrintParentNames(opString& s);
	//void PrintParentNames(opSectionStream& stream);

	void PrintInheritance(opString& s);
	void PrintInheritance(opSectionStream& stream);

	/**** node operations ****/
	void CloneParentNames(opNode* insertinto);
	void CloneInheritance(opNode* insertinto);

private:

	friend OPObjectNode;
	
	opArray<Parent> Parents;

	OPObjectNode* Object;
};

typedef context::Declaration< opNode > OPObjectBodyNodeParent;

class OPObjectBodyNode : public OPObjectBodyNodeParent
{
public:
	DECLARE_NODE(OPObjectBodyNode, OPObjectBodyNodeParent, G_OPOBJECT_BODY);
	
	void Init()
	{		
		Object = NULL;
	}

	void SetObject(OPObjectNode* node)
	{
		Object = node;
	}

	OPObjectNode* GetObject()
	{
		return Object;
	}
	
	virtual opString GetClassName()
	{
		return Object->GetClassName();
	}
	
	opString ErrorName();
	
private:
	//print the transformed body (emits to the same place as the original code)
	void PrintTransformedBody(opFileStream& stream);
		
public:
	OPObjectNode* Object;

	friend OPObjectNode;
};

///==========================================
/// State nodes
///==========================================

class StateBlockNode;

class StateNode : public opNode
{
public:
	DECLARE_NODE(StateNode,opNode,G_STATE);

	void Init()
	{
		Name = NULL;
		Body = NULL;
	}

	void PrintTransformed( opSectionStream& stream );

	void SetName(TerminalNode* t)
	{
		Name = t;
	}

	TerminalNode* GetName()
	{
		return Name;
	}

	void SetBody(StateBlockNode* inbody)
	{
		Body = inbody;
	}

	StateBlockNode* GetBody()
	{
		return Body;
	}
	
	opString ErrorName();

protected:
	TerminalNode*   Name;
	StateBlockNode* Body;
};

// state block node
typedef context::State< opNode > StateBlockNodeParent;
class StateBlockNode : public StateBlockNodeParent
{
public:
	DECLARE_NODE(StateBlockNode,StateBlockNodeParent,G_STATEBLOCK);

	opString ErrorName();

	void PrintTransformed( opSectionStream& stream );
};

///==========================================
/// OPDefineNode
///==========================================

class OPDefineBodyNode;

class OPDefineNode : public DialectNodeBase
{
public:
	DECLARE_NODE(OPDefineNode,opNode,G_OPDEFINE);
	
	void Init()
	{
		Name = NULL;
		Arguments = NULL;
		Body = NULL;
	}
	
	void PrintTransformed( opSectionStream& stream );
	
	void PrintNode(opFileStream& stream)
	{
		PrintTransformed(stream.header.body);
	}
	
	void PrintDialectNode(opDialectStream& stream)
	{
		PrintTransformed(stream.header.heading);
	}

	void SetName(TerminalNode* inname)
	{
		Name = inname;
	}

	void SetArguments(ParenBlockNode* inarguments)
	{
		Arguments = inarguments;
	}

	void SetBody(OPDefineBodyNode* inbody)
	{
		Body = inbody;
	}

	TerminalNode* GetName() 
	{
		return Name;
	}

	ParenBlockNode* GetArguments()
	{
		return Arguments;
	}

	opString ErrorName();

	void PrintString(opString& s)
	{
		s += "opdefine ";

		Name->PrintString(s);

		if (Arguments)
			Arguments->PrintString(s);

		s += " { ... }";
	}

private:

	TerminalNode*    Name;
	ParenBlockNode*  Arguments;
	OPDefineBodyNode* Body;
};

class OPDefineBodyNode : public opNode
{
public:
	DECLARE_NODE(OPDefineBodyNode,opNode,G_OPDEFINE_BODY);

	void PrintTransformed( opSectionStream& stream );
	bool Parse();

	opString ErrorName();

};

///==========================================
/// OPIncludeNode
///==========================================

class OPIncludeNode : public DialectNodeBase
{
public:
	DECLARE_NODE(OPIncludeNode,opNode,G_OPINCLUDE);

	void Init()
	{
		IncludedFile = NULL;
		FileName = NULL;
	}

	opString GetFileString()
	{
		opString filestring = FileName->GetValue().Right('"');
		filestring = filestring.RLeft('"');
		return filestring;
	}
	
	opString GetFullFileString();

	void SetFileName(TerminalNode* filename)
	{
		FileName = filename;
	}

	//action loads the included file (if necessary)
	FileNode* IncludeFile();

	//normal printing
	void PrintNode(opFileStream& stream);

	//dialect printing
	void PrintDialectNode(opDialectStream& stream);

	void PrintOriginal(opSectionStream& stream );
	void PrintTransformed( opSectionStream& stream );

	void PrintXml(opXmlStream& stream );

	opString ErrorName();

	FileNode* GetIncludedFile()
	{
		return IncludedFile;
	}

	void PrintString(opString& s)
	{
		s += "opinclude ";

		FileName->PrintString(s);
	}

private:

	//register dependencies
	void RegisterDependency(const opString& filename);

	boost::filesystem::path validpath;
	boost::filesystem::path generatedpath;
	
	TerminalNode* FileName;
	FileNode* IncludedFile;
};

///==========================================
/// OPMacroNode
///==========================================

class OPMacroArgumentNode : public opNode
{
public:
	DECLARE_NODE(OPMacroArgumentNode,opNode,G_OPMACRO_ARGUMENT);

	void Init()
	{
		VariableName = NULL;
	}

	bool PreParse();

	opNode* GetVariableName()
	{
		return VariableName;
	}

	opString ErrorName() { return ""; }

	void PrintString(opString& s)
	{
		VariableName->PrintString(s);
	}

private:
	opNode* VariableName;
};

typedef ListUtility< Clean< opNode > > OPMacroArgumentListNodeParent;

class OPMacroArgumentListNode : public OPMacroArgumentListNodeParent
{
public:
	DECLARE_NODE(OPMacroArgumentListNode,OPMacroArgumentListNodeParent,G_OPMACRO_ARGUMENT_LIST);

	bool PreParse();

	const vector<OPMacroArgumentNode*>& GetArguments()
	{
		return Arguments;
	}

	opString ErrorName();

	void PrintString(opString& s)
	{
		s += "(";

		int size = (int) Arguments.size();

		for (int i = 0; i < size; i++)
		{
			Arguments[i]->PrintString(s);

			if (i < size - 1)
				s += ",";
		}

		s += ")";
	}

private:
	vector<OPMacroArgumentNode*> Arguments;
};

// note: we don't use this in the DECLARE_NODE on purpose. :)
typedef ExpandCalls< OPMacros< opNode > > OPMacroBodyNodeParent;

class OPMacroBodyNode : public OPMacroBodyNodeParent
{
public:
	DECLARE_NODE(OPMacroBodyNode,opNode,G_OPMACRO_BODY);

	void CloneNode(opNode* newnode)
	{
		CloneChildren(newnode);
	}

	opString ErrorName();
};

class OPMacroNode : public DialectNodeBase
{
public:
	DECLARE_NODE(OPMacroNode,opNode,G_OPMACRO);

	void Init()
	{
		Arguments = NULL;
		Body = NULL;
		Name = NULL;
	}

	void SetName(TerminalNode* inname)
	{
		Name = inname;
	}

	void SetArguments(OPMacroArgumentListNode* inarguments)
	{
		Arguments = inarguments;
	}

	void SetBody(OPMacroBodyNode* inbody)
	{
		Body = inbody;
	}

	TerminalNode* GetName()
	{
		return Name;
	}

	OPMacroArgumentListNode* GetArguments()
	{
		return Arguments;
	}

	OPMacroBodyNode* GetBody()
	{
		return Body;
	}

	void PrintNode(opFileStream& stream )
	{
	}

	void PrintDialectNode(opDialectStream& stream)
	{
	}

	void PrintOriginal(opSectionStream& stream )
	{
	}

	void PrintTransformed(opSectionStream& stream )
	{
	}

	opString GetSignature();

	bool PreProcess();

	opString ErrorName();

	void PrintString(opString& s)
	{
		s += "opmacro ";

		Name->PrintString(s);

		if (Arguments)
			Arguments->PrintString(s);

		s += " { ... }";
	}

private:
	void ExpandChildren(opNode* cloned, ExpandCallArgumentNode* replacement, const opString& variable);
	void ExpandConcatenation(opNode* cloned);
	void ExpandStringize(opNode* cloned);

public:
	void Expand(opNode* cloned, ExpandCallArgumentListNode* args);
	
private:
	TerminalNode*            Name;
	OPMacroArgumentListNode* Arguments;
	OPMacroBodyNode*         Body;
};

///==========================================
/// ValuedModifierNode
///==========================================

class ValuedModifierArgumentNode : public opNode
{
public:
	DECLARE_NODE(ValuedModifierArgumentNode,opNode,G_VALUED_MODIFIER_ARGUMENT);
	
	//NOTE: Can manually add to the modifier value by using
	//		the Append/PrependTerminalNode functions.
	
	void PrintOriginal(opSectionStream& stream)
	{
		PrintOriginalUnspacedChildren(stream);
	}
	
	void PrintString(opString& s)
	{
		PrintStringUnspacedChildren(s);
	}
	
	void CloneNode(opNode* newnode) 
	{
		CloneChildren(newnode);
	}
	
	opString ErrorName();
};

class ValuedModifierNode : public opNode
{
public:
	DECLARE_NODE(ValuedModifierNode,opNode,G_VALUED_MODIFIER);
	
	void Init()
	{
		Modifier = NULL;
		Arguments = NULL;
	}

	void SetModifier(TerminalNode* node)
	{
		Modifier = node;
	}

	void SetArguments(ValuedModifierArgumentNode* node)
	{
		Arguments = node;
	}

	TerminalNode* GetModifier()
	{
		return Modifier;
	}

	ValuedModifierArgumentNode* GetArguments()
	{
		return Arguments;
	}
	
	void PrintNode(opFileStream& stream )
	{
	}
	
	void PrintOriginal(opSectionStream& stream )
	{
		stream << Modifier;
		stream << '(';
		Arguments->PrintOriginal(stream);
		stream << ')';
	}
	
	void PrintString(opString& s)
	{
		Modifier->PrintString(s);
		s += '(';
		Arguments->PrintString(s);
		s += ')';
	}

	void PrintTransformed(opSectionStream& stream )
	{
	}
	
	void CloneNode(ValuedModifierNode* node)
	{
		stacked<TerminalNode> modifier				  = Modifier->Clone();
		stacked<ValuedModifierArgumentNode> arguments = Arguments->Clone();
		node->SetModifier(*modifier);
		node->SetArguments(*arguments);
		node->AppendNode(modifier);
		node->AppendNode(arguments);
	}

	opString ErrorName();

private:
	TerminalNode* Modifier;
	ValuedModifierArgumentNode* Arguments;
};

///==========================================
/// CPlusPlusNode
///==========================================

class CPlusPlusBodyNode : public opNode
{
public:
	DECLARE_NODE(CPlusPlusBodyNode,opNode,G_CPLUSPLUS_BODY);

	void PrintOriginal(opSectionStream& stream)
	{
		PrintOriginalUnspacedChildren(stream);
	}

	void PrintTransformed(opSectionStream& stream)
	{
		PrintOriginalUnspacedChildren(stream);
	}

	void PrintString(opString& s)
	{
		PrintStringUnspacedChildren(s);
	}

	opString ErrorName()
	{
		return "";
	}
};

class CPlusPlusNode : public opNode
{
public:
	DECLARE_NODE(CPlusPlusNode,opNode,G_CPLUSPLUS);

	void Init()
	{
		Body = NULL;
	}

	void SetBody(CPlusPlusBodyNode* body)
	{
		Body = body;
	}

	void PrintOriginal(opSectionStream& stream)
	{
		PrintOriginalChildren(stream);
	}

	void PrintTransformed(opSectionStream& stream)
	{
		PrintTransformedChildren(stream);
	}

	void PrintString(opString& s)
	{
		PrintStringChildren(s);
	}

	void PrintNode(opFileStream& stream)
	{
		PrintOriginal(stream.header.body);
	}

	opString ErrorName();

private:
	CPlusPlusBodyNode* Body;
};

} // end of namespace nodes

