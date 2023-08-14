///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: RegexSupport.h
/// Date: 06/02/2007
///
/// Description:
///
/// Regex Support Wrappers
///****************************************************************

//regex support is wrapped
//so it doesn't need to be compiled so much
//could also accomplish this by adding a static library for it.

//#include "../../Lib/boost/boost/xpressive/regex_error.hpp"
#include <regex>

namespace opregex
{
	//may throw boost::xpressive::regex_error
	bool Match(const opString& matchstring, const opString& pattern);

	//may throw an opcpp message exception
	inline bool Match(const opString& matchstring, const opString& pattern, opNode* node)
	{
		try
		{
			return Match(matchstring,pattern);
		}
		catch (const std::regex_error& regexerror)
		{
			// regex error 'mismatched parentheses' in pattern '...'
			opString error = "regex error, ";
			error += "'";
			error += regexerror.what();
			error += "' in pattern ";
			error += "\"";
			error += pattern;
			error += "\"";

			opError::MessageError(node, error);
		}
		/*
		catch(const boost::xpressive::regex_error& regexerror)
		{
			// regex error 'mismatched parentheses' in pattern '...'
			opString error = "regex error, ";
			error += "'";
			error += regexerror.what();
			error += "' in pattern ";
			error += "\"";
			error += pattern;
			error += "\"";

			opError::MessageError(node,error);
		}
		*/

		return false;
	}
}
