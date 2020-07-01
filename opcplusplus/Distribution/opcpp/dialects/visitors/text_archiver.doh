//****************************************************************
// Copyright (C) 2007 OP Games LLC - All Rights Reserved
//
// Authors: Kevin Depue & Lucas Ellis
//
// File: textserialize.doh
// Date: 10/30/2007
//
// Description:
//
// Text Load/Save Visitors for opC++
//****************************************************************

//Requires: opC++ Standard Dialect
//			opinclude "opc++dialect.doh" first!
//
//Usage:
//		Dialect:
//			opinclude "visitors/textserialize.doh"
//		Saving:
//			opcpp::visitors::text_save_archiver visitor(output stream);
//			visitor << object;
//			visitor << data;
//		Loading:
//			opcpp::visitors::text_load_archiver visitor(input stream);
//			visitor >> object;
//			visitor >> data;

code heading
{
	#ifndef OPCPP_TEXT_SERIALIZE_DOH
	#define OPCPP_TEXT_SERIALIZE_DOH

	#include <iostream>
	#include <map>
	#include <string>
	#include <limits>
	#include <iomanip>
}

namespace opcpp
{
	namespace visitors
	{
		using std::string;
		using std::ostream;
		using std::istream;
		using namespace opcpp::accessors;
		using opcpp::base::visitor_base;
		using std::map;
		using std::numeric_limits;
		using std::setprecision;

		// Consists of these visitor states:
		//	Archiver  - base visitor dispatcher
		//	Field     - named fields - handles transient, derives from Data
		//	Container - container visitor, derives from Data
		//	Pointer   - pointer visitor - handles pointers
		//	Data      - unnamed fields

		//Text Save Visitor - the class you should use.
		class text_save_archiver;

		namespace detail
		{
			//uses blank as a separator character
			const char textend = ' ';
			
			//text save data visitor - applied to un-named data fields, shared by Container and the main Visitor
			class text_save_data_visitor : public visitor_base
			{
			public:
				text_save_data_visitor(ostream& o, text_save_archiver& r) 
					: 
					file(o), 
					root(&r) 
				{}

				text_save_data_visitor(ostream& o) 
					: 
				file(o)
				{}

				void visit(bool_info& info)
				{
					bool value = info.get_bool()?1:0;
					file << value << textend;
				}

				//int
				void visit(int_info& info)
				{
					file << info.get_int() << textend;
				}

				//serialize a string
				void serialize_string(const string& s)
				{
					file << s.size() << textend;

					file << s << textend;
				}

				//string
				void visit(string_info& info)
				{
					string& s = info.get_string();
					serialize_string(s);
				}

				//float
				void visit(float_info& info)
				{
					file << info.get_float() << textend;
				}

				//double
				void visit(double_info& info)
				{
					file << info.get_double() << textend;

				}

				//bytes
				void visit(bytes_info& info)
				{
					size_t num = info.type_size();

					unsigned char* current = (unsigned char*)info.address();

					for(size_t i = 0; i < num; i++)
					{
						unsigned short val = *current;
						file << val;
						file << textend;
						++current;
					}
				}

				//enum
				void visit(enum_info& info)
				{
					int value;
					info.to_int(value);

					file << value << textend;
				}

				//struct
				void visit(struct_info& info);

				//class
				void visit(class_info& info);

				//container
				void visit(value_container_info& info);

				//map
				void visit(key_container_info& info);

				//pointer
				void visit(pointer_info& info);

				//array
				void visit(array_info& info);

				//pair
				void visit(pair_info& info);

			protected:

				friend class opcpp::visitors::text_save_archiver;

				text_save_archiver* root;
				ostream& file;
			};

			//NOTE: could easily be a template class.
			//Text Save Field Visitor - applied to serialize named fields
			class text_save_field_visitor : public text_save_data_visitor
			{
			public:
				text_save_field_visitor(ostream& o, text_save_archiver& root)
					: 
					text_save_data_visitor(o,root)
				{}

				text_save_field_visitor(ostream& o)
					: 
					text_save_data_visitor(o)
				{}

				typedef text_save_data_visitor super;

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

				//map
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

			//Text Save Pointer Visitor - applied to serialize pointer types
			class text_save_pointer_visitor : public visitor_base
			{
			public:
				text_save_pointer_visitor(ostream& f, text_save_archiver& r, pointer_info& p)
					: 
				pointer(p), 
					file(f), 
					root(r)
				{}

				//pointer to struct
				void visit(struct_info& info);

				//pointer to class
				void visit(class_info& info);

			protected:
				text_save_archiver& root;
				ostream&		 file;
				pointer_info&	 pointer;
			};

			//Text Save Container Visitor - applied to serialize container contents
			class text_save_container_visitor : public text_save_data_visitor
			{
			public:
				text_save_container_visitor(ostream& f, text_save_archiver& root, value_container_info& c) 
					: 
					text_save_data_visitor(f,root),
					container(c)
				{}

				typedef text_save_data_visitor super;

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

				//pointers
				void visit(pointer_info& info);

				void visit(key_container_info& info);

				void visit(pair_info& info);

			protected:
				value_container_info& container;
			};

		}

		using detail::text_save_container_visitor;
		using detail::text_save_data_visitor;
		using detail::text_save_field_visitor;
		using detail::text_save_pointer_visitor;
		
		/// \brief Archiver for saving data to text.

		/// This archiver is able to save most any reflectable data type directly,
		/// including built-ins, stl, opclass, opstructs, and openums.
		/// 
		/// Use with text_load_archiver to load and save data using text.
		/// 
		/// This archiver writes directly to the stream and should have very good performance.
		///
		/// Usage:
		/// \code
		/// text_save_archiver archiver(stream);
		/// 
		/// archiver << data;
		/// \endcode
		/// 
		class text_save_archiver
		{
		public:
			/// 
			/// \brief The constructor, requires an istream.
			/// 
			text_save_archiver(ostream& o)
				: 
				pointercount(0), 
				datavisitor(o),
				fieldvisitor(o),
				stream(o)
			{
				datavisitor.root = this;
				fieldvisitor.root = this;
			}

			/// 
			/// \brief Load data from the stream.
			/// 
			/// This can also load pointer data, which will be restored as a new instance.
			/// 
			template< class T >
			friend text_save_archiver& operator << ( text_save_archiver& visitor, T& type )
			{
				//alter precision
				std::streamsize size = visitor.stream.precision();
				visitor.stream.precision( numeric_limits<double>::digits10 + 2 );

				opcpp::accessors::build_accessor(type).visit(visitor.datavisitor);

				//reset precision
				visitor.stream.precision(size);

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

			text_save_data_visitor  datavisitor;
			text_save_field_visitor fieldvisitor;

			friend class text_save_data_visitor;
			friend class text_save_field_visitor;
			friend class text_save_container_visitor;
			friend class text_save_pointer_visitor;

			//pointer to index map.
			typedef map<void*,int>	pointermaptype;
			pointermaptype			pointermap;
			int						pointercount;

			ostream& stream;
		};

		inline void text_save_data_visitor::visit(array_info& info)
		{
			member_info* inner = info.get_inner();

			//NOTE: could be faster with an additional visitor.
			size_t num = info.size();
			for(size_t i = 0; i < num; i++)
			{
				info.set_current(i);
				inner->visit(root->datavisitor);
			}
		}
		
		inline void text_save_data_visitor::visit(struct_info& info)
		{
			if(!info.is_serializable())
				return;

			info.visit_data_members(root->fieldvisitor);
		}
		
		inline void text_save_data_visitor::visit(class_info& info)
		{
			if(!info.is_serializable())
				return;

			info.visit_data_members(root->fieldvisitor);
		}

		inline void text_save_data_visitor::visit(pair_info& info)
		{
			text_save_data_visitor& visitor = root->datavisitor;

			info.get_first()->visit(visitor);
			info.get_second()->visit(visitor);
		}

		inline void text_save_container_visitor::visit(struct_info& info)
		{
			text_save_field_visitor& visitor = root->fieldvisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				info.visit_data_members(visitor);
			}
		}

		inline void text_save_container_visitor::visit(class_info& info)
		{
			text_save_field_visitor& visitor = root->fieldvisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				info.visit_data_members(visitor);
			}
		}

		inline void text_save_container_visitor::visit(pair_info& info)
		{
			text_save_data_visitor& visitor = root->datavisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				info.get_first()->visit(visitor);
				info.get_second()->visit(visitor);
			}
		}

		//a container of containers
		inline void text_save_container_visitor::visit(value_container_info& info)
		{
			text_save_data_visitor& visitor = root->datavisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				info.visit(visitor);
			}
		}

		//a container of containers
		inline void text_save_container_visitor::visit(key_container_info& info)
		{
			text_save_data_visitor& visitor = root->datavisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				info.visit(visitor);
			}
		}

		//pointers
		inline void text_save_container_visitor::visit(pointer_info& info)
		{
			text_save_pointer_visitor visitor(file,*root,info);

			for(bool i = container.begin(); i; i = container.increment())
			{
				info.get_inner()->visit(visitor);
			}
		}

		inline void text_save_data_visitor::visit(value_container_info& info)
		{
			if(!info.is_serializable())
				return;
			
			//pass this to a new container visitor...
			text_save_container_visitor visitor(file,*root,info);

			//save the number of elements
			file << info.size() << textend;

			//save the elements (efficiently)
			info.get_inner()->visit(visitor);
		}


		inline void text_save_data_visitor::visit(key_container_info& info)
		{
			if(!info.is_serializable())
				return;

			//save the number of elements
			file << info.size() << textend;
			
			text_save_data_visitor visitor(file,*root);

			for(bool i = info.begin(); i; i = info.increment())
			{
				info.get_key()->visit(visitor);
				info.get_inner()->visit(visitor);
			}
		}

		inline void text_save_data_visitor::visit(pointer_info& info)
		{
			text_save_pointer_visitor visitor(file,*root,info);

			info.get_inner()->visit(visitor);
		}

		inline void text_save_pointer_visitor::visit(struct_info& info)
		{
			//make sure this type is serializable
			if(!info.get_type().is_serializable())
				return;

			void* ptr = pointer.get_pointer();

			if(ptr == NULL)
			{
				file << -1 << textend;
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

				file << index << textend;	

				if(!bIndexed)
				{
					text_save_field_visitor visitor(file,root);

					info.visit_data_members(visitor);
				}
			}
		}

		inline void text_save_pointer_visitor::visit(class_info& info)
		{
			if(!info.get_type().is_serializable())
				return;

			void* ptr = pointer.get_pointer();

			if(ptr == NULL)
			{
				file << -1 << textend;
			}
			else
			{
				//make sure the class instance is serializable
				if(!info.get_class()->get_type().is_serializable())
				{
					file << -1 << textend;
					return;
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

				file << index << textend;	

				if(!bIndexed)
				{
					text_save_field_visitor visitor(file,root);

					const char* classname = info.get_class()->get_type().get_name();

					visitor.serialize_string(classname);

					info.visit_data_members(visitor);
				}
			}
		}

		//Text Save Visitor - the class you should use.
		class text_load_archiver;

		namespace detail
		{

			//Text Save Data Visitor - applied to un-named data fields, shared by Container and the main Visitor
			class text_load_data_visitor : public visitor_base
			{
			public:
				text_load_data_visitor(istream& i, text_load_archiver& r) : file(i), root(&r) {}
				text_load_data_visitor(istream& i) : file(i) {}

				void deserialize_string(string& s)
				{
					size_t num;
					file >> num;

					//skip textendine
					file.get();

					s.resize(num);

					file.read(&s[0],(std::streamsize)num);
				}

				//bool
				void visit(bool_info& info)
				{
					bool& b = info.get_bool();
					file >> b;
				}

				//int
				void visit(int_info& info)
				{
					int& i = info.get_int();
					file >> i;
				}

				//float
				void visit(float_info& info)
				{
					float& f = info.get_float();
					file >> f;
				}
				
				//double
				void visit(double_info& info)
				{
					double& d = info.get_double();
					file >> d;
				}

				//bytes
				void visit(bytes_info& info)
				{
					unsigned char* address = (unsigned char*)info.address();
					size_t size = info.type_size();
					
					for(size_t i = 0; i < size; i++)
					{
						unsigned short val;
						file >> val;
						*address = (unsigned char)val;
						++address;
					}
				}

				//string
				void visit(string_info& info)
				{
					string& s = info.get_string();

					deserialize_string(s);
				}

				//enum
				void visit(enum_info& info)
				{
					int value;
					file >> value;
					info.from_int(value);
				}

				//array
				void visit(array_info& info)
				{
					member_info* inner = info.get_inner();

					size_t num = info.size();
					for(size_t i = 0; i < num; i++)
					{
						info.set_current(i);
						inner->visit(*this);
					}
				}

				//struct
				void visit(struct_info& info);

				//class
				void visit(class_info& info);

				//container
				void visit(value_container_info& info);

				//vector
				void visit(vector_info& info);

				//pointer
				void visit(pointer_info& info);

				//maps
				void visit(key_container_info& info);

				//sets
				void visit(set_info& info);

				//multisets
				void visit(multiset_info& info);

				//pairs
				void visit(pair_info& info);

			protected:
				friend class opcpp::visitors::text_load_archiver;

				text_load_archiver* root;
				istream& file;
			};


			//Text Save Field Visitor - applied to serialize named fields
			class text_load_field_visitor : public text_load_data_visitor
			{
			public:
				text_load_field_visitor(istream& i, text_load_archiver& root) : text_load_data_visitor(i,root) {}
				text_load_field_visitor(istream& i) : text_load_data_visitor(i) {}

				typedef text_load_data_visitor super;

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

				void visit(key_container_info& info)
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

				//multiset
				void visit(multiset_info& info)
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

				void visit(pair_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}

			};

			//Text Save Pointer Visitor - applied to serialize pointer types
			class text_load_pointer_visitor : public visitor_base
			{
			public:
				text_load_pointer_visitor(istream& i, text_load_archiver& r, pointer_info& p) : pointer(p), file(i), root(r) {}

				//pointer to struct
				void visit(struct_info& info);

				//pointer to class
				void visit(class_info& info);

			protected:
				text_load_archiver& root;
				istream&		 file;
				pointer_info&	 pointer;
			};

			//Text Save Container Visitor - applied to serialize container contents
			class text_load_container_visitor : public text_load_data_visitor
			{
			public:
				text_load_container_visitor(istream& i, text_load_archiver& root, value_container_info& c, size_t n) 
					: 
					text_load_data_visitor(i,root),
					container(c),
					num(n)
				{}

				typedef text_load_data_visitor super;

				//bool
				void visit(bool_info& info)
				{
					for(size_t i = 0; i < num; i++)
					{
						bool b;
						file >> b;
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
						file >> i;
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

				//float
				void visit(float_info& info)
				{
					for(size_t i = 0; i < num; i++)
					{
						float f;
						file >> f;
						container.push_back(f);
					}
				}
				
				//double
				void visit(double_info& info)
				{
					for(size_t i = 0; i < num; i++)
					{
						double d;
						file >> d;
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
						
						unsigned char* address = (unsigned char*)info.address();

						for(size_t i = 0; i < size; i++)
						{
							unsigned short val;
							file >> val;
							*address = (unsigned char)val;
							++address;
						}
					}
				}

				//enum
				void visit(enum_info& info)
				{
					for(size_t i = 0; i < num; i++)
					{
						container.push_back_default();

						int value;
						file >> value;
						info.from_int(value);
					}
				}

				//struct
				void visit(struct_info& info);

				//class
				void visit(class_info& info);

				//a container of containers
				void visit(value_container_info& info);

				//a container of maps
				void visit(key_container_info& info);

				void visit(vector_info& info)
				{
					visit((value_container_info&)info);
				}

				//pointers
				void visit(pointer_info& info)
				{
					text_load_pointer_visitor visitor(file,*root,info);

					for(size_t i = 0; i < num; i++)
					{
						container.push_back_default();

						info.get_inner()->visit(visitor);
					}
				}

				void visit(set_info& info);

				void visit(multiset_info& info);

				void visit(pair_info& info);

			protected:
				value_container_info& container;
				size_t num;
			};

		}

		using detail::text_load_container_visitor;
		using detail::text_load_data_visitor;
		using detail::text_load_field_visitor;
		using detail::text_load_pointer_visitor;

		/// \brief Archiver for loading text data.

		/// This archiver is able to load most any reflectable data type directly,
		/// including built-ins, stl, opclass, opstructs, and openums.
		/// 
		/// Use with text_save_archiver to load and save data using binary.
		/// 
		/// This archiver reads directly from the stream and should have good performance.
		///
		/// Usage:
		/// \code
		/// text_load_archiver archiver(stream);
		/// 
		/// archiver >> data;
		/// \endcode
		/// 
		class text_load_archiver
		{
		public:
			/// 
			/// \brief The constructor, requires an istream.
			/// 
			text_load_archiver(istream& i)
				: 
				indexcount(0),
				datavisitor(i),
				fieldvisitor(i)
			{
				datavisitor.root = this;
				fieldvisitor.root = this;
			}

			/// 
			/// \brief Load data from the stream.
			/// 
			/// This can also load pointer data, which will be restored as a new instance.
			/// 
			template<class T>
			friend text_load_archiver& operator >> (text_load_archiver& visitor, T& value)
			{
				opcpp::accessors::build_accessor(value).visit(visitor.datavisitor);
				return visitor;
			}

		private:
			typedef map<int,void*>	indexmaptype;

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

			text_load_data_visitor  datavisitor;
			text_load_field_visitor fieldvisitor;

			friend class text_load_data_visitor;
			friend class text_load_field_visitor;
			friend class text_load_container_visitor;
			friend class text_load_pointer_visitor;

			//pointer to index map.
			indexmaptype			indexmap;
			int						indexcount;
		};

		inline void text_load_data_visitor::visit(value_container_info& info)
		{
			if(!info.is_serializable())
				return;

			//load the number of elements
			size_t num;
			file >> num;

			info.clear();

			//pass this to a new container visitor...
			text_load_container_visitor visitor(file,*root,info,num);

			//save the elements (efficiently)
			info.get_inner()->visit(visitor);
		}

		//struct
		inline void text_load_data_visitor::visit(struct_info& info)
		{
			info.visit_data_members(root->fieldvisitor);
		}

		//class
		inline void text_load_data_visitor::visit(class_info& info)
		{
			info.visit_data_members(root->fieldvisitor);
		}

		inline void text_load_data_visitor::visit(pair_info& info)
		{
			text_load_data_visitor& visitor = root->datavisitor;

			info.get_first()->visit(visitor);
			info.get_second()->visit(visitor);
		}

		inline void text_load_data_visitor::visit(vector_info& info)
		{
			if(!info.is_serializable())
				return;

			//load the number of elements
			size_t num;
			file >> num;

			info.resize(0);
			info.reserve(num);

			//pass this to a new container visitor...
			text_load_container_visitor visitor(file,*root,info,num);

			//save the elements (efficiently)
			info.get_inner()->visit(visitor);
		}

		inline void text_load_data_visitor::visit(key_container_info& info)
		{
			if(!info.is_serializable())
				return;

			//load the number of elements
			size_t num;
			file >> num;

			info.clear();

			text_load_data_visitor& visitor = root->datavisitor; 

			for(size_t i = 0; i < num; i++)
			{
				info.insert_visit(visitor,visitor);
			}
		}

		inline void text_load_data_visitor::visit(set_info& info)
		{
			if(!info.is_serializable())
				return;

			//load the number of elements
			size_t num;
			file >> num;

			info.clear();

			text_load_data_visitor& visitor = root->datavisitor; 

			for(size_t i = 0; i < num; i++)
			{
				info.insert_visit(visitor);
			}
		}

		inline void text_load_data_visitor::visit(multiset_info& info)
		{
			if(!info.is_serializable())
				return;

			//load the number of elements
			size_t num;
			file >> num;
			
			info.clear();

			text_load_data_visitor& visitor = root->datavisitor; 

			for(size_t i = 0; i < num; i++)
			{
				info.insert_visit(visitor);
			}
		}

		inline void text_load_data_visitor::visit(pointer_info& info)
		{
			text_load_pointer_visitor visitor(file,*root,info);

			info.get_inner()->visit(visitor);
		}

		inline void text_load_pointer_visitor::visit(struct_info& info)
		{
			//pointer to a struct
			if(!info.get_type().is_serializable())
				return;

			//load index
			int index;
			file >> index;

			if(index == -1)
			{
				pointer.set_pointer(NULL);
				return;
			}

			text_load_archiver::indexmaptype::iterator it;

			bool bIndexed = root.HasIndex(index,it);

			if(bIndexed)
			{
				void* ptr = (*it).second;

				//already indexed, set the pointer
				pointer.set_pointer(ptr);
			}
			else
			{
				//load the class (create a new instance!)		
				text_load_field_visitor& visitor = root.fieldvisitor;

				//get a new instance
				struct_base* newstruct = info.get_type().new_instance();

				(*it).second = newstruct;

				pointer.set_pointer(newstruct);

				info.visit_data_members(visitor);
			}
		}

		inline void text_load_pointer_visitor::visit(class_info& info)
		{
			if(!info.get_type().is_serializable())
				return;

			//load index
			int index;
			file >> index;

			if(index == -1)
			{
				pointer.set_pointer(NULL);
				return;
			}

			text_load_archiver::indexmaptype::iterator it;

			bool bIndexed = root.HasIndex(index,it);

			if(bIndexed)
			{
				void* ptr = (*it).second;

				//already indexed, set the pointer
				pointer.set_pointer(ptr);
			}
			else
			{
				//load the class (create a new instance!)		
				text_load_field_visitor visitor = root.fieldvisitor;

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

				pointer.set_pointer(newclass);

				info.visit_data_members(visitor);
			}
		}

		inline void text_load_container_visitor::visit(struct_info& info)
		{
			if(!info.is_serializable())
				return;

			text_load_field_visitor& visitor = root->fieldvisitor;

			for(size_t i = 0; i < num; i++)
			{
				container.push_back_default();

				//now we need to visit the inserted one...
				info.visit_data_members(visitor);
			}
		}

		inline void text_load_container_visitor::visit(pair_info& info)
		{
			text_load_data_visitor& visitor = root->datavisitor;

			for(size_t i = 0; i < num; i++)
			{
				container.push_back_default();

				//now we need to visit the inserted one...
				info.get_first()->visit(visitor);
				info.get_second()->visit(visitor);
			}
		}

		inline void text_load_container_visitor::visit(class_info& info)
		{
			if(!info.is_serializable())
				return;

			text_load_field_visitor& visitor = root->fieldvisitor;

			for(size_t i = 0; i < num; i++)
			{
				container.push_back_default();

				//now we need to visit the inserted one...
				info.visit_data_members(visitor);
			}
		}

		inline void text_load_container_visitor::visit(value_container_info& info)
		{
			if(!info.is_serializable())
				return;

			text_load_data_visitor& visitor = root->datavisitor;

			for(size_t i = 0; i < num; i++)
			{
				container.push_back_default();

				info.visit(visitor);
			}
		}

		inline void text_load_container_visitor::visit(key_container_info& info)
		{
			if(!info.is_serializable())
				return;

			text_load_data_visitor& visitor = root->datavisitor;

			for(size_t i = 0; i < num; i++)
			{
				container.push_back_default();

				info.visit(visitor);
			}
		}

		inline void text_load_container_visitor::visit(set_info& info)
		{
			visit((value_container_info&)info);
		}
		
		inline void text_load_container_visitor::visit(multiset_info& info)
		{
			visit((value_container_info&)info);
		}
	}
}

code heading
{
	#endif //OPCPP_TEXT_SERIALIZE_DOH
}


