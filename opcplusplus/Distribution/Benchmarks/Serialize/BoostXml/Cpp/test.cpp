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

#include "boost/archive/xml_iarchive.hpp"
#include "boost/archive/xml_oarchive.hpp"

#include <boost/serialization/vector.hpp>

#include <sstream>


std::stringstream ofs;


void testload()
{
	for(int i = 0; i < 10; i++)
	{
		Tester t;	
		boost::archive::xml_iarchive oa(ofs);
		oa >> BOOST_SERIALIZATION_NVP(t);
	}
}

void test()
{
	const Tester t;	

	for(int i = 0; i < 10; i++)
	{
		boost::archive::xml_oarchive oa(ofs);
		oa << BOOST_SERIALIZATION_NVP(t);
	}
}



