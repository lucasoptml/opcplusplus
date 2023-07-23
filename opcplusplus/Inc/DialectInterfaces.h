///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: DialectInterfaces.h
/// Date: 03/24/2007
///
/// Description:
///
/// Interfaces for dialect nodes.
///****************************************************************

#pragma once

namespace interfaces
{

///==========================================
/// Categories Interface
///==========================================

template<class Parent>
class Categories : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Categories);

	bool Parse();

	void FindCategories();
};

///==========================================
/// CodeLocations Interface
///==========================================

template<class Parent>
class CodeLocations : public Parent
{
public:
	IMPLEMENTS_INTERFACE(CodeLocations);

	bool PreParse();

	void FindCodeLocations();
};

///==========================================
/// Locations Interface
///==========================================

template<class Parent>
class CategoryLocations : public Parent
{
public:
	IMPLEMENTS_INTERFACE(CategoryLocations);
	REQUIRES_INTERFACE(Clean);

	bool Parse();

	void FindCategoryLocations();
};


///==========================================
/// Disallow Interface
///==========================================

template<class Parent>
class Disallows : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Disallows);
	REQUIRES_INTERFACE(Clean);

	bool Parse();

	void FindDisallows();
};

///==========================================
/// Maps Interface
///==========================================

template<class Parent>
class CategoryMaps : public Parent
{
public:
	IMPLEMENTS_INTERFACE(CategoryMaps);

	bool Parse();

	void FindCategoryMaps();
	
	template<Token grammar, Token token, class NodeClass>
	void FindCategoryMap();
};

///==========================================
/// DataModifiers Interface
///==========================================

template<class Parent>
class DataModifiers : public Parent
{
public:
	IMPLEMENTS_INTERFACE(DataModifiers);

	bool Parse();

	void FindDataModifiers();
};


///==========================================
/// FunctionModifiers Interface
///==========================================

template<class Parent>
class FunctionModifiers : public Parent
{
public:
	IMPLEMENTS_INTERFACE(FunctionModifiers);

	bool Parse();

	void FindFunctionModifiers();
};

///==========================================
/// CriteriaExpressions
///==========================================

template<class Parent>
class CriteriaExpressions : public Parent
{
public:
	IMPLEMENTS_INTERFACE(CriteriaExpressions);
	REQUIRES_INTERFACE(Clean);

	bool Parse();

	void FindCriteriaExpressions();

	template<Token grammar, Token token, class NodeClass, class NodeBodyClass>
	void FindIs();
};

///==========================================
/// Notes
///==========================================

template<class Parent>
class Notes : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Notes);

	bool Parse();

	void FindNotes();
};

///==========================================
/// NoteDefinitions
///==========================================

template<class Parent>
class NoteDefinitions : public Parent
{
public:
	IMPLEMENTS_INTERFACE(NoteDefinitions);
	
	bool Parse();

	void FindNoteDefinitions();
};

///==========================================
/// Enumerations Interface
///==========================================

template<class Parent>
class Enumerations : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Enumerations);

	bool Parse();

	void FindEnumerations();
};

///==========================================
/// EnumerationLocations
///==========================================

template<class Parent>
class EnumerationLocations : public Parent
{
public:
	IMPLEMENTS_INTERFACE(EnumerationLocations);
	REQUIRES_INTERFACE(Clean);

	bool Parse();

	void FindEnumerationLocations();
};

///==========================================
/// EnumerationMaps
///==========================================

template<class Parent>
class EnumerationMaps : public Parent
{
public:
	IMPLEMENTS_INTERFACE(EnumerationMaps);

	bool Parse();

	void FindEnumerationMaps();
};

///==========================================
/// Dialect Modifiers
///==========================================

template<class Parent>
class DialectModifiers : public Parent
{
public:
	IMPLEMENTS_INTERFACE(DialectModifiers);

	template<Token grammar, class ModifierClass>
	void FindDialectModifier();

	template<Token grammar, class ModifierClass>
	void FindValuedDialectModifier();
};

///==========================================
/// CriteriaValueModifiers
///==========================================

template<class Parent>
class CriteriaValueModifiers : public Parent
{
public:
	IMPLEMENTS_INTERFACE(CriteriaValueModifiers);

	bool Parse();

	void FindCriteriaValueModifiers();
};

///==========================================
/// CriteriaGroups
///==========================================

template<class Parent>
class CriteriaGroups : public Parent
{
public:
	IMPLEMENTS_INTERFACE(CriteriaGroups);

	bool Parse();

	void FindCriteriaGroups();
};

//==========================================
// Extensions
//==========================================

template<class Parent>
class Extensions : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Extensions);

	bool Parse();

	void FindExtensions();
};

//==========================================
// ExtendPoints
//==========================================

template<class Parent>
class ExtendPoints : public Parent
{
public:
	IMPLEMENTS_INTERFACE(ExtendPoints);

	bool Parse();

	void FindExtendPoints();
};

//==========================================
// ExtensionPoints
//==========================================

template<class Parent>
class ExtensionPoints : public Parent
{
public:
	IMPLEMENTS_INTERFACE(ExtensionPoints);

	bool Parse();

	void FindExtensionPoints();
};

//==========================================
// FileDeclarationLocations
//==========================================

template<class Parent>
class FileDeclarationLocations : public Parent
{
public:
	IMPLEMENTS_INTERFACE(FileDeclarationLocations);

	bool Parse();

	void FindFileDeclarationLocations();
};

//==========================================
// FileDeclarations
//==========================================

template<class Parent>
class FileDeclarations : public Parent
{
public:
	IMPLEMENTS_INTERFACE(FileDeclarations);

	bool Parse();

	void FindFileDeclarations();
};

//==========================================
// DialectNamespaces
//==========================================

template<class Parent>
class DialectNamespaces : public Parent
{
public:
	IMPLEMENTS_INTERFACE(DialectNamespaces)

	bool Parse();

	void FindDialectNamespaces();
};











} // end namespace interfaces