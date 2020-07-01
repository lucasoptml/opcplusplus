///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Config.h
/// Date: 03/13/2007
///
/// Description:
///
/// Configuration-related code. 
///****************************************************************

//linux detection
#if defined(__GNUC__) 
	#define PLATFORM_LINUX
#endif

//windows detection
#if defined(WIN32) || defined(WIN64)
	#define PLATFORM_WINDOWS
#endif

//check for stuff
#ifdef _INC_WINDOWS
	#error dont use windows.h!
#endif

//special library settings
#ifdef PLATFORM_WINDOWS
	#define BOOST_FILESYSTEM_NO_LIB
#endif


