///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Exceptions.cpp
/// Date: 01/11/2007
///
/// Description:
///
/// Exception-related source code.
///****************************************************************

#include "opCPP.h"

int opException::ExceptionCount = 0;

opException::ExceptionType opException::ExceptionOverride = opException::DefaultException;

