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
void testpoly();
void testload();
void testloadpoly();

int main()
{
	{
		opTimer timer;
	
		double start = timer.GetTimeSeconds();
		
		test();
			
		double end = timer.GetTimeSeconds();

		cout << "Serialize Binary Save / Boost: " << (end-start)*1000.0 << "ms" << endl;
	}


	{
		opTimer timer;

		double start = timer.GetTimeSeconds();

		testpoly();

		double end = timer.GetTimeSeconds();

		cout << "Serialize Binary Save / Poly Boost: " << (end-start)*1000.0 << "ms" << endl;
	}

	{
		opTimer timer;

		double start = timer.GetTimeSeconds();

		testload();

		double end = timer.GetTimeSeconds();

		cout << "Serialize Binary Load / Boost: " << (end-start)*1000.0 << "ms" << endl;
	}


	{
		opTimer timer;

		double start = timer.GetTimeSeconds();

		testloadpoly();

		double end = timer.GetTimeSeconds();

		cout << "Serialize Binary Load / Poly Boost: " << (end-start)*1000.0 << "ms" << endl;
	}


	return 0;
}

#include "generated/Cpp/generated.ocppindex"
