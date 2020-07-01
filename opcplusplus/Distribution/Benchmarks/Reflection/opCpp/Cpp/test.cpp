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

using namespace opcpp::accessors;

struct stringvisitor : public opcpp::base::visitor_base
{
	stringvisitor(MyObject& inobj) : o(inobj)
	{
		o.visit_data_members(*this);
	}
	
	void visit(string_info& info)
	{
		o.Blah(info.get_string());
	}

private:
	MyObject& o;
};

MyObject object;

void test()
{
	//perform the benchmark
	stringvisitor visitor(object);
}