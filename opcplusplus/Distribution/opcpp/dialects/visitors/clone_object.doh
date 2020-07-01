//****************************************************************
// Copyright (C) 2007 OP Games LLC - All Rights Reserved
//
// Authors: Kevin Depue & Lucas Ellis
//
// File: binaryserialize.doh
// Date: 10/30/2007
//
// Description:
//
// Binary Load/Save Visitors for opC++
//****************************************************************

//Requires: opC++ Standard Dialect
//			opinclude "opc++dialect.doh" first!
//
//Usage:
//		Dialect:
//			opinclude "visitors/clone_object.doh"
//		Usage:
//			opcpp::functions::clone_object(opclass)


#ifndef OPCPP_CLONE_OBJECT_DOH
#define OPCPP_CLONE_OBJECT_DOH

#include <sstream>

opinclude "binary_archiver.doh"

namespace opcpp
{
	namespace functions
	{
		template<class type>
		inline type* clone_object(type* object)
		{
			if(!object)
				return NULL;

			//can only clone objects that are serializable
			if(!object->get_type().is_serializable())
				return NULL;

			//first save the object to stream
			std::stringstream stream;
			opcpp::visitors::binary_save_archiver(stream,object);

			//now create the new object
			type* newobject = opcpp::casting::class_cast<type>(object->get_type().new_instance());

			if(!newobject)
				return NULL;

			opcpp::visitors::binary_load_archiver(stream,newobject);

			return newobject;
		}
	}
}

#endif //OPCPP_CLONE_OBJECT_DOH





