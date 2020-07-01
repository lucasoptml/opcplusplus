// The Project Dialect

// Includes.
#include <iostream>

using namespace std;

///==========================================
/// autoclass
///==========================================

// Create a new category called autoclass.
category autoclass
{
	/*=== getclassname note ===*/

	// Let's add a new generated method
	// to this category, GetClassName().
	// This will return the name of some 
	// autoclass.  We also specify that 
	// the note will be generated in the 
	// 'body' section of the generated code.
	location body
	{
		note getclassname;
	}

	/*=== log data modifier ===*/

	// Now let's declare a new data modifier
	// for logging the names of variables and
	// their values.
	datamodifier log;

	// We want the generated code to go in the body.
	location body
	{
		hidden datamap logmap
		{
			is log;
		}
	}

	/*=== privatemembermap note ===*/

	// Now let's generate a method that uses a map
	// to print the names of all private data members.
	location body
	{
		// Our datamap is applied to all members
		// that have private visibility.
		datamap privatemembermap
		{
			is private;
		}
	}
};

/*=== getclassname note code ===*/

// Here we actually define the 'getclassname' note.
// We pass in one of the many available attributes,
// in this case, the name of the autoclass.
note autoclass::body::getclassname(class_name)
{
// We want this method to be public.
public:

	// Define the function here.
	const char* GetClassName()
	{
		return ``class_name``;
	}
}

/*=== logmap note code ===*/

// This note represents the start of the 
// logmap datamap.  We define the signature
// and first open brace of a method named
// 'LogData()'.
note autoclass::body::logmap::start()
{
// We want this method to be public.
public:

	// Start the method.
	void Log()
	+{}
}

// When opCpp is generating code, this note gets
// called once for each data member in an autoclass.
// This will work for every data member supported by
// cout.  For more complicated output, you should use
// something more robust like a templated logging class.
note autoclass::body::logmap::mapping(member_name)
{
	cout << ``member_name: `` << member_name << endl;
}

// Here we put the final ending brace on the 
// 'LogData()' method.
note autoclass::body::logmap::end()
{
	// End the method.
	-{}
}

/*=== privatemembermap note code ===*/

// The start of our method to print private members.
note autoclass::body::privatemembermap::start()
{
// We want this method to be public.
public:

	// Start the method.
	void PrintPrivateMembers()
	+{}
}

// This is called by opCpp, once for each data member.
note autoclass::body::privatemembermap::mapping(member_name)
{
	cout << "Name: " << ``member_name`` << endl;
}

// End the method.
note autoclass::body::privatemembermap::end()
{
	-{}
}