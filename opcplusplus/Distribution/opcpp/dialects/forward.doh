//****************************************************************
// Copyright (C) 2007 OP Games LLC - All Rights Reserved
//
// Authors: Lucas Ellis
//
// File: forward.doh
// Date: 12/11/2007
//
// Description:
//
// Forward declarations.
//****************************************************************

/// \brief Root namespace, contains all opC++ related classes.
namespace opcpp
{
	/// \brief Contains all the different data accessor classes.
	namespace accessors
	{
		
	}
	
	/// \brief Base classes used by the opC++ Standard Dialect.
	namespace base
	{
		class  class_base;
		struct struct_base;
		class  visitor_base;
	}
	
	/// \brief Casting functions for opclass, accessors.
	namespace casting
	{
		
	}
	
	/// \brief Data-field classes used by opC++.
	namespace fields
	{
		struct data_field;
	}
	
	/// \brief External type reflection classes for opclass and opstruct.
	namespace registration
	{
		
	}
	
	/// \brief Archiver classes for reflected types.
	namespace visitors
	{

	}
}

//extension for forward declaring types.
extension forward_declare;

namespace opcpp
{
	extensionpoint forward_declare

}

// special defines
// opdefine OPCPP_STRCMP(a,b)
// {
// 	strcmp(a,b)
// }
