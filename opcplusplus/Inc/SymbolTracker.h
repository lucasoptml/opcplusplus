///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: SymbolTracker.h
/// Date: 01/02/2007
///
/// Description:
///
/// Declaration of SymbolTracker class(s).
///****************************************************************

///==========================================
/// opSymbolTracker
///==========================================

class opSymbolTracker 
{
public:
	/**** construction / destruction ****/
	opSymbolTracker() {}
	~opSymbolTracker() {}
	
	/**** opmacros stuff ****/
	
	//registration function
	void Register(OPMacroNode* innode);
	
	//query function
	OPMacroNode* OPMacroRegistered(const opString& name);

private:
	// NOTE: we have a different hash table for each opcpp construct
	opHashTable<opString, OPMacroNode*> OPMacrosTable;
};

