///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: opCPPSource.cpp
/// Date: 11/26/2006
///
/// Description:
///
/// This file is the source code "glob" file.  It merges all .cpp's into one 
/// before compiling.  This dramatically reduces compile times.
///****************************************************************

#include "opCPP.h"

//
// OPCPP Source Glob
//

#ifdef PLATFORM_WINDOWS
	//OPCPP Related:
	//print a compiling message to stdout ?
	#define OPCOMPILING_SOURCE(filestr)	\
		__pragma(message(filestr))

	#define OPCOMPILING_SOURCE_START \
		__pragma(message(" :::::::::::"))

	#define OPCOMPILING_SOURCE_END \
		__pragma(message(" =========== "))

#else
	#define OPCOMPILING_SOURCE(filestr) 
	#define OPCOMPILING_SOURCE_START 
	#define OPCOMPILING_SOURCE_END 
#endif

OPCOMPILING_SOURCE_START;

OPCOMPILING_SOURCE("Beta.cpp");
#include "Beta.cpp"

OPCOMPILING_SOURCE("Version.cpp");
#include "Version.cpp"

OPCOMPILING_SOURCE("Platforms.cpp");
#include "Platforms.cpp"

OPCOMPILING_SOURCE("opError.cpp");
#include "opError.cpp"

OPCOMPILING_SOURCE("Tokens.cpp");
#include "Tokens.cpp"

OPCOMPILING_SOURCE("Driver.cpp");
#include "Driver.cpp"

OPCOMPILING_SOURCE("Stream.cpp");
#include "Stream.cpp"

OPCOMPILING_SOURCE("opNode.cpp");
#include "opNode.cpp"

OPCOMPILING_SOURCE("SymbolTracker.cpp");
#include "SymbolTracker.cpp"

OPCOMPILING_SOURCE("PreParse.cpp");
#include "PreParse.cpp"

OPCOMPILING_SOURCE("Parameters.cpp");
#include "Parameters.cpp"

OPCOMPILING_SOURCE("Exceptions.cpp");
#include "Exceptions.cpp"

OPCOMPILING_SOURCE("Parse.cpp");
#include "Parse.cpp"

OPCOMPILING_SOURCE("PostParse.cpp");
#include "PostParse.cpp"

OPCOMPILING_SOURCE("Print.cpp");
#include "Print.cpp"

OPCOMPILING_SOURCE("PrintNotes.cpp")
#include "PrintNotes.cpp"

OPCOMPILING_SOURCE("PrintMaps.cpp")
#include "PrintMaps.cpp"

OPCOMPILING_SOURCE("FileNodes.cpp");
#include "FileNodes.cpp"

OPCOMPILING_SOURCE("StatementNodes.cpp");
#include "StatementNodes.cpp"

OPCOMPILING_SOURCE("ModifierNodes.cpp");
#include "ModifierNodes.cpp"

OPCOMPILING_SOURCE("DialectModifierNodes.cpp");
#include "DialectModifierNodes.cpp"

OPCOMPILING_SOURCE("ExtensionNodes.cpp");
#include "ExtensionNodes.cpp"

OPCOMPILING_SOURCE("MacroNodes.cpp")
#include "MacroNodes.cpp"

OPCOMPILING_SOURCE("Scanner.cpp");
#include "Scanner.cpp"

OPCOMPILING_SOURCE("Timer.cpp");
#include "Timer.cpp"

OPCOMPILING_SOURCE("DialectNodes.cpp");
#include "DialectNodes.cpp"

OPCOMPILING_SOURCE("PrintDialect.cpp");
#include "PrintDialect.cpp"

OPCOMPILING_SOURCE("Globber.cpp")
#include "Globber.cpp"

OPCOMPILING_SOURCE("MemoryTracker.cpp")
#include "MemoryTracker.cpp"

OPCOMPILING_SOURCE("DialectTracker.cpp")
#include "DialectTracker.cpp"

OPCOMPILING_SOURCE("States.cpp")
#include "States.cpp"

OPCOMPILING_SOURCE("ErrorNames.cpp")
#include "ErrorNames.cpp"

OPCOMPILING_SOURCE("main.cpp");
#include "main.cpp"

OPCOMPILING_SOURCE("Demo.cpp");
#include "Demo.cpp"

OPCOMPILING_SOURCE("Blacklist.cpp");
#include "Blacklist.cpp"

OPCOMPILING_SOURCE("Modifiers.cpp");
#include "Modifiers.cpp"

OPCOMPILING_SOURCE_END;


