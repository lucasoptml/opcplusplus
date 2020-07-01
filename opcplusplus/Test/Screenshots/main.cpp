#include "main.h"

// Include the generated source index.
// NOTE: Keep the project name and this path in sync.
#include "Generated/Screenshots/Generated.ocppindex"
#include <conio.h>
#include <iostream>
#include <vector>

using namespace opcpp;
using namespace opcpp::casting;

// Entry point into the application.
int main()
{
	vector<Actor*>  Objects;

	for (int i = 0; i < 10; i++)
		Objects.push_back(new Actor());

	RenderInterface ri;
	
	/*=== render all renderable objects ===*/
	
	int size = (int) Objects.size();

	for (int i = 0; i < size; i++)
	{
		component::Renderable* r = 
			component_cast<component::Renderable>(Objects[i]);

		if (r)
			r->Render(&ri);
	}

	for (int i = 0; i < size; i++)
		delete Objects[i];

	_getch();

	return 0;
}