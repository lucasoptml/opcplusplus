//****************************************************************
// Copyright © 2008 opGames Inc. - All Rights Reserved
//
// Authors: Kevin Depue & Lucas Ellis
//
// File: opMap.h
// Date: 11/26/2006
//
// Description:
//
// Wrapper for STL <map> class.
//****************************************************************

#ifndef __OPSTL_OPMAP__
#define __OPSTL_OPMAP__

#include "opstlcommon.h"
#include <map>

namespace opstl
{

//==========================================
// opMapper
//==========================================

// Specialize this if you want to change
// your less than predicate for type T.
template<class T>
struct opMapLessPredicate
{
	static bool Less(const T& t1, const T& t2) 
	{
		return t1 < t2;
	}
};

// This class tells the map classes how to 
// compare keys.
template<class T>
struct opMapper 
{
	// This function returns true if t1 is < t2.
	// t1 and t2 are keys.
	bool operator()(const T& t1, const T& t2) const
	{
		return opMapLessPredicate<T>::Less(t1, t2);
	}
};

//==========================================
// opMap
//==========================================

// stl map wrapper
template<class Key, class Value>
class opMap
{
public:
	/**** typedefs ****/
	typedef std::pair<const Key, Value> pair_type;
	typedef OPSTL_MAP_ALLOCATOR( pair_type )		  alloc_type;
	typedef          map<Key, Value, opMapper<Key>, alloc_type >  map_type;
	typedef typename map_type::key_type               key_type;
	typedef typename map_type::value_type             value_type;
	typedef typename map_type::size_type              size_type;
	typedef typename map_type::iterator				  iterator;
	typedef typename map_type::const_iterator		  const_iterator;
	typedef typename map_type::reverse_iterator		  reverse_iterator;
	typedef typename map_type::const_reverse_iterator const_reverse_iterator;
	typedef typename map_type::reference			  reference;
	typedef typename map_type::const_reference	      const_reference;

	/**** construction ****/

	opMap() {}

	opMap(const map_type& inmap)
		: m(inmap) {}

	opMap(const opMap<Key, Value>& inmap)
		: m(inmap.m) {}

	template<class InputIterator>
	opMap(InputIterator first, InputIterator last) 
		: m(first, last) {}

	/**** destruction ****/

	~opMap()
	{
		m.clear();
	}

	/**** utility ****/

	iterator Begin()
	{
		return m.begin();
	}

	iterator End()
	{
		return m.end();
	}

	const_iterator Begin() const
	{
		return m.begin();
	}

	const_iterator End() const
	{
		return m.end();
	}

	reverse_iterator RBegin()
	{
		return m.rbegin();
	}

	reverse_iterator REnd()
	{
		return m.rend();
	}

	const_reverse_iterator RBegin() const
	{
		return m.rbegin();
	}

	const_reverse_iterator REnd() const
	{
		return m.rend();
	}

	int Size() const
	{
		return (int) m.size();
	}

	int MaxSize() const
	{
		return (int) m.max_size();
	}

	bool IsEmpty() const
	{
		return m.empty();
	}

	void Swap(opMap<Key, Value>& inmap)
	{
		m.swap(inmap.m);
	}

	iterator Insert(const Key& inkey, const Value& inval)
	{
		pair<iterator, bool> r = m.insert(pair<Key, Value>(inkey, inval));

		return r.first;
	}

	iterator Insert(iterator position, const Key& inkey, const Value& inval)
	{
		return m.insert(position, pair<Key, Value>(inkey, inval));
	}

	template<class InputIterator>
	void InsertRange(InputIterator first, InputIterator last)
	{
		m.insert(first, last);
	}

	void Erase(iterator position)
	{
		m.erase(position);
	}

	void Erase(const Key& inkey)
	{
		m.erase(inkey);
	}

	void Erase(iterator first, iterator last)
	{
		m.erase(first, last);
	}

	void Clear()
	{
		m.clear();
	}

	iterator Find(const Key& inkey)
	{
		return m.find(inkey);
	}

	const_iterator Find(const Key& inkey) const
	{
		return m.find(inkey);
	}

	bool Find(const Key& inkey, Value& inval) const
	{
		const_iterator iter   = m.find(inkey);
		bool           result = (iter != m.end());

		if (result)
			inval = iter->second;

		return result;
	}

	bool Contains(const Key& inkey) const
	{
		return m.find(inkey) != m.end();
	}

	int Count(const Key& inkey) const
	{
		return (int) m.count(inkey);
	}

	map_type& GetMap()
	{
		return m;
	}

	const map_type& GetMap() const
	{
		return m;
	}

	void DeleteAll()
	{
		iterator i   = m.begin();
		iterator end = m.end();

		while (i != end)
		{
			delete i->first;
			delete i->second;
			++i;
		}
	}

	void DeleteAllKeys()
	{
		iterator i   = m.begin();
		iterator end = m.end();

		while (i != end)
		{
			delete i->first;
			++i;
		}
	}

	void DeleteAllValues()
	{
		iterator i   = m.begin();
		iterator end = m.end();

		while (i != end)
		{
			delete i->second;
			++i;
		}
	}

	/**** operator overloads ****/

	Value& operator [] (const Key& inkey)
	{
		return m[inkey];
	}

	friend bool operator == (const opMap<Key, Value>& m1, const opMap<Key, Value>& m2)
	{
		return m1.m == m2.m;
	}

	friend bool operator < (const opMap<Key, Value>& m1, const opMap<Key, Value>& m2)
	{
		return m1.m < m2.m;
	}

private:
	map_type m;

public:
	/**** original container functions for stl compatibility ****/

	iterator begin()
	{
		return m.begin();
	}

	iterator end()
	{
		return m.end();
	}

	const_iterator begin() const
	{
		return m.begin();
	}

	const_iterator end() const
	{
		return m.end();
	}

	reverse_iterator rbegin()
	{
		return m.rbegin();
	}

	reverse_iterator rend()
	{
		return m.rend();
	}

	const_reverse_iterator rbegin() const
	{
		return m.rbegin();
	}

	const_reverse_iterator rend() const
	{
		return m.rend();
	}

	size_type size() const
	{
		return m.size();
	}

	size_type max_size() const
	{
		return m.max_size();
	}

	bool empty() const
	{
		return m.empty();
	}

	void swap(map_type& inmap)
	{
		m.swap(inmap.m);
	}

	pair<iterator, bool> insert(const value_type& inval)
	{
		return m.insert(inval);
	}

	iterator insert(iterator position, const value_type& inval)
	{
		return m.insert(position, inval);
	}

	template<class InputIterator>
	void insert(InputIterator first, InputIterator last)
	{
		m.insert(first, last);
	}

	void erase(iterator position)
	{
		m.erase(position);
	}

	size_type erase(const key_type& inkey)
	{
		return m.erase(inkey);
	}

	void erase(iterator first, iterator last)
	{
		m.erase(first, last);
	}

	void clear()
	{
		m.clear();
	}

	iterator find(const key_type& inkey)
	{
		return m.find(inkey);
	}

	const_iterator find(const key_type& inkey) const
	{
		return m.find(inkey);
	}

	size_type count(const key_type& inkey) const
	{
		return m.count(inkey);
	}
};

//==========================================
// opMultiMap
//==========================================

// stl map wrapper
template<class Key, class Value>
class opMultiMap
{
public:
	/**** typedefs ****/

	typedef          multimap<Key, Value, opMapper<Key> >  map_type;
	typedef typename map_type::key_type                    key_type;
	typedef typename map_type::value_type                  value_type;
	typedef typename map_type::size_type                   size_type;
	typedef typename map_type::iterator				       iterator;
	typedef typename map_type::const_iterator		       const_iterator;
	typedef typename map_type::reverse_iterator		       reverse_iterator;
	typedef typename map_type::const_reverse_iterator      const_reverse_iterator;
	typedef typename map_type::reference			       reference;
	typedef typename map_type::const_reference	           const_reference;

	/**** construction ****/

	opMultiMap() {}

	opMultiMap(const map_type& inmap)
		: m(inmap) {}

	opMultiMap(const opMultiMap<Key, Value>& inmap)
		: m(inmap.m) {}

	template<class InputIterator>
	opMultiMap(InputIterator first, InputIterator last) 
		: m(first, last) {}

	/**** destruction ****/

	~opMultiMap()
	{
		m.clear();
	}

	/**** utility ****/

	iterator Begin()
	{
		return m.begin();
	}

	iterator End()
	{
		return m.end();
	}

	const_iterator Begin() const
	{
		return m.begin();
	}

	const_iterator End() const
	{
		return m.end();
	}

	reverse_iterator RBegin()
	{
		return m.rbegin();
	}

	reverse_iterator REnd()
	{
		return m.rend();
	}

	const_reverse_iterator RBegin() const
	{
		return m.rbegin();
	}

	const_reverse_iterator REnd() const
	{
		return m.rend();
	}

	int Size() const
	{
		return (int) m.size();
	}

	int MaxSize() const
	{
		return (int) m.max_size();
	}

	bool IsEmpty() const
	{
		return m.empty();
	}

	void Swap(opMap<Key, Value>& inmap)
	{
		m.swap(inmap.m);
	}

	iterator Insert(const Key& inkey, const Value& inval)
	{
		return m.insert(pair<Key, Value>(inkey, inval));
	}

	iterator Insert(iterator position, const Key& inkey, const Value& inval)
	{
		return m.insert(position, pair<Key, Value>(inkey, inval));
	}

	template<class InputIterator>
	void InsertRange(InputIterator first, InputIterator last)
	{
		m.insert(first, last);
	}

	void Erase(iterator position)
	{
		m.erase(position);
	}

	void Erase(const Key& inkey)
	{
		m.erase(inkey);
	}

	void Erase(iterator first, iterator last)
	{
		m.erase(first, last);
	}

	void Clear()
	{
		m.clear();
	}

	iterator Find(const Key& inkey)
	{
		return m.find(inkey);
	}

	const_iterator Find(const Key& inkey) const
	{
		return m.find(inkey);
	}

	bool Contains(const Key& inkey) const
	{
		return m.find(inkey) != m.end();
	}

	int Count(const Key& inkey) const
	{
		return (int) m.count(inkey);
	}

	map_type& GetMultiMap()
	{
		return m;
	}

	const map_type& GetMultiMap() const
	{
		return m;
	}

	void DeleteAll()
	{
		iterator i   = m.begin();
		iterator end = m.end();

		while (i != end)
		{
			delete i->first;
			delete i->second;
			++i;
		}
	}

	void DeleteAllKeys()
	{
		iterator i   = m.begin();
		iterator end = m.end();

		while (i != end)
		{
			delete i->first;
			++i;
		}
	}

	void DeleteAllValues()
	{
		iterator i   = m.begin();
		iterator end = m.end();

		while (i != end)
		{
			delete i->second;
			++i;
		}
	}

	/**** operator overloads ****/

	friend bool operator == (const opMultiMap<Key, Value>& m1, const opMultiMap<Key, Value>& m2)
	{
		return m1.m == m2.m;
	}

	friend bool operator < (const opMultiMap<Key, Value>& m1, const opMultiMap<Key, Value>& m2)
	{
		return m1.m < m2.m;
	}

private:
	map_type m;

public:
	/**** original container functions for stl compatibility ****/

	iterator begin()
	{
		return m.begin();
	}

	iterator end()
	{
		return m.end();
	}

	const_iterator begin() const
	{
		return m.begin();
	}

	const_iterator end() const
	{
		return m.end();
	}

	reverse_iterator rbegin()
	{
		return m.rbegin();
	}

	reverse_iterator rend()
	{
		return m.rend();
	}

	const_reverse_iterator rbegin() const
	{
		return m.rbegin();
	}

	const_reverse_iterator rend() const
	{
		return m.rend();
	}

	size_type size() const
	{
		return m.size();
	}

	size_type max_size() const
	{
		return m.max_size();
	}

	bool empty() const
	{
		return m.empty();
	}

	void swap(map_type& inmap)
	{
		m.swap(inmap.m);
	}

	iterator insert(const value_type& inval)
	{
		return m.insert(inval);
	}

	iterator insert(iterator position, const value_type& inval)
	{
		return m.insert(position, inval);
	}

	template<class InputIterator>
	void insert(InputIterator first, InputIterator last)
	{
		m.insert(first, last);
	}

	void erase(iterator position)
	{
		m.erase(position);
	}

	size_type erase(const key_type& inkey)
	{
		return m.erase(inkey);
	}

	void erase(iterator first, iterator last)
	{
		m.erase(first, last);
	}

	void clear()
	{
		m.clear();
	}

	iterator find(const key_type& inkey)
	{
		return m.find(inkey);
	}

	const_iterator find(const key_type& inkey) const
	{
		return m.find(inkey);
	}

	size_type count(const key_type& inkey) const
	{
		return m.count(inkey);
	}
};

} // end namespace opstl

#endif // end __OPSTL_OPMAP__

