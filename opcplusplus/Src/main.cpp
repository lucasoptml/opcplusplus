///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: main.cpp
/// Date: 11/26/2006
///
/// Description:
///
/// Entry point for the opcpp compiler.
///****************************************************************

//enable benchmark mode (runs x 3000)
// #define BENCHMARK
//enable memory leak checking...
// #define MALLOC_DEBUGGING

#ifdef MALLOC_DEBUGGING
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif

#include "opCPP.h"

// main function - entry point to app
int main(int argc, char** argv)
{
#ifdef MALLOC_DEBUGGING
	//create a memory checkpoint - because we don't want string literals or static initializations to show up
	//we only want to check things allocated after this checkpoint
	_CrtMemState memorystate;
	_CrtMemCheckpoint(&memorystate);

	//_CrtSetBreakAlloc(337);

	//NOTE: memory tracking
	//can set breakpoints on particular allocations
	//use _CrtSetBreakAlloc(number listed in report);

#endif

	/*=== initialize boost ===*/

	// speed up iostream a bit
	std::ios::sync_with_stdio(false);

	// setup boost filesystem's behavior 
	// (native instead of portable, so windows paths are allowed)
	
	//TODO: this might be needed, but moved?
	//path::default_name_check(boost::filesystem::native);
	
	boost::filesystem::initial_path();

	/*=== parse the executable location ===*/

	if (!opPlatform::ParseExeLocation(argv[0]))
		return -1;

	/*=== parse cmd params ===*/

	opParameters&    p = opParameters::GetWritable();
	vector<opString> params;

	for (int i = 1; i < argc; i++)
		params.push_back(opString(argv[i]));

	p.Init(params);

	if (!p.Parse())
		return -1;

	/*=== handle beta builds ===*/

	// If this is an expired beta build, terminate.
	if ( opBetaSupport::IsBetaBuild() && opBetaSupport::BetaExpired() )
	{
		opBetaSupport::PrintBetaExpiredString();

		return -1;
	}

	/*=== validate the license file ===*/

	if ( !opDemoSupport::ValidateLicense(p) )
	{
		if ( !p.Version )
			opVersion::PrintVersion();
	}

	/*=== if we're to print compiler version, do it and quit ===*/

	if (p.Version)
	{
		opVersion::PrintVersion();

		return 0;
	}

	/*=== initialize the driver and compile the code ===*/

	opDriver driver;

	driver.Initialize();
		
#ifdef BENCHMARK
	for(int loop = 0; loop < 3000; loop++)
#endif
	{
		// try to convert the input source
		if (!driver.Convert(p))
			return 1;
	}

	//TODO: memory freeing fixes:
	//		has issues with...
	//1. ParseErrors Array
	//2. opParameters (need to create and delete)

#ifdef MALLOC_DEBUGGING
	_CrtMemDumpAllObjectsSince(&memorystate);
#endif

	opParameters::Destroy();

	return 0;
}

