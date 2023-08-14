//****************************************************************
// Copyright © 2008 opGames Inc. - All Rights Reserved
//
// Authors: Kevin Depue & Lucas Ellis
//
// File: opList.h
// Date: 02/23/2007
//
// Description:
//
// opList - wrapper for STL <list> class
//****************************************************************

#ifndef __OPSTL_OPLIST__
#define __OPSTL_OPLIST__

#include "opstlcommon.h"
#include <list>

namespace opstl
{

//==========================================
// opList
//==========================================

// stl list wrapper
template<class T>
class opList 
{
public:
	/**** typedefs ****/

	typedef          list<T, OPSTL_LIST_ALLOCATOR(T) >                         list_type;
	typedef typename list_type::size_type                                      size_type;
	typedef typename list_type::iterator									   iterator;
	typedef typename list_type::const_iterator							       const_iterator;
	typedef typename list_type::reverse_iterator							   reverse_iterator;
	typedef typename list_type::const_reverse_iterator					       const_reverse_iterator;
	typedef typename list_type::reference									   reference;
	typedef typename list_type::const_reference							       const_reference;
	typedef typename list_type::value_type									   value_type;
	typedef          pointer_iterator<iterator, T>							   pointer_iterator;
	typedef          const_pointer_iterator<const_iterator, T>				   const_pointer_iterator;
	typedef          reverse_pointer_iterator<reverse_iterator, T>             reverse_pointer_iterator;
	typedef          const_reverse_pointer_iterator<const_reverse_iterator, T> const_reverse_pointer_iterator;

	/**** construction ****/

	opList() {}

	opList(const list_type& inlist)
		: cont(inlist) {}

	opList(const opList<T>& inlist)
		: cont(inlist.cont) {}

	template<class InputIterator>
	opList(InputIterator first, InputIterator last) 
		: cont(first, last) {}

	/**** destruction ****/

	~opList()
	{
		cont.clear();
	}

	/**** utility ****/

	iterator Begin()
	{
		return cont.begin();
	}

	iterator Last()
	{
		if (IsEmpty())
			return End();

		return --End();
	}

	iterator End()
	{
		return cont.end();
	}

	const_iterator Begin() const
	{
		return cont.begin();
	}

	const_iterator Last() const
	{
		if (IsEmpty())
			return End();

		return --End();
	}

	const_iterator End() const
	{
		return cont.end();
	}

	reverse_iterator RBegin()
	{
		return cont.rbegin();
	}

	reverse_iterator RLast() 
	{
		if (IsEmpty())
			return REnd();

		return --REnd();
	}

	reverse_iterator REnd()
	{
		return cont.rend();
	}

	const_reverse_iterator RBegin() const
	{
		return cont.rbegin();
	}

	const_reverse_iterator RLast() const
	{
		if (IsEmpty())
			return REnd();

		return --REnd();
	}

	const_reverse_iterator REnd() const
	{
		return cont.rend();
	}

	int Size() const
	{
		return (int) cont.size();
	}

	int Length() const
	{
		return (int) cont.size();
	}

	int MaxSize() const
	{
		return (int) cont.max_size();
	}

	bool IsEmpty() const
	{
		return cont.empty();
	}

	T& Front()
	{
		return cont.front();
	}

	T& Back()
	{
		return cont.back();
	}

	const T& Front() const
	{
		return cont.front();
	}

	const T& Back() const
	{
		return cont.back();
	}

	void PushFront(const T& inval)
	{
		cont.push_front(inval);
	}

	void PushBack(const T& inval)
	{
		cont.push_back(inval);
	}

	T PopFront()
	{
		T outval = cont.front();

		cont.pop_front();

		return outval;
	}

	T PopBack()
	{
		T outval = cont.back();

		cont.pop_back();

		return outval;
	}

	void Swap(opList<T>& inlist)
	{
		cont.swap(inlist.cont);
	}
	
	void Insert(iterator position, const T& inval)
	{
		cont.insert(position, inval);
	}

	template<class InputIterator>
	void Insert(iterator position, InputIterator first, InputIterator last)
	{
		cont.insert(position, first, last);
	}

	void InsertAfter(iterator position, const T& inval)
	{
		if (position == cont.end())
			cont.push_back(inval);
		else
		{
			iterator pos = position;

			++pos;

			cont.insert(pos, inval);
		}
	}

	iterator Erase(iterator position)
	{
		return cont.erase(position);
	}

	iterator Erase(iterator first, iterator last)
	{
		return cont.erase(first, last);
	}

	bool Erase(const T& t)
	{
		iterator i = Find( t );

		if ( i != cont.end() )
		{
			cont.erase( i );

			return true;
		}

		return false;
	}

	void Clear()
	{
		cont.clear();
	}

	void Resize(int num)
	{
		cont.resize(num);
	}

	void Splice(iterator position, opList<T>& inlist)
	{
		cont.splice(position, inlist.cont);
	}

	void Splice(iterator position, opList<T>& inlist, iterator inval)
	{
		cont.splice(position, inlist.cont, inval);
	}

	void Splice(iterator position, opList<T>& inlist, iterator first, iterator last)
	{
		cont.splice(position, inlist.cont, first, last);
	}

	void Remove(const T& inval)
	{
		cont.remove(inval);
	}

	template<class Predicate>
	void RemoveIf(Predicate pred)
	{
		cont.remove_if(pred);
	}

	void Unique()
	{
		cont.unique();
	}

	template<class BinaryPredicate>
	void Unique(BinaryPredicate compare)
	{
		cont.unique(compare);
	}

	void Merge(opList<T>& inlist)
	{
		cont.merge(inlist.cont);
	}

	template<class BinaryPredicate>
	void Merge(opList<T>& inlist, BinaryPredicate compare)
	{
		cont.merge(inlist.cont, compare);
	}

	void Reverse()
	{
		cont.reverse();
	}

	void Sort()
	{
		cont.sort();
	}

	template<class BinaryPredicate>
	void Sort(BinaryPredicate& predicate)
	{
		cont.sort(predicate);
	}

	iterator Find(const T& inval)
	{
		return find(cont.begin(), cont.end(), inval);
	}

	const_iterator Find(const T& inval) const
	{
		return find(cont.begin(), cont.end(), inval);
	}

	reverse_iterator RFind(const T& inval)
	{
		return find(cont.begin(), cont.end(), inval);
	}

	const_reverse_iterator RFind(const T& inval) const
	{
		return find(cont.begin(), cont.end(), inval);
	}

	bool Contains(const T& inval) const
	{
		return Find(inval) != cont.end();
	}

	list_type& GetList()
	{
		return cont;
	}

	const list_type& GetList() const
	{
		return cont;
	}

	void DeleteAll()
	{
		iterator i   = cont.begin();
		iterator end = cont.end();

		while (i != end)
		{
			delete *i;
			++i;
		}
	}

	//call this instead of size on linux!
	bool HasSize(int num) const
	{
#if defined(OPSTL_GNU_LINUX) || defined(OPSTL_INTEL_LINUX)
		const_iterator it = begin();
		int i = 0;
		while(i < num)
		{
			if(it == end())
				return false;
			++i;
			++it;
		}

		return true;
#else
		return Size() >= num;
#endif
	}

	/**** operator overloads ****/

	friend bool operator == (const opList<T>& inlist1, const opList<T>& inlist2)
	{
		return inlist1.cont == inlist2.cont;
	}

	friend bool operator < (const opList<T>& inlist1, const opList<T>& inlist2)
	{
		return inlist1.cont < inlist2.cont;
	}

private:
	/**** data ****/

	list_type cont;

public:
	/**** original container functions for stl compatibility ****/

	iterator begin()
	{
		return cont.begin();
	}

	iterator end()
	{
		return cont.end();
	}

	const_iterator begin() const
	{
		return cont.begin();
	}

	const_iterator end() const
	{
		return cont.end();
	}

	reverse_iterator rbegin()
	{
		return cont.rbegin();
	}

	reverse_iterator rend()
	{
		return cont.rend();
	}

	const_reverse_iterator rbegin() const
	{
		return cont.rbegin();
	}

	const_reverse_iterator rend() const
	{
		return cont.rend();
	}

	size_type size() const
	{
		return cont.size();
	}

	size_type max_size() const
	{
		return cont.max_size();
	}

	bool empty() const
	{
		return cont.empty();
	}

	reference front()
	{
		return cont.front();
	}

	reference back()
	{
		return cont.back();
	}

	const_reference front() const
	{
		return cont.front();
	}

	const_reference back() const
	{
		return cont.back();
	}

	void push_front(const T& inval)
	{
		cont.push_front(inval);
	}

	void push_back(const T& inval)
	{
		cont.push_back(inval);
	}

	void pop_front()
	{
		cont.pop_front();
	}

	void pop_back()
	{
		cont.pop_back();
	}

	void swap(list_type& inlist)
	{
		cont.swap(inlist);
	}

	iterator insert(iterator position, const T& inval)
	{
		return cont.insert(position, inval);
	}

	template<class InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last)
	{
		cont.insert(position, first, last);
	}

	void insert(iterator position, size_type n, const T& inval)
	{
		cont.insert(position, n, inval);
	}

	iterator erase(iterator position)
	{
		return cont.erase(position);
	}

	iterator erase(iterator first, iterator last)
	{
		return cont.erase(first, last);
	}

	void clear()
	{
		cont.clear();
	}

	void resize(size_type num, T t = T())
	{
		cont.resize(num, t);
	}

	void splice(iterator position, list_type& inlist)
	{
		cont.splice(position, inlist);
	}

	void splice(iterator position, list_type& inlist, iterator inval)
	{
		cont.splice(position, inlist, inval);
	}

	void splice(iterator position, list_type& inlist, iterator first, iterator last)
	{
		cont.splice(position, inlist, first, last);
	}

	void remove(const T& inval)
	{
		cont.remove(inval);
	}

	template<class Predicate>
	void remove_if(Predicate pred)
	{
		cont.remove_if(pred);
	}

	void unique()
	{
		cont.unique();
	}

	template<class BinaryPredicate>
	void unique(BinaryPredicate compare)
	{
		cont.unique(compare);
	}

	void merge(list_type& inlist)
	{
		cont.merge(inlist);
	}

	template<class BinaryPredicate>
	void merge(list_type& inlist, BinaryPredicate compare)
	{
		cont.merge(inlist, compare);
	}

	void reverse()
	{
		cont.reverse();
	}

	void sort()
	{
		cont.sort();
	}

	template<class BinaryPredicate>
	void sort(BinaryPredicate compare)
	{
		cont.sort(compare);
	}
};

} // end namespace opstl

#endif // end __OPSTL_OPLIST__

