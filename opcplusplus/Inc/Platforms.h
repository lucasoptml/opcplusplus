///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Platforms.h
/// Date: 03/13/2007
///
/// Description:
///
/// Platform-specific stuff.
///****************************************************************

//
// Platform Specific Code Header
//

class opPlatform
{
public:
	static bool     ParseExeLocation(const opString& cmdarg);
	static opString GetOpCppExecutableName();
	static opString GetOpCppPath();
	static opString GetOpCppDirectory();
	static time_t	GetOpCppTimeStamp();
	static void     Assertion();
	static void     Breakpoint();

private:
	static opString opCppExecutableName;
	static opString opCppPath;
	static opString opCppDirectory;
};



