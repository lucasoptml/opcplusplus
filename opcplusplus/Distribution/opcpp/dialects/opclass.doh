//****************************************************************
// Copyright © 2008 opGames LLC - All Rights Reserved
//
// Authors: Lucas Ellis
//
// File: opclass.doh
// Date: 08/22/2007
//
// Description:
//
// opclass dialect file
//****************************************************************

//extension to allow you to add members to class_base
extension class_base_body;

//extension to allow you to add code to the class_base destructor
extension class_base_destructor;

//extension to allow you to add code to the class_base constructor
extension class_base_constructor;

//extension to allow you to add code to the class_type instance for opclasses
extension class_type_instance_body;

// class_base
namespace opcpp
{
	namespace registration
	{
		class class_type;
	}

	namespace casting
	{
		namespace detail
		{
			struct cast_accessor;
		}
	}

	namespace base
	{
		class visitor_base;

		/// 
		/// \brief Base class for all opclasses.
		/// 
		/// All opclasses derive from this class.
		/// 
		class class_base
		{
		public:
			/// 
			/// \brief Typedef for the parent opclass.
			/// 
			/// Always automatically generated for all opclasses.
			/// 
			/// Should use this to call parent versions of functions.
			/// 
			/// \code
			/// //call the nearest parent class version of function().
			/// Super::function();
			/// \endcode
			/// 
			typedef class_base Super;//???no_class?
			typedef class_base ThisType;
			
			/// 
			/// \brief Visit all data members in this instance's type.
			/// 
			/// Visits all members, pass in a visitor instance.
			/// 
			virtual void visit_data_members(visitor_base& visitor)
			{}
			
			/// 
			/// \brief Get the class_type for this instance.
			/// 
			/// Returns the class_type for this opclass as a valid reference.
			/// 
			virtual opcpp::registration::class_type& get_type() = NULL;
			

			class_base()
			{
				extensionpoint class_base_constructor
			}

			///
			/// class_base has a virtual destructor, please note
			/// you may need to make parents have virtual destructors as well.
			virtual ~class_base()
			{
				extensionpoint class_base_destructor
			}
			
			extensionpoint class_base_body

		private:
			/// 
			/// Internal casting function.
			/// 
			virtual int get_id() = NULL;
			
			friend struct opcpp::casting::detail::cast_accessor;
		};
	}

	namespace registration
	{
		using opcpp::base::class_base;
		using opcpp::base::struct_base;
		using opcpp::base::visitor_base;
		using opcpp::fields::data_field;

		namespace detail
		{
			struct no_class_type
			{
				enum
				{
					total_field_count = 0,
					parent_field_count = 0,
					field_count = 0
				};

				static data_field* static_get_field(size_t)
				{
					return NULL;
				}

				template<class type>
				static void static_visit(type* object, visitor_base& visitor)
				{
				}

				//null type
				static class_type* get_instance()
				{
					return NULL;
				}
			};

			//grabs the associated type for a class
			template<class t>
			struct get_class_type
			{
				//visible scaffold access to a class type.
				static class_type*	get() { return NULL; }
				static class_type*  get_class() { return NULL; }
				static void			static_visit( class_base* object, visitor_base& visitor ) {}
				static data_field*	static_get_field(size_t index) { return NULL; }
				typedef no_class_type type;
			};
		}
		
		using detail::get_class_type;
		using detail::no_class_type;
	}
}

//opclass Super typedef
classprefix(op) category opclass
{
	location body
	{
		note thistype;

		note super
		{
			is not category_baseclass;
		}

		note super_noparent
		{
			is category_baseclass;
		}
	}
};

note opclass::body::thistype(class_name)
{
public:
	typedef class_name ThisType;
}

note opclass::body::super(parent_name)
{
public:
	typedef parent_name Super;
}

note opclass::body::super_noparent()
{
public:
	typedef ::opcpp::base::class_base Super;
}

//if opclass has no parent, this is its only parent.
note opclass::noparent()
{
	public ::opcpp::base::class_base
}

//==========================================
// opclass Metadata
//==========================================

namespace opcpp
{
	namespace metadata
	{
		template<class c>
		struct is_opclass
		{
			enum { value = false };
		};

		//allow it to work with single pointers also
		template<class c>
		struct is_opclass<c*>
		{
			enum { value = is_opclass<c>::value };
		};
	}
}

//setup prototypes, metadata - opclass specific
category opclass
{
	location heading
	{
		note prototype
		{
			is not inner_type;
		}
	}

	location footer
	{
		note metadata;
// 		{
// 			is not inner_type;
// 		}
	}
};

note opclass::heading::prototype(namespace_start,namespace_end,class_name)
{
	namespace_start

	class class_name;

	namespace_end
}

note opclass::footer::metadata(scope,class_name)
{
	namespace opcpp
	{
		namespace metadata
		{
// 			template<>
// 			struct get_name< scope::class_name >
// 			{
// 				static const char* get()
// 				{
// 					return ``class_name``;
// 				}
// 			};

			template<>
			struct is_opclass< scope::class_name >
			{
				enum { value = true };
			};
		}
	}
}

//generate member count function - opclass
category opclass
{
	location footer
	{
		datamap member_count
		{
			is not native;
		}
	}
};

note opclass::footer::member_count::mapping()
{
}

note opclass::footer::member_count::end(scope,class_name,map_count)
{
	namespace opcpp
	{
		namespace metadata
		{
			template<>
			struct field_count< scope::class_name >
			{
				enum 
				{ 
					value = map_count,
					total = map_count + field_count< scope::class_name::Super::ThisType >::total,
				};
			};
		}
	}		
}

// setup the registration class - opclass specific
category opclass
{
	location sourceheading
	{
		note registration;
	}

};

note opclass::sourceheading::registration(parent_name,alt_scope,scope,class_name,alt_namespace_start,alt_namespace_end,alt_class_name)
{
	//generate a class registration thing
	namespace opcpp
	{
		namespace registration
		{
			alt_namespace_start

			namespace alt_class_name
			{
				using ::opcpp::base::class_base;
				using ::opcpp::registration::class_type;
				using ::opcpp::base::visitor_base;
				using ::opcpp::fields::data_field;

				class type : public class_type
				{
				public:
					//
					// compile time types and constants
					//
					
					typedef scope::class_name									Type;
					typedef scope::class_name::Super::ThisType					Parent;
					typedef ::opcpp::registration::get_class_type<Parent>		ParentGetType;
					typedef ::opcpp::registration::get_class_type<Parent>::type	ParentType;
					
					enum
					{
						field_count			= ::opcpp::metadata::field_count<Type>::value,
						parent_field_count	= ::opcpp::metadata::field_count<Parent>::total,
						total_field_count   = parent_field_count + field_count
					};
					
					//
					// virtual functions
					//
					
					//visit all members of this and parent's class
					virtual bool		visit(class_base* object, visitor_base& visitor);
					
					//this grabs a specific field by index
					virtual data_field* get_field(size_t index);

					virtual data_field* get_field(const char* name);

					//this gets the total field count
					virtual size_t		get_field_count();
					
					//this gets the name of the type
					virtual const char* get_name();
					
					//create a new instance of this class (if possible)
					virtual class_base* new_instance();
					
					virtual bool is_serializable();
					
					//
					// static functions
					//
					
					static void			static_visit(scope::class_name* object, visitor_base& visitor);
					static data_field*	static_get_field(size_t index);
					static data_field*	static_get_field(const char* name);
					
					static int static_get_id()
					{
						return instance.get_id();
					}
					
					static int static_get_id_range()
					{
						return instance.get_id_range();
					}
					
					static type* get_instance()
					{
						return &instance;
					}

					static bool static_contains_id(int id)
					{
						return instance.contains_id(id);
					}

				private:

					void register_class()
					{
						//perform registration
						//1. tell the parent about me
						//2. set my parent_class
						parent_type = ParentGetType::get_class();

						if(parent_type)
							register_parent(parent_type,&instance);

						class_type::register_class(&instance);
					}
					
					type()
					{
						initialize();
						register_class();

						type_size = sizeof(Type);
					}
					
				private:
					static type instance;

					extensionpoint class_type_instance_body;
				};
			}

			alt_namespace_end

			class_type* get_class_type< scope::class_name >::get()
			{
				return opcpp::registration alt_scope::alt_class_name::type::get_instance();
			}

			class_type* get_class_type< scope::class_name >::get_class()
			{
				return get();
			}

			void get_class_type< scope::class_name >::static_visit(scope::class_name* object, visitor_base& visitor )
			{
				opcpp::registration alt_scope::alt_class_name::type::static_visit(object,visitor);
			}

			data_field* get_class_type< scope::class_name >::static_get_field(size_t index)
			{
				return opcpp::registration alt_scope::alt_class_name::type::static_get_field(index);
			}
		}
	}

	void scope::class_name::visit_data_members(::opcpp::base::visitor_base& visitor)
	{
		typedef opcpp::registration alt_scope::alt_class_name::type Type;
		Type::static_visit(this,visitor);
	}

}

category opclass
{
	location heading
	{
		note getclass;
	}
};

note opclass::heading::getclass(scope,alt_scope,alt_namespace_start,alt_namespace_end,class_name,alt_class_name)
{
	//generate the type meta function
	namespace opcpp
	{
		namespace registration
		{
			alt_namespace_start
			
			namespace alt_class_name
			{
				class type;
			}
			
			alt_namespace_end
			
			namespace detail
			{
				template<>
				struct OPCPP_API get_class_type< scope::class_name >
				{
					static class_type*	get();
					static class_type*	get_class();
					static void			static_visit( scope::class_name* object, visitor_base& visitor );
					static data_field*	static_get_field( size_t index );
					
					typedef opcpp::registration alt_scope::alt_class_name::type type;
				};
			}
		}
	}
}

category opclass
{
	location source
	{
		note registration;
		
		datamap visit_map
		{
			is not native;
		}
	}
};

note opclass::source::visit_map::start(alt_namespace_start,alt_class_name)
{
	namespace opcpp
	+{}
	namespace fields
	+{}
	
	alt_namespace_start
	
	namespace alt_class_name
	+{}
}

note opclass::source::visit_map::mapping(class_name,member_name,scope)
{
	bool member_name::attempt_visit_object(::opcpp::base::class_base* object, ::opcpp::base::visitor_base& visitor)
	{
		if( scope::class_name* correctobject = ::opcpp::casting::class_cast< scope::class_name >(object) )
		{
			static_visit(correctobject,visitor);
			return true;
		}
		return false;
	}

	bool member_name::attempt_visit_struct(const boxed_struct& boxed, visitor_base& visitor)
	{
		return false;
	}
}

note opclass::source::visit_map::end(alt_namespace_end)
{
	-{}
	
	alt_namespace_end

	-{}
	-{}
}

note opclass::source::registration(alt_scope,scope,alt_class_name,class_name)
{
	bool opcpp::registration alt_scope::alt_class_name::type::visit(::opcpp::base::class_base* object, ::opcpp::base::visitor_base& visitor)
	{
		if(scope::class_name* correctobject = ::opcpp::casting::class_cast< scope::class_name >(object))
		{
			static_visit(correctobject,visitor);
			return true;
		}
		return false;
	}
}

category opclass
{
	location source
	{
		note new_instance;
		note is_serializable;
	}
};

note opclass::source::new_instance(alt_scope,scope,alt_class_name,class_name)
{
	opcpp::base::class_base* opcpp::registration alt_scope::alt_class_name::type::new_instance()
	{
		//NOTE: this handles missing default ctors - this will return a new instance if 
		//		a default construction was available, or else it returns NULL.
		//		specialize for opcpp::registration::new_instance to override behavior.
		return ::opcpp::registration::new_instance< scope::class_name >::type::create();
	}
}

note opclass::source::is_serializable(alt_scope,scope,alt_class_name,class_name)
{
	bool opcpp::registration alt_scope::alt_class_name::type::is_serializable()
	{
		//NOTE: this handles missing default ctors - this will return a new instance if 
		//		a default construction was available, or else it returns NULL.
		//		specialize for opcpp::registration::new_instance to override behavior.
		return ::opcpp::metadata::is_serializable< scope::class_name >::value;
	}
}




