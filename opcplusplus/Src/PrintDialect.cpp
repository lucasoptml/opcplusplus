///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: PrintDialect.cpp
/// Date: 10/16/2007
///
/// Description:
///
/// Dialect Printing
///****************************************************************


void LocationNodeBase::PrintXml(opXmlStream& stream)
{
	opXmlTag locationtag(stream,"Location");

	{
		opXmlTextTag nametag(stream,"Name");
		stream << Name->GetStringValue();
	}

	PrintXmlChildren(stream);
}

void CategoryNode::PrintXml(opXmlStream& stream)
{
	opXmlTag categorytag(stream,"Category");

	{
		opXmlTextTag nametag(stream, "Name");
		stream << Name->GetValue();
	}

	PrintXmlChildren(stream);
}

void DisallowNode::PrintXml(opXmlStream& stream)
{
	opXmlTag disallowtag(stream,"Disallow");
	
	{
		opXmlTextTag nametag(stream,"Name");
		stream << Name->GetValue();
	}
	
	PrintXmlChildren(stream);
}

void NoteNode::PrintXml(opXmlStream& stream)
{
	opXmlTag notetag(stream,"Note");
	
	NoteInfo->PrintXml(stream);
	
	PrintXmlChildren(stream);
}

void EnumerationNode::PrintXml(opXmlStream& stream)
{
	opXmlTag notetag(stream,"Enumeration");
	
	{
		opXmlTextTag nametag(stream,"Name");
		stream << Name->GetValue();
	}
	
	PrintXmlChildren(stream);	
}

void EnumerationMapNode::PrintXml(opXmlStream& stream)
{
	opXmlTag notetag(stream,"Map");
	
	MapInfo->PrintXml(stream);

	PrintXmlChildren(stream);
}

void DataModifierNode::PrintXml(opXmlStream& stream)
{
	opXmlTag notetag(stream,"Modifier");

	{
		opXmlTextTag nametag(stream,"Name");
		stream << Name->GetValue();
	}
	
	{
		opXmlTextTag nametag(stream,"Type");
		stream << "Data";
	}

	if(Arguments)
	{
		opXmlTextTag valuetag(stream,"Value");

		if(TerminalNode* condition = Arguments->GetCondition())
			stream << condition->GetValue();
	}

	PrintXmlChildren(stream);
}

void FunctionModifierNode::PrintXml(opXmlStream& stream)
{
	opXmlTag notetag(stream,"Modifier");
	
	{
		opXmlTextTag nametag(stream,"Name");
		stream << Name->GetValue();
	}

	{
		opXmlTextTag nametag(stream,"Type");
		stream << "Function";

	}

	if(Arguments)
	{
		opXmlTextTag valuetag(stream,"Value");

		if(TerminalNode* condition = Arguments->GetCondition())
			stream << condition->GetValue();
	}

	PrintXmlChildren(stream);
}

void CategoryDatamapNode::PrintXml(opXmlStream& stream)
{
	opXmlTag notetag(stream,"Map");

	MapInfo->PrintXml(stream);

	PrintXmlChildren(stream);
}

void CategoryFunctionmapNode::PrintXml(opXmlStream& stream)
{
	opXmlTag notetag(stream,"Map");
	
	MapInfo->PrintXml(stream);

	PrintXmlChildren(stream);
}

void CategoryExpressionMapNode::PrintXml(opXmlStream& stream)
{
	if(ExpressionMap)
	{
		//opXmlTag modifierstag(stream,"Modifiers");

		opArray<opString> modifiers;
		
		ExpressionMap->GetModifiers(modifiers);
		
		opSet<opString> modifierset;
		modifierset.insert(modifiers.begin(),modifiers.end());
		
		opSet<opString>::iterator it = modifierset.begin();
		opSet<opString>::iterator end = modifierset.end();

		while(it != end)
		{
			opXmlTag tag(stream, "Modifier");

			{
				opXmlTextTag name(stream,"Name");
				stream << *it;
			}

			//now, we need to know if the modifier has a description...
			opString description;
			//NOTE: needs to look at all modifiers, valued modifiers
			if(ExpressionMap->GetModifierDescription(*it,description))
			{
				opXmlTextTag d(stream,"Description");
				stream << description;
			}

			++it;
		}
	}	
}


//
// Dialect Setting Printing
//

void DialectNote::PrintXml(opXmlStream& stream)
{
	{
		opXmlTextTag nametag(stream,"Name");
		stream << GetName();
	}

	if(GetNoteDefinition() && GetNoteDefinition()->GetFile())
	{
		{
			opXmlTextTag pathtag(stream,"Path");
			stream << GetNoteDefinition()->GetFile()->GetInputName();
		}

		{
			opXmlTextTag linetag(stream,"Line");
			stream << opString(GetNoteDefinition()->GetLine());
		}
	}

	//print all arguments
	vector<opString> arguments;
	BuildValidArguments(arguments);
	
	//opXmlTag argumentstag(stream,"Arguments");
	
	int num = (int)arguments.size();
	for(int i = 0; i < num; i++)
	{
		opXmlTag argument(stream,"Argument");
		
		{
			opXmlTextTag name(stream,"Name");
			stream << arguments[i];
		}

		opString description;
		//NOTE: needs to only look at valued modifiers!
		//		and arguments...
		if(GetArgumentDescription(arguments[i],description))
		{
			opXmlTextTag d(stream,"Description");
			stream << description;
		}

	}
}

void DialectEnumNote::PrintXml(opXmlStream& stream)
{
	DialectNote::PrintXml(stream);
}

void DialectMapNote::PrintXml(opXmlStream& stream)
{
	opXmlTag notetag(stream,"Note");

	DialectNote::PrintXml(stream);
}

void DialectEnumMapNote::PrintXml(opXmlStream& stream)
{
	opXmlTag notetag(stream,"Note");

	DialectNote::PrintXml(stream);
}

void DialectMap::PrintXml(opXmlStream& stream)
{
	{
		opXmlTextTag nametag(stream,"Name");
		stream << GetName();
	}

	{
		opXmlTextTag typetag(stream,"Type");

		if(Mode == DataMap)
			stream << "Data";
		else if(Mode == FunctionMap)
			stream << "Function";
	}
	
	//print all notes
	StartNote.PrintXml(stream);
	MappingNote.PrintXml(stream);
	EndNote.PrintXml(stream);
}

void DialectEnumMap::PrintXml(opXmlStream& stream)
{
	{
		opXmlTextTag nametag(stream,"Name");
		stream << GetName();
	}
	
	{
		opXmlTextTag typetag(stream,"Type");

		stream << "Enum";
	}
	
	//print all notes
	StartNote.PrintXml(stream);
	NameNote.PrintXml(stream);
	ValueNote.PrintXml(stream);
	EndNote.PrintXml(stream);
}

void OPIncludeNode::PrintXml(opXmlStream& stream )
{
	opXmlTag includetag(stream,"Include");

	{
		opXmlTextTag name(stream,"Name");
		stream << FileName->GetValue();
	}

	IncludedFile->PrintIncluded(stream);
}
