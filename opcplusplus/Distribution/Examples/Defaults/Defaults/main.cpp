#include "main.h"

// Include the generated source index.
// NOTE: Keep the project name and this path in sync.
#include "Generated/Defaults/Generated.ocppindex"
#include <conio.h>

// Entry point into the application.
int main()
{
	// Run this in debug mode and watch all the variables.
	// They will all be set correctly to their defaults.
	// This includes all the basic types we wrote specializations
	// for, all pointers, and all fish (or other categories with
	// the "defaults" feature).
	Goldfish g;
	Whale*   w = new Whale();

	delete w;

	_getch();
	
	return 0;
}