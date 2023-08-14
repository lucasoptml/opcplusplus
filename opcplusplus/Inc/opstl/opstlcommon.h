//****************************************************************
// Copyright © 2008 opGames Inc. - All Rights Reserved
//
// Authors: Kevin Depue & Lucas Ellis
//
// File: opstlcommon.h
// Date: 04/17/2007
//
// Description:
//
// Common macros/initialization for all opstl containers.
//****************************************************************

#ifndef __OPSTL_COMMON__
#define __OPSTL_COMMON__

// If we're compiling on Linux, figure
// out which compiler.
#if defined(__ICC)
	#define OPSTL_INTEL_LINUX
#elif defined(__GNUG__)
	#define OPSTL_GNU_LINUX
#endif

// Define overridable default allocator.
#ifndef OPSTL_ALLOCATOR
	#define OPSTL_ALLOCATOR(typ) std::allocator< typ >
#endif

#ifndef OPSTL_SET_ALLOCATOR
	#define OPSTL_SET_ALLOCATOR(typ) OPSTL_ALLOCATOR(typ)
#endif

#ifndef OPSTL_ARRAY_ALLOCATOR
	#define OPSTL_ARRAY_ALLOCATOR(typ) OPSTL_ALLOCATOR(typ)
#endif

#ifndef OPSTL_DEQUE_ALLOCATOR
	#define OPSTL_DEQUE_ALLOCATOR(typ) OPSTL_ALLOCATOR(typ)
#endif

#ifndef OPSTL_LIST_ALLOCATOR
	#define OPSTL_LIST_ALLOCATOR(typ) OPSTL_ALLOCATOR(typ)
#endif

#ifndef OPSTL_STRING_ALLOCATOR
	#define OPSTL_STRING_ALLOCATOR(typ) OPSTL_ALLOCATOR(typ)
#endif

#ifndef OPSTL_MAP_ALLOCATOR
	#define OPSTL_MAP_ALLOCATOR(typ) OPSTL_ALLOCATOR(typ)
#endif

// Every container uses these.
#include <iterator>
#include <algorithm>

using namespace std;

// Declare the opstl namespace and use it.
namespace opstl {}

using namespace opstl;

// Include iterator extensions.
#include "opInterfaces.h"

#endif // end __OPSTL_COMMON__

