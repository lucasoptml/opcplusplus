///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: opLog.h
/// Date: 08/07/2007
///
/// Description:
///
/// opLog Header
///****************************************************************

//we only add includes to headers that
//can be independently included
#include <iostream>
#include "opSTL.h"
#include <File.h>

//errors?
namespace errors
{

	///==========================================
	/// opLog
	///==========================================

	class opLog 
	{
	public:
		/*=== construction ===*/

		opLog()  {}
		~opLog() {}

		/*=== utility ===*/

		static void SetStream(ostream& s)
		{
			o = &s;
		}

		static void Log(const opString& s)
		{
			opString log = s;

			if (log.Trim() == "" && lastLog == "");
			else 
			{
				(*o) << s.GetString() << endl << flush;
				DebugLog(s + "\n");

				lastLog = log;
			}
		}

		static void DebugLog(const opString& s);

	private:
		/*=== data ===*/

		static ostream* o;
		static opString lastLog;
	};

	// Method to log an error.
	inline void Log(const opString& error)
	{
		opLog::Log(error);
	}

}