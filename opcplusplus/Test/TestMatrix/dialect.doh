// The Project Dialect

// opinclude the default opC++ dialect:
opinclude "opc++dialect.doh"

opinclude "visitors/xml_archiver.doh"
opinclude "visitors/text_archiver.doh"
opinclude "visitors/binary_archiver.doh"

// Specify custom dialect settings below:

#include <vector>
#include <list>
#include <deque>
#include <string>
#include <set>

category opclass
{
	location body
	{
		datamap ctor
		{
			is const;
			is not data_array;
		}

		datamap equality
		{
			is not data_reference;
		}

		datamap less
		{
			is not data_reference;
		}

		note init;
	}

	location footer
	{
		note init_start;

		datamap init
		{
			is not data_reference;
			is not const;
		}

		note init_end;
	}
};

template<class T>
struct int_initializer
{
	static void init(T& data)
	{
		data = rand();
	}
};

template<class T>
struct int_initializer<const T>
{
	static void init(const T& data)
	{
		const_cast<T&>(data) = rand();
	}
};

template<class T>
struct struct_initializer
{
	static void init(T& object)
	{
		object.Initialize();
	}
};

template<class T>
struct enum_initializer
{
	static void init(T& object)
	{
		object = T::max;
	}
};

template<class T>
struct no_initializer
{
	static void init(T& data)
	{}
};

using opcpp::metafunction::IF;
using opcpp::metafunction::remove_cv;
using opcpp::metadata::has_default_constructor;
using opcpp::metadata::is_opstruct;
using opcpp::metadata::is_opclass;
using opcpp::metadata::is_openum;

template<class T>
struct initializer
{
	typedef 
		typename
		IF< 
			opcpp::metadata::is_bytes< typename remove_cv<T>::type >::value,
			int_initializer<T>,
			typename
			IF<
				is_opstruct<T>::value || is_opclass<T>::value,
				struct_initializer<T>,
				typename
				IF<
					opcpp::metadata::is_openum<T>::value,
					enum_initializer<T>,
					no_initializer<T>
				>::RET
			>::RET
		>::RET
	selection;

	static void init(T& data)
	{
		selection::init(data);
	}
};

template<>
struct initializer<bool>
{
	static void init(bool& data)
	{
		data = true;
	}
};

template<>
struct initializer<volatile bool>
{
	static void init(volatile bool& data)
	{
		data = true;
	}
};

template<>
struct initializer< std::vector< bool > >
{
	static void init( std::vector<bool>& c )
	{
	}
};

template<class f ,class s>
struct initializer< std::pair<f,s> >
{
	static void init( std::pair<f,s>& p )
	{
		initializer<f>::init(p.first);
		initializer<s>::init(p.second);
	}
};

template<class T>
struct initializer< std::vector< T > >
{
	static void init( std::vector<T>& c )
	{
		for(int i = 0; i < 3; i++)
		{
			typename remove_cv<T>::type temp;
			initializer<T>::init(temp);
			c.push_back(temp);
		}
	}
};

template<class T>
struct initializer< std::list< T > >
{
	static void init( std::list<T>& c )
	{
		for(int i = 0; i < 2; i++)
		{
			typename remove_cv<T>::type temp;
			initializer<T>::init(temp);
			c.push_back(temp);
		}
	}
};

template<class T>
struct initializer< std::deque< T > >
{
	static void init( std::deque<T>& c )
	{
		for(int i = 0; i < 2; i++)
		{
			typename remove_cv<T>::type temp;
			initializer<T>::init(temp);
			c.push_back(temp);
		}
	}
};

template<class T>
struct initializer< std::set< T > >
{
	static void init( std::set<T>& c )
	{
		for(int i = 0; i < 1; i++)
		{
			typename remove_cv<T>::type temp;
			initializer<T>::init(temp);
			c.insert(temp);
		}
	}
};

template<class T>
struct initializer< std::multiset< T > >
{
	static void init( std::multiset<T>& c )
	{
		for(int i = 0; i < 2; i++)
		{
			typename remove_cv<T>::type temp;
			initializer<T>::init(temp);
			c.insert(temp);
		}
	}
};

template<class K, class T>
struct initializer< std::map< K,  T > >
{
	static void init( std::map< K, T >& c )
	{
		for(int i = 0; i < 2; i++)
		{
			typename remove_cv<K>::type tempkey;
			typename remove_cv<T>::type tempvalue;

			initializer<K>::init(tempkey);
			initializer<T>::init(tempvalue);

			c.insert( std::pair<K,T>(tempkey,tempvalue) );
		}
	}
};

template<class K, class T>
struct initializer< std::multimap< K,  T > >
{
	static void init( std::multimap< K, T >& c )
	{
		for(int i = 0; i < 2; i++)
		{
			typename remove_cv<K>::type tempkey;
			typename remove_cv<T>::type tempvalue;

			initializer<K>::init(tempkey);
			initializer<T>::init(tempvalue);

			c.insert( std::pair<K,T>(tempkey,tempvalue) );
		}
	}
};

template<>
struct initializer<std::string>
{
	static void init(std::string& data)
	{
		data = "test";
	}
};



template<>
struct initializer<float>
{
	static void init(float& data)
	{
		data = 1234.5f;
	}
};

template<>
struct initializer<volatile float>
{
	static void init(volatile float& data)
	{
		data = 1234.5f;
	}
};

template<>
struct initializer<double>
{
	static void init(double& data)
	{
		data = 6789.10;
	}
};

template<>
struct initializer<volatile double>
{
	static void init(volatile double& data)
	{
		data = 6789.10;
	}
};

template<class T, int n>
struct initializer<T[n]>
{
	static void init(T data[n])
	{
		for(int i = 0; i < n; i++)
			initializer<T>::init(data[i]);
	}
};

template<class T, int n, int m>
struct initializer<T[n][m]>
{
	static void init(T data[n][m])
	{
		for(int j = 0; j < m; j++)	
			for(int i = 0; i < n; i++)
				initializer<T>::init(data[i][j]);
	}
};

template<class T>
struct new_initializer
{
	static void init(T*& data)
	{
		data = new T();
		initializer<T>::init(*data);
	}
};

template<class T>
struct pointer_initializer
{
	static void init(T*& data)
	{
		data = 0;
	}
};

template<class T>
struct initializer<T*>
{
	typedef 
		typename
		IF< has_default_constructor<T>::value,
			new_initializer<T>,
			pointer_initializer<T>
		>::RET
		selection;

	static void init(T*& data)
	{
		selection::init(data);
	}
};

template<class T>
struct const_initializer
{
	static T get()
	{
		T temp;
		initializer<T>::init(temp);
		return temp;
	}
};

template<class T>
struct const_initializer<const T>
{
	static T get()
	{
		T temp;
		initializer<T>::init(temp);
		return temp;
	}
};

template<class T>
struct default_comparison
{
	static bool compare(const T& a, const T& b)
	{
		return a == b;
	}
};

template<class T>
struct is_container
{
	enum { value = false };
};

template<class T>
struct is_container< std::vector< T > >
{
	enum { value = true };
};

template<class T>
struct is_container< std::list< T > >
{
	enum { value = true };
};

template<class T>
struct is_container< std::deque< T > >
{
	enum { value = true };
};

template<class T>
struct is_container< std::set< T > >
{
	enum { value = true };
};

template<class T>
struct is_container< std::multiset< T > >
{
	enum { value = true };
};

template<class T, class K>
struct is_container< std::map< T, K > >
{
	enum { value = true };
};

template<class T, class K>
struct is_container< std::multimap< T, K > >
{
	enum { value = true };
};

template<class T>
struct container_comparison
{
	static bool compare(const T& a, const T& b);
// 	{
// 		typename T::const_iterator bit = b.begin();
// 		typename T::const_iterator bend = b.end();
// 
// 		typename T::const_iterator ait = a.begin();
// 		typename T::const_iterator aend = a.end();
// 
// 		while(ait != aend && bit != bend)
// 		{
// 			if( !comparison< typename T::value_type >::compare(*ait,*bit) )
// 				return false;
// 
// 			++ait;
// 			++bit;
// 		}
// 
// 		return true;
// 	}
};

template<class T>
struct comparison
{
	typedef
		typename
		IF< is_container<T>::value,
		container_comparison<T>,
		default_comparison<T>
		>::RET
		selection;

	static bool compare(const T& a, const T& b)
	{
		return selection::compare(a,b);
	}
};

template<class T>
inline bool container_comparison<T>::compare(const T& a, const T& b)
{
	typename T::const_iterator bit = b.begin();
	typename T::const_iterator bend = b.end();

	typename T::const_iterator ait = a.begin();
	typename T::const_iterator aend = a.end();

	while(1)
	{
		if(ait == aend && bit == bend)
			break;
		
		//breaks if 
		if(bit == bend)
			return false;

		if(ait == aend)
			return false;

		if( !comparison< typename T::value_type >::compare(*ait,*bit) )
			return false;

		++ait;
		++bit;
	}

	return true;
}

//deep compare pairs
template<class A, class B>
struct comparison< std::pair<A,B> >
{
	static bool compare( const std::pair<A,B>& a, const std::pair<A,B>& b)
	{
		if(!comparison<A>::compare(a.first,b.first))
			return false;
		if(!comparison<B>::compare(a.second,b.second))
			return false;
		return true;
	}
};

//want deep comparison w/ pointers
template<class T>
struct comparison<T*>
{
	static bool compare( T* const & a,  T* const & b)
	{
		if(!a || !b)
			return a == b;
		return comparison<T>::compare(*a,*b);
	}
};

template<>
struct comparison<void*>
{
	static bool compare( void* const & a, void* const & b)
	{
		return true;//a == b;
	}
};

template<>
struct comparison<const void*>
{
	static bool compare( const void* const & a, const void* const & b)
	{
		return true;//a == b;
	}
};

template<class T>
struct comparison< volatile T >
{
	static bool compare(const volatile T& a, const volatile T& b)
	{
		T& newa = const_cast<T&>(a);
		T& newb = const_cast<T&>(b);
		return comparison<T>::compare(newa,newb);
	}
};

template<class T>
struct comparison< const volatile T >
{
	static bool compare(const volatile T& a, const volatile T& b)
	{
		T& newa = const_cast<T&>(a);
		T& newb = const_cast<T&>(b);
		return comparison<T>::compare(newa,newb);
	}
};

template<class T, int n>
struct comparison<T[n]>
{
	static bool compare(const T a[n], const T b[n])
	{
		for(int i = 0; i < n; i++)
		{
			if( !comparison<T>::compare(a[i],b[i]) )
				return false;
		}
		return true;
	}
};

template<class T, int n, int m>
struct comparison<T[n][m]>
{
	static bool compare(const T a[n][m], const T b[n][m])
	{
		for(int j = 0; j < m; j++)
		{
			for(int i = 0; i < n; i++)
			{
				if( !comparison<T>::compare(a[i][j],b[i][j]) )
					return false;
			}
		}
		return true;
	}
};

template<class T>
struct lesser
{
	static bool lessthan(const T& a, const T& b)
	{
		return a < b;
	}
};

template<class T>
struct lesser< volatile T >
{
	static bool lessthan(const volatile T& a, const volatile T& b)
	{
		return const_cast<T&>(a) < const_cast<T&>(b);
	}
};

template<class T, int n>
struct lesser<T[n]>
{
	static bool lessthan(const T a[n], const T b[n])
	{
		for(int i = 0; i < n; i++)
		{
			if( !lesser<T>::lessthan(a[i],b[i]) )
				return false;
		}
		return true;
	}
};

template<class T, int n, int m>
struct lesser<T[n][m]>
{
	static bool lessthan(const T a[n][m], const T b[n][m])
	{
		for(int j = 0; j < m; j++)
		{
			for(int i = 0; i < n; i++)
			{
				if( !lesser<T>::lessthan(a[i][j],b[i][j]) )
					return false;
			}
		}
		return true;
	}
};

note opclass::body::ctor::start(class_name)
{
public:
	class_name() :
}

note opclass::body::ctor::mapping(data_full_type,member_name)
{
	member_name(const_initializer<data_full_type>::get()),
}

note opclass::body::ctor::end(class_name)
{
	Super() {}
}


note opclass::body::init()
{
public:
	void Initialize();
}

note opclass::footer::init_start(class_name)
{

	inline void class_name::Initialize()
	+{}
}

note opclass::footer::init::mapping(member_name,data_full_type)
{
	{
		typedef remove_cv< data_full_type >::type mytype;
		mytype& data = const_cast<mytype&>(member_name);
		initializer< mytype >::init(data);
	}
}

note opclass::footer::init_end()
{
	-{}
}

note opclass::body::equality::start(class_name)
{
	friend bool operator == (const class_name& a, const class_name& b)
	+{}
}

note opclass::body::equality::mapping(data_full_type,member_name)
{
	if( !comparison< data_full_type >::compare(a.member_name, b.member_name) )
	{
		comparison< data_full_type >::compare(a.member_name, b.member_name);
		return false;
	}
}

note opclass::body::equality::end(class_name)
{
		return true;
	-{}

	friend bool operator != (const class_name& a, const class_name& b)
	{
		return !(a == b);
	}
}

note opclass::body::less::start(class_name)
{
public:
	bool operator < (const class_name& other) const
	+{}
}

note opclass::body::less::mapping(data_full_type,member_name)
{
		if( lesser< data_full_type >::lessthan(member_name,other.member_name) )
			return true;
}

note opclass::body::less::end(class_name)
{
		return false;
	-{}
}

category opstruct
{
	location body
	{
		datamap equality;
		datamap less;

		note init;
	}

	location footer
	{
		note equality;
	}

	location footer
	{
		note init_start;

		hidden datamap init;

		note init_end;
	}
};

note opstruct::body::equality::start(class_name)
{
	friend bool operator == (const class_name& a, const class_name& b)
	+{}
}

note opstruct::body::equality::mapping(member_name)
{
	if(a.member_name != b.member_name)
		return false;
}

note opstruct::body::equality::end(class_name)
{
		return true;
	-{}

	friend bool operator != (const class_name& a, const class_name& b)
	{
		return !(a == b);
	}


}

note opstruct::footer::equality(class_name)
{

}

note opstruct::body::less::start(class_name)
{
public:
	bool operator < (const class_name& other) const
	+{}
}

note opstruct::body::less::mapping(data_full_type,member_name)
{
		if( lesser< data_full_type >::lessthan(member_name,other.member_name) )
			return true;
}

note opstruct::body::less::end(class_name)
{
		return false;
	-{}
}


note opstruct::body::init()
{
public:
	void Initialize();
}

note opstruct::footer::init_start(class_name)
{

	inline void class_name::Initialize()
	+{}
}

note opstruct::footer::init::mapping(member_name,data_full_type)
{
		{
			typedef remove_cv< data_full_type >::type mytype;

			initializer< mytype >::init(const_cast<mytype&>(member_name));
		}
}

note opstruct::footer::init_end()
{
	-{}
}

enumeration openum
{
	location body
	{
		note less;
	}
};

note openum::body::less(enum_type)
{
public:
	bool operator < (const enum_type& other) const
	{
		return data < other.data;
	}
}