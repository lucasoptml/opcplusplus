///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: DialectNodes.cpp
/// Date: 03/23/2007
///
/// Description:
///
/// Dialect nodes source file.
///****************************************************************

#include "opCPP.h"

// This is called for dialect files after post parsing is complete.
bool DialectFileNode::PostOperations()
{
	OPERATIONS_START
	{
		DialectRegistrationVisitor visitor;

		visit( visitor );

		if ( visitor.Failed() )
			return false;
	}
	OPERATIONS_END
}

void CategoryBodyNode::Register(DialectCategory* categoryinfo)
{
	ResetPosition();

	while(GetPosition() != GetEnd())
	{
		opNode* currentnode = CurrentNode();

		if(CategoryLocationStatementNode* statement = node_cast<CategoryLocationStatementNode>(currentnode))
		{
			CategoryLocationNode* node = node_cast<CategoryLocationNode>(statement->GetStatement());
			if(node)
				node->Register(categoryinfo);
		}
		else if(DataModifierStatementNode* statement = node_cast<DataModifierStatementNode>(currentnode))
		{
			DataModifierNode* node = node_cast<DataModifierNode>(statement->GetStatement());
			if(node)
				node->Register(categoryinfo);
		}
		else if(FunctionModifierStatementNode* statement = node_cast<FunctionModifierStatementNode>(currentnode))
		{
			FunctionModifierNode* node = node_cast<FunctionModifierNode>(statement->GetStatement());
			if(node)
				node->Register(categoryinfo);
		}
		else if(DisallowStatementNode* statement = node_cast<DisallowStatementNode>(currentnode))
		{
			DisallowNode* node = node_cast<DisallowNode>(statement->GetStatement());
			if(node)
				node->Register(categoryinfo);
		}

		IncrementPosition();
	}
}

void EnumerationBodyNode::Register(DialectEnumeration* enumerationinfo)
{
	ResetPosition();

	while(GetPosition() != GetEnd())
	{
		opNode* currentnode = CurrentNode();

		//TODO: need a new location statement node for this
		if(EnumerationLocationStatementNode* statement = node_cast<EnumerationLocationStatementNode>(currentnode))
		{
			EnumerationLocationNode* node = node_cast<EnumerationLocationNode>(statement->GetStatement());
			if(node)
				node->Register(enumerationinfo);
		}
		else if(DisallowStatementNode* statement = node_cast<DisallowStatementNode>(currentnode))
		{
			DisallowNode* node = node_cast<DisallowNode>(statement->GetStatement());
			if(node)
				node->Register(enumerationinfo);
		}

		IncrementPosition();
	}
}



void FunctionModifierNode::Register(DialectCategory* categorynode)
{
	Validate();
	categorynode->RegisterFunctionModifier(this);
}

void DataModifierNode::Register(DialectCategory* categorynode)
{
	Validate();
	categorynode->RegisterDataModifier(this);
}

void DisallowNode::Register(DialectTypeBase* typeinfo)
{
	DisallowInfo = typeinfo->RegisterDisallow(this);
	
	Body->Register(DisallowInfo);
}

void CategoryLocationNode::Register(DialectCategory* categoryinfo)
{
	DialectLocation* locationinfo = categoryinfo->RegisterLocation(this);
	
	Body->Register(locationinfo);
}

void EnumerationLocationNode::Register(DialectEnumeration* enuminfo)
{
	DialectEnumLocation* locationinfo = enuminfo->RegisterLocation(this);
	
	Body->Register(locationinfo);
}

void CategoryLocationBodyNode::Register(DialectLocation* locationinfo)
{
	ResetPosition();

	while(GetPosition() != GetEnd())
	{
		if(CategoryDatamapStatementNode* statement = node_cast<CategoryDatamapStatementNode>(CurrentNode()))
		{
			CategoryDatamapNode* node = node_cast<CategoryDatamapNode>(statement->GetStatement());
			if(node)
				node->Register(locationinfo);
		}
		else if(CategoryFunctionmapStatementNode* statement = node_cast<CategoryFunctionmapStatementNode>(CurrentNode()))
		{
			CategoryFunctionmapNode* node = node_cast<CategoryFunctionmapNode>(statement->GetStatement());
			if(node)
				node->Register(locationinfo);
		}
		else if(NoteStatementNode* statement = node_cast<NoteStatementNode>(CurrentNode()))
		{
			NoteNode* node = node_cast<NoteNode>(statement->GetStatement());
			if(node)
				node->Register(locationinfo);
		}

		IncrementPosition();
	}
}

void EnumerationLocationBodyNode::Register(DialectEnumLocation* locationinfo)
{
	ResetPosition();

	while(GetPosition() != GetEnd())
	{
		if(EnumerationMapStatementNode* statement = node_cast<EnumerationMapStatementNode>(CurrentNode()))
		{
			EnumerationMapNode* node = node_cast<EnumerationMapNode>(statement->GetStatement());
			if(node)
				node->Register(locationinfo);
		}
		else if(NoteStatementNode* statement = node_cast<NoteStatementNode>(CurrentNode()))
		{
			NoteNode* node = node_cast<NoteNode>(statement->GetStatement());
			if(node)
				node->Register(locationinfo);
		}

		IncrementPosition();
	}
}

void EnumerationMapNode::Register(DialectEnumLocation* locationinfo)
{
	MapInfo = locationinfo->RegisterEnumMap(this);
}

void NoteNode::Register(DialectLocationBase* locationinfo)
{
	NoteInfo = locationinfo->RegisterNote(this);

	if(Body)
	{
		Body->Register(NoteInfo);
	}
}

void CategoryMapNode::Register(DialectLocation* locationinfo)
{
	MapInfo = locationinfo->RegisterMap(this);
	
	if(Body)
	{
		Body->Register(MapInfo);
	}
}

void CategoryExpressionMapNode::Register(DialectExpressionMap* mapinfo)
{
	ExpressionMap = mapinfo;
	
	ResetPosition();

	while(GetPosition() != GetEnd())
	{
		DialectStatementBase* statement = CurrentNode()->ToDialectStatementBase();

		if(statement)
		{
			DialectNodeBase* node = statement->GetStatement();
			if(node)
			{
				CriteriaNodeBase* criteria = node->ToCriteriaNodeBase();
				mapinfo->RegisterCriteria(criteria);
			}
		}

		IncrementPosition();
	}

}

DialectNote* DialectLocation::RegisterNote(NoteNode* node)
{
	//find the note,
	//if found, we should error (already defined)
	opString name = node->GetName()->GetValue();
	
	DialectNote* foundnote = GetNote(name);
	if(foundnote)
	{
		opError::NoteRedeclaredError(foundnote,node);
	}

	//if not found, do below..
	DialectNote* newnote = new DialectNote(node->GetName()->GetValue(),this, node);

	Notes.push_back(newnote);

	InsertOrdered( newnote, node->GetOrder() );

	return newnote;
}

DialectNote* DialectEnumLocation::RegisterNote(NoteNode* node)
{
	//find the note,
	//if found, we should error (already defined)
	opString name = node->GetName()->GetValue();

	DialectEnumNote* foundnote = GetNote(name);
	if(foundnote)
	{
		opError::NoteRedeclaredError(foundnote,node);
	}

	//if not found, do below..
	DialectEnumNote* newnote = new DialectEnumNote(node->GetName()->GetValue(),this, node);

	Notes.push_back(newnote);
	InsertOrdered( newnote, node->GetOrder() );

	return newnote;
}

DialectNote* DialectLocation::GetNote(const opString& name)
{
	int num = (int)Notes.size();
	for(int i = 0; i < num; i++)
	{
		if(Notes[i]->GetName() == name)
			return Notes[i];
	}
	
	return NULL;
}


DialectEnumNote* DialectEnumLocation::GetNote(const opString& name)
{
	int num = (int)Notes.size();
	for(int i = 0; i < num; i++)
	{
		if(Notes[i]->GetName() == name)
			return Notes[i];
	}

	return NULL;
}

DialectMap* DialectLocation::GetMap(const opString& name)
{
	int num = (int)Maps.size();
	for(int i = 0; i < num; i++)
	{
		if(Maps[i]->GetName() == name)
			return Maps[i];
	}

	return NULL;
}


DialectEnumMap* DialectEnumLocation::GetEnumMap(const opString& name)
{
	int num = Maps.Size();
	for(int i = 0; i < num; i++)
	{
		if(Maps[i]->GetName() == name)
			return Maps[i];
	}

	return NULL;
}

DialectMap* DialectLocation::RegisterMap(CategoryMapNode* node)
{
	//find the map,
	//if found, we should error (already defined)
	opString name = node->GetName()->GetValue();
	
	DialectMap* foundmap = GetMap(name);
	if(foundmap)
	{
		opError::MapRedeclaredError(foundmap,node);
	}
	
	//if not found, do below..
	DialectMap* newmap = new DialectMap(node->GetName()->GetValue(),this,node);
	
	newmap->SetMode(node->GetMode());
	
	Maps.push_back(newmap);
	InsertOrdered( newmap, node->GetOrder() );
	
	return newmap;
}

DialectEnumMap* DialectEnumLocation::RegisterEnumMap(EnumerationMapNode* node)
{
	//find the map,
	//if found, we should error (already defined)
	opString name = node->GetName()->GetValue();

	DialectEnumMap* foundmap = GetEnumMap(name);
	if(foundmap)
	{
		//TODO: need to support this error
		//opError::MapRedeclaredError(foundmap,node);
		opError::MessageError(node,"map redeclared");
	}

	//if not found, do below..
	DialectEnumMap* newmap = new DialectEnumMap(node->GetName()->GetValue(),this,node);

	Maps.push_back(newmap);
	InsertOrdered( newmap, node->GetOrder() );

	return newmap;
}

DialectMapExpression* DialectExpressionMap::RegisterCriteria(CriteriaNodeBase* node)
{
	Expressions.PushBack(node);
	return &*Expressions.Last();
}

//
// Note Definition Node
//

bool NoteArgumentListNode::Parse()
{
	PARSE_START;
	{
		MakeCommaList<NoteArgumentNode>(Arguments);
	}
	PARSE_END;
}

bool NoteArgumentNode::Parse()
{
	PARSE_START;
	{
		ResetPosition();
		Name = Check<TerminalNode>(T_ID);
		CheckNone();
	}
	PARSE_END;
}

// returns the number of items in a note's path
int NoteDefinitionNode::NumPathItems()
{
	return GetPath()->GetScopeNum();
}

// returns true if the note has a path item at the given index
bool NoteDefinitionNode::HasPathItem(int index)
{
	if (NumPathItems() <= index)
		return false;
	return true;
}

// returns true and sets 'name' via reference if the note has a path item at the given index
bool NoteDefinitionNode::GetPathItem(int index, opString& name)
{
	if (NumPathItems() <= index)
		return false;

	TerminalNode* tnode = node_cast<TerminalNode>(GetPath()->GetScopeNode(index));

	if(!tnode)
		return false;
	
	name = tnode->GetValue();

	return true;
}

bool NoteDefinitionNode::GetPathType(opString& name)
{
	return GetPathItem(0,name);
}

bool NoteDefinitionNode::GetPathLocation(opString& name)
{
	return GetPathItem(1,name);
}

bool NoteDefinitionNode::GetPathTag(opString& name)
{
	return GetPathItem(2,name);
}

bool NoteDefinitionNode::GetPathSubTag(opString& name)
{
	return GetPathItem(3,name);
}

bool NoteDefinitionNode::HasPathTag()
{
	return HasPathItem(2);
}

bool NoteDefinitionNode::HasPathSubTag()
{
	return HasPathItem(3);
}

bool NoteDefinitionNode::PostParse()
{
	POSTPARSE_START;
	{
		MacroOperatorFinder findOperators(Body);
	}
	POSTPARSE_END;

	return true;
}

void NoteDefinitionNode::GetArguments(vector<opString>& arguments)
{
	iterator end = Arguments->GetEnd();
	iterator begin = Arguments->GetBegin();

	for(iterator it = begin; it != end; ++it)
	{
		if(NoteArgumentNode* argument = node_cast<NoteArgumentNode>(*it))
			arguments.push_back(argument->GetName()->GetValue());
	}
}

bool NoteDefinitionNode::IsBodyEmpty()
{
	iterator i   = Body->GetBegin();
	iterator end = Body->GetEnd();
	Token    id;

	// handle verbatim
	if (bVerbatim)
		return false;

	if (opParameters::Get().Compact)
	{
		while (i != end)
		{
			id = i->GetId();

			if (id != T_WHITESPACE
			&&  id != T_NEWLINE
			&&  id != T_COMMENT
			&&  id != T_CCOMMENT)
			{
				return false;
			}

			++i;
		}
	}
	else
	{
		while (i != end)
		{
			id = i->GetId();

			if (id != T_WHITESPACE
				&&  id != T_NEWLINE)
			{
				return false;
			}

			++i;
		}
	}

	return true;
}

bool CriteriaArgumentNode::Parse()
{
	PARSE_START;
	{
		ResetPosition();

		//is there a string in this?
		if (IsCurrent(T_STRING))
		{
			//pull it out as the regex condition
			Argument = Check<TerminalNode>(T_STRING);
		}

		CheckNone();
	}
	PARSE_END;
}

bool ModifierArgumentNode::Parse()
{
	PARSE_START;
	{
		ResetPosition();
		//is there a string in this?
		if(IsCurrent(T_STRING))
		{
			//pull it out as the regex condition
			Condition = Check<TerminalNode>(T_STRING);
		}
		CheckNone();
	}
	PARSE_END;
}


CategoryMapNode::MapMode CategoryMapNode::GetMode()
{
	return node_cast<CategoryDatamapNode>(this) ? MapModeEnum::DataMap : MapModeEnum::FunctionMap;
}

///==========================================
/// Expression Evaluation
///==========================================

void IsNode::GetKnownModifiers(opSet<opString>& modifiers)
{
	Body->BuildKnown(modifiers);
}

void CriteriaBodyNode::BuildKnown(opSet<opString>& knownnames, bool bNegative)
{
	int numoperators = Operators.Size();
	for(int i = 0; i < numoperators; i++)
	{
		Operator op = Operators[i];

		bool bOr = op == Or || op == Or_Not;

		if(bOr)
			return;
	}

	//grab the first modifiers
	BuildKnownOperand(Operands[0],knownnames,bNegate?!bNegative:bNegative);

	for(int i = 0; i < numoperators; i++)
	{
		Operator op = Operators[i];
		
		bool bNot = op == And_Not || op == Or_Not;
		
		BuildKnownOperand(Operands[i+1],knownnames,bNot?!bNegative:bNegative);
	}
}

void CriteriaBodyNode::BuildKnownOperand(opNode* operand, opSet<opString>& knownnames, bool bNegative)
{
	if(TerminalNode* node = node_cast<TerminalNode>(operand))
	{
		//knownnames.Insert(node->GetValue());
	}
	else if(CriteriaValueModifierNode* node = node_cast<CriteriaValueModifierNode>(operand))
	{
		knownnames.Insert(node->GetName()->GetValue());
	}
	else if(CriteriaGroupNode* node = node_cast<CriteriaGroupNode>(operand))
	{
		node->BuildKnown(knownnames,bNegative);
	}
}


bool CriteriaBodyNode::Validate(DialectExpressionMap* map)
{
	int numoperands = Operands.Size();

	bool result = true;
	for(int i = 0; i < numoperands; i++)
	{
		result = ValidateOperand(Operands[i],map)?result:false;
	}

	return result;
}

bool CriteriaBodyNode::ValidateOperand(opNode* operand, DialectExpressionMap* map)
{
	// lets validate the operand...
	
	if (TerminalNode* modifier = node_cast<TerminalNode>(operand))
	{
		if (!map->HasBasicModifier(modifier->GetValue()))
		{
			// prints out all available modifiers (grab from the expression map!)
			opError::OperandError(modifier,map,modifier->GetValue());
		}
		else
		{
			return true;
		}
	}
	else if (CriteriaValueModifierNode* valuemod = node_cast<CriteriaValueModifierNode>(operand))
	{
		const opString& name = valuemod->GetName()->GetValue();
		
		// attempt to grab from the user attached modifiers
		if (map->HasValueModifier(name))
		{
			if (!valuemod->GetArgument()->GetArgument())
				return true;
			
			// now check the regex 
			opString pattern = valuemod->GetArgument()->GetArgument()->GetValue();
			
			pattern = pattern.Substring(1,pattern.Length()-1);
			
			regex::Match("",pattern,valuemod);
			
			return true;
		}
		else
		{
			// prints out all available modifiers (grab from the expression map!)
			opError::OperandError(valuemod,map,name + "()");
		}
	}
	else if (CriteriaGroupNode* group = node_cast<CriteriaGroupNode>(operand))
	{
		return group->Validate(map);
	}
	
	return false;
}

bool CriteriaBodyNode::Evaluate(ModifierSupportBase* statement)
{
	bool currentvalue = EvaluateOperand(Operands[0],statement);
	
	if(bNegate)
		currentvalue = !currentvalue;
	
	int numoperators = Operators.Size();
	for(int i = 0; i < numoperators; i++)
	{
		Operator op = Operators[i];
		
		bool bAnd = op == And || op == And_Not;
		bool bNot = op == And_Not || op == Or_Not;
		
		if(bAnd)
		{
			//if false, can skip evaluating next operand
			if(!currentvalue)
				continue;
		}
		else
		{
			//if true, can skip evaluating next operand
			if(currentvalue)
				continue;
		}
		
		bool rightvalue = EvaluateOperand(Operands[i+1],statement);

		if(bNot)
			rightvalue = !rightvalue;
				
		//now set the current value
		if(bAnd)
		{
			if(!rightvalue)
				currentvalue = false;
		}
		else
		{
			if(rightvalue)
				currentvalue = true;
		}
	}

	return currentvalue;
}

bool CriteriaBodyNode::EvaluateOperand(opNode* operand, ModifierSupportBase* statement)
{
	if(TerminalNode* modifier = node_cast<TerminalNode>(operand))
	{
		return statement->HasModifier(modifier->GetValue());
	}
	else if(CriteriaValueModifierNode* valuemod = node_cast<CriteriaValueModifierNode>(operand))
	{
		const opString& name = valuemod->GetName()->GetValue();
		
		//attempt to grab from the user attached modifiers
		ValuedModifierNode* node = statement->GetValuedModifier(name);
		
		if(!node)
			return false;
		
		if(!valuemod->GetArgument()->GetArgument())
			return true;
		
		//now check the regex against it
		//NOTE: could cache here.
		opString modifiervalue;
		node->GetArguments()->PrintString(modifiervalue);
		
		opString pattern = valuemod->GetArgument()->GetArgument()->GetValue();
		
		pattern = pattern.Substring(1,pattern.Length()-1);
		
		return regex::Match(modifiervalue,pattern,valuemod);
	}
	else if(CriteriaGroupNode* group = node_cast<CriteriaGroupNode>(operand))
	{
		return group->Evaluate(statement);
	}
	
	return false;
}

bool IsNode::Evaluate(ModifierSupportBase* statement)
{
	return Body->Evaluate(statement);
}

bool IsNode::Validate(DialectExpressionMap* map)
{
	return Body->Validate(map);
}

///==========================================
/// Code Node
///==========================================

bool CodeNode::PostParse()
{
	POSTPARSE_START
	{
		const opString& name = Name->GetValue();

		if (name != "heading" 
		&&  name != "body" 
		&&  name != "footer" 
		&&  name != "source" 
		&&  name != "sourceheading"
		&&  name != "sourcefooter")
		{
			opError::MessageError(Name,"invalid code location specified");
		}
	}
	POSTPARSE_END
}

void CodeNode::PrintDialectNode(opDialectStream& stream)
{
	bool bCompact;

	if (bVerbatim)
	{
		bCompact = opParameters::Get().Compact;

		opParameters::GetWritable().Compact = false;
	}
	
	const opString& name = Name->GetValue();
	
	if (name == "heading")
	{
		Body->PrintOriginal(stream.header.heading);
	}
	else if (name == "body")
	{
		Body->PrintOriginal(stream.header.body);
	}
	else if (name == "footer")
	{
		Body->PrintOriginal(stream.header.footer);
	}
	else if (name == "source")
	{
		Body->PrintOriginal(stream.source.body);
	}
	else if (name == "sourceheading")
	{
		Body->PrintOriginal(stream.source.heading);
	}
	else if (name == "sourcefooter")
	{
		Body->PrintOriginal(stream.source.footer);
	}
	
	if (bVerbatim)
		opParameters::GetWritable().Compact = bCompact;
}

//==========================================
// ModifierNodeBase
//==========================================

// validate the regex expression, if it has one
void ModifierNodeBase::Validate()
{
	if(Arguments && Arguments->GetCondition())
	{
		opString pattern = Arguments->GetCondition()->GetValue();

		pattern = pattern.Substring(1,pattern.Length()-1);

		regex::Match("",pattern,this);
	}
}

bool ModifierNodeBase::GetDescription(opString& description)
{
	if(opNode* parent = GetParent())
	{
		if(DialectStatementBase* statement = parent->ToDialectStatementBase())
		{
			if(DialectModifiersNode* modifiers = statement->GetModifiers())
			{
				if(DescriptionNode* mod = node_cast<DescriptionNode>(modifiers->GetModifier(G_DESCRIPTION)))
				{
					description = mod->GetDescription()->GetValue();
					description.TrimQuotes();
					return true;
				}
			}
		}	
	}

	return false;
}

