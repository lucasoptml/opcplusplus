//****************************************************************
// Copyright (C) 2007 OP Games LLC - All Rights Reserved
//
// Authors: Lucas Ellis
//
// File: metafunction.doh
// Date: 10/21/2007
//
// Description:
//
// Metafunctions
//****************************************************************

namespace opcpp
{
	namespace metafunction
	{
		//Conditional IF /Then / Else metafunction
		template<bool Condition, class Then, class Else>
		struct IF
		{
			typedef Then RET;
		};
		
		template<class Then, class Else>
		struct IF<false,Then,Else>
		{
			typedef Else RET;
		};

		//Special product metafunction (1 or 2 args)
		template<int x, int y = 1>
		struct product
		{
			enum { value = x * y };
		};

		//Wrappers for passing reference types and removing qualifiers
		template<typename reftype>
		struct reference_wrapper
		{
			reference_wrapper(reftype& r)
				: pointer( &r )
			{
			}

			reference_wrapper(const reftype& r)
				: pointer( const_cast<reftype*>(&r) )
			{
			}
			
			reference_wrapper(volatile reftype& r)
				: pointer( const_cast<reftype*>(&r) )
			{
			}

			reference_wrapper(const volatile reftype& r)
				: pointer( const_cast<reftype*>(&r) )
			{
			}

			reftype* get_pointer()
			{
				return pointer;
			}

		private:
			reftype* pointer;
		};

		//TODO: this process is probably bad,
		//		could remove c/v then handle references (2 stage)

		template<typename t>
		struct wraptype
		{
			typedef t& type;

			static t& wrap(t& data)
			{
				return data;
			}
		};

		template<typename t>
		struct wraptype<const t>
		{
			typedef t& type;

			static t& wrap(const t& data)
			{
				return const_cast<type>(data);
			}
		};

		template<typename t>
		struct wraptype<volatile t>
		{
			typedef t& type;

			static t& wrap(volatile t& data)
			{
				return const_cast<type>(data);
			}
		};


		template<typename t>
		struct wraptype<const volatile t>
		{
			typedef t& type;

			static t& wrap(const volatile t& data)
			{
				return const_cast<type>(data);
			}
		};

		template<typename t>
		struct wraptype<const t*>
		{
			typedef t*& type;

			static t*& wrap(const t*& data)
			{
				return const_cast<type>(data);
			}
		};

		template<typename t>
		struct wraptype<volatile t*>
		{
			typedef t*& type;

			static t*& wrap(volatile t*& data)
			{
				return const_cast<type>(data);
			}
		};


		template<typename t>
		struct wraptype<const volatile t*>
		{
			typedef t*& type;

			static t*& wrap(const volatile t*& data)
			{
				return const_cast<type>(data);
			}
		};

		//we need to actually wrap reference types (no reference to reference in c++)
		template<typename t>
		struct wraptype<t&>
		{
			typedef reference_wrapper<t> type;

			static reference_wrapper<t> wrap(t& data)
			{
				return reference_wrapper<t>(data);
			}
		};

		template<typename t>
		struct wraptype<const t&>
		{
			typedef reference_wrapper<t> type;

			static reference_wrapper<t> wrap(const t& data)
			{
				return reference_wrapper<t>(data);
			}
		};

		template<typename t>
		struct wraptype<volatile t&>
		{
			typedef reference_wrapper<t> type;

			static reference_wrapper<t> wrap(volatile t& data)
			{
				return reference_wrapper<t>(data);
			}
		};

		template<typename t>
		struct wraptype<const volatile t&>
		{
			typedef reference_wrapper<t> type;

			static reference_wrapper<t> wrap(const volatile t& data)
			{
				return reference_wrapper<t>(data);
			}
		};

		//Arrays - handles 1 and 2 dimensions
		template<typename t, size_t n>
		struct wraptype<t[n]>
		{
			typedef t* type;

			static t* wrap(t data[n])
			{
				return data;
			}
		};


		template<typename t, size_t n>
		struct wraptype<const t[n]>
		{
			typedef t* type;

			static t* wrap(const t data[n])
			{
				return const_cast<t*>(data);
			}
		};

		template<typename t, size_t n>
		struct wraptype<volatile t[n]>
		{
			typedef t* type;

			static t* wrap(volatile t data[n])
			{
				return const_cast<t*>(data);
			}
		};

		template<typename t, size_t n>
		struct wraptype<const volatile t[n]>
		{
			typedef t* type;

			static t* wrap(const volatile t data[n])
			{
				return const_cast<t*>(data);
			}
		};

		template<typename t, size_t n, size_t m>
		struct wraptype<t[n][m]>
		{
			typedef t* type;

			static t* wrap(t data[n][m])
			{
				return data[0];
			}
		};


		template<typename t, size_t n, size_t m>
		struct wraptype<const t[n][m]>
		{
			typedef t* type;

			static t* wrap(const t data[n][m])
			{
				return const_cast<t*>(data[0]);
			}
		};

		template<typename t, size_t n, size_t m>
		struct wraptype<volatile t[n][m]>
		{
			typedef t* type;

			static t* wrap(volatile t data[n][m])
			{
				return const_cast<t*>(data[0]);
			}
		};

		template<typename t, size_t n, size_t m>
		struct wraptype<const volatile t[n][m]>
		{
			typedef t* type;

			static t* wrap(const volatile t data[n][m])
			{
				return const_cast<t*>(data[0]);
			}
		};


		//is a type a pointer?
		template<class type>
		struct is_pointer
		{
			enum { value = false };
		};

		template<class type>
		struct is_pointer<type*>
		{
			enum { value = true };
		};

		template<class type>
		struct is_pointer<const type*>
		{
			enum { value = true };
		};

		template<class type>
		struct is_pointer<volatile type*>
		{
			enum { value = true };
		};


		template<class type>
		struct is_pointer<const volatile type*>
		{
			enum { value = true };
		};

		template<class t>
		struct remove_pointer
		{
			typedef t type;
		};

		template<class t>
		struct remove_pointer<t*>
		{
			typedef t type;
		};

		template<class t>
		struct remove_cv
		{
			typedef t type;
		};

		template<class t>
		struct remove_cv<const t>
		{
			typedef t type;
		};

		template<class t>
		struct remove_cv<volatile t>
		{
			typedef t type;
		};

		template<class t>
		struct remove_cv<const volatile t>
		{
			typedef t type;
		};

		template<class t>
		struct pointer_from_void
		{
			static t convert( void* p )
			{
				return reinterpret_cast<t>(p);
			}
		};
	}
}