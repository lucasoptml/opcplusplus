///****************************************************************
/// Copyright (C) 2007 OP Games LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: test.cpp
/// Date: 10/12/2007
///
/// Description:
///
/// test file
///****************************************************************

#include "main.h"

#include "generated/opCpp/Generated.ocppindex"
#include <sstream>

std::stringstream file;

Tester object;

void testload()
{
	for(int i = 0; i < 10; i++)
	{ 
		opcpp::visitors::binary_load_archiver loadvisitor(file);

 		loadvisitor >> object;
	}
}

void testsave()
{
	for(int i = 0; i < 10; i++)
	{ 
		opcpp::visitors::binary_save_archiver savevisitor(file);

 		savevisitor << object;
	}
}