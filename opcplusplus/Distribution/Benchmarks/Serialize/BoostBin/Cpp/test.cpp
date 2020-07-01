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


#include "generated/Cpp/Generated.oohindex"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "boost/archive/polymorphic_text_oarchive.hpp"
#include "boost/archive/polymorphic_binary_oarchive.hpp"
#include "boost/archive/polymorphic_text_iarchive.hpp"
#include "boost/archive/polymorphic_binary_iarchive.hpp"

#include <boost/serialization/vector.hpp>

#include <sstream>

std::stringstream ofs;

const Tester tc;

Tester t;

void testpoly()
{
	for(int i = 0; i < 10; i++)
	{
		boost::archive::polymorphic_binary_oarchive oa(ofs);
		oa << tc;
	}
}
		
void testload()
{
	for(int i = 0; i < 10; i++)
	{
		boost::archive::binary_iarchive oa(ofs);
		oa >> t;
	}
}

void testloadpoly()
{
	for(int i = 0; i < 10; i++)
	{
		boost::archive::polymorphic_binary_iarchive oa(ofs);
		oa >> t;
	}
}

void test()
{
	for(int i = 0; i < 10; i++)
	{
		boost::archive::binary_oarchive oa(ofs);
		oa << tc;
	}
}


