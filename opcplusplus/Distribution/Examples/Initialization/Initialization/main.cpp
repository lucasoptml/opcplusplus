#include "main.h"

// Include the generated source index.
// NOTE: Keep the project name and this path in sync.
#include "Generated/Initialization/Generated.ocppindex"
#include <conio.h>

// Entry point into the application.
int main()
{
	// Let's declare a pear.
	Pear p;

	// If we print out the pear's values, note that
	// they're initialized to the values they're assigned
	// to when they're declared.  This happens because 
	// the generated Defaults() method is called in Pear's
	// constructor.
	cout << "Members:" << endl << endl;

	p.Log();

	// Now let's make the apple soggier and relog.
	// We can do this because Sogginess is public.

	p.Sogginess = 40;

	cout << endl << "Members: " << endl << endl;

	p.Log();

	// Now reset to the defaults and relog.

	p.Defaults();

	cout << endl << "Members: " << endl << endl;

	p.Log();

	_getch();
	
	return 0;
}