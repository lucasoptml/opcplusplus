///****************************************************************
/// Copyright (C) 2007 OP Games LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: main.h
/// Date: 10/12/2007
///
/// Description:
///
/// Benchmark Header
///****************************************************************

#include <Windows.h>
#include <iostream>

using namespace std;

class opTimer
{
public:
	opTimer()
	{
		InitTimeSeconds();
	}

	double GetTimeSeconds()
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);

		return ((double)(time.QuadPart))*invtimerfrequency;
	}

private:
	void InitTimeSeconds()
	{
		LARGE_INTEGER timerfrequency;
		QueryPerformanceFrequency(&timerfrequency);

		invtimerfrequency = (1.0)/((double)timerfrequency.QuadPart);
	}

private:
	double invtimerfrequency;
};
