///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Beta.h
/// Date: 12/13/2007
///
/// Description:
///
/// Beta-related code.
///****************************************************************

namespace beta
{

class opBetaSupport
{
public:

	static bool IsBetaBuild()
	{
		return isBetaBuild;
	}

	static bool BetaExpired();

	static void PrintBetaExpiredString()
	{
		Log("");
		Log("========================================");
		Log("This opC++ Beta Compiler is now expired.");
		Log("========================================");
		Log("");
	}

private:

	static time_t ExpireDate;
	static bool   isBetaBuild;
};

} // end namespace beta