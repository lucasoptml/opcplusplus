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
#include "UtilityInterfaces.h"
#include "StatementInterfaces.h"
#include "DialectStatementInterfaces.h"
#include "DialectInterfaces.h"

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
		
		bool Parse();
		
		void FindBasicTypes();
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

		bool Parse();

		bool PostParse();
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

		bool Parse();
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

		bool Parse();

		bool PostParse();
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

		bool Parse();
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

		bool Parse();
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

		bool PreParse();

		bool Parse();

		bool PostParse();
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

		bool Parse();

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

		bool PreParse();

		bool Parse();

		bool PostParse();
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
		
		bool Parse();

		bool PostParse();
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
		
		bool Parse();

		bool PostParse();
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

		bool Parse();

		bool PostParse();
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
		
		bool Parse();

		bool PostParse();
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

		bool Parse();

		bool PostParse();
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

		bool Parse();

		bool PostParse();
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

		bool Parse();
	};








} // end namespace context

