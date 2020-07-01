#include "main.h"

// Include the generated source index.
// NOTE: Keep the project name and this path in sync.
#include "Generated/Auto Functions/Generated.ocppindex"
#include <conio.h>

// Entry point into the application.
int main()
{
	// Declare a new 'Ball' instance.
	Ball b;

	// Output the name of the autoclass.
	cout << "Name: " << b.GetClassName() << endl << endl;

	// Now let's test out the logmap datamap we created
	// in the autoclass dialect.  This will log all data
	// members to the standard out that have the 'log'
	// data modifier in front of them (Weight and Diameter).
	cout << "Members:" << endl << endl;

	b.Log();

	// Now, test the privatemembermap datamap that prints
	// out all private members.
	cout << endl << "Private Members: " << endl << endl;

	b.PrintPrivateMembers();

	_getch();
	
	return 0;
}