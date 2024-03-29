///****************************************************************
/// Copyright � 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: RegexSource.cpp
/// Date: 06/02/2007
///
/// Description:
///
/// Regex Source
///****************************************************************

#include "opSTL.h"

//#include "../../Lib/boost/boost/xpressive/xpressive.hpp"
//
//using boost::xpressive::sregex;
//using boost::xpressive::smatch;
//using boost::xpressive::regex_match;

#include <regex>
//using std::regex;

namespace opregex
{
	bool Match(const opString& matchstring, const opString& pattern)
	{
		/*
		sregex expression = sregex::compile(pattern.GetString());
		smatch match;
		
		return regex_match(matchstring.GetString(),match,expression);
		*/

		std::smatch matches;
		
		std::regex regex(pattern.GetString());
		
		string _matchstring = matchstring.GetString();

		auto result = std::regex_match(_matchstring, 
			                           matches, 
			                           regex); 

		return result;
	}
};

