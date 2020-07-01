//****************************************************************
// Copyright © 2008 opGames LLC - All Rights Reserved
//
// Authors: Lucas Ellis
//
// File: stlsupport.doh
// Date: 07/25/2007
//
// Description:
//
// STL support for member_info accessors
//****************************************************************

//TODO: improve this,
//		need insert access, (hard, need casting)
//		push_back, front, pop 
//		algorithms (only a few are possible)
//			reverse
//			rotate
//			shuffle
//			sort <- I'd say for sure...might need more adapters...

//
// STL auto-detection
//

//TODO: need to improve w/ multiple projects (and dlls etc)
opmacro guard_start(name)
{
#ifdef OPCPP_STL_@name
#ifndef OPCPP_GUARD_@name
#define OPCPP_GUARD_@name
};

opmacro guard_end
{
#endif
#endif
};

opmacro guard_meta_start(name)
{
#ifdef OPCPP_STL_@name
#ifndef OPCPP_GUARD_META_@name
#define OPCPP_GUARD_META_@name
};

opmacro guard_meta_end
{
#endif
#endif
};



code footer
{
	//Visual Studio:
#ifdef _DEQUE_
#define OPCPP_STL_DEQUE
#endif
#ifdef _VECTOR_
#define OPCPP_STL_VECTOR
#endif
#ifdef _LIST_
#define OPCPP_STL_LIST
#endif
#ifdef _SET_
#define OPCPP_STL_SET
#define OPCPP_STL_MULTISET
#endif
#ifdef _MAP_
#define OPCPP_STL_MAP
#define OPCPP_STL_MULTIMAP
#endif
#ifdef _UTILITY_
#define OPCPP_STL_PAIR
#endif

	//G++ / LibStdC++3
#ifdef _STL_DEQUE_H
#define OPCPP_STL_DEQUE
#endif
#ifdef _STL_VECTOR_H
#define OPCPP_STL_VECTOR
#endif
#ifdef _STL_LIST_H
#define OPCPP_STL_LIST
#endif
#ifdef _STL_SET_H
#define OPCPP_STL_SET
#endif
#ifdef _STL_MULTISET_H
#define OPCPP_STL_MULTISET
#endif
#ifdef _STL_MAP_H
#define OPCPP_STL_MAP
#endif
#ifdef _STL_MULTIMAP_H
#define OPCPP_STL_MULTIMAP
#endif
#ifdef _STL_PAIR_H //?
#define OPCPP_STL_PAIR
#endif

	//G++ / LibStdC++4
#ifdef _DEQUE_H
#define OPCPP_STL_DEQUE
#endif
#ifdef _VECTOR_H
#define OPCPP_STL_VECTOR
#endif
#ifdef _LIST_H
#define OPCPP_STL_LIST
#endif
#ifdef _SET_H
#define OPCPP_STL_SET
#endif
#ifdef _MULTISET_H
#define OPCPP_STL_MULTISET
#endif
#ifdef _MAP_H
#define OPCPP_STL_MAP
#endif
#ifdef _MULTIMAP_H
#define OPCPP_STL_MULTIMAP
#endif
#ifdef _PAIR_H
#define OPCPP_STL_PAIR
#endif

	namespace opcpp
	{
		namespace accessors
		{
			namespace detail
			{
				//
				// Selectors for single value container infos
				//

				expand guard_start(VECTOR)

				template<typename t, typename a>
				struct vector_info_type : single_container_info< std::vector<t,a>, vector_info>
				{
					typedef single_container_info< std::vector<t,a>, vector_info> super;

					using super::data;
					using super::current;
					using super::inner;

					vector_info_type()
					{}

					vector_info_type(std::vector<t,a>& indata)
						:
						super(invalid_field::get_instance(),indata)
					{}

					vector_info_type(const data_field& f, std::vector<t,a>& indata)
						: 
						super(f,indata)
					{}

					virtual member_info* new_copy()
					{
						return new vector_info_type<t,a>(*this);
					}

					//implement vector_info virtuals
					void reserve(size_t value)
					{
						data->reserve(value);
					}

					void resize(size_t value)
					{
						data->resize(value);
					}

					bool seek(size_t index)
					{
						if(index >= data->size())
							return false;

						current = data->begin()+index;
						call_set_data(inner,&*current);

						return true;
					}
				};

				template<typename data_type, typename a>
				struct selector< std::vector<data_type,a> >
				{
					typedef vector_info_type<data_type,a> type;
				};

				//special handling for vector<bool> - not supported yet.
				template<typename a>
				struct selector< std::vector<bool,a> >
				{
					typedef unknown_info_type< std::vector< bool,a > > type;
				};

				template<typename type, typename a>
				struct select_member_cast< std::vector<type,a> >
				{
					static bool can_cast(vector_info* info)
					{
						return run_member_cast<type>::can_cast(info->get_inner());
					}

					static bool can_cast(member_info* info)
					{
						if(vector_info* vect = info_cast<vector_info>(info))
							return can_cast(vect);
						return false;
					}
				};

				expand guard_end

				expand guard_start(LIST)

				template<typename t, typename a>
				struct list_info_type : single_container_info< std::list<t,a>, list_info >
				{
					typedef single_container_info< std::list<t,a>, list_info > super;

					list_info_type()
					{
					}

					list_info_type(std::list<t,a>& indata)
						:
					super(invalid_field::get_instance(),indata)
					{}

					list_info_type(const data_field& f, std::list<t,a>& indata)
						: 
					super(f,indata)
					{}

					virtual member_info* new_copy()
					{
						return new list_info_type<t,a>(*this);
					}	
				};
				template<typename data_type, typename a>
				struct selector< std::list<data_type,a> >
				{
					typedef list_info_type<data_type,a> type;
				};

				template<typename type, typename a>
				struct select_member_cast< std::list<type,a> >
				{
					static bool can_cast(list_info* info)
					{
						return run_member_cast<type>::can_cast(info->get_inner());
					}

					static bool can_cast(member_info* info)
					{
						if(list_info* c = info_cast_visitor<list_info>(info).result())
							return can_cast(c);
						return false;
					}
				};

				expand guard_end

				expand guard_start(DEQUE)

				template<typename t, typename a>
				struct deque_info_type : single_container_info< std::deque<t,a>, deque_info>
				{
					typedef single_container_info< std::deque<t,a>, deque_info> super;

					using super::data;
					using super::current;
					using super::inner;

					deque_info_type()
					{}


					deque_info_type(std::deque<t,a>& indata)
						:
					super(invalid_field::get_instance(),indata)
					{}

					deque_info_type(const data_field& f, std::deque<t,a>& indata)
						: 
					super(f,indata)
					{}

					virtual member_info* new_copy()
					{
						return new deque_info_type<t,a>(*this);
					}

					void resize(size_t value)
					{
						data->resize(value);
					}

					bool seek(size_t index)
					{
						if(index >= data->size())
							return false;

						current = data->begin()+index;
						call_set_data(inner,&*current);

						return true;
					}
				};

				template<typename data_type, typename a>
				struct selector< std::deque<data_type,a> >
				{
					typedef deque_info_type<data_type,a> type;
				};

				template<typename type, typename a>
				struct select_member_cast< std::deque<type,a> >
				{
					static bool can_cast(deque_info* info)
					{
						return run_member_cast<type>::can_cast(info->get_inner());
					}

					static bool can_cast(member_info* info)
					{
						if(deque_info* c = info_cast_visitor<deque_info>(info).result())
							return can_cast(c);
						return false;
					}
				};

				expand guard_end

				expand guard_start(SET)

				template<typename t, typename p, typename a>
				struct set_info_type : single_container_info< std::set<t,p,a>, set_info >
				{
					typedef single_container_info< std::set<t,p,a>, set_info > super;

					using super::data;

					set_info_type()
					{}

					set_info_type(std::set<t,p,a>& indata)
						:
					super(invalid_field::get_instance(),indata)
					{}

					set_info_type(const data_field& f, std::set<t,p,a>& indata)
						: 
					super(f,indata)
					{}

					virtual member_info* new_copy()
					{
						return new set_info_type<t,p,a>(*this);
					}

					virtual bool insert_default()
					{
						return false;
					}

					virtual bool insert_visit(visitor_base& visitor)
					{
						return run_insert_visit<t,typename super::innertype>::type::insert(*data,visitor);
					}
				};

				template<typename data_type, typename p, typename a>
				struct selector< std::set<data_type,p,a> >
				{
					typedef set_info_type<data_type,p,a> type;
				};

				template<typename type, typename p, typename a>
				struct select_member_cast< std::set<type,p,a> >
				{
					static bool can_cast(set_info* info)
					{
						return run_member_cast<type>::can_cast(info->get_inner());
					}

					static bool can_cast(member_info* info)
					{
						if(set_info* c = info_cast_visitor<set_info>(info).result())
							return can_cast(c);
						return false;
					}
				};

				expand guard_end

				expand guard_start(MULTISET)

				template<typename t, typename p, typename a>
				struct multiset_info_type : single_container_info< std::multiset<t,p,a>, multiset_info>
				{
					typedef single_container_info< std::multiset<t,p,a>, multiset_info> super;

					using super::data;

					multiset_info_type()
					{}

					multiset_info_type(std::multiset<t,p,a>& indata)
						: 
					super(invalid_field::get_instance(),indata)
					{}

					multiset_info_type(const data_field& f, std::multiset<t,p,a>& indata)
						: 
					super(f,indata)
					{}

					virtual member_info* new_copy()
					{
						return new multiset_info_type<t,p,a>(*this);
					}

					virtual bool insert_default()
					{
						return false;
					}

					virtual bool insert_visit(visitor_base& visitor)
					{
						return run_insert_visit<t,typename super::innertype>::type::insert(*data,visitor);
					}
				};

				template<typename data_type, typename p, typename a>
				struct selector< std::multiset<data_type,p,a> >
				{
					typedef multiset_info_type<data_type,p,a> type;
				};

				template<typename type, typename p, typename a>
				struct select_member_cast< std::multiset<type,p,a> >
				{
					static bool can_cast(set_info* info)
					{
						return run_member_cast<type>::can_cast(info->get_inner());
					}

					static bool can_cast(member_info* info)
					{
						if(set_info* c = info_cast_visitor<set_info>(info).result())
							return can_cast(c);
						return false;
					}
				};

				expand guard_end

					//
					// Selectors for pair container infos
					//

				expand guard_start(MAP)

				template<typename k, typename t, typename p, typename a>
				struct map_info_type : pair_container_info< std::map<k,t,p,a>, map_info >
				{
					typedef pair_container_info< std::map<k,t,p,a>, map_info > super;

					map_info_type()
					{}

					map_info_type(std::map<k,t,p,a>& indata)
						: 
					super(invalid_field::get_instance(),indata)
					{}

					map_info_type(const data_field& f, std::map<k,t,p,a>& indata)
						: 
					super(f,indata)
					{}

					virtual member_info* new_copy()
					{
						return new map_info_type<k,t,p,a>(*this);
					}

					virtual bool insert_default()
					{
						return false;
					}
				};

				template<typename key, typename value, typename p, typename a>
				struct selector< std::map<key,value,p,a> >
				{
					typedef map_info_type<key,value,p,a> type;
				};

				template<typename key, typename value, typename p, typename a>
				struct select_member_cast< std::map<key,value,p,a> >
				{
					static bool can_cast(map_info* info)
					{
						return	run_member_cast<key>::can_cast(info->get_key())
							&&		run_member_cast<value>::can_cast(info->get_value());
					}

					static bool can_cast(member_info* info)
					{
						if(map_info* c = info_cast_visitor<map_info>(info).result())
							return can_cast(c);
						return false;
					}
				};

				expand guard_end

				expand guard_start(MULTIMAP)

				template<typename k, typename t, typename p, typename a>
				struct multimap_info_type : pair_container_info< std::multimap<k,t,p,a>, multimap_info >
				{
					typedef pair_container_info< std::multimap<k,t,p,a>, multimap_info > super;

					multimap_info_type()
					{}

					multimap_info_type(std::multimap<k,t,p,a>& indata)
						:
						super(invalid_field::get_instance(),indata)
					{}

					multimap_info_type(const data_field& f, std::multimap<k,t,p,a>& indata)
						:
						super(f,indata)
					{}

					virtual member_info* new_copy()
					{
						return new multimap_info_type<k,t,p,a>(*this);
					}

					virtual bool insert_default()
					{
						return false;
					}
				};

				template<typename key, typename value, typename p, typename a>
				struct selector< std::multimap<key,value,p,a> >
				{
					typedef multimap_info_type<key,value,p,a> type;
				};

				template<typename key, typename value, typename p, typename a>
				struct select_member_cast< std::multimap<key,value,p,a> >
				{
					static bool can_cast(multimap_info* info)
					{
						return	run_member_cast<key>  ::can_cast(info->get_key())
						&&		run_member_cast<value>::can_cast(info->get_value());
					}

					static bool can_cast(member_info* info)
					{
						if(multimap_info* c = info_cast_visitor<multimap_info>(info).result())
							return can_cast(c);
						return false;
					}
				};

				expand guard_end

					expand guard_start(PAIR)

					template<typename first, typename second>
				struct selector< std::pair<first,second> >
				{
					typedef pair_info_type< std::pair<first,second> > type;
				};

				template<typename first, typename second>
				struct select_member_cast< std::pair<first,second> >
				{
					static bool can_cast(pair_info* info)
					{
						return	run_member_cast<first> ::can_cast(info->get_first())
						&&		run_member_cast<second>::can_cast(info->get_second());
					}

					static bool can_cast(member_info* info)
					{
						if(pair_info* c = info_cast_visitor<pair_info>(info).result())
							return can_cast(c);
						return false;
					}
				};

				expand guard_end

			}//end of detail
		}
	}

	namespace opcpp
	{
		namespace metadata
		{

		expand guard_meta_start(VECTOR)
			template<typename type, typename a>
			struct has_default_constructor< std::vector<type,a> >
			{
				enum { value = true };
			};
		expand guard_meta_end

		expand guard_meta_start(LIST)
			template<typename type, typename a>
			struct has_default_constructor< std::list<type,a> >
			{
				enum { value = true };
			};
		expand guard_meta_end

		expand guard_meta_start(DEQUE)
			template<typename type, typename a>
			struct has_default_constructor< std::deque<type,a> >
			{
				enum { value = true };
			};
		expand guard_meta_end

		expand guard_meta_start(SET)
			template<typename type, typename p, typename a>
			struct has_default_constructor< std::set<type,p,a> >
			{
				enum { value = true };
			};
		expand guard_meta_end

		expand guard_meta_start(MULTISET)
			template<typename type, typename p, typename a>
			struct has_default_constructor< std::multiset<type,p,a> >
			{
				enum { value = true };
			};
		expand guard_meta_end

		expand guard_meta_start(MAP)
			template<typename key, typename type, typename p, typename a>
			struct has_default_constructor< std::map<key,type,p,a> >
			{
				enum { value = true };
			};
		expand guard_meta_end

		expand guard_meta_start(MULTIMAP)
			template<typename key, typename type, typename p, typename a>
			struct has_default_constructor< std::multimap<key,type,p,a> >
			{
				enum { value = true };
			};
		expand guard_meta_end

		expand guard_meta_start(PAIR)
			template<typename first, typename second>
			struct has_default_constructor< std::pair<first,second> >
			{
				enum { value = true };
			};
		expand guard_meta_end

		}//end of metadata
	}//end of opcpp
};




