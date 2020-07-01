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
	
	for(int i = 0; i < 1000000; i++)
		test();
		
	double end = timer.GetTimeSeconds();

	cout << "Reflection / opC++ Compile Time : ";
	cout << (end-start)*1000.0 << "ms" << endl;

	return 0;
}


