//****************************************************************
// Copyright (C) 2007 OP Games LLC - All Rights Reserved
//
// Authors: Lucas Ellis
//
// File: transient.doh
// Date: 12/11/2007
//
// Description:
//
// Transient modifier definition.
//****************************************************************

namespace opcpp
{
	namespace metadata
	{
		template<typename t>
		struct is_transient
		{
			enum { value = false };
		};
	}
}

//NOTE: move to sourceheading
opmacro add_transient(category_name)
{
	category category_name
	{
		description("Data member should not be serialized, and the field is marked transient.")
		datamodifier transient;

		location sourceheading
		{
			datamap transient
			{
				is transient;
			}
		}
	};

	note category_name::sourceheading::transient::start()
	{
		namespace opcpp
		+{}
		namespace metadata
		+{}
	}

	note category_name::sourceheading::transient::mapping(alt_scope,class_name,member_name,alt_class_name)
	{
		template<>
		struct is_transient< opcpp::fields alt_scope::alt_class_name::member_name >
		{
			enum { value = true };
		};
	}

	note category_name::sourceheading::transient::end()
	{
		-{}
		-{}
	}
}

