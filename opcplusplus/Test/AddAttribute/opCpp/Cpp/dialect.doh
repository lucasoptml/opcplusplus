///****************************************************************
/// Copyright (C) 2007 OP Games LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: dialect.doh
/// Date: 10/12/2007
///
/// Description:
///
/// dialect
///****************************************************************


opinclude "opc++dialect.doh"

opmacro add_value_attribute(category_name,type,modifier_name,get_name,default,regex)
{
	namespace opcpp
	{
		namespace metadata
		{
			template<class field>
			struct get_name
			{
				static type get()
				{
					return default;
				}
			};
		}
	}

	category category_name
	{
		datamodifier modifier_name();

		location footer
		{
			datamap modifier_name
			{
				is modifier_name(regex);
			}
		}
	};

	//dumb, it shouldn't need to be in the header.
	note opclass::footer::modifier_name::mapping(scope,alt_class_name,member_name,modifier_name)
	{
		namespace opcpp
		{
			namespace metadata
			{
				template<>
				struct get_name< scope::opcpp::fields::alt_class_name::member_name >
				{
					static type get()
					{
						return modifier_name;
					}
				};
			}
		}
	}

	extendpoint data_field_defaults
	{
		modifier_name = default;
	};


	extendpoint data_field_values
	{
		modifier_name = opcpp::metadata::get_name< mytype >::get();
	};


	extendpoint data_field_members
	{
	protected:
		type modifier_name;

	public:
		type get_name()
		{
			return modifier_name;
		}
	};
}

expand add_value_attribute(opclass,const char*,xml,get_xml,0,"\".*\"")

expand add_value_attribute(opclass,const char*,var,get_var,0,"\".*\"")

expand add_value_attribute(opclass,const char*,native,get_native,0,"\".*\"")

