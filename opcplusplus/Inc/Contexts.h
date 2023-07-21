///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Contexts.h
/// Date: 11/26/2006
///
/// Description:
///
/// Interface contexts code.
///****************************************************************

#pragma once

#include "Interfaces.h"

///==========================================
/// Context interfaces
///==========================================

namespace context
{
	// Context Interfaces are used by 
	// many nodes in order to unify parsing
	// across certain contexts

	///==========================================
	/// Declaration context
	///==========================================

	//NOTES: - order is often important in a context
	// Declaration Context is for the inner blocks of opclasses and opstructs (for now)
	// Inheritance Context is for the inheritance definition of opclasses (for now)
	
	template<class Parent>
	class BasicTypes : public	FunctionPointers<
								PointerMembers<
								Scopes< 
								Arrays< 
								FundamentalTypes<
								Pointers< 
								References< 
								TemplateTypes< 
								Blocks< 
								Clean< 
								Parent
								> > > > > > > > > >
	{
	public:
		IMPLEMENTS_INTERFACE(BasicTypes)
		
		bool Parse()
		{
			PARSE_START;
			{
				FindBasicTypes();
			}
			PARSE_END;
		}
		
		void FindBasicTypes()
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
	};

	template<class Parent>
	class Declaration : public CPPConstructs<
							   CPlusPluses<
							   Templated<
							   Usings<
		                       Typedefs<
							   TemplateDecls<
		                       Friends<
							   FundamentalTypes<
							   BasicStatements< 
							   OPObjects< 
							   OPEnums< 
							   Operators< 
							   VisibilityLabels< 
							   FunctionDefinitions< 
							   Functions<
							   Constructors<
							   Destructors<
							   ConstructorDefinitions<
							   DestructorDefinitions<
							   Modifiers<
							   ValuedModifiers< 
							   context::BasicTypes<
							   Parent
							   > > > > > > > > > > > > > > > > > > > > > >
	{
	public:
		IMPLEMENTS_INTERFACE(Declaration)
		REQUIRES_INTERFACE(Blocks)
		REQUIRES_INTERFACE(Clean)

		virtual opString GetClassName()
		{
			ABSTRACT_FUNCTION;
			return "";
		}

		bool Parse()
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

		bool PostParse()
		{
			POSTPARSE_START;
			{
				AllowOnlyBasicStatements();
			}
			POSTPARSE_END;
		}
	};

	///==========================================
	/// Argument context
	///==========================================

	template<class Parent>
	class Argument : public context::BasicTypes<Parent>
	{
	public:
		IMPLEMENTS_INTERFACE(Argument)
		REQUIRES_INTERFACE(Blocks)

		bool Parse()
		{
			PARSE_START;
			{
				FindBasicTypes();
			}
			PARSE_END;
		}
	};

	///==========================================
	/// State context
	///==========================================

	template<class Parent>
	class State : public	BasicStateStatements< 
							States< 
							FunctionPointers< 
							Scopes< 
							Arrays< 
							VisibilityLabels< 
							FunctionDefinitions<
							Pointers< 
							References< 
							Functions< 
							TemplateTypes< 
							Blocks< 
							Clean<
							Parent
							> > > > > > > > > > > > > 
	{
	public:
		IMPLEMENTS_INTERFACE(State)

		bool Parse()
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

		bool PostParse()
		{
			POSTPARSE_START;
			{
				AllowOnlyStateStatements();
			}
			POSTPARSE_END;
		}
	};

	///==========================================
	/// Inheritance context
	///==========================================

	template<class Parent>
	class Inheritance : public Scopes< 
							   TemplateTypes< 
							   Blocks< 
							   Clean< 
							   Parent 
							   > > > > 
	{
	public:
		IMPLEMENTS_INTERFACE(Inheritance)

		bool Parse()
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
	};

	///==========================================
	/// NamespaceDecl context
	/// 
	/// this is like namespace A::B {}, not inside the braces
	///==========================================

	template<class Parent>
	class NamespaceDecl : public Scopes< 
								 Clean< 
		                         Parent 
								 > > 
	{
	public:
		IMPLEMENTS_INTERFACE(NamespaceDecl)

		bool Parse()
		{
			PARSE_START;

			CleanAll();

			FindScopes();

			PARSE_END;
		}
	};

	///==========================================
	/// Global
	///==========================================

	template<class Parent>
	class Global : public ConditionalPreprocessorStatements<
						  CPlusPluses<
						  OPObjects<
						  OPEnums<
						  OPDefines<
						  Namespaces<
						  UsingNamespaceKeywords<
						  OPIncludes<
						  Parent
						  > > > > > > > > 
	{
	public:
		IMPLEMENTS_INTERFACE(Global)

		bool PreParse()
		{
			PREPARSE_START;
			{
				FindOPIncludes();
			}
			PREPARSE_END;
		}

		bool Parse()
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

		bool PostParse()
		{
			POSTPARSE_START;
			{
				NameResolverWalker walker(this);
			}
			POSTPARSE_END;
		}
	};

	///==========================================
	/// Operator context
	///==========================================

	template<class Parent>
	class Operator : public Scopes< 
							Pointers< 
							References< 
							Clean< 
		                    Parent 
						    > > > > 
	{
	public:
		IMPLEMENTS_INTERFACE(Operator)

		void Init()
		{
        	bCastOperator = false;
			OperatorType  = NULL;
		}

		bool Parse()
		{
			PARSE_START;

			//TODO: validate this, its probably wrong.
			FindScopes();

			if(!IsCurrent(T_STAR))
				FindPointers();
			
			if(!IsCurrent(T_AMPERSAND))
				FindReferences();

			if( IsCurrent(G_POINTER) || IsCurrent(T_ID) || IsCurrent(G_REFERENCE) || IsCurrent(G_SCOPE) )
			{
				bCastOperator = true;
				OperatorType  = CurrentNode();
				IncrementPosition();
			}
			else
				OperatorType = CheckOverloadableOperator();

			CheckNone();

			PARSE_END;
		}

	protected:
		opNode* OperatorType;
		bool    bCastOperator;
	};

	///==========================================
	/// Dialect context
	///==========================================
	
	template<class Parent>
	class Dialect : public GlobalDialectStatements<
						   DialectNamespaces<
						   OPMacros<
						   ExpandCalls<
						   OPDefines<
						   OPIncludes<
						   Enumerations<
						   Categories<
						   CodeLocations<
						   Scopes<
						   NoteDefinitions<						   
						   FileDeclarations<
						   Extensions<
						   ExtendPoints<
						   Clean<
						   Parent
						   > > > > > > > > > > > > > > > 
	{
	public:
		IMPLEMENTS_INTERFACE(Dialect)

		bool PreParse()
		{
			PREPARSE_START;
			{
				FindCodeLocations();
				FindOPIncludes();
			}
			PREPARSE_END;
		}

		bool Parse()
		{
			PARSE_START;
			{
				CleanAll();	
				FindOPDefines();
				FindScopes();
				FindDialectNamespaces();
				FindEnumerations();
				FindCategories();
				FindNoteDefinitions();
				FindFileDeclarations();
				FindExtensions();
				FindExtendPoints();

				// look for extensionpoint's (everywhere)
				{
					ExtensionPointWalker walker(this);
				}

				// parse global dialect statements
				FindGlobalDialectStatements();
			}
			PARSE_END;
		}

		bool PostParse()
		{
			POSTPARSE_START;
			{
				AllowOnlyGlobalDialectStatements();
			}
			POSTPARSE_END;
		}
	};

	///==========================================
	/// Category Context
	///==========================================

	template<class Parent>
	class Category : public DialectCategoryStatements<
							CategoryLocations<
							Disallows<
							FunctionModifiers<
							DataModifiers<
							Clean<
							Parent
							> > > > > >
	{
	public:
		IMPLEMENTS_INTERFACE(Category);
		
		bool Parse()
		{
			PARSE_START;
			{
				CleanAll();
				FindDataModifiers();
				FindFunctionModifiers();
				FindCategoryLocations();
				FindDisallows();

				FindDialectCategoryStatements();
			}
			PARSE_END;
		}

		bool PostParse()
		{
			POSTPARSE_START;
			{
				AllowOnlyDialectCategoryStatements();
			}
			POSTPARSE_END;
		}
	};

	///==========================================
	/// Category Location Context
	///==========================================

	template<class Parent>
	class CategoryLocation : public CategoryLocationStatements<
									Notes<
							        CategoryMaps<
							        Clean<
							        Parent
							        > > > >
	{
	public:
		IMPLEMENTS_INTERFACE(CategoryLocation);
		
		bool Parse()
		{
			PARSE_START;
			{
				CleanAll();
				FindNotes();
				FindCategoryMaps();

				// parse category location statements, then 
				// do an appropriate allow only
				FindCategoryLocationStatements();
			}
			PARSE_END;
		}

		bool PostParse()
		{
			POSTPARSE_START;
			{
				AllowOnlyCategoryLocationStatements();
			}
			POSTPARSE_END;
		}
	};

	///==========================================
	/// Map Context
	///==========================================

	template<class Parent>
	class CategoryMap : public DialectCriteriaStatements<
							   CriteriaExpressions<
					           Clean<
					           Parent
					           > > >
	{
	public:
		IMPLEMENTS_INTERFACE(CategoryMap);

		bool Parse()
		{
			PARSE_START;
			{
				CleanAll();
				FindCriteriaExpressions();

				// parse category map criteria expressions, and 
				// do the correct allowonly
				FindCriteriaStatements();
			}
			PARSE_END;
		}

		bool PostParse()
		{
			POSTPARSE_START;
			{
				AllowOnlyDialectCriteriaStatements();
			}
			POSTPARSE_END;
		}
	};

	///==========================================
	/// Enumeration Context
	///==========================================

	template<class Parent>
	class Enumeration : public DialectEnumerationStatements<
							   EnumerationLocations<
							   Disallows<
							   Clean<
							   Parent
							   > > > >
	{
	public:
		IMPLEMENTS_INTERFACE(Enumeration);
		
		bool Parse()
		{
			PARSE_START;
			{
				CleanAll();
				FindEnumerationLocations();
				FindDisallows();

				FindDialectEnumStatements();
			}
			PARSE_END;
		}

		bool PostParse()
		{
			POSTPARSE_START;
			{
				AllowOnlyDialectEnumStatements();
			}
			POSTPARSE_END;
		}
	};

	///==========================================
	/// Enumeration Location Context
	///==========================================

	template<class Parent>
	class EnumerationLocation : public EnumerationLocationStatements<
									   EnumerationMaps<
		                               Notes<
									   Clean<
									   Parent
									   > > > >
	{
	public:
		IMPLEMENTS_INTERFACE(EnumerationLocation);

		bool Parse()
		{
			PARSE_START;
			{
				CleanAll();
				FindNotes();
				FindEnumerationMaps();

				// parse statements
				FindEnumerationLocationStatements();
			}
			PARSE_END;
		}

		bool PostParse()
		{
			POSTPARSE_START;
			{
				AllowOnlyEnumerationLocationStatements();
			}
			POSTPARSE_END;
		}
	};

	//==========================================
	// FileDeclaration
	//==========================================

	template<class Parent>
	class FileDeclaration : public FileDeclarationLocationStatements<
								   FileDeclarationLocations<
		                           Clean<
								   Parent
								   > > >
	{
	public:
		IMPLEMENTS_INTERFACE(FileDeclaration);

		bool Parse()
		{
			PARSE_START;
			{
				CleanAll();
				FindFileDeclarationLocations();

				// parse into statements
				FindFileDeclarationLocationStatements();
			}
			PARSE_END;
		}

		bool PostParse()
		{
			POSTPARSE_START;
			{
				AllowOnlyFileDeclarationLocationStatements();
			}
			POSTPARSE_END;
		}
	};

	//==========================================
	// TemplateType
	//==========================================

	template<class Parent>
	class TemplateType : public TemplateTypes<
		                        Scopes<
		                        Clean<
		                        Parent
	                            > > > 
	{
	public:
		IMPLEMENTS_INTERFACE(TemplateType)

		bool Parse()
		{
			PARSE_START;
			{
				CleanAll();
				
				FindTemplateTypes();
				FindScopes();
			}
			PARSE_END;
		}
	};

} // end namespace context

