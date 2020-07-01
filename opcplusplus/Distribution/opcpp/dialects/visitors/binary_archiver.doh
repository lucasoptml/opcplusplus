//****************************************************************
// Copyright (C) 2007 opGames Inc. - All Rights Reserved
//
// Authors: Kevin Depue & Lucas Ellis
//
// File: binaryserialize.doh
// Date: 10/30/2007
//
// Description:
//
// Binary Load/Save Visitors for opC++
//****************************************************************

//Requires: opC++ Standard Dialect
//			opinclude "opc++dialect.doh" first!
//
//Usage:
//		Dialect:
//			opinclude "visitors/binaryserialize.doh"
//		Saving:
//			opcpp::visitors::binary_save_archiver visitor(output text stream);
//			visitor << object;
//			visitor << data;
//		Loading:
//			opcpp::visitors::binary_load_archiver visitor(input text stream);
//			visitor >> object;
//			visitor >> data;

//TODO: not all array types are supported yet.
//		not all vector types are optimized yet.
//		pair is not supported yet.

code heading
{
	#ifndef OPCPP_BINARY_SERIALIZE_DOH
	#define OPCPP_BINARY_SERIALIZE_DOH


	#include <iostream>
	#include <map>
	#include <string>
	#include <vector>
	#include <streambuf>
}

namespace opcpp
{
	namespace visitors
	{
		// Consists of these visitor states:
		//	Visitor   - base visitor, derives from Fields
		//	Fields    - named fields - handles transient, derives from Data
		//	Container - container visitor, derives from Data
		//	Vector    - optimized vector visitor, derives from Data
		//	Pointer   - pointer visitor - handles pointers
		//	Data      - unnamed fields
		//	Array	  - array visitor

		using std::string;
		using std::istream;
		using namespace opcpp::accessors;
		using opcpp::base::visitor_base;
		using std::endl;
		using std::map;

		//Binary Save Visitor - the class you should use.
		template<class streamtype>
		class binary_save_archiver;

		namespace detail
		{
			//Binary Save Data Visitor - applied to un-named data fields, shared by Container and the main Visitor
			template<class streamtype>
			class binary_save_data_visitor : public visitor_base
			{
			public:
				binary_save_data_visitor(streamtype& o, binary_save_archiver<streamtype>& r) : file(o), root(&r) {}
				binary_save_data_visitor(streamtype& o) : file(o) {}

				template<typename t>
				void serialize_bytes(t& data)
				{
					file.sputn((char*)&data,sizeof(t));		
				}

				void visit(bool_info& info)
				{
					serialize_bytes(info.get_bool());
				}

				//int
				void visit(int_info& info)
				{
					serialize_bytes(info.get_int());
				}

				//serialize a string
				void serialize_string(const string& s)
				{
					size_t num = s.size();
					serialize_bytes(num);

					size_t length = s.length();
					if(length)
					{
						file.sputn((char*)&s[0],(std::streamsize)length);
					}
				}

				//serialize a string
				void serialize_wstring(const wstring& s)
				{
					size_t num = s.size();
					serialize_bytes(num);

					size_t length = s.length();
					if(length)
					{
						file.sputn((const char*)&s[0],(std::streamsize)length * sizeof(wstring::value_type));
					}
				}

				//string
				void visit(string_info& info)
				{
					string& s = info.get_string();

					serialize_string(s);
				}
				
				//wstring
				void visit(wstring_info& info)
				{
					wstring& s = info.get_wstring();

					serialize_wstring(s);
				}

				//float
				void visit(float_info& info)
				{
					serialize_bytes(info.get_float());
				}

				//double
				void visit(double_info& info)
				{
					serialize_bytes(info.get_double());
				}

				//bytes
				void visit(bytes_info& info)
				{
					void* address = info.address();
					size_t size = info.type_size();
					file.sputn((char*)address,(std::streamsize)size);
				}

				//enum
				void visit(enum_info& info)
				{
					int value;
					info.to_int(value);

					serialize_bytes(value);
				}

				//struct
				void visit(struct_info& info)
				{
					info.visit_data_members(*this);
				}

				//class
				void visit(class_info& info)
				{
					info.visit_data_members(*this);
				}

				//container
				void visit(value_container_info& info);

				//vector
				void visit(vector_info& info);

				//maps
				void visit(key_container_info& info);

				//pointer
				void visit(pointer_info& info);

				//array
				void visit(array_info& info);

				void visit(pair_info& info)
				{
					info.get_first()->visit(*this);
					info.get_second()->visit(*this);
				}

			protected:
				friend class opcpp::visitors::binary_save_archiver<streamtype>;

				binary_save_archiver<streamtype>* root;
				streamtype& file;
			};

			//NOTE: could easily be a template class.
			//Binary Save Field Visitor - applied to serialize named fields
			template<class streamtype>
			class binary_save_field_visitor : public binary_save_data_visitor<streamtype>
			{
			public:
				binary_save_field_visitor(streamtype& o, binary_save_archiver<streamtype>& root) : binary_save_data_visitor(o,root) {}
				binary_save_field_visitor(streamtype& o) : binary_save_data_visitor<streamtype>(o) {}

				typedef binary_save_data_visitor<streamtype> super;

				//bool
				void visit(bool_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//int
				void visit(int_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//string
				void visit(string_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//wstring
				void visit(wstring_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//float
				void visit(float_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//double
				void visit(double_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//bytes
				void visit(bytes_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//enum
				void visit(enum_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//struct
				void visit(struct_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//class
				void visit(class_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//container
				void visit(value_container_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//vector
				void visit(vector_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//maps
				void visit(key_container_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//pointer
				void visit(pointer_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//arrays
				void visit(array_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//pairs
				void visit(pair_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}
			};

			//Binary Save Pointer Visitor - applied to serialize pointer types
			template<class streamtype>
			class binary_save_pointer_visitor : public visitor_base
			{
			public:
				binary_save_pointer_visitor(streamtype& f, binary_save_archiver<streamtype>& r, pointer_info& p) : pointer(p), file(f), root(r) {}

				template<typename t>
				void serialize_bytes(t& data)
				{
					file.sputn((char*)&data,sizeof(t));
				}

				//pointer to struct
				void visit(struct_info& info);

				//pointer to class
				void visit(class_info& info);

			protected:
				binary_save_archiver<streamtype>& root;
				streamtype&		      file;
				pointer_info&	      pointer;
			};

			//Binary Save Vector Visitor - applied to serialize container contents
			template<class streamtype>
			class binary_save_vector_visitor : public binary_save_data_visitor<streamtype>
			{
			public:
				binary_save_vector_visitor(streamtype& f, binary_save_archiver<streamtype>& root, vector_info& c) 
					: 
					binary_save_data_visitor<streamtype>(f,root),
					container(c)
				{}

				typedef binary_save_data_visitor<streamtype> super;

				void visit(bool_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(int_info& info)
				{
					//optimized saving of int vectors.
					//we know we're in a vector containing ints - so we can directly cast.
					std::vector<int>& vect = *(std::vector<int>*) container.address();
					std::streamsize size = (std::streamsize)(sizeof(int) * vect.size());
					
					if(size)
						file.sputn((char*)&vect[0], size );
				}

				void visit(string_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(wstring_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(float_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(double_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(bytes_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(enum_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(struct_info& info);

				void visit(class_info& info);

				//a vector of containers
				void visit(value_container_info& info);

				void visit(vector_info& info)
				{
					visit((value_container_info&)info);
				}

				void visit(key_container_info& info);

				//pointers
				void visit(pointer_info& info)
				{
					binary_save_pointer_visitor<streamtype> visitor(file,*root,info);

					for(bool i = container.begin(); i; i = container.increment())
					{
						info.get_inner()->visit(visitor);
					}
				}

				void visit(pair_info& info);

			protected:
				vector_info& container;
			};

			//Binary Save Array Visitor - applied to serialize array contents
			template<class streamtype>
			class binary_save_array_visitor : public binary_save_data_visitor<streamtype>
			{
			public:
				binary_save_array_visitor(streamtype& f, binary_save_archiver<streamtype>& root, array_info& c) 
					: 
					binary_save_data_visitor<streamtype>(f,root),
					container(c)
				{}

				typedef binary_save_data_visitor super;

				void visit(bool_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(int_info& info)
				{
					std::streamsize size = (std::streamsize)(sizeof(int)*container.size());
					void* address = container.address();
					file.sputn((char*)address, size );
				}

				void visit(string_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(wstring_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(float_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(double_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(bytes_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(enum_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(struct_info& info);
				
				void visit(class_info& info);

				void visit(pointer_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(vector_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(value_container_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}
				
 				void visit(key_container_info& info)
				{
					binary_save_data_visitor visitor(file,*root);

					for(bool i = container.begin(); i; i = container.increment())
						info.visit(visitor);
				}

				void visit(pair_info& info);

			protected:
				array_info& container;
			};


			//Binary Save Container Visitor - applied to serialize container contents
			template<class streamtype>
			class binary_save_container_visitor : public binary_save_data_visitor<streamtype>
			{
			public:
				binary_save_container_visitor(streamtype& f, binary_save_archiver<streamtype>& root, value_container_info& c) 
					: 
					binary_save_data_visitor<streamtype>(f,root),
					container(c)
				{}

				typedef binary_save_data_visitor super;

				void visit(bool_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(int_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(string_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(wstring_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(float_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(double_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(bytes_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(enum_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
						super::visit(info);
				}

				void visit(struct_info& info);

				void visit(class_info& info);

				//a container of containers
				void visit(value_container_info& info);

				void visit(key_container_info& info);

				void visit(vector_info& info)
				{
					visit((value_container_info&)info);
				}

				//pointers
				void visit(pointer_info& info)
				{
					binary_save_pointer_visitor<streamtype> visitor(file,*root,info);

					for(bool i = container.begin(); i; i = container.increment())
					{
						info.get_inner()->visit(visitor);
					}
				}

				void visit(pair_info& info);

			protected:
				value_container_info& container;
			};
		}

		using detail::binary_save_data_visitor;
		using detail::binary_save_field_visitor;
		using detail::binary_save_pointer_visitor;
		using detail::binary_save_vector_visitor;
		using detail::binary_save_array_visitor;
		using detail::binary_save_container_visitor;

		class has_complex_data_visitor : public visitor_base
		{
		public:
			has_complex_data_visitor() : bresult(false), size(0)
			{}

			bool get_result()
			{
				return bresult;
			}

			int get_size()
			{
				return size;
			}

		private:
			bool bresult;
			size_t size;

		public:
			void handle_info(member_info& info)
			{
				if(info.is_transient())
					bresult = true;

				size += info.type_size();
			}

			void visit(int_info& info)
			{
				handle_info(info);
			}

			void visit(bytes_info& info)
			{
				handle_info(info);
			}

			void visit(float_info& info)
			{
				handle_info(info);
			}
			
			void visit(double_info& info)
			{		
				handle_info(info);
			}

			void visit(bool_info& info)
			{
				handle_info(info);
			}

			void visit(array_info& info)
			{
				handle_info(info);
			}

			void visit(enum_info& info)
			{
				handle_info(info);
			}

			void visit(struct_info& info)
			{
				if(info.is_transient())
					bresult = true;
				else if(!info.is_serializable())
					bresult = true;
				else if(!bresult)
				{
					info.visit_data_members(*this);
				}
			}

			void visit(class_info& info)
			{
				if(info.is_transient())
					bresult = true;
				else if(!info.is_serializable())
					bresult = true;
				else if(!bresult)
				{
					info.visit_data_members(*this);
				}
			}

			void visit(member_info& info)
			{
				bresult = true;
			}
		};


		/// \brief Archiver for saving data to binary.

		/// This archiver is able to save most any reflectable data type directly,
		/// including built-ins, stl, opclass, opstructs, and openums.
		/// 
		/// Use with binary_load_archiver to load and save data using binary.
		/// 
		/// This archiver writes directly to the stream and should have very good performance.
		///
		/// Usage:
		/// \code
		/// binary_save_archiver archiver(stream);
		/// 
		/// archiver << data;
		/// \endcode
		/// 
		template<class streamtype>
		class binary_save_archiver
		{
		public:
			
			/// 
			/// \brief The constructor, requires an output stream.
			/// 
			/// Supported streams:
			///		- ofstream
			///		- stringstream
			///		- anything with sputn()
			/// 
			binary_save_archiver<streamtype>(streamtype& stream)
				: 
				pointercount(0), 
				datavisitor(*stream),
				fieldvisitor(*stream)
			{
				// Set stream states.
// 				stream.flags( ofstream::binary );
// 				stream << std::noskipws;

				datavisitor.root  = this;
				fieldvisitor.root = this;
			}
				
				
			template<class othertype>
			binary_save_archiver<streamtype>(othertype& stream)
				: 
				pointercount(0), 
				datavisitor(*stream.rdbuf()),
				fieldvisitor(*stream.rdbuf())
			{
				// Set stream states.
				// 				stream.flags( ofstream::binary );
				// 				stream << std::noskipws;

				datavisitor.root  = this;
				fieldvisitor.root = this;
			}

			/// 
			/// \brief Save data to the stream.
			/// 
			/// This can also save pointer data, which can be restored as another instance.
			/// 
			template< class T >
			friend binary_save_archiver& operator << ( binary_save_archiver& visitor, T& type )
			{
				opcpp::accessors::build_accessor(type).visit(visitor.fieldvisitor);
				return visitor;
			}
			
		private:
			bool HasPointer(void* pointer, int& index)
			{
				pointermaptype::iterator it	 = pointermap.find(pointer);
				pointermaptype::iterator end = pointermap.end();

				if(it != end)
				{
					index = (*it).second;
					return true;
				}

				index = pointercount;
				pointermap.insert( pointermaptype::value_type(pointer,pointercount) );
				++pointercount;

				return false;
			}

			binary_save_data_visitor<streamtype>	datavisitor;
			binary_save_field_visitor<streamtype>	fieldvisitor;

			friend binary_save_data_visitor<streamtype>;
			friend binary_save_field_visitor<streamtype>;
			friend binary_save_container_visitor<streamtype>;
			friend binary_save_vector_visitor<streamtype>;
			friend binary_save_pointer_visitor<streamtype>;
			friend binary_save_array_visitor<streamtype>;

		private:
			//pointer to index map.
			typedef map<void*,int>	pointermaptype;
			pointermaptype			pointermap;
			int						pointercount;
		};
		
		template<class streamtype>
		inline void binary_save_data_visitor<streamtype>::visit(array_info& info)
		{
			binary_save_array_visitor<streamtype> visitor(file,*root,info);
			
			member_info* inner = info.get_inner();
			
			inner->visit(visitor);
		}
		
		template<class streamtype>
		inline void binary_save_container_visitor<streamtype>::visit(struct_info& info)
		{
			binary_save_field_visitor<streamtype>& visitor = root->fieldvisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				info.visit_data_members(visitor);
			}
		}
		
		template<class streamtype>
		inline void binary_save_container_visitor<streamtype>::visit(pair_info& info)
		{
			binary_save_data_visitor<streamtype>& visitor = root->datavisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				info.get_first()->visit(visitor);
				info.get_second()->visit(visitor);
			}
		}
		
		template<class streamtype>
		inline void binary_save_container_visitor<streamtype>::visit(class_info& info)
		{
			binary_save_field_visitor<streamtype>& visitor = root->fieldvisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				info.visit_data_members(visitor);
			}
		}
		
		//a container of containers
		template<class streamtype>
		inline void binary_save_container_visitor<streamtype>::visit(value_container_info& info)
		{
			//binary_save_data_visitor visitor(file,*root);
			binary_save_data_visitor<streamtype>& visitor = root->datavisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				info.visit(visitor);
			}
		}

		//a container of containers
		template<class streamtype>
		inline void binary_save_container_visitor<streamtype>::visit(key_container_info& info)
		{
			//binary_save_data_visitor visitor(file,*root);
			binary_save_data_visitor<streamtype>& visitor = root->datavisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				info.visit(visitor);
			}
		}

		//a vector of struct instances
		template<class streamtype>
		inline void binary_save_vector_visitor<streamtype>::visit(struct_info& info)
		{
// 			has_complex_data_visitor hascomplexdata;
// 			info.visit(hascomplexdata);
// 
// 			if(hascomplexdata.get_result())
// 			{
// 				int i = 0;
// 				i = i;
// 			}
// 			else
// 			{
// 				int i = 0;
// 				i = i;
// 			}

			binary_save_field_visitor<streamtype>& visitor = root->fieldvisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				info.visit_data_members(visitor);
			}
		}

		//a vector of class instances
		template<class streamtype>
		inline void binary_save_vector_visitor<streamtype>::visit(class_info& info)
		{
			binary_save_field_visitor<streamtype>& visitor = root->fieldvisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				info.visit_data_members(visitor);
			}
		}

		//a vector of containers
		template<class streamtype>
		inline void binary_save_vector_visitor<streamtype>::visit(value_container_info& info)
		{
			binary_save_data_visitor<streamtype>& visitor = root->datavisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				info.visit(visitor);
			}
		}
		
		//a vector of pairs
		template<class streamtype>
		inline void binary_save_vector_visitor<streamtype>::visit(pair_info& info)
		{
			binary_save_data_visitor<streamtype>& visitor = root->datavisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				info.get_first()->visit(visitor);
				info.get_second()->visit(visitor);
			}
		}

		//a vector of containers
		template<class streamtype>
		inline void binary_save_vector_visitor<streamtype>::visit(key_container_info& info)
		{
			binary_save_data_visitor<streamtype>& visitor = root->datavisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				info.visit(visitor);
			}
		}

		template<class streamtype>
		inline void binary_save_data_visitor<streamtype>::visit(value_container_info& info)
		{
			if(!info.is_serializable())
				return;
			
			//pass this to a new container visitor...
			binary_save_container_visitor<streamtype> visitor(file,*root,info);

			//save the number of elements
			size_t size = info.size();
			serialize_bytes(size);

			//save the elements (efficiently)
			info.get_inner()->visit(visitor);
		}

		template<class streamtype>
		inline void binary_save_data_visitor<streamtype>::visit(vector_info& info)
		{
			if(!info.is_serializable())
				return;
			
			//pass this to a new container visitor...
			binary_save_vector_visitor<streamtype> visitor(file,*root,info);
			
			//save the number of elements
			size_t size = info.size();
			serialize_bytes(size);

			//save the elements (efficiently)
			info.get_inner()->visit(visitor);
		}
		
		template<class streamtype>
		inline void binary_save_data_visitor<streamtype>::visit(key_container_info& info)
		{
			if(!info.is_serializable())
				return;
			
			//pass this to a new container visitor...
			binary_save_data_visitor<streamtype>& visitor = root->datavisitor;
			
			//save the number of elements
			size_t size = info.size();
			serialize_bytes(size);
			
			for(bool i = info.begin(); i; i = info.increment())
			{
				//save the elements
				info.get_key()->visit(visitor);
				info.get_inner()->visit(visitor);
			}
		}

		template<class streamtype>
		inline void binary_save_data_visitor<streamtype>::visit(pointer_info& info)
		{
			binary_save_pointer_visitor<streamtype> visitor(file,*root,info);

			info.get_inner()->visit(visitor);
		}

		template<class streamtype>
		inline void binary_save_array_visitor<streamtype>::visit(struct_info& info)
		{
			binary_save_field_visitor<streamtype> visitor = root->fieldvisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				info.visit_data_members(visitor);
			}
		}

		template<class streamtype>
		inline void binary_save_array_visitor<streamtype>::visit(pair_info& info)
		{
			binary_save_data_visitor<streamtype> visitor = root->datavisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				info.get_first()->visit(visitor);
				info.get_second()->visit(visitor);
			}
		}

		template<class streamtype>
		inline void binary_save_array_visitor<streamtype>::visit(class_info& info)
		{
			binary_save_field_visitor<streamtype> visitor = root->fieldvisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				info.visit_data_members(visitor);
			}
		}

		template<class streamtype>
		inline void binary_save_pointer_visitor<streamtype>::visit(struct_info& info)
		{
			if(!info.is_serializable())
				return;
			
			void* ptr = pointer.get_pointer();

			if(ptr == NULL)
			{
				int index = -1;
				serialize_bytes(index);
			}
			else
			{
				//pointer to a struct

				//if(inside pointer map)
				//	serialize the index
				//else
				//	serialize the index
				//	must serialize the class type
				//	and serialize the data
				int index;
				bool bIndexed = root.HasPointer(ptr,index);

				serialize_bytes(index);

				if(!bIndexed)
				{
					binary_save_field_visitor<streamtype>& visitor = root.fieldvisitor;

					info.visit_data_members(visitor);
				}
			}
		}

		template<class streamtype>
		inline void binary_save_pointer_visitor<streamtype>::visit(class_info& info)
		{
			if(!info.is_serializable())
				return;

			void* ptr = pointer.get_pointer();

			if(ptr == NULL)
			{
				int index = -1;
				serialize_bytes(index);
			}
			else
			{
				if(!info.get_class()->get_type().is_serializable())
				{
					int index = -1;
					serialize_bytes(index);
				}
				
				//pointer to a class

				//if(inside pointer map)
				//	serialize the index
				//else
				//	serialize the index
				//	must serialize the class type
				//	and serialize the data
				int index;
				bool bIndexed = root.HasPointer(ptr,index);

				serialize_bytes(index);

				if(!bIndexed)
				{
					binary_save_field_visitor<streamtype>& visitor = root.fieldvisitor;

					const char* classname = info.get_class()->get_type().get_name();

					visitor.serialize_string(classname);

					info.visit_data_members(visitor);
				}
			}
		}

		//Binary Load Visitor - the class you should use.
		template<class type>
		class binary_load_archiver;

		namespace detail
		{
			//Binary Load Data Visitor - applied to un-named data fields, shared by Container and the main Visitor
			template<class type>
			class binary_load_data_visitor : public visitor_base
			{
			public:
				binary_load_data_visitor(type& i, binary_load_archiver<type>& r) 
					: 
					file(i), 
					root(&r)
				{}

				binary_load_data_visitor(type& i) 
					: 
					file(i)
				{}

				template<typename t>
				void deserialize_bytes(t& data)
				{
					file.read((char*)&data,sizeof(data));
				}

				void log(member_info& info, const char* type)
				{
				}

				//bool
				void visit(bool_info& info)
				{
					bool& b = info.get_bool();
					log(info,"bool");
					deserialize_bytes(b);
				}

				//int
				void visit(int_info& info)
				{
					int& i = info.get_int();
					log(info,"int");
					deserialize_bytes(i);
				}

				//float
				void visit(float_info& info)
				{
					float& f = info.get_float();
					log(info,"float");
					deserialize_bytes(f);
				}

				//double
				void visit(double_info& info)
				{
					double& d = info.get_double();
					log(info,"double");
					deserialize_bytes(d);
				}

				void visit(bytes_info& info)
				{
					void* address = info.address();
					size_t size = info.type_size();
					file.read((char*)address,(std::streamsize)size);
				}

				void deserialize_string(string& s)
				{
					size_t num;
					deserialize_bytes(num);

					s.resize(num);

					if(num)
						file.read(&s[0],(std::streamsize)num);
				}

				void deserialize_wstring(wstring& s)
				{
					size_t num;
					deserialize_bytes(num);

					s.resize(num);

					if(num)
						file.read((char*)&s[0],(std::streamsize)num * sizeof(wstring::value_type));
				}
				//string
				void visit(string_info& info)
				{
					string& s = info.get_string();
					log(info,"string");
					deserialize_string(s);
				}
				
				//wstring
				void visit(wstring_info& info)
				{
					wstring& s = info.get_wstring();
					log(info,"wstring");
					deserialize_wstring(s);
				}

				//enum
				void visit(enum_info& info)
				{
					int value;
					log(info,"enum");
					deserialize_bytes(value);
					info.from_int(value);
				}

				//struct
				void visit(struct_info& info)
				{
					log(info,"struct");
					info.visit_data_members(*this);
				}

				//class
				void visit(class_info& info)
				{
					log(info,"class");
					info.visit_data_members(*this);
				}

				//container
				void visit(value_container_info& info);

				//vector
				void visit(vector_info& info);

				//set
				void visit(set_info& info);

				//multiset
				void visit(multiset_info& info);

				//maps
				void visit(key_container_info& info);

				//pointer
				void visit(pointer_info& info);

				//array
				void visit(array_info& info);

				//pair
				void visit(pair_info& info);

			protected:
				friend binary_load_archiver<type>;

				binary_load_archiver<type>* root;
				type& file;
			};


			//Binary Save Field Visitor - applied to serialize named fields
			template<class type>
			class binary_load_field_visitor : public binary_load_data_visitor<type>
			{
			public:
				binary_load_field_visitor(type& i, binary_load_archiver<type>& root) 
					: 
					binary_load_data_visitor(i,root)
				{}

				binary_load_field_visitor(type& i)
					: 
					binary_load_data_visitor(i)
				{}

				typedef binary_load_data_visitor super;

				//bool
				void visit(bool_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//int
				void visit(int_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//string
				void visit(string_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//string
				void visit(wstring_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//float
				void visit(float_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//double
				void visit(double_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//bytes
				void visit(bytes_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//enum
				void visit(enum_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//struct
				void visit(struct_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//class
				void visit(class_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//container
				void visit(value_container_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//vector
				void visit(vector_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//set
				void visit(set_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}
				
				//set
				void visit(multiset_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//maps
				void visit(key_container_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//pointer
				void visit(pointer_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//arrays
				void visit(array_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

				//pairs
				void visit(pair_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}
			};

			//Binary Save Pointer Visitor - applied to serialize pointer types
			template<class type>
			class binary_load_pointer_visitor : public visitor_base
			{
			public:
				binary_load_pointer_visitor(type& i, binary_load_archiver<type>& r, pointer_info& p) : pointer(p), file(i), root(r) {}

				template<typename t>
				void deserialize_bytes(t& data)
				{
					file.read((char*)&data,sizeof(data));
				}

				//pointer to struct
				void visit(struct_info& info);

				//pointer to class
				void visit(class_info& info);

			protected:
				binary_load_archiver<type>& root;
				type&			            file;
				pointer_info&	            pointer;
			};


			//Binary Save Container Visitor - applied to serialize container contents
			template<class type>
			class binary_load_container_visitor : public binary_load_data_visitor<type>
			{
			public:
				binary_load_container_visitor(type& i, binary_load_archiver<type>& root, value_container_info& c, size_t n) 
					: 
					binary_load_data_visitor(i,root),
					container(c),
					num(n)
				{}

				typedef binary_load_data_visitor<type> super;

				//bool
				void visit(bool_info& info)
				{
					for(size_t i = 0; i < num; i++)
					{
						bool b;
						deserialize_bytes(b);

						container.push_back(b);
					}
				}

				//int
				void visit(int_info& info)
				{
					//container.begin();
					for(size_t is = 0; is < num; is++)
					{
						int i;
						deserialize_bytes(i);

						container.push_back(i);
					}
				}

				//string
				void visit(string_info& info)
				{
					for(size_t i = 0; i < num; i++)
					{
						container.push_back_default();
						deserialize_string(info.get_string());
					}
				}

				//wstring
				void visit(wstring_info& info)
				{
					for(size_t i = 0; i < num; i++)
					{
						container.push_back_default();
						deserialize_wstring(info.get_wstring());
					}
				}

				//float
				void visit(float_info& info)
				{
					for(size_t i = 0; i < num; i++)
					{
						float f;
						deserialize_bytes(f);

						container.push_back(f);
					}
				}

				//double
				void visit(double_info& info)
				{
					for(size_t i = 0; i < num; i++)
					{
						double d;
						deserialize_bytes(d);

						container.push_back(d);
					}
				}

				//bytes
				void visit(bytes_info& info)
				{
					size_t size = info.type_size();

					for(size_t i = 0; i < num; i++)
					{
						container.push_back_default();

						void* address = info.address();
						file.read((char*)address,(std::streamsize)size);
					}
				}

				//enum
				void visit(enum_info& info)
				{
					for(size_t i = 0; i < num; i++)
					{
						container.push_back_default();

						int value;
						deserialize_bytes(value);
						info.from_int(value);
					}
				}

				//struct
				void visit(struct_info& info);

				//class
				void visit(class_info& info);

				//a container of containers
				void visit(value_container_info& info);

				void visit(vector_info& info);

				//pointers
				void visit(pointer_info& info)
				{
					binary_load_pointer_visitor<type> visitor(file,*root,info);

					for(size_t i = 0; i < num; i++)
					{
						container.push_back_default();

						info.get_inner()->visit(visitor);
					}
				}

				void visit(key_container_info& info);

				void visit(set_info& info);

				void visit(multiset_info& info);

				void visit(pair_info& info);

			protected:
				value_container_info& container;
				size_t num;
			};

			//Binary Save Container Visitor - applied to serialize container contents
			template<class type>
			class binary_load_array_visitor : public binary_load_data_visitor<type>
			{
			public:
				binary_load_array_visitor(type& i, binary_load_archiver<type>& root, array_info& c, size_t n) 
					: 
					binary_load_data_visitor(i,root),
					container(c),
					num(n)
				{}

				typedef binary_load_data_visitor super;

				void visit(bool_info& info)
				{
					bool* address = (bool*)container.address();
					file.read((char*)address,(std::streamsize)(sizeof(bool)*num) );
				}

				//int
				void visit(int_info& info)
				{
					int* address = (int*)container.address();
					file.read((char*)address,(std::streamsize)(sizeof(int)*num) );
				}

				void visit(string_info& info)
				{
					for(size_t i = 0; i < num; i++)
					{
						container.set_current(i);
						deserialize_string(info.get_string());
					}
				}

				void visit(wstring_info& info)
				{
					for(size_t i = 0; i < num; i++)
					{
						container.set_current(i);
						deserialize_wstring(info.get_wstring());
					}
				}

				//float
				void visit(float_info& info)
				{
					float* address = (float*)container.address();
					file.read((char*)address,(std::streamsize)(sizeof(float)*num) );
				}

				//double
				void visit(double_info& info)
				{
					double* address = (double*)container.address();
					file.read((char*)address,(std::streamsize)(sizeof(double)*num) );
				}

				void visit(bytes_info& info)
				{
					char* address = (char*)container.address();
					file.read((char*)address,(std::streamsize)(info.type_size()*num) );
				}

				void visit(enum_info& info)
				{
					char* address = (char*)container.address();
					file.read((char*)address,(std::streamsize)(info.type_size()*num) );
				}

				void visit(struct_info& info);

				void visit(class_info& info);

				void visit(pointer_info& info)
				{
					binary_load_pointer_visitor<type> visitor(file,*root,info);

					for(size_t i = 0; i < num; i++)
					{
						container.set_current(i);
						info.get_inner()->visit(visitor);
					}
				}
				
				void visit(vector_info& info);

				void visit(value_container_info& info);

				void visit(set_info& info);

				void visit(multiset_info& info);

				void visit(key_container_info& info);
				
				void visit(pair_info& info);

			protected:
				array_info& container;
				size_t num;
			};

			//Binary Save Container Visitor - applied to serialize container contents
			template<class type>
			class binary_load_vector_visitor : public binary_load_data_visitor<type>
			{
			public:
				binary_load_vector_visitor(type& i, binary_load_archiver<type>& root, vector_info& c, size_t n) 
					: 
					binary_load_data_visitor(i,root),
					container(c),
					num(n)
				{}

				typedef binary_load_data_visitor super;

				//bool
				void visit(bool_info& info)
				{
					for(size_t i = 0; i < num; i++)
					{
						bool b;
						deserialize_bytes(b);
						container.push_back(b);
					}
				}

				//int
				void visit(int_info& info)
				{
					for(size_t is = 0; is < num; is++)
					{
						int i;
						deserialize_bytes(i);
						container.push_back(i);
					}
				}

				//string
				void visit(string_info& info)
				{
					for(size_t i = 0; i < num; i++)
					{
						string s;
						deserialize_string(s);
						container.push_back(s);
					}
				}

				//string
				void visit(wstring_info& info)
				{
					for(size_t i = 0; i < num; i++)
					{
						wstring s;
						deserialize_wstring(s);
						container.push_back(s);
					}
				}

				//float
				void visit(float_info& info)
				{
					for(size_t i = 0; i < num; i++)
					{
						float f;
						deserialize_bytes(f);
						container.push_back(f);
					}


				}

				//double
				void visit(double_info& info)
				{
					for(size_t i = 0; i < num; i++)
					{
						double d;
						deserialize_bytes(d);
						container.push_back(d);
					}


				}

				//bytes
				void visit(bytes_info& info)
				{
					size_t size = info.type_size();

// 					for(size_t i = 0; i < num; i++)
// 					{
// 						container.push_back_default();
// 
// 						void* address = info.address();
// 						file.read((char*)address,(std::streamsize)size);
// 					}

					container.resize(num);
					container.begin();
					void* data = info.address();
					file.read((char*)data,(std::streamsize)num*size);

				}

				//enum
				void visit(enum_info& info)
				{
					for(size_t i = 0; i < num; i++)
					{
						container.push_back_default();

						int value;
						deserialize_bytes(value);
						info.from_int(value);
					}
				}

				//struct
				void visit(struct_info& info);

				//class
				void visit(class_info& info);

				//a container of containers
				void visit(value_container_info& info);

				void visit(key_container_info& info);

				//pointers
				void visit(pointer_info& info)
				{
					binary_load_pointer_visitor<type> visitor(file,*root,info);

					for(size_t i = 0; i < num; i++)
					{
						container.push_back_default();

						info.get_inner()->visit(visitor);
					}
				}

				void visit(vector_info& info);

				void visit(set_info& info);

				void visit(multiset_info& info);

				void visit(pair_info& info);

			protected:
				vector_info& container;
				size_t num;
			};
		}
		
		using detail::binary_load_data_visitor;
		using detail::binary_load_field_visitor;
		using detail::binary_load_pointer_visitor;
		using detail::binary_load_vector_visitor;
		using detail::binary_load_array_visitor;
		using detail::binary_load_container_visitor;

		/// \brief Archiver for loading binary data.

		/// This archiver is able to load most any reflectable data type directly,
		/// including built-ins, stl, opclass, opstructs, and openums.
		/// 
		/// Use with binary_save_archiver to load and save data using binary.
		/// 
		/// This archiver reads directly from the stream and should have good performance.
		///
		/// Usage:
		/// \code
		/// binary_load_archiver archiver(stream);
		/// 
		/// archiver >> data;
		/// \endcode
		/// 
		template<class type> 
		class binary_load_archiver
		{
		public:
			/// 
			/// \brief The constructor, requires an istream.
			/// 
			binary_load_archiver(type& stream)
				: 
				indexcount(0),
				datavisitor(stream),
				fieldvisitor(stream)
			{
				// Set stream states.
// 				stream.flags( ifstream::binary );
// 				stream >> std::noskipws;

				datavisitor.root  = this;
				fieldvisitor.root = this;
			}
			
			/// 
			/// \brief Load data from the stream.
			/// 
			/// This can also load pointer data, which will be restored as a new instance.
			/// 
			template< class T >
			friend binary_load_archiver<type>& operator >> ( binary_load_archiver<type>& visitor, T& tt )
			{
				opcpp::accessors::build_accessor(tt).visit(visitor.fieldvisitor);
				return visitor;
			}
			
		private:
			typedef map<int,void*> indexmaptype;
			
			bool HasIndex(int index, indexmaptype::iterator& iterator)
			{
				indexmaptype::iterator it  = indexmap.find(index);
				indexmaptype::iterator end = indexmap.end();
				
				if(it != end)
				{
					iterator = it;
					return true;
				}
				
				index = indexcount;
				iterator = indexmap.insert( indexmaptype::value_type(indexcount,NULL) ).first;
				++indexcount;

				return false;
			}

			binary_load_data_visitor<type>  datavisitor;
			binary_load_field_visitor<type> fieldvisitor;

			friend binary_load_data_visitor<type>;
			friend binary_load_field_visitor<type>;
			friend binary_load_pointer_visitor<type>;
			friend binary_load_container_visitor<type>;
			friend binary_load_vector_visitor<type>;
			friend binary_load_array_visitor<type>;

			//pointer to index map.
			indexmaptype			indexmap;
			int						indexcount;
		};

		template<class type>
		inline void binary_load_data_visitor<type>::visit(pair_info& info)
		{
			info.get_first()->visit(*this);
			info.get_second()->visit(*this);
		}

		template<class type>
		inline void binary_load_data_visitor<type>::visit(value_container_info& info)
		{
			if(!info.is_serializable())
				return;
			
			log(info,"value container");	

			//load the number of elements
			size_t num;
			deserialize_bytes(num);
			
			//pass this to a new container visitor...
			binary_load_container_visitor<type> visitor(file,*root,info,num);

			info.clear();

			//load the elements (efficiently)
			info.get_inner()->visit(visitor);
		}

		template<class type>
		inline void binary_load_data_visitor<type>::visit(vector_info& info)
		{
			if(!info.is_serializable())
				return;

			//load the number of elements
			size_t num;
			deserialize_bytes(num);

			log(info,"vector");


			info.resize(0);
			info.reserve(num);
			
			if(num)
			{
				//pass this to a new container visitor...
				binary_load_vector_visitor<type> visitor(file,*root,info,num);

				//load the elements (efficiently)
				info.get_inner()->visit(visitor);
			}
		}

		template<class type>
		inline void binary_load_data_visitor<type>::visit(array_info& info)
		{
			size_t num = info.size();
			
			//pass this to a new container visitor...
			binary_load_array_visitor<type> visitor(file,*root,info,num);

			log(info,"array");

			//load the elements (efficiently)
			info.get_inner()->visit(visitor);
		}

		template<class type>
		inline void binary_load_data_visitor<type>::visit(set_info& info)
		{
			if(!info.is_serializable())
				return;

			//load the number of elements
			size_t num;
			deserialize_bytes(num);

			log(info,"set");

			info.clear();

			for(size_t i = 0; i < num; i++)
			{
				info.insert_visit(*this);
			}
		}

		template<class type>
		inline void binary_load_data_visitor<type>::visit(multiset_info& info)
		{
			if(!info.is_serializable())
				return;

			//load the number of elements
			size_t num;
			deserialize_bytes(num);

			log(info,"multiset");

			info.clear();

			for(size_t i = 0; i < num; i++)
			{
				info.insert_visit(*this);
			}
		}

		template<class type>
		inline void binary_load_data_visitor<type>::visit(key_container_info& info)
		{
			if(!info.is_serializable())
				return;

			//load the number of elements
			size_t num;
			deserialize_bytes(num);

			log(info,"key container");


			info.clear();

			for(size_t i = 0; i < num; i++)
			{
				info.insert_visit(*this,*this);
			}
		}

		template<class type>
		inline void binary_load_data_visitor<type>::visit(pointer_info& info)
		{
			binary_load_pointer_visitor<type> visitor(file,*root,info);
			
			log(info,"pointer");

			info.get_inner()->visit(visitor);
		}

		template<class type>
		inline void binary_load_pointer_visitor<type>::visit(struct_info& info)
		{
			//pointer to a struct
			if(!info.is_serializable())
				return;

			//load index
			int index;
			deserialize_bytes(index);

			if(index == -1)
			{
				pointer.set_pointer_unsafe(NULL);
				return;
			}

			binary_load_archiver<type>::indexmaptype::iterator it;

			bool bIndexed = root.HasIndex(index,it);

			if(bIndexed)
			{
				struct_base* ptr = (struct_base*)(*it).second;

				//already indexed, set the pointer
				pointer.set_pointer_struct(ptr);
			}
			else
			{
				struct_base* newstruct = info.get_type().new_instance();

				(*it).second = newstruct;

				pointer.set_pointer_struct(newstruct);

				info.visit_data_members(root.fieldvisitor);
			}
		}

		template<class type>
		inline void binary_load_pointer_visitor<type>::visit(class_info& info)
		{
			//pointer to a class
			if(!info.is_serializable())
				return;
			
			//load index
			int index;
			deserialize_bytes(index);

			if(index == -1)
			{
				pointer.set_pointer_unsafe(NULL);
				return;
			}

			binary_load_archiver<type>::indexmaptype::iterator it;

			bool bIndexed = root.HasIndex(index,it);

			if(bIndexed)
			{
				class_base* ptr = (class_base*)(*it).second;

				//already indexed, set the pointer
				pointer.set_pointer_class(ptr);
			}
			else
			{
				binary_load_field_visitor<type>& visitor = root.fieldvisitor;

				//load the classname
				string classname;
				visitor.deserialize_string(classname);

				//get the type from the name...
				class_type* type = opcpp::registration::class_tracker::get_type(classname);

				//shouldn't happen (error)
				if(!type)
					return;

				class_base* newclass = type->new_instance();

				(*it).second = newclass;

				pointer.set_pointer_class(newclass);

				info.visit_data_members(visitor);
			}
		}

		template<class type>
		inline void binary_load_container_visitor<type>::visit(struct_info& info)
		{
			binary_load_field_visitor<type>& visitor = root->fieldvisitor;

			for(size_t i = 0; i < num; i++)
			{
				container.push_back_default();

				//now we need to visit the inserted one...
				info.visit_data_members(visitor);
			}
		}

		template<class type>
		inline void binary_load_container_visitor<type>::visit(class_info& info)
		{
			binary_load_field_visitor<type>& visitor = root->fieldvisitor;

			for(size_t i = 0; i < num; i++)
			{
				container.push_back_default();

				//now we need to visit the inserted one...
				info.visit_data_members(visitor);
			}
		}

		template<class type>
		inline void binary_load_container_visitor<type>::visit(pair_info& info)
		{
			binary_load_data_visitor<type>& visitor = root->fieldvisitor;

			for(size_t i = 0; i < num; i++)
			{
				container.push_back_default();

				//now we need to visit the inserted one...
				info.get_first()->visit(visitor);
				info.get_second()->visit(visitor);
			}
		}

		template<class type>
		inline void binary_load_container_visitor<type>::visit(value_container_info& info)
		{
			binary_load_data_visitor<type>& visitor = root->datavisitor;

			for(size_t i = 0; i < num; i++)
			{
				container.push_back_default();
				info.visit(visitor);
			}
		}

		template<class type>
		inline void binary_load_container_visitor<type>::visit(vector_info& info)
		{
			visit( (value_container_info&)info );
		}

		template<class type>
		inline void binary_load_container_visitor<type>::visit(set_info& info)
		{
			visit( (value_container_info&)info );
		}
		
		template<class type>
		inline void binary_load_container_visitor<type>::visit(multiset_info& info)
		{
			visit( (value_container_info&)info );
		}

		template<class type>
		inline void binary_load_container_visitor<type>::visit(key_container_info& info)
		{
			binary_load_data_visitor& visitor = root->datavisitor;

			for(size_t i = 0; i < num; i++)
			{
				container.push_back_default();
				info.visit(visitor);
			}
		}

		template<class type>
		inline void binary_load_vector_visitor<type>::visit(set_info& info)
		{
			visit( (value_container_info&)info );
		}

		template<class type>
		inline void binary_load_vector_visitor<type>::visit(multiset_info& info)
		{
			visit( (value_container_info&)info );
		}
		
		template<class type>
		inline void binary_load_vector_visitor<type>::visit(vector_info& info)
		{
			visit( (value_container_info&)info );
		}

		template<class type>
		inline void binary_load_vector_visitor<type>::visit(struct_info& info)
		{
			if(num > 0)
			{
				//NOTE: the fast vector<struct> loader checks to see if a struct qualifies,
				//		if so it will load all but the first element in one operation!
				//		we need to load the first element because we're not doing external visiting.
				binary_load_field_visitor<type>& visitor = root->fieldvisitor;

				container.push_back_default();
				info.visit_data_members(visitor);

				has_complex_data_visitor hascomplexdata;
				info.visit(hascomplexdata);

				if(!hascomplexdata.get_result())
				{
					size_t structsize = info.type_size();
					if(hascomplexdata.get_size() == structsize)
					{
						// do fast version.
						container.resize(num);
						container.begin();
						container.increment();
						void* data = info.address();
						file.read((char*)data,(std::streamsize)(num-1)*structsize);

						return;
					}
				}

				for(size_t i = 1; i < num; i++)
				{
					container.push_back_default();

					//now we need to visit the inserted one...
					info.visit_data_members(visitor);
				}
			}




		}

		template<class type>
		inline void binary_load_vector_visitor<type>::visit(class_info& info)
		{
			binary_load_field_visitor<type>& visitor = root->fieldvisitor;

			for(size_t i = 0; i < num; i++)
			{
				container.push_back_default();

				//now we need to visit the inserted one...
				info.visit_data_members(visitor);
			}
		}

		template<class type>
		inline void binary_load_vector_visitor<type>::visit(key_container_info& info)
		{
			binary_load_data_visitor<type>& visitor = root->datavisitor;

			for(size_t i = 0; i < num; i++)
			{
				container.push_back_default();
				info.visit(visitor);
			}
		}

		template<class type>
		inline void binary_load_vector_visitor<type>::visit(value_container_info& info)
		{
			binary_load_data_visitor<type>& visitor = root->datavisitor;

			for(size_t i = 0; i < num; i++)
			{
				container.push_back_default();
				info.visit(visitor);
			}
		}

		template<class type>
		inline void binary_load_vector_visitor<type>::visit(pair_info& info)
		{
			binary_load_data_visitor<type>& visitor = root->datavisitor;

			for(size_t i = 0; i < num; i++)
			{
				container.push_back_default();
				
				info.get_first()->visit(visitor);
				info.get_second()->visit(visitor);
			}
		}

		template<class type>
		inline void binary_load_array_visitor<type>::visit(struct_info& info)
		{
			binary_load_data_visitor<type>& visitor = root->datavisitor;

			for(size_t i = 0; i < num; i++)
			{
				container.set_current(i);
				info.visit(visitor);
			}
		}

		template<class type>
		inline void binary_load_array_visitor<type>::visit(class_info& info)
		{
			binary_load_data_visitor<type>& visitor = root->datavisitor;

			for(size_t i = 0; i < num; i++)
			{
				container.set_current(i);
				info.visit(visitor);
			}
		}

		template<class type>
		inline void binary_load_array_visitor<type>::visit(key_container_info& info)
		{
			binary_load_data_visitor<type>& visitor = root->datavisitor;

			for(size_t i = 0; i < num; i++)
			{
				container.set_current(i);
				info.visit(visitor);
			}
		}

		template<class type>
		inline void binary_load_array_visitor<type>::visit(value_container_info& info)
		{
			binary_load_data_visitor<type>& visitor = root->datavisitor;

			for(size_t i = 0; i < num; i++)
			{
				container.set_current(i);
				info.visit(visitor);
			}
		}

		template<class type>
		inline void binary_load_array_visitor<type>::visit(vector_info& info)
		{
			binary_load_data_visitor<type>& visitor = root->datavisitor;

			for(size_t i = 0; i < num; i++)
			{
				container.set_current(i);
				info.visit(visitor);
			}
		}

		template<class type>
		inline void binary_load_array_visitor<type>::visit(set_info& info)
		{
			binary_load_data_visitor<type>& visitor = root->datavisitor;

			for(size_t i = 0; i < num; i++)
			{
				container.set_current(i);
				info.visit(visitor);
			}
		}

		template<class type>
		inline void binary_load_array_visitor<type>::visit(multiset_info& info)
		{
			binary_load_data_visitor<type>& visitor = root->datavisitor;

			for(size_t i = 0; i < num; i++)
			{
				container.set_current(i);
				info.visit(visitor);
			}
		}

		template<class type>
		inline void binary_load_array_visitor<type>::visit(pair_info& info)
		{
			binary_load_data_visitor<type>& visitor = root->datavisitor;

			for(size_t i = 0; i < num; i++)
			{
				container.set_current(i);
				info.get_first()->visit(visitor);
				info.get_second()->visit(visitor);
			}
		}

	}
}

code heading
{
	#endif //OPCPP_BINARY_SERIALIZE_DOH
};







