///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: PreParse.cpp
/// Date: 01/03/2007
///
/// Description:
///
/// PreParse Pass Source
///****************************************************************

#include "opCPP.h"

//
// opNode
//

// pre process
bool opNode::PreProcess()
{
	if (PreParse())
		return PreProcessChildren();
	
	return false;
}

// pre process children
bool opNode::PreProcessChildren()
{
	//iterator i		= GetBegin();
	iterator end	= GetEnd();
	bool     result = true;

	ResetPosition();

	//while (i != end)
	while (GetPosition() != end)
	{
		//result = i->PreProcess(tracker) ? result : false;
		result = GetPosition()->PreProcess() ? result : false;

		//++i;
		IncrementPosition();
	}

	ResetPosition();

	return result;
}

// post process
bool opNode::PostProcess()
{
	if (PostParse())
		return PostProcessChildren();

	return false;
}

// post process children
bool opNode::PostProcessChildren()
{
	iterator i		= GetBegin();
	iterator end	= GetEnd();
	bool     result = true;

	while (i != end)
	{
		result = i->PostProcess() ? result : false;

		++i;
	}

	return result;
}

// pre parse
bool opNode::PreParse()
{
	return true;
}

// post parse
bool opNode::PostParse()
{
	// NOTE: This is pointless - our parsing is pretty strict.
	//       Enabling this just limits what people can do.
	/*POSTPARSE_START;
	{
		Token id = GetId();
		
		// make sure no opcpp keywords remain
		if (TokenFunctions::IsOPCPPKeyword(id) || TokenFunctions::IsTemporaryGrammar(id))
		{
			opError::DisallowError(this);
		}
	}
	POSTPARSE_END;*/

	return true;
}

// 
// OPFileNode
// 

bool OPFileNode::PreParse()
{
	PREPARSE_START;
	{
		//FindOPMacros();
		//FindExpandCalls();		//??
		//ParseGlobalContext();
	}
	PREPARSE_END;
}

///
/// ExpandCallArgumentListNode
///

bool ExpandCallArgumentListNode::PreParse()
{
	PREPARSE_START;
	{
		//we can only make this once!
		//NOTE: maybe should assert?
		//if(!Arguments.size())
		MakeCommaList(Arguments);
	}
	PREPARSE_END;
}

///
/// ExpandCallArgumentNode
///

bool ExpandCallArgumentNode::PreParse()
{
	PREPARSE_START;
	{
		//automatically called
 		FindExpandCalls();
	}
	PREPARSE_END;
}

///
/// ExpandCallNode
///

opString ExpandCallNode::GetSignature()
{
	opString sig;
	Name->PrintString(sig);

	if(Arguments)
	{
		sig += '(';
		sig += (int)Arguments->GetArguments().size();
		sig += " arguments";
		sig += ')';
	}

	return sig;
}

bool ExpandCallNode::Expand(opSymbolTracker& tracker, opNode::iterator expandit, opNode* parent)
{
	//TODO: reimplement expansion depth checking
	OPERATIONS_START;

	//TODO: add good errors

	//need to have the arguments parsed
	if(Arguments)
		Arguments->PreProcess();
	
	opString signature = GetSignature();
	
	if (OPMacroNode* macro = tracker.OPMacroRegistered(signature))
	{			
		stacked<OPMacroBodyNode> cloned     = macro->GetBody()->Clone();
		opNode*			         parentNode = GetParent();

		++ExpansionDepth;

		if (ExpansionDepth > opParameters::Get().OPMacroExpansionDepth.GetValue())
		{
			opString error = 
				"Maximum opmacro expansion depth (" + 
				opString(opParameters::Get().OPMacroExpansionDepth.GetValue()) + 
				") exceeded!";

			opError::MessageError(this,error);
		}

		// parse arguments
		//NOTE: this in effect finds expands and expands the arguments
		//      before we substitute them into the opmacro

		if (Arguments)
		{
			opMacroExpander expander(tracker,Arguments);

			if(expander.Errored())
				opException::ThrowException();
		}
		
		// expand the cloned nodes
		// this performs the expansion and replacement operations...
		macro->Expand(*cloned, Arguments);
		
		//in order to expand arguments which are ... expand
		//we must determine what order to use for this.
		
		//A. expand within the expand argument (this makes some sort of sense) - I think this is best.
		//how?
		//1. find expands within argument
		//2. run pre-process on this early
		MacroConcatenationWalker operations;

		//perform initial opmacro/concat fixes
		operations.MacroConcatenations(*cloned);

		// identify opmacro and expansion calls
		cloned->FindOPMacros();

		//perform expand/concat fixes
		operations.ExpandConcatenations(*cloned);

		//find expand calls recursively using a walker class
		ExpandFinder expandfind(*cloned);

		//run expand calls recursively
		opMacroExpander expander(tracker,*cloned);
		
		if(expander.Errored())
			opException::ThrowException();
	
		parent->CollapseNode(cloned, expandit);
		
		ExpansionDepth--;
	}
	else
	{
		//TODO: this could be much improved
		opError::ExpandError(this,signature,tracker);
	}

	OPERATIONS_END;
}

///
/// OPMacroNode
///

bool OPMacroArgumentNode::PreParse()
{
	PREPARSE_START;
	{
		Check(T_ID);
		CheckNone();
		
		VariableName = FirstChild();
	}
	PREPARSE_END;
}

bool OPMacroArgumentListNode::PreParse()
{
	PREPARSE_START;
	{
		//we can only make this once!
		//NOTE: should we assert?
		if(!Arguments.size())
		{
			CleanAll();
			MakeCommaList(Arguments);
		}
	}
	PREPARSE_END;
}

bool OPMacroNode::PreProcess()
{
	//manually do this
	//we don't want pre-process the body initially
	//NOTE: this catches argument types I guess...
	if(Arguments && !Arguments->PreProcess())
		return false;
	
	//identify the operators in the body...
	MacroOperatorFinder findoperators(Body);

	return PreParse();
}

opString OPMacroNode::GetSignature()
{
	opString sig = GetName()->GetValue();
	if(Arguments)
	{
		sig += '(';
		sig += (int)Arguments->GetArguments().size();
		sig += " arguments";
		sig += ')';
	}
	
	return sig;
}


