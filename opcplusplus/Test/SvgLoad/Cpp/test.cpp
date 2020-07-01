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

#include <fstream>
#include <sstream>

using std::ifstream;

void testload()
{
	SvgReader::SvgObject object;
	
	ifstream file("level.svg",ifstream::binary);
	
	if(file.is_open())
	{
		opcpp::visitors::XmlLoadVisitor loadvisitor(file,&object,"svg");
	}
	
	object.Parse();

	list< SvgReader::SvgPath* > paths;

	object.GetLabelPaths("bubble",paths);
}

void testsave()
{
	
	
}


