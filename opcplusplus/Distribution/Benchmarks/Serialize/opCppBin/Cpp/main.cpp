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

void testsave();
void testload();

int main()
{
	{
		opTimer timer;

		double start = timer.GetTimeSeconds();

		testsave();

		double end = timer.GetTimeSeconds();

		cout << "Serialize Binary Save / opC++ : " << (end-start)*1000.0 << "ms" << endl;

	}

	{

		opTimer timer;

		double start = timer.GetTimeSeconds();

		testload();

		double end = timer.GetTimeSeconds();

		cout << "Serialize Binary Load / opC++ : " << (end-start)*1000.0 << "ms" << endl;

	}

	return 0;
}


