///****************************************************************
/// Copyright (C) 2007 OP Games LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: main.cpp
/// Date: 10/12/2007
///
/// Description:
///
/// Benchmark Source
///****************************************************************

#include "main.h"

void test();

int main()
{
	opTimer timer;

	double start = timer.GetTimeSeconds();
	
	test();
		
	double end = timer.GetTimeSeconds();

	cout << (end-start)*1000.0 << "ms" << endl;

	return 0;
}


