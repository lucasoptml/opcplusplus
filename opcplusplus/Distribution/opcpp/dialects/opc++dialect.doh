//****************************************************************
// Copyright © 2008 opGames LLC - All Rights Reserved
//
// Authors: Lucas Ellis
//
// File: opc++dialect.oh
// Date: 01/20/2007
//
// Description:
//
// opC++ Standard Dialect
//****************************************************************

code heading
{
	//guards for dialect heading support code.
	//NOTE: can we make this automatic? likely.
	#ifndef OPCPP_STANDARD_DIALECT
	#define OPCPP_STANDARD_DIALECT

	//NOTE: opC++ Standard Dialect depends on std::string
	#include <string>
}

opdefine NULL
{
	0
}

code heading
{
	//define OPCPP_API to get dll-linkage (msvc)
	#ifndef OPCPP_API
		#define OPCPP_API
	#endif
}

// Adds an extension point for
// extending opcpp namespace
extension opcpp_namespace;

namespace opcpp
{
	extensionpoint opcpp_namespace;
}

//==========================================
// forward declarations
//==========================================

opinclude "forward.doh"

//==========================================
// support metafunction code
//==========================================

opinclude "metafunction.doh"

//==========================================
// restriction (disallows) on standard 
// dialect types
//==========================================

opinclude "restrictions.doh"

//==========================================
// metadata support code
//==========================================

opinclude "metadata.doh"

//==========================================
// opclass support
//==========================================

opinclude "opclass.doh"

//==========================================
// opstruct
//==========================================

opinclude "opstruct.doh"

//==========================================
// openum Type
//==========================================

opinclude "openum.doh"

//==========================================
// visitors
//==========================================

opinclude "visitors.doh"

//==========================================
// casting support
//==========================================

opinclude "casting.doh"

//==========================================
// accessor stuff
//==========================================

opinclude "accessors.doh"

// add accessors to opclass
expand add_accessors(opclass);

// add accessors to opstruct
expand add_accessors(opstruct);

//==========================================
// opreflect support
//==========================================

opinclude "opreflect.doh"

// add opreflect for opclass
expand add_opreflect(opclass);

// add opreflect for opclass
expand add_opreflect(opstruct);

//==========================================
// opclass transient support
//==========================================

opinclude "transient.doh"

//add transient to opclass
expand add_transient(opclass);

//add transient to opstruct
expand add_transient(opstruct);

//==========================================
// fields
//==========================================

opinclude "fields.doh"

//add member_data for opclass
expand add_fields(opclass);

//add member_data for opstruct
expand add_fields(opstruct);

//==========================================
// registration support
//==========================================

opinclude "registration.doh"

// register opclass
expand add_registration(opclass,class_type,class_base,get_class_type);

// register opstruct
expand add_registration(opstruct,struct_type,struct_base,get_struct_type);

//==========================================
// accessors
//==========================================

//add member_info accessor support header
opinclude "memberinfo.doh"

//add stl support to member_info accessors
opinclude "stlsupport.doh"

//==========================================
// opstatic support
//==========================================

opinclude "opstatic.doh"

// add opstatic to opclass
expand add_opstatic(opclass);

// add opstatic to opstruct
expand add_opstatic(opstruct);

code heading
{
	#endif //OPCPP_STANDARD_DIALECT
}

