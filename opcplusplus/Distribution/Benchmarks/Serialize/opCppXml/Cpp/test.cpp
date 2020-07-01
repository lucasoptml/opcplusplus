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

#include "generated/opCpp/Generated.oohindex"
#include "generated/opCpp/Generated.ocppindex"

#include <sstream>

std::stringstream file[10];

void testload()
{
	Tester object;

	for(int i = 0; i < 10; i++)
	{ 
		opcpp::visitors::xml_load_archiver loader(file[i],"Tester");
		
		loader.load(object);
	}
}

void testsave()
{
	Tester object;

	for(int i = 0; i < 10; i++)
	{ 
		opcpp::visitors::xml_save_archiver saver(file[i],"Tester");

		saver.save(object);
	}
}