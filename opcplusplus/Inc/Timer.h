///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Timer.h
/// Date: 03/13/2007
///
/// Description:
///
/// Timer-related code.
///****************************************************************

//Lucas: Special Timer Code

namespace timing
{

class opTimer
{
public:
	static double GetTimeSeconds();
	static void InitTimeSeconds();
private:
	static unsigned long timerstart;
	static double        invtimerfrequency;
};

}; // end timing namespace

