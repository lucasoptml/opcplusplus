//****************************************************************
// Copyright © 2008 opGames LLC - All Rights Reserved
//
// Authors: Lucas Ellis
//
// File: fields.doh
// Date: 12/11/2007
//
// Description:
//
// Field-related code.
//****************************************************************


// Use extension data_field_members to add new
// data members to data_field.
extension data_field_members;

// Use extension data_field_defaults to setup
// default values for a field member.
extension data_field_defaults;

// Use extension data_field_defaults to setup
// default values for a field member.
extension data_field_values;

// Use extension temp_field to
// add members to temp_field.
extension temp_field_members;

namespace opcpp
{
	namespace fields
	{
		using opcpp::base::class_base;
		using opcpp::base::visitor_base;
		using opcpp::accessors::member_info;
		using opcpp::base::boxed_struct;

		namespace detail
		{
			inline bool no_visit_class(class_base* object, visitor_base& visitor)
			{
				return false;
			}

			inline bool no_visit_struct(const boxed_struct& boxed, visitor_base& visitor)
			{
				return false;
			}
		}

		using detail::no_visit_class;
		using detail::no_visit_struct;

		/// \detail Class that represents a field in an object.
		struct data_field
		{
		protected:
			/// 
			/// Name of the field
			/// 
			/// You may use the opreflect() data modifier to override the value of this name.
			/// 
			/// \code
			/// opreflect("item") vector< int > Items;
			/// \endcode
			/// 
			const char* field_name;

		public:
			typedef bool (*visit_object_type)(class_base* object,        visitor_base& visitor);
			typedef bool (*visit_struct_type)(const boxed_struct& boxed, visitor_base& visitor);

		protected:
			//TODO: use a void* visit function + offset to make field use less data / code.
			visit_object_type visit_object_function;
			visit_struct_type visit_struct_function;  
			
			/// 
			/// \brief Is the field transient?
			/// 
			/// False by default, set to true using the transient data modifier.
			/// 
			/// \code
			/// transient int value;
			/// \endcode
			/// 
			bool		transient:1;

			extensionpoint data_field_members;

		public:

			data_field()
			{
				visit_object_function = &no_visit_class;
				visit_struct_function = &no_visit_struct;
				field_name = NULL;
				transient = false;

				extensionpoint data_field_defaults;
			}

			/// 
			/// \brief Visit this field given an opclass and a visitor.
			/// 
			/// If the opclass does not have this field, it will return false.
			/// 
			/// \code
			/// field.visit(object,visitor);
			/// \endcode
			/// 
			bool visit(class_base* object, visitor_base& visitor)
			{
				return (*visit_object_function)(object,visitor);
			}

			/// 
			/// \brief Visit this field given a boxed struct and a visitor.
			/// 
			/// If the opclass does not have this field, it will return false.
			/// 
			/// \code
			/// field.visit(object,visitor);
			/// \endcode
			/// 
			bool visit(const boxed_struct& boxed, visitor_base& visitor)
			{
				return (*visit_struct_function)(boxed,visitor);
			}

			/// 
			/// \brief Returns this field's member name.
			/// 
			/// You may use the opreflect() data modifier to override the value of this name.
			/// 
			/// \code
			/// opreflect("item") vector< int > Items;
			/// \endcode
			/// 
			const char* member_name() const
			{
				return field_name;
			}

			/// 
			/// \brief Is this field valid?
			/// 
			/// Indicates if the field is an actual named member or un-named data.
			/// 
			bool is_valid() const
			{
				return field_name != 0;
			}

			/// 
			/// \brief Returns true if this field is transient.
			/// 
			/// False by default, set to true using the transient data modifier.
			/// 
			/// \code
			/// transient int value;
			/// \endcode
			/// 
			bool is_transient() const
			{
				return transient;
			}
			

			//TODO: should have more queries:
			//* type name (constructed via visitor)
			//* type size (constructed via visitor)
			//* string conversion?
			
			///
			/// \brief Given an opclass, returns a pointer to the data specified by this field.
			/// 
			/// Returns NULL if the type did not match.
			/// 
			/// \code
			/// int* i = field.get_value<int>(object);
			/// \endcode
			/// 
			template<class type>
			type* get_value(class_base* object);
			
			///
			/// \brief Given an opclass, returns a pointer to the data specified by this field (reference version).
			///
			template<class type>
			type* get_value(class_base& object)
			{
				return get_value<type>(&object);
			}
		};

		/// 
		/// \brief Default invalid field for field-less accessors.
		/// 
		/// Accessors will use invalid field anytime they have no associated field.
		/// For example, when accessing the elements of a vector, the element will
		/// have no field.
		/// 
		struct invalid_field : public data_field
		{
			///
			/// \brief Returns the singleton invalid_field instance.
			///
			static const invalid_field& get_instance()
			{
				return instance;
			}
		
		private:
			static invalid_field instance;	
		};

		/// 
		/// \brief User constructable temporary field.
		/// 
		/// Only necessary in rare cases, usually
		/// used together with build_accessor() to create
		/// a temporary field/accessor to visit.
		/// 
		struct temp_field : public data_field
		{
		public:
			///
			/// \brief Constructs a temporary field, requires a field name.
			/// 
			temp_field(const char* inname)
			{
				field_name = inname;
			}
			
			///
			/// \brief Returns true if this field is transient.
			///
			void set_transient(bool value)
			{
				transient = value;
			}

			extensionpoint temp_field_members;
		};
	}
}

code source
{
	opcpp::fields::invalid_field opcpp::fields::invalid_field::instance;
};

//==========================================
// Adds fields to a category.
//==========================================

opmacro add_fields(category_name)
{
	category category_name
	{
		//prototypes
		location heading
		{
			datamap fields
			{
				is not native;
			}
		}

		//friends
		location body
		{
			datamap fields
			{
				is not native;
			}
		}
	};

	//prototypes - only reason is for friend declarations.
	note category_name::heading::fields::start(alt_namespace_start,alt_class_name)
	{
		namespace opcpp
		+{}
		namespace fields
		+{}

		alt_namespace_start

		namespace alt_class_name
		+{}
	}

	note category_name::heading::fields::mapping(member_name)
	{
 		struct member_name;
	}

	note category_name::heading::fields::end(alt_namespace_end)
	{
		-{}
		-{}
		
		alt_namespace_end

		-{}
		
	}

	//friends
	note category_name::body::fields::mapping(alt_scope,alt_class_name,member_name)
	{
		friend struct opcpp::fields alt_scope::alt_class_name::member_name;
	}

	//
	// Source Heading Information
	//

	category category_name
	{
		//metadata structs
		location sourceheading
		{
			note field_start;

			datamap fields
			{
				is not native;
			}

			hidden datamap field_visit
			{
				is not native;
				is not data_bits();
			}

			hidden datamap field_visit_bits
			{
				is not native;
				is data_bits();
			}

			note field_end;
		}
	}

	note category_name::sourceheading::field_start(alt_namespace_start,alt_class_name)
	{
		namespace opcpp
		+{}
		namespace fields
		+{}

		alt_namespace_start

		namespace alt_class_name
		+{}

		using ::opcpp::fields::data_field;
		using ::opcpp::base::visitor_base;
		using ::opcpp::base::class_base;
		using ::opcpp::metafunction::wraptype;
	}

	note category_name::sourceheading::fields::mapping(scope,class_name,member_name,data_type,data_full_type,alt_class_name)
	{
		struct member_name : public data_field
		{	
			typedef member_name		mytype;
				
			member_name()
			{
				visit_object_function = &attempt_visit_object;
				visit_struct_function = &attempt_visit_struct;

				const char* reflectname = ::opcpp::metadata::reflect_name< mytype >::get_name();

				field_name	   = reflectname ? reflectname : ``member_name``;
				transient      = ::opcpp::metadata::is_transient<member_name>::value;

				extensionpoint data_field_values;
			}

			static member_name* get_instance()
			{
				return &instance;
			}

			static bool attempt_visit_object(class_base* object,        visitor_base& visitor);
			static bool attempt_visit_struct(const boxed_struct& boxed, visitor_base& visitor);

			static void static_visit(scope::class_name* object, visitor_base& visitor);

		private:
			static member_name instance;
		};

		member_name member_name::instance;

	}

	note category_name::sourceheading::field_visit::mapping(data_full_type,alt_scope,scope,alt_class_name,class_name,member_name,map_index)
	{
		void member_name::static_visit(scope::class_name* object, visitor_base& visitor)
		{
			wraptype< data_full_type >::type r = wraptype< data_full_type >::wrap(object->member_name);

			const data_field& field = *get_instance();
			opcpp::reflection alt_scope::alt_class_name::member_name info(field,r);

			visitor.visit(info);
		}
	}

	note category_name::sourceheading::field_visit_bits::mapping(data_full_type,alt_scope,scope,alt_class_name,class_name,member_name,map_index)
	{
		void member_name::static_visit(scope::class_name* object, opcpp::base::visitor_base& visitor)
		{
			data_type temp = object->member_name;

			opcpp::metafunction::wraptype< data_full_type >::type ref = opcpp::metafunction::wraptype< data_full_type >::wrap(temp);

			const data_field& field = *opcpp::fields alt_scope::alt_class_name::member_name::get_instance();

			opcpp::reflection alt_scope::alt_class_name::member_name info(field,ref);

			visitor.visit(info);

			object->member_name = temp;
		}
	}

	note category_name::sourceheading::field_end(alt_namespace_end)
	{
		-{}
		-{}

		alt_namespace_end

		-{}
	}

}