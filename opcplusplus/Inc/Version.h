///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Version.h
/// Date: 12/11/2007
///
/// Description:
///
/// Class to manage the compiler version.
///****************************************************************

class opVersion 
{
private:
	
	/*=== data ===*/

	static float    Version;
	static opString VersionString;

public:

	/*=== utility ===*/

	// Get the compiler version.
	static float GetVersion()
	{
		return Version;
	}

	// Get the compiler version string.
	static opString GetVersionString()
	{
		return VersionString;
	}

	// Print the compiler version (for -version).
	static void PrintVersion()
	{
		Log("");

		if ( opDemoSupport::IsDemoMode() )
			Log("======= DEMO VERSION - EVALUATION USE ONLY =======");

		Log( "opC++ Compiler v. " + VersionString );
		Log( "Build Date: " + opString(__DATE__) + " @ " + __TIME__ );
		Log( "Copyright 2007 opGames LLC" );
		
		if ( opDemoSupport::IsDemoMode() )
			Log("======= DEMO VERSION - EVALUATION USE ONLY =======");

		Log("");
	}
};