///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: ErrorNames.cpp
/// Date: 06/29/2007
///
/// Description:
///
/// Error Names
///****************************************************************


#include "opCpp.h"

///==========================================
/// grammars
///==========================================

///
/// OPObjectBodyNode
///

opString OPObjectBodyNode::ErrorName()
{
	if(!Object || !Object->GetCategorySettings())
		return "";

	opString name = Object->GetCategorySettings()->GetName();

	name += " ";

	if(Object->GetName())
	{
		name += Object->GetName()->GetValue();
	}
	else 
	{
		name += "Name";
	}

	name += " {" LCHEV " ... " RCHEV "};";

	return name;		
}

///
/// OPObjectNode
///

opString OPObjectNode::ErrorName()
{
	if(!GetCategorySettings())
		return "";

	opString name = GetCategorySettings()->GetName();

	name += " ";

	if(GetName())
	{
		name += GetName()->GetValue();
	}
	else 
	{
		name += "Name";
	}

	name += " { ... };";

	return name;	
}

///
/// OPObjectInheritanceNode
///

opString OPObjectInheritanceNode::ErrorName()
{
	if(!Object || !Object->GetCategorySettings())
		return "";

	opString name = Object->GetCategorySettings()->GetName();

	name += " ";

	if(Object->GetName())
	{
		name += Object->GetName()->GetValue();
	}
	else 
	{
		name += "Name";
	}

	//TODO: could improve this
	name += " " LCHEV " [: ";

 	name += "Parent] " RCHEV " { ... };";

	return name;
}

///
/// PointerNode
///

opString PointerNode::ErrorName()
{
	if(!Type)
		return "";

	opString name;
	Type->PrintString(name);

	int num = GetNumStars();
	for(int i = 0; i < num; i++)
	{
		StarType type = Stars[i];
		
		name += "*";

		if(type == Const)
			name += " const";
		else if(type == Volatile)
			name += " volatile";
		else if(type == ConstVolatile)
			name += " const volatile";
	}

	return name;
}

///
/// ReferenceNode
///

opString ReferenceNode::ErrorName()
{
	if(!Type)
		return "";

	opString name;
	Type->PrintString(name);

	name += "&";

	return name;
}

///
/// ScopeNode
///

opString ScopeNode::ErrorName()
{
	int num = (int)Scope.size();

	if(!num)
		return "";

	opString name;

	if(bGlobal)
		name += "::";

	for(int i = 0; i < num; i++)
	{
		Scope[i]->PrintString(name);

		if(i + 1 < num)
			name += "::";
	}

	return name;
}

///
/// ScopePointerNode
///

opString ScopePointerNode::ErrorName()
{
	int num = (int)Scope.size();

	if(!num)
		return "";

	opString name;

	if(bGlobal)
		name += "::";

	for(int i = 0; i < num; i++)
	{
		Scope[i]->PrintString(name);

		if(i + 1 < num)
			name += "::";
	}

	name += "::";
	
	for(int i = 0; i < Stars; i++)
		name += "*";

	return name;
}

///
/// PointerMemberNode
///

opString PointerMemberNode::ErrorName()
{
	if(!Scope)
		return "";
	
	opString name;
	
	if(Type)
		Type->PrintString(name);
	else
		name += "Type";
	
	name += ' ';
	
	if(Scope)
		Scope->PrintString(name);
	else
		name += "Scope";
	
	return name;
}


///
/// VisibilityLabelNode
///

opString VisibilityLabelNode::ErrorName()
{
	if(!Label)
		return "";

	return Label->GetValue() + ":";
}

///
/// FundamentalTypeNode
///

opString FundamentalTypeNode::ErrorName()
{
	if(!Type)
		return "";

	opString name;
	if(Signed)
		name += "signed";
	else
		name += "unsigned";

	name += " ";
	name += Type->GetValue();

	return name;
}

///
/// NamespaceNode
///

opString NamespaceNode::ErrorName()
{
// 	if(!GetDecl())
// 		return "";
	if(!GetName())
		return "";
	
	opString name = "namespace ";

	GetName()->PrintString(name);

	name += " { ... }";

	return name;
}

///
/// NamespaceAliasNode
///

opString NamespaceAliasNode::ErrorName()
{
	if(!GetName())
		return "";

	opString name = "namespace ";
	name += Name->GetValue();
	name += " = Value;";

	return name;
}

///
/// NamespaceBlockNode
///

opString NamespaceBlockNode::ErrorName()
{
	if(NamespaceNode* parent = FindParent<NamespaceNode>())
	{
//		if(!parent->GetDecl())
//			return "";
		if(!parent->GetName())
			return "";

		opString name = "namespace ";
		
		//parent->GetDecl()->GetName()->PrintString(name);
		parent->GetName()->PrintString(name);

		name += " {" LCHEV " ... " RCHEV "}";

		return name;
	}
	else
		return "";
}

///
/// OPEnumNode
///

opString OPEnumNode::ErrorName()
{
	if(!Name)
		return "";

	opString name = "openum ";
	name += Name->GetValue();
	name += " { ... };";

	return name;
}

///
/// OPEnumBodyNode
///

opString OPEnumBodyNode::ErrorName()
{
	if(OPEnumNode* parent = FindParent<OPEnumNode>())
	{
		if(!parent->GetName())
			return "";
		
		opString name = "openum ";
		name += parent->GetName()->GetValue();
		name += " {" LCHEV " ... " RCHEV "};";
		
		return name;
	}
	else
		return "";
}

///
/// OPEnumElementNode
///

opString OPEnumElementNode::ErrorName()
{
	if(!Name)
		return "";
	
	opString name;
	name += Name->GetValue();

	if(Initialization)
	{
		name += " = ";
		Initialization->PrintString(name);
	}

	name += ",";
	return name;
}

///
/// ArrayNode
///

opString ArrayNode::ErrorName()
{
	if (!Name)
		return "";

	opString name = Name->GetValue();

	int num = (int) Brackets.size();

	for (int i = 0; i < num; i++)
		Brackets[i]->PrintString(name);

	return name;
}

///
/// TypeArrayNode
///

opString TypeArrayNode::ErrorName()
{
	if (!Type)
		return "";

	opString name;
	
	Type->PrintString(name);

	int num = (int) Brackets.size();

	for (int i = 0; i < num; i++)
		Brackets[i]->PrintString(name);

	return name;
}


///
/// FriendNode
///

opString FriendNode::ErrorName()
{
	if ( !Friend )
		return "";

	opString name = "friend ";

	if (GetForwardDeclType() == Struct)
		name += "struct ";
	else if (GetForwardDeclType() == Class)
		name += "class ";

	name += Friend->ErrorName();
	//Friend->PrintString(name);

	return name;
}

///
/// CPPConstructNode
///

opString CPPConstructNode::ErrorName()
{
	opString name = Type;

	if (Name)
	{
		name += ' ';
		Name->PrintString(name);
	}

	name += " { ... };";

	return name;
}

//
// CPPConstructObjectNode
//

opString CPPConstructObjectNode::ErrorName()
{
	opString name = Type;

	if (Name)
	{
		name += ' ';
		Name->PrintString(name);

		if (Inheritance)
		{
			name += ' ';
			Inheritance->PrintString(name);
		}
	}

	name += " { ... };";

	return name;
}

///
/// TypedefNode
///

opString TypedefNode::ErrorName()
{
	if (!Type)
		return "";

	opString name = "typedef ";

	if (Modifier)
	{
		name += Modifier->GetValue();
		name += " ";
	}

	Type->PrintString(name);

	name += " ";

	if (Name)
		Name->PrintString(name);
	else
		name += "Name";

	return name;
}

///
/// StateNode
///

opString StateNode::ErrorName()
{
	if(!Name)
		return "";

	opString name = "state ";
	name += Name->GetValue();
	name += " ";
	name += "{ ... }";

	return name;
}

///
/// StateBodyNode
///

opString StateBlockNode::ErrorName()
{
	if(StateNode* parent = FindParent<StateNode>())
	{
		if(!parent->GetName())
			return "";

		opString name = "state ";
		name += parent->GetName()->GetValue();
		name += " ";
		name += "{" LCHEV " ... " RCHEV "}";

		return name;
	}

	return "";
}

///
/// TemplateTypeNode
///

opString TemplateTypeNode::ErrorName()
{
	if ( !Name )
		return "";

	opString name;

	Name->PrintString(name);

	if ( !Braces )
		name += "<" LCHEV " ... " RCHEV ">";
	else
		Braces->PrintString(name);

	return name;
}

///
/// PreprocessorNodes
///

opString PreprocessorNode::ErrorName()
{
	opString name;

	name += "#";
	name += DirectiveName;

	PrintString(name);

	opError::TruncateErrorName(name);

	return name;
}

///
/// TemplateDeclNode
///

opString TemplateDeclNode::ErrorName()
{
	if(!braces)
		return "";

	opString name;
	name += "template";
	braces->PrintString(name);

	return name;
}

///
/// FunctionNode
///

opString FunctionNode::ErrorName()
{
	if(!Name)
		return "";

	opString name;
	//Name->PrintString(name);//NOTE: this is potentially sketchy for operators. use errorname instead?
	//Better?
	opString err = Name->ErrorName();
	if(!err.Length())
		err = opError::GetParseError(Name->GetId());

	name += err;

	if(!Arguments)
	{
		name += "( ... )";
	}
	else
	{
		//name += '(';
		Arguments->PrintString(name);
		//name += ')';
	}

	if(bConst)
		name += " const";

	if(Assignment)
	{
		name += " = ";
		name += Assignment->GetValue();
	}

	return name;
}

///
/// FunctionDefinitionNode
///

opString FunctionDefinitionNode::ErrorName()
{
	if (!Function)
		return "";

	opString name;

	if (Return)
		Return->PrintString(name);
	else
		name += "ReturnType";

	name += " ";

	opString fname = Function->ErrorName();

	if (!fname.Length())
		name += opError::GetParseError(G_FUNCTION);
	else
		name += fname;

	if (Body)
		name += " { ... }";
	else
		name += " " LCHEV " ... " RCHEV " { ... }";

	return name;
}

///
/// FunctionPrototypeNode
///

opString FunctionPrototypeNode::ErrorName()
{
	if(!Function)
		return "";

	opString name;
	if(Return)
	{
		Return->PrintString(name);
	}
	else
		name += "ReturnType";

	name += " ";

	opString fname = Function->ErrorName();
	if(!fname.Length())
		name += opError::GetParseError(G_FUNCTION);
	else
		name += fname;

	return name;
}

///
/// FunctionArgumentListNode
///

opString FunctionArgumentListNode::ErrorName()
{
	if(FunctionNode* parent = FindParent<FunctionNode>())
	{
		if(!parent->GetName())
			return "";

		opString name;
		parent->GetName()->PrintString(name);

		name += "(" LCHEV " ... " RCHEV ")";

		if(parent->IsConst())
			name += " const";

		return name;
	}

	return "";
}

///
/// FunctionArgumentNode
///

opString FunctionArgumentNode::ErrorName()
{
	if(!Definition)
		return "";

	opString name;
	Definition->PrintString(name);

	if(Default)
	{
		name += " = ";
		Default->PrintString(name);
	}

	return name;
}


///
/// TypenameNode
///

opString TypenameNode::ErrorName()
{
	if(!Name)
		return "";

	opString name = "typename ";

	Name->PrintString(name);

	return name;
}

///
/// ValuedModifierNode
///

opString ValuedModifierNode::ErrorName()
{
	if(!Modifier)
		return "";

	opString name;
	name += Modifier->GetValue();

	if(Arguments)
	{
		Arguments->PrintString(name);
	}
	else
		name += "( ... )";

	return name;
}

///
/// ValueModifierArgumentNode
///

opString ValuedModifierArgumentNode::ErrorName()
{
	if(ValuedModifierNode* parent = FindParent<ValuedModifierNode>())
	{
		if(!parent->GetModifier())
			return "";
		
		opString name;
		name = parent->GetModifier()->GetValue();
		
		name += "(" LCHEV " ... " RCHEV ")";
		
		return name;
	}
	else
		return "";
}

///
/// OPIncludeNode
///

opString OPIncludeNode::ErrorName()
{
	if(!FileName)
		return "";
	
	opString name = "opinclude ";
	name += FileName->GetValue();
	
	return name;
}


///
/// ConstructorNode
///

opString ConstructorNode::ErrorName()
{
	if(!function)
		return "";
	return function->ErrorName();
}

///
/// DestructorNode
///

opString DestructorNode::ErrorName()
{
	//TODO: this is wrong, need to build a different one.
	if(!function)
		return "";

	return function->ErrorName();
}

///
/// DestructorPrototypeNode
///

opString DestructorPrototypeNode::ErrorName()
{
	if(!Destructor)
		return "";
	return Destructor->ErrorName();
}

///
/// DestructorDefinitionNode
///

opString DestructorDefinitionNode::ErrorName()
{
	if(!Destructor)
		return "";

	opString errorname = Destructor->ErrorName();
	if(!errorname.Length())
		return "";

	opString name = errorname + " { ... }";

	return name;
}

///
/// DestructorDefinitionStatementNode
///

opString DestructorDefinitionStatementNode::ErrorName()
{
	opString name;

	if(GetModifiers())
	{
		GetModifiers()->PrintString(name);
		name += " ";
	}

	opString fname;

	if(DestructorDefinition)
		fname = DestructorDefinition->ErrorName();

	if(!fname.Length())
		name += opError::GetParseError(G_DESTRUCTOR_DEFINITION);
	else
		name += fname;

	return name;
}

///
/// DestructorPrototypeStatementNode
///

opString DestructorPrototypeStatementNode::ErrorName()
{
	opString name;

	if(GetModifiers())
	{
		GetModifiers()->PrintString(name);
		name += " ";
	}

	opString fname;

	if(Destructor)
		fname = Destructor->ErrorName();

	if(!fname.Length())
		name += opError::GetParseError(G_DESTRUCTOR_PROTOTYPE);
	else
		name += fname;

	return name;
}

///
/// CPPConstructStatementNode
///

opString CPPConstructStatementNode::ErrorName()
{
	if (!Construct)
		return "";

	return Construct->ErrorName();
}

///
/// ConstructorPrototypeNode
///

opString ConstructorPrototypeNode::ErrorName()
{
	if(!Constructor)
		return "";
	return Constructor->ErrorName();
}

///
/// ConstructorDefinitionNode
///

opString ConstructorDefinitionNode::ErrorName()
{
	if(!Constructor)
		return "";

	opString errorname = Constructor->ErrorName();
	if(!errorname.Length())
		return "";

	opString name = errorname + " { ... }";
	
	return name;
}

///
/// ConstructorInitializerNode
///

opString ConstructorInitializerNode::ErrorName()
{
	if(!Name)
		return "";

	opString name;
	Name->PrintString(name);
	name += "( ... )";
	
	return name;
}

///
/// ConstructorInitializerListNode
///

opString ConstructorInitializerListNode::ErrorName()
{
	if(ConstructorDefinitionNode* parent = FindParent<ConstructorDefinitionNode>())
	{
		if(!parent->GetConstructor())
			return "";
		
		opString errorname = parent->GetConstructor()->ErrorName();
		if(!errorname.Length())
			return "";
		
		opString name = errorname + " : " LCHEV " ... " RCHEV;
		
		return name;
	}
	
	return "";
}

///
/// ConstructorInitializerArgumentNode
///

opString ConstructorInitializerArgumentNode::ErrorName()
{
	if(ConstructorInitializerNode* parent = FindParent<ConstructorInitializerNode>())
	{
		if(!parent->GetName())
			return "";
		
		opString name;
		parent->GetName()->PrintString(name);
		name += "(" LCHEV " ... " RCHEV ")";

		return name;
	}

	return "";
}


///
/// OPDefineNode
///

opString OPDefineNode::ErrorName()
{
	if(!Name)
		return "";

	opString name;
	name += "opdefine ";
	name += Name->GetValue();
	
	if(!Arguments)
	{
		if(!Body)
			name += " [optional ( ... )] { ... }";
		else
			name += " { ... }";
	}
	else
	{
		name += " ( ... ) { ... }";
	}

	return name;
}

///
/// OPDefineBodyNode
///

opString OPDefineBodyNode::ErrorName()
{
	if(OPDefineNode* parent = FindParent<OPDefineNode>())
	{
		if(!parent->GetName())
			return "";

		opString name;
		name += "opdefine ";
		name += parent->GetName()->GetValue();

		if(parent->GetArguments())
			name += " ( ... )";
		
		name += " {" LCHEV " ... " RCHEV "}";

		return name;
	}
	return "";
}

///
/// OPMacroNode
///

opString OPMacroNode::ErrorName()
{
	if(!Name)
		return "";

	opString name;
	name += "opmacro ";
	name += Name->GetValue();

	if(!Arguments)
	{
		if(!Body)
			name += " [optional ( ... )] { ... }";
		else
			name += " { ... }";
	}
	else
	{
		name += " ( ... ) { ... }";
	}

	return name;
}

///
/// OPMacroBodyNode
///

opString OPMacroBodyNode::ErrorName()
{
	if(OPMacroNode* parent = FindParent<OPMacroNode>())
	{
		if(!parent->GetName())
			return "";

		opString name;
		name += "opmacro ";
		name += parent->GetName()->GetValue();

		if(parent->GetArguments())
			name += " ( ... )";

		name += " {" LCHEV " ... " RCHEV "}";

		return name;
	}
	return "";
}

///
/// OPMacroArgumentListNode
///

opString OPMacroArgumentListNode::ErrorName()
{
	if(OPMacroNode* parent = FindParent<OPMacroNode>())
	{
		if(!parent->GetName())
			return "";

		opString name;
		name += "opmacro ";
		name += parent->GetName()->GetValue();

		name += " (" LCHEV " ... " RCHEV ")";

		return name;
	}
	return "";
}

///
/// ExpandCallNode
///

opString ExpandCallNode::ErrorName()
{
	if(!Name)
		return "";

	opString name;
	name += "expand ";
	Name->PrintString(name);

	if(Arguments)
	{
		name += "( ... )";
	}

	return name;
}

///
/// ExpandCallArgumentListNode
///

opString ExpandCallArgumentListNode::ErrorName()
{
	if(ExpandCallNode* parent = FindParent<ExpandCallNode>())
	{
		if(!parent->GetName())
			return "";

		opString name = "expand ";
		parent->GetName()->PrintString(name);
		name += "(" LCHEV " ... " RCHEV ")";
		
		return name;
	}

	return "";
}

///
/// ConcatenationOperatorNode
///

opString ConcatenationOperatorNode::ErrorName()
{
	if (!Left)
		return "";

	opString name;

	Left->PrintString(name);
	name += "@";

	if (Right)
		Right->PrintString(name);
	else
		name += LCHEV " Identifier " RCHEV;

	return name;
}

///
/// SingleQuoteOperatorNode
///

opString SingleQuoteOperatorNode::ErrorName()
{
	opString text;

	PrintStringChildren(text);

	opError::TruncateErrorName(text);

	opString name = "`" + text + "`";

	return name;
}

///
/// DoubleQuoteOperatorNode
///

opString DoubleQuoteOperatorNode::ErrorName()
{
	opString text;

	PrintString(text);

	opError::TruncateErrorName(text);

	opString name = '"' + text + '"';

	return name;
}

///
/// CPlusPlusNode
///

opString CPlusPlusNode::ErrorName()
{
	return "";
}

///
/// FunctionPointerNode
///

opString FunctionPointerNode::ErrorName()
{
	if(!Decl)
		return "";

	opString name;

	if(Return)
	{
		Return->PrintString(name);
	}
	else
		name += "ReturnType";

	name += " ";

	Decl->PrintString(name);

	if(Arguments)
		Arguments->PrintString(name);
	else
		name += "( ... )";

	return name;
}

///
/// FunctionPointerDeclNode
///

opString FunctionPointerDeclNode::ErrorName()
{
	if(FunctionPointerNode* parent = FindParent<FunctionPointerNode>())
	{
		opString name;

		if(parent->GetReturn())
		{
			parent->GetReturn()->PrintString(name);
		}
		else
			name += "ReturnType";

		name += " ";

		name += "(" LCHEV " Scope::* Name " RCHEV ")";

		if(parent->GetArguments())
			parent->GetArguments()->PrintString(name);
		else
			name += "( ... )";

		return name;

	}

	return "";
}

///
/// OperatorNode
///

opString OperatorNode::ErrorName()
{
	if(!OperatorType)
		return "";

	opString name;
	name += "operator ";
	OperatorType->PrintString(name);
	
	return name;
}

//
// UsingNode
//

opString UsingNode::ErrorName()
{
	if (!Scope)
		return "";

	opString name = "using ";

	Scope->PrintString(name);

	return name;
}

//
// CodeNode
//

opString CodeNode::ErrorName()
{
	return "";
}

opString CodeBodyNode::ErrorName()
{
	return "";
}

//
// ExtensionNode
//

opString ExtensionNode::ErrorName()
{
	if (!Name)
		return "";

	opString name = "extension ";

	Name->PrintString(name);

	return name;
}

//
// ExtendPointNode
//

opString ExtendPointNode::ErrorName()
{
	if (!Name)
		return "";

	opString name = "extendpoint ";

	Name->PrintString(name);

	name += " { ... }";

	return name;
}

//
// ExtensionPointNode
//

opString ExtensionPointNode::ErrorName()
{
	if (!Name)
		return "";

	opString name = "extensionpoint ";

	Name->PrintString(name);

	return name;
}

//
// FileDeclarationLocationNode
//

opString FileDeclarationLocationNode::ErrorName()
{
	if (!Name)
		return "";

	opString name = "location ";

	Name->PrintString(name);

	return name;
}

//
// FileDeclarationNode
//

opString FileDeclarationNode::ErrorName()
{
	if (!Name)
		return "";

	opString name = "file ";

	Name->PrintString(name);

	name += " { ... }";

	return name;
}

//
// DialectNamespaceNode
//

opString DialectNamespaceNode::ErrorName()
{
	if (!Name)
		return "";

	opString name = "namespace ";

	Name->PrintString(name);

	name += " { ... }";

	return name;
}

//
// TemplatedNode
//

opString TemplatedNode::ErrorName()
{
	if ( !TheTemplate )
		return "";

	opString name = "";

	TheTemplate->PrintString(name);

	if ( Templated )
		Templated->PrintString(name);
	else
		name += " Construct";

	return name;
}

///==========================================
/// statements
///==========================================

///
/// FriendStatementNode
///

opString FriendStatementNode::ErrorName()
{
	if (Friend)
		return Friend->ErrorName();

	return "";
}

///
/// UsingStatementNode
///

opString UsingStatementNode::ErrorName()
{
	if (Using)
		return Using->ErrorName();

	return "";
}

///
/// TypedefStatementNode
///

opString TypedefStatementNode::ErrorName()
{
	if (Typedef)
		return Typedef->ErrorName();

	return "";
}

///
/// StatementNode
///

opString StatementNode::ErrorName()
{
	if(OPObjectBodyNode* parent = FindParent<OPObjectBodyNode>())
		return parent->ErrorName();

	return "";
}

opString PreprocessorStatementNode::ErrorName()
{
	if (!Preprocessor)
		return "";

	return Preprocessor->ErrorName();
}

///
/// StateStatementNode
///

opString StateStatementNode::ErrorName()
{
	if(!State)
		return "";

	return State->ErrorName();
}

///
/// TemplateStatementNode
///

opString TemplateStatementNode::ErrorName()
{
	if ( !Templated )
		return "";

	opString name = "";

	Templated->PrintString(name);

	return name;
}

///
/// FuncPointerStatementNode
///

opString FuncPointerStatementNode::ErrorName()
{
	opString name;
	
	if(GetModifiers())
	{
		GetModifiers()->PrintString(name);
		name += " ";
	}
	
	opString fname;

	if(FunctionPointer)
		fname = FunctionPointer->ErrorName();
	
	if(!fname.Length())
		name += opError::GetParseError(G_FUNCTION_POINTER);
	else
		name += fname;

	return name;
}

///
/// FunctionDefinitionStatementNode
///

opString FunctionDefinitionStatementNode::ErrorName()
{
	opString name;
	
	if(GetModifiers())
	{
		GetModifiers()->PrintString(name);
		name += " ";
	}
	
	opString fname;

	if(FunctionDefinition)
		fname = FunctionDefinition->ErrorName();

	if(!fname.Length())
		name += opError::GetParseError(G_FUNCTION_DEFINITION);
	else
		name += fname;
	
	return name;
}

///
/// FunctionPrototypeStatementNode
///

opString FuncPrototypeStatementNode::ErrorName()
{
	opString name;

	if(GetModifiers())
	{
		GetModifiers()->PrintString(name);
		name += " ";
	}

	opString fname;

	if(FunctionPrototype)
		fname = FunctionPrototype->ErrorName();

	if(!fname.Length())
		name += opError::GetParseError(G_FUNCTION_PROTOTYPE);
	else
		name += fname;

	return name;
}

///
/// DataStatementNode
///

opString DataStatementNode::ErrorName()
{
	opString name;

	if(GetModifiers())
	{
		GetModifiers()->PrintString(name);
		name += " ";
	}

	if(!GetType())
		name += "Type";
	else
		GetType()->PrintString(name);

	name += " ";

	if(!GetName())
		name += "Name";
	else
		GetName()->PrintString(name);

	if(GetBits())
	{
		name += " : ";
		name += GetBits()->GetNumber()->GetValue();
	}

	if(Initialization)
	{
		name += " = ";
		Initialization->PrintString(name);
	}

	return name;
}

///
/// DataDeclarationNode
///

opString DataDeclarationNode::ErrorName()
{
	if(DataStatementNode* parent = FindParent<DataStatementNode>())
	{
		opString name;

		if(parent->GetModifiers())
		{
			parent->GetModifiers()->PrintString(name);
			name += " ";
		}

		if(Type)
			Type->PrintString(name);
		else
			name += "Type";

		name += " ";

		if(Name)
			Name->PrintString(name);
		else
			name += "Name";

		if(Bits && Bits->GetNumber())
		{
			name += " : ";
			name += Bits->GetNumber()->GetValue();
		}

		return name;
	}

	return "";
}

///
/// DataInitializationNode
///

opString DataInitializationNode::ErrorName()
{
	if(DataStatementNode* parent = FindParent<DataStatementNode>())
	{
		opString name;

		if(parent->GetModifiers())
		{
			parent->GetModifiers()->PrintString(name);
			name += " ";
		}

		if(parent->GetType())
			parent->GetType()->PrintString(name);
		else
			name += "Type";

		name += " ";

		if(parent->GetName())
			parent->GetName()->PrintString(name);
		else
			name += "Name";

		if(parent->GetBits() && parent->GetBits()->GetNumber())
		{
			name += " : ";
			name += parent->GetBits()->GetNumber()->GetValue();
		}

		return name;
	}

	return "";
}

///
/// ConstructorDefinitionStatementNode
///

opString ConstructorDefinitionStatementNode::ErrorName()
{
	opString name;

	//TODO: it needs to only print out valid modifiers I guess...
// 	if(GetModifiers())
// 	{
// 		GetModifiers()->PrintString(name);
// 		name += " ";
// 	}

	opString fname;

	if(ConstructorDefinition)
		fname = ConstructorDefinition->ErrorName();

	if(!fname.Length())
		name += opError::GetParseError(G_CONSTRUCTOR_DEFINITION);
	else
		name += fname;

	return name;
}

///
/// ConstructorPrototypeStatementNode
///

opString ConstructorPrototypeStatementNode::ErrorName()
{
	opString name;

	if(GetModifiers())
	{
		GetModifiers()->PrintString(name);
		name += " ";
	}
	
	opString fname;
	
	if(Constructor)
		fname = Constructor->ErrorName();

	if(!fname.Length())
		name += opError::GetParseError(G_CONSTRUCTOR_PROTOTYPE);
	else
		name += fname;
	
	return name;
}

///
/// OPEnumStatementNode
///

opString OPEnumStatementNode::ErrorName()
{
	opString name;

	if(GetModifiers())
	{
		GetModifiers()->PrintString(name);
		name += " ";
	}

	opString fname;

	if(Enum)
		fname = Enum->ErrorName();

	if(!fname.Length())
		name += opError::GetParseError(G_OPENUM);
	else
		name += fname;

	return name;
}

///
/// OPObjectStatementNode
///

opString OPObjectStatementNode::ErrorName()
{
	opString name;

	if(GetModifiers())
	{
		GetModifiers()->PrintString(name);
		name += " ";
	}

	opString fname;

	if(Object)
		fname = Object->ErrorName();

	if(!fname.Length())
		name += opError::GetParseError(G_OPOBJECT);
	else
		name += fname;

	return name;
}

///==========================================
/// dialect grammars
///==========================================

///
/// CategoryNode
///

opString CategoryNode::ErrorName()
{
	if(!Name)
		return "";
	
	opString name;
	name = "category ";
	name += Name->GetValue();
	
	name += " { ... }";
	
	return name;
}

///
/// CategoryBodyNode
///

opString CategoryBodyNode::ErrorName()
{
	if(CategoryNode* parent = FindParent<CategoryNode>())
	{
		if(!parent->GetName())
			return "";

		opString name;
		name = "category ";
		name += parent->GetName()->GetValue();
		name += " {" LCHEV " ... " RCHEV "}";

		return name;
	}

	return "";
}

///
/// CategoryDatamapNode
///

opString CategoryDatamapNode::ErrorName()
{
	if(!Name)
		return "";

	opString name;
	name = "datamap ";
	name += Name->GetValue();
	name += " { ... }";

	return name;
}

///
/// CategoryFunctionmapNode
///

opString CategoryFunctionmapNode::ErrorName()
{
	if(!Name)
		return "";

	opString name;
	name = "functionmap ";
	name += Name->GetValue();
	name += " { ... }";

	return name;
}

///
/// DataModifierNode
///

opString DataModifierNode::ErrorName()
{
	if(!Name)
		return "";

	opString name;
	name = "datamodifier ";
	name += Name->GetValue();

	if(Arguments)
		Arguments->PrintString(name);

	return name;
}

///
/// FunctionModifierNode
///

opString FunctionModifierNode::ErrorName()
{
	if(!Name)
		return "";

	opString name;
	name = "functionmodifier ";
	name += Name->GetValue();

	if(Arguments)
		Arguments->PrintString(name);

	return name;
}

///
/// IsNode
///

opString IsNode::ErrorName()
{
	if(!Body)
		return "";
	
	opString name;
	name += "is ";
	
	name += Body->ErrorName();

	return name;
}

///
/// CriteriaGroupNode
///

opString CriteriaGroupNode::ErrorName()
{
	return "(" + CriteriaBodyNode::ErrorName() + ")";
}

///
/// CriteriaBodyNode
///

opString CriteriaBodyNode::ErrorName()
{
	opString name;

	if(!Operands.Size())
		return "";

	if(bNegate)
		name += "not ";

	Operands[0]->PrintString(name);
	name += " ";

	int numoperators = Operators.Size();
	for(int i = 0; i < numoperators; i++)
	{
		Operator op = Operators[i];

		if(op == And)
			name += "and";
		else if(op == And_Not)
			name += "and not";
		else if(op == Or)
			name += "or";
		else if(op == Or_Not)
			name += "or not";

		name += " ";

		Operands[i+1]->PrintString(name);
		
		if(i + 1 < numoperators)
			name += " ";
	}

	return name;
}

///
/// CriteriaArgumentNode
///

opString CriteriaArgumentNode::ErrorName()
{
	opString name;
	
	if(CriteriaValueModifierNode* parent = node_cast<CriteriaValueModifierNode>(GetParent()))
	{
		if(!parent->GetName())
			return "";

		name += "is";
		name += " ";
		name += parent->GetName()->GetValue();
	}
	else
		return "";
	
	name += "(" LCHEV " \"optional regex pattern\" " RCHEV ")";

	return name;
}

///
/// CriteriaValueModifierNode
///

opString CriteriaValueModifierNode::ErrorName()
{
	if(!Name)
		return "";

	opString name;
	
	name += Name->GetValue();
	name += "(\"optional regex pattern\")";
	
	return name;
}


///
/// CategoryLocationNode
///

opString LocationNodeBase::ErrorName()
{
	if(!Name)
		return "";

	opString name;
	name += "location ";
	Name->PrintString(name);
	name += " { ... }";

	return name;
}

///
/// CategoryLocationBodyNode
///

opString CategoryLocationBodyNode::ErrorName()
{
	if(CategoryLocationNode* parent = FindParent<CategoryLocationNode>())
	{
		if(!parent->GetName())
			return "";
		
		opString name;
		name += "location ";
		parent->GetName()->PrintString(name);

		name += " {" LCHEV " ... " RCHEV "}";
		return name;
	}

	return "";
}

///
/// NoteNode
///

opString NoteNode::ErrorName()
{
	if(!Name)
		return "";

	opString name;
	name = "note ";
	Name->PrintString(name);
	
	return name;
}

///
/// NoteBodyNode
///

opString NoteBodyNode::ErrorName()
{
	//TODO: complete this
	return "";
}

///
/// ModifierArgumentNode
///

opString ModifierArgumentNode::ErrorName()
{
	if(DataModifierNode* parent = node_cast<DataModifierNode>(GetParent()))
	{
		opString name;
		parent->GetName()->PrintString(name);
		name += "(" LCHEV " \"optional regex expression\" " RCHEV ")";
		return name;
	}
	else if(FunctionModifierNode* parent = node_cast<FunctionModifierNode>(GetParent()))
	{
		opString name;
		parent->GetName()->PrintString(name);
		name += "(" LCHEV " \"optional regex expression\" " RCHEV ")";
		return name;
	}

	return "";
}

///
/// CategoryMapBodyNode
///

opString CategoryMapBodyNode::ErrorName()
{
	if(CategoryDatamapNode* parent = node_cast<CategoryDatamapNode>(GetParent()))
	{
		opString name;
		name = "datamap ";
		if(parent->GetName())
			parent->GetName()->PrintString(name);
		else
			name += "Name";
		name += " {" LCHEV " ... " RCHEV "}";
		return name;
	}
	else if(CategoryFunctionmapNode* parent = node_cast<CategoryFunctionmapNode>(GetParent()))
	{
		opString name;
		name = "functionmap ";
		if(parent->GetName())
			parent->GetName()->PrintString(name);
		else
			name += "Name";
		name += " { " LCHEV " ... " RCHEV "}";
		return name;
	}

	return "";
}

///
/// NoteDefinitionNode
///

opString NoteDefinitionNode::ErrorName()
{
	if(!Path)
		return "";

	opString name;

	name = "note ";
	Path->PrintString(name);
	
	name += "( ... ) { ... }";

	return name;
}

///
/// NoteArgumentListNode
///

opString NoteArgumentListNode::ErrorName()
{
	if(NoteDefinitionNode* parent = FindParent<NoteDefinitionNode>())
	{
		opString name;

		if(!parent->GetPath())
			return "";

		name = "note ";
		parent->GetPath()->PrintString(name);
		
		name += "( ... ) { ... }";
		
		return name;
	}

	return "";
}

///
/// EnumerationNode
///

opString EnumerationNode::ErrorName()
{
	if(!Name)
		return "";

	opString name;
	name = "enumeration ";
	Name->PrintString(name);
	name += " { ... }";

	return name;
}


///
/// EnumerationBodyNode
///

opString EnumerationBodyNode::ErrorName()
{
	if(EnumerationNode* parent = FindParent<EnumerationNode>())
	{
		if(!parent->GetName())
			return "";

		opString name;
		name = "enumeration ";
		parent->GetName()->PrintString(name);
		name += " {" LCHEV " ... " RCHEV "}";

		return name;
	}

	return "";
}

///
/// EnumerationLocationNode
///

opString EnumerationLocationNode::ErrorName()
{
	if(!Name)
		return "";

	opString name;
	name = "location ";
	Name->PrintString(name);
	name += " { ... }";
	
	return name;
}

///
/// EnumerationLocationBodyNode
///

opString EnumerationLocationBodyNode::ErrorName()
{
	if(EnumerationLocationNode* parent = FindParent<EnumerationLocationNode>())
	{
		if(!parent->GetName())
			return "";

		opString name;
		name = "location ";
		parent->GetName()->PrintString(name);
		name += " {" LCHEV " ... " RCHEV "}";

		return name;
	}

	return "";
}

///
/// EnumerationMapNode
///

opString EnumerationMapNode::ErrorName()
{
	if(!Name)
		return "";

	opString name;
	name = "enummap ";
	Name->PrintString(name);

	return name;
}

///
/// DisallowNode
///

opString DisallowNode::ErrorName()
{
	if(!Name)
		return "";

	opString name;
	name += "disallow";
	name += " ";
	Name->PrintString(name);
	name += " { ... }";
	
	return name;
}

///
/// DisallowBodyNode
///

opString DisallowBodyNode::ErrorName()
{
	if(DisallowNode* parent = FindParent<DisallowNode>())
	{
		if(!parent->GetName())
			return "";

		opString name;
		name += "disallow";
		name += " ";
		parent->GetName()->PrintString(name);
		name += " {" LCHEV " ... " RCHEV "}";

		return name;

	}

	return "";
	
}

///==========================================
/// dialect statements
///==========================================

opString DialectStatementBase::ErrorName()
{
	if(!Statement)
		return "";
	
	opString name;
	
	if(Modifiers)
	{
		Modifiers->PrintString(name);
		name += " ";
	}

	if(Statement)
	{
		Statement->PrintString(name);
	}
	
	return name;
}

