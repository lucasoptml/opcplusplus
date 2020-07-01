//****************************************************************
// Copyright (C) 2007 OP Games LLC - All Rights Reserved
//
// Authors: Lucas Ellis
//
// File: restrictions.doh
// Date: 10/21/2007
//
// Description:
//
// Current Restrictions On Usage
//****************************************************************

//
// Disallows
//

category opclass
{
	//we dont want to allow starting underscores, because
	//it can interfere with the name mangling potentially
	disallow Underscores
	{
		is class_name("_.*")
		or member_name("_.*");
	};

	//NOTE: maybe not the best pattern
	//we only support one and two dimensional arrays currently.
	error("only one and two dimensional arrays supported")
	disallow UnsupportedDimensions
	{
		is data_array_brackets("(\[[^\[]*){3,}");
	}
};

//we dont want to allow starting underscores, because
//it can interfere with the name mangling potentially
enumeration openum
{
	disallow Underscores
	{
		is enum_type("_.*");
	};
};

//we dont want to allow starting underscores, because
//it can interfere with the name mangling potentially
category opstruct
{
	disallow Underscores
	{
		is class_name("_.*");
	};
};

//NOTE: on adding support for these:
//		where are the issues currently?
//		can we reorder to get around this?
//		can we add a new section to get around this?
//		do we need to use proxy classes to get around this (I hope not.)

//gradually remove these restrictions with proper testing!
enumeration openum
{
	error("inner openum not currently supported - but will be soon!")
	disallow InnerType
	{
		is inner_type;
	}
};

category opstruct
{
// 	error("inner opstruct not currently supported - but will be soon!")
// 	disallow InnerType
// 	{
// 		is inner_type;
// 	}
};

category opclass
{
	error("inner opclass not currently supported - but will be soon!")
	disallow InnerType
	{
		is inner_type;
	}
};

