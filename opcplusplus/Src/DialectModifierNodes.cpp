//****************************************************************
// Copyright © 2008 opGames LLC - All Rights Reserved
//
// Authors: Kevin Depue & Lucas Ellis
//
// File: DialectModifierNodes.cpp
// Date: 01/05/2008
//
// Description:
//
// Source for dialect modifier nodes.
//****************************************************************

#include "opCPP.h"

//
// DialectModifiersNode
//

// make sure we don't have any duplicate modifiers
void DialectModifiersNode::CheckForDuplicates()
{
	iterator i   = GetBegin();
	iterator end = GetEnd();
	iterator j;

	while (i != end)
	{
		j = i;
		++j;

		while (j != end)
		{
			if (i->GetId() == j->GetId()) 
			{
				opString modstring;

				j->PrintString(modstring);

				opError::MessageError(*j, "Duplicate modifier '" + modstring + "' found!");			
			}

			++j;
		}

		++i;
	}
}