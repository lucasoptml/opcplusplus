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
		if (IsCurrent(grammar))
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
	bool DataModifierStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_DATAMODIFIER, DataModifierStatementNode, DataModifierNode>(stuff);
	}

	// Is the current statement a functionmodifier statement?
	bool FunctionModifierStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_FUNCTIONMODIFIER, FunctionModifierStatementNode, FunctionModifierNode>(stuff);
	}

	// Is the current statement a category location statement?
	bool CategoryLocationStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_CATEGORY_LOCATION, CategoryLocationStatementNode, CategoryLocationNode>(stuff);
	}

	// Is the current statement a note statement?
	bool NoteStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_NOTE, NoteStatementNode, NoteNode>(stuff);
	}

	// Is the current statement a datamap statement?
	bool DatamapStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_CATEGORY_DATAMAP, CategoryDatamapStatementNode, CategoryDatamapNode>(stuff);
	}

	// Is the current statement a functionmap statement?
	bool FunctionmapStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_CATEGORY_FUNCTIONMAP, CategoryFunctionmapStatementNode, CategoryFunctionmapNode>(stuff);
	}

	// Is the current statement a disallow statement?
	bool DisallowStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_DISALLOW, DisallowStatementNode, DisallowNode>(stuff);
	}

	// Is the current statement an is statement?
	bool IsStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_IS, IsStatementNode, IsNode>(stuff);
	}

	// Is the current statement an enum location statement?
	bool EnumLocationStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_ENUMERATION_LOCATION, EnumerationLocationStatementNode, EnumerationLocationNode>(stuff);
	}

	// Is the current statement an enummap statement?
	bool EnumMapStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_ENUMERATION_MAP, EnumerationMapStatementNode, EnumerationMapNode>(stuff);
	}

	// Is the current statement a file declaration location statement?
	bool FileDeclarationLocationStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_FILE_DECLARATION_LOCATION, FileDeclarationLocationStatementNode, FileDeclarationLocationNode>(stuff);
	}

	// Is the current statement a note definition statement?
	bool NoteDefinitionStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_NOTE_DEFINITION, NoteDefinitionStatementNode, NoteDefinitionNode>(stuff);
	}

	// Is the current statement an opmacro statement?
	bool OPMacroStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_OPMACRO, DialectOPMacroStatementNode, OPMacroNode>(stuff);
	}

	// Is the current statement a category statement?
	bool CategoryStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_CATEGORY, CategoryStatementNode, CategoryNode>(stuff);
	}

	// Is the current statement a dialect namespace statement?
	bool DialectNamespaceStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_DIALECT_NAMESPACE, DialectNamespaceStatementNode, DialectNamespaceNode>(stuff);
	}

	// Is the current statement an opinclude statement?
	bool OPIncludeStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_OPINCLUDE, DialectOPIncludeStatementNode, OPIncludeNode>(stuff);
	}

	// Is the current statement a code statement?
	bool CodeStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_CODE, CodeStatementNode, CodeNode>(stuff);
	}

	// Is the current statement an enumeration statement?
	bool EnumerationStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_ENUMERATION, EnumerationStatementNode, EnumerationNode>(stuff);
	}

	// Is the current statement a file declaration statement?
	bool FileDeclarationStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_FILE_DECLARATION, FileDeclarationStatementNode, FileDeclarationNode>(stuff);
	}

	// Is the current statement an extension statement?
	bool ExtensionStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_EXTENSION, ExtensionStatementNode, ExtensionNode>(stuff);
	}

	// Is the current statement an extend point statement?
	bool ExtendPointStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_EXTEND_POINT, ExtendPointStatementNode, ExtendPointNode>(stuff);
	}

	// Is the current statement an opdefine statement?
	bool OPDefineStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_OPDEFINE, DialectOPDefineStatementNode, OPDefineNode>(stuff);
	}

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

	bool Parse()
	{
		PARSE_START;
		{
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

	// Finds dialect category statements.
	void FindDialectCategoryStatements()
	{
		bool bFinished = false;

		while (!bFinished)
		{
			stacked<opNode> stuff = opNode::PushUntilOr<opNode>(T_SEMICOLON,
																G_DATAMODIFIER,
																G_FUNCTIONMODIFIER,
																G_CATEGORY_LOCATION,
																G_DISALLOW,
																false);

			bFinished = !(*stuff);

			if (bFinished)
				break;

			if      (DataModifierStatement(stuff));
			else if (FunctionModifierStatement(stuff));
			else if (CategoryLocationStatement(stuff));
			else if (DisallowStatement(stuff));
			else if (IsCurrent(T_SEMICOLON))
			{
				Erase(T_SEMICOLON);
				CollapseNodeAtCurrent(stuff);
			}
			else
			{
				CollapseNodeAtCurrent(stuff);

				bFinished = true;
			}
		}
	}

	// Makes sure that only valid dialect category statements
	// remain after statement parsing.
	void AllowOnlyDialectCategoryStatements()
	{
		AllowOnly( G_DATAMODIFIER_STATEMENT,
				   G_FUNCTIONMODIFIER_STATEMENT,
				   G_CATEGORY_LOCATION_STATEMENT,
				   G_DISALLOW_STATEMENT );
	}
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

	bool Parse()
	{
		PARSE_START;
		{
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

	// Finds dialect category statements.
	void FindDialectEnumStatements()
	{
		bool bFinished = false;

		while (!bFinished)
		{
			stacked<opNode> stuff = opNode::PushUntilOr<opNode>(T_SEMICOLON,
																G_ENUMERATION_LOCATION,
																G_DISALLOW,
																false);

			bFinished = !(*stuff);

			if (bFinished)
				break;

			if      (EnumLocationStatement(stuff));
			else if (DisallowStatement(stuff));
			else if (IsCurrent(T_SEMICOLON))
			{
				Erase(T_SEMICOLON);
				CollapseNodeAtCurrent(stuff);
			}
			else
			{
				CollapseNodeAtCurrent(stuff);

				bFinished = true;
			}
		}
	}

	// Makes sure that only valid dialect category statements
	// remain after statement parsing.
	void AllowOnlyDialectEnumStatements()
	{
		AllowOnly( G_ENUMERATION_LOCATION_STATEMENT,
			       G_DISALLOW_STATEMENT );
	}
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

	bool Parse()
	{
		PARSE_START;
		{
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

	// Finds dialect category statements.
	void FindEnumerationLocationStatements()
	{
		bool bFinished = false;

		while (!bFinished)
		{
			stacked<opNode> stuff = opNode::PushUntilOr<opNode>(T_SEMICOLON,
																G_ENUMERATION_MAP,
																G_NOTE,
																false);

			bFinished = !(*stuff);

			if (bFinished)
				break;

			if      (EnumMapStatement(stuff));
			else if (NoteStatement(stuff));
			else if (IsCurrent(T_SEMICOLON))
			{
				Erase(T_SEMICOLON);
				CollapseNodeAtCurrent(stuff);
			}
			else
			{
				CollapseNodeAtCurrent(stuff);

				bFinished = true;
			}
		}
	}

	// Makes sure that only valid dialect category statements
	// remain after statement parsing.
	void AllowOnlyEnumerationLocationStatements()
	{
		AllowOnly( G_ENUMERATIONMAP_STATEMENT,
			       G_NOTE_STATEMENT );
	}
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

	bool Parse()
	{
		PARSE_START;
		{
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

	// Finds dialect location statements.
	void FindCategoryLocationStatements()
	{
		bool bFinished = false;

		while (!bFinished)
		{
			stacked<opNode> stuff = opNode::PushUntilOr<opNode>(T_SEMICOLON,
																G_NOTE,
																G_CATEGORY_DATAMAP,
																G_CATEGORY_FUNCTIONMAP,
																false);

			bFinished = !(*stuff);

			if (bFinished)
				break;

			if      (NoteStatement(stuff));
			else if (DatamapStatement(stuff));
			else if (FunctionmapStatement(stuff));
			else if (IsCurrent(T_SEMICOLON))
			{
				Erase(T_SEMICOLON);
				CollapseNodeAtCurrent(stuff);
			}
			else
			{
				CollapseNodeAtCurrent(stuff);
				
				bFinished = true;
			}
		}
	}

	// Makes sure that only valid dialect location statements
	// remain after statement parsing.
	void AllowOnlyCategoryLocationStatements()
	{
		AllowOnly( G_NOTE_STATEMENT,
				   G_CATEGORY_DATAMAP_STATEMENT,
				   G_CATEGORY_FUNCTIONMAP_STATEMENT );
	}
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

	bool Parse()
	{
		PARSE_START;
		{
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

	// Finds dialect criteria statements.
	void FindCriteriaStatements()
	{
		bool bFinished = false;

		while (!bFinished)
		{
			stacked<opNode> stuff = opNode::PushUntilOr<opNode>(T_SEMICOLON,
																G_IS,
																false);

			bFinished = !(*stuff);

			if (bFinished)
				break;

			if      (IsStatement(stuff));
			else if (IsCurrent(T_SEMICOLON))
			{
				Erase(T_SEMICOLON);
				CollapseNodeAtCurrent(stuff);
			}
			else
			{
				CollapseNodeAtCurrent(stuff);

				bFinished = true;
			}
		}
	}

	// Makes sure that only valid dialect criteria statements
	// remain after statement parsing.
	void AllowOnlyDialectCriteriaStatements()
	{
		AllowOnly( G_IS_STATEMENT );
	}
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

	bool Parse()
	{
		PARSE_START;
		{
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

	// Finds dialect location statements.
	void FindFileDeclarationLocationStatements()
	{
		bool bFinished = false;

		while (!bFinished)
		{
			stacked<opNode> stuff = opNode::PushUntilOr<opNode>(T_SEMICOLON,
																G_FILE_DECLARATION_LOCATION,
																false);

			bFinished = !(*stuff);

			if (bFinished)
				break;

			if (FileDeclarationLocationStatement(stuff));
			else if (IsCurrent(T_SEMICOLON))
			{
				Erase(T_SEMICOLON);
				CollapseNodeAtCurrent(stuff);
			}
			else
			{
				CollapseNodeAtCurrent(stuff);

				bFinished = true;
			}
		}
	}

	// Makes sure that only valid dialect location statements
	// remain after statement parsing.
	void AllowOnlyFileDeclarationLocationStatements()
	{
		AllowOnly( G_FILE_DECLARATION_LOCATION_STATEMENT );
	}
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

	bool Parse()
	{
		PARSE_START;
		{
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

	// Finds statements in the global dialect context.
	void FindGlobalDialectStatements()
	{
		bool bFinished = false;

		while (!bFinished)
		{
			stacked<opNode> stuff = opNode::PushUntilOr<opNode>( T_SEMICOLON,
				                                                 G_NOTE_DEFINITION,
																 G_OPMACRO,
																 G_CATEGORY,
																 G_DIALECT_NAMESPACE,
																 G_OPINCLUDE,
																 G_CODE,
																 G_ENUMERATION,
																 G_FILE_DECLARATION,
																 G_EXTENSION,
																 G_EXTEND_POINT,
																 G_OPDEFINE,
																 false);

			bFinished = !(*stuff);

			if (bFinished)
				break;

			if      (NoteDefinitionStatement(stuff));
			else if (OPMacroStatement(stuff));
			else if (CategoryStatement(stuff));
			else if (DialectNamespaceStatement(stuff));
			else if (OPIncludeStatement(stuff));
			else if (CodeStatement(stuff));
			else if (EnumerationStatement(stuff));
			else if (FileDeclarationStatement(stuff));
			else if (ExtensionStatement(stuff));
			else if (ExtendPointStatement(stuff));
			else if (OPDefineStatement(stuff));
			else if (IsCurrent(T_SEMICOLON))
			{
				Erase(T_SEMICOLON);
				CollapseNodeAtCurrent(stuff);
			}
			else
			{
				CollapseNodeAtCurrent(stuff);

				bFinished = true;
			}
		}
	}

	// Makes sure that only valid global dialect statements are allowed.
	void AllowOnlyGlobalDialectStatements()
	{
		AllowOnly( G_NOTE_DEFINITION_STATEMENT,
				   G_DIALECT_OPMACRO_STATEMENT,
				   G_CATEGORY_STATEMENT,
				   G_DIALECT_NAMESPACE_STATEMENT,
				   G_DIALECT_OPINCLUDE_STATEMENT,
				   G_CODE_STATEMENT,
				   G_ENUMERATION_STATEMENT,
				   G_FILE_DECLARATION_STATEMENT,
				   G_EXTENSION_STATEMENT,
				   G_EXTEND_POINT_STATEMENT,
				   G_DIALECT_OPDEFINE_STATEMENT );
	}
};

} // end namespace interfaces