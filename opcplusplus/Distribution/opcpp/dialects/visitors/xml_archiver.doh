//****************************************************************
// Copyright © 2008 opGames LLC - All Rights Reserved
//
// Authors: Kevin Depue & Lucas Ellis
//
// File: textserialize.doh
// Date: 10/30/2007
//
// Description:
//
// Xml Load/Save Visitors for opC++
//****************************************************************

//Requires: opC++ Standard Dialect
//			opinclude "opc++dialect.doh" first!
//			
//			Before the source index is included:
//			#define OPCPP_TINYXML_SOURCE
//
//Usage:
//		Dialect:
//			opinclude "visitors/xml_archiver.doh"
//		Saving:
//			opcpp::visitors::xml_save_archiver visitor(output text stream, root tag name );
//			visitor.save(data,tag);
//			visitor.save(data);
//		Loading:
//			opcpp::visitors::xml_load_archiver visitor(input text stream, root tag name );
//			visitor.load(data,tag);
//			visitor.load(data);

code heading
{
	#ifndef OPCPP_XML_SERIALIZE_DOH
	#define OPCPP_XML_SERIALIZE_DOH

	#include <iostream>
	#include <map>
	#include <string>
	#include <sstream>
	#include <limits>
}

//
// These code blocks include the TinyXML parser into your headers/source
//

// These code blocks import the tinyxml headers verbatim.
// Included verbatim in order to comply with the zlib license, which requires 
// notices be displayed.
verbatim code heading
{
	//place this code in the dialect header glob.
	#ifndef OPCPP_TINYXML_HEADER
	#define OPCPP_TINYXML_HEADER

	#define TIXML_USE_STL
	opinclude "../headers/tinyxml/tinyxml.h"

	#endif
	//end of tinyxml headers
};

//place this code in the dialect source glob.
verbatim code source
{
	#ifdef OPCPP_TINYXML_SOURCE
	
	opinclude "../headers/tinyxml/tinyxml.cpp"
	opinclude "../headers/tinyxml/tinyxmlparser.cpp"
	opinclude "../headers/tinyxml/tinyxmlerror.cpp"
	
	#endif
	//end of tinyxml source
};

//=========================================================================================
// XML notes:
// Save - here we need to pass around an indention value (or store actually) and an ostream
//        should create tags and values using objects
// Load - here we utilize tinyxml to look for tags within nodes
//
// All  - theres no such thing as an unnamed type here,
//        we need to restructure it in a way that applies
//        the names as tags, and the container tags
//		  externally.
//=========================================================================================

namespace opcpp
{
	namespace visitors
	{
		// Consists of these visitor states:
		//	Visitor   - base visitor
		//	Fields    - named fields - handles transient, derives from Data
		//	Container - container visitor, derives from Data
		//	Pointer   - pointer visitor - handles pointers
		//	Data      - unnamed fields

		//Xml Save Visitor - the class you should use.
		class xml_save_archiver;

		using std::string;
		using std::ostream;
		using std::istream;
		using namespace opcpp::accessors;
		using opcpp::base::visitor_base;
		using std::endl;
		using std::map;

		// tiny xml stuff..
		using opcpp::xml::TiXmlElement;
		using opcpp::xml::TiXmlDocument;
		using opcpp::xml::TiXmlNode;
		using opcpp::xml::TiXmlText;

		namespace detail
		{
			//we have multiple values in this tag
			struct xml_save_tag
			{
				xml_save_tag(xml_save_archiver& v, member_info& i);
				xml_save_tag(xml_save_archiver& v, const char* t);
				~xml_save_tag();
				
			private:
				xml_save_archiver& visitor;
				const char*	    tag;
			};

			//we have a single value in this tag
			struct xml_save_value
			{
				xml_save_value(xml_save_archiver& v, member_info& i);
				xml_save_value(xml_save_archiver& v, const char* t);
				~xml_save_value();

			private:
				xml_save_archiver&	visitor;
				const char*		tag;
			};

			//Xml Save Data Visitor - assumes the fields have names!
			class xml_save_data_visitor : public visitor_base
			{
			public:
				xml_save_data_visitor(ostream& o, xml_save_archiver& r) : file(o), root(&r) {}
				xml_save_data_visitor(ostream& o) : file(o) {}

				template<class T>
				void serialize_value(const T& value)
				{
					file << value;
				}

				void serialize_string(const string& value)
				{
					file << value;
				}

				void visit(bool_info& info)
				{
					serialize_value( (int) info.get_bool() ? 1 : 0 );
				}

				//int
				void visit(int_info& info)
				{
					serialize_value(info.get_int());
				}

				//string
				void visit(string_info& info)
				{
					serialize_string(info.get_string());
				}

				//float
				void visit(float_info& info)
				{
					serialize_value(info.get_float());
				}

				//double
				void visit(double_info& info)
				{
					serialize_value(info.get_double());
				}

				//bytes
				void visit(bytes_info& info)
				{
					size_t num = info.type_size();
					
					unsigned char* current = (unsigned char*)info.address();

					std::stringstream ss;

					for(size_t i = 0; i < num; i++)
					{
						unsigned short val = *current;
						ss << val;
						ss << ' ';
						++current;
					}

					serialize_string(ss.str());
				}

				//enum
				void visit(enum_info& info)
				{
					int value;
					info.to_int(value);

					serialize_value(value);
				}

				//struct
				void visit(struct_info& info);

				//class
				void visit(class_info& info);

				//container
				void visit(value_container_info& info);

				//map
				void visit(key_container_info& info);

				//pair
				void visit(pair_info& info);

				//pointer
				void visit(pointer_info& info);

				//array
				void visit(array_info& info)
				{
					if(!info.is_serializable())
						return;

					member_info* inner = info.get_inner();

					size_t num = info.size();
					for(size_t i = 0; i < num; i++)
					{
						info.set_current(i);
						
						xml_save_tag xmltag(*root,info);

						inner->visit(*this);
					}
				}

			protected:
				friend class opcpp::visitors::xml_save_archiver;

				xml_save_archiver* root;
				ostream&		   file;
			};

			//NOTE: could easily be a template class.
			//Xml Save Field Visitor - applied to serialize named fields
			class xml_save_field_visitor : public xml_save_data_visitor
			{
			public:
				xml_save_field_visitor(ostream& o, xml_save_archiver& root) 
					: 
					xml_save_data_visitor(o,root)
				{}

				xml_save_field_visitor(ostream& o)
					:
					xml_save_data_visitor(o)
				{}

				typedef xml_save_data_visitor super;

				//bool
				void visit(bool_info& info)
				{
					if(!info.is_transient())
					{
						xml_save_value xmltag(*root,info);
						super::visit(info);
					}
				}

				//int
				void visit(int_info& info)
				{
					if(!info.is_transient())
					{
						xml_save_value xmltag(*root,info);
						super::visit(info);
					}
				}

				//string
				void visit(string_info& info)
				{
					if(!info.is_transient())
					{
						xml_save_value xmltag(*root,info);
						super::visit(info);
					}
				}

				//float
				void visit(float_info& info)
				{
					if(!info.is_transient())
					{
						xml_save_value xmltag(*root,info);
						super::visit(info);
					}
				}
				
				//double
				void visit(double_info& info)
				{
					if(!info.is_transient())
					{
						xml_save_value xmltag(*root,info);
						super::visit(info);
					}
				}

				//bytes
				void visit(bytes_info& info)
				{
					if(!info.is_transient())
					{
						xml_save_value xmltag(*root,info);
						super::visit(info);
					}
				}

				//enum
				void visit(enum_info& info)
				{
					if(!info.is_transient())
					{
						xml_save_value xmltag(*root,info);
						super::visit(info);
					}
				}

				//struct
				void visit(struct_info& info)
				{
					if(!info.is_transient())
					{
						xml_save_tag xmltag(*root,info);
						super::visit(info);
					}
				}

				//class
				void visit(class_info& info)
				{
					if(!info.is_transient())
					{
						xml_save_tag xmltag(*root,info);
						super::visit(info);
					}
				}

				//container
				void visit(value_container_info& info)
				{
					if(!info.is_transient())
					{
						//xml_save_tag xmltag(*root,info);
						super::visit(info);
					}
				}
				
				//map
				void visit(key_container_info& info)
				{
					if(!info.is_transient())
					{
						//xml_save_tag xmltag(*root,info);
						super::visit(info);
					}
				}

				//pointer
				void visit(pointer_info& info)
				{
					if(!info.is_transient())
					{
						xml_save_tag xmltag(*root,info);
						super::visit(info);
					}
				}
				
				//pointer
				void visit(pair_info& info)
				{
					if(!info.is_transient())
					{
						xml_save_tag xmltag(*root,info);
						super::visit(info);
					}
				}

				//arrays
				void visit(array_info& info)
				{
					if(!info.is_transient())
					{
						//xml_save_value xmltag(*root,info);
						super::visit(info);
					}
				}

			};

			//Xml Save Pointer Visitor - applied to serialize pointer types
			class xml_save_pointer_visitor : public visitor_base
			{
			public:
				xml_save_pointer_visitor(ostream& f, xml_save_archiver& r, pointer_info& p) 
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
				xml_save_archiver&  root;
				ostream&		 file;
				pointer_info&	 pointer;
			};

			//Xml Save Container Visitor - applied to serialize container contents
			class xml_save_container_visitor : public xml_save_data_visitor
			{
			public:
				xml_save_container_visitor(ostream& f, xml_save_archiver& root, value_container_info& c) 
					: 
					xml_save_data_visitor(f,root),
					container(c)
				{}

				typedef xml_save_data_visitor super;

				void visit(bool_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
					{
						xml_save_value xmlvalue(*root,container);
						super::visit(info);
					}
				}

				void visit(int_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
					{
						xml_save_value xmlvalue(*root,container);
						super::visit(info);
					}
				}

				void visit(string_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
					{
						xml_save_value xmlvalue(*root,container);
						super::visit(info);
					}
				}

				void visit(float_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
					{
						xml_save_value xmlvalue(*root,container);
						super::visit(info);
					}
				}
				
				void visit(double_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
					{
						xml_save_value xmlvalue(*root,container);
						super::visit(info);
					}
				}

				void visit(bytes_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
					{
						xml_save_value xmlvalue(*root,container);
						super::visit(info);
					}
				}

				void visit(enum_info& info)
				{
					for(bool i = container.begin(); i; i = container.increment())
					{
						xml_save_value xmlvalue(*root,container);
						super::visit(info);
					}
				}

				void visit(struct_info& info);

				void visit(class_info& info);

				//a container of containers
				void visit(value_container_info& info);

				//a container of maps
				void visit(key_container_info& info);

				//pointers
				void visit(pointer_info& info);

				//pairs
				void visit(pair_info& info);

			protected:
				value_container_info& container;
			};		

		}

		using detail::xml_save_container_visitor;
		using detail::xml_save_data_visitor;
		using detail::xml_save_field_visitor;
		using detail::xml_save_pointer_visitor;
		using detail::xml_save_tag;
		using detail::xml_save_value;

		/// \brief Archiver for saving data to xml.

		/// This archiver is able to save most any reflectable data type directly,
		/// including built-ins, stl, opclass, opstructs, and openums.
		/// 
		/// Use with xml_load_archiver to load and save data using xml.
		/// 
		/// This archiver writes directly to the stream and should have very good performance.
		///
		class xml_save_archiver
		{
		public:
			
			///
			/// \brief The constructor, pass in an output stream and the root xml tag.
			/// 
			xml_save_archiver(ostream& stream, const char* root_tag)
				: 
				pointercount(0), 
				datavisitor(stream), 
				fieldvisitor(stream),
				file(stream),
				indention(0),
				root(root_tag)
			{
				datavisitor.root  = this;
				fieldvisitor.root = this;

				{
					//write the xml header
					stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl << endl;
				}
				
				stream << '<' << root << '>' << endl;
				indent();
			}
			
			/// 
			/// \brief Destructor
			/// 
			~xml_save_archiver()
			{
				deindent();
				file << "</" << root << '>' << endl << endl;
			}
			
			///
			/// \brief Save a class object, it will output all its members directly within the root tag.
			///
			/// Since it outputs directly nested within the root tag, you should only save a single object
			/// using this method.
			/// 
			void save(opcpp::base::class_base& object)
			{
				save(&object);
			}

			/// 
			/// \brief Save a class object, it will output all its members directly within the root tag.
			///
			/// The un-tagged load function in xml_load_archiver should be used opposite this.
			/// 
			void save(opcpp::base::class_base* object)
			{
				std::streamsize size = file.precision();
				file.precision( std::numeric_limits<double>::digits10 + 2 );
				
				object->visit_data_members(fieldvisitor);
			}
			
			/// 
			/// \brief Save a value to xml - this may be anything except references.
			///
			/// This function takes a tag - which should be unique if you intend to save multiple pieces of data.
			/// 
			template<class T>
			void save(T& value, const char* tag)
			{
				std::streamsize size = file.precision();
				file.precision( std::numeric_limits<double>::digits10 + 2 );
				
				opcpp::fields::temp_field field(tag);
				opcpp::accessors::build_accessor(value,field).visit(fieldvisitor);
				
				file.precision(size);
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

			void indent()
			{
				++indention;
			}

			void deindent()
			{
				--indention;
			}

			void printindent()
			{
				for(int i = 0; i < indention; i++)
					file << "\t";
			}
			
			friend struct xml_save_tag;
			friend struct xml_save_value;
			
			friend class xml_save_data_visitor;
			friend class xml_save_field_visitor;
			friend class xml_save_container_visitor;
			friend class xml_save_pointer_visitor;
			
			xml_save_data_visitor	datavisitor;
			xml_save_field_visitor	fieldvisitor;
			ostream&				file;
			
			//indention
			int indention;

			const char* root;

			//pointer to index map.
			typedef map<void*,int>	pointermaptype;
			pointermaptype			pointermap;
			int						pointercount;
		};

		inline xml_save_value::xml_save_value(xml_save_archiver& v, member_info& info)
			:
			visitor(v)
		{
			tag = info.member_name();
			tag = tag ? tag : "Value";

			visitor.printindent();
			visitor.file << '<' << tag << '>';
		}

		inline xml_save_value::xml_save_value(xml_save_archiver& v, const char* t)
			:
			tag(t),
			visitor(v)
		{
			visitor.printindent();
			visitor.file << '<' << tag << '>';
		}

		inline xml_save_value::~xml_save_value()
		{
			visitor.file << "</" << tag << '>' << endl;
		}

		inline xml_save_tag::xml_save_tag(xml_save_archiver& v, member_info& info) : visitor(v)
		{
			tag = info.member_name();
			tag = tag ? tag : "Value";

			visitor.printindent();
			visitor.file << '<' << tag << '>' << endl;
			visitor.indent();
		}

		inline xml_save_tag::xml_save_tag(xml_save_archiver& v, const char* t) : tag(t), visitor(v)
		{
			visitor.printindent();
			visitor.file << '<' << tag << '>' << endl;
			visitor.indent();

		}

		inline xml_save_tag::~xml_save_tag()
		{
			visitor.deindent();
			visitor.printindent();
			visitor.file << "</" << tag << '>' << endl;
		}

		//struct
		inline void xml_save_data_visitor::visit(struct_info& info)
		{
			xml_save_field_visitor visitor(file,*root);
			info.visit_data_members(visitor);
		}

		//class
		inline void xml_save_data_visitor::visit(class_info& info)
		{
			xml_save_field_visitor visitor(file,*root);
			info.visit_data_members(visitor);
		}

		inline void xml_save_data_visitor::visit(pair_info& info)
		{
			xml_save_field_visitor visitor(file,*root);
			
			{
				//First
				xml_save_tag firsttag(*root,"First");
				info.get_first()->visit(visitor);
			}

			{
				//Second
				xml_save_tag secondtag(*root,"Second");
				info.get_second()->visit(visitor);
			}
		}

		inline void xml_save_container_visitor::visit(struct_info& info)
		{
			xml_save_field_visitor& visitor = root->fieldvisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				xml_save_tag xmltag(*root,container);

				info.visit_data_members(visitor);
			}
		}

		inline void xml_save_container_visitor::visit(class_info& info)
		{
			xml_save_field_visitor& visitor = root->fieldvisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				xml_save_tag xmltag(*root,container);

				info.visit_data_members(visitor);
			}
		}

		//a container of containers
		inline void xml_save_container_visitor::visit(value_container_info& info)
		{
			xml_save_data_visitor& visitor = root->datavisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				xml_save_tag xmltag(*root,container);

				info.visit(visitor);
			}
		}

		//a container of containers
		inline void xml_save_container_visitor::visit(pair_info& info)
		{
			xml_save_data_visitor& visitor = root->datavisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				xml_save_tag xmltag(*root,container);

				info.visit(visitor);
			}
		}

		//a container of maps
		inline void xml_save_container_visitor::visit(key_container_info& info)
		{
			xml_save_data_visitor& visitor = root->datavisitor;

			for(bool i = container.begin(); i; i = container.increment())
			{
				xml_save_tag xmltag(*root,container);

				info.visit(visitor);
			}
		}

		//pointers
		inline void xml_save_container_visitor::visit(pointer_info& info)
		{
			xml_save_pointer_visitor visitor(file,*root,info);

			for(bool i = container.begin(); i; i = container.increment())
			{
				xml_save_tag xmltag(*root,container);

				info.get_inner()->visit(visitor);
			}
		}

		inline void xml_save_data_visitor::visit(value_container_info& info)
		{
			if(!info.is_serializable())
				return;
			
			//pass this to a new container visitor...
			xml_save_container_visitor visitor(file,*root,info);

			//save the elements (efficiently)
			info.get_inner()->visit(visitor);
		}


		inline void xml_save_data_visitor::visit(key_container_info& info)
		{
			if(!info.is_serializable())
				return;

			xml_save_field_visitor visitor(file,*root);
			
			//NOTE: not the fastest.
			for(bool i = info.begin(); i; i = info.increment())
			{
				//<Name> <Key> blah <Value> blah
				xml_save_tag xmltag(*root,info);
				
				//<Key>
				{
					xml_save_tag keytag(*root,"Key");
					info.get_key()->visit(visitor);
				}

				//<Value>
				{
					//xml_save_tag valuetag(*root,"Value");
					//note: will automatically get <Value> due to un-named field
					info.get_inner()->visit(visitor);
				}

			}
 		}

		inline void xml_save_data_visitor::visit(pointer_info& info)
		{
			xml_save_pointer_visitor visitor(file,*root,info);

			info.get_inner()->visit(visitor);
		}

		inline void xml_save_pointer_visitor::visit(struct_info& info)
		{
			//make sure this type is serializable
			if(!info.get_type().is_serializable())
				return;

			void* ptr = pointer.get_pointer();

			if(ptr == NULL)
			{
				xml_save_value xmltag(root,"Id");
				file << -1;
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
				
				{
					xml_save_value xmltag(root,"Id");
					file << index;	
				}
				
				if(!bIndexed)
				{
					xml_save_field_visitor visitor(file,root);
					
					xml_save_tag xmltag(root,"Data");
					
					info.visit_data_members(visitor);
				}
			}
		}

		inline void xml_save_pointer_visitor::visit(class_info& info)
		{
			if(!info.get_type().is_serializable())
				return;
			
			void* ptr = pointer.get_pointer();
			
			if(ptr == NULL)
			{
				xml_save_value xmltag(root,"Id");
				file << -1;
			}
			else
			{
				//make sure the class instance is serializable
				if(!info.get_class()->get_type().is_serializable())
				{
					xml_save_value xmltag(root,"Id");
					
					file << -1;
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

				{
					xml_save_value xmltag(root,"Id");
					file << index;	
				}
	
				if(!bIndexed)
				{
					xml_save_field_visitor visitor(file,root);
					
					{
						xml_save_value xmltag(root,"Class");

						const char* classname = info.get_class()->get_type().get_name();
						file << classname;
					}

					xml_save_tag xmltag(root,"Data");

					info.visit_data_members(visitor);
				}
			}
		}


		//
		// xml_load_archiver
		//

		//Xml Save Visitor - the class you should use.
		class xml_load_archiver;

		namespace detail
		{

			//Xml Save Data Visitor - applied to un-named data fields, shared by Container and the main Visitor
			class xml_load_data_visitor : public visitor_base
			{
			public:
				xml_load_data_visitor(TiXmlElement* n, xml_load_archiver& r) : node(n), root(&r) {}

				TiXmlElement* get_element(member_info& info)
				{
					if(const char* name = info.member_name())
					{
						return get_element(name);
					}
					else
					{
						return get_element("Value");
					}
				}

				TiXmlElement* get_element(TiXmlElement* parent, const char* name)
				{
					if(TiXmlElement* element = parent->FirstChildElement(name))
						return element;

					return NULL;
				}


				TiXmlElement* get_element(const char* name)
				{
					return get_element(node,name);
				}

				template<class T>
				void string_to(const char* source, T& dest)
				{
					std::stringstream ss;
					ss << source;
					ss >> dest;
				}

				//deserialize a given element
				template<class T>
				void deserialize(TiXmlElement* element, T& value)
				{
					if(TiXmlNode* textnode = element->FirstChild())
					{
						if(TiXmlText* text = textnode->ToText())
						{
							string_to<T>(text->Value(),value);
						}
					}
				}

				//deserialize a basic type.
				template<class T>
				void deserialize(member_info& info, T& value)
				{
					if(const char* name = info.member_name())
					{
						deserialize_value(name,value);
					}
					else
					{
						deserialize_value("Value",value);
					}
				}


				//used to grab basic tagged values (looks at attributes and elements)
				template<class T>
				void deserialize_value(const char* tag, T& value)
				{
					//attempt to grab the value as text from a child element
					if(TiXmlElement* element = node->FirstChildElement(tag))
					{
						if(TiXmlNode* textnode = element->FirstChild())
						{
							if(TiXmlText* text = textnode->ToText())
							{
								string_to<T>(text->Value(),value);
							}
						}
					}
					//if we dont have a tag check the attributes
					else
					{
						const char* strvalue = node->Attribute(tag);

						if(strvalue)
						{
							string_to(strvalue,value);
						}
					}
				}

				TiXmlElement* get_first(member_info& info)
				{
					if(const char* name = info.member_name())
					{
						return get_first(name);
					}
					else
					{
						return get_first("Value");
					}
				}

				//this will be used to grab the first basic container element
				TiXmlElement* get_first(const char* tag)
				{
					//attempt to grab the value as text from a child element
					if(TiXmlElement* element = node->FirstChildElement(tag))
					{
						return element;
					}

					return NULL;
				}

				TiXmlElement* get_next(TiXmlElement* lastvalue, member_info& info)
				{
					if(const char* name = info.member_name())
					{
						return get_next(lastvalue,name);
					}
					else
					{
						return get_next(lastvalue,"Value");
					}
				}

				TiXmlElement* get_next(TiXmlElement* lastvalue, const char* tag)
				{
					if(TiXmlElement* element = lastvalue->NextSiblingElement(tag))
						return element;

					return NULL;
				}

				//bool
				void visit(bool_info& info)
				{
					deserialize(info,info.get_bool());
				}

				//int
				void visit(int_info& info)
				{
					deserialize(info,info.get_int());
				}

				//float
				void visit(float_info& info)
				{
					deserialize(info,info.get_float());
				}
				
				//double
				void visit(double_info& info)
				{
					deserialize(info,info.get_double());
				}

				//bytes
				void visit(bytes_info& info)
				{
					unsigned char* current = (unsigned char*)info.address();
					size_t size = info.type_size();
					
					string bytestring;
					deserialize(info,bytestring);

					std::stringstream ss;
					ss << bytestring;

					for(size_t i = 0; i < size; i++)
					{
						unsigned short val;
						ss >> val;
						*current = (unsigned char)val;
						++current;
					}
				}

				//string
				void visit(string_info& info)
				{
					deserialize(info,info.get_string());
				}

				//NOTE: reverify this!
				//enum
				void visit(enum_info& info)
				{
					int value;
					deserialize(info,value);
					info.from_int(value);
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

				//pairs
				void visit(pair_info& info);

				//array
				void visit(array_info& info)
				{
					if(!info.is_serializable())
						return;

					if(TiXmlElement* found = get_element(info))
					{
						
						member_info* inner = info.get_inner();
						
						size_t i = 0;
						size_t num = info.size();
						
						TiXmlElement* current = get_first(info);
						
						while(current && i < num)
						{
							xml_load_data_visitor visitor(current,*root);

							info.set_current(i);
							inner->visit(visitor);

							current = get_next(current,info);
							++i;	
						}
					}

				}

				void visit(set_info& info);
	
				void visit(multiset_info& info);

			protected:
				friend class xml_load_archiver;
				
				xml_load_archiver*	root;
				TiXmlElement*	node;
			};


			//visit tags
			class xml_load_basic_visitor : public xml_load_data_visitor
			{
			public:
				xml_load_basic_visitor(TiXmlElement* n, xml_load_archiver& r) 
					:
					xml_load_data_visitor(n,r)
				{}
				
				//used to grab basic tagged values (looks at attributes and elements)
				template<class T>
				void deserialize(T& value)
				{
					//attempt to grab the value as text from a child element
					if(TiXmlNode* textnode = node->FirstChild())
					{
						if(TiXmlText* text = textnode->ToText())
						{
							string_to<T>(text->Value(),value);
						}
					}
				}

				//bool
				void visit(bool_info& info)
				{
					deserialize(info.get_bool());
				}

				//int
				void visit(int_info& info)
				{
					deserialize(info.get_int());
				}

				//float
				void visit(float_info& info)
				{
					deserialize(info.get_float());
				}

				//double
				void visit(double_info& info)
				{
					deserialize(info.get_double());
				}

				//bytes
				void visit(bytes_info& info)
				{
					size_t size = info.type_size();

					unsigned char* valptr = (unsigned char*)info.address();

					string bytestring;
					deserialize(bytestring);

					std::stringstream ss;
					ss << bytestring;

					for(size_t i = 0; i < size; i++)
					{
						unsigned short val;
						ss >> val;
						*valptr = (unsigned char)val;
						++valptr;
					}
				}

				//string
				void visit(string_info& info)
				{
					deserialize(info.get_string());
				}

				//enum
				void visit(enum_info& info)
				{
					int value;
					deserialize(value);
					info.from_int(value);
				}

				//struct
				void visit(struct_info& info);

				//class
				void visit(class_info& info);

				//pointer
				void visit(pointer_info& info);

				//pairs
				void visit(pair_info& info);

				//??? containers?
			};


			template<>
			inline void xml_load_data_visitor::string_to<string>(const char* source, string& dest)
			{
				dest = source;
			}

			//Xml Save Field Visitor - applied to serialize named fields
			class xml_load_field_visitor : public xml_load_data_visitor
			{
			public:
				xml_load_field_visitor(TiXmlElement* n, xml_load_archiver& root) : xml_load_data_visitor(n,root) {}
				
				typedef xml_load_data_visitor super;

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

				//pairs
				void visit(pair_info& info)
				{
					if(!info.is_transient())
						super::visit(info);
				}
			};

			//Xml Save Pointer Visitor - applied to serialize pointer types
			class xml_load_pointer_visitor : public visitor_base
			{
			public:
				xml_load_pointer_visitor(TiXmlElement* n, xml_load_archiver& r, pointer_info& p) : pointer(p), node(n), root(r) {}

				TiXmlElement* get_element(member_info& info)
				{
					if(const char* name = info.member_name())
					{
						return get_element(name);
					}
					else
					{
						return get_element("Value");
					}
				}

				TiXmlElement* get_element(const char* name)
				{
					if(TiXmlElement* element = node->FirstChildElement(name))
						return element;

					return NULL;
				}

				//deserialize a given element
				template<class T>
				void deserialize(TiXmlElement* element, T& value)
				{
					if(TiXmlNode* textnode = element->FirstChild())
					{
						if(TiXmlText* text = textnode->ToText())
						{
							std::stringstream ss;
							ss << text->Value();
							ss >> value;
						}
					}
				}

				//pointer to struct
				void visit(struct_info& info);

				//pointer to class
				void visit(class_info& info);

			protected:
				xml_load_archiver& root;
				TiXmlElement*	node;
				pointer_info&	pointer;
			};


			//Xml Save Container Visitor - applied to serialize container contents
			class xml_load_container_visitor : public xml_load_data_visitor
			{
			public:
				xml_load_container_visitor(TiXmlElement* n, xml_load_archiver& root, value_container_info& c) 
					: 
					xml_load_data_visitor(n,root),
					container(c)
				{}

				typedef xml_load_data_visitor super;

				//bool
				void visit(bool_info& info)
				{
					TiXmlElement* current = get_first(container);

					while(current)
					{
						bool value;
						deserialize(current,value);
						container.push_back(value);	
										
						current = get_next(current,container);
					}
				}

				//int
				void visit(int_info& info)
				{
					TiXmlElement* current = get_first(container);

					while(current)
					{
						int value;
						deserialize(current,value);
						container.push_back(value);	

						current = get_next(current,container);
					}
				}

				//string
				void visit(string_info& info)
				{
					TiXmlElement* current = get_first(container);

					while(current)
					{
						string value;
						deserialize(current,value);
						container.push_back(value);	

						current = get_next(current,container);
					}
				}

				//float
				void visit(float_info& info)
				{
					TiXmlElement* current = get_first(container);

					while(current)
					{
						float value;
						deserialize(current,value);
						container.push_back(value);	

						current = get_next(current,container);
					}
				}
				
				//double
				void visit(double_info& info)
				{
					TiXmlElement* current = get_first(container);

					while(current)
					{
						double value;
						deserialize(current,value);
						container.push_back(value);	

						current = get_next(current,container);
					}
				}

				//bytes
				void visit(bytes_info& info)
				{
					size_t size = info.type_size();		

					TiXmlElement* current = get_first(container);

					while(current)
					{
						container.push_back_default();
						
						unsigned char* valptr = (unsigned char*)info.address();
						
						string bytestring;
						deserialize(current,bytestring);
						
						std::stringstream ss;
						ss << bytestring;
						
						for(size_t i = 0; i < size; i++)
						{
							unsigned short val;
							ss >> val;
							*valptr = (unsigned char)val;
							++valptr;
						}
						
						current = get_next(current,container);
					}
				}

				//enum
				void visit(enum_info& info)
				{
					TiXmlElement* current = get_first(container);

					while(current)
					{
						container.push_back_default();	

						int value;
						deserialize(current,value);
						info.from_int(value);

						current = get_next(current,container);
					}

				}

				//struct
				void visit(struct_info& info);

				//class
				void visit(class_info& info);

				//pair
				void visit(pair_info& info);

				//a container of containers
				void visit(value_container_info& info);

				//pointers
				void visit(pointer_info& info)
				{
					TiXmlElement* current = get_first(container);

					while(current)
					{
						xml_load_pointer_visitor visitor(current,*root,info);

						container.push_back_default();

						info.get_inner()->visit(visitor);

						current = get_next(current,container);
					}
				}

				void visit(vector_info& info);

				void visit(key_container_info& info);

				void visit(multiset_info& info);

				void visit(set_info& info);

			protected:
				value_container_info& container;

			};
		}

		using detail::xml_load_container_visitor;
		using detail::xml_load_data_visitor;
		using detail::xml_load_field_visitor;
		using detail::xml_load_pointer_visitor;
		using detail::xml_load_basic_visitor;


		/// \brief Archiver for loading xml data.

		/// This archiver is able to load most any reflectable data type directly,
		/// including built-ins, stl, opclass, opstructs, and openums.
		/// 
		/// Use with xml_save_archiver to save and load data using xml. 
		/// 
		/// You may also use this class to load other xml formats,
		/// It will read in attributes as well as tags, as long
		/// as the names match.
		/// 
		/// This archiver uses TinyXml internally to load xml.
		/// 
		class xml_load_archiver
		{
		public:

			/// 
			/// \brief The constructor, pass in an input stream and a root tag name.
			///
			/// This tag name should coorespond to the root tag in the loaded xml file.
			/// 
			xml_load_archiver(istream& stream, const char* root_tag)
				: 
				indexcount(0),
				rootelement(0),
				root(root_tag)
			{
				doc.SetCondenseWhiteSpace(false);

				stream >> doc;
			
				if(Error())
					return;

				TiXmlElement* element = doc.RootElement();

				if(element->ValueStr() == root)
					rootelement = element;
			}

			///
			/// \brief May call anytime after construction, returns true if there were errors.
			/// 
			bool Error()
			{
				if(doc.Error())
					return true;
				return false;
			}

			/// 
			/// \brief If there was an error, gives you the file row.
			/// 
			int ErrorRow()
			{
				return doc.ErrorRow();
			}

			/// 
			/// \brief If there was an error, gives you the file column.
			/// 
			int ErrorCol()
			{
				return doc.ErrorCol();
			}

			/// 
			/// \brief If there was an error, gives you the error string description.
			/// 
			const char* ErrorDescription()
			{
				return doc.ErrorDesc();
			}

			/// 
			/// \brief Load an object's contents, from directly within the root node.
			/// 
			bool load(opcpp::base::class_base& object)
			{
				return load(&object);
			}

			/// 
			/// \brief Load an object's contents, from directly within the root node.
			///
			/// Takes a pointer - but you must give it a valid object for it to load.
			/// 
			bool load(opcpp::base::class_base* object)
			{
				if(Error() || !rootelement || !object)
					return false;

				xml_load_field_visitor visitor(rootelement,*this);
				object->visit_data_members(visitor);

				return Error();
			}
			
			/// 
			/// \brief Load xml into almost any data type, given a value to fill and a tag name.
			///
			/// This cooresponds with the tagged save function in xml_save_archiver.
			/// 
			/// This load is able to read almost any type, except references.
			/// It is also able to load pointers to opclass or opstructs,
			/// where it will instantiate a new object and set the pointer to the new instance.
			/// 
			/// Usage: archiver.load(data,"tag");
			/// 
			template<class T>
			bool load(T& value, const char* tag)
			{
				if(Error() || !rootelement)
					return false;
				
				xml_load_field_visitor visitor(rootelement,*this);
				
				//temporarily fakes a field
				opcpp::fields::temp_field field(tag);
				opcpp::accessors::build_accessor(value,field).visit(visitor);

				return Error();
			}

		private:
			TiXmlElement* rootelement;
			const char* root;

			bool bSuccess;

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

			friend class xml_load_pointer_visitor;

			//pointer to index map.
			indexmaptype			indexmap;
			int						indexcount;
		
			TiXmlDocument doc;
		};
		
		//struct
		inline void xml_load_data_visitor::visit(struct_info& info)
		{
			if(TiXmlElement* found = get_element(info))
			{
				xml_load_field_visitor visitor(found,*root);

				info.visit_data_members(visitor);
			}
		}

		inline void xml_load_basic_visitor::visit(struct_info& info)
		{
			xml_load_field_visitor visitor(node,*root);

			info.visit_data_members(visitor);
		}
	
		//class
		inline void xml_load_data_visitor::visit(class_info& info)
		{
			if(TiXmlElement* found = get_element(info))
			{
				xml_load_field_visitor visitor(found,*root);

				info.visit_data_members(visitor);
			}
		}

		inline void xml_load_basic_visitor::visit(class_info& info)
		{
			xml_load_field_visitor visitor(node,*root);

			info.visit_data_members(visitor);
		}

		inline void xml_load_data_visitor::visit(pair_info& info)
		{
			if(TiXmlElement* found = get_element(info))
			{
				//First
				if(TiXmlElement* first = get_element(found,"First"))
				{
					xml_load_field_visitor visitor(first,*root);
					info.get_first()->visit(visitor);
				}

				//Second
				if(TiXmlElement* second = get_element(found,"Second"))
				{
					xml_load_field_visitor visitor(second,*root);
					info.get_second()->visit(visitor);
				}

			}
		}

		inline void xml_load_basic_visitor::visit(pair_info& info)
		{
			//First
			if(TiXmlElement* first = get_element("First"))
			{
				xml_load_field_visitor visitor(first,*root);
				info.get_first()->visit(visitor);
			}

			//Second
			if(TiXmlElement* second = get_element("Second"))
			{
				xml_load_field_visitor visitor(second,*root);
				info.get_second()->visit(visitor);
			}
		}
		inline void xml_load_data_visitor::visit(value_container_info& info)
		{
			if(!info.is_serializable())
				return;
			
			//pass this to a new container visitor...
			xml_load_container_visitor visitor(node,*root,info);

			info.clear();

			//save the elements (efficiently)
			info.get_inner()->visit(visitor);
		}
		
		//???basic

		inline void xml_load_data_visitor::visit(vector_info& info)
		{
			if(!info.is_serializable())
				return;
			
			//TODO: load # of elements and reserve (requires an optional count attribute...)
			info.resize(0);
			
			//pass this to a new container visitor...
			xml_load_container_visitor visitor(node,*root,info);
			
			//save the elements (efficiently)
			info.get_inner()->visit(visitor);
		}
		
		//??? basic

		inline void xml_load_data_visitor::visit(key_container_info& info)
		{
			if(!info.is_serializable())
				return;
			
			info.clear();

			TiXmlElement* current = get_first(info);
			
			while(current)
			{
				//Key
				if(TiXmlElement* key = get_element(current,"Key"))
				{
					//Value
					//note: value uses autogenerated <Value> from un-named field
					{
						xml_load_data_visitor keyvisitor	(key,	 *root);
						xml_load_data_visitor valuevisitor	(current,*root);
						
						info.insert_visit(keyvisitor,valuevisitor);						
					}
				}
				
				current = get_next(current,info);
			}
		}


		inline void xml_load_data_visitor::visit(set_info& info)
		{
			if(!info.is_serializable())
				return;
			
			info.clear();

			TiXmlElement* current = get_first(info);
			
			while(current)
			{
				xml_load_basic_visitor visitor(current,*root);
				
				info.insert_visit(visitor);
				
				current = get_next(current,info);
			}
		}

		inline void xml_load_data_visitor::visit(multiset_info& info)
		{
			if(!info.is_serializable())
				return;
			
			info.clear();

			TiXmlElement* current = get_first(info);
			
			while(current)
			{
				xml_load_basic_visitor visitor(current,*root);
				
				info.insert_visit(visitor);
				
				current = get_next(current,info);
			}
		}

		inline void xml_load_data_visitor::visit(pointer_info& info)
		{
			if(TiXmlElement* element = get_element(info))
			{
				xml_load_pointer_visitor visitor(element,*root,info);
				
				info.get_inner()->visit(visitor);
			}
		}
		
		inline void xml_load_basic_visitor::visit(pointer_info& info)
		{
			xml_load_pointer_visitor visitor(node,*root,info);

			info.get_inner()->visit(visitor);
		}
		
		inline void xml_load_pointer_visitor::visit(struct_info& info)
		{
			//pointer to a struct
			if(!info.get_type().is_serializable())
				return;

			//Load the id index
			int id = -1;
			if(TiXmlElement* xmlid = get_element("Id"))
			{
				deserialize(xmlid,id);
			}

			if(id == -1)
			{
				pointer.set_pointer_unsafe(NULL);
				return;
			}

			xml_load_archiver::indexmaptype::iterator it;

			bool bIndexed = root.HasIndex(id,it);

			if(bIndexed)
			{
				struct_base* ptr = (struct_base*)(*it).second;

				//already indexed, set the pointer
				pointer.set_pointer_struct(ptr);
			}
			else
			{
				if(TiXmlElement* xmldata = get_element("Data"))
				{
					xml_load_field_visitor visitor(xmldata,root);
					
					struct_base* newstruct = info.get_type().new_instance();
						
					(*it).second = newstruct;

					pointer.set_pointer_struct(newstruct);

					info.visit_data_members(visitor);
				}
			}
		}

		inline void xml_load_pointer_visitor::visit(class_info& info)
		{
			if(!info.get_type().is_serializable())
				return;

			int id = -1;
			if(TiXmlElement* xmlid = get_element("Id"))
			{
				deserialize(xmlid,id);
			}

			if(id == -1)
			{
				pointer.set_pointer_unsafe(NULL);
				return;
			}

			xml_load_archiver::indexmaptype::iterator it;

			bool bIndexed = root.HasIndex(id,it);

			if(bIndexed)
			{
				class_base* ptr = (class_base*)(*it).second;

				//already indexed, set the pointer
				pointer.set_pointer_class(ptr);
			}
			else
			{
				//load the class (create a new instance!)		
				if(TiXmlElement* xmldata = get_element("Data"))
				{
					//load the classname
					string classname;
					if(TiXmlElement* xmlclass = get_element("Class"))
					{
						deserialize(xmlclass,classname);
					}
					else
						return;

					//get the type from the name...
					class_type* type = opcpp::registration::class_tracker::get_type(classname);

					//shouldn't happen (error)
					if(!type)
						return;
		
					xml_load_field_visitor visitor(xmldata,root);

					class_base* newclass = type->new_instance();

					(*it).second = newclass;

					pointer.set_pointer_class(newclass);

					info.visit_data_members(visitor);
				}
			}
		}

		inline void xml_load_container_visitor::visit(struct_info& info)
		{
			TiXmlElement* current = get_first(container);

			while(current)
			{
				xml_load_field_visitor visitor(current,*root);

				container.push_back_default();
		
				//now we need to visit the inserted one...
				info.visit_data_members(visitor);

				current = get_next(current,container);
			}
		}

		inline void xml_load_container_visitor::visit(class_info& info)
		{
			TiXmlElement* current = get_first(container);

			while(current)
			{
				xml_load_field_visitor visitor(current,*root);

				container.push_back_default();

				//now we need to visit the inserted one...
				info.visit_data_members(visitor);

				current = get_next(current,container);
			}
		}

		inline void xml_load_container_visitor::visit(value_container_info& info)
		{
			TiXmlElement* current = get_first(container);

			while(current)
			{
				xml_load_data_visitor visitor(current,*root);

				container.push_back_default();
				
				//now we need to visit the inserted one...
				info.visit(visitor);

				current = get_next(current,container);
			}
		}
		
		inline void xml_load_container_visitor::visit(vector_info& info)
		{
			visit((value_container_info&)info);
		}

		inline void xml_load_container_visitor::visit(set_info& info)
		{
			visit((value_container_info&)info);
		}

		inline void xml_load_container_visitor::visit(multiset_info& info)
		{
			visit((value_container_info&)info);
		}


		inline void xml_load_container_visitor::visit(key_container_info& info)
		{
			TiXmlElement* current = get_first(container);

			while(current)
			{
				xml_load_data_visitor visitor(current,*root);

				container.push_back_default();

				//now we need to visit the inserted one...
				info.visit(visitor);

				current = get_next(current,container);
			}
		}

		inline void xml_load_container_visitor::visit(pair_info& info)
		{
			TiXmlElement* current = get_first(container);

			while(current)
			{
				xml_load_basic_visitor visitor(current,*root);

				container.push_back_default();

				//now we need to visit the inserted one...
				info.visit(visitor);

				current = get_next(current,container);
			}
		}
	}
}

verbatim code heading
{
	#endif //OPCPP_XML_SERIALIZE_DOH
}




