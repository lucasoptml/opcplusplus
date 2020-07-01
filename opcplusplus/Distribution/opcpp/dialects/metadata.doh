//****************************************************************
// Copyright (C) 2007 OP Games LLC - All Rights Reserved
//
// Authors: Lucas Ellis
//
// File: metadata.doh
// Date: 07/25/2007
//
// Description:
//
// Metadata template functions
//****************************************************************


namespace opcpp
{
	namespace metadata
	{
		struct no_parent {};

		template<class c>
		struct get_parent
		{
			typedef no_parent type;
		};

		template<typename type>
		struct get_size
		{
			enum { value = sizeof(type) };
		};
			
		template<typename type>
		struct get_size<type&>
		{
			enum { value = sizeof(type*) };
		};

// 		template<class type>
// 		struct get_name
// 		{
// 			static const char* get() { return ""; }
// 		};
// 		
// 		//basic type definitions
// 		template<>
// 		struct get_name<int>
// 		{
// 			static const char* get() { return "int"; }
// 		};
// 		
// 		template<>
// 		struct get_name<float>
// 		{
// 			static const char* get() { return "float"; }
// 		};
// 		
// 		template<>
// 		struct get_name<bool>
// 		{
// 			static const char* get() { return "bool"; }
// 		};
// 		
// 		template<>
// 		struct get_name<std::string>
// 		{
// 			static const char* get() { return "string"; }
// 		};

		//
		// Byte Types - char, bytes, integers of various size.
		//

		template<class type>
		struct is_bytes
		{
			enum { value = false };
		};

		template<>
		struct is_bytes<char>
		{
			enum { value = true };
		};

		template<>
		struct is_bytes<unsigned char>
		{
			enum { value = true };
		};

		template<>
		struct is_bytes<short>
		{
			enum { value = true };
		};

		template<>
		struct is_bytes<unsigned short>
		{
			enum { value = true };
		};

		template<>
		struct is_bytes<long>
		{
			enum { value = true };
		};

		template<>
		struct is_bytes<unsigned long>
		{
			enum { value = true };
		};

		template<>
		struct is_bytes<int>
		{
			enum { value = true };
		};
		
		template<>
		struct is_bytes<unsigned int>
		{
			enum { value = true };
		};

		template<>
		struct is_bytes<long long>
		{
			enum { value = true };
		};

		template<>
		struct is_bytes<unsigned long long>
		{
			enum { value = true };
		};

		//
		// Unsigned Types
		//

		template<class type>
		struct is_unsigned
		{
			enum { value = false };
		};

		template<>
		struct is_unsigned< unsigned char >
		{
			enum { value = true };
		};

		template<>
		struct is_unsigned< unsigned short >
		{
			enum { value = true };
		};

		template<>
		struct is_unsigned< unsigned long >
		{
			enum { value = true };
		};

		template<>
		struct is_unsigned< unsigned int >
		{
			enum { value = true };
		};

		template<>
		struct is_unsigned< unsigned long long >
		{
			enum { value = true };
		};

		//
		// Basic Types
		//
		
		template<class type>
		struct is_basic
		{
			enum 
			{ 
				//if a type is bytes its also basic.
				value = is_bytes<type>::value 
			};
		};

		template<>
		struct is_basic<bool>
		{
			enum { value = true };
		};

		template<>
		struct is_basic<float>
		{
			enum { value = true };
		};
		
		template<>
		struct is_basic<double>
		{
			enum { value = true };
		};

		template<>
		struct is_basic<std::string>
		{
			enum { value = true };
		};


		template<class type>
		struct has_default_constructor
		{
			enum { value = false };
		};

		using opcpp::metafunction::is_pointer;

		template<class type>
		struct is_serializable
		{
			enum { 
				value = 
					is_pointer<type>::value ? true :
					is_basic<type>::value ? true :
					has_default_constructor<type>::value ? true :
				false
			};
		};
		
		template<class type>
		struct is_serializable<const type>
		{
			enum { value = is_serializable< type >::value };
		};
		
		template<class type>
		struct is_serializable<const volatile type>
		{
			enum { value = is_serializable< type >::value };
		};
		
		template<class type>
		struct is_serializable<volatile type>
		{
			enum { value = is_serializable< type >::value };
		};
		
		template<class type>
		struct field_count
		{
			enum
			{
				value = 0,
				total = 0,
			};
		};
	}
}