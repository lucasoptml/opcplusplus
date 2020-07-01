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

	bool Parse()
	{
		PARSE_START;
		{
			FindCategories();
		}
		PARSE_END;
	}

	void FindCategories()
	{
		LOOP_START(G_CATEGORY)
		{
			HIT(T_CATEGORY)
			{
				stackedcontext<CategoryNode> newNode = opNode::Make<CategoryNode>(T_CATEGORY);
				
				Erase(T_CATEGORY);

				EatWhitespaceAndComments();
				
				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				EatWhitespaceAndComments();
				
				newNode->SetName(*name);
				newNode->AppendNode(name);
				
				stacked<BraceBlockNode>   bbn  = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<CategoryBodyNode> body = opNode::Transform<CategoryBodyNode>(bbn);
				
				newNode->SetBody(*body);
				newNode->AppendNode(body);
				
				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}
};

///==========================================
/// CodeLocations Interface
///==========================================

template<class Parent>
class CodeLocations : public Parent
{
public:
	IMPLEMENTS_INTERFACE(CodeLocations);

	bool PreParse()
	{
		PREPARSE_START;
		{
			FindCodeLocations();
		}
		PREPARSE_END;
	}

	void FindCodeLocations()
	{
		LOOP_START(G_CODE)
		{
			HIT(T_CODE)
			{
				stackedcontext<CodeNode> newNode = opNode::Make<CodeNode>(T_CODE);

				Erase(T_CODE);

				EatWhitespaceAndComments();

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				EatWhitespaceAndComments();

				newNode->SetName(*name);
				newNode->AppendNode(name);

				stacked<BraceBlockNode> bbn = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<CodeBodyNode> body = opNode::Transform<CodeBodyNode>(bbn);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}
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

	bool Parse()
	{
		PARSE_START;
		{
			FindCategoryLocations();
		}
		PARSE_END;
	}

	void FindCategoryLocations()
	{
		LOOP_START(G_CATEGORY_LOCATION)
		{
			HIT(T_LOCATION)
			{
				stackedcontext<CategoryLocationNode> newNode = opNode::Make<CategoryLocationNode>(T_LOCATION);
				
				Erase(T_LOCATION);
				
				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);
				
				stacked<BraceBlockNode> bbn = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<CategoryLocationBodyNode> body = opNode::Transform<CategoryLocationBodyNode>(bbn);
				
				newNode->SetBody(*body);
				newNode->AppendNode(body);
				
				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END
	}
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

	bool Parse()
	{
		PARSE_START;
		{
			FindDisallows();
		}
		PARSE_END;
	}

	void FindDisallows()
	{
		LOOP_START(G_DISALLOW)
		{
			HIT(T_DISALLOW)
			{
				stackedcontext<DisallowNode> newNode = opNode::Make<DisallowNode>(T_DISALLOW);

				Erase(T_DISALLOW);

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				stacked<BraceBlockNode> bbn = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<DisallowBodyNode> body = opNode::Transform<DisallowBodyNode>(bbn);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END
	}
};

///==========================================
/// Maps Interface
///==========================================

template<class Parent>
class CategoryMaps : public Parent
{
public:
	IMPLEMENTS_INTERFACE(CategoryMaps);

	bool Parse()
	{
		PARSE_START;
		{
			FindCategoryMaps();
		}
		PARSE_END;
	}

	void FindCategoryMaps()
	{
		FindCategoryMap<G_CATEGORY_DATAMAP,     T_DATAMAP,     CategoryDatamapNode>();
		FindCategoryMap<G_CATEGORY_FUNCTIONMAP, T_FUNCTIONMAP, CategoryFunctionmapNode>();
	}
	
	template<Token grammar, Token token, class NodeClass>
	void FindCategoryMap()
	{
		LOOP_START(grammar)
		{
			HIT(token)
			{
				stackedcontext<NodeClass> newNode = opNode::Make<NodeClass>(token);
				
				Erase(token);
				
				stacked<TerminalNode>        name = opNode::Expect<TerminalNode>(T_ID);
				
				newNode->SetName(*name);
				newNode->AppendNode(name);
				
				if(IsCurrent(G_BRACE_BLOCK))
				{
					stacked<BraceBlockNode>      bbn  = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
					stacked<CategoryMapBodyNode> body = opNode::Transform<CategoryMapBodyNode>(bbn);

					newNode->SetBody(*body);
					newNode->AppendNode(body);
				}


				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}
};

///==========================================
/// DataModifiers Interface
///==========================================

template<class Parent>
class DataModifiers : public Parent
{
public:
	IMPLEMENTS_INTERFACE(DataModifiers);

	bool Parse()
	{
		PARSE_START;
		{
			FindDataModifiers();
		}
		PARSE_END;
	}

	void FindDataModifiers()
	{
		LOOP_START(G_DATAMODIFIER)
		{
			HIT(T_DATAMODIFIER)
			{
				stackedcontext<DataModifierNode> newNode = opNode::Make<DataModifierNode>(T_DATAMODIFIER);
				
				Erase(T_DATAMODIFIER);

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);
				
				newNode->SetName(*name);
				newNode->AppendNode(name);

				if(IsCurrent(G_PAREN_BLOCK))
				{
					stacked<ParenBlockNode> paren = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);
					stacked<ModifierArgumentNode> args = opNode::Transform<ModifierArgumentNode>(paren);

					newNode->SetArgument(*args);
					newNode->AppendNode(args);
				}

				Erase(T_SEMICOLON);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}
};


///==========================================
/// FunctionModifiers Interface
///==========================================

template<class Parent>
class FunctionModifiers : public Parent
{
public:
	IMPLEMENTS_INTERFACE(FunctionModifiers);

	bool Parse()
	{
		PARSE_START;
		{
			FindFunctionModifiers();
		}
		PARSE_END;
	}

	void FindFunctionModifiers()
	{
		LOOP_START(G_FUNCTIONMODIFIER)
		{
			HIT(T_FUNCTIONMODIFIER)
			{
				stackedcontext<FunctionModifierNode> newNode = opNode::Make<FunctionModifierNode>(T_FUNCTIONMODIFIER);

				Erase(T_FUNCTIONMODIFIER);

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				if(IsCurrent(G_PAREN_BLOCK))
				{
					stacked<ParenBlockNode> paren = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);
					stacked<ModifierArgumentNode> args = opNode::Transform<ModifierArgumentNode>(paren);

					newNode->SetArgument(*args);
					newNode->AppendNode(args);
				}
				
				Erase(T_SEMICOLON);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}
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

	bool Parse()
	{
		PARSE_START;
		{
			FindCriteriaExpressions();
		}
		PARSE_END;
	}

	void FindCriteriaExpressions()
	{
		FindIs<G_IS, T_IS, IsNode, IsBodyNode>();
//		FindIs<G_ISNOT, T_ISNOT, IsnotNode, IsnotBodyNode>();
//		FindEverything();
	}

	template<Token grammar, Token token, class NodeClass, class NodeBodyClass>
	void FindIs()
	{
		LOOP_START(grammar)
		{
			HIT(token)
			{
				stackedcontext<NodeClass> newNode = opNode::Make<NodeClass>(token);
				
				Erase(token);

				stacked<NodeBodyClass> body = opNode::PushUntil<NodeBodyClass>(T_SEMICOLON);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				Erase(T_SEMICOLON);
				
				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END
	}
};

///==========================================
/// Notes
///==========================================

template<class Parent>
class Notes : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Notes);

	bool Parse()
	{
		PARSE_START;
		{
			FindNotes();
		}
		PARSE_END;
	}

	void FindNotes()
	{
		LOOP_START(G_NOTE)
		{
			HIT(T_NOTE)
			{
				stackedcontext<NoteNode> newNode = opNode::Make<NoteNode>(T_NOTE);

				Erase(T_NOTE);

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				if(IsCurrent(G_BRACE_BLOCK))
				{
					stacked<BraceBlockNode> bbn = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
					stacked<NoteBodyNode> body = opNode::Transform<NoteBodyNode>(bbn);

					newNode->SetBody(*body);
					newNode->AppendNode(body);
				}
				else
					Erase(T_SEMICOLON);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}
};

///==========================================
/// NoteDefinitions
///==========================================

template<class Parent>
class NoteDefinitions : public Parent
{
public:
	IMPLEMENTS_INTERFACE(NoteDefinitions);
	
	bool Parse()
	{
		PARSE_START;
		{
			FindNoteDefinitions();
		}
		PARSE_END;
	}

	void FindNoteDefinitions()
	{
		LOOP_START(G_NOTE_DEFINITION)
		{
			HIT(T_NOTE)
			{
				stackedcontext<NoteDefinitionNode> newNode = opNode::Make<NoteDefinitionNode>(T_NOTE);
				
				Erase(T_NOTE);

				EatWhitespaceAndComments();

				stacked<ScopeNode> pathnode = opNode::Expect<ScopeNode>(G_SCOPE);

				newNode->SetPath(*pathnode);
				newNode->AppendNode(pathnode);

				EatWhitespaceAndComments();

				stacked<ParenBlockNode>       ppn  = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);
				stacked<NoteArgumentListNode> args = opNode::Transform<NoteArgumentListNode>(ppn);

				newNode->SetArguments(*args);
				newNode->AppendNode(args);

				EatWhitespaceAndComments();

				stacked<BraceBlockNode> body = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				
				newNode->SetBody(*body);
				newNode->AppendNode(body);
				
				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}
};

///==========================================
/// Enumerations Interface
///==========================================

template<class Parent>
class Enumerations : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Enumerations);

	bool Parse()
	{
		PARSE_START;
		{
			FindEnumerations();
		}
		PARSE_END;
	}

	void FindEnumerations()
	{
		LOOP_START(G_ENUMERATION)
		{
			HIT(T_ENUMERATION)
			{
				stackedcontext<EnumerationNode> newNode = opNode::Make<EnumerationNode>(T_ENUMERATION);

				Erase(T_ENUMERATION);

				EatWhitespaceAndComments();

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				EatWhitespaceAndComments();

				newNode->SetName(*name);
				newNode->AppendNode(name);

				stacked<BraceBlockNode>      bbn  = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<EnumerationBodyNode> body = opNode::Transform<EnumerationBodyNode>(bbn);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}
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

	bool Parse()
	{
		PARSE_START;
		{
			FindEnumerationLocations();
		}
		PARSE_END;
	}

	void FindEnumerationLocations()
	{
		LOOP_START(G_ENUMERATION_LOCATION)
		{
			HIT(T_LOCATION)
			{
				stackedcontext<EnumerationLocationNode> newNode = opNode::Make<EnumerationLocationNode>(T_LOCATION);

				Erase(T_LOCATION);

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				stacked<BraceBlockNode>              bbn  = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<EnumerationLocationBodyNode> body = opNode::Transform<EnumerationLocationBodyNode>(bbn);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END
	}
};

///==========================================
/// EnumerationMaps
///==========================================

template<class Parent>
class EnumerationMaps : public Parent
{
public:
	IMPLEMENTS_INTERFACE(EnumerationMaps);

	bool Parse()
	{
		PARSE_START;
		{
			FindEnumerationMaps();
		}
		PARSE_END;
	}

	void FindEnumerationMaps()
	{
		LOOP_START(G_ENUMERATION_MAP)
		{
			HIT(T_ENUMMAP)
			{
				stackedcontext<EnumerationMapNode> newNode = opNode::Make<EnumerationMapNode>(T_ENUMMAP);

				opNode::Erase(T_ENUMMAP);

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}
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
	void FindDialectModifier()
	{
		LOOP_START(grammar)
		{
			HIT(T_ID)
			{
				TerminalNode* t = node_cast<TerminalNode>(CurrentNode());

				if (t->GetValue() == ModifierClass::StaticModifierName())
				{
					stackedcontext<ModifierClass> newNode = opNode::Make<ModifierClass>(T_ID);

					Erase(T_ID);
					
					InsertNodeAtCurrent(newNode);
				}
				else
					IncrementPosition();
			}
		}
		LOOP_END;
	}

	template<Token grammar, class ModifierClass>
	void FindValuedDialectModifier()
	{
		LOOP_START(grammar)
		{
			HIT(T_ID)
			{
				TerminalNode* t = node_cast<TerminalNode>(CurrentNode());

				if (t->GetValue() == ModifierClass::StaticModifierName())
				{
					stackedcontext<ModifierClass> newNode = opNode::Make<ModifierClass>(T_ID);

					Erase(T_ID);

					stacked<ParenBlockNode> value = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);

					newNode->SetValue(*value);
					newNode->AppendNode(value);

					InsertNodeAtCurrent(newNode);
				}
				else
					IncrementPosition();
			}
		}
		LOOP_END;
	}
};

///==========================================
/// CriteriaValueModifiers
///==========================================

template<class Parent>
class CriteriaValueModifiers : public Parent
{
public:
	IMPLEMENTS_INTERFACE(CriteriaValueModifiers);

	bool Parse()
	{
		PARSE_START;
		{
			FindCriteriaValueModifiers();
		}	
		PARSE_END;
	}

	void FindCriteriaValueModifiers()
	{
		LOOP_START(G_CRITERIA_VALUE_MODIFIER)
		{
			HIT(G_PAREN_BLOCK)
			{
				if (IsPrevious(T_ID))
				{
					stackedcontext<CriteriaValueModifierNode> newNode = opNode::Make<CriteriaValueModifierNode>(G_PAREN_BLOCK);

					stacked<ParenBlockNode>		  paren = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);
					stacked<CriteriaArgumentNode> arg   = opNode::Transform<CriteriaArgumentNode>(paren);

					newNode->SetArgument(*arg);

					stacked<TerminalNode> id = opNode::ReverseExpect<TerminalNode>(T_ID, *arg);

					// Grab basics from the id.
					newNode->CopyBasics(*id);
					newNode->SetName(*id);

					newNode->AppendNode(id);
					newNode->AppendNode(arg);

					InsertNodeAtCurrent(newNode);
				}
				else
					IncrementPosition();
			}
		}
		LOOP_END;
	}
};

///==========================================
/// CriteriaGroups
///==========================================

template<class Parent>
class CriteriaGroups : public Parent
{
public:
	IMPLEMENTS_INTERFACE(CriteriaGroups);

	bool Parse()
	{
		PARSE_START;
		{
			FindCriteriaGroups();
		}
		PARSE_END;
	}

	void FindCriteriaGroups()
	{
		LOOP_START(G_CRITERIA_GROUP)
		{
			HIT(G_PAREN_BLOCK)
			{
				stacked<ParenBlockNode> paren = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);
				stacked<CriteriaGroupNode> newNode = opNode::Transform<CriteriaGroupNode>(paren);
				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}
};

//==========================================
// Extensions
//==========================================

template<class Parent>
class Extensions : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Extensions);

	bool Parse()
	{
		PARSE_START;
		{
			FindExtensions();
		}
		PARSE_END;
	}

	void FindExtensions()
	{
		LOOP_START(G_EXTENSION)
		{
			HIT(T_EXTENSION)
			{
				stackedcontext<ExtensionNode> newNode = opNode::Make<ExtensionNode>(T_EXTENSION);

				Erase(T_EXTENSION);

				EatWhitespaceAndComments();

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				EatWhitespaceAndComments();

				newNode->SetName(*name);
				newNode->AppendNode(name);

				Erase(T_SEMICOLON);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}
};

//==========================================
// ExtendPoints
//==========================================

template<class Parent>
class ExtendPoints : public Parent
{
public:
	IMPLEMENTS_INTERFACE(ExtendPoints);

	bool Parse()
	{
		PARSE_START;
		{
			FindExtendPoints();
		}
		PARSE_END;
	}

	void FindExtendPoints()
	{
		LOOP_START(G_EXTEND_POINT)
		{
			HIT(T_EXTEND_POINT)
			{
				stackedcontext<ExtendPointNode> newNode = opNode::Make<ExtendPointNode>(T_EXTEND_POINT);

				Erase(T_EXTEND_POINT);

				EatWhitespaceAndComments();

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				EatWhitespaceAndComments();

				newNode->SetName(*name);
				newNode->AppendNode(name);

				stacked<BraceBlockNode> body = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}
};

//==========================================
// ExtensionPoints
//==========================================

template<class Parent>
class ExtensionPoints : public Parent
{
public:
	IMPLEMENTS_INTERFACE(ExtensionPoints);

	bool Parse()
	{
		PARSE_START;
		{
			FindExtensionPoints();
		}
		PARSE_END;
	}

	void FindExtensionPoints()
	{
		LOOP_START(G_EXTENSION_POINT)
		{
			HIT(T_EXTENSION_POINT)
			{
				stackedcontext<ExtensionPointNode> newNode = opNode::Make<ExtensionPointNode>(T_EXTENSION_POINT);

				Erase(T_EXTENSION_POINT);

				EatWhitespaceAndComments();

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}
};

//==========================================
// FileDeclarationLocations
//==========================================

template<class Parent>
class FileDeclarationLocations : public Parent
{
public:
	IMPLEMENTS_INTERFACE(FileDeclarationLocations);

	bool Parse()
	{
		PARSE_START;
		{
			FindFileDeclarationLocations();
		}
		PARSE_END;
	}

	void FindFileDeclarationLocations()
	{
		LOOP_START(G_FILE_DECLARATION_LOCATION)
		{
			HIT(T_LOCATION)
			{
				stackedcontext<FileDeclarationLocationNode> newNode = opNode::Make<FileDeclarationLocationNode>(T_LOCATION);

				Erase(T_LOCATION);

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END
	}
};

//==========================================
// FileDeclarations
//==========================================

template<class Parent>
class FileDeclarations : public Parent
{
public:
	IMPLEMENTS_INTERFACE(FileDeclarations);

	bool Parse()
	{
		PARSE_START;
		{
			FindFileDeclarations();
		}
		PARSE_END;
	}

	void FindFileDeclarations()
	{
		LOOP_START(G_FILE_DECLARATION)
		{
			HIT(T_FILE_DECLARATION)
			{
				stackedcontext<FileDeclarationNode> newNode = opNode::Make<FileDeclarationNode>(T_FILE_DECLARATION);

				Erase(T_FILE_DECLARATION);

				EatWhitespaceAndComments();

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				EatWhitespaceAndComments();

				stacked<BraceBlockNode>          bbn  = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<FileDeclarationBodyNode> body = opNode::Transform<FileDeclarationBodyNode>(bbn);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}
};

//==========================================
// DialectNamespaces
//==========================================

template<class Parent>
class DialectNamespaces : public Parent
{
public:
	IMPLEMENTS_INTERFACE(DialectNamespaces)

	bool Parse()
	{
		PARSE_START;
		{
			FindDialectNamespaces();
		}
		PARSE_END;
	}

	void FindDialectNamespaces()
	{
		LOOP_START(G_DIALECT_NAMESPACE)
		{
			HIT(T_NAMESPACE)
			{
				stackedcontext<DialectNamespaceNode> newNode = opNode::Make<DialectNamespaceNode>(T_NAMESPACE);

				Erase(T_NAMESPACE);

				EatWhitespaceAndComments();

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				EatWhitespaceAndComments();

				stacked<BraceBlockNode> body = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}
};

} // end namespace interfaces