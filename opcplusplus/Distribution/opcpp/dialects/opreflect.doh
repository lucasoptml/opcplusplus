//****************************************************************
// Copyright (C) 2007 OP Games LLC - All Rights Reserved
//
// Authors: Lucas Ellis
//
// File: opreflect.doh
// Date: 12/11/2007
//
// Description:
//
// Adds opreflect data modifier.
//****************************************************************

namespace opcpp
{
	namespace metadata
	{
		//specialized when opreflect is used 
		template<class type>
		struct reflect_name
		{
			static const char* get_name()
			{
				return NULL;
			}
		};
	}
}

//==========================================
// Adds opreflect support.
//==========================================

//NOTE: move to sourceheading
opmacro add_opreflect(category_name)
{
	category category_name
	{
		datamodifier opreflect();

		location sourceheading
		{
			datamap opreflectmap
			{
				is not native;
				is opreflect();
			}
		}
	}

	note category_name::sourceheading::opreflectmap::start()
	{
		namespace opcpp
		+{}
		namespace metadata
		+{}
	}

	note category_name::sourceheading::opreflectmap::mapping(alt_scope,alt_class_name,member_name,opreflect)
	{
			template<>
			struct reflect_name< opcpp::fields alt_scope::alt_class_name::member_name >
			{
				static const char* get_name()
				{
					return opreflect;
				}
			};
	}

	note category_name::sourceheading::opreflectmap::end()
	{
		-{}
		-{}
	}
}