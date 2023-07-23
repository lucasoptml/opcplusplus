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
/// Windows Platform Specific Code
///****************************************************************

/*=== logging ===*/

#include "Config.h"
#include "opLog.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void errors::opLog::DebugLog(const opString& s)
{
	OutputDebugString(s.GetString().c_str());
}

/*=== platform ===*/

#include "Platforms.h"

// If you hit a breakpoint, step out.
void opPlatform::Breakpoint()
{
	DebugBreak();
}

// Parse the executable location.
bool opPlatform::ParseExeLocation(const opString& cmdarg)
{
	char filename[1024];

	::GetModuleFileName(NULL, filename, 1024);

	opCppPath           = filename;
	opCppDirectory      = opCppPath.RLeft("\\"); 
	opCppExecutableName = opCppPath.RRight("\\"); 
	return true;
}

/*=== timing ===*/

#include "Timer.h"

double timing::opTimer::GetTimeSeconds()
{
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);

	return ((double)(time.QuadPart))*invtimerfrequency;
}

void timing::opTimer::InitTimeSeconds()
{
	LARGE_INTEGER timerfrequency;
	QueryPerformanceFrequency(&timerfrequency);

	invtimerfrequency = (1.0)/((double)timerfrequency.QuadPart);
}


