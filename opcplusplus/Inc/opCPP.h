///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: opCPP.h
/// Date: 11/26/2006
///
/// Description:
///
/// opC++ base header.
///****************************************************************

#ifndef __OPCPP_H__
#define __OPCPP_H__

// other includes
#include <fstream>
#include <iomanip>
#include <time.h>

// configuration related
#include "Config.h"

// opstl
#include "opstl.h"

// boost
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/exception.hpp"
#include "boost/filesystem/convenience.hpp"
#include "boost/filesystem/fstream.hpp"
using boost::filesystem::path;

// md5 stuff..
#include "opmd5.h"

// opCPP includes
#include "opTime.h"
#include "Macros.h"
#include "Platforms.h"
#include "File.h"
#include "Namespaces.h"
#include "Delegates.h"
#include "MemoryTracker.h"
#include "Parameters.h"
#include "Stream.h"
#include "Demo.h"
#include "Exceptions.h"
#include "Tokens.h"
#include "opLog.h"
#include "Beta.h"
#include "Version.h"
#include "opError.h"
#include "Scanner.h"
#include "UtilityInterfaces.h"
#include "SymbolTracker.h"
#include "Interfaces.h"
#include "StatementInterfaces.h"
#include "DialectInterfaces.h"
#include "DialectStatementInterfaces.h"
#include "Contexts.h"
#include "Visitors.h"
#include "opNode.h"
#include "FileNodes.h"
#include "DialectTracker.h"
#include "BasicNodes.h"
#include "DialectNodes.h"
#include "ModifierNodes.h"
#include "DialectModifierNodes.h"
#include "ExtensionNodes.h"
#include "MacroNodes.h"
#include "StatementNodes.h"
#include "DialectStatementNodes.h"
#include "opNodeInlines.h"
#include "Paths.h"
#include "Driver.h"
#include "Inlines.h"
#include "Globber.h"
#include "Mappings.h"
#include "Modifiers.h"
#include "Timer.h"
#include "InterfaceInlines.h"
#include "RegexSupport.h"
#include "MacroInterfaces.h"
#include "Walkers.h"
#include "DialectVisitors.h"
#include "CodeVisitors.h"

#endif//__OPCPP_H__


