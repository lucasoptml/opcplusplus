///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Tokens.cpp
/// Date: 11/26/2006
///
/// Description:
///
/// Contains code initializing the data structures that keep track of all tokens,
/// grammars, etc.
///****************************************************************

#include "opCPP.h"

///
/// opTokenMap
///

opString                     opTokenMap::TokenToString[Tokens_MAX];
opHashTable<opString, Token> opTokenMap::StringToToken;
opHashTable<Token, opString> opTokenMap::TokenToDialectModifier;
opString                     opTokenNames::TokenNames[Tokens_MAX];

namespace metaprogramming
{

//initialize token strings meta-program
template<Token current>
struct InitializeTokenStrings
{
	static void Exec()
	{
		//map this token	
		MapToken();

		//map next token
		InitializeTokenStrings<(Token)(current+1)>::Exec();
	}

private:
	static void MapToken()
	{
		//lets yell if we aren't mapped but should be
		//lets set up the string array...
		//lets set up all the mapping hashes
		//should this token be mapped?
		//TODO: more of this can be made compile time I think.
		if(!TokenFunctions::IsStartEndToken(current))
		{
			//should be mapped
			if(!tokenmapping::IsMapped<current>::value)
			{
				//log an error! point to the token list maybe?
				Log(opString("error: found unmapped token (") + (int)current + ')');

				return;
			}

			// if we have a dialect modifier grammar, register it
			if (tokenmapping::IsDialectModifier<current>::value)
			{
				tokenmapping::RegisterDialectModifier<current>();
			}

			//map the token id string value
			//this says nothing about the friendly name
			opTokenNames::SetTokenName(current,tokenmapping::TokenIdString<current>());

			tokenmapping::RegisterMapping<current>();
		}
	}
};

//ends on Tokens_MAX
template<>
struct InitializeTokenStrings<Tokens_MAX>
{
	static void Exec() {}
};

}

using namespace metaprogramming;

// initialize tokens
void InitTokens()
{
	//iterate over all keywords....
	//throw some string errors if they
	//aren't mapped but should be.
	//I may have made this too complicated
	//possibly...but I don't think so
	InitializeTokenStrings<Tokens_MIN>::Exec();
}

