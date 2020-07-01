//****************************************************************
// Copyright © 2008 opGames LLC - All Rights Reserved
//
// Authors: Lucas Ellis
//
// File: accessors.doh
// Date: 12/11/2007
//
// Description:
//
// Accessor stuff.
//****************************************************************

//this adds data member accessors
opmacro add_accessors(category_name)
{
	category category_name
	{
		description("Native removes a member from reflection, and should be used whenever possible to cut down on code bloat.")
			datamodifier native;

		location body
		{
			note data_members;
		}	
		
		location sourceheading
		{
			datamap accessors
			{
				//is not static; //NOTE: static can have reflection generated, but usually you just want native static!
				is not native;
			}
		}
	};

	note category_name::body::data_members()
	{
	public:
		void visit_data_members(opcpp::base::visitor_base& visitor);
	}

	//create the member infos
	note category_name::sourceheading::accessors::start(alt_class_name,alt_namespace_start)
	{
		
		namespace opcpp
		+{}
		namespace reflection
		+{}

		alt_namespace_start

		namespace alt_class_name
		+{}
	}

	note category_name::sourceheading::accessors::mapping(member_name,data_full_type)
	{
		typedef ::opcpp::accessors::selector< data_full_type >::type member_name;
	}

	note category_name::sourceheading::accessors::end(alt_namespace_end)
	{
		-{}
		-{}

		alt_namespace_end
			
		-{}

	}

} // end of add_data_members