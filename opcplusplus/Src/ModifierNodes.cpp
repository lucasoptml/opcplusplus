///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: ModifierNodes.cpp
/// Date: 12/20/2006
///
/// Description:
///
/// Modifier node source.
///****************************************************************

#include "opCPP.h"

///
/// ModifiersBase
///

// has a basic modifier? by token
bool ModifiersBase::HasModifier(Token tok)
{
	iterator i   = GetBegin();
	iterator end = GetEnd();

	while (i != end)
	{
		if (i->GetId() == tok)
			return true;

		++i;
	}

	return false;
}

// has a basic modifier? by string
bool ModifiersBase::HasModifier(const opString& modifiername)
{
	iterator i   = GetBegin();
	iterator end = GetEnd();

	while (i != end)
	{
		if ( TerminalNode* node = node_cast<TerminalNode>(*i) )
		{
			if(node->GetValue() == modifiername)
				return true;
		}

		++i;
	}

	return false;
}

TerminalNode* ModifiersBase::FindModifier(Token modifiertoken)
{
	iterator i   = GetBegin();
	iterator end = GetEnd();

	while (i != end)
	{
		if (i->GetId() == modifiertoken)
			return node_cast<TerminalNode>(*i);

		++i;
	}

	return NULL;
}

opNode* ModifiersBase::GetModifier(Token modifiertoken)
{
	iterator i   = GetBegin();
	iterator end = GetEnd();

	while (i != end)
	{
		if (i->GetId() == modifiertoken)
			return *i;

		++i;
	}

	return NULL;
}

ValuedModifierNode* ModifiersBase::GetValuedModifier(const opString& modifiername)
{
	iterator i   = GetBegin();
	iterator end = GetEnd();
	
	while (i != end)
	{
		if (ValuedModifierNode* valmod = node_cast<ValuedModifierNode>(*i))
		{
			if(valmod->GetModifier()->GetValue() == modifiername)
				return valmod;
		}

		++i;
	}

	return NULL;
}

void ModifiersBase::BuildValueModifiers(vector<ValuedModifierNode*>& modifierlist)
{
	iterator i   = GetBegin();
	iterator end = GetEnd();

	while (i != end)
	{
		if (ValuedModifierNode* valmod = node_cast<ValuedModifierNode>(*i))
		{
			modifierlist.push_back(valmod);
		}

		++i;
	}
}

///
/// ModifiersNode
///

void ModifiersNode::CheckDataModifiers()
{
	ResetPosition();

	while(GetPosition() != GetEnd())
	{
		opNode* node = CurrentNode();

		if(ValuedModifierNode* newnode = node_cast<ValuedModifierNode>(node))
		{
			//make sure its a valid value data modifier
			opString name = newnode->GetModifier()->GetValue();

			DialectCategory* category = FindParent<OPObjectNode>()->GetCategorySettings();

			DataModifierNode* modifier = category->FindValueDataModifier(name);

			//ask the modifier
			if(!modifier)
			{
				opError::InvalidDataModifierError(newnode);
				//GenericError("Not a data modifier (...)");
			}
			else if(modifier->GetArguments()->GetCondition())
			{
				opString stringvalue;
				newnode->GetArguments()->PrintString(stringvalue);

				opString pattern = modifier->GetArguments()->GetCondition()->GetValue();
				pattern = pattern.Substring(1,pattern.Size()-1);

				//check regex pattern!
				if(!opregex::Match(stringvalue,pattern,modifier))
				{
					//GenericError("Invalid data modifier value, pattern mismatch (" + pattern + ")");
					opError::InvalidValueError(newnode,modifier);
				}
			}		
		}
		else if ( node->GetId() == T_MODIFIER )
		{
			//make sure its a valid data modifier
			opString name = node->GetStringValue();

			DialectCategory* category = FindParent<OPObjectNode>()->GetCategorySettings();

			//ask the modifier
			if(!category->FindDataModifier(name))
			{
				opError::InvalidDataModifierError(node);
			}
		}
		else if ( node->GetId() == T_ID )
		{
			opError::InvalidDataModifierError(node);
		}

		IncrementPosition();
	}
}

void ModifiersNode::CheckFunctionModifiers()
{
	ResetPosition();

	while(GetPosition() != GetEnd())
	{
		opNode* node = CurrentNode();

		if(ValuedModifierNode* newnode = node_cast<ValuedModifierNode>(node))
		{
			//make sure its a valid value function modifier
			opString name = newnode->GetModifier()->GetValue();
			
			DialectCategory* category = FindParent<OPObjectNode>()->GetCategorySettings();
			
			FunctionModifierNode* modifier = category->FindValueFunctionModifier(name);
			//ask the modifier
			if(!modifier)
			{
				opError::InvalidFunctionModifierError(newnode);
			}
			else if(modifier->GetArguments()->GetCondition())
			{
				opString stringvalue;
				newnode->GetArguments()->PrintString(stringvalue);

				opString pattern = modifier->GetArguments()->GetCondition()->GetValue();
				pattern = pattern.Substring(1,pattern.Size()-1);

				//check regex pattern!
				if(!opregex::Match(stringvalue,pattern,modifier))
				{
					opError::InvalidValueError(newnode,modifier);
				}
			}
		}
		else if ( node->GetId() == T_MODIFIER )
		{
			//make sure its a valid function modifier
			opString name = node->GetStringValue();

			DialectCategory* category = FindParent<OPObjectNode>()->GetCategorySettings();

			//ask the modifier
			if(!category->FindFunctionModifier(name))
			{
				opError::InvalidFunctionModifierError(node);
			}
		}
		else if ( node->GetId() == T_ID )
		{
			opError::InvalidFunctionModifierError(node);
		}

		IncrementPosition();
	}
}

//
// StatementModifierBase
//

void StatementModifierBase::SetVisibilityMode(VisibilityMode mode)
{
	if(modifiers)
	{
		//if we have a visibility modifier, cache it for later
		if(TerminalNode* modifier = modifiers->FindModifier(T_PUBLIC))
			VisibilityModifier = modifier;
		else if(TerminalNode* modifier = modifiers->FindModifier(T_PROTECTED))
			VisibilityModifier = modifier;
		else if(TerminalNode* modifier = modifiers->FindModifier(T_PRIVATE))
			VisibilityModifier = modifier;
	}
	
	if(!VisibilityModifier)
	{
		Token token;
		opString value;
		
		//if not, create one using the mode.
		if(mode == vismode_public)
		{
			token = T_PUBLIC;
			value = "public";
		}
		else if(mode == vismode_protected)
		{
			token = T_PROTECTED;
			value = "protected";
		}
		else if(mode == vismode_private)
		{
			token = T_PRIVATE;
			value = "private";
		}
		else if(mode == vismode_default)
		{
			token = T_DEFAULT;
			value = "default";
		}
		
		VisibilityModifier = AddBasicModifier(value,token);
	}
	
	//now create the automatic visibility_mode modifier
	ValuedModifierNode* visibility_mode = AddValueModifier("visibility_mode");
	
	//set the argument value
	stacked<TerminalNode> cloned = VisibilityModifier->Clone();
	visibility_mode->GetArguments()->AppendNode(cloned);
}

TerminalNode* ModifierSupportBase::AddBasicModifier(const opString& modifiername, Token token)
{
	CreateModifiersNode();
	
	TerminalNode* node = automodifiers->AppendTerminalNode(modifiername,token);
	
	node->CopyBasics(automodifiers);
	
	return node;
}

ValuedModifierNode* ModifierSupportBase::AddValueModifier(const opString& modifiername)
{
	CreateModifiersNode();
	
	//create the value modifier
	stacked<ValuedModifierNode> valuemod = NEWNODE(ValuedModifierNode);
	
	//create the modifier name
	TerminalNode* mod = valuemod->AppendTerminalNode(modifiername,T_MODIFIER);
	mod->CopyBasics(automodifiers);
	
	valuemod->SetModifier(mod);
	
	//create the argument
	stacked<ValuedModifierArgumentNode> valuearg = NEWNODE(ValuedModifierArgumentNode);
	
	ValuedModifierArgumentNode* valuearguments = *valuearg;
	valuearguments->CopyBasics(automodifiers);
	
	valuemod->SetArguments(*valuearg);
	valuemod->AppendNode(valuearg);
	
	ValuedModifierNode* valuemodifier = *valuemod;
	valuemodifier->CopyBasics(automodifiers);
	
	automodifiers->AppendNode(valuemod);
	
	return valuemodifier;
}

void FuncPointerStatementNode::RegisterAutoModifiers()
{
	/*=== function pointer ===*/
	
	ModifierDelegate data_statement(this,&FuncPointerStatementNode::ModifierDataStatement);
	RegisterModifier("data_statement",data_statement);
	
	ModifierDelegate function_pointer(this,&FuncPointerStatementNode::ModifierFunctionPointer);
	RegisterModifier("function_pointer",function_pointer);	
	
	//????
// 	ModifierDelegate function_pointer_signature(this,&FuncPointerStatementNode::ModifierFunctionPointerSignature);
// 	RegisterModifier("function_pointer_signature",function_pointer_signature);	
}

void StatementModifierBase::GenerateStatementModifiers()
{
	// Register member_name	
	ModifierDelegate member_name(this,&StatementModifierBase::ModifierMemberName);
	RegisterModifier("member_name",member_name);
	
	// All modifiers
	ModifierDelegate all_modifiers(this,&StatementModifierBase::ModifierAllModifiers);
	RegisterModifier("all_modifiers",all_modifiers);
}

void DataStatementNode::RegisterAutoModifiers()
{	
	GenerateStatementModifiers();
	
	//Register data_statement
	ModifierDelegate data_statement(this,&DataStatementNode::ModifierDataStatement);
	RegisterModifier("data_statement",data_statement);
	
	//Register data_type
	ModifierDelegate data_type(this,&DataStatementNode::ModifierDataType);
	RegisterModifier("data_type",data_type);
	
	if(Declaration->GetBits())
	{
		ModifierDelegate data_bits(this,&DataStatementNode::ModifierDataBits);
		RegisterModifier("data_bits",data_bits);
	}
	
	if(Initialization)
	{
		ModifierDelegate data_initialized(this,&DataStatementNode::ModifierDataInitialized);
		RegisterModifier("data_initialized",data_initialized);
	}
	
	if(node_cast<ReferenceNode>(GetType())
	|| node_cast<PointerNode>(GetType()))
	{
		ModifierDelegate data_base_type(this,&DataStatementNode::ModifierDataBaseType);
		RegisterModifier("data_base_type",data_base_type);
	}
	
	//type related
	if(ReferenceNode* node = node_cast<ReferenceNode>(GetType()))
	{
		ModifierDelegate data_reference(this,&DataStatementNode::ModifierDataReference);	
		RegisterModifier("data_reference",data_reference);
	}
	if(PointerNode* node = node_cast<PointerNode>(GetType()))
	{
		ModifierDelegate data_pointer(this,&DataStatementNode::ModifierDataPointer);	
		RegisterModifier("data_pointer",data_pointer);
		
		ModifierDelegate data_pointer_level(this,&DataStatementNode::ModifierDataPointerLevel);
		RegisterModifier("data_pointer_level",data_pointer_level);
	}
	if(ScopeNode* node = node_cast<ScopeNode>(GetType()))
	{
		ModifierDelegate data_qualified(this,&DataStatementNode::ModifierDataQualified);
		RegisterModifier("data_qualified",data_qualified);
	}
	if(TemplateTypeNode* node = node_cast<TemplateTypeNode>(GetType()))
	{
		ModifierDelegate data_template(this,&DataStatementNode::ModifierDataTemplate);
		RegisterModifier("data_template",data_template);
	}
	if(TerminalNode* node = node_cast<TerminalNode>(GetType()))
	{
		ModifierDelegate data_basic(this,&DataStatementNode::ModifierDataBasic);
		RegisterModifier("data_basic",data_basic);
	}
	
	ModifierDelegate data_full_type(this,&DataStatementNode::ModifierDataFullType);
	RegisterModifier("data_full_type",data_full_type);
	
	//name related
	if(ArrayNode* node = node_cast<ArrayNode>(GetName()))
	{
		ModifierDelegate data_array(this,&DataStatementNode::ModifierDataArray);
		RegisterModifier("data_array",data_array);
		
		ModifierDelegate data_array_brackets(this,&DataStatementNode::ModifierDataArrayBrackets);
		RegisterModifier("data_array_brackets",data_array_brackets);
		
		ModifierDelegate data_array_commas(this,&DataStatementNode::ModifierDataArrayCommas);
		RegisterModifier("data_array_commas",data_array_commas);
	}
	
	
	
}

// Register common function modifiers.
void FunctionStatementBase::RegisterFunctionModifiers()
{
	GenerateStatementModifiers();

	/*=== functions - common ===*/

	ModifierDelegate function_statement(this,&FunctionStatementBase::ModifierFunctionStatement);
	RegisterModifier("function_statement",function_statement);	
	
	if(GetFunction()->GetAssignment())
	{
		ModifierDelegate function_pure(this,&FunctionStatementBase::ModifierFunctionPure);
		RegisterModifier("function_pure",function_pure);	
	}

	if(GetFunction()->IsConst())
	{
		ModifierDelegate function_const(this,&FunctionStatementBase::ModifierFunctionConst);
		RegisterModifier("function_const",function_const);
	}
	
	ModifierDelegate function_arguments(this,&FunctionStatementBase::ModifierFunctionArguments);
	RegisterModifier("function_arguments",function_arguments);
	
	ModifierDelegate function_return_type(this,&FunctionStatementBase::ModifierFunctionReturnType);
	RegisterModifier("function_return_type",function_return_type);
	
	ModifierDelegate function_arguments_count(this,&FunctionStatementBase::ModifierFunctionArgumentCount);
	RegisterModifier("function_arguments_count",function_arguments_count);
	
	ModifierDelegate function_signature(this,&FunctionStatementBase::ModifierFunctionSignature);
	RegisterModifier("function_signature",function_signature);
	
	ModifierDelegate function_arguments_signature(this,&FunctionStatementBase::ModifierFunctionArgumentSignature);
	RegisterModifier("function_arguments_signature",function_arguments_signature);
	
// 	ModifierDelegate function_pointer_signature(this,&FunctionStatementBase::ModifierFunctionPointerSignature);
// 	RegisterModifier("function_pointer_signature",function_pointer_signature);
	
// 	ModifierDelegate function_signature_defaults(this,&FunctionStatementBase::ModifierFunctionSignatureDefaults);
// 	RegisterModifier("function_signature_defaults",function_signature_defaults);
	
	ModifierDelegate function_arguments_defaults(this,&FunctionStatementBase::ModifierFunctionArgumentsDefaults);
	RegisterModifier("function_arguments_defaults",function_arguments_defaults);
	
	//operator modifiers
	if(OperatorNode* op = node_cast<OperatorNode>(GetFunction()->GetName()))
	{
		//is an operator
		RegisterBasicModifier("function_operator");
		
		//whats its type
		ModifierDelegate function_operator_type(this,&FunctionStatementBase::ModifierFunctionOperatorType);
		RegisterModifier("function_operator_type",function_operator_type);
		
	}

}

void FuncPrototypeStatementNode::RegisterAutoModifiers()
{
	RegisterFunctionModifiers();

	//prototype specific registrations

	ModifierDelegate function_prototype(this,&FuncPrototypeStatementNode::ModifierFunctionPrototype);
	RegisterModifier("function_prototype",function_prototype);
}

// Register modifiers for function definitions.
void FunctionDefinitionStatementNode::RegisterAutoModifiers()
{
	RegisterFunctionModifiers();

	//definition specific registrations

	RegisterBasicModifier("function_definition");

	ModifierDelegate function_body(this,&FunctionDefinitionStatementNode::ModifierFunctionBody);
	RegisterModifier("function_body",function_body);
}

void ConstructorStatementBase::RegisterConstructorModifiers()
{
	RegisterFunctionModifiers();

	RegisterBasicModifier("function_constructor");
}

void ConstructorDefinitionStatementNode::RegisterAutoModifiers()
{
	RegisterConstructorModifiers();


	RegisterBasicModifier("function_definition");

	//TODO: add normal definition modifiers
	ModifierDelegate function_body(this,&ConstructorDefinitionStatementNode::ModifierFunctionBody);
	RegisterModifier("function_body",function_body);
}

void ConstructorPrototypeStatementNode::RegisterAutoModifiers()
{
	RegisterConstructorModifiers();
	
	//TODO: add normal prototype modifiers
	RegisterBasicModifier("function_prototype");
}

void DestructorStatementBase::RegisterDestructorModifiers()
{
	RegisterFunctionModifiers();

	ModifierDelegate function_destructor(this,&DestructorStatementBase::ModifierFunctionDestructor);
	RegisterModifier("function_destructor",function_destructor);
}

void DestructorDefinitionStatementNode::RegisterAutoModifiers()
{
	RegisterDestructorModifiers();

	//TODO: add normal definition modifiers
	RegisterBasicModifier("function_definition");

	ModifierDelegate function_body(this,&DestructorDefinitionStatementNode::ModifierFunctionBody);
	RegisterModifier("function_body",function_body);

}


void DestructorPrototypeStatementNode::RegisterAutoModifiers()
{
	RegisterDestructorModifiers();
	
	//TODO: add normal prototype modifiers
	RegisterBasicModifier("function_prototype");
}


// Register modifiers for opTypes (opObject, opEnum)
void OPTypeBase::RegisterAutoModifiers()
{
	if(IsInside(G_OPOBJECT))
	{
		ModifierDelegate inner_type(this,&OPTypeBase::ModifierInnerType);
		RegisterModifier("inner_type",inner_type);
	}
	
	ModifierDelegate scope(this,&OPTypeBase::ModifierScope);
	RegisterModifier("scope",scope);
	
	ModifierDelegate alt_scope(this,&OPTypeBase::ModifierAltScope);
	RegisterModifier("alt_scope",alt_scope);
	
	ModifierDelegate namespace_start(this,&OPTypeBase::ModifierNamespaceStart);
	RegisterModifier("namespace_start",namespace_start);
	
	ModifierDelegate namespace_end(this,&OPTypeBase::ModifierNamespaceEnd);
	RegisterModifier("namespace_end",namespace_end);
	
	ModifierDelegate alt_namespace_start(this,&OPTypeBase::ModifierAltNamespaceStart);
	RegisterModifier("alt_namespace_start",alt_namespace_start);
	
	ModifierDelegate alt_namespace_end(this,&OPTypeBase::ModifierAltNamespaceEnd);
	RegisterModifier("alt_namespace_end",alt_namespace_end);
}

///==========================================
/// Automatic Modifier Methods
///==========================================

/*=== class (instance) ===*/

opNode* OPObjectNode::ModifierClassName(const opString& name)
{
	ValuedModifierNode* class_name = AddValueModifier(name);

	stacked<opNode> cloned = GetName()->CloneGeneric();
	class_name->GetArguments()->AppendNode(cloned);

	return class_name;
}

opNode* OPObjectNode::ModifierAltClassName(const opString& name)
{
	ValuedModifierNode* alt_class_name = AddValueModifier(name);
	opNode* node = alt_class_name->GetArguments();

	node->AppendTerminalNode("_" + GetName()->GetValue(),T_ID);

	return alt_class_name;
}


opNode* OPObjectNode::ModifierParentName(const opString& name)
{
	ValuedModifierNode*       parent_name = AddValueModifier(name);
	OPObjectInheritanceNode* inheritance = GetInheritance();

	if (inheritance)
		inheritance->CloneParentNames(parent_name->GetArguments());

	return parent_name;
}

/*=== category ===*/

opNode* OPObjectNode::ModifierCategoryStatement(const opString& name)
{
	return AddBasicModifier(name, T_MODIFIER);
}

opNode* OPObjectNode::ModifierCategoryAbstract(const opString& name)
{
	return AddBasicModifier(name,T_MODIFIER);
}

opNode* OPObjectNode::ModifierCategoryBaseclass(const opString& name)
{
	return AddBasicModifier(name,T_MODIFIER);
}

opNode* OPObjectNode::ModifierCategorySingleInheritance(const opString& name)
{
	return AddBasicModifier(name,T_MODIFIER);
}

opNode* OPObjectNode::ModifierCategoryMultipleInheritance(const opString& name)
{
	return AddBasicModifier(name,T_MODIFIER);
}

opNode* OPObjectNode::ModifierCategoryHasConstructor(const opString& name)
{
	return AddBasicModifier(name,T_MODIFIER);
}

opNode* OPObjectNode::ModifierCategoryHasDestructor(const opString& name)
{
	return AddBasicModifier(name,T_MODIFIER);
}

opNode* OPObjectNode::ModifierCategoryHasSimpleConstructor(const opString& name)
{
	return AddBasicModifier(name,T_MODIFIER);
}

opNode* OPObjectNode::ModifierCategoryHasComplexConstructor(const opString& name)
{
	return AddBasicModifier(name,T_MODIFIER);
}
// 
// opNode* OPObjectNode::ModifierCategoryName(const opString& name)
// {
// 	//????
// 	return NULL;
// }

/*=== functions - common ===*/

opNode* FunctionStatementBase::ModifierFunctionStatement(const opString& name)
{
	return AddBasicModifier(name,T_MODIFIER);
}

opNode* FunctionStatementBase::ModifierFunctionPure(const opString& name)
{
	return AddBasicModifier(name,T_MODIFIER);
}

opNode* FunctionStatementBase::ModifierFunctionConst(const opString& name)
{
	return AddBasicModifier(name,T_MODIFIER);
}
//NOTE: BUG: functionarguments does not remember const
// arguments, no defaults
opNode* FunctionStatementBase::ModifierFunctionArguments(const opString& name)
{
	ValuedModifierNode* argmod = AddValueModifier(name);
	FunctionArgumentListNode* arglist = GetFunction()->GetArguments();

	int numargs = arglist->GetNumArguments();
	for(int i = 0; i < numargs; i++)
	{
		stacked<FunctionArgumentDefinitionNode> clonearg = arglist->GetArgument(i)->GetDefinition()->Clone();
		argmod->GetArguments()->AppendNode(clonearg);

		if(i < numargs - 1)
			argmod->GetArguments()->AppendTerminalNode(',',T_COMMA);
	}

	return argmod;
}

opNode* FunctionStatementBase::ModifierFunctionReturnType(const opString& name)
{
	ValuedModifierNode* retmod = AddValueModifier(name);
	if(GetReturnType())
	{
		stacked<opNode> cloneret = GetReturnType()->CloneGeneric();
		retmod->GetArguments()->AppendNode(cloneret);
	}
	return retmod;
}

opNode* FunctionStatementBase::ModifierFunctionArgumentCount(const opString& name)
{
	//function_argument_count()
	ValuedModifierNode* countmod = AddValueModifier(name);
	int numarguments = GetFunction()->GetArguments()->GetNumArguments();
	countmod->GetArguments()->AppendTerminalNode(numarguments,T_MODIFIER);
	return countmod;
}

// function signature, no defaults
opNode* FunctionStatementBase::ModifierFunctionSignature(const opString& name)
{
	ValuedModifierNode* modifier = AddValueModifier(name);
	opNode*			   arguments = modifier->GetArguments();
	
	// return type
	if(GetReturnType())
	{
		stacked<opNode> cloned = GetReturnType()->CloneGeneric();
		arguments->AppendNode(cloned);
	}
	
	// now we need a space (not really)
	arguments->AppendTerminalNode(" ",T_WHITESPACE);
	
	// create the parentheses
	stacked<ParenBlockNode> parens = NEWNODE(ParenBlockNode());
	parens->CopyBasics(GetFunction()->GetArguments());
	
	// now loop over all the arguments...
	int numargs = GetFunction()->GetArguments()->GetNumArguments();
	for(int i = 0; i < numargs; i++)
	{
		FunctionArgumentNode* arg = GetFunction()->GetArguments()->GetArgument(i);
		FunctionArgumentDefinitionNode* def = arg->GetDefinition();
		
		//NOTE: what about arrays? :P~
		stacked<opNode> type = def->GetInnerStatement()->GetType()->CloneGeneric();
		
		parens->AppendNode(type);
		
		if(i + 1 < numargs)
			parens->AppendTerminalNode(",",T_COMMA);
	}
	
	arguments->AppendNode(parens);
	
	// ignore const in this modifier...
	
	return modifier;
}

opNode* FunctionStatementBase::ModifierFunctionArgumentSignature(const opString& name)
{
	ValuedModifierNode* modifier = AddValueModifier(name);
	opNode*			   arguments = modifier->GetArguments();

	// create the parentheses
	stacked<ParenBlockNode> parens = NEWNODE(ParenBlockNode());
	parens->CopyBasics(GetFunction()->GetArguments());

	// now loop over all the arguments...
	int numargs = GetFunction()->GetArguments()->GetNumArguments();
	for(int i = 0; i < numargs; i++)
	{
		FunctionArgumentNode* arg = GetFunction()->GetArguments()->GetArgument(i);
		FunctionArgumentDefinitionNode* def = arg->GetDefinition();
		
		//NOTE: what about arrays? :P~
		stacked<opNode> type = def->GetInnerStatement()->GetType()->CloneGeneric();
		
		parens->AppendNode(type);
		
		if(i + 1 < numargs)
			parens->AppendTerminalNode(",",T_COMMA);
	}
	
	arguments->AppendNode(parens);
	
	return modifier;
}

// opNode* FunctionStatementBase::ModifierFunctionPointerSignature(const opString& name)
// {
// 	//note: can utilize the function arguments for this.
// 	// first, clone the function arguments
// 	// second, tell it to become a signature (by deleting nodes)
// 	// needs a const or something.... I guess it will work
// 	
// 	// may not be useful...
// 
// 	//TODO: do this!
// 	return NULL;
// }

// returntype (type arg, type arg = default)
// opNode* FunctionStatementBase::ModifierFunctionSignatureDefaults(const opString& name)
// {
// 	ValuedModifierNode* modifier = AddValueModifier(name);
// 	opNode*			   arguments = modifier->GetArguments();
// 
// 	// return type
// 	arguments->AppendNode(GetReturnType()->CloneGeneric());
// 
// 	// now we need a space (not really)
// 	arguments->AppendTerminalNode(" ",T_WHITESPACE);
// 
// 	// create the parentheses
// 	stacked<ParenBlockNode> parens = NEWNODE(ParenBlockNode());
// 	parens->CopyBasics(GetFunction()->GetArguments());
// 
// 	// now loop over all the arguments...
// 	int numargs = GetFunction()->GetArguments()->GetNumArguments();
// 	for(int i = 0; i < numargs; i++)
// 	{
// 		FunctionArgumentNode* arg = GetFunction()->GetArguments()->GetArgument(i);
// 		FunctionArgumentDefinitionNode* def = arg->GetDefinition();
// 
// 		//NOTE: what about arrays? :P~
// 		stacked<opNode> type = def->GetInnerStatement()->GetType()->CloneGeneric();
// 
// 		parens->AppendNode(type);
// 
// 		if(arg->GetDefault())
// 		{
// 			parens->AppendTerminalNode("=",T_ASSIGN);
// 			
// 			stacked<opNode> argdefault = arg->GetDefault()->CloneGeneric();
// 			
// 			parens->CollapseNode(argdefault,parens->GetEnd());
// 		}
// 		
// 		if(i + 1 < numargs)
// 			parens->AppendTerminalNode(",",T_COMMA);
// 	}
// 	
// 	arguments->AppendNode(parens);
// 	
// 
// 	// first, clone the function arguments
// 	// second, tell it to become a signature (by deleting nodes)
// 	// needs a const or something.... I guess it will work
// 	//return type
// 	//function sig
// 
// 	//TODO: do this!
// 	return modifier;
// }

opNode* FunctionStatementBase::ModifierFunctionOperatorType(const opString& name)
{
	ValuedModifierNode* modifier = AddValueModifier(name);
	opNode*			   arguments = modifier->GetArguments();
	
	OperatorNode* node = node_cast<OperatorNode>(GetFunction()->GetName());
	stacked<OperatorNode> op = node->Clone();
	
	arguments->CollapseNode(op,arguments->GetEnd());
	
	return modifier;
}


//format: type arg, int name = default ...
opNode* FunctionStatementBase::ModifierFunctionArgumentsDefaults(const opString& name)
{
	ValuedModifierNode* modifier = AddValueModifier(name);
	opNode*			   arguments = modifier->GetArguments();

	// now loop over all the arguments...
	int numargs = GetFunction()->GetArguments()->GetNumArguments();
	for(int i = 0; i < numargs; i++)
	{
		FunctionArgumentNode* arg = GetFunction()->GetArguments()->GetArgument(i);
		FunctionArgumentDefinitionNode* def = arg->GetDefinition();
		
		//NOTE: what about arrays? :P~
		stacked<opNode> type = def->GetInnerStatement()->GetType()->CloneGeneric();
		
		arguments->AppendNode(type);
		
		if(arg->GetDefault())
		{
			arguments->AppendTerminalNode("=",T_ASSIGN);

			stacked<opNode> argdefault = arg->GetDefault()->CloneGeneric();

			arguments->CollapseNode(argdefault,arguments->GetEnd());
		}

		if(i + 1 < numargs)
			arguments->AppendTerminalNode(",",T_COMMA);
	}

	return modifier;
}

/*=== function prototypes ===*/

opNode* FuncPrototypeStatementNode::ModifierFunctionPrototype(const opString& name)
{
	return AddBasicModifier(name,T_MODIFIER);
}

/*=== function definitions ===*/

opNode* FunctionDefinitionStatementNode::ModifierFunctionDefinition(const opString& name)
{
	return AddBasicModifier(name,T_MODIFIER);
}

opNode* FunctionDefinitionStatementNode::ModifierFunctionBody(const opString& name)
{
	//function body (no braces)
	stacked<opNode> cloned = FunctionDefinition->GetBody()->CloneGeneric();
	
	ValuedModifierNode* function_body = AddValueModifier(name);
	function_body->GetArguments()->CollapseNode(cloned,function_body->GetArguments()->GetBegin());
	
	return function_body;
}

/*=== destructors ===*/

opNode* DestructorStatementBase::ModifierFunctionDestructor(const opString& name)
{
	return AddBasicModifier(name,T_MODIFIER);
}

opNode* DestructorDefinitionStatementNode::ModifierFunctionBody(const opString& name)
{
	//function body (no braces)
	stacked<opNode> cloned = DestructorDefinition->GetBody()->CloneGeneric();

	ValuedModifierNode* function_body = AddValueModifier(name);
	function_body->GetArguments()->CollapseNode(cloned,function_body->GetArguments()->GetBegin());

	return function_body;
}

/*=== constructors ===*/

opNode* ConstructorStatementBase::ModifierFunctionConstructor(const opString& name)
{
	return AddBasicModifier(name,T_MODIFIER);
}

opNode* ConstructorDefinitionStatementNode::ModifierFunctionBody(const opString& name)
{
	//function body (no braces)
	stacked<opNode> cloned = ConstructorDefinition->GetBody()->CloneGeneric();
	
	ValuedModifierNode* function_body = AddValueModifier(name);
	function_body->GetArguments()->CollapseNode(cloned,function_body->GetArguments()->GetBegin());
	
	return function_body;
}

/*=== function pointer ===*/

opNode* FuncPointerStatementNode::ModifierDataStatement(const opString& name)
{
	return AddBasicModifier(name,T_MODIFIER);
}

opNode* FuncPointerStatementNode::ModifierFunctionPointer(const opString& name)
{
	return AddBasicModifier(name,T_MODIFIER);
}

//TODO: add these back... if they make any sense
// opNode* FuncPointerStatementNode::ModifierFunctionPointerName(const opString& name)
// {
// 	ValuedModifierNode* function_pointer_name = AddValueModifier(name);
// 	function_pointer_name->GetArguments()->AppendNode(FunctionPointer->GetDecl()->GetName()->CloneGeneric());
// 	return function_pointer_name;
//}

// opNode* FuncPointerStatementNode::ModifierFunctionPointerSignature(const opString& name)
// {
// 	//theres no such thing... :/
// 	//TODO: do this!
// 	return NULL;
//}



opNode* DataStatementNode::ModifierDataType(const opString& name)
{
	//data_type
	ValuedModifierNode* data_type = AddValueModifier(name);
	if(HasModifier(T_CONST))
	{
		data_type->GetArguments()->AppendTerminalNode("const",T_CONST);	
		data_type->GetArguments()->AppendTerminalNode(" ",T_WHITESPACE);	
	}
	if(HasModifier(T_VOLATILE))
	{
		data_type->GetArguments()->AppendTerminalNode("volatile",T_VOLATILE);	
		data_type->GetArguments()->AppendTerminalNode(" ",T_WHITESPACE);	
	}

	stacked<opNode> cloned = GetDataType()->CloneGeneric();
	data_type->GetArguments()->AppendNode(cloned);
	
	return data_type;
}

opNode* DataStatementNode::ModifierDataStatement(const opString& name)
{
	return AddBasicModifier(name,T_MODIFIER);
}

opNode* DataStatementNode::ModifierDataBits(const opString& name)
{
	ValuedModifierNode* mod = AddValueModifier(name);
	mod->GetArguments()->AppendTerminalNode(Declaration->GetBits()->GetNumber()->GetValue(),T_NUMBER);
	return mod;
}

opNode* DataStatementNode::ModifierDataInitialized(const opString& name)
{
	ValuedModifierNode* mod = AddValueModifier(name);

	stacked<DataInitializationNode> init = Initialization->Clone();
	DataInitializationNode* cloned = *init;
	mod->GetArguments()->CollapseNode(init,mod->GetArguments()->GetBegin());
	return mod;
}

opNode* DataStatementNode::ModifierDataReference(const opString& name)
{
	//data_reference
	return AddBasicModifier(name,T_MODIFIER);
}

opNode* DataStatementNode::ModifierDataPointer(const opString& name)
{
	//data_pointer
	return AddBasicModifier(name,T_MODIFIER);
}

opNode* DataStatementNode::ModifierDataPointerLevel(const opString& name)
{
	PointerNode* node = node_cast<PointerNode>(GetType());

	ValuedModifierNode* pmod = AddValueModifier(name);
	pmod->GetArguments()->AppendTerminalNode(node->GetNumStars(),T_NUMBER);

	return pmod;
}

opNode* DataStatementNode::ModifierDataBaseType(const opString& name)
{
	opNode* type = GetDataType();
	
	if(ReferenceNode* node = node_cast<ReferenceNode>(type))
	{
		//data_base_type()
		ValuedModifierNode* mod = AddValueModifier(name);
		stacked<opNode> value = node->GetType()->CloneGeneric();
		mod->GetArguments()->AppendNode(value);
		return mod;
	}
	else if(PointerNode* node = node_cast<PointerNode>(type))
	{
		//data_base_type()
		ValuedModifierNode* mod = AddValueModifier(name);
		stacked<opNode> value = node->GetType()->CloneGeneric();
		mod->GetArguments()->AppendNode(value);
		return mod;
	}

	return NULL;
}

opNode* DataStatementNode::ModifierDataTemplate(const opString& name)
{
	//data_template
	return AddBasicModifier(name,T_MODIFIER);
}

opNode* DataStatementNode::ModifierDataQualified(const opString& name)
{
	//data_template
	return AddBasicModifier(name,T_MODIFIER);	
}

opNode* DataStatementNode::ModifierDataBasic(const opString& name)
{
	//data_basic
	return AddBasicModifier(name,T_MODIFIER);
}

opNode* DataStatementNode::ModifierDataFullType(const opString& name)
{
	//TODO: this needs to be fixed...

	ValuedModifierNode* data_full_type = AddValueModifier(name);
	if(HasModifier(T_CONST))
	{
		data_full_type->GetArguments()->AppendTerminalNode("const",T_CONST);	
		data_full_type->GetArguments()->AppendTerminalNode(" ",T_WHITESPACE);	
	}
	if(HasModifier(T_VOLATILE))
	{
		data_full_type->GetArguments()->AppendTerminalNode("volatile",T_VOLATILE);	
		data_full_type->GetArguments()->AppendTerminalNode(" ",T_WHITESPACE);	
	}

	stacked<opNode> cloneddatatype = GetDataType()->CloneGeneric();
	data_full_type->GetArguments()->AppendNode(cloneddatatype);
	

	if(	ArrayNode* node = node_cast<ArrayNode>(GetName()) )
	{
		int numbrackets = node->GetNumBrackets();
		for(int i = 0; i < numbrackets; i++)
		{
			BracketBlockNode* bracket = node->GetBracket(i);	
			
			stacked<BracketBlockNode> clonedbracket = bracket->Clone();
			data_full_type->GetArguments()->AppendNode(clonedbracket);
		}
	}

	return data_full_type;
}

opNode* DataStatementNode::ModifierDataArray(const opString& name)
{
	//data_array
	return AddBasicModifier(name,T_MODIFIER);	
}

opNode* DataStatementNode::ModifierDataArrayBrackets(const opString& name)
{
	ArrayNode* node = node_cast<ArrayNode>(GetName());

	//data_array_brackets()
	ValuedModifierNode* mod = AddValueModifier("data_array_brackets");

	int numbrackets = node->GetNumBrackets();
	for(int i = 0; i < numbrackets; i++)
	{
		BracketBlockNode* bracket = node->GetBracket(i);	
		stacked<BracketBlockNode> cloned = bracket->Clone();
		mod->GetArguments()->AppendNode(cloned);
	}

	return mod;
}

opNode* DataStatementNode::ModifierDataArrayCommas(const opString& name)
{
	ArrayNode* node = node_cast<ArrayNode>(GetName());
	
	//data_array_commas()
	ValuedModifierNode* cmod = AddValueModifier("data_array_commas");
	
	//this needs to iterate all the brackets,
	//extract them, and collapse them, adding commas
	int numbrackets = node->GetNumBrackets();
	for(int i = 0; i < numbrackets; i++)
	{
		BracketBlockNode* bracket = node->GetBracket(i);
		
		stacked<BracketBlockNode> clone = bracket->Clone();
		
		iterator end = cmod->GetArguments()->GetEnd();
		
		cmod->GetArguments()->CollapseNode(clone,end);
		
		if(i < numbrackets - 1)
			cmod->GetArguments()->AppendTerminalNode(',',T_COMMA);
	}
	
	return cmod;
}

opNode* StatementModifierBase::ModifierMemberName(const opString& name)
{
	ValuedModifierNode* member_name = AddValueModifier(name);
	stacked<opNode> cloned = GetMemberName()->CloneGeneric();
	member_name->GetArguments()->AppendNode(cloned);

	return member_name;
}

opNode* StatementModifierBase::ModifierAllModifiers(const opString& name)
{
	ValuedModifierNode* all_modifiers = AddValueModifier(name);
	opNode* arguments = all_modifiers->GetArguments();

	if(modifiers)
	{
		//loop through all modifiers
		//add cloned modifiers to arguments
		opNode::iterator it  = modifiers->GetBegin();
		opNode::iterator end = modifiers->GetEnd();

		while(it != end)
		{
			stacked<opNode> cloned = it->CloneGeneric();
			arguments->AppendNode(cloned);

			++it;

			if(it != end)
				arguments->AppendTerminalNode(" ",T_WHITESPACE);
		}
	}
	
	return all_modifiers;
}

//
// OPTypeBase Modifier Generator
//

opNode* OPTypeBase::ModifierInnerType(const opString& name)
{
	return AddBasicModifier(name,T_MODIFIER);
}

opNode* OPTypeBase::ModifierScope(const opString& name)
{
	//build the scope argument
	ValuedModifierNode* modifier = AddValueModifier(name);
	opNode* node = modifier->GetArguments();

	opNode* current = GetParent();
	while(current)
	{
		if(OPObjectNode* objectnode = node_cast<OPObjectNode>(current))
		{
			//extract string
			opString scopename;
			objectnode->GetName()->PrintString(scopename);

			node->PrependTerminalNode(scopename,T_ID);
			node->PrependTerminalNode("::",T_SCOPE_RESOLUTION);
		}
		else if(NamespaceNode* namespacenode = node_cast<NamespaceNode>(current))
		{
			//extract string
			opString scopename;
			namespacenode->GetName()->PrintString(scopename);

			node->PrependTerminalNode(scopename,T_ID);
			node->PrependTerminalNode("::",T_SCOPE_RESOLUTION);
		}

		current = current->GetParent();
	}

	return modifier;
}

opNode* OPTypeBase::ModifierAltScope(const opString& name)
{
	//build the alt scope argument
	ValuedModifierNode* modifier = AddValueModifier(name);
	opNode* node = modifier->GetArguments();

	opNode* current = GetParent();
	while(current)
	{
		if(OPObjectNode* objectnode = node_cast<OPObjectNode>(current))
		{
			//extract string
			opString scopename;
			objectnode->GetName()->PrintString(scopename);

			node->PrependTerminalNode("_" + scopename,T_ID);
			node->PrependTerminalNode("::",T_SCOPE_RESOLUTION);
		}
		else if(NamespaceNode* namespacenode = node_cast<NamespaceNode>(current))
		{
			//extract string
			opString scopename;
			namespacenode->GetName()->PrintString(scopename);

			node->PrependTerminalNode("_" + scopename,T_ID);
			node->PrependTerminalNode("::",T_SCOPE_RESOLUTION);
		}

		current = current->GetParent();
	}

	return modifier;
}

opNode* OPTypeBase::ModifierNamespaceStart(const opString& name)
{

	//build the namespace start argument
	ValuedModifierNode* modifier = AddValueModifier(name);
	opNode* node = modifier->GetArguments();

	NamespaceNode* current = FindParent<NamespaceNode>();

	while(current)
	{
		// add backwards...
		// "namespace name { " -> " { name namespace"
		node->PrependTerminalNode(" ",T_WHITESPACE);
		node->PrependTerminalNode("{",T_LEFT_BRACE);
		node->PrependTerminalNode(" ",T_WHITESPACE);
		node->PrependTerminalNode(current->GetName()->GetValue(),T_ID);
		node->PrependTerminalNode(" ",T_WHITESPACE);
		node->PrependTerminalNode("namespace",T_NAMESPACE);

		current = current->FindParent<NamespaceNode>();
	}

	return modifier;
}

opNode* OPTypeBase::ModifierNamespaceEnd(const opString& name)
{
	//build the namespace start argument
	ValuedModifierNode* modifier = AddValueModifier(name);
	opNode* node = modifier->GetArguments();

	NamespaceNode* current = FindParent<NamespaceNode>();

	while(current)
	{
		node->AppendTerminalNode("}",T_RIGHT_BRACE);
		node->AppendTerminalNode(" ",T_WHITESPACE);

		current = current->FindParent<NamespaceNode>();
	}

	return modifier;
}

opNode* OPTypeBase::ModifierAltNamespaceStart(const opString& name)
{
	//build the alt namespace start argument
	ValuedModifierNode* modifier = AddValueModifier(name);
	opNode* node = modifier->GetArguments();

	opNode* current = GetParent();

	while(current)
	{
		// add backwards...
		// "namespace _name { " -> " { _name namespace"
		if(NamespaceNode* c = node_cast<NamespaceNode>(current))
		{
			node->PrependTerminalNode(" ",T_WHITESPACE);
			node->PrependTerminalNode("{",T_LEFT_BRACE);
			node->PrependTerminalNode(" ",T_WHITESPACE);
			node->PrependTerminalNode("_" + c->GetName()->GetValue(),T_ID);
			node->PrependTerminalNode(" ",T_WHITESPACE);
			node->PrependTerminalNode("namespace",T_NAMESPACE);
		}
		else if(OPObjectNode* o = node_cast<OPObjectNode>(current))
		{
			node->PrependTerminalNode(" ",T_WHITESPACE);
			node->PrependTerminalNode("{",T_LEFT_BRACE);
			node->PrependTerminalNode(" ",T_WHITESPACE);
			node->PrependTerminalNode("_" + o->GetName()->GetValue(),T_ID);
			node->PrependTerminalNode(" ",T_WHITESPACE);
			node->PrependTerminalNode("namespace",T_NAMESPACE);
		}

		current = current->GetParent();
	}

	return modifier;
}

opNode* OPTypeBase::ModifierAltNamespaceEnd(const opString& name)
{
	//build the alt namespace end argument
	ValuedModifierNode* modifier = AddValueModifier(name);
	opNode* node = modifier->GetArguments();
	
	opNode* current = GetParent();
	
	while(current)
	{
		// add backwards...
		// "namespace _name { " -> " { _name namespace"
		if(node_cast<NamespaceNode>(current)
			|| node_cast<OPObjectNode>(current))
		{
			node->AppendTerminalNode("}",T_RIGHT_BRACE);
			node->AppendTerminalNode(" ",T_WHITESPACE);
		}

		current = current->GetParent();
	}

	return modifier;
}







