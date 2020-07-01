//****************************************************************
// Copyright © 2008 opGames LLC - All Rights Reserved
//
// Authors: Lucas Ellis
//
// File: openum.doh
// Date: 07/25/2007
//
// Description:
//
// openum
//****************************************************************

//
// Declare openum enumeration
//

enumprefix(op) enumeration openum
{
	location heading
	{
		hidden note prototype
		{
			is not inner_type;
		}
	}

	location body
	{
		enummap definitions;
		note    constructors;
		note    storage;
		note    functions;
		enummap count;
	}

	location source
	{
		hidden enummap tostring;
		note fromstring;

		hidden enummap toint;
		hidden enummap fromint;
	}

	location post
	{
		enummap globals;
	}
};

note openum::heading::prototype(enum_type,namespace_start,namespace_end)
{
	namespace_start

	struct enum_type;

	namespace_end
}

note openum::identifier()
{
	struct
}

namespace opcpp
{
	namespace base
	{
		/// 
		/// \brief Base type for openums.
		/// 
		/// Empty base-struct for openum.
		/// 
		/// openum is implemented as a combination of typesafe struct and external enum definitions.
		/// 
		struct enum_base
		{
		};
	}
}

note openum::parent()
{
	: public opcpp::base::enum_base
}


note openum::body::definitions::start()
{
public:
	enum type
		+{}
}
note openum::body::definitions::name(enum_name)
{
	enum_name,
}
note openum::body::definitions::value(enum_name,enum_value)
{
	enum_name = enum_value,
}
note openum::body::definitions::end()
{
	-{};
}

// count
note openum::body::count::name()
{
	
}
note openum::body::count::value()
{
	
}
note openum::body::count::end(map_count)
{
	enum { count = map_count };
}

//openum's default to end???
note openum::body::constructors(enum_type)
{
public:
	enum_type() : data((type)min) {}
	enum_type(const type& e) : data(e) {}
	enum_type(const enum_type& e) : data(e.data) {}
}

note openum::body::functions(enum_type)
{
public:
	int to_int() const
	{
		return (int)data;
	}	
	
	//sets the string representation if valid
	bool to_string(std::string& s) const;

	//returns the string version of the enum
	std::string get_string() const
	{
		std::string s;

		to_string(s);

		return s;
	}

	//returns the int version of the enum
	int get_int() const
	{
		int i;

		to_int(i);

		return i;
	}
	
	//sets if success, returns false if failed
	bool from_string(const std::string& s);
	
	//sets the int representation if valid
	bool to_int(int& i) const;
	
	//sets if success, returns false if failed
	bool from_int(const int& i);
	
	/*friend bool operator==(const enum_type& a, const enum_type& b)
	{
		return a.data == b.data;
	}
	
	friend bool operator!=(const enum_type& a, const enum_type& b)
	{
		return a.data != b.data;
	}*/

	bool operator==(const enum_type& other) const
	{
		return data == other.data;
	}

	bool operator!=(const enum_type& other) const
	{
		return data != other.data;
	}
	
	bool operator < (const enum_type& other) const
	{
		return data < other.data;
	}
	
	bool operator > (const enum_type& other) const
	{
		return data > other.data;
	}
	
	bool operator <= (const enum_type& other) const
	{
		return data <= other.data;
	}
	
	bool operator >= (const enum_type& other) const
	{
		return data >= other.data;
	}
	
	int key_count() const
	{
		return total_key_count;
	}
	
	const char* key_string(int index) const
	{
		return key_table[index].text;
	}
	
	type key_value(int index) const
	{
		return key_table[index].key;
	}
}

note openum::source::tostring::start(scope,namespace_start,enum_type)
{
	namespace_start

	inline bool scope::enum_type::to_string(std::string& s) const
	+{}

		switch(data)
		+{}
}

note openum::source::tostring::name(enum_name)
{
			case enum_name: s = ``enum_name``; return true;
}

note openum::source::tostring::value(enum_name)
{
			case enum_name: s = ``enum_name``; return true;
}

note openum::source::tostring::end(namespace_end)
{
		-{}

	return false;	
	-{}

	namespace_end
}

namespace opcpp
{
	namespace base
	{
		namespace detail
		{
			template< class type >
			struct enum_key
			{
				type		key;
				const char* text;
			};
		}
	}
}

//generate mapping array
enumeration openum
{
	location body
	{
		enummap keycount;
		note mappingarray;
	}

	location source
	{
		enummap mappingarray;
	}
};

note openum::body::keycount::name()
{
}

note openum::body::keycount::value()
{
}

note openum::body::keycount::end(map_count)
{
public:
	enum { total_key_count = map_count };
}

note openum::body::mappingarray()
{
public:
	typedef opcpp::base::detail::enum_key< type > key_table_type;

private:
	static key_table_type key_table[total_key_count];
};

note openum::source::mappingarray::start(scope,enum_type)
{
	scope::enum_type::key_table_type
	(scope::enum_type::key_table)[] = 
	+{}
}

note openum::source::mappingarray::name(scope,enum_type,enum_name)
{
	{
		scope::enum_type::enum_name,
		``enum_name``
	},
}

note openum::source::mappingarray::value(scope,enum_type,enum_name)
{
	{
		scope::enum_type::enum_name,
		``enum_name``
	},
}


note openum::source::mappingarray::end()
{
	-{};
}

note openum::source::fromstring(scope,enum_type,namespace_start,namespace_end)
{
	namespace_start
	
	inline bool scope::enum_type::from_string(const std::string& s)
	{
		if(!s.length())
			return false;
		
		for(int i = 0; i < total_key_count; i++)
		{
			if(s == key_table[i].text)
			{
				data = key_table[i].key;
				return true;
			}
		}

		return false;
	}
	
	namespace_end
}

note openum::source::toint::start(scope,namespace_start,enum_type)
{
	namespace_start

	inline bool scope::enum_type::to_int(int& i) const
	+{}
		switch(data)
		+{}
}

note openum::source::toint::name(enum_name)
{
			case enum_name: i = (int)enum_name; return true;
}

note openum::source::toint::value(enum_name)
{
			case enum_name: i = (int)enum_name; return true;
}

note openum::source::toint::end(namespace_end)
{
		-{}
	
		return false;	
	-{}

	namespace_end
}


note openum::source::fromint::start(scope,namespace_start,enum_type)
{
	namespace_start

	inline bool scope::enum_type::from_int(const int& i)
	+{}

		type value = (type)i;

		if(value < (type)min || value > (type)max)
			return false;

		switch(value)
		+{}

}

note openum::source::fromint::name(enum_name)
{
		case enum_name:
			data = enum_name;
			return true;
}

note openum::source::fromint::value(enum_name)
{
		case enum_name:
			data = enum_name;
			return true;
}

note openum::source::fromint::end(namespace_end)
{
		-{}
		return false;
	-{}

	namespace_end
}

note openum::body::storage()
{
public:
	const type& get_data() const 
	{ 
		return data; 
	}

private:
	type data;
}

note openum::post::globals::start()
{
}

note openum::post::globals::name(enum_type,enum_name)
{
	const static enum_type::type enum_name = enum_type::enum_name;
}

note openum::post::globals::value(enum_type,enum_name)
{
	const static enum_type::type enum_name = enum_type::enum_name;
}

note openum::post::globals::end()
{
}


//
// openum metadata
//

namespace opcpp
{
	namespace metadata
	{
		template<class type>
		struct is_openum
		{
			enum { value = false };
		};
	}
}

enumeration openum
{
	location footer
	{
		note metadata;
	}
}

note openum::footer::metadata(enum_type,scope)
{
	namespace opcpp
	{
		namespace metadata
		{
			template<>
			struct is_openum< scope::enum_type >
			{
				enum { value = true };
			};
				
			//convertible to openum anyways
			template<>
			struct is_openum< scope::enum_type::type >
			{
				enum { value = true };
			};
		}
	}
}



enumeration openum
{
	location body
	{
		enummap min;
		enummap minend;
		enummap max;
		enummap maxend;
	}

}

namespace opcpp
{
	namespace metafunction
	{

		template<int A, int B, bool bUseA = false>
		struct min
		{
			enum 
			{
				RET = A < B ? A : B
			};
		};

		//fake min, always A
		template<int A, int B>
		struct min<A,B,true>
		{
			enum 
			{
				RET = A
			};
		};

		template<int A, int B, bool bUseA = false>
		struct max
		{
			enum 
			{
				RET = A > B ? A : B
			};
		};


		//fake max, always A
		template<int A, int B>
		struct max<A,B,true>
		{
			enum 
			{
				RET = A
			};
		};
	}
}

// 
//  min start: 
// 	min = 
//  min name/value:
// 	opcpp::metafunction::min<Mangoes,
// 	opcpp::metafunction::min<Mangoes,
//  minend start:
//  end
//  minend name/value:
//  >::RET
//  >::RET


note openum::body::min::start()
{
public:
	enum min_type
	+{}
		min = (type)
}

note openum::body::min::name(enum_name)
{
	opcpp::metafunction::min<(int)enum_name,
}
note openum::body::min::value(enum_name)
{
	opcpp::metafunction::min<(int)enum_name,
}
note openum::body::min::end()
{	
	0,false
}

note openum::body::minend::start()
{
}
note openum::body::minend::name()
{
	>::RET
}
note openum::body::minend::value()
{
	>::RET
}
note openum::body::minend::end(enum_type)
{
	-{};

	enum_type(const min_type& m) : data((type)m) {}
}

// max start: 
//	max = 
// max name/value:
//	opcpp::metafunction::max<Mangoes,
//	opcpp::metafunction::max<Mangoes,
// max end:
// 0,false
// maxend name/value:
// >::RET
// >::RET

note openum::body::max::start()
{
public:
	enum max_type
	+{}
		max =
}

note openum::body::max::name(enum_name)
{
	opcpp::metafunction::max<(int)enum_name,
}
note openum::body::max::value(enum_name)
{
	opcpp::metafunction::max<(int)enum_name,
}
note openum::body::max::end()
{
	0,false
}

note openum::body::maxend::start()
{
}
note openum::body::maxend::name()
{
	>::RET
}
note openum::body::maxend::value()
{
	>::RET
}
note openum::body::maxend::end(enum_type)
{
	-{};

	enum_type(const max_type& m) : data((type)m) {}

}

//==========================================
// Generate an openum id
//==========================================

namespace opcpp
{
	namespace metadata
	{
		template<class type>
		struct id
		{
			static void* get_id()
			{
				return NULL;
			}
		};
	}
}

enumeration openum
{
	location footer
	{
		note staticid
		{
			is not inner_type;
		}
	}

	location source
	{
		note staticid
		{
			is not inner_type;
		}
	}
};

note openum::footer::staticid(scope,enum_type)
{
	namespace opcpp
	{
		namespace metadata
		{
			template<>
			struct OPCPP_API id< scope::enum_type >
			{
				static void* get_id()
				{
					return &static_id;
				}

			private:
				static bool static_id;
			};

			template<>
			struct id< scope::enum_type::type >
			{
				static void* get_id()
				{
					return id< scope::enum_type >::get_id();
				}
			};
		}
	}
}

note openum::source::staticid(scope,enum_type)
{
	bool opcpp::metadata::id< scope::enum_type >::static_id = true;
}

enumeration openum
{
	location footer
	{
		note has_default_constructor;
	}
};

//NOTE: move to sourceheading ?
note openum::footer::has_default_constructor(scope,enum_type)
{
	namespace opcpp
	{
		namespace metadata
		{
			template<>
			struct has_default_constructor< scope::enum_type >
			{
				enum { value = true };
			};
		}
	}
}


//
// Registration for openums
//

namespace opcpp
{
	namespace registration
	{
		/// 
		/// \brief Base class for opC++ openum types.
		/// 
		/// Provides type related functions,
		/// as well as external enum value access.
		///
		class enum_type
		{
		public:
			/// 
			/// \brief Get the next (sibling) type.
			/// 
			/// Returns NULL if no sibling exists.
			/// 
			enum_type* get_next()
			{
				return next_type;
			}

			///
			/// \brief Returns the name of the type.
			///
			virtual const char* get_name() = NULL;

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
				}

				binitialized = true;
			}
			
			///
			/// Internal registration of types.
			///
			void register_next(enum_type* entry)
			{
				//should always switch from NULL to entry
				next_type = entry;
			}
			
			///
			/// Internal registration of types.
			///
			inline void register_class(enum_type* entry);
			
			enum_type* next_type;
			
			friend class enum_tracker;
		};
		
		/// 
		/// \brief Singleton for external openum type access.
		///  
		/// Allows you to access all registered openum types.
		/// 
		class OPCPP_API enum_tracker
		{
		public:
			///
			/// \brief Gets a type given a name string.
			/// 
			/// If you pass it an unknown name it will return NULL.
			/// 
			/// Note: this function is currently linear speed depending on the number of types.
			/// 
			/// This currently does not handle fully qualified names.
			/// 
			static enum_type* get_type(const std::string& name)
			{
				if(enum_type* first_type = get_instance()->first_type)
					return get_type_utility(first_type,name);
				return NULL;
			}

			/// 
			/// \brief Gets the root registered type.
			/// 
			/// You can use this type to manually navigate the type list.
			/// 
			/// Use the enum_type::get_next() function
			/// for navigation.
			/// 
			static enum_type* get_first_type()
			{
				return get_instance()->first_type;
			}

		private:

			static enum_tracker* get_instance()
			{
				static enum_tracker instance;
				return &instance;
			}

			static enum_type* get_type_utility(enum_type* current, const std::string& name)
			{
				while(current)
				{
					if(current->get_name() == name)
						return current;

					current = current->get_next();
				}

				return NULL;
			}

			enum_tracker()
				: 
				first_type(NULL),
				binitialized(false)
			{}

			void register_class(enum_type* type)
			{
				if(!first_type)
				{
					first_type = type;
				}
				else
				{
					type->register_next(first_type);
					first_type = type;
				}
			}

			enum_type*	first_type;
			bool		binitialized;

			friend class enum_type;
		};

		inline void enum_type::register_class(enum_type* entry)
		{
			enum_tracker::get_instance()->register_class(entry);
		}

		template<class type>
		struct get_enum_type
		{
			static enum@_type* get() { return NULL; }
		};
	}
}


//
// Registration Implementation
//

enumeration openum
{
	location footer
	{
		note get_type;
	}

	location body
	{
		note get_type;
	}

	location source
	{
		note get_type;
	}

	location sourceheading
	{
		note registration;
	}
};

note openum::footer::get_type(scope,enum_type)
{
	namespace opcpp
	{
		namespace registration
		{
			template<>
			struct OPCPP_API get_enum_type< scope::enum_type >
			{
				static enum@_type* get();
			};
		}
	}
}

note openum::body::get_type()
{
public:
	opcpp::registration::enum_type& get_type();
}

note openum::source::get_type(alt_scope,scope,enum_type)
{
	opcpp::registration::enum@_type& scope::enum_type::get_type()
	{
		return *opcpp::registration::get_enum_type<enum_type>::get();
	}
}

note openum::sourceheading::registration(alt_scope,scope,enum_type,alt_namespace_start,alt_namespace_end)
{
	//generate a class registration thing
	namespace opcpp
	{
		namespace registration
		{
			alt_namespace_start

			namespace _@enum_type
			{
				using ::opcpp::registration::enum@_type;
				
				class type : public enum@_type
				{
				public:
					//
					// compile time types and constants
					//
					
					//
					// virtual functions
					//
					
					//this gets the name of the type
					virtual const char* get_name()
					{
						return ``enum_type``;
					}
					
					//
					// static functions
					//
					
					static type* get_instance()
					{
						return &instance;
					}
					
				private:
					
					void register_class()
					{
						enum@_type::register_class(&instance);
					}
					
					type()
					{
						initialize();
						register_class();
					}
					
				private:
					static type instance;
				};
				
				//initialize static instance in source
				type type::instance;
			}
			
			alt_namespace_end
			
			enum@_type* get_enum_type< scope::enum_type >::get()
			{
				return opcpp::registration alt_scope::_@enum_type::type::get_instance();
			}
		}
	}
}



