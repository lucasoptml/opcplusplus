//****************************************************************
// Copyright © 2008 opGames LLC - All Rights Reserved
//
// Authors: Lucas Ellis
//
// File: registration.doh
// Date: 10/30/2007
//
// Description:
//
// registration dialect code
//****************************************************************

// extension for class_type
extension class_type_body;

// extension for struct_type
extension struct_type_body;

verbatim code heading
{
#ifndef OPCPP_NEW
#define OPCPP_NEW(typ) new typ()
#endif

};

namespace opcpp
{
	namespace registration
	{
		using opcpp::metadata::has_default_constructor;
		using opcpp::metafunction::IF;

		namespace detail
		{
			//cannot create a default instance, returns NULL
			template<class type>
			struct no_instance
			{
				static type* create()
				{
					return NULL;
				}
			};

			//creates a new default instance using the default constructor
			template<class type>
			struct create_instance
			{
				static type* create()
				{
					return OPCPP_NEW(type);
				}
			};

			template<class t>
			struct new_instance
			{
				//if we have a default constructor, use new type(), else return NULL
				typedef typename
					IF< 
					has_default_constructor<t>::value,
					create_instance<t>,
					no_instance<t>
					>::RET
					type;
			};
		}

		using detail::new_instance;

		class class_tracker;
		class struct_tracker;

		/// 
		/// \brief Base class for opC++ types (opclass, opstruct).
		/// 
		class type_base
		{
		public:
			///
			/// \brief Returns the name of the type.
			///
			virtual const char* get_name() = NULL;

			///
			/// \brief Returns a field in the type, given an index.
			///
			/// If the index is out of range, NULL is returned.
			///
			virtual data_field*	get_field(size_t index) = NULL;

			///
			/// \brief Returns a field in the type, given a field name.
			///
			/// If the specified field name is not found, NULL is returned.
			virtual data_field* get_field(const char* name) = NULL;

			///
			/// \brief Returns the number of fields in the type.
			///
			virtual size_t	get_field_count() = NULL;
		};

		/// 
		/// \brief Base class for opC++ opclass types.
		/// 
		/// Provides type related functions,
		/// as well as external type and field access.
		///
		class class_type : public type_base
		{
		public:

			/// 
			/// \brief Get the next (sibling) type.
			/// 
			/// Returns NULL if no sibling exists.
			/// 
			class_type* get_next()
			{
				return next_type;
			}

			/// 
			/// \brief Get the first child type.
			/// 
			/// Returns NULL if no child types exist.
			/// 
			class_type* get_child()
			{
				return child_type;
			}

			/// 
			/// \brief Get the parent type.
			/// 
			/// Returns NULL if no parent types exist.
			/// This means the opclass had no parent listed, 
			/// it still derives from class_base however.
			/// 
			class_type* get_parent()
			{
				return parent_type;
			}

			/// 
			/// \brief Create a new default constructed instance of this type.
			/// 
			/// This function creates a new instance of the type.
			/// 
			/// If a class is not default constructable, it will return NULL.
			/// 
			virtual class_base* new_instance() = NULL;

			/// 
			/// \brief Is this opclass serializable?
			/// 
			/// Returns whether this class is serializable.
			/// 
			virtual bool is_serializable() = NULL;


			/// 
			/// \brief What is the size of this class type?
			/// 
			/// Returns the size of the class type.
			/// 
			size_t get_size()
			{
				return type_size;
			}

		protected:
			/// 
			/// Internal automatic initialization of types.
			/// 
			void initialize()
			{
				static bool binitialized = false;

				if(!binitialized)
				{
					next_type		= NULL;
					child_type		= NULL;
					parent_type		= NULL;
					start_id		= -1;
					id_range		= 0;
				}

				binitialized = true;
			}

			///
			/// Internal registration of types.
			///
			void register_next(class_type* entry)
			{
				if(next_type)
					entry->next_type = next_type;
				next_type = entry;
			}

			///
			/// Internal registration of types.
			///
			void register_child(class_type* entry)
			{
				if(!child_type)
					child_type = entry;
				else
				{
// 					entry->register_next(child_type);
// 					child_type = entry;

					// insertion sort by name
					class_type* prev = 0;
					class_type* current = child_type;

					while( current && strcmp(current->get_name(),entry->get_name()) < 0 )
					{
						prev = current;
						current = current->next_type;
					}

					if(!prev)
					{
						entry->register_next(child_type);
						child_type = entry;
					}
					else
						prev->register_next(entry);
				}
			}

			///
			/// Internal registration of types.
			///
			void register_parent(class_type* parent, class_type* entry)
			{
				parent->register_child(entry);
			}

			///
			/// Internal registration of types.
			///
			inline void register_class(class_type* entry);

			class_type* next_type;
			class_type* child_type;
			class_type* parent_type;

			friend class class_tracker;

		public:
			///
			/// \brief Get the opclass id.
			/// 
			/// Used by opcpp::casting::class_cast().
			///
			int get_id() const
			{
				return start_id;
			}

			///
			/// \brief Get the opclass id range.
			///
			/// Inclusive range size of this class and all child classes.
			/// Used by opcpp::casting::class_cast(). 
			///
			int get_id_range() const
			{
				return id_range;
			}

			/// 
			/// \brief Does this type contain this id? 
			/// 
			/// Used by opcpp::casting::class_cast().
			///
			bool contains_id(int id) const
			{
				//fast integer range check:
				//from book Hackers Delight
				return (unsigned int)(id - start_id) <= (unsigned int)(id_range);
				//equivalent to:
				//if(id > instance.end_id
				//|| id < instance.start_id)
				//	return false;
				//return true;
			}


			/// 
			/// \brief Is this type a child of (or same as) another type? 
			/// 
			/// Use to quickly verify inheritance of types.
			///
			bool is_child_of(const class_type& type) const
			{
				return type.contains_id(get_id());
			}

		protected:
			size_t type_size;

		private:
			/// 
			/// Internal id generation.
			/// 
			void set_id(int id)
			{
				start_id = id;
			}

			/// 
			/// Internal id generation.
			/// 
			void set_id_range(int id)
			{
				id_range = id;
			}

			int start_id;
			int id_range;

		public:		
			/// 
			/// \brief Visit all members of this instance given an object and a visitor.
			/// 
			/// Visits every member in this type, with the given visitor and object.
			/// 
			/// Returns false if the object passed in does not derive from this type.
			/// 
			virtual bool visit(class_base* object, visitor_base& visitor) = NULL;

			extensionpoint class_type_body;
		};

		/// 
		/// \brief Base class for opC++ opstruct types.
		/// 
		/// Provides type related functions,
		/// as well as external type and field access.
		///
		class struct_type : public type_base
		{
		public:
			/// 
			/// \brief Get the next (sibling) type.
			/// 
			/// Returns NULL if no sibling exists.
			/// 
			struct_type* get_next()
			{
				return next_type;
			}

			/// 
			/// \brief Get the child type.
			/// 
			/// Returns NULL if no child types exist.
			/// 
			struct_type* get_child()
			{
				return child_type;
			}

			/// 
			/// \brief Get the parent type.
			/// 
			/// Returns NULL if no parent types exist.
			/// This means the opclass had no parent listed, 
			/// it still derives from class_base however.
			/// 
			struct_type* get_parent()
			{
				return parent_type;
			}

			/// 
			/// \brief Create a new default constructed instance of this type.
			/// 
			/// This function creates a new instance of the type.
			/// 
			/// If a class is not default constructable, it will return NULL.
			/// 
			/// This function returns a struct_base*, but since opstruct
			/// is not cast-able, you will need to unsafely assign this to
			/// a pointer of the known correct type.
			/// See binary_load_archiver for an example of this.
			/// 
			virtual struct_base* new_instance() = NULL;

			/// 
			/// \brief Is this opstruct serializable?
			/// 
			/// Returns whether this struct is serializable.
			/// 
			virtual bool is_serializable() = NULL;

		protected:
			/// 
			/// Internal automatic initialization of types.
			/// 
			void initialize()
			{
				static bool binitialized = false;

				if(!binitialized)
				{
					next_type		= NULL;
					child_type		= NULL;
					parent_type		= NULL;
					start_id		= -1;
					id_range		= 0;
				}

				binitialized = true;
			}

			///
			/// Internal registration of types.
			///
			void register_next(struct_type* entry)
			{
				if(next_type)
					entry->next_type = next_type;
				next_type = entry;
			}

			///
			/// Internal registration of types.
			///
			void register_child(struct_type* entry)
			{
				if(!child_type)
					child_type = entry;
				else
				{
					entry->register_next(child_type);
					child_type = entry;

					// insertion sort by name
// 					struct_type* prev = 0;
// 					struct_type* current = child_type;
// 
// 					while( current && OPCPP_STRCMP(current->get_name(),entry->get_name()) < 0 )
// 					{
// 						prev = current;
// 						current = current->next_type;
// 					}
// 
// 					if(!prev)
// 					{
// 						entry->register_next(child_type);
// 						child_type = entry;
// 					}
// 					else
// 						prev->register_next(entry);
				}
			}

			///
			/// Internal registration of types.
			///
			void register_parent(struct_type* parent, struct_type* entry)
			{
				parent->register_child(entry);
			}

			///
			/// Internal registration of types.
			///
			inline void register_class(struct_type* entry);

			struct_type* next_type;
			struct_type* child_type;
			struct_type* parent_type;

			friend class struct_tracker;

		public:
			///
			/// \brief Get the opstruct id.
			/// 
			int get_id() const
			{
				return start_id;
			}

			///
			/// \brief Get the opstruct id range.
			///
			/// Inclusive range size of this struct and all child structs.
			///
			int get_id_range() const
			{
				return id_range;
			}

			/// 
			/// \brief Does this type contain this id? 
			/// 
			bool contains_id(int id) const
			{
				//fast integer range check:
				//from book Hackers Delight
				return (unsigned int)(id - start_id) <= (unsigned int)(id_range);
				//equivalent to:
				//if(id > instance.end_id
				//|| id < instance.start_id)
				//	return false;
				//return true;
			}


			/// 
			/// \brief Is this type a child of (or same as) another type? 
			/// 
			/// Use to quickly verify inheritance of types.
			///
			bool is_child_of(const struct_type& type) const
			{
				return type.contains_id(get_id());
			}

		private:
			/// 
			/// Internal id generation.
			/// 
			void set_id(int id)
			{
				start_id = id;
			}

			/// 
			/// Internal id generation.
			/// 
			void set_id_range(int id)
			{
				id_range = id;
			}

			int start_id;
			int id_range;

		public:		

			// 
			// \brief Visit all members of this instance given a boxed struct and a visitor.
			// 
			// Visits every member in this type, with the given visitor and boxed struct.
			// 
			// Returns false if the boxed struct passed in is not of this type.
			// 
			//virtual bool visit(const boxed_struct& boxed, visitor_base& visitor) = NULL;

			extensionpoint struct_type_body;
		};

		//Tracker singleton
		// * needs to handle registrations
		// * needs to be able to sort out children, next, parent
		// * preferably should be able to do this without any help
		// * needs to be able to initiate post-registration stuff (must be called by program)

		/// 
		/// \brief Singleton for external opclass type access.
		///  
		/// Allows you to access all registered opclass types.
		/// 
		class OPCPP_API class_tracker
		{
		public:
			static void initialize()
			{
				class_tracker* tracker = get_instance();

				tracker->generate_ids();
			}

			///
			/// \brief Gets a type given a name string.
			/// 
			/// If you pass it an unknown name it will return NULL.
			/// 
			/// Note: this function is currently linear speed depending on the number of types.
			/// 
			/// This currently does not handle fully qualified names.
			/// 
			static class_type* get_type(const std::string& name)
			{
				if(class_type* first_type = get_instance()->first_type)
					return get_type_utility(first_type,name);
				return NULL;
			}

			///
			/// \brief Gets a type given an id.
			/// 
			/// If you pass it an unknown id it will return NULL.
			/// 
			/// Note: this function is currently close to linear speed depending on the number of types,
			///       it is faster than the string lookup version due to id range checking.
			/// 
			static class_type* get_type(int id)
			{
				if(class_type* first_type = get_instance()->first_type)
					return get_type_utility(first_type,id);
				return NULL;
			}

			/// 
			/// \brief Gets the root registered type.
			/// 
			/// You can use this type to manually navigate the type tree.
			/// Use the class_type::get_next(), class_type::get_parent(), and class_type::get_child() functions
			/// for navigation.
			/// 
			static class_type* get_first_type()
			{
				return get_instance()->first_type;
			}

		private:

			static class_tracker* get_instance()
			{
				static class_tracker instance;
				return &instance;
			}

			static class_type* get_type_utility(class_type* current, const std::string& name)
			{
				while(current)
				{
					if(current->get_name() == name)
						return current;

					class_type* result = get_type_utility(current->get_child(),name);
					if(result)
						return result;

					current = current->get_next();
				}

				return NULL;
			}

			static class_type* get_type_utility(class_type* current, int id)
			{
				while(current)
				{
					if(current->contains_id(id))
					{
						if(current->get_id() == id)
							return current;

						class_type* result = get_type_utility(current->get_child(),id);
						if(result)
							return result;
					}

					current = current->get_next();
				}

				return NULL;
			}

			class_tracker()
				: 
				first_type(NULL),
				binitialized(false)
			{}

			void register_class(class_type* entry)
			{
				//only want to store classes w/o parents, since the others are reachable.
				if(entry->get_parent())
					return;

				if(!first_type)
				{
					first_type = entry;
				}
				else
				{
					//entry->register_next(first_type);
					//first_type = entry;
					
					// insertion sort by name
					class_type* prev = 0;
					class_type* current = first_type;

					while( current && strcmp(current->get_name(),entry->get_name()) < 0 )
					{
						prev = current;
						current = current->next_type;
					}

					if(!prev)
					{
						entry->register_next(current);
						first_type = entry;
					}
					else
						prev->register_next(entry);
				}
			}

			void generate_ids()
			{
				int id = 0;
				generate_ids(first_type,id);
			}

			void generate_ids(class_type* type, int& id)
			{
				class_type* current = type;

				while(current)
				{
					int start_id = id;
					current->set_id(start_id);
					id++;

					if(class_type* child = current->get_child())
						generate_ids(child,id);

					current->set_id_range(id-1-start_id);

					current = current->get_next();
				}
			}

			class_type* first_type;
			bool		binitialized;

			friend class class_type;
		};

		inline void class_type::register_class(class_type* entry)
		{
			class_tracker::get_instance()->register_class(entry);
		}

		/// 
		/// \brief Singleton for external opstruct type access.
		///  
		/// Allows you to access all registered opstruct types.
		/// 
		class OPCPP_API struct_tracker
		{
		public:
			static void initialize()
			{
				struct_tracker* tracker = get_instance();

				tracker->generate_ids();
			}

			///
			/// \brief Gets a type given a name string.
			/// 
			/// If you pass it an unknown name it will return NULL.
			/// 
			/// Note: this function is currently linear speed depending on the number of types.
			/// 
			/// This currently does not handle fully qualified names.
			/// 
			static struct_type* get_type(const std::string& name)
			{
				if(struct_type* first_type = get_instance()->first_type)
					return get_type_utility(first_type,name);
				return NULL;
			}

			/// 
			/// \brief Gets the root registered type.
			/// 
			/// You can use this type to manually navigate the type tree.
			/// 
			/// Use the struct_type::get_next(), struct_type::get_parent(), and struct_type::get_child() functions
			/// for navigation.
			/// 
			static struct_type* get_first_type()
			{
				return get_instance()->first_type;
			}

		private:
			void generate_ids()
			{
				int id = 0;
				generate_ids(first_type,id);
			}

			void generate_ids(struct_type* type, int& id)
			{
				struct_type* current = type;

				while(current)
				{
					int start_id = id;
					current->set_id(start_id);
					id++;

					if(struct_type* child = current->get_child())
						generate_ids(child,id);

					current->set_id_range(id-1-start_id);

					current = current->get_next();
				}
			}

			static struct_tracker* get_instance()
			{
				static struct_tracker instance;
				return &instance;
			}

			static struct_type* get_type_utility(struct_type* current, const std::string& name)
			{
				while(current)
				{
					if(current->get_name() == name)
						return current;

					struct_type* result = get_type_utility(current->get_child(),name);
					if(result)
						return result;

					current = current->get_next();
				}

				return NULL;
			}

			struct_tracker()
				: 
				first_type(NULL),
				binitialized(false)
			{}

			void register_class(struct_type* entry)
			{
				//only want to store classes w/o parents, since the others are reachable.
				if(entry->get_parent())
					return;

				if(!first_type)
				{
					first_type = entry;
				}
				else
				{
					entry->register_next(first_type);
					first_type = entry;

					// insertion sort by name
// 					struct_type* prev = 0;
// 					struct_type* current = first_type;
// 
// 					while( current && OPCPP_STRCMP(current->get_name(),entry->get_name()) < 0 )
// 					{
// 						prev = current;
// 						current = current->next_type;
// 					}
// 
// 					if(!prev)
// 					{
// 						entry->register_next(current);
// 						first_type = entry;
// 					}
// 					else
// 						prev->register_next(entry);
				}
			}

			struct_type* first_type;
			bool		 binitialized;

			friend class struct_type;
		};

		inline void struct_type::register_class(struct_type* entry)
		{
			struct_tracker::get_instance()->register_class(entry);
		}

		namespace detail
		{
			struct initialize_ids
			{
				initialize_ids()
				{
					class_tracker::initialize();
					struct_tracker::initialize();
				}
			};
		}
	}
}

//initialize the id's for this glob (may happen multiple times per class...)
code sourcefooter
{
	namespace opcpp
	{
		namespace registration
		{
			namespace detail
			{
				initialize_ids initids;
			}
		}
	}
};

opmacro add_registration(category_name,registration_type,type_base,get_registration_type)
{
	//needs to be able to self-register
	category category_name
	{
		location body
		{
			note getclass;
		}

		location source
		{
			note getclass;
			note register;
		}
	};

	note category_name::body::getclass(scope,class_name)
	{
	public:
		opcpp::registration::registration_type& get_type();
	}

	note category_name::source::getclass(alt_scope,scope,alt_class_name,class_name)
	{
		opcpp::registration::registration_type& scope::class_name::get_type()
		{
			return *opcpp::registration alt_scope::alt_class_name::type::get_instance();
		}
	}

	note category_name::source::register(alt_namespace_start,alt_namespace_end,scope,class_name,alt_class_name)
	{
		namespace opcpp
		{
			namespace registration
			{
				alt_namespace_start

				namespace alt_class_name
				{
					using opcpp::fields::data_field;

					type type::instance;

					const char* type::get_name()
					{
						return ``class_name``;
					}

					size_t type::get_field_count()
					{
						return total_field_count;
					}

					data_field* type::get_field(size_t index)
					{
						return static_get_field(index);
					}

					data_field* type::get_field(const char* name)
					{
						return static_get_field(name);
					}
				}

				alt_namespace_end
			}
		}
	}

	//registration prototype - needed for inner types
	category category_name
	{
		location heading
		{
			note registrationtype
			{
				is inner_type;
			}
		}
	}

	//prototypes - only reason is for friend declarations.
	note category_name::heading::registrationtype(alt_namespace_start,alt_namespace_end,alt_class_name)
	{
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
			}
		}
	}


	//generate static_get_field

	category category_name
	{
		location source
		{
			datamap static_get_field
			{
				is not native;
			}
		}
	};

	note category_name::source::static_get_field::start(alt_scope,alt_class_name)
	{
		opcpp::fields::data_field* opcpp::registration alt_scope::alt_class_name::type::static_get_field(size_t index)
		+{}
			if(index < parent_field_count)
				return ParentGetType::static_get_field(index);//TODO: fix...

			switch(index)
			+{}
	}

	note category_name::source::static_get_field::mapping(member_name,alt_scope,alt_class_name,map_index)
	{
				case (parent_field_count + map_index):
					return opcpp::fields alt_scope::alt_class_name::member_name::get_instance();
					break;
	}

	note category_name::source::static_get_field::end(map_count)
	{
				case (parent_field_count + map_count):
					break;
			-{}

			return NULL;
		-{}
	}


	//generate static_visit
	category category_name
	{
		location source
		{
			datamap static_visit
			{
				is not native;
			}
		}
	};

	note category_name::source::static_visit::start(alt_scope,scope,class_name,alt_class_name)
	{
		void opcpp::registration alt_scope::alt_class_name::type::static_visit(scope::class_name* object, ::opcpp::base::visitor_base& visitor)
		+{}
			ParentGetType::static_visit(object,visitor);
	}

	note category_name::source::static_visit::mapping(member_name,alt_scope,alt_class_name,map_index)
	{
		{
			opcpp::fields alt_scope::alt_class_name::member_name::static_visit(object,visitor);
		}
	}

	note category_name::source::static_visit::end(map_count)
	{
		-{}
	}

	//new_instance support functions

	//does the type have default construction available
	category category_name
	{
		location footer
		{
			note has_default_constructor
			{
				is category_has_simple_constructor
				or not category_has_constructor;
			}
		}
	};

	note category_name::footer::has_default_constructor(scope,class_name)
	{
		namespace opcpp
		{
			namespace metadata
			{
				template<>
				struct has_default_constructor< scope::class_name >
				{
					enum { value = true };
				};
			}
		}
	}

	// generate field init
	category category_name
	{
		location source
		{
			note get_field_name;
		}
	};

	note category_name::source::get_field_name(alt_scope,alt_class_name)
	{
		opcpp::fields::data_field* opcpp::registration alt_scope::alt_class_name::type::static_get_field(const char* name)
		{
			//could definitely be faster ( sorted array ? )
			for(size_t i = 0; i < total_field_count; i++)
			{
				::opcpp::fields::data_field* field = static_get_field(i);

				if(field->member_name() == std::string(name))
					return field;
			}

			return NULL;
		}
	}
};
