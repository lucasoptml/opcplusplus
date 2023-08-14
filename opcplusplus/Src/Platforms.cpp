///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Platforms.cpp
/// Date: 10/28/2007
///
/// Description:
///
/// Platform specific source code.
///****************************************************************

#include "opCPP.h"

//
// opPlatform
//

opString opPlatform::opCppExecutableName;
opString opPlatform::opCppPath;
opString opPlatform::opCppDirectory;

// Get the executable name.
opString opPlatform::GetOpCppExecutableName()
{
	return opCppExecutableName;
}

// Get the executable path.
opString opPlatform::GetOpCppPath()
{
	return opCppPath;
}

// Get the executable directory.
opString opPlatform::GetOpCppDirectory()
{
	return opCppDirectory;
}

// Returns the timestamp on the executable.
std::filesystem::file_time_type opPlatform::GetOpCppTimeStamp()
{
	std::filesystem::path opcpppath = GetOpCppPath().GetString();	

	return last_write_time(opcpppath);
}

#include <assert.h>

//you hit an assertion, step out
void opPlatform::Assertion()
{
	assert(0);
}
