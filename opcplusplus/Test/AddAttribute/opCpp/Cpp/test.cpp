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

void test()
{
	

	opcpp::registration::class_type* type = opcpp::registration::class_tracker::get_type("XmlThing");
	opcpp::fields::data_field* field = type->get_field("a");

	const char* xmlvalue	= field->get_xml();
	const char* var			= field->get_var();
	const char* native		= field->get_native();

	xmlvalue = xmlvalue;

}