// The Project Dialect

// opinclude the default opC++ dialect:
opinclude "opc++dialect.doh"

opinclude "visitors/xml_archiver.doh"
opinclude "visitors/text_archiver.doh"
opinclude "visitors/binary_archiver.doh"

// Specify custom dialect settings below:

#include <vector>
#include <list>
#include <deque>
#include <string>
#include <map>
#include <set>

/*! \mainpage Overview
*
* This documentation is designed to give you a basic class reference for the opC++ Standard Dialect.  
* 
* It includes a reference for the reflection and serialization classes.
* 
* See the opC++ manual for documentation on the dialect language and compiler.
* 
* \section Reflection Data Reflection
* 
* Data reflection means you can get information about classes in your program at run-time. 
* It gives you the ability to iterate over an object's data, without needing to know the exact class you're dealing with.
* 
* \subsection Accessors Accessors
*
* Accessors represent the type of a field.  You can use them to learn about a field's type at runtime, 
* as well as manipulate the underlying data.
* They also give you access to metadata about the field.  
*
* \code
* // Get metadata from an accessor.
* void visit(opcpp::accessors::int_info& info)
* {
*     cout << info.get_int() << endl;
* }
* \endcode
*
* A visual overview of accessors can be found <a href="http://www.opcpp.com/?n=Language.Reflection">here</a>.
* 
* \subsection ExternalReflection External Reflection
* 
* If you do not have any instances of the type you want to learn about, you can still use external reflection.
* 
* \code
* // Attempt to get access to a type called "Wooble"
* opcpp::registration::class_type* type = opcpp::registration::class_tracker::get_type("Wooble");
*
* if ( type )
*     cout << "Num Fields: " << type->get_field_count() << endl;
* \endcode
* 
* A visual overview of external reflection can be found <a href="http://www.opcpp.com/?n=Language.Fields">here</a>.
*
* A quick tutorial on external reflection can be found <a href="http://www.opcpp.com/?n=Quick.ExternalReflection">here</a>.
*
* \subsection InstanceReflection Instance Reflection
*
* If you have actual instances of the objects you're interested in, you can query their field's reflected data directly.
*
* \code
* // Search for a specific data member in an instance.
* ProgrammingLanguage pl;
*
* opcpp::registration::class_type& type  = pl.get_type();
* opcpp::fields::data_field*       field = type.get_field("Keywords");
*
* if ( field )
* {
*     vector< string >* keywords = field->get_value< vector< string > >( pl );
*     ...
* }
* \endcode
*
* A quick tutorial on instance reflection can be found <a href="http://www.opcpp.com/?n=Quick.InstanceReflection">here</a>.
*
* \subsection Visitors Visitors
*
* Visitors allow you to perform more complicated reflection routines by visiting members based on type.  
* This is done using the accessors described above.
*
* \code
* // Simple visitor declaration.
* class simple_visitor : public opcpp::base::visitor_base 
* {
*     void visit(opcpp::accessors::member_info& info)
*     {
*         ...
*     }
* };
* \endcode
*
* A quick tutorial on reflection visitors can be found <a href="http://www.opcpp.com/?n=Quick.ReflectionVisitors">here</a>.
*
* \subsection Casting Casting
*
* opC++ has casting functions for accessors, opclasses, and for extracting data.
*
* - opcpp::casting::class_cast : Cast an opclass object to another opclass type.
* - opcpp::accessors::member_info::cast : Cast an accessors data to a specific type.
* - opcpp::fields::data_field::get_value : Cast a fields data to a specific type.
* - opcpp::casting::info_cast : Cast an accessor to another accessor type.
*
* \code
* // class_cast: dynamic_cast replacement.
* opclass A {}
* opclass B : public A {}
*
* A* a = new A();
* B* b = class_cast< B >( a );
*
* if ( b )
*     ...
* \endcode
*
* \section Serialization Serialization
*
* Serialization is the loading and saving of objects to files or memory.
* You can use opC++ archivers to load and save data.
*
* Archivers are simply implementations of opC++ visitors.
*
* \subsection Archivers Archivers
*
* The opC++ Standard dialect implements efficient serialization by using it's reflective visitor capabilities.  
* Archiver classes exist in the opC++ Standard Dialect to load and save data in the following formats:
*
* - Binary
*	- opcpp::visitors::binary_save_archiver : archiver for saving binary
*	- opcpp::visitors::binary_load_archiver : archiver for loading binary
* - Text
*	- opcpp::visitors::text_save_archiver : archiver for saving text
*	- opcpp::visitors::text_load_archiver : archiver for loading text
* - Xml
*	- opcpp::visitors::xml_save_archiver : archiver for saving xml
*	- opcpp::visitors::xml_load_archiver : archiver for loading xml
* 
* \code
* // Save some data to binary.
* vector< string >                      Strings;
* bool                                  bBoolean;
* opcpp::visitors::binary_save_archiver archiver( stream );
*
* archiver << Strings;
* archiver << bBoolean;
* \endcode
* 
* A quick serialization example can be found <a href="http://www.opcpp.com/?n=Quick.Rss">here</a>.
*
*/



