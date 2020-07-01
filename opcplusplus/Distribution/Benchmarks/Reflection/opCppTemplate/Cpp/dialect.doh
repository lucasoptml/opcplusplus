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

//initialize the data
category class
{
	location body
	{
		datamap init
		{
			is data_initialized();
		}
	}
};

note class::body::init::start(class_name)
{
public:
	class_name()
	+{}
}

note class::body::init::mapping(member_name,data_initialized)
{
		member_name = data_initialized;
}

note class::body::init::end()
{
	-{}
}

//run through stuffs
category class
{
	location body
	{
		datamap visit;
		note callblah;
	}
};

note class::body::callblah()
{
	template<class type>
	void callblah(const type& t)
	{
	}

	template<>
	void callblah<string>(const string& t)
	{
		Blah(t);
	}
}

note class::body::visit::start()
{
public:
	void visit()
	+{}
}

note class::body::visit::mapping(member_name)
{
	callblah(member_name);
}

note class::body::visit::end()
{
	-{}
}
