#include "Contexts.h"

#pragma once

//
// BasicTypes
//

template<class Parent>
inline void BasicTypes<Parent>::FindBasicTypes()
{
	FindAngles();

	CleanAll();

	//NOTE: we want standalone angles anytime we can have expressions.
	Disallow(T_LESS_THAN);//??
	Disallow(T_GREATER_THAN);

	FindTemplateTypes();

	FindScopes(); // id::id

	FindSigned();
	FindUnsigned();

	FindArrays(); // id[...][...]

	FindPointers();
	FindReferences();

	FindFunctionPointers();

	FindPointerMembers();
}

//
// Declaration
//

template<class Parent>
inline bool Declaration<Parent>::Parse()
{
	PARSE_START;

	CleanAll();

	//needs to be before arrays
	FindOperators();	 // operator ... [(...)]

	FindAngles();

	ConcatenationWalker performconcat(this);

	FindCPlusPluses();

	FindTemplateDecls(); // template< ... >

	FindTemplateTypes();

	FindSigned();
	FindUnsigned();

	FindModifiers();
	FindValuedModifiers();

	FindScopes(); // id::id

	FindArrays(); // id[...][...]

	FindPointers();

	FindReferences();

	FindFunctionPointers();

	FindPointerMembers();

	FindFunctions();

	FindDestructors(GetClassName());

	FindConstructors(GetClassName());

	FindDestructorDefinitions();

	FindConstructorDefinitions();

	FindFunctionDefinitions();

	FindFriends();

	FindUsings();

	FindTypedefs();

	FindVisibilityLabels();

	FindCPPConstructs();

	FindOPEnums();
	FindOPObjects();

	FindTemplated();

	FindBasicStatements();

	PARSE_END;
}

template<class Parent>
inline bool context::Declaration<Parent>::PostParse()
{
	POSTPARSE_START;
	{
		AllowOnlyBasicStatements();
	}
	POSTPARSE_END;
}

//
// State
//


template<class Parent>
inline bool State<Parent>::Parse()
{
	PARSE_START;

	//TODO: definitely should group these things... (share between stuff...)
	FindAngles();

	CleanAll();

	Disallow(T_LESS_THAN);
	Disallow(T_GREATER_THAN);

	FindTemplateTypes();

	FindScopes();

	FindArrays();

	FindPointers();
	FindReferences();

	FindFunctionPointers();

	FindFunctions();
	FindFunctionDefinitions();

	FindStates();

	FindVisibilityLabels();

	FindStateStatements();

	PARSE_END;
}

template<class Parent>
inline bool context::State<Parent>::PostParse()
{
	POSTPARSE_START;
	{
		AllowOnlyStateStatements();
	}
	POSTPARSE_END;
}


//
// Inheritance
//

template<class Parent>
inline bool context::Inheritance<Parent>::Parse()
{
	PARSE_START;

	FindAngles();

	CleanAll();

	Disallow(T_LESS_THAN);
	Disallow(T_GREATER_THAN);

	FindTemplateTypes();
	FindScopes();

	PARSE_END;
}

//
// NamespaceDecl
//

template<class Parent>
inline bool context::NamespaceDecl<Parent>::Parse()
{
	PARSE_START;

	CleanAll();

	FindScopes();

	PARSE_END;
}

//
// Global
//


template<class Parent>
inline bool context::Global<Parent>::PreParse()
{
	PREPARSE_START;
	{
		FindOPIncludes();
	}
	PREPARSE_END;
}

template<class Parent>
inline bool context::Global<Parent>::Parse()
{
	PARSE_START;
	{
		ConcatenationWalker performconcat(this);

		FindOPDefines();
		FindUsingNamespaceKeywords();
		FindNamespaces();
		FindCPlusPluses();
		FindOPEnums();
		FindOPObjects();
		FindConditionalPreprocessorStatements();
	}
	PARSE_END;
}

template<class Parent>
inline bool context::Global<Parent>::PostParse()
{
	POSTPARSE_START;
	{
		NameResolverWalker walker(this);
	}
	POSTPARSE_END;
}

//
// Operator
//

template<class Parent>
inline bool Operator<Parent>::Parse()
{
	PARSE_START;

	//TODO: validate this, its probably wrong.
	FindScopes();

	if (!IsCurrent(T_STAR))
		FindPointers();

	if (!IsCurrent(T_AMPERSAND))
		FindReferences();

	if (IsCurrent(G_POINTER) || IsCurrent(T_ID) || IsCurrent(G_REFERENCE) || IsCurrent(G_SCOPE))
	{
		bCastOperator = true;
		OperatorType = CurrentNode();
		IncrementPosition();
	}
	else
		OperatorType = CheckOverloadableOperator();

	CheckNone();

	PARSE_END;
}

