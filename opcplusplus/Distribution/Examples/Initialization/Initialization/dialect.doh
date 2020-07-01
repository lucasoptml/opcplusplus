// The Project Dialect

// Includes.
#include <iostream>

using namespace std;

///==========================================
/// fruit
///==========================================

// Here we declare a simple fruit category.
// In this example we will show how to implement 
// some simple initialization maps via opCpp
// initializers.
category fruit
{
	/*=== initializermap ===*/

	// Let's create a datamap for all data members
	// that have an initializer attached to them.
	location body
	{
		datamap defaultsmap
		{
			// If a data member is initialized in the fruit,
			// (e.g., int x = 4;), then we can use the special
			// data_initialized automatic modifier to filter the 
			// data members so we only get the initialized ones.
			// Note: data_initialized is a valued modifier, so we
			// need to add the parentheses.
			is data_initialized();
		}
	}

	/*=== logmap ===*/

	// Let's create a simple log map to print out the
	// names of the data members and their values.
	location body
	{
		datamap logmap
		{
		}
	}
};

/*=== defaultsmap note code ===*/

// Begin the defaults method.
note fruit::body::defaultsmap::start()
{
// We want this method to be public.
public:

	void Defaults()
	+{}
}

// Here we define the actual mapping for each data member that has the
// data_initialized automatic valued modifier attached to it.  By passing
// in the data_initialized automatic modifier into the note as a note argument,
// we substitute in the initialized value.
//
// What really happens is..
//
// int x = 1 + 2; --> data_initialized(1 + 2) int x;
//
// However, data_initialized is an automatic modifier, so the opCpp compiler makes
// this substitution automatically.
note fruit::body::defaultsmap::mapping(member_name, data_initialized)
{
	member_name = data_initialized;
}

// End the defaults method.
note fruit::body::defaultsmap::end()
{
	-{}
}

/*=== logmap note code ===*/

// Begin the log method.
note fruit::body::logmap::start()
{
// We want this method to be public.
public:

	void Log()
	+{}
}

// Here we print out all data members that
// are supported by cout.
note fruit::body::logmap::mapping(member_name)
{
	cout << ``member_name = `` << member_name << endl;
}

// End the log method.
note fruit::body::logmap::end()
{
	-{}
}