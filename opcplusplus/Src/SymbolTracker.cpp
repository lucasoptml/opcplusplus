///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: SymbolTracker.cpp
/// Date: 01/02/2007
///
/// Description:
///
/// SymbolTracker source code.
///****************************************************************

#include "opCPP.h"

///==========================================
/// opSymbolTracker
///==========================================

void opSymbolTracker::Register(OPMacroNode* innode)
{
	opString keystring = innode->GetSignature();
	
	//just to make sure we have newer opmacros replace already defined ones
	//we may want to allow some control of this behavior
	OPMacrosTable.Erase(keystring);
	OPMacrosTable.Insert(keystring, innode);
}

OPMacroNode* opSymbolTracker::OPMacroRegistered(const opString& signature)
{
	OPMacroNode* result = NULL;

	OPMacrosTable.Find(signature, result);

	return result;
}
