//****************************************************************
// Copyright © 2008 opGames Inc. - All Rights Reserved
//
// Authors: Kevin Depue & Lucas Ellis
//
// File: opArray.h
// Date: 04/17/2007
//
// Description:
//
// Wrapper for standard <vector> class.
//****************************************************************

#ifndef __OPSTL_OPARRAY__
#define __OPSTL_OPARRAY__

#include "opstlcommon.h"
#include <vector>

namespace opstl
{

// stl vector wrapper
template<class T>
class opArray
{
public:
	/**** typedefs ****/

	typedef          vector<T, OPSTL_ARRAY_ALLOCATOR(T) >                      array_type;
	typedef typename array_type::size_type                                     size_type;
	typedef typename array_type::iterator									   iterator;
	typedef typename array_type::const_iterator							       const_iterator;
	typedef typename array_type::reverse_iterator							   reverse_iterator;
	typedef typename array_type::const_reverse_iterator					       const_reverse_iterator;
	typedef typename array_type::reference									   reference;
	typedef typename array_type::const_reference							   const_reference;
	typedef typename array_type::value_type									   value_type;
	typedef          pointer_iterator<iterator, T>							   pointer_iterator;
	typedef          const_pointer_iterator<const_iterator, T>				   const_pointer_iterator;
	typedef          reverse_pointer_iterator<reverse_iterator, T>             reverse_pointer_iterator;
	typedef          const_reverse_pointer_iterator<const_reverse_iterator, T> const_reverse_pointer_iterator;

	/**** construction ****/

	opArray() {}

	opArray(const array_type& inarray)
		: a( inarray ) {}

	opArray(const opArray<T>& inarray)
		: a(inarray.a) {}

	template<class InputIterator>
	opArray(InputIterator first, InputIterator last) 
		: a(first, last) {}

	//NOTE: fix for compiling opC++.
#ifndef OPCPP
	opArray(const vector<T>& inarray)
	{
		a.resize( inarray.size() );
    
		copy( inarray.begin(), inarray.end(), a.begin() );
	}
#endif

	/**** destruction ****/

	~opArray()
	{
		a.clear();
	}

	/**** utility ****/

	iterator Begin()
	{
		return a.begin();
	}

	iterator Last()
	{
		if (IsEmpty())
			return End();

		return --End();
	}

	iterator End()
	{
		return a.end();
	}

	const_iterator Begin() const
	{
		return a.begin();
	}

	const_iterator Last() const
	{
		if (IsEmpty())
			return End();

		return --End();
	}

	const_iterator End() const
	{
		return a.end();
	}

	reverse_iterator RBegin()
	{
		return a.rbegin();
	}

	reverse_iterator RLast() 
	{
		if (IsEmpty())
			return REnd();

		return --REnd();
	}

	reverse_iterator REnd()
	{
		return a.rend();
	}

	const_reverse_iterator RBegin() const
	{
		return a.rbegin();
	}

	const_reverse_iterator RLast() const
	{
		if (IsEmpty())
			return REnd();

		return --REnd();
	}

	const_reverse_iterator REnd() const
	{
		return a.rend();
	}

	int Size() const
	{
		return (int) a.size();
	}

	int Length() const
	{
		return (int) a.size();
	}

	int MemSize() const
	{
		return Size() * sizeof( T );
	}

	int MaxSize() const
	{
		return (int) a.max_size();
	}

	bool IsEmpty() const
	{
		return a.empty();
	}

	int Capacity() const
	{
		return (int) a.capacity();
	}

	void Reserve(size_type num)
	{
		a.reserve(num);
	}

	T& Front()
	{
		return a.front();
	}

	T& Back()
	{
		return a.back();
	}

	const T& Front() const
	{
		return a.front();
	}

	const T& Back() const
	{
		return a.back();
	}

	void PushBack(const T& inval)
	{
		a.push_back(inval);
	}

	T PopBack()
	{
		T outval = a.back();

		a.pop_back();

		return outval;
	}

	void Swap(opArray<T>& inarray)
	{
		a.swap(inarray.a);
	}

	void Insert(iterator position, const T& inval)
	{
		a.insert(position, inval);
	}

	void InsertAfter(iterator position, const T& inval)
	{
		if (position == a.end())
			a.push_back(inval);
		else
		{
			iterator pos = position;

			++pos;

			a.insert(pos, inval);
		}
	}

	template<class InputIterator>
	void Insert(iterator position, InputIterator first, InputIterator last)
	{
		a.insert(position, first, last);
	}

	iterator Erase(iterator position)
	{
		return a.erase(position);
	}

	iterator Erase(int index)
	{
		return a.erase( a.begin() + index );
	}

	iterator Erase(iterator first, iterator last)
	{
		return a.erase(first, last);
	}

	bool Erase(const T& t)
	{
		iterator i = Find( t );

		if ( i != a.end() )
		{
			a.erase( i );

			return true;
		}

		return false;
	}

	void Clear()
	{
		a.clear();
	}

	void Resize(size_type num)
	{
		a.resize(num);
	}

	void Add(size_type num = 1)
	{
		a.resize( a.size() + num );
	}

	void AddZeroed(size_type num = 1)
	{
		a.resize( a.size() + num );
		memset( &a[ a.size() - num ], 0, num * sizeof( T ) );
	}

	void Sort()
	{
		sort(a.begin(), a.end());
	}

	template<class BinaryPredicate>
	void Sort(BinaryPredicate& predicate)
	{
		sort(a.begin(), a.end(), predicate);
	}

	iterator Find(const T& inval)
	{
		return find(a.begin(), a.end(), inval);
	}

	const_iterator Find(const T& inval) const
	{
		return find(a.begin(), a.end(), inval);
	}

	reverse_iterator RFind(const T& inval)
	{
		return find(a.begin(), a.end(), inval);
	}

	const_reverse_iterator RFind(const T& inval) const
	{
		return find(a.begin(), a.end(), inval);
	}

	size_type FindIndex(const T& inval)
	{
		size_type size = a.size();

		for (size_type i = 0; i < size; i++)
		{
			if ( a[ i ] == inval )
				return i;
		}

		return -1;
	}

	bool Contains(const T& inval) const
	{
		return Find(inval) != a.end();
	}

	array_type& GetArray()
	{
		return a;
	}

	const array_type& GetArray() const
	{
		return a;
	}

	void DeleteAll()
	{
		iterator i   = a.begin();
		iterator end = a.end();

		while (i != end)
		{
			delete *i;
			++i;
		}
	}

	/**** operator overloads ****/

	T& operator [] (int i)
	{
		return a[i];
	}

	const T& operator [] (int i) const
	{
		return a[i];
	}

	T& operator [] (std::size_t i)
	{
		return a[i];
	}

	const T& operator [] (std::size_t i) const
	{
		return a[i];
	}

	friend bool operator == (const opArray<T>& inarray1, const opArray<T>& inarray2)
	{
		return inarray1.a == inarray2.a;
	}

	friend bool operator < (const opArray<T>& inarray1, const opArray<T>& inarray2)
	{
		return inarray1.a < inarray2.a;
	}

private:
	array_type a;

public:
	/**** original container functions for stl compatibility ****/

	iterator begin()
	{
		return a.begin();
	}

	iterator end()
	{
		return a.end();
	}

	const_iterator begin() const
	{
		return a.begin();
	}

	const_iterator end() const
	{
		return a.end();
	}

	reverse_iterator rbegin()
	{
		return a.rbegin();
	}

	reverse_iterator rend()
	{
		return a.rend();
	}

	const_reverse_iterator rbegin() const
	{
		return a.rbegin();
	}

	const_reverse_iterator rend() const
	{
		return a.rend();
	}

	size_type size() const
	{
		return a.size();
	}

	size_type max_size() const
	{
		return a.max_size();
	}

	bool empty() const
	{
		return a.empty();
	}

	size_type capacity() const
	{
		return a.capacity();
	}

	void reserve(size_type n)
	{
		a.reserve(n);
	}

	reference front()
	{
		return a.front();
	}

	reference back()
	{
		return a.back();
	}

	const_reference front() const
	{
		return a.front();
	}

	const_reference back() const
	{
		return a.back();
	}

	void push_back(const T& inval)
	{
		a.push_back(inval);
	}

	void pop_back()
	{
		a.pop_back();
	}

	void swap(array_type& inarray)
	{
		a.swap(inarray);
	}

	iterator insert(iterator position, const T& inval)
	{
		return a.insert(position, inval);
	}

	template<class InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last)
	{
		a.insert(position, first, last);
	}

	void insert(iterator position, size_type n, const T& inval)
	{
		a.insert(position, n, inval);
	}

	iterator erase(iterator position)
	{
		return a.erase(position);
	}

	iterator erase(iterator first, iterator last)
	{
		return a.erase(first, last);
	}

	void clear()
	{
		a.clear();
	}

	void resize(size_type num, T t = T())
	{
		a.resize(num, t);
	}
};

} // end namespace opstl

#endif // end __OPSTL_OPARRAY__

