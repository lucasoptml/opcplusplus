///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: PostParse.cpp
/// Date: 05/27/2007
///
/// Description:
///
/// PostParse Related Source File
///****************************************************************

bool OPFileNode::PostParse()
{
	POSTPARSE_START;
	{
		ConditionalFinder findconditions(this);
	}
	POSTPARSE_END;
}

bool OPObjectNode::PostParse()
{
	//TODO: need to iterate over all statements in the body
	//we need to keep track of the visibility labels,
	//and notify the statements about their visibility
	// - these statements either find a visibility in their modifiers already
	//   or else they create a visibility modifier and cache the pointer to it.
	POSTPARSE_START;
	{
		ProcessVisibility();

		ProcessStatements();

		if(!ProcessDisallows())
			return false;

		// do name checking
		{
			NameCheckingWalker walker(this);
		}
	}
	POSTPARSE_END;
}

bool OPEnumNode::PostParse()
{
	POSTPARSE_START;
	{
		RegisterAutoModifiers();
	}
	POSTPARSE_END;
}

void OPObjectNode::ProcessVisibility()
{
	iterator end = Body->GetEnd();

	VisibilityMode mode = vismode_default;

	for(iterator it = Body->GetBegin(); it != end; ++it)
	{
		StatementBase* node = (*it)->ToStatementBase();

		if(!node)
			continue;

		if(VisibilityStatementNode* statement = node->ToVisibilityStatementNode())
		{
			mode = statement->GetVisibilityMode();
		}
		else
		{
			node->SetVisibilityMode(mode);
		}
	}
}

void OPObjectNode::RegisterAutoModifiers()
{
	Super::RegisterAutoModifiers();

	/*=== class (instance) ===*/
	
	ModifierDelegate class_name(this,&OPObjectNode::ModifierClassName);
	RegisterModifier("class_name",class_name);	
	
	ModifierDelegate alt_class_name(this,&OPObjectNode::ModifierAltClassName);
	RegisterModifier("alt_class_name",alt_class_name);	
	
	ModifierDelegate parent_name(this,&OPObjectNode::ModifierParentName);
	RegisterModifier("parent_name",parent_name);
	
	/*=== category ===*/
	
	ModifierDelegate category_statement(this,&OPObjectNode::ModifierCategoryStatement);
	RegisterModifier("category_statement",category_statement);
	
	//if we have any pure functions, add category_abstract
	if (HasPureFunctions())
	{
		ModifierDelegate category_abstract(this,&OPObjectNode::ModifierCategoryAbstract);
		RegisterModifier("category_abstract",category_abstract);
	}
	
	if (!GetInheritance())
	{
		ModifierDelegate category_baseclass(this,&OPObjectNode::ModifierCategoryBaseclass);
		RegisterModifier("category_baseclass",category_baseclass);
	}
	
	if (GetInheritance())
	{
		if (GetInheritance()->GetParents().Size() == 1)
		{
			ModifierDelegate category_single_inheritance(this,&OPObjectNode::ModifierCategorySingleInheritance);
			RegisterModifier("category_single_inheritance",category_single_inheritance);
		}
		
		ModifierDelegate category_multiple_inheritance(this,&OPObjectNode::ModifierCategoryMultipleInheritance);
		RegisterModifier("category_multiple_inheritance",category_multiple_inheritance);
	}
	
	if (HasConstructor())
	{
		ModifierDelegate category_has_constructor(this,&OPObjectNode::ModifierCategoryHasConstructor);
		RegisterModifier("category_has_constructor",category_has_constructor);
	}
	
	if (HasDestructor())
	{
		ModifierDelegate category_has_destructor(this,&OPObjectNode::ModifierCategoryHasDestructor);
		RegisterModifier("category_has_destructor",category_has_destructor);
	}
	
	if (HasSimpleConstructor())
	{
		ModifierDelegate category_has_simple_constructor(this,&OPObjectNode::ModifierCategoryHasSimpleConstructor);
		RegisterModifier("category_has_simple_constructor",category_has_simple_constructor);
	}
	
	if (HasComplexConstructor())
	{
		ModifierDelegate category_has_complex_constructor(this,&OPObjectNode::ModifierCategoryHasComplexConstructor);
		RegisterModifier("category_has_complex_constructor",category_has_complex_constructor);
	}
}

void OPObjectNode::ProcessStatements()
{
	RegisterAutoModifiers();
		
	iterator end = Body->GetEnd();
	
	bool bresult = true;

	for(iterator it = Body->GetBegin(); it != end; ++it)
	{
		StatementBase* node = (*it)->ToStatementBase();
		
		if(!node)
			continue;
		
		bresult = node->ProcessModifiers() ? bresult : false;
	}

	//if we errored, throw?
	if(!bresult)
		opException::ThrowException();
}

///==========================================
/// Process Object Disallows
///==========================================

bool OPObjectNode::ProcessObjectDisallows()
{
	DialectCategory* CategorySettings = GetCategorySettings();

	DISALLOW_START;
	{
		//iterate over all disallows in the category settings
		int numdisallows = CategorySettings->GetNumDisallows();
		for(int i = 0; i < numdisallows; i++)
		{
			DialectDisallow* disallow = CategorySettings->GetDisallow(i);

			if(disallow->IsMapped(this))
			{
				opError::DisallowError(disallow->GetNode(),this);
			}
		}		
	}
	DISALLOW_END;

}

///==========================================
/// Process Disallows
///==========================================

bool OPObjectNode::ProcessDisallows()
{
	bool bErrored = false;
	
	DialectCategory* CategorySettings = GetCategorySettings();
	
	if(!ProcessObjectDisallows())
		return false;
	
	//check all statements
	iterator end = Body->GetEnd();
	
	for(iterator it = Body->GetBegin(); it != end; ++it)
	{
		StatementBase* node = (*it)->ToStatementBase();

		if(!node)
			continue;

		if(!node->ProcessDisallows(CategorySettings))
			bErrored = true;
	}
	
	return !bErrored;
}

bool FunctionStatementBase::ProcessDisallows(DialectCategory* CategorySettings)
{
	DISALLOW_START;
	{
		//iterate over all disallows in the category settings
		int numdisallows = CategorySettings->GetNumDisallows();
		for(int i = 0; i < numdisallows; i++)
		{
			DialectDisallow* disallow = CategorySettings->GetDisallow(i);

			if(disallow->IsMapped(this))
			{
				opError::DisallowError(disallow->GetNode(),this);
			}
		}
	}
	DISALLOW_END;
}

bool DataStatementBase::ProcessDisallows(DialectCategory* CategorySettings)
{
	DISALLOW_START;
	{
		//iterate over all disallows in the category settings
		int numdisallows = CategorySettings->GetNumDisallows();
		for(int i = 0; i < numdisallows; i++)
		{
			DialectDisallow* disallow = CategorySettings->GetDisallow(i);

			if(disallow->IsMapped(this))
			{
				opError::DisallowError(disallow->GetNode(),this);
			}
		}
	}
	DISALLOW_END;
}


bool OPObjectNode::HasPureFunctions()
{
	//TODO: need to loop over all statements...
	opNode::iterator it = Body->GetBegin();
	opNode::iterator end = Body->GetEnd();

	while(it != end)
	{
		//convert to function
		//does the function have 
		opNode* node = *it;
		if(FunctionStatementBase* function = node->ToFunctionStatementBase())
		{
			if(function->IsPureFunction())
			{
				return true;
			}
		}
		
		++it;
	}

	return false;
}

bool OPObjectNode::HasConstructor()
{
	opNode::iterator it = Body->GetBegin();
	opNode::iterator end = Body->GetEnd();

	while(it != end)
	{
		//convert to function
		//does the function have 
		opNode* node = *it;
		if(ConstructorStatementBase* statement = node->ToConstructorStatementBase())
		{
			return true;
		}
		
		++it;
	}

	return false;
}

bool OPObjectNode::HasDestructor()
{
	opNode::iterator it = Body->GetBegin();
	opNode::iterator end = Body->GetEnd();
	
	while(it != end)
	{
		//convert to function
		//does the function have 
		opNode* node = *it;
		if(DestructorStatementBase* statement = node->ToDestructorStatementBase())
		{
			return true;
		}
		
		++it;
	}
	
	return false;
}

bool OPObjectNode::HasSimpleConstructor()
{
	opNode::iterator it = Body->GetBegin();
	opNode::iterator end = Body->GetEnd();

	while(it != end)
	{
		//convert to function
		//does the function have 
		opNode* node = *it;
		if(ConstructorStatementBase* statement = node->ToConstructorStatementBase())
		{
			if(ConstructorNode* ctor = statement->GetConstructor())
			{
				if(ctor->GetFunction()->GetArguments()->GetNumArguments() == 0)
				{
					return true;
				}
			}
		}

		++it;
	}

	return false;
}

bool OPObjectNode::HasComplexConstructor()
{
	opNode::iterator it = Body->GetBegin();
	opNode::iterator end = Body->GetEnd();

	while(it != end)
	{
		//convert to function
		//does the function have 
		opNode* node = *it;
		if(ConstructorStatementBase* statement = node->ToConstructorStatementBase())
		{
			if(ConstructorNode* ctor = statement->GetConstructor())
			{
				if(ctor->GetFunction()->GetArguments()->GetNumArguments() > 0)
				{
					return true;
				}
			}
		}

		++it;
	}

	return false;
}

