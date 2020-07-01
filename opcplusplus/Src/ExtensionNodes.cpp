///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: ExtentionNodes.cpp
/// Date: 01/04/2007
///
/// Description:
///
/// Extention nodes source code.
///****************************************************************

#include "opCPP.h"

///
/// ExpandCallNode
///

int ExpandCallNode::ExpansionDepth = 0;

///
/// OPMacroNode
///

// expand a single argument
void OPMacroNode::ExpandChildren(opNode* cloned, ExpandCallArgumentNode* replacement, const opString& variable)
{
	iterator i   = cloned->GetBegin();
	iterator end = cloned->GetEnd();

	while (i != end)
	{
		opNode* currentChild = *i;

		if (currentChild->IsTerminal())
		{
			if (currentChild->GetStringValue() == variable)
			{
				iterator newi = i;

				++i;
				cloned->DeleteNode(newi);

				stacked<ExpandCallArgumentNode> newReplacement = replacement->Clone();

				cloned->CollapseNode(newReplacement, i);
				continue;
			}
		}
		else
			ExpandChildren(currentChild, replacement, variable);

		++i;
	}
}

//TODO: should we replace this with @/spacer?
// do concatenation expansion
void OPMacroNode::ExpandConcatenation(opNode* cloned)
{
	iterator i	 = cloned->GetBegin();
	iterator end = cloned->GetEnd();

	// check for concatenation at ends
	if (i != end)
	{
		opNode* errored = NULL;
		if (i->GetId() == T_CONCATENATION) 
			errored = *i;
		else if (cloned->GetLast()->GetId() == T_CONCATENATION)
			errored = *cloned->GetLast();
		
		if(errored)
		{
			opError::MessageError(errored,"## opmacro operator cannot concatenate block boundaries.");
		}
	}

	while (i != end)
	{
		opNode* current = *i;

		if (current->IsTerminal())
		{
			if (current->GetId() != T_CONCATENATION)
			{
				++i;
				continue;
			}

			iterator nexti    = i; ++nexti;
			iterator previ    = i; --previ;
			opNode*  previous = *previ;
			opNode*  next	  = *nexti;

			if (next->IsTerminal() && previous->IsTerminal())
			{
				//make sure they aren't whitespace!
				if (previous->IsWhitespace() || next->IsWhitespace())
				{
					opError::MessageError(current,"## opmacro operator cannot concatenate whitespace.");
				}

				//all we have to do is
				//delete all 3
				//insert a node at previous's position (preserve line #!
				//set the value to our string
				//set the id to T_ID, or else do a lookup on it
				//thats all folks

				//now we need to grab the pasted string
				opString  pastedstring = previous->GetStringValue() + next->GetStringValue();
				int       line		   = current->GetLine();
				FileNode* file		   = current->GetFile();

				iterator newi = nexti; ++newi;

				//delete all 3 nodes
				cloned->DeleteNode(previ);
				cloned->DeleteNode(i);
				cloned->DeleteNode(nexti);

				stacked<TerminalNode> newnode = NEWNODE(TerminalNode(opToken(T_ID,pastedstring,line),file));

				cloned->InsertNode(newnode,newi);

				i = newi;
				continue;
			}
		}
		else
			ExpandConcatenation(current);

		++i;
	}
}

// expand an opmacro call
void OPMacroNode::Expand(opNode* cloned, ExpandCallArgumentListNode* args)
{
	// make argument size checks
	int NumMacroArgs  = (Arguments == NULL) ? 0 : (int) Arguments->GetArguments().size();
	int NumExpandArgs = (args == NULL) ? 0 : (int) args->GetArguments().size();

	if (NumMacroArgs != NumExpandArgs)
	{
		opError::MessageError(args,
			opString("Number of expand arguments does not match number of opmacro arguments. (got ")
			+ NumExpandArgs + ", needed " + NumMacroArgs + ')'			
			);
	}	
	else if (NumMacroArgs == 0)
		return;
	
	// do the expansion
	const vector<OPMacroArgumentNode*>&    MacroArgs  = Arguments->GetArguments();
	const vector<ExpandCallArgumentNode*>& ExpandArgs = args->GetArguments();

	for (int i = 0; i < NumExpandArgs; i++)
	{
		ExpandChildren(cloned, ExpandArgs[i], MacroArgs[i]->GetVariableName()->GetStringValue());
	}
	
	// expand concatenation
	//ExpandConcatenation(cloned);
	//MacroOperationWalker runoperations(cloned);

	// expand stringize
}

void OPObjectNode::SetCategory(opNode* category)
{
	//extract the category name now...
	if(TerminalNode* node = node_cast<TerminalNode>(category))
	{
		SetCategoryName(node->GetValue());
	}
}

DialectCategory* OPObjectNode::GetCategorySettings()
{
	if(!CategorySettings)
	{
		opString categoryname = GetCategoryName();
		CategorySettings = DialectTracker::GetCategory(categoryname);
	}
	
	return CategorySettings;
}

DialectEnumeration* OPEnumNode::GetEnumSettings()
{
	if(!EnumSettings)
	{
		opString enumname = Identifier->GetValue();
		EnumSettings = DialectTracker::GetEnumeration(enumname);
	}

	return EnumSettings;
}

///
/// OPObjectInheritanceNode
///

void OPObjectInheritanceNode::CloneParentNames(opNode* insertinto)
{
	int num = Parents.Size();
	for(int i = 0; i < num; i++)
	{
		Parent& p = Parents[i];
		
		stacked<opNode> cloned = p.Type->CloneGeneric();
		insertinto->AppendNode(cloned);

		if(i + 1 < num)
		{
			insertinto->AppendTerminalNode(",",T_COMMA);
			insertinto->AppendTerminalNode(" ",T_WHITESPACE);
		}
	}
}

void OPObjectInheritanceNode::CloneInheritance(opNode* insertinto)
{
	int num = Parents.Size();
	for(int i = 0; i < num; i++)
	{
		Parent& p = Parents[i];

		if(p.Virtual)
		{
			stacked<TerminalNode> clonedvirtual = p.Virtual->Clone();
			insertinto->AppendNode(clonedvirtual);
		}

		if(p.Visibility)
		{
			stacked<TerminalNode> clonedvisibility = p.Visibility->Clone();
			insertinto->AppendNode(clonedvisibility);
		}

		stacked<opNode> clonedtype = p.Type->CloneGeneric();
		insertinto->AppendNode(clonedtype);

		if(i + 1 < num)
		{
			insertinto->AppendTerminalNode(",",T_COMMA);
			insertinto->AppendTerminalNode(" ",T_WHITESPACE);
		}
	}
}

