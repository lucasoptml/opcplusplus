//****************************************************************
// Copyright (C) 2007 OP Games LLC - All Rights Reserved
//
// Authors: Lucas Ellis
//
// File: accessors.doh
// Date: 10/21/2007
//
// Description:
//
// Accessors and visitors
//****************************************************************

// This extension point allows you to add functions
// into visitor_base.
extension visitor_base_body;

namespace opcpp
{
	namespace casting
	{
		struct cast_accessor;
	}	

	namespace accessors
	{
		struct member_info;
			struct basic_info;
				struct float_info;
				struct double_info;
				struct bool_info;
				struct int_info;
				struct text_info;
					struct string_info;
					struct wstring_info;
				struct bytes_info;
		struct enum_info;
		struct array_info;
		struct container_info;
			struct value_container_info;	
				struct vector_info;
				struct set_info;
				struct multiset_info;
				struct deque_info;
				struct list_info;
			struct key_container_info;
				struct map_info;
				struct multimap_info;
		struct class_info;
		struct struct_info;
		struct pointer_info;
		struct reference_info;
		struct pair_info;
		struct unknown_info;
	}
	
	namespace base
	{
		using opcpp::accessors::member_info;
			using opcpp::accessors::basic_info;
				using opcpp::accessors::float_info;
				using opcpp::accessors::double_info;
				using opcpp::accessors::int_info;
				using opcpp::accessors::text_info;
					using opcpp::accessors::string_info;
					using opcpp::accessors::wstring_info;
				using opcpp::accessors::bool_info;
				using opcpp::accessors::bytes_info;
			using opcpp::accessors::array_info;
			using opcpp::accessors::container_info;
				using opcpp::accessors::value_container_info;
					using opcpp::accessors::vector_info;
					using opcpp::accessors::deque_info;
					using opcpp::accessors::list_info;
					using opcpp::accessors::set_info;
					using opcpp::accessors::multiset_info;
				using opcpp::accessors::key_container_info;
					using opcpp::accessors::map_info;
					using opcpp::accessors::multimap_info;
			using opcpp::accessors::class_info;
			using opcpp::accessors::struct_info;
			using opcpp::accessors::enum_info;
			using opcpp::accessors::pointer_info;
			using opcpp::accessors::reference_info;
			using opcpp::accessors::pair_info;
			using opcpp::accessors::unknown_info;

		/// 
		/// \brief Base class for opC++ visitors.
		///
		/// To implement a new visitor, you must inherit from this class.
		///
		/// Usage:
		/// \code
		/// using opcpp::base::visitor_base;
		///
		/// class my_visitor : public visitor_base
		/// {
		/// 
		/// };
		/// \endcode
		class visitor_base
		{
		public:

			//NOTE: write for a specific type by overloading that type,
			//		or write one general function, and use the to-functions to derive them
			//		(writing all overloads is faster and is recommended)

			///
			/// \brief Visit any type.
			///
			/// All the other info classes inherit from this class.  Overriding the 
			/// visit method for member_info will catch any data members not caught by more
			/// specific visit methods.
			virtual void visit(member_info& info)
			{

			}

				///
				/// \brief Visit a basic type.
				/// 
				/// Handles:
				///		- float
				///		- double			
				///		- bool			
				///		- int			
				///		- string
				///		- bytes			
				///
				/// If not caught, forwards to the member_info overload.
				///
				virtual void visit(basic_info& info)
				{
					visit((member_info&)info);
				}
					
					///
					/// \brief Visit a float.
					///
					/// If not caught, forwards to the basic_info overload.
					///
					virtual void visit(float_info& info)
					{
						visit((basic_info&)info);
					}

					///
					/// \brief Visit a double.
					///
					/// If not caught, forwards to the basic_info overload.
					///
					virtual void visit(double_info& info)
					{
						visit((basic_info&)info);
					}

					///
					/// \brief Visit a int.
					///
					/// If not caught, forwards to the basic_info overload.
					///
					virtual void visit(int_info& info)
					{
						visit((basic_info&)info);
					}

					///
					/// \brief Visit a text type.
					///
					/// If not caught, forwards to the basic_info overload.
					///
					virtual void visit(text_info& info)
					{
						visit((basic_info&)info);
					}

						///
						/// \brief Visit a stl string.
						///
						/// If not caught, forwards to the text_info overload.
						///
						virtual void visit(string_info& info)
						{
							visit((text_info&)info);
						}

						///
						/// \brief Visit a stl string.
						///
						/// If not caught, forwards to the text_info overload.
						///
						virtual void visit(wstring_info& info)
						{
							visit((text_info&)info);
						}

					///
					/// \brief Visit a bool.
					///
					/// If not caught, forwards to the basic_info overload.
					///
					virtual void visit(bool_info& info)
					{
						visit((basic_info&)info);
					}

					///
					/// \brief Visit a byte-expressable type.
					///
					/// If not caught, forwards to the basic_info overload.
					///
					virtual void visit(bytes_info& info)
					{
						visit((basic_info&)info);
					}

				///
				/// \brief Visit an array of data.
				///
				/// If not caught, forwards to the member_info overload.
				///
				virtual void visit(array_info& info)
				{
					visit((member_info&)info);
				}

				///
				/// \brief Visit an stl container of data.
				///
				/// If not caught, forwards to the member_info overload.
				///
				virtual void visit(container_info& info)
				{
					visit((member_info&)info);
				}

					///
					/// \brief Visit a single value stl container of data.
					///
					/// Handles:
					///		- vector
					///		- deque
					///		- list
					///		- set
					///		- multiset
					///
					/// If not caught, forwards to the container_info overload.
					///
					virtual void visit(value_container_info& info)
					{
						visit((container_info&)info);
					}

						///
						/// \brief Visit an stl vector of data.
						///
						/// If not caught, forwards to the value_container_info overload.
						///
						virtual void visit(vector_info& info)
						{
							visit((value_container_info&)info);
						}

						///
						/// \brief Visit an stl deque of data.
						///
						/// If not caught, forwards to the value_container_info overload.
						///
						virtual void visit(deque_info& info)
						{
							visit((value_container_info&)info);
						}

						///
						/// \brief Visit an stl list of data.
						///
						/// If not caught, forwards to the value_container_info overload.
						///
						virtual void visit(list_info& info)
						{
							visit((value_container_info&)info);
						}

						///
						/// \brief Visit an stl set of data.
						///
						/// If not caught, forwards to the value_container_info overload.
						///
						virtual void visit(set_info& info)
						{
							visit((value_container_info&)info);
						}

						///
						/// \brief Visit an stl multiset of data.
						///
						/// If not caught, forwards to the value_container_info overload.
						///
						virtual void visit(multiset_info& info)
						{
							visit((value_container_info&)info);
						}

					///
					/// \brief Visit a key/value stl container of data.
					/// 
					/// Handles:
					///		- map
					///		- multimap
					/// 
					/// If not caught, forwards to the container_info overload.
					///
					virtual void visit(key_container_info& info)
					{
						visit((container_info&)info);
					}

						///
						/// \brief Visit an stl map of data.
						///
						/// If not caught, forwards to the key_container_info overload.
						///
						virtual void visit(map_info& info)
						{
							visit((key_container_info&)info);
						}

						///
						/// \brief Visit an stl multimap of data.
						///
						/// If not caught, forwards to the key_container_info overload.
						///
						virtual void visit(multimap_info& info)
						{
							visit((key_container_info&)info);
						}

				///
				/// \brief Visit an opC++ opclass.
				///
				/// If not caught, forwards to the member_info overload.
				///
				virtual void visit(class_info& info)
				{
					visit((member_info&)info);
				}

				///
				/// \brief Visit an opC++ opstruct.
				///
				/// If not caught, forwards to the member_info overload.
				///
				virtual void visit(struct_info& info)
				{
					visit((member_info&)info);
				}

				///
				/// \brief Visit an opC++ openum.
				///
				/// If not caught, forwards to the member_info overload.
				///
				virtual void visit(enum_info& info)
				{
					visit((member_info&)info);
				}

				///
				/// \brief Visit a pointer.
				///
				/// If not caught, forwards to the member_info overload.
				///
				virtual void visit(pointer_info& info)
				{
					visit((member_info&)info);
				}

				///
				/// \brief Visit a reference.
				///
				/// If not caught, forwards to the member_info overload.
				///
				virtual void visit(reference_info& info)
				{
					visit((member_info&)info);
				}

				///
				/// \brief Visit an stl pair.
				///
				/// If not caught, forwards to the member_info overload.
				///
				virtual void visit(pair_info& info)
				{
					visit((member_info&)info);
				}

				///
				/// \brief Visit an unknown type.
				///
				/// If not caught, forwards to the member_info overload.
				///
				virtual void visit(unknown_info& info)
				{
					visit((member_info&)info);
				}

				extensionpoint visitor_base_body;
		};
	}
}
