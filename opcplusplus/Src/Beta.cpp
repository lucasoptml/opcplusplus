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

#include "opCPP.h"

///
/// opBetaSupport
///

// set beta stuff here!

bool   opBetaSupport::isBetaBuild = false;
time_t opBetaSupport::ExpireDate  = opTimeUtility::GetTime(5, 1, 2008);

// returns true if this beta compiler is expired
bool opBetaSupport::BetaExpired()
{
	return opTimeUtility::GetCurrentTime() > ExpireDate;
}