#include "main.h"

// Include the generated source index.
// NOTE: Keep the project name and this path in sync.
#include "Generated/Migration/Generated.ocppindex"
#include <iostream>
#include <conio.h>

using namespace std;

// Entry point into the application.
int main()
{
	/*=== we declare an instance of A throughout the code migration ===*/

	cpp::A a1;

	cout << "c++: name = " << a1.GetClassName() << ", size = " << a1.GetClassSize() << endl << endl; 

	preprocessor::A a2;

	cout << "preprocessor: name = " << a2.GetClassName() << ", size = " << a2.GetClassSize() << endl << endl;

	op_define::A a3;

	cout << "opdefine: name = " << a3.GetClassName() << ", size = " << a3.GetClassSize() << endl << endl;

	op_macro::A a4;

	cout << "opmacro: name = " << a4.GetClassName() << ", size = " << a4.GetClassSize() << endl << endl;

	automatic::A a5;

	cout << "automatic: name = " << a5.GetClassName() << ", size = " << a5.GetClassSize() << endl << endl;

	_getch();

	return 0;
}