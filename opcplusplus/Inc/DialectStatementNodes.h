///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: DialectStatementBases.h
/// Date: 06/13/2007
///
/// Description:
///
/// Contains nodes for dialect statements.
///****************************************************************

namespace dialect_nodes
{

///==========================================
/// DialectStatementBase
///==========================================

class DialectStatementBase : public opNode
{
public:
	DECLARE_NODE(DialectStatementBase,opNode,T_UNKNOWN);

	virtual DialectStatementBase* ToDialectStatementBase()
	{
		return this;
	}

	void Init()
	{
		Modifiers = NULL;
		Statement = NULL;
	}

	/*=== set ===*/

	void SetModifiers(DialectModifiersNode* innode)
	{
		Modifiers = innode;
	}

	void SetStatement(DialectNodeBase* innode)
	{
		Statement = innode;
	}

	/*=== get ===*/

	DialectModifiersNode* GetModifiers()
	{
		return Modifiers;
	}

	DialectNodeBase* GetStatement()
	{
		return Statement;
	}

	/*=== utility ===*/

	void PrintDialectNode(opDialectStream& stream)
	{
		GetStatement()->PrintDialectNode(stream);
	}

	opString ErrorName();

protected:

	/*=== data ===*/

	DialectModifiersNode* Modifiers;
	DialectNodeBase*      Statement;
};

///==========================================
/// DataModifierStatementNode
///==========================================

class DataModifierStatementNode : public DialectStatementBase
{
public:
	DECLARE_NODE(DataModifierStatementNode,DialectStatementBase,G_DATAMODIFIER_STATEMENT);

	bool Parse()
	{
		PARSE_START;
		{
			if (Modifiers)
			{
				Modifiers->FindValuedDialectModifier<G_ERROR, ErrorNode>();
				Modifiers->FindValuedDialectModifier<G_DESCRIPTION, DescriptionNode>();				
			}
		}
		PARSE_END;
	}

	bool PostParse()
	{
		POSTPARSE_START;
		{
			if (Modifiers)
			{
				Modifiers->AllowOnly(G_ERROR,G_DESCRIPTION);
			}
		}
		POSTPARSE_END;
	}

	void PrintDialectNode(opDialectStream& stream)
	{
		PrintNodeChildren(stream);
	}
};

///==========================================
/// FunctionModifierStatementNode
///==========================================

class FunctionModifierStatementNode : public DialectStatementBase
{
public:
	DECLARE_NODE(FunctionModifierStatementNode,DialectStatementBase,G_FUNCTIONMODIFIER_STATEMENT);

	bool Parse()
	{
		PARSE_START;
		{
			if (Modifiers)
			{
				Modifiers->FindValuedDialectModifier<G_ERROR, ErrorNode>();
				Modifiers->FindValuedDialectModifier<G_DESCRIPTION, DescriptionNode>();
			}
		}
		PARSE_END;
	}

	bool PostParse()
	{
		POSTPARSE_START;
		{
			if (Modifiers)
			{
				Modifiers->AllowOnly(G_ERROR,G_DESCRIPTION);
			}
		}
		POSTPARSE_END;
	}

	void PrintDialectNode(opDialectStream& stream)
	{
		PrintNodeChildren(stream);
	}
};

///==========================================
/// CategoryLocationStatementNode
///==========================================

class CategoryLocationStatementNode : public DialectStatementBase
{
public:
	DECLARE_NODE(CategoryLocationStatementNode,DialectStatementBase,G_CATEGORY_LOCATION_STATEMENT);

	bool Parse()
	{
		PARSE_START;
		{
			if (Modifiers)
				Modifiers->CheckNone();
		}
		PARSE_END;
	}
};

///==========================================
/// DisallowStatementNode
///==========================================

class DisallowStatementNode : public DialectStatementBase
{
public:
	DECLARE_NODE(DisallowStatementNode,DialectStatementBase,G_DISALLOW_STATEMENT);

	bool Parse()
	{
		PARSE_START;
		{
			if (Modifiers)
			{
				Modifiers->FindValuedDialectModifier<G_ERROR, ErrorNode>();
			}
		}
		PARSE_END;
	}

	bool PostParse()
	{
		POSTPARSE_START;
		{
			if (Modifiers)
			{
				Modifiers->AllowOnly(G_ERROR);
			}
		}
		POSTPARSE_END;
	}
};

///==========================================
/// NoteStatement
///==========================================

typedef DialectModifiers< DialectStatementBase > NoteStatementNodeParent;

class NoteStatementNode : public NoteStatementNodeParent 
{
public:
	DECLARE_NODE(NoteStatementNode,NoteStatementNodeParent,G_NOTE_STATEMENT);

	bool Parse()
	{
		PARSE_START;
		{
			if (Modifiers)
			{
				Modifiers->FindDialectModifier<G_HIDDEN, HiddenNode>();
				Modifiers->FindValuedDialectModifier<G_BEFORE, BeforeNode>();
				Modifiers->FindValuedDialectModifier<G_AFTER,  AfterNode>();
			}		
		}
		PARSE_END;
	}

	bool PostParse()
	{
		POSTPARSE_START;
		{
			if (Modifiers)
			{
				Modifiers->AllowOnly( G_HIDDEN, G_BEFORE, G_AFTER );

				// before and after are mutually exclusive..
				Modifiers->DisallowBoth(G_BEFORE, G_AFTER);

				NoteNode* notenode = node_cast<NoteNode>( Statement );

				// If this note has the before modifier, set it.
				if ( BeforeNode* before = node_cast<BeforeNode>( Modifiers->GetModifier(G_BEFORE) ) )
					notenode->SetOrder( before );
				// Else if this note has the after modifier, set it.
				else if ( AfterNode* after = node_cast<AfterNode>( Modifiers->GetModifier(G_AFTER) ) )
					notenode->SetOrder( after );
			}
		}
		POSTPARSE_END;
	}

	void PrintDialectNode(opDialectStream& stream)
	{
		PrintNodeChildren(stream);
	}

	void PrintString(opString& s)
	{
		if (Modifiers)
		{
			Modifiers->PrintString(s);
			s += " ";
		}

		Statement->PrintString(s);
	}
};

///==========================================
/// EnumerationMapStatementNode
///==========================================

class EnumerationMapStatementNode : public DialectStatementBase
{
public:
	DECLARE_NODE(EnumerationMapStatementNode,DialectStatementBase,G_ENUMERATIONMAP_STATEMENT)

	bool Parse()
	{
		PARSE_START;
		{
			// parse modifiers on notes
			if (Modifiers)
			{
				Modifiers->FindDialectModifier<G_HIDDEN, HiddenNode>();
				Modifiers->FindValuedDialectModifier<G_BEFORE, BeforeNode>();
				Modifiers->FindValuedDialectModifier<G_AFTER,  AfterNode>();			
			}		
		}
		PARSE_END;
	}

	bool PostParse()
	{
		POSTPARSE_START;
		{
			if (Modifiers)
			{
				Modifiers->AllowOnly( G_HIDDEN, G_BEFORE, G_AFTER );

				// before and after are mutually exclusive..
				Modifiers->DisallowBoth(G_BEFORE, G_AFTER);

				EnumerationMapNode* enummap = node_cast<EnumerationMapNode>( Statement );

				// If this note has the before modifier, set it.
				if ( BeforeNode* before = node_cast<BeforeNode>( Modifiers->GetModifier(G_BEFORE) ) )
					enummap->SetOrder( before );
				// Else if this note has the after modifier, set it.
				else if ( AfterNode* after = node_cast<AfterNode>( Modifiers->GetModifier(G_AFTER) ) )
					enummap->SetOrder( after );	
			}
		}
		POSTPARSE_END;
	}
};

///==========================================
/// EnumerationLocationStatementNode
///==========================================

class EnumerationLocationStatementNode : public DialectStatementBase
{
public:
	DECLARE_NODE(EnumerationLocationStatementNode,DialectStatementBase,G_ENUMERATION_LOCATION_STATEMENT)

	bool Parse()
	{
		PARSE_START;
		{
			if (Modifiers)
				Modifiers->CheckNone();
		}
		PARSE_END;
	}
};

///==========================================
/// CategoryDatamapStatementNode
///==========================================

class CategoryDatamapStatementNode : public DialectStatementBase
{
public:
	DECLARE_NODE(CategoryDatamapStatementNode,DialectStatementBase,G_CATEGORY_DATAMAP_STATEMENT);

	bool Parse()
	{
		PARSE_START;
		{
			// parse modifiers on notes
			if (Modifiers)
			{
				Modifiers->FindDialectModifier<G_HIDDEN, HiddenNode>();
				Modifiers->FindValuedDialectModifier<G_BEFORE, BeforeNode>();
				Modifiers->FindValuedDialectModifier<G_AFTER,  AfterNode>();
			}		
		}
		PARSE_END;
	}

	bool PostParse()
	{
		POSTPARSE_START;
		{
			if (Modifiers)
			{
				Modifiers->AllowOnly( G_HIDDEN, G_BEFORE, G_AFTER );

				// before and after are mutually exclusive..
				Modifiers->DisallowBoth(G_BEFORE, G_AFTER);

				CategoryDatamapNode* datamap = node_cast<CategoryDatamapNode>( Statement );
				
				// If this note has the before modifier, set it.
				if ( BeforeNode* before = node_cast<BeforeNode>( Modifiers->GetModifier(G_BEFORE) ) )
					datamap->SetOrder( before );
				// Else if this note has the after modifier, set it.
				else if ( AfterNode* after = node_cast<AfterNode>( Modifiers->GetModifier(G_AFTER) ) )
					datamap->SetOrder( after );	
			}
		}
		POSTPARSE_END;
	}

	void PrintDialectNode(opDialectStream& stream)
	{
		PrintNodeChildren(stream);
	}

	void PrintString(opString& s)
	{
		if (Modifiers)
		{
			Modifiers->PrintString(s);
			s += " ";
		}

		Statement->PrintString(s);
	}
};

///==========================================
/// CategoryFunctionmapStatementNode
///==========================================

class CategoryFunctionmapStatementNode : public DialectStatementBase
{
public:
	DECLARE_NODE(CategoryFunctionmapStatementNode,DialectStatementBase,G_CATEGORY_FUNCTIONMAP_STATEMENT);

	bool Parse()
	{
		PARSE_START;
		{
			// parse modifiers on notes
			if (Modifiers)
			{
				Modifiers->FindDialectModifier<G_HIDDEN, HiddenNode>();
				Modifiers->FindValuedDialectModifier<G_BEFORE, BeforeNode>();
				Modifiers->FindValuedDialectModifier<G_AFTER,  AfterNode>();
			}		
		}
		PARSE_END;
	}

	bool PostParse()
	{
		POSTPARSE_START;
		{
			if (Modifiers)
			{
				Modifiers->AllowOnly( G_HIDDEN, G_BEFORE, G_AFTER );

				// before and after are mutually exclusive..
				Modifiers->DisallowBoth(G_BEFORE, G_AFTER);

				CategoryFunctionmapNode* functionmap = node_cast<CategoryFunctionmapNode>( Statement );

				// If this note has the before modifier, set it.
				if ( BeforeNode* before = node_cast<BeforeNode>( Modifiers->GetModifier(G_BEFORE) ) )
					functionmap->SetOrder( before );
				// Else if this note has the after modifier, set it.
				else if ( AfterNode* after = node_cast<AfterNode>( Modifiers->GetModifier(G_AFTER) ) )
					functionmap->SetOrder( after );	
			}
		}
		POSTPARSE_END;
	}

	void PrintDialectNode(opDialectStream& stream)
	{
		PrintNodeChildren(stream);
	}

	void PrintString(opString& s)
	{
		if (Modifiers)
		{
			Modifiers->PrintString(s);
			s += " ";
		}

		Statement->PrintString(s);
	}
};

///==========================================
/// IsStatementNode
///==========================================

class IsStatementNode : public DialectStatementBase
{
public:
	DECLARE_NODE(IsStatementNode,DialectStatementBase,G_IS_STATEMENT);
	
	bool Parse()
	{
		PARSE_START;
		{
			if (Modifiers)
				Modifiers->CheckNone();
		}
		PARSE_END;
	}
};

///==========================================
/// FileDeclarationLocationStatementNode
///==========================================

class FileDeclarationLocationStatementNode : public DialectStatementBase
{
public:
	DECLARE_NODE(FileDeclarationLocationStatementNode,DialectStatementBase,G_FILE_DECLARATION_LOCATION_STATEMENT);

	bool Parse()
	{
		PARSE_START;
		{
			if (Modifiers)
			{
				Modifiers->FindValuedDialectModifier<G_BEFORE, BeforeNode>();
				Modifiers->FindValuedDialectModifier<G_AFTER,  AfterNode>();
			}		
		}
		PARSE_END;
	}

	bool PostParse()
	{
		POSTPARSE_START;
		{
			if (Modifiers)
			{
				Modifiers->AllowOnly(G_BEFORE,G_AFTER);

				// before and after are mutually exclusive..
				Modifiers->DisallowBoth(G_BEFORE, G_AFTER);				
				
				FileDeclarationLocationNode* loc = node_cast<FileDeclarationLocationNode>( Statement );

				// If this note has the before modifier, set it.
				if ( BeforeNode* before = node_cast<BeforeNode>( Modifiers->GetModifier(G_BEFORE) ) )
					loc->SetOrder( before );
				// Else if this note has the after modifier, set it.
				else if ( AfterNode* after = node_cast<AfterNode>( Modifiers->GetModifier(G_AFTER) ) )
					loc->SetOrder( after );	
			}
		}
		POSTPARSE_END;
	}
};

//==========================================
// DialectOPIncludeStatementNode
//==========================================

class DialectOPIncludeStatementNode : public DialectStatementBase
{
public:
	DECLARE_NODE(DialectOPIncludeStatementNode,DialectStatementBase,G_DIALECT_OPINCLUDE_STATEMENT);

	bool Parse()
	{
		PARSE_START;
		{
			if (Modifiers)
				Modifiers->CheckNone();
		}
		PARSE_END;
	}
};

//==========================================
// CategoryStatementNode
//==========================================

class CategoryStatementNode : public DialectStatementBase
{
public:
	DECLARE_NODE(CategoryStatementNode,DialectStatementBase,G_CATEGORY_STATEMENT);

	bool Parse()
	{
		PARSE_START;
		{
			if (Modifiers)
			{
				Modifiers->FindValuedDialectModifier<G_CLASS_PREFIX,  ClassPrefixNode>();
				Modifiers->FindValuedDialectModifier<G_STRUCT_PREFIX, StructPrefixNode>();
			}		
		}
		PARSE_END;
	}

	bool PostParse()
	{
		POSTPARSE_START;
		{
			if (Modifiers)
			{
				Modifiers->AllowOnly( G_CLASS_PREFIX, G_STRUCT_PREFIX );

				Modifiers->DisallowBoth( G_CLASS_PREFIX, G_STRUCT_PREFIX );
				
				CategoryNode* category = node_cast<CategoryNode>( Statement );

				// if we have a class prefix, set it
				if ( ClassPrefixNode* prefix = node_cast<ClassPrefixNode>( Modifiers->GetModifier(G_CLASS_PREFIX) ) )
					category->SetClassPrefix( prefix );
				// if we have a struct prefix, set it
				else if ( StructPrefixNode* prefix = node_cast<StructPrefixNode>( Modifiers->GetModifier(G_STRUCT_PREFIX) ) )
					category->SetStructPrefix( prefix );
			}
		}
		POSTPARSE_END;
	}
};

//==========================================
// EnumerationStatementNode
//==========================================

class EnumerationStatementNode : public DialectStatementBase
{
public:
	DECLARE_NODE(EnumerationStatementNode,DialectStatementBase,G_ENUMERATION_STATEMENT);

	bool Parse()
	{
		PARSE_START;
		{
			if (Modifiers)
			{
				Modifiers->FindValuedDialectModifier<G_ENUM_PREFIX, EnumPrefixNode>();
			}		
		}
		PARSE_END;
	}

	bool PostParse()
	{
		POSTPARSE_START;
		{
			if (Modifiers)
			{
				Modifiers->AllowOnly( G_ENUM_PREFIX );
				
				EnumerationNode* enumeration = node_cast<EnumerationNode>( Statement );

				// if we have an enum prefix, set it
				if ( EnumPrefixNode* prefix = node_cast<EnumPrefixNode>( Modifiers->GetModifier(G_ENUM_PREFIX) ) )
					enumeration->SetEnumPrefix( prefix );
			}
		}
		POSTPARSE_END;
	}
};

//==========================================
// DialectOPDefineStatementNode
//==========================================

class DialectOPDefineStatementNode : public DialectStatementBase
{
public:
	DECLARE_NODE(DialectOPDefineStatementNode,DialectStatementBase,G_DIALECT_OPDEFINE_STATEMENT);

	bool Parse()
	{
		PARSE_START;
		{
			if (Modifiers)
				Modifiers->CheckNone();
		}
		PARSE_END;
	}
};

//==========================================
// NoteDefinitionStatementNode
//==========================================

class NoteDefinitionStatementNode : public DialectStatementBase
{
public:
	DECLARE_NODE(NoteDefinitionStatementNode,DialectStatementBase,G_NOTE_DEFINITION_STATEMENT);

	bool Parse()
	{
		PARSE_START;
		{
			if (Modifiers)
			{
				Modifiers->FindDialectModifier<G_OVERRIDE, OverrideNode>();
				Modifiers->FindDialectModifier<G_VERBATIM, VerbatimNode>();
			}		
		}
		PARSE_END;
	}

	bool PostParse()
	{
		POSTPARSE_START;
		{
			if (Modifiers)
			{
				Modifiers->AllowOnly( G_OVERRIDE, G_VERBATIM );
				
				NoteDefinitionNode* notedef = node_cast<NoteDefinitionNode>( Statement );

				// If this node has the override modifier, set it on the note.
				if ( OverrideNode* override = node_cast<OverrideNode>( Modifiers->GetModifier(G_OVERRIDE) ) )
					notedef->SetOverride( override );

				// If this node has the verbatim modifier, set it on the note.
				if ( VerbatimNode* verbatim = node_cast<VerbatimNode>( Modifiers->GetModifier(G_VERBATIM) ) )
				    notedef->SetVerbatim(true);
			}
		}
		POSTPARSE_END;
	}
};

//==========================================
// FileDeclarationStatementNode
//==========================================

class FileDeclarationStatementNode : public DialectStatementBase
{
public:
	DECLARE_NODE(FileDeclarationStatementNode,DialectStatementBase,G_FILE_DECLARATION_STATEMENT);

	bool Parse()
	{
		PARSE_START;
		{
			if (Modifiers)
				Modifiers->CheckNone();
		}
		PARSE_END;
	}
};

//==========================================
// CodeStatementNode
//==========================================

class CodeStatementNode : public DialectStatementBase
{
public:
	DECLARE_NODE(CodeStatementNode,DialectStatementBase,G_CODE_STATEMENT);

	bool Parse()
	{
		PARSE_START;
		{
			// parse modifiers on notes
			if (Modifiers)
			{
				Modifiers->FindDialectModifier<G_VERBATIM, VerbatimNode>();
			}		
		}
		PARSE_END;
	}

	bool PostParse()
	{
		POSTPARSE_START;
		{
			if (Modifiers)
			{
				Modifiers->AllowOnly( G_VERBATIM );
				
				// Get the actual code statement.
				CodeNode* codenode = node_cast<CodeNode>( Statement );

				// Check for the verbatim modifier.
				// If it exists, set it on the code node.
				if ( Modifiers->HasModifier( G_VERBATIM ) )
					codenode->SetVerbatim(true);
			}
		}
		POSTPARSE_END;
	}
};

//==========================================
// DialectNamespaceStatementNode
//==========================================

class DialectNamespaceStatementNode : public DialectStatementBase
{
public:
	DECLARE_NODE(DialectNamespaceStatementNode,DialectStatementBase,G_DIALECT_NAMESPACE_STATEMENT);

	bool Parse()
	{
		PARSE_START;
		{
			if (Modifiers)
				Modifiers->CheckNone();
		}
		PARSE_END;
	}
};

//==========================================
// ExtensionStatementNode
//==========================================

class ExtensionStatementNode : public DialectStatementBase
{
public:
	DECLARE_NODE(ExtensionStatementNode,DialectStatementBase,G_EXTENSION_STATEMENT);

	bool Parse()
	{
		PARSE_START;
		{
			if (Modifiers)
				Modifiers->CheckNone();
		}
		PARSE_END;
	}
};

//==========================================
// ExtendPointStatementNode
//==========================================

class ExtendPointStatementNode : public DialectStatementBase
{
public:
	DECLARE_NODE(ExtendPointStatementNode,DialectStatementBase,G_EXTEND_POINT_STATEMENT);

	bool Parse()
	{
		PARSE_START;
		{
			if (Modifiers)
				Modifiers->CheckNone();
		}
		PARSE_END;
	}
};

//==========================================
// DialectOPMacroStatementNode
//==========================================

class DialectOPMacroStatementNode : public DialectStatementBase
{
public:
	DECLARE_NODE(DialectOPMacroStatementNode,DialectStatementBase,G_DIALECT_OPMACRO_STATEMENT);

	bool Parse()
	{
		PARSE_START;
		{
			if (Modifiers)
				Modifiers->CheckNone();
		}
		PARSE_END;
	}
};

} // end namespace dialect_nodes
