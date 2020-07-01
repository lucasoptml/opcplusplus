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


struct baseclass
{
	virtual void blah()
	{}
};

//1
struct A : baseclass
{
	
};

//2
struct AA : A
{

};

struct AB : A
{

};

//4
struct AAA : AA
{

};

struct AAB : AA
{

};

struct ABB : AB
{

};

struct ABA : AB
{

};

//8
struct AAAA : AAA
{

};

struct AABA : AAB
{

};

struct ABBA : ABB
{

};

struct ABAA : ABA
{

};

struct AAAB : AAA
{

};

struct AABB : AAB
{

};

struct ABBB : ABB
{

};

struct ABAB : ABA
{

};

A* structs[] = 
{
	new A(),
	new AA(),
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
		if(AA* aa = dynamic_cast<AA*>(structs[i]))
			aa->blah();
		if(AB* ab = dynamic_cast<AB*>(structs[i]))
			ab->blah();
	}
}