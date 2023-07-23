///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Interfaces.h
/// Date: 11/26/2006
///
/// Description:
///
/// This file contains C++ interfaces implementing various node class
/// functionality.  For example, inheriting from the Braces interface 
/// allows the class to automatically know how to parse for braces.  This 
/// increases code reusability.
///****************************************************************

#pragma once

#include "BasicNodes.h"


namespace interfaces
{

///==========================================
/// Namespace interface
///==========================================

template<class Parent>
class Namespaces : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Namespaces)
	
	bool Parse();

	void FindNamespaces();
};

//==========================================
// UsingNamespaces
//==========================================

template<class Parent>
class UsingNamespaceKeywords : public Parent
{
public:
	IMPLEMENTS_INTERFACE(UsingNamespaceKeywords)

	bool Parse();

	void FindUsingNamespaceKeywords();
};

///==========================================
/// Usings
///==========================================

template<class Parent>
class Usings : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Usings)
	REQUIRES_INTERFACE(Scopes)

	bool Parse();

	void FindUsings();
};

///==========================================
/// Blocks interface
///==========================================

template<class Parent>
class Blocks : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Blocks)

	//TODO:
	//FIXME: this is unsafe
	template<Token Grammar, Token Left, Token Right, class T>
	void FindMatching();

	// stuff we actually care about
	void FindBraces();

	void FindParentheses();

	void FindBrackets();

	void FindAngles();
};

///==========================================
/// Pointer interface
///==========================================

template<class Parent>
class Pointers : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Pointers)

	bool Parse();

	void FindPointers();
};

///==========================================
/// Member Pointer Interface
///==========================================

template<class Parent>
class MemberPointers : public Parent
{
public:
	IMPLEMENTS_INTERFACE(MemberPointers);

	bool Parse();

	void FindMemberPointers();
};

///==========================================
/// Operators interface
///==========================================

template<class Parent>
class Operators : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Operators)

	bool Parse();

	void FindOperators();
};

//==========================================
// Modifiers
//==========================================

template<class Parent>
class Modifiers : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Modifiers)

	bool Parse();

	void FindModifiers();
};

///==========================================
/// ValuedModifiers
///==========================================

template<class Parent>
class ValuedModifiers : public Parent
{
public:
	IMPLEMENTS_INTERFACE(ValuedModifiers)
	
	bool Parse();
	
	void FindValuedModifiers();
};

///==========================================
/// Function interface
///==========================================

template<class Parent>
class Functions : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Functions)

	bool Parse();

	void FindFunctions();
};

///==========================================
/// Interface for function declarations
///==========================================

template<class Parent>
class FunctionDefinitions : public Parent
{
public:
	IMPLEMENTS_INTERFACE(FunctionDefinitions)
	REQUIRES_INTERFACE(Clean)

	bool Parse();

	void FindFunctionDefinitions();

private:
	stacked<opNode> GetReturnType(opNode* after);
};

///==========================================
/// Interface for constructors
///==========================================

template<class Parent>
class ConstructorDefinitions : public Parent
{
public:
	IMPLEMENTS_INTERFACE(ConstructorDefinitions)
	REQUIRES_INTERFACE(Clean)

	bool Parse();

	void FindConstructorDefinitions();
};

///==========================================
/// Interface for Destructors
///==========================================

template<class Parent>
class DestructorDefinitions : public Parent
{
public:
	IMPLEMENTS_INTERFACE(DestructorDefinitions);
	REQUIRES_INTERFACE(Clean)

	bool Parse();

	void FindDestructorDefinitions();
};


///==========================================
/// OPEnum interface
///==========================================

template<class Parent>
class OPEnums : public Parent
{
public:
	IMPLEMENTS_INTERFACE(OPEnums)

	bool Parse();

	void FindOPEnums();
};

///==========================================
/// OPObject interface
///==========================================

template<class Parent>
class OPObjects : public Parent
{
public:
	IMPLEMENTS_INTERFACE(OPObjects)

	bool Parse();

	void FindOPObjects();
};

///==========================================
/// States interface
///==========================================

template<class Parent>
class States : public Parent
{
public:
	IMPLEMENTS_INTERFACE(States)

	bool Parse();

	void FindStates();
};

///==========================================
/// TemplateTypes
///==========================================

template<class Parent>
class TemplateTypes : public Parent
{
public:
	IMPLEMENTS_INTERFACE(TemplateTypes)
	
	bool Parse();

	void FindTemplateTypes();
};

///==========================================
/// References header
///==========================================

template<class Parent>
class References : public Parent
{
public:
	IMPLEMENTS_INTERFACE(References)

	bool Parse();

	void FindReferences();
};

///==========================================
/// Arrays interface
///==========================================

template<class Parent>
class Arrays : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Arrays)

	bool Parse();

	void FindArrays();

	template<typename T>
	void AppendBrackets(opArray< stacked<BracketBlockNode> >& brackets, T* node)
	{
		int num = brackets.Size();
		for(int i = 0; i < num; i++)
		{
			node->AddBracket(*brackets[i]);
			node->AppendNode(brackets[i]);
		}
	}
};

///==========================================
/// Visibility interface
///==========================================

template<class Parent>
class VisibilityLabels : public Parent
{
public:
	IMPLEMENTS_INTERFACE(VisibilityLabels)
	REQUIRES_INTERFACE(Clean)

	bool Parse();

	void FindVisibilityLabels()
	{
		FindVisibilityLabel<T_PUBLIC>();
		FindVisibilityLabel<T_PRIVATE>();
		FindVisibilityLabel<T_PROTECTED>();
	}

	template< Token token >
	void FindVisibilityLabel();
};

///==========================================
/// Scope interface
///==========================================

template<class Parent>
class Scopes : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Scopes)

	bool Parse();

	void FindScopes();

	bool CheckScopePointer( stacked<ScopeNode>& scope, opArray< stacked< opNode > >& scopes, bool bGlobal);
};

///==========================================
/// PointerMembers Interface
///==========================================

template<class Parent>
class PointerMembers : public Parent
{
public:
	IMPLEMENTS_INTERFACE(PointerMembers);

	bool Parse();

	void FindPointerMembers();

};

///==========================================
/// Typenames interface
///==========================================

template<class Parent>
class Typenames : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Typenames)

	bool Parse();

	void FindTypenames();
};

///==========================================
/// Function pointers
///==========================================

template<class Parent>
class FunctionPointers : public Parent
{
public:
	IMPLEMENTS_INTERFACE(FunctionPointers)

	bool Parse();

	void FindFunctionPointers();
};

///==========================================
/// OPDefines interface
///==========================================

template<class Parent>
class OPDefines : public Parent
{
public: 
	IMPLEMENTS_INTERFACE(OPDefines)

	bool Parse();

	void FindOPDefines();
};

///==========================================
/// Define interface
///==========================================

template<class Parent>
class Preprocessors : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Preprocessors)

	bool Parse();

	void FindAllPounds()
	{
		FindPoundDefines();
		FindPoundIf();
		FindPoundIfDef();
		FindPoundIfnDef();
		FindPoundEndIf();
		FindPoundInclude();
		FindPoundElse();
		FindPoundError();
		FindPoundImport();
		FindPoundLine();
		FindPoundPragma();
		FindPoundUnDef();
		FindPoundUsing();
		FindPoundWarning();
	}

	//TODO: a couple possible issues remain,
	//		we need to make sure this starts on a newline only
	//		and we need to know what happens if you have a #define name \, 
	//		followed immediately by another directive 
	template<Token Grammar, Token HitToken, class NodeType>
	void FindDirective(const string& directivename);

	void FindPoundDefines();

	void FindPoundIf();

	void FindPoundIfDef();

	void FindPoundIfnDef();

	void FindPoundEndIf();

	void FindPoundInclude();

	void FindPoundElse();

	void FindPoundError();

	void FindPoundImport();

	void FindPoundLine();

	void FindPoundPragma();

	void FindPoundUnDef();

	void FindPoundUsing();

	void FindPoundWarning();
};

///==========================================
/// OPIncludes
///==========================================

template<class Parent>
class OPIncludes : public Parent
{
public:
	IMPLEMENTS_INTERFACE(OPIncludes)

	//NOTE: this is now a preparse option
	bool PreParse();

	void FindOPIncludes();
};

///==========================================
/// ExpandCalls
///==========================================

template<class Parent>
class ExpandCalls : public Parent
{
public:
	IMPLEMENTS_INTERFACE(ExpandCalls)

	//NOTE: this is a pre-parse interface
	bool PreParse();

	void FindExpandCalls();
};

///==========================================
/// OPMacros interface
///==========================================

template<class Parent>
class OPMacros : public Parent
{
public: 
	IMPLEMENTS_INTERFACE(OPMacros)

	bool PreParse();

	void FindOPMacros();
};

///==========================================
/// CPlusPluses
///==========================================

template<class Parent>
class CPlusPluses : public Parent
{
public: 
	IMPLEMENTS_INTERFACE(CPlusPluses)

	bool Parse();

	void FindCPlusPluses();
};

///==========================================
/// BasicTypes
///==========================================

template<class Parent>
class FundamentalTypes : public Parent
{
public: 
	IMPLEMENTS_INTERFACE(FundamentalTypes)

	bool PreParse();

	void FindSigned();

	void FindUnsigned();
};

///==========================================
/// Constructors
///==========================================

template<class Parent>
class Constructors : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Constructors)
	
	void FindConstructors(const opString& classname);
		
};

///==========================================
/// Destructors
///==========================================

template<class Parent>
class Destructors : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Destructors)

	void FindDestructors(const opString& classname);
};

///==========================================
/// Friends
///==========================================

template<class Parent>
class Friends : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Friends);

	bool Parse();

	void FindFriends();
};

///==========================================
/// Typedefs
///==========================================

template<class Parent>
class Typedefs : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Typedefs);

	bool Parse();

	void FindTypedefs();
};

//==========================================
// TemplateDecls
//==========================================

template<class Parent>
class TemplateDecls : public Parent
{
public:
	IMPLEMENTS_INTERFACE(TemplateDecls);

	bool Parse();

	void FindTemplateDecls();
};

///==========================================
/// CPPConstructs
///==========================================

template<class Parent>
class CPPConstructs : public Parent
{
public:
	IMPLEMENTS_INTERFACE(CPPConstructs);

	bool Parse();

	void FindCPPConstructs();

	// Find cpp constructs (enum/union).
	template<class NodeClass, Token Grammar, Token Hit>
	void FindCPPConstructs();

	// Find cpp object constructs (struct/class).
	template<class NodeClass, Token Grammar, Token Hit>
	void FindCPPConstructObjects();
};

///==========================================
/// Templated
///==========================================

template<class Parent>
class Templated : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Templated);

	bool Parse();

	void FindTemplated();
};


template<NodeType Parent>
class ConditionalSupport : public Parent
{
public:
	IMPLEMENTS_INTERFACE(ConditionalSupport);

	void Init()
	{
		Condition = NULL;
	}

	//returns how many levels printed
	int PrintConditions(opSectionStream& section)
	{
		if (Condition)
			return Condition->PrintCondition(section);
		return 0;
	}

	void PrintConditionEnd(int number, opSectionStream& stream)
	{
		if (!number)
			return;

		stream << endl;

		for (int i = 0; i < number; i++)
			stream << "#endif" << endl;
	}

	void SetCondition(PreprocessorStatementNode* condition)
	{
		Condition = condition;
	}

private:
	PreprocessorStatementNode* Condition;
};


























} //end namespace interface

