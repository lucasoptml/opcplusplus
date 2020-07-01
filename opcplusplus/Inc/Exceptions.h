///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Exceptions.h
/// Date: 01/11/2007
///
/// Description:
///
/// Exception-related code.
///****************************************************************

namespace exceptions
{

// base exception class
class opException
{
public:
	/**** construction / destruction ****/

	opException() {}
	~opException() {}


	/**** exception types ****/

	// macros/exceptions for nodes, etc.
	// stops looking for a certain grammar because it errored
	struct opCPP_Exception
	{
	private:
		opCPP_Exception()
		{
			opException::Throw();
		}

		friend opException;
	};

	//throws out to end the compiling
	struct opCPP_FatalException
	{
	private:
		opCPP_FatalException()
		{
			opException::Throw();
		}

		friend opException;
	};

	//throws out to Parse()
	struct opCPP_ParseException
	{
	private:
		opCPP_ParseException()
		{
			opException::Throw();
		}

		friend opException;
	};

	//throws
	struct opCPP_AssertException
	{
	private:
		opCPP_AssertException(const string& estring) : exceptionstring(estring)
		{
			opException::Throw();
		}
	public:
		string exceptionstring;

		friend opException;
	};

	/**** utility ****/

	static void Throw()
	{
		ExceptionCount++;
		
		//NOTE: question is, are there actual scenarios where this is outside [0,1]?
		//		if not we should assert(ExceptionCount <= 1)
		assert(ExceptionCount <= 1);//???
	}

	static bool InException()
	{
		return ExceptionCount > 0;
	}

	// called immediately AFTER the catch block
	static int CaughtException(opCPP_Exception& e)
	{
		return CaughtException();
	}

	static int CaughtException(opCPP_ParseException& e)
	{
		return CaughtException();
	}
	
	static int CaughtException()
	{
		return ExceptionCount = 0;
	}

	static inline int GetExceptionCount()
	{
		return ExceptionCount;
	}



	/**** exception throwing functions ****/

	static void ThrowException()
	{
		DispatchException<opCPP_Exception>();
	}

	static void ThrowFatalException()
	{
		DispatchException<opCPP_FatalException>();
	}

	static void ThrowParseException()
	{
		DispatchException<opCPP_ParseException>();
	}

	static void ThrowAssertException(const opString& assertstring, const char* file, int line)
	{
		//no dispatch
		//TODO: add file and line
		throw opCPP_AssertException(assertstring);
	}

	enum ExceptionType
	{
		DefaultException,
		Exception,
		ParseException,
		AssertException,
		FatalException,
	};

private:
	static int ExceptionCount;

	//exception override
	static ExceptionType ExceptionOverride;
	
	//handles override dispatching
	template<class defaulttype>
	static void DispatchException()
	{
		if(ExceptionOverride == DefaultException)
			throw defaulttype();
		else if(ExceptionOverride == Exception)
			throw opCPP_Exception();
		else if(ExceptionOverride == ParseException)
			throw opCPP_ParseException();
		else if(ExceptionOverride == FatalException)
			throw opCPP_FatalException();
	}

	friend setexceptionmode;
};

//NOTE: this is necessary for resetting the exception counter
//		only when you return within a catch block
//		(otherwise call opException::CaughtException())
class ExceptionCatchHandler
{
public:
	ExceptionCatchHandler() : bReset(false)
	{
	}
	
	void CaughtException()
	{
		bReset = true;
	}

	~ExceptionCatchHandler()
	{
		//NOTE: two scenarios...
		//		1. caught an exception, should reset upon return
		//		2. didn't catch it here! falls through w/o resetting

		if(bReset)
			opException::CaughtException();
	}

private:
	bool bReset;
};

class setexceptionmode
{
public:
	setexceptionmode(opException::ExceptionType exceptiontype)
	{
		oldoverride = opException::ExceptionOverride;
		opException::ExceptionOverride = exceptiontype;
	}

	~setexceptionmode()
	{
		opException::ExceptionOverride = oldoverride;
	}

private:
	opException::ExceptionType oldoverride;
};

} // end namespace exceptions

