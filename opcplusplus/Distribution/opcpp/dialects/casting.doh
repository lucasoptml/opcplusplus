//****************************************************************
// Copyright (C) 2007 OP Games LLC - All Rights Reserved
//
// Authors: Lucas Ellis
//
// File: casting.doh
// Date: 07/25/2007
//
// Description:
//
// Casting support
//****************************************************************

//==========================================
// class_cast
//==========================================

namespace opcpp
{
	namespace casting
	{
		namespace detail
		{
			struct cast_accessor
			{
				static int get_id(opcpp::base::class_base* object)
				{
					return object->get_id();
				}
			};

			template<class type>
			struct no_class_cast
			{
				static type* run(opcpp::base::class_base* object)
				{
					return NULL;
				}			
			};

			template<class type>
			struct run_class_cast
			{
				static type* run(opcpp::base::class_base* object)
				{
					if(!object)
						return NULL;
					
					//typedef typename opcpp::registration::get_class_type<type>::type class_type;
					
					int object_id = cast_accessor::get_id(object);
					
					//bool bresult = class_type::static_contains_id(object_id);
					//NOTE: might be worth restoring type to header to get inlining here?
					bool bresult = opcpp::registration::get_class_type<type>::get()->contains_id(object_id);
					
					if(!bresult)
						return NULL;
					
					return static_cast<type*>(object);
				}
			};
			
			template<class type>
			struct select_class_cast
			{
				typedef 
					typename opcpp::metafunction::IF< 
						opcpp::metadata::is_opclass<type>::value,
					//TODO: if type is known at compile time to be
					//		a parent class, do a fast static_cast.
						run_class_cast<type>,
						no_class_cast<type>
					>::RET
					select;
			};

		}

		using detail::select_class_cast;

		/// \brief Fast casting of opclass pointers to any opclass type.

		/// 
		/// \code
		/// using opcpp::casting::class_cast;
		/// 
		/// myclass* a = class_cast< myclass >(object);
		/// \endcode
		/// 
		template<class type>
		type* class_cast(opcpp::base::class_base* object)
		{
			return select_class_cast<type>::select::run(object);
		}
	}
}

category opclass
{
	location body
	{
		note get_id;
	}

	location source
	{
		note get_id;
	}
};

//prototype for get_id
note opclass::body::get_id()
{
private:
	virtual int get_id();
}

//implementation of get_id
note opclass::source::get_id(scope,alt_scope,alt_class_name,class_name)
{
	int scope::class_name::get_id()
	{
		return opcpp::registration alt_scope::alt_class_name::type::static_get_id();
	}
}

//==========================================
// universal cast
//==========================================

namespace opcpp
{
	namespace casting
	{
// 		using opcpp::metafunction::IF;
// 		using opcpp::metadata::is_opclass;
// 
// 		namespace detail
// 		{
// 			template<typename type>
// 			struct do_class_cast
// 			{
// 				static type* cast(opcpp::base::class_base* object)
// 				{
// 					return class_cast<type>(object);
// 				}
// 			};
// 			
// 
// 			template<typename type>
// 			struct do_static_cast
// 			{
// 				template<typename current>
// 				static type* cast(current* object)
// 				{
// 					return static_cast<type*>(object);
// 				}
// 			};
// 
// 			template<typename t>
// 			struct run_cast
// 			{
// 				typedef
// 					typename
// 					IF<	is_opclass<t>::value,
// 						//then				
// 						do_class_cast<t>,
// 						//else
// 						do_static_cast<t>
// 					>::RET
// 				
// 				//if is opclass, return class_cast
// 				//else use static_cast
// 
// 				type;
// 
// 				template<typename current>
// 				static t* cast(current* object)
// 				{
// 					return type::cast(object);
// 				}
// 			};		
// 		}
// 
// 		using detail::run_cast;
			
		// 
		// TODO: Universal cast function.
		// 
		//
		// 
// 
// 		template<typename type, typename current>
// 		inline type* cast(current* object)
// 		{
// 			return run_cast<type>::cast(object);
// 		}
	}
}