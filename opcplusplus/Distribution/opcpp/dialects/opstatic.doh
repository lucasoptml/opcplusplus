//****************************************************************
// Copyright (C) 2007 OP Games LLC - All Rights Reserved
//
// Authors: Lucas Ellis
//
// File: opstatic.doh
// Date: 08/09/2007
//
// Description:
//
// opstatic support
//****************************************************************

opmacro add_opstatic(category_name)
{
	category category_name
	{
		datamodifier opstatic;

		error("initialization is supported only on static and opstatic")
		disallow Statics
		{
			is data_initialized();
			is not static;
			is not opstatic;
		}

		error("static initialization must have a value")
		disallow StaticValue
		{
			is data_initialized("[ \t]*");
		}

		error("static and opstatic not allowed together")
		disallow NoStaticOpstatic
		{
			is static;
			is opstatic;
		}

		location source
		{
			datamap opstatic
			{
				is opstatic;
				is not data_initialized();
			}

			datamap opstatic_init
			{
				is opstatic;
				is data_initialized();
			}
		}
	};

	note category_name::source::opstatic::mapping(scope,member_name,class_name,data_type)
	{
		data_type (scope::class_name::member_name);
	}

	note category_name::source::opstatic_init::mapping(member_name,scope,class_name,data_type,data_initialized)
	{
		data_type (scope::class_name::member_name) = data_initialized;
	}
}


