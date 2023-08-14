//****************************************************************
// Copyright © 2008 opGames Inc. - All Rights Reserved
//
// Authors: Kevin Depue & Lucas Ellis
//
// File: opHash.h
// Date: 01/03/2007
//
// Description:
//
// Wrapper for STL <hash_set> and <hash_map> classes.
//****************************************************************

#ifndef __OPSTL_OPHASH__
#define __OPSTL_OPHASH__

#include "opstlcommon.h"

// Include correct hash containers
// based on compiler.
#if defined(OPSTL_GNU_LINUX)
    #include <tr1/unordered_set>
    #include <tr1/unordered_map>
	using namespace std::tr1;

#elif defined(OPSTL_INTEL_LINUX)
    #include <ext/hash_set>
    #include <ext/hash_map>
	using namespace __gnu_cxx;
#else
	#include <unordered_set>
	#include <unordered_map>
	using namespace stdext;
#endif	

namespace opstl
{

//==========================================
// opHasher
//==========================================

// Specialize this if you want to change 
// your hash function for type T.
template<class T>
struct opHashFunction
{
	static size_t Hash(const T& t) 
	{
		return (size_t) t;
	}
};

// Specialize this if you want to change
// your comparison predicate for type T.
// NOTE: This is different depending on your
//       OS!  In Windows, it's <, in GNU Linux,
//       it's ==.
template<class T>
struct opHashCompare
{
	static bool Compare(const T& t1, const T& t2) 
	{
		return t1 < t2;
	}
};

// This class tells the hash classes how to 
// hash a value and compare via '<'.
template<class T>
struct opHasher 
{
	// This function returns the hash value.
	size_t operator()(const T& t) const
	{
		return opHashFunction<T>::Hash(t);
	}

	// This function returns true if t1 is < t2.
	// t1 and t2 are keys.
	bool operator()(const T& t1, const T& t2) const
	{
		return opHashCompare<T>::Compare(t1, t2);
	}
	
	enum
	{	// parameters for hash table
		bucket_size = 4,	// 0 < bucket_size
		min_buckets = 8		// min_buckets = must be a power of 2, and greater than 0
	};
};

//==========================================
// opHashSet
//==========================================

template<class Key>
class opHashSet
{
public:
	/**** typedefs ****/

#ifdef OPSTL_GNU_LINUX
	typedef          unordered_set<Key, opHasher<Key>, opHasher<Key> > hash_set_type;
#else
	typedef          unordered_set<Key, opHasher<Key> >                     hash_set_type;
#endif
	typedef typename hash_set_type::key_type                           key_type;
	typedef typename hash_set_type::value_type                         value_type;
	typedef typename hash_set_type::size_type                          size_type;
	typedef typename hash_set_type::iterator					       iterator;
	typedef typename hash_set_type::const_iterator				       const_iterator;
	typedef typename hash_set_type::reference					       reference;
	typedef typename hash_set_type::const_reference				       const_reference;
	typedef          pointer_iterator<iterator, Key>			       pointer_iterator;
	typedef          const_pointer_iterator<const_iterator, Key>       const_pointer_iterator;

	/**** construction ****/

	opHashSet() {}

	opHashSet(const hash_set_type& inhashset)
		: hs(inhashset) {}

	opHashSet(const opHashSet<Key>& inhashset)
		: hs(inhashset.hs) {}

	template<class InputIterator>
	opHashSet(InputIterator first, InputIterator last) 
		: hs(first, last) {}

	/**** destruction ****/

	~opHashSet() 
	{
		hs.clear();
	}

	/**** utility ****/

	iterator Begin() 
	{
		return hs.begin();
	}

	iterator Last()
	{
		if (IsEmpty())
			return End();

		return --End();
	}

	iterator End() 
	{
		return hs.end();
	}

	int Size() const
	{
		return (int) hs.size();
	}

	int Length() const
	{
		return (int) hs.size();
	}

	int MaxSize() const
	{
		return (int) hs.max_size();
	}

	bool IsEmpty() const
	{
		return hs.empty();
	}

	void Swap(opHashSet<Key>& inhashset)
	{
		hs.swap(inhashset.hs);
	}

	iterator Insert(const Key& key)
	{
		pair<iterator, bool> r = hs.insert(key);

		return r.first;
	}

	template<class InputIterator>
	void Insert(InputIterator first, InputIterator last)
	{
		hs.insert(first, last);
	}

	void Erase(iterator position)
	{
		hs.erase(position);
	}

	void Erase(const Key& key)
	{
		hs.erase(key);
	}

	void Erase(iterator first, iterator last)
	{
		hs.erase(first, last);
	}

	void Clear()
	{
		hs.clear();
	}

	iterator Find(const Key& key) 
	{
		return hs.find(key);
	}

	const_iterator Find(const Key& key) const
	{
		return hs.find(key);
	}

	bool Contains(const Key& key) const
	{
		return hs.find(key) != hs.end();
	}

	int Count(const Key& key) const
	{
		return (int) hs.count(key);
	}

	hash_set_type& GetHashSet()
	{
		return hs;
	}

	const hash_set_type& GetHashSet() const
	{
		return hs;
	}

	void DeleteAll()
	{
		iterator i   = hs.begin();
		iterator end = hs.end();

		while (i != end)
		{
			delete *i;
			++i;
		}
	}

	/**** operator overloads ****/

	friend bool operator == (const opHashSet<Key>& ht1, const opHashSet<Key>& ht2)
	{
		return ht1.hs == ht2.hs;
	}

private:
	hash_set_type hs;

public:
	/**** original container functions for stl compatibility ****/

	iterator begin() const
	{
		return hs.begin();
	}

	iterator end() const
	{
		return hs.end();
	}

	size_type size() const
	{
		return hs.size();
	}

	size_type max_size() const
	{
		return hs.max_size();
	}

	bool empty() const
	{
		return hs.empty();
	}

	void swap(const hash_set_type& inhashset)
	{
		hs.swap(inhashset.hs);
	}

	pair<iterator, bool> insert(const value_type& key)
	{
		return hs.insert(key);
	}

	template<class InputIterator>
	void insert(InputIterator first, InputIterator last)
	{
		hs.insert(first, last);
	}

	void erase(iterator position)
	{
		hs.erase(position);
	}

	size_type erase(const key_type& key)
	{
		return hs.erase(key);
	}

	void erase(iterator first, iterator last)
	{
		hs.erase(first, last);
	}

	void clear()
	{
		hs.clear();
	}

	iterator find(const key_type& key)
	{
		return hs.find(key);
	}

	const_iterator find(const key_type& key) const
	{
		return hs.find(key);
	}

	size_type count(const key_type& key) const
	{
		return hs.count(key);
	}
};

//==========================================
// opHashTable
//==========================================

template<class Key, class Value>
class opHashTable
{
public:
	/**** typedefs ****/

#ifdef OPSTL_GNU_LINUX
	typedef          unordered_map<Key, Value, opHasher<Key>, opHasher<Key> > hash_table_type;
#else
	typedef          unordered_map<Key, Value, opHasher<Key> >                     hash_table_type;
#endif
	typedef typename hash_table_type::key_type                                key_type;
	typedef typename hash_table_type::value_type                              value_type;
	typedef typename hash_table_type::size_type                               size_type;
	typedef typename hash_table_type::iterator					              iterator;
	typedef typename hash_table_type::const_iterator			              const_iterator;
	typedef typename hash_table_type::reference					              reference;
	typedef typename hash_table_type::const_reference                         const_reference;

	/**** construction ****/

	opHashTable() {}

	opHashTable(const hash_table_type& inhashtable)
		: ht(inhashtable) {}

	opHashTable(const opHashTable<Key, Value>& inhashtable)
		: ht(inhashtable.ht) {}

	template<class InputIterator>
	opHashTable(InputIterator first, InputIterator last) 
		: ht(first, last) {}

	/**** destruction ****/

	~opHashTable() 
	{
		ht.clear();
	}

	/**** utility ****/

	iterator Begin()
	{
		return ht.begin();
	}

	iterator Last()
	{
		if (IsEmpty())
			return End();

		return --End();
	}

	iterator End()
	{
		return ht.end();
	}

	const_iterator Begin() const
	{
		return ht.begin();
	}

	const_iterator Last() const
	{
		if (IsEmpty())
			return End();

		return --End();
	}

	const_iterator End() const
	{
		return ht.end();
	}

	int Size() const
	{
		return (int) ht.size();
	}

	int Length() const
	{
		return (int) ht.size();
	}

	int MaxSize() const
	{
		return (int) ht.max_size();
	}

	bool IsEmpty() const
	{
		return ht.empty();
	}
	
	void Swap(opHashTable<Key, Value>& inhashtable)
	{
		ht.swap(inhashtable.ht);
	}

	iterator Insert(const Key& inkey, const Value& inval)
	{
		pair<iterator, bool> r = ht.insert(pair<Key, Value>(inkey, inval));

		return r.first;
	}

	template<class InputIterator>
	void Insert(InputIterator first, InputIterator last)
	{
		ht.insert(first, last);
	}

	void Erase(iterator position)
	{
		ht.erase(position);
	}

	void Erase(const Key& inkey)
	{
		ht.erase(inkey);
	}

	void Erase(iterator first, iterator last)
	{
		ht.erase(first, last);
	}

	void Clear()
	{
		ht.clear();
	}

	iterator Find(const Key& inkey) 
	{
		return ht.find(inkey);
	}

	const_iterator Find(const Key& inkey) const
	{
		return ht.find(inkey);
	}

	bool Find(const Key& inkey, Value& inval) const
	{
		const_iterator iter   = ht.find(inkey);
		bool           result = (iter != ht.end());

		if (result)
			inval = iter->second;

		return result;
	}

	bool Contains(const Key& inkey) const
	{
		return ht.find(inkey) != ht.end();
	}

	int Count(const Key& inkey) const
	{
		return (int) ht.count(inkey);
	}

	hash_table_type& GetHashTable()
	{
		return ht;
	}

	const hash_table_type& GetHashTable() const
	{
		return ht;
	}

	void DeleteAllKeys()
	{
		iterator i   = ht.begin();
		iterator end = ht.end();

		while (i != end)
		{
			delete i->first;
			++i;
		}
	}

	void DeleteAllValues()
	{
		iterator i   = ht.begin();
		iterator end = ht.end();

		while (i != end)
		{
			delete i->second;
			++i;
		}
	}

	/**** operator overloads ****/

	Value& operator [] (const Key& inkey)
	{
		return ht[inkey];
	}

	friend bool operator == (const opHashTable<Key, Value>& ht1, const opHashTable<Key, Value>& ht2)
	{
		return ht1.ht == ht2.ht;
	}

private:
	hash_table_type ht;

public:
	/**** original container functions for stl compatibility ****/

	iterator begin()
	{
		return ht.begin();
	}

	iterator end()
	{
		return ht.end();
	}

	const_iterator begin() const
	{
		return ht.begin();
	}

	const_iterator end() const
	{
		return ht.end();
	}

	size_type size() const
	{
		return ht.size();
	}

	size_type max_size() const
	{
		return ht.max_size();
	}

	bool empty() const
	{
		return ht.empty();
	}

	void swap(const hash_table_type& inhashtable)
	{
		ht.swap(inhashtable.ht);
	}

	pair<iterator, bool> insert(const value_type& inval)
	{
		return ht.insert(inval);
	}

	template<class InputIterator>
	void insert(InputIterator first, InputIterator last)
	{
		ht.insert(first, last);
	}

	void erase(iterator position)
	{
		ht.erase(position);
	}

	size_type erase(const key_type& inkey)
	{
		ht.erase(inkey);
	}

	void erase(iterator first, iterator last)
	{
		ht.erase(first, last);
	}

	void clear()
	{
		ht.clear();
	}

	iterator find(const key_type& inkey) 
	{
		return ht.find(inkey);
	}

	const_iterator find(const key_type& inkey) const
	{
		return ht.find(inkey);
	}

	size_type count(const key_type& inkey) const
	{
		return ht.count(inkey);
	}
};

} // end namespace opstl

#endif // end __OPSTL_OPHASH__

