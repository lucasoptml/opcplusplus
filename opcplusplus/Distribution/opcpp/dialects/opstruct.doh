//****************************************************************
// Copyright © 2008 opGames LLC - All Rights Reserved
//
// Authors: Lucas Ellis
//
// File: opstruct.doh
// Date: 08/22/2007
//
// Description:
//
// opstruct dialect file
//****************************************************************

//extension for struct_type instance
extension struct_type_instance_body;



structprefix(op) category opstruct
{

};

note opstruct::identifier()
{
	struct
}

extension struct_base_body;

//
// opstruct registration types
//

namespace opcpp
{
	namespace registration
	{
		class struct_type;
	}

	namespace accessors
	{
		struct struct_info;
	}

	namespace base
	{
		/// 
		/// \brief Base struct for all opstructs.
		/// 
		/// This is not extremely useful, since it has no polymorphism,
		/// and no virtual functions.
		/// 
		struct struct_base
		{
			/// 
			/// \brief Typedef for the parent opclass.
			/// 
			/// Always automatically generated for all opstructs.
			/// 
			/// Should use this to call parent versions of functions.
			/// 
			/// \code
			/// //call the nearest parent class version of function().
			/// Super::function();
			/// \endcode
			/// 
			typedef struct_base Super;
			typedef struct_base ThisType;

			/// 
			/// \brief Non-virtual, re-implemented versions will visit all data members.
			///
			void visit_data_members(visitor_base& visitor)
			{
			}

			//extensionpoint for struct_base
			extensionpoint struct_base_body
		};

		/// 
		/// \brief boxed struct - allows you to safely pass around an instance with a known type.
		/// 
		/// This is necessary since opstruct has no polymorphism, and no virtual functions.
		/// can only be constructed from struct_info, and from an opstruct itself.
		/// 
		struct boxed_struct
		{
			template<class T>
			boxed_struct(T* instance)
				:
				object(instance),
				object_type(instance ? instance->get_type() : NULL)
			{
			}

			boxed_struct( const boxed_struct& boxed )
				:
				object(boxed.object),
				object_type(boxed.object_type)
			{
			}

			struct_base* get_object() const
			{
				return object;
			}

			opcpp::registration::struct_type* get_type() const
			{
				return object_type;
			}

		private:
			boxed_struct(opcpp::registration::struct_type* instancetype, struct_base* instance)
				:
				object(instance),
				object_type(instancetype)
			{
			}

			friend opcpp::accessors::struct_info;

			opcpp::registration::struct_type* object_type;
			struct_base* object;
		};
	}

	namespace metadata
	{
		template<class c>
		struct is_opstruct
		{
			enum { value = false };
		};

		//allow it to work with single pointers also
		template<class c>
		struct is_opstruct<c*>
		{
			enum { value = is_opstruct<c>::value };
		};
	}

	namespace registration
	{
		using opcpp::fields::data_field;
		using opcpp::base::visitor_base;

		class struct_type;

		namespace detail
		{
			struct no_struct_type
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

				static struct_type* get_instance()
				{
					return NULL;
				}
			};

			template<class t>
			struct get_struct_type
			{
				//visible scaffold access to the struct type.
				static struct_type*	get() { return NULL; }
				static void			static_visit( struct_base* object, visitor_base& visitor ) {}
				static data_field*	static_get_field(size_t index) { return NULL; }
				typedef no_struct_type type;
			};
		}

		using detail::get_struct_type;
		using detail::no_struct_type;

	}
}

//
// opstruct metadata notes
//

category opstruct
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
	}

	location source
	{
		note id;
	}
};


note opstruct::heading::prototype(namespace_start,namespace_end,class_name)
{
	namespace_start

	struct class_name;

	namespace_end
}

note opstruct::footer::metadata(scope,class_name)
{
	namespace opcpp
	{
		namespace metadata
		{
			template<>
			struct is_opstruct< scope::class_name >
			{
				enum { value = true };
			};

			//opstruct unique id
			template<>
			struct OPCPP_API id< scope::class_name >
			{	
				static void* get_id()
				{
					return &static_id;
				}
			private:
				static bool static_id;
			};
		}
	}
}

note opstruct::source::id(scope,class_name)
{
	bool opcpp::metadata::id< scope::class_name >::static_id = false;
}

//Super typedef
category opstruct
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


note opstruct::body::thistype(class_name)
{
public:
	typedef class_name ThisType;
}

note opstruct::body::super(parent_name)
{
public:
	typedef parent_name Super;
}

note opstruct::body::super_noparent()
{
public:
	typedef opcpp::base::struct_base Super;
}

//if opclass has no parent, this is its only parent.
note opstruct::noparent()
{
	public opcpp::base::struct_base
}

//generate member count function - opstruct
category opstruct
{
	location footer
	{
		datamap member_count
		{
			is not native;
		}
	}
};

//NOTE: move to sourceheading?
note opstruct::footer::member_count::mapping()
{
}

note opstruct::footer::member_count::end(scope,class_name,map_count)
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

// setup the registration class - opstruct specific
category opstruct
{
	location sourceheading
	{
		note registration;
	}

};

//NOTE: move to sourceheading
note opstruct::sourceheading::registration(parent_name,alt_scope,scope,class_name,alt_namespace_start,alt_namespace_end,alt_class_name)
{
	//generate a class registration thing
	namespace opcpp
	{
		namespace registration
		{
			alt_namespace_start

			namespace alt_class_name
			{
				class type : public struct_type
				{
				public:
					//
					// compile time types and constants
					//
					
					typedef scope::class_name									Type;
					typedef scope::class_name::Super::ThisType					Parent;
					typedef opcpp::registration::get_struct_type<Parent>		ParentGetType;
					typedef opcpp::registration::get_struct_type<Parent>::type	ParentType;
					
					enum
					{
						field_count			= opcpp::metadata::field_count<Type>::value,
						parent_field_count	= opcpp::metadata::field_count<Parent>::total,
						total_field_count   = parent_field_count + field_count
					};

					//
					// virtual functions
					//

					//visit all members of this and parent's class
					//virtual bool		visit(class_base* object, visitor_base& visitor);

					//this grabs a specific field by index
					virtual data_field* get_field(size_t index);

					virtual data_field* get_field(const char* name);

					//this gets the total field count
					virtual size_t		get_field_count();

					//this gets the name of the type
					virtual const char* get_name();

					//create a new instance of this struct (if possible)
					//note that it will not be safely castable, you need to know the exact type
					virtual struct_base* new_instance();

					virtual bool is_serializable();

					//
					// static functions
					//

					static void			static_visit(scope::class_name* object, visitor_base& visitor);
					static data_field*	static_get_field(size_t index);
					static data_field*	static_get_field(const char* name);

					static type* get_instance()
					{
						return &instance;
					}

					private:

					void register_class();

					type()
					{
						initialize();
						register_class();
					}

				private:
					static type instance;

					extensionpoint struct_type_instance_body;
				};
			}

			alt_namespace_end

			struct_type* get_struct_type< scope::class_name >::get()
			{
				return opcpp::registration alt_scope::alt_class_name::type::get_instance();
			}

			void get_struct_type< scope::class_name >::static_visit(scope::class_name* object, visitor_base& visitor )
			{
				opcpp::registration alt_scope::alt_class_name::type::static_visit(object,visitor);
			}

			data_field* get_struct_type< scope::class_name >::static_get_field(size_t index)
			{
				return opcpp::registration alt_scope::alt_class_name::type::static_get_field(index);
			}
		}
	}

	inline void scope::class_name::visit_data_members(opcpp::base::visitor_base& visitor)
	{
		typedef opcpp::registration alt_scope::alt_class_name::type Type;
		Type::static_visit(this,visitor);
	}
}

category opstruct
{
	location footer
	{
		note getstruct;
	}
};

note opstruct::footer::getstruct(scope,alt_scope,alt_namespace_start,alt_namespace_end,class_name,alt_class_name)
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
				struct OPCPP_API get_struct_type< scope::class_name >
				{
					typedef opcpp::registration alt_scope::alt_class_name::type type;
					static struct_type*	get();
					static void			static_visit( scope::class_name* object, visitor_base& visitor );
					static data_field*	static_get_field(size_t index);
				};
			}
		}
	}
}

category opstruct
{
	location source
	{
		note registration;
	}
};

note opstruct::source::registration(alt_scope,alt_class_name)
{
	void opcpp::registration alt_scope::alt_class_name::type::register_class()
	{
		//perform registration
		//1. tell the parent about me
		//2. set my parent_class
		parent_type = ParentGetType::get();

		if(parent_type)
			register_parent(parent_type,&instance);

		struct_type::register_class(&instance);
	}
}

category opstruct
{
	location source
	{
		note new_instance;
		note is_serializable;
	}
};

note opstruct::source::new_instance(alt_scope,scope,alt_class_name,class_name)
{
	opcpp::base::struct_base* opcpp::registration alt_scope::alt_class_name::type::new_instance()
	{
		//NOTE: this handles missing default ctors - this will return a new instance if 
		//		a default construction was available, or else it returns NULL.
		//		specialize for opcpp::registration::new_instance to override behavior.
		return opcpp::registration::new_instance< scope::class_name >::type::create();
	}
}

note opstruct::source::is_serializable(alt_scope,scope,alt_class_name,class_name)
{
	bool opcpp::registration alt_scope::alt_class_name::type::is_serializable()
	{
		//NOTE: this handles missing default ctors - this will return a new instance if 
		//		a default construction was available, or else it returns NULL.
		//		specialize for opcpp::registration::new_instance to override behavior.
		return opcpp::metadata::is_serializable< scope::class_name >::value;
	}
}

category opstruct
{
	location source
	{
		datamap visit_map
		{
			is not native;
		}
	}
};

note opstruct::source::visit_map::start(alt_namespace_start,alt_class_name)
{
	namespace opcpp
	+{}
	namespace fields
	+{}

	alt_namespace_start

	namespace alt_class_name
	+{}
}

//cannot externally visit structs
note opstruct::source::visit_map::mapping(member_name,alt_scope,alt_class_name,scope,class_name)
{
	bool member_name::attempt_visit_object(opcpp::base::class_base* object, opcpp::base::visitor_base& visitor)
	{
		return false;
	}

	bool member_name::attempt_visit_struct(const boxed_struct& boxed, opcpp::base::visitor_base& visitor)
	{
		if( boxed.get_type()->is_child_of( *opcpp::registration alt_scope::alt_class_name::type::get_instance() ) )
		{
			scope::class_name* correctobject = static_cast<scope::class_name*>(boxed.get_object());
			static_visit(correctobject,visitor);
			return true;
		}
		return false;
	}

}

note opstruct::source::visit_map::end(alt_namespace_end)
{
	-{}
	
	alt_namespace_end	
		
	-{}
	-{}

}


//TODO: friend fix: what is friend fix?
//		friend fix says that if you have a type, you must have a friend for its registration type.
//		struct A {};
//		friend opcpp::reg...::_A::type;
//		this should occur in post location, if it's an inner type.

category opstruct
{
	location post
	{
		note friendfix
		{
			is inner_type;
		}

		datamap memberfix
		{
			is inner_type;
			is not native;
		}
	}
};

note opstruct::post::friendfix(alt_scope,alt_class_name,scope)
{
	friend class opcpp::registration alt_scope::alt_class_name::type;
}

note opstruct::post::memberfix::mapping(alt_scope,alt_class_name,member_name)
{
	friend struct opcpp::fields alt_scope::alt_class_name::member_name;
}

//==========================================
// Fix for opstructs whose default 
// constructor is not public.
//==========================================

category opstruct
{
	location body
	{
		note private_constructor_fix;
	}
}

note opstruct::body::private_constructor_fix(class_name,scope)
{
	friend opcpp::registration::detail::create_instance< scope::class_name >;
}


