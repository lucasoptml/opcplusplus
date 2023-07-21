//****************************************************************
// Copyright © 2008 opGames Inc. - All Rights Reserved
//
// Authors: Kevin Depue & Lucas Ellis
//
// File: opDeque.h
// Date: 01/09/2007
//
// Description:
//
// Wrapper for standard <deque> class.
//****************************************************************

#ifndef __OPSTL_OPDEQUE__
#define __OPSTL_OPDEQUE__

#include "opstlcommon.h"
#include <deque>

namespace opstl
{

// stl deque wrapper
template<class T>
class opDeque 
{
public:
	/**** typedefs ****/

	typedef          deque<T, OPSTL_DEQUE_ALLOCATOR(T) >                       deque_type;
	typedef typename deque_type::size_type                                     size_type;
	typedef typename deque_type::iterator									   iterator;
	typedef typename deque_type::const_iterator							       const_iterator;
	typedef typename deque_type::reverse_iterator							   reverse_iterator;
	typedef typename deque_type::const_reverse_iterator					       const_reverse_iterator;
	typedef typename deque_type::reference									   reference;
	typedef typename deque_type::const_reference							   const_reference;
	typedef typename deque_type::value_type									   value_type;
	typedef          pointer_iterator<iterator, T>							   pointer_iterator;
	typedef          const_pointer_iterator<const_iterator, T>				   const_pointer_iterator;
	typedef          reverse_pointer_iterator<reverse_iterator, T>             reverse_pointer_iterator;
	typedef          const_reverse_pointer_iterator<const_reverse_iterator, T> const_reverse_pointer_iterator;

	/**** construction ****/

	opDeque() {}

	opDeque(const deque_type& indeque)
		: d(indeque) {}

	opDeque(const opDeque<T>& indeque)
		: d(indeque.d) {}

	template<class InputIterator>
	opDeque(InputIterator first, InputIterator last) 
		: d(first, last) {}

	/**** destruction ****/

	~opDeque()
	{
		d.clear();
	}

	/**** utility ****/

	iterator Begin()
	{
		return d.begin();
	}

	iterator Last()
	{
		if (IsEmpty())
			return End();

		return --End();
	}

	iterator End()
	{
		return d.end();
	}

	const_iterator Begin() const
	{
		return d.begin();
	}

	const_iterator Last() const
	{
		if (IsEmpty())
			return End();

		return --End();
	}

	const_iterator End() const
	{
		return d.end();
	}

	reverse_iterator RBegin()
	{
		return d.rbegin();
	}

	reverse_iterator RLast() 
	{
		if (IsEmpty())
			return REnd();

		return --REnd();
	}

	reverse_iterator REnd()
	{
		return d.rend();
	}

	const_reverse_iterator RBegin() const
	{
		return d.rbegin();
	}

	const_reverse_iterator RLast() const
	{
		if (IsEmpty())
			return REnd();

		return --REnd();
	}

	const_reverse_iterator REnd() const
	{
		return d.rend();
	}

	int Size() const
	{
		return (int) d.size();
	}

	int Length() const
	{
		return (int) d.size();
	}

	int MaxSize() const
	{
		return (int) d.max_size();
	}

	bool IsEmpty() const
	{
		return d.empty();
	}

	T& Front()
	{
		return d.front();
	}

	T& Back()
	{
		return d.back();
	}

	const T& Front() const
	{
		return d.front();
	}

	const T& Back() const
	{
		return d.back();
	}

	void PushFront(const T& inval)
	{
		d.push_front(inval);
	}

	void PushBack(const T& inval)
	{
		d.push_back(inval);
	}

	T PopFront()
	{
		T outval = d.front();

		d.pop_front();

		return outval;
	}

	T PopBack()
	{
		T outval = d.back();

		d.pop_back();

		return outval;
	}

	void Swap(opDeque<T>& indeque)
	{
		d.swap(indeque.d);
	}

	void Insert(iterator position, const T& inval)
	{
		d.insert(position, inval);
	}

	void InsertAfter(iterator position, const T& inval)
	{
		if (position == d.end())
			d.push_back(inval);
		else
		{
			iterator pos = position;

			++pos;

			d.insert(pos, inval);
		}
	}

	template<class InputIterator>
	void Insert(iterator position, InputIterator first, InputIterator last)
	{
		d.insert(position, first, last);
	}

	iterator Erase(iterator position)
	{
		return d.erase(position);
	}

	iterator Erase(int index)
	{
		return d.erase( d.begin() + index );
	}

	iterator Erase(iterator first, iterator last)
	{
		return d.erase(first, last);
	}

	bool Erase(const T& t)
	{
		iterator i = Find( t );

		if ( i != d.end() )
		{
			d.erase( i );

			return true;
		}

		return false;
	}

	void Clear()
	{
		d.clear();
	}

	void Resize(size_type num)
	{
		d.resize(num);
	}

	void Add(size_type num = 1)
	{
		d.resize( d.size() + num );
	}

	void AddZeroed(size_type num = 1)
	{
		d.resize( d.size() + num );
		memset( &d[ d.size() - num ], 0, num * sizeof( T ) );
	}

	void Sort()
	{
		sort(d.begin(), d.end());
	}

	template<class BinaryPredicate>
	void Sort(BinaryPredicate& predicate)
	{
		sort(d.begin(), d.end(), predicate);
	}

	iterator Find(const T& inval)
	{
		return find(d.begin(), d.end(), inval);
	}

	const_iterator Find(const T& inval) const
	{
		return find(d.begin(), d.end(), inval);
	}

	reverse_iterator RFind(const T& inval)
	{
		return find(d.begin(), d.end(), inval);
	}

	const_reverse_iterator RFind(const T& inval) const
	{
		return find(d.begin(), d.end(), inval);
	}

	size_type FindIndex(const T& inval)
	{
		size_type size = d.size();

		for (size_type i = 0; i < size; i++)
		{
			if ( d[ i ] == inval )
				return i;
		}

		return -1;
	}

	bool Contains(const T& inval) const
	{
		return Find(inval) != d.end();
	}

	deque_type& GetDeque()
	{
		return d;
	}

	const deque_type& GetDeque() const
	{
		return d;
	}

	void DeleteAll()
	{
		iterator i   = d.begin();
		iterator end = d.end();

		while (i != end)
		{
			delete *i;
			++i;
		}
	}

	/**** operator overloads ****/

	T& operator [] (int i)
	{
		return d[i];
	}

	const T& operator [] (int i) const
	{
		return d[i];
	}

	T& operator [] (std::size_t i)
	{
		return d[i];
	}

	const T& operator [] (std::size_t i) const
	{
		return d[i];
	}

	friend bool operator == (const opDeque<T>& indeque1, const opDeque<T>& indeque2)
	{
		return indeque1.d == indeque2.d;
	}

	friend bool operator < (const opDeque<T>& indeque1, const opDeque<T>& indeque2)
	{
		return indeque1.d < indeque2.d;
	}

private:
	deque_type d;

public:
	/**** original container functions for stl compatibility ****/

	iterator begin()
	{
		return d.begin();
	}

	iterator end()
	{
		return d.end();
	}

	const_iterator begin() const
	{
		return d.begin();
	}

	const_iterator end() const
	{
		return d.end();
	}

	reverse_iterator rbegin()
	{
		return d.rbegin();
	}

	reverse_iterator rend()
	{
		return d.rend();
	}

	const_reverse_iterator rbegin() const
	{
		return d.rbegin();
	}

	const_reverse_iterator rend() const
	{
		return d.rend();
	}

	size_type size() const
	{
		return d.size();
	}

	size_type max_size() const
	{
		return d.max_size();
	}

	bool empty() const
	{
		return d.empty();
	}

	reference front()
	{
		return d.front();
	}

	reference back()
	{
		return d.back();
	}

	const_reference front() const
	{
		return d.front();
	}

	const_reference back() const
	{
		return d.back();
	}

	void push_front(const T& inval)
	{
		d.push_front(inval);
	}

	void push_back(const T& inval)
	{
		d.push_back(inval);
	}

	void pop_front()
	{
		d.pop_front();
	}

	void pop_back()
	{
		d.pop_back();
	}

	void swap(deque_type& indeque)
	{
		d.swap(indeque);
	}

	iterator insert(iterator position, const T& inval)
	{
		return d.insert(position, inval);
	}

	template<class InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last)
	{
		d.insert(position, first, last);
	}

	void insert(iterator position, size_type n, const T& inval)
	{
		d.insert(position, n, inval);
	}

	iterator erase(iterator position)
	{
		return d.erase(position);
	}

	iterator erase(iterator first, iterator last)
	{
		return d.erase(first, last);
	}

	void clear()
	{
		d.clear();
	}

	void resize(size_type num, T t = T())
	{
		d.resize(num, t);
	}
};

} // end namespace opstl

#endif // end __OPSTL_OPDEQUE__

