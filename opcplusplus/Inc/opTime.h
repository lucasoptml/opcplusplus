///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Time.h
/// Date: 12/13/2007
///
/// Description:
///
/// Time-related stuff (not to be confused with timers).
///****************************************************************

class opTimeUtility
{
public:

	// Assumes month is [1,12], day is [1,31], year is [1900, ..].
	static time_t GetTime(int month, int day, int year)
	{
		struct tm t;

		memset( &t, 0, sizeof(struct tm) );

 		t.tm_mon  = month - 1;
		t.tm_mday = day;	
		t.tm_year = year - 1900;

		return mktime( &t );
	}

	// Returns the current time.
	static time_t GetCurrentTime()
	{
		return time( NULL );
	}
};