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


A* opclasses[] = 
{
	new A(),
	new AA(),
	new AB(),
	new AAA(),
	new ABA(),
	new AAB(),
	new ABB(),
	new AAAA(),
	new ABAA(),
	new AABA(),
	new ABBA(),
	new AAAB(),
	new ABAB(),
	new AABB(),
	new ABBB()
};

void test()
{
	for(int i = 0; i < 15; i++)
	{
		if(AA* aa = opcpp::casting::class_cast<AA>(opclasses[i]))
			aa->blah();
		if(AB* ab = opcpp::casting::class_cast<AB>(opclasses[i]))
			ab->blah();
	}
}