//****************************************************************
// Copyright © 2008 opGames Inc. - All Rights Reserved
//
// Authors: Kevin Depue & Lucas Ellis
//
// File: Interfaces.h
// Date: 04/02/2007
//
// Description:
//
// opSTL Interfaces
//****************************************************************

#ifndef __OPSTL_OPINTERFACES__
#define __OPSTL_OPINTERFACES__

namespace opstl
{

//==========================================
// pointer_iterator
//==========================================

template<class Iterator, class T>
class pointer_iterator : public Iterator
{
public:
	/**** construction ****/

	pointer_iterator()
	{
	}

	pointer_iterator(Iterator iter) 
		: Iterator(iter)
	{	
	}

	/**** operator overloads ****/

	T operator -> () const
	{
		return *(*this);
	}
};

//==========================================
// const_pointer_iterator
//==========================================

template<class Iterator, class T>
class const_pointer_iterator : public Iterator
{
public:
	/**** construction ****/

	const_pointer_iterator()
	{
	}

	const_pointer_iterator(Iterator iter) 
		: Iterator(iter)
	{	
	}

	/**** operator overloads ****/

	const T operator -> () const
	{
		return *(*this);
	}
};

//==========================================
// reverse_pointer_iterator
//==========================================

template<class Iterator, class T>
class reverse_pointer_iterator : public Iterator
{
public:
	/**** construction ****/

	reverse_pointer_iterator()
	{
	}

	reverse_pointer_iterator(Iterator iter) 
		: Iterator(iter)
	{	
	}

	/**** operator overloads ****/

	T operator -> () const
	{
		return *(*this);
	}
};

//==========================================
// const_reverse_pointer_iterator
//==========================================

template<class Iterator, class T>
class const_reverse_pointer_iterator : public Iterator
{
public:
	/**** construction ****/

	const_reverse_pointer_iterator()
	{
	}

	const_reverse_pointer_iterator(Iterator iter) 
		: Iterator(iter)
	{	
	}

	/**** operator overloads ****/

	const T operator -> () const
	{
		return *(*this);
	}
};

} // end namespace opstl

#endif // end __OPSTL_OPINTERFACES__

