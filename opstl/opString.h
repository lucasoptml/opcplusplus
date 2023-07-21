//****************************************************************
// Copyright © 2006 opGames Inc. - All Rights Reserved
//
// Authors: Kevin Depue & Lucas Ellis
//
// File: opString.h
// Date: 11/26/2006
//
// Description:
//
// Wrapper for STL string class.
//****************************************************************

#ifndef __OPSTL_OPSTRING__
#define __OPSTL_OPSTRING__

#include "opstlcommon.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

namespace opstl
{

/**** wrapper for stl string class ****/

class opString
{
public:

	typedef basic_string< char, char_traits< char >, OPSTL_STRING_ALLOCATOR(char) > string_type;

	/**** construction ****/

	opString()
	{

	}

	opString(const opString &_s)
	{
		s = _s.s;
	}

	template< class a >
	opString(const basic_string< char, char_traits<char>, a>& _s)
		: s(_s.begin(),_s.end())
	{
		
	}

	opString(const char* _s)
	{
		s = _s;
	}

	opString(const char c)
	{
		s = c;
	}

	opString(int i)
	{
		ToString(i, *this);
	}

	opString(unsigned int i)
	{
		ToString(i, *this);
	}

	opString(float f)
	{
		ToString(f, *this);
	}

	opString(double d)
	{
		ToString(d, *this);
	}

	opString(bool b)
	{
		s = b ? "true" : "false";
	}

	// Initialize a string to a line in a file.
	opString(istream& i, const char term = '\n')
	{
		Getline(i, term);
	}

	// Initialize a string given a wide string.
	opString(const wstring& _s)
	{
		s = string_type( _s.begin(), _s.end() );
	}

	opString(const vector<char>& _s)
	{
		s = string_type( _s.begin(), _s.end() );
	}

	opString(const opArray<char>& _s)
	{
		s = string_type( _s.begin(), _s.end() );
	}

	/**** destruction ****/

	~opString() {}

	/**** get ****/

	string_type& GetString()
	{
		return s;
	}

	const string_type& GetString() const
	{
		return s;
	}

	const string_type& get_string() const
	{
		return s;
	}

	string_type& get_string()
	{
		return s;
	}

	const char* GetCString() const
	{
		return s.c_str();
	}

	char GetCharacter(int i) const
	{
		return s[i];
	}

	int ToInt() const
	{
		return atoi( s.c_str() );
	}

	float ToFloat() const
	{
		return (float) atof( s.c_str() );
	}
 
	double ToDouble() const
	{
		return (double) atof( s.c_str() );
	}

	bool ToBool() const
	{
		return s == "true" ? true : false;
	}

	/**** set ****/

	opString& SetString(const opString &_s)
	{
		s = _s.s;

		return *this;
	}

	/**** utility ****/

	// Convert T to a string, set via reference (faster).
	template<class T>
	static void ToString(T t, opString& _s)
	{
		stringstream ss;
		ss << t;
		_s.s = ss.str();
	}

	// Convert T to a string, return result.
	template<class T>
	static opString ToString(T t)
	{
		stringstream ss;
		ss << t;
		return opString( ss.str() );
	}

	// Convert a string to it's correct type.
	// NOTE: This only works for int, float, double.
	template<class T>
	static T StringTo(const opString& _s)
	{
		return _s;
	}

	// StringTo specializations (int, float, double)
	template<> static int    StringTo<int>(const opString& _s);
	template<> static float  StringTo<float>(const opString& _s);
	template<> static double StringTo<double>(const opString& _s);

	// Get a fixed point string from a numeric input type.
	template<class T>
	static opString FixedString(const T& data)
	{
		std::stringstream ss;

		ss << std::fixed << data;

		opString s;
		ss >> s;

		return s;
	}

	// Get a rounded string from a numeric input type.
	// Note that the string is also fixed.
	template<class T>
	static opString RoundedString(const T& data, int num_digits)
	{
		std::stringstream ss;

		ss << std::fixed << std::setprecision(num_digits) << data;

		opString s;
		ss >> s;

		return s;
	}

	// size
	int Size() const
	{
		return (int) s.size();
	}

	// length - equivalent to size
	int Length() const
	{
		return (int) s.size();
	}

	// empty - returns true if s is ""
	bool IsEmpty() const
	{
		return s.size() == 0;
	}

	// clear - sets s to ""
	opString& Clear()
	{
		s.resize(0);

		return *this;
	}

	opString& ClearMemory()
	{
		s.clear();

		return *this;
	}

	void Reserve(size_t length)
	{
		s.reserve(length);
	}

	void Resize(size_t length)
	{
		s.resize(length);
	}

	// returns last char
	char Back() const
	{
		return s[(int) s.size() - 1];
	}

	// return first char
	char Front() const
	{
		return s[0];
	}

	// pop back - pops off the last character
	char PopBack()
	{
		int  end = (int) s.size() - 1;
		char c   = s[end];

		Remove(end);

		return c;
	}

	// pop front - pops off the first character
	char PopFront()
	{
		char c = s[0];

		Remove(0);

		return c;
	}

	// insert - inserts _s before i
	opString& Insert(int i, const opString &_s)
	{
		s.insert(i, _s.s);

		return *this;
	}

	// append
	opString& Append(const opString &_s)
	{
		s += _s.s;

		return *this;
	}

	// remove - removes a single character at position i
	opString& Remove(int i)
	{
		s.erase(s.begin() + i);

		return *this;
	}

	// remove - removes characters in range [start, end)
	opString& Remove(int start, int end)
	{
		string_type::iterator begin  = s.begin();
		string_type::iterator _start = begin + start;
		string_type::iterator _end   = begin + end;

		s.erase(_start, _end);

		return *this;
	}

	// remove - removes all occurances of the substring _ss
	opString& Remove(const opString &_ss)
	{
		int               start;
		int               length = (int) _ss.s.length();
		string::size_type pos    = s.find(_ss.s);

		while (pos != string::npos)
		{
			start = (int) pos;

			Remove(start, start + length);

			if (pos < s.size())
				pos = s.find(_ss.s, pos);
			else
				break;
		}

		return *this;
	}

	// remove - char version b/c compiler is dumb
	opString& Remove(const char c)
	{
		return Remove(opString(c));
	}

	opString& TrimLeft()
	{
		int size     = (int) s.size();
		int endindex = size;

		for (int i = 0; i < size; i++)
		{
			char c = s[i];
			
			if (c != ' ' && c != '\n' && c != '\t')
			{
				endindex = i;
				break;
			}
		}

		if (endindex > 0)
			Remove(0,endindex);

		return *this;
	}

	opString& TrimRight()
	{
		int size     = (int) s.size();
		int endindex = -1;

		for (int i = size - 1; i > -1; i--)
		{
			char c = s[i];

			if (c != ' ' && c != '\n' && c != '\t')
			{
				endindex = i;
				break;
			}
		}

		if (endindex + 1 < size)
			Remove(endindex+1,size);

		return *this;
	}

	// trim - trims whitespace at both ends of s
	opString& Trim()
	{
		TrimLeft();
		TrimRight();

		return *this;
	}

	// trims quotes
	opString& TrimQuotes()
	{
		if (IsEmpty())
			return *this;

		if (s[0] == '\"')
			Remove(0);

		if (IsEmpty())
			return *this;

		if (s[Length() - 1] == '\"')
			Remove(Length() - 1);

		return *this;
	}

	// removes all whitespace
	opString& RemoveWhitespace()
	{
		int    size = (int) s.size();
		string_type _s   = "";

		for (int i = 0; i < size; i++)
		{
			char c = s[i];

			if (c != ' ' && c != '\n' && c != '\t')
				_s += c;
		}

		s = _s;

		return *this;
	}

	// replace - replaces character at i with _ss
	opString& Replace(int i, const opString &_ss)
	{
		Remove(i);
		Insert(i, _ss);

		return *this;
	}

	// replace - replaces range [start, end) with _ss
	opString& Replace(int start, int end, const opString &_ss)
	{
		Remove(start, end);
		Insert(start, _ss);

		return *this;
	}

	// replace - replaces all occurances of _ss1 with _ss2
	opString& Replace(const opString &_ss1, const opString &_ss2)
	{
		string::size_type length1 = _ss1.s.length();
		string::size_type length2 = _ss2.s.length();
		string::size_type pos     = s.find(_ss1.s);

		while (pos != string::npos)
		{
			Replace((int) pos, (int) (pos + length1), _ss2);

			pos = s.find(_ss1.s, pos + length2);
		}

		return *this;
	}

	// replace - char version 1 b/c compiler is retarded
	opString& Replace(const char c, const opString &_ss2)
	{
		return Replace(opString(c), _ss2);
	}

	// replace - char version 1 b/c compiler is gay
	opString& Replace(const opString &_ss1, const char c)
	{
		return Replace(_ss1, opString(c));
	}

	// replace - char version 1 b/c compiler is a piece of sh*t
	opString& Replace(const char c1, const char c2)
	{
		return Replace(opString(c1), opString(c2));
	}

	// substring - returns the substring of s defined by [start, end)
	opString Substring(int start, int end) const
	{
		string_type _s(s, start, end - start);

		return opString(_s);
	}

	// left - returns the string to the left of i
	opString Left(int i) const
	{
		return Substring(0, i);
	}

	// left - returns the string left of the first
	// occurance of _s
	// note: searches forward!
	opString Left(const opString &_s) const
	{
		int index;

		if (Find(_s, index))
			return Left(index);

		return opString(s);
	}

	// left - char version b/c compiler is an idiot
	opString Left(const char c) const
	{
		return Left(opString(c));
	}

	// reverse left - returns the string left of the first
	// occurance of _s
	// note: searches backward!
	opString RLeft(const opString &_s) const
	{
		int index;

		if (RFind(_s, index))
			return Left(index);

		return opString(s);
	}

	opString RLeft(const char c) const
	{
		return RLeft(opString(c));
	}

	opString RLeft(int i) const
	{
		return Left(Length() - i);
	}

	// right - returns the string to the right of i
	opString Right(int i) const
	{
		int length = (int) s.size();

		return Substring(i + 1, length);
	}

	// right - returns the string right of the first
	// occurance of _s
	// note: searches forward!
	opString Right(const opString &_s) const
	{
		int index;
		int size = (int) _s.s.size();

		if (Find(_s, index))
			return Right(index + size - 1);

		return opString(s);
	}

	// right - char version b/c compiler should be shot
	opString Right(const char c) const
	{
		return Right(opString(c));
	}

	// reverse right - returns the string right of the first
	// occurance of _s
	// note: searches backward!
	opString RRight(const opString &_s) const
	{
		int index;
		int size = (int) _s.s.size();

		if (RFind(_s, index))
			return Right(index + size - 1);

		return opString(s);
	}

	opString RRight(char c) const
	{
		return RRight(opString(c));
	}

	opString RRight(int i) const
	{
		return Right(Length() - i - 1);
	}

	bool StartsWith(const opString& other) const
	{
		int olength = other.Length();

		if (Length() < olength)
			return false;
		
		return equal(other.s.begin(),other.s.end(),s.begin());
	}

	bool EndsWith(const opString& other) const
	{
		int olength = other.Length();

		if (Length() < olength)
			return false;
		
		return equal(other.s.begin(),other.s.end(),s.begin()+Length()-olength);
	}

	// find - returns true if found and sets result
	// to index via reference or false and sets result
	// to -1 via reference.  start is where the search
	// starts, and defaults to 0
	// note: searches forward!
	bool Find(const opString &_ss, int &result, int start = 0) const
	{
		string::size_type _start = start;
		string::size_type pos    = s.find(_ss.s, _start);

		if (pos == string::npos)
		{
			result = -1;
			return false;
		}

		result = (int) pos;

		return true;
	}

	// find w/o result requirement
	bool Find(const opString &_ss) const
	{
		int  r;
		bool result = Find(_ss, r, 0);

		return result;
	}

	// reverse find - returns true if found and sets result
	// to index via reference or false and sets result
	// to -1 via reference.  start is where the search
	// starts, and defaults to the end of the container
	// note: searches backward!
	bool RFind(const opString &_ss, int &result, int start = (int) string::npos) const
	{
		string::size_type _start = start;
		string::size_type pos    = s.rfind(_ss.s, _start);

		if (pos == string::npos)
		{
			result = -1;
			return false;
		}

		result = (int) pos;

		return true;
	}

	// rfind w/o result requirement
	bool RFind(const opString &_ss) const
	{
		int  r;
		bool result = RFind(_ss, r, (int) string::npos);

		return result;
	}

	// tokenize - sets a vector of strings via reference,
	// splitting on the specified substring _s
	template<class T>
	void Tokenize(const opString &_s, T &container) const
	{
		int size1 = (int) s.size();
		int size2 = (int) _s.s.size();
		int start = 0;
		int i;

		while (Find(_s, i, start))
		{
			if (start != i)
			{
				opString _s2 = Substring(start, i);
				container.push_back(_s2);
			}

			start = i + size2;
		}

		if (start != -1 && start != size1)
		{
			opString _s2 = Substring(start, size1);
			container.push_back(_s2);
		}
	}

	// tokenize on all delimiters in delimiter string
	template<class T>
	void TokenizeDelimiters(const opString& delimiters, T& c) const
	{
		string::size_type lastPos(s.find_first_not_of(delimiters.s, 0));
		string::size_type pos(s.find_first_of(delimiters.s, lastPos));
		while (string::npos != pos || string::npos != lastPos)
		{
			std::back_insert_iterator< T > it( c );
			*it = s.substr(lastPos, pos - lastPos);
			lastPos = s.find_first_not_of(delimiters.s, pos);
			pos = s.find_first_of(delimiters.s, lastPos);
		}
	}

	// tokenize on whitespace - first trims the string, then
	template<class T>
	void TokenizeOnWhitespace(T &container) const
	{
		opString base  = s;
		base.Trim();//is this a good idea?
		int      size  = base.Size();
		int      start = 0;
		opString token;

		// if we have no tokens...return
		if (base == "")
			return;

		for (int i = 0; i < size; i++)
		{
			char c = base[i];

			if (c == ' ' || c == '\n' || c == '\t')
			{
				token = base.Substring(start, i);
				container.push_back( opString::StringTo< typename T::value_type >(token) );

				for (int j = i + 1; j < size; j++)
				{
					char c2 = base[j];

					if (c2 != ' ' && c2 != '\n' && c2 != '\t')
					{
						start = i = j;
						break;
					}
				}
			}
		}

		token = base.Substring(start, size);
		container.push_back( opString::StringTo< typename T::value_type >(token) );
	}

	// returns a unique hash value for the current string
	// based on the djb2 algorithm
	int Hash() const
	{
		int hash = 5381;

		if (!s.length())
			return hash;

		const char* str = &s[0];

		int c;

		while (c = *str++)
			hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

		return hash;
	}

	// static version of Hash
	static int Hash(const opString& s) 
	{
		return s.Hash();
	}

	// getline - given an istream&, reads the current line into
	// s until reaching terminating character term (which defaults
	// to the null character)
	istream& Getline(istream& i, const char term = '\n')
	{
		return getline(i, s, term);
	}

	// make valid path - checks if the string has a '/' at the end
	// if not, adds one
	opString& MakeValidPath()
	{
		int size = (int) s.size();

		if (size == 0)
			s += '/';
		else if (s[size - 1] != '/')
			s += '/';

		return *this;
	}

	// converts this string to uppercase
	opString& ToUpper()
	{
		transform(s.begin(), s.end(), s.begin(), (int(*)(int)) toupper);

		return *this;
	}

	// converts this string to lowercase
	opString& ToLower()
	{
		transform(s.begin(), s.end(), s.begin(), (int(*)(int)) tolower);

		return *this;
	}

	// repetition
	static opString FilledString(char c, int rep)
	{
		opString newString = "";

		newString.Reserve(rep);

		for (int i = 0; i < rep; i++)
			newString += c;

		return newString;
	}

	// Returns true if the character is alphabetic.
	static bool IsAlpha(char c) 
	{
		return (c >= 'a' && c <= 'z')
			|| (c >= 'A' && c <= 'Z');
	}

	// Returns true if the character is numeric.
	static bool IsNumeric(char c) 
	{
		return c >= '0' && c <= '9';
	}

	// Returns true if the character is alphanumeric.
	static bool IsAlphaNumeric(char c) 
	{
		return IsAlpha(c) || IsNumeric(c);
	}

	// true if this string is alphabetic
	bool IsAlpha() const
	{
		int size = Size();

		for (int i = 0; i < size; i++)
		{
			if ( !IsAlpha(s[i]) )
				return false;
		}

		return true;
	}

	// true if this string is numeric
	bool IsNumeric() const
	{
		int size = Size();

		for (int i = 0; i < size; i++)
		{
			if ( !IsNumeric(s[i]) )
				return false;
		}

		return true;
	}

	// true if the string is alphanumeric
	bool IsAlphaNumeric() const
	{
		int size = Size();

		for (int i = 0; i < size; i++)
		{
			if ( !IsAlphaNumeric(s[i]) )
				return false;
		}

		return true;
	}

	// returns a unicode string.
	wstring ToUnicode() const
	{
		return std::wstring( s.begin(), s.end() );
	}

	// Load a file into a string.
	void LoadFile(std::ifstream& ifs)
	{
		ifs.unsetf( ios::skipws );

		copy( istream_iterator<char>( ifs ),
			  istream_iterator<char>(),
			  back_inserter( s ) );
	}

	// Returns the opString version of the integer
	// in hexadecimal format.
	template<class type>
	static opString AsHex(type i)
	{
		stringstream ss;

		ss << std::hex << i; 

		opString _s = "0x";

		_s += ss.str();

		return _s;
	}

	// removes lamo microsoft \r crap
	void FixNewlines()
	{
		string_type _s   = s;
		size_t size = _s.size();
		
		s.resize(0);

		for (size_t i = 0; i < size; i++)
		{
			if (_s[i] != '\r')
				s += _s[i];
		}
	}

	/**** operator overloads ****/

	// weird overloads for functions that take const string&,
	// string&, and const char*
	operator const string_type& () const
	{
		return s;
	}

	operator string_type& ()
	{
		return s;
	}

	operator const char* () const
	{
		return s.c_str();
	}

	// indexing
	char& operator[] (int i)
	{
		return s[i];
	}

	const char& operator[] (int i) const
	{
		return s[i];
	}

	const char& operator[] (size_t i) const
	{
		return s[i];
	}

	char& operator[] (size_t i)
	{
		return s[i];
	}

	// assignment
	opString& operator= (const opString &_s)
	{
		s = _s.s;

		return *this;
	}

	template< class a >
	opString& operator= (const basic_string< char, char_traits<char>, a>& _s)
	{
		s.assign(_s.begin(),_s.end());

		return *this;
	}

// 	opString& operator= (const string &_s)
// 	{
// 		s = _s;
// 
// 		return *this;
// 	}

	opString& operator= (const char* _s)
	{
		s = _s;

		return *this;
	}

	opString& operator= (const char c)
	{
		s = c;

		return *this;
	}

	opString& operator= (int i)
	{
		ToString(i, *this);

		return *this;
	}

	opString& operator= (unsigned int i)
	{
		ToString(i, *this);

		return *this;
	}

	opString& operator= (float f)
	{
		ToString(f, *this);

		return *this;
	}

	opString& operator= (double d)
	{
		ToString(d, *this);

		return *this;
	}

	// equality ==
	friend bool operator== (const opString &lhs, const opString &rhs)
	{
		return lhs.s == rhs.s;
	}

	friend bool operator== (const opString &lhs, const string &rhs)
	{
		return lhs.s == rhs;
	}

	friend bool operator== (const string &lhs, const opString &rhs)
	{
		return lhs == rhs.s;
	}

	friend bool operator== (const opString &lhs, const char* rhs)
	{
		return lhs.s == rhs;
	}

	friend bool operator== (const char* lhs, const opString &rhs)
	{
		return lhs == rhs.s;
	}

	friend bool operator== (const opString &lhs, char rhs)
	{
		string _s = "";
		_s += rhs;
		return lhs.s == _s;
	}

	friend bool operator== (char lhs, const opString &rhs)
	{
		string _s = "";
		_s += lhs;
		return _s == rhs.s;
	}

	// equality !=
	friend bool operator!= (const opString &lhs, const opString &rhs)
	{
		return !(lhs.s == rhs.s);
	}

	friend bool operator!= (const opString &lhs, const string &rhs)
	{
		return !(lhs.s == rhs);
	}

	friend bool operator!= (const string &lhs, const opString &rhs)
	{
		return !(lhs == rhs.s);
	}

	friend bool operator!= (const opString &lhs, const char* rhs)
	{
		return !(lhs.s == rhs);
	}

	friend bool operator!= (const char* lhs, const opString &rhs)
	{
		return !(lhs == rhs.s);
	}

	friend bool operator!= (const opString &lhs, char rhs)
	{
		string _s = "";
		_s += rhs;
		return !(lhs.s == _s);
	}

	friend bool operator!= (char lhs, const opString &rhs)
	{
		string _s = "";
		_s += lhs;
		return !(_s == rhs.s);
	}

	// plus equals
	opString& operator+= (const opString &_s)
	{
		s += _s.s;

		return *this;
	}

	opString& operator+= (const string &_s)
	{
		s += _s;

		return *this;
	}

	opString& operator+= (const char* _s)
	{
		s += _s;

		return *this;
	}

	opString& operator+= (const char c)
	{
		s += c;

		return *this;
	}

	opString& operator+= (int i)
	{
		opString _s;
		ToString(i, _s);
		s += _s.s;

		return *this;
	}

	opString& operator+= (unsigned int i)
	{
		opString _s;
		ToString(i, _s);
		s += _s.s;

		return *this;
	}

	opString& operator+= (float f)
	{
		opString _s;
		ToString(f, _s);
		s += _s.s;

		return *this;
	}

	opString& operator+= (double d)
	{
		opString _s;
		ToString(d, _s);
		s += _s.s;

		return *this;
	}

	// plus
	opString operator+ (const opString &rhs) const
	{
		opString _s = s;
		_s.s += rhs.s;
		
		return _s;
	}

	opString operator+ (const string &rhs) const
	{
		opString _s = s;
		_s.s += rhs;

		return _s;
	}

	opString operator+ (const char* rhs) const
	{
		opString _s = s;
		_s.s += rhs;

		return _s;
	}

	opString operator+ (const char rhs) const
	{
		string __s;
		__s = rhs;
		opString _s = s;
		_s.s += __s;

		return _s;
	}

	opString operator+ (int rhs) const
	{
		opString __s;
		ToString(rhs, __s);
		opString _s = s;
		_s.s += __s.s;

		return _s;
	}

	opString operator+ (unsigned int rhs) const
	{
		opString __s;
		ToString(rhs, __s);
		opString _s = s;
		_s.s += __s.s;

		return _s;
	}

	opString operator+ (float rhs) const
	{
		opString __s;
		ToString(rhs, __s);
		opString _s = s;
		_s.s += __s.s;

		return _s;
	}

	opString operator+ (double rhs) const
	{
		opString __s;
		ToString(rhs, __s);
		opString _s = s;
		_s.s += __s.s;

		return _s;
	}

	// I/O
	friend std::istream& operator>> (std::istream &i, opString &_s)
	{
		return i >> _s.s;
	}

	friend std::ostream& operator<< (std::ostream &o, const opString &_s)
	{
		return o << _s.s;
	}

	// <
	bool operator< (const opString &rhs) const
	{
		return s < rhs.s;
	}

	bool operator< (const string &rhs) const
	{
		return s < rhs;
	}

	bool operator< (const char* rhs) const
	{
		return s < rhs;
	}

	bool operator< (const char rhs) const
	{
		string _s;
		_s = rhs;

		return s < _s;
	}

	// >
	bool operator> (const opString &rhs) const
	{
		return s > rhs.s;
	}

	bool operator> (const string &rhs) const
	{
		return s > rhs;
	}

	bool operator> (const char* rhs) const
	{
		return s > rhs;
	}

	bool operator> (const char rhs) const
	{
		string _s;
		_s = rhs;

		return s > _s;
	}

private:
	string_type s;
};

/*=== specializations ===*/

// Specialize for ints.
template<>
int opString::StringTo<int>(const opString& _s)
{
	return _s.ToInt();
}

// Specialize for floats.
template<>
float opString::StringTo<float>(const opString& _s)
{
	return _s.ToFloat();
}

// Specialize for doubles.
template<>
double opString::StringTo<double>(const opString& _s)
{
	return _s.ToDouble();
}

/**** global operators ****/

inline opString operator + (const string& left_string, const opString& right_string)
{
	return opString(left_string) + right_string;
}

inline opString operator + (const char* left_string, const opString& right_string)
{
	return opString(left_string) + right_string;
}

inline opString operator + (char c, const opString& right_string)
{
	return opString(c) + right_string;
}

inline opString operator + (int i, const opString& right_string)
{
	return opString(i) + right_string;
}

inline opString operator + (unsigned int i, const opString& right_string)
{
	return opString(i) + right_string;
}

inline opString operator + (float f, const opString& right_string)
{
	return opString(f) + right_string;
}

inline opString operator + (double d, const opString& right_string)
{
	return opString(d) + right_string;
}

/**** opStringUtility static function definitions ****/

#ifdef __OPSTL_OPHASH__
	
	/**** opHasher related code ****/
	
	// specialize opHasher for opString as a key
	template<>
	struct opHashFunction<opString>
	{
		static size_t Hash(const opString& t)
		{
			return (size_t) t.Hash();
		}
	};
	
	#ifdef OPSTL_GNU_LINUX
	template<>
	struct opHashCompare<opString>
	{
		static bool Compare(const opString& t1, const opString& t2) 
		{
			return t1 == t2;
		}
	};
	#endif
	
#endif

} // end namespace opstl

#endif // end __OPSTL_OPSTRING__

