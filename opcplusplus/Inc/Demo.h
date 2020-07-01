///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Demo.h
/// Date: 08/25/2007
///
/// Description:
///
/// opCpp Demo
///****************************************************************

//TODO: figure out a good way of inserting the code watermark (it has issues w/ some things)
//		I'm thinking a class in the header, and an initialization
//		in the source, once per file errr...thats bad
//		issue is I need to print the front note for template types...hmmmm

#include <stdlib.h>
#include <time.h>

namespace demo
{

///
/// opDemoSupport
///

class opDemoSupport
{
	friend class opParameters;

public:
	/*=== utility ===*/

	static bool IsDemoMode()
	{
		return DemoMode;
	}

	// Demo string related..
	static const char* GetDemoBanner();
	static opString    DecToOctal(int decimal);
	static opString    RandLetters();
	static opString    GenerateDemoString();
	static void        PrintDemoString(opFileStream& stream, opString classname, OPObjectNode* object);

	// Blacklist related..
	static void PopulateBlacklist();
	static bool IsBlacklisted(const opString& key, const opString& hash);

	// Reading license key related..
	static bool ValidateLicense(opParameters& params);

private:
	static bool                           DemoMode;
	static const char*                    LicenseFileName;
	static const char*                    ExtraHashString;
	static opMultiMap<opString, opString> Blacklist;
};

} // end namespace demo

