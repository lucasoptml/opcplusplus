#include "main.h"

// Include the generated source index.
// NOTE: Keep the project name and this path in sync.
#include "Generated/TemplatedVisitor/Generated.ocppindex"
#include <conio.h>
#include <iostream>

using namespace opcpp::reflection::types;
using namespace opcpp::base;

class PrintNameVisitor : public visitor_base
{
public:
	
	// Override the visit(...) method.
	virtual void visit(member_info& info)
	{
		// Get member name from member_info accessor.
		cout << info.member_name() << endl;
	}
};

// Entry point into the application.
int main()
{
	Vehicle          vehicle;
	PrintNameVisitor nv;

	vehicle.visit_data_members(nv);

	_getch();	
	
	return 0;
}