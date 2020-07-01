///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: PrintNotes.cpp
/// Date: 05/21/2007
///
/// Description:
///
/// Dialect Based Note Printing
///****************************************************************

//print notes and maps for a given location
void OPObjectNode::PrintLocationNotes(DialectLocation* location, opSectionStream& stream)
{
	if(!location)
		return;

	//iterate over all registered types, print them in order
	//we should print the default visibility before and after each item
	if(location->GetId() == DialectLocation::body)
		PrintVisibility(vismode_default,stream);

	DialectLocationBase::OrderedNoteList&          OrderedNotes = location->GetOrderedNotes();
	DialectLocationBase::OrderedNoteList::iterator i            = OrderedNotes.begin();
	DialectLocationBase::OrderedNoteList::iterator end          = OrderedNotes.end();

	while (i != end)
	{
		DialectNoteBase* item = *i;

		bool bPrintVisibility = true;

		if(DialectNote* noteitem = item->ToNote())
		{
			//print a note
			PrintNote(*location,*noteitem,stream);

			bPrintVisibility = !noteitem->GetNoteDefinition()->IsBodyEmpty();
		}
		else if(DialectMap* mapitem = item->ToMap())
		{
			//print a map
			PrintMap(*location,*mapitem,stream);

			bPrintVisibility = !mapitem->IsAllNotesEmpty();
		}

		if (bPrintVisibility)
		{
			if(location->GetId() == DialectLocation::body)
				PrintVisibility(vismode_default,stream);
		}

		++i;
	}
}

//print notes and maps for a given location
void OPEnumNode::PrintLocationNotes(DialectEnumLocation* location, opSectionStream& stream)
{
	if(!location)
		return;

	DialectLocationBase::OrderedNoteList&          OrderedNotes = location->GetOrderedNotes();
	DialectLocationBase::OrderedNoteList::iterator i            = OrderedNotes.begin();
	DialectLocationBase::OrderedNoteList::iterator end          = OrderedNotes.end();

	while (i != end)
	{
		DialectNoteBase* item = *i;

		if(DialectNote* noteitem = item->ToNote())
		{
			//print a note
			PrintNote(*location,*noteitem,stream);
		}
		else if(DialectEnumMap* mapitem = item->ToEnumMap())
		{
			//print a map
			PrintEnumMap(stream,*GetEnumSettings(),*location,*mapitem);
		}

		++i;
	}
}

//print a mapping
void OPObjectNode::PrintMap(DialectLocation& location, 
							DialectMap& mapitem, 
							opSectionStream& stream)
{
	//print a data or function map?
	if(mapitem.GetMode() == DialectMap::DataMap)
		PrintDataMap(stream,*GetCategorySettings(),location,mapitem);
	else if(mapitem.GetMode() == DialectMap::FunctionMap)
		PrintFunctionMap(stream,*GetCategorySettings(),location,mapitem);
}

void OPTypeBase::PrintNote(DialectLocationBase& location, DialectNote& noteitem, opSectionStream& stream)
{
	if(!noteitem.GetNoteDefinition())
		return;
	
	//check against criteria
	if(!noteitem.IsMapped(this))
		return;
	
	NoteDefinitionNode* notenode = noteitem.GetNoteDefinition();
	
	if(!notenode)
		return;
	
	//NOTE: should I print something on skip?
	if(notenode->IsBodyEmpty())
		return;
	
	//print initial conditionals
	int conditions = PrintConditions(stream);
	
	stream.PrintComment("\t//defined note: " + GetTypeSettings()->GetName() + "::" + location.GetName() + "::" + noteitem.GetName());
	stream << endl;

	PrintNote(noteitem,stream);
	
	//print ending conditionals
	PrintConditionEnd(conditions, stream);
}

void OPTypeBase::PrintNote(DialectNote& note, opSectionStream& stream, NoteType type, int map_count )
{
	const opParameters &p = opParameters::Get();
	
	NoteDefinitionNode* notenode = note.GetNoteDefinition();

	if(!notenode)
		return;

	//NOTE: should I print something on skip?
	if(notenode->IsBodyEmpty())
		return;

	//get the argument names from the note definition
	vector<opString> argumentnames;
	notenode->GetArguments(argumentnames);
	
	stream << endl;
	
	//TODO: create clone for replacement
	stacked<BraceBlockNode> clonebody = notenode->GetBody()->Clone();
	stacked<ExpandableNode> clonenode = opNode::Transform<ExpandableNode>(clonebody);
	
	stackedgroup tempmodifiers;

	//loop over all the argument names, find values for them
	int numargs = (int)argumentnames.size();
	for(int i = 0; i < numargs; i++)
	{
		opNode* argumentvalue = NULL;
		opString argumentname = argumentnames[i];
		
		if(!argumentvalue)
			argumentvalue = GetContextArgument(argumentname);
		
		if(!argumentvalue && type == End)
			argumentvalue = GetEndArgument(argumentname,map_count,tempmodifiers);
		
		if(!argumentvalue)
		{
			//error couldn't find argument
			//this should never happen in release
			//since it should be caught earlier
			//opError::MessageError(this,"internal error! couldn't find note argument: " + argumentnames[i]);
			Log("internal error! couldn't find note argument: " + argumentnames[i]);
			opException::ThrowException();
		}
		else
		{
			if(p.Notations)
			{
				IgnoreNewlines setting(stream.body);
				
				//print the argument value to the stream.
				opString value;
				value = "//\targument '" + argumentname + "' = \"";
				argumentvalue->PrintString(value);
				
				stream.PrintComment(value);
				
				setting.Disable();
				
				stream << "\"" << endl;
			}
			
			clonenode->ReplaceNodes(argumentname,argumentvalue);
		}
	}

	// now we need to do recursive macro replacement
//	MacroOperationWalker runOperations(*clonenode);
	
	//TODO: hook up hidden note printing
	bool bHidden = note.IsHidden();
	
	stream.Indent();
	
	bool bCompact;

	// handle verbatim 
	if (notenode->GetVerbatim())
	{
		bCompact = opParameters::Get().Compact;

		opParameters::GetWritable().Compact = false;
	}

	//TODO: note the origin line here
	{
		LineOrigin originsetting(stream,this);

		if(p.Notations)
		{
			stream.PrintComment("//expansion start");
			stream << endl;
		}
		if(!bHidden)
		{
			clonenode->PrintOriginal(stream);
			stream << endl;
		}
		else
		{
			LineOverride linesetting(stream,this);
			clonenode->PrintOriginal(stream);
			stream << endl;
		}
		if(p.Notations)
		{
			stream.PrintComment("//expansion end");
			stream << endl;
		}
	}

	// handle verbatim
	if (notenode->GetVerbatim())
		opParameters::GetWritable().Compact = bCompact;
	
	stream.DeIndent();

	//TODO: delete the clone, or else cache it.
	clonenode.Delete();

}


TerminalNode* opNode::AppendTerminalNode(const opString& stringvalue, Token tokenid)
{
	stacked<TerminalNode> terminal = NEWNODE(TerminalNode(stringvalue,tokenid,GetLine(),GetFile()));

	TerminalNode* newnode = *terminal;
	
	AppendNode(terminal);

	return newnode;
}

TerminalNode* opNode::PrependTerminalNode(const opString& stringvalue, Token tokenid)
{
	stacked<TerminalNode> terminal = NEWNODE(TerminalNode(stringvalue,tokenid,GetLine(),GetFile()));

	TerminalNode* newnode = *terminal;

	PrependNode(terminal);

	return newnode;
}

opNode* OPObjectNode::GetContextArgument(const opString& argumentname)
{

	return OPTypeBase::GetContextArgument(argumentname);
}

opNode* OPEnumNode::GetContextArgument(const opString& argumentname)
{
	//fast, doesn't really need generators
	if(argumentname == "enum_type")
	{
		return Name;
	}

	return OPTypeBase::GetContextArgument(argumentname);
}

opNode* OPEnumNode::GetElementArgument(const opString& argumentname, OPEnumElementNode* element)
{
	//fast, doesn't really need generators
	if(argumentname == "enum_name")
	{
		return element->GetName();
	}
	else if(argumentname == "enum_value")
	{
		return element->GetInitialization();
	}

	return NULL;
}

opNode* OPTypeBase::GetContextArgument(const opString& argumentname)
{
	return FetchModifier(argumentname);
}

opNode* OPTypeBase::GetModifierArgument(const opString& argumentname, vector<ValuedModifierNode*>& modifiervalues)
{
	//I assume all the names and values pair up.
	int nummodifiers = (int)modifiervalues.size();
	for(int i = 0; i < nummodifiers; i++)
	{
		opString modifiervalue = modifiervalues[i]->GetModifier()->GetValue();
		
		if(modifiervalue == argumentname)
		{
			return modifiervalues[i]->GetArguments();
		}
	}
	
	return NULL;
}

opNode* OPObjectNode::GetStatementArgument(const opString& argumentname, StatementBase* statement, stackedgroup& tempmodifiers)
{
	if(opNode* node = statement->FetchModifier(argumentname))
		return node;
	
	if(argumentname == "source_line")
	{
		int linenumber = statement->GetLine();
		
		stacked<ExpandableArgumentNode> node = NEWNODE(ExpandableArgumentNode);
		node->CopyBasics(this);
		node->SetValue("source_line");
		
		node->AppendTerminalNode(linenumber,T_NUMBER);

		opNode* SourceLineArgument = *node;
		
		tempmodifiers.Push(node);

		return SourceLineArgument;
	}
	else if(argumentname == "source_file")
	{
		FileNode* file = statement->GetFile();
		opString filename = file->GetInputName();

		stacked<ExpandableArgumentNode> node = NEWNODE(ExpandableArgumentNode);
		node->CopyBasics(this);
		node->SetValue("source_file");

		node->AppendTerminalNode(filename,T_ID);

		opNode* SourceFileArgument = *node;

		tempmodifiers.Push(node);

		return SourceFileArgument;
	}

	return NULL;
}

opNode* OPTypeBase::GetStartArgument(const opString& argumentname, stackedgroup& tempmodifiers)
{
	return NULL;
}

opNode* OPTypeBase::GetMappingArgument(const opString& argumentname, int map_index, stackedgroup& tempmodifiers)
{
	if(argumentname == "map_index")
	{
		stacked<ExpandableArgumentNode> node = NEWNODE(ExpandableArgumentNode);
		node->CopyBasics(this);
		node->SetValue("map_index");

		node->AppendTerminalNode(map_index,T_NUMBER);

		opNode* mapindex = *node;

		tempmodifiers.Push(node);

		return mapindex;
	}
	
	return NULL;
}

opNode* OPTypeBase::GetEndArgument(const opString& argumentname, int map_count, stackedgroup& tempmodifiers)
{
	if(argumentname == "map_count")
	{
		stacked<ExpandableArgumentNode> node = NEWNODE(ExpandableArgumentNode);
		node->CopyBasics(this);
		node->SetValue("map_count");

		node->AppendTerminalNode(map_count,T_NUMBER);

		opNode* mapcount = *node;

		tempmodifiers.Push(node);

		return mapcount;
	}

	return NULL;
}

//
// Enum Map Printing
//

void OPEnumNode::PrintEnumMap(opSectionStream& stream, 
							  DialectEnumeration& enumeration, 
							  DialectEnumLocation& location, 
							  DialectEnumMap& mapinfo)
{
	const opParameters& p = opParameters::Get();
	
	iterator i       = Body->GetBegin();
	iterator end     = Body->GetEnd();

	stream << endl;
	stream.PrintComment("// enum map '" + mapinfo.GetName() + "'");
	stream << endl;

	//add a printer that uses the categories...
	//which could easily map into an expansion layer.
	PrintEnumMapStart(stream,enumeration,location,mapinfo);

	int map_index = 0;
	while (i != end)
	{
		OPEnumElementNode* element = node_cast<OPEnumElementNode>(*i);

		if(element)
		{
			if(p.Notations)
			{
				stream << "\t// enum mapping found: ";
				{
					IgnoreNewlines ignorelines(stream.body);
					element->PrintOriginal(stream);
				}			
				stream << endl;
			}
			
			PrintEnumMapping(stream,enumeration,location,mapinfo,element,map_index);
			++map_index;
		}

		++i;

	}

	PrintEnumMapEnd(stream,enumeration,location,mapinfo,map_index);
}

void OPEnumNode::PrintEnumMapStart(opSectionStream& stream, 
									 DialectEnumeration& enumeration, 
									 DialectEnumLocation& location, 
									 DialectEnumMap& mapinfo)
{
	DialectNote& noteitem = mapinfo.GetStartNote();

	if(!noteitem.GetNoteDefinition())
		return;

	stream.PrintComment("\t//defined note: " + enumeration.GetName() + "::" + location.GetName() + "::" + mapinfo.GetName() + "::start");
	stream << endl;

	PrintNote(noteitem,stream);
}

void OPEnumNode::PrintEnumMapEnd(opSectionStream& stream, 
								   DialectEnumeration& enumeration, 
								   DialectEnumLocation& location, 
								   DialectEnumMap& mapinfo,
								   int map_count)
{
	DialectNote& noteitem = mapinfo.GetEndNote();
	
	if(!noteitem.GetNoteDefinition())
		return;
	
	stream.PrintComment("\t//defined note: " + enumeration.GetName() + "::" + location.GetName() + "::" + mapinfo.GetName() + "::end");
	stream << endl;
	
	PrintNote(noteitem,stream,End,map_count);
}

void OPEnumNode::PrintEnumMapping(opSectionStream& stream,
								  DialectEnumeration& enumeration,
								  DialectEnumLocation& location,
								  DialectEnumMap& mapinfo,
								  OPEnumElementNode* element,
								  int map_index)
{
	const opParameters& p = opParameters::Get();
	
	DialectNote& note = element->GetInitialization()?mapinfo.GetValueNote():mapinfo.GetNameNote();
	NoteDefinitionNode& notenode = *note.GetNoteDefinition();

	opString notename = note.GetName();

	stream.PrintComment("\t//defined note: " + enumeration.GetName() + "::" + location.GetName() + "::" + mapinfo.GetName() + "::" + notename);
	stream << endl;

	//get the argument names from the note definition
	vector<opString> argumentnames;
	notenode.GetArguments(argumentnames);

	stacked<BraceBlockNode> clonebody = notenode.GetBody()->Clone();
	stacked<ExpandableNode> clonenode = opNode::Transform<ExpandableNode>(clonebody);

	stackedgroup temparguments;

	//loop over all the argument names, find values for them
	int numargs = (int)argumentnames.size();
	for(int i = 0; i < numargs; i++)
	{
		opNode* argumentvalue = NULL;
		opString argumentname = argumentnames[i];

		if(!argumentvalue)
			argumentvalue = GetContextArgument(argumentname);
		//no modifiers currently (ever?)
// 		if(!argumentvalue)
// 			argumentvalue = GetModifierArgument(argumentname, arguments);
		if(!argumentvalue)
			argumentvalue = GetElementArgument(argumentname, element);
		if(!argumentvalue)
			argumentvalue = GetMappingArgument(argumentname, map_index, temparguments);

		if(!argumentvalue)
		{
			//error couldn't find argument
			//this should never happen in release
			//since it should be caught earlier
			Log("internal error! couldn't find enum mapping argument: " + argumentnames[i]);
			opException::ThrowException();
		}
		else
		{
			if(p.Notations)
			{
				IgnoreNewlines setting(stream.body);

				stream << "\t//\targument '" << argumentnames[i] << "' = \"";
				argumentvalue->PrintOriginal(stream);

				setting.Disable();

				stream << "\"" << endl;
			}

			clonenode->ReplaceNodes(argumentname,argumentvalue);
		}
	}

	//perform operations
//	MacroOperationWalker operations(*clonenode);

	//TODO: hook up hidden note printing
	bool bHidden = note.IsHidden();

	stream.Indent();
	stream.Indent();

	bool bCompact;

	// handle verbatim 
	if (notenode.GetVerbatim())
	{
		bCompact = opParameters::Get().Compact;

		opParameters::GetWritable().Compact = false;
	}

	{
		LineOrigin origin(stream,element);

		if(p.Notations)
		{
			stream.PrintComment("//expansion start");
			stream << endl;
		}
		if(!bHidden)
		{
			clonenode->PrintOriginal(stream);
			stream << endl;
		}
		else
		{
			LineOverride linesetting(stream,element);
			clonenode->PrintOriginal(stream);
			stream << endl;
		}
		if(p.Notations)
		{
			stream.PrintComment("//expansion end");
			stream << endl;
		}
	}

	// handle verbatim 
	if (notenode.GetVerbatim())
		opParameters::GetWritable().Compact = bCompact;

	stream.DeIndent();
	stream.DeIndent();

	clonenode.Delete();
}



//
// Data Map Printing
//

void OPObjectNode::PrintDataMap(opSectionStream& stream, 
								DialectCategory& category, 
								DialectLocation& location, 
								DialectMap& mapinfo)
{
	const opParameters& p = opParameters::Get();

	iterator i       = Body->GetBegin();
	iterator end     = Body->GetEnd();

	stream << endl;
	stream.PrintComment("// data map '" + mapinfo.GetName() + "'");
	stream << endl;
	
	// print conditions on this map...
	int conditions = PrintConditions(stream);

	//add a printer that uses the categories...
	//which could easily map into an expansion layer.
	PrintDataMapStart(stream,category,location,mapinfo);
	
	int map_index = 0;
	while (i != end)
	{
		DataStatementBase* statement = i->ToDataStatementBase();
		
		if(statement)
		{
			//does this statement satisfy the mapping criteria?
			if(!mapinfo.IsMapped(statement))
			{
				++i;
				continue;
			}
			
			if(p.Notations)
			{
				stream << "\t// data mapping found: ";
				{
					IgnoreNewlines ignorelines(stream.body);
					statement->PrintOriginal(stream);
				}			
				stream << endl;

				if(ModifiersNode* automods = statement->GetAutoModifiers())
				{
					stream << "\t//\tautomatic modifiers: ";
					{
						IgnoreNewlines ignorelines(stream.body);
						automods->PrintOriginal(stream);
					}
					stream << endl;
				}
			}
			
			PrintDataMapping(stream,category,location,mapinfo,statement,map_index);
			++map_index;
		}
		
		++i;

	}
	
	PrintDataMapEnd(stream,category,location,mapinfo,map_index);

	// print end conditions
	PrintConditionEnd(conditions, stream);
}

void OPObjectNode::PrintDataMapStart(opSectionStream& stream, 
									 DialectCategory& category, 
									 DialectLocation& location, 
									 DialectMap& mapinfo)
{
	DialectNote& noteitem = mapinfo.GetStartNote();
	
	if(!noteitem.GetNoteDefinition())
		return;

	stream.PrintComment("\t//defined note: " + category.GetName() + "::" + location.GetName() + "::" + mapinfo.GetName() + "::start");
	stream << endl;
	
	PrintNote(noteitem,stream);
}

void OPObjectNode::PrintDataMapEnd(opSectionStream& stream, 
								   DialectCategory& category, 
								   DialectLocation& location, 
								   DialectMap& mapinfo,
								   int map_count)
{
	DialectNote& noteitem = mapinfo.GetEndNote();
	
	if(!noteitem.GetNoteDefinition())
		return;

	stream.PrintComment("\t//defined note: " + category.GetName() + "::" + location.GetName() + "::" + mapinfo.GetName() + "::end");
	stream << endl;

	PrintNote(noteitem,stream,End,map_count);
}


void OPObjectNode::PrintDataMapping(opSectionStream& stream,
					  DialectCategory& category,
					  DialectLocation& location,
					  DialectMap& mapinfo,
					  DataStatementBase* statement,
					  int map_index)
{
	const opParameters& p = opParameters::Get();
	
	DialectNote& note = mapinfo.GetMappingNote();
	NoteDefinitionNode& notenode = *note.GetNoteDefinition();

	if (notenode.IsBodyEmpty())
		return;

	stream.PrintComment("\t//defined note: " + category.GetName() + "::" + location.GetName() + "::" + mapinfo.GetName() + "::mapping");
	stream << endl;

	// print conditions
	int conditions = statement->PrintConditions(stream);

	//get all available modifier values
	vector<ValuedModifierNode*> arguments;
	//statement->BuildArguments(valuedmodifiers,arguments);
	
	statement->BuildValueModifiers(arguments);
	
	//get the argument names from the note definition
	vector<opString> argumentnames;
	notenode.GetArguments(argumentnames);
	
	stacked<BraceBlockNode> clonebody = notenode.GetBody()->Clone();
	stacked<ExpandableNode> clonenode = opNode::Transform<ExpandableNode>(clonebody);
	
	stackedgroup temparguments;
	
	
	//loop over all the argument names, find values for them
	int numargs = (int)argumentnames.size();
	for(int i = 0; i < numargs; i++)
	{
		opNode* argumentvalue = NULL;
		opString argumentname = argumentnames[i];
		
		// grabs normal context arguments.. (and fetches auto modifiers)
		if(!argumentvalue)
			argumentvalue = GetContextArgument(argumentname);
		// grabs specified modifiers
 		if(!argumentvalue)
			argumentvalue = GetModifierArgument(argumentname, arguments);
 		// grabs statement arguments and temporaries (source_line,source_file)
		if(!argumentvalue)
 			argumentvalue = GetStatementArgument(argumentname, statement, temparguments);
		// grabs temp mapping arguments (map_index)
		if(!argumentvalue)
			argumentvalue = GetMappingArgument(argumentname, map_index, temparguments);
		
		if(!argumentvalue)
		{
			//error couldn't find argument
			//this should never happen in release
			//since it should be caught earlier
			Log("internal error! couldn't find data mapping argument: " + argumentnames[i]);
			opException::ThrowException();
		}
		else
		{
			if(p.Notations)
			{
				IgnoreNewlines setting(stream.body);
				
				stream << "\t//\targument '" << argumentnames[i] << "' = \"";
				argumentvalue->PrintOriginal(stream);
				
				setting.Disable();
				
				stream << "\"" << endl;
			}
			
			clonenode->ReplaceNodes(argumentname,argumentvalue);
		}
	}
	
	//perform operations
//	MacroOperationWalker operations(*clonenode);
	
	bool bHidden = note.IsHidden();
	
	stream.Indent();
	stream.Indent();
	
	if(p.Notations)
	{
		stream.PrintComment("//expansion start");
		stream << endl;
	}

	bool bCompact;

	// handle verbatim 
	if (notenode.GetVerbatim())
	{
		bCompact = opParameters::Get().Compact;

		opParameters::GetWritable().Compact = false;
	}

	{
		LineOrigin origin(stream,statement);

		if(!bHidden)
		{
			clonenode->PrintOriginal(stream);
			stream << endl;
		}
		else
		{
			LineOverride linesetting(stream,statement);
			clonenode->PrintOriginal(stream);
			stream << endl;
		}
		if(p.Notations)
		{
			stream.PrintComment("//expansion end");			
			stream << endl;
		}
	}

	// handle verbatim 
	if (notenode.GetVerbatim())
		opParameters::GetWritable().Compact = bCompact;

	stream.DeIndent();
	stream.DeIndent();
	
	clonenode.Delete();

	statement->PrintConditionEnd(conditions,stream);
}

//
// Function Map Printing
//

void OPObjectNode::PrintFunctionMap(opSectionStream& stream, 
									DialectCategory& category, 
									DialectLocation& location, 
									DialectMap& mapinfo)
{
	const opParameters& p = opParameters::Get();
	
	iterator i       = Body->GetBegin();
	iterator end     = Body->GetEnd();

	stream << endl;
	stream.PrintComment("// function map '" + mapinfo.GetName() + "'");
	stream << endl;

	int conditions = PrintConditions(stream);

	//add a printer that uses the categories...
	//which could easily map into an expansion layer.
	PrintFunctionMapStart(stream,category,location,mapinfo);
	
	int map_index = 0;

	while (i != end)
	{
		FunctionStatementBase* statement = i->ToFunctionStatementBase();
		
		if(statement)
		{
			//does this statement satisfy the mapping criteria?
			if(!mapinfo.IsMapped(statement))
			{
				++i;
				continue;
			}
			
			if(p.Notations)
			{
				stream << "\t// function mapping found: ";
				{
					IgnoreNewlines ignorelines(stream.body);
					statement->PrintOriginal(stream);
				}
				stream << endl;
				
				if(ModifiersNode* automods = statement->GetAutoModifiers())
				{
						stream << "\t//\tautomatic modifiers: ";
						{
							IgnoreNewlines ignorelines(stream.body);
							automods->PrintOriginal(stream);
						}
						stream << endl;
				}
			}
			
			PrintFunctionMapping(stream,category,location,mapinfo,statement,map_index);
			++map_index;
		}
		
		++i;
	}
	
	PrintFunctionMapEnd(stream,category,location,mapinfo,map_index);

	//print end conditions
	PrintConditionEnd(conditions,stream);
}

void OPObjectNode::PrintFunctionMapStart(opSectionStream& stream, 
									 DialectCategory& category, 
									 DialectLocation& location, 
									 DialectMap& mapinfo)
{
	DialectNote& noteitem = mapinfo.GetStartNote();
	
	if(!noteitem.GetNoteDefinition())
		return;

	stream.PrintComment("\t//defined note: " + category.GetName() + "::" + location.GetName() + "::" + mapinfo.GetName() + "::start");
	stream << endl;

	PrintNote(noteitem,stream);
}

void OPObjectNode::PrintFunctionMapEnd(opSectionStream& stream, 
								   DialectCategory& category, 
								   DialectLocation& location, 
								   DialectMap& mapinfo,
								   int map_count)
{
	DialectNote& noteitem = mapinfo.GetEndNote();

	if(!noteitem.GetNoteDefinition())
		return;

	stream.PrintComment("\t//defined note: " + category.GetName() + "::" + location.GetName() + "::" + mapinfo.GetName() + "::end");
	stream << endl;

	PrintNote(noteitem,stream,End,map_count);
}


void OPObjectNode::PrintFunctionMapping(opSectionStream& stream,
									DialectCategory& category,
									DialectLocation& location,
									DialectMap& mapinfo,
									FunctionStatementBase* statement,
									int map_index)
{
	const opParameters& p = opParameters::Get();
	
	DialectNote& note = mapinfo.GetMappingNote();
	NoteDefinitionNode& notenode = *note.GetNoteDefinition();

	stream.PrintComment("\t//defined note: " + category.GetName() + "::" + location.GetName() + "::" + mapinfo.GetName() + "::mapping");
	stream << endl;

	// print all start conditions
	int conditions = PrintConditions(stream);

	//get all available modifier values
	vector<ValuedModifierNode*> arguments;
	statement->BuildValueModifiers(arguments);

	//get the argument names from the note definition
	vector<opString> argumentnames;
	notenode.GetArguments(argumentnames);

	stacked<BraceBlockNode> clonebody = notenode.GetBody()->Clone();
	stacked<ExpandableNode> clonenode = opNode::Transform<ExpandableNode>(clonebody);

	stackedgroup temparguments;

	//loop over all the argument names, find values for them
	int numargs = (int)argumentnames.size();
	for(int i = 0; i < numargs; i++)
	{

		opNode* argumentvalue = NULL;
		opString argumentname = argumentnames[i];
		
		if(!argumentvalue)
			argumentvalue = GetContextArgument(argumentname);
		if(!argumentvalue)
			argumentvalue = GetModifierArgument(argumentname, arguments);
		if(!argumentvalue)
			argumentvalue = GetStatementArgument(argumentname, statement, temparguments);
		if(!argumentvalue)
			argumentvalue = GetMappingArgument(argumentname,map_index,temparguments);
		
		if(!argumentvalue)
		{
			//error couldn't find argument
			//this should never happen in release
			//since it should be caught earlier
			Log("internal error! couldn't find function mapping argument: " + argumentnames[i]);
			opException::ThrowException();
		}
		else
		{
			if(p.Notations)
			{
				IgnoreNewlines setting(stream.body);

				stream << "\t//\targument '" << argumentnames[i] << "' = \"";
				argumentvalue->PrintOriginal(stream);
				setting.Disable();

				stream << "\"" << endl;
			}
			
			clonenode->ReplaceNodes(argumentname,argumentvalue);
		}
	}

	//perform operations
//	MacroOperationWalker operations(*clonenode);
	
	bool bHidden = note.IsHidden();
	
	stream.Indent();
	stream.Indent();
	
	if(p.Notations)
	{
		stream.PrintComment("//expansion start");
		stream << endl;
	}
	
	bool bCompact;

	// handle verbatim 
	if (notenode.GetVerbatim())
	{
		bCompact = opParameters::Get().Compact;

		opParameters::GetWritable().Compact = false;
	}

	{
		LineOrigin origin(stream,statement);

		if(!bHidden)
		{
			clonenode->PrintOriginal(stream);
			stream << endl;
		}
		else
		{
			LineOverride linesetting(stream,statement);
			clonenode->PrintOriginal(stream);
			stream << endl;
		}
		if(p.Notations)
		{
			stream.PrintComment("//expansion end");
			stream << endl;
		}
	}

	// handle verbatim 
	if (notenode.GetVerbatim())
		opParameters::GetWritable().Compact = bCompact;

	stream.DeIndent();
	stream.DeIndent();

	clonenode.Delete();

	//print conditions end
	PrintConditionEnd(conditions, stream);
}


