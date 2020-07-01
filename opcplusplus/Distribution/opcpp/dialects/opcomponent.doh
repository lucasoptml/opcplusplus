//****************************************************************
// Copyright © 2008 opGames LLC - All Rights Reserved
//
// Authors: Lucas Ellis
//
// File: opcomponent.doh
// Date: 07/25/2007
//
// Description:
//
// opcomponent
//****************************************************************

//==========================================
// opcomponent
//==========================================


namespace opcpp
{
	namespace casting
	{
		template<class T>
		T* component_cast(opcpp::base::class_base* object)
		{
			if(!object)
				return NULL;
			
			class_type* type = get_class_type< T >::get();
			
			opcpp::base::component_base* result = object->get_component(type);
			
			return static_cast<T*>(result);
		}
	}
}

extendpoint class_base_body
{
public:
	virtual opcpp::base::component_base* get_component(opcpp::registration::class_type* type) { return NULL; }
};

category opcomponent
{
	location body
	{
		note get_component;
	}
};

note opcomponent::body::get_component(scope,class_name)
{
	opcpp::base::component_base* get_component(opcpp::registration::class_type* type)
	{
		if( type == get_class_type<ThisType>::get() )
			return (opcpp::component scope::class_name*)this;
		return Super::get_component(type);
	}
}

classprefix(component) 
category opcomponent
{

};

//==========================================
// opcomponent metadata
//==========================================

namespace opcpp
{
	namespace metadata
	{
		template<class type>
		struct is_opcomponent
		{
			enum { value = false };
		};
	}
}

category opcomponent
{
	location footer
	{
		note iscomponent;
	}
};

note opcomponent::footer::iscomponent(scope,class_name)
{
	namespace opcpp
	{
		namespace metadata
		{
			template<class ParentClass>
			struct is_opcomponent< scope::class_name<ParentClass> >
			{
				enum { value = true };
			};
		}
	}
}

category opcomponent
{
	error("no uninlined functions allowed in opcomponent!")
	disallow UninlineFunctions
	{
		is function_statement;
		is not inline;
	}
};



note opcomponent::identifier()
{
	template<class ParentClass> 
	class 
}

note opcomponent::default()
{
public:
}

//always inherits from this
note opcomponent::parent(class_name,scope)
{
	public ParentClass, public opcpp::component scope::class_name
}

//function prefix
note opcomponent::prefix()
{
	template<class ParentClass>
}

//function postfix
note opcomponent::postfix()
{
	<ParentClass>
}

//opclass Super typedef
category opcomponent
{
	location body
	{
		note thistype;

		note super;
	}
};

note opcomponent::body::thistype(class_name)
{
public:
	typedef class_name ThisType;
}

note opcomponent::body::super(parent_name)
{
public:
	typedef ParentClass Super;
}

//NOTE: do I need is_component metafunction? could add it.

//setup prototypes, metadata - opclass specific
category opcomponent
{
	location heading
	{
		note prototype
		{
			is not inner_type;
		}
	}
};

note opcomponent::heading::prototype(namespace_start,namespace_end,class_name)
{
	namespace_start

	template<class ParentClass>
	class class_name;

	namespace_end

		//declare the component type.
		namespace opcpp
	{
		namespace component
		{
			namespace_start

			class class_name;

			namespace_end
		}
	}
}


//generate member count function - opcomponent
category opcomponent
{
	location footer
	{
		datamap member_count
		{
			is not native;
		}
	}
};

note opcomponent::footer::member_count::mapping()
{
}

note opcomponent::footer::member_count::end(scope,class_name,map_count)
{
	namespace opcpp
	{
		namespace metadata
		{
			template<class ParentClass>
			struct field_count< scope::class_name<ParentClass> >
			{
				enum 
				{ 
					value = map_count,
					total = map_count + field_count< scope::class_name<ParentClass>::Super::ThisType >::total,
				};
			};

			template<> struct field_count< opcpp::component scope::class_name >
			{
				enum
				{
					value = map_count
				};
			};
		}
	}		
}


// setup the registration class - opclass specific
category opcomponent
{
	location sourceheading
	{
		note registration;
	}

};

note opcomponent::sourceheading::registration(parent_name,alt_scope,scope,class_name,alt_namespace_start,alt_namespace_end,alt_class_name)
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

					typedef opcpp::component scope::class_name									Type;
					// 					typedef scope::class_name::Super::ThisType					Parent;
					// 					typedef ::opcpp::registration::get_class_type<Parent>		ParentGetType;
					//typedef ::opcpp::registration::get_class_type<Parent>::type	ParentType;

					enum
					{
						field_count			= ::opcpp::metadata::field_count<Type>::value,
					//	parent_field_count	= ::opcpp::metadata::field_count<Parent>::total,
					//	total_field_count   = parent_field_count + field_count
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

					static void			static_visit(opcpp::component scope::class_name* object, visitor_base& visitor);
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
						// 						parent_type = ParentGetType::get();
						// 
						// 						if(parent_type)
						// 							register_parent(parent_type,&instance);

						class_type::register_class(&instance);
					}

					type()
					{
						initialize();
						register_class();
					}

				private:
					static type instance;
				};
			}

			alt_namespace_end

				class_type* get_class_type< opcpp::component scope::class_name >::get()
			{
				return opcpp::registration alt_scope::alt_class_name::type::get_instance();
			}

			void get_class_type< opcpp::component scope::class_name >::static_visit(opcpp::component scope::class_name* object, visitor_base& visitor )
			{
				opcpp::registration alt_scope::alt_class_name::type::static_visit(object,visitor);
			}

			data_field* get_class_type< opcpp::component scope::class_name >::static_get_field(size_t index)
			{
				return opcpp::registration alt_scope::alt_class_name::type::static_get_field(index);
			}
		}
	}

	void opcpp::component scope::class_name::visit_data_members(::opcpp::base::visitor_base& visitor)
	{
		typedef opcpp::registration alt_scope::alt_class_name::type Type;
		Type::static_visit(this,visitor);
	}

}

category opcomponent
{
	location footer
	{
		note getclass;
	}
};

note opcomponent::footer::getclass(scope,alt_scope,alt_namespace_start,alt_namespace_end,class_name,alt_class_name)
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
				template< class ParentClass >
				struct get_class_type< scope::class_name<ParentClass> >
				{
					//get my component class type?
					//this might present a problem, since it could interpret it as an actual opclass.
					static class_type*	get()
					{
						return get_class_type< opcpp::component scope::class_name >::get();
					}

					static class_type* get_class()
					{
						return get_class_type< ParentClass::ThisType >::get_class();
					}

					static void	static_visit( scope::class_name<ParentClass>* object, visitor_base& visitor )
					{
						//visit my parent first, then...
						get_class_type< ParentClass::ThisType >::static_visit(object,visitor);

						//visit my component stuff.
						//NOTE: this is very weird.
						get_class_type< opcpp::component scope::class_name >::static_visit(object,visitor);
					}

					static data_field* static_get_field( size_t index )
					{
						//grab from my parent first - if we get anything, return it
						//or else grab from my component stuff.

						if( data_field* f = get_class_type< ParentClass::ThisType >::static_get_field(index) )
							return f;

						//TODO: need access to # of parent_fields.
						//		issue is it needs to be recursive on opcomponents
						size_t parent_fields = opcpp::metadata::field_count< ParentClass::ThisType >::total;
						size_t myindex = index - parent_fields;
						return get_class_type< opcpp::component scope::class_name >::static_get_field(myindex);
					}

					typedef opcpp::registration alt_scope::alt_class_name::type type;
				};

				template<>
				struct OPCPP_API get_class_type< opcpp::component scope::class_name >
				{
					//get my component class type?
					//this might present a problem, since it could interpret it as an actual opclass.
					static class_type*	get();
					static class_type*  get_class() { return NULL; }
					static void			static_visit( opcpp::component scope::class_name* object, visitor_base& visitor );
					static data_field*	static_get_field( size_t index );

					typedef opcpp::registration alt_scope::alt_class_name::type type;
				};

			}
		}
	}
}

category opcomponent
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

note opcomponent::source::visit_map::start(alt_namespace_start,alt_class_name)
{
	namespace opcpp
		+{}
	namespace fields
		+{}

	alt_namespace_start

		namespace alt_class_name
		+{}
}

note opcomponent::source::visit_map::mapping(class_name,member_name,scope)
{
	bool member_name::attempt_visit(::opcpp::base::class_base* object, ::opcpp::base::visitor_base& visitor)
	{
		if( opcpp::component scope::class_name* correctobject = 
			opcpp::casting::component_cast< opcpp::component scope::class_name >(object) )
		{
			static_visit(correctobject,visitor);
			return true;
		}
		return false;
	}
}

note opcomponent::source::visit_map::end(alt_namespace_end)
{
	-{}

	alt_namespace_end

	-{}
	-{}
}

note opcomponent::source::registration(alt_scope,scope,alt_class_name,class_name)
{
	bool opcpp::registration alt_scope::alt_class_name::type::visit(::opcpp::base::class_base* object, ::opcpp::base::visitor_base& visitor)
	{
		if( opcpp::component scope::class_name* correctobject = 
			opcpp::casting::component_cast< opcpp::component scope::class_name >(object) )
		{
			static_visit(correctobject,visitor);
			return true;
		}
		return false;
	}
}

category opcomponent
{
	location source
	{
		note new_instance;
		note is_serializable;
	}
};

note opcomponent::source::new_instance(alt_scope,scope,alt_class_name,class_name)
{
	opcpp::base::class_base* opcpp::registration alt_scope::alt_class_name::type::new_instance()
	{
		return NULL;
	}
}

note opcomponent::source::is_serializable(alt_scope,scope,alt_class_name,class_name)
{
	bool opcpp::registration alt_scope::alt_class_name::type::is_serializable()
	{
		return false;
	}
}

// 	==========================================
// 	opcomponent Proxy Implementation
// 	==========================================
	
	category opcomponent
	{
		location body
		{
			datamap component_data_public
			{
				is public or default;
				is not data_bits();
				is not static;
			};
	
			datamap component_data_private
			{
				is private or protected;
				is not data_bits();
				is not static;
			}
	
			datamap component_bits_public
			{
				is public or default;
				is data_bits();
				is not static;
			}
	
			datamap component_bits_private
			{
				is private or protected;
				is data_bits();
				is not static;
			}
// 	
// 			datamap component_data
// 			{
// 				is public or default;
// 			};
	
		}
	};
	
	note opcomponent::body::component_data_public::start()
	{
		//gets and sets
	}
	
	note opcomponent::body::component_data_public::mapping(data_type,member_name)
	{
		virtual data_type& get_@member_name()
		{
			return member_name;
		}
	
		virtual void set_@member_name(data_type& in)
		{
			member_name = in;
		}
	}
	
	note opcomponent::body::component_data_public::end()
	{
	}
	
//	private data
	note opcomponent::body::component_data_private::start()
	{
		//gets and sets
	}
	
	note opcomponent::body::component_data_private::mapping(data_type,member_name)
	{
	public:
		virtual data_type& get_@member_name()
		{
			return member_name;
		}
	
		virtual void set_@member_name(data_type& in)
		{
			member_name = in;
		}
	}
	
	note opcomponent::body::component_data_private::end()
	{
	}
	
//	public bits
	note opcomponent::body::component_bits_public::start()
	{
	}
	
	note opcomponent::body::component_bits_public::mapping(data_type,member_name)
	{
		virtual data_type get_@member_name()
		{
			return member_name;
		}
	
		virtual void set_@member_name(data_type& in)
		{
			member_name = in;
		}
	}
	
	note opcomponent::body::component_bits_public::end()
	{
	}
	
//	private bits
	note opcomponent::body::component_bits_private::start()
	{
	}
	
	note opcomponent::body::component_bits_private::mapping(data_type,member_name)
	{
	protected:
		virtual data_type get_@member_name()
		{
			return member_name;
		}
	
		virtual void set_@member_name(data_type& in)
		{
			member_name = in;
		}
	}
	
	note opcomponent::body::component_bits_private::end()
	{
	}
	
	category opcomponent
	{
		location footer
		{
			note visit;
		}
	};

	note opcomponent::footer::visit(scope,class_name)
	{
		template<class ParentClass>
		void scope::class_name<ParentClass>::visit_data_members(opcpp::base::visitor_base& visitor)
		{
			opcpp::registration::get_class_type< scope::class_name<ParentClass> >::static_visit(this,visitor);
		}
	}


