/*
classprefix(op) category uclass
{
	location body
	{
		note aaaa;
		note bbbb;
	}
}

verbatim note uclass::body::aaaa()
{
	// aaaa
}

verbatim note uclass::body::bbbb()
{
	// bbbb
}

category uclass 
{
	location body
	{
		after(aaaa)  note cccc;
		before(bbbb) note dddd;
		before(aaaa) datamap xxxx;
		before(xxxx) functionmap yyyy;
		after(dddd)  note wwww;
	}

	location footer
	{
		note fatso;
	}
}

verbatim note uclass::body::cccc()
{
	// cccc
}

verbatim note uclass::body::dddd()
{
	// dddd
}

verbatim note uclass::body::xxxx::start()
{
	// xxxx start
}

verbatim note uclass::body::xxxx::mapping()
{
	// xxxx mapping
}

verbatim note uclass::body::xxxx::end()
{
	// xxxx end
}

verbatim note uclass::body::yyyy::start()
{
	// yyyy start
}

verbatim note uclass::body::yyyy::mapping()
{
	// yyyy mapping
}

verbatim note uclass::body::yyyy::end()
{
	// yyyy end
}

verbatim note uclass::body::wwww()
{
	// wwww
}

verbatim note uclass::footer::fatso(class_name)
{
	// this note's a big footer fatty
	string class_name = ``class_name``;
}

enumeration openum
{
	location body
	{
		note xxxx;
		before(xxxx) note yyyy;
		before(xxxx) enummap flarg;
		note llll;
	}
};

verbatim note openum::body::xxxx()
{
	// xxxx
}

verbatim note openum::body::yyyy()
{
	// yyyy
}

verbatim note openum::body::flarg::start()
{
	// flarg start
}

verbatim note openum::body::flarg::name()
{
	// flarg name
}

verbatim note openum::body::flarg::value()
{
	// flarg value
}

verbatim note openum::body::flarg::end()
{	
	// flarg end
}

verbatim note openum::body::llll()
{
	// llll
}
*/

classprefix(op) category opclass
{
	/*location body
	{
		note aaaa;
		note bbbb;
	}*/
}

structprefix(op) category opstruct
{
	location body
	{
		note x;
		hidden note y;
	}

	location post
	{
		datamap gorm;
	}
};

note opstruct::identifier()
{
	struct
}

note opstruct::body::x()
{
	int w;
}

note opstruct::body::y()
{
	int z;
}

note opstruct::post::gorm::mapping(member_name)
{
	// member_name
	effin a
}

enumprefix(op) enumeration openum
{

};