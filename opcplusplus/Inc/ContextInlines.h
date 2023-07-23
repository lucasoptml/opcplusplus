
#pragma once

#include "Walkers.h"
#include "Contexts.h"

//
// BasicTypes
//

template<class Parent>
inline bool context::BasicTypes<Parent>::Parse()
{
	PARSE_START;
	{
		FindBasicTypes();
	}
	PARSE_END;
}

template<class Parent>
inline void context::BasicTypes<Parent>::FindBasicTypes()
{
	this->FindAngles();

	this->CleanAll();

	//NOTE: we want standalone angles anytime we can have expressions.
	opNode::Disallow(T_LESS_THAN);//??
	opNode::Disallow(T_GREATER_THAN);

	this->FindTemplateTypes();

	this->FindScopes(); // id::id

	this->FindSigned();
	this->FindUnsigned();

	this->FindArrays(); // id[...][...]

	this->FindPointers();
	this->FindReferences();

	this->FindFunctionPointers();

	this->FindPointerMembers();
}

//
// Declaration
//

template<class Parent>
inline bool context::Declaration<Parent>::Parse()
{
	PARSE_START;

	this->CleanAll();

	//needs to be before arrays
	this->FindOperators();	 // operator ... [(...)]

	this->FindAngles();

	ConcatenationWalker performconcat(this);

	this->FindCPlusPluses();

	this->FindTemplateDecls(); // template< ... >

	this->FindTemplateTypes();

	this->FindSigned();
	this->FindUnsigned();

	this->FindModifiers();
	this->FindValuedModifiers();

	this->FindScopes(); // id::id

	this->FindArrays(); // id[...][...]

	this->FindPointers();

	this->FindReferences();

	this->FindFunctionPointers();

	this->FindPointerMembers();

	this->FindFunctions();

	this->FindDestructors(GetClassName());

	this->FindConstructors(GetClassName());

	this->FindDestructorDefinitions();

	this->FindConstructorDefinitions();

	this->FindFunctionDefinitions();

	this->FindFriends();

	this->FindUsings();

	this->FindTypedefs();

	this->FindVisibilityLabels();

	this->FindCPPConstructs();

	this->FindOPEnums();
	this->FindOPObjects();

	this->FindTemplated();

	this->FindBasicStatements();

	PARSE_END;
}

template<class Parent>
inline bool context::Declaration<Parent>::PostParse()
{
	POSTPARSE_START;
	{
		this->AllowOnlyBasicStatements();
	}
	POSTPARSE_END;
}

//
// State
//


template<class Parent>
inline bool context::State<Parent>::Parse()
{
	PARSE_START;

	//TODO: definitely should group these things... (share between stuff...)
	this->FindAngles();

	this->CleanAll();

	opNode::Disallow(T_LESS_THAN);
	opNode::Disallow(T_GREATER_THAN);

	this->FindTemplateTypes();

	this->FindScopes();

	this->FindArrays();

	this->FindPointers();
	this->FindReferences();

	this->FindFunctionPointers();

	this->FindFunctions();
	this->FindFunctionDefinitions();

	this->FindStates();

	this->FindVisibilityLabels();

	this->FindStateStatements();

	PARSE_END;
}

template<class Parent>
inline bool context::State<Parent>::PostParse()
{
	POSTPARSE_START;
	{
		this->AllowOnlyStateStatements();
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

	this->FindAngles();

	this->CleanAll();

	this->Disallow(T_LESS_THAN);
	this->Disallow(T_GREATER_THAN);

	this->FindTemplateTypes();
	this->FindScopes();

	PARSE_END;
}

//
// NamespaceDecl
//

template<class Parent>
inline bool context::NamespaceDecl<Parent>::Parse()
{
	PARSE_START;

	this->CleanAll();

	this->FindScopes();

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
		this->FindOPIncludes();
	}
	PREPARSE_END;
}

template<class Parent>
inline bool context::Global<Parent>::Parse()
{
	PARSE_START;
	{
		ConcatenationWalker performconcat(this);

		this->FindOPDefines();
		this->FindUsingNamespaceKeywords();
		this->FindNamespaces();
		this->FindCPlusPluses();
		this->FindOPEnums();
		this->FindOPObjects();
		this->FindConditionalPreprocessorStatements();
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
inline bool context::Operator<Parent>::Parse()
{
	PARSE_START;

	//TODO: validate this, its probably wrong.
	opNode::FindScopes();

	if (!opNode::IsCurrent(T_STAR))
		opNode::FindPointers();

	if (!opNode::IsCurrent(T_AMPERSAND))
		opNode::FindReferences();

	if (opNode::IsCurrent(G_POINTER) || opNode::IsCurrent(T_ID) || opNode::IsCurrent(G_REFERENCE) || opNode::IsCurrent(G_SCOPE))
	{
		bCastOperator = true;
		OperatorType = opNode::CurrentNode();
		opNode::IncrementPosition();
	}
	else
		OperatorType = opNode::CheckOverloadableOperator();

	opNode::CheckNone();

	PARSE_END;
}


//
// Dialect
//

template<class Parent>
inline bool context::Dialect<Parent>::PreParse()
{
	PREPARSE_START;
	{
		this->FindCodeLocations();
		this->FindOPIncludes();
	}
	PREPARSE_END;
}

template<class Parent>
inline bool context::Dialect<Parent>::Parse()
{
	PARSE_START;
	{
		this->CleanAll();
		this->FindOPDefines();
		this->FindScopes();
		this->FindDialectNamespaces();
		this->FindEnumerations();
		this->FindCategories();
		this->FindNoteDefinitions();
		this->FindFileDeclarations();
		this->FindExtensions();
		this->FindExtendPoints();

		// look for extensionpoint's (everywhere)
		{
			ExtensionPointWalker walker(this);
		}

		// parse global dialect statements
		this->FindGlobalDialectStatements();
	}
	PARSE_END;
}

template<class Parent>
inline bool context::Dialect<Parent>::PostParse()
{
	POSTPARSE_START;
	{
		this->AllowOnlyGlobalDialectStatements();
	}
	POSTPARSE_END;
}

//
// Category
//

template<class Parent>
inline bool context::Category<Parent>::Parse()
{
	PARSE_START;
	{
		this->CleanAll();
		this->FindDataModifiers();
		this->FindFunctionModifiers();
		this->FindCategoryLocations();
		this->FindDisallows();

		this->FindDialectCategoryStatements();
	}
	PARSE_END;
}

template<class Parent>
inline bool context::Category<Parent>::PostParse()
{
	POSTPARSE_START;
	{
		this->AllowOnlyDialectCategoryStatements();
	}
	POSTPARSE_END;
}

//
// CategoryLocation
//

template<class Parent>
inline bool context::CategoryLocation<Parent>::Parse()
{
	PARSE_START;
	{
		this->CleanAll();
		this->FindNotes();
		this->FindCategoryMaps();

		// parse category location statements, then 
		// do an appropriate allow only
		this->FindCategoryLocationStatements();
	}
	PARSE_END;
}

template<class Parent>
inline bool context::CategoryLocation<Parent>::PostParse()
{
	POSTPARSE_START;
	{
		this->AllowOnlyCategoryLocationStatements();
	}
	POSTPARSE_END;
}

//
// CategoryMap
//


template<class Parent>
inline bool context::CategoryMap<Parent>::Parse()
{
	PARSE_START;
	{
		this->CleanAll();
		this->FindCriteriaExpressions();

		// parse category map criteria expressions, and 
		// do the correct allowonly
		this->FindCriteriaStatements();
	}
	PARSE_END;
}

template<class Parent>
inline bool context::CategoryMap<Parent>::PostParse()
{
	POSTPARSE_START;
	{
		this->AllowOnlyDialectCriteriaStatements();
	}
	POSTPARSE_END;
}

//
// Enumeration 
//

template<class Parent>
inline bool context::Enumeration<Parent>::Parse()
{
	PARSE_START;
	{
		this->CleanAll();
		this->FindEnumerationLocations();
		this->FindDisallows();

		this->FindDialectEnumStatements();
	}
	PARSE_END;
}

template<class Parent>
inline bool context::Enumeration<Parent>::PostParse()
{
	POSTPARSE_START;
	{
		this->AllowOnlyDialectEnumStatements();
	}
	POSTPARSE_END;
}

//
// EnumerationLocation
//

template<class Parent>
inline bool context::EnumerationLocation<Parent>::Parse()
{
	PARSE_START;
	{
		this->CleanAll();
		this->FindNotes();
		this->FindEnumerationMaps();

		// parse statements
		this->FindEnumerationLocationStatements();
	}
	PARSE_END;
}

template<class Parent>
inline bool context::EnumerationLocation<Parent>::PostParse()
{
	POSTPARSE_START;
	{
		this->AllowOnlyEnumerationLocationStatements();
	}
	POSTPARSE_END;
}

//
// FileDeclaration
//

template<class Parent>
inline bool context::FileDeclaration<Parent>::Parse()
{
	PARSE_START;
	{
		this->CleanAll();
		this->FindFileDeclarationLocations();

		// parse into statements
		this->FindFileDeclarationLocationStatements();
	}
	PARSE_END;
}

template<class Parent>
inline bool context::FileDeclaration<Parent>::PostParse()
{
	POSTPARSE_START;
	{
		this->AllowOnlyFileDeclarationLocationStatements();
	}
	POSTPARSE_END;
}

//
// TemplateType
//

template<class Parent>
inline bool context::TemplateType<Parent>::Parse()
{
	PARSE_START;
	{
		this->CleanAll();

		this->FindTemplateTypes();
		this->FindScopes();
	}
	PARSE_END;
}

//
// Argument
//


template<class Parent>
inline bool context::Argument<Parent>::Parse()
{
	PARSE_START;
	{
		this->FindBasicTypes();
	}
	PARSE_END;
}