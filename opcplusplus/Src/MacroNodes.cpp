///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: ExpandNodes.cpp
/// Date: 04/19/2007
///
/// Description:
///
/// Expand Operation Nodes
///****************************************************************

#include "opCPP.h"

void ExpandableNode::ReplaceNodes(const opString& matchname, opNode* node)
{
	DoReplacement(this,matchname,node);
}

void ExpandableNode::ReplaceNodes(const opString& matchname, iterator start, iterator end)
{
	DoReplacement(this,matchname,start,end);
}

void ExpandableNode::CallOperators()
{
	//perform operators.

}

void ExpandableNode::CallExpands()
{
	//find expand nodes
	//call expand nodes
}

void ExpandableNode::DoReplacement(opNode* currentnode, const opString& matchname, iterator instart, iterator inend)
{
	iterator i   = currentnode->GetBegin();
	iterator end = currentnode->GetEnd();
	
	while (i != end)
	{
		opNode* currentChild = *i;
		
		if (currentChild->IsTerminal())
		{
			if (currentChild->GetStringValue() == matchname)
			{
				//delete the matched node
				iterator newi = i;
				++i;
				currentnode->DeleteNode(newi);
				
				//add the replacement nodes
				iterator init = instart;
				while(init != inend)
				{
					stacked<opNode> newReplacement = init->CloneGeneric();
					
					if(node_cast<ExpandableArgumentNode>(*newReplacement)
					|| node_cast<ValuedModifierArgumentNode>(*newReplacement))
						currentnode->CollapseNode(newReplacement, i);
					else
						currentnode->InsertNode(newReplacement, i);
					
					++init;
				}
				
				continue;
			}
		}
		else
			DoReplacement(currentChild, matchname, instart, inend);
		
		++i;
	}
}

void ExpandableNode::DoReplacement(opNode* currentnode, const opString& matchname, opNode* replacement)
{
	iterator i   = currentnode->GetBegin();
	iterator end = currentnode->GetEnd();
	
	while (i != end)
	{
		opNode* currentChild = *i;
		
		if (TerminalNode* terminal = node_cast<TerminalNode>(currentChild))
		{
			if (terminal->GetValue() == matchname)
			{
				//delete the matched node
				iterator newi = i;
				++i;
				currentnode->DeleteNode(newi);
				
				opNode* replace = replacement;
				if(ValuedModifierNode* node = node_cast<ValuedModifierNode>(replacement))
					replace = node->GetArguments();
				
				//add the replacement node
				stacked<opNode> newReplacement = replace->CloneGeneric();
				
				if(node_cast<ExpandableArgumentNode>(*newReplacement)
				|| node_cast<ValuedModifierArgumentNode>(*newReplacement))
					currentnode->CollapseNode(newReplacement, i);
				else
					currentnode->InsertNode(newReplacement, i);
				
				continue;
			}
		}
		else
			DoReplacement(currentChild, matchname, replacement);
		
		++i;
	}
}




