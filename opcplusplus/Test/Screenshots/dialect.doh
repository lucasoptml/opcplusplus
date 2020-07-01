// The Project Dialect

// opinclude the default opC++ dialect:
opinclude "opc++dialect.doh"

// Specify custom dialect settings below:

category fruit
{
	datamodifier log();
};

category fruit
{
	datamodifier xml();
};

category fruit
{
	location body
	{
		datamap somemap
		{
			is xml();
		}

		note    super;
		note    getclassname;
	}
};

note fruit::body::somemap::mapping(data_type,xml)
{

}

note fruit::body::super(parent_name)
{
	typedef parent_name Super;
}

note fruit::body::getclassname(class_name)
{
	const char* GetClassName() const
	{
		return ``class_name``;
	}
}



















category opclass
{
	/*=== Force member names to be capitalized. ===*/

	error("Members must be capitalized.")
	disallow capitalizemembers
	{
		is data_statement or function_statement;
		is not member_name("[A-Z].*");
	}
};