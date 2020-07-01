///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: WindowsSource.cpp
/// Date: 08/07/2007
///
/// Description:
///
/// Linux Platform Specific Code
///****************************************************************

#include "opCPP.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

/*=== logging ===*/

void errors::opLog::DebugLog(const opString& s)
{
	// do nothing, we have no IDE..
}

///==========================================
/// Windows opCpp.exe Path
///==========================================

void opPlatform::Breakpoint()
{
	// nothing to do here..
}

// Parse the executable location.
bool opPlatform::ParseExeLocation(const opString& cmdarg)
{
	opString resolved;

	resolved.Resize(10000);

	int length = readlink("/proc/self/exe", &resolved[0], 10000);

	// If we have a problem parsing the exe location, error.
	if ( length == -1 )
	{
		const char* upxhack = getenv("   ");

		if (!upxhack)
		{
			Log("An error occurred when determining the executable path in Linux.  Please report to www.opcpp.com.");

			return false;			
		}

		resolved = upxhack;
	}
	else
	{
		resolved.Resize(length);
	}

	path resolvedpath = resolved.GetString();
	resolved = resolvedpath.string();

	opCppPath      = resolved;
	opCppDirectory = resolved.RLeft('/'); 
	opCppExecutableName = resolved.RRight('/'); 

	return true;
}

/*=== timing ===*/

double timing::opTimer::GetTimeSeconds()
{
	timeval  curr_time;
	double   time;

	gettimeofday(&curr_time, 0);

	time  = curr_time.tv_sec;
	time += (double) curr_time.tv_usec * 1e-6;

	return time;
}

void timing::opTimer::InitTimeSeconds()
{
	timeval  curr_time;
	double   time;

	gettimeofday(&curr_time, 0);

	time  = curr_time.tv_sec;
	time += (double) curr_time.tv_usec * 1e-6;

	timerstart = time;
}


