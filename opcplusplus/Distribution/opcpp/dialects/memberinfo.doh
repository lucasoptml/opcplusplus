//****************************************************************
// Copyright © 2008 opGames LLC - All Rights Reserved
//
// Authors: Lucas Ellis
//
// File: memberinfo.doh
// Date: 07/25/2007
//
// Description:
//
// member_info accessor support header
//****************************************************************

namespace opcpp
{
	namespace accessors
	{
		namespace detail
		{
			//prototypes
			template<typename type> 
			struct selector;
			
			template<typename type>
			struct run_member_cast;
		}
		
		using detail::selector;
		using detail::run_member_cast;
		
		//using meta-programs
		using opcpp::fields::data_field;
		using opcpp::base::visitor_base;
		using opcpp::base::class_base;
		using opcpp::base::struct_base;
		using opcpp::casting::class_cast;
		using opcpp::fields::invalid_field;
		using opcpp::metadata::is_opclass;
		using opcpp::metadata::is_openum;
		using opcpp::metadata::is_opstruct;
		using opcpp::metadata::has_default_constructor;
		using opcpp::metadata::is_serializable;
		using opcpp::metadata::is_bytes;
		using opcpp::metadata::is_basic;
		using opcpp::metadata::is_unsigned;
		using opcpp::metafunction::reference_wrapper;
		using opcpp::metafunction::wraptype;
		using opcpp::metafunction::IF;
		using opcpp::metafunction::remove_pointer;
		using opcpp::metafunction::is_pointer;
		using opcpp::metafunction::remove_cv;
		using opcpp::metafunction::pointer_from_void;
		using opcpp::registration::get_class_type;
		using opcpp::registration::get_struct_type;
		using opcpp::registration::get_enum_type;
		using opcpp::registration::class_type;
		using opcpp::registration::struct_type;
		using opcpp::registration::enum_type;
	}

	namespace casting
	{
		using opcpp::accessors::member_info;

		namespace detail
		{
			template<class type>
			struct info_cast_visitor : public opcpp::base::visitor_base
			{
				info_cast_visitor(member_info* info);

				void visit(type& info)
				{
					bresult = true;
				}

				bool result()
				{
					return bresult;
				}

			private:
				bool bresult;
			};
		}

		using detail::info_cast_visitor;

		template<class type>
		type* info_cast(member_info* info);
		
		template<class type>
		type* info_cast(member_info& info);
	}

	namespace accessors
	{
		using opcpp::casting::info_cast;

		/// \brief Base accessor type.
		/// 
		/// All accessors derive from member_info.
		/// Implements all the universally supplied methods including
		/// field access, size, address, and casting.
		/// 
		struct member_info
		{
			member_info(const data_field& f) : field(f)
			{}

			///
			/// \brief Creates a new copy of this accessor on the heap.
			///
			/// Uses the new operator, so be sure to delete!
			/// 
			/// Use of heap accessors is discouraged.
			/// 
			virtual member_info* new_copy() = NULL;

			/// 
			/// \brief Given a visitor, calls visit on it with the appropriate overload.  
			/// 
			/// This should call the exact visit overload for the accessor's type.
			/// 
			virtual void visit(visitor_base& visitor) = NULL;

			/// 
			/// \brief Get the member name for an accessor.  
			/// 
			/// This will be the C++
			/// name of the data member, or the specified value of
			/// the opreflect() modifier.
			/// 
			/// If the accessor has no underlying field it will return NULL.
			/// This can happen anytime you're using an accessor for an
			/// inner type.  For example, the inner accessor of a container.
			/// 
			const char* member_name()
			{
				return get_field().member_name();
			}

			/// 
			/// \brief Is the field named / valid?  
			/// 
			/// If the accessor has no underlying field it will return false.
			/// This can happen anytime you're using an accessor for an
			/// inner type.  For example, the inner accessor of a container.
			/// 
			bool is_field_valid()
			{
				return get_field().is_valid();
			}

			//get the type name, if available
			//TODO: implement using a visitor
			//const char* type_name() { return "" };

			/// 
			/// \brief Get the unsafe void* address of the accessor's data.
			/// 
			/// This gets the address of the accessor's data. 
			///
			virtual void* address() = NULL;

			/// 
			/// \brief Get the size of the accessor's data type.
			/// 
			/// This gets the sizeof() of the accessor's data type.
			/// 
			virtual size_t type_size() = NULL;

			///
			/// \brief Attempt to cast the accessor's data to a certain type.
			/// 
			/// This will safely return NULL if the cast failed.
			/// If it succeeds it will return a valid pointer to the data type.
			/// 
			/// Generally this only works if casting to the exact
			/// matching type.
			/// 
			/// Usage: 
			/// \code 
			/// int* data = accessor.cast< int >();
			/// 
			/// //if the pointer is valid we have access to the int.
			/// if( data != NULL )
			///		*data = 100;
			/// \endcode
			/// 
			template<typename type>
			type* cast();

			/// 
			/// \brief Is the accessor's field marked transient?
			/// 
			/// Returns whether the accessor's field is marked transient.
			/// 
			bool is_transient()
			{
				return get_field().is_transient();
			}

			/// 
			/// \brief Attempt to cast this accessor to another accessor type.
			/// 
			/// You can also use the function opcpp::casting::info_cast() to do this.
			///
			/// Usage: 
			/// \code
			/// int_info* i = accessor.to< int_info >();
			/// \endcode
			/// 
			template<class infotype>
			infotype* to()
			{
				return info_cast<infotype>(this);
			}

		protected:
			template<typename infotype, typename type>
			void call_set_data(infotype& info, type data)
			{
				info.set_data( data );
			}

		public:

			/// 
			/// \brief Get the data field.
			///
			/// It may be an invalid field, although you can still safely access the default invalid field instance.
			/// 
			const data_field& get_field()
			{
				return field;
			}

		protected:
			const data_field& field;
		};
	}

	namespace accessors
	{
		using opcpp::casting::info_cast_visitor;

		namespace detail
		{
			struct no_member_cast
			{
				static bool can_cast(member_info* info)
				{
					return false;
				}
			};

			template<typename type>
			struct struct_member_cast
			{
				static bool can_cast(member_info* info);
			};
			
			template<typename type>
			struct class_member_cast
			{
				static bool can_cast(member_info* info);
			};

			template<typename type>
			struct enum_member_cast
			{
				static bool can_cast(member_info* info);
			};

			template<typename type>
			struct bytes_member_cast
			{
				static bool can_cast(member_info* info);
			};

			template<typename type>
			struct select_member_cast
			{
				typedef
					typename
					IF< is_opclass<type>::value,
						//then
						class_member_cast<type>,
						//else
						typename
						IF< is_openum<type>::value,
							//then
							enum_member_cast<type>,
							//else
							typename
							IF< is_opstruct<type>::value,
								//then
								struct_member_cast<type>,
								//else
								no_member_cast
							>::RET
						>::RET
					>::RET

					selected;

				static bool can_cast(member_info* info)
				{
					return selected::can_cast(info);
				}
			};
			
			//remove cv specifiers
			template<typename type>
			struct run_member_cast
			{
				typedef select_member_cast< typename remove_cv<type>::type > selected;

				static bool can_cast(member_info* info)
				{
					return selected::can_cast(info);
				}
			};
		}

		using detail::run_member_cast;
			
		/// 
		/// \brief Accessor for all basic types.
		///
		/// This provides no functionality other than classifying the child accessor types as basic.
		/// 
		struct basic_info : member_info
		{
			basic_info(const data_field& f)
				:
				member_info(f)
			{}
			
			template<typename type>
			type* cast()
			{
				if(run_member_cast<type>::can_cast(this))
					return (type*)address();
				return NULL;
			}
		};

		/// 
		/// \brief Accessor for C++ float data.
		/// 
		/// Provides direct access to reflected floats.
		/// 
		struct float_info : basic_info
		{
			typedef basic_info super;

			float_info(const data_field& f, float& indata) 
				: 
				super(f),
				data(&indata) 
			{}

			float_info() 
				: 
				super(invalid_field::get_instance()),
				data(NULL) 
			{}

			size_t type_size()
			{
				return sizeof(float);
			}
			
			void visit(visitor_base& visitor)
			{
				visitor.visit(*this);
			}

			/// 
			/// \brief Get a reference to the float data.
			/// 
			float& get_float()
			{
				return *data;
			}

			void* address()
			{
				return data;
			}

			member_info* new_copy()
			{
				return new float_info(*this);
			}

			template<typename type>
			type* cast()
			{
				if(run_member_cast<type>::can_cast(this))
					return (type*)address();
				return NULL;
			}

		private:
			void set_data(float* newdata)
			{
				data = newdata;
			}
			void set_data(const float* newdata)
			{
				data = const_cast<float*>(newdata);
			}

			friend struct member_info;

			float* data;
		};

		/// 
		/// \brief Accessor for C++ double data.
		/// 
		/// Provides direct access to reflected doubles.
		/// 
		struct double_info : basic_info
		{
			typedef basic_info super;

			double_info(const data_field& f, double& indata) 
				: 
				super(f),
				data(&indata)
			{}

			double_info() 
				: 
				super(invalid_field::get_instance()),
				data(NULL) 
			{}
			
			size_t type_size()
			{
				return sizeof(double);
			}

			virtual void visit(visitor_base& visitor)
			{
				visitor.visit(*this);
			}

			/// 
			/// \brief Get a reference to the double data.
			/// 
			double& get_double()
			{
				return *data;
			}

			virtual void* address()
			{
				return data;
			}

			virtual member_info* new_copy()
			{
				return new double_info(*this);
			}

			template<typename type>
			type* cast()
			{
				if(run_member_cast<type>::can_cast(this))
					return (type*)address();
				return NULL;
			}

		private:
			void set_data(double* newdata)
			{
				data = newdata;
			}
			void set_data(const double* newdata)
			{
				data = const_cast<double*>(newdata);
			}

			friend struct member_info;

			double* data;
		};

		/// 
		/// \brief Accessor for all text types.
		///
		/// This provides no functionality other than classifying the child accessor types as text.
		/// 
		struct text_info : basic_info
		{
			text_info(const data_field& f)
				:
				basic_info(f)
			{}


			/// 
			/// \brief Get the char size on the string.
			/// 
			virtual size_t char_size() = NULL;

			/// 
			/// \brief Get the char data pointer on the string.
			/// 
			virtual const void* char_data() = NULL;

			template<typename type>
			type* cast()
			{
				if(run_member_cast<type>::can_cast(this))
					return (type*)address();
				return NULL;
			}
		};

		/// 
		/// \brief Accessor for STL std::string data.
		/// 
		/// Provides direct access to reflected strings.
		/// 
		struct string_info : text_info
		{
			typedef text_info super;

			string_info(const data_field& f, std::string& indata)
				: 
				super(f),
				data(&indata)
			{}

			string_info() 
				:
				super(invalid_field::get_instance()),
				data(NULL) 
			{}

			size_t type_size()
			{
				return sizeof(std::string);
			}

			size_t char_size()
			{
				return sizeof(std::string::value_type);
			}

			const void* char_data()
			{
				return data->c_str();
			}
		
			virtual void visit(visitor_base& visitor)
			{
				visitor.visit(*this);
			}
			
			/// 
			/// \brief Get a reference to the string data.
			/// 
			std::string& get_string()
			{
				return *data;
			}
			
			virtual void* address()
			{
				return data;
			}
			
			virtual member_info* new_copy()
			{
				return new string_info(*this);
			}
			
			template<typename type>
			type* cast()
			{
				if(run_member_cast<type>::can_cast(this))
					return (type*)address();
				return NULL;
			}
			
		protected:
			void set_data(std::string* newdata)
			{
				data = newdata;
			}
			void set_data(const std::string* newdata)
			{
				data = const_cast<std::string*>(newdata);
			}

			friend struct member_info;

			std::string* data;
		};

		/// 
		/// \brief Accessor for STL std::wstring data.
		/// 
		/// Provides direct access to reflected strings.
		/// 
		struct wstring_info : text_info
		{
			typedef text_info super;

			wstring_info(const data_field& f, std::wstring& indata)
				: 
				super(f),
				data(&indata)
			{}

			wstring_info() 
				:
				super(invalid_field::get_instance()),
				data(NULL) 
			{}

			size_t type_size()
			{
				return sizeof(std::wstring);
			}

			size_t char_size()
			{
				return sizeof(std::wstring::value_type);
			}

			const void* char_data()
			{
				return data->c_str();
			}

			virtual void visit(visitor_base& visitor)
			{
				visitor.visit(*this);
			}

			/// 
			/// \brief Get a reference to the string data.
			/// 
			std::wstring& get_wstring()
			{
				return *data;
			}

			virtual void* address()
			{
				return data;
			}

			virtual member_info* new_copy()
			{
				return new wstring_info(*this);
			}

			template<typename type>
			type* cast()
			{
				if(run_member_cast<type>::can_cast(this))
					return (type*)address();
				return NULL;
			}

		protected:
			void set_data(std::wstring* newdata)
			{
				data = newdata;
			}
			void set_data(const std::wstring* newdata)
			{
				data = const_cast<std::wstring*>(newdata);
			}

			friend struct member_info;

			std::wstring* data;
		};


		/// 
		/// \brief Accessor for C++ int data.
		/// 
		/// Provides direct access to reflected ints.
		/// 
		struct int_info : basic_info
		{
			typedef basic_info super;

			int_info(const data_field& f, int& indata) 
				: 
				super(f),
				data(&indata) 
			{}

			int_info() 
				: 
				super(invalid_field::get_instance()),
				data(NULL) 
			{}

			size_t type_size()
			{
				return sizeof(int);
			}

			virtual void visit(visitor_base& visitor)
			{
				visitor.visit(*this);
			}

			/// 
			/// \brief Get a reference to the int data.
			/// 
			int& get_int()
			{
				return *data;
			}
			
			virtual void* address()
			{
				return data;
			}

			virtual member_info* new_copy()
			{
				return new int_info(*this);
			}

			template<typename type>
			type* cast()
			{
				if(run_member_cast<type>::can_cast(this))
					return (type*)address();
				return NULL;
			}

		private:
			void set_data(int* newdata)
			{
				data = newdata;
			}
			void set_data(const int* newdata)
			{
				data = const_cast<int*>(newdata);
			}

			friend struct member_info;

			int* data;
		};

		/// 
		/// \brief Accessor for C++ bool data.
		/// 
		/// Provides direct access to reflected bools.
		/// 
		/// Bit specifiers are also supported in opC++.
		/// 
		/// However the address will point to a temporary object on bit members.
		/// 
		/// \code
		/// //This will work also:
		/// bool bits : 1;
		/// \endcode
		/// 
		struct bool_info : basic_info
		{
			typedef basic_info super;

			bool_info(const data_field& f, bool& indata) 
				: 
				super(f),
				data(&indata) 
			{}
			
			bool_info() 
				: 
				super(invalid_field::get_instance()),
				data(NULL) 
			{}
			
			size_t type_size()
			{
				return sizeof(bool);
			}
			
			virtual void visit(visitor_base& visitor)
			{
				visitor.visit(*this);
			}
			
			/// 
			/// \brief Get a reference to the bool data.
			/// 
			bool& get_bool()
			{
				return *data;
			}
			
			virtual void* address()
			{
				return data;
			}
			
			virtual member_info* new_copy()
			{
				return new bool_info(*this);
			}

			template<typename type>
			type* cast()
			{
				if(run_member_cast<type>::can_cast(this))
					return (type*)address();
				return NULL;
			}

		private:
			void set_data(bool* newdata)
			{
				data = newdata;
			}
			void set_data(const bool* newdata)
			{
				data = const_cast<bool*>(newdata);
			}

			friend struct member_info;

			bool* data;

		};

		/// 
		/// \brief Accessor for C++ byte-expressible data.
		/// 
		/// Provides indirect access to reflected byte types.
		/// 
		/// This will reflect all C++ integer types except int.
		///
		/// Supported: 
		///		- unsigned types
		///		- char
		///		- short
		///		- long
		///		- long long
		///		- unsigned int
		///
		struct bytes_info : basic_info
		{
			bytes_info(const data_field& f)
				:
				basic_info(f)
			{}
			
			virtual void visit(visitor_base& visitor)
			{
				visitor.visit(*this);
			}

			template<typename type>
			type* cast()
			{
				if(run_member_cast<type>::can_cast(this))
					return (type*)address();
				return NULL;
			}
			
			/// 
			/// \brief Is this byte type unsigned?
			/// 
			virtual bool is_unsigned() = NULL;
			
			/// 
			/// \brief Can we cast to this byte type?
			/// 
			/// Usually you'll want to use get_bytes() instead.
			/// 
			template<typename type>
			bool can_cast_bytes()
			{
				//signing matches
				if(opcpp::metadata::is_unsigned<type>::value
				&& is_unsigned())
				{
					//size matches
					if(type_size() == sizeof(type))
					{
						//ok, return it
						return true;
					}
				}
				return false;
			}
			
			/// 
			/// \brief Attempts to safely cast to a given type.
			/// 
			/// This allows you to match the data to a byte type,
			/// and get access to the data.
			/// 
			/// If the cast fails it returns NULL.
			/// 
			/// Usage:
			/// \code
			/// unsigned int* value = accessor.get_bytes< unsigned int >();
			/// \endcode
			/// 
			template<typename type>
			type* get_bytes()
			{
				if(can_cast_bytes<type>())
					return (type*)address();
				return NULL;
			}
		};
		
		namespace detail
		{
			template<class type>
			struct bytes_info_type : bytes_info
			{
				typedef bytes_info super;

				bytes_info_type(const data_field& f, type& indata) 
					: 
					super(f),
					data(&indata)
				{}

				bytes_info_type() 
					:
				super(invalid_field::get_instance())
				{}

				size_t type_size()
				{
					return sizeof(type);
				}

				virtual void* address()
				{
					return data;
				}

				virtual member_info* new_copy()
				{
					return new bytes_info_type<type>(*this);
				}

				virtual bool is_unsigned()
				{
					return opcpp::metadata::is_unsigned<type>::value;
				}

			private:
				//only container_info's should have access to this!
				void set_data(type* newdata)
				{
					data = newdata;
				}
				void set_data(const type* newdata)
				{
					data = const_cast<type*>(newdata);
				}

				friend struct member_info;

				type* data;	
			};
		}

		using detail::bytes_info_type;

		/// 
		/// \brief Accessor for opC++ opstruct data.
		/// 
		/// Provides access to reflected opstructs.
		/// 
		/// This accessor deals with the actual type, not a pointer to opstruct.
		/// 
		/// Pointers to opstructs will have this accessor nested within a pointer_info.
		/// See pointer_info for more information about inner accessors.
		/// 
		struct struct_info : member_info
		{
			struct_info(const data_field& f)
				:
				member_info(f)
			{}
			
			virtual void visit(visitor_base& visitor)
			{
				visitor.visit(*this);
			}
			
			template<typename type>
			type* cast()
			{
				if(run_member_cast<type>::can_cast(this))
					return (type*)address();
				return NULL;
			}

			/// 
			/// \brief Visits the data members of the opstruct instance.
			/// 
			virtual void visit_data_members(visitor_base& visitor) = NULL;

			
			/// 
			/// \brief Gets the opC++ type associated with this opstruct. 
			///
			virtual struct_type& get_type() = NULL;

			/// 
			/// \brief Gets a pointer to the opstruct data.
			/// 
			/// This is not very useful, since opstruct is not
			/// polymorphic and has no methods.
			///
			virtual struct_base* get_struct() = NULL;

			/// 
			/// \brief Gets a boxed struct.
			/// 
			/// This is taken by some struct_type functions,
			/// which require a correctly typed instance for safety.
			///
			opcpp::base::boxed_struct get_boxed_struct()
			{
				return opcpp::base::boxed_struct(&get_type(),get_struct());
			}

			/// 
			/// \brief Is this opstruct type serializable?
			/// 
			/// This will return false in cases where there is no default constructor,
			/// or no default construction method known.
			/// 
			bool is_serializable()
			{
				return get_type().is_serializable();
			}

			///
			/// \brief Gets the opC++ id of the opstruct type.  For Internal Use.
			///
			virtual void* get_id() = NULL;
		};
	
		namespace detail
		{
			template<class type>
			struct struct_info_type : struct_info
			{
				typedef struct_info super;

				struct_info_type(const data_field& f, type& indata) 
					: 
					super(f),
					data(&indata) 
				{}

				struct_info_type()
					:
					super(invalid_field::get_instance())
				{}

				size_t type_size()
				{
					return sizeof(type);
				}

				virtual void* address()
				{
					return data;
				}

				virtual struct_base* get_struct()
				{
					return static_cast<struct_base*>(data);
				}

				virtual void* get_id()
				{
					return opcpp::metadata::id<type>::get_id();
				}

				virtual void visit_data_members(visitor_base& visitor)
				{
					data->visit_data_members(visitor);
				}

				virtual struct_type& get_type()
				{
					return *get_struct_type<type>::get();
				}

				virtual member_info* new_copy()
				{
					return new struct_info_type<type>(*this);
				}

			private:
				void set_data(type* newdata)
				{
					data = newdata;
				}
				void set_data(const type* newdata)
				{
					data = const_cast<type*>(newdata);
				}

				friend struct member_info;

				type* data;	
			};
		}
		
		using detail::struct_info_type;

		/// 
		/// \brief Accessor for opC++ opclass data.
		/// 
		/// Provides access to reflected opclasses.
		/// 
		/// This accessor deals with the actual type, not a pointer to opclass.
		/// 
		/// Pointers to opclasses will have this accessor nested within a pointer_info.
		/// See pointer_info for more information about inner accessors.
		/// 
		struct class_info : member_info
		{
			class_info(const data_field& f, class_base& indata)
				: 
				member_info(f),
				data(&indata)
			{}
			
			class_info(const data_field& f) 
				:
				member_info(f),
				data(NULL)
			{}
			
			virtual void visit(visitor_base& visitor)
			{
				visitor.visit(*this);
			}
			
			virtual void* address()
			{
				return data;
			}

			///
			/// \brief Visits the data members of the opclass instance.
			///
			void visit_data_members(visitor_base& visitor)
			{
				get_class()->visit_data_members(visitor);
			}
			
			/// 
			/// \brief Get the opclass instance.
			///
			/// This is useful since opclass is polymorphic,
			/// and you can use opcpp::casting::class_cast() to quickly cast
			/// to types and their parent classes.
			/// 
			class_base* get_class()
			{
				return data;
			}
			
			/// 
			/// \brief Get the opC++ class_type for this object.
			/// 
			/// Note that for pointers this will only
			/// give you the specified type.
			/// 
			/// To get the actual type in that case you will need to do this:
			/// \code
			/// class_type& actual_type = accessor.get_class()->get_type();
			/// \endcode
			/// 
			virtual class_type& get_type() = NULL;

			/// 
			/// \brief Is this opclass type serializable?
			/// 
			/// This will return false in cases where there is no default constructor,
			/// or no default construction method known.
			/// 
			/// Note that for pointers this will only give you
			/// the specified type's value.
			/// 
			/// To get the actual value in that case you will need to query the actual class_type:
			/// \code
			/// accessor.get_class()->get_type().is_serializable();
			/// \endcode
			/// 
			bool is_serializable()
			{
				class_type& type = get_type();
				return type.is_serializable();
			}

		private:
			void set_data(class_base* newdata)
			{
				data = newdata;
			}
			void set_data(const class_base* newdata)
			{
				data = const_cast<class_base*>(newdata);
			}

			friend struct member_info;

			class_base* data;
		};

		namespace detail
		{
			template<class type>
			struct class_info_type : class_info
			{
				typedef class_info super;

				class_info_type(const data_field& f, type& indata)
					: 
					super(f,indata) 
				{}

				class_info_type() 
					:
					super(invalid_field::get_instance())
				{}

				size_t type_size()
				{
					return sizeof(type);
				}

				virtual member_info* new_copy()
				{
					return new class_info_type<type>(*this);
				}

				virtual class_type& get_type()
				{
					return *get_class_type<type>::get();
				}
			};
		}

		using detail::class_info_type;

		/// 
		/// \brief Accessor for opC++ openum data.
		/// 
		/// Provides access to reflected openums.
		/// 
		struct enum_info : member_info
		{
			enum_info(const data_field& f)
				:
				member_info(f)
			{}

			virtual void visit(visitor_base& visitor)
			{
				visitor.visit(*this);
			}
			

			/// 
			/// \brief Gets the openum type object.
			/// 
			virtual enum_type& get_type() = NULL;

			/// 
			/// \brief Attempt to convert the enum's value to a string value.
			/// 
			/// Returns false if the enum value is invalid.
			/// 
			virtual bool to_string(std::string& string)			= NULL;

			/// 
			/// \brief Attempt to set the enum's value with a string value.
			/// 
			/// Returns false if the conversion was invalid.
			/// 
			virtual bool from_string(const std::string& string) = NULL;
			
			/// 
			/// \brief Attempt to convert the enum's value to an int.
			/// 
			/// Returns false if the enum value is invalid.
			/// 
			virtual bool to_int(int& intvalue)					= NULL;

			/// 
			/// \brief Attempt to set the enum's value with an int.
			/// 
			/// Returns false if the conversion was invalid.
			/// 
			virtual bool from_int(const int& intvalue)			= NULL;
			
			//TODO: add enum_type, eliminate more vftable bloat.
			//get the enum type entry
			//most of the functions should run through this
			//virtual enum_type* get_type() = NULL;

			template<typename type>
			type* cast()
			{
				if(run_member_cast<type>::can_cast(this))
					return (type*)address();
				return NULL;
			}

			///
			/// \brief Gets the opC++ id of the openum type.  For Internal Use.
			///
			virtual void* get_id() = NULL;
		};


		namespace detail
		{
			template<typename type>
			struct enum_info_type : enum_info
			{
				typedef enum_info super;

				enum_info_type(const data_field& f, type& indata) 
					: 
					super(f),
					data(&indata) 
				{}

				enum_info_type() 
					: 
					super(invalid_field::get_instance()),
					data(NULL) 
				{}

				size_t type_size()
				{
					return sizeof(type);
				}

				virtual void* address()
				{
					return data;
				}

				enum_type& get_type()
				{
					return *get_enum_type<type>::get();
				}

				virtual member_info* new_copy()
				{
					return new enum_info_type<type>(*this);
				}

				virtual void* get_id()
				{
					return opcpp::metadata::id<type>::get_id();
				}

				virtual bool to_string(std::string& string)
				{
					return data->to_string(string);
				}

				virtual bool from_string(const std::string& string)
				{
					return data->from_string(string);
				}

				virtual bool to_int(int& intvalue)
				{
					return data->to_int(intvalue);
				}

				virtual bool from_int(const int& intvalue)
				{
					return data->from_int(intvalue);
				}

			private:
				void set_data(type* newdata)
				{
					data = newdata;
				}
				void set_data(const type* newdata)
				{
					data = const_cast<type*>(newdata);
				}

				friend struct member_info;

				type* data;
			};
		}

		using detail::enum_info_type;
		
		/// 
		/// \brief Accessor for all container data.
		/// 
		/// Provides access to reflected container data.
		/// 
		struct container_info : member_info
		{
			container_info(const data_field& f)
				:
				member_info(f)
			{}
			
			
			/// 
			/// \brief Gets the inner accessor for the inner type.
			/// 
			/// This provides access to an accessor that uses
			/// the current container element.
			///
			/// The current container element will initially be invalid,
			/// so before you access the data be sure to call an iteration
			/// function like begin() and be sure the container contains elements.
			/// 
			/// Iterating this container using the iteration functions will
			/// alter the data within this single inner accessor.
			/// 
			virtual member_info* get_inner() = NULL;

			///
			/// \brief Get the number of elements within this container.
			/// 
			virtual size_t size() = NULL;

			/// 
			/// \brief Clear this container - calls the appropriate STL clear function.
			/// 
			virtual void clear() = NULL;

			/// 
			/// \brief Goto the beginning of this container.
			/// 
			/// Moves the inner accessor to the first element.
			///
			/// Returns false if there are no elements.
			/// 
			virtual bool begin() = NULL;
			
			/// 
			/// \brief Goto the STL end of this container.
			/// 
			/// Not a valid data element. 
			///
			virtual void end() = NULL;

			/// 
			/// \brief Goto the last element of this container.
			/// 
			/// Returns false if there are no elements.
			/// 
			bool last()
			{
				end();
				if(size() && decrement())
					return true;
				return false;
			}
			
			///
			/// \brief Goto the next element in this container. 
			///
			/// Moves the inner accessor to the next element.
			///
			/// Returns false if it reached the STL end.
			/// 
			virtual bool increment() = NULL;
			
			///
			/// \brief Goto the previous element in this container. 
			///
			/// Moves the inner accessor to the previous element.
			///
			/// Returns false if you attempted to decrement past the first element.
			/// 
			virtual bool decrement() = NULL;
			
			/// 
			/// \brief Erase the current element in this container.
			/// 
			/// After this operation the inner accessor will point to the next element.
			/// 
			virtual bool erase() = NULL;
			
			/// 
			/// \brief Is this container serializable? 
			/// 
			/// All STL containers are serializable.
			/// 
			virtual bool is_serializable() = NULL;
			
			/// 
			/// \brief Slowly seek to a particular indexed element.
			/// 
			/// Returns false if the index is invalid. 
			///
			/// If you really need to quickly seek using indexes you
			/// should use/cast to vector_info, or deque_info,
			/// which are quickly seekable.
			/// 
			bool slow_seek(size_t index)
			{
				if(!begin())
					return false;

				for(size_t i = 0; i < index; i++)
				{
					if(!increment())
						return false;
				}

				return true;
			}
			
			template<typename type>
			type* cast()
			{
				if(run_member_cast<type>::can_cast(this))
					return (type*)address();
				return NULL;
			}
		};

		/// 
		/// \brief Accessor for all single value STL container data.
		/// 
		/// Provides access to reflected single value container data.
		/// 
		/// Use this accessor as an easy abstraction for all single
		/// value container types.
		/// 
		/// Supports:
		///		- vector
		///		- deque
		///		- list
		///		- set
		///		- multiset
		/// 
		struct value_container_info : container_info
		{
			value_container_info(const data_field& f)
				:
				container_info(f)
			{}

			/// 
			/// \brief Insert a data value at the current element.
			/// 
			/// This is the recommended way to insert elements, as it switches between
			/// typed insertion methods at compile time.
			/// 
			/// If successful, the current element becomes the inserted element.
			///
			/// Usage:
			/// \code
			/// bool value = false;
			/// accessor.insert(value);
			/// \endcode
			/// 
			template<typename type>
			bool insert(const type& value);
			
			/// 
			/// \brief Push back a data value at the end of the container.
			/// 
			/// If successful, the current element becomes the inserted element.
			///
			/// If unsuccessful, the current element becomes the invalid end element.
			/// 
			/// Usage:
			/// \code
			/// bool value = false;
			/// accessor.insert(value);
			/// \endcode
			///
			template<typename type>
			bool push_back(const type& value)
			{
				end();
				return insert<type>(value);
			}

			/// 
			/// \brief Insert default constructed data at (before) the current element.
			/// 
			/// The current element becomes the inserted element.
			/// 
			/// Returns false if the element type is not default constructable.
			/// 
			virtual bool insert_default() = NULL;

			/// 
			/// \brief Push back default constructed data at the end of this container.
			/// 
			/// If successful, the current element becomes the inserted element.
			/// 
			/// Returns false if the element type is not default constructable,
			/// and sets the current element to the end.
			///
			bool push_back_default()
			{
				end();
				return insert_default();
			}

			/// 
			/// \brief Attempt to insert an opclass.
			/// 
			virtual bool insert_class(class_base* object) = NULL;

			bool insert_class(const class_base& object)
			{
				return insert_class(const_cast<class_base*>(&object));
			}

			///
			/// \brief Attempt to insert an int.
			/// 
			bool insert_basic(const int& value)
			{
				if(info_cast_visitor<int_info>(get_inner()).result())
				{
					insert_unsafe( const_cast<int*>(&value) );
					return true;
				}

				return false;
			}
			
			/// 
			/// \brief Attempt to insert a float.
			/// 
			bool insert_basic(const float& value)
			{
				if(info_cast_visitor<float_info>(get_inner()).result())
				{
					insert_unsafe( const_cast<float*>(&value) );
					return true;
				}

				return false;
			}

			/// 
			/// \brief Attempt to insert a double.
			/// 
			bool insert_basic(const double& value)
			{
				if(info_cast_visitor<double_info>(get_inner()).result())
				{
					insert_unsafe( const_cast<double*>(&value) );
					return true;
				}

				return false;
			}

			/// 
			/// \brief Attempt to insert a c-style string.
			/// 
			bool insert_basic(const std::string::value_type* value)
			{
				return insert_basic(std::string(value));
			}

			/// 
			/// \brief Attempt to insert a STL string.
			/// 
			bool insert_basic(const std::string& value)
			{
				if(info_cast_visitor<string_info>(get_inner()).result())
				{
					insert_unsafe( const_cast<std::string*>(&value) );
					return true;
				}

				return false;				
			}

			/// 
			/// \brief Attempt to insert a c-style wstring.
			/// 
			bool insert_basic(const std::wstring::value_type* value)
			{
				return insert_basic(std::wstring(value));
			}
			
			/// 
			/// \brief Attempt to insert a STL wstring.
			/// 
			bool insert_basic(const std::wstring& value)
			{
				if(info_cast_visitor<wstring_info>(get_inner()).result())
				{
					insert_unsafe( const_cast<std::wstring*>(&value) );
					return true;
				}

				return false;				
			}

			/// 
			/// \brief Attempt to insert a bool string.
			/// 
			bool insert_basic(const bool& value)
			{
				if(info_cast_visitor<bool_info>(get_inner()).result())
				{
					insert_unsafe( const_cast<bool*>(&value) );
					return true;
				}

				return false;
			}
			
			
			/// 
			/// \brief Attempt to insert a byte type.
			/// 
			template<class type>
			bool insert_bytes(const type& bytes)
			{
				if(bytes_info* binfo = info_cast<bytes_info>(this))
				{
					if(binfo->can_cast_bytes<type>())
					{
						insert_unsafe( const_cast<void*>(&bytes) );
					}
				}
			}
			
			/// 
			/// \brief Attempt to insert an enum type.
			/// 
			template<class enumtype>
			bool insert_enum(const enumtype& value)
			{
				if(info_cast_visitor<enum_info>(get_inner()).result())
				{
					void* id = opcpp::metadata::id<enumtype>::get_id();
					
					//make sure id is valid
					if(id != NULL)
						return insert_id( const_cast<enumtype*>(&value), id);
				}
				
				return false;
			}
			
			/// 
			/// \brief Attempt to insert an opstruct type.
			/// 
			template<class structtype>
			bool insert_struct(const structtype& value)
			{
				if(info_cast_visitor<struct_info>(get_inner()).result())
				{
					void* id = opcpp::metadata::id<structtype>::get_id();

					//make sure id is valid
					if(id != NULL)
						return insert_id( const_cast<structtype*>(&value), id);
				}

				return false;
			}
			
			template<typename type>
			type* cast()
			{
				if(run_member_cast<type>::can_cast(this))
					return (type*)address();
				return NULL;
			}
			
			virtual bool insert_visit(visitor_base& visitor) = NULL;
			
		public:

			/// 
			/// \brief Unsafe fast insertion - use only if the type is known to be correct!
			/// 
			virtual void insert_unsafe(void* value)		  = NULL;

			/// 
			/// \brief Unsafe fast push back - use only if the type is known to be correct!
			/// 
			void push_back_unsafe(void* value)
			{
				end();
				insert_unsafe(value);
			}
			
		private:
			
			virtual bool insert_id(void* value, void* id) = NULL;
		};

		namespace detail
		{
			//universal insert functions
			struct no_insert
			{
				template<typename type>
				static bool insert(const type& value, value_container_info& info)
				{
					return false;
				}
			};

			struct enum_insert
			{
				template<typename type>
				static bool insert(const type& value, value_container_info& info)
				{
					return info.insert_enum(value);
				}
			};

			struct class_insert
			{
				static bool insert(const opcpp::base::class_base& value, value_container_info& info)
				{
					return info.insert_class(value);
				}
			};

			struct struct_insert
			{
				template<typename type>
				static bool insert(const type& value, value_container_info& info)
				{
					return info.insert_struct(value);
				}
			};

			struct bytes_insert
			{
				template<typename type>
				static bool insert(const type& value, value_container_info& info)
				{
					return info.insert_bytes<type>(value);
				}
			};

			template<typename type>
			struct run_insert
			{
				typedef
					typename 
					IF< is_openum<type>::value,
					//then
						enum_insert,
					//else
						typename
						IF< is_opstruct<type>::value,
						//then
							struct_insert,
						//else
							typename 
							IF< is_opclass<type>::value,
							//then
								class_insert,
							//else
								typename
								IF< is_bytes<type>::value,
								//then
									bytes_insert,
								//else
									no_insert
								>::RET
							>::RET
						>::RET
					>::RET

					selected;

				static bool insert(const type& value, value_container_info& info)
				{
					return selected::insert(value,info);
				}
			};

			template<>
			struct run_insert<bool>
			{
				static bool insert(const bool& value, value_container_info& info)
				{
					return info.insert_basic(value);
				}
			};

			template<>
			struct run_insert<int>
			{
				static bool insert(const int& value, value_container_info& info)
				{
					return info.insert_basic(value);
				}
			};

			template<>
			struct run_insert<float>
			{
				static bool insert(const float& value, value_container_info& info)
				{
					return info.insert_basic(value);
				}
			};
		
			template<>
			struct run_insert<double>
			{
				static bool insert(const double& value, value_container_info& info)
				{
					return info.insert_basic(value);
				}
			};

			template<>
			struct run_insert<std::string>
			{
				static bool insert(const std::string& value, value_container_info& info)
				{
					return info.insert_basic(value);
				}
			};

			template<>
			struct run_insert<std::wstring>
			{
				static bool insert(const std::wstring& value, value_container_info& info)
				{
					return info.insert_basic(value);
				}
			};

			template<>
			struct run_insert<std::string::value_type*>
			{
				static bool insert(const std::string::value_type*& value, value_container_info& info)
				{
					return info.insert_basic(value);
				}
			};

			template<>
			struct run_insert<std::wstring::value_type*>
			{
				static bool insert(const std::wstring::value_type*& value, value_container_info& info)
				{
					return info.insert_basic(value);
				}
			};
		}

		using detail::run_insert;


		template<typename type>
		inline bool value_container_info::insert(const type& value)
		{
			return run_insert<type>::insert(value,*this);
		}

		/// 
		/// \brief Accessor for STL key/value container types.
		/// 
		/// Provides access to reflected key/value container data.
		/// 
		/// Use this accessor as an easy abstraction for all key/value
		/// container types.
		/// 
		/// Supports:
		///		- map
		///		- multimap
		/// 
		struct key_container_info : container_info
		{
			key_container_info(const data_field& f)
				:
				container_info(f)
			{}

			/// 
			/// \brief Get the inner accessor for the key data.
			/// 
			/// Will access the current element.
			/// 
			/// Do not attempt to alter key values.
			/// 
			virtual member_info* get_key() = NULL;

			/// 
			/// \brief Get the inner accessor for the value data.
			/// 
			/// Will access the current element.
			///
			/// You can also access the value data using get_inner().
			/// 
			member_info* get_value()
			{
				return get_inner();
			}

			/// 
			/// \brief Insert key and value simultaneously using visitors.
			/// 
			/// This is the only method available currently for inserting
			/// key/value pairs.  This should be used by serialization for
			/// loading keyed containers.
			/// 
			/// This method creates a default constructed pair of values temporarily on the stack,
			/// so be aware of this.  It will return false if it's unable to insert.
			/// 
			virtual bool insert_visit(visitor_base& keyvisitor, visitor_base& valuevisitor) = NULL;

			template<typename type>
			type* cast()
			{
				if(run_member_cast<type>::can_cast(this))
					return (type*)address();
				return NULL;
			}
		};

		/// 
		/// \brief Accessor for STL vector container data.
		/// 
		/// Provides access to reflected vector container data.
		/// 
		/// This accessor gives you extra control for vector
		/// containers, including:
		///		- fast indexed seek
		///		- STL reserve
		///		- STL resize
		///		
		struct vector_info : value_container_info
		{
			vector_info(const data_field& f)
				:
				value_container_info(f)
			{}

			virtual void visit(visitor_base& visitor)
			{
				visitor.visit(*this);
			}

			/// 
			/// \brief Reserve memory for a number of elements.
			/// 
			/// Calls the STL reserve function on the container.
			/// 
			/// Useful to avoid reallocations if the number of elements is known ahead of time.
			/// 
			virtual void reserve(size_t count) = NULL;
			
			/// 
			/// \brief Resize the container to a number of elements.
			/// 
			/// Calls the STL resize function on the container.
			/// 
			/// Useful for filling the container using the iteration functions,
			/// along with the inner accessor.
			/// 
			virtual void resize(size_t count)  = NULL;

			/// 
			/// \brief Seek to a particular index in the container.
			/// 
			/// Adjusts the inner accessor to use this data if successful.
			///
			/// Returns false if the index was invalid.
			/// 
			virtual bool seek(size_t index)	   = NULL;

			///
			/// \brief Erase a particular indexed element.
			/// 
			/// Sets the new element to index + 1.
			/// 
			/// Use erase() instead if possible.
			/// 
			bool erase(int index)
			{
				if(seek(index))
					return container_info::erase();
				return false;
			}

			template<typename type>
			type* cast()
			{
				if(run_member_cast<type>::can_cast(this))
					return (type*)address();
				return NULL;
			} 
		};

		/// 
		/// \brief Accessor for STL deque container data.
		/// 
		/// Provides access to reflected deque container data.
		/// 
		/// This accessor gives you extra control for deque
		/// containers, including:
		///		- fast indexed seek
		///		- STL resize
		///		- push front
		///		
		struct deque_info : value_container_info
		{
			deque_info(const data_field& f)
				:
				value_container_info(f)
			{}
			
			virtual void visit(visitor_base& visitor)
			{
				visitor.visit(*this);
			}

			/// 
			/// \brief Resize the container to a number of elements.
			/// 
			/// Calls the STL resize function on the container.
			/// 
			/// Useful for filling the container using the iteration functions,
			/// along with the inner accessor.
			/// 
			virtual void resize(size_t count)  = NULL;

			/// 
			/// \brief Seek to a particular index in the container.
			/// 
			/// Adjusts the inner accessor to use this data if successful.
			///
			/// Returns false if the index was invalid.
			/// 
			virtual bool seek(size_t index)	   = NULL;

			///
			/// \brief Erase a particular indexed element.
			/// 
			/// Sets the new element to index + 1.
			/// 
			/// Use erase() instead if possible.
			/// 
			bool erase(int index)
			{
				if(seek(index))
					return container_info::erase();
				return false;
			}

			///
			/// \brief Push an element on the front of the deque.
			/// 
			/// Sets the current element to the new element.
			/// 
			/// If unsuccessful sets the current element the first container element.
			/// 
			template<typename type>
			bool push_front(const type& value)
			{
				begin();
				return insert(value);
			}

			template<typename type>
			type* cast()
			{
				if(run_member_cast<type>::can_cast(this))
					return (type*)address();
				return NULL;
			}
		};

		/// 
		/// \brief Accessor for STL list container data.
		/// 
		/// Provides access to reflected list container data.
		/// 
		/// This accessor gives you extra control for list
		/// containers, including:
		///		- push front
		/// 
		struct list_info : value_container_info
		{
			list_info(const data_field& f)
				:
				value_container_info(f)
			{}

			virtual void visit(visitor_base& visitor)
			{
				visitor.visit(*this);
			}
			
			///
			/// \brief Push an element on the back of the list.
			/// 
			/// Sets the current element to the new element.
			/// 
			/// If unsuccessful sets the current element the invalid end element.
			/// 
			template<typename type>
			bool push_back(const type& value)
			{
				end();
				return insert(value);
			}

			///
			/// \brief Push an element on the front of the list.
			/// 
			/// Sets the current element to the new element.
			/// 
			/// If unsuccessful sets the current element the first container element.
			/// 
			template<typename type>
			bool push_front(const type& value)
			{
				begin();
				return insert(value);
			}
			
			template<typename type>
			type* cast()
			{
				if(run_member_cast<type>::can_cast(this))
					return (type*)address();
				return NULL;
			}	
		};

		/// 
		/// \brief Accessor for STL set container data.
		/// 
		/// Provides access to reflected set container data.
		/// 
		/// This accessor gives you extra control for set
		/// containers, including:
		///		- insertion visit
		/// 
		struct set_info : value_container_info
		{
			set_info(const data_field& f)
				:
				value_container_info(f)
			{}

			virtual void visit(visitor_base& visitor)
			{
				visitor.visit(*this);
			}

			template<typename type>
			type* cast()
			{
				if(run_member_cast<type>::can_cast(this))
					return (type*)address();
				return NULL;
			}
			
			/// 
			/// \brief Insertion via visitor.
			/// 
			/// This is currently the only way to add elements to set containers.
			/// You'll need to use this for loading serialized sets.
			/// 
			/// Be aware that this default constructs a temporary data value on the stack.
			/// 
			virtual bool insert_visit(visitor_base& visitor) = NULL;
		};
		
		/// 
		/// \brief Accessor for STL multiset container data.
		/// 
		/// Provides access to reflected multiset container data.
		/// 
		/// This accessor gives you extra control for multiset
		/// containers, including:
		///		- insertion visit
		/// 
		struct multiset_info : value_container_info
		{
			multiset_info(const data_field& f)
				:
				value_container_info(f)
			{}

			/// 
			/// \brief Insertion via visitor.
			/// 
			/// This is currently the only way to add elements to set containers.
			/// You'll need to use this for loading serialized sets.
			/// 
			/// Be aware that this default constructs a temporary data value on the stack.
			/// 
			virtual bool insert_visit(visitor_base& visitor) = NULL;

			virtual void visit(visitor_base& visitor)
			{
				visitor.visit(*this);
			}

			template<typename type>
			type* cast()
			{
				if(run_member_cast<type>::can_cast(this))
					return (type*)address();
				return NULL;
			}
		};

		/// 
		/// \brief Accessor for STL map container data.
		/// 
		/// Provides access to reflected map container data.
		/// 
		/// See key_container_info for map related functionality.
		/// 		
		struct map_info : key_container_info
		{
			map_info(const data_field& f)
				:
				key_container_info(f)
			{}
			
			virtual void visit(visitor_base& visitor)
			{
				visitor.visit(*this);
			}
			
			template<typename type>
			type* cast()
			{
				if(run_member_cast<type>::can_cast(this))
					return (type*)address();
				return NULL;
			}
		};

		/// 
		/// \brief Accessor for STL multimap container data.
		/// 
		/// Provides access to reflected multimap container data.
		/// 
		/// See key_container_info for multimap related functionality.
		/// 	
		struct multimap_info : key_container_info
		{

			multimap_info(const data_field& f)
				:
				key_container_info(f)
			{}
			
			virtual void visit(visitor_base& visitor)
			{
				visitor.visit(*this);
			}

			template<typename type>
			type* cast()
			{
				if(run_member_cast<type>::can_cast(this))
					return (type*)address();
				return NULL;
			}
		};

		/// 
		/// \brief Accessor for unknown data types.
		/// 
		/// Provides accessor identification for an unknown type.
		/// 
		/// Currently has no casting or type discovery mechanism.
		/// 	
		struct unknown_info : member_info
		{
			unknown_info(const data_field& f) 
				: 
				member_info(f)
			{}
			
			virtual void visit(visitor_base& visitor)
			{
				visitor.visit(*this);
			}

			/// 
			/// \brief unknown_info is unable to cast currently.
			/// 
			template<typename type>
			type* cast()
			{
				//NOTE: no way to cast unknown types currently.
				return NULL;
			}
		};

		namespace detail
		{
			template<typename type>
			struct unknown_info_type : unknown_info
			{
				typedef unknown_info super;

				//unknown type info
				unknown_info_type()
					: 
					super(invalid_field::get_instance()),
					data(NULL)
				{}

				unknown_info_type(const data_field& f, type& indata) 
					: 
					super(f),
					data(&indata)
				{}
				
				member_info* new_copy()
				{
					return NULL;
				}

				size_t type_size()
				{
					return sizeof(type);
				}

				virtual void* address()
				{
					return data;
				}

			private:
				//only container_info's should have access to this!
				void set_data(type* newdata)
				{
					data = newdata;
				}
				void set_data(const type* newdata)
				{
					data = const_cast<type*>(newdata);
				}

				friend struct member_info;

				type* data;
			};

			//alters a member_info for an inner container type
			template<class innerinfotype, class innerdatatype>
			struct inner_info : public innerinfotype
			{
				inner_info()
				{}

				inner_info(innerdatatype& indata) 
					: 
					innerinfotype(invalid_field::get_instance(),indata)
				{}

				inner_info(const data_field& f, innerdatatype& indata) 
					: 
					innerinfotype(f,indata)
				{}
			};

			//==========================================
			// Insertion of class types metafunction
			//==========================================

			template<class type>
			struct do_insert_class
			{
				//we want to insert a non-pointer here.
				template<class c>
				static bool insert(c& container, typename c::iterator& current, class_base* object)
				{
					type* casted = class_cast<type>(object);
					if(!casted)
						return false;
					
					current = container.insert(current,*casted);

					return true;
				}
			};
			
			template<class type>
			struct do_insert_class<type*>
			{
				//we want to insert a non-pointer here.
				template<class c>
				static bool insert(c& container, typename c::iterator& current, class_base* object)
				{
					type* casted = class_cast<type>(object);
					if(!casted)
						return false;

					current = container.insert(current,casted);

					return true;
				}
			};

			struct no_insert_class
			{
				template<class c>
				static bool insert(c& container, typename c::iterator& current, class_base* object)
				{
					return false;
				}
			};

			template<class type>
			struct run_insert_class
			{
				typedef
					typename IF< 
						is_opclass<type>::value,
						no_insert_class,
						do_insert_class<type>
					>::RET
					inserter;

				template<class c>
				static bool insert(c& container, typename c::iterator& current, class_base* object)
				{
					return inserter::insert(container,current,object);
				}
			};

			//==========================================
			// Insertion metafunctions
			//==========================================

			struct do_insert_null
			{
				template<class c>
				static bool insert(c& container, typename c::iterator& current)
				{
					current = container.insert(current,NULL);
					return true;
				}
			};
			
			struct no_insert_null
			{
				template<class c>
				static bool insert(c& container, typename c::iterator& current)
				{
					return false;
				}
			};


			template<class t>
			struct do_insert_default_type
			{
				template<class c>
				static bool insert(c& container, typename c::iterator& current)
				{
					//here we want the type to construct...if available!	
					current = container.insert(current,t());
					return true;
				}
			};


			template<class t>
			struct do_insert_enum
			{
				template<class c>
				static bool insert(c& container, typename c::iterator& current)
				{
					//here we want the type to construct...if available!	
					current = container.insert(current,t());
					return true;
				}
			};

			template<class type>
			struct run_insert_default
			{
				typedef
					//handle pointers
					typename IF<
						is_pointer<type>::value,
						do_insert_null,
						//handle openums
						typename IF<
							is_openum<type>::value,
							do_insert_enum<type>,
							//handle opstruct, opclasses, containers, basic types
							typename IF<
								is_serializable<type>::value,
								do_insert_default_type<type>,
								no_insert_null
							>::RET
						>::RET
					>::RET
					inserter;
			};

	

			//
			// Single value container
			//

			using opcpp::accessors::member_info;
			
			template< class container, class infotype = container_info>
			struct single_container_info : infotype
			{
				typedef container											containertype;
				typedef typename container::value_type						type;
				typedef typename containertype::iterator					iterator;
				typedef single_container_info<container,infotype>			thistype;
				
				typedef inner_info< 
					typename selector< typename remove_cv<type>::type >::type,
					typename remove_cv<type>::type 
				>	
				innertype;
				
				typedef infotype super;
				
				single_container_info()
					:
					super(invalid_field::get_instance()),
					data(NULL)
				{}
				
				single_container_info(const data_field& f, containertype& indata)
					:
					super(f),
					data(&indata)//,
					//current(data->end())
				{}

				size_t type_size()
				{
					return sizeof(containertype);
				}

				virtual void* address()
				{
					return data;
				}
				
				bool begin()
				{
					current = data->begin();
					if(current != data->end())
					{
						call_set_data(inner,&*current);
						return true;
					}
					return false;
				}
				
				bool increment()
				{
					++current;
					if(current != data->end())
					{
						call_set_data(inner,&*current);
						return true;
					}
					return false;
				}
				
				bool decrement()
				{	
					if(current != data->begin())
					{
						--current;	
						call_set_data(inner,&*current);
						return true;
					}
					return false;
				}

				void end()
				{
					current = data->end();
				}

				void clear()
				{
					data->clear();
				}

				size_t size()
				{
					return data->size();
				}

				bool erase()
				{
					if(current == data->end())
						return false;

					iterator last = current;
					this->thistype::increment();

					data->erase(last);
					return true;
				}

				virtual member_info* get_inner()
				{
					return &inner;
				}
			
				virtual bool is_serializable()
				{
					return opcpp::metadata::is_serializable<type>::value;
				}

				//insertion of actual type, and single pointer types
				virtual bool insert_class(class_base* object)
				{
					if(run_insert_class<type>::insert(*data,current,object))
					{
						call_set_data(inner,&*current);
						return true;
					}
					return false;
				}

				//insertion of null for pointer types
				virtual bool insert_default()
				{
					if(run_insert_default<type>::inserter::insert(*data,current))
					{
						call_set_data(inner,&*current);
						return true;
					}
					return false;
				}

				//you must know that the types are matches when using this.
				virtual void insert_unsafe(void* value)
				{
					type* typed = ((type*)value);
					current = data->insert( current, *typed );
					call_set_data(inner,&*current);
				}

				//insert via visitor
				virtual bool insert_visit(visitor_base& visitor)
				{
					if(insert_default())
					{
						inner.visit(visitor);
						return true;
					}
					return false;
				}

			private:

				virtual bool insert_id(void* value, void* id)
				{
					void* myid = opcpp::metadata::id<type>::get_id();
					if(id != myid)
						return false;
					
					insert_unsafe(value);
					return true;
				}

			private:
				//only container_info's should have access to this!
				void set_data(containertype* newdata)
				{
					data = newdata;
				}
				void set_data(const containertype* newdata)
				{
					data = const_cast<containertype*>(newdata);
				}

				friend struct member_info;	

		protected:
				containertype*	data;
				innertype		inner;
				iterator		current;
			};
			
			//
			// visit insertion metaprograms
			//
			
			template<typename temptype, typename tempinfo>
			struct do_insert_visit
			{
				template<typename t>
				static bool insert(t& data, visitor_base& visitor)
				{
					typename remove_cv<temptype>::type temporary;
					tempinfo tempinner(temporary);
					visitor.visit(tempinner);
					
					data.insert(temporary);
					return true;
				}
			};
			
			struct no_insert_visit
			{
				template<typename t>
				static bool insert(t& data, visitor_base& visitor)
				{
					return false;
				}
			};
			
			//run insert visit only if the type is_serializable
			template<typename temptype, typename innertype>
			struct run_insert_visit
			{
				typedef
					typename IF<
						is_serializable<temptype>::value,
						do_insert_visit<temptype,innertype>,
						no_insert_visit
					>::RET
				type;
			};

			struct no_insert_visit_map
			{
				template<typename t>
				static bool insert(t& data, visitor_base& keyvisitor, visitor_base& valuevisitor)
				{
					return false;
				}
			};

			template<typename keytype, typename keyinnertype, typename valuetype, typename valueinnertype>
			struct do_insert_visit_map
			{
				template<typename t>
				static bool insert(t& data, visitor_base& keyvisitor, visitor_base& valuevisitor)
				{
					typename remove_cv<keytype>::type tempkey;
					keyinnertype tempinnerkey(tempkey);
					keyvisitor.visit(tempinnerkey);

					typename remove_cv<valuetype>::type tempvalue;
					valueinnertype tempinnervalue(tempvalue);
					valuevisitor.visit(tempinnervalue);
					
					data.insert( typename t::value_type(tempkey,tempvalue) );
					return true;
				}
			};
			
			//run insert visit only if the types are both is_serializable
			template<typename keytype, typename keyinnertype, typename valuetype, typename valueinnertype>
			struct run_insert_visit_map
			{
				typedef
					typename IF<
						is_serializable<keytype>::value && is_serializable<valuetype>::value,
						do_insert_visit_map<keytype,keyinnertype,valuetype,valueinnertype>,
						no_insert_visit_map
					>::RET
				type;
			};

			//
			// Pair value containers
			//

			template< class container, class infotype = key_container_info >
			struct pair_container_info : infotype
			{
				typedef container										containertype;
				typedef typename containertype::value_type::first_type	key_type;
				typedef typename containertype::value_type::second_type value_type;
				typedef typename containertype::value_type			    pair_type;
				typedef typename containertype::iterator				iterator;
				typedef pair_container_info<container,infotype>			thistype;

				typedef infotype super;

				pair_container_info()
					:
					super(invalid_field::get_instance()),
					data(NULL)
				{}

				pair_container_info(const data_field& f, containertype& indata)
					:
					super(f),
					data(&indata)
				{}

				size_t type_size()
				{
					return sizeof(containertype);
				}

				virtual void* address()
				{
					return &data;
				}

				bool begin()
				{
					current = data->begin();
					if(current != data->end())
					{
						call_set_data(keydata,&(current->first));
						call_set_data(valuedata,&(current->second));
						return true;
					}
					return false;
				}

				bool increment()
				{
					++current;
					if(current != data->end())
					{
						call_set_data(keydata,&(current->first));
						call_set_data(valuedata,&(current->second));
						return true;
					}
					return false;
				}

				bool decrement()
				{	
					--current;
					if(current != data->end())
					{
						call_set_data(keydata,&(current->first));
						call_set_data(valuedata,&(current->second));
						return true;
					}
					return false;
				}

				void end()
				{
					current = data->end();
				}

				bool at_end()
				{
					return current == data->end();
				}

				bool at_begin()
				{
					return current == data->begin();
				}

				size_t size()
				{
					return data->size();
				}

				void clear()
				{
					data->clear();
				}

				virtual member_info* get_key()
				{
					return &keydata;
				}
				
				bool erase()
				{
					if(current == data->end())
						return false;

					iterator last = current;
					this->thistype::increment();

					data->erase(last);
					return true;
				}

				virtual member_info* get_inner()
				{
					return &valuedata;
				}

				typedef typename remove_cv<key_type>::type key_type_noconst;
				typedef typename remove_cv<value_type>::type value_type_noconst;

				typedef inner_info< typename selector<key_type_noconst>::type, key_type_noconst >		keyinnertype;
				typedef inner_info< typename selector<value_type_noconst>::type, value_type_noconst >	valueinnertype;

				virtual bool insert_visit(visitor_base& keyvisitor, visitor_base& valuevisitor)
				{
					return run_insert_visit_map<key_type_noconst,keyinnertype,value_type,valueinnertype>
						::type::insert(*data,keyvisitor,valuevisitor);

					return true;
				}

				virtual bool is_serializable()
				{
					if(opcpp::metadata::is_serializable<key_type_noconst>::value
					&& opcpp::metadata::is_serializable<value_type>::value)
						return true;
					return false;
				}

			private:
				//only container_info's should have access to this!
				void set_data(containertype* newdata)
				{
					data = newdata;
				}
				void set_data(const containertype* newdata)
				{
					data = const_cast<containertype*>(newdata);
				}

				friend struct member_info;

			protected:
				containertype*	data;

				keyinnertype	keydata;
				valueinnertype	valuedata;

				iterator current;
			};
		}

		using detail::unknown_info_type;
		using detail::inner_info;
		using detail::run_insert_default;
		using detail::single_container_info;
		using detail::pair_container_info;

		//
		// Array info
		//
		
		//??? should array be a container_info? (w/o insertion)
		
		/// \brief Accessor for C++ fixed size arrays.
		/// 
		/// Provides access to reflected array data.
		/// 
		/// Allows you to iterate the elements of the array,
		/// similar to the container_info mechanism.  Of course,
		/// it has no way of inserting elements or expanding array size.
		/// 	
		struct array_info : member_info
		{
			array_info(const data_field& f, size_t inelements) 
				: 
				member_info(f),
				elements(inelements), 
				current(0) 
			{}

			virtual void visit(visitor_base& visitor)
			{
				visitor.visit(*this);
			}

			///
			/// \brief Get the inner accessor for the current element.
			/// 
			/// This accessor will always point to the current element, even if you use the array iteration functions.
			/// 
			virtual member_info* get_inner() = NULL;

			/// 
			/// \brief Goto the 0-th (first) element in the array.
			/// 
			bool begin()
			{
				return set_current(0);
			}

			/// 
			/// \brief Goto the last element in the array.
			/// 
			void last()
			{
				set_current(elements-1);
			}

			/// 
			/// \brief Goto the next (+1) element in the array.
			/// 
			/// Returns false if you attempted to move out of bounds.
			/// 
			bool increment()
			{
				return set_current(get_current() + 1);
			}
			
			/// 
			/// \brief Goto the previous (-1) element in the array.
			/// 
			/// Returns false if you attempted to move out of bounds.
			/// 
			bool decrement()
			{
				return set_current(get_current() - 1);
			}

			/// 
			/// \brief Goto an indexed element in the array.
			/// 
			/// Returns false if you attempted to move out of bounds.
			/// 
			virtual bool set_current(size_t newindex) = NULL;
			
			/// 
			/// \brief Get the current element's index in the array.
			/// 
			size_t get_current()
			{
				return current;
			}

			/// 
			/// \brief Get the constant number of elements in the array.
			/// 
			size_t size()
			{
				return elements;
			}

			/// 
			/// \brief Is this array serializable?
			/// 
			virtual bool is_serializable() = NULL;

		protected:
			size_t current;
			size_t elements;
		};

		namespace detail
		{
			template<typename basetype, size_t inelements>
			struct array_info_type : array_info
			{
				array_info_type() 
					: 
					data(NULL), 
					array_info(invalid_field::get_instance(),inelements)
				{}

				array_info_type(const data_field& f, const basetype* indata)
					: 
					data( const_cast<basetype*>(indata) ),
					array_info(f,inelements)
				{
					call_set_data(inner,data);
				}

				array_info_type(const data_field& f, basetype* indata)
					: 
					data(indata),
					array_info(f,inelements)
				{
					call_set_data(inner,data);
				}

				member_info* new_copy()
				{
					return new array_info_type<basetype,inelements>(*this);
				}

				typedef basetype data_type;

				void* address()
				{
					return data;
				}

				size_t type_size()
				{
					return inelements*sizeof(basetype);
				}

				member_info* get_inner()
				{
					return &inner;
				}

				bool set_current(size_t newcurrent)
				{
					current = newcurrent;
					if(current < size())
					{
						call_set_data(inner,&data[current]);
						return true;
					}
					return false;
				}
					
				bool is_serializable()
				{
					return opcpp::metadata::is_serializable< basetype >::value;
				}
				
			private:
				
				typedef typename remove_cv<basetype>::type basetype_noconst;

				basetype* data;

				inner_info< typename selector< basetype_noconst >::type, basetype_noconst > inner;
			};
		}

		namespace detail
		{
			template<class t, class base>
			struct do_pointer_from_base
			{
				template<class basetype>
				static bool run(basetype* data, base* value)
				{
					t* temp = static_cast<t*>(value);
					*data = pointer_from_void<basetype>::convert(temp);
					return true;
				}
			};

			struct no_pointer_from_base
			{
				static bool run(void* data, void* value)
				{
					return false;
				}
			};

			template<class t>
			struct run_pointer_from_class
			{
				typedef typename remove_pointer<t>::type
					pointer_type;

				typedef
					typename IF< 
						is_opclass< pointer_type >::value,
						do_pointer_from_base< pointer_type, class_base >,
						no_pointer_from_base >::RET
					type;
			};

			template<class t>
			struct run_pointer_from_struct
			{
				typedef typename remove_pointer<t>::type
					pointer_type;

				typedef
					typename IF< 
					is_opstruct< pointer_type >::value,
					do_pointer_from_base< pointer_type, struct_base >,
					no_pointer_from_base >::RET
					type;
			};

		}


		using detail::array_info_type;

		/// 
		/// \brief Accessor for pointer types.
		/// 
		/// Provides access to pointer data.
		/// 
		/// Allows you to access the inner type of the pointer.
		/// 
		/// This means for Type* you can access the inner Type.
		/// 	
		struct pointer_info : member_info
		{
			pointer_info(const data_field& f) 
				: 
				member_info(f)
			{}
			
			virtual void visit(visitor_base& visitor)
			{
				visitor.visit(*this);
			}

			/// 
			/// \brief Accessor for the inner type.
			/// 
			/// You can safely access the inner accessor even if
			/// the pointer is NULL.  You may not however access
			/// the inner data if the pointer is NULL, always check
			/// before accessing!
			/// 
			virtual member_info* get_inner() = NULL;
			
			///
			/// \brief Set the pointer value, unsafe.
			/// 
			/// Only use this if you know the pointer types match exactly.
			/// 
			virtual void  set_pointer_unsafe(void* newpointer) = NULL;

			///
			/// \brief Set the pointer value for an opclass pointer.
			/// 
			/// Returns if the set was successful.
			/// 
			virtual bool  set_pointer_class(class_base* newpointer) = NULL;

			///
			/// \brief Set the pointer value for an opstruct pointer.
			/// 
			/// Returns if the set was successful.
			/// 
			virtual bool  set_pointer_struct(struct_base* newpointer) = NULL;

			///
			/// \brief Get the pointer value as void*.
			/// 
			/// The only safe use of this is comparison to NULL.
			/// 
			virtual void* get_pointer()					= NULL;

			/// 
			/// \brief Is the pointer value NULL?
			/// 
			bool is_null()
			{
				return get_pointer() == NULL;
			}
		};

		namespace detail
		{
			template< class basetype, class infotype = pointer_info >
			struct pointer_info_type : infotype
			{
				typedef infotype super;

				pointer_info_type() 
					: 
					super(invalid_field::get_instance()),
					data(NULL) 
				{}

				pointer_info_type(const data_field& f, const basetype& indata) 
					: 
					super(f),
					data( const_cast<basetype*>(&indata) )
				{
					call_set_data(inner,*data);
				}

				pointer_info_type(const data_field& f, basetype& indata)
					: 
					super(f),
					data(&indata)
				{
					call_set_data(inner,*data);
				}
				
				size_t type_size()
				{
					return sizeof(basetype);
				}

				virtual void* address()
				{
					return data;
				}

				virtual void* get_pointer()
				{
					return *data;
				}

				virtual member_info* new_copy()
				{
					return new pointer_info_type<basetype,infotype>(*this);
				}

				virtual void set_pointer_unsafe(void* newpointer)
				{
					*data = pointer_from_void<basetype>::convert(newpointer);
					call_set_data(inner,*data);
				}

				virtual bool set_pointer_class(class_base* newpointer)
				{
					if(run_pointer_from_class<basetype>::type::run(data,newpointer))
					{
						call_set_data(inner,*data);
						return true;
					}
					return false;
				}

				virtual bool set_pointer_struct(struct_base* newpointer)
				{
					if(run_pointer_from_struct<basetype>::type::run(data,newpointer))
					{
						call_set_data(inner,*data);
						return true;
					}
					return false;
				}

				virtual member_info* get_inner()
				{
					return &inner;
				}

			private:
				typedef typename remove_cv< typename remove_pointer<basetype>::type >::type basetype_noconst;
				
				void set_data(basetype* newdata)
				{
					data = newdata;
					call_set_data(inner,*data);
				}
				void set_data(const basetype* newdata)
				{
					data = const_cast<basetype*>(newdata);
					call_set_data(inner,*data);
				}
				
				friend struct member_info;
				
				basetype* data;


				inner_info< typename selector< basetype_noconst >::type, basetype_noconst > inner;
			};		
		}

		using detail::pointer_info_type;

		/// 
		/// \brief Accessor for reference types.
		/// 
		/// Provides access to reference data.
		/// 
		/// Allows you to access the inner type of the reference.
		/// 
		/// This means for Type& you can access the inner Type.
		/// 
		/// Similar warnings to pointer_info apply here as well.
		/// 
		struct reference_info : member_info
		{
			reference_info(const data_field& f)
				:
				member_info(f)
			{}

			virtual void visit(visitor_base& visitor)
			{
				visitor.visit(*this);
			}

			/// 
			/// \brief Accessor for the inner type.
			/// 
			virtual member_info* get_inner() = NULL;

			///
			/// \brief Set the pointer value of the reference, unsafe.
			/// 
			/// Only use this if you know the pointer types match.
			/// 
			virtual void  set_pointer(void* newpointer) = NULL;

			///
			/// \brief Get the pointer value of the reference as void*.
			/// 
			/// The only safe use of this is comparison to NULL.
			/// 
			virtual void* get_pointer()					= NULL;

			/// 
			/// \brief Is the pointer value of the reference NULL?
			/// 
			bool is_null()
			{
				return get_pointer() == NULL;
			}
		};

		namespace detail
		{
			template< class basetype >
			struct reference_info_type : reference_info
			{
				typedef reference_info super;

				typedef basetype& data_type;

				reference_info_type() 
					: 
					super(invalid_field::get_instance()),
					data(NULL) 
				{}

				reference_info_type(const data_field& f, reference_wrapper<basetype> r)
					:
					super(f),
					data(r.get_pointer())
				{
					call_set_data(inner,data);
				}

				size_t type_size()
				{
					return sizeof(basetype*);
				}

				member_info* new_copy()
				{
					return new reference_info_type<basetype>(*this);
				}

				void* address()
				{
					return data;
				}

				virtual void* get_pointer()
				{
					return data;
				}

				virtual void set_pointer(void* newpointer)
				{
					data = static_cast<basetype*>(newpointer);
					call_set_data(inner,data);
				}

				virtual member_info* get_inner()
				{
					return &inner;
				}

			private:
				void set_data(basetype* newdata)
				{
					data = newdata;
					call_set_data(inner,data);
				}
				void set_data(const basetype* newdata)
				{
					data = const_cast<basetype*>(newdata);
					call_set_data(inner,data);
				}

				friend struct member_info;

				//NOTE: only one indirection is needed,
				//		simply because you cannot stick references inside containers
				basetype* data;

				typedef typename remove_cv<basetype>::type basetype_noconst;

				inner_info< typename selector< basetype_noconst >::type, basetype_noconst > inner;		
			};
		}
		
		using detail::reference_info_type;

		/// 
		/// \brief Accessor for STL pair types.
		/// 
		/// Provides access to pair data.
		/// 
		/// Allows you to access the first and second data in the pair.
		/// 
		struct pair_info : member_info
		{
			pair_info(const data_field& f)
				:
				member_info(f)
			{}

			virtual void visit(visitor_base& visitor)
			{
				visitor.visit(*this);
			}

			/// 
			/// \brief Get the accessor for the first data member.
			/// 
			virtual member_info* get_first() = NULL;

			/// 
			/// \brief Get the accessor for the second data member.
			/// 
			virtual member_info* get_second() = NULL;
		};

		namespace detail
		{
			template< class basetype >
			struct pair_info_type : pair_info
			{
				typedef pair_info super;

				typedef basetype& data_type;

				typedef typename basetype::first_type	first_type;
				typedef typename basetype::second_type	second_type;

				pair_info_type() 
					: 
				super(invalid_field::get_instance()),
					data(NULL) 
				{}

				pair_info_type(const data_field& f, basetype& indata)
					:
				super(f),
					data(&indata)
				{
					call_set_data(first,	&data->first);
					call_set_data(second,	&data->second);
				}

				size_t type_size()
				{
					return sizeof(basetype);
				}

				member_info* new_copy()
				{
					return new pair_info_type< basetype >(*this);
				}

				void* address()
				{
					return data;
				}

				member_info* get_first()
				{
					return &first;
				}

				member_info* get_second()
				{
					return &second;
				}

			private:
				void set_data(basetype* newdata)
				{
					data = newdata;
					call_set_data(first,	&data->first);
					call_set_data(second,	&data->second);
				}
				void set_data(const basetype* newdata)
				{
					data = const_cast<basetype*>(newdata);
					call_set_data(first,	&data->first);
					call_set_data(second,	&data->second);
				}

				friend struct member_info;

				typedef typename remove_cv< first_type >::type	first_type_noconst;
				typedef typename remove_cv< second_type >::type second_type_noconst;

				inner_info< typename selector< first_type_noconst >::type, first_type_noconst >   first;	
				inner_info< typename selector< second_type_noconst >::type, second_type_noconst > second;

				basetype* data;
			};


			//
			// type selector configuration
			//
			
			template<typename data_type>
			struct selector
			{
				// this implements a metaprogram to
				// select between containers,
				// struct, class, enum, bytes, and unknown_info
				
				typedef
					typename
					IF< is_opclass<data_type>::value,
					//THEN
						class_info_type<data_type>,
					//ELSE
						typename
						IF< is_openum<data_type>::value,
						//THEN
							enum_info_type<data_type>,
						//ELSE
							typename
							IF< is_opstruct<data_type>::value,	
							//THEN
								struct_info_type<data_type>,
							//ELSE
								typename
								IF< is_bytes<data_type>::value,
								//THEN
									bytes_info_type<data_type>,
								//ELSE
									unknown_info_type<data_type>
								>::RET
							>::RET
						>::RET
					>::RET
				type;
			};

			//basic type selections
			template<>
			struct selector<float>
			{
				typedef float_info type;
			};
			
			template<>
			struct selector<double>
			{
				typedef double_info type;
			};

			template<>
			struct selector<int>
			{
				typedef int_info type;
			};

			template<>
			struct selector<bool>
			{
				typedef bool_info type;
			};

			template<>
			struct selector<std::string>
			{
				typedef string_info type;
			};
		
			template<>
			struct selector<std::wstring>
			{
				typedef wstring_info type;
			};

			//void* special case
			template<>
			struct selector< void* >
			{
				typedef unknown_info_type<void*> type;
			};
			
			template<>
			struct selector< const void* >
			{
				typedef unknown_info_type<const void*> type;
			};

			//redirect const types to normal accessors
			template<typename t>
			struct selector<const t>
			{
				typedef typename selector< t >::type type;
			};
			
			//strip volatile
			template<typename t>
			struct selector<volatile t>
			{
				typedef typename selector< t >::type type;
			};

			//strip const volatile
			template<typename t>
			struct selector<const volatile t>
			{
				typedef typename selector< t >::type type;
			};

			//redirect pointers of any indirection to pointer_info
			template<typename t>
			struct selector<t*>
			{
				typedef pointer_info_type<t*> type;
			};			

			//const pointers
			template<typename t>
			struct selector<const t*>
			{
				typedef pointer_info_type<t*> type;
			};		
			
			//v pointers
			template<typename t>
			struct selector<volatile t*>
			{
				typedef pointer_info_type<t*> type;
			};	
			
			//cv pointers
			template<typename t>
			struct selector<const volatile t*>
			{
				typedef pointer_info_type<t*> type;
			};

			template<typename t>
			struct selector<t&>
			{
				typedef reference_info_type<t> type;
			};
			
			//const references
			template<typename t>
			struct selector<const t&>
			{
				typedef reference_info_type<t> type;
			};
			
			//v references
			template<typename t>
			struct selector<volatile t&>
			{
				typedef reference_info_type<t> type;
			};

			//cv references
			template<typename t>
			struct selector<const volatile t&>
			{
				typedef reference_info_type<t> type;
			};

			template<typename t, size_t n>
			struct selector<t[n]>
			{
				typedef array_info_type<t,n> type;
			};

			template<typename t, size_t n>
			struct selector<const t[n]>
			{
				typedef array_info_type<t,n> type;
			};

			template<typename t, size_t n>
			struct selector<volatile t[n]>
			{
				typedef array_info_type<t,n> type;
			};

			template<typename t, size_t n>
			struct selector<const volatile t[n]>
			{
				typedef array_info_type<t,n> type;
			};
			
			//2d array - need a version that can handle n-dim
			template<typename t, size_t n, size_t m>
			struct selector<t[n][m]>
			{
				typedef array_info_type<t,n*m> type;
			};

			template<typename t, size_t n, size_t m>
			struct selector<const t[n][m]>
			{
				typedef array_info_type<t,n*m> type;
			};

			template<typename t, size_t n, size_t m>
			struct selector<volatile t[n][m]>
			{
				typedef array_info_type<t,n*m> type;
			};

			template<typename t, size_t n, size_t m>
			struct selector<const volatile t[n][m]>
			{
				typedef array_info_type<t,n*m> type;
			};

		}
		
		using detail::selector;
		using detail::pair_info_type;



		//
		// cast function definitions
		//

		/// 
		/// \brief Build an accessor on the stack for passing into a visitor.
		/// 
		/// This should usually not be used, but can be for reflecting
		/// custom data with invalid_field.
		/// 
		template< class T >
		inline 
			typename selector<T>::type
			build_accessor(T& value)
		{
			return 
				typename
				selector<T>::type
				(
					opcpp::fields::invalid_field::get_instance(),
					typename opcpp::metafunction::wraptype<T>::type(value)
				);
		}
		
		/// 
		/// \brief Build an accessor on the stack for passing into a visitor, along with a data field.
		/// 
		/// This should usually not be used, but can be for reflecting
		/// custom data with a custom field.
		/// 
		template< class T >
		inline 
			typename selector<T>::type
			build_accessor(T& value, opcpp::fields::data_field& field)
		{
			return 
				typename
				selector<T>::type
				(
					field,
					opcpp::metafunction::wraptype<T>::type(value)
				);
		}

		template<typename type>
		inline type* member_info::cast()
		{
			if(run_member_cast<type>::can_cast(this))
				return (type*)address();
			return NULL;
		}


		namespace detail
		{
			template<typename type>
			inline bool struct_member_cast<type>::can_cast(member_info* info)
			{
				if(struct_info* structinfo = info_cast<struct_info>(info))
				{
					void* myid = opcpp::metadata::id<type>::get_id();
					void* id   = structinfo->get_id();
					if(myid == id)
						return true;
				}
				return false;
			}

			template<typename type>
			inline bool class_member_cast<type>::can_cast(member_info* info)
			{
				if(class_info* classinfo = info_cast<class_info>(info))
				{
					typedef get_class_type<type> class_type;

					class_type* entry = classinfo->get_type();

					int id = class_type::static_start_id();

					if(entry->contains_id(id))
						return true;
				}
				return false;
			}

			template<typename type>
			inline bool enum_member_cast<type>::can_cast(member_info* info)
			{
				if(enum_info* enuminfo = info_cast<enum_info>(info))
				{
					void* myid = opcpp::metadata::id<type>::get_id();
					void* id = enuminfo->get_id();
					if(myid == id)
						return true;
				}
				return false;
			}

			template<typename type>
			inline bool bytes_member_cast<type>::can_cast(member_info* info)
			{
				if(bytes_info* bytes = info_cast<bytes_info>(info))
				{
					return bytes->can_cast_bytes<type>();
				}
				return false;
			}

		}
	}


	namespace casting
	{
		using opcpp::accessors::member_info;

		namespace detail
		{
			template<class type>
			info_cast_visitor<type>::info_cast_visitor(member_info* info)
				: 
				bresult(false)
			{
				info->visit(*this);
			}
		}

		/// \brief Quickly cast from one accessor type to another.

		///
		/// \code
		/// using opcpp::casting::info_cast;
		/// 
		/// int_info* i = info_cast< int_info >(accessor);
		/// \endcode
		/// 
		template<class type>
		inline type* info_cast(member_info* info)
		{
			info_cast_visitor<type> visitor(info);
			if(visitor.result())
				return (type*)info;
			return NULL;
		}

		/// \brief Quickly cast from one accessor type to another (reference version).

		///
		/// \code
		/// using opcpp::casting::info_cast;
		/// 
		/// int_info* i = info_cast< int_info >(accessor);
		/// \endcode
		/// 
		template<class type>
		inline type* info_cast(member_info& info)
		{
			return info_cast<type>(&info);
		}
	}

}


opmacro add_basic_cast(type,type_name)
{
	template<>
	struct select_member_cast< type >
	{
		static bool can_cast(type_name@_info* info)
		{
			return true;
		}

		static bool can_cast(member_info* info)
		{
			if(info_cast_visitor<type_name@_info>(info).result())
				return true;
			return false;
		}
	};
}

namespace opcpp
{
	namespace fields
	{
		namespace detail
		{
			template<class type>
			class get_value_visitor : public visitor_base
			{
			public:
				get_value_visitor()
					: value(0)
				{}

				void visit(member_info& info)
				{
					value = info.cast<type>();
				}

				type* get_value()
				{
					return value;
				}

			private:
				type* value;
			};
		}

		using detail::get_value_visitor;

		template<class type>
		inline type* data_field::get_value(class_base* object)
		{
			opcpp::fields::get_value_visitor<type> visitor;
			visit(object,visitor);
			return visitor.get_value();
		}
	}

	namespace accessors
	{
		namespace detail
		{
			expand add_basic_cast(int,int);
			expand add_basic_cast(float,float);
			expand add_basic_cast(double,double);
			expand add_basic_cast(bool,bool);
			expand add_basic_cast(std::string,string);
			expand add_basic_cast(std::wstring,wstring);

			//pointer extraction
			template<typename type>
			struct select_member_cast< type* >
			{
				static bool can_cast(pointer_info* info)
				{
					return run_member_cast<type>::can_cast(info->get_inner());
				}

				static bool can_cast(member_info* info)
				{
					if(pointer_info* pointer = info_cast<pointer_info>(info))
						return can_cast(pointer);
					return false;
				}
			};

		}
	}
}

