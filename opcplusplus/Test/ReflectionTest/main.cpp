#include "main.h"


#include <iostream>

using namespace std;

using namespace opcpp::accessors;
using opcpp::base::visitor_base;

class identify_visitor : public visitor_base
{
	class inner_container_visitor : public visitor_base
	{
	public:
		inner_container_visitor(value_container_info& c) : container(c)
		{
		}

		void visit(int_info& info)
		{
			//visiting all ints
			for(bool i = container.begin(); i; i = container.increment())
			{
				cout << "inner int : " << info.get_int() << endl;
			}
		}

		//a container of containers - use double inner container visitor to
		//							  minimize type discovery calls
		void visit(value_container_info& info);

	protected:
		value_container_info& container;
	};

	class double_inner_container_visitor : public inner_container_visitor
	{
	public:
		typedef inner_container_visitor super;

		double_inner_container_visitor(value_container_info& o, value_container_info& c)
			:
			outer(o),
			inner_container_visitor(c)
		{
		}
		
		//for each type, you must call the super version.
		void visit(int_info& info)
		{
			for(bool i = outer.begin(); i; i = outer.increment())
			{
				super::visit(info);
			}
		}

		void visit(value_container_info& info)
		{
			for(bool i = container.begin(); i; i = container.increment())
			{
				cout << "inner container" << endl;
				info.get_inner()->visit(*this);
			}
		}

	private:
		value_container_info& outer;
	};

public:

	virtual void visit(basic_info& info)
	{
		//hit a basic type
		cout << info.member_name() << " : " << "basic type" << endl;
	}

	virtual void visit(value_container_info& info)
	{
		inner_container_visitor visit_inner_container(info);

		cout << info.member_name() << " : " << "container" << endl;
		//got a container
		//NOTE: how should we look inside these - what's the quickest method?

		//let the visitor handle the iteration.
		//I want to discover the type (of the inner type), the inner visitor must do the iteration.
		info.get_inner()->visit(visit_inner_container);
		//cast and iterate.
	}
};

inline void identify_visitor::inner_container_visitor::visit(value_container_info& info)
{
	double_inner_container_visitor visitor(container,info);
	cout << "double container" << endl;
	info.get_inner()->visit(visitor);
}

using opcpp::registration::class_tracker;

// Entry point into the application.
int main()
{
	//gotta initialize the tracker first - or casts will fail spectacularly
	class_tracker::initialize();
	
	//testing visit_data_members polymorphic access...
	Containers c;
	

	//an id printing visitor
	identify_visitor id_visitor;
	
	c.visit_data_members(id_visitor);
	
	//testing external accessor access...
	class_type& containerstype = c.get_type();
	
	cout << endl << "printing class " << containerstype.get_name() << " externally." << endl << endl;
	
	size_t numfields = containerstype.get_field_count();
	
	for(size_t i = 0; i < numfields; i++)
	{
		data_field* field = containerstype.get_field(i);
		field->visit(&c,id_visitor);
	}
	

	//testing external string conversions...
	class_type* findtype = class_tracker::get_type("Containers");
	
	data_field* findfield = findtype->get_field("ints");

	findfield->visit(&c,id_visitor);
	
	class_base* object = findtype->new_instance();

	return 0;
}

// Include the generated source index.
// NOTE: Keep the project name and this path in sync.
#include "Generated/ReflectionTest/Generated.ocppindex"
