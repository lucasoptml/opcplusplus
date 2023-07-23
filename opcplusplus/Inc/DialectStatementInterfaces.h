///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: DialectStatementInterfaces.h
/// Date: 06/04/2007
///
/// Description:
///
/// Contains interfaces for dialect statements.
///****************************************************************

#pragma once

namespace interfaces
{

///==========================================
/// DialectStatementsBase
///==========================================

template<class Parent>
class DialectStatementsBase : public Parent
{
public:
	IMPLEMENTS_INTERFACE(DialectStatementsBase)

	// This method checks for a statement + modifiers based on the template args.
	template<Token grammar, class StatementNodeClass, class NodeClass>
	bool HandleStatement(stacked<opNode>& stuff)
	{
		if (this.IsCurrent(grammar))
		{
			stackedcontext<StatementNodeClass> statement = opNode::Make<StatementNodeClass>(grammar);
			stacked<NodeClass>                 node      = opNode::Expect<NodeClass>(grammar);

			if (!stuff->IsEmpty())
			{
				stacked<DialectModifiersNode> modifiers = opNode::Transform<DialectModifiersNode>(stuff);

				statement->SetModifiers(*modifiers);
				statement->AppendNode(modifiers);
			}
			else
				stuff.Delete();

			statement->SetStatement(*node);
			statement->AppendNode(node);

			Statements.push_back(*statement);
			InsertNodeAtCurrent(statement);

			return true;
		}

		return false;
	}

	// Is the current statement a datamodifier statement?
	bool DataModifierStatement(stacked<opNode>& stuff);

	// Is the current statement a functionmodifier statement?
	bool FunctionModifierStatement(stacked<opNode>& stuff);

	// Is the current statement a category location statement?
	bool CategoryLocationStatement(stacked<opNode>& stuff);

	// Is the current statement a note statement?
	bool NoteStatement(stacked<opNode>& stuff);

	// Is the current statement a datamap statement?
	bool DatamapStatement(stacked<opNode>& stuff);

	// Is the current statement a functionmap statement?
	bool FunctionmapStatement(stacked<opNode>& stuff);

	// Is the current statement a disallow statement?
	bool DisallowStatement(stacked<opNode>& stuff);

	// Is the current statement an is statement?
	bool IsStatement(stacked<opNode>& stuff);

	// Is the current statement an enum location statement?
	bool EnumLocationStatement(stacked<opNode>& stuff);

	// Is the current statement an enummap statement?
	bool EnumMapStatement(stacked<opNode>& stuff);

	// Is the current statement a file declaration location statement?
	bool FileDeclarationLocationStatement(stacked<opNode>& stuff);

	// Is the current statement a note definition statement?
	bool NoteDefinitionStatement(stacked<opNode>& stuff);

	// Is the current statement an opmacro statement?
	bool OPMacroStatement(stacked<opNode>& stuff);

	// Is the current statement a category statement?
	bool CategoryStatement(stacked<opNode>& stuff);

	// Is the current statement a dialect namespace statement?
	bool DialectNamespaceStatement(stacked<opNode>& stuff);

	// Is the current statement an opinclude statement?
	bool OPIncludeStatement(stacked<opNode>& stuff);

	// Is the current statement a code statement?
	bool CodeStatement(stacked<opNode>& stuff);

	// Is the current statement an enumeration statement?
	bool EnumerationStatement(stacked<opNode>& stuff);

	// Is the current statement a file declaration statement?
	bool FileDeclarationStatement(stacked<opNode>& stuff);

	// Is the current statement an extension statement?
	bool ExtensionStatement(stacked<opNode>& stuff);

	// Is the current statement an extend point statement?
	bool ExtendPointStatement(stacked<opNode>& stuff);

	// Is the current statement an opdefine statement?
	bool OPDefineStatement(stacked<opNode>& stuff);

protected:
	/*=== data ===*/

	vector<DialectStatementBase*> Statements;
};

///==========================================
/// DialectCategoryStatements
///==========================================

template<class Parent>
class DialectCategoryStatements : public DialectStatementsBase<Parent>
{
public:
	IMPLEMENTS_INTERFACE(DialectCategoryStatements)
	REQUIRES_INTERFACE(DialectStatementsBase)

	bool Parse();

	bool PostParse();

	// Finds dialect category statements.
	void FindDialectCategoryStatements();

	// Makes sure that only valid dialect category statements
	// remain after statement parsing.
	void AllowOnlyDialectCategoryStatements();
};

///==========================================
/// DialectEnumerationStatements
///==========================================

template<class Parent>
class DialectEnumerationStatements : public DialectStatementsBase<Parent>
{
public:
	IMPLEMENTS_INTERFACE(DialectEnumerationStatements)
	REQUIRES_INTERFACE(DialectStatementsBase)

	bool Parse();

	bool PostParse();

	// Finds dialect category statements.
	void FindDialectEnumStatements();

	// Makes sure that only valid dialect category statements
	// remain after statement parsing.
	void AllowOnlyDialectEnumStatements();
};

///==========================================
/// EnumerationLocationStatements
///==========================================

template<class Parent>
class EnumerationLocationStatements : public DialectStatementsBase<Parent>
{
public:
	IMPLEMENTS_INTERFACE(EnumerationLocationStatements)
	REQUIRES_INTERFACE(DialectStatementsBase)

	bool Parse();

	bool PostParse();

	// Finds dialect category statements.
	void FindEnumerationLocationStatements();

	// Makes sure that only valid dialect category statements
	// remain after statement parsing.
	void AllowOnlyEnumerationLocationStatements();
};

///==========================================
/// CategoryLocationStatements
///==========================================

template<class Parent>
class CategoryLocationStatements : public DialectStatementsBase<Parent>
{
public:
	IMPLEMENTS_INTERFACE(CategoryLocationStatements)
	REQUIRES_INTERFACE(DialectStatementsBase)

	bool Parse();

	bool PostParse();

	// Finds dialect location statements.
	void FindCategoryLocationStatements();

	// Makes sure that only valid dialect location statements
	// remain after statement parsing.
	void AllowOnlyCategoryLocationStatements();
};

///==========================================
/// DialectCriteriaStatements
///==========================================

template<class Parent>
class DialectCriteriaStatements : public DialectStatementsBase<Parent>
{
public:
	IMPLEMENTS_INTERFACE(DialectCriteriaStatements)
	REQUIRES_INTERFACE(DialectStatementsBase)

	bool Parse();

	bool PostParse();

	// Finds dialect criteria statements.
	void FindCriteriaStatements();

	// Makes sure that only valid dialect criteria statements
	// remain after statement parsing.
	void AllowOnlyDialectCriteriaStatements();
};

//==========================================
// DialectFileDeclarationLocationStatements
//==========================================

template<class Parent>
class FileDeclarationLocationStatements : public DialectStatementsBase<Parent>
{
public:
	IMPLEMENTS_INTERFACE(FileDeclarationLocationStatements)
	REQUIRES_INTERFACE(DialectStatementsBase)

	bool Parse();

	bool PostParse();

	// Finds dialect location statements.
	void FindFileDeclarationLocationStatements();

	// Makes sure that only valid dialect location statements
	// remain after statement parsing.
	void AllowOnlyFileDeclarationLocationStatements();
};

///==========================================
/// GlobalDialectStatements
///==========================================

template<class Parent>
class GlobalDialectStatements : public DialectStatementsBase<Parent>
{
public:
	IMPLEMENTS_INTERFACE(GlobalDialectStatements)
	REQUIRES_INTERFACE(DialectStatementsBase)

	bool Parse();

	bool PostParse();

	// Finds statements in the global dialect context.
	void FindGlobalDialectStatements();

	// Makes sure that only valid global dialect statements are allowed.
	void AllowOnlyGlobalDialectStatements();
};








} // end namespace interfaces