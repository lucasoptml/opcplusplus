//****************************************************************
// Copyright © 2008 opGames Inc. - All Rights Reserved
//
// Authors: Kevin Depue & Lucas Ellis
//
// File: opSet.h
// Date: 04/04/2007
//
// Description:
//
// opSet class
//****************************************************************

#ifndef __OPSTL_OPSET__
#define __OPSTL_OPSET__

#include "opstlcommon.h"
#include <set>

namespace opstl
{

//==========================================
// opSetter
//==========================================

// Specialize this if you want to change
// your less than predicate for type T.
template<class T>
struct opSetLessPredicate
{
	static bool Less(const T& t1, const T& t2) 
	{
		return t1 < t2;
	}
};

// This class tells the set classes how to 
// compare keys.
template<class T>
struct opSetter 
{
	// This function returns true if t1 is < t2.
	// t1 and t2 are keys.
	bool operator()(const T& t1, const T& t2) const
	{
		return opSetLessPredicate<T>::Less(t1, t2);
	}
};

//==========================================
// opSet
//==========================================

// stl set wrapper
template<class T>
class opSet
{
public:
	/**** typedefs ****/

	typedef          set<T, opSetter<T>, OPSTL_SET_ALLOCATOR(T) >              set_type;
	typedef typename set_type::size_type                                       size_type;
	typedef typename set_type::key_type                                        key_type;
	typedef typename set_type::value_type                                      value_type;
	typedef typename set_type::iterator									       iterator;
	typedef typename set_type::const_iterator							       const_iterator;
	typedef typename set_type::reverse_iterator							       reverse_iterator;
	typedef typename set_type::const_reverse_iterator					       const_reverse_iterator;
	typedef typename set_type::reference									   reference;
	typedef typename set_type::const_reference							       const_reference;
	typedef          pointer_iterator<iterator, T>							   pointer_iterator;
	typedef          const_pointer_iterator<const_iterator, T>				   const_pointer_iterator;
	typedef          reverse_pointer_iterator<reverse_iterator, T>             reverse_pointer_iterator;
	typedef          const_reverse_pointer_iterator<const_reverse_iterator, T> const_reverse_pointer_iterator;

	/**** construction ****/

	opSet() {}

	opSet(const set_type& inset)
		: s(inset) {}

	opSet(const opSet<T>& inset)
		: s(inset.s) {}

	template<class InputIterator>
	opSet(InputIterator first, InputIterator last) 
		: s(first, last) {}

	/**** destruction ****/

	~opSet()
	{
		s.clear();
	}

	/**** utility ****/

	iterator Begin()
	{
		return s.begin();
	}

	iterator Last()
	{
		if (IsEmpty())
			return End();

		return --End();
	}

	const_iterator Begin() const
	{
		return s.begin();
	}

	const_iterator End() const
	{
		return s.end();
	}

	iterator End()
	{
		return s.end();
	}

	reverse_iterator RBegin()
	{
		return s.rbegin();
	}

	reverse_iterator RLast()
	{
		if (IsEmpty())
			return End();

		return --End();
	}

	reverse_iterator REnd()
	{
		return s.rend();
	}

	int Size() const
	{
		return (int) s.size();
	}

	int Length() const
	{
		return (int) s.size();
	}

	int MaxSize() const
	{
		return (int) s.max_size();
	}

	bool IsEmpty() const
	{
		return s.empty();
	}

	void Swap(opSet<T>& inset)
	{
		s.swap(inset.s);
	}

	iterator Insert(const T& inval)
	{
		pair<iterator, bool> r = s.insert(inval);
		
		return r.first;
	}

	iterator Insert(iterator position, const T& inval)
	{
		return s.insert(position, inval);
	}

	void InsertAfter(iterator position, const T& inval)
	{
		if (position == s.end())
			s.insert(s.end(), inval);
		else
		{
			iterator pos = position;

			++pos;

			s.insert(pos, inval);
		}
	}

	template<class InputIterator>
	void Insert(InputIterator first, InputIterator last)
	{
		s.insert(first, last);
	}

	void Erase(iterator position)
	{
		s.erase(position);
	}

	void Erase(iterator first, iterator last)
	{
		s.erase(first, last);
	}

	bool Erase(const T& t)
	{
		iterator i = Find( t );

		if ( i != s.end() )
		{
			s.erase( i );

			return true;
		}

		return false;
	}

	void Clear()
	{
		s.clear();
	}

	iterator Find(const T& inval)
	{
		return s.find(inval);
	}

	bool Contains(const T& inval)
	{
		return s.find(inval) != s.end();
	}

	int Count(const T& inval)
	{
		return (int) s.count(inval);
	}

	bool Includes(const opSet<T>& inset)
	{
		return includes(s.begin(), s.end(), inset.s.begin(), inset.s.end(), opSetter<T>());
	}

	void Union(const opSet<T>& inset, opSet<T>& result) const
	{
		result.Clear();
		set_union(s.begin(), s.end(), inset.s.begin(), inset.s.end(), result.s.begin(), opSetter<T>());
	}

	void Union(const opSet<T>& inset)
	{
		const_iterator start = inset.Begin();
		const_iterator end = inset.End();
		insert(start,end);
	}

	void Intersection(const opSet<T>& inset, opSet<T>& result)
	{
		result.Clear();
		set_intersection(s.begin(), s.end(), inset.s.begin(), inset.s.end(), result.s.begin(), opSetter<T>());
	}

	void Difference(const opSet<T>& inset, opSet<T>& result)
	{
		result.Clear();
		set_difference(s.begin(), s.end(), inset.s.begin(), inset.s.end(), result.s.begin(), opSetter<T>());
	}

	void SymmetricDifference(const opSet<T>& inset, opSet<T>& result)
	{
		result.Clear();
		set_symmetric_difference(s.begin(), s.end(), inset.s.begin(), inset.s.end(), result.s.begin(), opSetter<T>());
	}

	set_type& GetSet()
	{
		return s;
	}

	const set_type& GetSet() const
	{
		return s;
	}

	void DeleteAll()
	{
		iterator i   = s.begin();
		iterator end = s.end();

		while (i != end)
		{
			delete *i;
			++i;
		}
	}

	/**** operator overloads ****/

	friend bool operator == (const opSet<T>& inset1, const opSet<T>& inset2)
	{
		return inset1.s == inset2.s;
	}

	friend bool operator < (const opSet<T>& inset1, const opSet<T>& inset2)
	{
		return inset1.s < inset2.s;
	}

private:
	set_type s;

public:
	/**** original container functions for stl compatibility ****/

	iterator begin()
	{
		return s.begin();
	}

	iterator end()
	{
		return s.end();
	}

	const_iterator begin() const
	{
		return s.begin();
	}

	const_iterator end() const
	{
		return s.end();
	}

	reverse_iterator rbegin()
	{
		return s.rbegin();
	}

	reverse_iterator rend()
	{
		return s.rend();
	}

	const_reverse_iterator rbegin() const
	{
		return s.rbegin();
	}

	const_reverse_iterator rend() const
	{
		return s.rend();
	}

	size_type size() const
	{
		return s.size();
	}

	size_type max_size() const
	{
		return s.max_size();
	}

	bool empty() const
	{
		return s.empty();
	}

	void swap(const set_type& inset)
	{
		s.swap(inset.s);
	}

	pair<const_iterator,bool> insert(const value_type& inval)
	{
		return s.insert(inval);
	}

	iterator insert(iterator position, const value_type& inval)
	{
		return s.insert(position, inval);
	}

	template<class InputIterator>
	void insert(InputIterator first, InputIterator last)
	{
		s.insert(first, last);
	}

	void erase(iterator position)
	{
		s.erase(position);
	}

	size_type erase(const key_type& inval)
	{
		return s.erase(inval);
	}

	void erase(iterator first, iterator last)
	{
		s.erase(first, last);
	}

	void clear()
	{
		s.clear();
	}

	iterator find(const key_type& inval)
	{
		return s.find(inval);
	}

	size_type count(const key_type& inval)
	{
		return s.count(inval);
	}
};

} // end namespace opstl

#endif // end __OPSTL_OPSET__

